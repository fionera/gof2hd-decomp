#ifndef GOF2_MININGGAME_H
#define GOF2_MININGGAME_H
#include "gof2/common.h"
// MiningGame — the asteroid-core mining minigame.
// Real named struct recovered from per-method byte-offset accesses.
struct Hud;
struct Sprite;
struct MarqueeImage;

struct MiningGame {
    float field_0x0;     // +0x00  velocity x (input)
    float field_0x4;     // +0x04  velocity y (input)
    float field_0x8;     // +0x08  drift x
    float field_0xc;     // +0x0c  drift y
    float field_0x10;    // +0x10  position x
    float field_0x14;    // +0x14  position y
    int field_0x18;      // +0x18  layer
    int field_0x1c;      // +0x1c  station
    int field_0x20;      // +0x20  loss timer
    float field_0x24;    // +0x24  ore amount
    float field_0x28;    // +0x28  ore rate
    float field_0x2c;    // +0x2c  control divisor
    int field_0x30;      // +0x30
    int field_0x34;      // +0x34  ore image height
    int field_0x38;      // +0x38
    int field_0x3c;      // +0x3c
    int field_0x40;      // +0x40
    int field_0x44;      // +0x44
    int field_0x48;      // +0x48  progress bar width
    int field_0x4c;      // +0x4c  progress bar height
    int field_0x50;      // +0x50  progress bar x
    int field_0x54;      // +0x54  progress bar y
    int field_0x58;      // +0x58  center x
    int field_0x5c;      // +0x5c  center y
    int field_0x60;      // +0x60  core image id
    float field_0x64;    // +0x64  text alpha
    float field_0x68;    // +0x68  sprite anim accumulator
    int field_0x6c;      // +0x6c  drift timer
    int field_0x70;      // +0x70
    int field_0x74;      // +0x74  layer timer
    int field_0x78;      // +0x78  current layer
    int field_0x7c;      // +0x7c  target layer
    uint8_t field_0x80;  // +0x80  is core layer
    uint8_t field_0x81;  // +0x81  game won
    uint8_t field_0x82;  // +0x82  game lost
    uint8_t field_0x83;  // +0x83  got core
    uint8_t field_0x84;  // +0x84  campaign flag
    uint8_t _pad_85[3];  // padding to 0x88
    void* field_0x88;    // +0x88  ore MarqueeImage
    void* field_0x8c;    // +0x8c  left MarqueeImage
    void* field_0x90;    // +0x90  right MarqueeImage
    void* field_0x94;    // +0x94  drill Sprite
    int field_0x98;      // +0x98  image ids ...
    int field_0x9c;      // +0x9c
    int field_0xa0;      // +0xa0
    int field_0xa4;      // +0xa4
    int field_0xa8;      // +0xa8
    int field_0xac;      // +0xac
    int field_0xb0;      // +0xb0
    int field_0xb4;      // +0xb4
    int field_0xb8;      // +0xb8
    int field_0xbc;      // +0xbc
    int field_0xc0;      // +0xc0
    int field_0xc4;      // +0xc4
    int field_0xc8;      // +0xc8  marquee width
    int field_0xcc;      // +0xcc  prompt pulse timer
    void* field_0xd0;    // +0xd0  Hud*

    MiningGame(int layer, int station, Hud *hud);
    ~MiningGame();
    void up(float amount);
    void down(float amount);
    void left(float amount);
    void right(float amount);
    int getOreAmount();
    uint8_t gameWon();
    uint8_t gameLost();
    uint8_t gotCore();
    bool isInCurrentLayer();
    int update(int delta);
    void render2D();
};
#endif
