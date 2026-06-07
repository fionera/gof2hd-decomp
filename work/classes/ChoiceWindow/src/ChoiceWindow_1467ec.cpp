#include "class.h"

extern "C" void String_ctor(void *self);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_config_1467ec;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth_1467ec;

struct ChoiceWindow_Int4 {
    int a;
    int b;
    int c;
    int d;
};

ChoiceWindow::ChoiceWindow()
{
    void *primaryText = (char *)this + 0x20;
    String_ctor(primaryText);
    String_ctor((char *)this + 0x3c);

    void **configHolder = g_ChoiceWindow_config_1467ec;
    int *screenWidthPtr = g_ChoiceWindow_screenWidth_1467ec;
    void *config = *configHolder;
    int width = F<int>(config, 0x264);
    F<int>(this, 0x8) = width;
    F<ChoiceWindow_Int4>(this, 0x48) = F<ChoiceWindow_Int4>(config, 0x268);

    int screenWidth = *screenWidthPtr;
    F<ChoiceWindow_Int4>(this, 0x10) = ChoiceWindow_Int4();
    F<int>(this, 0x34) = -1;
    F<int>(this, 0x38) = -1;
    F<uint8_t>(this, 0x59) = 1;
    F<int>(this, 0x0) = screenWidth / 2 - width / 2;
}
