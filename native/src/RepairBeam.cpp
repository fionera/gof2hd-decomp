#include "gof2/RepairBeam.h"


extern "C" void AEGeometry_render(void *geom);
extern "C" int RepairBeam_Status_getShip();
extern "C" int RepairBeam_Ship_getFirstEquipmentOfSort(int ship);
extern "C" int RepairBeam_Item_getAttribute(int item);
extern "C" void RepairBeam_Array_Geo_ctor(void *arr);
extern "C" void RepairBeam_Array_int_ctor(void *arr);
extern "C" void RepairBeam_Array_float_ctor(void *arr);
extern "C" void RepairBeam_ArraySetLength_Geo(int n, void *arr);
extern "C" void RepairBeam_ArraySetLength_int(int n, void *arr);
extern "C" void RepairBeam_ArraySetLength_float(int n, void *arr);
extern "C" void RepairBeam_AEGeometry_ctor(void *geom, unsigned short id, void *canvas, bool b);
extern "C" __attribute__((visibility("hidden"))) void **g_RepairBeam_canvas;
extern "C" void RepairBeam_ArrayReleaseClasses_Geo(void *arr);
extern "C" void *RepairBeam_Array_Geo_dtor(void *arr);
extern "C" void *RepairBeam_Array_int_dtor(void *arr);
extern "C" void *RepairBeam_Array_float_dtor(void *arr);
extern "C" void operator_delete(void *p);
extern "C" int RB_Level_getEnemies(void *level);
extern "C" void *RB_Level_getPlayer(void *level);
extern "C" int RB_PlayerEgo_isDead(void *ego);
extern "C" int RB_Status_getShip();
extern "C" int RB_Ship_getFirstEquipmentOfSort(int ship);
extern "C" int RB_Ship_getIndex();
extern "C" int RB_Item_getAttribute(int item);
extern "C" int RB_KIPlayer_isDead(void *kp);
extern "C" int RB_KIPlayer_isDying(void *kp);
extern "C" int RB_Player_getHitpoints();
extern "C" int RB_Player_getMaxHitpoints();
extern "C" int RB_Player_getShieldDamageRate(void *pl);
extern "C" void RB_Player_getPosition(Vector *out);
extern "C" void RB_PlayerEgo_getPosition(Vector *out);
extern "C" void RB_Player_damage(int pl, bool b, int z);
extern "C" void RB_Player_heal(int pl, float amt);
extern "C" void RB_Player_regenerateShield(void *pl, float amt);
extern "C" float RB_VectorLength(Vector *v);
extern "C" void RB_VectorNormalize(Vector *out, Vector *in);
extern "C" void RB_Vector_assign(Vector *dst, const Vector *src);
extern "C" void RB_Vector_subassign(Vector *dst, const Vector *src);
extern "C" void RB_Vector_addassign(Vector *dst, const Vector *src);
extern "C" void RB_Vector_add(Vector *out, const Vector *a);
extern "C" void RB_Vector_sub(Vector *out, const Vector *a);
extern "C" void RB_Vector_mul(Vector *out, float s);
extern "C" void RB_AEGeometry_getDirection(void *geom, Vector *out);
extern "C" void RB_AEGeometry_getUpVector(void *geom, Vector *out);
extern "C" void RB_AEGeometry_setDirection(void *geom, Vector *dir);
extern "C" void RB_AEGeometry_setPosition(void *geom, Vector *pos);
extern "C" void RB_AEGeometry_setScaling(float x, float y, float z);
extern "C" float RB_PlayerEgo_GetDirVector();
extern "C" float RB_PlayerEgo_GetUpVector();
extern "C" long long RB_PaintCanvas_TransformGetTransform(void *canvas);
extern "C" void RB_Transform_Update(long long t, bool b);
extern "C" int RB_FModSound_isPlaying(int snd);
extern "C" void RB_FModSound_play(int snd, void *ev, void *p, float f);
extern "C" void RB_FModSound_stop(int snd);
extern "C" void RB_FModSound_updateEvent3DAttributes(void *snd, int ev, Vector *pos, void *p, bool b);
extern "C" void RB___stack_chk_fail();

// ---- render_a7a48.cpp ----
// RepairBeam::render() — render each beam geometry whose target id slot is active (!= -1).

