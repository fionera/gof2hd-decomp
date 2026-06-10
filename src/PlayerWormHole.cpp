#include "gof2/PlayerWormHole.h"
#include "gof2/GameText.h"
#include "gof2/Hud.h"
#include "gof2/KIPlayer.h"
// NOTE: gof2/Player.h is not included here because that header currently has an
// unresolved member/method name collision ('turnedEnemy' is both a data member at
// 0xe0 and a converted accessor method) that is owned by the Player batch. This TU
// only needs Player::setRadius, so a minimal local declaration is provided instead.
struct Player {
    void setRadius(int value);
};
#include "gof2/PlayerEgo.h"
#include "gof2/Station.h"
#include "gof2/String.h"


extern "C" void *PlayerStaticFar_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void PlayerStaticFar_ctor(PlayerWormHole *self, int playerId, AEGeometry *geometry, float x, float y, float z);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transform);
extern "C" void Transform_SetAnimationState(void *transform, int state, int frame);
extern "C" void AEGeometry_render(void *geometry);
extern "C" void AEGeometry_positionChanged(void *geometry);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transformId);
extern "C" void Transform_Update(void *transform, long long elapsed, bool flag);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" void AEGeometry_setVisible(void *geometry, bool visible);
extern "C" void PlayerStaticFar_update(PlayerWormHole *self, int elapsed);
extern "C" int PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, int current);
void MatrixGetPosition(void *out, void *matrix);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_getPosition(void *out, void *geometry);
extern "C" void Vector_sub_assign(Vector *self, Vector *rhs);
void VectorNormalize(void *out, Vector *value);
extern "C" void AEGeometry_setDirection(void *geometry, Vector *direction, Vector *up);
extern "C" void *Level_getPlayer(void *level);

// ---- isShrinking_a5324.cpp ----

bool PlayerWormHole::isShrinking()
{
    return this->field_0x150 > 60000;
}

// ---- _PlayerWormHole_a5302.cpp ----
void _ZN14PlayerWormHoleD0Ev(void *self)
{
    return operator_delete(PlayerStaticFar_dtor(self));
}

// ---- open_a5314.cpp ----

int PlayerWormHole::open(char *, int, ...)
{
    this->field_0x150 = -3000;
    this->field_0x154 = 0;
    return (int)(long)this;
}

// ---- getPosition_a57a0.cpp ----

Vector PlayerWormHole::getPosition()
{
    float x = (float)this->field_0x124;
    float y = (float)this->field_0x128;
    float z = (float)this->field_0x12c;
    Vector result = {x, y, z};
    return result;
}

// ---- PlayerWormHole_a5254.cpp ----


__attribute__((visibility("hidden"))) extern void **g_playerWormHole_text;
__attribute__((visibility("hidden"))) extern char *g_playerWormHole_vtable;
__attribute__((visibility("hidden"))) extern void **g_playerWormHole_canvas;

PlayerWormHole::PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible)
{
    PlayerStaticFar_ctor(this, playerId, geometry, x, y, z);
    void **textSource = g_playerWormHole_text;
    this->field_0x0 = g_playerWormHole_vtable + 8;
    AbyssEngine::String *text = (AbyssEngine::String *)((GameText *)(*textSource))->getText(0x221);
    ((String *)((AbyssEngine::String *)((char *)this + 0x18)))->assign(text);
    ((KIPlayer *)(this))->setVisible(visible);
    ((Player *)(this->field_0x4))->setRadius(40000);
    void *transform = PaintCanvas_TransformGetTransform(*g_playerWormHole_canvas, F<int>(this->field_0x8, 0xc));
    Transform_SetAnimationState(transform, 2, 0);
    this->field_0x15c = 1;
    this->field_0x150 = 0;
    this->field_0x154 = 0x1000;
}

// ---- freeMissionLock_a5378.cpp ----

void PlayerWormHole::freeMissionLock()
{
    this->field_0x15c = 0;
}

// ---- render_a536a.cpp ----


void PlayerWormHole::render()
{
    if (this->field_0xf5 == 0)
        return;
    return AEGeometry_render(this->field_0x8);
}

// ---- reset_a52ec.cpp ----

void PlayerWormHole::reset(bool shrinking)
{
    int value = 0;
    if (shrinking)
        value = 59000;
    this->field_0x150 = value;
    this->field_0x154 = 0x1000;
}

// ---- setPosition_a5336.cpp ----


