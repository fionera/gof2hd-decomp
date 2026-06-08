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

    void reset(bool shrinking);
};

void PlayerWormHole::reset(bool shrinking)
{
    int value = 0;
    if (shrinking)
        value = 59000;
    this->f_150 = value;
    this->f_154 = 0x1000;
}
