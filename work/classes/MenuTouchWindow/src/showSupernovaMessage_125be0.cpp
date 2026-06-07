#include "class.h"

extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b);
// hidden PC-relative pointer-to-pointer global (the GameText singleton holder).
extern void *const gSupernovaGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::showSupernovaMessage()
struct MenuTouchWindow { void showSupernovaMessage(); };
void MenuTouchWindow::showSupernovaMessage()
{
    void *cw = pp(this, 0x104);
    void *gt = *(void **)gSupernovaGameText;
    void *s1 = _mtw_GameText_getText(gt, 0x266);
    void *s2 = _mtw_GameText_getText(*(void **)gSupernovaGameText, 0x267);
    _mtw_ChoiceWindow_set(cw, s1, s2, false);
    u8(this, 0x170) = 1;
    u8(this, 0x180) = 1;
}
