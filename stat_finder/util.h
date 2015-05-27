#ifndef UTIL_H
#define UTIL_H

#include <sys/resource.h>
#include <sys/time.h>

/////////////////////////////////
// how to print other stats
/////////////////////////////////

void printRusage(FILE* out, struct rusage* stuff);
void printTimeDiff(FILE* out, const struct timeval* start, const struct timeval* end);
void printReturnCode(FILE* out, long return_code);

void printRusage(FILE* out, struct rusage* stuff) {
	fprintf(out, "ru_utime:%ld.%06ld\n", stuff->ru_utime.tv_sec, stuff->ru_utime.tv_usec);
	fprintf(out, "ru_stime:%ld.%06ld\n", stuff->ru_stime.tv_sec, stuff->ru_stime.tv_usec);
	fprintf(out, "ru_maxrss:%lu\n", stuff->ru_maxrss);
	fprintf(out, "ru_ixrss:%lu\n", stuff->ru_ixrss);
	fprintf(out, "ru_idrss:%lu\n", stuff->ru_idrss);
	fprintf(out, "ru_isrss:%lu\n", stuff->ru_isrss);
	fprintf(out, "ru_minflt:%lu\n", stuff->ru_minflt);
	fprintf(out, "ru_maxflt:%lu\n", stuff->ru_majflt);
	fprintf(out, "ru_nswap:%lu\n", stuff->ru_nswap);
	fprintf(out, "ru_inblock:%lu\n", stuff->ru_inblock);
	fprintf(out, "ru_oublock:%lu\n", stuff->ru_oublock);
	fprintf(out, "ru_msgsnd:%lu\n", stuff->ru_msgsnd);
	fprintf(out, "ru_msgrcv:%lu\n", stuff->ru_msgrcv);
	fprintf(out, "ru_nsignals:%lu\n", stuff->ru_nsignals);
	fprintf(out, "ru_nvcsw:%lu\n", stuff->ru_nvcsw);
	fprintf(out, "ru_nivcsw:%lu\n", stuff->ru_nivcsw);
}

void printTimeDiff(FILE* out, const struct timeval* start, const struct timeval* end) {
	struct timeval diff = {.tv_sec = end->tv_sec - start->tv_sec, .tv_usec = end->tv_usec - start->tv_usec};
	fprintf(out, "%s:%ld\n", "wall_time_sec", diff.tv_sec);
	fprintf(out, "%s:%ld\n", "wall_time_usec", diff.tv_usec);
}

void printReturnCode(FILE* out, long return_code) {
	fprintf(out, "%s:%ld\n", "return_code", return_code);
}

#endif /* UTIL_H */
