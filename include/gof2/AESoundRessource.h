#ifndef GOF2_AESOUNDRESSOURCE_H
#define GOF2_AESOUNDRESSOURCE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct AESoundInfo {
    int id;
    int field_4;
    int field_8;
};
struct AESoundInterface;
}



struct AESoundStackFrame {
    char info[12];
    int index;
    volatile int cookie;
};

struct AESoundRessource {
    volatile int field_0x0;             // +0x0
    uint32_t field_0x4;                 // +0x4
    char* field_0x8;                    // +0x8
    volatile int field_0x10;            // +0x10
};
#endif
