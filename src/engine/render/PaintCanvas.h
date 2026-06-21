#ifndef GOF2_PAINTCANVAS_H
#define GOF2_PAINTCANVAS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
// The real cross-class headers below co-include cleanly, so PaintCanvas uses the real
// ::Engine, ::Node (both global) and AbyssEngine::Mesh directly instead of self-contained
// stubs. Transform still keeps a local stub: the real engine/math/Transform.h layout
// disagrees with Ghidra (it has worldMatrix@0x0 where the binary has meshCount@0x3c /
// meshes@0x40), so Transform.h must be corrected before it can be the source of truth here.
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/core/Node.h"

// operator new/delete and memcpy/memset/strcmp come from the standard headers
// pulled in by common.h. The cross-class struct forward-decls live globally in
// fwd.h (also via common.h); we model below only the offsets PaintCanvas touches.

namespace AbyssEngine { struct Material; }
using ::AbyssEngine::Material;
namespace AbyssEngine { struct Resource; }
namespace AbyssEngine { struct Image; }
// Engine-side enums recovered from the binary's mangled method signatures. Only
// the (namespace-qualified) type names are load-bearing for the ABI; the named
// constants are conventional. Each is defined with the SAME canonical nonfixed
// form the rest of the engine uses (e.g. `enum BlendMode { BlendMode_dummy };`
// in ResourceMaterial.h / ParticleSystem*.h), guarded so a TU that already pulls
// one of those definers does not get a redefinition.
namespace AbyssEngine {
#ifndef GOF2_ENUM_FogMode
#define GOF2_ENUM_FogMode
enum FogMode { FogMode_dummy = 0, FogMode_1 = 1, FogMode_linear = 0x2601 };
#endif
#ifndef GOF2_ENUM_BlendMode
#define GOF2_ENUM_BlendMode
enum BlendMode { BlendMode_dummy = 0, BlendMode_1 = 1, BlendMode_2 = 2, BlendMode_8 = 8, BlendMode_0x15 = 0x15 };
#endif
#ifndef GOF2_ENUM_ResourceType
#define GOF2_ENUM_ResourceType
enum ResourceType { ResourceType_dummy };
#endif
#ifndef GOF2_ENUM_LandscapeMode
#define GOF2_ENUM_LandscapeMode
enum LandscapeMode { LandscapeMode_dummy = 0, LandscapeMode_1 = 1, LandscapeMode_2 = 2, LandscapeMode_3 = 3 };
#endif
}
struct ResourceTexture;
struct ResourceMesh;
namespace AbyssEngine { struct ImageFont; }
using ::AbyssEngine::ImageFont;
namespace AbyssEngine { struct Image2D; }
using ::AbyssEngine::Image2D;
namespace AbyssEngine { struct Transform; }
struct Camera;
namespace AbyssEngine { struct SpriteSystem; }
using ::AbyssEngine::SpriteSystem;

namespace AbyssEngine {

namespace AEMath {

struct BSphere {
    Vector center;
    float radius;
    float radius2;
};

Matrix MatrixIdentity(Matrix &matrix);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);
Matrix MatrixGetInverse(const Matrix &matrix);
void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
void MatrixGetGL(const Matrix &matrix, float *out);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorCross(const Vector &lhs, const Vector &rhs);
float VectorLength(const Vector &value);
float Sqrtf(float value);
float Sinf(float value);
float Cosf(float value);
float ACosf(float value);
float ATanf(float value);
float Absf(float value);

} // namespace AEMath

} // namespace AbyssEngine

