#include "gof2/PlayerGasCloud.h"


extern "C" void AEGeometry_translate_v(void *geom, Vector const &v);
extern "C" void AEGeometry_setPosition_v(void *geom, Vector const &v);
extern "C" Vector AEGeometry_getPosition_ret(void *geom);
extern "C" void operator_delete(void *p);
extern "C" void *operator_new(uint32_t);
extern "C" void Player_ctor(void *self, int a, int hp, int b, int c, int d);
extern "C" char PlayerGasCloud_vtable;
extern "C" void Player_setKIPlayer(void *player, void *ki);
extern "C" void Player_setMaxHitpoints(void *player, int hp);
extern "C" void Vector_assign(void *dst, const void *src);   // Vector::operator=(Vector const&)
extern "C" void AEGeometry_ctor(void *self, uint16_t meshId, void *canvas, bool b);
extern "C" void ArrayReleaseClasses_AEGeometry(void *arr);
extern "C" void *Array_AEGeometry_dtor(void *p);
extern "C" void ArrayReleaseClasses_Vector(void *arr);
extern "C" void *Array_Vector_dtor(void *p);
extern "C" void *Array_float_dtor(void *p);
extern "C" void *Array_int_dtor(void *p);
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void *PlayerGasCloud_baseDtor(void *self);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" int PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, int current);
extern "C" void AEGeometry_setScaling(void *geom, float s);
extern "C" void AEGeometry_setDirection(void *geom, Vector const *dir, Vector const *up);
extern "C" void AEGeometry_render(void *geom);

// ---- translate_176658.cpp ----
void PlayerGasCloud::translate(Vector const &param_1)
{
    return AEGeometry_translate_v(this->field_0x8, param_1);
}

// ---- isSparkAlive_176610.cpp ----
bool PlayerGasCloud::isSparkAlive(int param_1)
{
    void *arr = this->field_0x138;
    if (arr == 0)
        return false;
    if (F<unsigned int>(arr, 0x0) <= (unsigned int)param_1)
        return false;
    int *base = F<int *>(this->field_0x14c, 0x4);
    return -1500 < base[param_1];
}

// ---- setSparkInSight_1765f8.cpp ----
void PlayerGasCloud::setSparkInSight(int param_1, bool param_2)
{
    void *arr = this->field_0x138;
    if (arr == 0)
        return;
    unsigned int len = F<unsigned int>(arr, 0x0);
    if ((unsigned int)param_1 < len) {
        char *base = F<char *>(this->field_0x150, 0x4);
        base[param_1] = (char)param_2;
    }
}

// ---- setPosition_176652.cpp ----
void PlayerGasCloud::setPosition(Vector const &param_1)
{
    return AEGeometry_setPosition_v(this->field_0x8, param_1);
}

// ---- getSparks_176640.cpp ----
void *PlayerGasCloud::getSparks()
{
    return this->field_0x138;
}

// ---- getPosition_176646.cpp ----
Vector PlayerGasCloud::getPosition()
{
    return AEGeometry_getPosition_ret(this->field_0x8);
}

// ---- _PlayerGasCloud_1765e8.cpp ----
// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
extern "C" void *_ZN14PlayerGasCloudD1Ev(void *self); // complete object dtor

extern "C" void _ZN14PlayerGasCloudD0Ev(void *self)
{
    return operator_delete(_ZN14PlayerGasCloudD1Ev(self));
}

// ---- hasExploded_1769ac.cpp ----
uint8_t PlayerGasCloud::hasExploded()
{
    return this->field_0x154;
}

// ---- PlayerGasCloud_1763cc.cpp ----
extern "C" void KIPlayer_ctor(void *self, int id, int faction, void *player, void *geom,
                              float x, float y, float z, bool flag);

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas;

