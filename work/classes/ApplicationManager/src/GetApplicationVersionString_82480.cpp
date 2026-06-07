#include "class.h"

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_GetApplicationVersionString(String *out)
{
    new (out) String("2.0.16", false);
}
