#include "class.h"

// MenuTouchWindow::OnTouchBegin(int y, int x, int touchId). Routes the press to whichever
// sub-widget owns the current menu state (this->0x16c), kicking off scroll drags, slider
// grabs, list selection and the in-flight 3D steer. r0=this, r1=y, r2=x, r3=touchId. Returns 0.

extern "C" void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y);
extern "C" int  _mtw_Layout_OnTouchBegin(void *layout, int y);
extern "C" void _mtw_TouchButton_OnTouchBegin(void *btn, int y);
extern "C" void _mtw_TouchButton_OnTouchBeginXY(void *btn, int y, int x);
extern "C" float _mtw_TouchButton_setPosition(void *btn, int x, int y);
extern "C" void _mtw_TouchSlider_OnTouchBegin(void *sl, int y);
extern "C" void _mtw_ScrollTouchWindow_OnTouchBegin(void *w, int y);
extern "C" void _mtw_MissionsWindow_OnTouchBegin(void *w, int y);
extern "C" void _mtw_FModSound_play(void *snd, int id, void *pos, float v);
extern "C" int  _mtw_idiv(int a, int b);
extern "C" int  _mtw_PaintCanvas_GetImage2DWidth(void *pc);
extern "C" int  _mtw_PaintCanvas_GetImage2DHeight(void *pc);

extern void *const gBgLayout   __attribute__((visibility("hidden"))); // *holder -> layout (=*piVar5)
extern void *const gBgScreenW  __attribute__((visibility("hidden")));
extern void *const gBgScrollDiv __attribute__((visibility("hidden")));
extern void *const gBgListPosX __attribute__((visibility("hidden")));
extern void *const gBgFmod     __attribute__((visibility("hidden")));
extern void *const gBgFlagA    __attribute__((visibility("hidden")));
extern void *const gBgFlagB    __attribute__((visibility("hidden")));
extern void *const gBgFlagC    __attribute__((visibility("hidden")));
extern void *const gBgFlagD    __attribute__((visibility("hidden")));
extern void *const gBgObjA     __attribute__((visibility("hidden")));
extern void *const gBgObjB     __attribute__((visibility("hidden")));
extern void *const gBgScreenH2 __attribute__((visibility("hidden")));
extern void *const gBgScrollImg __attribute__((visibility("hidden")));
extern void *const gBgScreenW2 __attribute__((visibility("hidden")));

