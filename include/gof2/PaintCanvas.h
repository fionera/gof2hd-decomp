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

// Engine container free-functions used by PaintCanvas (definitions external).
void ArrayAddMesh(Mesh *item, void *array);
void ArrayAddUint(unsigned int item, void *array);
void ArrayAddTransform(Transform *item, void *array);
void ArrayAddMaterial(Material *item, void *array);
void ArrayAddImageFont(ImageFont *item, void *array);
void ArrayAddImage2D(Image2D *item, void *array);
void ArrayAddSpriteSystem(SpriteSystem *item, void *array);
} // namespace AbyssEngine

#endif