// --- minimal cross-class shapes (fields PaintCanvas reads by offset) ----------
// Real layouts live in the respective class headers; only the offsets touched
// from PaintCanvas are modelled here so this TU is self-contained.
// Transform lives in namespace AbyssEngine to match the binary's mangling
// (N11AbyssEngine9TransformE), with a global `using` alias so bare/`::Transform`
// references in this TU still resolve to the same (now-namespaced) type.
namespace AbyssEngine {
struct Transform {
    char pad_0x0[0x3c];
    unsigned int field_0x3c;            // +0x3c  mesh-child count
    AbyssEngine::Mesh** field_0x40;     // +0x40  mesh-child array
    char pad_0x44[0x48 - 0x44];         // +0x44
    unsigned int field_0x48;            // +0x48  per-node color/id
    unsigned int field_0x4c;            // +0x4c  transform-child count
    Transform** field_0x50;             // +0x50  transform-child array
    char pad_0x58[0x5c - 0x58];         // +0x54..0x58 (8 bytes on 32-bit; pad to 0x5c)
    char field_0x5c[0x3c];              // +0x5c  local matrix (15 floats)
    char field_0x98[0x3c];              // +0x98  uv matrix (15 floats)
    char pad_0xec[0xec - (0x98 + 0x3c)];
    unsigned char field_0xec;           // +0xec  render-enabled flag (boundingCenter+0x18)
    char pad_0x11c[0x11c - (0xec + 1)];
    unsigned int field_0x11c;           // +0x11c keyframe count
};
}
using ::AbyssEngine::Transform;

// PaintCanvas's real type lives in namespace AbyssEngine (to match the binary's
// mangling N11AbyssEngine11PaintCanvasE); the global `using` alias after the
// namespace close lets bare `PaintCanvas` resolve to AbyssEngine::PaintCanvas.
namespace AbyssEngine {

class PaintCanvas {
public:
    // NOTE on layout: the byte labels in `field_0xNN` names are the offsets from
    // the original 32-bit ARM binary; the native (64-bit) build lays the members
    // out by natural alignment, so the labels are conventional, not physical. The
    // {count,data,cap} array headers (mesh/transform/camera/material/sprite/
    // resource/font/image lists + the glow scratch buffers + loaded-texture-name
    // list) are still the engine's three-word in-place headers shared verbatim
    // with PaintCanvasClass.h; only the genuine scalar / pointer / matrix members
    // are converted to named typed members here. The matrix + line-vertex + glow
    // members below are NEW relative to PaintCanvasClass.h -- that header must be
    // mirrored to keep the two declarations layout-identical (see report).
    unsigned char initialized;          // +0x0   init flag
    int culledCount;                    // +0x4   per-frame culled-object count
    char* quad2dMesh;                   // +0x8   2D-quad mesh handle
    int field_0xc;                      // +0xc
    unsigned char field_0x1c;           // +0x1c  init flag
    unsigned int cubeTextureCount;      // +0x10  loaded-texture-name list: count
    char** cubeTextures;                // +0x14  loaded-texture-name list: data
    char* mask2dImage;                  // +0x20  current 2D-mask Image2D pointer
    unsigned int meshCount;             // +0x24  meshes: count
    char** meshes;                      // +0x28  meshes: data
    int gameOrientation;                // +0x30  game orientation
    void* engine;                       // +0x34  engine back-reference
    AbyssEngine::AEMath::Matrix projMatrix3d;    // +0x38  perspective projection matrix
    AbyssEngine::AEMath::Matrix projOrthoMatrix; // +0x78  orthographic projection matrix
    AbyssEngine::AEMath::Matrix worldViewMatrix; // +0xb8  world-view matrix
    AbyssEngine::AEMath::Matrix identityMatrix;  // +0xf8  identity fallback (Camera/TransformGetLocal)
    unsigned int resourceCount;         // +0x134 resources: count
    char** resources;                   // +0x138 resources: data
    unsigned int fontCount;             // +0x140 fonts: count
    void** fonts;                       // +0x144 fonts: data
    unsigned int imageCount;            // +0x14c images: count
    char** images;                      // +0x150 images: data
    unsigned int transformCount;        // +0x158 transforms: count
    char** transforms;                  // +0x15c transforms: data
    unsigned int cameraCount;           // +0x164 cameras: count
    void** cameras;                     // +0x168 cameras: data
    unsigned int currentCamera;         // +0x170 current camera index
    unsigned int materialCount;         // +0x174 materials: count
    void** materials;                   // +0x178 materials: data
    unsigned int spriteSystemCount;     // +0x180 sprite systems: count
    char** spriteSystems;               // +0x184 sprite systems: data
    unsigned int glowMeshes_count;      // +0x18c glow meshes: count
    void* glowMeshes_data;              // +0x190 glow meshes: data
    unsigned int glowMeshes_cap;        // +0x194 glow meshes: capacity
    unsigned int glowMatA_count;        // +0x198 glow matrix list A: count
    void* glowMatA_data;                // +0x19c glow matrix list A: data
    unsigned int glowMatA_cap;          // +0x1a0 glow matrix list A: capacity
    unsigned int glowMatB_count;        // +0x1a4 glow matrix list B: count
    void* glowMatB_data;                // +0x1a8 glow matrix list B: data
    unsigned int glowMatB_cap;          // +0x1ac glow matrix list B: capacity
    unsigned int glowUints_count;       // +0x1b0 glow uint list: count
    void* glowUints_data;               // +0x1b4 glow uint list: data
    unsigned int glowUints_cap;         // +0x1b8 glow uint list: capacity
    unsigned int glowMatC_count;        // +0x1bc glow matrix list C: count
    void* glowMatC_data;                // +0x1c0 glow matrix list C: data
    unsigned int glowMatC_cap;          // +0x1c4 glow matrix list C: capacity
    char* lineMesh;                     // +0x1c8 glow line-draw mesh handle
    unsigned int field_0x1cc;           // +0x1cc
    float lineVerts[8];                 // +0x1d0 2D line/rect vertex scratch buffer
    unsigned char bgFlagSaved;          // +0x1f0 saved background-render flag
    char fogMode;                       // +0x1f1 active fog mode
    int fogEnableFlag;                  // +0x1f4 fog enable flag
    unsigned char field_0x1f8;          // +0x1f8 init flag
    float colorR;                       // +0x1fc color r / fog r
    float colorG;                       // +0x200 color g / fog g
    float colorB;                       // +0x204 color b / fog b
    float colorA;                       // +0x208 color a / fog a

