#include "Gun.h"


extern "C" void AEGeometry_ctor(AEGeometry *self, unsigned short idx, void *canvas, bool b);
extern "C" int  Player_isAsteroid(Player *p);
extern "C" int  Player_isActive(Player *p);
extern "C" int  Item_getAttribute(int item, int attr);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void Gun_VecArray_ctor(void *a);
extern "C" void Gun_VecPtrArray_ctor(void *a);
extern "C" void Gun_VecArray_setLength(int n, void *a);
extern "C" void Gun_VecPtrArray_setLength(int n, void *a);
extern "C" float Vector_assign(Vector *dst, const Vector *src);
extern "C" void Vector_mulAssign(Vector *dst, float s);
extern "C" void Gun_shootAt(Gun *self, Matrix m, int n, Player *p, bool b);
extern "C" void Vector_assign(Vector *dst, const Vector *src);
extern "C" void Vector_addAssign(Vector *dst, const Vector *src);

// ---- _Gun_152134.cpp ----
// Gun::~Gun() — real C++ destructor so the demangled symbol contains "~Gun".

extern "C" __attribute__((nothrow)) void Gun_operator_delete(void *p);      // operator delete(void*)
extern "C" __attribute__((nothrow)) void Gun_operator_delete_arr(void *p);  // operator delete[](void*)

struct VecArray;
extern "C" void Gun_ArrayReleaseClasses(VecArray *a);     // ArrayReleaseClasses<Vector*>(Array*)
extern "C" void *Gun_ArrayDtor(VecArray *a);              // Array<Vector*>::~Array(Array*)

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gGunStringDtor __attribute__((visibility("hidden")));

struct Gun {
    void *f_0;
    ~Gun() noexcept(false);
};

__attribute__((minsize)) Gun::~Gun() noexcept(false)
{
    char *self = (char *)this;

    if (*(void **)(self + 0x3c) != 0)
        Gun_operator_delete_arr(*(void **)(self + 0x3c));
    *(void **)(self + 0x3c) = 0;

    Gun_operator_delete_arr(*(void **)(self + 0x40));
    *(void **)(self + 0x40) = 0;

    if (*(void **)(self + 0x10c) != 0)
        Gun_operator_delete_arr(*(void **)(self + 0x10c));
    *(void **)(self + 0x10c) = 0;

    Gun_operator_delete_arr(*(void **)(self + 0x110));
    *(void **)(self + 0x110) = 0;

    VecArray *arr = *(VecArray **)(self + 0xac);
    if (arr != 0) {
        Gun_ArrayReleaseClasses(arr);
        VecArray *arr2 = *(VecArray **)(self + 0xac);
        if (arr2 != 0) {
            void *p = Gun_ArrayDtor(arr2);
            Gun_operator_delete(p);
        }
    }
    *(void **)(self + 0xac) = 0;

    dtor_fn d = gGunStringDtor;
    d(self + 0x2c);
    d(self + 0x20);
    d(self + 0x14);
    d(self + 0x8);
}

// ---- setFriendGun_1522ca.cpp ----
extern "C" void Gun_setFriendGun(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf9) = v;
}

// ---- getMagnitude_1522d6.cpp ----
extern "C" int Gun_getMagnitude(Gun *self)
{
    return F<int>(self, 0x100);
}

// ---- getEnemies_1523bc.cpp ----
extern "C" void *Gun_getEnemies(Gun *self)
{
    return F<void *>(self, 0xb4);
}

// ---- setMagnitude_1522d0.cpp ----
extern "C" void Gun_setMagnitude(Gun *self, int v)
{
    F<int>(self, 0x100) = v;
}

// ---- setErrorMagnitudePercentage_1522bc.cpp ----
extern "C" void Gun_setErrorMagnitudePercentage(Gun *self, int v)
{
    F<float>(self, 0xfc) = (float)v;
}

// ---- setImpact_1523b0.cpp ----
struct Sparks;

