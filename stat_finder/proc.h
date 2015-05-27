/*
	retrieved and adapted from Prof. Alva L. Couch's code
	at http://www.cs.tufts.edu/comp/111/assignments/a3/proc.c
*/

#ifndef PROC_H
#define PROC_H

#include <stdio.h>

#ifndef LINE_MAX
#define LINE_MAX 256
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif

#define READ_WRAPPER(FILENAME, FUNC)                                                                                   \
	int return_code = 0;                                                                                                \
	char buf[FILENAME_MAX];                                                                                             \
	FILE* proc;                                                                                                         \
	snprintf(buf, FILENAME_MAX, "/proc/%d/%s", pid, FILENAME);                                                          \
	proc = fopen(buf, "r");                                                                                             \
	if (proc) {                                                                                                         \
		return_code = FUNC(proc, s);                                                                                     \
		fclose(proc);                                                                                                    \
	}                                                                                                                   \
	return return_code;

#define XSTR(A) STR(A)
#define STR(A) #A

// how to read and print various things in /proc

/////////////////////////////////
// how to read /proc/*/stat
/////////////////////////////////

struct statStuff {
	int pid;	// %d
	char comm[LINE_MAX + 1];	// %s
	char state;	// %c
	int ppid;	// %d
	int pgrp;	// %d
	int session;	// %d
	int tty_nr;	// %d
	int tpgid;	// %d
	unsigned flags;	// %u //changed
	unsigned long minflt;	// %lu
	unsigned long cminflt;	// %lu
	unsigned long majflt;	// %lu
	unsigned long cmajflt;	// %lu
	unsigned long utime;	// %lu
	unsigned long stime;	// %lu
	long cutime;	// %ld
	long cstime;	// %ld
	long priority;	// %ld
	long nice;	// %ld
	long num_threads;	// %ld
	long itrealvalue;	// %ld
	unsigned long long starttime;	// %llu //changed
	unsigned long vsize;	// %lu
	long rss;	// %ld
	unsigned long rlim;	// %lu
	unsigned long startcode;	// %lu
	unsigned long endcode;	// %lu
	unsigned long startstack;	// %lu
	unsigned long kstkesp;	// %lu
	unsigned long kstkeip;	// %lu
	unsigned long signal;	// %lu
	unsigned long blocked;	// %lu
	unsigned long sigignore;	// %lu
	unsigned long sigcatch;	// %lu
	unsigned long wchan;	// %lu
	unsigned long nswap;	// %lu
	unsigned long cnswap;	// %lu
	int exit_signal;	// %d
	int processor;	// %d
	unsigned rt_priority;	// %u //changed
	unsigned policy;	// %u //changed
	unsigned long long delayacct_blkio_ticks;	// %llu
	unsigned long guest_time;	// %lu // new
	long cguest_time;	// %ld
	unsigned long start_data;	// %lu
	unsigned long end_data;	// %lu
	unsigned long start_brk;	// %lu
	unsigned long arg_start;	// %lu
	unsigned long arg_end;	// %lu
	unsigned long env_start;	// %lu
	unsigned long env_end;	// %lu
	int exit_code;	// %d
};

static int _readStat(FILE* proc, struct statStuff* s) {

	const char* format =
		"%d %" XSTR(LINE_MAX) "s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu"
		" %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld %lu %lu %lu "
		"%lu %lu %lu %lu %d";

	return 52 == fscanf(proc, format, &s->pid, s->comm, &s->state, &s->ppid, &s->pgrp, &s->session, &s->tty_nr,
						&s->tpgid, &s->flags, &s->minflt, &s->cminflt, &s->majflt, &s->cmajflt, &s->utime, &s->stime,
						&s->cutime, &s->cstime, &s->priority, &s->nice, &s->num_threads, &s->itrealvalue, &s->starttime,
						&s->vsize, &s->rss, &s->rlim, &s->startcode, &s->endcode, &s->startstack, &s->kstkesp,
						&s->kstkeip, &s->signal, &s->blocked, &s->sigignore, &s->sigcatch, &s->wchan, &s->nswap,
						&s->cnswap, &s->exit_signal, &s->processor, &s->rt_priority, &s->policy,
						&s->delayacct_blkio_ticks, &s->guest_time, &s->cguest_time, &s->start_data, &s->end_data,
						&s->start_brk, &s->arg_start, &s->arg_end, &s->env_start, &s->env_end, &s->exit_code);
}

static int readStat(int pid, struct statStuff* s) {
	READ_WRAPPER("stat", _readStat)
}

