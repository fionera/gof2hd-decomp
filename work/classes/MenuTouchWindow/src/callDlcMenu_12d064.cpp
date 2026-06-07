#include "class.h"

extern "C" void *_mtw_GetApplicationData(void *app);
extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void _mtw_DlcMenu_call(void *win, void *s1, void *s2);
extern void *const gAppHolder __attribute__((visibility("hidden")));
extern void *const gDlcGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::callDlcMenu()
struct MenuTouchWindow { void callDlcMenu(); };
void MenuTouchWindow::callDlcMenu()
{
    void *holder = gAppHolder;
    void *ad = _mtw_GetApplicationData(*(void **)holder);
    u8(ad, 0x4c) = 0;
    ad = _mtw_GetApplicationData(*(void **)holder);
    void *gt = gDlcGameText;
    u8(ad, 0x3d) = 1;
    void *win = pp(this, 0x104);
    u8(this, 0x170) = 1;
    u8(this, 0x17a) = 1;
    void *s1 = _mtw_GameText_getText(*(void **)gt, 0x47);
    void *s2 = _mtw_GameText_getText(*(void **)gt, 0x1a9);
    return _mtw_DlcMenu_call(win, s1, s2);
}