extern "C" void RepairBeam_render(RepairBeam *self)
{
    int **targetIds = (int **)self->field_0x14;
    for (unsigned i = 0; i < (unsigned)targetIds[0][0]; i = i + 1) {
        if (targetIds[1][i] != -1) {
            void ***geoms = (void ***)self->field_0x10;
            AEGeometry_render(geoms[1][i]);
        }
    }
}

// ---- RepairBeam_a7100.cpp ----
// RepairBeam::RepairBeam(int shipIndex, int sort) — builds the per-target geometry/id/charge
// arrays sized to the player's equipment-of-sort attribute count.

extern "C" RepairBeam *RepairBeam_ctor(RepairBeam *self, int shipIndex, int sort)
{
    self->field_0x1c = sort;
    self->field_0x0 = shipIndex;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xc = 0;

    int ship = RepairBeam_Status_getShip();
    int equip = RepairBeam_Ship_getFirstEquipmentOfSort(ship);
    int count = RepairBeam_Item_getAttribute(equip);

    // geometry array
    void *geoms = operator new(0xc);
    RepairBeam_Array_Geo_ctor(geoms);
    self->field_0x10 = (void **)geoms;
    RepairBeam_ArraySetLength_Geo(count, geoms);

    unsigned short geoId = 0x4a95;
    if (sort == 0x25) {
        geoId = 0x4a94;
    }
    void **canvas = g_RepairBeam_canvas;
    for (int i = 0; i < count; i = i + 1) {
        void *geo = operator new(0xc0);
        RepairBeam_AEGeometry_ctor(geo, geoId, *canvas, false);
        ((void **)self->field_0x10[1])[i] = geo;
    }

    // target-id array
    void *ids = operator new(0xc);
    RepairBeam_Array_int_ctor(ids);
    self->field_0x14 = (void **)ids;
    RepairBeam_ArraySetLength_int(count, ids);
    self->field_0x20 = 0x9c4;
    {
        unsigned *arr = (unsigned *)self->field_0x14;
        for (unsigned k = 0; k < arr[0]; k = k + 1) {
            ((int *)arr[1])[k] = -1;
        }
    }

    // charge array
    void *charges = operator new(0xc);
    RepairBeam_Array_float_ctor(charges);
    self->field_0x18 = (void **)charges;
    RepairBeam_ArraySetLength_float(count, charges);
    {
        int *arr = (int *)self->field_0x18;
        int off = 0;
        for (int j = arr[0]; j != 0; j = j - 1) {
            *(int *)((char *)arr[1] + off) = 0;
            off = off + 4;
        }
    }

    return self;
}

// ---- _RepairBeam_a72a0.cpp ----
// RepairBeam::~RepairBeam() — releases the contained AEGeometry* array (also releasing the
// pointed-to objects) and the int/float arrays, nulling each handle.

extern "C" RepairBeam *RepairBeam_dtor(RepairBeam *self)
{
    void *geoms = self->field_0x10;
    if (geoms != 0) {
        RepairBeam_ArrayReleaseClasses_Geo(geoms);
        void *g = self->field_0x10;
        if (g != 0) {
            operator_delete(RepairBeam_Array_Geo_dtor(g));
        }
        self->field_0x10 = 0;
    }
    void *ids = self->field_0x14;
    if (ids != 0) {
        operator_delete(RepairBeam_Array_int_dtor(ids));
    }
    self->field_0x14 = 0;
    void *charges = self->field_0x18;
    if (charges != 0) {
        operator_delete(RepairBeam_Array_float_dtor(charges));
    }
    self->field_0x18 = 0;
    return self;
}

// ---- update_a72f8.cpp ----
// RepairBeam::update(int dt, Radar* radar, Level* level, Hud* hud)
// Per the target, param_2 ("radar") is reused as the integer delta in the math; we keep the
// original ABI (Radar* radar) but treat it as an int where the disasm does.
//
// Drives the repair/heal/shield beams: assigns targets, animates beam geometry toward them,
// applies heal/shield/damage effects, and manages the looping beam sound.


