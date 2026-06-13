#ifndef GOF2_MININGGAME_H
#define GOF2_MININGGAME_H
#include "gof2/common.h"
// MiningGame — the asteroid-core mining minigame.
// Real named struct recovered from per-method byte-offset accesses.
struct Hud;
struct Sprite;
struct MarqueeImage;

class MiningGame {
public:
    float inputX;     // +0x00  velocity x (input)
    float inputY;     // +0x04  velocity y (input)
    float driftX;     // +0x08  drift x
    float driftY;     // +0x0c  drift y
    float posX;    // +0x10  position x
    float posY;    // +0x14  position y
    int layer;      // +0x18  layer
    int station;      // +0x1c  station
    int lossTimer;      // +0x20  loss timer
    float oreAmount;    // +0x24  ore amount
    float oreRate;    // +0x28  ore rate
    float controlDivisor;    // +0x2c  control divisor
    int field_0x30;      // +0x30
    int oreImageHeight;      // +0x34  ore image height
    int field_0x38;      // +0x38
    int field_0x3c;      // +0x3c
    int oreIconOffsetX;      // +0x40
    int oreIconOffsetY;      // +0x44
    int progressBarWidth;      // +0x48  progress bar width
    int progressBarHeight;      // +0x4c  progress bar height
    int progressBarX;      // +0x50  progress bar x
    int progressBarY;      // +0x54  progress bar y
    int centerX;      // +0x58  center x
    int centerY;      // +0x5c  center y
    int coreImageId;      // +0x60  core image id
    float textAlpha;    // +0x64  text alpha
    float animAccumulator;    // +0x68  sprite anim accumulator
    int driftTimer;      // +0x6c  drift timer
    int field_0x70;      // +0x70
    int layerTimer;      // +0x74  layer timer
    int currentLayer;      // +0x78  current layer
    int targetLayer;      // +0x7c  target layer
    uint8_t isCoreLayer;  // +0x80  is core layer
    uint8_t gameWonFlag;  // +0x81  game won
    uint8_t gameLostFlag;  // +0x82  game lost
    uint8_t gotCoreFlag;  // +0x83  got core
    uint8_t campaignFlag;  // +0x84  campaign flag
    uint8_t _pad_85[3];  // padding to 0x88
    void* oreMarquee;    // +0x88  ore MarqueeImage
    void* leftMarquee;    // +0x8c  left MarqueeImage
    void* rightMarquee;    // +0x90  right MarqueeImage
    void* drillSprite;    // +0x94  drill Sprite
    int oreIconImageId;      // +0x98  ore icon image (res 0x4e7)
    int oreLabelImageId;     // +0x9c  ore label glow image (res 0x4e5)
    int oreTextImageId;      // +0xa0  ore-amount text strip image (res 0x4e4)
    int cornerImageId;       // +0xa4  progress-bar corner decoration (res 0x4e3)
    int progressBarImageId;  // +0xa8  progress bar image (res 0x4e8)
    int ringEvenNear;        // +0xac  even-layer ring tile, near band (res 0x4e2)
    int ringEvenFar;         // +0xb0  even-layer ring tile, far band (res 0x4dd)
    int ringEvenMid;         // +0xb4  even-layer ring tile, mid band (res 0x4de)
    int ringOddNear;         // +0xb8  odd-layer ring tile, near band (res 0x4e1)
    int ringOddMid;          // +0xbc  odd-layer ring tile, mid band (res 0x4df)
    int ringOddFar;          // +0xc0  odd-layer ring tile, far band (res 0x4e0)
    int progressLabelImageId;// +0xc4  progress-bar label image (res 0x4ed)
    int marqueeWidth;      // +0xc8  marquee width
    int promptPulseTimer;      // +0xcc  prompt pulse timer
    void* hud;    // +0xd0  Hud*

    MiningGame(int layer, int station, Hud *hud);
    ~MiningGame();
    // Heap factory used by PlayerEgo::approachAsteroid — allocates the 0xd4-byte
    // instance and runs the constructor with the asteroid quality/seed.
    static MiningGame *create(int quality, int seed, Hud *hud);
    void up(float amount);
    void down(float amount);
    void left(float amount);
    void right(float amount);
    // Interworking veneers exported to PlayerEgo's input dispatch. Each is a thin
    // forwarder that hands the steering delta to the matching axis handler above:
    //   steerXR -> right, steerX -> left, steerY -> down, steerYAlt -> up.
    // They mirror the ARM-Thumb PLT thunks at 0x1abaf8..0x1abb38 in the original.
    float steerXR(float delta);
    float steerX(float delta);
    float steerY(float delta);
    float steerYAlt(float delta);
    int getOreAmount();
    uint8_t gameWon();
    uint8_t gameLost();
    uint8_t gotCore();
    bool isInCurrentLayer();
    int update(int delta);
    void render2D();
};
#endif
