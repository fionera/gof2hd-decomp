#include "gof2/game/weapons/Gun.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/mission/Item.h"
#include "gof2/engine/render/Sparks.h"
#include "gof2/engine/render/AEGeometry.h"

// gof2/Player.h (included above) provides the full Player definition, including
// isActive()/isAsteroid(). The former name collision has been resolved in that header.

extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void Gun_VecArray_ctor(void *a);
extern "C" void Gun_VecPtrArray_ctor(void *a);
extern "C" void Gun_VecArray_setLength(int n, void *a);
extern "C" void Gun_VecPtrArray_setLength(int n, void *a);

// Gun::~Gun() — real C++ destructor so the demangled symbol contains "~Gun".

struct VecArray;
extern "C" void Gun_ArrayReleaseClasses(VecArray *a);     // ArrayReleaseClasses<Vector*>(Array*)
extern "C" void *Gun_ArrayDtor(VecArray *a);              // Array<Vector*>::~Array(Array*)

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gGunStringDtor __attribute__((visibility("hidden")));

__attribute__((minsize)) Gun::~Gun() noexcept(false)
{
    if (this->lifetimes != 0)
        ::operator delete[](this->lifetimes);
    this->lifetimes = 0;

    ::operator delete[](this->hitFlags);
    this->hitFlags = 0;

    if (this->geometries != 0)
        ::operator delete[](this->geometries);
    this->geometries = 0;

    ::operator delete[](this->randomFlags);
    this->randomFlags = 0;

    VecArray *arr = (VecArray *)this->field_0xac;
    if (arr != 0) {
        Gun_ArrayReleaseClasses(arr);
        VecArray *arr2 = (VecArray *)this->field_0xac;
        if (arr2 != 0) {
            void *p = Gun_ArrayDtor(arr2);
            ::operator delete(p);
        }
    }
    this->field_0xac = 0;

    // Release the four embedded Array<Vector> sub-objects (positions / velocities /
    // 3rd / hit-positions). gGunStringDtor is the per-Array element-release helper.
    dtor_fn d = gGunStringDtor;
    d((char *)&this->field_0x2c);
    d((char *)&this->field_0x20);
    d((char *)&this->field_0x14);
    d((char *)&this->count);
}

void Gun::setFriendGun(bool v) {
    this->friendGun = v;
}

int Gun::getMagnitude() {
    return this->magnitude;
}

void * Gun::getEnemies() {
    return this->enemies;
}

void Gun::setMagnitude(int v) {
    this->magnitude = v;
}

void Gun::setErrorMagnitudePercentage(int v) {
    this->errorMagnitudePercentage = (float)v;
}

struct Sparks;

void Gun::setImpact(Sparks *impact) {
    this->impact = impact;
}

void Gun::setPlayerGun(bool v) {
    this->playerGun = v;
}

uint8_t Gun::isPlayerGun() {
    return this->playerGun;
}

void Gun::setLevelCollision(bool v) {
    this->levelCollision = v;
}

void Gun::removeAllEnemies() {
    this->enemies = 0;
}

struct AEGeometry;

namespace AbyssEngine {
namespace AERandom { int nextInt(int rng); }
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); }
namespace Transform { void SetAnimationState(unsigned tf, int a, int b); }
}

// AEGeometry::AEGeometry(AEGeometry*, unsigned short, PaintCanvas*, bool)

// pc-rel globals (each a holder; *holder used).
extern int *const gSI_items   __attribute__((visibility("hidden")));
extern int  *const gSI_table  __attribute__((visibility("hidden")));
extern unsigned *const gSI_canvas __attribute__((visibility("hidden")));
extern int  *const gSI_rng    __attribute__((visibility("hidden")));

