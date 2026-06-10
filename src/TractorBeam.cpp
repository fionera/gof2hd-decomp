#include "gof2/TractorBeam.h"
#include "gof2/Radar.h"

// KIPlayer (the grabbed crate) is the minimal vtable-only struct from Radar.h; its
// data fields are not modeled in any in-batch header, so read them by typed offset.
// (MISSING FIELD KIPlayer.field_0x4 (void*), field_0x3c (uint8_t), field_0x40 (uint8_t),
//  field_0x41 (uint8_t), field_0x78 (void*) -- belong to out-of-batch KIPlayer.)
static inline void *&kiPtr(void *p, int off) { return *(void **)((char *)p + off); }
static inline uint8_t &kiByte(void *p, int off) { return *((uint8_t *)p + off); }


extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t meshId, PaintCanvas *canvas, bool visible);
extern "C" void AEGeometry_render(AEGeometry *geo);
extern "C" void AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *p);

// ---- TractorBeam_1511b4.cpp ----
// AEGeometry::AEGeometry(mesh-id, canvas, visible) ctor @ 0x0007207c.
// Hidden PC-relative pointer to the active PaintCanvas root.
extern void *const gCanvasRoot __attribute__((visibility("hidden")));

// TractorBeam::TractorBeam(AEGeometry*, int)
//   Zeroes the embedded state then constructs the beam mesh geometry. The mesh
//   id is the base id 0x3798 offset by the (truncated) integer argument.
void TractorBeam_ctor(TractorBeam *self, AEGeometry * /*unused*/, int param2)
{
    self->field_0x0 = 0.0f;
    self->field_0x4 = 0.0f;
    self->field_0x8 = 0.0f;
    self->field_0xc = 0;
    self->field_0x10 = 0;

    AEGeometry *geo = (AEGeometry *)operator new(0xc0);
    uint16_t meshId = (uint16_t)((short)param2 + 0x3798);
    PaintCanvas *canvas = *(PaintCanvas **)(*(void **)gCanvasRoot);
    AEGeometry_ctor(geo, meshId, canvas, false);

    self->field_0x14 = geo;
    self->field_0x18 = 0;
}

// ---- render_151580.cpp ----
// AEGeometry::render(AEGeometry*) -- resolved global render entry for the beam mesh.

// TractorBeam::render()
//   Only draws while the beam is active; forwards to the geometry's render.
void TractorBeam_render(TractorBeam *self)
{
    if (self->field_0x10 == 0)
        return;
    AEGeometry_render(self->field_0x14);
}

// ---- update_15122c.cpp ----
using namespace AbyssEngine::AEMath;

// --- cross-subsystem callees (resolved blx targets) ---------------------------
extern "C" void *Level_getPlayer(Level *level);                       // 0x72034
extern "C" int   PlayerEgo_isInTurretMode(void *player);              // 0x7678c
extern "C" void  PlayerEgo_getPosition(Vector *out, void *player);    // 0x73540
extern "C" void  PlayerEgo_GetDirVector(Vector *out, void *player);   // 0x73540 family
extern "C" void  PlayerEgo_GetUpVector(Vector *out, void *player);    // 0x7354c
extern "C" int   Player_isActive(void *crate);                       // 0x71f14
extern "C" int   Player_getHitpoints(void *obj);                     // 0x724f0
extern "C" int   KIPlayer_cargoAvailable(void *crate);               // 0x71f14 family
extern "C" void  KIPlayer_createCrate(void *crate, int kind);        // 0x75904
extern "C" void  KIPlayer_captureCrate(void *crate, Hud *hud);       // 0x772fc
extern "C" int   KIPlayer_isDead(void *crate);                       // 0x71ec0
extern "C" int   KIPlayer_isDying(void *crate);                      // 0x71f50

extern "C" void *Status_getShip(void);                               // 0x71a58
extern "C" int   Ship_getIndex(void *ship);                          // 0x719c8

extern "C" uint32_t PaintCanvas_TransformGetTransform(uint32_t canvas); // 0x72088
extern "C" void  Transform_Update(uint32_t tf, int frameTime);          // 0x6f7cc

extern "C" void  AEGeometry_getPosition(Vector *out, void *geo);     // 0x720b8
extern "C" void  AEGeometry_setScaling(void *geo, float x, float y, float z); // 0x727b4
extern "C" void  AEGeometry_setDirection(void *geo, Vector *dir);    // 0x726ac
extern "C" void  AEGeometry_setPosition(void *geo, Vector *pos);     // 0x72148
extern "C" void  AEGeometry_translate(void *geo, Vector *delta);     // 0x72628 family

extern "C" int   FModSound_play(void *snd, void *a, void *b, float c); // 0x71548
extern "C" int   FModSound_stop(void *snd);                            // 0x724a8

// Vector aggregate ops resolved as helpers.
extern "C" void  Vector_assign(Vector *dst, const Vector *src);      // 0x6eb3c
extern "C" void  Vector_subAssign(Vector *dst, const Vector *src);   // 0x72694
extern "C" void  Vector_mulAssign(Vector *dst, float s);             // 0x726ac family
extern "C" void  Vector_scale(Vector *out, const Vector *v, float s);// 0x6ec74

// Hidden PC-relative roots.
extern void *const gCanvasRoot __attribute__((visibility("hidden")));
extern void *const gPullSound __attribute__((visibility("hidden")));
extern void *const gCaptureSound __attribute__((visibility("hidden")));
extern const float gCaptureDistance;

