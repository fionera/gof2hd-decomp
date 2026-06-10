#include "gof2/PlayerAsteroid.h"

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
extern "C" void Explosion_render(Explosion *explosion);
extern "C" void AEGeometry_setPosition(AEGeometry *geometry, const Vector *position);
extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);
extern "C" void *PlayerAsteroid_complete_dtor(PlayerAsteroid *self);
extern "C" int Player_isActive(Player *player);
extern "C" int Player_getHitpoints(Player *player);
extern "C" void Explosion_update(Explosion *explosion, int delta, TargetFollowCamera *camera);
extern "C" int Explosion_isPlaying(Explosion *explosion);
extern "C" void KIPlayer_setActive(PlayerAsteroid *self, bool active);
extern "C" void Player_setBombForce(Player *player, float force);
extern "C" Vector Vector_scale(const Vector *vector, float scale);
extern "C" void AEGeometry_rotate(AEGeometry *geometry, const Vector *rotation);
extern "C" float Player_getBombForce(Player *player);
extern "C" Vector Player_getHitVector(Player *player);
extern "C" Vector *Vector_assign(Vector *self, const Vector *other);
extern "C" Vector *Vector_scale_assign(Vector *self, float scale);
extern "C" void Explosion_translate(Explosion *explosion, const Vector *delta);
extern "C" void Level_asteroidDied(void *level);
extern "C" int AERandom_nextInt(void *random, int max);
extern "C" void ArrayInt_ctor(ArrayInt *array);
extern "C" void ArrayAdd_int(int value, ArrayInt *array);
extern "C" void KIPlayer_createCrate(PlayerAsteroid *self, int amount);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *geometry);
extern "C" void Explosion_setMatrix(Explosion *explosion, Matrix *matrix);
extern "C" Vector *Vector_sub_assign(Vector *self, const Vector *other);
extern "C" Vector VectorNormalize(const Vector *vector);
extern "C" void Player_ctor(Player *self, int hitpoints, int damage, int a, int b, int c);
extern "C" void Player_setKIPlayer(Player *player, PlayerAsteroid *self);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transformId);
extern "C" void Player_setRadius(Player *player, int radius);
extern "C" void Player_setMaxHitpoints(Player *player, int hitpoints);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setScaling(Explosion *self, float scaling);
extern "C" void AEGeometry_setScaling(AEGeometry *geometry, float x, float y, float z);
extern "C" void AEGeometry_setRotation(AEGeometry *geometry, float x, float y, float z);
extern "C" float VectorLength(const Vector *vector);
extern "C" void MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z);
extern "C" Matrix Matrix_mul(const Matrix *a, const Matrix *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *geometry, const Matrix *matrix);
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *KIPlayer_dtor(PlayerAsteroid *self);
extern "C" Vector Vector_sub(const Vector *a, const Vector *b);

// ---- setAsteroidIndex_e29c4.cpp ----
void PlayerAsteroid::setAsteroidIndex(int asteroidIndex)
{
    this->field_0x128 = asteroidIndex;
}

// ---- translate_e29dc.cpp ----
void PlayerAsteroid::translate(const Vector &delta)
{
    return AEGeometry_translate(this->field_0x8, &delta);
}

// ---- render_e3038.cpp ----
void PlayerAsteroid::render()
{
    if (this->field_0xf5 != 0) {
        if (this->field_0x78 != 0) {
            AEGeometry_render(this->field_0x78);
        }
        int state = this->field_0x88;
        if (state != 0) {
            if (state == 3) {
                return Explosion_render(this->field_0x12c);
            }
            return;
        }
        return AEGeometry_renderBase(this->field_0x8);
    }
}

// ---- setPosition_e29d6.cpp ----
void PlayerAsteroid::setPosition(const Vector &position)
{
    return AEGeometry_setPosition(this->field_0x8, &position);
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
    return AEGeometry_getPosition(this->field_0x8);
}

// ---- setRotationEnabled_e2a90.cpp ----
void PlayerAsteroid::setRotationEnabled(bool enabled)
{
    this->field_0x14c = enabled;
}

// ---- getQualityFrameIndex_e29f4.cpp ----
int PlayerAsteroid::getQualityFrameIndex()
{
    return 7 - this->field_0x150;
}

