#include "KIPlayer.h"


extern "C" void FModSound_resumeEvent(int a, int b);
extern "C" void FModSound_pauseEvent(int player);
extern "C" void AEGeometry_translate(void *geom, const Vector &v);
extern "C" void Route_translate(Route *route, const Vector &v);
extern "C" void MatrixGetPosition(Vector *out, float *matrix);
extern "C" void Object_setVisible(int obj);
extern "C" void operator_delete(void *p);
extern "C" void FModSound_stopEvent(int player);
extern "C" void Player_render(void *player);
extern "C" void Player_setActive(void *player, int active);
extern "C" void Player_setActive(void *player);
extern "C" void FModSound_playEvent(int a, int b, int c);
extern "C" void *Route_dtor(Route *r);
extern "C" void *Player_dtor(void *p);
extern "C" void *Route_dtor(Route *p);
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void *ArrayUint_dtor(void *p);
extern "C" void *ArrayInt_dtor(void *p);
extern "C" void ArrayReleaseClasses_SpacePoint(void *arr);
extern "C" void *ArraySpacePoint_dtor(void *p);
extern "C" void String_dtor(void *p);
extern "C" void Player_awake(void *player, int b);
extern "C" void KIPlayer_setActive(KIPlayer *self);
extern "C" void Player_setDead(void *self, int arg);
extern "C" void Player_addGun_b(void *player);
extern "C" void Player_reset(void *player);
extern "C" void KIPlayer_setToSleep(KIPlayer *self);
extern "C" void Route_reset(Route *route);
extern "C" void Player_setEnemies(void *player);
extern "C" void *operator_new(unsigned int sz);
extern "C" void *gCanvas;
extern "C" void AEGeometry_ctor2(void *self, void *canvas);
extern "C" void AEGeometry_addChild(void *self, int child);
extern "C" void AEGeometry_setPosition4(void *geom, int a, int b, int c);
extern "C" void *AEGeometry_getMatrix(void);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void Matrix_mulassign(void *dst, void *src);
extern "C" void AEGeometry_setPosition(void *geom, const Vector &v);
extern "C" void *AEGeometry_getMatrix(void *geom);
extern "C" void *Matrix_assign(void *dst, void *src);
extern "C" void *Matrix_mulassign(void *dst, void *src);
extern "C" void Player_addGun_a(void *player);
extern "C" void **gCanvasPtr;
extern "C" void AEGeometry_ctor(void *self, unsigned short type, void *canvas, bool b);
extern "C" void AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void AEGeometry_setPosition_v(void *self, Vector *v);
extern "C" void Player_setKIPlayer(void *player, KIPlayer *self);

// ---- getType_a61de.cpp ----
extern "C" int KIPlayer_getType(KIPlayer *self)
{
    return *(int *)((char *)self + 0xac);
}

// ---- projectCollisionOnSurface_a63d4.cpp ----
extern "C" void KIPlayer_projectCollisionOnSurface(Vector *out)
{
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

// ---- isDead_a610c.cpp ----
extern "C" bool KIPlayer_isDead(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 4;
}

// ---- getSpacePoints_a5b44.cpp ----
extern "C" Array<SpacePoint *> *KIPlayer_getSpacePoints(KIPlayer *self)
{
    return *(Array<SpacePoint *> **)((char *)self + 0xcc);
}

// ---- setJumpSphere_a6224.cpp ----
extern "C" void KIPlayer_setJumpSphere(KIPlayer *self, uint32_t sphere)
{
    *(uint32_t *)((char *)self + 0xd4) = sphere;
}

// ---- isWingMan_a6128.cpp ----
extern "C" uint8_t KIPlayer_isWingMan(KIPlayer *self)
{
    return *(uint8_t *)((char *)self + 0xdc);
}

// ---- ResumeEngineSound_a61a8.cpp ----
extern "C" void KIPlayer_ResumeEngineSound(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_resumeEvent(*(int *)((char *)self + 0x4), 0);
    }
}

// ---- setWingmanCommand_a613a.cpp ----
extern "C" void KIPlayer_setWingmanCommand(KIPlayer *self, int cmd, KIPlayer *target)
{
    *(int *)((char *)self + 0xe4) = cmd;
    *(KIPlayer **)((char *)self + 0xe8) = target;
}

// ---- PauseEngineSound_a617e.cpp ----
extern "C" void KIPlayer_PauseEngineSound(KIPlayer *self)
{
    int player = *(int *)((char *)self + 0x4);
    if (player != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_pauseEvent(player);
    }
}

