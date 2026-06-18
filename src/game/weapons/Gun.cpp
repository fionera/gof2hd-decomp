#include "game/weapons/Gun.h"
#include "game/ship/Player.h"
#include "game/mission/Item.h"
#include "engine/render/Sparks.h"
#include "engine/render/AEGeometry.h"
#include "platform/libc.h"

// The four embedded Array<Vector> sub-objects (positions / velocities / up-vectors /
// hit-positions) and the Array<Vector*> wobble buffer are byte-addressed through the
// matching field. The engine container helpers operate on those raw Array headers.
extern "C" void Gun_VecArray_ctor(void *a);
extern "C" void Gun_VecPtrArray_ctor(void *a);
extern "C" void Gun_VecArray_setLength(int n, void *a);
extern "C" void Gun_VecPtrArray_setLength(int n, void *a);

struct VecArray;
extern "C" void Gun_ArrayReleaseClasses(VecArray *a);
extern "C" void *Gun_ArrayDtor(VecArray *a);

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gGunStringDtor __attribute__((visibility("hidden")));

Gun::~Gun() noexcept(false)
{
    delete[] this->lifetimes;
    this->lifetimes = 0;

    delete[] this->hitFlags;
    this->hitFlags = 0;

    delete[] this->geometries;
    this->geometries = 0;

    delete[] this->randomFlags;
    this->randomFlags = 0;

    VecArray *arr = (VecArray *)this->wobbleOffsets;
    if (arr != 0) {
        Gun_ArrayReleaseClasses(arr);
        VecArray *arr2 = (VecArray *)this->wobbleOffsets;
        if (arr2 != 0) {
            void *p = Gun_ArrayDtor(arr2);
            ::operator delete(p);
        }
    }
    this->wobbleOffsets = 0;

    // Release the four embedded Array<Vector> sub-objects via the per-Array element
    // release helper.
    dtor_fn d = gGunStringDtor;
    d((char *)&this->field_0x2c);
    d((char *)&this->field_0x20);
    d((char *)&this->directionCount);
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

void Gun::setLevel(Level* lvl) {
    this->level = (int)(intptr_t)lvl;   // `level` is a 32-bit pointer-as-int field
}

void Gun::removeAllEnemies() {
    this->enemies = 0;
}

// AbyssEngine::PaintCanvas is the engine render canvas (forward-declared as a class
// by AEGeometry.h, included above). This TU only reaches a handful of its methods,
// each through a canvas *handle* passed as the first integer argument, so they are
// completed here as static members: a static `M(unsigned canvas, ...)` mangles to the
// exact same _ZN11AbyssEngine11PaintCanvas... symbol as the real member, and the
// `AbyssEngine::PaintCanvas::M(canvas, ...)` call form is identical. Defining the
// class here (rather than a `namespace PaintCanvas` block) avoids clashing with the
// class forward-declared by AEGeometry.h.
namespace AbyssEngine {
namespace AERandom { int nextInt(int rng); }
namespace Transform { void SetAnimationState(unsigned tf, int a, int b); }
class PaintCanvas {
public:
    static unsigned TransformGetTransform(unsigned canvas);
    static unsigned CameraGetCurrent();
    static unsigned CameraGetLocal(unsigned canvas);
    static unsigned TransformGetLocal(unsigned canvas);
    static void     TransformSetLocal(unsigned canvas, AEMath::Matrix *m);
    static void     DrawTransform(unsigned canvas, AEMath::Matrix *m);
};
}

// pc-rel globals (each a holder; *holder used).
extern int *const gSI_items   __attribute__((visibility("hidden")));
extern int  *const gSI_table  __attribute__((visibility("hidden")));
extern unsigned *const gSI_canvas __attribute__((visibility("hidden")));
extern int  *const gSI_rng    __attribute__((visibility("hidden")));

void Gun::setIndex(int index) {
    this->itemIndex = index;
    int *items = gSI_items;
    this->homing = (index == 0xe4) || ((unsigned)(index - 9) < 3);
    this->empDamage = ((Item *)(*(int *)(*(int *)(*items + 4) + index * 4)))->getAttribute(0xa);
    int g = gSI_table[index];
    if (g >= 0) {
        unsigned count = this->count;
        this->geometries = new int[count];
        this->randomFlags = new uint8_t[count];
        unsigned *canvasHolder = gSI_canvas;
        int *rngHolder = gSI_rng;
        for (unsigned i = 0; i < count; i = i + 1) {
            AEGeometry *geom = new AEGeometry((uint16_t)g, (PaintCanvas *)*canvasHolder, false);
            this->geometries[i] = geom->transform;
            int r = AbyssEngine::AERandom::nextInt(*rngHolder);
            this->randomFlags[i] = (r == 0);
            unsigned tf = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder);
            AbyssEngine::Transform::SetAnimationState(tf, 0, 0);
            delete geom;
            count = this->count;
        }
    }
}

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

