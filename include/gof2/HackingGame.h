#ifndef GOF2_HACKINGGAME_H
#define GOF2_HACKINGGAME_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- HackingGame (Android libgof2hdaa.so, armv7 Thumb).
// The station "hacking" tile-rotation mini-game. State is stored as a block of ints (current
// arrangement at 0x1c.., target arrangement at 0x4.., working/scratch at 0x34.., timers at
// 0x12c/0x130, reward item/amount/docking index at 0x134/0x138/0x13c, cached image handles at
// 0x10c..0x120, per-cell image slots from 0x64). The wide field range is reached through the
// I()/B() accessor helpers (raw int/byte offset reads into the game state block).

struct HackingGame;

static inline int32_t &I(HackingGame *self, uint32_t off)
{
    return *(int32_t *)((char *)self + off);
}

static inline uint8_t &B(HackingGame *self, uint32_t off)
{
    return *(uint8_t *)((char *)self + off);
}

struct HackingGame {
    HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex);

    int  getDockingIndex();
    int  getRewardAmount();
    int  getRewardItem();
    bool isRotating();
    int  gameWon();
    int  gameWon(int *state);
    void rotateLeftCW(bool sound);
    void rotateLeftCW(int *state);
    void rotateRightCW(bool sound);
    void rotateRightCW(int *state);
    void render2D();
    void reInit();
    int  solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state);
    int  update(int dt);

    // game-state block (offset-addressed through the I()/B() helpers)
    char  field_storage[0x128];
    // 0x128/0x129 are the "rotating left"/"rotating right" flags; the code clears both at once
    // through the 16-bit slot named f_128.
    uint16_t f_128;                     // +0x128 (left flag = low byte, right flag = high byte)
    char  field_storage2[0x320 - 0x12a];
};
#endif