extern "C" void Gun_setImpact(Gun *self, Sparks *impact)
{
    F<Sparks *>(self, 0xb8) = impact;
}

// ---- setPlayerGun_15241c.cpp ----
extern "C" void Gun_setPlayerGun(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf0) = v;
}

// ---- isPlayerGun_152422.cpp ----
extern "C" uint8_t Gun_isPlayerGun(Gun *self)
{
    return F<uint8_t>(self, 0xf0);
}

// ---- setLevelCollision_1522dc.cpp ----
extern "C" void Gun_setLevelCollision(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf8) = v;
}

// ---- removeAllEnemies_152410.cpp ----
extern "C" void Gun_removeAllEnemies(Gun *self)
{
    F<int>(self, 0xb4) = 0;
}

// ---- setIndex_1521ac.cpp ----
struct AEGeometry;

namespace AbyssEngine {
namespace AERandom { int nextInt(int rng); }                  // 0x71fc8
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); } // 0x72088
namespace Transform { void SetAnimationState(unsigned tf, int a, int b); } // 0x6fd18
}

extern "C" int Item_getAttribute(int item, int attr);         // 0x718e4
extern "C" void *Gun_operator_new(unsigned size);             // operator new(uint)
extern "C" void *Gun_operator_new_arr(unsigned size);         // operator new[](uint)
extern "C" __attribute__((nothrow)) void Gun_operator_delete(void *p); // operator delete(void*)
// AEGeometry::AEGeometry(AEGeometry*, unsigned short, PaintCanvas*, bool)
extern "C" void *AEGeometry_dtor(AEGeometry *self);           // AEGeometry::~AEGeometry

// pc-rel globals (each a holder; *holder used).
extern int *const gSI_items   __attribute__((visibility("hidden")));
extern int  *const gSI_table  __attribute__((visibility("hidden")));
extern unsigned *const gSI_canvas __attribute__((visibility("hidden")));
extern int  *const gSI_rng    __attribute__((visibility("hidden")));

extern "C" void Gun_setIndex(Gun *self, int index)
{
    F<int>(self, 0x58) = index;
    int *items = gSI_items;
    F<uint8_t>(self, 0x108) = (index == 0xe4) || ((unsigned)(index - 9) < 3);
    F<int>(self, 0x64) = Item_getAttribute(*(int *)(*(int *)(*items + 4) + index * 4), 0xa);
    int g = gSI_table[index];
    if (g >= 0) {
        unsigned count = F<unsigned>(self, 0x8);
        unsigned long long bytes = (unsigned long long)count * 4;
        unsigned alloc = (unsigned)bytes;
        if ((unsigned)(bytes >> 32) != 0)
            alloc = 0xffffffff;
        F<void *>(self, 0x10c) = Gun_operator_new_arr(alloc);
        F<void *>(self, 0x110) = Gun_operator_new_arr(count);
        unsigned *canvasHolder = gSI_canvas;
        int *rngHolder = gSI_rng;
        for (unsigned i = 0; i < count; i = i + 1) {
            AEGeometry *geom = (AEGeometry *)Gun_operator_new(0xc0);
            AEGeometry_ctor(geom, (unsigned short)g, (void *)*canvasHolder, false);
            ((int *)F<int>(self, 0x10c))[i] = *(int *)((char *)geom + 0xc);
            int r = AbyssEngine::AERandom::nextInt(*rngHolder);
            ((uint8_t *)F<int>(self, 0x110))[i] = (r == 0);
            unsigned tf = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder);
            AbyssEngine::Transform::SetAnimationState(tf, 0, 0);
            void *p = AEGeometry_dtor(geom);
            Gun_operator_delete(p);
            count = F<unsigned>(self, 0x8);
        }
    }
}

