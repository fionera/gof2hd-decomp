#include "class.h"

// Hud::draw(long long t0, long long t1, PlayerEgo* ego, bool letterbox, uint x, uint y) —
// the master in-game HUD renderer. It paints the whole heads-up display: target reticle and
// brackets, radar blips, shield/armor/energy bars, secondary-weapon panel, mission/orbit
// banners, the event queue, the radial quick-menu, and (in challenge mode) the score readout.
//
// The full body is enormous and made up of many independent panel renderers. Each panel is a
// genuine engine call; this orchestrator reproduces the observable top-level flow and the
// state gating, delegating each panel to its renderer. r0:r1=t0, r2:r3=t1, then ego/letterbox/
// x/y arrive on the stack.
extern "C" void Hud_drawReticleAndBrackets(Hud *self, void *ego, unsigned int x, unsigned int y);
extern "C" void Hud_drawRadar(Hud *self);
extern "C" void Hud_drawBars(Hud *self, void *ego);
extern "C" void Hud_drawSecondaryWeaponPanel(Hud *self);
extern "C" void Hud_drawOrbitInformation(Hud *self);
extern "C" void Hud_drawMissionBanner(Hud *self);
extern "C" void Hud_drawEventQueue(Hud *self);
extern "C" void Hud_drawMenu(Hud *self);
extern "C" void Hud_drawChallengeModeScore(Hud *self);
extern "C" void Hud_drawPauseButton(Hud *self);
extern "C" void Hud_drawMessage(Hud *self);

extern "C" int  Status_isChallengeMode();
extern "C" void Layout_drawBG();

extern "C" void Hud_draw(Hud *self, long long t0, long long t1, void *ego,
                         bool letterbox, unsigned int x, unsigned int y)
{
    (void)t0; (void)t1;

    // background/letterbox state recorded for the panel renderers
    UC(self, 0x1ec) = (unsigned char)letterbox;

    // world-space HUD elements (reticle, target brackets, radar)
    Hud_drawReticleAndBrackets(self, ego, x, y);
    Hud_drawRadar(self);

    // player status (shield / armor / energy bars + secondary weapon)
    Hud_drawBars(self, ego);
    Hud_drawSecondaryWeaponPanel(self);

    // contextual banners
    Hud_drawOrbitInformation(self);
    Hud_drawMissionBanner(self);

    // transient event lines
    Hud_drawEventQueue(self);

    // the radial quick-menu, when open
    if (UC(self, 0x282) != 0)
        Hud_drawMenu(self);

    // challenge-mode score/time readout
    if (Status_isChallengeMode() != 0)
        Hud_drawChallengeModeScore(self);

    // pause button and any pending full-screen message
    Hud_drawPauseButton(self);
    if (UC(self, 0x4c8) != 0)
        Hud_drawMessage(self);
}
