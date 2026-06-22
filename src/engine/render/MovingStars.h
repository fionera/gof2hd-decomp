#ifndef GOF2_MOVINGSTARS_H
#define GOF2_MOVINGSTARS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

class MovingStars {
public:
    uint32_t *billboardIds; // billboard ids array (50 entries)
    uint32_t *transformHandles; // transform handles array (50 entries)
    uint32_t textureHandle; // texture handle
    int *lifeArray; // per-star lifetime/timer array (50 entries)
    int *velocityArray; // per-star velocity array (50 entries)
    uint8_t animResetFlag; // anim flag: low byte of the 0x101 anim word
    uint8_t animActiveFlag; // anim flag: high byte of the 0x101 anim word
    char pad[2];
    uint32_t tickAccumulator; // tick accumulator

    MovingStars();

    ~MovingStars();

    void update(int param1, AbyssEngine::AEMath::Matrix m, bool flag, float param19);

    void translate(const AbyssEngine::AEMath::Vector &v);

    void render();
};
#endif
