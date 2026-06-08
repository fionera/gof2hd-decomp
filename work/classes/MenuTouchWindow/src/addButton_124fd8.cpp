#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void _mtw_TouchButton_ctor(void *btn, void *label, int a, int x, int y, int w,
                                      char type, char id);
extern "C" void _mtw_ArrayAdd_TouchButton(void *btn, void *arr);
extern "C" void _mtw_TouchButton_getPosition(void *out, void *btn);

// PC-relative singleton holders.
extern void *const gAddBtnScreenW __attribute__((visibility("hidden")));  // *holder -> [0]=screen width
extern void *const gAddBtnLayout  __attribute__((visibility("hidden")));  // *holder -> layout, +0x30=row height
extern void *const gAddBtnScreenH __attribute__((visibility("hidden")));  // *holder -> [0]=screen height
extern void *const gAddBtnPosX    __attribute__((visibility("hidden")));  // x cache array
extern void *const gAddBtnPosY    __attribute__((visibility("hidden")));  // y cache array
extern void *const gAddBtnCount   __attribute__((visibility("hidden")));  // *holder -> count cell

// MenuTouchWindow::addButton(int id, String label, int row, Array<TouchButton*>* arr, int yOff)
struct MenuTouchWindow { void addButton(int id, void *label, int row, void *arr, int yOff); };
void MenuTouchWindow::addButton(int id, void *label, int row, void *arr, int yOff)
{
    void *btn = operator_new(0xc8);

    int btnW = i32(this, 0x1a8);
    int screenW = *(int *)*(void **)gAddBtnScreenW;
    int screenH = *(int *)*(void **)gAddBtnScreenH;
    int rowH = *(int *)((char *)*(void **)gAddBtnLayout + 0x30);

    int x = screenW / 2 - btnW / 2;
    int y = (rowH + i32(this, 0x1b0)) * row + (yOff - i32(this, 0x1ac)) + screenH / 2;

    _mtw_TouchButton_ctor(btn, label, 0, x, y, btnW, 0x11, 0x04);
    i32(btn, 0) = id;
    i32(btn, 4) = id >> 31;
    _mtw_ArrayAdd_TouchButton(btn, arr);

    int *posX = (int *)*(void **)gAddBtnPosX;
    int *posY = (int *)*(void **)gAddBtnPosY;
    for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
        if (i < 10) {
            char buf[12];
            void *b = ((void **)i32(arr, 4))[i];
            _mtw_TouchButton_getPosition(buf, b);
            posX[i] = (int)*(float *)(buf + 0);
            b = ((void **)i32(arr, 4))[i];
            _mtw_TouchButton_getPosition(buf, b);
            posY[i] = (int)*(float *)(buf + 4);
        }
    }
    *(uint32_t *)*(void **)gAddBtnCount = *(uint32_t *)arr;
}
