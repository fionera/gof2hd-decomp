#include "class.h"

struct PlayerWormHole {
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    unsigned char _pad_c[72];
    void* f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    unsigned char _pad_64[44];
    int f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    unsigned char _pad_9c[89];
    int f_f5; // 0xf5
    unsigned char _pad_f9[43];
    int f_124; // 0x124
    int f_128; // 0x128
    int f_12c; // 0x12c
    unsigned char _pad_130[32];
    int f_150; // 0x150
    int f_154; // 0x154
    unsigned char _pad_158[4];
    int f_15c; // 0x15c

    void update(int elapsed);
};

typedef int (*RandomNextIntFn)(void *random, int limit);
typedef void (*VectorAssignFn)(Vector *dst, void *src);
typedef void *(*GetPlayerFn)(void *level);
typedef void (*SetPositionFn)(PlayerWormHole *self, float x, float y, float z);

extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transformId);
extern "C" void Transform_Update(void *transform, long long elapsed, bool flag);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" int Station_isAttackedByAliens(void *station);
extern "C" void AEGeometry_setVisible(void *geometry, bool visible);
extern "C" void PlayerStaticFar_update(PlayerWormHole *self, int elapsed);
extern "C" int PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, int current);
extern "C" void MatrixGetPosition(void *out, void *matrix);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_getPosition(void *out, void *geometry);
extern "C" void Vector_sub_assign(Vector *self, Vector *rhs);
extern "C" void VectorNormalize(void *out, Vector *value);
extern "C" void AEGeometry_setDirection(void *geometry, Vector *direction, Vector *up);
extern "C" void *Level_getPlayer(void *level);
extern "C" void PlayerEgo_getPosition(void *out, void *player);
extern "C" int PlayerEgo_goingToWormhole(void *player);
extern "C" void *PlayerEgo_getHUD(void *player);
extern "C" void Hud_hudEvent(void *hud, int event, void *player, int value);
extern "C" void PlayerEgo_setAutoPilot(void *player, bool active);

__attribute__((visibility("hidden"))) extern void **g_playerWormHole_update_canvas;
__attribute__((visibility("hidden"))) extern void **g_playerWormHole_update_status;
__attribute__((visibility("hidden"))) extern void **g_playerWormHole_update_random;
__attribute__((visibility("hidden"))) extern RandomNextIntFn g_playerWormHole_update_randomAlien;
__attribute__((visibility("hidden"))) extern RandomNextIntFn g_playerWormHole_update_randomNormal;
__attribute__((visibility("hidden"))) extern GetPlayerFn g_playerWormHole_update_getPlayer;
__attribute__((visibility("hidden"))) extern VectorAssignFn g_playerWormHole_update_vectorAssign;

static inline int wormholeSign(int value)
{
    return value == 0 ? 1 : -1;
}

