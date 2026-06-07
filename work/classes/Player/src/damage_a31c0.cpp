#include "class.h"

extern "C" int Status_getSystem();
extern "C" int KIPlayer_isWingMan();
extern "C" int SolarSystem_getRace();
extern "C" int SolarSystem_getAttackRace(void *self);
extern "C" int Status_hardCoreMode();
extern "C" void Level_friendTurnedEnemy(int level);
extern "C" void Level_attackWanted(void *level, int a);
extern "C" void Level_alarmAllFriends(void *level, int a, bool b);
extern "C" void Level_almostKillWanted(int level);
extern "C" void Level_killWanted(int level);
extern "C" void *Level_getEnemies();
extern "C" void *Level_getPlayer();
extern "C" int PlayerEgo_getHUD(void *self);
extern "C" void Hud_hudEvent(int hud, void *a, int b);
extern "C" void *Status_getShip();
extern "C" void *Status_getStanding();
extern "C" int Ship_getSignatureRace(void *self);
extern "C" void *Ship_getFirstEquipmentOfSort(int self);
extern "C" void Ship_removeEquipment(void *self, void *item);
extern "C" void Standing_applyDelict(void *self, int a, int b);
extern "C" void Standing_applyKill(void *self, int a);
extern "C" void Standing_setPlayerSignatureRace(void *self, int r);
extern "C" int Status_inBlackMarketSystem();
extern "C" int Status_getCampaignMission();
extern "C" void *GameText_getText(int id);
extern "C" int String_Compare(void *a, void *b);
extern "C" void Mission_getStatusValue();
extern "C" void Mission_setStatusValue(int v);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" void Player_updateDamageRate(Player *self);
extern "C" void Player_damage2(Player *self, int amount);

extern "C" int **g_damage_globals;
extern "C" int **g_damage_text;
extern "C" const float k_damage_full;   // DAT_b3518
extern "C" const float k_damage_hc;     // DAT_b351c
extern "C" const float k_damage_full2;  // DAT_b367c
extern "C" const float k_damage_hc2;    // DAT_b3680
extern "C" const float k_damage_regen;  // DAT_b3684

extern "C" void Player_damage(Player *self, int amount, int flag, int missionId)
{
    if (self->vulnerable == 0) return;
    if (self->active == 0) return;
    if (self->hitpoints < 1) return;

    if (flag == 0 && self->kiPlayer != 0) {
        char *ki = (char *)self->kiPlayer;
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)(ki + 0x28) - 9) > 1 &&
            Status_getSystem() != 0 &&
            ((*(unsigned char *)((char *)self + 0x5c) == 0) || (*(char *)((char *)self + 0xe0) != 0))) {
            ki = (char *)self->kiPlayer;
            if (*(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
                    Level_attackWanted(*(void **)(ki + 0x54), *(int *)(ki + 0x48));
                }
                goto LAB_3488;
            }
        } else {
            ki = (char *)self->kiPlayer;
        }

        if (ki != 0 && self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)(ki + 0x28) - 9) > 1 &&
            KIPlayer_isWingMan() == 0 && Status_getSystem() != 0 &&
            ((*(unsigned char *)((char *)self + 0x5c) == 0) || (*(char *)((char *)self + 0xe0) != 0))) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            bool sameRace = (race == SolarSystem_getRace());
            if (!sameRace) {
                int race2 = *(int *)((char *)self->kiPlayer + 0x28);
                void *sys = (void *)(long)Status_getSystem();
                if (race2 != SolarSystem_getAttackRace(sys)) {
                    goto LAB_342a;
                }
            }
            self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
            int hc = Status_hardCoreMode();
            float thr1 = (hc != 0) ? k_damage_hc : k_damage_full;
            float f1 = thr1 * (float)self->maxHitpoints;
            float dmgF = (float)self->damageDoneByPlayer;
            float frac2 = (hc != 0) ? 0.25f : 0.5f;
            float thr3 = (hc != 0) ? k_damage_hc2 : k_damage_full2;

            if (f1 < dmgF) {
                Level_friendTurnedEnemy(*(int *)((char *)self->kiPlayer + 0x54));
                void *ship = Status_getShip();
                void *standing = Status_getStanding();
                if (Ship_getSignatureRace(ship) >= 0) {
                    bool match = ((unsigned int)Ship_getSignatureRace(ship) ==
                                  *(unsigned int *)((char *)self->kiPlayer + 0x28));
                    unsigned int dis = 0;
                    if (match) dis = *(unsigned char *)((char *)self->kiPlayer + 0x42);
                    if (match && dis == 0) {
                        void *item = Ship_getFirstEquipmentOfSort((int)(long)ship);
                        Ship_removeEquipment(ship, item);
                        void *st2 = Status_getStanding();
                        Standing_applyDelict(st2, Ship_getSignatureRace(ship), 100);
                        Standing_setPlayerSignatureRace(standing, -1);
                        void *ego = Level_getPlayer();
                        int hud = PlayerEgo_getHUD(ego);
                        int p = (int)(long)Level_getPlayer();
                        Hud_hudEvent(hud, (void *)0x1f, p);
                    }
                }
            }

            float f3 = (float)self->maxHitpoints;
            float dmgF3 = (float)self->damageDoneByPlayer;
            if (frac2 * f3 < dmgF3) {
                void *ship = Status_getShip();
                void *standing = Status_getStanding();
                if (Ship_getSignatureRace(ship) >= 0 &&
                    *(int *)((char *)self->kiPlayer + 0x28) < 4 &&
                    *(char *)((char *)self->kiPlayer + 0x42) == 0) {
                    void *item = Ship_getFirstEquipmentOfSort((int)(long)ship);
                    Ship_removeEquipment(ship, item);
                    void *st2 = Status_getStanding();
                    Standing_applyDelict(st2, Ship_getSignatureRace(ship), 100);
                    Standing_setPlayerSignatureRace(standing, -1);
                    void *ego = Level_getPlayer();
                    int hud = PlayerEgo_getHUD(ego);
                    int p = (int)(long)Level_getPlayer();
                    Hud_hudEvent(hud, (void *)0x1f, p);
                }
                self->turnedEnemy = 1;
            }

            float f4a = (float)self->maxHitpoints;
            float dmgF4 = (float)self->damageDoneByPlayer;
            if (thr3 * f4a < dmgF4) {
                Level_alarmAllFriends(*(void **)((char *)self->kiPlayer + 0x54),
                                      *(int *)((char *)self->kiPlayer + 0x28), true);
            }
            goto LAB_3488;
        }
    }

