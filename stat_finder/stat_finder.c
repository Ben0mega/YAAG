/*
 *	Stops a process to get stats for it - adapted from stopper.c ,
 * retrieved from http://www.wienand.org/junkcode/linux/stopper.c
 * which has the following notice:
 * very simple ptrace example to stop a process before it exits.  This
 * allows us to inspect /proc/pid/ values, for example.
 * (C) 2007 Ian Wienand <ianw@ieee.org>
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <sys/ptrace.h>

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <unistd.h>

#include "perf_stats.h"
#include "proc.h"
#include "util.h"

static void wait_for_exit(void) __attribute__((noreturn));
static void write_stats(struct rusage* ru, long return_code);

static pid_t pid;
static struct timeval start;
static char outfile[FILENAME_MAX] = "stat.out";

static const char* usage = "[-f output_file] command";

int main(int argc, char* argv[]) {
	extern char* optarg;
	extern int optind, opterr, optopt;
	char c;

	while ((c = getopt(argc, argv, "+f:")) != -1) {
		switch (c) {

			case 'f':
				strncpy((char*)outfile, optarg, sizeof(outfile) / sizeof(char));
				break;
			default:
				printf("Usage: %s %s\n", argv[0], usage);
				exit(1);
		}
	}

	if (optind == argc) {
		printf("Usage: %s %s\n", argv[0], usage);
		exit(1);
	}

	/* find the file to exec, borrowed from strace */
	struct stat statbuf;
	char* filename;
	char pathname[MAXPATHLEN];

	filename = argv[optind];
	if (strchr(filename, '/')) {
		if (strlen(filename) > sizeof(pathname) - 1) {
			errno = ENAMETOOLONG;
			perror("stopper: exec failed");
			exit(1);
		}
		strcpy(pathname, filename);
	} else {
		char* path;
		int m, n, len;

		for (path = getenv("PATH"); path && *path; path += m) {
			if (strchr(path, ':')) {
				n = strchr(path, ':') - path;
				m = n + 1;
			} else {
				m = n = strlen(path);
			}
			if (n == 0) {
				getcwd(pathname, MAXPATHLEN);
				len = strlen(pathname);
			} else if (n > sizeof pathname - 1) {
				continue;
			} else {
				strncpy(pathname, path, n);
				len = n;
			}
			if (len && pathname[len - 1] != '/') {
				pathname[len++] = '/';
			}
			strcpy(pathname + len, filename);
			if (stat(pathname, &statbuf) == 0 &&
					/* Accept only regular files
					 with some execute bits set.
					 XXX not perfect, might still fail */
					S_ISREG(statbuf.st_mode) && (statbuf.st_mode & 0111)) {
				break;
			}
		}
	}

	if (stat(pathname, &statbuf) < 0) {
		fprintf(stderr, "%s: %s: command not found\n", argv[0], filename);
		exit(1);
	}

	gettimeofday(&start, NULL);
	switch (pid = fork()) {
		case (pid_t) - 1:
			perror("stopper: fork");
			exit(1);
			break;
		case 0:
			if (ptrace(PTRACE_TRACEME, 0, (char*)1, 0) < 0) {
				perror("stopper: ptrace(PTRACE_TRACEME, ...)");
				return -1;
			}
			execv(pathname, &argv[optind]);
			perror("stopper: execv");
			_exit(1);

		default:
			/* wait for first signal, which is from child when it execs */
			wait(NULL);

			/* set option to tell us when the signal exits */
			if (ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACEEXIT)) {
				perror("stopper: ptrace(PTRACE_SETOPTIONS, ...)");
				return -1;
			}

			/* perf all the things! */
			init_perf_events(pid);

			/* allow child to continue */
			if (ptrace(PTRACE_CONT, pid, 0, (void*)0)) {
				perror("stopper: ptrace(PTRACE_CONT, ...)");
				return -1;
			}

			wait_for_exit();
	}

	/* shouldn't get here */
	exit(1);
}

/* wait for the pid to exit */
void wait_for_exit(void) {
	int status;
	struct rusage ru;
	while (1) {
		/* wait for signal from child */
		wait4(pid, &status, 0, &ru);

		/* any signal stops the child, so check that
			the incoming signal is a SIGTRAP and the
			event_exit flag is set */
		if ((WSTOPSIG(status) == SIGTRAP) && (status & (PTRACE_EVENT_EXIT << 8))) {
			break;
		}

		/* if not, pass the original signal onto the child */
		if (ptrace(PTRACE_CONT, pid, 0, WSTOPSIG(status))) {
			perror("stopper: ptrace(PTRACE_CONT, ...)");
			exit(1);
		}
	}

	long ret_code;
	ptrace(PTRACE_GETEVENTMSG, pid, 0, &ret_code);
	write_stats(&ru, ret_code);

	if (ptrace(PTRACE_CONT, pid, 0, 0)) {
		perror("stopper: ptrace(PTRACE_CONT, ...)");
		exit(1);
	}
	exit(0);
}

void write_stats(struct rusage* ru, long ret) {
	FILE* fout = fopen(outfile, "w");
	struct timeval end;
	gettimeofday(&end, NULL);

	printPerf(fout);

	struct statStuff s;
	if (readStat(pid, &s)) {
		printStat(fout, &s);
	}

	struct statmStuff sm;
	if (readStatm(pid, &sm)) {
		printStatm(fout, &sm);
	}

	struct statusStuff ss;
	if (readStatus(pid, &ss)) {
		printStatus(fout, &ss);
	}

	struct ioStuff is;
	if (readIO(pid, &is)) {
		printIO(fout, &is);
	}

	printRusage(fout, ru);

	printTimeDiff(fout, &start, &end);
	printReturnCode(fout, ret);
	fclose(fout);
}