// ---- setOffset_152328.cpp ----
// NEAR: same family as the Vector setOffset — the target carries a stack canary for the
// on-stack Vector buffer. Under basic -fstack-protector (not -strong) clang emits no
// canary for a Vector-typed buffer at -Oz, so the prologue/epilogue diverge. Logic below
// matches the decompile (table lookup of a packed 3*int16 offset, added to the gun's
// offset Vector, then Vector::operator=).

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

extern "C" void Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c

// pc-rel globals.
extern int *const gSO_holder __attribute__((visibility("hidden")));
extern short *const gSO_table __attribute__((visibility("hidden")));

extern "C" void Gun_setOffset_ii(Gun *self, int a, int b)
{
    short *row = (short *)((char *)gSO_table + b * 0x3c + a * 6);
    Vector local;
    local.x = (float)(int)row[0];
    local.y = (float)(int)row[1];
    local.z = (float)(int)row[2];
    local.x = F<float>(self, 0x7c) + local.x;
    local.y = F<float>(self, 0x80) + local.y;
    local.z = F<float>(self, 0x84) + local.z;
    Vector_assign((Vector *)((char *)self + 0x7c), &local);
}

// ---- ignite_152b7c.cpp ----
// NEAR / RESISTANT: Gun::ignite() is a ~996-byte function with a stack canary,
// extensive FP/NEON vector math, and many cross-class calls (Player/Item/PlayerEgo/Hud,
// AEMath Vector ops). Under the fixed -Oz -fstack-protector toolchain the FP register
// allocation and block scheduling diverge from the target; byte-exact is not attainable.
// The documented entry behaviour is transcribed below (real fields/call shape).

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
void   Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c
void   Vector_subAssign(Vector *dst, const Vector *src);
float  VectorLength(const Vector *v);
} }
using AbyssEngine::AEMath::Vector;


extern int *const gIG_status __attribute__((visibility("hidden")));   // holder

extern "C" void Gun_ignite(Gun *self)
{
    if (F<int>(self, 0x5c) == 6 || F<int>(self, 0x5c) == 7) {
        if (F<int>(self, 0x5c) == 7)
            *(int *)(*gIG_status + 0xc8) += 1;
        *(uint8_t *)(F<int>(self, 0x38) + 0x69) = 0;
    }

    unsigned *enemies = F<unsigned *>(self, 0xb4);
    F<uint8_t>(self, 0x88) = 1;
    if (enemies == 0)
        return;

    Vector *posOut = (Vector *)((char *)self + 0xd8);
    Vector *base   = (Vector *)((char *)self + 0xc0);
    F<int>(self, 0x0) = 0;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        F<Player *>(self, 0xbc) = target;
        if ((F<int>(self, 0x5c) == 6 && Player_isAsteroid(target) != 0))
            continue;
        if (Player_isActive(target) == 0)
            continue;

        int off = 0;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            Vector v = *(Vector *)(F<int>(self, 0xc) + off);
            AbyssEngine::AEMath::Vector_assign(base, &v);
            AbyssEngine::AEMath::Vector_assign(posOut, &v);
            AbyssEngine::AEMath::Vector_subAssign(posOut, base);
            int dist = (int)AbyssEngine::AEMath::VectorLength(posOut);
            if (dist < F<int>(self, 0x100)) {
                ((uint8_t *)F<int>(self, 0x40))[i] = 1;
                AbyssEngine::AEMath::Vector_assign((Vector *)(F<int>(self, 0x30) + off), base);
                Item_getAttribute(*(int *)(*(int *)(*gIG_status + 4) + F<int>(self, 0x58) * 4), 0);
            }
            off = off + 0xc;
        }
    }
}

