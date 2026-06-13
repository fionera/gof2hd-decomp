#include "gof2/game/ship/PlayerWormHole.h"
// engine shim (unmapped)
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/ship/PlayerStaticFar.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/ship/KIPlayer.h"
// NOTE: gof2/Player.h is not included here because that header currently has an
// unresolved member/method name collision ('turnedEnemy' is both a data member at
// 0xe0 and a converted accessor method) that is owned by the Player batch. This TU
// only needs Player::setRadius, so a minimal local declaration is provided instead.
struct Player {
    void setRadius(int value);
};
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/core/String.h"

extern "C" void PlayerStaticFar_ctor(PlayerWormHole *self, int playerId, AEGeometry *geometry, float x, float y, float z);
// UNRECOVERED: this AEGeometry callback is reached through a runtime ARM->Thumb
// veneer (PlayerWormHole::setPosition at a5336 calls *(fp)(geometry)); the indirect
// target could not be resolved to a named AEGeometry method, so the shim is retained.
namespace AbyssEngine { namespace PaintCanvas {
void *TransformGetTransform(void *canvas, int transformId);
int   CameraGetCurrent(void *canvas);
void *CameraGetLocal(void *canvas, int current);
} }
void MatrixGetPosition(void *out, void *matrix);
void VectorNormalize(void *out, Vector *value);

bool PlayerWormHole::isShrinking()
{
    return this->timer > 60000;
}

void _ZN14PlayerWormHoleD0Ev(void *self)
{
    ((PlayerStaticFar *)(self))->dtor();
    ::operator delete(self);
}

int PlayerWormHole::open(char *, int, ...)
{
    this->timer = -3000;
    this->field_0x154 = 0;
    return (int)(long)this;
}

Vector PlayerWormHole::getPosition()
{
    float x = (float)this->intPositionX;
    float y = (float)this->intPositionY;
    float z = (float)this->intPositionZ;
    Vector result = {x, y, z};
    return result;
}

__attribute__((visibility("hidden"))) extern void **g_playerWormHole_text;
__attribute__((visibility("hidden"))) extern char *g_playerWormHole_vtable;
__attribute__((visibility("hidden"))) extern void **g_playerWormHole_canvas;

PlayerWormHole::PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible)
{
    PlayerStaticFar_ctor(this, playerId, geometry, x, y, z);
    void **textSource = g_playerWormHole_text;
    this->vtable = g_playerWormHole_vtable + 8;
    AbyssEngine::String *text = (AbyssEngine::String *)((GameText *)(*textSource))->getText(0x221);
    ((String *)((AbyssEngine::String *)((char *)this + 0x18)))->assign(text);
    ((KIPlayer *)(this))->setVisible(visible);
    ((Player *)(this->player))->setRadius(40000);
    void *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*g_playerWormHole_canvas, F<int>(this->geometry, 0xc));
    ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)2, 0);
    this->missionLock = 1;
    this->timer = 0;
    this->field_0x154 = 0x1000;
}

void PlayerWormHole::freeMissionLock()
{
    this->missionLock = 0;
}

void PlayerWormHole::render()
{
    if (this->visible == 0)
        return;
    return ((AEGeometry *)(this->geometry))->render();
}

void PlayerWormHole::reset(bool shrinking)
{
    int value = 0;
    if (shrinking)
        value = 59000;
    this->timer = value;
    this->field_0x154 = 0x1000;
}

