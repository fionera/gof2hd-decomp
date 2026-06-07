#include "class.h"

// Base sub-window at +0x18; forward the release first.
extern "C" void _liw_stw_OnTouchEnd(void *stw, int y);

// ListItemWindow::OnTouchEnd(int, int)
struct ListItemWindow { void OnTouchEnd(int, int); };
void ListItemWindow::OnTouchEnd(int x, int y)
{
    _liw_stw_OnTouchEnd(pp(this, 0x18), x);
    if (u8(this, 0x54) && u8(this, 0x138)) {
        int dv = i32(this, 0x128);
        int sum = i32(this, 0x11c) + dv;
        float vel = (float)dv;
        int a = dv < 0 ? -dv : dv;
        float v = 0.0f;
        if (a > 3) v = vel;
        f32(this, 0x12c) = 0.9f;
        u8(this, 0x138) = 0;
        i32(this, 0x11c) = sum;
        i32(this, 0x124) = sum;
        f32(this, 0x130) = v;
    }
}
