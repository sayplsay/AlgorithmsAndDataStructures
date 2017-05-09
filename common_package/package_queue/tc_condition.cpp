#include "tc_condition.h"
#include "tc_current_thread.h"
#include "tc_thread.h"
#include <errno.h>

// returns true if time out, false otherwise.
bool taf::Condition::waitForSeconds(int seconds)
{
  struct timespec abstime;
  // FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
  clock_gettime(CLOCK_REALTIME, &abstime);
  abstime.tv_sec += seconds;
  MutexLock::UnassignGuard ug(mutex_);
  return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}

