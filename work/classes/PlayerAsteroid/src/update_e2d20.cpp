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
    this->f_134 = delta;
    if (delta == 0) {
        return;
    }

    Player *player = this->f_4;
    if (Player_isActive(player) == 0 && this->f_88 == 4) {
        this->f_124 = 0;
        return;
    }

    int hitpoints = Player_getHitpoints(player);
    int state = this->f_88;
    if (hitpoints <= 0 && state == 0) {
        this->f_88 = 3;
        Level_asteroidDied(PlayerAsteroid_level);

        if (this->f_4c != 0) {
            int quality = this->f_150;
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
                this->f_50 = items;
                int item = this->f_128;
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
                ArrayAdd_int(count, this->f_50);
                KIPlayer_createCrate(this, this->f_128 == 0xa4 ? 2 : 1);
            } else {
                this->f_4c = 0;
                this->f_50 = 0;
            }
        } else {
            this->f_4c = 0;
            this->f_50 = 0;
        }

        Explosion_setMatrix(this->f_12c, AEGeometry_getMatrix(this->f_8));
        return;
    }

    if (state == 3) {
        Explosion_update(this->f_12c, delta, 0);
        if (Explosion_isPlaying(this->f_12c) == 0) {
            this->f_88 = 4;
            Player_setBombForce(player, 0.0f);
        }
    } else if (state == 4) {
        KIPlayer_setActive(this, false);
        Player_setBombForce(player, 0.0f);
    }

    int oldHitpoints = this->f_158;
    hitpoints = Player_getHitpoints(player);
    if (hitpoints < oldHitpoints) {
        this->f_15c = 1;
        this->f_160 = 1.0f;
        this->f_158 = Player_getHitpoints(player);
    }

    if (this->f_14c != 0) {
        Vector rotation = Vector_scale((Vector *)((char *)this + 0x140), (float)delta * 0.001f);
        AEGeometry_rotate(this->f_8, &rotation);
    }

    float bombForce = Player_getBombForce(player);
    if (bombForce > 0.0f && this->f_88 == 3) {
        Vector hit = Player_getHitVector(player);
        Vector *hitSlot = (Vector *)((char *)this + 0x90);
        Vector_assign(hitSlot, &hit);
        float scaling = this->f_138;
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
        Explosion_translate(this->f_12c, hitSlot);
        if (this->f_78 != 0) {
            AEGeometry_translate(this->f_78, hitSlot);
        }
        float nextForce = bombForce * 0.75f;
        if (nextForce < 0.01f) {
            nextForce = 0.0f;
        }
        Player_setBombForce(player, nextForce);
    }
}