    // ---- construction / destruction -------------------------------------------
    explicit PaintCanvas(Engine *engine);
    ~PaintCanvas();

    // ---- methods (recovered from the binary) ----------------------------------
    // Frame / render-state
    void Initialize(bool landscape);
    void Resume();
    void Suspend();
    void Begin2d();
    void End2d();
    void Begin3d();
    void End3d();
    void BeginBG();
    void EndBG();
    void ClearBuffer(unsigned int mask);
    void SetColor(unsigned int color);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    unsigned int GetColor();
    void SetBlendMode(AbyssEngine::BlendMode mode);
    void SetTexture(unsigned int, unsigned int);
    void SetShaderMode(int mode);
    void SetGameOrientation(AbyssEngine::LandscapeMode orientation);
    void SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &);
    void SetProjOrthoMatrix();
    void SetProjectionMatrix3d(float fov, float nearPlane, float farPlane);
    void ResetPersMatrix();
    void EnableClip(int x, int y, int w, int h);
    void DisableClip();
    void FogEnable(bool mode, AbyssEngine::FogMode enable);
    void FogSetParameter(AbyssEngine::FogMode mode, float fogStart, float fogEnd, float fogDensity, unsigned int color);
    void ChangeCubeTexture(unsigned int idx);
    void GetWidth();
    void GetHeight();
    void HasVibration();
    void Vibrate(unsigned short);
    void GetAccelValue();
    void GetGravValue();
    void SwapBuffer();
    void StartDraw2FBO();
    void StopDraw2FBO();
    void CheckNUseRefractFBO(bool);
    void ClearDepth();
    void DrawFrameBufferTexture(int, int, int, int, int, int);
    bool WarmUpTexture();