// TractorBeam::update(int frameTime, Radar*, Level*, Hud*)
//   Drives the tractor beam: locks onto a nearby crate, animates the beam mesh
//   from the player ship toward the crate, pulls the crate in, and captures it
//   once it is close enough.
void TractorBeam_update(TractorBeam *self, int frameTime, Radar *radar, Level *level, Hud *hud)
{
    KIPlayer *crate = self->field_0xc;
    KIPlayer *radarCrate = (KIPlayer *)radar->field_0x1c;

    if (radarCrate == 0 && crate == 0)
        return;

    if (crate == 0) {
        void *player = Level_getPlayer(level);
        if (PlayerEgo_isInTurretMode(player) == 0) {
            if (KIPlayer_cargoAvailable(radarCrate) == 0) {
                self->field_0x10 = 0;
                radar->field_0x1c = 0;
            } else {
                if (kiPtr(radarCrate, 0x78) == 0)
                    KIPlayer_createCrate(radarCrate, 0);
                self->field_0xc = radarCrate;
                self->field_0x18 = (int)Player_getHitpoints(kiPtr(radarCrate, 0x4));
                self->field_0x10 = 1;
            }
            return;
        }
        crate = self->field_0xc;
        if (crate == 0)
            goto detach;
    }

    // Drop the beam if the crate has lost its body or the carrier is gone.
    if (kiPtr(crate, 0x78) == 0 ||
        (kiByte(crate, 0x3c) == 0 && Player_isActive(crate) == 0)) {
    detach:
        radar->field_0x8 = 0;
        radar->field_0x1c = 0;
        self->field_0xc = 0;
        self->field_0x10 = 0;
        return;
    }

    // Animate the visible beam.
    uint32_t canvas = *(uint32_t *)(*(void **)gCanvasRoot);
    uint32_t tf = PaintCanvas_TransformGetTransform(canvas);
    Transform_Update(tf, frameTime);

    Vector pos;
    AEGeometry_getPosition(&pos, self->field_0x14);
    Vector_assign((Vector *)self, &pos);

    Vector playerPos;
    PlayerEgo_getPosition(&playerPos, Level_getPlayer(level));
    Vector_subAssign((Vector *)self, &playerPos);
    float dist = VectorLength((Vector *)self);

    void *ship = Status_getShip();
    int idx = Ship_getIndex(ship);
    Vector offset = {0.0f, 0.0f, 0.0f};
    if (idx == 0x2c) {
        Vector dir;
        PlayerEgo_GetDirVector(&dir, Level_getPlayer(level));
        Vector_scale(&offset, &dir, 0.5f);
    } else {
        ship = Status_getShip();
        idx = Ship_getIndex(ship);
        if (idx == 0x31) {
            Vector dir, up, tmp;
            PlayerEgo_GetDirVector(&dir, Level_getPlayer(level));
            Vector_scale(&tmp, &dir, 0.5f);
            PlayerEgo_GetUpVector(&up, Level_getPlayer(level));
            Vector_scale(&up, &up, 0.5f);
            offset.x = tmp.x + dir.x;
            offset.y = tmp.y + dir.y;
            offset.z = tmp.z + dir.z;
        }
    }
    AEGeometry_setScaling(self->field_0x14, offset.x, offset.y, offset.z);

    // Aim the beam at the crate and place its tail at the player position.
    void *geo = self->field_0x14;
    Vector beamDir = AbyssEngine::AEMath::operator-(*(Vector *)&offset, *(Vector *)self);
    Vector n;
    VectorNormalize(&n, &beamDir);
    AEGeometry_setDirection(geo, &n);

    Vector tail;
    PlayerEgo_getPosition(&tail, Level_getPlayer(level));
    AEGeometry_setPosition(geo, &tail);

    if (dist > gCaptureDistance) {
        // Crate still far -- pull it toward the ship along the beam.
        radar->field_0x1c = 0;
        VectorNormalize(&n, (Vector *)self);
        Vector_assign((Vector *)self, &n);
        Vector_mulAssign((Vector *)self, (float)(frameTime * 10));
        void *crateGeo = kiPtr(self->field_0xc, 0x78);
        Vector pull = AbyssEngine::AEMath::operator-(n, *(Vector *)self);
        AEGeometry_translate(crateGeo, &pull);

        if (KIPlayer_isDead(self->field_0xc) != 0 ||
            KIPlayer_isDying(self->field_0xc) != 0) {
            KIPlayer *c = self->field_0xc;
            if (kiByte(c, 0x40) == 0 && kiByte(c, 0x41) == 0) {
                Vector d = AbyssEngine::AEMath::operator-(n, *(Vector *)self);
                (void)d;
            }
        }
        if (self->field_0x11 == 0) {
            FModSound_play(*(void **)gPullSound, 0, 0, 0.0f);
            self->field_0x11 = 1;
        }
    } else {
        // Crate reached the ship -- capture it.
        KIPlayer_captureCrate(self->field_0xc, hud);
        self->field_0xc = 0;
        self->field_0x10 = 0;
        radar->field_0x8 = 0;
        radar->field_0x1c = 0;
        self->field_0x11 = 0;
        void *snd = *(void **)gCaptureSound;
        FModSound_stop(snd);
        FModSound_play(snd, (void *)4, 0, 0.0f);
    }
}

// ---- _TractorBeam_151210.cpp ----
// AEGeometry::~AEGeometry() @ 0x00071fc8.

// TractorBeam::~TractorBeam() (D2). Returns `this`.
//   Destroys and frees the beam geometry, then clears the slot.
extern "C" TractorBeam *_ZN11TractorBeamD2Ev(TractorBeam *self)
{
    AEGeometry *geo = self->field_0x14;
    if (geo != 0) {
        AEGeometry_dtor(geo);
        operator_delete(geo);
    }
    self->field_0x14 = 0;
    return self;
}