// ---- setRotationSpeed_a6144.cpp ----
extern "C" void KIPlayer_setRotationSpeed(KIPlayer *self, float speed)
{
    *(float *)((char *)self + 0xa8) = speed;
}

// ---- isEnemy_a61e4.cpp ----
extern "C" uint8_t KIPlayer_isEnemy(KIPlayer *self)
{
    return *(uint8_t *)(*(char **)((char *)self + 0x4) + 0x5c);
}

// ---- isDocked_a61fa.cpp ----
extern "C" bool KIPlayer_isDocked(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 9;
}

// ---- getProjectionVector_a63ca.cpp ----
extern "C" void KIPlayer_getProjectionVector(Vector *out)
{
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

// ---- isDying_a611a.cpp ----
extern "C" bool KIPlayer_isDying(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 3;
}

// ---- translate_a6332.cpp ----
extern "C" void KIPlayer_translate(KIPlayer *self, const Vector &v)
{
    AEGeometry_translate(*(void **)((char *)self + 0x8), v);
    Route *route = *(Route **)((char *)self + 0x6c);
    if (route == 0) {
        return;
    }
    return Route_translate(route, v);
}

// ---- getPosition_a61ec.cpp ----
// Returns the ship position by value (sret in r0, this in r1).
Vector KIPlayer_getPosition(KIPlayer *self)
{
    Vector v;
    MatrixGetPosition(&v, (float *)(*(char **)((char *)self + 0x4) + 0x4));
    return v;
}

// ---- setVisible_a5b2c.cpp ----
extern "C" void KIPlayer_setVisible(KIPlayer *self, bool visible)
{
    int obj = *(int *)((char *)self + 0x8);
    *(uint8_t *)((char *)self + 0xf5) = visible;
    if (obj != 0) {
        return Object_setVisible(obj);
    }
}

// ---- _KIPlayer_a5b16.cpp ----
// Deleting destructor (D0): run the complete dtor, then tail-call operator delete.
extern "C" void *_ZN8KIPlayerD1Ev(KIPlayer *self); // complete object dtor

extern "C" void _ZN8KIPlayerD0Ev(KIPlayer *self)
{
    return operator_delete(_ZN8KIPlayerD1Ev(self));
}

// ---- StopEngineSound_a61be.cpp ----
extern "C" void KIPlayer_StopEngineSound(KIPlayer *self)
{
    int player = *(int *)((char *)self + 0x4);
    if (player != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_stopEvent(player);
    }
}

// ---- render_a632c.cpp ----
extern "C" void KIPlayer_render(KIPlayer *self)
{
    return Player_render(*(void **)((char *)self + 0x8));
}

// ---- setToSleep_a5e2a.cpp ----
extern "C" void KIPlayer_setToSleep(KIPlayer *self)
{
    void *player = *(void **)((char *)self + 0x4);
    *(uint32_t *)((char *)self + 0x88) = 5;
    Player_setActive(player, 0);
    *(uint8_t *)((char *)self + 0xb1) = 1;
}

// ---- setState_a524e.cpp ----
extern "C" void KIPlayer_setState(KIPlayer *self, int state)
{
    *(int *)((char *)self + 0x88) = state;
}

// ---- jump_a6350.cpp ----
extern "C" void KIPlayer_jump(KIPlayer *self)
{
    *(uint8_t *)((char *)self + 0xf4) = 0;
    *(uint8_t *)((char *)self + 0xec) = 1;
}

// ---- setActive_a5e46.cpp ----
extern "C" void KIPlayer_setActive(KIPlayer *self)
{
    return Player_setActive(*(void **)((char *)self + 0x4));
}

// ---- PlayEngineSound_a6192.cpp ----
extern "C" void KIPlayer_PlayEngineSound(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_playEvent(*(int *)((char *)self + 0x4),
                                   *(int *)((char *)self + 0xf8), 0);
    }
}

// ---- setRoute_a6150.cpp ----
extern "C" void KIPlayer_setRoute(KIPlayer *self, Route *route)
{
    Route *old = *(Route **)((char *)self + 0x6c);
    if (old != 0) {
        operator_delete(Route_dtor(old));
    }
    *(Route **)((char *)self + 0x6c) = 0;
    if (route != 0) {
        *(Route **)((char *)self + 0xb4) = route;
    }
    *(Route **)((char *)self + 0x6c) = route;
}

