#ifndef GOF2_OBJECTGUN_H
#define GOF2_OBJECTGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct AEGeometry;
struct Explosion;
struct Gun;
struct Level;
struct ObjectGun;
struct PaintCanvas;
struct Player;
struct TargetFollowCamera;





struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct MeshId {
    uint16_t id;
    uint16_t pad;
};

class ObjectGun {
public:
    void* field_0x0;                    // +0x0
    int field_0x4;                      // +0x4
    Gun* field_0x8;                     // +0x8
    Level* field_0xc;                   // +0xc
    uint32_t field_0x10;                // +0x10
    int field_0x14;                     // +0x14
    AEGeometry* field_0x18;             // +0x18
    uint8_t field_0x1c;                 // +0x1c
    uint8_t field_0x1d;                 // +0x1d
    uint32_t field_0x20;                // +0x20
    uint8_t field_0x24;                 // +0x24
    int field_0x28;                     // +0x28
    Array<Explosion*>* field_0x2c;      // +0x2c explosions for cluster guns
    uint8_t* field_0x30;                // +0x30 per-explosion ready flags
    int field_0x34;                     // +0x34
    float field_0x3c;                   // +0x3c scale x
    float field_0x40;                   // +0x40 scale y
    float field_0x44;                   // +0x44 scale z
    float field_0x48;                   // +0x48
    uint8_t field_0x4c;                 // +0x4c
    float field_0x50;                   // +0x50  Vector (dir) x
    float field_0x54;                   // +0x54  y
    float field_0x58;                   // +0x58  z
    float field_0x5c;                   // +0x5c  Vector (up) x
    float field_0x60;                   // +0x60  y
    float field_0x64;                   // +0x64  z
    float field_0x68;                   // +0x68  Vector (side) x
    float field_0x6c;                   // +0x6c  y
    float field_0x70;                   // +0x70  z
    float field_0x74;                   // +0x74  Matrix start
    float field_0x78;                   // +0x78
    float field_0x7c;                   // +0x7c
    float field_0x80;                   // +0x80
    float field_0x84;                   // +0x84
    float field_0x88;                   // +0x88
    float field_0x8c;                   // +0x8c
    float field_0x90;                   // +0x90
    float field_0x94;                   // +0x94
    float field_0x98;                   // +0x98
    float field_0x9c;                   // +0x9c

    ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level);
    ~ObjectGun();
    void setScaling(int x, int y);
    void replaceGun(int mesh);
    void setEnemies(Array<Player*> *enemies);
    void update(int dt);
    void render();
};
#endif