    // 2D drawing
    void FillRectangle(int x, int y, int w, int h);
    void DrawRectangle(int x, int y, int w, int h);
    void DrawLine(int x0, int y0, int x1, int y1);
    void DrawImage2D(unsigned int index, int x, int y);
    void DrawImage2D(unsigned int index, int x, int y, unsigned char flipFlags);
    void DrawImage2D(unsigned int index, int x, int y, unsigned char regionAlignFlags, unsigned char placeFlags);
    void DrawImage2D(unsigned int index, int x, int y, int w, int h, unsigned char alignFlags, unsigned char anchorFlags, unsigned char flipFlags);
    void DrawRegion2D(unsigned int index, float unused, int rotSteps, int pivotX, int pivotY, int transX, float scaleX, float scaleY);
    void DrawRegion2D(unsigned int index, int srcX, int srcY, int destW, int destH, float unused, int transY, int pivotX, int pivotY, int transX);
    void Image2DCreate(unsigned short resId, unsigned int &out);
    unsigned short GetImage2DWidth(unsigned int index);
    unsigned short GetImage2DHeight(unsigned int index);
    void RestoreImage2D(AbyssEngine::Image2D *image);

    // Text
    void DrawString(unsigned int index, const unsigned short *str, int x, int y, bool b);
    void DrawString(unsigned int index, const AbyssEngine::String &str, int x, int y, bool b);
    void DrawStringColor(unsigned int index, const AbyssEngine::String &text, int x, int y, bool b);
    void DrawTextLines(unsigned int font, ::Array<AbyssEngine::String *> *arr, int x, int y);
    void DrawTextLines(unsigned int font, ::Array<AbyssEngine::String *> *arr, int x, int y, bool center);
    void DrawTextLines(unsigned int font, ::Array<AbyssEngine::String *> *arr, int x, int y, unsigned int p5, bool flag);
    int GetTextWidth(unsigned int index, const AbyssEngine::String &str);
    int GetTextWidth(unsigned int index, const AbyssEngine::String &str, unsigned int begin, unsigned int end);
    AbyssEngine::String GetReverseString(AbyssEngine::String in);
    AbyssEngine::String GetReverseString(AbyssEngine::String in, bool reverse);
    int GetTextHeight(unsigned int index);
    void GetLine(unsigned int font, AbyssEngine::String str, int maxWidth, AbyssEngine::String *out);
    void GetLineArray(unsigned int font, const AbyssEngine::String &str, int width, ::Array<AbyssEngine::String *> *outArray);
    void CheckString(unsigned int index, const AbyssEngine::String &str);
    void FontCreate(unsigned short resId, unsigned int &out, bool unused);
    int FontGetSpacing(unsigned int index);
    void FontSetSpacing(unsigned int index, short spacing);
    int FontGetYOffset(unsigned int index);
    void FontSetYOffset(unsigned int index, short yoff);

    // Cameras
    void CameraCreate(unsigned int &out);
    unsigned int CameraGetCurrent();
    void CameraSetCurrent(unsigned int index);
    void *CameraGetLocal(unsigned int index);
    void CameraSetLocal(unsigned int index, const Matrix &matrix);
    void CameraSetPerspective(unsigned int index, float a, float b, float c);
    void CameraSetPerspective(unsigned int index, float fov, float aspect);
    float CameraGetCurrentFactor1();
    int CameraIsSphereinViewFrustum(const AbyssEngine::AEMath::Vector &point, float radius);
    int CameraIsPointinViewFrustum(const AbyssEngine::AEMath::Vector &point);

    // Result of a 2D pick test: the (u,v) texture coordinate hit by the pick ray,
    // or {-1.0f, -1.0f} when nothing was hit.
    struct PickedTextureRegion {
        float u;
        float v;
    };

