#include "class.h"

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_canvas;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;
__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_rotationVector;
__attribute__((visibility("hidden"))) extern int *PlayerAsteroid_someCounter;

extern "C" void Player_ctor(Player *self, int hitpoints, int damage, int a, int b, int c);
extern "C" void KIPlayer_ctor(PlayerAsteroid *self, int playerId, int team, Player *player,
                              AEGeometry *geometry, float x, float y, float z, int flags);
extern "C" void Player_setKIPlayer(Player *player, PlayerAsteroid *self);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transformId);
extern "C" void Player_setRadius(Player *player, int radius);
extern "C" void Player_setMaxHitpoints(Player *player, int hitpoints);
extern "C" int Player_getHitpoints(Player *player);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setScaling(Explosion *self, float scaling);
extern "C" void AEGeometry_setScaling(AEGeometry *geometry, float x, float y, float z);
extern "C" int AERandom_nextInt(void *random, int max);
extern "C" Vector *Vector_assign(Vector *self, const Vector *other);
extern "C" void AEGeometry_setRotation(AEGeometry *geometry, float x, float y, float z);
extern "C" Vector Vector_scale(const Vector *vector, float scale);

PlayerAsteroid::PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                               const Vector &position, float scaling, int quality)
{
    Player *player = (Player *)operator_new(0x114);
    Player_ctor(player, 0x5dc, 0x1e, 0, 0, 0);
    KIPlayer_ctor(this, playerId, -1, player, geometry, position.x, position.y, position.z, 0);

    this->f_0 = (char *)PlayerAsteroid_vtable + 8;
    this->f_140 = 0;
    this->f_144 = 0;
    this->f_148 = 0;
    this->f_164 = 0;
    this->f_168 = 0;
    this->f_16c = 0;
    Player_setKIPlayer(this->f_4, this);

    this->f_124 = 0;
    this->f_128 = asteroidIndex;
    this->f_138 = scaling;
    this->f_150 = quality;

    Player *ownedPlayer = this->f_4;
    void *transform = PaintCanvas_TransformGetTransform(*(void **)PlayerAsteroid_canvas, geometry->f_c);
    Player_setRadius(ownedPlayer, (int)(transform->f_e0 * scaling * 0.5f));
    Player_setMaxHitpoints(this->f_4, (int)(scaling * 100.0f + 30.0f));
    int hitpoints = Player_getHitpoints(this->f_4);
    this->f_13c = quality > 3;
    this->f_158 = hitpoints;

    Explosion *explosion = (Explosion *)operator_new(0x68);
    Explosion_ctor(explosion, explosionType + 2);
    this->f_12c = explosion;
    Explosion_setScaling(explosion, scaling);
    AEGeometry_setScaling(this->f_8, scaling, scaling, scaling);
    this->setPosition(position);

    void *random = PlayerAsteroid_random;
    Vector spin = {
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
        (float)(AERandom_nextInt(random, 0x2000) - 0x1000) * 0.001f,
    };
    Vector_assign(PlayerAsteroid_rotationVector, &spin);

    AEGeometry *ownedGeometry = this->f_8;
    AEGeometry_setRotation(ownedGeometry,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f,
                           (float)AERandom_nextInt(random, 100) * 0.01f * 6.2831855f);

    this->f_78 = 0;
    this->f_14c = 1;
    Vector axis = {
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
        (float)(AERandom_nextInt(random, 3) - 1),
    };
    Vector scaledAxis = Vector_scale(&axis, 1.0f);
    Vector_assign((Vector *)((char *)this + 0x140), &scaledAxis);

    this->f_4c = 1;
    this->f_3c = 1;
    this->f_88 = 0;
    *PlayerAsteroid_someCounter = 0;
    this->f_15c = 0;
    this->f_160 = 0.0f;
}