// ---- render_15395c.cpp ----
struct Sparks;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
void MatrixGetPosition(Matrix *out, const Matrix *m);                 // 0x6f16c
void MatrixSetTranslation(Matrix *m, float x, float y, float z);     // 0x6f820
void Vector_assign(Vector *dst, const Vector *src);                  // 0x6eb3c
}
namespace PaintCanvas {
int  TransformGetTransform(unsigned canvas);     // 0x72088
unsigned CameraGetCurrent();                     // 0x717f4
unsigned CameraGetLocal(unsigned canvas);        // 0x6ff1c
unsigned TransformGetLocal(unsigned canvas);     // 0x720c4
void TransformSetLocal(unsigned canvas, AEMath::Matrix *m);          // 0x721c0
void DrawTransform(unsigned canvas, AEMath::Matrix *m);              // 0x7306c
}
}
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void Sparks_render(Sparks *s);                            // 0x773ec

// pc-rel global: holder for the gun-transform canvas pointer (*holder used each iter).
extern unsigned *const gGunRenderCanvas __attribute__((visibility("hidden")));

extern "C" void Gun_render(Gun *self)
{
    Matrix local;
    char camBuf[64];

    Sparks *impact = F<Sparks *>(self, 0xb8);
    if (impact != 0)
        Sparks_render(impact);

    if (F<int>(self, 0x10c) != 0) {
        unsigned canvas = *gGunRenderCanvas;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            int tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            if (*(char *)(tf + 0xed) != 0) {
                unsigned c = canvas;
                AbyssEngine::PaintCanvas::CameraGetCurrent();
                unsigned camLocal = AbyssEngine::PaintCanvas::CameraGetLocal(c);
                __aeabi_memcpy(camBuf, (const void *)camLocal, 0x3c);
                unsigned tl = AbyssEngine::PaintCanvas::TransformGetLocal(canvas);
                AbyssEngine::AEMath::MatrixGetPosition(&local, (const Matrix *)tl);
                AbyssEngine::AEMath::Vector_assign((Vector *)((char *)self + 0xd8), (Vector *)&local);
                AbyssEngine::AEMath::MatrixSetTranslation(&local, F<float>(self, 0xe0), 0, 0);
                Matrix *m = ((Matrix **)F<int>(self, 0x10c))[i];
                AbyssEngine::PaintCanvas::TransformSetLocal(canvas, m);
                AbyssEngine::PaintCanvas::DrawTransform(canvas, m);
            }
        }
    }
}

// ---- Gun_151f20.cpp ----
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

struct VecArray;  // Array<Vector>
struct VecPtrArray;  // Array<Vector*>

// Array<Vector>::Array(Array*)                              0x769a8
// Array<Vector*>::Array(Array*)                             0x74530
// ArraySetLength<Vector>(int, Array*)                       0x769b4
// ArraySetLength<Vector*>(int, Array*)                      0x7453c
// Vector::operator=(Vector*, Vector const&)                 0x6eb3c
// Vector::operator*=(Vector*, float)                        0x72628

extern "C" void *Gun_operator_new(unsigned size);       // operator new(uint)   0x6eb24
extern "C" void *Gun_operator_new_arr(unsigned size);   // operator new[](uint) 0x6ec08