PlayerGasCloud::PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                               Vector const &param_4)
{
    void *this_00 = operator_new(0x114);
    Player_ctor(this_00, 0, 9999999, 0, 0, 0);
    KIPlayer_ctor(this, param_1, -1, this_00, param_3, param_4.x, param_4.y, param_4.z, false);

    *(void **)this = &PlayerGasCloud_vtable + 8;
    this->field_0x128 = 0;
    this->field_0x12c = 0;
    this->field_0x130 = 0;
    Player_setKIPlayer(this->field_0x4, this);
    Player_setMaxHitpoints(this->field_0x4, 1);

    void **vt = *(void ***)this;
    typedef void (*pfn)(void *, Vector const &);
    ((pfn)vt[0x11])(this, param_4);

    this->field_0x158 = 0;
    this->field_0x78 = 0;
    this->field_0x25 = 0;

    int iVar1 = 0x4a35;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar1 = param_1 + 0x496c;
    int iVar2 = 0x4a39;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar2 = param_1 + 0x4970;
    this->field_0x160 = param_1;
    this->field_0x164 = iVar2;
    this->field_0x168 = iVar1;

    // Two 16-byte vector stores clear the spark-array pointer block (0x138..0x154).
    this->field_0x138 = 0;
    this->field_0x13c = 0;
    this->field_0x140 = 0;
    this->field_0x144 = 0;
    this->field_0x145 = 0;
    this->field_0x148 = 0;
    this->field_0x14c = 0;
    this->field_0x150 = 0;
    this->field_0x154 = 0;

    Vector_assign((char *)this + 0x128, &param_4);

    void *geom = operator_new(0xc0);
    AEGeometry_ctor(geom, this->field_0x168, *g_pgc_canvas, false);
    this->field_0x134 = geom;
    AEGeometry_setPosition_v(geom, param_4);

    this->field_0x4c = 1;
    this->field_0x44 = 1;
    this->field_0x88 = 0;
    this->field_0xf5 = 1;
    this->field_0x15c = 0;
}

// ---- _PlayerGasCloud_176520.cpp ----
// Complete object destructor (D1). Sets the vtable, tears down the spark arrays and the
// base geometry, then tail-calls the base destructor.

extern "C" void *_ZN14PlayerGasCloudD1Ev(void *selfv)
{
    PlayerGasCloud *self = (PlayerGasCloud *)selfv;
    *(void **)self = &PlayerGasCloud_vtable + 8;

    void *a0 = self->field_0x138;
    if (a0 != 0) {
        ArrayReleaseClasses_AEGeometry(a0);
        void *p = self->field_0x138;
        if (p != 0)
            operator_delete(Array_AEGeometry_dtor(p));
        self->field_0x138 = 0;
    }

    void *a1 = self->field_0x13c;
    if (a1 != 0) {
        ArrayReleaseClasses_Vector(a1);
        void *p = self->field_0x13c;
        if (p != 0)
            operator_delete(Array_Vector_dtor(p));
        self->field_0x13c = 0;
    }

    void *a2 = self->field_0x140;
    if (a2 != 0)
        operator_delete(Array_float_dtor(a2));
    self->field_0x140 = 0;

    void *a3 = self->field_0x144;
    if (a3 != 0)
        operator_delete(Array_float_dtor(a3));
    self->field_0x144 = 0;

    void *a4 = self->field_0x148;
    if (a4 != 0)
        operator_delete(Array_float_dtor(a4));
    self->field_0x148 = 0;

    void *a5 = self->field_0x14c;
    if (a5 != 0)
        operator_delete(Array_int_dtor(a5));
    self->field_0x14c = 0;

    void *g = self->field_0x134;
    if (g != 0)
        operator_delete(AEGeometry_dtor(g));
    self->field_0x134 = 0;

    return PlayerGasCloud_baseDtor(self);
}

// ---- explode_176660.cpp ----
using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud;

// --- engine callees ---------------------------------------------------------
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *operator_new(uint32_t size);

void KIPlayer_setActive(void *self, bool active);

// Array<T> default constructors (all share the 0xc-byte container layout).
void ArrayGeom_ctor(void *self);    // Array<AEGeometry*>
void ArrayVec_ctor(void *self);     // Array<Vector*>
void ArrayFloat_ctor(void *self);   // Array<float>
void ArrayInt_ctor(void *self);     // Array<int>
void ArrayBool_ctor(void *self);    // Array<bool>