void PlayerWormHole::update(int elapsed)
{
    char tmpStorage[13];
    void *tmpOut = (void *)tmpStorage;
    Vector *tmp = (Vector *)tmpOut;

    void **canvasHolder = g_playerWormHole_update_canvas;
    void *canvas = *canvasHolder;
    void *transform =
        PaintCanvas_TransformGetTransform(canvas, F<int>(this->f_8, 0xc));
    Transform_Update(transform, (long long)elapsed, false);

    if (this->f_f5 == 0)
        return;

    int time = this->f_150 + elapsed;
    this->f_150 = time;
    if (time < 0) {
        this->f_154 =
            0x1000 - (int)(((float)-time / 3000.0f) * 4096.0f);
    } else if (time > 60000) {
        void **statusHolder = g_playerWormHole_update_status;
        void *status = *statusHolder;
        int mission = Status_getCurrentCampaignMission(status);
        int current = time;

        if (this->f_15c != 0) {
            bool lockTime = false;
            if (mission != 0x2a) {
                if (mission == 0x28 && Status_inAlienOrbit(status) == 0)
                    lockTime = true;
            } else if (Status_inAlienOrbit(status) != 0) {
                lockTime = true;
            }
            if (lockTime) {
                current = 60000;
                this->f_150 = 60000;
            }
        }

        this->f_154 =
            0x1000 - (int)(((float)(current - 60000) / 3000.0f) * 4096.0f);

        if (current > 63000) {
            int alien = Status_inAlienOrbit(status);
            if (alien != 0 ||
                Station_isAttackedByAliens(Status_getStation(status)) != 0) {
                bool closeWormhole = false;
                if (this->f_15c != 0) {
                    int alienNow = Status_inAlienOrbit(status);
                    if (mission == 0x2a && alienNow == 0)
                        closeWormhole = true;
                }
                if (!closeWormhole) {
                    this->f_150 = -3000;

                    alien = Status_inAlienOrbit(status);
                    void **randomHolder = g_playerWormHole_update_random;
                    void *random = *randomHolder;
                    int x;
                    int y;
                    int z;
                    if (alien == 0) {
                        RandomNextIntFn next = g_playerWormHole_update_randomNormal;
                        x = (next(random, 40000) + 20000) *
                            wormholeSign(next(random, 2));
                        y = (next(random, 40000) + 20000) *
                            wormholeSign(next(random, 2));
                        z = (next(random, 40000) + 20000) *
                            wormholeSign(next(random, 2));
                    } else {
                        RandomNextIntFn next = g_playerWormHole_update_randomAlien;
                        x = (next(random, 60000) + 30000) *
                            wormholeSign(next(random, 2));
                        y = next(random, 40000) + 20000;
                        z = -60000 - next(random, 40000);
                    }

                    Vector *position = (Vector *)((char *)this + 0x90);
                    void *level = this->f_54;
                    if (mission == 0x1d || mission == 0x29) {
                        void *player = Level_getPlayer(level);
                        PlayerEgo_getPosition(tmpOut, player);
                        VectorAssignFn assign = g_playerWormHole_update_vectorAssign;
                        assign(position, tmpOut);

                        z = (int)(this->f_98 + (float)z * 1.7f + (float)z);
                        y = (int)(this->f_94 + (float)y * 1.7f + (float)y);
                        x = (int)(this->f_90 + (float)x * 1.7f + (float)x);
                    }

                    SetPositionFn setPosition =
                        *(SetPositionFn *)((char *)this->f_0 + 0x48);
                    setPosition(this, (float)x, (float)y, (float)z);

                    void *player = Level_getPlayer(this->f_54);
                    if (PlayerEgo_goingToWormhole(player) != 0) {
                        GetPlayerFn getPlayer = g_playerWormHole_update_getPlayer;
                        void *target = getPlayer(this->f_54);
                        void *hud = PlayerEgo_getHUD(target);
                        target = getPlayer(this->f_54);
                        Hud_hudEvent(hud, 6, target, 0);
                        target = getPlayer(this->f_54);
                        PlayerEgo_setAutoPilot(target, false);
                    }
                } else {
                    this->f_f5 = 0;
                    AEGeometry_setVisible(this->f_8, false);
                }
            } else {
                this->f_f5 = 0;
                AEGeometry_setVisible(this->f_8, false);
            }
        }
    }

    PlayerStaticFar_update(this, elapsed);
    canvas = *canvasHolder;
    int currentCamera = PaintCanvas_CameraGetCurrent(canvas);
    MatrixGetPosition(tmpOut, PaintCanvas_CameraGetLocal(canvas, currentCamera));

    Vector *direction = (Vector *)((char *)this + 0x90);
    VectorAssignFn assign = g_playerWormHole_update_vectorAssign;
    assign(direction, tmpOut);

    float scale = (float)(this->f_154 << 4) * 0.0000152587890625f;
    AEGeometry_setScaling(this->f_8, scale, scale, scale);

    AEGeometry_getPosition(tmpOut, this->f_8);
    Vector *geometryPosition = (Vector *)((char *)this + 0x134);
    assign(geometryPosition, tmpOut);
    Vector_sub_assign(direction, geometryPosition);
    VectorNormalize(tmpOut, direction);
    assign(direction, tmpOut);
    this->f_90 = this->f_90 + 0.5f;

    tmp[0].x = 0.0f;
    tmp[0].y = 1.0f;
    tmp[0].z = 0.0f;
    AEGeometry_setDirection(this->f_8, direction, tmp);
}
