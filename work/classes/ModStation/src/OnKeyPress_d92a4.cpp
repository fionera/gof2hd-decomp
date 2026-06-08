#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_okp_stack; // [DAT_000e9614]

extern "C" {
void Status_checkForLevelUp_okp();
// Opens station sub-screen `which` (0 hangar, 1 lounge, 2 star map, 3 missions, 4 status), with
// all the campaign-gating, sound, help-window and hangar-rebuild logic the decompiler corrupted.
// Returns nonzero if the screen actually opened.
int  ModStation_okp_openScreen(ModStation *self, int which);
// Shows the "feature locked" choice-window message for the current campaign progress.
void ModStation_okp_showLocked(ModStation *self);
void ModStation_leaveStation_okp(ModStation *self);
void ModStation_okp_toggleHelp(ModStation *self);
}

// ModStation::OnKeyPress(self, key) — drives the station main-menu cursor and screen selection.
extern "C" void ModStation_OnKeyPress(ModStation *self, long long key)
{
    char *s = (char *)self;
    if (C(s, 0x24) == 0)
        return;

    Status_checkForLevelUp_okp();

    // a level-up banner is showing: SELECT just dismisses it.
    int t34 = I(s, 0x34);
    bool banner = 999 < U(s, 0x30);
    if (!((-(int)banner - t34) < 0)) {
        if (key == 0x10000) {
            U(s, 0x30) = 0x3e9;
            I(s, 0x34) = 0;
        }
        return;
    }

    // swallow keys while any modal sub-window owns input.
    if (C(s, 0x63) != 0 || C(s, 0x69) != 0 || C(s, 0x65) != 0 || C(s, 0x66) != 0 ||
        C(s, 0x67) != 0 || C(s, 0x64) != 0 || C(s, 0x68) != 0)
        return;

    // DLC menu open: SELECT closes the station.
    if (C(s, 0x6a) != 0) {
        if (key == 0x10000)
            ModStation_leaveStation_okp(self);
        return;
    }

    unsigned char held = UC(s, 0x61);
    char sub = C(s, 0x62);
    if (held == 0) {
        // not in the sub-menu: BACK leaves the station.
        if (key == 0x40000 && sub == 0)
            ModStation_leaveStation_okp(self);
        return;
    }

    if (sub != 0) {
        // in a sub-state; toggle help on the SOFT key.
        if (key == 0x20000)
            ModStation_okp_toggleHelp(self);
        return;
    }

    // main button row navigation.
    if (key == 0x4000) {            // left
        int sel = I(s, 0x44);
        int *btn = (int *)I(s, 0x48);
        btn[sel] = 0;
        sel = (0 < sel) ? (sel - 1) : 4;
        I(s, 0x44) = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x8000) {            // right
        int sel = I(s, 0x44);
        int *btn = (int *)I(s, 0x48);
        btn[sel] = 0;
        sel = (sel < 4) ? (sel + 1) : 0;
        I(s, 0x44) = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x20000)
        return;
    if (key != 0x10000)
        return;

    // SELECT: open the highlighted screen.
    int which = I(s, 0x44);
    if (which < 0 || which > 4) {
        ModStation_okp_showLocked(self);
        return;
    }
    if (ModStation_okp_openScreen(self, which) == 0)
        ModStation_okp_showLocked(self);
}
