#pragma once

#ifdef DEBUG
#include <ArduinoLog.h>
#include <MemoryFree.h>

namespace
{
    int lastFreeMem{0};
}

template<typename T, typename... Args>
inline void mylog(T msg, Args... args)
{
    Log.verboseln(msg, args...);
}

template<typename T, typename... Args>
inline void mylogmem(T msg, Args... args)
{
    if (freeMemory() != lastFreeMem) 
    {
        lastFreeMem = freeMemory();
        Log.verbose(msg, args...); 
        Log.verboseln(F("  free: %d"), freeMemory());
    }
}

inline void mylogmem()
{
    if (freeMemory() != lastFreeMem) 
    {
        lastFreeMem = freeMemory();
        Log.verboseln(F("  free: %d"), lastFreeMem);
    }
}

#else
#define DISABLE_LOGGING
#define mylog(msg, ...)
#endif