void PlayerWormHole::setPosition(float x, float y, float z)
{
    this->positionX = x;
    this->positionY = y;
    this->positionZ = z;
    this->intPositionY = (int)y;
    this->intPositionX = (int)x;
    this->intPositionZ = (int)z;
    return ((AEGeometry *)(this->geometry))->positionChanged();
}

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
        AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<int>(this->geometry, 0xc));
    ((AbyssEngine::Transform *)(transform))->Update((long long)elapsed, false);

    if (this->visible == 0)
        return;

    int time = this->timer + elapsed;
    this->timer = time;
    if (time < 0) {
        this->field_0x154 =
            0x1000 - (int)(((float)-time / 3000.0f) * 4096.0f);
    } else if (time > 60000) {
        void **statusHolder = g_playerWormHole_update_status;
        void *status = *statusHolder;
        int mission = ((Status *)(status))->getCurrentCampaignMission();
        int current = time;

        if (this->missionLock != 0) {
            bool lockTime = false;
            if (mission != 0x2a) {
                if (mission == 0x28 && ((Status *)(status))->inAlienOrbit() == 0)
                    lockTime = true;
            } else if (((Status *)(status))->inAlienOrbit() != 0) {
                lockTime = true;
            }
            if (lockTime) {
                current = 60000;
                this->timer = 60000;
            }
        }

        this->field_0x154 =
            0x1000 - (int)(((float)(current - 60000) / 3000.0f) * 4096.0f);

        if (current > 63000) {
            int alien = ((Status *)(status))->inAlienOrbit();
            if (alien != 0 ||
                ((Station *)(((Status *)(status))->getStation()))->isAttackedByAliens() != 0) {
                bool closeWormhole = false;
                if (this->missionLock != 0) {
                    int alienNow = ((Status *)(status))->inAlienOrbit();
                    if (mission == 0x2a && alienNow == 0)
                        closeWormhole = true;
                }
                if (!closeWormhole) {
                    this->timer = -3000;

                    alien = ((Status *)(status))->inAlienOrbit();
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
                    void *level = this->level;
                    if (mission == 0x1d || mission == 0x29) {
                        void *player = (void *)(intptr_t)((Level *)(level))->getPlayer();
                        ((PlayerEgo *)(tmpOut))->getPosition();
                        VectorAssignFn assign = g_playerWormHole_update_vectorAssign;
                        assign(position, tmpOut);

                        z = (int)(this->directionZ + (float)z * 1.7f + (float)z);
                        y = (int)(this->directionY + (float)y * 1.7f + (float)y);
                        x = (int)(this->directionX + (float)x * 1.7f + (float)x);
                    }

                    SetPositionFn setPosition =
                        *(SetPositionFn *)((char *)this->vtable + 0x48);
                    setPosition(this, (float)x, (float)y, (float)z);

                    void *player = (void *)(intptr_t)((Level *)(this->level))->getPlayer();
                    if (((PlayerEgo *)(player))->goingToWormhole() != 0) {
                        GetPlayerFn getPlayer = g_playerWormHole_update_getPlayer;
                        void *target = getPlayer(this->level);
                        void *hud = (void *)(intptr_t)((PlayerEgo *)(target))->getHUD();
                        target = getPlayer(this->level);
                        ((Hud *)(hud))->hudEvent(6, target, 0);
                        target = getPlayer(this->level);
                        ((PlayerEgo *)(target))->setAutoPilot((void *)0);
                    }
                } else {
                    this->visible = 0;
                    ((AEGeometry *)(this->geometry))->setVisible(false);
                }
            } else {
                this->visible = 0;
                ((AEGeometry *)(this->geometry))->setVisible(false);
            }
        }
    }

    ((PlayerStaticFar *)(this))->update(elapsed);
    canvas = *canvasHolder;
    int currentCamera = AbyssEngine::PaintCanvas::CameraGetCurrent(canvas);
    MatrixGetPosition(tmpOut, AbyssEngine::PaintCanvas::CameraGetLocal(canvas, currentCamera));

    Vector *direction = (Vector *)((char *)this + 0x90);
    VectorAssignFn assign = g_playerWormHole_update_vectorAssign;
    assign(direction, tmpOut);

    float scale = (float)(this->field_0x154 << 4) * 0.0000152587890625f;
    ((AEGeometry *)(this->geometry))->setScaling(scale);

    ((AEGeometry *)(tmpOut))->getPosition();
    Vector *geometryPosition = (Vector *)((char *)this + 0x134);
    assign(geometryPosition, tmpOut);
    *direction -= *geometryPosition;
    VectorNormalize(tmpOut, direction);
    assign(direction, tmpOut);
    this->directionX = this->directionX + 0.5f;

    tmp[0].x = 0.0f;
    tmp[0].y = 1.0f;
    tmp[0].z = 0.0f;
    ((AEGeometry *)(this->geometry))->setDirection(*direction, *tmp);
}
