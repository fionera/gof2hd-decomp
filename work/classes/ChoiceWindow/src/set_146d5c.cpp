#include "class.h"

extern "C" String *GameText_getText(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146d5c;

void ChoiceWindow::set(String const &text, bool flag)
{
    String *title = GameText_getText(*g_ChoiceWindow_gameText_146d5c, 0x186);
    set(*title, text, flag);
}