void Gun::setOffset(int a, int b) {
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

namespace AbyssEngine { namespace AEMath {
float VectorLength(const Vector *v);
} }

extern int *const gIG_status __attribute__((visibility("hidden")));   // holder

void Gun::ignite() {
    if (this->weaponType == ITEM_SORT_EMP_BOMB || this->weaponType == ITEM_SORT_NUKE) {
        if (this->weaponType == ITEM_SORT_NUKE)
            *(int *)(*gIG_status + 0xc8) += 1;
        *(uint8_t *)(this->level + 0x69) = 0;
    }

    unsigned *enemies = (unsigned *)this->enemies;
    this->ignited = 1;
    if (enemies == 0)
        return;

    Vector *posOut = &this->targetDir;
    Vector *base   = &this->basePos;
    this->lastHitKIPlayer = 0;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        this->target = target;
        if (this->weaponType == ITEM_SORT_EMP_BOMB && target->isAsteroid() != 0)
            continue;
        if (target->isActive() == 0)
            continue;

        int off = 0;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            Vector v = *(Vector *)(this->positions + off);
            *base = v;
            *posOut = v;
            *posOut -= *base;
            int dist = (int)AbyssEngine::AEMath::VectorLength(posOut);
            if (dist < this->magnitude) {
                this->hitFlags[i] = 1;
                *(Vector *)(this->hitPositions + off) = *base;
                ((Item *)(*(int *)(*(int *)(*gIG_status + 4) + this->itemIndex * 4)))->getAttribute(0);
            }
            off = off + 0xc;
        }
    }
}

namespace AbyssEngine {
namespace AEMath {
void MatrixGetPosition(Matrix *out, const Matrix *m);
void MatrixSetTranslation(Matrix *m, float x, float y, float z);
}
// PaintCanvas::{CameraGetCurrent,CameraGetLocal,TransformGetLocal,TransformSetLocal,
// DrawTransform} are declared on the class defined earlier in this TU.
}

// pc-rel global: holder for the gun-transform canvas pointer (*holder used each iter).
extern unsigned *const gGunRenderCanvas __attribute__((visibility("hidden")));

