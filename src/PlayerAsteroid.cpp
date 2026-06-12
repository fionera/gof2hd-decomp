#include "gof2/PlayerAsteroid.h"
#include "gof2/AEGeometry.h"
#include "gof2/Level.h"
#include "gof2/Explosion.h"
#include "gof2/KIPlayer.h"
#include "gof2/Player.h"

// AEGeometry / Transform are only touched via opaque pointers plus two byte-offset
// field reads; we forward-declare them and provide minimal accessors below rather
// than pulling in their full (mutually conflicting) headers.
struct AEGeometry;
struct Transform;

// AEGeometry::field_0xc (int32 model/transform id) — minimal accessor view.
struct AEGeometryFields { int32_t field_0xc; };
// Transform::field_0xe0 (float bounding radius) — minimal accessor view.
struct TransformFields { char _pad[0xe0]; float field_0xe0; };
// Player::field_0x40 (int collision radius) — minimal accessor view.
struct PlayerRadiusFields { char _pad[0x40]; int field_0x40; };


extern "C" void *PlayerAsteroid_complete_dtor(PlayerAsteroid *self);
extern "C" void Explosion_update(Explosion *explosion, int delta, TargetFollowCamera *camera);
Vector Player_getHitVector(Player *player);
namespace AbyssEngine { namespace AERandom { int nextInt(int rng, int bound); } }
extern "C" void ArrayInt_ctor(ArrayInt *array);
extern "C" void ArrayAdd_int(int value, ArrayInt *array);
extern "C" void Explosion_setMatrix(Explosion *explosion, Matrix *matrix);
Vector VectorNormalize(const Vector *vector);
namespace AbyssEngine { namespace PaintCanvas { ::Transform *TransformGetTransform(void *canvas, uint32_t handle); } }
extern "C" void Explosion_ctor(Explosion *self, int type);
float VectorLength(const Vector *vector);
void MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z);
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *KIPlayer_dtor(PlayerAsteroid *self);

// Free math operators (defined in libgof2hd / AEMath.cpp); declared here to avoid
// pulling in gof2/AEMath.h, which conflicts with the local opaque forward-decls above.
namespace AbyssEngine { namespace AEMath {
Vector operator*(const Vector &lhs, float rhs);
Vector operator-(const Vector &lhs, const Vector &rhs);
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
} }

// ---- setAsteroidIndex_e29c4.cpp ----
void PlayerAsteroid::setAsteroidIndex(int asteroidIndex)
{
    this->asteroidIndex = asteroidIndex;
}

// ---- translate_e29dc.cpp ----
void PlayerAsteroid::translate(const Vector &delta)
{
    return ((AEGeometry *)(this->geometry))->translate(delta);
}

// ---- render_e3038.cpp ----
void PlayerAsteroid::render()
{
    if (this->visible != 0) {
        if (this->secondaryGeometry != 0) {
            ((AEGeometry *)(this->secondaryGeometry))->render();
        }
        int state = this->state;
        if (state != 0) {
            if (state == 3) {
                return ((Explosion *)(this->explosion))->render();
            }
            return;
        }
        return ((AEGeometry *)this->geometry)->render();
    }
}

// ---- setPosition_e29d6.cpp ----
void PlayerAsteroid::setPosition(const Vector &position)
{
    return ((AEGeometry *)(this->geometry))->setPosition(position);
}

// ---- outerCollide_e3078.cpp ----
typedef void (*OuterCollideVectorFn)(PlayerAsteroid *, Vector);

void PlayerAsteroid::outerCollide(Vector value)
{
    return (*(OuterCollideVectorFn *)((char *)*(void **)this + 0x38))(this, value);
}

// ---- getPosition_e29ca.cpp ----
Vector PlayerAsteroid::getPosition()
{
    return ((AEGeometry *)(this->geometry))->getPosition();
}

// ---- setRotationEnabled_e2a90.cpp ----
void PlayerAsteroid::setRotationEnabled(bool enabled)
{
    this->rotationEnabled = enabled;
}

// ---- getQualityFrameIndex_e29f4.cpp ----
int PlayerAsteroid::getQualityFrameIndex()
{
    return 7 - this->quality;
}

// ---- _PlayerAsteroid_e29b4.cpp ----
void _ZN14PlayerAsteroidD0Ev(PlayerAsteroid *self)
{
    return ::operator delete(PlayerAsteroid_complete_dtor(self));
}

// ---- outerCollide_e306e.cpp ----
typedef void (*OuterCollideFn)(PlayerAsteroid *, float, float, float);

