#ifndef GOF2_PLAYERCREATURE_H
#define GOF2_PLAYERCREATURE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct PlayerCreature;
struct KIPlayer;
struct Player;
struct AEGeometry;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {





void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);

}
}

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

struct PlayerCreature {
    char* field_0x0;                    // +0x0
    Player* field_0x4;                  // +0x4
    AEGeometry* field_0x8;              // +0x8
    void* field_0x54;                   // +0x54
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
    float field_0x60;                   // +0x60
    AEGeometry* field_0x78;             // +0x78
    int field_0x88;                     // +0x88
    int field_0xac;                     // +0xac
    int field_0x124;                    // +0x124
    uint16_t field_0x128;               // +0x128
    uint8_t field_0x129;                // +0x129
    uint8_t field_0x12a;                // +0x12a
    float field_0x12c;                  // +0x12c
    int field_0x130;                    // +0x130
    int field_0x134;                    // +0x134
    int field_0x138;                    // +0x138
    int field_0x13c;                    // +0x13c
    int field_0x140;                    // +0x140
};
#endif
