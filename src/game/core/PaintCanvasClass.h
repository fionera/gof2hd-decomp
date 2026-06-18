#ifndef GOF2_PAINTCANVASCLASS_H
#define GOF2_PAINTCANVASCLASS_H
// Lean, conflict-free PaintCanvas class declaration for consumer translation
// units that already pull in Transform.h / Mesh.h / String.h / externs.h (whose
// own Mesh/Transform definitions clash with the self-contained PaintCanvas.h).
// This header forward-declares the few cross-class shapes (only used as pointer
// parameters) instead of redefining them, so it co-includes cleanly. The class
// body (fields + method declarations) is byte/signature identical to the one in
// PaintCanvas.h, so the mangled PaintCanvas:: symbols match the real definitions.
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

struct Transform;
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;
namespace AbyssEngine { class Mesh; namespace AEMath { struct Matrix; struct Vector; } }
namespace AbyssEngine { struct Material; }
namespace AbyssEngine { struct Resource; }
namespace AbyssEngine { struct Image; }
// Engine-side enums recovered from the binary's mangled method signatures (see
// the matching block in src/engine/render/PaintCanvas.h). Only the namespace-
// qualified type names matter for the ABI; the constants are conventional. Each
// uses the SAME canonical nonfixed form the engine already uses elsewhere
// (e.g. `enum BlendMode { BlendMode_dummy };` in ResourceMaterial.h /
// ParticleSystem*.h), guarded so this header never re-defines its own enum.
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

// PaintCanvas's real type lives in namespace AbyssEngine (to match the binary's
// mangling N11AbyssEngine11PaintCanvasE); the global `using` alias after the
// namespace close lets bare `PaintCanvas` resolve to AbyssEngine::PaintCanvas.
namespace AbyssEngine {

class PaintCanvas {
public:
    // ---- construction / destruction (real definitions live in PaintCanvas.cpp) -
    explicit PaintCanvas(Engine *engine);
    ~PaintCanvas();

    // LAYOUT-FAITHFUL declaration (see report). The field_0xNN names are the
    // offsets in the original 32-bit ARM binary (Ghidra PaintCanvas = 524 bytes,
    // engine@0x34, orientation@0x30). On this native 64-bit build pointers are
    // 8 bytes, so the *physical* offsets are larger (608 bytes total) -- what
    // matters is that this declaration is STRUCTURALLY identical, member-for-
    // member, to the canonical src/engine/render/PaintCanvas.h (which is what
    // PaintCanvas::PaintCanvas in PaintCanvas.cpp is compiled against). Both
    // link into one program, so the two layouts MUST agree; the consumer TUs
    // (Engine.cpp/Level.cpp/ApplicationManager.cpp) reach field_0x30/field_0x34
    // by member name and now resolve to the same words PaintCanvas.cpp wrote.
    // The earlier lean header was SPARSE -- it dropped field_0x1c, the four
    // 0x40-byte matrices, the glow Array<> triples, the lineVerts scratch and
    // field_0x1f8, so every member from field_0x20 on landed at the wrong
    // physical offset. Those gap members are restored below (matrices/lineVerts/
    // glow as raw byte pads of the exact canonical size, since AEMath::Matrix
    // cannot co-include with this lean header).
    bool gravityRotationEnabled;        // +0x0    initialized flag
    int field_0x4;                      // +0x4    culledCount
    char* field_0x8;                    // +0x8    quad2dMesh
    int field_0xc;                      // +0xc
    unsigned char field_0x1c;           // +0x1c   init flag (source-order before cubeTextureCount, as in canonical)
    unsigned int field_0x10;            // +0x10   cubeTextureCount
    char** field_0x14;                  // +0x14   cubeTextures
    char* field_0x20;                   // +0x20   current 2D-mask Image2D pointer
    unsigned int field_0x24;            // +0x24   meshCount
    char** field_0x28;                  // +0x28   meshes
    int field_0x30;                     // +0x30   game orientation
    void* field_0x34;                   // +0x34   engine back-reference
    char projMatrix3d[0x40];            // +0x38   AEMath::Matrix
    char projOrthoMatrix[0x40];         // +0x78   AEMath::Matrix
    char worldViewMatrix[0x40];         // +0xb8   AEMath::Matrix
    char identityMatrix[0x40];          // +0xf8   AEMath::Matrix
    unsigned int field_0x134;           // +0x134  resourceCount
    char** field_0x138;                 // +0x138  resources
    unsigned int field_0x140;           // +0x140  fontCount
    void** field_0x144;                 // +0x144  fonts
    unsigned int field_0x14c;           // +0x14c  imageCount
    char** field_0x150;                 // +0x150  images
    unsigned int field_0x158;           // +0x158  transformCount
    char** field_0x15c;                 // +0x15c  transforms
    unsigned int field_0x164;           // +0x164  cameraCount
    void** field_0x168;                 // +0x168  cameras
    unsigned int field_0x170;           // +0x170  currentCamera
    unsigned int field_0x174;           // +0x174  materialCount
    void** field_0x178;                 // +0x178  materials
    unsigned int field_0x180;           // +0x180  spriteSystemCount
    char** field_0x184;                 // +0x184  spriteSystems
    unsigned int glow_18c_count;        // +0x18c  glow meshes: count
    void* glow_190_data;                // +0x190  glow meshes: data
    unsigned int glow_194_cap;          // +0x194  glow meshes: capacity
    unsigned int glow_198_count;        // +0x198  glow matrix list A: count
    void* glow_19c_data;                // +0x19c  glow matrix list A: data
    unsigned int glow_1a0_cap;          // +0x1a0  glow matrix list A: capacity
    unsigned int glow_1a4_count;        // +0x1a4  glow matrix list B: count
    void* glow_1a8_data;                // +0x1a8  glow matrix list B: data
    unsigned int glow_1ac_cap;          // +0x1ac  glow matrix list B: capacity
    unsigned int glow_1b0_count;        // +0x1b0  glow uint list: count
    void* glow_1b4_data;                // +0x1b4  glow uint list: data
    unsigned int glow_1b8_cap;          // +0x1b8  glow uint list: capacity
    unsigned int glow_1bc_count;        // +0x1bc  glow matrix list C: count
    void* glow_1c0_data;                // +0x1c0  glow matrix list C: data
    unsigned int glow_1c4_cap;          // +0x1c4  glow matrix list C: capacity
    char* field_0x1c8;                  // +0x1c8  glow line-draw mesh handle
    unsigned int field_0x1cc;           // +0x1cc
    float lineVerts[8];                 // +0x1d0  2D line/rect vertex scratch buffer
    unsigned char field_0x1f0;          // +0x1f0  saved background-render flag
    char field_0x1f1;                   // +0x1f1  active fog mode
    int field_0x1f4;                    // +0x1f4  fog enable flag
    unsigned char field_0x1f8;          // +0x1f8  init flag
    float field_0x1fc;                  // +0x1fc  color r / fog r
    float field_0x200;                  // +0x200  color g / fog g
    float field_0x204;                  // +0x204  color b / fog b
    float field_0x208;                  // +0x208  color a / fog a