static void printStat(FILE* out, struct statStuff* stuff) {
	fprintf(out, "pid:%d\n", stuff->pid);
	fprintf(out, "comm:%s\n", stuff->comm);
	fprintf(out, "state:%c\n", stuff->state);
	fprintf(out, "ppid:%d\n", stuff->ppid);
	fprintf(out, "pgrp:%d\n", stuff->pgrp);
	fprintf(out, "session:%d\n", stuff->session);
	fprintf(out, "tty_nr:%d\n", stuff->tty_nr);
	fprintf(out, "tpgid:%d\n", stuff->tpgid);
	fprintf(out, "flags:%u\n", stuff->flags);
	fprintf(out, "minflt:%lu\n", stuff->minflt);
	fprintf(out, "cminflt:%lu\n", stuff->cminflt);
	fprintf(out, "majflt:%lu\n", stuff->majflt);
	fprintf(out, "cmajflt:%lu\n", stuff->cmajflt);
	fprintf(out, "utime:%lu\n", stuff->utime);
	fprintf(out, "utime_sec:%f\n", (double)stuff->utime / sysconf(_SC_CLK_TCK));
	fprintf(out, "stime:%lu\n", stuff->stime);
	fprintf(out, "stime_sec:%f\n", (double)stuff->stime / sysconf(_SC_CLK_TCK));
	fprintf(out, "cutime:%ld\n", stuff->cutime);
	fprintf(out, "cutime_sec:%f\n", (double)stuff->cutime / sysconf(_SC_CLK_TCK));
	fprintf(out, "cstime:%ld\n", stuff->cstime);
	fprintf(out, "cstime_sec:%f\n", (double)stuff->cstime / sysconf(_SC_CLK_TCK));
	fprintf(out, "priority:%ld\n", stuff->priority);
	fprintf(out, "nice:%ld\n", stuff->nice);
	fprintf(out, "num_threads:%ld\n", stuff->num_threads);
	fprintf(out, "itrealvalue:%ld\n", stuff->itrealvalue);
	fprintf(out, "starttime:%llu\n", stuff->starttime);
	fprintf(out, "starttime_sec:%f\n", (double)stuff->starttime / sysconf(_SC_CLK_TCK));
	fprintf(out, "vsize:%lu\n", stuff->vsize);
	fprintf(out, "rss:%ld\n", stuff->rss);
	fprintf(out, "rlim:%lu\n", stuff->rlim);
	fprintf(out, "startcode:%lu\n", stuff->startcode);
	fprintf(out, "endcode:%lu\n", stuff->endcode);
	fprintf(out, "startstack:%lu\n", stuff->startstack);
	fprintf(out, "kstkesp:%lu\n", stuff->kstkesp);
	fprintf(out, "kstkeip:%lu\n", stuff->kstkeip);
	fprintf(out, "signal:%lu\n", stuff->signal);
	fprintf(out, "blocked:%lu\n", stuff->blocked);
	fprintf(out, "sigignore:%lu\n", stuff->sigignore);
	fprintf(out, "sigcatch:%lu\n", stuff->sigcatch);
	fprintf(out, "wchan:%lu\n", stuff->wchan);
	fprintf(out, "nswap:%lu\n", stuff->nswap);
	fprintf(out, "cnswap:%lu\n", stuff->cnswap);
	fprintf(out, "exit_signal:%d\n", stuff->exit_signal);
	fprintf(out, "processor:%d\n", stuff->processor);
	fprintf(out, "rt_priority:%u\n", stuff->rt_priority);
	fprintf(out, "policy:%u\n", stuff->policy);
	fprintf(out, "delayacct_blkio_ticks:%llu\n", stuff->delayacct_blkio_ticks);
	fprintf(out, "guest_time:%lu\n", stuff->guest_time);
	fprintf(out, "guest_time_sec:%f\n", (double)stuff->guest_time / sysconf(_SC_CLK_TCK));
	fprintf(out, "cguest_time:%ld\n", stuff->cguest_time);
	fprintf(out, "cguest_time_sec:%f\n", (double)stuff->cguest_time / sysconf(_SC_CLK_TCK));
	fprintf(out, "start_data:%lu\n", stuff->start_data);
	fprintf(out, "end_data:%lu\n", stuff->end_data);
	fprintf(out, "start_brk:%lu\n", stuff->start_brk);
	fprintf(out, "arg_start:%lu\n", stuff->arg_start);
	fprintf(out, "arg_end:%lu\n", stuff->arg_end);
	fprintf(out, "env_start:%lu\n", stuff->env_start);
	fprintf(out, "env_end:%lu\n", stuff->env_end);
	fprintf(out, "exit_code:%d\n", stuff->exit_code);
}

