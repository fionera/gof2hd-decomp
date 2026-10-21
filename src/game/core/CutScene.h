#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/math/Vector.h"

class AEGeometry;
class Level;
class PlayerEgo;
class TargetFollowCamera;

class CutScene {
public:
    Level *level;
    float cameraRotX;
    Vector vec8;
    float vec8w;
    int shipPosY;
    int shipPosZ;
    AEGeometry *rootGeom;
    float rotationSpeed;
    AEGeometry *geom28;
    AEGeometry *geom2c;
    AEGeometry *geom30;
    AEGeometry *geom34;
    Array<AEGeometry *> *geometries;
    int64_t currentTime;   // 0x40
    int64_t lastTime;      // 0x48
    int64_t totalTime;     // 0x50
    int frameDelta;        // 0x58
    uint8_t initialized;
    PlayerEgo *player;
    AEGeometry *turretGeom;
    TargetFollowCamera *followCamera;
    uint32_t cameraId6c;
    uint32_t cameraId70;
    uint32_t cameraId74;
    uint32_t transformId78;
    int animTimer7c;
    int animTimer80;
    int fogTimer84;
    int mode;
    // Both original allocation sites (SpaceLounge ctor, ModMainMenu) do operator new(0xa0): the
    // object is 0xa0 bytes; the members past `mode` (0x8c..0x9f) are not yet identified.
    uint8_t _unknown_0x8c[0x14];

    CutScene(int mode);

    ~CutScene();

    uint8_t isInitialized();

    void update();

    void update(int delta);

    void render3D();

    void render2D();

    void renderBG();

    void process(int delta);

    void replacePlayerShip(int shipIndex, int race);

    void initialize();

    void resetCamera();

    void checkForTurret();
};

static_assert(sizeof(CutScene) == 0xa0, "CutScene size (operator new(0xa0) at both sites)");

#endif
