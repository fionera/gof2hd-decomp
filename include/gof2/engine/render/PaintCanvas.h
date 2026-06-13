#ifndef GOF2_PAINTCANVAS_H
#define GOF2_PAINTCANVAS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
//
// Self-contained translation unit: the per-class cross-reference headers
// (Engine.h / Mesh.h / Node.h / String.h) were each generated to be the *only*
// class included and conflict when co-included, so PaintCanvas pulls the few
// cross-class shapes it actually needs straight into namespace AbyssEngine here.

// operator new/delete and memcpy/memset/strcmp come from the standard headers
// pulled in by common.h. The cross-class struct forward-decls live globally in
// fwd.h (also via common.h); we model below only the offsets PaintCanvas touches.

struct Material;
struct Resource;
struct ResourceTexture;
struct ResourceMesh;
struct ImageFont;
struct Image2D;
struct Transform;
struct Camera;
struct SpriteSystem;

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
struct Mesh {
    char pad_0x0[0x1c];
    float field_0x1c;                   // +0x1c  shader anim value (mode 1)
    float field_0x20;                   // +0x20  shader anim value (mode 2)
    float field_0x24;                   // +0x24  shader anim value (mode 4)
    char pad_0x28[0x30 - 0x28];
    void* field_0x30;                   // +0x30  material back-reference
    void* field_0x34;                   // +0x34  resource ptr (passed to shader_anim)
    char pad_0x38[0x84 - 0x38];
    unsigned char field_0x84;           // +0x84  vbo flag
};

struct Transform {
    char pad_0x0[0x3c];
    unsigned int field_0x3c;            // +0x3c  mesh-child count
    Mesh** field_0x40;                  // +0x40  mesh-child array
    char pad_0x44[0x4c - 0x44];
    unsigned int field_0x4c;            // +0x4c  transform-child count
    Transform** field_0x50;             // +0x50  transform-child array
};

struct Node {
    char pad_0x0[0x30];
    unsigned int field_0x30;            // +0x30
};

struct Engine {
    char pad_0x0[0xfc];
    unsigned char field_0xfc;           // +0xfc
};

class PaintCanvas {
public:
    int field_0x4;                      // +0x4
    char* field_0x8;                    // +0x8
    int field_0xc;                      // +0xc
    unsigned int field_0x10;            // +0x10
    char** field_0x14;                  // +0x14
    char* field_0x20;                   // +0x20  current 2D-mask Image2D pointer
    unsigned int field_0x24;            // +0x24
    char** field_0x28;                  // +0x28
    int field_0x30;                     // +0x30
    void* field_0x34;                   // +0x34
    unsigned int field_0x134;           // +0x134
    char** field_0x138;                 // +0x138
    unsigned int field_0x140;           // +0x140
    void** field_0x144;                 // +0x144
    unsigned int field_0x14c;           // +0x14c
    char** field_0x150;                 // +0x150
    unsigned int field_0x158;           // +0x158
    char** field_0x15c;                 // +0x15c
    unsigned int field_0x164;           // +0x164
    void** field_0x168;                 // +0x168
    unsigned int field_0x170;           // +0x170
    unsigned int field_0x174;           // +0x174
    void** field_0x178;                 // +0x178
    unsigned int field_0x180;           // +0x180
    char** field_0x184;                 // +0x184
    char* field_0x1c8;                  // +0x1c8
    unsigned char field_0x1f0;          // +0x1f0
    char field_0x1f1;                   // +0x1f1
    int field_0x1f4;                    // +0x1f4
    float field_0x1fc;                  // +0x1fc
    float field_0x200;                  // +0x200
    float field_0x204;                  // +0x204
    float field_0x208;                  // +0x208