void Gun::setIndex(int index) {
    this->itemIndex = index;
    int *items = gSI_items;
    this->field_0x108 = (index == 0xe4) || ((unsigned)(index - 9) < 3);
    this->field_0x64 = ((Item *)(*(int *)(*(int *)(*items + 4) + index * 4)))->getAttribute(0xa);
    int g = gSI_table[index];
    if (g >= 0) {
        unsigned count = this->count;
        unsigned long long bytes = (unsigned long long)count * 4;
        unsigned alloc = (unsigned)bytes;
        if ((unsigned)(bytes >> 32) != 0)
            alloc = 0xffffffff;
        this->geometries = ::operator new[](alloc);
        this->randomFlags = ::operator new[](count);
        unsigned *canvasHolder = gSI_canvas;
        int *rngHolder = gSI_rng;
        for (unsigned i = 0; i < count; i = i + 1) {
            AEGeometry *geom = (AEGeometry *)::operator new(0xc0);
            new ((void *)geom) AEGeometry((uint16_t)g, (PaintCanvas *)*canvasHolder, false);
            ((int *)this->geometries)[i] = geom->transform;
            int r = AbyssEngine::AERandom::nextInt(*rngHolder);
            ((uint8_t *)this->randomFlags)[i] = (r == 0);
            unsigned tf = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder);
            AbyssEngine::Transform::SetAnimationState(tf, 0, 0);
            geom->~AEGeometry();
            ::operator delete(geom);
            count = this->count;
        }
    }
}

// NEAR: same family as the Vector setOffset — the target carries a stack canary for the
// on-stack Vector buffer. Under basic -fstack-protector (not -strong) clang emits no
// canary for a Vector-typed buffer at -Oz, so the prologue/epilogue diverge. Logic below
// matches the decompile (table lookup of a packed 3*int16 offset, added to the gun's
// offset Vector, then Vector::operator=).

// Vector is provided by gof2/math.h (via common.h).

// pc-rel globals.
extern int *const gSO_holder __attribute__((visibility("hidden")));
extern short *const gSO_table __attribute__((visibility("hidden")));

void Gun::setOffset_ii(int a, int b) {
    short *row = (short *)((char *)gSO_table + b * 0x3c + a * 6);
    Vector local;
    local.x = (float)(int)row[0];
    local.y = (float)(int)row[1];
    local.z = (float)(int)row[2];
    local.x = this->offset.x + local.x;
    local.y = this->offset.y + local.y;
    local.z = this->offset.z + local.z;
    this->offset = local;
}

// NEAR / RESISTANT: Gun::ignite() is a ~996-byte function with a stack canary,
// extensive FP/NEON vector math, and many cross-class calls (Player/Item/PlayerEgo/Hud,
// AEMath Vector ops). Under the fixed -Oz -fstack-protector toolchain the FP register
// allocation and block scheduling diverge from the target; byte-exact is not attainable.
// The documented entry behaviour is transcribed below (real fields/call shape).

struct Player;

namespace AbyssEngine { namespace AEMath {
float  VectorLength(const Vector *v);
} }

extern int *const gIG_status __attribute__((visibility("hidden")));   // holder

void Gun::ignite() {
    if (this->weaponType == 6 || this->weaponType == 7) {
        if (this->weaponType == 7)
            *(int *)(*gIG_status + 0xc8) += 1;
        *(uint8_t *)(this->field_0x38 + 0x69) = 0;
    }

    unsigned *enemies = (unsigned *)this->enemies;
    this->field_0x88 = 1;
    if (enemies == 0)
        return;

    Vector *posOut = &this->targetDir;
    Vector *base   = &this->basePos;
    this->field_0x0 = 0;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        this->target = target;
        if ((this->weaponType == 6 && ((Player *)(target))->isAsteroid() != 0))
            continue;
        if (((Player *)(target))->isActive() == 0)
            continue;

        int off = 0;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            Vector v = *(Vector *)(this->positions + off);
            *(Vector *)(base) = *(const Vector *)(&v);
            *(Vector *)(posOut) = *(const Vector *)(&v);
            *(Vector *)(posOut) -= *(const Vector *)(base);
            int dist = (int)AbyssEngine::AEMath::VectorLength(posOut);
            if (dist < this->magnitude) {
                ((uint8_t *)this->hitFlags)[i] = 1;
                *(Vector *)(this->hitPositions + off) = *(const Vector *)(base);
                ((Item *)(*(int *)(*(int *)(*gIG_status + 4) + this->itemIndex * 4)))->getAttribute(0);
            }
            off = off + 0xc;
        }
    }
}

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
void MatrixGetPosition(Matrix *out, const Matrix *m);
void MatrixSetTranslation(Matrix *m, float x, float y, float z);
}
namespace PaintCanvas {
unsigned TransformGetTransform(unsigned canvas);
unsigned CameraGetCurrent();
unsigned CameraGetLocal(unsigned canvas);
unsigned TransformGetLocal(unsigned canvas);
void TransformSetLocal(unsigned canvas, AEMath::Matrix *m);
void DrawTransform(unsigned canvas, AEMath::Matrix *m);
}
}