LAB_342a:
    {
        int *globals = *g_damage_globals;
        if (Status_inBlackMarketSystem() != 0) {
            char *ki = (char *)self->kiPlayer;
            if (ki != 0 && *(int *)(ki + 0x28) == 8) {
                self->turnedEnemy = 1;
                Level_alarmAllFriends(*(void **)(ki + 0x54), 8, true);
                int *enemies = (int *)Level_getEnemies();
                if (enemies != 0) {
                    int count = enemies[0];
                    int i = 0;
                    while (count != i) {
                        int e = *(int *)(enemies[1] + i * 4);
                        i++;
                        if (*(int *)(e + 0x28) == 8) {
                            *(char *)(e + 0x25) = 1;
                        }
                    }
                }
                *(unsigned short *)((char *)globals + 0x110) = 0x100;
            }
        }
    }

LAB_3488:
    {
        int shieldI = (int)self->shieldHP - amount;
        if (shieldI < 0) {
            *(int *)&self->shieldHP = 0;
            shieldI = shieldI + self->armorHP;
            if (shieldI < 0) {
                self->armorHP = 0;
                *(char *)((char *)self + 0x66) = 1;
                self->hitpoints = shieldI + self->hitpoints;
            } else {
                *(char *)((char *)self + 0x65) = 1;
                self->armorHP = shieldI;
            }
        } else {
            *(char *)((char *)self + 0x64) = 1;
            self->shieldHP = (float)shieldI;
        }
    }

    {
        int hp;
        char *ki = (char *)self->kiPlayer;
        if (ki != 0 && *(char *)(ki + 0x3c) == 0 && *(char *)(ki + 0x3d) == 0 &&
            *(char *)(ki + 0x42) != 0) {
            hp = self->hitpoints;
            if (__aeabi_idiv(self->maxHitpoints, 3) > hp) {
                Level_almostKillWanted(*(int *)(ki + 0x54));
            } else {
                goto LAB_34f8_hp;
            }
        }
        hp = self->hitpoints;
    LAB_34f8_hp:
        if (hp < 1) {
            self->hitpoints = 0;
            if (flag != 0) {
                *(char *)((char *)self + 0x44) = 1;
            } else {
                char *ki2 = (char *)self->kiPlayer;
                if (ki2 != 0 && *(char *)(ki2 + 0x3c) == 0 && *(char *)(ki2 + 0x3d) == 0 &&
                    Status_inBlackMarketSystem() == 0) {
                    if (*(char *)((char *)self->kiPlayer + 0x42) == 0) {
                        void *st = Status_getStanding();
                        Standing_applyKill(st, *(int *)((char *)self->kiPlayer + 0x28));
                    }
                    int mission = Status_getCampaignMission();
                    char *ki3 = (char *)self->kiPlayer;
                    void *txt = GameText_getText(*g_damage_text[0]);
                    int cmp = String_Compare((void *)(ki3 + 0x18), txt);
                    if (missionId == 0xb3 && cmp == 0) {
                        Mission_getStatusValue();
                        Mission_setStatusValue(mission);
                    }
                    if (*(char *)((char *)self->kiPlayer + 0x42) != 0) {
                        Level_killWanted(*(int *)((char *)self->kiPlayer + 0x54));
                    }
                }
            }
        }
    }

    self->damaged = 1;
    *(float *)&self->field_60 = *(float *)&self->field_60 + k_damage_regen;
    Player_updateDamageRate(self);
    if (self->kiPlayer != 0) {
        int slave = *(int *)((char *)self->kiPlayer + 0x10);
        if (slave != 0) {
            Player *other = *(Player **)(slave + 4);
            other->vulnerable = 1;
            Player_damage2(other, amount);
            *(char *)(*(int *)(*(int *)((char *)self->kiPlayer + 0x10) + 4) + 0xc2) = 0;
        }
    }
}
