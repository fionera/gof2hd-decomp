#include "gof2/PlayerAsteroid.h"
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


extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);
extern "C" void AEGeometry_render(AEGeometry *geometry);
extern "C" void AEGeometry_renderBase(AEGeometry *geometry);
extern "C" void AEGeometry_setPosition(AEGeometry *geometry, const Vector *position);
extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);
extern "C" void *PlayerAsteroid_complete_dtor(PlayerAsteroid *self);
extern "C" void Explosion_update(Explosion *explosion, int delta, TargetFollowCamera *camera);
extern "C" Vector Vector_scale(const Vector *vector, float scale);
extern "C" void AEGeometry_rotate(AEGeometry *geometry, const Vector *rotation);
Vector Player_getHitVector(Player *player);
extern "C" Vector *Vector_assign(Vector *self, const Vector *other);
extern "C" Vector *Vector_scale_assign(Vector *self, float scale);
extern "C" void Level_asteroidDied(void *level);
extern "C" int AERandom_nextInt(void *random, int max);
extern "C" void ArrayInt_ctor(ArrayInt *array);
extern "C" void ArrayAdd_int(int value, ArrayInt *array);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *geometry);
extern "C" void Explosion_setMatrix(Explosion *explosion, Matrix *matrix);
extern "C" Vector *Vector_sub_assign(Vector *self, const Vector *other);
Vector VectorNormalize(const Vector *vector);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transformId);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void AEGeometry_setScaling(AEGeometry *geometry, float x, float y, float z);
extern "C" void AEGeometry_setRotation(AEGeometry *geometry, float x, float y, float z);
float VectorLength(const Vector *vector);
void MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z);
extern "C" Matrix Matrix_mul(const Matrix *a, const Matrix *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *geometry, const Matrix *matrix);
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *KIPlayer_dtor(PlayerAsteroid *self);
extern "C" Vector Vector_sub(const Vector *a, const Vector *b);

// ---- setAsteroidIndex_e29c4.cpp ----
void PlayerAsteroid::setAsteroidIndex(int asteroidIndex)
{
    this->asteroidIndex = asteroidIndex;
}

// ---- translate_e29dc.cpp ----
void PlayerAsteroid::translate(const Vector &delta)
{
    return AEGeometry_translate(this->geometry, &delta);
}

// ---- render_e3038.cpp ----
void PlayerAsteroid::render()
{
    if (this->visible != 0) {
        if (this->secondaryGeometry != 0) {
            AEGeometry_render(this->secondaryGeometry);
        }
        int state = this->state;
        if (state != 0) {
            if (state == 3) {
                return ((Explosion *)(this->explosion))->render();
            }
            return;
        }
        return AEGeometry_renderBase(this->geometry);
    }
}