// ---- _PlayerAsteroid_e29b4.cpp ----
extern "C" void _ZN14PlayerAsteroidD0Ev(PlayerAsteroid *self)
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
    return this->field_0x150;
}

// ---- getScaling_e29ee.cpp ----
float PlayerAsteroid::getScaling()
{
    return this->field_0x138;
}

// ---- isMinable_e29e2.cpp ----
uint8_t PlayerAsteroid::isMinable()
{
    return this->field_0x13c;
}

// ---- getQualityString_e2a00.cpp ----
__attribute__((visibility("hidden"))) extern const char *PlayerAsteroid_qualityNames[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityDefault[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityFour[];

String PlayerAsteroid::getQualityString()
{
    int quality = this->field_0x150;
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
    this->field_0x134 = delta;
    if (delta == 0) {
        return;
    }

    Player *player = this->field_0x4;
    if (Player_isActive(player) == 0 && this->field_0x88 == 4) {
        this->field_0x124 = 0;
        return;
    }

    int hitpoints = Player_getHitpoints(player);
    int state = this->field_0x88;
    if (hitpoints <= 0 && state == 0) {
        this->field_0x88 = 3;
        Level_asteroidDied(PlayerAsteroid_level);

        if (this->field_0x4c != 0) {
            int quality = this->field_0x150;
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
                this->field_0x50 = items;
                int item = this->field_0x128;
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
                ArrayAdd_int(count, this->field_0x50);
                KIPlayer_createCrate(this, this->field_0x128 == 0xa4 ? 2 : 1);
            } else {
                this->field_0x4c = 0;
                this->field_0x50 = 0;
            }
        } else {
            this->field_0x4c = 0;
            this->field_0x50 = 0;
        }

        Explosion_setMatrix(this->field_0x12c, AEGeometry_getMatrix(this->field_0x8));
        return;
    }

    if (state == 3) {
        Explosion_update(this->field_0x12c, delta, 0);
        if (Explosion_isPlaying(this->field_0x12c) == 0) {
            this->field_0x88 = 4;
            Player_setBombForce(player, 0.0f);
        }
    } else if (state == 4) {
        KIPlayer_setActive(this, false);
        Player_setBombForce(player, 0.0f);
    }

    int oldHitpoints = this->field_0x158;
    hitpoints = Player_getHitpoints(player);
    if (hitpoints < oldHitpoints) {
        this->field_0x15c = 1;
        this->field_0x160 = 1.0f;
        this->field_0x158 = Player_getHitpoints(player);
    }

    if (this->field_0x14c != 0) {
        Vector rotation = Vector_scale((Vector *)((char *)this + 0x140), (float)delta * 0.001f);
        AEGeometry_rotate(this->field_0x8, &rotation);
    }

    float bombForce = Player_getBombForce(player);
    if (bombForce > 0.0f && this->field_0x88 == 3) {
        Vector hit = Player_getHitVector(player);
        Vector *hitSlot = (Vector *)((char *)this + 0x90);
        Vector_assign(hitSlot, &hit);
        float scaling = this->field_0x138;
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
        Explosion_translate(this->field_0x12c, hitSlot);
        if (this->field_0x78 != 0) {
            AEGeometry_translate(this->field_0x78, hitSlot);
        }
        float nextForce = bombForce * 0.75f;
        if (nextForce < 0.01f) {
            nextForce = 0.0f;
        }
        Player_setBombForce(player, nextForce);
    }
}

// ---- getProjectionVector_e3084.cpp ----
typedef void (*VectorAssignFn)(Vector *self, const Vector *other);

__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_projectionScratch;
__attribute__((visibility("hidden"))) extern VectorAssignFn PlayerAsteroid_vectorAssign;


Vector PlayerAsteroid::getProjectionVector(const Vector &value)
{
    Vector result = value;
    Vector position = AEGeometry_getPosition(this->field_0x8);
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

extern "C" void KIPlayer_ctor(PlayerAsteroid *self, int playerId, int team, Player *player,
                              AEGeometry *geometry, float x, float y, float z, int flags);

PlayerAsteroid::PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                               const Vector &position, float scaling, int quality)
{
    Player *player = (Player *)operator_new(0x114);
    Player_ctor(player, 0x5dc, 0x1e, 0, 0, 0);
    KIPlayer_ctor(this, playerId, -1, player, geometry, position.x, position.y, position.z, 0);

    this->field_0x0 = (char *)PlayerAsteroid_vtable + 8;
    this->field_0x140 = 0;
    this->field_0x144 = 0;
    this->field_0x148 = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    Player_setKIPlayer(this->field_0x4, this);

    this->field_0x124 = 0;
    this->field_0x128 = asteroidIndex;
    this->field_0x138 = scaling;
    this->field_0x150 = quality;

    Player *ownedPlayer = this->field_0x4;
    void *transform = PaintCanvas_TransformGetTransform(*(void **)PlayerAsteroid_canvas,
                                                        ((AEGeometryFields *)geometry)->field_0xc);
    Player_setRadius(ownedPlayer, (int)(((TransformFields *)transform)->field_0xe0 * scaling * 0.5f));
    Player_setMaxHitpoints(this->field_0x4, (int)(scaling * 100.0f + 30.0f));
    int hitpoints = Player_getHitpoints(this->field_0x4);
    this->field_0x13c = quality > 3;
    this->field_0x158 = hitpoints;

    Explosion *explosion = (Explosion *)operator_new(0x68);
    Explosion_ctor(explosion, explosionType + 2);
    this->field_0x12c = explosion;
    Explosion_setScaling(explosion, scaling);
    AEGeometry_setScaling(this->field_0x8, scaling, scaling, scaling);
    this->setPosition(position);

    void *random = PlayerAsteroid_random;
    Vector spin = {
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
    };
    Vector_assign(PlayerAsteroid_rotationVector, &spin);

    AEGeometry *ownedGeometry = this->field_0x8;
    AEGeometry_setRotation(ownedGeometry,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f);

    this->field_0x78 = 0;
    this->field_0x14c = 1;
    Vector axis = {
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
    };
    Vector scaledAxis = Vector_scale(&axis, 1.0f);
    Vector_assign((Vector *)((char *)this + 0x140), &scaledAxis);

    this->field_0x4c = 1;
    this->field_0x3c = 1;
    this->field_0x88 = 0;
    *PlayerAsteroid_someCounter = 0;
    this->field_0x15c = 0;
    this->field_0x160 = 0.0f;
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
    if (Player_getHitpoints(self->field_0x4) > 0) {
        GeometryPositionFn getPosition = PlayerAsteroid_geometryPosition;
        float px = getPosition(self->field_0x8).x;
        float py = getPosition(self->field_0x8).y;
        float pz = getPosition(self->field_0x8).z;
        int radiusInt = ((PlayerRadiusFields *)self->field_0x4)->field_0x40;
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
    int remaining = this->field_0x104;
    if (remaining > 0) {
        remaining -= delta;
        this->field_0x104 = remaining;
        float t = (float)remaining / (float)this->field_0x108;

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
        MatrixSetRotation(&rotation, &identity, t * this->field_0x118,
                          t * this->field_0x11c, t * this->field_0x120);

        int frameDelta = this->field_0x134;
        AEGeometry *geometry = this->field_0x8;
        if (frameDelta > 0) {
            Matrix combined = Matrix_mul(&rotation, AEGeometry_getMatrix(geometry));
            AEGeometry_setMatrix(geometry, &combined);
            frameDelta = this->field_0x134;
        }

        Vector baseMove = Vector_scale((Vector *)((char *)this + 0x10c), (float)frameDelta);
        float scale = (2.0f - t) * 3.0f * ((float)this->field_0x108 / 1000.0f);
        Vector move = Vector_scale(&baseMove, scale);
        AEGeometry_translate(geometry, &move);
    }
}

// ---- _PlayerAsteroid_e2980.cpp ----
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;

extern "C" void *_ZN14PlayerAsteroidD1Ev(PlayerAsteroid *self)
{
    self->field_0x0 = (char *)PlayerAsteroid_vtable + 8;
    Explosion *explosion = self->field_0x12c;
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    self->field_0x12c = 0;
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
    this->field_0x104 = pushFrames;
    this->field_0x108 = pushFrames;

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
