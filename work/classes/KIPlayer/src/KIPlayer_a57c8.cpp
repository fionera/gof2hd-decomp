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
    self->f_90 = 0;  self->f_94 = 0;
    self->f_98 = 0;  self->f_9c = 0;
    self->f_10c = 0; self->f_110 = 0;
    self->f_114 = 0; self->f_118 = 0;
    self->f_2c = 0;  self->f_30 = 0;  self->f_34 = 0;
    self->f_54 = 0;  self->f_6c = 0;  F<unsigned>(self, 0xb4) = 0;
    self->f_a0 = 0;  self->f_a4 = 0;
    self->f_11c = 0; self->f_120 = 0;

    self->f_4 = player;
    self->f_8 = 0;
    self->f_c = 0;
    self->f_c4 = 0;
    self->f_c8 = 0;
    self->f_cc = 0;

    // geometry: when a parent geometry is supplied (and it has children) build a child group.
    bool haveChild = (geom != 0) && (*(int *)((char *)geom + 0x18) != 0);
    if (geom != 0 && haveChild) {
        self->f_c = geom;
        void *child = operator_new(0xc0);
        AEGeometry_ctor2(child, *(void **)gCanvas);
        self->f_8 = child;
        AEGeometry_addChild(child, *(int *)((char *)self->f_c + 0xc));
        *(int *)((char *)self->f_c + 0x24) = *(int *)((char *)self->f_8 + 0xc);
    } else {
        self->f_8 = geom;
        self->f_c = 0;
    }

    self->f_50 = 0;
    self->f_28 = group;
    F<uint8_t>(self, 0x72) = 0;
    F<uint8_t>(self, 0x25) = 0;
    self->f_75 = 0;
    F<uint16_t>(self, 0x42) = 0;
    self->f_44 = 0;
    self->f_48 = -1;
    self->f_8c = 1;
    self->f_3c = 0;
    self->f_4c = 0;
    self->f_d0 = 0;
    self->f_68 = 0;
    F<uint8_t>(self, 0x6a) = 0;
    self->f_dc = 0;
    self->f_ec = 0;
    self->f_f4 = 0;

    {
        String12 tmp;
        String_ctor_char(&tmp, "", false);
        String_assign((char *)self + 0x18, &tmp);
        String_dtor(&tmp);
    }

    self->f_24 = 0;
    F<uint8_t>(self, 0xf5) = 1;
    self->f_e8 = 0;
    self->f_70 = 0;
    self->f_40 = 0;
    self->f_d8 = 0;
    self->f_80 = -1;
    self->f_84 = -1;
    self->f_104 = 0;

    Player_setKIPlayer(self->f_4, self);

    self->f_88 = 0;
    self->f_10 = 0;
    self->f_b1 = 0x100;
    self->f_bc = 0;
    self->f_c0 = 0xff;
    F<unsigned>(self, 0x78) = 0;
    self->f_a8 = KIPlayer_initB;
    self->f_ac = faction;
    self->f_64 = KIPlayer_initA;

    if (geom != 0) {
        AEGeometry_setPosition3f(geom, x, y, z);
        Matrix_assign((char *)self->f_4 + 4, AEGeometry_getMatrix());
        if (self->f_c != 0)
            Matrix_mulassign((char *)self->f_4 + 4, AEGeometry_getMatrix());
    }

    *(float *)((char *)self + 0x58) = x;
    *(float *)((char *)self + 0x5c) = y;
    *(float *)((char *)self + 0x60) = z;
    self->f_14 = 0;
    F<uint16_t>(self, 0x73) = 0;
    F<uint8_t>(self, 0x76) = 0;
    self->f_f8 = -1;
    self->f_fc = 0;
    self->f_100 = 0x100;
    (void)active;
}
