#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>

struct perf_stat {
	const char * name;
	struct perf_event_attr attr;
	int file;
};

//.size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING
struct perf_stat stats[] = {
	{
		.name = "task_clock",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_TASK_CLOCK		},
		.file=-1
	},
	{
		.name = "cpu_clock",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_CLOCK		},
		.file=-1
	},
	{
		.name = "context_switches",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CONTEXT_SWITCHES		},
		.file=-1
	},
	{
		.name = "cpu_migrations",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_MIGRATIONS		},
		.file=-1
	},
	{
		.name = "page_faults",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_PAGE_FAULTS		},
		.file=-1
	},
	{
		.name ="cpu_cycles",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES		},
		.file=-1
	},
	{
		.name ="stalled_cycles_frontend",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND		},
		.file=-1
	},
	{
		.name ="stalled_cycles_backend",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND		},
		.file=-1
	},
	{
		.name ="instructions",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS		},
		.file=-1
	},
	{
		.name ="branch_instructions",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS		},
		.file=-1
	},
	{
		.name ="branch_misses",
		.attr={ .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_BRANCH_MISSES		},
		.file=-1
	},
	{
	.name="l1d_access",
	.attr = { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
	.file=-1
	},
	{
		.name="l1d_miss",
		.attr =   { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{
	.name="ll_access",
	.attr = { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_LL		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
	.file=-1
	},
	{
		.name="ll_miss",
		.attr =   { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_LL		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{
	.name="l1i_access",
	.attr = { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1I		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
	.file=-1
	},
	{
		.name="l1i_miss",
		.attr =   { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1I		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{
		.name="dtlb_access",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_DTLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
		.file=-1
	},
	{
		.name="dtlb_miss",
		.attr= { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_DTLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{
		.name = "itlb_access",
		.attr=  { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_ITLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
		.file=-1
	},
	{
		.name = "itlb_miss",
		.attr=  { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_ITLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{
	.name="l1d_prefetch_access",
	.attr = { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_PREFETCH	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},
	.file=-1
	},
	{
		.name="l1d_prefetch_miss",
		.attr =   { .size = sizeof(struct perf_event_attr), .inherit=1, .read_format=PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING, .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_PREFETCH	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
		.file=-1
	},
	{ .name = NULL }
};
