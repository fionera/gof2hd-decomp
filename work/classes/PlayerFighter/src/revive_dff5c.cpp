#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);
extern "C" int Player_turnedEnemy(int player);
extern "C" void Player_reset(int player);
extern "C" void Player_turnEnemy(int player);
extern "C" void AEString_ctor_default(void *s);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);
extern "C" void Route_reset(int route);
extern "C" int Player_getHitpoints(int player);
extern "C" void KIPlayer_setActive(PlayerFighter *self, int on);
extern "C" void Explosion_reset(int e);
extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis);
extern "C" void AEGeometry_setVisible(int geom, int vis);
extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void *ArrayInt_dtor(void *p);
extern "C" void Generator_ctor(void *g);
extern "C" int Generator_getLootList(void *g, int a, int b);
extern "C" void *Generator_dtor(void *g);

__attribute__((minsize)) extern "C" void PlayerFighter_revive(PlayerFighter *self)
{
    volatile uint32_t stackGuard = (uint32_t)__stack_chk_guard;

    int enemy = Player_turnedEnemy(F<int32_t>(self, 4));
    Player_reset(F<int32_t>(self, 4));
    if (enemy != 0) {
        Player_turnEnemy(F<int32_t>(self, 4));
    }
    {
        String s;
        AEString_ctor_default(&s);
        AEString_assign((char *)self + 0x18, &s);
        AEString_dtor(&s);
    }
    F<int32_t>(self, 0x78) = 0;
    F<int32_t>(self, 0x88) = 1;
    F<uint8_t>(self, 0x12e) = 0;
    F<int32_t>(self, 0x38) = -1;
    Route_reset(F<int32_t>(self, 0x6c));
    F<int32_t>(self, 0x1d8) = Player_getHitpoints(F<int32_t>(self, 4));
    F<int32_t>(self, 0x1dc) = 0;
    F<uint8_t>(self, 0x1e0) = 0;
    F<int32_t>(self, 0xd8) = 0;
    KIPlayer_setActive(self, 1);
    F<int32_t>(self, 0xf0) = 0;
    F<int32_t>(self, 0x1e8) = F<int32_t>(self, 0x1ac);
    Explosion_reset(F<int32_t>(self, 0x124));
    F<int32_t>(self, 0x104) = 0;
    F<uint8_t>(self, 0x24) = 0;
    PlayerFighter_setExhaustVisible(self, true);
    int geom = F<int32_t>(self, 0xc);
    F<uint8_t>(self, 0xf5) = 1;
    if (geom == 0) {
        geom = F<int32_t>(self, 8);
    }
    AEGeometry_setVisible(geom, 1);
    if ((unsigned)(F<int32_t>(self, 0x28) - 9) < 2) {
        void *a = F<void *>(self, 0x50);
        if (a != 0) {
            operator_delete(ArrayInt_dtor(a));
        }
        F<void *>(self, 0x50) = 0;
    } else {
        void *g = operator_new(1);
        Generator_ctor(g);
        void *a = F<void *>(self, 0x50);
        if (a != 0) {
            operator_delete(ArrayInt_dtor(a));
            F<void *>(self, 0x50) = 0;
        }
        F<int32_t>(self, 0x50) = Generator_getLootList(g, -1, -1);
        operator_delete(Generator_dtor(g));
    }

    uint32_t diff = (uint32_t)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
