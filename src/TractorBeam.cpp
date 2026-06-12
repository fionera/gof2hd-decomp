#include "gof2/TractorBeam.h"
#include "gof2/AEGeometry.h"
#include "gof2/FModSound.h"
#include "gof2/Level.h"
#include "gof2/Transform.h"
#include "gof2/KIPlayer.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Status.h"

// Player.h cannot be included here: it both declares a data member 'turnedEnemy'
// (field 0xe0) and a method 'turnedEnemy()' inside the same struct, which is
// ill-formed C++ and makes the header fail to self-compile. TractorBeam only needs
// two Player accessors, so a minimal local declaration is used instead.
struct Player {
    int getHitpoints();
    unsigned char isActive();
};

// Radar.h re-defines a minimal vtable-only 'struct KIPlayer' (no GOF2_KIPLAYER_H
// guard), which collides with the full definition pulled in from KIPlayer.h above.
// Rename Radar.h's copy via the preprocessor so only the 'Radar' struct is taken
// from it; the cpp always uses the full KIPlayer from KIPlayer.h.
// Radar.h also defines a minimal global 'struct Status' (only getSystem) and declares
// 'gStatus' against it; that collides with the full Status from Status.h above. Rename
// Radar.h's copy via the preprocessor and re-declare gStatus against the real Status.
#define KIPlayer KIPlayer_RadarUnused
#define Status Status_RadarUnused
#define gStatus gStatus_RadarUnused
#include "gof2/Radar.h"
#undef gStatus
#undef Status
#undef KIPlayer

extern Status *gStatus;

// KIPlayer (the grabbed crate) is the minimal vtable-only struct from Radar.h; its
// data fields are not modeled in any in-batch header, so read them by typed offset.
// (MISSING FIELD KIPlayer.player (void*), field_0x3c (uint8_t), field_0x40 (uint8_t),
//  field_0x41 (uint8_t), field_0x78 (void*) -- belong to out-of-batch KIPlayer.)
static inline void *&kiPtr(void *p, int off) { return *(void **)((char *)p + off); }
static inline uint8_t &kiByte(void *p, int off) { return *((uint8_t *)p + off); }


extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t meshId, PaintCanvas *canvas, bool visible);
extern "C" void AEGeometry_dtor(AEGeometry *self);

// ---- TractorBeam_1511b4.cpp ----
// AEGeometry::AEGeometry(mesh-id, canvas, visible) ctor @ 0x0007207c.
// Hidden PC-relative pointer to the active PaintCanvas root.
extern void *const gCanvasRoot __attribute__((visibility("hidden")));

// TractorBeam::TractorBeam(AEGeometry*, int)
//   Zeroes the embedded state then constructs the beam mesh geometry. The mesh
//   id is the base id 0x3798 offset by the (truncated) integer argument.
void TractorBeam::ctor(AEGeometry * /*unused*/, int param2) {
    TractorBeam *self = this;
    self->dirX = 0.0f;
    self->dirY = 0.0f;
    self->dirZ = 0.0f;
    self->grabbedCrate = 0;
    self->active = 0;

    AEGeometry *geo = (AEGeometry *)operator new(0xc0);
    uint16_t meshId = (uint16_t)((short)param2 + 0x3798);
    PaintCanvas *canvas = *(PaintCanvas **)(*(void **)gCanvasRoot);
    AEGeometry_ctor(geo, meshId, canvas, false);

    self->beamGeometry = geo;
    self->storedHitpoints = 0;
}

// ---- render_151580.cpp ----
// AEGeometry::render(AEGeometry*) -- resolved global render entry for the beam mesh.

// TractorBeam::render()
//   Only draws while the beam is active; forwards to the geometry's render.
void TractorBeam::render() {
    TractorBeam *self = this;
    if (self->active == 0)
        return;
    ((AEGeometry *)(self->beamGeometry))->render();
}

// ---- update_15122c.cpp ----
using namespace AbyssEngine::AEMath;

// --- cross-subsystem callees (resolved blx targets) ---------------------------
// 0x72034
// 0x7678c
// 0x73540
// 0x73540 family
// 0x7354c
// 0x71f14
// 0x724f0
// 0x71f14 family
// 0x75904
// 0x772fc
// 0x71ec0
// 0x71f50

// 0x71a58
extern "C" int   Ship_getIndex(void *ship);                          // 0x719c8

// PaintCanvas::TransformGetTransform is an engine static helper; PaintCanvas is a
// class type in this TU (not an openable namespace), so it stays an external symbol.
extern "C" uint32_t PaintCanvas_TransformGetTransform(uint32_t canvas); // 0x72088
// 0x6f7cc

// 0x720b8
// 0x727b4
// 0x726ac
// 0x72148
// 0x72628 family

// 0x71548
// 0x724a8

// Vector aggregate ops resolved as AEMath helpers.
namespace AbyssEngine { namespace AEMath {
void  Vector_assign(Vector *dst, const Vector *src);      // 0x6eb3c
void  Vector_subAssign(Vector *dst, const Vector *src);   // 0x72694
void  Vector_mulAssign(Vector *dst, float s);             // 0x726ac family
void  Vector_scale(Vector *out, const Vector *v, float s);// 0x6ec74
} }

// Hidden PC-relative roots.
extern void *const gCanvasRoot __attribute__((visibility("hidden")));
extern void *const gPullSound __attribute__((visibility("hidden")));
extern void *const gCaptureSound __attribute__((visibility("hidden")));
extern const float gCaptureDistance;