// ---- isVisible_a5b26.cpp ----
extern "C" uint8_t KIPlayer_isVisible(KIPlayer *self)
{
    return *(uint8_t *)((char *)self + 0xf5);
}

// ---- _KIPlayer_a59f4.cpp ----
extern "C" char KIPlayer_vtable;       // vtable symbol base

// Complete object destructor (D1). Returns this.
extern "C" void *_ZN8KIPlayerD1Ev(KIPlayer *self)
{
    *(void **)self = &KIPlayer_vtable + 8;

    void *p = *(void **)((char *)self + 0x4);
    if (p != 0) operator_delete(Player_dtor(p));
    *(void **)((char *)self + 0x4) = 0;

    Route *r = *(Route **)((char *)self + 0x6c);
    if (r != 0) operator_delete(Route_dtor(r));
    *(void **)((char *)self + 0x6c) = 0;

    void *g1 = *(void **)((char *)self + 0x78);
    if (g1 != 0) operator_delete(AEGeometry_dtor(g1));
    *(void **)((char *)self + 0x78) = 0;

    void *au = *(void **)((char *)self + 0xc4);
    if (au != 0) operator_delete(ArrayUint_dtor(au));
    *(void **)((char *)self + 0xc4) = 0;

    void *g2 = *(void **)((char *)self + 0x8);
    if (g2 != 0) operator_delete(AEGeometry_dtor(g2));
    *(void **)((char *)self + 0x8) = 0;

    void *g3 = *(void **)((char *)self + 0xc);
    if (g3 != 0) operator_delete(AEGeometry_dtor(g3));
    *(void **)((char *)self + 0xc) = 0;

    void *ai = *(void **)((char *)self + 0x50);
    if (ai != 0) operator_delete(ArrayInt_dtor(ai));
    *(void **)((char *)self + 0x50) = 0;

    void *sp = *(void **)((char *)self + 0xcc);
    if (sp != 0) {
        ArrayReleaseClasses_SpacePoint(sp);
        void *sp2 = *(void **)((char *)self + 0xcc);
        if (sp2 != 0) operator_delete(ArraySpacePoint_dtor(sp2));
        *(void **)((char *)self + 0xcc) = 0;
    }

    String_dtor((char *)self + 0x18);
    return self;
}

// ---- awake_a6214.cpp ----
extern "C" void KIPlayer_awake(KIPlayer *self)
{
    void *player = *(void **)((char *)self + 0x4);
    *(uint32_t *)((char *)self + 0x88) = 1;
    return Player_awake(player, 1);
}

// ---- setInitActive_a5e4c.cpp ----
extern "C" void KIPlayer_setInitActive(KIPlayer *self)
{
    KIPlayer_setActive(self);
    *(uint8_t *)((char *)self + 0xb2) = 0;
}