/////////////////////////////////
// how to read /proc/*/statm
/////////////////////////////////

struct statmStuff {
	unsigned long size;
	unsigned long resident;
	unsigned long share;
	unsigned long text;
	unsigned long lib;
	unsigned long data;
	unsigned long dt;
};

static int _readStatm(FILE* proc, struct statmStuff* s) {

	const char* format = "%lu %lu %lu %lu %lu %lu %ld";

	return 7 == fscanf(proc, format, &s->size, &s->resident, &s->share, &s->text, &s->lib, &s->data, &s->dt);
}

static int readStatm(int pid, struct statmStuff* s) {
	READ_WRAPPER("statm", _readStatm)
}

static void printStatm(FILE* out, struct statmStuff* stuff) {
	fprintf(out, "size:%lu\n", stuff->size);
	fprintf(out, "resident:%lu\n", stuff->resident);
	fprintf(out, "share:%lu\n", stuff->share);
	fprintf(out, "text:%lu\n", stuff->text);
	fprintf(out, "lib:%lu\n", stuff->lib);
	fprintf(out, "data:%lu\n", stuff->data);
	fprintf(out, "dt:%lu\n", stuff->dt);
}

/////////////////////////////////
// how to read /proc/*/io
/////////////////////////////////

struct ioStuff {
	unsigned long rchar;
	unsigned long wchar;
	unsigned long syscr;
	unsigned long syscw;
	unsigned long read_bytes;
	unsigned long write_bytes;
	unsigned long cancelled_write_bytes;
};

static int _readIO(FILE* proc, struct ioStuff* s) {
	char buf[LINE_MAX];
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "rchar:\t%lu", &s->rchar) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "wchar:\t%lu", &s->wchar) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "syscr:\t%lu", &s->syscr) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "syscw:\t%lu", &s->syscw) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "read_bytes:\t%lu", &s->read_bytes) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "write_bytes:\t%lu", &s->write_bytes) != 1) {
		return 0;
	}
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "cancelled_write_bytes:\t%lu", &s->cancelled_write_bytes) != 1) {
		return 0;
	}
	return 1;
}

static int readIO(int pid, struct ioStuff* s) {
	READ_WRAPPER("io", _readIO)
}

static void printIO(FILE* out, struct ioStuff* stuff) {
	fprintf(out, "rchar:%lu\n", stuff->rchar);
	fprintf(out, "wchar:%lu\n", stuff->rchar);
	fprintf(out, "syscr:%lu\n", stuff->syscr);
	fprintf(out, "syscw:%lu\n", stuff->syscr);
	fprintf(out, "read_bytes:%lu\n", stuff->read_bytes);
	fprintf(out, "write_bytes:%lu\n", stuff->write_bytes);
	fprintf(out, "cancelled_write_bytes:%lu\n", stuff->cancelled_write_bytes);
}

/////////////////////////////////
// how to read /proc/*/status
/////////////////////////////////

struct statusStuff {
	char Name[LINE_MAX + 1];	// tcsh
	char State;	// S (sleeping)
	unsigned long Tgid;	//	20616
	unsigned long Ngid;	//	0
	unsigned long Pid;	//	20616
	unsigned long PPid;	//	20612
	unsigned long TracerPid;	//	0
	unsigned long Uid[4];	//	418	418	418	418
	unsigned long Gid[4];	//	30	30	30	30
	unsigned long FDSize;	//	64
	unsigned long Groups[16];	//	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245
	unsigned long VmPeak;	//	   64732 kB
	unsigned long VmSize;	//	   64700 kB
	unsigned long VmLck;	//	       0 kB
	unsigned long VmPin;	//		    0 kB
	unsigned long VmHWM;	//	    1756 kB
	unsigned long VmRSS;	//	    1756 kB
	unsigned long VmData;	//	    1112 kB
	unsigned long VmStk;	//	     348 kB
	unsigned long VmExe;	//	     320 kB
	unsigned long VmLib;	//	    1496 kB
	unsigned long VmPMD;	//	       4 kB
	unsigned long VmPTE;	//	      68 kB
	unsigned long VmSwap;	//	       0 kB
	unsigned long Threads;	//	1
	unsigned long SigQ[2];	//	1/16368
	unsigned long SigPnd;	//	0000000000000000
	unsigned long ShdPnd;	//	0000000000000000
	unsigned long SigBlk;	//	0000000000000002
	unsigned long SigIgn;	//	0000000000384004
	unsigned long SigCgt;	//	0000000009812003
	unsigned long CapInh;	//	0000000000000000
	unsigned long CapPrm;	//	0000000000000000
	unsigned long CapEff;	//	0000000000000000
	unsigned long CapBnd;	//	0000000000000000
	unsigned long Seccomp;	//		0
	unsigned long Cpus_allowed[8];	// ff
	char Cpus_allowed_list[LINE_MAX + 1];	// 0-7
	unsigned long Mems_allowed[2];	//  00000000,00000001
	char Mems_allowed_list[LINE_MAX + 1];	// 0
	unsigned long voluntary_ctxt_switches;
	unsigned long nonvoluntary_ctxt_switches;	//	00000000,00000001
};