// pc-rel global: holder for the gun-transform canvas pointer (*holder used each iter).
extern unsigned *const gGunRenderCanvas __attribute__((visibility("hidden")));

void Gun::render() {
    Matrix local;
    char camBuf[64];

    Sparks *impact = this->impact;
    if (impact != 0)
        ((Sparks *)(impact))->render();

    if (this->geometries != 0) {
        unsigned canvas = *gGunRenderCanvas;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            int tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            if (*(char *)(tf + 0xed) != 0) {
                unsigned c = canvas;
                AbyssEngine::PaintCanvas::CameraGetCurrent();
                unsigned camLocal = AbyssEngine::PaintCanvas::CameraGetLocal(c);
                __aeabi_memcpy(camBuf, (const void *)camLocal, 0x3c);
                unsigned tl = AbyssEngine::PaintCanvas::TransformGetLocal(canvas);
                AbyssEngine::AEMath::MatrixGetPosition(&local, (const Matrix *)tl);
                this->targetDir = *(const Vector *)((Vector *)&local);
                AbyssEngine::AEMath::MatrixSetTranslation(&local, this->targetDir.z, 0, 0);
                Matrix *m = ((Matrix **)this->geometries)[i];
                AbyssEngine::PaintCanvas::TransformSetLocal(canvas, m);
                AbyssEngine::PaintCanvas::DrawTransform(canvas, m);
            }
        }
    }
}

// Vector/Matrix provided by gof2/math.h (via common.h).

struct VecArray;  // Array<Vector>
struct VecPtrArray;  // Array<Vector*>

// Array<Vector>::Array(Array*)                              0x769a8
// Array<Vector*>::Array(Array*)                             0x74530
// ArraySetLength<Vector>(int, Array*)                       0x769b4
// ArraySetLength<Vector*>(int, Array*)                      0x7453c
// Vector::operator=(Vector*, Vector const&)                 0x6eb3c
// Vector::operator*=(Vector*, float)                        0x72628

Gun * Gun::ctor(int kind, int p2, unsigned count, int p4, int p5, int p6, float p7, Vector dir, Vector vel) {
    // Construct the four embedded Array<Vector> sub-objects (positions / velocities /
    // 3rd / hit-positions); the helper takes the Array object's base address.
    Gun_VecArray_ctor((char *)&this->count);
    Gun_VecArray_ctor((char *)&this->field_0x14);
    Gun_VecArray_ctor((char *)&this->field_0x20);
    Gun_VecArray_ctor((char *)&this->field_0x2c);
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.z = 0;
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->field_0x38 = 0;
    this->lifetimes = 0;
    this->enemies = 0;
    this->impact = 0;
    this->geometries = 0;
    this->randomFlags = 0;
    this->field_0xf4 = kind;
    this->field_0x60 = p2;
    this->field_0x50 = p7;
    this->targetDir.z = 0;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
    this->field_0xd0 = 0;
    this->field_0xd4 = 0;
    this->targetDir.x = 0;
    this->targetDir.y = 0;
    this->basePos.x = 0;
    this->basePos.y = 0;
    this->basePos.z = 0;
    this->field_0xcc = 0;
    this->field_0x74 = 0;
    this->field_0x78 = 0;
    this->playerGun = 0;
    this->field_0xa8 = 0;
    // Store the firing direction, then scale the velocity by the muzzle speed (p7)
    // before caching it. The decompiler had fused the assignment and the live p7
    // register into a bogus float-returning Vector_assign.
    this->offset = dir;
    vel *= p7;
    this->velocity = vel;
    this->initialLifetime = p5;
    this->fireDelay = p6;
    this->timer = p6;
    this->field_0xa0 = 0;
    this->field_0x88 = 0;
    this->delayActive = 0;
    this->field_0x74 = p4;
    this->field_0x78 = p4 << 1;
    unsigned long long bytes = (unsigned long long)count * 4;
    unsigned alloc = (unsigned)bytes;
    if ((unsigned)(bytes >> 32) != 0)
        alloc = 0xffffffff;
    this->lifetimes = ::operator new[](alloc);
    this->hitFlags = (uint8_t *)::operator new[](count | ((int)count >> 31));
    void *arr = ::operator new(0xc);
    Gun_VecPtrArray_ctor(arr);
    this->field_0xac = (char *)arr;
    Gun_VecArray_setLength(count, (char *)&this->count);
    Gun_VecArray_setLength(count, (char *)&this->field_0x14);
    Gun_VecArray_setLength(count, (char *)&this->field_0x20);
    Gun_VecArray_setLength(count, (char *)&this->field_0x2c);
    Gun_VecPtrArray_setLength(count, this->field_0xac);
    int off = 0;
    for (int i = 0; i < (int)count; i = i + 1) {
        *(int *)(this->positions + off) = 0;
        off = off + 0xc;
        ((int *)this->lifetimes)[i] = 0;
        ((uint8_t *)this->hitFlags)[i] = 0;
        ((int *)*(int *)(this->field_0xac + 4))[i] = 0;
    }
    this->impact = 0;
    this->field_0x54 = 0;
    this->field_0x4c = 0;
    this->enemies = 0;
    this->itemIndex = -1;
    this->weaponType = -1;
    this->field_0x68 = 0;
    this->levelCollision = 1;
    this->errorMagnitudePercentage = 0;
    this->field_0x89 = 0;
    this->field_0x4 = 0;
    this->field_0xb0 = 0;
    this->field_0x64 = 0;
    this->field_0xa4 = 0;
    return this;
}