// ---- getCollisionNormal_a5244.cpp ----
extern "C" void KIPlayer_getCollisionNormal(Vector *out)
{
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

// ---- setWingman_a612e.cpp ----
extern "C" void KIPlayer_setWingman(KIPlayer *self, bool b, int cmd)
{
    *(uint8_t *)((char *)self + 0xdc) = b;
    *(int *)((char *)self + 0xe0) = cmd;
    *(uint32_t *)((char *)self + 0xe4) = 1;
}

// ---- setSpacePoints_a5b3e.cpp ----
extern "C" void KIPlayer_setSpacePoints(KIPlayer *self, Array<SpacePoint *> *pts)
{
    *(Array<SpacePoint *> **)((char *)self + 0xcc) = pts;
}

// ---- isJumper_a5dde.cpp ----
extern "C" uint8_t KIPlayer_isJumper(KIPlayer *self)
{
    return *(uint8_t *)((char *)self + 0xec);
}

// ---- outerCollide_a5224.cpp ----
typedef void (*CollideFn)(KIPlayer *, float, float, float);

extern "C" void KIPlayer_outerCollide(KIPlayer *self, const Vector &v)
{
    CollideFn fn = *(CollideFn *)(*(char **)self + 0x3c);
    return fn(self, v.x, v.y, v.z);
}

// ---- setJumper_a5dd8.cpp ----
extern "C" void KIPlayer_setJumper(KIPlayer *self, bool b)
{
    *(uint8_t *)((char *)self + 0xec) = b;
}

// ---- cargoAvailable_a6304.cpp ----
extern "C" int KIPlayer_cargoAvailable(KIPlayer *self)
{
    unsigned int *arr = *(unsigned int **)((char *)self + 0x50);
    if (arr != 0) {
        unsigned int len = arr[0];
        unsigned int i = 0;
        while (i < len) {
            int *data = *(int **)((char *)arr + 4);
            if (data[i + 1] > 0) {
                return 1;
            }
            i += 2;
        }
    }
    return 0;
}

// ---- setDead_a6208.cpp ----
extern "C" void KIPlayer_setDead(KIPlayer *self)
{
    *(uint32_t *)((char *)self + 0x88) = 4;
    return Player_setDead(self, 0);
}

// ---- KIPlayer_a57c8.cpp ----
// 12-byte AbyssEngine::String, constructed/destroyed via engine wrappers.
struct String12 { unsigned a, b, c; };

extern "C" {
void *operator_new(unsigned int sz);                             // 0x6eb24
void *gCanvas;                                                   // engine PaintCanvas holder
extern char KIPlayer_vtable[];                                   // KIPlayer vtable

void String_ctor_default(void *s);                              // 0x6efbc  String::String()
void String_ctor_char(void *s, const char *t, bool copy);       // 0x6ee18  String(char*,bool)
void String_assign(void *dst, void *src);                       // 0x6f2b0  operator=
void String_dtor(void *s);                                      // 0x6ee30  ~String

void AEGeometry_ctor2(void *self, void *canvas);                // 0x720f4
void AEGeometry_addChild(void *self, int child);               // 0x720e8
void *AEGeometry_getMatrix(void);                               // 0x721cc
void AEGeometry_setPosition3f(void *geom, float x, float y, float z);   // 0x73048
void Matrix_assign(void *dst, void *src);                       // 0x6f148
void Matrix_mulassign(void *dst, void *src);                    // 0x6f4a8
void Player_setKIPlayer(void *player, KIPlayer *ki);            // 0x73288

// Two engine constants captured from PC-relative slots (init colour/flags).
extern unsigned KIPlayer_initA;                                 // DAT_000b59dc
extern unsigned KIPlayer_initB;                                 // DAT_000b59e0
}

// KIPlayer::KIPlayer(int faction, int group, Player* player, AEGeometry* geom,
//                    float x, float y, float z, bool active)
extern "C" void KIPlayer_ctor(KIPlayer *self, int faction, int group, void *player, void *geom,
                              float x, float y, float z, bool active)
{
    *(int *)self = (int)(KIPlayer_vtable + 8);
    String_ctor_default((char *)self + 0x18);

    // zero a wide block of fields (the engine writes zero-vectors via NEON stores).
    self->f_90 = 0;  self->f_94 = 0;
    self->f_98 = 0;  self->f_9c = 0;
    self->f_10c = 0; self->f_110 = 0;
    self->f_114 = 0; self->f_118 = 0;
    self->f_2c = 0;  self->f_30 = 0;  self->f_34 = 0;
    self->f_54 = 0;  self->f_6c = 0;  F<unsigned>(self, 0xb4) = 0;
    self->f_a0 = 0;  self->f_a4 = 0;
    self->f_11c = 0; self->f_120 = 0;

    self->f_4 = player;
    self->f_8 = 0;
    self->f_c = 0;
    self->f_c4 = 0;
    self->f_c8 = 0;
    self->f_cc = 0;

    // geometry: when a parent geometry is supplied (and it has children) build a child group.
    bool haveChild = (geom != 0) && (*(int *)((char *)geom + 0x18) != 0);
    if (geom != 0 && haveChild) {
        self->f_c = geom;
        void *child = operator_new(0xc0);
        AEGeometry_ctor2(child, *(void **)gCanvas);
        self->f_8 = child;
        AEGeometry_addChild(child, *(int *)((char *)self->f_c + 0xc));
        *(int *)((char *)self->f_c + 0x24) = *(int *)((char *)self->f_8 + 0xc);
    } else {
        self->f_8 = geom;
        self->f_c = 0;
    }

    self->f_50 = 0;
    self->f_28 = group;
    F<uint8_t>(self, 0x72) = 0;
    F<uint8_t>(self, 0x25) = 0;
    self->f_75 = 0;
    F<uint16_t>(self, 0x42) = 0;
    self->f_44 = 0;
    self->f_48 = -1;
    self->f_8c = 1;
    self->f_3c = 0;
    self->f_4c = 0;
    self->f_d0 = 0;
    self->f_68 = 0;
    F<uint8_t>(self, 0x6a) = 0;
    self->f_dc = 0;
    self->f_ec = 0;
    self->f_f4 = 0;

    {
        String12 tmp;
        String_ctor_char(&tmp, "", false);
        String_assign((char *)self + 0x18, &tmp);
        String_dtor(&tmp);
    }

    self->f_24 = 0;
    F<uint8_t>(self, 0xf5) = 1;
    self->f_e8 = 0;
    self->f_70 = 0;
    self->f_40 = 0;
    self->f_d8 = 0;
    self->f_80 = -1;
    self->f_84 = -1;
    self->f_104 = 0;

    Player_setKIPlayer(self->f_4, self);

    self->f_88 = 0;
    self->f_10 = 0;
    self->f_b1 = 0x100;
    self->f_bc = 0;
    self->f_c0 = 0xff;
    F<unsigned>(self, 0x78) = 0;
    self->f_a8 = KIPlayer_initB;
    self->f_ac = faction;
    self->f_64 = KIPlayer_initA;

    if (geom != 0) {
        AEGeometry_setPosition3f(geom, x, y, z);
        Matrix_assign((char *)self->f_4 + 4, AEGeometry_getMatrix());
        if (self->f_c != 0)
            Matrix_mulassign((char *)self->f_4 + 4, AEGeometry_getMatrix());
    }

    *(float *)((char *)self + 0x58) = x;
    *(float *)((char *)self + 0x5c) = y;
    *(float *)((char *)self + 0x60) = z;
    self->f_14 = 0;
    F<uint16_t>(self, 0x73) = 0;
    F<uint8_t>(self, 0x76) = 0;
    self->f_f8 = -1;
    self->f_fc = 0;
    self->f_100 = 0x100;
    (void)active;
}

// ---- getNearestDockingPoint_a5c54.cpp ----
extern "C" {
void *AEGeometry_getMatrix2(void *geom);                          // 0x721cc
void AEMath_MatrixRotateVector(void *out, void *m, void *v);      // 0x6f694
void AEMath_VectorAdd(void *out, void *a, void *b);              // 0x6f1cc
void AEMath_VectorSub(void *out, void *a, void *b);              // 0x6ec38
float AEMath_VectorLength(void *v);                              // 0x6ec44
}

// KIPlayer::getNearestDockingPoint(AbyssEngine::AEMath::Vector const& dir)
//   Returns the nearest space point of type 2 (docking) to the supplied direction vector,
//   measured from the player's transformed position.
extern "C" void *KIPlayer_getNearestDockingPoint(KIPlayer *self, Vector *dir)
{
    void *arr = self->f_cc;
    if (arr == 0)
        return 0;

    Vector selfPos;
    {
        void **vtbl = *(void ***)self;
        typedef void (*fn_t)(KIPlayer *, Vector *);
        ((fn_t)vtbl[0x28 / 4])(self, &selfPos);
    }

    void *best = 0;
    float bestLen = 1e30f;
    unsigned count = *(unsigned *)arr;
    for (unsigned i = 0; i < count; i = i + 1) {
        void *pt = ((void **)(*(char **)((char *)arr + 4)))[i];
        if (*(int *)((char *)pt + 0x18) != 2)
            continue;

        void *mat = AEGeometry_getMatrix2(self->f_8);
        Vector rotated;
        AEMath_MatrixRotateVector(&rotated, mat, ((void **)(*(char **)((char *)arr + 4)))[i]);
        Vector world;
        AEMath_VectorAdd(&world, &selfPos, &rotated);
        Vector delta;
        AEMath_VectorSub(&delta, &world, dir);
        float len = AEMath_VectorLength(&delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            best = ((void **)(*(char **)((char *)self->f_cc + 4)))[i];
            bestLen = alen;
        }
        count = *(unsigned *)self->f_cc;
    }
    return best;
}

// ---- setPosition_a6394.cpp ----
// KIPlayer::setPosition(float, float, float)
//   Packs the three floats into a Vector and dispatches through vtable slot +0x44
//   (the virtual setPosition(Vector const&)).
extern "C" void KIPlayer_setPosition3(KIPlayer *self, float x, float y, float z)
{
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    void **vtbl = *(void ***)self;
    typedef void (*fn_t)(KIPlayer *, Vector *);
    ((fn_t)vtbl[0x44 / 4])(self, &v);
}

// ---- addGun_a61d8.cpp ----
extern "C" void KIPlayer_addGun_b(KIPlayer *self)
{
    return Player_addGun_b(*(void **)((char *)self + 0x4));
}

// ---- reset_a5de6.cpp ----
extern "C" void KIPlayer_reset(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0) {
        Player_reset(*(void **)((char *)self + 0x4));
    }
    if (*(uint8_t *)((char *)self + 0xb1) != 0 || *(uint8_t *)((char *)self + 0xb2) == 0) {
        KIPlayer_setToSleep(self);
    }
    Route *r = *(Route **)((char *)self + 0xb4);
    if (r != 0) {
        *(Route **)((char *)self + 0x6c) = r;
        Route_reset(r);
    } else if (*(Route **)((char *)self + 0x6c) != 0) {
        Route_reset(*(Route **)((char *)self + 0x6c));
    }
    *(uint32_t *)((char *)self + 0xfc) = 0;
    *(uint8_t *)((char *)self + 0x100) = 0;
}

