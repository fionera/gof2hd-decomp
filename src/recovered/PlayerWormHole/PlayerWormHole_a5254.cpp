#include "class.h"

struct PlayerWormHole {
    PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);
};

extern "C" void PlayerStaticFar_ctor(PlayerWormHole *self, int playerId, AEGeometry *geometry, float x, float y, float z);
extern "C" AbyssEngine::String *GameText_getText(void *gameText, int id);
extern "C" void String_assign(AbyssEngine::String *dst, AbyssEngine::String *src);
extern "C" void KIPlayer_setVisible(PlayerWormHole *self, bool visible);
extern "C" void Player_setRadius(void *player, int radius);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transform);
extern "C" void Transform_SetAnimationState(void *transform, int state, int frame);

__attribute__((visibility("hidden"))) extern void **g_playerWormHole_text;
__attribute__((visibility("hidden"))) extern char *g_playerWormHole_vtable;
__attribute__((visibility("hidden"))) extern void **g_playerWormHole_canvas;

PlayerWormHole::PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible)
{
    PlayerStaticFar_ctor(this, playerId, geometry, x, y, z);
    void **textSource = g_playerWormHole_text;
    F<char *>(this, 0x0) = g_playerWormHole_vtable + 8;
    AbyssEngine::String *text = GameText_getText(*textSource, 0x221);
    String_assign((AbyssEngine::String *)((char *)this + 0x18), text);
    KIPlayer_setVisible(this, visible);
    Player_setRadius(F<void *>(this, 0x4), 40000);
    void *transform = PaintCanvas_TransformGetTransform(*g_playerWormHole_canvas, F<int>(F<void *>(this, 0x8), 0xc));
    Transform_SetAnimationState(transform, 2, 0);
    F<uint8_t>(this, 0x15c) = 1;
    F<int>(this, 0x150) = 0;
    F<int>(this, 0x154) = 0x1000;
}