extern "C" __attribute__((visibility("hidden"))) int **g_RB_canary;     // stack-guard global
extern "C" __attribute__((visibility("hidden"))) void **g_RB_canvas;    // PaintCanvas holder
extern "C" __attribute__((visibility("hidden"))) int *g_RB_dmgThresh;   // DAT_000b767c
extern "C" __attribute__((visibility("hidden"))) float g_RB_scaleDiv;   // DAT_000b7688
extern "C" __attribute__((visibility("hidden"))) float g_RB_healMul;    // DAT_000b7680
extern "C" __attribute__((visibility("hidden"))) float g_RB_shieldMul;  // DAT_000b7684
extern "C" __attribute__((visibility("hidden"))) int *g_RB_sndPlay;     // play sound holder
extern "C" __attribute__((visibility("hidden"))) int *g_RB_sndPlayEv;   // event-array base
extern "C" __attribute__((visibility("hidden"))) int **g_RB_sndStop;    // stop sound holder
extern "C" __attribute__((visibility("hidden"))) int *g_RB_sndDead;     // player-dead sound holder
extern "C" __attribute__((visibility("hidden"))) int **g_RB_sndUpd;     // update sound holder
extern "C" __attribute__((visibility("hidden"))) int *g_RB_sndUpdEv;    // update event-array base
extern "C" __attribute__((visibility("hidden"))) char **g_RB_sndFlag;   // gating flag holder

static inline int *targetIds(RepairBeam *self) { return (int *)self->field_0x14; }
static inline int targetCount(RepairBeam *self) { return ((int **)((char *)self + 0x14))[0][0]; }