// ---- captureCrate_a5e60.cpp ----
// PC-relative singleton holders.
extern void *const gAERandom __attribute__((visibility("hidden")));   // DAT_000b6100
extern void *const gStatus __attribute__((visibility("hidden")));     // DAT_000b6104
extern void *const gItemDb __attribute__((visibility("hidden")));     // DAT_000b6108

extern "C" {
void setActive(KIPlayer *self, int v);                        // 0x732f4
int AERandom_nextInt(void *rng, int bound);                  // 0x71848
void *Status_getShip(void);                                  // 0x71a58
int Ship_getFreeSpace(void *ship);                           // 0x722ec
void *Item_makeItem(int infoPtr);                            // 0x718c0
void Level_stealFriendCargo(void *level);                    // 0x73300
void *Status_getStanding(void);                              // 0x71b84
void Standing_applyStealCargo(void *standing, int faction);  // 0x7330c
int Item_getIndex(void *item);                               // 0x718d8
int Item_getAmount(void *item);                              // 0x7183c
int Ship_spaceAvailable(void *ship, int amount);             // 0x73318
void Status_crateCaptured(void *status, int amount);         // 0x73324
void Item_setUnsaleable(void *item, int v);                  // 0x73330
int Item_getType(void *item);                                // 0x718fc
void *Ship_getEquipment(void *ship);                         // 0x7333c
void Item_changeAmount(void *item, int amount);              // 0x71854
void Ship_addCargo(void *ship, void *item);                  // 0x72cdc
void Hud_catchCargo(void *hud, int index, int amount, int flagA, int flagB, int flagC, int flagD); // 0x72ce8 / 0x72cdc area
}