// ---- setPosition_e29d6.cpp ----
void PlayerAsteroid::setPosition(const Vector &position)
{
    return AEGeometry_setPosition(this->geometry, &position);
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
    return AEGeometry_getPosition(this->geometry);
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
    return operator_delete(PlayerAsteroid_complete_dtor(self));
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
        Level_asteroidDied(PlayerAsteroid_level);

        if (this->dropsLoot != 0) {
            int quality = this->quality;
            void *random = PlayerAsteroid_random;
            bool spawn = true;
            if (quality == 7) {
                spawn = AERandom_nextInt(random, 100) < 4;
            } else if (quality > 6 || AERandom_nextInt(random, 100) > 0x13) {
                spawn = false;
            }

            if (spawn) {
                ArrayInt *items = (ArrayInt *)operator_new(0xc);
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
                    count = AERandom_nextInt(random, 3) + 1;
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

        Explosion_setMatrix(this->explosion, AEGeometry_getMatrix(this->geometry));
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
        Vector rotation = Vector_scale((Vector *)((char *)this + 0x140), (float)delta * 0.001f);
        AEGeometry_rotate(this->geometry, &rotation);
    }

    float bombForce = ((Player *)(player))->getBombForce();
    if (bombForce > 0.0f && this->state == 3) {
        Vector hit = Player_getHitVector(player);
        Vector *hitSlot = (Vector *)((char *)this + 0x90);
        Vector_assign(hitSlot, &hit);
        float scaling = this->scaling;
        float clamped = scaling;
        if (clamped > 1.0f) {
            clamped = 1.0f;
        }
        if (scaling < 0.5f) {
            clamped = 1.0f;
        }
        float force = (float)(int)(bombForce * 0.1f * (clamped / 2.0f + 1.0f));
        Vector_scale_assign(hitSlot, force);
        typedef void (*PushFn)(PlayerAsteroid *, Vector *);
        (*(PushFn *)((char *)*(void **)this + 0x20))(this, hitSlot);
        ((Explosion *)(this->explosion))->translate(hitSlot);
        if (this->secondaryGeometry != 0) {
            AEGeometry_translate(this->secondaryGeometry, hitSlot);
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
    Vector position = AEGeometry_getPosition(this->geometry);
    Vector *scratch = PlayerAsteroid_projectionScratch;
    VectorAssignFn assign = PlayerAsteroid_vectorAssign;
    assign(scratch, &position);
    Vector_sub_assign(scratch, &result);
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
    Player *player = (Player *)operator_new(0x114);
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
    void *transform = PaintCanvas_TransformGetTransform(*(void **)PlayerAsteroid_canvas,
                                                        ((AEGeometryFields *)geometry)->field_0xc);
    ((Player *)(ownedPlayer))->setRadius((int)(((TransformFields *)transform)->field_0xe0 * scaling * 0.5f));
    ((Player *)(this->player))->setMaxHitpoints((int)(scaling * 100.0f + 30.0f));
    int hitpoints = ((Player *)(this->player))->getHitpoints();
    this->minable = quality > 3;
    this->lastHitpoints = hitpoints;

    Explosion *explosion = (Explosion *)operator_new(0x68);
    Explosion_ctor(explosion, explosionType + 2);
    this->explosion = explosion;
    ((Explosion *)(explosion))->setScaling(scaling);
    AEGeometry_setScaling(this->geometry, scaling, scaling, scaling);
    this->setPosition(position);

    void *random = PlayerAsteroid_random;
    Vector spin = {
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
    };
    Vector_assign(PlayerAsteroid_rotationVector, &spin);

    AEGeometry *ownedGeometry = this->geometry;
    AEGeometry_setRotation(ownedGeometry,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f);

    this->secondaryGeometry = 0;
    this->rotationEnabled = 1;
    Vector axis = {
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
    };
    Vector scaledAxis = Vector_scale(&axis, 1.0f);
    Vector_assign((Vector *)((char *)this + 0x140), &scaledAxis);

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
    Vector_assign(PlayerAsteroid_center, local);
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
            Matrix combined = Matrix_mul(&rotation, AEGeometry_getMatrix(geometry));
            AEGeometry_setMatrix(geometry, &combined);
            frameDelta = this->lastDelta;
        }

        Vector baseMove = Vector_scale((Vector *)((char *)this + 0x10c), (float)frameDelta);
        float scale = (2.0f - t) * 3.0f * ((float)this->pushDuration / 1000.0f);
        Vector move = Vector_scale(&baseMove, scale);
        AEGeometry_translate(geometry, &move);
    }
}

// ---- _PlayerAsteroid_e2980.cpp ----
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;

void *_ZN14PlayerAsteroidD1Ev(PlayerAsteroid *self)
{
    self->vtable = (char *)PlayerAsteroid_vtable + 8;
    Explosion *explosion = self->explosion;
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
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
    Vector delta = Vector_sub(&target, &current);
    float ratio = VectorLength(&delta) / (float)duration;
    float clamped = 1.0f;
    if (ratio < 1.0f) {
        clamped = ratio;
    }
    int pushFrames = (int)((1.0f - clamped) * 1000.0f);
    this->pushTimer = pushFrames;
    this->pushDuration = pushFrames;

    Vector here = getVector(this);
    Vector directionSource = Vector_sub(&here, &target);
    Vector direction = VectorNormalize(&directionSource);
    Vector_assign((Vector *)((char *)this + 0x10c), &direction);

    void *random = PlayerAsteroid_random;
    Vector randomVector = {
        (float)(AERandom_nextInt(random, 200) - 100),
        (float)(AERandom_nextInt(random, 200) - 100),
        (float)(AERandom_nextInt(random, 200) - 100),
    };
    Vector randomDirection = VectorNormalize(&randomVector);
    Vector randomScaled = Vector_scale(&randomDirection, 0.01f);
    Vector_assign((Vector *)((char *)this + 0x118), &randomScaled);
}
