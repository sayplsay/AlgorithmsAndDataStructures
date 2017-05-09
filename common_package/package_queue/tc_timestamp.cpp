#include "tc_timestamp.h"

#include <stdio.h>
#include <sys/time.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

namespace taf {

Timestamp Timestamp::Now()
{
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    return Timestamp(tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::ToString() const
{
    int64_t seconds = microseconds_since_epoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microseconds_since_epoch_ % kMicroSecondsPerSecond;

    char time[32];
    snprintf(time, sizeof(time)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return time;
}

std::string Timestamp::LocalTimeToString(time_t t)
{
    struct tm result;
    localtime_r(&t, &result);
    int year = result.tm_year + 1900;
    int mon = result.tm_mon + 1;
    int day = result.tm_mday;
    int hour = result.tm_hour;
    int min = result.tm_min;
    int sec = result.tm_sec;

    char buf[1024];
    snprintf(buf, sizeof buf, "%d-%02d-%02d %02d:%02d:%02d",
             year, mon, day, hour, min, sec);
    return buf;
}

} // namespace cloud_ucs
