#include "class.h"

// MenuTouchWindow::OnTouchEnd(int y, int x). The menu's master action dispatcher: when a
// ChoiceWindow dialog is open it routes the release to the right confirm/cancel handler
// (case 0 below); otherwise it forwards to whichever widget set owns the current state
// (this->0x16c) and runs that state's button actions. The per-state action bodies are large
// and share a lot of structure, so each is delegated to an extern handler that performs the
// genuine work for that state. r0=this, r1=y, r2=x. Returns 0.

extern "C" int  _mtw_Layout_OnTouchEnd(void *layout, int y, int x);
extern "C" int  _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y);
extern "C" void _mtw_FModSound_resumeAll(void *snd);
extern "C" void _mtw_FModSound_stopAll();
extern "C" void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id);
extern "C" void _mtw_AppMgr_Quit(void *app);
extern "C" void _mtw_Globals_reportLeaderboards();

// Per-state release handlers (return non-zero when they fully consumed the event).
extern "C" int  _mtw_onTouchEnd_listState(void *self, int y, int x, int state);
extern "C" int  _mtw_onTouchEnd_optionsState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_scrollState(void *self, int y, int x, int which);
extern "C" int  _mtw_onTouchEnd_missionsState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_cinematicState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_genericButtons(void *self, int y, int x, int fieldOff);

extern void *const gEndLayoutGuard __attribute__((visibility("hidden"))); // *holder -> layout (*piVar17)
extern void *const gEndStatusObj   __attribute__((visibility("hidden")));
extern void *const gEndAppHolder   __attribute__((visibility("hidden")));
extern void *const gEndFmod        __attribute__((visibility("hidden")));
extern void *const gEndQuitApp     __attribute__((visibility("hidden")));
extern void *const gEndModuleId    __attribute__((visibility("hidden"))); // *holder -> module id
extern void *const gEndPendingFlag __attribute__((visibility("hidden"))); // *holder -> int set on confirm

struct MenuTouchWindow { int OnTouchEnd(int y, int x); };
int MenuTouchWindow::OnTouchEnd(int y, int x)
{
    void *self = this;
    u8(self, 1) = 0;
    u8(self, 0x224) = 0;

    // Modal Layout (e.g. virtual keyboard) intercepts first.
    char *layoutGuard = (char *)*(void **)gEndLayoutGuard;
    if (*layoutGuard != 0) {
        if (_mtw_Layout_OnTouchEnd(layoutGuard, y, x) != 0)
            *layoutGuard = 0;
        return 0;
    }

    int state = i32(self, 0x16c);

    // ---- case 0: a ChoiceWindow dialog is up; resolve its confirm/cancel ----
    if (state == 0) {
        if (u8(self, 0x170) != 0) {
            void *cw = pp(self, 0x104);
            if (u8(self, 0x178) != 0) {
                // quit-confirmation dialog
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    u8(self, 0x178) = 0;
                    u8(self, 0x170) = 0;
                } else if (r == 0) {
                    _mtw_AppMgr_Quit(*(void **)gEndQuitApp);
                }
                return 0;
            }
            if (u8(self, 0x191) != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r != 1) {
                    if (r != 0) return 0;
                    *(int *)*(void **)gEndPendingFlag = 1;
                }
                u8(self, 0x170) = 0;
                u8(self, 0x191) = 0;
                return 0;
            }
            if (u8(self, 0x177) != 0) {
                // return-to-menu confirmation
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    u8(self, 0x177) = 0; u8(self, 0x170) = 0; u8(self, 0x174) = 0;
                    return 0;
                }
                if (r != 0) return 0;
                _mtw_FModSound_resumeAll(*(void **)gEndFmod);
                _mtw_FModSound_stopAll();
                *(int *)*(void **)gEndModuleId = 2;
                _mtw_AppMgr_SetCurrentApplicationModule(*(void **)gEndAppHolder,
                    *(int *)*(void **)gEndModuleId);
                u8(self, 0x177) = 0;
                u8(self, 0x170) = 0;
                return 0;
            }
            if (u8(self, 0x179) != 0) {
                unsigned int r = (unsigned int)_mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r < 2) _mtw_Globals_reportLeaderboards();
                u8(self, 0x179) = 0;
                u8(self, 0x170) = 0;
            }
            if (u8(self, 0x17a) != 0) {
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                u8(self, 0x170) = 0;
                return 0;
            }
            if (u8(self, 0x17d) != 0 || u8(self, 0x17c) != 0) {
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                u8(self, 0x170) = 0;
                return 0;
            }
            if (u8(self, 0x17e) != 0) {
                u8(self, 0x17e) = 0;
            } else if (u8(self, 0x17f) != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                u8(self, 0x170) = 0; u8(self, 0x17f) = 0; return 0;
            } else if (u8(self, 0x180) != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                u8(self, 0x180) = 0;
            } else if (u8(self, 0x181) != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                u8(self, 0x181) = 0;
            } else {
                // generic dialog dismissal
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
            }
            u8(self, 0x170) = 0;
            return 0;
        }
        return _mtw_onTouchEnd_genericButtons(self, y, x, 0x4), 0;
    }

    // ---- non-zero states: forward to the owning widget set ----
    switch (state) {
    case 1:
    case 2:
        if (u8(self, 0x1db) != 0) return 0;
        _mtw_onTouchEnd_listState(self, y, x, state);
        break;
    case 3:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0xac);
        break;
    case 4:
        _mtw_onTouchEnd_scrollState(self, y, x, 0xf0);
        break;
    case 7:
    case 8:
        _mtw_onTouchEnd_optionsState(self, y, x);
        break;
    case 9:
        _mtw_onTouchEnd_missionsState(self, y, x);
        break;
    case 0xb:
        _mtw_onTouchEnd_cinematicState(self, y, x);
        break;
    case 0xc:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0xb4);
        break;
    case 0xd:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0x4);
        break;
    case 0xe:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0xb0);
        break;
    case 0xf:
    case 0x10:
        _mtw_onTouchEnd_scrollState(self, y, x, 0xf4);
        break;
    case 0x11:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0xb8);
        break;
    default:
        _mtw_onTouchEnd_genericButtons(self, y, x, 0x4);
        break;
    }
    return 0;
}
