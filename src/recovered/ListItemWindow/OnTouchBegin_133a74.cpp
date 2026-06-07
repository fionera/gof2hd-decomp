#include "class.h"

// Base sub-window at +0x18.
extern "C" void _liw_stw_OnTouchBegin(void *stw, int x);
// PC-relative singleton: a pointer to a pointer to the layout/screen object.
__attribute__((visibility("hidden"))) extern int **g_liw_screen;

// ListItemWindow::OnTouchBegin(int, int)
struct ListItemWindow { void OnTouchBegin(int, int); };
void ListItemWindow::OnTouchBegin(int x, int y)
{
    _liw_stw_OnTouchBegin(pp(this, 0x18), x);
    if (u8(this, 0x54) &&
        i32(this, 0x64) + (i32(this, 0x6c) >> 1) < x) {
        int *obj = *g_liw_screen;
        if (y < i32(this, 0x68) + obj[0xc / 4] + obj[0x20 / 4] + i32(this, 0x20)) {
            i32(this, 0x120) = x;
            i32(this, 0x134) = x;
            u32(this, 0x128) = 0;
            u8(this, 0x138) = 1;
        }
    }
}
