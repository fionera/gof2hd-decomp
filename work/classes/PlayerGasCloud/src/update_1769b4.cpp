#include "class.h"

using AbyssEngine::AEMath::Vector;

// --- engine callees ---------------------------------------------------------
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *Level_getPlayer();
void *PlayerEgo_getTurretPosition(void *ego, Vector *out);
int PlayerEgo_isInTurretMode(void *ego);
int PlayerEgo_getHUD(void *ego);
int PlayerEgo_getCampaignProgress(void *ego);

void AEGeometry_getPosition(void *geom, Vector *out);
void AEGeometry_setPosition(void *geom, const Vector *pos);

void *Status_getShip();
void *Status_getMission(void *m);
int Status_getCurrentCampaignMission();

int Ship_getFreeSpace(void *ship);
int Ship_getFirstEquipmentOfSort(void *ship, int sort);
void Ship_addCargo(void *ship, void *item);

void *Item_makeItem(void *def);
int Item_getAttribute(void *item);

void Hud_catchCargo(int hud, int itemId, bool a, bool b, bool c, bool d, bool e);

float FModSound_stop(void *snd);
void FModSound_play(void *snd, void *a, void *b, float vol);

int Mission_isEmpty(void *mission);

void Level_createRadioMessage(void *level, int id, int type);

void *PaintCanvas_TransformGetTransform(void *canvas);
void Transform_Update(void *transform, int withChildren, bool b);

// Vector math.
void Vector_sub(Vector *out, const Vector *a, const Vector *b);
void Vector_scale(float s, Vector *inout);      // operator*(float, Vector&)
void Vector_add(Vector *out, const Vector *b);  // operator+(out, b) -> out += b
float VectorLength(const Vector *v);
void VectorNormalize(Vector *out, const Vector *v);
void Vector_assign(Vector *dst, const Vector *src);

// GOT globals.
extern void *g_pgcu_canvasRoot;   // *(DAT_186d78 / DAT_186e14): paint canvas
extern void *g_pgcu_itemDefs;     // *(DAT_186e08): item-definition table
extern void *g_pgcu_pickupSound;  // *(DAT_186e0c): pickup FModSound
extern void *g_pgcu_campaign;     // *(DAT_186e10): campaign state

// Tuning constants.
extern float g_pgcu_velScale;     // DAT_186de4
extern float g_pgcu_catchDist;    // DAT_186de8
extern int   g_pgcu_minTimer;     // DAT_186dec
extern int   g_pgcu_resetTimer;   // DAT_186e00
extern float g_pgcu_fadeLo;       // DAT_186df0
extern float g_pgcu_fadeAdd;      // DAT_186df4
extern float g_pgcu_fadeDiv;      // DAT_186df8
extern float g_pgcu_growDiv;      // DAT_186dfc
}

