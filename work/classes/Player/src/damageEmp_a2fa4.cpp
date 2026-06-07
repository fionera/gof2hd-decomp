#include "class.h"

extern "C" int Status_getSystem();
extern "C" int KIPlayer_isWingMan();
extern "C" int SolarSystem_getRace();
extern "C" int __aeabi_idiv(int a, int b);
extern "C" void Level_friendTurnedEnemy(int level);
extern "C" void Level_attackWanted(void *level, int a);
extern "C" void Level_alarmAllFriends(void *level, int a, bool b);
extern "C" void *Status_getStanding();
extern "C" void Standing_applyDisable(void *self, int a);
extern "C" int Achievements_hasMedal(void *self, int a, int b);
extern "C" int Achievements_getValue(void *self, int a, int b);
extern "C" void *Level_getPlayer();
extern "C" void *PlayerEgo_getHUD(void *self);
extern "C" void Hud_hudEventMedal(void *self, int a, int b);
extern "C" void Player_damageEmp_tail(Player *self);

extern "C" int **g_damageEmp_progress;
extern "C" void **g_damageEmp_achievements;

extern "C" void Player_damageEmp(Player *self, int amount, bool flag)
{
    if (self->vulnerable == 0 || self->active == 0) {
        return;
    }
    int hp = self->empPoints;
    if (hp > 0) {
        hp = self->hitpoints;
    }
    if (hp < 1) {
        return;
    }

    if (!flag && self->kiPlayer != 0 && self->alwaysEnemy == 0) {
        char *ki = (char *)self->kiPlayer;
        bool runLab = true;
        if ((unsigned int)(*(int *)(ki + 0x28) - 9) >= 2) {
            int sys = Status_getSystem();
            ki = (char *)self->kiPlayer;
            if (sys != 0 && *(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    Level_attackWanted(*(void **)(ki + 0x54), *(int *)(ki + 0x48));
                }
                runLab = false;
            } else if (self->kiPlayer == 0) {
                runLab = false;
            }
        }
        // LAB_000b3016
        if (runLab && self->alwaysEnemy == 0 && KIPlayer_isWingMan() == 0 &&
            (unsigned int)(*(int *)((char *)self->kiPlayer + 0x28) - 9) > 1 &&
            Status_getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            if (race == SolarSystem_getRace()) {
                int prev = self->field_dc;
                self->field_dc = prev + amount;
                if (__aeabi_idiv(self->maxEmpPoints, 3) < prev + amount) {
                    self->turnedEnemy = 1;
                    Level_friendTurnedEnemy(*(int *)((char *)self->kiPlayer + 0x54));
                }
            }
        }
    }

    {
        int ep = self->empPoints - amount;
        self->empPoints = ep;
        if (ep > 0) {
            return Player_damageEmp_tail(self);
        }
    }
    if (!flag && self->kiPlayer != 0) {
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)((char *)self->kiPlayer + 0x28) - 9) > 1 &&
            Status_getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            if (race == SolarSystem_getRace()) {
                Level_alarmAllFriends(*(void **)((char *)self->kiPlayer + 0x54),
                                      *(int *)((char *)self->kiPlayer + 0x28), false);
            }
        }
        char *ki = (char *)self->kiPlayer;
        if (*(char *)(ki + 0x3c) != 0) {
            goto lab_30e2;
        }
        if (*(char *)(ki + 0x3d) != 0) {
            goto lab_30f4;
        }
        if (*(char *)(ki + 0x42) != 0) {
            goto lab_30f4;
        }
        {
            void *st = Status_getStanding();
            Standing_applyDisable(st, *(int *)((char *)self->kiPlayer + 0x28));
            ki = (char *)self->kiPlayer;
        }
    lab_30e2:
        if (ki == 0) {
            goto lab_3164;
        }
    lab_30f4:
        if (self->field_68 != 0) {
            goto lab_3164;
        }
        {
            int *prog = *g_damageEmp_progress;
            if (*(unsigned int *)((char *)prog + 0x134) > 0x7fffffff) {
                *(int *)((char *)prog + 0x134) = 0;
            }
            void *ach = *g_damageEmp_achievements;
            if (Achievements_hasMedal(ach, 0x2a, 1) == 0) {
                int cnt = *(int *)((char *)(*g_damageEmp_progress) + 0x134) + 1;
                *(int *)((char *)(*g_damageEmp_progress) + 0x134) = cnt;
                if (Achievements_getValue(*g_damageEmp_achievements, 0x2a, 1) <= cnt) {
                    void *ego = Level_getPlayer();
                    void *hud = PlayerEgo_getHUD(ego);
                    Hud_hudEventMedal(hud, 0x2a, 100);
                    *(char *)((char *)(*g_damageEmp_progress) + 0x138) = 1;
                }
            }
        }
    }

lab_3164:
    // LAB_000b3164
    float f = (float)self->empData;
    self->field_68 = 1;
    self->empPoints = 0;
    *(int *)((char *)self + 0xe8) = 0;
    self->empForce = f;
    return Player_damageEmp_tail(self);
}
