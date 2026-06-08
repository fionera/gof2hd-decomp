#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    this->f_8 = width;
    this->f_0 = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}
