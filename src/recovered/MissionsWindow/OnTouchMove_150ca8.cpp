#include "class.h"

extern "C" int  _mw_m_tail(void *obj, int p1, int p2);          // 0x1ac528 tail
extern "C" void _mw_m_Choice(void *w, int p1, int p2);          // 0x7474c
extern "C" void _mw_m_StarMap(void *w, int p1, int p2);         // 0x750e8
extern "C" int  _mw_m_wantedBoardAccessible(void *st);          // 0x77290
extern "C" void _mw_m_Button(void *btn, int p1, int p2);        // 0x74764
extern "C" void _mw_m_Layout(void *l, int p1, int p2);          // 0x74758
extern "C" void _mw_m_STW(void *w, int p1, int p2);             // 0x75bbc

__attribute__((visibility("hidden"))) extern void **g_mw_m_status;
__attribute__((visibility("hidden"))) extern void **g_mw_m_layout;

struct ArrHdr { unsigned len; void **data; };

// MissionsWindow::OnTouchMove(int, int)
struct MissionsWindow { int OnTouchMove(int, int); };
int MissionsWindow::OnTouchMove(int p1, int p2)
{
    if (i32(this, 0x40) == 1)
        return _mw_m_tail(pp(this, 0x10), p1, p2);
    if (u8(this, 0x20)) {
        _mw_m_Choice(pp(this, 0xc), p1, p2);
    } else if (u8(this, 0x22)) {
        _mw_m_StarMap(pp(this, 0x8), p1, p2);
    } else {
        if (_mw_m_wantedBoardAccessible(*g_mw_m_status)) {
            for (unsigned i = 0;; i++) {
                ArrHdr *arr = (ArrHdr *)pp(this, 0x14);
                if (i >= arr->len) break;
                _mw_m_Button(arr->data[i], p1, p2);
            }
        }
        _mw_m_Layout(*g_mw_m_layout, p1, p2);
        _mw_m_STW(pp(this, 0x0), p1, p2);
        _mw_m_STW(pp(this, 0x4), p1, p2);
        if (pp(this, 0x24)) _mw_m_Button(pp(this, 0x24), p1, p2);
        if (pp(this, 0x2c)) _mw_m_Button(pp(this, 0x2c), p1, p2);
        if (pp(this, 0x28)) _mw_m_Button(pp(this, 0x28), p1, p2);
    }
    return 0;
}
