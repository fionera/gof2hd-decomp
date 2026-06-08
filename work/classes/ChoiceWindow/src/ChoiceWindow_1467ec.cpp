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
    int width = config->f_264;
    this->f_8 = width;
    this->f_48 = config->f_268;

    int screenWidth = *screenWidthPtr;
    this->f_10 = ChoiceWindow_Int4();
    this->f_34 = -1;
    this->f_38 = -1;
    F<uint8_t>(this, 0x59) = 1;
    this->f_0 = screenWidth / 2 - width / 2;
}