    // ---- methods (recovered from the binary) ----------------------------------
    // Frame / render-state
    void Initialize(bool reset);
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
    void SetProjectionMatrix3d(float fov, float zNear, float zFar);
    void ResetPersMatrix();
    void EnableClip(int x, int y, int w, int h);
    void FogEnable(bool mode, AbyssEngine::FogMode enable);
    void FogSetParameter(AbyssEngine::FogMode mode, float start, float end, float density, unsigned int color);
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

    // 2D drawing
    void FillRectangle(int x, int y, int w, int h);
    void DrawRectangle(int x, int y, int w, int h);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawImage2D(unsigned int index, int x, int y);
    void DrawImage2D(unsigned int index, int x, int y, unsigned char flipFlags);
    void DrawImage2D(unsigned int index, int x, int y, unsigned char align, unsigned char anchor);
    void DrawImage2D(unsigned int index, int x, int y, int w, int h, unsigned char anchor, unsigned char align, unsigned char flipFlags);
    void DrawRegion2D(unsigned int index, float angle, int x, int y, int w, int h, float scaleX, float scaleY);
    void DrawRegion2D(unsigned int index, int srcX, int srcY, int srcW, int srcH, float angle, int dstX, int dstY, int dstW, int dstH);
    void Image2DCreate(unsigned short id, unsigned int &out);
    unsigned short GetImage2DWidth(unsigned int index);
    unsigned short GetImage2DHeight(unsigned int index);

