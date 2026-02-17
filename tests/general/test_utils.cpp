#include "test_utils.hpp"
#include <cstddef>

#ifdef WIN64
#include <windows.h>
#else
#include <sys/time.h>
#endif

int get_time_ms() {
#ifdef WIN64
    return GetTickCount();
#else
    struct timeval time_value;
    gettimeofday(&time_value, NULL);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
#endif
}
