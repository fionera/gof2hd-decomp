#include "class.h"

// MenuTouchWindow::update(int dt). Applies scroll inertia (state 0/3 lists), reacts to the
// async DLC purchase/restore result codes carried on the ApplicationData object, drives the
// active sub-window's update, and refreshes the ChoiceWindow. r0=this, r1=dt.

extern "C" void *_mtw_AppMgr_GetApplicationData();
extern "C" void *_mtw_AppMgr_GetEngine();
extern "C" int  _mtw_PaintCanvas_GetImage2DHeight(void *pc);
extern "C" void _mtw_RecordHandler_saveOptions(void *rh);
extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void _mtw_ChoiceWindow_set(void *cw, void *s);
extern "C" void _mtw_ChoiceWindow_update(void *cw);
extern "C" void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);
extern "C" void _mtw_TouchButton_setYPosition(void *btn, int y);
extern "C" int  _mtw_TouchButton_isVisible(void *btn);
extern "C" void _mtw_TouchButton_getPosition(void *out, void *btn);
extern "C" void _mtw_ScrollTouchWindow_update(void *w);
extern "C" void _mtw_MissionsWindow_update(void *w);

// PC-relative holders (each *holder -> the relevant singleton/state object).
extern void *const gUpStatusGuard __attribute__((visibility("hidden")));
extern void *const gUpLayout      __attribute__((visibility("hidden")));
extern void *const gUpScreenW     __attribute__((visibility("hidden")));
extern void *const gUpCanvas      __attribute__((visibility("hidden")));
extern void *const gUpOptObj      __attribute__((visibility("hidden")));
extern void *const gUpRecHandler  __attribute__((visibility("hidden")));
extern void *const gUpGameText    __attribute__((visibility("hidden")));
extern void *const gUpStatusObj   __attribute__((visibility("hidden")));
extern void *const gUpDlcFlags    __attribute__((visibility("hidden")));
extern void *const gUpScreenH     __attribute__((visibility("hidden")));
extern void *const gUpListLayout  __attribute__((visibility("hidden")));

// helper: pull a GameText id from one of the PC-relative literal pools.
extern const int g_mtw_upTextIds[16] __attribute__((visibility("hidden")));

