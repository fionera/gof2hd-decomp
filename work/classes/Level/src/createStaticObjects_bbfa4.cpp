#include "class.h"

struct Station;
struct PlayerFixedObject;
struct Player;
struct KIPlayer;
struct String;
struct Vector;

__attribute__((visibility("hidden"))) extern int  *g_cso_stack;     // [DAT_000cc1e4]
__attribute__((visibility("hidden"))) extern int **g_cso_textA;     // [DAT_000cc1ec]
__attribute__((visibility("hidden"))) extern int **g_cso_textB;     // [DAT_000cc1f0] area
__attribute__((visibility("hidden"))) extern float g_cso_posX;      // DAT_000cc1dc
__attribute__((visibility("hidden"))) extern float g_cso_posZ;      // DAT_000cc1e0

extern "C" {
int  Status_inAlienOrbit_cso();
int  Status_getStation_cso();
int  Station_getIndex_cso(Station *s);
int  Status_getCurrentCampaignMission_cso();
int  Level_createStaticObject_cso(Level *self, int wp, int type, int turret);
void PlayerFixedObject_setMoving_cso(PlayerFixedObject *o, int flag);
void StarSystem_getLightDirection_cso(void *dst);
void AEGeometry_setDirection_cso(Vector *geo, Vector *dir);
int  GameText_getText_cso(int id);
void String_assign_cso(String *dst, String *src);
void String_ctor_cso(String *dst, String *src, int own);
void String_dtor_cso(String *s);
void PlayerFixedObject_setName_cso(PlayerFixedObject *o, String *n);
void PlayerFixedObject_setDockingType_cso(PlayerFixedObject *o, int t);
void Player_setAlwaysFriend_cso(Player *p, int flag);
void *Level_opnew_cso(unsigned size);
void ArrayKIPlayer_ctor_cso(void *a);
void *Array_int_dtor_cso(void *a);
void operator_delete_cso(void *p);
void ArrayAdd_KIPlayer_cso(KIPlayer *k, void *a);
}

// Level::createStaticObjects() — spawns campaign-specific landmark/escort objects.
void Level::createStaticObjects()
{
    char *self = (char *)this;

    // --- carrier escort that follows the campaign progression (station 0x70) ---
    if (Status_inAlienOrbit_cso() == 0) {
        Station *st = (Station *)Status_getStation_cso();
        if (Station_getIndex_cso(st) == 0x70 &&
            0x7f < Status_getCurrentCampaignMission_cso()) {
            int type;
            if (Status_getCurrentCampaignMission_cso() < 0x83)      type = 0x493e;
            else if (Status_getCurrentCampaignMission_cso() < 0x87) type = 0x4941;
            else if (Status_getCurrentCampaignMission_cso() < 0x8a) type = 0x4944;
            else if (Status_getCurrentCampaignMission_cso() < 0x8e) type = 0x4947;
            else if (0x91 < Status_getCurrentCampaignMission_cso()) type = -1;
            else                                                    type = 0x494a;

            if (type != -1) {
                char *o = (char *)Level_createStaticObject_cso(this, 0, type, 0);
                (*(void (**)(char *, float, int, float))(*(int *)o + 0x48))(o, g_cso_posX, 0,
                                                                           g_cso_posZ);
                PlayerFixedObject_setMoving_cso((PlayerFixedObject *)o, 0);
                Vector *geo = *(Vector **)(o + 8);
                *(char *)(o + 0x70) = 0;
                char dir[12];
                StarSystem_getLightDirection_cso(dir);
                AEGeometry_setDirection_cso(geo, (Vector *)dir);
                String *txt = (String *)GameText_getText_cso(**g_cso_textA);
                String_assign_cso((String *)(o + 0x18), txt);
                Player_setAlwaysFriend_cso(*(Player **)(o + 4), 1);
                void *arr = *(void **)(self + 0xf8);
                if (arr == 0) {
                    arr = Level_opnew_cso(0xc);
                    ArrayKIPlayer_ctor_cso(arr);
                    *(void **)(self + 0xf8) = arr;
                }
                ArrayAdd_KIPlayer_cso((KIPlayer *)o, arr);
                if (*(void **)(o + 0x50) != 0)
                    operator_delete_cso(Array_int_dtor_cso(*(void **)(o + 0x50)));
                *(int *)(o + 0x50) = 0;
            }
        }
    }

    // --- station 0x67 dockable beacon (mid/late campaign) ---
    if (0x54 < Status_getCurrentCampaignMission_cso() &&
        Status_getCurrentCampaignMission_cso() != 0x87 &&
        Status_inAlienOrbit_cso() == 0) {
        Station *st = (Station *)Status_getStation_cso();
        if (Station_getIndex_cso(st) == 0x67) {
            char *o = (char *)Level_createStaticObject_cso(this, 0, 0x4a88, 0);
            (*(void (**)(char *, int, int, int))(*(int *)o + 0x48))(o, 0, 0, 0);
            PlayerFixedObject_setMoving_cso((PlayerFixedObject *)o, 0);
            *(char *)(o + 0x70) = 1;
            String *txt = (String *)GameText_getText_cso(**g_cso_textB);
            char name[12];
            String_ctor_cso((String *)name, txt, 0);
            PlayerFixedObject_setName_cso((PlayerFixedObject *)o, (String *)name);
            String_dtor_cso((String *)name);
            PlayerFixedObject_setDockingType_cso((PlayerFixedObject *)o, 1);
            if (*(void **)(o + 0x50) != 0)
                operator_delete_cso(Array_int_dtor_cso(*(void **)(o + 0x50)));
            *(int *)(o + 0x50) = 0;
            Player_setAlwaysFriend_cso(*(Player **)(o + 4), 1);
            void *arr = *(void **)(self + 0xf8);
            if (arr == 0) {
                arr = Level_opnew_cso(0xc);
                ArrayKIPlayer_ctor_cso(arr);
                *(void **)(self + 0xf8) = arr;
            }
            ArrayAdd_KIPlayer_cso((KIPlayer *)o, arr);
        }
    }
}
