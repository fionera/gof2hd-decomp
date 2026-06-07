#include "class.h"

// *g_GameText_language -> current language id (signed short).
extern "C" __attribute__((visibility("hidden"))) short *g_GameText_language;

// GameText::getLanguage() -> (int)current language id.
extern "C" int GameText_getLanguage()
{
    return *g_GameText_language;
}
