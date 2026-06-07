#include "class.h"

struct RocketGun {
    void seekEnemy(int unused, int index);
};

extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func)(void *out, void *in);
extern "C" int Gun_getEnemies(void *gun);
extern "C" int Gun_isPlayerGun(void *gun);
extern "C" void *Player_getKIPlayer(void *player);
extern "C" int Player_getEnemies(void *player);
extern "C" void *Player_getEnemy(void *player, int index);
extern "C" void *Level_getPlayer(void *level);
extern "C" int PlayerEgo_isInFreeLookMode(void *player);
extern "C" void Player_getPosition(void *out, void *player);
extern "C" void Vector_sub(void *out, void *a, void *b);
extern "C" void *Vector_assign(void *dst, void *src);
extern "C" void Vector_isub(void *dst, void *src);
extern "C" void Vector_idiv(void *dst, float value);
extern "C" void Vector_iadd(void *dst, void *src);
extern "C" void Vector_imul(void *dst, float value);

void RocketGun::seekEnemy(int unused, int index)
{
    char tmp1[12];
    char tmp0[12];
    char enemyPos[12];

    int hasEnemies = Gun_getEnemies(F<void *>(this, 0x8));
    void *enemy;

    if (F<void *>(F<void *>(this, 0x8), 0x4) == 0)
        goto fallback;
    if (Gun_isPlayerGun(F<void *>(this, 0x8)) != 0)
        goto fallback;
    if (F<void *>(F<void *>(this, 0x8), 0x4) == 0)
        goto fallback;
    enemy = F<void *>(F<void *>(this, 0x8), 0x4);
    if (F<int>(Player_getKIPlayer(enemy), 0x38) < 0)
        goto fallback;
    if (Player_getEnemies(F<void *>(F<void *>(this, 0x8), 0x4)) == 0)
        goto fallback;
    enemy = F<void *>(F<void *>(this, 0x8), 0x4);
    enemy = Player_getEnemy(enemy, F<int>(Player_getKIPlayer(enemy), 0x38));
    goto have_enemy;

fallback:
    if (hasEnemies == 0)
        return;
    if (F<void *>(this, 0xb0) == 0)
        return;
    enemy = F<void *>(F<void *>(this, 0xb0), 0x4);
    if (enemy == 0)
        return;
    if (F<uint8_t>(enemy, 0x76) == 0 || F<uint8_t>(enemy, 0x74) != 0)
        return;
    if (PlayerEgo_isInFreeLookMode(Level_getPlayer(F<void *>(F<void *>(this, 0xb0), 0x0))) != 0)
        return;
    enemy = F<void *>(F<void *>(F<void *>(this, 0xb0), 0x4), 0x4);

have_enemy:
    if (enemy != 0) {
        Player_getPosition(enemyPos, enemy);
        Vector_sub(tmp0, enemyPos, (char *)F<void *>(F<void *>(this, 0x8), 0xc) + index * 0xc);
        void (*fn)(void *, void *) = RocketGun_vector_func;
        fn(tmp1, tmp0);
        void *steer = (char *)this + 0xb4;
        Vector_assign(steer, tmp1);
        fn(tmp1, (char *)F<void *>(F<void *>(this, 0x8), 0x18) + index * 0xc);
        Vector_isub(steer, tmp1);
        Vector_idiv(steer, F<float>(this, 0xc8) * 20.0f);
        Vector_iadd(tmp1, steer);
        fn(tmp0, tmp1);
        Vector_assign((char *)F<void *>(F<void *>(this, 0x8), 0x18) + index * 0xc, tmp0);
        Vector_imul((char *)F<void *>(F<void *>(this, 0x8), 0x18) + index * 0xc,
                    F<float>(F<void *>(this, 0x8), 0x50));
    }
}
