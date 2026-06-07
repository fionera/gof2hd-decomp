#include "class.h"

extern "C" String *GameText_getText(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468c8;

void ChoiceWindow::set(String const &text)
{
    String *title = GameText_getText(*g_ChoiceWindow_gameText_1468c8, 0x186);
    set(*title, text, false);
}
