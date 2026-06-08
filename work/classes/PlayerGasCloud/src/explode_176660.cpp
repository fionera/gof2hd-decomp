#include "class.h"

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
extern "C" void PlayerGasCloud_explode(void *self, int itemIndex, Vector src, float radius)
{
    void *volatile cookie = __stack_chk_guard;

    if (*(char *)((char *)self + 0x154) == 0) {
        *(int *)((char *)self + 0x88) = 3;
        KIPlayer_setActive(self, false);
        *(unsigned char *)((char *)self + 0x154) = 1;

        void *aGeom = operator_new(0xc);  ArrayGeom_ctor(aGeom);
        *(void **)((char *)self + 0x138) = aGeom;
        void *aVec = operator_new(0xc);   ArrayVec_ctor(aVec);
        *(void **)((char *)self + 0x13c) = aVec;
        void *aLife = operator_new(0xc);  ArrayFloat_ctor(aLife);
        *(void **)((char *)self + 0x140) = aLife;
        void *aLifeMin = operator_new(0xc); ArrayFloat_ctor(aLifeMin);
        *(void **)((char *)self + 0x144) = aLifeMin;
        void *aTimer = operator_new(0xc); ArrayInt_ctor(aTimer);
        *(void **)((char *)self + 0x14c) = aTimer;
        void *aActive = operator_new(0xc); ArrayBool_ctor(aActive);
        *(void **)((char *)self + 0x150) = aActive;
        void *aScale = operator_new(0xc); ArrayFloat_ctor(aScale);
        *(void **)((char *)self + 0x148) = aScale;

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
            AEGeometry_ctor(shard, *(uint16_t *)((char *)self + 0x164),
                            *(void **)g_pgc_canvasRoot, false);
            AEGeometry_setPosition(shard, (Vector *)((char *)self + 0x128));

            void *rng = *(void **)g_pgc_rng;
            float jx = (float)AERandom_next(rng, 10000);
            float jy = (float)AERandom_next(rng, 10000);
            float jz = (float)AERandom_next(rng, 10000);

            Vector p;
            p.x = ((*(float *)((char *)self + 0x128) + delta.x) - spread) + t * jx;
            p.y = ((*(float *)((char *)self + 0x12c) + delta.y) - spread) + t * jy;
            p.z = ((*(float *)((char *)self + 0x130) + delta.z) - spread) + t * jz;

            // Direction = normalized (p - center).
            Vector d, dn;
            Vector_sub(&d, &p, (Vector *)((char *)self + 0x128));
            VectorNormalize(&dn, &d);

            float life = ((float)AERandom_next(rng, 200) / lifeDiv) * 3.0f + 3.0f;
            int timer = AERandom_next(rng, 14000);

            ArrayAdd_float(life * 7.0f, *(void **)((char *)self + 0x140));
            ArrayAdd_float(life, *(void **)((char *)self + 0x144));
            ArrayAdd_int(timer + 8000, *(void **)((char *)self + 0x14c));

            Vector *velCopy = (Vector *)operator_new(0xc);
            *velCopy = dn;
            ArrayAdd_vec(velCopy, *(void **)((char *)self + 0x13c));
            ArrayAdd_geom(shard, *(void **)((char *)self + 0x138));
            ArrayAdd_bool(false, *(void **)((char *)self + 0x150));
            ArrayAdd_float(1.0f, *(void **)((char *)self + 0x148));
        }
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
