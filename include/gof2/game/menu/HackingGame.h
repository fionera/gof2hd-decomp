#ifndef GOF2_HACKINGGAME_H
#define GOF2_HACKINGGAME_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- HackingGame (Android libgof2hdaa.so, armv7 Thumb).
// The station "hacking" tile-rotation mini-game. State is a plain block of ints/bytes:
// the target/current/working tile arrangements (6 ints each), a per-variant tile-image
// table, the six cached UI image handles, the rotation/win timers, and the reward/docking
// metadata. All members were recovered from the per-method disassembly; trailing comments
// give the original 32-bit field offsets, the live layout is natural 64-bit.

class HackingGame {
public:
    HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex);
    // State is a plain block of ints/bytes; destruction is trivial.
    ~HackingGame();

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

    // ---- named layout (recovered from the binary) ----
    int      difficulty;        // +0x00  puzzle difficulty/type (drives reInit branching)
    int      target[6];         // +0x04  goal tile arrangement
    int      current[6];        // +0x1c  currently displayed tile arrangement
    int      working[6];        // +0x34  scratch arrangement used while rotating
    int      tileImages[48];    // +0x4c  per-variant tile-image handle table (variant*0x30 + cell*4)
    int      topImage;          // +0x10c cached title/top image handle (id 0x1f49)
    int      mainImage;         // +0x110 cached center/reference image handle (id 0x1f47)
    int      bottomImage;       // +0x114 cached bottom image handle (id 0x1f48)
    int      arrowActive;       // +0x118 arrow image while rotating (id 0x1f46)
    int      arrowIdle;         // +0x11c arrow image while idle (id 0x1f44)
    int      markImage;         // +0x120 target/check mark image handle (id 0x1f45)
    int      type;              // +0x124 image-set/variant index (drives tile typeOffset)
    // 0x128/0x129 are the "rotating left"/"rotating right" flags; the code clears both at once
    // through the 16-bit slot named f_128 (left flag = low byte, right flag = high byte).
    uint16_t f_128;             // +0x128
    uint16_t pad_12a;           // +0x12a
    int      rotateTimer;       // +0x12c elapsed time of the in-progress rotation (ms)
    int      wonTimer;          // +0x130 elapsed time since the puzzle was solved (ms)
    int      rewardItem;        // +0x134
    int      rewardAmount;      // +0x138
    int      dockingIndex;      // +0x13c
    char     field_storage[0x320 - 0x140]; // +0x140 trailing state block
};
#endif