// KIPlayer::captureCrate(Hud* hud)
//   On a crate capture: rolls/limits the amount against ship free space, creates the item,
//   applies steal/standing penalties, moves the cargo into the ship's hold or equipment, bumps
//   the relevant statistics, and notifies the HUD.
extern "C" void KIPlayer_captureCrate(KIPlayer *self, void *hud)
{
    if ((unsigned)(self->f_88 - 3) < 2) {
        self->f_4c = 0;
        if (F<uint8_t>(self, 0x101) != 0)
            setActive(self, 0);
    }

    void *cargo = self->f_50;
    F<unsigned>(self, 0x78) = 0;
    if (cargo == 0)
        return;

    unsigned count = *(unsigned *)cargo;
    for (unsigned i = 0; i < count; i = i + 2) {
        int amount = *(int *)(*(char **)((char *)cargo + 4) + i * 4 + 4);
        if (amount < 1)
            continue;

        // randomise the captured amount unless we are in a "guaranteed" state.
        if ((unsigned)(self->f_88 - 3) >= 2)
            amount = AERandom_nextInt(*(void **)gAERandom, amount);

        void *status = *(void **)gStatus;

        // clamp to the ship's free space.
        Status_getShip();
        int free1 = Ship_getFreeSpace(Status_getShip());
        int amt = amount;
        if (free1 <= amount)
            amt = Ship_getFreeSpace(Status_getShip());
        if (amt < 1) {
            amount = 1;
        } else {
            int free2 = Ship_getFreeSpace(Status_getShip());
            if (free2 <= amount)
                amount = Ship_getFreeSpace(Status_getShip());
        }

        // resolve the item info and decrement the crate's remaining count.
        int itemId = *(int *)(*(char **)((char *)self->f_50 + 4) + i * 4);
        int infoPtr = *(int *)(*(char **)(*(char **)gItemDb + 4) + itemId * 4);
        void *item = Item_makeItem(infoPtr);
        int *slot = (int *)(*(char **)((char *)self->f_50 + 4) + i * 4);
        slot[1] = slot[1] - amount;
        if (item == 0)
            return;

        if (*(char *)((char *)self->f_4 + 0x5d) != 0)
            Level_stealFriendCargo(self->f_54);

        if (self->f_3c == 0)
            Standing_applyStealCargo(Status_getStanding(), self->f_28);

        // determine whether this is a special (illegal) cargo item.
        bool special = false;
        if (self->f_d0 != 0) {
            int idx = Item_getIndex(item);
            if (idx == 0x74)
                special = true;
            else
                special = (Item_getIndex(item) == 0x75);
        }

        void *ship = Status_getShip();
        int avail = Ship_spaceAvailable(ship, Item_getAmount(item));
        int hudIndex;
        int hudAmount;
        int flagA = special ? 1 : 0;
        int flagB;

        if (avail == 0) {
            if (special)
                self->f_68 = 1;
            hudIndex = Item_getIndex(item);
            hudAmount = Item_getAmount(item);
            flagB = 1;
            Hud_catchCargo(hud, hudIndex, hudAmount, flagA, 0, 0, flagB);
            return;
        }

        Status_crateCaptured(status, Item_getAmount(item));
        if (special)
            Item_setUnsaleable(item, 1);

        bool merged = false;
        if (Item_getType(item) == 1) {
            void *equip = Ship_getEquipment(Status_getShip());
            if (equip != 0) {
                unsigned ecount = *(unsigned *)equip;
                for (unsigned j = 0; j < ecount; j = j + 1) {
                    void *e = ((void **)(*(char **)((char *)equip + 4)))[j];
                    if (e != 0 && Item_getIndex(e) == Item_getIndex(item)) {
                        void *e2 = ((void **)(*(char **)((char *)equip + 4)))[j];
                        Item_changeAmount(e2, Item_getAmount(item));
                        merged = true;
                    }
                }
            }
        }
        if (!merged)
            Ship_addCargo(Status_getShip(), item);

        *(int *)((char *)self->f_54 + 0x1c) =
            Item_getAmount(item) + *(int *)((char *)self->f_54 + 0x1c);

        if (special) {
            F<uint8_t>(self, 0x69) = 1;
        } else if (self->f_28 == 9) {
            void *st = *(void **)gStatus;
            *(int *)((char *)st + 0xcc) = Item_getAmount(item) + *(int *)((char *)st + 0xcc);
        } else {
            int idx = Item_getIndex(item);
            if (idx >= 0x84 && Item_getIndex(item) < 0x9a) {
                void *st = *(void **)gStatus;
                char *tbl = *(char **)((char *)st + 0xac);
                *(char *)(*(char **)(tbl + 4) + Item_getIndex(item) - 0x84) = 1;
            }
        }

        hudIndex = Item_getIndex(item);
        hudAmount = Item_getAmount(item);
        Hud_catchCargo(hud, hudIndex, hudAmount, flagA, 0, 0, 0);
        return;
    }
}

