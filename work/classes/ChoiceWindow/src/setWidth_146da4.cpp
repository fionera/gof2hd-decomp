#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    F<int>(this, 0x8) = width;
    F<int>(this, 0x0) = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}