extern "C" void RepairBeam_update(RepairBeam *self, int dt, void *level, void *hud)
{
    int *canary = *g_RB_canary;
    int saved = *canary;

    int *enemies = (int *)RB_Level_getEnemies(level);
    self->field_0x20 = self->field_0x20 - dt;

    if (enemies != 0) {
        void *ego = RB_Level_getPlayer(level);
        if (RB_PlayerEgo_isDead(ego) == 0) {
            int ship = RB_Status_getShip();
            int equip = RB_Ship_getFirstEquipmentOfSort(ship);
            int attr = RB_Item_getAttribute(equip);
            float attrF = (float)attr;

            int *ids = targetIds(self);
            Vector *beamPos = (Vector *)((char *)self + 4);

            if (self->field_0x20 < 0) {
                // re-arm: clear all target slots and charges
                int off = 0;
                for (unsigned i = 0; i < (unsigned)ids[0]; i = i + 1) {
                    ((int *)ids[1])[i] = -1;
                    *(int *)((char *)((int *)self->field_0x18[1]) + off) = 0;
                    off = off + 4;
                }
                self->field_0x20 = self->field_0x20 + 0x9c4;

                int *enDataArr = (int *)enemies[1];
                for (unsigned e = 0; e < (unsigned)enemies[0]; e = e + 1) {
                    void *kp = (void *)enDataArr[e];
                    if (RB_KIPlayer_isDead(kp) == 0 && RB_KIPlayer_isDying(kp) == 0) {
                        int sort = self->field_0x1c;
                        bool consider = false;
                        if (sort == 0x25) {
                            char *pl = *(char **)((char *)kp + 4);
                            if (pl[0x5d] != 0) {
                                int hp = RB_Player_getHitpoints();
                                int mhp = RB_Player_getMaxHitpoints();
                                if (hp < mhp)
                                    consider = true;
                            }
                        } else {
                            bool isShield = (sort == 0x29);
                            int dmgflag = isShield ? (int)*(unsigned char *)((char *)kp + 0x74) : 1;
                            if (isShield && dmgflag == 0 &&
                                (*(char **)((char *)kp + 4))[0x5c] != 0) {
                                void **plp = (void **)RB_Level_getPlayer(level);
                                if (RB_Player_getShieldDamageRate((void *)*plp) < 100) {
                                    int sh = RB_Status_getShip();
                                    if (RB_Ship_getFirstEquipmentOfSort(sh) != 0)
                                        consider = true;
                                }
                            }
                        }
                        if (consider) {
                            Vector tmp;
                            RB_Player_getPosition(&tmp);
                            RB_Vector_assign(beamPos, &tmp);
                            RB_Level_getPlayer(level);
                            RB_PlayerEgo_getPosition(&tmp);
                            RB_Vector_subassign(beamPos, &tmp);
                            float dist = RB_VectorLength(beamPos);
                            if (dist <= attrF) {
                                int *t = targetIds(self);
                                bool placed = false;
                                for (unsigned s = 0; !placed && s < (unsigned)t[0]; s = s + 1) {
                                    int *data = (int *)t[1];
                                    placed = data[s] == -1;
                                    if (placed)
                                        data[s] = (int)e;
                                }
                                if (!placed) {
                                    int srcHp = RB_Player_getHitpoints();
                                    unsigned best = 0xffffffff;
                                    int bestHp = *g_RB_dmgThresh;
                                    int *t2;
                                    for (unsigned s = 0; (t2 = targetIds(self)), s < (unsigned)t2[0];
                                         s = s + 1) {
                                        if (((int *)t2[1])[s] != -1) {
                                            int hp = RB_Player_getHitpoints();
                                            if (hp < bestHp && srcHp < hp) {
                                                bestHp = hp;
                                                best = s;
                                            }
                                        }
                                    }
                                    if (best != 0xffffffff)
                                        ((int *)t2[1])[best] = (int)e;
                                }
                            }
                        }
                    }
                }
            }

            // animate beams
            float scaleDiv = g_RB_scaleDiv;
            Vector *beamPos2 = (Vector *)((char *)self + 4);
            int *enData = enemies + 1;
            bool allInactive = true;
            float dtF = (float)dt;
            void **canvas = g_RB_canvas;
            float healAmt = dtF * g_RB_healMul;
            float shieldAmt = dtF * g_RB_shieldMul;

            int off = 0;
            for (unsigned i = 0; i < (unsigned)targetCount(self); i = i + 1) {
                if (((int *)targetIds(self)[1])[off / 4] != -1 &&
                    *(int *)((char *)((int *)targetIds(self)[1]) + off) != -1) {
                    long long t = RB_PaintCanvas_TransformGetTransform(*canvas);
                    RB_Transform_Update(t, (bool)dt);
                    Vector tmp;
                    RB_Player_getPosition(&tmp);
                    RB_Vector_assign(beamPos2, &tmp);

                    int idSlot = *(int *)((char *)((int *)targetIds(self)[1]) + off);
                    int *enemyObj = (int *)(((int *)(*enData))[idSlot]);
                    int kind = *(int *)((char *)enemyObj + 0x7c);

                    Vector dir, up, contrib;
                    if (kind == 0x2c) {
                        RB_AEGeometry_getDirection(enemyObj, &dir);
                        RB_VectorNormalize(&dir, &dir);
                        RB_Vector_mul(&dir, RB_PlayerEgo_GetDirVector());
                        RB_Vector_assign(&contrib, &dir);
                    } else if (kind == 0x31) {
                        RB_AEGeometry_getDirection(enemyObj, &dir);
                        RB_VectorNormalize(&dir, &dir);
                        RB_Vector_mul(&dir, RB_PlayerEgo_GetDirVector());
                        RB_AEGeometry_getUpVector(enemyObj, &up);
                        RB_VectorNormalize(&up, &up);
                        RB_Vector_mul(&up, RB_PlayerEgo_GetUpVector());
                        RB_Vector_add(&dir, &up);
                        RB_Vector_assign(&contrib, &dir);
                    } else {
                        contrib.x = 0;
                        contrib.y = 0;
                        contrib.z = 0;
                    }
                    RB_Vector_addassign(beamPos2, &contrib);
                    RB_Level_getPlayer(level);
                    RB_PlayerEgo_getPosition(&tmp);
                    RB_Vector_subassign(beamPos2, &tmp);

                    RB_Status_getShip();
                    int idx = RB_Ship_getIndex();
                    Vector ddir;
                    if (idx == 0x2c) {
                        RB_Level_getPlayer(level);
                        RB_Vector_mul(&ddir, RB_PlayerEgo_GetDirVector());
                    } else {
                        RB_Status_getShip();
                        if (RB_Ship_getIndex() == 0x31) {
                            RB_Level_getPlayer(level);
                            RB_Vector_mul(&ddir, RB_PlayerEgo_GetDirVector());
                            Vector dup;
                            RB_Level_getPlayer(level);
                            RB_Vector_mul(&dup, RB_PlayerEgo_GetUpVector());
                            RB_Vector_add(&ddir, &dup);
                        } else {
                            ddir.x = 0;
                            ddir.y = 0;
                            ddir.z = 0;
                        }
                    }

                    float len = RB_VectorLength(beamPos2);
                    RB_AEGeometry_setScaling(len, 0.5f, 0.5f);

                    void *geo = ((void **)self->field_0x10[1])[off / 4];
                    Vector ndir;
                    RB_Vector_sub(&ndir, beamPos2);   // (placeholder) - beamPos2 - geom-frame
                    RB_VectorNormalize(&ndir, &ndir);
                    RB_AEGeometry_setDirection(geo, &ndir);

                    void *geo2 = ((void **)self->field_0x10[1])[off / 4];
                    RB_Level_getPlayer(level);
                    RB_PlayerEgo_getPosition(&ndir);
                    RB_AEGeometry_setPosition(geo2, &ndir);

                    long long t2 = RB_PaintCanvas_TransformGetTransform(*canvas);
                    RB_Transform_Update(t2, (bool)dt);

                    if (self->field_0x1c == 0x29) {
                        void **plp = (void **)RB_Level_getPlayer(level);
                        if (RB_Player_getShieldDamageRate((void *)*plp) < 100) {
                            int sh = RB_Status_getShip();
                            int eq = RB_Ship_getFirstEquipmentOfSort(sh);
                            float a = (float)RB_Item_getAttribute(eq);
                            float *charge = (float *)((char *)((int *)self->field_0x18[1]) + off);
                            float nv = *charge + (shieldAmt * a) / scaleDiv;
                            *charge = nv;
                            if (nv < 1.0f) {
                                RB_Player_damage(*(int *)((char *)enemyObj + 4), true, 0);
                                *charge = *charge - 1.0f;
                            }
                            void **plp2 = (void **)RB_Level_getPlayer(level);
                            int sh2 = RB_Status_getShip();
                            int eq2 = RB_Ship_getFirstEquipmentOfSort(sh2);
                            float a2 = (float)RB_Item_getAttribute(eq2);
                            RB_Player_regenerateShield((void *)*plp2, (shieldAmt * a2) / scaleDiv);
                        }
                    } else if (self->field_0x1c == 0x25) {
                        int pl = ((int *)enemyObj)[1];
                        int sh = RB_Status_getShip();
                        int eq = RB_Ship_getFirstEquipmentOfSort(sh);
                        float a = (float)RB_Item_getAttribute(eq);
                        RB_Player_heal(pl, (healAmt * a) / scaleDiv);
                    }

                    int snd = *g_RB_sndPlay;
                    int *evArr = g_RB_sndPlayEv;
                    if (RB_FModSound_isPlaying(snd) == 0) {
                        RB_FModSound_play(snd, (void *)evArr[self->field_0x0], 0, 0.0f);
                    }
                    allInactive = false;
                }
                off = off + 4;
            }
            if (allInactive) {
                RB_FModSound_stop(**g_RB_sndStop);
            }
        }
    }

    // stop beam sound when player dead
    void *ego2 = RB_Level_getPlayer(level);
    if (RB_PlayerEgo_isDead(ego2) != 0) {
        int snd = *g_RB_sndDead;
        if (RB_FModSound_isPlaying(snd) != 0)
            RB_FModSound_stop(snd);
    }

    // 3D-attribute update of the looping sound
    if ((*g_RB_sndFlag)[0xf] != 0) {
        int *evArr = g_RB_sndUpdEv;
        int *snd = *g_RB_sndUpd;
        if (RB_FModSound_isPlaying(*snd) != 0) {
            int shipIdx = self->field_0x0;
            RB_Level_getPlayer(level);
            Vector tmp;
            RB_PlayerEgo_getPosition(&tmp);
            RB_Vector_assign((Vector *)((char *)self + 4), &tmp);
            RB_FModSound_updateEvent3DAttributes(snd, evArr[shipIdx],
                                                 (Vector *)((char *)self + 4), 0, false);
        }
    }

    if (*canary != saved) {
        RB___stack_chk_fail();
    }
}
