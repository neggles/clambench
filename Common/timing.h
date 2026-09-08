#ifndef timingincluded
#define timingincluded
#ifdef _MSC_VER
#include <sys\timeb.h>
extern struct timeb start, end;
#else
#include <sys/time.h>
#endif
void start_timing(void);
unsigned int end_timing(void);

#ifdef _MSC_VER
void start_timing_ts(struct timeb* startTimeb);
unsigned int end_timing_ts(struct timeb* startTimeb);
#else
void start_timing_ts(struct timeval* start);
unsigned int end_timing_ts(struct timeval* start);
#endif
unsigned long long scale_iterations_to_target(unsigned long long last_iteration_count, float last_time, float target_time);
#endif
