#ifndef WORK_CLASSES_HACKINGGAME_SRC_CLASS_H
#define WORK_CLASSES_HACKINGGAME_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct HackingGame {
    // @portable-fields
    unsigned char _pad_0[296];
    int f_128; // 0x128

    HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex);

    int getRewardItem();
    int getRewardAmount();
    int getDockingIndex();
    bool isRotating();

    int gameWon(int *state);
    int gameWon();

    void rotateRightCW(bool sound);
    void rotateLeftCW(bool sound);
    void rotateLeftCW(int *state);
    void rotateRightCW(int *state);

    int update(int dt);
    int solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state);
    void reInit();
    void render2D();
};

template <typename T>
static inline T &F(HackingGame *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

static inline int32_t &I(HackingGame *self, uint32_t off)
{
    return F<int32_t>(self, off);
}

static inline uint8_t &B(HackingGame *self, uint32_t off)
{
    return F<uint8_t>(self, off);
}

#endif