    // Transforms
    void TransformCreate(unsigned int &out);
    void TransformCreate(unsigned short resId, unsigned int &out);
    void *TransformGetLocal(unsigned int index);
    void TransformSetLocal(unsigned int index, const Matrix &matrix);
    void TransformSetColor(unsigned int index, unsigned int color);
    void *TransformGetTransform(unsigned int index);
    int TransformGetTriCount(unsigned int index);
    int TransformGetTriCount(::Transform *transform);
    void TransformAddChild(unsigned int parent, unsigned int child);
    void TransformRemoveChild(unsigned int parent, unsigned int child);
    void TransformAddMesh(unsigned int transformIndex, unsigned short meshId, bool b);
    void TransformAddMeshId(unsigned int transformIndex, unsigned int meshIndex);
    void TransformRemoveMesh(unsigned int transformIndex, unsigned short meshResId);
    void TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex);
    void DrawTransform(::Transform *tf, const AbyssEngine::AEMath::Matrix &m2, AbyssEngine::AEMath::Matrix &m3);
    void DrawTransform(unsigned int index, const AbyssEngine::AEMath::Matrix *viewMatrix);
    PickedTextureRegion TransformGet2DPickedTextureRegion(unsigned int transformIndex, int x, int y, int z, int shift);
    PickedTextureRegion TransformGet2DPickedTextureRegion(::Transform *transform, int x, int y, int z, int shift);

    // Meshes
    void MeshCreate(unsigned short resId, unsigned int &out, bool forceClone);
    void MeshCreate(unsigned short a, unsigned short b, signed char c, unsigned int &out);
    void MeshCreate(unsigned short vertexCount, unsigned short triangleCount, signed char meshType, unsigned short matResId, unsigned int &out);
    AbyssEngine::Mesh *MeshGetPointer(unsigned int index);
    int MeshGetTriCount(AbyssEngine::Mesh *mesh);
    void DrawMesh(unsigned int index);
    void DrawMesh(AbyssEngine::Mesh *mesh, AbyssEngine::AEMath::Matrix &worldMatrix, AbyssEngine::AEMath::Matrix &viewMatrix, unsigned int color, AbyssEngine::AEMath::Matrix &uvMatrix);
    void MeshConvertToVBO(unsigned int index);
    void MeshChangeMaterial(unsigned int meshIndex, unsigned short matIndex);
    void MeshChangeMaterialIntern(AbyssEngine::Mesh *mesh, AbyssEngine::Material *mat);
    void MeshChangeMaterialIntern(::Transform *transform, AbyssEngine::Material *material);
    void MeshChangeResourceMaterial(unsigned int meshIndex, unsigned short resId);
    void MeshResourceChangeMaterial(unsigned short matId, unsigned short value);
    void MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value);
    void MeshCloneMaterial(unsigned int index, unsigned int &out);
    void MeshChangeShaderAnimValue(AbyssEngine::Mesh *mesh, float value, unsigned int mode);
    void MeshChangeShaderAnimValue(::Transform *transform, float value, unsigned int mode);
    float MeshSetPoint(unsigned int index, unsigned short vtx, float x, float y, float z);
    void MeshSetUv(unsigned int index, unsigned short sub, float u, float v);
    void MeshSetNormal(unsigned int index, unsigned short vtx, const Vector &value);
    void MeshSetTangent(unsigned int index, unsigned short vtx, const Vector &value);
    void MeshSetBiTangent(unsigned int index, unsigned short vtx, const Vector &value);
    void MeshSetColor(unsigned int index, unsigned short sub, unsigned int color);
    void MeshSetColor(unsigned int index, unsigned short sub, float r, float g, float b, float a);
    void MeshSetTriangle(unsigned int meshIndex, unsigned short tri, unsigned short v0, unsigned short v1, unsigned short v2);
    void MeshSetTriangleCount(unsigned int index, unsigned short count);
    void MeshTranslatePoint(unsigned int index, unsigned short sub, float x, float y, float z);
    void MeshSet2DMask(unsigned int index, int, int);
    void MeshClear2DMask();

    // Materials / textures / resources
    void MaterialCreate(unsigned int &out, AbyssEngine::BlendMode mode, unsigned int textures, unsigned short p4);
    void MaterialCreate(unsigned short resId, unsigned int &out);
    void *MaterialGetMaterial(unsigned int index);
    void MaterialChange(unsigned int index, AbyssEngine::BlendMode param3, unsigned int param4);
    void MaterialResourceChangeTexture(unsigned int resId, unsigned int texture, int slot);
    void TextureCreate(unsigned short resId, void (*callback)(AbyssEngine::Image *, void *), void *userData, unsigned int &out, bool useCallbackLoader);
    void TextureCreate(unsigned short id, unsigned int &out, bool flag);
    void TextureCreateGlobal(AbyssEngine::String name, unsigned int unit);
    void AddResource(AbyssEngine::Resource *resource);
    void SetResourceList(AbyssEngine::Resource *const *list, unsigned int count);
    void *FindResource(unsigned short id);
    int ResourceLoaded(unsigned int index, AbyssEngine::ResourceType type);
    unsigned int GetMeshResourceId(AbyssEngine::String &name);
    unsigned int GetMeshResourceId(AbyssEngine::String &name, unsigned short p2);
    unsigned int GetTextureResourceId(AbyssEngine::String &name);
    void ReleaseAllResources();
    void ReloadTextures();
    void RemoveAllMatsForGlow();
    void SetMatForGlow(AbyssEngine::Material *glowSource);

    // Sprite systems
    void SpriteSystemCreate(unsigned short resId, bool flag, unsigned int &out);
    void SpriteSystemCreate(unsigned short resId, bool flag, unsigned short matResId, unsigned int &out);
    void DrawSpriteSystem(unsigned int index);
    void DrawSpriteSystem(unsigned int index, AbyssEngine::AEMath::Matrix mat);
    void DrawSpriteSystem(unsigned int index, AbyssEngine::AEMath::Matrix matA, AbyssEngine::AEMath::Matrix matB);
    void ReleaseSpriteSystemResource(unsigned int index);
    float SpriteSystemSetPosition(unsigned int index, unsigned short sub, float x, float y, float z);
    void SpriteSystemAddPosition(unsigned int index, unsigned short sub, float x, float y, float z);
    void SpriteSystemGetPosition(unsigned int index, unsigned short sub, Vector &out);
    void SpriteSystemGetPosition(unsigned int index, unsigned short sub, const Matrix &m, Vector &out);
    void SpriteSystemSetSize(unsigned int index, unsigned short sub, short value);
    void SpriteSystemAddSize(unsigned int index, unsigned short sub, short delta);
    void SpriteSystemSetAllSize(unsigned int index, short size);
    void SpriteSystemSetUv(unsigned int index, unsigned short sub, float a, float b, float c, float d);
    void SpriteSystemSetAllUv(unsigned int index, float a, float b, float c, float d);
    void SpriteSystemSetRGBA(unsigned int index, unsigned short sub, float a, float b, float c, float d);

    // Screen / misc
    int GetScreenPosition(const AbyssEngine::AEMath::Vector &a, AbyssEngine::AEMath::Vector &b);
    void GetScreenPosition(const AbyssEngine::AEMath::Matrix &srcMatrix, AbyssEngine::AEMath::Vector &outVec);
    void GetScreenPosition(AbyssEngine::AEMath::Matrix &m, const AbyssEngine::AEMath::Vector &worldPos, AbyssEngine::AEMath::Vector &outVec);
};

} // namespace AbyssEngine

// PaintCanvas already lives in this namespace (AbyssEngine::PaintCanvas); the
// global alias below keeps bare `PaintCanvas` resolving to it everywhere this
// header is included.
using ::AbyssEngine::PaintCanvas;

// The recovered source refers to these by their fully-qualified AbyssEngine name
// in some places and unqualified in others. The real types live at global scope
// (to match fwd.h's global forward declarations); re-export them into the
// namespace so AbyssEngine::Mesh and bare Mesh are the same type.
namespace AbyssEngine {
using ::Node;
// Engine, Material, SpriteSystem, ImageFont and Image2D already live in this namespace
// (AbyssEngine::X); no re-export needed. PaintCanvas already lives here too.
using ::Camera;

// Array<T> alias inside the namespace (the global one lives in Array.h).
template <class T> using Array = ::Array<T>;
} // namespace AbyssEngine

extern PaintCanvas* gCanvas;       // canonical render canvas singleton (binary .bss 0x2281b8)

#endif