void PlayerWormHole::setPosition(float x, float y, float z)
{
    this->field_0x58 = x;
    this->field_0x5c = y;
    this->field_0x60 = z;
    this->field_0x128 = (int)y;
    this->field_0x124 = (int)x;
    this->field_0x12c = (int)z;
    return AEGeometry_positionChanged(this->field_0x8);
}

// ---- update_a5380.cpp ----

typedef int (*RandomNextIntFn)(void *random, int limit);
typedef void (*VectorAssignFn)(Vector *dst, void *src);
typedef void *(*GetPlayerFn)(void *level);
typedef void (*SetPositionFn)(PlayerWormHole *self, float x, float y, float z);


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
        PaintCanvas_TransformGetTransform(canvas, F<int>(this->field_0x8, 0xc));
    Transform_Update(transform, (long long)elapsed, false);

    if (this->field_0xf5 == 0)
        return;

    int time = this->field_0x150 + elapsed;
    this->field_0x150 = time;
    if (time < 0) {
        this->field_0x154 =
            0x1000 - (int)(((float)-time / 3000.0f) * 4096.0f);
    } else if (time > 60000) {
        void **statusHolder = g_playerWormHole_update_status;
        void *status = *statusHolder;
        int mission = Status_getCurrentCampaignMission(status);
        int current = time;

        if (this->field_0x15c != 0) {
            bool lockTime = false;
            if (mission != 0x2a) {
                if (mission == 0x28 && Status_inAlienOrbit(status) == 0)
                    lockTime = true;
            } else if (Status_inAlienOrbit(status) != 0) {
                lockTime = true;
            }
            if (lockTime) {
                current = 60000;
                this->field_0x150 = 60000;
            }
        }

        this->field_0x154 =
            0x1000 - (int)(((float)(current - 60000) / 3000.0f) * 4096.0f);

        if (current > 63000) {
            int alien = Status_inAlienOrbit(status);
            if (alien != 0 ||
                ((Station *)(Status_getStation(status)))->isAttackedByAliens() != 0) {
                bool closeWormhole = false;
                if (this->field_0x15c != 0) {
                    int alienNow = Status_inAlienOrbit(status);
                    if (mission == 0x2a && alienNow == 0)
                        closeWormhole = true;
                }
                if (!closeWormhole) {
                    this->field_0x150 = -3000;

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
                    void *level = this->field_0x54;
                    if (mission == 0x1d || mission == 0x29) {
                        void *player = Level_getPlayer(level);
                        ((PlayerEgo *)(tmpOut))->getPosition();
                        VectorAssignFn assign = g_playerWormHole_update_vectorAssign;
                        assign(position, tmpOut);

                        z = (int)(this->field_0x98 + (float)z * 1.7f + (float)z);
                        y = (int)(this->field_0x94 + (float)y * 1.7f + (float)y);
                        x = (int)(this->field_0x90 + (float)x * 1.7f + (float)x);
                    }

                    SetPositionFn setPosition =
                        *(SetPositionFn *)((char *)this->field_0x0 + 0x48);
                    setPosition(this, (float)x, (float)y, (float)z);

                    void *player = Level_getPlayer(this->field_0x54);
                    if (((PlayerEgo *)(player))->goingToWormhole() != 0) {
                        GetPlayerFn getPlayer = g_playerWormHole_update_getPlayer;
                        void *target = getPlayer(this->field_0x54);
                        void *hud = (void *)(intptr_t)((PlayerEgo *)(target))->getHUD();
                        target = getPlayer(this->field_0x54);
                        ((Hud *)(hud))->hudEvent(6, target, 0);
                        target = getPlayer(this->field_0x54);
                        ((PlayerEgo *)(target))->setAutoPilot((void *)0);
                    }
                } else {
                    this->field_0xf5 = 0;
                    AEGeometry_setVisible(this->field_0x8, false);
                }
            } else {
                this->field_0xf5 = 0;
                AEGeometry_setVisible(this->field_0x8, false);
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

    float scale = (float)(this->field_0x154 << 4) * 0.0000152587890625f;
    AEGeometry_setScaling(this->field_0x8, scale, scale, scale);

    AEGeometry_getPosition(tmpOut, this->field_0x8);
    Vector *geometryPosition = (Vector *)((char *)this + 0x134);
    assign(geometryPosition, tmpOut);
    Vector_sub_assign(direction, geometryPosition);
    VectorNormalize(tmpOut, direction);
    assign(direction, tmpOut);
    this->field_0x90 = this->field_0x90 + 0.5f;

    tmp[0].x = 0.0f;
    tmp[0].y = 1.0f;
    tmp[0].z = 0.0f;
    AEGeometry_setDirection(this->field_0x8, direction, tmp);
}