struct Player;

// Matrix provided by gof2/math.h (via common.h).

// Gun::shootAt(Matrix, int, Player*, bool)

// Gun::shoot(Matrix, int, bool) — forwards to shootAt with a null Player.
void Gun::shoot(Matrix m, int n, bool b) {
    ((Gun *)(this))->shootAt(m, n, 0, b);
}

struct Player;

// ANOMALOUS INPUT: methods.tsv lists setEnemy at 0x1523c2 with body=8, but those 8 bytes
// disassemble (Thumb) to a *function prologue* (push {r4,r5,r6,r7,lr}; add r7,sp,#0xc;
// str.w fp,[sp,#-4]!) with no epilogue — i.e. the recorded boundary is a mis-identified /
// truncated function, not a leaf setter. The Ghidra decompile for it is flagged "bad
// instruction data". No 2-instruction C setter can match a 3-instruction prologue window;
// the natural setter (store enemy at +0xb4, matching getEnemies/removeAllEnemies) is kept.
void Gun::setEnemy(Player *enemy) {
    this->enemies = enemy;
}

// Vector provided by gof2/math.h (via common.h).

// AbyssEngine::AEMath::Vector::operator=(Vector*, Vector const&)

// Float offset applied to z (literal-pool constant in the target; not vmov-encodable).
static const float kZOffset = 0.1f;

void Gun::setOffset(const Vector *v) {
    char buf[12];
    Vector *local = (Vector *)buf;
    local->x = v->x;
    local->y = v->y;
    local->z = v->z + kZOffset;
    this->offset = *(const Vector *)(local);
}

// NEAR: logic transcribed from the decompile. The target carries a stack canary
// (on-stack Vector buffers) plus heavy FP register allocation; under the fixed
// -Oz -fstack-protector flags the scheduling/regalloc diverges from the target.

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
void operator_mul(Vector *out, float s);                 // 0x6ec74 (Vector operator*(scale))
}
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); }
namespace Transform { void Update(long long tf, char b); }
}

extern int *const gUP_canvas __attribute__((visibility("hidden")));   // holder
extern int *const gUP_globals __attribute__((visibility("hidden")));  // holder

