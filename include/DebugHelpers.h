#ifdef DEBUG
#include <ArduinoLog.h>
#include <MemoryFree.h>

#define mylog(...) Log.verboseln(__VA_ARGS__)
#else
#define DISABLE_LOGGING
#define mylog(msg, ...)
#endif