// ---- getNearestNavigationPoint_a5b4c.cpp ----
extern "C" {
void *AEGeometry_getMatrix2(void *geom);                          // 0x721cc (this->0x8 matrix)
void AEMath_MatrixRotateVector(void *out, void *m, void *v);      // 0x6f694
void AEMath_VectorAdd(void *out, void *a, void *b);              // 0x6f1cc
void AEMath_VectorSub(void *out, void *a, void *b);              // 0x6ec38
float AEMath_VectorLength(void *v);                              // 0x6ec44
int SpacePoint_isFree(void *sp);                                 // 0x732c4
// virtual getPosition(out) at vtable slot +0x28.
}

// KIPlayer::getNearestNavigationPoint(AbyssEngine::AEMath::Vector const& dir, SpacePoint* target)
//   Among the player's space points of type 1 (navigation), returns the nearest free one
//   (or the explicitly requested `target`) measured against the player's transformed position.
extern "C" void *KIPlayer_getNearestNavigationPoint(KIPlayer *self, Vector *dir, void *target)
{
    void *arr = self->f_cc;
    if (arr == 0)
        return 0;

    // this->getPosition(&selfPos) via vtable +0x28.
    Vector selfPos;
    {
        void **vtbl = *(void ***)self;
        typedef void (*fn_t)(KIPlayer *, Vector *);
        ((fn_t)vtbl[0x28 / 4])(self, &selfPos);
    }

    void *best = 0;
    float bestLen = 1e30f;
    unsigned count = *(unsigned *)arr;
    for (unsigned i = 0; i < count; i = i + 1) {
        void *pt = ((void **)(*(char **)((char *)arr + 4)))[i];
        if (*(int *)((char *)pt + 0x18) != 1)
            continue;

        void *mat = AEGeometry_getMatrix2(self->f_8);
        Vector rotated;
        AEMath_MatrixRotateVector(&rotated, mat, ((void **)(*(char **)((char *)arr + 4)))[i]);
        Vector world;
        AEMath_VectorAdd(&world, &selfPos, &rotated);
        Vector delta;
        AEMath_VectorSub(&delta, &world, dir);
        float len = AEMath_VectorLength(&delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            void *cand = ((void **)(*(char **)((char *)self->f_cc + 4)))[i];
            if (SpacePoint_isFree(cand) != 0 || cand == target) {
                best = cand;
                bestLen = alen;
            } else {
                best = target;
                bestLen = alen;
            }
        }
        count = *(unsigned *)self->f_cc;
    }
    return best;
}