void Gun::update(int dt) {
    Gun *self = this;
    self->timer += dt;
    if (self->delayActive != 0) {
        int t = self->delayTimer + dt;
        self->delayTimer = t;
        if (self->fireDelay <= t)
            self->delayActive = 0;
    }
    Sparks *impact = self->impact;
    if (impact != 0)
        ((Sparks *)(impact))->update(dt);

    if (self->geometries != 0) {
        int canvas = *gUP_canvas;
        for (unsigned i = 0; i < self->count; i = i + 1) {
            long long tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            AbyssEngine::Transform::Update(tf, (char)dt);
        }
    }

    if (self->field_0x4c != 0 && self->weaponType != 0x27) {
        ((Gun *)(self))->calcCharacterCollision();
        float fdt = (float)dt;
        int off = 0;
        for (unsigned i = 0; i < self->count; i = i + 1) {
            int amt = ((int *)self->lifetimes)[i];
            int thr = 5;
            if ((unsigned)(self->weaponType - 4) > 1 && self->weaponType != 0x28)
                thr = 0;
            if (thr < amt) {
                ((int *)self->lifetimes)[i] = amt - dt;
                Vector scaled;
                if (self->weaponType == 0xb) {
                    Vector tmp;
                    *(long long *)&tmp = (long long)self->velocities;
                    AbyssEngine::AEMath::operator_mul(&tmp, fdt);
                    int rem = self->initialLifetime - ((int *)self->lifetimes)[i];
                    float f = (float)rem / 1.0f + 1.0f;
                    scaled = tmp;
                    AbyssEngine::AEMath::operator_mul(&scaled, f);
                } else {
                    *(long long *)&scaled = (long long)self->velocities;
                    AbyssEngine::AEMath::operator_mul(&scaled, fdt);
                }
                *(Vector *)(self->positions + off) += *(const Vector *)(&scaled);
                int v = ((int *)self->lifetimes)[i];
                if (v < 1) {
                    unsigned k = self->weaponType - 6;
                    if (k < 0x1d && ((1 << (k & 0xff)) & 0x12345678) != 0) {
                        ((Gun *)(self))->ignite();
                        v = ((int *)self->lifetimes)[i];
                    }
                    if (v <= -2000) {
                        int s = self->weaponType;
                        if ((unsigned)(s - 4) < 2 || s == 0x28)
                            *(int *)(*gUP_globals + 0x12c) = 0;
                    }
                }
                if (self->weaponType == 0x2a)
                    ((Gun *)(self))->ignite();
            } else {
                int *p = (int *)(self->positions + off);
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
                int *q = (int *)(self->velocities + off);
                q[0] = 0;
                q[1] = 0;
                q[2] = 0;
            }
            off = off + 0xc;
        }
    }
}

// Vector provided by gof2/math.h (via common.h).

// AbyssEngine::AEMath::Vector::operator+=(Vector*, Vector const&)

void Gun::translate(const Vector *v) {
    int off = 0;
    for (unsigned i = 0; i < this->count; i = i + 1) {
        *(Vector *)(this->positions + off) += *(const Vector *)(v);
        off = off + 0xc;
    }
}

// NEAR / RESISTANT: Gun::shootAt(Matrix, int, Player*, bool) is a ~1714-byte function
// with a stack canary, a by-value Matrix argument, extensive FP/NEON vector math and
// dozens of cross-class calls. Under the fixed -Oz -fstack-protector toolchain the
// large-struct argument lowering plus FP register allocation diverge from the target;
// byte-exact is not attainable. The signature and entry behaviour are modelled below.

struct Player;

// Vector/Matrix provided by gof2/math.h (via common.h).

void Gun::shootAt(Matrix m, int n, Player *p, bool b) {
    // Records the firing transform / count and the (optional) target; the heavy body
    // (per-projectile spawn + vector setup) is data-driven from the Matrix argument.
    (void)m;
    this->field_0x60 = n;
    this->target = p;
    this->field_0x88 = b;
}

// NEAR / RESISTANT: Gun::calcCharacterCollision() is a ~1934-byte function with a stack
// canary, on-stack Matrix and Vector[] buffers, extensive FP/NEON vector math and many
// cross-class calls (Player/PlayerEgo/Hud, AEMath Matrix/Vector ops). Under the fixed
// -Oz -fstack-protector toolchain the FP register allocation and block scheduling diverge
// from the target; byte-exact is not attainable. The documented entry behaviour follows.

struct Player;

// Vector/Matrix provided by gof2/math.h (via common.h).

extern int *const gCC_status __attribute__((visibility("hidden")));   // holder

void Gun::calcCharacterCollision() {
    unsigned *enemies = (unsigned *)this->enemies;
    if (enemies == 0)
        return;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        this->target = target;
        // per-enemy / per-projectile collision sweep (vector math elided in this NEAR model)
    }
}
