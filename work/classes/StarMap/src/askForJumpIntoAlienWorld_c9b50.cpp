#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_alien_text;

extern "C" String *GameText_getText(void *gameText, int id);
extern "C" void ChoiceWindow_set(void *choiceWindow, String *text, bool enabled);

void StarMap::askForJumpIntoAlienWorld()
{
    void *window = *(void *volatile *)((char *)this + 0x5c);
    field<uint8_t>(this, 0x120) = 1;
    String *text = GameText_getText(*g_StarMap_alien_text, 0x1a6);
    ChoiceWindow_set(window, text, true);
    field<uint8_t>(this, 0xa9) = 1;
}
