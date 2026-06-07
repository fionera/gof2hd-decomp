#include "class.h"

typedef String *(*GetTextFn)(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468f4;
__attribute__((visibility("hidden"))) extern GetTextFn g_ChoiceWindow_getText_1468f4;

void ChoiceWindow::set(String const &title, String const &message, bool flag)
{
    void **gameText = g_ChoiceWindow_gameText_1468f4;
    GetTextFn getText = g_ChoiceWindow_getText_1468f4;
    String *left = getText(*gameText, 0x86);
    String *center = getText(*gameText, 0x87);
    String *right = getText(*gameText, 0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
}