void PlayerAsteroid::outerCollide(float x, float y, float z)
{
    return (*(OuterCollideFn *)((char *)*(void **)this + 0x38))(this, x, y, z);
}

// ---- getQuality_e29e8.cpp ----
int PlayerAsteroid::getQuality()
{
    return this->quality;
}

// ---- getScaling_e29ee.cpp ----
float PlayerAsteroid::getScaling()
{
    return this->scaling;
}

// ---- isMinable_e29e2.cpp ----
uint8_t PlayerAsteroid::isMinable()
{
    return this->minable;
}

// ---- getQualityString_e2a00.cpp ----
__attribute__((visibility("hidden"))) extern const char *PlayerAsteroid_qualityNames[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityDefault[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityFour[];

String PlayerAsteroid::getQualityString()
{
    int quality = this->quality;
    unsigned int index = quality - 5U;
    const char *text;
    if (index < 3U) {
        text = PlayerAsteroid_qualityNames[index];
    } else {
        text = PlayerAsteroid_qualityFour;
        if (quality != 4) {
            text = PlayerAsteroid_qualityDefault;
        }
    }
    String result;
    for (const char *p = text; *p; ++p)
        result.s.push_back((char16_t)(unsigned char)*p);
    return result;
}

// ---- update_e2d20.cpp ----
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_level;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;


void PlayerAsteroid::update(int delta)
{
    this->lastDelta = delta;
    if (delta == 0) {
        return;
    }

    Player *player = this->player;
    if (((Player *)(player))->isActive() == 0 && this->state == 4) {
        this->field_0x124 = 0;
        return;
    }

    int hitpoints = ((Player *)(player))->getHitpoints();
    int state = this->state;
    if (hitpoints <= 0 && state == 0) {
        this->state = 3;
        ((Level *)(PlayerAsteroid_level))->asteroidDied();

        if (this->dropsLoot != 0) {
            int quality = this->quality;
            void *random = PlayerAsteroid_random;
            bool spawn = true;
            if (quality == 7) {
                spawn = AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 100) < 4;
            } else if (quality > 6 || AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 100) > 0x13) {
                spawn = false;
            }

            if (spawn) {
                ArrayInt *items = (ArrayInt *)::operator new(0xc);
                ArrayInt_ctor(items);
                this->loot = items;
                int item = this->asteroidIndex;
                if (item == 0xd9) {
                    if (quality == 7) {
                        item = 0xda;
                    }
                } else if (quality == 7) {
                    item += 0xb;
                }
                ArrayAdd_int(item, items);
                int count = 1;
                if (quality != 7) {
                    count = AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 3) + 1;
                }
                ArrayAdd_int(count, this->loot);
                ((KIPlayer *)(this))->createCrate(this->asteroidIndex == 0xa4 ? 2 : 1);
            } else {
                this->dropsLoot = 0;
                this->loot = 0;
            }
        } else {
            this->dropsLoot = 0;
            this->loot = 0;
        }

        Matrix geometryMatrix = ((AEGeometry *)(this->geometry))->getMatrix();
        Explosion_setMatrix(this->explosion, &geometryMatrix);
        return;
    }

    if (state == 3) {
        Explosion_update(this->explosion, delta, 0);
        if (((Explosion *)(this->explosion))->isPlaying() == 0) {
            this->state = 4;
            ((Player *)(player))->setBombForce(0.0f);
        }
    } else if (state == 4) {
        ((Player *)(player))->setBombForce(0.0f);
    }

    int oldHitpoints = this->lastHitpoints;
    hitpoints = ((Player *)(player))->getHitpoints();
    if (hitpoints < oldHitpoints) {
        this->hitFlashActive = 1;
        this->hitFlashTimer = 1.0f;
        this->lastHitpoints = ((Player *)(player))->getHitpoints();
    }

    if (this->rotationEnabled != 0) {
        Vector rotation = *(const Vector*)((char *)this + 0x140) * ((float)delta * 0.001f);
        ((AEGeometry *)(this->geometry))->rotate(rotation);
    }

    float bombForce = ((Player *)(player))->getBombForce();
    if (bombForce > 0.0f && this->state == 3) {
        Vector hit = Player_getHitVector(player);
        Vector *hitSlot = (Vector *)((char *)this + 0x90);
        *(Vector*)(hitSlot) = *(const Vector*)(&hit);
        float scaling = this->scaling;
        float clamped = scaling;
        if (clamped > 1.0f) {
            clamped = 1.0f;
        }
        if (scaling < 0.5f) {
            clamped = 1.0f;
        }
        float force = (float)(int)(bombForce * 0.1f * (clamped / 2.0f + 1.0f));
        *(Vector*)(hitSlot) *= (force);
        typedef void (*PushFn)(PlayerAsteroid *, Vector *);
        (*(PushFn *)((char *)*(void **)this + 0x20))(this, hitSlot);
        ((Explosion *)(this->explosion))->translate(hitSlot);
        if (this->secondaryGeometry != 0) {
            ((AEGeometry *)(this->secondaryGeometry))->translate(*hitSlot);
        }
        float nextForce = bombForce * 0.75f;
        if (nextForce < 0.01f) {
            nextForce = 0.0f;
        }
        ((Player *)(player))->setBombForce(nextForce);
    }
}

