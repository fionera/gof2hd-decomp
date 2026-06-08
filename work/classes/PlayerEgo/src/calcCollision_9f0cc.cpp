#include "class.h"

// PlayerEgo::calcCollision(Array<KIPlayer*>* candidates)
//   Resolves collisions between the player and each candidate object this frame
//   (skipped while in the final docking approach). For each candidate that the
//   broad-phase reports as overlapping it dispatches by object kind:
//     * gas cloud / wormhole (type 0x4262): plays the proximity rumble whose
//       gain scales with distance, and -- when close -- nudges the ship away,
//     * solid obstacle (field_3C == 0): snaps the ship to the surface contact
//       point and re-derives the HUD transform,
//     * destructible (field_3C != 0): applies mutual collision damage and a
//       roll kick.
//   The per-kind vector/matrix physics is delegated to the PE_cc_* helpers; the
//   recoverable iteration and branch selection stay inline.

extern "C" void  AEGeometry_getPosition(void *geo, void *out);
extern "C" float Vec_length(const void *v);
extern "C" int   Status_inAlienOrbit();
extern "C" int   KIPlayer_getType(void *o);
extern "C" int   KIPlayer_isVisible(void *o);
extern "C" int   KIPlayer_isDying(void *o);
extern "C" int   KIPlayer_isDead(void *o);
extern "C" int   PlayerWormHole_isShrinking(void *o);
extern "C" void  hitCamera_(PlayerEgo *self);
// kind-specific resolvers (own the corrupted vector / matrix physics).
extern "C" void  PE_cc_wormhole(PlayerEgo *self, void *obj);
extern "C" void  PE_cc_obstacle(PlayerEgo *self, void *obj, unsigned idx);
extern "C" void  PE_cc_destructible(PlayerEgo *self, void *obj);

extern const float g_PE_cc_alarmDist;   // 0xaf470

extern "C" void PlayerEgo_calcCollision(PlayerEgo *self, void *candidates)
{
    if (candidates == 0)
        return;
    if (C(self, 0x356) != 0 && (unsigned)(I(self, 0x1c4) - 1) < 3)
        return;   // in final docking approach: no collisions

    unsigned int *arr = (unsigned int *)candidates;
    for (unsigned i = 0; i < arr[0]; i++) {
        void *obj = *(void **)(arr[1] + i * 4);
        if (obj == 0)
            continue;

        // first candidate: trip the proximity alarm flag when very close.
        if (i == 0 && Status_inAlienOrbit() == 0) {
            if (Vec_length((char *)self + 0x148) < g_PE_cc_alarmDist && C(obj, 0x71) != 0)
                C(self, 0x234) = 1;
        }

        // broad-phase overlap test (virtual +0x40).
        float pos[3];
        AEGeometry_getPosition(P(self, 0x8), pos);
        typedef int (*overlap_fn)(void *, void *);
        overlap_fn overlaps = *(overlap_fn *)(*(char **)obj + 0x40);
        if (!overlaps(obj, pos))
            continue;

        if (KIPlayer_getType(obj) == 0x4262 && KIPlayer_isVisible(obj) != 0) {
            if (PlayerWormHole_isShrinking(obj) == 0 && P(self, 0x1e4) == 0)
                PE_cc_wormhole(self, obj);
        } else if (C(obj, 0x3c) == 0) {
            if (KIPlayer_isVisible(obj) != 0) {
                // skip the object we are actively docking with.
                bool docking = (C(self, 0x1c0) != 0 || C(self, 0x356) != 0)
                               && P(self, 0x1bc) == obj;
                if (!docking)
                    PE_cc_obstacle(self, obj, i);
                hitCamera_(self);
            }
        } else {
            if (KIPlayer_isDying(obj) == 0 && KIPlayer_isDead(obj) == 0) {
                bool skipDockTarget = (C(self, 0x1c0) != 0 || C(self, 0x356) != 0)
                                      && obj == P(self, 0x1bc);
                if (!skipDockTarget) {
                    PE_cc_destructible(self, obj);
                    hitCamera_(self);
                }
            }
        }
    }
}
