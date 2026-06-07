#include "class.h"

extern "C" int Player_getHitpoints(Player *self);                      // 0x724f0
extern "C" int Level_junkDied(void *level);                            // 0x77908
extern "C" void FModSound_play(void *self, int id, Vector *a, Vector *b, float volume); // 0x71548
extern "C" int AERandom_nextInt(void *self, int max);                  // 0x71848
extern "C" void Array_int_ctor(void *array);                          // 0x701f8
extern "C" void ArrayAdd_int(int value, void *array);                 // 0x7021c
extern "C" void KIPlayer_createCrate(void *self, int kind);          // 0x75904
extern "C" void Player_setActive(Player *self, bool active);          // 0x72580
extern "C" void *Level_getPlayer(void *level);                        // 0x72034
extern "C" void ParticleSystemManager_emitManual(int manager, Vector *emitter, Vector *position,
                                                 int unused, Vector *zero, float value); // 0x72a30

__attribute__((visibility("hidden"))) extern void **g_PJ_sound;       // -> FModSound
__attribute__((visibility("hidden"))) extern void **g_PJ_random;      // -> AERandom

// PlayerJunk::update(int) - when destroyed, play the death sound, maybe drop a crate, detach
// from the player target, and emit the wreck particle burst.
void PlayerJunk_update(void *self, int elapsed) {
    Vector zero;
    char workBytes[12];

    F<int>(self, 0x124) = elapsed;
    if (Player_getHitpoints(F<Player *>(self, 0x4)) < 1) {
        int state = F<int>(self, 0x88);
        if ((uint32_t)(state - 3) >= 2) {
            Level_junkDied(F<void *>(self, 0x54));
            F<int>(self, 0x88) = 3;
            FModSound_play(*g_PJ_sound, 0x16, 0, 0, 0.0f);
            void **randHolder = g_PJ_random;
            if (AERandom_nextInt(*randHolder, 100) < 10) {
                F<uint8_t>(self, 0x4c) = 1;
                void *arr = operator new(0xc);
                Array_int_ctor(arr);
                F<void *>(self, 0x50) = arr;
                ArrayAdd_int(99, arr);
                ArrayAdd_int(AERandom_nextInt(*randHolder, 10) + 1, F<void *>(self, 0x50));
                KIPlayer_createCrate(self, 3);
                F<uint8_t>(self, 0x4c) = 1;
            } else {
                Player_setActive(F<Player *>(self, 0x4), false);
                void *player = Level_getPlayer(F<void *>(self, 0x54));
                if (F<void *>(F<void *>(player, 0x14), 0x1c) == self) {
                    player = Level_getPlayer(F<void *>(self, 0x54));
                    F<void *>(F<void *>(player, 0x14), 0x1c) = 0;
                }
            }
            void *levelObject = F<void *>(self, 0x54);
            struct V3 { uint32_t x, y, z; };
            V3 position = F<V3>(self, 0x58);
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            ParticleSystemManager_emitManual(F<int>(levelObject, 0x74),
                                             F<Vector *>(levelObject, 0x34),
                                             (Vector *)&position, 0, &zero, -1.0f);
        }
    }

    if (F<int>(self, 0x88) == 3)
        F<int>(self, 0x88) = 4;
}