void ArrayAdd_int(int value, void *arr);
void ArrayAdd_float(float value, void *arr);
void ArrayAdd_geom(void *value, void *arr);
void ArrayAdd_vec(void *value, void *arr);
void ArrayAdd_bool(bool value, void *arr);

void AEGeometry_ctor(void *self, uint16_t id, void *canvas, bool b);
void AEGeometry_setPosition(void *geom, const Vector *pos);

int Item_getAttribute(void *item);

int AERandom_next(void *rng, int bound);

// Vector math helpers.
void Vector_sub(Vector *out, const Vector *a, const Vector *b); // out = a - b (binary operator-)
float VectorLength(const Vector *v);
void VectorNormalize(Vector *out, const Vector *v);

// Globals reached through GOT.
extern void *g_pgc_itemList;     // **(DAT_186954): root item table
extern void *g_pgc_canvasRoot;   // *(DAT_186958): paint canvas
extern void *g_pgc_rng;          // *(DAT_18695c): random generator

// Tuning constants (DAT_ floats).
extern float g_pgc_countScale;   // DAT_186940
extern float g_pgc_attrDiv;      // DAT_186944
extern float g_pgc_spread;       // DAT_186948
extern float g_pgc_lifeDiv;      // DAT_18694c
}

// PlayerGasCloud::explode(int itemIndex, Vector src, float radius)
extern "C" void PlayerGasCloud_explode(void *selfv, int itemIndex, Vector src, float radius)
{
    PlayerGasCloud *self = (PlayerGasCloud *)selfv;
    void *volatile cookie = __stack_chk_guard;

    if (self->field_0x154 == 0) {
        self->field_0x88 = 3;
        KIPlayer_setActive(self, false);
        self->field_0x154 = 1;

        void *aGeom = operator_new(0xc);  ArrayGeom_ctor(aGeom);
        self->field_0x138 = aGeom;
        void *aVec = operator_new(0xc);   ArrayVec_ctor(aVec);
        self->field_0x13c = aVec;
        void *aLife = operator_new(0xc);  ArrayFloat_ctor(aLife);
        self->field_0x140 = aLife;
        void *aLifeMin = operator_new(0xc); ArrayFloat_ctor(aLifeMin);
        self->field_0x144 = aLifeMin;
        void *aTimer = operator_new(0xc); ArrayInt_ctor(aTimer);
        self->field_0x14c = aTimer;
        void *aActive = operator_new(0xc); ArrayBool_ctor(aActive);
        self->field_0x150 = aActive;
        void *aScale = operator_new(0xc); ArrayFloat_ctor(aScale);
        self->field_0x148 = aScale;

        // Distance from the explosion source to the cloud centre (+0x128).
        Vector delta;
        Vector_sub(&delta, &src, (Vector *)((char *)self + 0x128));
        float dist = VectorLength(&delta);
        float t = 1.5f - dist / radius;
        float countBase = t * g_pgc_countScale;

        // Item-defined intensity multiplier.
        void *itemTable = *(void **)((char *)*(void **)g_pgc_itemList);
        void *item = *(void **)((char *)*(void **)((char *)itemTable + 4) + itemIndex * 4);
        int attr = Item_getAttribute(item);
        float attrF = (float)attr / g_pgc_attrDiv;
        float spread = t * g_pgc_spread;
        float lifeDiv = g_pgc_lifeDiv;

        int count = (int)((countBase / 1.5f + 10.0f) * attrF);
        for (int i = 0; i < count; i++) {
            void *shard = operator_new(0xc0);
            AEGeometry_ctor(shard, self->field_0x164,
                            *(void **)g_pgc_canvasRoot, false);
            AEGeometry_setPosition(shard, (Vector *)((char *)self + 0x128));

            void *rng = *(void **)g_pgc_rng;
            float jx = (float)AERandom_next(rng, 10000);
            float jy = (float)AERandom_next(rng, 10000);
            float jz = (float)AERandom_next(rng, 10000);

            Vector p;
            p.x = ((self->field_0x128 + delta.x) - spread) + t * jx;
            p.y = ((self->field_0x12c + delta.y) - spread) + t * jy;
            p.z = ((self->field_0x130 + delta.z) - spread) + t * jz;

            // Direction = normalized (p - center).
            Vector d, dn;
            Vector_sub(&d, &p, (Vector *)((char *)self + 0x128));
            VectorNormalize(&dn, &d);

            float life = ((float)AERandom_next(rng, 200) / lifeDiv) * 3.0f + 3.0f;
            int timer = AERandom_next(rng, 14000);

            ArrayAdd_float(life * 7.0f, self->field_0x140);
            ArrayAdd_float(life, self->field_0x144);
            ArrayAdd_int(timer + 8000, self->field_0x14c);

            Vector *velCopy = (Vector *)operator_new(0xc);
            *velCopy = dn;
            ArrayAdd_vec(velCopy, self->field_0x13c);
            ArrayAdd_geom(shard, self->field_0x138);
            ArrayAdd_bool(false, self->field_0x150);
            ArrayAdd_float(1.0f, self->field_0x148);
        }
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

// ---- update_1769b4.cpp ----
using AbyssEngine::AEMath::Vector;

// --- engine callees ---------------------------------------------------------
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *Level_getPlayer();
void *PlayerEgo_getTurretPosition(void *ego, Vector *out);
int PlayerEgo_isInTurretMode(void *ego);
int PlayerEgo_getHUD(void *ego);
int PlayerEgo_getCampaignProgress(void *ego);

void AEGeometry_getPosition(void *geom, Vector *out);
void AEGeometry_setPosition(void *geom, const Vector *pos);

void *Status_getShip();
void *Status_getMission(void *m);
int Status_getCurrentCampaignMission();

int Ship_getFreeSpace(void *ship);
int Ship_getFirstEquipmentOfSort(void *ship, int sort);
void Ship_addCargo(void *ship, void *item);

void *Item_makeItem(void *def);
int Item_getAttribute(void *item);

void Hud_catchCargo(int hud, int itemId, bool a, bool b, bool c, bool d, bool e);

float FModSound_stop(void *snd);
void FModSound_play(void *snd, void *a, void *b, float vol);

int Mission_isEmpty(void *mission);

void Level_createRadioMessage(void *level, int id, int type);

void *PaintCanvas_TransformGetTransform(void *canvas);
void Transform_Update(void *transform, int withChildren, bool b);

// Vector math.
void Vector_sub(Vector *out, const Vector *a, const Vector *b);
void Vector_scale(float s, Vector *inout);      // operator*(float, Vector&)
void Vector_add(Vector *out, const Vector *b);  // operator+(out, b) -> out += b
float VectorLength(const Vector *v);
void VectorNormalize(Vector *out, const Vector *v);

// GOT globals.
extern void *g_pgcu_canvasRoot;   // *(DAT_186d78 / DAT_186e14): paint canvas
extern void *g_pgcu_itemDefs;     // *(DAT_186e08): item-definition table
extern void *g_pgcu_pickupSound;  // *(DAT_186e0c): pickup FModSound
extern void *g_pgcu_campaign;     // *(DAT_186e10): campaign state

// Tuning constants.
extern float g_pgcu_velScale;     // DAT_186de4
extern float g_pgcu_catchDist;    // DAT_186de8
extern int   g_pgcu_minTimer;     // DAT_186dec
extern int   g_pgcu_resetTimer;   // DAT_186e00
extern float g_pgcu_fadeLo;       // DAT_186df0
extern float g_pgcu_fadeAdd;      // DAT_186df4
extern float g_pgcu_fadeDiv;      // DAT_186df8
extern float g_pgcu_growDiv;      // DAT_186dfc
}

// PlayerGasCloud::update(int dt)
extern "C" void PlayerGasCloud_update(void *self, int dt)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    if (dt != 0) {
        void *arr = *(void **)(s + 0x138);
        if (*(char *)(s + 0x154) == 0 || *(char *)(s + 0x15c) != 0 || arr == 0) {
            // Idle / pre-explosion: just advance the bound transform.
            void *t = PaintCanvas_TransformGetTransform(*(void **)g_pgcu_canvasRoot);
            Transform_Update(t, 1, (bool)dt);
        } else {
            float dtf = (float)dt;
            *(unsigned char *)(s + 0x15c) = 1;
            *(int *)(s + 0x158) += dt;
            float velStep = dtf * g_pgcu_velScale;

            unsigned int i = 0;
            while (i < *(unsigned int *)arr) {
                int *timer = (int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4);
                *timer -= dt;

                float *life = (float *)(*(int *)(*(int *)(s + 0x140) + 4) + i * 4);
                float newLife = *life - velStep;
                *life = newLife;
                float lifeMin = *(float *)(*(int *)(*(int *)(s + 0x144) + 4) + i * 4);
                if (newLife < lifeMin)
                    *life = lifeMin;

                // Distance from this shard to the player's turret.
                void *player = Level_getPlayer();
                Vector turretPos, shardPos, delta;
                PlayerEgo_getTurretPosition(player, &turretPos);
                void *geom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);
                AEGeometry_getPosition(geom, &shardPos);
                Vector_sub(&delta, &turretPos, &shardPos);
                float dist = VectorLength(&delta);

                bool collected = false;
                if (dist < g_pgcu_catchDist && *(int *)(s + 0x158) >= 2000) {
                    void *p2 = Level_getPlayer();
                    if (PlayerEgo_isInTurretMode(p2) != 0 &&
                        *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4) >= g_pgcu_minTimer) {
                        *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4) = g_pgcu_resetTimer;
                        void *ship = Status_getShip();
                        int itemId = *(int *)(s + 0x160);
                        if (Ship_getFreeSpace(ship) < 1) {
                            if (Level_getPlayer() != 0) {
                                float v = FModSound_stop(g_pgcu_pickupSound);
                                FModSound_play(g_pgcu_pickupSound, (void *)0x8d0, (void *)0, v);
                                void *ego = Level_getPlayer();
                                int hud = PlayerEgo_getHUD(ego);
                                Hud_catchCargo(hud, *(int *)(s + 0x160), false, true, false, true, false);
                            }
                        } else {
                            void *def = *(void **)(*(int *)(*(int *)g_pgcu_itemDefs + 4) + itemId * 4);
                            Item_makeItem(def);
                            if (Level_getPlayer() != 0) {
                                void *ego = Level_getPlayer();
                                int hud = PlayerEgo_getHUD(ego);
                                Hud_catchCargo(hud, *(int *)(s + 0x160), true, false, false, false, false);

                                void *camp = *(void **)g_pgcu_campaign;
                                if (*(char *)((char *)camp + 0x2d) == 0 &&
                                    Status_getCurrentCampaignMission() > 0x8e) {
                                    Vector missionBuf;
                                    Status_getMission(&missionBuf);
                                    if (Mission_isEmpty(&missionBuf) != 0 &&
                                        *(int *)(s + 0x160) == 0xcc) {
                                        *(char *)((char *)camp + 0x2d) = 1;
                                        Level_createRadioMessage(*(void **)(s + 0x54), 0x1a, 0);
                                    }
                                }
                            }
                            float v = FModSound_stop(g_pgcu_pickupSound);
                            FModSound_play(g_pgcu_pickupSound, (void *)0x8d0, (void *)0, v);
                            void *ship2 = Status_getShip();
                            Ship_addCargo(ship2, def);
                        }
                        *(int *)(*(int *)(*(int *)(s + 0x148) + 4) + i * 4) = 0;
                        collected = true;
                    }
                }

                if (!collected) {
                    // Fade / grow the shard's scale based on its remaining timer.
                    int tval = *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4);
                    float *scale = (float *)(*(int *)(*(int *)(s + 0x148) + 4) + i * 4);
                    if (tval <= 0) {
                        float sc = (float)tval / g_pgcu_growDiv + 1.0f;
                        *scale = sc;
                        if (sc < 0.0f)
                            *scale = 0.0f;
                    } else if (dist < g_pgcu_fadeLo) {
                        *scale = (dist + g_pgcu_fadeAdd) / g_pgcu_fadeDiv;
                    }
                    void *t = PaintCanvas_TransformGetTransform(*(void **)g_pgcu_canvasRoot);
                    Transform_Update(t, 1, (bool)dt);
                }

                // Advance the shard's position.
                void *moveGeom;
                Vector moved;
                bool homing = *(char *)(*(int *)(*(int *)(s + 0x150) + 4) + i) != 0 &&
                              *(int *)(s + 0x158) >= 2000;
                if (homing) {
                    void *ship = Status_getShip();
                    if (Ship_getFirstEquipmentOfSort(ship, 0x23) != 0) {
                        // Steer toward the turret.
                        Vector dir, dn;
                        Vector_sub(&dir, &turretPos, &shardPos);
                        VectorNormalize(&dn, &dir);
                        Vector_assign(*(Vector **)(*(int *)(*(int *)(s + 0x13c) + 4) + i * 4), &dn);
                        moveGeom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);

                        void *ship2 = Status_getShip();
                        int eq = Ship_getFirstEquipmentOfSort(ship2, 0x23);
                        int attr = Item_getAttribute((void *)(long)eq);
                        float step = (float)(attr * dt);
                        Vector vel; Vector_assign(&vel, &turretPos);
                        Vector_scale(step, &vel);
                        Vector_add(&vel, &shardPos);
                        moved = vel;
                        AEGeometry_setPosition(moveGeom, &moved);
                        goto advance;
                    }
                }
                {
                    moveGeom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);
                    Vector vel;
                    Vector_assign(&vel, *(Vector **)(*(int *)(*(int *)(s + 0x13c) + 4) + i * 4));
                    Vector_scale(*(float *)(*(int *)(*(int *)(s + 0x140) + 4) + i * 4) * dtf, &vel);
                    Vector_add(&vel, &shardPos);
                    moved = vel;
                    AEGeometry_setPosition(moveGeom, &moved);
                }

            advance:
                if (g_pgcu_minTimer <= *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4))
                    *(unsigned char *)(s + 0x15c) = 0;
                i++;
                arr = *(void **)(s + 0x138);
            }

            if (*(char *)(s + 0x15c) != 0)
                *(unsigned char *)(s + 0xf5) = 0;
        }
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

