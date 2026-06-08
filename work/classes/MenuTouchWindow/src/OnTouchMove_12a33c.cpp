#include "class.h"

// MenuTouchWindow::OnTouchMove(int y, int x). Dispatches the drag to whatever sub-widget owns
// the current menu state (this->0x16c). r0=this, r1=y, r2(=param_3)=x. Returns 0.

extern "C" void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y);
extern "C" void _mtw_TouchButton_OnTouchMove(void *btn, int y);
extern "C" void _mtw_TouchButton_OnTouchMoveXY(void *btn, int y, int x);
extern "C" void _mtw_TouchButton_OnTouchBegin(void *btn, int y);
extern "C" void _mtw_TouchSlider_OnTouchMove(void *sl, int y);
extern "C" float _mtw_TouchSlider_getValue(void *sl);
extern "C" void _mtw_ScrollTouchWindow_OnTouchMove(void *w, int y);
extern "C" void _mtw_MissionsWindow_OnTouchMove(void *w, int y);
extern "C" int  _mtw_FModSound_tryToStopMusicForBGMusic();
extern "C" void _mtw_FModSound_setVolume(void *snd, float v);
extern "C" void _mtw_steerFromTouch(void *self, int y, int x); // corrupted case-10 cinematic steer

extern void *const gMvLayout    __attribute__((visibility("hidden"))); // *holder -> layout obj (=*piVar8)
extern void *const gMvScreenW   __attribute__((visibility("hidden")));
extern void *const gMvFlagA     __attribute__((visibility("hidden"))); // *holder -> [0] byte
extern void *const gMvFlagB     __attribute__((visibility("hidden")));
extern void *const gMvFmod      __attribute__((visibility("hidden")));

static void doSliders(void *self, int y) {
    void *fmod = *(void **)gMvFmod;
    int *sl = (int *)i32(pp(self, 0xec), 4);
    if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
        _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[1]));
    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[2]));
    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[3]));
}