// TractorBeam::update(int frameTime, Radar*, Level*, Hud*)
//   Drives the tractor beam: locks onto a nearby crate, animates the beam mesh
//   from the player ship toward the crate, pulls the crate in, and captures it
//   once it is close enough.
void TractorBeam::update(int frameTime, Radar *radar, Level *level, Hud *hud) {
    TractorBeam *self = this;
    KIPlayer *crate = self->grabbedCrate;
    KIPlayer *radarCrate = (KIPlayer *)radar->field_0x1c;

    if (radarCrate == 0 && crate == 0)
        return;

    if (crate == 0) {
        void *player = (void *)(intptr_t)((Level *)(level))->getPlayer();
        if (((PlayerEgo *)(player))->isInTurretMode() == 0) {
            if (((KIPlayer *)(radarCrate))->cargoAvailable() == 0) {
                self->active = 0;
                radar->field_0x1c = 0;
            } else {
                if (kiPtr(radarCrate, 0x78) == 0)
                    ((KIPlayer *)(radarCrate))->createCrate(0);
                self->grabbedCrate = radarCrate;
                self->storedHitpoints = (int)((Player *)(kiPtr(radarCrate, 0x4)))->getHitpoints();
                self->active = 1;
            }
            return;
        }
        crate = self->grabbedCrate;
        if (crate == 0)
            goto detach;
    }

    // Drop the beam if the crate has lost its body or the carrier is gone.
    if (kiPtr(crate, 0x78) == 0 ||
        (kiByte(crate, 0x3c) == 0 && ((Player *)(crate))->isActive() == 0)) {
    detach:
        radar->field_0x8 = 0;
        radar->field_0x1c = 0;
        self->grabbedCrate = 0;
        self->active = 0;
        return;
    }

    // Animate the visible beam.
    uint32_t canvas = *(uint32_t *)(*(void **)gCanvasRoot);
    uint32_t tf = PaintCanvas_TransformGetTransform(canvas);
    ((AbyssEngine::Transform *)(tf))->Update(frameTime, false);

    Vector pos;
    ((AEGeometry *)(&pos))->getPosition();
    Vector_assign((Vector *)self, &pos);

    Vector playerPos;
    ((PlayerEgo *)(&playerPos))->getPosition();
    Vector_subAssign((Vector *)self, &playerPos);
    float dist = VectorLength((Vector *)self);

    void *ship = gStatus->getShip();
    int idx = Ship_getIndex(ship);
    Vector offset = {0.0f, 0.0f, 0.0f};
    if (idx == 0x2c) {
        Vector dir;
        ((PlayerEgo *)(&dir))->GetDirVector();
        Vector_scale(&offset, &dir, 0.5f);
    } else {
        ship = gStatus->getShip();
        idx = Ship_getIndex(ship);
        if (idx == 0x31) {
            Vector dir, up, tmp;
            ((PlayerEgo *)(&dir))->GetDirVector();
            Vector_scale(&tmp, &dir, 0.5f);
            ((PlayerEgo *)(&up))->GetUpVector();
            Vector_scale(&up, &up, 0.5f);
            offset.x = tmp.x + dir.x;
            offset.y = tmp.y + dir.y;
            offset.z = tmp.z + dir.z;
        }
    }
    ((AEGeometry *)(self->beamGeometry))->setScaling(offset.x);

    // Aim the beam at the crate and place its tail at the player position.
    void *geo = self->beamGeometry;
    Vector beamDir = AbyssEngine::AEMath::operator-(*(Vector *)&offset, *(Vector *)self);
    Vector n;
    VectorNormalize(&n, &beamDir);
    ((AEGeometry *)(geo))->setDirection(n, n);

    Vector tail;
    ((PlayerEgo *)(&tail))->getPosition();
    ((AEGeometry *)(geo))->setPosition(tail);

    if (dist > gCaptureDistance) {
        // Crate still far -- pull it toward the ship along the beam.
        radar->field_0x1c = 0;
        VectorNormalize(&n, (Vector *)self);
        Vector_assign((Vector *)self, &n);
        Vector_mulAssign((Vector *)self, (float)(frameTime * 10));
        void *crateGeo = kiPtr(self->grabbedCrate, 0x78);
        Vector pull = AbyssEngine::AEMath::operator-(n, *(Vector *)self);
        ((AEGeometry *)(crateGeo))->translate(pull);

        if (((KIPlayer *)(self->grabbedCrate))->isDead() != 0 ||
            ((KIPlayer *)(self->grabbedCrate))->isDying() != 0) {
            KIPlayer *c = self->grabbedCrate;
            if (kiByte(c, 0x40) == 0 && kiByte(c, 0x41) == 0) {
                Vector d = AbyssEngine::AEMath::operator-(n, *(Vector *)self);
                (void)d;
            }
        }
        if (self->soundPlaying == 0) {
            ((FModSound *)(*(void **)gPullSound))->play(0, 0, 0, 0.0f);
            self->soundPlaying = 1;
        }
    } else {
        // Crate reached the ship -- capture it.
        ((KIPlayer *)(self->grabbedCrate))->captureCrate(hud);
        self->grabbedCrate = 0;
        self->active = 0;
        radar->field_0x8 = 0;
        radar->field_0x1c = 0;
        self->soundPlaying = 0;
        void *snd = *(void **)gCaptureSound;
        ((FModSound *)(snd))->stop((void *)0);
        ((FModSound *)(snd))->play(4, 0, 0, 0.0f);
    }
}

// ---- _TractorBeam_151210.cpp ----
// AEGeometry::~AEGeometry() @ 0x00071fc8.

// TractorBeam::~TractorBeam() (D2). Returns `this`.
//   Destroys and frees the beam geometry, then clears the slot.
TractorBeam *_ZN11TractorBeamD2Ev(TractorBeam *self)
{
    AEGeometry *geo = self->beamGeometry;
    if (geo != 0) {
        AEGeometry_dtor(geo);
        ::operator delete(geo);
    }
    self->beamGeometry = 0;
    return self;
}
