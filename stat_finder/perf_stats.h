#ifndef PERF_STATS_H
#define PERF_STATS_H

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>

#include <sys/syscall.h>
#include <unistd.h>

struct perf_stat {
	const char* name;
	struct perf_event_attr attr;
	int file;
};

void init_perf_events(pid_t pid);
void printPerf(FILE* out);

static void printPerfStat(FILE* out, struct perf_stat* ps);

static struct perf_stat stats[] = {
	{
		.name = "task_clock",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_SOFTWARE,
			.config = PERF_COUNT_SW_TASK_CLOCK
		},
		.file = -1
	},
	{
		.name = "cpu_clock",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_SOFTWARE,
			.config = PERF_COUNT_SW_CPU_CLOCK
		},
		.file = -1
	},
	{
		.name = "context_switches",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_SOFTWARE,
			.config = PERF_COUNT_SW_CONTEXT_SWITCHES
		},
		.file = -1
	},
	{
		.name = "cpu_migrations",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_SOFTWARE,
			.config = PERF_COUNT_SW_CPU_MIGRATIONS
		},
		.file = -1
	},
	{
		.name = "page_faults",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_SOFTWARE,
			.config = PERF_COUNT_SW_PAGE_FAULTS
		},
		.file = -1
	},
	{
		.name = "cpu_cycles",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_CPU_CYCLES
		},
		.file = -1
	},
	{
		.name = "stalled_cycles_frontend",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND
		},
		.file = -1
	},
	{
		.name = "stalled_cycles_backend",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND
		},
		.file = -1
	},
	{
		.name = "instructions",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_INSTRUCTIONS
		},
		.file = -1
	},
	{
		.name = "branch_instructions",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS
		},
		.file = -1
	},
	{
		.name = "branch_misses",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HARDWARE,
			.config = PERF_COUNT_HW_BRANCH_MISSES
		},
		.file = -1
	},
	{
		.name = "l1d_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1D << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "l1d_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1D << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{
		.name = "ll_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_LL << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "ll_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_LL << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{
		.name = "l1i_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1I << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "l1i_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1I << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{
		.name = "dtlb_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_DTLB << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "dtlb_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_DTLB << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{
		.name = "itlb_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_ITLB << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "itlb_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_ITLB << 0 | (PERF_COUNT_HW_CACHE_OP_READ << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{
		.name = "l1d_prefetch_access",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1D << 0 | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
		},
		.file = -1
	},
	{
		.name = "l1d_prefetch_miss",
		.attr = {
			.size = sizeof(struct perf_event_attr),
			.inherit = 1,
			.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING,
			.type = PERF_TYPE_HW_CACHE,
			.config = PERF_COUNT_HW_CACHE_L1D << 0 | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) |
			(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
		},
		.file = -1
	},
	{.name = NULL}
};

static int perf_event_open(struct perf_event_attr* hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
	return (int)syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void init_perf_events(pid_t pid) {
	for (int i = 0; stats[i].name; ++i) {
		stats[i].file = perf_event_open(&stats[i].attr, pid, -1, -1, PERF_FLAG_FD_CLOEXEC);
	}
}

struct perfRead {
	unsigned long long val;
	unsigned long long time_e;
	unsigned long long time_r;
};

void printPerf(FILE* out) {
	for (int i = 0; stats[i].name; ++i) {
		printPerfStat(out, &stats[i]);
		close(stats[i].file);
	}
}

void printPerfStat(FILE* out, struct perf_stat* ps) {
	struct perfRead r;
	if (ps->file == -1) {
		return;
	}
	read(ps->file, &r, sizeof(r));
	if (r.time_e == 0 || r.time_r == 0) {
		return;
	}
	double mult = (0.0 + r.time_e) / r.time_r;
	unsigned long long val = mult * r.val + 0.5;

	fprintf(out, "%s:%llu\n", ps->name, val);
}

#endif /* PERF_STATS_H */
