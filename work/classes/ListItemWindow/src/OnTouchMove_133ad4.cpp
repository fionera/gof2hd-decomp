#include "class.h"

// Base sub-window at +0x18 (a ScrollTouchWindow*); forward the touch first.
extern "C" void _liw_stw_OnTouchMove(void *stw, int y);

// ListItemWindow::OnTouchMove(int, int)
struct ListItemWindow { void OnTouchMove(int, int); };
void ListItemWindow::OnTouchMove(int x, int y)
{
    _liw_stw_OnTouchMove(pp(this, 0x18), x);
    if (u8(this, 0x54) && u8(this, 0x138)) {
        int d = x - i32(this, 0x120);
        i32(this, 0x128) = d;
        u32(this, 0x12c) = 0x3f800000;
        i32(this, 0x11c) = i32(this, 0x11c) + d;
        i32(this, 0x120) = x;
    }
}
