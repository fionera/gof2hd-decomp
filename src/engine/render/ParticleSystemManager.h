#ifndef GOF2_PARTICLESYSTEMMANAGER_H
#define GOF2_PARTICLESYSTEMMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ParticleSettings.h"

// AbyssEngine::BlendMode -- defined with the SAME canonical guarded form the rest of the
// engine uses (PaintCanvas.h / ResourceMaterial.h). Guarded so a TU that already pulls one
// of those definers does not get a redefinition.
namespace AbyssEngine {
#ifndef GOF2_ENUM_BlendMode
#define GOF2_ENUM_BlendMode
enum BlendMode { BlendMode_dummy = 0, BlendMode_1 = 1, BlendMode_2 = 2, BlendMode_8 = 8, BlendMode_0x15 = 0x15 };
#endif
}

// PaintCanvas is used only by pointer here, so forward-declare it (do NOT include the full
// PaintCanvas.h, whose stub-redefining co-includes would break this header).
namespace AbyssEngine { class PaintCanvas; }
using ::AbyssEngine::PaintCanvas;

// Galaxy on Fire 2 -- ParticleSystemManager.
//
// Owns the per-frame particle sub-systems for a scene. Particles come in two flavours, each kept in
// its own dynamic array: 2D sprite systems (rendered through the canvas sprite pipeline) and 3D mesh
// systems (rendered through a particle mesh + transform). A system handle packs the array selector in
// bit 17: when set the handle indexes the mesh array, otherwise the sprite array; -1 means "no system".

class ParticleSystemManager {
public:
    uint16_t flags;                  // low byte = sprite-active, high byte = mesh-active
    void*    canvas;                 // PaintCanvas*
    int32_t  cameraSet;              // ParticleSettings::CameraSet
    int32_t  accumulatedDt;          // accumulated update delta
    uint8_t  enabled;                // manager active/enabled flag

    uint32_t spriteSystemCount;      // sprite array: element count
    void*    spriteSystems;          // sprite array: data pointer (IParticleSystem*[])
    uint32_t spriteSystemCapacity;   // sprite array: capacity
    int16_t  spriteTextureId;        // sprite texture id (0xffff = create from uv id)
    int16_t  spriteUvId;             // sprite uv/texture-source id
    uint32_t spriteBlendMode;        // sprite blend mode
    uint32_t spriteMeshId;           // sprite render handle (init 0xffffffff)
    uint32_t spriteSystemId;         // created sprite-system handle (init 0xffffffff)
    uint32_t spriteParticleCount;    // accumulated sprite particle count
    uint8_t  spriteUsesExtra;        // sprite extra/mirror flag

    uint32_t meshSystemCount;        // mesh array: element count
    void*    meshSystems;            // mesh array: data pointer (IParticleSystem*[])
    uint32_t meshSystemCapacity;     // mesh array: capacity
    int16_t  meshTextureId;          // mesh texture id (0xffff = create from uv id)
    int16_t  meshUvId;               // mesh uv/texture-source id
    uint32_t meshBlendMode;          // mesh blend mode
    uint32_t meshExtraId;            // mesh render handle
    uint32_t meshId;                 // created particle-mesh handle (init 0xffffffff)
    uint32_t transformId;            // created transform handle (init 0xffffffff)
    uint32_t meshParticleCount;      // packed mesh vertex/index particle count
    uint8_t  meshUsesExtra;          // mesh extra/mirror flag

    ParticleSystemManager(PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet,
                          unsigned short spriteTex, bool spriteFlag,
                          unsigned short meshTex, bool meshFlag);
    ParticleSystemManager(PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet,
                          unsigned short spriteTex, AbyssEngine::BlendMode spriteBlend,
                          bool spriteFlag, unsigned short meshTex,
                          AbyssEngine::BlendMode meshBlend, bool meshFlag);

    void update(long long dt);
    void reset();
    void releaseSprites();
    void construct();
    void render3d();
    void setParticleSetByIndex(int handle, unsigned char setIndex);
    void enableSystemRender(int handle, bool enable);
    void release();
    void cameraToggle(ParticleSettings::CameraSet cam);
    unsigned int addMeshSystem(AbyssEngine::AEMath::Matrix const *matrix,
                               Array<ParticleSettings::ParticleSet> const &sets, bool flag);
    unsigned long long emitManual(int handle, AbyssEngine::AEMath::Vector const &pos, int ret, float p4);
    unsigned long long emitManual(int handle, AbyssEngine::AEMath::Vector const &pos, int ret,
                                  AbyssEngine::AEMath::Vector const &velocity, float p5);
    void renderSprites();
    void systemSetMatrix(int handle, AbyssEngine::AEMath::Matrix const *matrix);
    void setParticleSetBySet(int handle, ParticleSettings::ParticleSet set);
    void enableSystemUpdate(int handle, bool enable);
    void initSprites();
    int  addSpriteSystem(AbyssEngine::AEMath::Matrix const *matrix,
                         Array<ParticleSettings::ParticleSet> const &sets, bool flag);
    void initMesh();
    void enableSystemEmit(int handle, bool enable);
    void enableSystemEmit2(int handle, bool enable);
    void enableSystemEmit3(int handle, bool enable);
    void attachSystem(int handle, bool enable);
    int  addSystem(AbyssEngine::AEMath::Matrix const *matrix, ParticleSettings::ParticleSet set, bool flag);
    int  init();
    void resetSystem(int handle);
    void renderMeshes();
};
#endif