// ---- setEnemies_a6178.cpp ----
extern "C" void KIPlayer_setEnemies(KIPlayer *self)
{
    return Player_setEnemies(*(void **)((char *)self + 0x4));
}

// ---- setShipGroup_a5d40.cpp ----
extern "C" void KIPlayer_setShipGroup(KIPlayer *self, int param2, int flag, int cond)
{
    *(int *)((char *)self + 0x7c) = flag;
    if (param2 == 0 || cond == 0) {
        *(int *)((char *)self + 0x8) = param2;
        void *g = *(void **)((char *)self + 0xc);
        if (g != 0) operator_delete(AEGeometry_dtor(g));
        *(void **)((char *)self + 0xc) = 0;
    } else {
        void *grp = *(void **)((char *)self + 0x8);
        *(int *)((char *)self + 0xc) = param2;
        if (grp == 0) {
            grp = operator_new(0xc0);
            AEGeometry_ctor2(grp, gCanvas);
            *(void **)((char *)self + 0x8) = grp;
        }
        AEGeometry_addChild(grp, *(int *)(*(char **)((char *)self + 0xc) + 0xc));
        *(int *)(*(char **)((char *)self + 0xc) + 0x24) = *(int *)(*(char **)((char *)self + 0x8) + 0xc);
    }
    AEGeometry_setPosition4(*(void **)((char *)self + 0x8),
                            *(int *)((char *)self + 0x58),
                            *(int *)((char *)self + 0x5c),
                            *(int *)((char *)self + 0x60));
    Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    if (*(int *)((char *)self + 0xc) != 0) {
        Matrix_mulassign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    }
}

// ---- setPosition_a6360.cpp ----
extern "C" void KIPlayer_setPosition_vec(KIPlayer *self, const Vector &v)
{
    void *geom = *(void **)((char *)self + 0x8);
    if (geom != 0) {
        AEGeometry_setPosition(geom, v);
        void *m = AEGeometry_getMatrix(*(void **)((char *)self + 0x8));
        Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, m);
        void *cond = *(void **)((char *)self + 0xc);
        if (cond != 0) {
            void *m2 = AEGeometry_getMatrix(cond);
            Matrix_mulassign(*(char **)((char *)self + 0x4) + 0x4, m2);
        }
    }
}

// ---- addGun_a61d2.cpp ----
extern "C" void KIPlayer_addGun_a(KIPlayer *self)
{
    return Player_addGun_a(*(void **)((char *)self + 0x4));
}

// ---- createCrate_a622c.cpp ----
extern "C" void KIPlayer_createCrate(KIPlayer *self, int type)
{
    void *crate = operator_new(0xc0);
    unsigned short id;
    if (type == 1) {
        id = 0x421e;
    } else if (type == 2) {
        id = 0x421f;
    } else if (type == 3) {
        id = 0x4218;
    } else {
        int st = *(int *)((char *)self + 0x28);
        if (st == 1) {
            id = 0x425f;
        } else if (st == 3) {
            id = 0x425e;
        } else if (st == 9) {
            id = 0x4214;
        } else {
            id = 0x4261;
            if (st == 0) {
                id = 0x4260;
            }
        }
    }
    AEGeometry_ctor(crate, id, gCanvasPtr[0], false);
    *(void **)((char *)self + 0x78) = crate;
    Vector pos;
    AEGeometry_getPosition(&pos, *(void **)((char *)self + 0x8));
    AEGeometry_setPosition_v(crate, &pos);
    Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    Player_setKIPlayer(*(void **)((char *)self + 0x4), self);
}