// ---- getProjectionVector_e3084.cpp ----
typedef void (*VectorAssignFn)(Vector *self, const Vector *other);

__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_projectionScratch;
__attribute__((visibility("hidden"))) extern VectorAssignFn PlayerAsteroid_vectorAssign;


Vector PlayerAsteroid::getProjectionVector(const Vector &value)
{
    Vector result = value;
    Vector position = ((AEGeometry *)(this->geometry))->getPosition();
    Vector *scratch = PlayerAsteroid_projectionScratch;
    VectorAssignFn assign = PlayerAsteroid_vectorAssign;
    assign(scratch, &position);
    *(Vector*)(scratch) -= *(const Vector*)(&result);
    assign(&result, scratch);
    Vector normalized = VectorNormalize(&result);
    assign(&result, &normalized);
    return result;
}

// ---- PlayerAsteroid_e2648.cpp ----
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_canvas;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;
__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_rotationVector;
__attribute__((visibility("hidden"))) extern int *PlayerAsteroid_someCounter;


PlayerAsteroid::PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                               const Vector &position, float scaling, int quality)
{
    Player *player = (Player *)::operator new(0x114);
    ((Player *)(player))->ctor(0x5dc, 0x1e, 0, 0, 0);

    this->vtable = (char *)PlayerAsteroid_vtable + 8;
    this->spinX = 0;
    this->spinY = 0;
    this->spinZ = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    ((Player *)(this->player))->setKIPlayer((KIPlayer *)this);

    this->field_0x124 = 0;
    this->asteroidIndex = asteroidIndex;
    this->scaling = scaling;
    this->quality = quality;

    Player *ownedPlayer = this->player;
    void *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*(void **)PlayerAsteroid_canvas,
                                                        ((AEGeometryFields *)geometry)->field_0xc);
    ((Player *)(ownedPlayer))->setRadius((int)(((TransformFields *)transform)->field_0xe0 * scaling * 0.5f));
    ((Player *)(this->player))->setMaxHitpoints((int)(scaling * 100.0f + 30.0f));
    int hitpoints = ((Player *)(this->player))->getHitpoints();
    this->minable = quality > 3;
    this->lastHitpoints = hitpoints;

    Explosion *explosion = (Explosion *)::operator new(0x68);
    Explosion_ctor(explosion, explosionType + 2);
    this->explosion = explosion;
    ((Explosion *)(explosion))->setScaling(scaling);
    ((AEGeometry *)(this->geometry))->setScaling(scaling);
    this->setPosition(position);

    void *random = PlayerAsteroid_random;
    Vector spin = {
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 0x2000) - 0x1000) * 0.001f,
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 0x2000) - 0x1000) * 0.001f,
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 0x2000) - 0x1000) * 0.001f,
    };
    *(Vector*)(PlayerAsteroid_rotationVector) = *(const Vector*)(&spin);

    AEGeometry *ownedGeometry = this->geometry;
    ((AEGeometry *)(ownedGeometry))->setRotation((float)AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 100) * 0.01f * 6.2831855f, (float)AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 100) * 0.01f * 6.2831855f, (float)AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 100) * 0.01f * 6.2831855f);

    this->secondaryGeometry = 0;
    this->rotationEnabled = 1;
    Vector axis = {
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 3) - 1),
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 3) - 1),
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 3) - 1),
    };
    Vector scaledAxis = *(const Vector*)(&axis) * (1.0f);
    *(Vector*)((char *)this + 0x140) = *(const Vector*)(&scaledAxis);

    this->dropsLoot = 1;
    this->field_0x3c = 1;
    this->state = 0;
    *PlayerAsteroid_someCounter = 0;
    this->hitFlashActive = 0;
    this->hitFlashTimer = 0.0f;
}

// ---- setAsteroidCenter_e2a3c.cpp ----
__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_center;
__attribute__((visibility("hidden"))) extern int *PlayerAsteroid_centerLength;