static int _readStatus(FILE* proc, struct statusStuff* s) {
	int i;
	char buf[LINE_MAX];
	// Name:	tcsh
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Name:\t%" XSTR(LINE_MAX) "s", s->Name) != 1) {
		return 0;
	}
	// State:\t	S (sleeping)
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "State:\t%c", &s->State) != 1) {
		return 0;
	}
	// Tgid:\t	20616
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Tgid:\t%lu", &s->Tgid) != 1) {
		return 0;
	}
	// Ngid:\t	0
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Ngid:\t%lu", &s->Ngid) != 1) {
		return 0;
	}
	// Pid:\t	20616
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Pid:\t%lu", &s->Pid) != 1) {
		return 0;
	}
	// PPid:\t	20612
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "PPid:\t%lu", &s->PPid) != 1) {
		return 0;
	}
	// TracerPid:\t	0
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "TracerPid:\t%lu", &s->TracerPid) != 1) {
		return 0;
	}
	// Uid:\t	418	418	418	418
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Uid:\t%lu\t%lu\t%lu\t%lu", s->Uid, s->Uid + 1, s->Uid + 2, s->Uid + 3) != 4) {
		return 0;
	}
	// Gid:\t	30	30	30	30
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Gid:\t%lu\t%lu\t%lu\t%lu", s->Gid, s->Gid + 1, s->Gid + 2, s->Gid + 3) != 4) {
		return 0;
	}
	// FDSize:\t	64
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "FDSize:\t%lu", &s->FDSize) != 1) {
		return 0;
	}
	// Groups:\t	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245
	fgets(buf, LINE_MAX, proc);
	for (i = 0; i < 16; i++) {
		s->Groups[i] = 0;
	}
	i = sscanf(buf, "Groups:\t%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld", s->Groups, s->Groups + 1, s->Groups + 2,
			   s->Groups + 3, s->Groups + 4, s->Groups + 5, s->Groups + 6, s->Groups + 7, s->Groups + 8, s->Groups + 9,
			   s->Groups + 10, s->Groups + 11, s->Groups + 12, s->Groups + 13, s->Groups + 14, s->Groups + 15);
	if (i < 1) {
		return 0;
	}
	// VmPeak:\t	   64732 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmPeak:\t%lu", &s->VmPeak) != 1) {
		return 0;
	}
	// VmSize:\t	   64700 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmSize:\t%lu", &s->VmSize) != 1) {
		return 0;
	}
	// VmLck:\t	       0 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmLck:\t%lu", &s->VmLck) != 1) {
		return 0;
	}
	// VmPin:\t	       0 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmPin:\t%lu", &s->VmPin) != 1) {
		return 0;
	}
	// VmHWM:\t	    1756 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmHWM:\t%lu", &s->VmHWM) != 1) {
		return 0;
	}
	// VmRSS:\t	    1756 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmRSS:\t%lu", &s->VmRSS) != 1) {
		return 0;
	}
	// VmData:\t	    1112 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmData:\t%lu", &s->VmData) != 1) {
		return 0;
	}
	// VmStk:\t	     348 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmStk:\t%lu", &s->VmStk) != 1) {
		return 0;
	}
	// VmExe:\t	     320 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmExe:\t%lu", &s->VmExe) != 1) {
		return 0;
	}
	// VmLib:\t	    1496 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmLib:\t%lu", &s->VmLib) != 1) {
		return 0;
	}
	// VmPTE:\t	      68 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmPTE:\t%lu", &s->VmPTE) != 1) {
		return 0;
	}
	// VmPMD:\t	      68 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmPMD:\t%lu", &s->VmPMD) != 1) {
		return 0;
	}
	// VmSwap:\t	      68 kB
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "VmSwap:\t%lu", &s->VmSwap) != 1) {
		return 0;
	}
	// Threads:\t	1
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Threads:\t%lu", &s->Threads) != 1) {
		return 0;
	}
	// SigQ:\t	1/16368
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "SigQ:\t%lu/%lu", s->SigQ, s->SigQ + 1) != 2) {
		return 0;
	}
	// SigPnd:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "SigPnd:\t%lx", &s->SigPnd) != 1) {
		return 0;
	}
	// ShdPnd:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "ShdPnd:\t%lx", &s->ShdPnd) != 1) {
		return 0;
	}
	// SigBlk:\t	0000000000000002
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "SigBlk:\t%lx", &s->SigBlk) != 1) {
		return 0;
	}
	// SigIgn:\t	0000000000384004
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "SigIgn:\t%lx", &s->SigIgn) != 1) {
		return 0;
	}
	// SigCgt:\t	0000000009812003
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "SigCgt:\t%lx", &s->SigCgt) != 1) {
		return 0;
	}
	// CapInh:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "CapInh:\t%lx", &s->CapInh) != 1) {
		return 0;
	}
	// CapPrm:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "CapPrm:\t%lx", &s->CapPrm) != 1) {
		return 0;
	}
	// CapEff:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "CapEff:\t%lx", &s->CapEff) != 1) {
		return 0;
	}
	// CapBnd:\t	0000000000000000
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "CapBnd:\t%lx", &s->CapBnd) != 1) {
		return 0;
	}
	// Seccomp:\t	00
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Seccomp:\t%lx", &s->Seccomp) != 1) {
		return 0;
	}
	// Cpus_allowed:\t	00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff
	fgets(buf, LINE_MAX, proc);
	for (i = 0; i < 8; i++) {
		s->Cpus_allowed[i] = 0;
	}
	if (sscanf(buf, "Cpus_allowed:\t%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx", s->Cpus_allowed, s->Cpus_allowed + 1,
			   s->Cpus_allowed + 2, s->Cpus_allowed + 3, s->Cpus_allowed + 4, s->Cpus_allowed + 5, s->Cpus_allowed + 6,
			   s->Cpus_allowed + 7) < 1) {
		return 0;
	}
	// Cpus_allowed_list:\t	0-7
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Cpus_allowed_list:\t%" XSTR(LINE_MAX) "s", &s->Cpus_allowed_list) != 1) {
		return 0;
	}
	// Mems_allowed:\t	00000000,00000001
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Mems_allowed:\t%lx,%lx", &(s->Mems_allowed[0]), &(s->Mems_allowed[1])) < 1) {
		return 0;
	}
	// Mems_allowed_list:\t	0
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "Mems_allowed_list:\t%" XSTR(LINE_MAX) "s", &s->Mems_allowed_list) != 1) {
		return 0;
	}
	// voluntary_ctxt_switches:\t	1
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "voluntary_ctxt_switches:\t%lu", &s->voluntary_ctxt_switches) != 1) {
		return 0;
	}
	// nonvoluntary_ctxt_switches:\t	1
	fgets(buf, LINE_MAX, proc);
	if (sscanf(buf, "nonvoluntary_ctxt_switches:\t%lu", &s->nonvoluntary_ctxt_switches) != 1) {
		return 0;
	}
	return 1;
}

