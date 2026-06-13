#ifndef GOF2_EXPLOSION_H
#define GOF2_EXPLOSION_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- Explosion class (Android libgof2hdaa.so, armv7 Thumb).
// Real named struct recovered from per-method byte-offset access.

struct AEGeometry;
struct PaintCanvas;
struct TargetFollowCamera;

// Explosion: a transient VFX object that drives one or two AEGeometry meshes
// (plus an optional vector of fire-streak meshes) through their transform
// animations. 0x68 bytes in the target; here laid out as a natural struct.
class Explosion {
public:
    int                    type;   // +0x00 explosion type
    AEGeometry            *primaryMesh;    // +0x04 primary mesh
    AEGeometry            *secondaryMesh;    // +0x08 secondary mesh (optional)
    Array<AEGeometry *>   *fireStreaks;    // +0x0c fire-streak meshes (optional)
    long long              duration;   // +0x10 total duration
    long long              elapsed;   // +0x18 elapsed time
    byte                   playing;   // +0x20 playing flag
    float                  scale;   // +0x24 scale
    int                    weaponIndex;   // +0x28 sound id (-1 == none)
    Matrix                 rotation;   // +0x2c rotation matrix

    // ---- special members ----
    Explosion(int type);
    ~Explosion();

    // ---- methods (converted from free functions) ----
    void addFireStreaks();
    uint8_t isPlaying();
    bool peakReached();
    void playSound(Vector *pos);
    void render();
    void reset();
    void setMatrix(const Matrix *matrix);
    void setScaling(float scale);
    void setWeaponIndex(int index);
    void start(const Vector *position, const Vector *direction);
    void start_matrix(const Matrix *matrix);
    void translate(const Vector *v);
    void update(int dt, TargetFollowCamera *camera);
    void update_camera(int dt, TargetFollowCamera *camera);
    void update_vector(int dt, const Vector *position);

    // Tail helpers extracted from translate()/update_vector(): translating the
    // optional secondary mesh, and the terminal reset once the animation finishes.
    void tail_translate(AEGeometry *geometry, const Vector *v);
    void reset_tail();
};

// Byte-offset accessors for EXTERNAL opaque objects (AEGeometry, Transform,
// PaintCanvas) whose layouts are not part of this class.
static inline int      &I(void *p, int off) { return *(int *)((char *)p + off); }

#endif
