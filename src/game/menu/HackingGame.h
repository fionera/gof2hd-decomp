#ifndef GOF2_HACKINGGAME_H
#define GOF2_HACKINGGAME_H
#include <cstdint>

// The station "hacking" tile-rotation mini-game. The player rotates two overlapping rings
// of tiles until the current arrangement matches the randomly generated target.
class HackingGame {
public:
    HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex);
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

    int      difficulty;        // puzzle difficulty/type (drives reInit branching)
    int      target[6];         // goal tile arrangement
    int      current[6];        // currently displayed tile arrangement
    int      working[6];        // scratch arrangement used while rotating
    int      tileImages[48];    // per-variant tile-image handle table (variant*0x30 + cell*4)
    int      topImage;          // cached title/top image handle
    int      mainImage;         // cached center/reference image handle
    int      bottomImage;       // cached bottom image handle
    int      arrowActive;       // arrow image while rotating
    int      arrowIdle;         // arrow image while idle
    int      markImage;         // target/check mark image handle
    int      type;              // image-set/variant index (drives tile typeOffset)
    bool     rotatingLeft;      // a left rotation is currently animating
    bool     rotatingRight;     // a right rotation is currently animating
    uint16_t pad_12a;
    int      rotateTimer;       // elapsed time of the in-progress rotation (ms)
    int      wonTimer;          // elapsed time since the puzzle was solved (ms)
    int      rewardItem;
    int      rewardAmount;
    int      dockingIndex;
    char     field_storage[0x320 - 0x140]; // trailing state block
};
#endif
