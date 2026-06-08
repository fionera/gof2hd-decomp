#include "class.h"

// Hud::hudEvent(int eventId, PlayerEgo* ego, int arg) — central dispatcher for in-game HUD
// notifications. Most events compose a localized String into +0x1e0 and push it onto the
// event queue; a handful (autofire/cloak toggles) only flip status bytes. The string-building
// per event id is large and repetitive, so it is delegated to an extern builder that performs
// the genuine GameText formatting and queueing for that id. r0=this, r1=eventId, r2=ego, r3=arg.
//
// Several events are gated on HUD-enable flags (+0x221 autofire, +0x21e boost) before building.
extern "C" int  PlayerEgo_readyToBoost(void *ego);
extern "C" void Hud_hudEventBuild(Hud *self, int eventId, void *ego, int arg); // builds + queues

extern "C" void Hud_hudEvent(Hud *self, int eventId, void *ego, int arg)
{
    switch (eventId) {
    case 1:
    case 2:
        // autofire on/off notice — only when the autofire UI is present
        if (self->f_221 == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;
    case 3:
        if (self->f_21e == 0 || PlayerEgo_readyToBoost((void *)(long)arg) == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;
    case 4:
        if (self->f_21e == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;

    // ---- pure status-flag events (no queue entry) ----
    case 0x23:
        I(self, 0x468) = 0;
        self->f_27a = 1;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x25:
        I(self, 0x468) = 0;
        self->f_27a = 1;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x27:
        I(self, 0x468) = 0;
        self->f_27a = 0;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x29:
        I(self, 0x468) = 0;
        self->f_27a = 0;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x24:
    case 0x26:
    case 0x28:
    case 0x2a:
        // these clear the "showing" flag and set a fixed localized line, no queue
        self->f_278 = 0;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;

    default:
        // all remaining events compose a localized line and enqueue it
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;
    }
}