struct MenuTouchWindow { int OnTouchBegin(int y, int x, int touchId); };
int MenuTouchWindow::OnTouchBegin(int y, int x, int touchId)
{
    void *self = this;
    if (u8(self, 0x170) != 0) { _mtw_ChoiceWindow_OnTouchBegin(pp(self, 0x104), y); return 0; }

    char *layout = (char *)*(void **)gBgLayout;
    if (*layout != 0) { _mtw_Layout_OnTouchBegin(layout, y); return 0; }

    int state = i32(self, 0x16c);
    switch (state) {
    case 1:
    case 2: {
        pp(self, 0x220) = (void *)(long)x;
        pp(self, 0x20c) = (void *)(long)x;
        i32(self, 0x214) = 0;
        u8(self, 0x224) = 1;
        int oldRow = i32(self, 0x18c);
        int leftMargin = *(int *)(layout + 0xc);
        if (leftMargin < x && x < *(int *)*(void **)gBgScreenW - *(int *)(layout + 0x10)) {
            int rowH = *(int *)(layout + 0x70);
            int gap = i32(self, 0x1b4);
            int top = *(int *)(layout + 0x20);
            int off = i32(self, 0x194);
            int row = _mtw_idiv(x + ((-top - leftMargin) - off), 1);
            if (row < *(int *)*(void **)gBgScrollDiv) {
                int *posX = (int *)*(void **)gBgListPosX;
                i32(self, 0x18c) = row;
                float v = _mtw_TouchButton_setPosition(pp(self, 0xc4),
                    (*posX - i32(self, 0x198)) - *(int *)(layout + 0x28),
                    row * (gap + rowH) + top + leftMargin + off + *(int *)(layout + 0x108));
                _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, v);
            }
        }
        if (oldRow == i32(self, 0x18c))
            _mtw_TouchButton_OnTouchBegin(pp(self, 0xc4), y);
    } break;
    case 3: {
        if (*(char *)*(void **)gBgFlagA == 0) {
            void **arr = (void **)pp(self, 0xac);
            for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
            break;
        }
        _mtw_TouchButton_OnTouchBegin(pp(self, 0xe4), y);
        *(unsigned short *)((char *)self + 0x108) = 0;
        int b28 = *(int *)(layout + 0x28);
        int top = b28 + i32(self, 0x19c);
        if (top < y && y < i32(self, 0x158) + top &&
            *(int *)(layout + 0xc) + b28 < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + i32(self, 0x154)) {
            u8(self, 0x108) = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
            b28 = *(int *)(layout + 0x28);
            top = b28 + i32(self, 0x19c);
        }
        int bottom = top + i32(self, 0x158);
        if (bottom < y && y < (i32(self, 0x19c) - b28) + i32(self, 0x1a0) &&
            b28 + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + i32(self, 0x154)) {
            u8(self, 0x109) = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
        }
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xcc), y, x);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xd0), y, x);
        _mtw_TouchSlider_OnTouchBegin(*(void **)i32(pp(self, 0xec), 4), y);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xd4), y, x);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xd8), y, x);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xdc), y, x);
        void **arr = (void **)pp(self, 0xec);
        for (unsigned int i = 1; i < *(unsigned int *)arr; i++)
            _mtw_TouchSlider_OnTouchBegin(((void **)arr[1])[i], y);
        if (*(char *)*(void **)gBgFlagB != 0 && pp(self, 0xe8) != 0)
            _mtw_TouchButton_OnTouchBegin(pp(self, 0xe8), y);
    } break;
    case 4: {
        _mtw_ScrollTouchWindow_OnTouchBegin(pp(self, 0xf0), y);
        void **arr = (void **)pp(self, 0xc0);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            unsigned int t = e[0];
            if (((unsigned int)(t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                || (e[1] == 0 && t == 0x16))
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
    } break;
    case 6:
    case 0xd:
        break;
    case 7: {
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xd4), y, x);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xd8), y, x);
        _mtw_TouchButton_OnTouchBeginXY(pp(self, 0xdc), y, x);
        void **arr = (void **)pp(self, 0xec);
        unsigned int n = *(unsigned int *)arr;
        for (unsigned int i = 1; i < n; i++) {
            if (i == 5 && (unsigned char)*(char *)(*(char **)gBgLayout + 0x284) == 0) continue;
            _mtw_TouchSlider_OnTouchBegin(((void **)arr[1])[i], y);
        }
    } break;
    case 8: {
        *(unsigned short *)((char *)self + 0x108) = 0;
        int b28 = *(int *)(layout + 0x28);
        int top = b28 + i32(self, 0x19c);
        if (top < y && y < i32(self, 0x158) + top &&
            *(int *)(layout + 0xc) + b28 < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + i32(self, 0x154)) {
            u8(self, 0x108) = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
            layout = (char *)*(void **)gBgLayout;
            b28 = *(int *)(layout + 0x28);
            top = b28 + i32(self, 0x19c);
        }
        int bottom = top + i32(self, 0x158);
        if (bottom < y && y < (i32(self, 0x19c) - b28) + i32(self, 0x1a0) &&
            b28 + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + i32(self, 0x154)) {
            u8(self, 0x109) = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
        }
        _mtw_TouchButton_OnTouchBegin(pp(self, 0xcc), y);
        _mtw_TouchButton_OnTouchBegin(pp(self, 0xd0), y);
        _mtw_TouchSlider_OnTouchBegin(*(void **)i32(pp(self, 0xec), 4), y);
    } break;
    case 9:
        _mtw_MissionsWindow_OnTouchBegin(pp(self, 0xfc), y);
        break;
    case 0xb: {
        if (*(char *)*(void **)gBgFlagC != 0 && *(char *)*(void **)gBgFlagD == 0) {
            _mtw_TouchButton_OnTouchBegin(pp(self, 0x14), y);
            _mtw_TouchButton_OnTouchBegin(pp(self, 0x18), y);
            if (touchId != 0 &&
                (i32(self, 8) != 0 || y > 0xd1 || i32(self, 0xc) == touchId ||
                 x <= *(int *)(*(char **)gBgObjA + 0x54) - 0x14 ||
                 *(int *)(*(char **)gBgObjA + 0x54) + 300 <= x)) {
                if (i32(self, 0xc) == touchId || touchId == 0 ||
                    i32(self, 0xc) != 0 || y <= *(int *)*(void **)gBgScreenH2 - 0xdc ||
                    x <= *(int *)(*(char **)gBgObjB + 0x58) - 0x14 ||
                    *(int *)(*(char **)gBgObjB + 0x58) + 0xe6 <= x) {
                    *(unsigned short *)((char *)self + 0x98) = 0;
                } else {
                    i32(self, 0xc) = touchId;
                    *(unsigned short *)((char *)self + 0x98) = 0x100;
                    pp(self, 0x94) = (void *)(long)x;
                }
            } else {
                i32(self, 8) = touchId;
                *(unsigned short *)((char *)self + 0x98) = 1;
                pp(self, 0x90) = (void *)(long)x;
            }
        }
    } break;
    case 0xc: {
        void **arr = (void **)pp(self, 0xb4);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    case 0xe: {
        void **arr = (void **)pp(self, 0xb0);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    case 0x10: {
        void **arr = (void **)pp(self, 0xc0);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            if ((unsigned int)(e[0] - 0x65) < 5 && e[1] == 0)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
    } // fallthrough into 0xf
    [[fallthrough]];
    case 0xf: {
        _mtw_ScrollTouchWindow_OnTouchBegin(pp(self, 0xf4), y);
        void **arr = (void **)pp(self, 0xc0);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            if ((e[0] | 8) == 0x3c && e[1] == 0)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
        void **slots = (void **)pp(self, 0xf8);
        for (unsigned int i = 0; i < *(unsigned int *)slots; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)slots[1])[i], y);

        void *img = *(void **)gBgScrollImg;
        int bound = *(int *)*(void **)gBgScreenW2;
        int b28 = *(int *)(*(char **)gBgLayout + 0x28);
        int iw = _mtw_PaintCanvas_GetImage2DWidth(img);
        unsigned char hit;
        if ((bound - b28) - iw < y) {
            int lc = *(int *)(*(char **)gBgLayout + 0xc);
            int tp = *(int *)(*(char **)gBgLayout + 0x20);
            int ih = _mtw_PaintCanvas_GetImage2DHeight(img);
            hit = (x < ih + tp + lc) ? 1 : 0;
        } else hit = 0;
        u8(self, 0x1d9) = hit;

        b28 = *(int *)(*(char **)gBgLayout + 0x28);
        iw = _mtw_PaintCanvas_GetImage2DWidth(img);
        if (y < iw + b28) {
            pp(self, 0x220) = (void *)(long)x;
            pp(self, 0x20c) = (void *)(long)x;
            i32(self, 0x214) = 0;
            u8(self, 0x224) = 1;
        }
    } break;
    case 0x11: {
        void **arr = (void **)pp(self, 0xb8);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    default: {
        void **arr = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
        void **arr2 = (void **)pp(self, 0xc0);
        unsigned int n = *(unsigned int *)arr2;
        for (unsigned int i = 0; i < n; i++) {
            int *e = (int *)((void **)arr2[1])[i];
            if ((unsigned int)(e[0] - 0x17) < 2) _mtw_TouchButton_OnTouchBegin(e, y);
        }
        for (unsigned int i = 0; i < n; i++) {
            unsigned int *e = (unsigned int *)((void **)arr2[1])[i];
            unsigned int t = e[0], id = e[1];
            bool go;
            if (t == 5 && id == 0) go = true;
            else {
                unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                go = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
            }
            if (go) { _mtw_TouchButton_OnTouchBegin(e, y); arr2 = (void **)pp(self, 0xc0); n = *(unsigned int *)arr2; }
        }
    } break;
    }

    int r = _mtw_Layout_OnTouchBegin(*(void **)gBgLayout, y);
    if (r != 0 && i32(self, 0x16c) == 0xd)
        u8(self, 1) = 1;
    return 0;
}
