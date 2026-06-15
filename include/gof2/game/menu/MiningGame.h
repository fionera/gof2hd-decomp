#ifndef GOF2_MININGGAME_H
#define GOF2_MININGGAME_H
#include <cstdint>

#include "gof2/engine/render/MarqueeImage.h"
#include "gof2/engine/render/Sprite.h"

class Hud;

/// The asteroid-core mining minigame: the player nudges an ore reticle through
/// concentric layer rings toward the asteroid core while drift pushes it away.
class MiningGame {
public:
    float inputX;              ///< velocity x (steering input)
    float inputY;              ///< velocity y (steering input)
    float driftX;              ///< drift x
    float driftY;              ///< drift y
    float posX;               ///< reticle position x
    float posY;               ///< reticle position y
    int layer;
    int station;
    int lossTimer;
    float oreAmount;
    float oreRate;
    float controlDivisor;
    int field_0x30;
    int oreImageHeight;
    int field_0x38;
    int field_0x3c;
    int oreIconOffsetX;
    int oreIconOffsetY;
    int progressBarWidth;
    int progressBarHeight;
    int progressBarX;
    int progressBarY;
    int centerX;
    int centerY;
    int coreImageId;
    float textAlpha;
    float animAccumulator;     ///< sprite animation accumulator
    int driftTimer;
    int field_0x70;
    int layerTimer;
    int currentLayer;
    int targetLayer;
    uint8_t isCoreLayer;
    uint8_t gameWonFlag;
    uint8_t gameLostFlag;
    uint8_t gotCoreFlag;
    uint8_t campaignFlag;
    uint8_t _pad_85[3];
    MarqueeImage* oreMarquee;
    MarqueeImage* leftMarquee;
    MarqueeImage* rightMarquee;
    Sprite* drillSprite;
    int oreIconImageId;        ///< ore icon image (res 0x4e7)
    int oreLabelImageId;       ///< ore label glow image (res 0x4e5)
    int oreTextImageId;        ///< ore-amount text strip image (res 0x4e4)
    int cornerImageId;         ///< progress-bar corner decoration (res 0x4e3)
    int progressBarImageId;    ///< progress bar image (res 0x4e8)
    int ringEvenNear;          ///< even-layer ring tile, near band (res 0x4e2)
    int ringEvenFar;           ///< even-layer ring tile, far band (res 0x4dd)
    int ringEvenMid;           ///< even-layer ring tile, mid band (res 0x4de)
    int ringOddNear;           ///< odd-layer ring tile, near band (res 0x4e1)
    int ringOddMid;            ///< odd-layer ring tile, mid band (res 0x4df)
    int ringOddFar;            ///< odd-layer ring tile, far band (res 0x4e0)
    int progressLabelImageId;  ///< progress-bar label image (res 0x4ed)
    int marqueeWidth;
    int promptPulseTimer;
    Hud* hud;

    MiningGame(int layer, int station, Hud* hud);
    ~MiningGame();

    /// Heap factory used by PlayerEgo::approachAsteroid; allocates the instance
    /// and runs the constructor with the asteroid quality/seed.
    static MiningGame* create(int quality, int seed, Hud* hud);

    void up(float amount);
    void down(float amount);
    void left(float amount);
    void right(float amount);

    /// Steering veneers exported to PlayerEgo's input dispatch. Each is a thin
    /// forwarder that hands the steering delta to the matching axis handler:
    ///   steerXR -> right, steerX -> left, steerY -> down, steerYAlt -> up.
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
