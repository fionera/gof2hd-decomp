#include "class.h"

typedef String *(*GetTextFn)(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146954;
__attribute__((visibility("hidden"))) extern GetTextFn g_ChoiceWindow_getText_146954;

void ChoiceWindow::set(String const &a, String const &b)
{
    void **gameText = g_ChoiceWindow_gameText_146954;
    GetTextFn getText = g_ChoiceWindow_getText_146954;
    String *title = getText(*gameText, 0x186);
    String *left = getText(*gameText, 0x86);
    String *center = getText(*gameText, 0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}