struct MenuTouchWindow { void update(int dt); };
void MenuTouchWindow::update(int dt)
{
    void *self = this;

    // ---- scroll inertia for list states (0x16c in {1,2,15}) ----
    unsigned int st = u32(self, 0x16c);
    if (st < 0x10 && ((1u << (st & 0xff)) & 0x8006u) != 0) {
        if (u8(self, 0x224) == 0) {
            float v = f32(self, 0x218) * f32(self, 0x21c);
            f32(self, 0x21c) = v;
            if (v < 1.0f) {
                f32(self, 0x194) = (float)(v + (float)i32(self, 0x194)); // accumulate offset
                i32(self, 0x194) = (int)(v + (float)i32(self, 0x194));
            }
        }
        int off = i32(self, 0x194);
        if (off > 0) {
            u32(self, 0x218) = 0x3f800000;
            f32(self, 0x21c) = (float)(-off) * 0.5f;
        }
        int overshoot = i32(self, 0x228) - i32(self, 0x22c);
        if (overshoot < 0) {
            if (off < overshoot) {
                u32(self, 0x218) = 0x3f800000;
                f32(self, 0x21c) = (float)(overshoot - off) * 0.5f;
            }
        } else {
            f32(self, 0x21c) = 0;
            i32(self, 0x194) = 0;
        }
    }

    void *appData = _mtw_AppMgr_GetApplicationData();
    unsigned char busy = u8(self, 0x17a);

    if (busy != 0 || u8(self, 0x190) != 0) {
        // a purchase flow is active: maybe rebuild list geometry
        if (*(char *)((char *)appData + 0x40) != 0) {
            int *layout = (int *)*(void **)gUpLayout;
            void *canvas = *(void **)gUpCanvas;
            i32(self, 0x228) = (((*(int *)*(void **)gUpScreenW - layout[4]) - layout[3])
                                - layout[8]) - layout[9];
            int ih = _mtw_PaintCanvas_GetImage2DHeight(canvas);
            i32(self, 0x1e4) = ih;
            void *optObj = *(void **)gUpOptObj;
            int rowH = layout[0xb]; // +0x2c
            u32(self, 0x16c) = 0xf;
            u8(self, 0x170) = 0;
            u8(self, 0x17a) = 0;
            i32(self, 0x22c) = (ih + rowH) * 5;
            *(char *)((char *)appData + 0x40) = 0;
            *(char *)((char *)optObj + 0x3b) = 1;
            _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
            busy = u8(self, 0x17a);
        }
        if (busy != 0) {
            if (*(char *)((char *)appData + 0x42) != 0) {
                void *cw = pp(self, 0x104);
                void *s = _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[0]);
                _mtw_ChoiceWindow_set(cw, s);
                u8(self, 0x17d) = 1;
                u8(self, 0x170) = 1;
                *(unsigned short *)((char *)self + 0x17a) = 0;
                *(char *)((char *)appData + 0x42) = 0;
                u8(self, 0x190) = 0;
            }
        }
    }

    // ---- DLC purchase result dispatch (appData+0x48 code, gated by +0x41) ----
    unsigned int code = *(unsigned int *)((char *)appData + 0x48);
    bool handled = false;
    if (code <= 4 && *(char *)((char *)appData + 0x41) != 0) {
        void *cw = pp(self, 0x104);
        switch (code) {
            case 0: {
                void *status = *(void **)gUpStatusObj;
                *(char *)(*(char **)gUpStatusObj + 0x35) = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[1]));
            } break;
            case 1: {
                *(int *)(*(char **)gUpStatusObj + 0x114) = 3;
                *(char *)(*(char **)gUpOptObj + 0x36) = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[2]));
            } break;
            case 2: {
                void *status = *(void **)gUpStatusObj;
                *(char *)(*(char **)gUpStatusObj + 0x37) = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[3]));
            } break;
            case 3: {
                *(char *)(*(char **)gUpOptObj + 0x38) = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[4]));
            } break;
            case 4: {
                char *flags = *(char **)gUpOptObj;
                void *status = *(void **)gUpStatusObj;
                flags[0x35] = 1; flags[0x39] = 1; flags[0x37] = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[5]));
            } break;
        }
        _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        u8(self, 0x17c) = 1;
        u8(self, 0x17a) = 0;
        u8(self, 0x170) = 1;
        *(char *)((char *)appData + 0x41) = 0;
        handled = true;
    } else if (u8(self, 0x190) != 0 && *(char *)((char *)appData + 0x41) != 0) {
        _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        if (u8(self, 0x190) != 0) {
            void *cw = pp(self, 0x104);
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[6]));
            u8(self, 0x190) = 0;
        }
        u8(self, 0x17c) = 1;
        u8(self, 0x17a) = 0;
        u8(self, 0x170) = 1;
        *(char *)((char *)appData + 0x41) = 0;
        handled = true;
    }
    (void)handled;

    // supernova nag dialog
    if (*(char *)*(void **)gUpStatusGuard != 0) {
        void *flags = *(void **)gUpDlcFlags;
        if (u8(self, 0x170) == 0 && *(char *)((char *)flags + 0x35) == 0) {
            void *cw = pp(self, 0x104);
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[7]));
            u8(self, 0x181) = 1;
            u8(self, 0x170) = 1;
            *(char *)((char *)flags + 0x35) = 1;
            _mtw_Status_setSystemVisibility(*(void **)gUpStatusObj, 0x19, true);
            _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        }
    }

    // ---- per-state button layout / sub-window update ----
    int state = i32(self, 0x16c);
    if (state == 0) {
        if (i32(self, 0x168) == 1) {
            int rowGap = i32(self, 0x1b0);
            int *layout = (int *)*(void **)gUpListLayout;
            void **arr = (void **)pp(self, 0x4);
            unsigned int n = *(unsigned int *)arr;
            int total = n * layout[0xc]; // +0x30
            int screenH = *(int *)*(void **)gUpScreenH;
            for (unsigned int i = 0; i < n; i++) {
                _mtw_TouchButton_setYPosition(((void **)arr[1])[i],
                    (layout[0xc] + i32(self, 0x1b0)) * i +
                    (screenH / 2 - (int)((unsigned int)(rowGap * (n - 1) + total) >> 1)));
                void *b = ((void **)i32(pp(self, 0x4), 4))[i];
                if (i32(b, 0) == 0x12 && i32(b, 4) == 0 && _mtw_TouchButton_isVisible(b) != 0 &&
                    u8(self, 0x238) != 0) {
                    b = ((void **)i32(pp(self, 0x4), 4))[i];
                    char pos[12];
                    _mtw_TouchButton_getPosition(pos, b);
                    _mtw_TouchButton_setYPosition(b, (int)*(float *)(pos + 4));
                }
                arr = (void **)pp(self, 0x4);
                n = *(unsigned int *)arr;
            }
        }
    } else if (state == 3) {
        int rowGap = i32(self, 0x1b0);
        int *layout = (int *)*(void **)gUpListLayout;
        void **arr = (void **)pp(self, 0xac);
        unsigned int n = *(unsigned int *)arr;
        int firstCount = *(int *)*(void **)pp(self, 0x4);
        int total = n * layout[0xc];
        int screenH = *(int *)*(void **)gUpScreenH;
        for (unsigned int i = 0; i < n; i++) {
            _mtw_TouchButton_setYPosition(((void **)arr[1])[i],
                (layout[0xc] + i32(self, 0x1b0)) * i +
                (screenH / 2 - (int)((unsigned int)((firstCount - 1) * rowGap + total) >> 1)));
            arr = (void **)pp(self, 0xac);
            n = *(unsigned int *)arr;
        }
    } else if (state == 4) {
        _mtw_ScrollTouchWindow_update(pp(self, 0xf0));
    } else if (state == 9) {
        if (pp(self, 0xfc) != 0) _mtw_MissionsWindow_update(pp(self, 0xfc));
    } else if (state == 0xf) {
        _mtw_ScrollTouchWindow_update(pp(self, 0xf4));
    }

    if (u8(self, 0x170) != 0)
        _mtw_ChoiceWindow_update(pp(self, 0x104));

    // ---- async store-init result (state 0xd) ----
    if (state == 0xd) {
        appData = _mtw_AppMgr_GetApplicationData();
        if (*(char *)((char *)appData + 0xc) != 0) {
            void *eng = _mtw_AppMgr_GetEngine();
            int r = *(int *)((char *)eng + 0x100);
            if (r == 2 || r == 1) {
                void *cw = pp(self, 0x104);
                int id = (r == 2) ? g_mtw_upTextIds[8] : g_mtw_upTextIds[9];
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, id));
                u8(self, 0x189) = 0;
                u8(self, 0x170) = 1;
                appData = _mtw_AppMgr_GetApplicationData();
                *(char *)((char *)appData + 0xc) = 0;
            }
        }
    }

    // ---- async restore result ----
    appData = _mtw_AppMgr_GetApplicationData();
    if (*(char *)((char *)appData + 5) != 0) {
        appData = _mtw_AppMgr_GetApplicationData();
        int r = *(int *)((char *)appData + 8);
        if (r == 2 || r == 1) {
            void *cw = pp(self, 0x104);
            int id = (r == 2) ? g_mtw_upTextIds[10] : g_mtw_upTextIds[11];
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, id));
            i32(self, 0x184) = -1;
            u8(self, 0x170) = 1;
            appData = _mtw_AppMgr_GetApplicationData();
            *(char *)((char *)appData + 5) = 0;
        }
    }
}
