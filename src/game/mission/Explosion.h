#ifndef GOF2_EXPLOSION_H
#define GOF2_EXPLOSION_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class AEGeometry;
class TargetFollowCamera;

namespace AbyssEngine { namespace AEMath { struct Vector; struct Matrix; } }

// Explosion: a transient VFX object that drives one or two AEGeometry meshes
// (plus an optional vector of fire-streak meshes) through their transform
// animations.
class Explosion {
public:
    int                  type;          // explosion type
    AEGeometry          *primaryMesh;   // primary mesh
    AEGeometry          *secondaryMesh; // secondary mesh (optional)
    Array<AEGeometry *> *fireStreaks;   // fire-streak meshes (optional)
    long long            duration;      // total duration
    long long            elapsed;       // elapsed time
    uint8_t              playing;       // playing flag
    float                scale;         // scale
    int                  weaponIndex;   // sound id (-1 == none)
    Matrix               rotation;      // rotation matrix

    explicit Explosion(int type);
    ~Explosion();

    void    addFireStreaks();
    uint8_t isPlaying();
    bool    peakReached();
    void    playSound(Vector *pos);
    void    render();
    void    reset();
    void    setMatrix(const Matrix *matrix);
    void    setScaling(float scale);
    void    setWeaponIndex(int index);
    void    start(const Vector *position, const Vector *direction);
    void    start_matrix(const Matrix *matrix);
    void    translate(const Vector *v);
    void    update(int dt, TargetFollowCamera *camera);
    void    update_camera(int dt, TargetFollowCamera *camera);
    void    update_vector(int dt, const Vector *position);

    // Tail helpers extracted from translate()/update_vector(): translating the
    // optional secondary mesh, and the terminal reset once the animation finishes.
    void    tail_translate(AEGeometry *geometry, const Vector *v);
    void    reset_tail();
};

#endif
