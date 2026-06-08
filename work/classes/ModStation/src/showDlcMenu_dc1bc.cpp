#include "class.h"

struct MenuTouchWindow;

__attribute__((visibility("hidden"))) extern int  *g_dlc_stack;     // [DAT_000ec28c]
__attribute__((visibility("hidden"))) extern int  *g_dlc_btnX;      // [DAT_000ec290] cached button X array
__attribute__((visibility("hidden"))) extern int  *g_dlc_btnY;      // [DAT_000ec294]
__attribute__((visibility("hidden"))) extern int **g_dlc_btnCount;  // [DAT_000ec298]

extern "C" {
void *ModStation_opnew_dlc(unsigned size);
void  MenuTouchWindow_ctor_dlc(MenuTouchWindow *w, int kind);
void  MenuTouchWindow_callDlcMenu_dlc(MenuTouchWindow *w);
void  TouchButton_getPosition_dlc(void *dst, void *win, unsigned idx);
}

// ModStation::showDlcMenu() — opens (creating it on first use) the in-station DLC store window,
// snapshotting the current button layout first.
extern "C" void ModStation_showDlcMenu(ModStation *self)
{
    MenuTouchWindow *win = (MenuTouchWindow *)P(self, 0x50);
    if (win == 0) {
        win = (MenuTouchWindow *)ModStation_opnew_dlc(0x240);
        MenuTouchWindow_ctor_dlc(win, 2);
        I(self, 0x50) = (int)win;
    }
    C(self, 0x6a) = 1; // m_nStarMapWindowOpen + 2

    int *bx = g_dlc_btnX;
    int *by = g_dlc_btnY;
    for (unsigned i = 0; i < **(unsigned **)((char *)win + 4); i = i + 1) {
        if (i < 10) {
            float pos[3];
            TouchButton_getPosition_dlc(pos, win, i);
            bx[i] = (int)pos[0];
            TouchButton_getPosition_dlc(pos, win, i);
            by[i] = (int)pos[1];
            win = (MenuTouchWindow *)P(self, 0x50);
        }
    }
    **g_dlc_btnCount = **(unsigned **)((char *)win + 4);
    C(self, 0x66) = 0; // field_64 + 2
    MenuTouchWindow_callDlcMenu_dlc(win);
}