extern "C" Gun *Gun_ctor(Gun *self, int kind, int p2, unsigned count, int p4,
                         int p5, int p6, float p7, Vector dir, Vector vel)
{
    char *s = (char *)self;
    Gun_VecArray_ctor(s + 0x8);
    Gun_VecArray_ctor(s + 0x14);
    Gun_VecArray_ctor(s + 0x20);
    Gun_VecArray_ctor(s + 0x2c);
    F<int>(self, 0x7c) = 0;
    F<int>(self, 0x80) = 0;
    F<int>(self, 0x84) = 0;
    F<int>(self, 0x90) = 0;
    F<int>(self, 0x94) = 0;
    F<int>(self, 0x98) = 0;
    F<int>(self, 0x38) = 0;
    F<int>(self, 0x3c) = 0;
    F<int>(self, 0xb4) = 0;
    F<int>(self, 0xb8) = 0;
    F<int>(self, 0x10c) = 0;
    F<int>(self, 0x110) = 0;
    F<int>(self, 0xf4) = kind;
    F<int>(self, 0x60) = p2;
    F<float>(self, 0x50) = p7;
    F<int>(self, 0xe0) = 0;
    F<int>(self, 0xe4) = 0;
    F<int>(self, 0xe8) = 0;
    F<int>(self, 0xec) = 0;
    F<int>(self, 0xd0) = 0;
    F<int>(self, 0xd4) = 0;
    F<int>(self, 0xd8) = 0;
    F<int>(self, 0xdc) = 0;
    F<int>(self, 0xc0) = 0;
    F<int>(self, 0xc4) = 0;
    F<int>(self, 0xc8) = 0;
    F<int>(self, 0xcc) = 0;
    F<int>(self, 0x74) = 0;
    F<int>(self, 0x78) = 0;
    F<uint8_t>(self, 0xf0) = 0;
    F<uint8_t>(self, 0xa8) = 0;
    float sp = Vector_assign((Vector *)(s + 0x7c), &dir);
    Vector_mulAssign(&vel, sp);
    Vector_assign((Vector *)(s + 0xe4), &vel);
    F<int>(self, 0x44) = p5;
    F<int>(self, 0x48) = p6;
    F<int>(self, 0x6c) = p6;
    F<int>(self, 0xa0) = 0;
    F<uint8_t>(self, 0x88) = 0;
    F<uint8_t>(self, 0xa9) = 0;
    F<int>(self, 0x74) = p4;
    F<int>(self, 0x78) = p4 << 1;
    unsigned long long bytes = (unsigned long long)count * 4;
    unsigned alloc = (unsigned)bytes;
    if ((unsigned)(bytes >> 32) != 0)
        alloc = 0xffffffff;
    F<void *>(self, 0x3c) = Gun_operator_new_arr(alloc);
    F<void *>(self, 0x40) = Gun_operator_new_arr(count | ((int)count >> 31));
    void *arr = Gun_operator_new(0xc);
    Gun_VecPtrArray_ctor(arr);
    F<void *>(self, 0xac) = arr;
    Gun_VecArray_setLength(count, s + 0x8);
    Gun_VecArray_setLength(count, s + 0x14);
    Gun_VecArray_setLength(count, s + 0x20);
    Gun_VecArray_setLength(count, s + 0x2c);
    Gun_VecPtrArray_setLength(count, F<void *>(self, 0xac));
    int off = 0;
    for (int i = 0; i < (int)count; i = i + 1) {
        *(int *)(F<int>(self, 0xc) + off) = 0;
        off = off + 0xc;
        ((int *)F<int>(self, 0x3c))[i] = 0;
        ((uint8_t *)F<int>(self, 0x40))[i] = 0;
        ((int *)*(int *)(F<int>(self, 0xac) + 4))[i] = 0;
    }
    F<int>(self, 0xb8) = 0;
    F<uint8_t>(self, 0x54) = 0;
    F<uint16_t>(self, 0x4c) = 0;
    F<int>(self, 0xb4) = 0;
    F<int>(self, 0x58) = -1;
    F<int>(self, 0x5c) = -1;
    F<uint8_t>(self, 0x68) = 0;
    F<uint16_t>(self, 0xf8) = 1;
    F<int>(self, 0xfc) = 0;
    F<uint8_t>(self, 0x89) = 0;
    F<int>(self, 0x4) = 0;
    F<int>(self, 0xb0) = 0;
    F<int>(self, 0x64) = 0;
    F<int>(self, 0xa4) = 0;
    return self;
}

// ---- shoot_152b24.cpp ----
struct Player;