    // ---- methods (recovered from the binary) ----------------------------------
    // Frame / render-state
    void Initialize(bool param_1);
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
    void SetBlendMode(int param_2);
    void SetTexture(unsigned int, unsigned int);
    void SetShaderMode(int mode);
    void SetGameOrientation(int param_2);
    void SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &);
    void SetProjOrthoMatrix();
    void SetProjectionMatrix3d(float param_1, float param_2, float param_3);
    void ResetPersMatrix();
    void EnableClip(int param_1, int param_2, int param_3, int param_4);
    void FogEnable(int mode, int enable);
    void FogSetParameter(int param_1, float param_2, float param_3, float param_4, unsigned int param_5);
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
    void DrawRectangle(int param_1, int param_2, int param_3, int param_4);
    void DrawLine(int param_1, int param_2, int param_3, int param_4);
    void DrawImage2D(unsigned int index, int x, int y);
    void DrawImage2D(unsigned int param_1, int param_2, int param_3, unsigned char param_4);
    void DrawImage2D(unsigned int param_1, int param_2, int param_3, unsigned char param_4, unsigned char param_5);
    void DrawImage2D(unsigned int param_1, int param_2, int param_3, int param_4, int param_5, unsigned char param_6, unsigned char param_7, unsigned char param_8);
    void DrawRegion2D(unsigned int param_1, float param_2, int param_3, int param_4, int param_5, int param_6, float param_7, float param_8);
    void DrawRegion2D(unsigned int param_1, int param_2, int param_3, int param_4, int param_5, float param_6, int param_7, int param_8, int param_9, int param_10);
    void Image2DCreate(unsigned short param_1, unsigned int *param_2);
    unsigned short GetImage2DWidth(unsigned int index);
    unsigned short GetImage2DHeight(unsigned int index);

    // Text
    void DrawString(unsigned int index, const unsigned short *str, int x, int y, bool b);
    void DrawString(unsigned int index, void *str, int x, int y, bool b);
    void DrawStringColor(unsigned int param_1, void *param_2, int param_3, int param_4, bool param_5);
    void DrawTextLines(unsigned int font, ::Array<void *> *arr, int x, int y, bool center);
    void DrawTextLines(unsigned int font, ::Array<void *> *arr, int x, int y, unsigned int p5, bool flag);
    int GetTextWidth(unsigned int index, void *str);
    int GetTextWidth(unsigned int index, void *str, unsigned int begin, unsigned int end);
    int GetTextHeight(unsigned int index);
    void GetLine(unsigned int param_1, void *param_3, int param_4, void *param_5);
    void GetLineArray(unsigned int param_1, void *param_2, int param_3, char *param_4);
    void CheckString(unsigned int index, void *str);
    void FontCreate(unsigned short param_1, unsigned int *param_2, bool param_3);
    int FontGetSpacing(unsigned int index);
    void FontSetSpacing(unsigned int index, short spacing);
    int FontGetYOffset(unsigned int index);
    void FontSetYOffset(unsigned int index, short yoff);

    // Cameras
    void CameraCreate(unsigned int *out);
    unsigned int CameraGetCurrent();
    void CameraSetCurrent(unsigned int index);
    void *CameraGetLocal(unsigned int index);
    void CameraSetLocal(unsigned int index, const Matrix &matrix);
    void CameraSetPerspective(unsigned int index, float a, float b, float c);
    void CameraSetPerspective(unsigned int index, float fov, float aspect);
    float CameraGetCurrentFactor1();
    int CameraIsSphereinViewFrustum(void *param_1, float param_2);
    int CameraIsPointinViewFrustum(void *param_1);

    // Transforms
    void TransformCreate(unsigned int *out);
    void TransformCreate(unsigned short param_1, unsigned int *param_2);
    void *TransformGetLocal(unsigned int index);
    void TransformSetLocal(unsigned int index, const Matrix &matrix);
    void TransformSetColor(unsigned int index, unsigned int color);
    void *TransformGetTransform(unsigned int index);
    int TransformGetTriCount(unsigned int index);
    int TransformGetTriCount(char *transform);
    void TransformAddChild(unsigned int parent, unsigned int child);
    void TransformRemoveChild(unsigned int parent, unsigned int child);
    void TransformAddMesh(unsigned int transformIndex, unsigned short meshId, bool b);
    void TransformAddMeshId(unsigned int param_1, unsigned int param_2);
    void TransformRemoveMesh(unsigned int transformIndex, void *mesh);
    void TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex);
    void DrawTransform(char *tf, void *m2, void *m3);
    void DrawTransform(unsigned int param_1, const float *param_2);

    // Meshes
    void MeshCreate(unsigned short param_1, unsigned int *param_2, bool param_3);
    void MeshCreate(unsigned short a, unsigned short b, signed char c, unsigned int *out);
    void MeshCreate(unsigned short param_1, unsigned short param_2, signed char param_3, unsigned short param_4, unsigned int *param_6);
    void *MeshGetPointer(unsigned int index);
    int MeshGetTriCount(char *mesh);
    void DrawMesh(unsigned int index);
    void DrawMesh(char *param_1, const float *param_2, const float *param_3, unsigned int param_4, const float *param_5);
    void MeshConvertToVBO(unsigned int index);
    void MeshChangeMaterial(unsigned int meshIndex, unsigned int matIndex);
    void MeshChangeMaterialIntern(::Mesh *mesh, void *mat);
    void MeshChangeMaterialIntern(char *transform, void *material);
    void MeshChangeResourceMaterial(unsigned int meshIndex, unsigned int resId);
    void MeshResourceChangeMaterial(unsigned short matId, unsigned short value);
    void MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value);
    void MeshCloneMaterial(unsigned int index, unsigned int *out);
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
    void MaterialCreate(unsigned short param_1, unsigned int *param_2);
    void *MaterialGetMaterial(unsigned int index);
    void MaterialChange(unsigned int index, unsigned int param3, unsigned int param4);
    void MaterialResourceChangeTexture(unsigned short resId, void *texture, unsigned int slot);
    void TextureCreate(unsigned short param_1, void *param_2, void *param_3, unsigned int *param_4, bool param_5);
    void TextureCreate(unsigned short id, unsigned int *out, bool flag);
    void TextureCreateGlobal(::String *name, int unit);
    void AddResource(void *resource);
    void SetResourceList(void **list, unsigned int count);
    void *FindResource(unsigned short id);
    int ResourceLoaded(unsigned int index, unsigned int type);
    unsigned int GetMeshResourceId(void *name);
    unsigned int GetMeshResourceId(void *name, unsigned short p2);
    unsigned int GetTextureResourceId(void *name);
    void ReleaseAllResources();
    void ReloadTextures();
    void RemoveAllMatsForGlow();
    void SetMatForGlow(char *param_1);

    // Sprite systems
    void SpriteSystemCreate(unsigned short param_1, bool param_2, unsigned int *param_3);
    void SpriteSystemCreate(unsigned short param_1, bool param_2, unsigned short param_3, unsigned int *param_4);
    void DrawSpriteSystem(unsigned int param_1);
    void DrawSpriteSystem(unsigned int param_1, const float *mat);
    void DrawSpriteSystem(unsigned int param_1, const float *matA, const float *matB);
    void ReleaseSpriteSystemResource(unsigned int index);
    float SpriteSystemSetPosition(unsigned int index, unsigned short sub, float x, float y, float z);
    void SpriteSystemAddPosition(unsigned int index, unsigned short sub, float x, float y, float z);
    void SpriteSystemGetPosition(unsigned int index, unsigned short sub, Vector &out);
    void SpriteSystemGetPosition(unsigned int index, unsigned short sub, const Matrix &m, Vector &out);
    void SpriteSystemSetSize(unsigned int index, unsigned int sub, unsigned short value);
    void SpriteSystemAddSize(unsigned int index, unsigned short sub, short delta);
    void SpriteSystemSetAllSize(unsigned int index, unsigned int size);
    void SpriteSystemSetUv(unsigned int index, unsigned short sub, float a, float b, float c, float d);
    void SpriteSystemSetAllUv(unsigned int index, float a, float b, float c, float d);
    void SpriteSystemSetRGBA(unsigned int index, unsigned short sub, float a, float b, float c, float d);

    // Screen / misc
    void *GetScreenPosition(Vector *a, Vector *b);
    void GetScreenPosition(char *param_1, char *param_2);
    void GetScreenPosition(void *param_1, void *param_2, char *param_3);
};

// The recovered source refers to these by their fully-qualified AbyssEngine name
// in some places and unqualified in others. The real types live at global scope
// (to match fwd.h's global forward declarations); re-export them into the
// namespace so AbyssEngine::Mesh and bare Mesh are the same type.
namespace AbyssEngine {
using ::Mesh;
using ::Node;
using ::Engine;
using ::PaintCanvas;
using ::Material;
using ::Transform;
using ::Camera;
using ::SpriteSystem;
using ::ImageFont;
using ::Image2D;

// Array<T> alias inside the namespace (the global one lives in common.h).
template <class T> using Array = ::Array<T>;
} // namespace AbyssEngine

#endif