void PlayerAsteroid::setAsteroidCenter(Vector center)
{
    volatile char localBytes[13];
    Vector *local = (Vector *)(void *)localBytes;
    *local = center;
    *(Vector*)(PlayerAsteroid_center) = *(const Vector*)(local);
    *PlayerAsteroid_centerLength = (int)VectorLength(local);
}

// ---- collide_e3104.cpp ----
typedef Vector (*GeometryPositionFn)(AEGeometry *geometry);

__attribute__((visibility("hidden"))) extern GeometryPositionFn PlayerAsteroid_geometryPosition;


bool PlayerAsteroid::collide(float x, float y, float z)
{
    PlayerAsteroid *self = this;
    if (((Player *)(self->player))->getHitpoints() > 0) {
        GeometryPositionFn getPosition = PlayerAsteroid_geometryPosition;
        float px = getPosition(self->geometry).x;
        float py = getPosition(self->geometry).y;
        float pz = getPosition(self->geometry).z;
        int radiusInt = ((PlayerRadiusFields *)self->player)->field_0x40;
        float radius = (float)radiusInt;
        float dx = x - px;
        if (dx < radius) {
            float negativeRadius = (float)-radiusInt;
            if (dx > negativeRadius) {
                float dy = y - py;
                if (dy < radius && dy > negativeRadius) {
                    float dz = z - pz;
                    if (dz < radius) {
                        if (dz > negativeRadius) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// ---- push_e2bd8.cpp ----
void PlayerAsteroid::push(int delta)
{
    int remaining = this->pushTimer;
    if (remaining > 0) {
        remaining -= delta;
        this->pushTimer = remaining;
        float t = (float)remaining / (float)this->pushDuration;

        Matrix identity;
        {
            static const float kIdentity[16] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            };
            for (int i = 0; i < 16; ++i) identity.m[i] = kIdentity[i];
        }
        Matrix rotation;
        MatrixSetRotation(&rotation, &identity, t * this->pushSpinX,
                          t * this->pushSpinY, t * this->pushSpinZ);

        int frameDelta = this->lastDelta;
        AEGeometry *geometry = this->geometry;
        if (frameDelta > 0) {
            Matrix geometryMatrix = ((AEGeometry *)(geometry))->getMatrix();
            Matrix combined = *(const Matrix*)(&rotation) * *(const Matrix*)(&geometryMatrix);
            ((AEGeometry *)geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(&combined));
            frameDelta = this->lastDelta;
        }

        Vector baseMove = *(const Vector*)((char *)this + 0x10c) * ((float)frameDelta);
        float scale = (2.0f - t) * 3.0f * ((float)this->pushDuration / 1000.0f);
        Vector move = *(const Vector*)(&baseMove) * (scale);
        ((AEGeometry *)(geometry))->translate(move);
    }
}

// ---- _PlayerAsteroid_e2980.cpp ----
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;

void *_ZN14PlayerAsteroidD1Ev(PlayerAsteroid *self)
{
    self->vtable = (char *)PlayerAsteroid_vtable + 8;
    Explosion *explosion = self->explosion;
    if (explosion != 0) {
        ::operator delete(Explosion_dtor(explosion));
    }
    self->explosion = 0;
    return KIPlayer_dtor(self);
}

// ---- initPush_e2a98.cpp ----
typedef Vector (*VirtualVectorFn)(PlayerAsteroid *self);

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;


void PlayerAsteroid::initPush(const Vector &target, int duration)
{
    VirtualVectorFn getVector = *(VirtualVectorFn *)((char *)*(void **)this + 0x28);
    Vector current = getVector(this);
    Vector delta = *(const Vector*)(&target) - *(const Vector*)(&current);
    float ratio = VectorLength(&delta) / (float)duration;
    float clamped = 1.0f;
    if (ratio < 1.0f) {
        clamped = ratio;
    }
    int pushFrames = (int)((1.0f - clamped) * 1000.0f);
    this->pushTimer = pushFrames;
    this->pushDuration = pushFrames;

    Vector here = getVector(this);
    Vector directionSource = *(const Vector*)(&here) - *(const Vector*)(&target);
    Vector direction = VectorNormalize(&directionSource);
    *(Vector*)((char *)this + 0x10c) = *(const Vector*)(&direction);

    void *random = PlayerAsteroid_random;
    Vector randomVector = {
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 200) - 100),
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 200) - 100),
        (float)(AbyssEngine::AERandom::nextInt((int)(intptr_t)random, 200) - 100),
    };
    Vector randomDirection = VectorNormalize(&randomVector);
    Vector randomScaled = *(const Vector*)(&randomDirection) * (0.01f);
    *(Vector*)((char *)this + 0x118) = *(const Vector*)(&randomScaled);
}
