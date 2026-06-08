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
    this->f_0 = g_playerWormHole_vtable + 8;
    AbyssEngine::String *text = GameText_getText(*textSource, 0x221);
    String_assign((AbyssEngine::String *)((char *)this + 0x18), text);
    KIPlayer_setVisible(this, visible);
    Player_setRadius(this->f_4, 40000);
    void *transform = PaintCanvas_TransformGetTransform(*g_playerWormHole_canvas, F<int>(this->f_8, 0xc));
    Transform_SetAnimationState(transform, 2, 0);
    this->f_15c = 1;
    this->f_150 = 0;
    this->f_154 = 0x1000;
}
