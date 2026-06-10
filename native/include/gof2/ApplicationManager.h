#ifndef GOF2_APPLICATIONMANAGER_H
#define GOF2_APPLICATIONMANAGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

namespace AbyssEngine {
struct ApplicationManager;
struct ConfigReader;
struct PaintCanvas;


}

using ApplicationManager = AbyssEngine::ApplicationManager;
using ConfigReader = AbyssEngine::ConfigReader;
using PaintCanvas = AbyssEngine::PaintCanvas;
using String = AbyssEngine::String;

struct ApplicationManager {
    uint32_t field_0x8;                 // +0x8
    uint32_t field_0xc;                 // +0xc
    char* field_0x10;                   // +0x10
    bool field_0x14;                    // +0x14
    void* field_0x18;                   // +0x18
    QuitCallback* field_0x1c;           // +0x1c
    LoadingCallback* field_0x20;        // +0x20
    void* field_0x24;                   // +0x24
    ResumeCallback* field_0x28;         // +0x28
    void* field_0x2c;                   // +0x2c
    void* field_0x30;                   // +0x30
    bool field_0x34;                    // +0x34
    void* field_0x38;                   // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    char* field_0x48;                   // +0x48
    unsigned int field_0x50;            // +0x50
    char* field_0x54;                   // +0x54
    unsigned int field_0x5c;            // +0x5c
    void* field_0x60;                   // +0x60
    uint64_t field_0x68;                // +0x68
    uint64_t field_0x70;                // +0x70
    uint64_t field_0x78;                // +0x78
    uint64_t field_0x80;                // +0x80
    uint32_t field_0x84;                // +0x84
    unsigned int field_0x88;            // +0x88
    char* field_0x8c;                   // +0x8c
    uint32_t field_0x98;                // +0x98
    uint32_t field_0x9c;                // +0x9c
    uint64_t field_0xa0;                // +0xa0
    uint32_t field_0xa4;                // +0xa4
    void* field_0xa8;                   // +0xa8
    void* field_0xac;                   // +0xac
    bool field_0xb0;                    // +0xb0
    bool field_0xb1;                    // +0xb1
    bool field_0xb2;                    // +0xb2
    int field_0xb4;                     // +0xb4
    int field_0xb8;                     // +0xb8
};
#endif
