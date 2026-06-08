#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void operator_delete(void *p);
extern "C" void _mtw_RecordHandler_ctor(void *rh);
extern "C" void *_mtw_RecordHandler_readRecord(void *rh);
extern "C" void *_mtw_RecordHandler_dtor(void *rh);
extern "C" void *_mtw_GameRecord_dtor(void *gr);
extern "C" void _mtw_GameRecord_load(void *gr);
extern "C" void _mtw_Status_resetGame();
extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void _mtw_ChoiceWindow_set(void *cw, void *s, bool b);
extern "C" void *_mtw_AppMgr_GetApplicationModule(void *app, int id);
extern "C" void _mtw_ModStation_setGameLoaded(void *ms);
extern "C" void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id);

// PC-relative singleton holders.
extern void *const gLoadStatusFlags __attribute__((visibility("hidden")));   // *holder -> flags, +0x35/+0x37
extern void *const gLoadGameText __attribute__((visibility("hidden")));      // *holder -> GameText
extern void *const gLoadAppHolder __attribute__((visibility("hidden")));     // *holder -> app
extern void *const gLoadResetCell __attribute__((visibility("hidden")));     // *holder -> int cell to clear

// MenuTouchWindow::loadGame(int slot)
struct MenuTouchWindow { int loadGame(int slot); };
int MenuTouchWindow::loadGame(int slot)
{
    void *rh = operator_new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    void *rec = _mtw_RecordHandler_readRecord(rh);

    if (rec == 0) {
        _mtw_Status_resetGame();
        void *cw = pp(this, 0x104);
        void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x64);
        _mtw_ChoiceWindow_set(cw, s, false);
        u8(this, 0x17e) = 1;
        u8(this, 0x170) = 1;
        operator_delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    void *flags = *(void **)gLoadStatusFlags;
    bool versionOk = (u8(rec, 0x117) == 0) || (*(char *)((char *)flags + 0x37) != 0);
    if (versionOk) {
        bool dlcOk = (u8(rec, 0x115) == 0) || (*(char *)((char *)flags + 0x35) != 0);
        if (dlcOk) {
            // fully compatible -> load it
            _mtw_Status_resetGame();
            _mtw_GameRecord_load(rec);
            operator_delete(_mtw_RecordHandler_dtor(rh));
            operator_delete(_mtw_GameRecord_dtor(rec));
            void *app = *(void **)gLoadAppHolder;
            void *ms = _mtw_AppMgr_GetApplicationModule(*(void **)app, 5);
            _mtw_ModStation_setGameLoaded(ms);
            *(int *)*(void **)gLoadResetCell = 0;
            _mtw_AppMgr_SetCurrentApplicationModule(*(void **)app, 5);
            return 1;
        }
        // DLC missing
        void *cw = pp(this, 0x104);
        void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x65);
        _mtw_ChoiceWindow_set(cw, s, false);
        u8(this, 0x170) = 1;
        operator_delete(_mtw_GameRecord_dtor(rec));
        operator_delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    // version mismatch
    void *cw = pp(this, 0x104);
    void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x66);
    _mtw_ChoiceWindow_set(cw, s, false);
    u8(this, 0x170) = 1;
    operator_delete(_mtw_GameRecord_dtor(rec));
    operator_delete(_mtw_RecordHandler_dtor(rh));
    return 0;
}
