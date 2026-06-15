#ifndef GOF2_MOVINGSTARS_H
#define GOF2_MOVINGSTARS_H
#include "gof2/common.h"
#include "gof2/math.h"

// MovingStars — animated starfield rendered through PaintCanvas billboards/transforms.
// Top-level class (not in the AbyssEngine namespace). Four parallel 50-entry arrays hold the
// per-star billboard ids, transform handles, lifetimes and velocities.
class MovingStars {
public:
    uint32_t *billboardIds;      // billboard ids array (50 entries)
    uint32_t *transformHandles;  // transform handles array (50 entries)
    uint32_t  textureHandle;     // texture handle
    int      *lifeArray;         // per-star lifetime/timer array (50 entries)
    int      *velocityArray;     // per-star velocity array (50 entries)
    uint8_t   animResetFlag;     // anim flag: low byte of the 0x101 anim word
    uint8_t   animActiveFlag;    // anim flag: high byte of the 0x101 anim word
    char      pad[2];
    uint32_t  tickAccumulator;   // tick accumulator

    MovingStars();
    ~MovingStars();
    void update(const AbyssEngine::AEMath::Matrix &m, bool flag, float param19);
    void translate(const AbyssEngine::AEMath::Vector &v);
    void render();
};
#endif
