#include "class.h"

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_level;
__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;

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
extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);
extern "C" void Level_asteroidDied(void *level);
extern "C" int AERandom_nextInt(void *random, int max);
extern "C" void ArrayInt_ctor(ArrayInt *array);
extern "C" void ArrayAdd_int(int value, ArrayInt *array);
extern "C" void KIPlayer_createCrate(PlayerAsteroid *self, int amount);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *geometry);
extern "C" void Explosion_setMatrix(Explosion *explosion, Matrix *matrix);

void PlayerAsteroid::update(int delta)
{
    F<int>(this, 0x134) = delta;
    if (delta == 0) {
        return;
    }

    Player *player = F<Player *>(this, 0x4);
    if (Player_isActive(player) == 0 && F<int>(this, 0x88) == 4) {
        F<uint8_t>(this, 0x124) = 0;
        return;
    }

    int hitpoints = Player_getHitpoints(player);
    int state = F<int>(this, 0x88);
    if (hitpoints <= 0 && state == 0) {
        F<int>(this, 0x88) = 3;
        Level_asteroidDied(PlayerAsteroid_level);

        if (F<uint8_t>(this, 0x4c) != 0) {
            int quality = F<int>(this, 0x150);
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
                F<ArrayInt *>(this, 0x50) = items;
                int item = F<int>(this, 0x128);
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
                ArrayAdd_int(count, F<ArrayInt *>(this, 0x50));
                KIPlayer_createCrate(this, F<int>(this, 0x128) == 0xa4 ? 2 : 1);
            } else {
                F<uint8_t>(this, 0x4c) = 0;
                F<int>(this, 0x50) = 0;
            }
        } else {
            F<uint8_t>(this, 0x4c) = 0;
            F<int>(this, 0x50) = 0;
        }

        Explosion_setMatrix(F<Explosion *>(this, 0x12c), AEGeometry_getMatrix(F<AEGeometry *>(this, 0x8)));
        return;
    }

    if (state == 3) {
        Explosion_update(F<Explosion *>(this, 0x12c), delta, 0);
        if (Explosion_isPlaying(F<Explosion *>(this, 0x12c)) == 0) {
            F<int>(this, 0x88) = 4;
            Player_setBombForce(player, 0.0f);
        }
    } else if (state == 4) {
        KIPlayer_setActive(this, false);
        Player_setBombForce(player, 0.0f);
    }

    int oldHitpoints = F<int>(this, 0x158);
    hitpoints = Player_getHitpoints(player);
    if (hitpoints < oldHitpoints) {
        F<int>(this, 0x15c) = 1;
        F<float>(this, 0x160) = 1.0f;
        F<int>(this, 0x158) = Player_getHitpoints(player);
    }

    if (F<uint8_t>(this, 0x14c) != 0) {
        Vector rotation = Vector_scale((Vector *)((char *)this + 0x140), (float)delta * 0.001f);
        AEGeometry_rotate(F<AEGeometry *>(this, 0x8), &rotation);
    }

    float bombForce = Player_getBombForce(player);
    if (bombForce > 0.0f && F<int>(this, 0x88) == 3) {
        Vector hit = Player_getHitVector(player);
        Vector *hitSlot = (Vector *)((char *)this + 0x90);
        Vector_assign(hitSlot, &hit);
        float scaling = F<float>(this, 0x138);
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
        Explosion_translate(F<Explosion *>(this, 0x12c), hitSlot);
        if (F<AEGeometry *>(this, 0x78) != 0) {
            AEGeometry_translate(F<AEGeometry *>(this, 0x78), hitSlot);
        }
        float nextForce = bombForce * 0.75f;
        if (nextForce < 0.01f) {
            nextForce = 0.0f;
        }
        Player_setBombForce(player, nextForce);
    }
}