// ---- render_176e18.cpp ----
namespace AbyssEngine { namespace AEMath {
void MatrixGetDir(Vector *out, Matrix const *m);
void MatrixGetUp(Vector *out, Matrix const *m);
float operator-(Vector &a, Vector const &b);
} }

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas2;

void PlayerGasCloud::render()
{
    Vector local_80;
    Vector dir;
    char cameraLocal[60];

    if (this->field_0xf5 == 0)
        return;
    int mode = this->field_0x88;
    if (mode != 3 && mode != 0)
        return;

    void *cam = *g_pgc_canvas2;
    int cur = PaintCanvas_CameraGetCurrent(cam);
    __aeabi_memcpy(cameraLocal, PaintCanvas_CameraGetLocal(cam, cur), 0x3c);

    AbyssEngine::AEMath::MatrixGetDir((Vector *)&local_80, (Matrix *)cameraLocal);
    float scale = AbyssEngine::AEMath::operator-(dir, *(Vector *)&local_80);

    void *arr;
    if (this->field_0x154 == 0 || (arr = this->field_0x138) == 0) {
        void *geom = this->field_0x134;
        AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
        AEGeometry_setDirection(geom, &dir, (Vector *)&local_80);
        AEGeometry_render(geom);
    } else {
        for (unsigned int i = 0, off = 0; i < F<unsigned int>(arr, 0x0); i++, off += 4) {
            void *g = *(void **)((char *)F<void *>(this->field_0x138, 0x4) + off);
            float si = *(float *)((char *)F<void *>(this->field_0x148, 0x4) + off);
            AEGeometry_setScaling(g, si);
            AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
            AEGeometry_setDirection(g, &dir, (Vector *)&local_80);
            AEGeometry_render(g);
            arr = this->field_0x138;
        }
    }
}
