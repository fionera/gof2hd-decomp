#ifndef GOF2_ROCKETGUN_H
#define GOF2_ROCKETGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct RocketGun;
struct Gun;
struct Level;
struct Radar;



namespace AbyssEngine {
namespace AEMath {


}
}

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;



typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

struct RocketGun {
    int field_0x4;                      // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    uint32_t field_0x10;                // +0x10
    void* field_0x18;                   // +0x18
    uint8_t field_0x1c;                 // +0x1c
    uint8_t field_0x1d;                 // +0x1d
    void* field_0xb0;                   // +0xb0
    int field_0xb4;                     // +0xb4
    int field_0xb8;                     // +0xb8
    int field_0xbc;                     // +0xbc
    uint8_t field_0xc0;                 // +0xc0
    int field_0xc4;                     // +0xc4
    uint32_t field_0xc8;                // +0xc8
    int field_0xcc;                     // +0xcc
    int field_0xd0;                     // +0xd0
    int field_0xd4;                     // +0xd4
    void* field_0xd8;                   // +0xd8
    void* field_0xdc;                   // +0xdc
    void* field_0xe0;                   // +0xe0
    int field_0xe4;                     // +0xe4
};
#endif
