#include "class.h"

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

// FModSound::updateEvent3DAttributes(int, Vector*, Vector*, bool)
//   events[p1] = updateEvent3DAttributes(events[p1], p1, p2, p3, p4)
struct FModSound {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    uint8_t f_8; // 0x8
    unsigned char _pad_9[3];
    void* f_c; // 0xc
    uint8_t f_10; // 0x10
    unsigned char _pad_11[9199];
    int f_2400; // 0x2400
    int f_2404; // 0x2404
    int f_2408; // 0x2408
    int f_240c; // 0x240c
    unsigned char _pad_2410[20];
    void* f_2424; // 0x2424
    void* f_2428; // 0x2428
    void* f_242c; // 0x242c
    void* f_2430; // 0x2430
    void* f_2434; // 0x2434
    void* f_2438; // 0x2438

    void *updateEvent3DAttributes(void *event, int idx, Vector *a, Vector *b, bool c);
    void updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c);
};
void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c)
{
    void **slot = (void **)((char *)this + idx * 4 + OFF_EVENTS);
    *slot = updateEvent3DAttributes(*slot, idx, a, b, c);
}
