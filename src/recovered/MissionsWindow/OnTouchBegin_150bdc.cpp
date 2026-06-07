#include "class.h"

extern "C" int  _mw_b_tail(void *obj, int p1, int p2);          // 0x1ac518 tail
extern "C" void _mw_b_Choice(void *w, int p1, int p2);          // 0x7471c
extern "C" void _mw_b_StarMap(void *w, int p1, int p2);         // 0x74704
extern "C" int  _mw_b_wantedBoardAccessible(void *st);          // 0x77290
extern "C" void _mw_b_Button(void *btn, int p1, int p2);        // 0x74734
extern "C" void _mw_b_Layout(void *l, int p1, int p2);          // 0x74728
extern "C" void _mw_b_STW(void *w, int p1, int p2);             // 0x75ba4

__attribute__((visibility("hidden"))) extern void **g_mw_b_status;
__attribute__((visibility("hidden"))) extern void **g_mw_b_layout;

struct ArrHdr { unsigned len; void **data; };

// MissionsWindow::OnTouchBegin(int, int)
struct MissionsWindow { int OnTouchBegin(int, int); };
int MissionsWindow::OnTouchBegin(int p1, int p2)
{
    if (i32(this, 0x40) == 1)
        return _mw_b_tail(pp(this, 0x10), p1, p2);
    if (u8(this, 0x20)) {
        _mw_b_Choice(pp(this, 0xc), p1, p2);
    } else if (u8(this, 0x22)) {
        _mw_b_StarMap(pp(this, 0x8), p1, p2);
    } else {
        if (_mw_b_wantedBoardAccessible(*g_mw_b_status)) {
            for (unsigned i = 0;; i++) {
                ArrHdr *arr = (ArrHdr *)pp(this, 0x14);
                if (i >= arr->len) break;
                _mw_b_Button(arr->data[i], p1, p2);
            }
        }
        _mw_b_Layout(*g_mw_b_layout, p1, p2);
        _mw_b_STW(pp(this, 0x0), p1, p2);
        _mw_b_STW(pp(this, 0x4), p1, p2);
        if (pp(this, 0x24)) _mw_b_Button(pp(this, 0x24), p1, p2);
        if (pp(this, 0x2c)) _mw_b_Button(pp(this, 0x2c), p1, p2);
        if (pp(this, 0x28)) _mw_b_Button(pp(this, 0x28), p1, p2);
    }
    return 0;
}
