#include "class.h"

extern "C" uint64_t g_perfElapsed;
extern "C" uint64_t g_perfActionCount;
extern "C" uint64_t g_perfLimitValue;
extern "C" uint64_t g_perfTotal;
extern "C" uint64_t g_perfFrame;
extern "C" char g_perfEnabled;
extern "C" char g_perfPendingFlag;

extern "C" void ApplicationManager_EnablePerformanceTest(int count)
{
    g_perfElapsed = 0;
    g_perfActionCount = (long long)count;
    g_perfLimitValue = 0;
    g_perfTotal = 0;
    g_perfFrame = 0;
    g_perfEnabled = 1;
    g_perfPendingFlag = 0;
}