    // Text
    void DrawString(unsigned int index, const unsigned short *str, int x, int y, bool b);
    void DrawString(unsigned int index, void *str, int x, int y, bool b);
    void DrawStringColor(unsigned int index, const AbyssEngine::String &str, int x, int y, bool b);
    void DrawTextLines(unsigned int font, ::Array<AbyssEngine::String *> *arr, int x, int y, bool center);
    void DrawTextLines(unsigned int font, ::Array<AbyssEngine::String *> *arr, int x, int y, unsigned int p5, bool flag);
    int GetTextWidth(unsigned int index, void *str);
    int GetTextWidth(unsigned int index, const AbyssEngine::String &str, unsigned int begin, unsigned int end);
    int GetTextHeight(unsigned int index);
    void GetLine(unsigned int font, void *str, int maxWidth, void *out);
    void GetLineArray(unsigned int font, void *str, int maxWidth, char *out);
    void CheckString(unsigned int index, void *str);
    void FontCreate(unsigned short id, unsigned int &out, bool flag);
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
    int CameraIsSphereinViewFrustum(const AbyssEngine::AEMath::Vector &center, float radius);
    int CameraIsPointinViewFrustum(const AbyssEngine::AEMath::Vector &point);

    // Transforms
    void TransformCreate(unsigned int &out);
    void TransformCreate(unsigned short id, unsigned int &out);
    void *TransformGetLocal(unsigned int index);
    void TransformSetLocal(unsigned int index, const Matrix &matrix);
    void TransformSetColor(unsigned int index, unsigned int color);
    void *TransformGetTransform(unsigned int index);
    int TransformGetTriCount(unsigned int index);
    int TransformGetTriCount(char *transform);
    void TransformAddChild(unsigned int parent, unsigned int child);
    void TransformRemoveChild(unsigned int parent, unsigned int child);
    void TransformAddMesh(unsigned int transformIndex, unsigned short meshId, bool b);
    void TransformAddMeshId(unsigned int transformIndex, unsigned int meshIndex);
    void TransformRemoveMesh(unsigned int transformIndex, void *mesh);
    void TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex);
    void DrawTransform(char *tf, void *m2, void *m3);
    void DrawTransform(unsigned int index, const AbyssEngine::AEMath::Matrix *matrix);

    // Meshes
    void MeshCreate(unsigned short id, unsigned int &out, bool flag);
    void MeshCreate(unsigned short a, unsigned short b, signed char c, unsigned int &out);
    void MeshCreate(unsigned short a, unsigned short b, signed char c, unsigned short d, unsigned int &out);
    void *MeshGetPointer(unsigned int index);
    int MeshGetTriCount(AbyssEngine::Mesh *mesh);
    void DrawMesh(unsigned int index);
    void DrawMesh(char *mesh, const float *worldMatrix, const float *projMatrix, unsigned int color, const float *uvMatrix);
    void MeshConvertToVBO(unsigned int index);
    void MeshChangeMaterial(unsigned int meshIndex, unsigned short matIndex);
    void MeshChangeMaterialIntern(AbyssEngine::Mesh *mesh, AbyssEngine::Material *mat);
    void MeshChangeMaterialIntern(char *transform, void *material);
    void MeshChangeResourceMaterial(unsigned int meshIndex, unsigned short resId);
    void MeshResourceChangeMaterial(unsigned short matId, unsigned short value);
    void MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value);
    void MeshCloneMaterial(unsigned int index, unsigned int &out);
    void MeshChangeShaderAnimValue(char *mesh, float value, unsigned int mode);
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
    void MeshSet2DMask(unsigned int index, int);
    void MeshClear2DMask();

    // Materials / textures / resources
    void MaterialCreate(unsigned int *out, void *p2, void *p3);
    void MaterialCreate(unsigned short id, unsigned int &out);
    void *MaterialGetMaterial(unsigned int index);
    void MaterialChange(unsigned int index, AbyssEngine::BlendMode param3, unsigned int param4);
    void MaterialResourceChangeTexture(unsigned short resId, void *texture, unsigned int slot);
    void TextureCreate(unsigned short id, void (*loadCallback)(AbyssEngine::Image *, void *), void *userData, unsigned int &out, bool flag);
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
    void SetMatForGlow(char *material);

    // Sprite systems
    void SpriteSystemCreate(unsigned short id, bool flag, unsigned int &out);
    void SpriteSystemCreate(unsigned short id, bool flag, unsigned short matId, unsigned int &out);
    void DrawSpriteSystem(unsigned int index);
    void DrawSpriteSystem(unsigned int index, const float *mat);
    void DrawSpriteSystem(unsigned int index, const float *matA, const float *matB);
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
    void *GetScreenPosition(Vector *a, Vector *b);
    void GetScreenPosition(char *worldPos, char *out);
    void GetScreenPosition(void *matrix, void *worldPos, char *out);
};

} // namespace AbyssEngine

// PaintCanvas already lives in this namespace (AbyssEngine::PaintCanvas); the
// global alias keeps bare `PaintCanvas` resolving to it everywhere this header
// is included.
using ::AbyssEngine::PaintCanvas;

#endif
