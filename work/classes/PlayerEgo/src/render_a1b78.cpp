#include "class.h"

// PlayerEgo::render(bool allowHud)
//   Draws the ship and all attached visual effects. When alive it renders the
//   explosion shell (if any), the hull geometry, shield/cloak overlays, the
//   tractor beam and every active repair beam, then tail-calls the level draw
//   with a flag derived from the docking state. When dead it only renders the
//   fading explosion debris.

extern "C" int  PlayerEgo_isDead(PlayerEgo *self);
extern "C" void Explosion_render(void *exp);
extern "C" void AEGeometry_render(void *geo);
extern "C" void AEGeometry_setVisible(void *geo, bool vis);
extern "C" void TractorBeam_render(void *tb);
extern "C" void RepairBeam_render(void *rb);
extern "C" void PlayerEgo_render_tail(void *level, int flag);   // 0x1abc38 veneer

extern "C" void PlayerEgo_render(PlayerEgo *self, int allowHud)
{
    void *level = P(self, 0xc);

    if (PlayerEgo_isDead(self) == 0) {
        if (C(self, 0x309) == 0)
            return;

        if (P(self, 0x8c) != 0) {
            Explosion_render(P(self, 0x8c));
            if (I(self, 0x2f8) <= 0xbb7)        // explosion < 3000 ticks old
                AEGeometry_render(P(self, 0x8));
        }
        if (P(self, 0x90) != 0)
            Explosion_render(P(self, 0x90));

        AEGeometry_render(P(self, 0x8));

        if (P(self, 0xac) != 0 && I(self, 0x30c) >= 1)
            AEGeometry_render(P(self, 0x8));    // shield overlay reuses hull geo

        if (C(self, 0x38) != 0)
            AEGeometry_render(P(self, 0x34));

        if (C(self, 0x170) != 0) {              // cloak overlay
            if (P(self, 0x30) != 0)
                AEGeometry_setVisible(P(self, 0x30), C(self, 0x1a0) != 0);
            AEGeometry_render(P(self, 0x2c));
        }

        if (P(self, 0x1b4) != 0)
            TractorBeam_render(P(self, 0x1b4));

        unsigned int *beams = (unsigned int *)P(self, 0x1b8);
        if (beams != 0) {
            for (unsigned int i = 0; i < beams[0]; i++) {
                RepairBeam_render(*(void **)(beams[1] + i * 4));
                beams = (unsigned int *)P(self, 0x1b8);
            }
        }

        int flag = 1;
        if (C(self, 0x24) == 0 && allowHud != 0)
            flag = (I(self, 0x1c4) - 1) != 0 ? 1 : 0;
        else
            flag = 1;
        PlayerEgo_render_tail(level, flag);
    } else {
        if (P(self, 0x8c) != 0) {
            Explosion_render(P(self, 0x8c));
            if (I(self, 0x2f8) < 3000)
                AEGeometry_render(P(self, 0x8));
        }
        if (P(self, 0xac) != 0)
            Explosion_render(P(self, 0x8c));
        PlayerEgo_render_tail(level, 1);
    }
}