struct MenuTouchWindow { int OnTouchMove(int y, int x); };
int MenuTouchWindow::OnTouchMove(int y, int x)
{
    void *self = this;
    if (u8(self, 0x170) != 0) {
        _mtw_ChoiceWindow_OnTouchMove(pp(self, 0x104), y);
        return 0;
    }
    char *layout = (char *)*(void **)gMvLayout;
    if (*layout != 0) {
        // corrupted-state guard path -> in-flight 3D steer
        _mtw_steerFromTouch(self, y, x);
        return 0;
    }

    int state = i32(self, 0x16c);
    switch (state - 1) {
    case 0: // state 1
    case 1: // state 2 list-drag (scroll)
        if (*(int *)(layout + 0xc) < x &&
            x < *(int *)*(void **)gMvScreenW - *(int *)(layout + 0x10)) {
            int dy = x - i32(self, 0x20c);
            pp(self, 0x20c) = (void *)(long)x;
            i32(self, 0x214) = dy;
            u32(self, 0x218) = 0x3f800000;
            i32(self, 0x194) = i32(self, 0x194) + dy;
        }
        _mtw_TouchButton_OnTouchMove(pp(self, 0xc4), y);
        break;
    case 2: { // state 3
        if (*(char *)*(void **)gMvFlagA == 0) {
            void **arr = (void **)pp(self, 0xac);
            for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **)arr[1])[i], y);
        } else {
            _mtw_TouchButton_OnTouchMove(pp(self, 0xe4), y);
            *(unsigned short *)((char *)self + 0x108) = 0;
            int base = *(int *)(layout + 0x28);
            int top = base + i32(self, 0x19c);
            int bottom = i32(self, 0x158) + top;
            if (top < y && y < bottom &&
                *(int *)(layout + 0xc) + base < x &&
                x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + i32(self, 0x154))
                u8(self, 0x108) = 1;
            if (bottom < y && y < (i32(self, 0x19c) - base) + i32(self, 0x1a0) &&
                base + *(int *)(layout + 0xc) < x &&
                x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + i32(self, 0x154))
                u8(self, 0x109) = 1;
            _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xcc), y, x);
            _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xd0), y, x);
            _mtw_TouchSlider_OnTouchMove(*(void **)i32(pp(self, 0xec), 4), y);
            _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xd4), y, x);
            _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xd8), y, x);
            _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xdc), y, x);
            doSliders(self, y);
            void **arr = (void **)pp(self, 0xec);
            for (unsigned int i = 1; i < *(unsigned int *)arr; i++)
                _mtw_TouchSlider_OnTouchMove(((void **)arr[1])[i], y);
            if (*(char *)*(void **)gMvFlagB != 0 && pp(self, 0xe8) != 0)
                _mtw_TouchButton_OnTouchMove(pp(self, 0xc4), y);
        }
    } break;
    case 3: { // state 4
        _mtw_ScrollTouchWindow_OnTouchMove(pp(self, 0xf0), y);
        void **arr = (void **)pp(self, 0xc0);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            unsigned int t = e[0];
            if (((unsigned int)(t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                || (e[1] == 0 && t == 0x16))
                _mtw_TouchButton_OnTouchMove(e, y);
        }
    } break;
    case 5: // state 6
        _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xd4), y, x);
        _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xd8), y, x);
        _mtw_TouchButton_OnTouchMoveXY(pp(self, 0xdc), y, x);
        doSliders(self, y);
        {
            void **arr = (void **)pp(self, 0xec);
            for (unsigned int i = 1; i < *(unsigned int *)arr; i++) {
                if (i == 5 && (unsigned char)*(char *)(*(char **)gMvLayout + 0x284) == 0) continue;
                _mtw_TouchSlider_OnTouchMove(((void **)arr[1])[i], y);
            }
        }
        break;
    case 6: { // state 7
        *(unsigned short *)((char *)self + 0x108) = 0;
        int base = *(int *)(layout + 0x28);
        int top = base + i32(self, 0x19c);
        int bottom = i32(self, 0x158) + top;
        if (top < y && y < bottom &&
            *(int *)(layout + 0xc) + base < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + i32(self, 0x154))
            u8(self, 0x108) = 1;
        if (bottom < y && y < (i32(self, 0x19c) - base) + i32(self, 0x1a0) &&
            base + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + i32(self, 0x154))
            u8(self, 0x109) = 1;
        _mtw_TouchButton_OnTouchMove(pp(self, 0xcc), y);
        _mtw_TouchButton_OnTouchMove(pp(self, 0xd0), y);
        _mtw_TouchSlider_OnTouchMove(*(void **)i32(pp(self, 0xec), 4), y);
    } break;
    case 7: // state 8
        _mtw_MissionsWindow_OnTouchMove(pp(self, 0xfc), y);
        break;
    case 4:   // state 5
    case 0xb: // state 12
        break;
    default: { // state 10 / 0xf and others -> top-level button + 0xc0 array
        void **arr = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchMove(((void **)arr[1])[i], y);
        void **arr2 = (void **)pp(self, 0xc0);
        unsigned int n = *(unsigned int *)arr2;
        for (unsigned int i = 0; i < n; i++) {
            int *e = (int *)((void **)arr2[1])[i];
            if ((unsigned int)(e[0] - 0x17) < 2)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
        for (unsigned int i = 0; i < n; i++) {
            unsigned int *e = (unsigned int *)((void **)arr2[1])[i];
            unsigned int t = e[0], id = e[1];
            bool hit;
            if (t == 5 && id == 0) hit = true;
            else {
                unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                hit = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
            }
            if (hit) {
                _mtw_TouchButton_OnTouchMove(e, y);
                arr2 = (void **)pp(self, 0xc0);
                n = *(unsigned int *)arr2;
            }
        }
        if (state == 10)
            _mtw_steerFromTouch(self, y, x);
    } break;
    }
    return 0;
}