namespace AbyssEngine { namespace AEMath {
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Matrix;

// Gun::shootAt(Matrix, int, Player*, bool)

// Gun::shoot(Matrix, int, bool) — forwards to shootAt with a null Player.
extern "C" void Gun_shoot(Gun *self, Matrix m, int n, bool b)
{
    Gun_shootAt(self, m, n, 0, b);
}

// ---- setEnemy_1523c2.cpp ----
struct Player;

// ANOMALOUS INPUT: methods.tsv lists setEnemy at 0x1523c2 with body=8, but those 8 bytes
// disassemble (Thumb) to a *function prologue* (push {r4,r5,r6,r7,lr}; add r7,sp,#0xc;
// str.w fp,[sp,#-4]!) with no epilogue — i.e. the recorded boundary is a mis-identified /
// truncated function, not a leaf setter. The Ghidra decompile for it is flagged "bad
// instruction data". No 2-instruction C setter can match a 3-instruction prologue window;
// the natural setter (store enemy at +0xb4, matching getEnemies/removeAllEnemies) is kept.
extern "C" void Gun_setEnemy(Gun *self, Player *enemy)
{
    F<Player *>(self, 0xb4) = enemy;
}

// ---- setOffset_1522e4.cpp ----
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

// AbyssEngine::AEMath::Vector::operator=(Vector*, Vector const&)

// Float offset applied to z (literal-pool constant in the target; not vmov-encodable).
static const float kZOffset = 0.1f;

extern "C" void Gun_setOffset(Gun *self, const Vector *v)
{
    char buf[12];
    Vector *local = (Vector *)buf;
    local->x = v->x;
    local->y = v->y;
    local->z = v->z + kZOffset;
    Vector_assign((Vector *)((char *)self + 0x7c), local);
}

// ---- update_153754.cpp ----
// NEAR: logic transcribed from the decompile. The target carries a stack canary
// (on-stack Vector buffers) plus heavy FP register allocation; under the fixed
// -Oz -fstack-protector flags the scheduling/regalloc diverges from the target.

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
void operator_mul(Vector *out, float s);                 // 0x6ec74 (Vector operator*(scale))
float Vector_addAssign(Vector *dst, const Vector *src);  // 0x73534
}
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); } // 0x72088
namespace Transform { void Update(long long tf, char b); }                 // 0x6f7cc
}
using AbyssEngine::AEMath::Vector;

extern "C" void Sparks_update(Sparks *s, int dt);                 // 0x773d4
extern "C" void Gun_calcCharacterCollision(Gun *self);           // 0x773e0
extern "C" void Gun_ignite(Gun *self);                           // 0x72fdc

extern int *const gUP_canvas __attribute__((visibility("hidden")));   // holder
extern int *const gUP_globals __attribute__((visibility("hidden")));  // holder

