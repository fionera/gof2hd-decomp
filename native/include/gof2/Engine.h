#ifndef GOF2_ENGINE_H
#define GOF2_ENGINE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Engine {
    uint32_t field_0x0;                 // +0x0
    uint32_t field_0x4;                 // +0x4
    uint8_t field_0x8;                  // +0x8
    uint32_t field_0xc;                 // +0xc
    uint32_t field_0x10;                // +0x10
    uint8_t field_0x20;                 // +0x20
    void* field_0x24;                   // +0x24
    uint32_t field_0x28;                // +0x28
    bool field_0x2c;                    // +0x2c
    char** field_0x30;                  // +0x30
    uint32_t field_0x34;                // +0x34
    uint64_t field_0x58;                // +0x58
    uint64_t field_0x68;                // +0x68
    uint32_t field_0x70;                // +0x70
    bool field_0x74;                    // +0x74
    int field_0x78;                     // +0x78
    int field_0x80;                     // +0x80
    float field_0xd0;                   // +0xd0
    float field_0xd4;                   // +0xd4
    float field_0xd8;                   // +0xd8
    float field_0xdc;                   // +0xdc
    int field_0xe0;                     // +0xe0
    unsigned char field_0xfc;           // +0xfc
    uint16_t field_0xfd;                // +0xfd
    uint32_t field_0x100;               // +0x100
    uint32_t field_0x1c4;               // +0x1c4
    uint32_t field_0x1c8;               // +0x1c8
    uint32_t field_0x1cc;               // +0x1cc
    uint32_t field_0x1d0;               // +0x1d0
    uint32_t field_0x1d4;               // +0x1d4
    uint32_t field_0x1d8;               // +0x1d8
    uint32_t field_0x1dc;               // +0x1dc
    uint32_t field_0x1e0;               // +0x1e0
    uint32_t field_0x1e4;               // +0x1e4
    uint32_t field_0x1e8;               // +0x1e8
    uint32_t field_0x1ec;               // +0x1ec
    uint32_t field_0x1f0;               // +0x1f0
    uint32_t field_0x1f4;               // +0x1f4
    uint32_t field_0x1f8;               // +0x1f8
    uint32_t field_0x1fc;               // +0x1fc
    uint32_t field_0x200;               // +0x200
    uint32_t field_0x204;               // +0x204
    uint32_t field_0x208;               // +0x208
    uint32_t field_0x20c;               // +0x20c
    uint32_t field_0x210;               // +0x210
    uint32_t field_0x214;               // +0x214
    uint32_t field_0x218;               // +0x218
    uint32_t field_0x21c;               // +0x21c
    uint32_t field_0x220;               // +0x220
    uint32_t field_0x224;               // +0x224
    uint64_t field_0x268;               // +0x268
    uint32_t field_0x270;               // +0x270
    uint32_t field_0x274;               // +0x274
    float field_0x288;                  // +0x288
    float field_0x28c;                  // +0x28c
    float field_0x290;                  // +0x290
    uint32_t field_0x294;               // +0x294
    float field_0x298;                  // +0x298
    float field_0x29c;                  // +0x29c
    float field_0x2a0;                  // +0x2a0
    uint32_t field_0x2a4;               // +0x2a4
    float field_0x2a8;                  // +0x2a8
    float field_0x2ac;                  // +0x2ac
    float field_0x2b0;                  // +0x2b0
    uint32_t field_0x2b4;               // +0x2b4
    float field_0x2b8;                  // +0x2b8
    float field_0x2bc;                  // +0x2bc
    float field_0x2c0;                  // +0x2c0
    uint32_t field_0x2c4;               // +0x2c4
    float field_0x2c8;                  // +0x2c8
    float field_0x320;                  // +0x320
    float field_0x324;                  // +0x324
    float field_0x328;                  // +0x328
    int field_0x32c;                    // +0x32c
    Vector field_0x330;                 // +0x330
    Vector field_0x33c;                 // +0x33c
    uint64_t field_0x340;               // +0x340
    int field_0x348;                    // +0x348
    uint64_t field_0x34c;               // +0x34c
    float field_0x350;                  // +0x350
    uint32_t field_0x354;               // +0x354
    void* field_0x358;                  // +0x358
    int field_0x35c;                    // +0x35c
    uint32_t field_0x360;               // +0x360
    int field_0x368;                    // +0x368
    int field_0x36c;                    // +0x36c
    int field_0x370;                    // +0x370
    int field_0x374;                    // +0x374
    uint32_t field_0x378;               // +0x378
    uint32_t field_0x37c;               // +0x37c
    char* field_0x380;                  // +0x380
    float field_0x384;                  // +0x384
    uint32_t field_0x388;               // +0x388
    uint32_t field_0x38c;               // +0x38c
    uint32_t field_0x390;               // +0x390
    u32x4 field_0x394;                  // +0x394
    float field_0x398;                  // +0x398
    uint32_t field_0x39c;               // +0x39c
    uint32_t field_0x3a0;               // +0x3a0
    u32x4 field_0x3a4;                  // +0x3a4
    uint32_t field_0x3a8;               // +0x3a8
    unsigned int field_0x3ac;           // +0x3ac
    uint32_t field_0x3b0;               // +0x3b0
    u32x4 field_0x3b4;                  // +0x3b4
    unsigned int field_0x3b8;           // +0x3b8
    uint32_t field_0x3bc;               // +0x3bc
    unsigned int field_0x3c0;           // +0x3c0
    uint8_t field_0x3c4;                // +0x3c4
    uint32_t field_0x3c8;               // +0x3c8
    uint64_t field_0x3cc;               // +0x3cc
    uint32_t field_0x3d4;               // +0x3d4
    uint32_t field_0x3d8;               // +0x3d8
    char* field_0x3dc;                  // +0x3dc
    int field_0x3e4;                    // +0x3e4
    uint32_t field_0x3e8;               // +0x3e8
    uint32_t field_0x3ec;               // +0x3ec
    Vector field_0x3f0;                 // +0x3f0
    uint64_t field_0x400;               // +0x400
    uint32_t field_0x40c;               // +0x40c
    uint32_t field_0x410;               // +0x410
    FBOContainer* field_0x414;          // +0x414
    FBOContainer* field_0x418;          // +0x418
    uint8_t field_0x41c;                // +0x41c
    uint32_t field_0x420;               // +0x420
    uint8_t field_0x424;                // +0x424
    Vector field_0x468;                 // +0x468
    Vector field_0x474;                 // +0x474
    uint64_t field_0x478;               // +0x478
    bool field_0x480;                   // +0x480
    DestroyCallback* field_0x484;       // +0x484
    uint32_t field_0x488;               // +0x488
    int field_0x48c;                    // +0x48c
    int field_0x490;                    // +0x490
    uint32_t field_0x4a4;               // +0x4a4
    uint32_t field_0x4a8;               // +0x4a8
    double field_0x4b0;                 // +0x4b0
    double field_0x4b8;                 // +0x4b8
    volatile double field_0x4c0;        // +0x4c0
    volatile double field_0x4c8;        // +0x4c8
    volatile double field_0x4d0;        // +0x4d0
    double field_0x4d8;                 // +0x4d8
    double field_0x4e0;                 // +0x4e0
    double field_0x4e8;                 // +0x4e8
    volatile double field_0x4f0;        // +0x4f0
    volatile double field_0x4f8;        // +0x4f8
    volatile double field_0x500;        // +0x500
    double field_0x508;                 // +0x508
    uint32_t field_0x510;               // +0x510
    char* field_0x514;                  // +0x514
};
#endif