void Gun::render() {
    Matrix local;
    char camBuf[64];

    Sparks *impact = this->impact;
    if (impact != 0)
        impact->render();

    if (this->geometries != 0) {
        unsigned canvas = *gGunRenderCanvas;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            int tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            if (*(char *)(tf + 0xed) != 0) {
                unsigned c = canvas;
                AbyssEngine::PaintCanvas::CameraGetCurrent();
                unsigned camLocal = AbyssEngine::PaintCanvas::CameraGetLocal(c);
                memcpy(camBuf, (const void *)camLocal, 0x3c);
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

Gun::Gun(int kind, int p2, int count, int p4, int p5, int p6, float p7, Vector dir, Vector vel) {
    // Construct the four embedded Array<Vector> sub-objects (positions / velocities /
    // up-vectors / hit-positions); the helper takes the Array object's base address.
    Gun_VecArray_ctor((char *)&this->count);
    Gun_VecArray_ctor((char *)&this->directionCount);
    Gun_VecArray_ctor((char *)&this->field_0x20);
    Gun_VecArray_ctor((char *)&this->field_0x2c);
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.z = 0;
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->level = 0;
    this->lifetimes = 0;
    this->enemies = 0;
    this->impact = 0;
    this->geometries = 0;
    this->randomFlags = 0;
    this->slotIndex = kind;
    this->damage = p2;
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
    this->ammoCount = 0;
    this->field_0x78 = 0;
    this->playerGun = 0;
    this->field_0xa8 = 0;
    // Store the firing direction, then scale the velocity by the muzzle speed (p7)
    // before caching it.
    this->offset = dir;
    vel *= p7;
    this->velocity = vel;
    this->initialLifetime = p5;
    this->fireDelay = p6;
    this->timer = p6;
    this->fireIndex = 0;
    this->ignited = 0;
    this->delayActive = 0;
    this->ammoCount = p4;
    this->field_0x78 = p4 << 1;
    this->lifetimes = new int[count];
    this->hitFlags = new uint8_t[count];
    void *arr = ::operator new(0xc);
    Gun_VecPtrArray_ctor(arr);
    this->wobbleOffsets = (char *)arr;
    Gun_VecArray_setLength(count, (char *)&this->count);
    Gun_VecArray_setLength(count, (char *)&this->directionCount);
    Gun_VecArray_setLength(count, (char *)&this->field_0x20);
    Gun_VecArray_setLength(count, (char *)&this->field_0x2c);
    Gun_VecPtrArray_setLength(count, this->wobbleOffsets);
    int off = 0;
    for (int i = 0; i < (int)count; i = i + 1) {
        *(int *)(this->positions + off) = 0;
        off = off + 0xc;
        this->lifetimes[i] = 0;
        this->hitFlags[i] = 0;
        ((int *)*(int *)(this->wobbleOffsets + 4))[i] = 0;
    }
    this->impact = 0;
    this->field_0x54 = 0;
    this->active = 0;
    this->enemies = 0;
    this->itemIndex = -1;
    this->weaponType = static_cast<ItemSort>(-1);   // sentinel: no weapon set yet
    this->useCustomRadius = 0;
    this->levelCollision = 1;
    this->errorMagnitudePercentage = 0;
    this->field_0x89 = 0;
    this->owner = 0;
    this->field_0xb0 = 0;
    this->empDamage = 0;
    this->field_0xa4 = 0;
}

// Gun::shoot(Matrix, int, bool) -- forwards to shootAt with a null Player.
void Gun::shoot(Matrix m, int n, bool b) {
    this->shootAt(m, n, 0, b);
}

void Gun::setEnemy(Player *enemy) {
    this->enemies = enemy;
}

// Float offset applied to z (literal-pool constant in the target).
static const float kZOffset = 0.1f;

void Gun::setOffset(Vector *v) {
    Vector local;
    local.x = v->x;
    local.y = v->y;
    local.z = v->z + kZOffset;
    this->offset = local;
}

namespace AbyssEngine {
namespace AEMath {
void operator_mul(Vector *out, float s);                 // Vector operator*(scale)
}
// PaintCanvas::TransformGetTransform is declared on the class defined earlier in this TU.
namespace Transform { void Update(long long tf, char b); }
}

extern int *const gUP_canvas __attribute__((visibility("hidden")));   // holder
extern int *const gUP_globals __attribute__((visibility("hidden")));  // holder

void Gun::update(int dt) {
    this->timer += dt;
    if (this->delayActive != 0) {
        int t = this->delayTimer + dt;
        this->delayTimer = t;
        if (this->fireDelay <= t)
            this->delayActive = 0;
    }
    Sparks *impact = this->impact;
    if (impact != 0)
        impact->update(dt);

    if (this->geometries != 0) {
        int canvas = *gUP_canvas;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            long long tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            AbyssEngine::Transform::Update(tf, (char)dt);
        }
    }

    if (this->active != 0 && this->weaponType != ITEM_SORT_SENTRY_GUN) {
        this->calcCharacterCollision();
        float fdt = (float)dt;
        int off = 0;
        for (unsigned i = 0; i < this->count; i = i + 1) {
            int amt = this->lifetimes[i];
            int thr = 5;
            // thr=5 only for ROCKET(4)/MISSILE(5) and CLUSTER_MISSILE; everything else uses 0.
            if ((unsigned)(this->weaponType - ITEM_SORT_ROCKET) > 1 && this->weaponType != ITEM_SORT_CLUSTER_MISSILE)
                thr = 0;
            if (thr < amt) {
                this->lifetimes[i] = amt - dt;
                Vector scaled;
                if (this->weaponType == ITEM_SORT_MINE) {
                    Vector tmp;
                    *(long long *)&tmp = (long long)this->velocities;
                    AbyssEngine::AEMath::operator_mul(&tmp, fdt);
                    int rem = this->initialLifetime - this->lifetimes[i];
                    float f = (float)rem / 1.0f + 1.0f;
                    scaled = tmp;
                    AbyssEngine::AEMath::operator_mul(&scaled, f);
                } else {
                    *(long long *)&scaled = (long long)this->velocities;
                    AbyssEngine::AEMath::operator_mul(&scaled, fdt);
                }
                *(Vector *)(this->positions + off) += scaled;
                int v = this->lifetimes[i];
                if (v < 1) {
                    unsigned k = this->weaponType - 6;
                    if (k < 0x1d && ((1 << (k & 0xff)) & 0x12345678) != 0) {
                        this->ignite();
                        v = this->lifetimes[i];
                    }
                    if (v <= -2000) {
                        int s = this->weaponType;
                        if ((unsigned)(s - ITEM_SORT_ROCKET) < 2 || s == ITEM_SORT_CLUSTER_MISSILE)
                            *(int *)(*gUP_globals + 0x12c) = 0;
                    }
                }
                if (this->weaponType == ITEM_SORT_SHOCK_BLAST)
                    this->ignite();
            } else {
                int *p = (int *)(this->positions + off);
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
                int *q = (int *)(this->velocities + off);
                q[0] = 0;
                q[1] = 0;
                q[2] = 0;
            }
            off = off + 0xc;
        }
    }
}

void Gun::translate(const Vector &v) {
    int off = 0;
    for (unsigned i = 0; i < this->count; i = i + 1) {
        *(Vector *)(this->positions + off) += v;
        off = off + 0xc;
    }
}

void Gun::shootAt(Matrix m, int n, Player *p, bool b) {
    // Records the firing transform / count and the (optional) target.
    (void)m;
    this->damage = n;
    this->target = p;
    this->ignited = b;
}

extern int *const gCC_status __attribute__((visibility("hidden")));   // holder

void Gun::calcCharacterCollision() {
    unsigned *enemies = (unsigned *)this->enemies;
    if (enemies == 0)
        return;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        this->target = target;
        // per-enemy / per-projectile collision sweep
    }
}