static int readStatus(int pid, struct statusStuff* s) {
	READ_WRAPPER("status", _readStatus)
}

static void printStatus(FILE* out, struct statusStuff* s) {
	// Name:	tcsh
	fprintf(out, "Name:%s\n", s->Name);
	// State:	S (sleeping)
	fprintf(out, "State:%c\n", s->State);
	// Tgid:	20616
	fprintf(out, "Tgid:%lu\n", s->Tgid);
	// Tgid:	20616
	fprintf(out, "Ngid:%lu\n", s->Ngid);
	// Pid:	20616
	fprintf(out, "Pid:%lu\n", s->Pid);
	// PPid:	20612
	fprintf(out, "PPid:%lu\n", s->PPid);
	// TracerPid:	0
	fprintf(out, "TracerPid:%lu\n", s->TracerPid);
	// Uid:	418	418	418	418
	fprintf(out, "Uid:%lu\t%lu\t%lu\t%lu\n", s->Uid[0], s->Uid[1], s->Uid[2], s->Uid[3]);
	// Gid:	30	30	30	30
	fprintf(out, "Gid:%lu\t%lu\t%lu\t%lu\n", s->Gid[0], s->Gid[1], s->Gid[2], s->Gid[3]);
	// FDSize:	64
	fprintf(out, "FDSize:%lu\n", s->FDSize);
	// Groups:	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245
	fprintf(out, "Groups:%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", s->Groups[0], s->Groups[1],
			s->Groups[2], s->Groups[3], s->Groups[4], s->Groups[5], s->Groups[6], s->Groups[7], s->Groups[8],
			s->Groups[9], s->Groups[10], s->Groups[11], s->Groups[12], s->Groups[13], s->Groups[14], s->Groups[15]);
	// VmPeak:	   64732 kB
	fprintf(out, "VmPeak:%lu\n", s->VmPeak);
	// VmSize:	   64700 kB
	fprintf(out, "VmSize:%lu\n", s->VmSize);
	// VmLck:	       0 kB
	fprintf(out, "VmLck:%lu\n", s->VmLck);
	// VmPin:	       0 kB
	fprintf(out, "VmPin:%lu\n", s->VmPin);
	// VmHWM:	    1756 kB
	fprintf(out, "VmHWM:%lu\n", s->VmHWM);
	// VmRSS:	    1756 kB
	fprintf(out, "VmRSS:%lu\n", s->VmRSS);
	// VmData:	    1112 kB
	fprintf(out, "VmData:%lu\n", s->VmData);
	// VmStk:	     348 kB
	fprintf(out, "VmStk:%lu\n", s->VmStk);
	// VmExe:	     320 kB
	fprintf(out, "VmExe:%lu\n", s->VmExe);
	// VmLib:	    1496 kB
	fprintf(out, "VmLib:%lu\n", s->VmLib);
	// VmPMD:	      68 kB
	fprintf(out, "VmPMD:%lu\n", s->VmPMD);
	// VmPTE:	      68 kB
	fprintf(out, "VmPTE:%lu\n", s->VmPTE);
	// VmSwap:	      68 kB
	fprintf(out, "VmSwap:%lu\n", s->VmSwap);
	// Threads:	1
	fprintf(out, "Threads:%lu\n", s->Threads);
	// SigQ:	1/16368
	fprintf(out, "SigQ:%lu/%lu\n", s->SigQ[0], s->SigQ[1]);
	// SigPnd:	0000000000000000
	fprintf(out, "SigPnd:%lx\n", s->SigPnd);
	// ShdPnd:	0000000000000000
	fprintf(out, "ShdPnd:%lx\n", s->ShdPnd);
	// SigBlk:	0000000000000002
	fprintf(out, "SigBlk:%lx\n", s->SigBlk);
	// SigIgn:	0000000000384004
	fprintf(out, "SigIgn:%lx\n", s->SigIgn);
	// SigCgt:	0000000009812003
	fprintf(out, "SigCgt:%lx\n", s->SigCgt);
	// CapInh:	0000000000000000
	fprintf(out, "CapInh:%lx\n", s->CapInh);
	// CapPrm:	0000000000000000
	fprintf(out, "CapPrm:%lx\n", s->CapPrm);
	// CapEff:	0000000000000000
	fprintf(out, "CapEff:%lx\n", s->CapEff);
	// Cpus_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff
	fprintf(out, "Cpus_allowed:\t%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx\n", s->Cpus_allowed[0], s->Cpus_allowed[1],
			s->Cpus_allowed[2], s->Cpus_allowed[3], s->Cpus_allowed[4], s->Cpus_allowed[5], s->Cpus_allowed[6],
			s->Cpus_allowed[7]);
	// Cpus_allowed_list:	0-7
	fprintf(out, "Cpus_allowed_list:%s\n", s->Cpus_allowed_list);
	// Mems_allowed:	00000000,00000001
	fprintf(out, "Mems_allowed:\t%lx,%lx\n", s->Mems_allowed[0], s->Mems_allowed[1]);
	// Mems_allowed_list:	0
	fprintf(out, "Mems_allowed:%s\n", s->Mems_allowed_list);
	// voluntary_ctxt_switches:	1
	fprintf(out, "voluntary_ctxt_switches:%lu\n", s->voluntary_ctxt_switches);
	// nonvoluntary_ctxt_switches:	1
	fprintf(out, "nonvoluntary_ctxt_switches:%lu\n", s->nonvoluntary_ctxt_switches);
}
#endif /* PROC_H */
