#include "class.h"

// 12-byte AbyssEngine::String, constructed/destroyed via engine wrappers.
struct String12 { unsigned a, b, c; };

extern "C" {
void *operator_new(unsigned int sz);                             // 0x6eb24
void *gCanvas;                                                   // engine PaintCanvas holder
extern char KIPlayer_vtable[];                                   // KIPlayer vtable

void String_ctor_default(void *s);                              // 0x6efbc  String::String()
void String_ctor_char(void *s, const char *t, bool copy);       // 0x6ee18  String(char*,bool)
void String_assign(void *dst, void *src);                       // 0x6f2b0  operator=
void String_dtor(void *s);                                      // 0x6ee30  ~String

void AEGeometry_ctor2(void *self, void *canvas);                // 0x720f4
void AEGeometry_addChild(void *self, int child);               // 0x720e8
void *AEGeometry_getMatrix(void);                               // 0x721cc
void AEGeometry_setPosition3f(void *geom, float x, float y, float z);   // 0x73048
void Matrix_assign(void *dst, void *src);                       // 0x6f148
void Matrix_mulassign(void *dst, void *src);                    // 0x6f4a8
void Player_setKIPlayer(void *player, KIPlayer *ki);            // 0x73288

// Two engine constants captured from PC-relative slots (init colour/flags).
extern unsigned KIPlayer_initA;                                 // DAT_000b59dc
extern unsigned KIPlayer_initB;                                 // DAT_000b59e0
}

// KIPlayer::KIPlayer(int faction, int group, Player* player, AEGeometry* geom,
//                    float x, float y, float z, bool active)
extern "C" void KIPlayer_ctor(KIPlayer *self, int faction, int group, void *player, void *geom,
                              float x, float y, float z, bool active)
{
    *(int *)self = (int)(KIPlayer_vtable + 8);
    String_ctor_default((char *)self + 0x18);

    // zero a wide block of fields (the engine writes zero-vectors via NEON stores).
    F<unsigned>(self, 0x90) = 0;  F<unsigned>(self, 0x94) = 0;
    F<unsigned>(self, 0x98) = 0;  F<unsigned>(self, 0x9c) = 0;
    F<unsigned>(self, 0x10c) = 0; F<unsigned>(self, 0x110) = 0;
    F<unsigned>(self, 0x114) = 0; F<unsigned>(self, 0x118) = 0;
    F<unsigned>(self, 0x2c) = 0;  F<unsigned>(self, 0x30) = 0;  F<unsigned>(self, 0x34) = 0;
    F<unsigned>(self, 0x54) = 0;  F<unsigned>(self, 0x6c) = 0;  F<unsigned>(self, 0xb4) = 0;
    F<unsigned>(self, 0xa0) = 0;  F<unsigned>(self, 0xa4) = 0;
    F<unsigned>(self, 0x11c) = 0; F<unsigned>(self, 0x120) = 0;

    F<void *>(self, 0x4) = player;
    F<unsigned>(self, 0x8) = 0;
    F<unsigned>(self, 0xc) = 0;
    F<unsigned>(self, 0xc4) = 0;
    F<unsigned>(self, 0xc8) = 0;
    F<unsigned>(self, 0xcc) = 0;

    // geometry: when a parent geometry is supplied (and it has children) build a child group.
    bool haveChild = (geom != 0) && (*(int *)((char *)geom + 0x18) != 0);
    if (geom != 0 && haveChild) {
        F<void *>(self, 0xc) = geom;
        void *child = operator_new(0xc0);
        AEGeometry_ctor2(child, *(void **)gCanvas);
        F<void *>(self, 0x8) = child;
        AEGeometry_addChild(child, *(int *)((char *)F<void *>(self, 0xc) + 0xc));
        *(int *)((char *)F<void *>(self, 0xc) + 0x24) = *(int *)((char *)F<void *>(self, 0x8) + 0xc);
    } else {
        F<void *>(self, 0x8) = geom;
        F<unsigned>(self, 0xc) = 0;
    }

    F<unsigned>(self, 0x50) = 0;
    F<int>(self, 0x28) = group;
    F<uint8_t>(self, 0x72) = 0;
    F<uint8_t>(self, 0x25) = 0;
    F<uint8_t>(self, 0x75) = 0;
    F<uint16_t>(self, 0x42) = 0;
    F<uint8_t>(self, 0x44) = 0;
    F<int>(self, 0x48) = -1;
    F<uint8_t>(self, 0x8c) = 1;
    F<unsigned>(self, 0x3c) = 0;
    F<uint8_t>(self, 0x4c) = 0;
    F<uint8_t>(self, 0xd0) = 0;
    F<uint16_t>(self, 0x68) = 0;
    F<uint8_t>(self, 0x6a) = 0;
    F<uint8_t>(self, 0xdc) = 0;
    F<uint8_t>(self, 0xec) = 0;
    F<uint8_t>(self, 0xf4) = 0;

    {
        String12 tmp;
        String_ctor_char(&tmp, "", false);
        String_assign((char *)self + 0x18, &tmp);
        String_dtor(&tmp);
    }

    F<uint8_t>(self, 0x24) = 0;
    F<uint8_t>(self, 0xf5) = 1;
    F<unsigned>(self, 0xe8) = 0;
    F<uint16_t>(self, 0x70) = 0;
    F<uint16_t>(self, 0x40) = 0;
    F<unsigned>(self, 0xd8) = 0;
    F<int>(self, 0x80) = -1;
    F<int>(self, 0x84) = -1;
    F<unsigned>(self, 0x104) = 0;

    Player_setKIPlayer(F<void *>(self, 0x4), self);

    F<unsigned>(self, 0x88) = 0;
    F<unsigned>(self, 0x10) = 0;
    F<uint16_t>(self, 0xb1) = 0x100;
    F<uint8_t>(self, 0xbc) = 0;
    F<unsigned>(self, 0xc0) = 0xff;
    F<unsigned>(self, 0x78) = 0;
    F<unsigned>(self, 0xa8) = KIPlayer_initB;
    F<int>(self, 0xac) = faction;
    F<unsigned>(self, 0x64) = KIPlayer_initA;

    if (geom != 0) {
        AEGeometry_setPosition3f(geom, x, y, z);
        Matrix_assign((char *)F<void *>(self, 0x4) + 4, AEGeometry_getMatrix());
        if (F<int>(self, 0xc) != 0)
            Matrix_mulassign((char *)F<void *>(self, 0x4) + 4, AEGeometry_getMatrix());
    }

    *(float *)((char *)self + 0x58) = x;
    *(float *)((char *)self + 0x5c) = y;
    *(float *)((char *)self + 0x60) = z;
    F<unsigned>(self, 0x14) = 0;
    F<uint16_t>(self, 0x73) = 0;
    F<uint8_t>(self, 0x76) = 0;
    F<int>(self, 0xf8) = -1;
    F<unsigned>(self, 0xfc) = 0;
    F<uint16_t>(self, 0x100) = 0x100;
    (void)active;
}