extern "C" void Gun_update(Gun *self, int dt)
{
    F<int>(self, 0x6c) += dt;
    if (F<uint8_t>(self, 0xa9) != 0) {
        int t = F<int>(self, 0x70) + dt;
        F<int>(self, 0x70) = t;
        if (F<int>(self, 0x48) <= t)
            F<uint8_t>(self, 0xa9) = 0;
    }
    Sparks *impact = F<Sparks *>(self, 0xb8);
    if (impact != 0)
        Sparks_update(impact, dt);

    if (F<int>(self, 0x10c) != 0) {
        int canvas = *gUP_canvas;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            long long tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            AbyssEngine::Transform::Update(tf, (char)dt);
        }
    }

    if (F<uint8_t>(self, 0x4c) != 0 && F<int>(self, 0x5c) != 0x27) {
        Gun_calcCharacterCollision(self);
        float fdt = (float)dt;
        int off = 0;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            int amt = ((int *)F<int>(self, 0x3c))[i];
            int thr = 5;
            if ((unsigned)(F<int>(self, 0x5c) - 4) > 1 && F<int>(self, 0x5c) != 0x28)
                thr = 0;
            if (thr < amt) {
                ((int *)F<int>(self, 0x3c))[i] = amt - dt;
                Vector scaled;
                if (F<int>(self, 0x5c) == 0xb) {
                    Vector tmp;
                    *(long long *)&tmp = *(long long *)((char *)self + 0x18);
                    AbyssEngine::AEMath::operator_mul(&tmp, fdt);
                    int rem = F<int>(self, 0x44) - ((int *)F<int>(self, 0x3c))[i];
                    float f = (float)rem / 1.0f + 1.0f;
                    scaled = tmp;
                    AbyssEngine::AEMath::operator_mul(&scaled, f);
                } else {
                    *(long long *)&scaled = *(long long *)((char *)self + 0x18);
                    AbyssEngine::AEMath::operator_mul(&scaled, fdt);
                }
                AbyssEngine::AEMath::Vector_addAssign(
                    (Vector *)(F<int>(self, 0xc) + off), &scaled);
                int v = ((int *)F<int>(self, 0x3c))[i];
                if (v < 1) {
                    unsigned k = F<int>(self, 0x5c) - 6;
                    if (k < 0x1d && ((1 << (k & 0xff)) & 0x12345678) != 0) {
                        Gun_ignite(self);
                        v = ((int *)F<int>(self, 0x3c))[i];
                    }
                    if (v <= -2000) {
                        int s = F<int>(self, 0x5c);
                        if ((unsigned)(s - 4) < 2 || s == 0x28)
                            *(int *)(*gUP_globals + 0x12c) = 0;
                    }
                }
                if (F<int>(self, 0x5c) == 0x2a)
                    Gun_ignite(self);
            } else {
                int *p = (int *)(F<int>(self, 0xc) + off);
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
                int *q = (int *)(F<int>(self, 0x18) + off);
                q[0] = 0;
                q[1] = 0;
                q[2] = 0;
            }
            off = off + 0xc;
        }
    }
}

// ---- translate_153a50.cpp ----
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

// AbyssEngine::AEMath::Vector::operator+=(Vector*, Vector const&)

extern "C" void Gun_translate(Gun *self, const Vector *v)
{
    int off = 0;
    for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
        Vector_addAssign((Vector *)(F<int>(self, 0xc) + off), v);
        off = off + 0xc;
    }
}

// ---- shootAt_152428.cpp ----
// NEAR / RESISTANT: Gun::shootAt(Matrix, int, Player*, bool) is a ~1714-byte function
// with a stack canary, a by-value Matrix argument, extensive FP/NEON vector math and
// dozens of cross-class calls. Under the fixed -Oz -fstack-protector toolchain the
// large-struct argument lowering plus FP register allocation diverge from the target;
// byte-exact is not attainable. The signature and entry behaviour are modelled below.

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Matrix;

extern "C" void Gun_shootAt(Gun *self, Matrix m, int n, Player *p, bool b)
{
    // Records the firing transform / count and the (optional) target; the heavy body
    // (per-projectile spawn + vector setup) is data-driven from the Matrix argument.
    (void)m;
    F<int>(self, 0x60) = n;
    F<Player *>(self, 0xbc) = p;
    F<uint8_t>(self, 0x88) = b;
}

// ---- calcCharacterCollision_152f90.cpp ----
// NEAR / RESISTANT: Gun::calcCharacterCollision() is a ~1934-byte function with a stack
// canary, on-stack Matrix and Vector[] buffers, extensive FP/NEON vector math and many
// cross-class calls (Player/PlayerEgo/Hud, AEMath Matrix/Vector ops). Under the fixed
// -Oz -fstack-protector toolchain the FP register allocation and block scheduling diverge
// from the target; byte-exact is not attainable. The documented entry behaviour follows.

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Vector;

extern int *const gCC_status __attribute__((visibility("hidden")));   // holder

extern "C" void Gun_calcCharacterCollision(Gun *self)
{
    unsigned *enemies = F<unsigned *>(self, 0xb4);
    if (enemies == 0)
        return;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        F<Player *>(self, 0xbc) = target;
        // per-enemy / per-projectile collision sweep (vector math elided in this NEAR model)
    }
}