// PlayerGasCloud::update(int dt)
extern "C" void PlayerGasCloud_update(void *self, int dt)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    if (dt != 0) {
        void *arr = *(void **)(s + 0x138);
        if (*(char *)(s + 0x154) == 0 || *(char *)(s + 0x15c) != 0 || arr == 0) {
            // Idle / pre-explosion: just advance the bound transform.
            void *t = PaintCanvas_TransformGetTransform(*(void **)g_pgcu_canvasRoot);
            Transform_Update(t, 1, (bool)dt);
        } else {
            float dtf = (float)dt;
            *(unsigned char *)(s + 0x15c) = 1;
            *(int *)(s + 0x158) += dt;
            float velStep = dtf * g_pgcu_velScale;

            unsigned int i = 0;
            while (i < *(unsigned int *)arr) {
                int *timer = (int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4);
                *timer -= dt;

                float *life = (float *)(*(int *)(*(int *)(s + 0x140) + 4) + i * 4);
                float newLife = *life - velStep;
                *life = newLife;
                float lifeMin = *(float *)(*(int *)(*(int *)(s + 0x144) + 4) + i * 4);
                if (newLife < lifeMin)
                    *life = lifeMin;

                // Distance from this shard to the player's turret.
                void *player = Level_getPlayer();
                Vector turretPos, shardPos, delta;
                PlayerEgo_getTurretPosition(player, &turretPos);
                void *geom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);
                AEGeometry_getPosition(geom, &shardPos);
                Vector_sub(&delta, &turretPos, &shardPos);
                float dist = VectorLength(&delta);

                bool collected = false;
                if (dist < g_pgcu_catchDist && *(int *)(s + 0x158) >= 2000) {
                    void *p2 = Level_getPlayer();
                    if (PlayerEgo_isInTurretMode(p2) != 0 &&
                        *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4) >= g_pgcu_minTimer) {
                        *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4) = g_pgcu_resetTimer;
                        void *ship = Status_getShip();
                        int itemId = *(int *)(s + 0x160);
                        if (Ship_getFreeSpace(ship) < 1) {
                            if (Level_getPlayer() != 0) {
                                float v = FModSound_stop(g_pgcu_pickupSound);
                                FModSound_play(g_pgcu_pickupSound, (void *)0x8d0, (void *)0, v);
                                void *ego = Level_getPlayer();
                                int hud = PlayerEgo_getHUD(ego);
                                Hud_catchCargo(hud, *(int *)(s + 0x160), false, true, false, true, false);
                            }
                        } else {
                            void *def = *(void **)(*(int *)(*(int *)g_pgcu_itemDefs + 4) + itemId * 4);
                            Item_makeItem(def);
                            if (Level_getPlayer() != 0) {
                                void *ego = Level_getPlayer();
                                int hud = PlayerEgo_getHUD(ego);
                                Hud_catchCargo(hud, *(int *)(s + 0x160), true, false, false, false, false);

                                void *camp = *(void **)g_pgcu_campaign;
                                if (*(char *)((char *)camp + 0x2d) == 0 &&
                                    Status_getCurrentCampaignMission() > 0x8e) {
                                    Vector missionBuf;
                                    Status_getMission(&missionBuf);
                                    if (Mission_isEmpty(&missionBuf) != 0 &&
                                        *(int *)(s + 0x160) == 0xcc) {
                                        *(char *)((char *)camp + 0x2d) = 1;
                                        Level_createRadioMessage(*(void **)(s + 0x54), 0x1a, 0);
                                    }
                                }
                            }
                            float v = FModSound_stop(g_pgcu_pickupSound);
                            FModSound_play(g_pgcu_pickupSound, (void *)0x8d0, (void *)0, v);
                            void *ship2 = Status_getShip();
                            Ship_addCargo(ship2, def);
                        }
                        *(int *)(*(int *)(*(int *)(s + 0x148) + 4) + i * 4) = 0;
                        collected = true;
                    }
                }

                if (!collected) {
                    // Fade / grow the shard's scale based on its remaining timer.
                    int tval = *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4);
                    float *scale = (float *)(*(int *)(*(int *)(s + 0x148) + 4) + i * 4);
                    if (tval <= 0) {
                        float sc = (float)tval / g_pgcu_growDiv + 1.0f;
                        *scale = sc;
                        if (sc < 0.0f)
                            *scale = 0.0f;
                    } else if (dist < g_pgcu_fadeLo) {
                        *scale = (dist + g_pgcu_fadeAdd) / g_pgcu_fadeDiv;
                    }
                    void *t = PaintCanvas_TransformGetTransform(*(void **)g_pgcu_canvasRoot);
                    Transform_Update(t, 1, (bool)dt);
                }

                // Advance the shard's position.
                void *moveGeom;
                Vector moved;
                bool homing = *(char *)(*(int *)(*(int *)(s + 0x150) + 4) + i) != 0 &&
                              *(int *)(s + 0x158) >= 2000;
                if (homing) {
                    void *ship = Status_getShip();
                    if (Ship_getFirstEquipmentOfSort(ship, 0x23) != 0) {
                        // Steer toward the turret.
                        Vector dir, dn;
                        Vector_sub(&dir, &turretPos, &shardPos);
                        VectorNormalize(&dn, &dir);
                        Vector_assign(*(Vector **)(*(int *)(*(int *)(s + 0x13c) + 4) + i * 4), &dn);
                        moveGeom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);

                        void *ship2 = Status_getShip();
                        int eq = Ship_getFirstEquipmentOfSort(ship2, 0x23);
                        int attr = Item_getAttribute((void *)(long)eq);
                        float step = (float)(attr * dt);
                        Vector vel; Vector_assign(&vel, &turretPos);
                        Vector_scale(step, &vel);
                        Vector_add(&vel, &shardPos);
                        moved = vel;
                        AEGeometry_setPosition(moveGeom, &moved);
                        goto advance;
                    }
                }
                {
                    moveGeom = *(void **)(*(int *)(*(int *)(s + 0x138) + 4) + i * 4);
                    Vector vel;
                    Vector_assign(&vel, *(Vector **)(*(int *)(*(int *)(s + 0x13c) + 4) + i * 4));
                    Vector_scale(*(float *)(*(int *)(*(int *)(s + 0x140) + 4) + i * 4) * dtf, &vel);
                    Vector_add(&vel, &shardPos);
                    moved = vel;
                    AEGeometry_setPosition(moveGeom, &moved);
                }

            advance:
                if (g_pgcu_minTimer <= *(int *)(*(int *)(*(int *)(s + 0x14c) + 4) + i * 4))
                    *(unsigned char *)(s + 0x15c) = 0;
                i++;
                arr = *(void **)(s + 0x138);
            }

            if (*(char *)(s + 0x15c) != 0)
                *(unsigned char *)(s + 0xf5) = 0;
        }
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
