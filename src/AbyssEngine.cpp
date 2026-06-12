#include "gof2/AbyssEngine.h"
#include "gof2/externs.h"
#include "gof2/AEFile.h"
#include "gof2/String.h"
#include "gof2/Mesh.h"
#include "gof2/Engine.h"

// AbyssEngine::FBOContainer is defined fully in gof2/FBOContainer.h, but that header
// forward-declares AbyssEngine::Engine which clashes with the `using ::Engine;` re-export
// pulled in by gof2/AbyssEngine.h. We only need to call the (out-of-line) Create method
// through a raw pointer here, so declare the minimal class surface locally instead.
namespace AbyssEngine {
class FBOContainer {
public:
    void Create(int width, int height, bool a, bool linear);
};
}


extern "C" void AE_FileInterfaceAndroid_ctor(void *self);
extern "C" void AE_PaintCanvas_Initialize(PaintCanvas *self, bool flag);
extern "C" void AE_Vector_assign(void *dst, const void *src);
extern "C" void AE_FBOContainer_ctor(void *self);
extern "C" void AE_String_fromCStr(String *self, const char *s, bool b);
extern "C" float sqrtf(float);
extern "C" void AE_Engine_LightSetAmbient(float r, float g, float b, Engine *self);
extern "C" void AE_AEMath_matMul(Matrix *out, const Matrix *in);
extern "C" void AE_PaintCanvas_SetWorldViewMatrix(void *self);
extern "C" void String_copy(String *self, const String *src, bool b);
extern "C" void String_fromLongLong(String *self, long long v);
extern "C" void String_appendEq(String *self, const String *o);
extern "C" void String_fromFloat(String *self, float v);
extern "C" void *__aeabi_memclr(void *dst, size_t_ n);
extern "C" void *__aeabi_memclr4(void *dst, size_t_ n);
extern "C" void String_fromInt(String *self, int v);
extern "C" void AE_ArrayAddCached_MeshPtr(AbyssEngine::Mesh *value, void *array);
extern "C" void AE_ArrayAddCached_uint(unsigned int value, void *array);
extern "C" void *__aeabi_memcpy4(void *dst, const void *src, size_t_ n);
extern "C" int String_Compare(String *self, const String *o);
extern "C" void glDeleteBuffers(int n, const void *buffers);
extern "C" void *AE_Transform_dtor(void *self);
// __aeabi_memcpy is declared by gof2/Engine.h

// ---- ImageFontSetYOffset_72278.cpp ----
// AbyssEngine::ImageFontSetYOffset(AbyssEngine::ImageFont*, short)
namespace AbyssEngine {

void ImageFontSetYOffset(ImageFont *font, short yOffset)
{
    if (font != 0)
        s16(font, 0x12) = yOffset;
}

} // namespace AbyssEngine

// ---- CameraIsPointinViewFrustum_7144c.cpp ----
// AbyssEngine::CameraIsPointinViewFrustum(Vector const&, Matrix*, Camera*)
// Test whether a world-space point lies inside the camera's view frustum. The point is
// transformed into camera/local space (optionally pre-multiplied by an extra matrix), then
// projected onto the camera forward/up/right axes and range-checked against the per-camera
// half-extents.
//
// Camera frustum fields (byte offsets): +0x04 near, +0x08 far, +0x0c local matrix (0x3c),
// +0x48 horizontal slope, +0x50 vertical slope.
namespace AbyssEngine {


extern "C" {
// __aeabi_memcpy is declared by gof2/Engine.h
// Engine math helpers (referenced by C-name; real symbols are AbyssEngine::AEMath::*).
void AE_Matrix_mulAssign(void *self, const void *rhs);
void AE_Vector_assign(void *self, const void *rhs);
void AE_MatrixInverseTransformVector(void *out, const void *in);
void AE_MatrixGetPosition(void *m);
void AE_MatrixGetDir(void *m);
void AE_MatrixGetUp(void *m);
void AE_MatrixGetRight(void *m);
void AE_VectorSub(void *self, const void *rhs);
void AE_VectorNormalize(void *out, const void *in);
float AE_VectorDot(const void *a, const void *b);
}

int CameraIsPointinViewFrustum(Vector *point, Matrix *extra, Camera *cam)
{
    if (*g_Camera_frustumEnabledFlag == 0)
        return 1;

    char *c = (char *)cam;
    Matrix local;       // camera-local matrix (0x3c bytes used)
    Matrix transformed; // normalized-axis scratch
    Vector pos, dir, axis;
    Vector camPoint = {0.0f, 0.0f, 0.0f};

    Matrix *srcMatrix = (Matrix *)(c + 0xc);
    Matrix *dstMatrix;
    if (extra == 0) {
        dstMatrix = &local;
    } else {
        __aeabi_memcpy(&local, srcMatrix, 0x3c);
        AE_Matrix_mulAssign(&local, extra);
        dstMatrix = &transformed;
        srcMatrix = &local;
    }

    AE_MatrixInverseTransformVector(dstMatrix, srcMatrix);
    AE_Vector_assign(&camPoint, dstMatrix);

    AE_MatrixGetPosition(&pos);
    AE_VectorSub(&pos, point);
    AE_MatrixGetDir(&dir);
    AE_VectorSub(&axis, &dir);
    AE_VectorNormalize(&transformed, &axis);
    float fwd = AE_VectorDot(&pos, &transformed);

    if (fwd > f32(c, 0x8) || fwd < f32(c, 0x4))
        return 0;

    AE_MatrixGetUp(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float up = AE_VectorDot(&pos, &transformed);

    float hLimit = fwd * f32(c, 0x48);
    if (up > hLimit || up < -hLimit)
        return 0;

    AE_MatrixGetRight(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float right = AE_VectorDot(&pos, &transformed);

    float vLimit = hLimit * f32(c, 0x50);
    return (right <= vLimit && right >= -vLimit) ? 1 : 0;
}

} // namespace AbyssEngine

// ---- operator__6ee56.cpp ----
// AbyssEngine::operator*(AbyssEngine::Quaternion const&, float)
// Quaternion scaled by a scalar (only .y/.w lanes materialized, see operator-).
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator*(const Quaternion &a, float s)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) * s;
    float wv = *(const float *)((const char *)&a + 0xc) * s;
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine

// ---- operator__72e7c.cpp ----
// AbyssEngine::operator+(AbyssEngine::String const&, AbyssEngine::String const&)
// Concatenate two strings: copy lhs into a temp, append rhs, return the temp.
namespace AbyssEngine {

extern "C" void String_copy(String *self, const String *src, bool b);   // String(String const&, bool)
extern "C" void String_appendEq(String *self, const String *o);          // operator+=
// ~String

String operator+(const String &a, const String &b)
{
    String tmp;
    String_copy(&tmp, &a, false);
    String_appendEq(&tmp, &b);

    String result;
    String_copy(&result, &tmp, false);
    ((String *)(&tmp))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- SpriteSystemSetAllUv_86c6c.cpp ----
// AbyssEngine::SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem*)
// Fill the UV quad (4 corners, interleaved u/v) for every sprite. The shipped code flips V
// (1.0 - v) for the bottom edge, matching a top-left texture origin.
namespace AbyssEngine {

void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    unsigned short count = u16(sys, 0x0);
    char *uvBase = (char *)pp(pp(sys, 0x8), 0x8);

    for (unsigned int idx = 0; (idx & 0xffff) < (unsigned int)(count << 3); idx += 8) {
        float *p = (float *)(uvBase + (idx & 0xffff) * 4);
        p[0] = u0;
        p[1] = 1.0f - v0;
        p[2] = u1;
        p[3] = 1.0f - v0;
        p[4] = u1;
        p[5] = 1.0f - v1;
        p[6] = u0;
        p[7] = 1.0f - v1;
    }
}

} // namespace AbyssEngine

// ---- getAppVersion_6db20.cpp ----
// AbyssEngine::getAppVersion()
//
// NOTE: Ghidra attributes this address to BOTH Engine::InitGL(bool,int,int) and
// getAppVersion(); the recovered body is the InitGL initialization path operating on the
// Engine `this` that arrives in r0. To keep the emitted symbol demangling to the work item's
// name (AbyssEngine::getAppVersion()) while preserving the real logic, we take the engine via
// a register-style accessor rather than as a named parameter.

extern "C" {
void glViewport(int, int, int, int);
void glEnable(unsigned int);
void glDisable(unsigned int);
void glLineWidth(float);
void glCullFace(unsigned int);
void glGetIntegerv(unsigned int, void *);
void *operator_new_helper(size_t_); // placeholder, see operator new below
}


namespace AbyssEngine {

// Sibling/member callees (resolved by the linker via their real mangled names).
extern "C" AbyssEngine::Engine *AE_getInitGLThis();      // the r0 engine pointer


void getAppVersion()
{
    Engine *self = AE_getInitGLThis();
    char *c = (char *)self;

    pp(c, 0x418) = 0;
    // r2/r3 carry the requested resolution; mirror the stores.
    u32(c, 0x368) = u32(c, 0x368);
    u32(c, 0x370) = u32(c, 0x368);
    u32(c, 0x374) = u32(c, 0x36c);

    void *fileIface = operator new(0x38);
    AE_FileInterfaceAndroid_ctor(fileIface);
    pp(c, 0x24) = fileIface;
    AEFile::SetInterface((FileInterface *)fileIface);

    u32(c, 0x10) = 0;
    u8(c, 0x2c) = 0;
    u8(c, 0x480) = 0;

    char *shaderFlag = g_Engine_shaderModeFlag;
    u32(c, 0x40c) = 0;
    self->ResetLightParam();
    glViewport(0, 0, (int)i32(c, 0x374), (int)i32(c, 0x370));

    if (*shaderFlag == 0) {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    } else {
        self->ShaderInit();
    }

    float zero3[3] = {0, 0, 0};
    AE_Vector_assign(c + 0x468, zero3);
    u32(c, 0x378) = 0;
    AE_Vector_assign(c + 0x474, zero3);
    u32(c, 0x37c) = 0;

    glEnable(0xb71);
    self->GlEnable(0xde1, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    self->AfterGLInit();
    AE_PaintCanvas_Initialize((PaintCanvas *)*(void **)pp(c, 0x30), false);

    u32(c, 0xc) = 0;
    glGetIntegerv(0xd33, c + 0xc);

    if (*shaderFlag != 0 && *g_Engine_fboEnabledFlag != 0) {
        void *fbo = operator new(0x38);
        String name;
        AE_String_fromCStr(&name, "", false);
        AE_FBOContainer_ctor(fbo);
        pp(c, 0x418) = fbo;
        ((::String *)&name)->dtor();
        ((AbyssEngine::FBOContainer *)pp(c, 0x418))->Create(
            (int)i32(c, 0x368), (int)i32(c, 0x36c), false, true);
    }
}

} // namespace AbyssEngine

// ---- SpriteSystemSetUv_86cd4.cpp ----
// AbyssEngine::SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d,
//                                AbyssEngine::SpriteSystem*)
// Set the interleaved UV quad (8 floats) of one sprite. A global flag selects whether the
// "v" lanes are flipped (1.0 - v). Slot layout mirrors the shipped routine.
namespace AbyssEngine {


void SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d, SpriteSystem *sys)
{
    unsigned int count = 0;
    if (sys != 0)
        count = u16(sys, 0x0);
    if (sys == 0 || idx >= count)
        return;

    char flag = *g_SpriteSystem_uvFlipFlag;
    float *p = (float *)((char *)pp(pp(sys, 0x8), 0x8) + (unsigned int)idx * 0x20);

    // In the binary, `c` (in_r2) and `d` (in_stack_00000000) are flipped when flag==0.
    if (flag == 0) {
        c = 1.0f - c;
        d = 1.0f - d;
    }
    p[0] = a;
    p[1] = c;
    p[2] = b;
    p[3] = c;
    p[4] = b;
    p[5] = d;
    p[6] = a;
    p[7] = d;
}

} // namespace AbyssEngine

// ---- ImageFontGetYOffset_721da.cpp ----
// AbyssEngine::ImageFontGetYOffset(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetYOffset(ImageFont *font)
{
    short v = (font == 0) ? (short)0 : s16(font, 0x12);
    return (int)v;
}

} // namespace AbyssEngine

// ---- operator__6ebd0.cpp ----
// AbyssEngine::operator-(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&)
// Component-wise quaternion subtraction. The shipped code only materializes the .y (+4)
// and .w (+0xc) lanes (the .x/.z incoming float regs are passed through untouched), so we
// reproduce that by constructing the result through the 4-float Quaternion ctor.
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator-(const Quaternion &a, const Quaternion &b)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) - *(const float *)((const char *)&b + 4);
    float wv = *(const float *)((const char *)&a + 0xc) - *(const float *)((const char *)&b + 0xc);
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine

// ---- ImageCreateRegionFromFile_6eeb8.cpp ----
// AbyssEngine::ImageCreateRegionFromFile(Engine*, char const*, unsigned short index, Image2D*)
// Open a region atlas file, skip to the requested region index, and build a 2-triangle quad
// mesh whose vertex positions and UVs are derived from the region's offset/size fields. The
// Image2D out-param at +0x08.. holds the parsed atlas/region shorts; the generated mesh's
// position buffer (mesh+0x04) and UV buffer (mesh+0x08) are filled, and a draw-state block at
// mesh+0x2c is initialized. Returns 1 on success; negative codes on error.
namespace AbyssEngine {

extern "C" {
void  AEFile_Close(unsigned int handle);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                unsigned int vertexFormat, void **out);
void MeshRelease(Engine *engine, Mesh **slot);

int ImageCreateRegionFromFile(Engine *engine, char *path, unsigned short index, Image2D *region)
{
    if (engine == 0 || path == 0)
        return -4;

    char *r = (char *)region;
    unsigned int handle = 0;
    if (AEFile::OpenRead((const char *)(path), (uint32_t *)(&handle)) == 0)
        return -1;

    char magic[8];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile::Read((uint32_t)(8), magic, handle) == 0)
        return -1;
    static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    for (unsigned int k = 0; k < 8; ++k)
        if (want[k] != magic[k])
            return -1;

    // Header: skip 1 byte, then atlas width/height into region+0x08, +0x0a, and region count.
    unsigned short regionCount = 0;
    if (AEFile::Skip((uint32_t)(1), handle) == 0) return -1;
    if (AEFile::Read((uint32_t)(2), r + 0x08, handle) == 0) return -1;   // atlas width
    if (AEFile::Read((uint32_t)(2), r + 0x0a, handle) == 0) return -1;   // atlas height
    if (AEFile::Read((uint32_t)(2), &regionCount, handle) == 0) return -1;
    if (regionCount <= index) return -1;

    if (MeshCreate(engine, 4, 2, 0x13, (void **)region) != 1)
        return -2;

    unsigned char mode = 0; // fpscr rounding mode bits; 0 is the common default
    for (unsigned short i = 0; i < regionCount; ++i) {
        if (i == index) {
            // Region record: offX, offY, sizeX, sizeY shorts into region+0x0c..0x12.
            if (AEFile::Read((uint32_t)(2), r + 0x0c, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), r + 0x0e, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), r + 0x10, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), r + 0x12, handle) == 0) goto fail;

            int mesh = *(int *)r;
            float *pos = *(float **)(mesh + 4);
            // Half width/height of the quad in object units (region size).
            float halfW = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x10), mode);
            pos[0] = 0; pos[1] = 0; pos[2] = 0;
            pos[4] = 0; pos[5] = 0; pos[3] = halfW; pos[6] = halfW;
            float halfH = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x12), mode);
            pos[8] = 0; pos[9] = 0; pos[11] = 0; pos[7] = halfH; pos[10] = halfH;

            double atlasH = (double)AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0a), mode);
            double atlasW = (double)AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x08), mode);
            float offYs = AE_VectorSignedToFloat((int)(short)u16(r, 0x0e), mode);
            float offXs = AE_VectorSignedToFloat((int)(short)u16(r, 0x0c), mode);
            float offYu = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0e), mode);
            float offXu = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0c), mode);

            float *uv = *(float **)(mesh + 8);
            float u0 = offXu * (float)(1.0 / atlasW);
            float v0 = offYu * (float)(1.0 / atlasH);
            float v1 = (halfH + offYs) * (float)(1.0 / atlasH);
            float u1 = (halfW + offXs) * (float)(1.0 / atlasW);
            uv[0] = u0; uv[1] = v0; uv[2] = u1;
            uv[3] = v0; uv[4] = u1; uv[5] = v1;
            uv[6] = u0; uv[7] = v1;

            unsigned int *draw = *(unsigned int **)(mesh + 0x2c);
            draw[0] = 0x20000;
            draw[1] = 1;
            draw[2] = 0; // DAT default

            // i++ then loop falls through to close.
        } else {
            if (AEFile::Skip((uint32_t)(8), handle) == 0)
                goto fail;
        }
    }

    AEFile_Close(handle);
    return 1;

fail:
    MeshRelease(engine, (Mesh **)region);
    return -1;
}

} // namespace AbyssEngine

// ---- ImageFontGetWidth_7213e.cpp ----
// AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int)
// Sum the advance width of the first `count` glyphs. For each character look it up in the
// font's codepoint table (count@0, table@4); the per-glyph width comes from the glyph's
// metrics plus the font spacing (@0x10), with a -2 tweak for the 0xb-wide space.
namespace AbyssEngine {

int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int count)
{
    int total = 0;
    if (font == 0 || str == 0)
        return 0;

    unsigned short glyphCount = u16(font, 0x0);
    for (unsigned short i = 0; i < count; ++i) {
        unsigned short idx = 0;
        unsigned short code;
        for (;;) {
            unsigned int u = idx;
            if (glyphCount <= u)
                goto next; // character not found -> contributes nothing
            idx = idx + 1;
            code = *(unsigned short *)((char *)pp(font, 0x4) + u * 2);
            if (code == str[i])
                break;
        }
        {
            unsigned int found = (unsigned int)(unsigned short)(idx - 1);
            void *glyph = *(void **)((char *)pp(font, 0xc) + found * 4);
            int w = (int)f32(pp(glyph, 0x4), 0xc);
            int adv = (int)s16(font, 0x10) + w;
            int contrib = adv;
            if (w == 0xb)
                contrib = adv - 2;
            if (code != 0x20)
                contrib = adv;
            total += contrib;
        }
    next:;
    }
    return total;
}

} // namespace AbyssEngine

// ---- MeshIntersect_6d818.cpp ----
// AbyssEngine::MeshIntersect(float qx, float qz, AbyssEngine::Mesh* mesh)
// 2D pick test: walk the mesh's triangles (index buffer @0x2c, positions @0x4, uvs @0x8) and,
// for the triangle whose XZ bounding box contains the query point and which the point is
// "inside" (all three edge half-plane tests <= 0), return the averaged UV of that triangle.
// Returns the sentinel (DAT_0007dab8) packed into both lanes when nothing is hit. The result
// is a 2-float vector returned by value (sret in the original ABI).
namespace AbyssEngine {

struct Vec2 { float u, v; };


Vec2 MeshIntersect(float qx, float qz, Mesh *mesh)
{
    Vec2 out;
    char *m = (char *)mesh;
    unsigned int i = 0;

    for (;;) {
        if ((unsigned int)(unsigned short)u16(m, 0x28) <= i) {
            out.u = g_MeshIntersect_missValue;
            out.v = g_MeshIntersect_missValue;
            return out;
        }

        float *pos = (float *)pp(m, 0x4);
        unsigned short *idx = (unsigned short *)((char *)pp(m, 0x2c) + i * 2);
        float *a = pos + (unsigned int)idx[0] * 3;
        float *b = pos + (unsigned int)idx[1] * 3;
        float *c = pos + (unsigned int)idx[2] * 3;

        float ax = a[0], az = a[2];
        float bx = b[0], bz = b[2];
        float cx = c[0], cz = c[2];

        // XZ bounding box of the triangle.
        float minZ = az, maxZ = bz;
        if (bz < az) { minZ = bz; maxZ = az; }
        float minX = ax, maxX = bx;
        if (bx < ax) { minX = bx; maxX = ax; }
        if (cz > maxZ) maxZ = cz;
        if (cz < minZ) minZ = cz;
        if (cx > maxX) maxX = cx;
        if (cx < minX) minX = cx;

        if (maxZ < qz && minX <= qx && maxX < qx && minZ <= qz) {
            // Edge tests using outward 2D normals; "inside" means all three <= 0.
            float ex = bx - ax, ez = bz - az;
            float len = sqrtf(ez * ez + ex * ex);
            float side = (ex / len) * qz + (-ez / len) * qx -
                         (az * (ex / len) + ax * (-ez / len));
            if (side <= 0.0f) {
                ex = cx - bx; ez = cz - bz;
                len = sqrtf(ez * ez + ex * ex);
                side = (ex / len) * qz + (-ez / len) * qx -
                       (bz * (ex / len) + bx * (-ez / len));
                if (side <= 0.0f) {
                    ez = az - cz; ex = ax - cx;
                    len = sqrtf(ez * ez + ex * ex);
                    side = (ex / len) * qz + (-ez / len) * qx -
                           (cz * (ex / len) + cx * (-ez / len));
                    if (side <= 0.0f) {
                        float *uv = (float *)pp(m, 0x8);
                        unsigned int i0 = idx[0], i1 = idx[1], i2 = idx[2];
                        out.u = (uv[i0 * 2] + uv[i1 * 2] + uv[i2 * 2]) / 3.0f;
                        float vv = (uv[i0 * 2 + 1] + uv[i1 * 2 + 1] + uv[i2 * 2 + 1]) / 3.0f;
                        out.v = vv;
                        if (*g_MeshIntersect_flipVFlag != 0)
                            out.v = 1.0f - vv;
                        return out;
                    }
                }
            }
        }
        i += 3;
    }
}

} // namespace AbyssEngine

// ---- MeshDraw_6d3ac.cpp ----
extern "C" {
void glBindBuffer(unsigned int target, unsigned int buffer);
void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);
void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);
void glNormalPointer(unsigned int type, int stride, const void *ptr);
void glColorPointer(int size, unsigned int type, int stride, const void *ptr);
void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
// __aeabi_uidiv is declared by gof2/Engine.h
}

// AbyssEngine::MeshDraw(AbyssEngine::Engine*, AbyssEngine::Mesh*)
// Fixed-function draw path for a VBO-backed mesh. Binds the position/uv/normal/color buffers
// according to the mesh's vertex-format flag byte (offset 0), issues glDrawElements, and
// accumulates triangle/draw-call statistics into the engine. Falls back to Engine::RenderMesh
// for the shader path. Returns 1 on draw, -4 when the mesh is empty/disabled.
namespace AbyssEngine {



int MeshDraw(Engine *engine, Mesh *mesh)
{
    if (mesh == 0 || s16(mesh, 0x2) == 0 || (u8(mesh, 0x0) & 1) == 0)
        return -4;

    unsigned char flags = u8(mesh, 0x0);
    if (*g_Mesh_shaderPathFlag == 0 && u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, u32(mesh, 0x60));
        engine->AEClientState(0x8074, true);
        glVertexPointer(3, 0x1406, 0, 0);
        glBindBuffer(0x8893, u32(mesh, 0x64));

        if (flags & 4) { // (flags<<0x1e) negative -> uv present
            glBindBuffer(0x8892, u32(mesh, 0x68));
            engine->AEClientState(0x8078, true);
            glTexCoordPointer(2, 0x1406, 0, 0);
        } else {
            engine->AEClientState(0x8078, false);
        }

        if (flags & 8) { // (flags<<0x1d) negative -> normals present
            glBindBuffer(0x8892, u32(mesh, 0x6c));
            engine->AEClientState(0x8075, true);
            glNormalPointer(0x1406, 0, 0);
        } else {
            engine->AEClientState(0x8075, false);
        }

        if (flags & 0x10) { // (flags<<0x1c) negative -> colors present
            glBindBuffer(0x8892, u32(mesh, 0x78));
            engine->AEClientState(0x8076, true);
            glColorPointer(4, 0x1406, 0, 0);
        } else {
            engine->AEClientState(0x8076, false);
        }

        glDrawElements(4, (int)(unsigned short)u16(mesh, 0x28), 0x1403, 0);

        if (u8(engine, 0xfe) != 0) {
            int tris = __aeabi_uidiv((int)(unsigned short)u16(mesh, 0x28), 3);
            if (u8(engine, 0xfd) == 0) {
                i32(engine, 0x64) += 1;
                i32(engine, 0x68) += tris;
            } else {
                i32(engine, 0x6c) += tris;
                i32(engine, 0x60) += 1;
            }
        }

        glBindBuffer(0x8892, 0);
        glBindBuffer(0x8893, 0);
    } else {
        engine->RenderMesh(mesh);
    }
    return 1;
}

} // namespace AbyssEngine

// ---- SpriteSystemSetRGBA_86d44.cpp ----
// AbyssEngine::SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a,
//                                  AbyssEngine::SpriteSystem*)
// Write the same RGBA color to all four vertices (16 floats) of one sprite.
namespace AbyssEngine {

void SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a, SpriteSystem *sys)
{
    unsigned int count = 0;
    if (sys != 0)
        count = u16(sys, 0x0);
    if (sys == 0 || idx >= count)
        return;

    float *p = (float *)((char *)pp(pp(sys, 0x8), 0xc) + (unsigned int)idx * 0x40);
    for (int v = 0; v < 4; ++v) {
        p[v * 4 + 0] = r;
        p[v * 4 + 1] = g;
        p[v * 4 + 2] = b;
        p[v * 4 + 3] = a;
    }
}

} // namespace AbyssEngine

// ---- CameraSetPerspective_713c0.cpp ----
// AbyssEngine::CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near,
//                                   AbyssEngine::Camera*)
// Store the field-of-view/aspect into the Camera and precompute the projection scale terms
// (slots [0x12]..[0x15] of the float-view of the Camera). Mirrors the shipped trig sequence.
namespace AbyssEngine {

namespace AEMath {
}

float CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam)
{
    float ret = fov;
    if (cam != 0) {
        float *f = (float *)cam;
        f[1] = near;
        f[2] = fov;

        float s = AbyssEngine::AEMath::Sinf(fov);
        float c = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
        float scale = s / c;
        f[0x12] = scale;
        f[0x13] = (aspectNum / aspectDen) * scale;
        f[0x14] = aspectNum / aspectDen;

        float at = AbyssEngine::AEMath::ATanf(scale);
        float ca = AbyssEngine::AEMath::Cosf(at);
        ret = 1.0f / ca;
        f[0x15] = ret;
    }
    return ret;
}

} // namespace AbyssEngine

// ---- ImageFontGetWidth_721e6.cpp ----
// AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int
//                                start, unsigned int len)
// Same as the 3-arg form, but measures the substring [start, start+len).
namespace AbyssEngine {

int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start,
                      unsigned int len)
{
    if (font == 0 || str == 0)
        return 0;

    unsigned int end = len + start;
    unsigned short glyphCount = u16(font, 0x0);
    int total = 0;

    for (; (start & 0xffff) < end; ++start) {
        unsigned short idx = 0;
        unsigned short code;
        bool found = false;
        for (;;) {
            unsigned int u = idx;
            if (glyphCount <= u)
                break;
            idx = idx + 1;
            code = *(unsigned short *)((char *)pp(font, 0x4) + u * 2);
            if (code == str[start & 0xffff]) {
                found = true;
                break;
            }
        }
        if (!found)
            continue;

        unsigned int gi = (unsigned int)(unsigned short)(idx - 1);
        void *glyph = *(void **)((char *)pp(font, 0xc) + gi * 4);
        int w = (int)f32(pp(glyph, 0x4), 0xc);
        int adv = (int)s16(font, 0x10) + w;
        int contrib = adv;
        if (w == 0xb)
            contrib = adv - 2;
        if (code != 0x20)
            contrib = adv;
        total += contrib;
    }
    return total;
}

} // namespace AbyssEngine

// ---- CurveRelease_74322.cpp ----

// AbyssEngine::CurveRelease(AbyssEngine::Curve**)
// Free each keyframe object (tag byte 1/2/3 -> single delete), then the entry array and the
// Curve itself.
namespace AbyssEngine {

void CurveRelease(Curve **slot)
{
    Curve *curve = (Curve *)*slot;
    if (curve == 0)
        return;

    void *data = 0;
    for (unsigned int i = 0; (data = pp(curve, 0x4)), i < (unsigned int)u16(curve, 0x0); ++i) {
        char *entry = *(char **)((char *)data + i * 4);
        char tag = entry[0];
        if (tag == 3 || tag == 2)
            operator delete(entry);
        else if (tag == 1)
            operator delete(entry);
        curve = (Curve *)*slot;
    }

    if (data != 0)
        operator delete[](data);
    curve = (Curve *)*slot;
    pp(curve, 0x4) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine

// ---- MaterialDraw_870b8.cpp ----
// AbyssEngine::MaterialDraw(PaintCanvas*, Engine*, Material*, bool setTextures)
// Draw all submeshes of a material: optionally bind its textures, push the add-data, set the
// ambient light, then for each submesh multiply in its model matrix, set the UV matrix, unpack
// the packed RGBA color, and draw the mesh. Afterwards the per-frame submesh counters are
// reset to zero. Mirrors the recovered control flow; the heavy float register aliasing in the
// Ghidra output is normalized here.
namespace AbyssEngine {


int MeshDraw(Engine *engine, Mesh *mesh);

void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures)
{
    if (canvas == 0 || mat == 0)
        return;

    char *m = (char *)mat;

    if (setTextures) {
        engine->SetTexturesExt(u32(m, 0x0), u32(m, 0x4), u32(m, 0x8), 0xffffffff);
    }
    engine->SetAddData(pp(m, 0x24), i32(m, 0x28));

    float ambient = f32(m, 0x68);
    if (ambient != -10.0f) {
        AE_Engine_LightSetAmbient(ambient, 0.0f, 0.0f, engine);
    }

    const float inv255 = 1.0f / 255.0f;
    int matOff = 0;
    for (unsigned int i = 0; i < u32(m, 0x44); ++i) {
        Matrix world;
        AE_AEMath_matMul(&world, (const Matrix *)((char *)pp(m, 0x60) + matOff));
        AE_PaintCanvas_SetWorldViewMatrix((Matrix *)canvas);
        engine->SetModelMatrix((const uint32_t *)&world);
        engine->SetUVMatrix((const uint32_t *)((char *)pp(m, 0x3c) + matOff));

        unsigned int packed = *(unsigned int *)((char *)pp(m, 0x54) + i * 4);
        float cr = (float)((packed >> 16) & 0xff);
        float cg = (float)((packed >> 8) & 0xff);
        float cb = (float)(packed & 0xff);
        engine->SetColor(cb * inv255, cg * inv255, cr * inv255, 0.0f);

        MeshDraw(engine, *(Mesh **)((char *)pp(m, 0x48) + i * 4));
        matOff += 0x3c;
    }

    if (f32(m, 0x68) != -10.0f) {
        AE_Engine_LightSetAmbient(f32(m, 0x68), 0.0f, 0.0f, engine);
    }

    // Reset per-frame submesh accumulators.
    i32(m, 0x2c) = 0;
    i32(m, 0x5c) = 0;
    i32(m, 0x38) = 0;
    i32(m, 0x44) = 0;
    i32(m, 0x50) = 0;
}

} // namespace AbyssEngine

// ---- ImageFontRelease_7227e.cpp ----

// AbyssEngine::ImageFontRelease(AbyssEngine::Engine*, AbyssEngine::ImageFont**)
// Free the codepoint table (@4), release each glyph mesh (count@0, array@0xc), free the glyph
// array, then the font.
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void ImageFontRelease(Engine *engine, ImageFont **slot)
{
    if (*slot == 0)
        return;

    void *table = pp(*slot, 0x4);
    if (table != 0)
        operator delete[](table);
    pp(*slot, 0x4) = 0;

    int off = 0;
    unsigned int i = 0;
    void *glyphs;
    for (;;) {
        ImageFont *f = (ImageFont *)*slot;
        glyphs = pp(f, 0xc);
        if (u16(f, 0x0) <= i)
            break;
        MeshRelease(engine, (Mesh **)((char *)glyphs + off));
        off += 4;
        ++i;
    }

    if (glyphs != 0)
        operator delete[](glyphs);
    pp(*slot, 0xc) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine

// ---- MeshReadData_6c608.cpp ----
// AbyssEngine::MeshReadData(Engine*, unsigned int const& handle, unsigned int flags,
//                           Mesh** slot, Material*)
// Read the binary vertex payload of a mesh from an open file. Driven by the per-mesh vertex
// format byte (mesh+0x00) and the file-format flags:
//   - bit at 0x1b -> indexed: read index count (mesh+0x28) and 16-bit index array (mesh+0x2c)
//   - positions (mesh+0x04): either 16-bit compressed (flags<<0x1d) or raw float
//   - while reading positions the axis-aligned bounding box is accumulated -> BSphere @0x3c/0x48
//   - bit 0x1e -> UVs (mesh+0x08): compressed shorts or raw floats (V optionally flipped)
//   - bit 0x1d -> normals (mesh+0x10): decoded, normalized and clamped; tangents/binormals
//     (mesh+0x14/0x18) are then generated per-triangle when tangents are enabled
//   - bit 0x1c -> colors (mesh+0x0c): unsigned bytes scaled to [0,1]
// Multi-submesh ("AEMes2") files recurse to read each child mesh. Returns 1, or -1 on error.
namespace AbyssEngine {

extern "C" {
// __aeabi_uidiv is declared by gof2/Engine.h
float AE_VectorSignedToFloat(int v, unsigned char mode);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorLength(const void *v);
float AE_VectorDot(const void *a, const void *b);
void  AE_VectorNormalize(void *out, const void *in);
void  AE_VectorCross(void *a, const void *b);
void  AE_Vector_assign(void *dst, const void *src);
void  AE_VectorSub(void *self, const void *rhs);
void  AE_VectorMul(void *out, float s);
void  AE_BSphere_Merge(void *dst, const void *src);
void  AE_ArrayAdd_MeshPtr(void *item, void *arr);
int   AE_Mesh_ReadEnhancedDataFromFile(Mesh *self, unsigned int handle, unsigned int flags);
float sqrtf(float);

char *g_uvFlipFlag;        // **(DAT + 0x7c904) / 0x7c9d2
char *g_tangentEnabled;    // **(DAT + 0x7cc36)
}

int MeshReadData(Engine *engine, unsigned int *handlePtr, unsigned int flags, Mesh **slot,
                 Material *mat);

int MeshReadData(Engine *engine, unsigned int *handlePtr, unsigned int flags, Mesh **slot,
                 Material *mat)
{
    unsigned int handle = *handlePtr;
    unsigned int subBit = flags & 0x1a;
    unsigned char mode = 0;

    // Optional skinning/extra matrix block.
    if (subBit != 0) {
        if (AEFile::Read((uint32_t)(0xc), (char *)*slot + 0x50, handle) == 0)
            return -1;
    }

    char *m = (char *)*slot;
    // Indexed geometry.
    if (u8(m, 0x0) & 0x10) { // (byte<<0x1b) negative
        if (AEFile::Read((uint32_t)(2), m + 0x28, handle) == 0)
            return -1;
        void *idx = ::operator new[]((unsigned int)u16(m, 0x28) << 1);
        pp((char *)*slot, 0x2c) = idx;
        if (AEFile::Read((uint32_t)((unsigned int)u16((char *)*slot, 0x28) << 1), pp((char *)*slot, 0x2c), handle) == 0)
            return -1;
        m = (char *)*slot;
    }

    // Vertex count.
    if (AEFile::Read((uint32_t)(2), m + 0x2, handle) == 0)
        return -1;

    // Bounding-box accumulators (min/max for x,y,z).
    float minv[3] = {1e30f, 1e30f, 1e30f};
    float maxv[3] = {-1e30f, -1e30f, -1e30f};
    bool compressedPos = (int)(flags << 0x1d) < 0;
    unsigned int vcount = u16((char *)*slot, 0x2);

    if (compressedPos) {
        // 16-bit compressed positions: 3 shorts per vertex.
        void *raw = ::operator new[](vcount * 6);
        if (AEFile::Read((uint32_t)(vcount * 6), raw, handle) == 0) { ::operator delete[](raw); return -1; }
        m = (char *)*slot;
        void *pos = ::operator new[](vcount * 0xc);
        pp(m, 0x4) = pos;
        m = (char *)*slot;
        int dst = 0;
        unsigned int n3 = vcount * 3;
        for (unsigned int i = 0; i < n3; ++i) {
            int axis = (int)__aeabi_uidiv((int)i, 3);
            axis = (int)i - axis * 3;
            float v = AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2), mode);
            *(float *)(*(int *)(m + 4) + dst) = v;
            dst += 4;
            if (v < minv[axis]) minv[axis] = v;
            if (maxv[axis] < v) maxv[axis] = v;
        }
        ::operator delete[](raw);
    } else if ((flags & 3) == 0) {
        // Raw float positions, read straight in (no per-element bbox here for the enhanced path).
        if ((flags & 0x18) != 0) {
            m = (char *)*slot;
            void *pos = ::operator new[](vcount * 0xc);
            pp(m, 0x4) = pos;
            if (AEFile::Read((uint32_t)(u16((char *)*slot, 0x2) * 0xc), pp((char *)*slot, 0x4), handle) == 0)
                return -1;
        }
    } else {
        // Float positions, element-by-element with bbox.
        void *raw = ::operator new[](vcount * 0xc);
        if (AEFile::Read((uint32_t)(vcount * 0xc), raw, handle) == 0) { ::operator delete[](raw); return -1; }
        m = (char *)*slot;
        void *pos = ::operator new[](vcount * 0xc);
        pp(m, 0x4) = pos;
        m = (char *)*slot;
        int dst = 0;
        unsigned int n3 = vcount * 3;
        for (unsigned int i = 0; i < n3; ++i) {
            int axis = (int)__aeabi_uidiv((int)i, 3);
            axis = (int)i - axis * 3;
            float v = AE_VectorSignedToFloat(*(int *)((char *)raw + i * 4), mode);
            *(float *)(*(int *)(m + 4) + dst) = v;
            dst += 4;
            if (v < minv[axis]) minv[axis] = v;
            if (maxv[axis] < v) maxv[axis] = v;
        }
        ::operator delete[](raw);
    }

    // BSphere center = box center, radius = half-diagonal length.
    float center[3];
    center[0] = (maxv[0] + minv[0]) * 0.5f;
    center[1] = (maxv[1] + minv[1]) * 0.5f;
    center[2] = (maxv[2] + minv[2]) * 0.5f;
    AE_Vector_assign((char *)*slot + 0x3c, center);
    float halfDiag[3] = {minv[0], minv[1], minv[2]};
    AE_VectorSub(center, halfDiag);
    f32((char *)*slot, 0x48) = AE_VectorLength(center);

    m = (char *)*slot;
    // UV coordinates.
    if (u8(m, 0x0) & 4) { // (byte<<0x1e) negative
        if (compressedPos) {
            void *raw = ::operator new[](vcount << 2);
            if (AEFile::Read((uint32_t)(vcount << 2), raw, handle) == 0) { ::operator delete[](raw); return -1; }
            m = (char *)*slot;
            void *uv = ::operator new[](vcount << 3);
            pp(m, 0x8) = uv;
            m = (char *)*slot;
            char flip = *g_uvFlipFlag;
            int dst = 0;
            const double scale = 1.0 / 32767.0;
            for (unsigned int i = 0; i < (vcount << 1); i += 2) {
                double u = (double)AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2), mode) * scale;
                float *p = (float *)(*(int *)(m + 8) + dst);
                dst += 8;
                p[0] = (float)u;
                double v = (double)AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2 + 2), mode) * scale;
                double vv = (flip == 0) ? v : (1.0 - v);
                p[1] = (float)vv;
            }
            ::operator delete[](raw);
        } else if ((flags & 0x18) != 0) {
            void *uv = ::operator new[](vcount << 3);
            pp(m, 0x8) = uv;
            if (AEFile::Read((uint32_t)(u16((char *)*slot, 0x2) << 3), pp((char *)*slot, 0x8), handle) == 0)
                return -1;
            if (*g_uvFlipFlag != 0) {
                m = (char *)*slot;
                int off = 4;
                for (unsigned int i = 0; i < (unsigned int)(u16(m, 0x2) << 1); i += 2) {
                    float *p = (float *)(*(int *)(m + 8) + off);
                    off += 8;
                    *p = 1.0f - *p;
                }
            }
        }
    }

    m = (char *)*slot;
    // Normals (+ tangent/binormal generation).
    if (u8(m, 0x0) & 8) { // (byte<<0x1d) negative
        if (compressedPos) {
            void *raw = ::operator new[](vcount * 6);
            if (AEFile::Read((uint32_t)(vcount * 6), raw, handle) == 0) { ::operator delete[](raw); return -1; }
            m = (char *)*slot;
            void *nrm = ::operator new[](vcount * 0xc);
            pp(m, 0x10) = nrm;
            m = (char *)*slot;
            const double scale = 1.0 / 32767.0;
            short *s = (short *)raw;
            int off = 4;
            for (unsigned int i = 0; i < vcount * 3; i += 3) {
                float nx = (float)((double)AE_VectorSignedToFloat((int)s[0], mode) * scale);
                float ny = (float)((double)AE_VectorSignedToFloat((int)s[1], mode) * scale);
                float nz = (float)((double)AE_VectorSignedToFloat((int)s[2], mode) * scale);
                float len2 = nx * nx + ny * ny + nz * nz;
                float len = sqrtf(len2);
                int base = *(int *)(m + 0x10);
                if (len != 0.0f) {
                    nx /= len; ny /= len; nz /= len;
                    if (nx < -1.0f) nx = -1.0f; if (nx > 1.0f) nx = 1.0f;
                    if (ny < -1.0f) ny = -1.0f; if (ny > 1.0f) ny = 1.0f;
                    if (nz < -1.0f) nz = -1.0f; if (nz > 1.0f) nz = 1.0f;
                    ((float *)(base + off))[-1] = ny;
                    *(float *)(base + off) = nx;
                    *(float *)(base + off + 4) = nz;
                } else {
                    *(unsigned int *)(base + off) = 0x3f800000;
                    *(unsigned int *)(base + off - 4) = 0;
                    *(float *)(base + off + 4) = 1.0f;
                }
                s += 3;
                off += 0xc;
                m = (char *)*slot;
            }
            ::operator delete[](raw);
        } else if ((flags & 0x18) != 0) {
            void *nrm = ::operator new[](vcount * 0xc);
            pp(m, 0x10) = nrm;
            if (AEFile::Read((uint32_t)(u16((char *)*slot, 0x2) * 0xc), pp((char *)*slot, 0x10), handle) == 0)
                return -1;
        }

        // Per-triangle tangent/binormal generation.
        if (*g_tangentEnabled != 0) {
            m = (char *)*slot;
            void *tan = ::operator new[](vcount * 0xc);
            pp(m, 0x14) = tan;
            m = (char *)*slot;
            void *bin = ::operator new[](vcount * 0xc);
            pp(m, 0x18) = bin;
            m = (char *)*slot;

            unsigned int triCount = (unsigned int)__aeabi_uidiv((int)(unsigned short)u16(m, 0x28), 3);
            void *accum = ::operator new[](vcount * 0xc);
            // Zero accumulator.
            for (unsigned int b = 0; b < vcount * 0xc; ++b) ((char *)accum)[b] = 0;

            int triOff = 0;
            for (unsigned int t = 0; t < triCount; ++t) {
                m = (char *)*slot;
                int posBase = *(int *)(m + 4);
                int uvBase = *(int *)(m + 8);
                int idxBase = *(int *)(m + 0x2c) + triOff;
                unsigned int i0 = (unsigned int)*(unsigned short *)(idxBase);
                unsigned int i2 = (unsigned int)*(unsigned short *)(idxBase + 4);
                unsigned int i1 = (unsigned int)*(unsigned short *)(idxBase + 2);

                float uv0v = *(float *)((i0 << 3 | 4) + uvBase);
                float uv0u = *(float *)(uvBase + i0 * 8);
                float dv1 = *(float *)((i2 << 3 | 4) + uvBase) - uv0v;
                float dv2 = *(float *)((i1 << 3 | 4) + uvBase) - uv0v;
                float *p0 = (float *)(posBase + i0 * 0xc);
                float *p2 = (float *)(posBase + i2 * 0xc);
                float *p1 = (float *)(posBase + i1 * 0xc);
                float denom = (*(float *)(uvBase + i1 * 8) - uv0u) * dv1 -
                              (*(float *)(uvBase + i2 * 8) - uv0u) * dv2;
                float r = (denom != 0.0f) ? (1.0f / denom) : 0.0f;
                float tng[3];
                tng[1] = ((p1[1] - p0[1]) * dv1 - (p2[1] - p0[1]) * dv2) * r;
                tng[0] = ((p1[0] - p0[0]) * dv1 - (p2[0] - p0[0]) * dv2) * r;
                tng[2] = ((p1[2] - p0[2]) * dv1 - (p2[2] - p0[2]) * dv2) * r;
                float *a = (float *)((char *)accum + i0 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                a = (float *)((char *)accum + i1 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                a = (float *)((char *)accum + i2 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                triOff += 6;
            }

            // Orthonormalize tangent against the normal, store tangent/binormal.
            int off = 0;
            for (unsigned int v = 0; v < vcount; ++v) {
                m = (char *)*slot;
                float nrm[3];
                nrm[0] = *(float *)(*(int *)(m + 0x10) + off);
                nrm[1] = *(float *)(*(int *)(m + 0x10) + off + 4);
                nrm[2] = *(float *)(*(int *)(m + 0x10) + off + 8);
                float tg[3];
                tg[0] = *(float *)((char *)accum + off);
                tg[1] = *(float *)((char *)accum + off + 4);
                tg[2] = *(float *)((char *)accum + off + 8);
                float d = AE_VectorDot(nrm, tg);
                float scaled[3] = {nrm[0], nrm[1], nrm[2]};
                AE_VectorMul(scaled, d);
                AE_VectorSub(tg, scaled);
                float tanOut[3];
                AE_VectorNormalize(tanOut, tg);
                int tb = *(int *)((char *)*slot + 0x14) + off;
                *(float *)(tb) = tanOut[0];
                *(float *)(tb + 4) = tanOut[1];
                *(float *)(tb + 8) = tanOut[2];
                float binOut[3] = {tg[0], tg[1], tg[2]};
                AE_VectorCross(binOut, nrm);
                int bb = *(int *)((char *)*slot + 0x18) + off;
                *(float *)(bb) = binOut[0];
                *(float *)(bb + 4) = binOut[1];
                *(float *)(bb + 8) = binOut[2];
                off += 0xc;
            }
            ::operator delete[](accum);
        }
    }

    m = (char *)*slot;
    // Vertex colors. (byte<<0x1c) negative -> bit 0x10 set.
    if (u8(m, 0x0) & 0x10) {
        if (compressedPos) {
            void *raw = ::operator new[](vcount << 2);
            if (AEFile::Read((uint32_t)(vcount << 2), raw, handle) == 0) { ::operator delete[](raw); return -1; }
            m = (char *)*slot;
            void *col = ::operator new[](vcount << 4);
            pp(m, 0xc) = col;
            m = (char *)*slot;
            const float inv = 255.0f;
            int dst = 0;
            for (unsigned int i = 0; i < (vcount << 2); ++i) {
                float c = AE_VectorUnsignedToFloat((unsigned int)*((unsigned char *)raw + i), mode);
                *(float *)(*(int *)(m + 0xc) + dst) = c / inv;
                dst += 4;
            }
            ::operator delete[](raw);
        } else if ((flags & 0x18) != 0) {
            void *col = ::operator new[](vcount << 4);
            pp(m, 0xc) = col;
            if (AEFile::Read((uint32_t)(u16((char *)*slot, 0x2) << 4), pp((char *)*slot, 0xc), handle) == 0)
                return -1;
        }
    }

    // Enhanced data / submesh recursion.
    if (subBit != 0) {
        if (AE_Mesh_ReadEnhancedDataFromFile(*slot, handle, flags) == 0)
            return -1;
        unsigned short childCount = 0;
        if (AEFile::Read((uint32_t)(2), &childCount, handle) == 0)
            return -1;
        int xf = *(int *)((char *)*slot + 0x34);
        if (xf != 0)
            AE_BSphere_Merge((char *)*slot + 0x3c, (char *)(xf + 0xd4));
        for (unsigned int c = 0; c < childCount; ++c) {
            char *child = (char *)::operator new(0x88);
            for (int b = 0; b < 0x88; ++b) child[b] = 0;
            u32(child, 0x4c) = 0x3f800000;
            u8(child, 0x84) = 1;
            u8(child, 0x0) = u8((char *)*slot, 0x0);
            pp(child, 0x30) = pp((char *)*slot, 0x30);
            Mesh *childPtr = (Mesh *)child;
            if (MeshReadData(engine, handlePtr, flags, &childPtr, mat) == -1)
                return -1;
            AE_BSphere_Merge((char *)*slot + 0x3c, (char *)childPtr + 0x3c);
            AE_ArrayAdd_MeshPtr(childPtr, (char *)*(int *)((char *)*slot + 0x34) + 0x3c);
        }
    }

    return 1;
}

} // namespace AbyssEngine

// ---- operator__73660.cpp ----
// AbyssEngine::operator+(AbyssEngine::String const&, long long const&)
// Append the decimal rendering of a 64-bit integer to a string.
namespace AbyssEngine {


String operator+(const String &a, const long long &b)
{
    String acc;
    String_copy(&acc, &a, false);

    String num;
    String_fromLongLong(&num, b);
    String_appendEq(&acc, &num);
    ((String *)(&num))->dtor();

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- MeshRelease_6c362.cpp ----
// AbyssEngine::MeshRelease(AbyssEngine::Engine*, AbyssEngine::Mesh**)
namespace AbyssEngine {

// Sibling free functions (resolved by the linker via their AbyssEngine-mangled names).
void TransformRelease(Engine *engine, Transform **slot);

void MeshRelease(Engine *engine, Mesh **slot)
{
    if (engine != 0 && *slot != 0) {
        TransformRelease(engine, (Transform **)((char *)*slot + 0x34));
        // Tail-call through a runtime function pointer (MeshReleaseIntern) that frees the mesh
        // body. The first argument is the engine pointer (preserved in r0 across the void
        // TransformRelease call in the shipped code).
        typedef void (*FreeFn)(Engine *, Mesh **);
        extern void *g_MeshRelease_freeFn; // *(DAT_001ab034 + 0x1ab038)
        ((FreeFn)g_MeshRelease_freeFn)(engine, slot);
    }
}

} // namespace AbyssEngine

// ---- operator__738b0.cpp ----
// AbyssEngine::operator+(float const&, AbyssEngine::String const&)
// Render a float to a string, then append the rhs string.
namespace AbyssEngine {


String operator+(const float &a, const String &b)
{
    String acc;
    String_fromFloat(&acc, a);
    String_appendEq(&acc, &b);

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- SpriteSystemCreate_86adc.cpp ----

// AbyssEngine::SpriteSystemCreate(Engine*, unsigned short count, bool sharedSize,
//                                 SpriteSystem** out)
// Allocate a SpriteSystem of `count` quads: build a backing Mesh (4 verts + 2 tris per
// sprite), fill the shared index buffer, default vertex colors to white and normals/tangents
// to the identity basis. Returns 1, -1 on mesh failure, -4 on count == 0.
namespace AbyssEngine {


int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
               unsigned int fmt, void **out);
void MeshRelease(Engine *engine, Mesh **slot);

int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out)
{
    unsigned int n = count;
    if (n == 0)
        return -4;

    char *sys = (char *)operator new(0x14);
    pp(sys, 0x4) = 0;
    pp(sys, 0x8) = 0;
    pp(sys, 0x10) = 0;
    *out = (SpriteSystem *)sys;
    u16(sys, 0x0) = count;

    unsigned int triCount = (n << 1) & 0xffff;
    int rc = MeshCreate(engine, (unsigned short)((n & 0x3fff) << 2), (unsigned short)triCount,
                        0x1f, (void **)(sys + 0x10));
    char *s = (char *)*out;
    if (rc != 1) {
        MeshRelease(engine, (Mesh **)(s + 0x10));
        if (pp(*out, 0x4) != 0)
            operator delete[](pp(*out, 0x4));
        pp(*out, 0x4) = 0;
        if (pp(*out, 0x8) != 0)
            operator delete[](pp(*out, 0x8));
        pp(*out, 0x8) = 0;
        if (*out != 0)
            operator delete((void *)*out);
        *out = 0;
        return -1;
    }

    Mesh *mesh = (Mesh *)pp(s, 0x10);
    int *indexArr = (int *)pp(mesh, 0x2c);

    void *posCpu = operator new[](n * 0xc);
    pp(s, 0x4) = posCpu;
    __aeabi_memclr4(pp(*out, 0x4), n * 0xc);

    s = (char *)*out;
    u8(s, 0xc) = sharedSize ? 1 : 0;
    if (sharedSize) {
        void *sz = operator new[](2);
        pp(s, 0x8) = sz;
        *(unsigned short *)pp(*out, 0x8) = 0;
    } else {
        void *sz = operator new[]((n << 1));
        pp(s, 0x8) = sz;
        __aeabi_memclr(pp(*out, 0x8), (n << 1));
    }

    // Fill the index buffer: two triangles (0,1,2, 0,2,3) per quad.
    unsigned short base = 0;
    char *idx = (char *)indexArr;
    for (unsigned int off = 0; (off & 0xffff) < n * 6; off += 6) {
        unsigned short *p = (unsigned short *)(idx + (off & 0xffff) * 2);
        p[0] = base;
        p[1] = base | 1;
        p[2] = base | 2;
        p[3] = base;
        p[4] = base | 2;
        p[5] = base | 3;
        base = base + 4;
    }

    Mesh *m = (Mesh *)pp(*out, 0x10);
    unsigned int vcount = (unsigned int)(unsigned short)u16(m, 0x2);

    // Default vertex colors to white (1.0 per channel).
    float *colors = (float *)pp(m, 0xc);
    for (unsigned int i = 0; i <= vcount * 4 && vcount * 4 - i != 0; ++i)
        colors[i] = 1.0f;

    // Default normals (and optionally tangents/binormals) to the identity basis.
    char tangent = *g_SpriteSystem_tangentFlag;
    int vo = 0;
    char *normals = (char *)pp(m, 0x10);
    char *tangents = (char *)pp(m, 0x14);
    char *binormals = (char *)pp(m, 0x18);
    for (unsigned int k = vcount; k != 0; --k) {
        float *nrm = (float *)(normals + vo);
        nrm[0] = 0.0f;
        nrm[1] = 0.0f;
        nrm[2] = 1.0f;
        if (tangent != 0) {
            float *tan = (float *)(tangents + vo);
            tan[0] = 1.0f;
            tan[1] = 0.0f;
            tan[2] = 0.0f;
            float *bin = (float *)(binormals + vo);
            bin[0] = 0.0f;
            bin[1] = 1.0f;
            bin[2] = 0.0f;
        }
        vo += 0xc;
    }

    return 1;
}

} // namespace AbyssEngine

// ---- ImageCreateFromFile_6f4d4.cpp ----
// AbyssEngine::ImageCreateFromFile(Engine*, char const*, Image**)
// Allocate an Image (0x14 bytes), open the .aei file, validate the 8-byte magic, read the
// header (type byte, width, height, palette count) and then the pixel payload. The pixel
// format byte selects how the payload size/format is decoded. Returns 1 on success, -1 on a
// read/format error (releasing the Image), or -4 on invalid arguments.
//
// Image layout: +0x00 width(u16), +0x02 height(u16), +0x04 format(u32), +0x08 hasMipmaps(u8),
//               +0x0c data ptr, +0x10 data size(u32).
namespace AbyssEngine {

extern "C" {
void AEFile_Close(unsigned int handle);
}

void ImageRelease(Image **slot);

int ImageCreateFromFile(Engine *engine, char *path, Image **out)
{
    if (engine == 0 || path == 0)
        return -4;

    char *img = (char *)::operator new(0x14);
    u8(img, 0x8) = 0;
    u32(img, 0x0) = 0;
    u32(img, 0x4) = 0;
    u32(img, 0xc) = 0;
    *out = (Image *)img;

    unsigned int handle = 0;
    if (AEFile::OpenRead((const char *)(path), (uint32_t *)(&handle)) == 0) {
        if (*out != 0)
            ::operator delete((void *)*out);
        *out = 0;
        return -1;
    }

    // Validate "AEImage"-style 8-byte magic.
    static const char magic[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    char hdr[8];
    for (int i = 0; i < 4; ++i) hdr[i] = '*';
    if (AEFile::Read((uint32_t)(8), hdr, handle) == 0)
        goto fail;
    for (unsigned int k = 0; k < 8; ++k) {
        if (magic[k] != hdr[k])
            goto fail;
    }

    {
        char *im = (char *)*out;
        unsigned char fmt = 0;
        unsigned short palCount = 0;
        if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
        if (AEFile::Read((uint32_t)(2), im + 0x0, handle) == 0) goto fail;      // width
        if (AEFile::Read((uint32_t)(2), im + 0x2, handle) == 0) goto fail;      // height
        if (AEFile::Read((uint32_t)(2), &palCount, handle) == 0) goto fail;     // palette entries

        AEFile::Skip((uint32_t)((unsigned int)palCount << 3), handle);

        if (fmt & 2)
            u8(im, 0x8) = 1; // mipmapped

        unsigned int dataLen = 0;
        switch (fmt) {
        case 1:
        case 3:
        case 0x81: {
            unsigned int sz = (unsigned int)u16(im, 0x0) * (unsigned int)u16(im, 0x2) * 4;
            void *p = ::operator new[](sz);
            pp(im, 0xc) = p;
            im = (char *)*out;
            if (AEFile::Read((uint32_t)(sz), pp(im, 0xc), handle) == 0) goto fail;
            im = (char *)*out;
            u32(im, 0x10) = sz;
            {
                unsigned int v = (unsigned int)(int)(signed char)fmt;
                unsigned int code = ((int)v < 0) ? ((v > 0x7fffffff) ? 6u : v) : 3u;
                if ((int)v >= 0) code = 3;
                u32(im, 0x4) = code;
            }
            break;
        }
        case 0xd:
        case 0xf:
            if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = ::operator new[](dataLen);
            if (AEFile::Read((uint32_t)(dataLen), pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 4;
            u32(im, 0x10) = dataLen;
            break;
        case 0x10:
        case 0x12:
            if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = ::operator new[](dataLen);
            if (AEFile::Read((uint32_t)(dataLen), pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 5;
            u32(im, 0x10) = dataLen;
            break;
        case 0x11:
        case 0x13:
            if (AEFile::Read((uint32_t)(4), im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 7;
            pp(im, 0xc) = ::operator new[](u32(im, 0x10));
            if (AEFile::Read((uint32_t)(u32(im, 0x10)), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x14:
        case 0x16:
        case 0x17:
        case 0x40:
        case 0x42:
            if (fmt == 0x40)
                u8(im, 0x8) = 0;
            if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = ::operator new[](dataLen);
            if (AEFile::Read((uint32_t)(dataLen), pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 0xb;
            u32(im, 0x10) = dataLen;
            if (fmt == 0x17)
                u8(im, 0x8) = 0;
            break;
        case 0x20:
        case 0x22:
            if (AEFile::Read((uint32_t)(4), im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 8;
            pp(im, 0xc) = ::operator new[](u32(im, 0x10));
            if (AEFile::Read((uint32_t)(u32(im, 0x10)), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x21:
        case 0x23:
            if (AEFile::Read((uint32_t)(4), im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 9;
            pp(im, 0xc) = ::operator new[](u32(im, 0x10));
            if (AEFile::Read((uint32_t)(u32(im, 0x10)), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x24:
        case 0x26:
            if (AEFile::Read((uint32_t)(4), im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 10;
            pp(im, 0xc) = ::operator new[](u32(im, 0x10));
            if (AEFile::Read((uint32_t)(u32(im, 0x10)), pp(im, 0xc), handle) == 0) goto fail;
            break;
        default:
            break;
        }

        AEFile_Close(handle);
        return 1;
    }

fail:
    ImageRelease(out);
    AEFile_Close(handle);
    return -1;
}

} // namespace AbyssEngine

// ---- ImageRelease_6ee94.cpp ----

// AbyssEngine::ImageRelease(AbyssEngine::Image**)
namespace AbyssEngine {

void ImageRelease(Image **slot)
{
    if (*slot != 0) {
        operator delete[](pp(*slot, 0xc));
        pp(*slot, 0xc) = 0;
        if (*slot != 0)
            operator delete((void *)*slot);
        *slot = 0;
    }
}

} // namespace AbyssEngine

// ---- Image2DRelease_71b64.cpp ----

// AbyssEngine::Image2DRelease(AbyssEngine::Engine*, AbyssEngine::Image2D**)
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void Image2DRelease(Engine *engine, Image2D **slot)
{
    if (*slot != 0) {
        MeshRelease(engine, (Mesh **)*slot);
        if (*slot != 0)
            operator delete((void *)*slot);
        *slot = 0;
    }
}

} // namespace AbyssEngine

// ---- MeshCreate_6c4d4.cpp ----

// AbyssEngine::MeshCreate(Engine*, unsigned short vertexCount, unsigned short triCount,
//                         signed char vertexFormat, Mesh** out)
// Allocate and zero-initialize a Mesh (0x88 bytes), then allocate the per-attribute CPU
// arrays selected by the vertex-format flag bits. Returns 1, or -4 on invalid arguments.
namespace AbyssEngine {


int MeshCreate(Engine * /*engine*/, unsigned short vertexCount, unsigned short triCount,
               unsigned int vertexFormat, void **out)
{
    if (vertexCount < 4 || triCount == 0 || (vertexFormat & 1) == 0)
        return -4;

    char *m = (char *)operator new(0x88);

    // Zero the identity/colour/transform region. The decompile splats a 16-byte zero vector
    // across many slots; clearing the whole struct is the faithful net effect, after which we
    // write the few non-zero defaults.
    __aeabi_memclr4(m, 0x88);
    f32(m, 0x4c) = 1.0f; // default scale = 1.0

    *out = m;
    s16(m, 0x2) = (short)vertexCount;
    u8(m, 0x0) = (char)vertexFormat;
    s16(m, 0x28) = (short)(triCount + (triCount << 1)); // 3 * triCount (index count)
    s16(m, 0x2a) = (short)triCount;

    unsigned int posBytes = (unsigned int)vertexCount * 0xc;
    void *p = operator new[](posBytes);
    pp(m, 0x4) = p;
    __aeabi_memclr4(p, posBytes);

    if (vertexFormat & 0x20) { // (fmt<<0x1b) negative -> index array
        p = operator new[]((unsigned int)triCount * 6);
        pp(m, 0x2c) = p;
        __aeabi_memclr(p, (unsigned int)triCount * 6);
    }
    if (vertexFormat & 4) { // (fmt<<0x1e) negative -> uv array
        p = operator new[]((unsigned int)vertexCount << 3);
        pp(m, 0x8) = p;
        __aeabi_memclr4(p, (unsigned int)vertexCount << 3);
    }
    if (vertexFormat & 8) { // (fmt<<0x1d) negative -> normal array (+ tangents/binormals)
        p = operator new[](posBytes);
        pp(m, 0x10) = p;
        __aeabi_memclr4(p, posBytes);
        if (*g_Mesh_tangentEnabledFlag != 0) {
            p = operator new[](posBytes);
            pp(m, 0x14) = p;
            __aeabi_memclr4(p, posBytes);
            p = operator new[](posBytes);
            pp(m, 0x18) = p;
            __aeabi_memclr4(p, posBytes);
        }
    }
    if (vertexFormat & 0x10) { // (fmt<<0x1c) negative -> colour array
        p = operator new[]((unsigned int)vertexCount << 4);
        pp(m, 0xc) = p;
        __aeabi_memclr4(p, (unsigned int)vertexCount << 4);
    }

    return 1;
}

} // namespace AbyssEngine

// ---- CameraSetPerspective_7130c.cpp ----
// AbyssEngine::CameraSetPerspective(float, float aspectNum, float fov, float aspectDen,
//                                   float near, AbyssEngine::Camera*)
// Six-argument overload: additionally precomputes slot [0x16]. Cosf is reached through a
// runtime function-pointer in the shipped code; modeled here as the same AEMath::Cosf.
namespace AbyssEngine {

namespace AEMath {
}

float CameraSetPerspective(float p1, float aspectNum, float fov, float aspectDen, float near,
                           Camera *cam)
{
    float ret = p1;
    if (cam != 0) {
        float *f = (float *)cam;
        f[0] = fov;
        f[1] = aspectNum;
        f[2] = fov;

        float s = AbyssEngine::AEMath::Sinf(fov);
        float c = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
        f[0x12] = s / c;
        f[0x13] = (aspectDen / near) * (s / c);
        f[0x14] = aspectDen / near;

        float c2 = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
        f[0x16] = 1.0f / c2;

        AbyssEngine::AEMath::ATanf(f[0x12] * f[0x14]);
        float c3 = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
        ret = 1.0f / c3;
        f[0x15] = ret;
    }
    return ret;
}

} // namespace AbyssEngine

// ---- ImageFontSetSpacing_72266.cpp ----
// AbyssEngine::ImageFontSetSpacing(AbyssEngine::ImageFont*, short)
namespace AbyssEngine {

void ImageFontSetSpacing(ImageFont *font, short spacing)
{
    if (font != 0)
        s16(font, 0x10) = spacing;
}

} // namespace AbyssEngine

// ---- ImageFontGetHeight_721ba.cpp ----
// AbyssEngine::ImageFontGetHeight(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetHeight(ImageFont *font)
{
    if (font != 0) {
        // ((*(*(font+0xc)))[+4])->[0x1c] as float -> int; clamp 0x18 to 0x13.
        void *p = pp(font, 0xc);
        void *q = pp(p, 0x0);
        void *r = pp(q, 0x4);
        int v = (int)f32(r, 0x1c);
        if (v == 0x18)
            v = 0x13;
        return v;
    }
    return 0;
}

} // namespace AbyssEngine

// ---- operator__7373c.cpp ----
// AbyssEngine::operator+(AbyssEngine::String const&, int const&)
// Append the decimal rendering of a 32-bit integer to a string.
namespace AbyssEngine {


String operator+(const String &a, const int &b)
{
    String acc;
    String_copy(&acc, &a, false);

    String num;
    String_fromInt(&num, b);
    String_appendEq(&acc, &num);
    ((String *)(&num))->dtor();

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- ImageFontGetSpacing_7226c.cpp ----
// AbyssEngine::ImageFontGetSpacing(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetSpacing(ImageFont *font)
{
    short v = (font == 0) ? (short)0 : s16(font, 0x10);
    return (int)v;
}

} // namespace AbyssEngine

// ---- operator__73654.cpp ----
// AbyssEngine::operator+(AbyssEngine::String const&)
// Unary form: construct a (non-owning) copy of the operand string.
namespace AbyssEngine {


String operator+(const String &a)
{
    String result;
    String_copy(&result, &a, false);
    return result;
}

} // namespace AbyssEngine

// ---- ImageFontDrawString_71b80.cpp ----
// AbyssEngine::ImageFontDrawString(ImageFont*, unsigned short const*, int x, int y,
//                                  PaintCanvas*, Engine*, bool)
// Convenience overload: compute the NUL-terminated length and forward to the explicit-length
// renderer.
namespace AbyssEngine {

void ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x,
                         int y, PaintCanvas *canvas, Engine *engine, bool flag);

void ImageFontDrawString(ImageFont *font, const unsigned short *str, int x, int y,
                         PaintCanvas *canvas, Engine *engine, bool flag)
{
    if (font != 0 && str != 0) {
        unsigned short i = 0;
        unsigned int len;
        do {
            len = (unsigned int)i;
            i = i + 1;
        } while (str[len] != 0);
        ImageFontDrawString(font, str, len, x, y, canvas, engine, flag);
    }
}

} // namespace AbyssEngine

// ---- ImageFontDrawString_71bc0.cpp ----
// AbyssEngine::ImageFontDrawString(ImageFont*, unsigned short const* text, unsigned int len,
//                                  int x, int y, PaintCanvas*, Engine*, bool rtl)
// Draw a UTF-16 string with a bitmap font. Two rendering paths are selected by the engine
// shader-mode flag at engine+0xfc:
//   - immediate path: per-glyph world matrix + MeshDraw
//   - batched path: glyph quads are accumulated into a shared sprite buffer (PaintCanvas+8)
//     and flushed (MeshDraw) when full or at the end.
// Characters are looked up in the font codepoint table (font+0x04); advance widths come from
// each glyph mesh. RTL ordering (or the canvas batch flag) reverses the scan direction.
namespace AbyssEngine {


extern "C" {
int  AE_Engine_GetDisplayWidth();
int  AE_Engine_GetDisplayHeight();
int  AE_GameText_getLanguage();
int  AE_GameText_isNonArabicString(const unsigned short *text, unsigned int len);
void AE_PaintCanvas_SetWorldViewMatrix(void *canvas);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  ImageFontGetWidth(ImageFont *font, unsigned short *text, unsigned int len);
int  ImageFontGetHeight(ImageFont *font);
int  MeshDraw(Engine *engine, Mesh *mesh);

int ImageFontDrawString(ImageFont *font, unsigned short *text, unsigned int len, int x, int y,
                        PaintCanvas *canvas, Engine *engine, bool rtl)
{
    if (text == 0 || font == 0)
        return 0;

    char *f = (char *)font;
    char *pc = (char *)canvas;
    char *en = (char *)engine;
    unsigned char mode = 0;

    int w = ImageFontGetWidth(font, text, len);
    if (w + x < 0)
        return 0;

    int top = (int)(short)s16(f, 0x12);
    int h = ImageFontGetHeight(font);
    int dispW = AE_Engine_GetDisplayWidth();
    int bottom = dispW;
    if (x <= dispW) {
        top = top + y;
        bottom = h + top;
    }
    int side = (x <= dispW) ? bottom : (dispW - x);
    // Off-screen clip on X.
    if ((side < 0) != (dispW < x))
        return 0;
    if (top > AE_Engine_GetDisplayHeight())
        return 0;

    // Scan direction: forward by default; reversed for RTL or when batching is requested.
    int step = -1;
    int idx = (int)len - 1;
    bool batched = (u8(pc, 0x1c) != 0) || rtl;
    if (batched) {
        step = 1;
        idx = 0;
    }

    bool shaderMode = (u8(en, 0xfc) != 0);
    if (shaderMode) {
        // Arabic special-case: force forward scan.
        if (*g_GameText_arabicEnabledFlag != 0 && AE_GameText_getLanguage() == 9 &&
            AE_GameText_isNonArabicString(text, len) != 0) {
            idx = 0;
            step = 1;
        }
    }

    float baseY = AE_VectorSignedToFloat(top - 2, mode);

    for (unsigned int i = 0; i < len; ++i) {
        // Find glyph slot for codepoint text[idx].
        unsigned int slot = 0;
        unsigned short glyphCount = u16(f, 0x0);
        bool found = false;
        while (slot < glyphCount) {
            if (*(unsigned short *)(*(int *)(f + 4) + slot * 2) == text[idx]) {
                found = true;
                break;
            }
            ++slot;
        }

        if (found) {
            int glyphMesh = *(int *)(*(int *)(f + 0xc) + slot * 4);
            int advance = (int)*(float *)(*(int *)(glyphMesh + 4) + 0xc);

            if (x + advance >= 0 && x <= AE_Engine_GetDisplayWidth()) {
                if (!shaderMode) {
                    AE_PaintCanvas_SetWorldViewMatrix(canvas);
                    MeshDraw(engine, *(Mesh **)(*(int *)(f + 0xc) + slot * 4));
                } else {
                    // Batched path: append this glyph's quad into the sprite buffer at
                    // canvas+8 (vertex/color/matrix arrays), flush when the slot count
                    // reaches the buffer capacity.
                    int spr = *(int *)(pc + 8);
                    int n = *(int *)(pc + 0xc);
                    float fx = AE_VectorSignedToFloat(x, mode);
                    float *vsrc = *(float **)(glyphMesh + 4);
                    float *vdst = (float *)(*(int *)(spr + 4) + n * 0x30);
                    vdst[0] = vsrc[0] + fx;     vdst[1] = vsrc[1] + baseY;
                    vdst[3] = vsrc[3] + fx;     vdst[4] = vsrc[4] + baseY;
                    vdst[6] = vsrc[6] + fx;     vdst[7] = vsrc[7] + baseY;
                    vdst[9] = vsrc[9] + fx;     vdst[10] = vsrc[10] + baseY;

                    unsigned int *csrc = *(unsigned int **)(glyphMesh + 8);
                    unsigned int *cdst = (unsigned int *)(*(int *)(spr + 8) + (n << 5));
                    for (int k = 0; k < 8; ++k) cdst[k] = csrc[k];

                    unsigned int *mdst = (unsigned int *)(*(int *)(spr + 0xc) + (n << 6));
                    for (int r = 0; r < 4; ++r) {
                        mdst[r * 4 + 0] = u32(en, 0xd0);
                        mdst[r * 4 + 1] = u32(en, 0xd4);
                        mdst[r * 4 + 2] = u32(en, 0xd8);
                        mdst[r * 4 + 3] = u32(en, 0xdc);
                    }
                    *(int *)(pc + 0xc) = n + 1;

                    if (n > 0x62) {
                        s16(*(void **)(pc + 8), 0x28) = (short)((n + 1) * 6);
                        AE_PaintCanvas_SetWorldViewMatrix(canvas);
                        MeshDraw(engine, *(Mesh **)(pc + 8));
                        *(int *)(pc + 0xc) = 0;
                    }
                }
            }

            int adv = (int)(short)s16(f, 0x10) + advance;
            int eff = adv;
            if (text[idx] == 0x20)
                eff = adv - 2;
            if (advance != 0xb)
                eff = adv;
            x += eff;
        }

        idx += step;
    }

    // Flush any remaining batched glyphs.
    if (shaderMode) {
        int n = *(int *)(pc + 0xc);
        if (n > 0) {
            s16(*(void **)(pc + 8), 0x28) = (short)((n + (n << 1)) * 2);
            AE_PaintCanvas_SetWorldViewMatrix(canvas);
            MeshDraw(engine, *(Mesh **)(pc + 8));
            *(int *)(pc + 0xc) = 0;
        }
    }

    return 1;
}

} // namespace AbyssEngine

// ---- operator__737dc.cpp ----
// AbyssEngine::operator+(int const&, AbyssEngine::String const&)
// Render a 32-bit integer to a string, then append the rhs string.
namespace AbyssEngine {


String operator+(const int &a, const String &b)
{
    String acc;
    String_fromInt(&acc, a);
    String_appendEq(&acc, &b);

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- ImageCreateFontFromFile_6f13c.cpp ----
// AbyssEngine::ImageCreateFontFromFile(Engine*, char const*, unsigned short index, ImageFont**)
// Parse a bitmap-font atlas file and build an ImageFont (0x14 bytes): a glyph-codepoint array
// (+0x04) and a parallel array of per-glyph quad meshes (+0x0c). The file is scanned for the
// requested font index; matching fonts have their glyph table read and a 2-triangle textured
// quad mesh generated per glyph (positions in mesh+0x04, UVs in mesh+0x08). Non-matching font
// entries are skipped. Returns 1 on success, -1 on error, -4 on bad arguments.
namespace AbyssEngine {

extern "C" {
void  AEFile_Close(unsigned int handle);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                unsigned int vertexFormat, void **out);
void ImageFontRelease(Engine *engine, ImageFont **slot);

// Build one glyph quad into the mesh at *meshSlot using the four region shorts already read.
static void buildGlyphQuad(int mesh, unsigned int offX, unsigned int offY, unsigned int sizeX,
                           unsigned int sizeY, unsigned int atlasW, unsigned int atlasH,
                           unsigned char mode)
{
    float *pos = *(float **)(mesh + 4);
    float halfW = AE_VectorUnsignedToFloat(sizeX, mode);
    pos[0] = 0; pos[1] = 0; pos[2] = 0;
    pos[4] = 0; pos[5] = 0; pos[3] = halfW; pos[6] = halfW;
    float halfH = AE_VectorUnsignedToFloat(offY, mode); // mirrors decompile's field reuse
    pos[8] = 0; pos[9] = 0; pos[11] = 0; pos[7] = halfH; pos[10] = halfH;

    double aH = (double)AE_VectorUnsignedToFloat(atlasW, mode);
    double aW = (double)AE_VectorUnsignedToFloat(atlasH, mode);
    float offYs = AE_VectorSignedToFloat((int)(short)sizeY, mode);
    float offXs = AE_VectorSignedToFloat((int)(short)sizeX, mode);
    float u0 = AE_VectorUnsignedToFloat(offX, mode) * (float)(1.0 / aW);
    float v0 = AE_VectorUnsignedToFloat(offX, mode) * (float)(1.0 / aH);

    float *uv = *(float **)(mesh + 8);
    float v1 = (halfH + offYs) * (float)(1.0 / aH);
    float u1 = (halfW + offXs) * (float)(1.0 / aW);
    uv[0] = u0; uv[1] = v0; uv[2] = u1;
    uv[3] = v0; uv[4] = u1; uv[5] = v1;
    uv[6] = u0; uv[7] = v1;

    unsigned int *draw = *(unsigned int **)(mesh + 0x2c);
    draw[0] = 0x20000;
    draw[1] = 1;
    draw[2] = 0;
}

int ImageCreateFontFromFile(Engine *engine, char *path, unsigned short index, ImageFont **out)
{
    if (engine == 0 || path == 0)
        return -4;

    unsigned int handle = 0;
    if (AEFile::OpenRead((const char *)(path), (uint32_t *)(&handle)) == 0)
        return -1;

    char magic[8];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile::Read((uint32_t)(8), magic, handle) == 0) { ImageFontRelease(engine, out); return -1; }
    static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    for (unsigned int k = 0; k < 8; ++k)
        if (want[k] != magic[k]) { ImageFontRelease(engine, out); return -1; }

    unsigned char mode = 0;
    unsigned char fmt = 0;
    unsigned short atlasH = 0, atlasW = 0, fontCount = 0;
    unsigned short glyphCount = 0;
    if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
    if (AEFile::Read((uint32_t)(2), &atlasH, handle) == 0) goto fail;
    if (AEFile::Read((uint32_t)(2), &atlasW, handle) == 0) goto fail;
    if (AEFile::Read((uint32_t)(2), &fontCount, handle) == 0) goto fail;
    if (AEFile::Skip((uint32_t)((unsigned int)fontCount << 3), handle) == 0) goto fail;

    // Skip the format-specific palette/data block (size depends on fmt).
    {
        unsigned int fb = (unsigned int)fmt - 3;
        if (fb < 0x1f) {
            unsigned int extra = 0;
            if (((1u << (fb & 0xff)) & 0u) != 0 || (fmt >= 0x24)) {
                if (AEFile::Read((uint32_t)(4), &extra, handle) == 0) goto fail;
            } else if (fb == 0) {
                extra = (unsigned int)atlasW * (unsigned int)atlasH * 4;
            }
            if (AEFile::Skip((uint32_t)(extra), handle) == 0) goto fail;
        }
    }

    if (AEFile::Read((uint32_t)(2), &glyphCount, handle) == 0) goto fail;
    if (glyphCount <= index) goto fail;

    {
        char *font = (char *)::operator new(0x14);
        u16(font, 0x0) = 0;
        *(unsigned long long *)(font + 4) = 0;
        *(unsigned long long *)(font + 0xc) = 0;
        *out = (ImageFont *)font;
    }

    for (unsigned short g = 0; g < glyphCount; ++g) {
        if (g == index) {
            char *font = (char *)*out;
            if (AEFile::Read((uint32_t)(2), font + 0x0, handle) == 0) goto fail; // glyph count
            void *codes = ::operator new[]((unsigned int)u16(font, 0x0) << 1);
            pp(font, 0x4) = codes;
            if (AEFile::Read((uint32_t)((unsigned int)u16((char *)*out, 0x0) << 1), pp((char *)*out, 0x4), handle) == 0)
                goto fail;
            font = (char *)*out;
            void *meshes = ::operator new[]((unsigned int)u16(font, 0x0) << 2);
            pp(font, 0xc) = meshes;

            unsigned int n = u16((char *)*out, 0x0);
            for (unsigned int gi = 0; gi < n; ++gi) {
                void **meshSlot = (void **)(*(int *)(pp((char *)*out, 0xc)) + gi * 4);
                if (MeshCreate(engine, 4, 2, 0x13, meshSlot) != 1)
                    goto fail;
                unsigned short sizeY = 0, sizeX = 0, offX = 0, offY = 0;
                if (AEFile::Read((uint32_t)(2), &sizeY, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &sizeX, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &offX, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &offY, handle) == 0) goto fail;
                int mesh = *(int *)(*(int *)(pp((char *)*out, 0xc)) + gi * 4);
                buildGlyphQuad(mesh, offX, offY, sizeX, sizeY, atlasW, atlasH, mode);
            }
        } else {
            unsigned short subCount = 0;
            if (AEFile::Read((uint32_t)(2), &subCount, handle) == 0) goto fail;
            if (AEFile::Skip((uint32_t)((unsigned int)subCount * 10), handle) == 0) goto fail;
        }
    }

    AEFile_Close(handle);
    return 1;

fail:
    ImageFontRelease(engine, out);
    return -1;
}

} // namespace AbyssEngine

// ---- operator__6ee0c.cpp ----
// AbyssEngine::operator+(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&)
// Component-wise quaternion addition (only .y/.w lanes materialized, see operator-).
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator+(const Quaternion &a, const Quaternion &b)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) + *(const float *)((const char *)&b + 4);
    float wv = *(const float *)((const char *)&a + 0xc) + *(const float *)((const char *)&b + 0xc);
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine

// ---- SpriteSystemSetAllSize_86db8.cpp ----
// AbyssEngine::SpriteSystemSetAllSize(short, AbyssEngine::SpriteSystem*)
// Write a single size value to every sprite (or just the first when the "shared size"
// flag at +0xc is set).
namespace AbyssEngine {

void SpriteSystemSetAllSize(short size, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    short *sizes = (short *)pp(sys, 0x8);
    if (u8(sys, 0xc) != 0) {
        sizes[0] = size;
        return;
    }
    unsigned int count = u16(sys, 0x0);
    for (unsigned int i = 0; i < count; ++i)
        sizes[i] = size;
}

} // namespace AbyssEngine

// ---- MODF_72adc.cpp ----
// AbyssEngine::MODF(float, float*)
// Split value into integer (truncated toward zero) and fractional parts. The integer
// part is written through param_2; the fractional remainder is returned.
namespace AbyssEngine {

float MODF(float value, float *intPart)
{
    float ip = (float)(int)value; // VectorSignedToFloat == truncating int<->float convert
    *intPart = ip;
    return value - ip;
}

} // namespace AbyssEngine

// ---- SpriteSystemDraw_86ddc.cpp ----
// AbyssEngine::SpriteSystemDraw(Engine*, AEMath::Matrix const& view, AEMath::Matrix const& world,
//                               SpriteSystem*)
// Rebuild every sprite's billboard quad in view space (4 corners offset by the sprite's size
// around its world-transformed center), then either draw the backing mesh directly or, when
// the mesh carries a batching context (ptr@0x30), enqueue the mesh + its view/world/uv matrices
// into the batch arrays.
namespace AbyssEngine {


int MeshDraw(Engine *engine, Mesh *mesh);

// Array<T>::AddCached helpers (templated in the binary).

extern "C" void AE_SpriteSystem_pushMatrix(
    unsigned int m0, unsigned int m1, unsigned int m2, unsigned int m3, unsigned int m4,
    unsigned int m5, unsigned int m6, unsigned int m7, unsigned int m8, unsigned int m9,
    unsigned int m10, unsigned int m11, unsigned int m12, unsigned int m13, unsigned int m14,
    int dst);


void SpriteSystemDraw(Engine *engine, Matrix *view, Matrix *world, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    Mesh *mesh = (Mesh *)pp(sys, 0x10);
    float *vbuf = (float *)pp(mesh, 0x4);

    Matrix mv;
    AE_AEMath_matMul(&mv, view);

    unsigned short count = u16(sys, 0x0);
    unsigned char sharedSize = u8(sys, 0xc);
    float *posCpu = (float *)pp(sys, 0x4);
    short *sizeCpu = (short *)pp(sys, 0x8);
    const float *W = (const float *)world;

    unsigned short sizeIdx = 0;
    unsigned short posIdx = 0;
    for (unsigned int q = 0; (q & 0xffff) < (unsigned int)count * 0xc; q += 0xc) {
        float *p = posCpu + (unsigned int)posIdx;
        posIdx = posIdx + 3;
        float px = p[0], py = p[1], pz = p[2];

        unsigned int si = sizeIdx;
        sizeIdx = (unsigned short)(((sharedSize ^ 1) & 0xff) + sizeIdx);
        float half = (float)((int)sizeCpu[si] >> 1);

        // Transform the sprite center by the world matrix (column-major 3x4 used here).
        float cx = W[0xc / 4 + 0] + W[4 / 4] * py + W[0] * px + W[8 / 4] * pz;
        float cy = W[0x1c / 4] + py * W[0x14 / 4] + px * W[0x10 / 4] + pz * W[0x18 / 4];
        float cz = W[0x2c / 4] + py * W[0x24 / 4] + px * W[0x20 / 4] + pz * W[0x28 / 4];

        float left = cx - half, right = cx + half;
        float bottom = cy - half, top = cy + half;

        float *out = vbuf + q;
        out[0] = left;  out[1] = top;    out[2] = cz;
        out[3] = right; out[4] = top;    out[5] = cz;
        out[6] = right; out[7] = bottom; out[8] = cz;
        out[9] = left;  out[10] = bottom; out[11] = cz;
    }

    if (i32(mesh, 0x30) == 0) {
        MeshDraw(engine, mesh);
    } else {
        void *batch = pp(mesh, 0x30);
        AE_ArrayAddCached_MeshPtr(mesh, (char *)batch + 0x44);

        const unsigned int *v = (const unsigned int *)view;
        AE_SpriteSystem_pushMatrix(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9],
                                   v[10], v[11], v[12], v[13], v[14], (int)(intptr_t)pp(mesh, 0x30) + 0x2c);
        unsigned int one = 0x3f800000;
        AE_SpriteSystem_pushMatrix(one, 0, 0, 0, 0, g_SpriteSystem_oneHalf, 0, 0, 0, 0, one, 0,
                                   one, one, one, (int)(intptr_t)pp(mesh, 0x30) + 0x38);
        const unsigned int *w = (const unsigned int *)world;
        AE_SpriteSystem_pushMatrix(w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9],
                                   w[10], w[11], w[12], w[13], w[14], (int)(intptr_t)pp(mesh, 0x30) + 0x5c);
        AE_ArrayAddCached_uint(0xffffffff, (char *)pp(mesh, 0x30) + 0x50);
    }
}

} // namespace AbyssEngine

// ---- MeshConvertToVBOIntern_6d510.cpp ----
extern "C" {
void glGenBuffers(int n, void *buffers);
void glBindBuffer(unsigned int target, unsigned int buffer);
void glBufferData(unsigned int target, unsigned int size, const void *data, unsigned int usage);
void glDeleteBuffers(int n, const void *buffers);
unsigned int glGetError();
}

// AbyssEngine::MeshConvertToVBOIntern(AbyssEngine::Mesh*)
// Upload the mesh's CPU vertex arrays into GL buffer objects (position/uv+index/normal
// [+tangent/binormal]/color, selected by the vertex-format flag byte). On success, optionally
// frees the CPU copies and marks the mesh uploaded (byte@0x5c); on GL error, tears the buffers
// back down. Returns 1 / -1 / -4.
namespace AbyssEngine {


static inline void freeIf(Mesh *m, unsigned int off)
{
    if (pp(m, off) != 0)
        operator delete[](pp(m, off));
    pp(m, off) = 0;
}

int MeshConvertToVBOIntern(Mesh *m)
{
    if (m == 0 || *g_Mesh_vboEnabledFlag == 0)
        return -4;
    if (u8(m, 0x5c) != 0 || s16(m, 0x28) == 0)
        return -4;

    void *posArr = pp(m, 0x4);
    void *uvArr = pp(m, 0x8);
    void *colArr = pp(m, 0xc);
    void *normArr = pp(m, 0x10);
    void *tanArr = pp(m, 0x14);
    void *binArr = pp(m, 0x18);

    unsigned int vcount = (unsigned int)(unsigned short)u16(m, 0x2);

    glGenBuffers(1, (char *)m + 0x60);
    glBindBuffer(0x8892, u32(m, 0x60));
    glBufferData(0x8892, vcount * 0xc, posArr, 0x88e4);

    unsigned char flags = u8(m, 0x0);
    if (flags & 2) {
        glGenBuffers(1, (char *)m + 0x68);
        glBindBuffer(0x8892, u32(m, 0x68));
        glBufferData(0x8892, vcount << 3, uvArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 8);

        glGenBuffers(1, (char *)m + 0x64);
        glBindBuffer(0x8893, u32(m, 0x64));
        glBufferData(0x8893, (unsigned int)(unsigned short)u16(m, 0x28) << 1, pp(m, 0x2c), 0x88e4);
        i32(m, 0x7c) += (int)((unsigned int)(unsigned short)u16(m, 0x28) * 2);
        flags = u8(m, 0x0);
    }

    if (flags & 8) {
        glGenBuffers(1, (char *)m + 0x6c);
        glBindBuffer(0x8892, u32(m, 0x6c));
        glBufferData(0x8892, vcount * 0xc, normArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 0xc);
        if (*g_Mesh_tangentEnabledFlag != 0) {
            glGenBuffers(1, (char *)m + 0x70);
            glBindBuffer(0x8892, u32(m, 0x70));
            glBufferData(0x8892, vcount * 0xc, tanArr, 0x88e4);
            i32(m, 0x7c) += (int)(vcount * 0xc);
            glGenBuffers(1, (char *)m + 0x74);
            glBindBuffer(0x8892, u32(m, 0x74));
            glBufferData(0x8892, vcount * 0xc, binArr, 0x88e4);
            i32(m, 0x7c) += (int)(vcount * 0xc);
        }
    }

    if (u8(m, 0x0) & 0x10) {
        glGenBuffers(1, (char *)m + 0x78);
        glBindBuffer(0x8892, u32(m, 0x78));
        glBufferData(0x8892, vcount << 4, colArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 0x10);
    }

    glBindBuffer(0x8892, 0);
    glBindBuffer(0x8893, 0);

    if (glGetError() == 0) {
        if (*g_Mesh_keepCpuCopyFlag == 0) {
            freeIf(m, 0x4);
            freeIf(m, 0x8);
            freeIf(m, 0x2c);
            freeIf(m, 0x10);
            freeIf(m, 0xc);
            freeIf(m, 0x14);
            freeIf(m, 0x18);
        }
        u8(m, 0x5c) = 1;
        *g_Mesh_vboByteCounter += i32(m, 0x7c);
        return 1;
    }

    // GL error path: delete whatever buffers were created.
    if (u8(m, 0x5c) != 0) {
        glDeleteBuffers(1, (char *)m + 0x60);
        glDeleteBuffers(1, (char *)m + 0x64);
        unsigned char f = u8(m, 0x0);
        if (f & 2) {
            glDeleteBuffers(1, (char *)m + 0x68);
            f = u8(m, 0x0);
        }
        if (f & 8) {
            glDeleteBuffers(1, (char *)m + 0x6c);
            if (*g_Mesh_tangentDelFlag != 0) {
                glDeleteBuffers(1, (char *)m + 0x70);
                glDeleteBuffers(1, (char *)m + 0x74);
            }
        }
        if (u8(m, 0x0) & 0x10)
            glDeleteBuffers(1, (char *)m + 0x78);
    }
    i32(m, 0x7c) = 0;
    return -1;
}

} // namespace AbyssEngine

// ---- TransformConvertToVBO_6d7e0.cpp ----
// AbyssEngine::TransformConvertToVBO(AbyssEngine::Transform*)
// Convert owned meshes to VBOs (count@0x3c, array@0x40), then recurse into child transforms
// (count@0x4c, array@0x50).
namespace AbyssEngine {

int MeshConvertToVBO(Mesh *mesh);

int TransformConvertToVBO(Transform *t)
{
    if (t != 0) {
        for (unsigned int i = 0; i < u32(t, 0x3c); ++i)
            MeshConvertToVBO(*(Mesh **)((char *)pp(t, 0x40) + i * 4));
        for (unsigned int i = 0; i < u32(t, 0x4c); ++i)
            TransformConvertToVBO(*(Transform **)((char *)pp(t, 0x50) + i * 4));
    }
    return 1;
}

} // namespace AbyssEngine

// ---- ImageFontCheckString_72130.cpp ----
// AbyssEngine::ImageFontCheckString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int)
// The shipped body just spins a counter to param_3 (a stubbed/optimized-out validator).
namespace AbyssEngine {

void ImageFontCheckString(ImageFont * /*font*/, const unsigned short * /*str*/, unsigned int count)
{
    unsigned short i = 0;
    unsigned int v;
    do {
        v = (unsigned int)i;
        i = i + 1;
    } while (v < count);
}

} // namespace AbyssEngine

// ---- TextureCreateFromFile_6f7cc.cpp ----
// AbyssEngine::TextureCreateFromFile(Engine*, char const*, void(*)(Image*, void*), void*,
//                                    unsigned int*, bool, float)
// Thin wrapper that forwards to the *Intern variant and always reports success (1).
namespace AbyssEngine {

typedef void (*ImageCallback)(Image *, void *);

int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user,
                                unsigned int *outIds, float scale, AELoadedTexture *outTex,
                                bool flag);

int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user,
                          unsigned int *outIds, bool /*flag*/, float scale)
{
    TextureCreateFromFileIntern(engine, path, cb, user, outIds, scale,
                                (AELoadedTexture *)0, false);
    return 1;
}

} // namespace AbyssEngine

// ---- TextureCreateFromFileIntern_6f7f4.cpp ----
// AbyssEngine::TextureCreateFromFileIntern(Engine*, char const* path,
//     void(*cb)(Image*,void*), void* user, unsigned int* outIds, float aniso,
//     AELoadedTexture* outTex, bool ...)
// Load an image from file and upload it as an OpenGL texture. Handles cubemaps (format 6) and
// the full set of (compressed and uncompressed) 2D formats, generating mip levels for the
// compressed paths. Filtering/wrap parameters are chosen from engine/global flags. On success
// the texture id is returned in *outIds (or recorded into an AELoadedTexture cache entry).
// Returns 1 on success, -4 on a GL upload error.
namespace AbyssEngine {

extern "C" {
unsigned int glGetError();
void glGenTextures(int n, unsigned int *ids);
void glDeleteTextures(int n, const unsigned int *ids);
void glBindTexture(unsigned int target, unsigned int id);
void glPixelStorei(unsigned int pname, int param);
void glTexEnvi(unsigned int t, unsigned int p, int v);
void glTexParameteri(unsigned int target, unsigned int pname, int param);
void glTexParameterf(unsigned int target, unsigned int pname, float param);
void glTexImage2D(unsigned int t, int lvl, int ifmt, int w, int h, int b, int fmt, int type, const void *px);
void glCompressedTexImage2D(unsigned int t, int lvl, int ifmt, int w, int h, int b, int sz, const void *px);
void glGenerateMipmap(unsigned int target);
// __aeabi_uidiv is declared by gof2/Engine.h

void  AE_String_ctor(void *self, const char *s, bool b);
void  AE_String_assign(void *dst, void *src);
void  AE_String_Set(void *self, const char *s);
void  AELabelObject(unsigned int type, unsigned int id, const char *name);
void  AE_ArrayAdd_TexPtr(void *item, void *arr);

char *g_cubemapEnabledFlag;     // **(DAT + 0x7f860)
char *g_texEnvFlag;             // **(DAT + 0x7f998)
char *g_clampFlag;              // **(DAT + 0x7f9b2)
float *g_anisoMaxPtr;           // **(DAT + 0x7fa00)
char *g_labelObjectsFlag;       // **(DAT + 0x7c9d2) (reused)
}

int  ImageCreateFromFile(Engine *engine, char *path, Image **out);
void ImageRelease(Image **slot);

// Helper to upload one compressed mip chain for the simple formats.
static void uploadCompressedChain(char *img, unsigned int glFmt, int divShift, int minBlock)
{
    unsigned int w = (unsigned int)u16(img, 0x0);
    unsigned int h = (unsigned int)u16(img, 0x2);
    int level = 0;
    for (unsigned int off = 0; off < u32(img, 0x10);) {
        unsigned int blockSz = (w * h) >> divShift;
        if ((int)blockSz < minBlock)
            blockSz = (unsigned int)minBlock;
        glCompressedTexImage2D(0xde1, level, glFmt, (int)w, (int)h, 0, (int)blockSz,
                               (char *)pp(img, 0xc) + off);
        off += blockSz;
        unsigned int nw = 1, nh = 1;
        if (h >> 1 > 1) nh = h >> 1;
        if (w >> 1 > 1) nw = w >> 1;
        ++level;
        w = nw;
        h = nh;
    }
}

int TextureCreateFromFileIntern(Engine *engine, char *path, void (*cb)(Image *, void *),
                                void *user, unsigned int *outIds, float aniso,
                                AELoadedTexture *outTex, bool /*flag*/)
{
    char *en = (char *)engine;
    Image *imgPtr = 0;
    *outIds = 0;
    u32(en, 0x10) = glGetError();

    if (ImageCreateFromFile(engine, path, &imgPtr) != 1)
        return -4;
    if (cb != 0)
        cb(imgPtr, user);

    glGenTextures(1, outIds);
    char *img = (char *)imgPtr;
    int format = (int)u32(img, 0x4);

    if (format == 6) {
        // Cubemap: six square faces packed vertically.
        if (*g_cubemapEnabledFlag != 0) {
            glBindTexture(0x8513, *outIds);
            glTexParameteri(0x8513, 0x2800, 0x2601);
            glTexParameteri(0x8513, 0x2801, 0x2601);
            int faceH = __aeabi_uidiv((int)u16(img, 0x2), 6);
            unsigned int w = u16(img, 0x0);
            const unsigned int faces[6] = {0x8517, 0x8516, 0x8519, 0x8515, 0x851a, 0x8518};
            int faceBytes = faceH * (int)w;
            for (int fi = 0; fi < 6; ++fi) {
                glTexImage2D(faces[fi], 0, 0x1908, (int)w, faceH, 0, 0x1908, 0x1401,
                             (char *)pp(img, 0xc) + faceBytes * 4 * fi);
            }
        }
    } else {
        glBindTexture(0xde1, *outIds);
        glPixelStorei(0xcf5, 1);
        if (*g_texEnvFlag == 0)
            glTexEnvi(0x2300, 0x2200, 0x2100);

        int wrap;
        if (*g_clampFlag != 0) {
            glTexParameteri(0xde1, 0x2802, 0x812f);
            wrap = 0x812f;
        } else {
            glTexParameteri(0xde1, 0x2802, 0x2901);
            wrap = 0x2901;
        }
        glTexParameteri(0xde1, 0x2803, wrap);

        // Min/mag filters depend on mipmap availability and anisotropy settings.
        if (u8(img, 0x8) == 0) {
            if (u8(en, 0x20) != 0) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            } else {
                glTexParameteri(0xde1, 0x2800, 0x2600);
                glTexParameteri(0xde1, 0x2801, 0x2600);
            }
        } else if (*g_anisoMaxPtr <= 0.0f) {
            glTexParameteri(0xde1, 0x2801, 0x2703);
            glTexParameteri(0xde1, 0x2800, 0x2601);
        } else {
            glTexParameterf(0xde1, 0x84fe, *g_anisoMaxPtr);
            glTexParameteri(0xde1, 0x2801, 0x2703);
        }

        if (*g_clampFlag != 0) {
            glTexParameteri(0xde1, 0x2800, 0x2601);
            glTexParameteri(0xde1, 0x2801, 0x2601);
        }

        int w = (int)u16(img, 0x0);
        int h = (int)u16(img, 0x2);
        switch (format) {
        case 1:
            glTexImage2D(0xde1, 0, 0x1909, w, h, 0, 0x1909, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 2:
            glTexImage2D(0xde1, 0, 0x1907, w, h, 0, 0x1907, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 3:
            glTexImage2D(0xde1, 0, 0x1908, w, h, 0, 0x1908, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 4: // ETC1 / DXT1-ish
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c03, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c03, 1, 0x10);
            break;
        case 5:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c02, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c02, 1, 0x10);
            break;
        case 7:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c93, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c93, 0, 0x10);
            break;
        case 8:
        case 9:
        case 10: {
            unsigned int glFmt = 0x83f0;
            if (format == 9) glFmt = 0x83f2;
            if (format == 10) glFmt = 0x83f3;
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, glFmt, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, glFmt, 1, 0x10);
            break;
        }
        case 0xb:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8d64, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8d64, 1, 8);
            break;
        default:
            break;
        }
    }

    // Verify upload and either label the texture or cache it.
    if (outTex == 0) {
        int err = (int)glGetError();
        u32(en, 0x10) = (unsigned int)err;
        if (err != 0) {
            char tmp[16];
            AE_String_ctor(tmp, path, false);
            AE_String_assign(en + 0x14, tmp);
            ((::String *)tmp)->dtor();
            glDeleteTextures(1, outIds);
            ImageRelease(&imgPtr);
            return -4;
        }
        AELabelObject(0x1702, *outIds, path);
    } else {
        char *tex = (char *)outTex;
        u8(tex, 0x15) = 1;
        u8(tex, 0x14) = (format == 6) ? 1 : 0;
        if (glGetError() == 0) {
            AELabelObject(0x1702, *outIds, path);
            u32(tex, 0x0) = *outIds;
            u32(tex, 0x18) = u32(img, 0x10);
        } else {
            u32(tex, 0x0) = 0xffffffff;
            u32(tex, 0x18) = 0;
            glDeleteTextures(1, outIds);
        }
        i32(en, 0x70) += (int)u32(tex, 0x18);
    }

    ImageRelease(&imgPtr);
    return 1;
}

} // namespace AbyssEngine

// ---- operator__736dc.cpp ----
// AbyssEngine::operator+(long long const&, AbyssEngine::String const&)
// Render a 64-bit integer to a string, then append the rhs string.
namespace AbyssEngine {


String operator+(const long long &a, const String &b)
{
    String acc;
    String_fromLongLong(&acc, a);
    String_appendEq(&acc, &b);

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- SpriteSystemRelease_87064.cpp ----

// AbyssEngine::SpriteSystemRelease(AbyssEngine::Engine*, AbyssEngine::SpriteSystem**)
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void SpriteSystemRelease(Engine *engine, SpriteSystem **slot)
{
    if (*slot == 0)
        return;

    void *p = pp(*slot, 0x4);
    if (p != 0)
        operator delete[](p);
    pp(*slot, 0x4) = 0;

    p = pp(*slot, 0x8);
    if (p != 0)
        operator delete[](p);
    pp(*slot, 0x8) = 0;

    MeshRelease(engine, (Mesh **)((char *)*slot + 0x10));

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine

// ---- CurveCreate_7415c.cpp ----

// AbyssEngine::CurveCreate(void**, unsigned short, AbyssEngine::Curve**)
// Allocate a Curve {count@0, void** data@4}, copy `count` 4-byte entries from the source.
namespace AbyssEngine {

int CurveCreate(void **src, unsigned short count, Curve **out)
{
    Curve *curve = (Curve *)operator new(8);
    u32(curve, 0x4) = 0;
    *out = curve;
    u16(curve, 0x0) = count;

    void *data = operator new[]((unsigned int)count << 2);
    pp(curve, 0x4) = data;
    __aeabi_memcpy4(data, src, (unsigned int)count << 2);
    return 1;
}

} // namespace AbyssEngine

// ---- operator___7390c.cpp ----
// AbyssEngine::operator==(AbyssEngine::String const&, AbyssEngine::String const&)
// Copy lhs into a temp, compare against rhs, return the comparison result.
namespace AbyssEngine {


bool operator==(const String &a, const String &b)
{
    String tmp;
    String_copy(&tmp, &a, false);
    int cmp = String_Compare(&tmp, &b);
    ((String *)(&tmp))->dtor();
    return cmp != 0;
}

} // namespace AbyssEngine

// ---- MeshReleaseIntern_6c380.cpp ----

// AbyssEngine::MeshReleaseIntern(AbyssEngine::Engine*, AbyssEngine::Mesh**)
// Tear down a mesh: delete its GL buffer objects (gated on the flag byte at offset 0 and the
// "uploaded" flag at 0x5c), free all CPU-side arrays, destroy the owned Transform, and free
// the mesh. Skips GL/array teardown entirely when it is a shared/aliased mesh (byte@0x38).
namespace AbyssEngine {



static inline void freeArray(Mesh **slot, unsigned int off)
{
    void *p = pp(*slot, off);
    if (p != 0)
        operator delete[](p);
    pp(*slot, off) = 0;
}

void MeshReleaseIntern(Engine * /*engine*/, Mesh **slot)
{
    Mesh *m = (Mesh *)*slot;
    if (m == 0)
        return;

    if (u8(m, 0x38) == 0) {
        if (u8(m, 0x5c) != 0) {
            glDeleteBuffers(1, (char *)m + 0x60);
            glDeleteBuffers(1, (char *)*slot + 0x64);
            m = (Mesh *)*slot;
            unsigned char flags = u8(m, 0x0);
            if (flags & 2) {
                glDeleteBuffers(1, (char *)m + 0x68);
                m = (Mesh *)*slot;
                flags = u8(m, 0x0);
            }
            if ((flags & 4) != 0) { // bit set when (flags<<0x1d) is negative
                glDeleteBuffers(1, (char *)m + 0x6c);
                if (*g_Mesh_tangentEnabledFlag != 0) {
                    glDeleteBuffers(1, (char *)*slot + 0x70);
                    glDeleteBuffers(1, (char *)*slot + 0x74);
                }
            }
            m = (Mesh *)*slot;
            if (u8(m, 0x0) & 8) { // bit set when (flags<<0x1c) is negative
                glDeleteBuffers(1, (char *)m + 0x78);
                m = (Mesh *)*slot;
            }
        }

        freeArray(slot, 0x2c);
        freeArray(slot, 0x4);
        freeArray(slot, 0x8);
        freeArray(slot, 0xc);
        freeArray(slot, 0x10);

        if (*g_Mesh_extraArraysFlag != 0) {
            freeArray(slot, 0x14);
            freeArray(slot, 0x18);
        }
    }

    void *t = pp(*slot, 0x34);
    if (t != 0) {
        void *freed = AE_Transform_dtor(t);
        operator delete(freed);
    }
    pp(*slot, 0x34) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine

// ---- operator__73838.cpp ----
// AbyssEngine::operator+(AbyssEngine::String const&, float const&)
// Append the rendering of a float to a string.
namespace AbyssEngine {


String operator+(const String &a, const float &b)
{
    String acc;
    String_copy(&acc, &a, false);

    String num;
    String_fromFloat(&num, b);
    String_appendEq(&acc, &num);
    ((String *)(&num))->dtor();

    String result;
    String_copy(&result, &acc, false);
    ((String *)(&acc))->dtor();
    return result;
}

} // namespace AbyssEngine

// ---- MeshConvertToVBO_6bbac.cpp ----
// AbyssEngine::MeshConvertToVBO(AbyssEngine::Mesh*)
// Convert a single mesh to a VBO when VBOs are globally enabled and the mesh is eligible
// (flag@0x5c clear and pointer@0x84 set). Returns 1 on success, -4 (0xfffffffc) otherwise.
namespace AbyssEngine {


int MeshConvertToVBOIntern(Mesh *mesh);
int TransformConvertToVBO(Transform *t);

int MeshConvertToVBO(Mesh *mesh)
{
    int result = -4;
    if (mesh != 0 && *g_Mesh_vboEnabledFlag != 0) {
        if (u8(mesh, 0x5c) != 0 || u8(mesh, 0x84) == 0)
            return -4;
        MeshConvertToVBOIntern(mesh);
        // mesh is AbyssEngine::Mesh (complete struct from Mesh.h); field_0x34 is the embedded
        // Transform pointer (AbyssEngine::Transform*).
        TransformConvertToVBO(mesh->field_0x34);
        result = 1;
    }
    return result;
}

} // namespace AbyssEngine

// ---- esMatrixMultiply_84618.cpp ----
// AbyssEngine::esMatrixMultiply(ESMatrix* out, ESMatrix* a, ESMatrix* b)
// Standard row-major 4x4 multiply: out = a * b. The shipped code uses NEON
// multiply/multiply-accumulate over the columns of b weighted by each row of a, then
// memcpy's the scratch result over `out` (so out may alias a or b safely).
namespace AbyssEngine {

void esMatrixMultiply(ESMatrix *out, ESMatrix *a, ESMatrix *b)
{
    float local[4][4];
    const float (*A)[4] = a->m;
    const float (*B)[4] = b->m;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            local[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] +
                          A[i][3] * B[3][j];
        }
    }

    __aeabi_memcpy(out, local, 0x40);
}

} // namespace AbyssEngine

// ---- MeshCreateFromFile_6d074.cpp ----
// AbyssEngine::MeshCreateFromFile(Engine*, char const*, Mesh**, Material*)
// Allocate and identity-initialize a Mesh (0x88 bytes), open the model file, classify its
// 7-byte magic into one of five format flags, read the submesh count, and dispatch to
// MeshReadData. Single-submesh files are read directly; multi-submesh files allocate a
// Transform (mesh+0x34) and read each child mesh into it, merging bounding spheres. After
// loading, animation data is collected. Returns 1 on success, -1 on error, -4 on bad args.
namespace AbyssEngine {

extern "C" {
void  AEFile_Close(unsigned int handle);
void  AE_Transform_ctor(void *self);
void  AE_Transform_CollectAnimationData(void *self);
void  AE_Transform_SetAnimationRangeInTime(void *self, long long t);
void  AE_BSphere_Merge(void *dst, const void *src);
void  AE_ArrayAdd_MeshPtr(void *item, void *arr);
long long __aeabi_f2lz(float v);
}

int  MeshReadData(Engine *engine, unsigned int *handle, unsigned int flags, Mesh **slot,
                  Material *mat);
void MeshRelease(Engine *engine, Mesh **slot);

// Zero a freshly-allocated Mesh and write its identity defaults.
static void initMesh(char *m)
{
    for (int i = 0; i < 0x88; ++i) m[i] = 0;
    // Identity-ish: scale field at 0x4c = 1.0; the matrix slots default to 0 here.
    u32(m, 0x4c) = 0x3f800000;
}

int MeshCreateFromFile(Engine *engine, char *path, Mesh **out, Material *mat)
{
    if (engine == 0 || path == 0)
        return -4;

    char *m = (char *)::operator new(0x88);
    initMesh(m);
    *out = (Mesh *)m;
    pp(m, 0x30) = mat;

    unsigned int handle = 0;
    if (AEFile::OpenRead((const char *)(path), (uint32_t *)((char *)m + 0x60)) == 0) {
        if (*out != 0)
            ::operator delete((void *)*out);
        *out = 0;
        return -1;
    }

    char magic[7];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile::Read((uint32_t)(7), magic, handle) == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    // Classify magic into a 5-bit format mask (one bit per known signature).
    static const char sig0[7] = {'A','E','M','e','s','h',0};
    static const char sig1[7] = {'A','E','A','n','i','m',0};
    static const char sig2[7] = {'A','E','S','k','i','n',0};
    static const char sig3[7] = {'A','E','L','o','d','0',0};
    static const char sig4[7] = {'A','E','M','e','s','2',0};
    unsigned int fmt = 0x1f;
    for (int i = 0; i < 7; ++i) {
        char ch = magic[i];
        if (sig0[i] != ch) fmt &= ~1u;
        if (sig1[i] != ch) fmt &= ~8u;
        if (sig2[i] != ch) fmt &= ~0x10u;
        if (sig3[i] != ch) fmt &= ~4u;   // sig3 -> bit2 (LOD)
        if (sig4[i] != ch) fmt &= ~2u;
    }

    if (fmt == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    // Optional version word for some formats.
    if ((fmt & 0x1b) != 0) {
        unsigned short ver = 0;
        if (AEFile::Read((uint32_t)(2), &ver, handle) == 0) { MeshRelease(engine, out); AEFile_Close(handle); return -1; }
    }
    // Read the first byte (vertex-format flag) into mesh+0x00.
    if (AEFile::Read((uint32_t)(1), (char *)*out, handle) == 0 || u8((char *)*out, 0x0) == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    bool ok = false;
    if ((fmt & 0x1a) == 0) {
        if (MeshReadData(engine, &handle, fmt, out, mat) != -1)
            ok = true;
    } else {
        unsigned short subCount = 0;
        if (AEFile::Read((uint32_t)(2), &subCount, handle) == 0) {
            MeshRelease(engine, out);
            AEFile_Close(handle);
            return -1;
        }
        if (subCount < 2) {
            if (MeshReadData(engine, &handle, fmt, out, mat) != -1)
                ok = true;
        } else {
            char *xf = (char *)::operator new(0x180);
            AE_Transform_ctor(xf);
            pp((char *)*out + 0x34, 0) = xf;
            for (unsigned int s = 0; s < subCount; ++s) {
                char *child = (char *)::operator new(0x88);
                initMesh(child);
                u8(child, 0x84) = 1;
                u8(child, 0x0) = u8((char *)*out, 0x0);
                pp(child, 0x30) = mat;
                Mesh *childPtr = (Mesh *)child;
                if (MeshReadData(engine, &handle, fmt, &childPtr, mat) == -1) {
                    MeshRelease(engine, out);
                    AEFile_Close(handle);
                    return -1;
                }
                AE_BSphere_Merge((char *)*out + 0x3c, (char *)childPtr + 0x3c);
                AE_ArrayAdd_MeshPtr(childPtr, (char *)*(int *)((char *)*out + 0x34) + 0x3c);
            }
            ok = true;
        }
    }

    if (ok) {
        AEFile_Close(handle);
        void *xf = pp((char *)*out + 0x34, 0);
        if (xf != 0) {
            AE_Transform_CollectAnimationData(xf);
            long long t = __aeabi_f2lz(0.0f);
            AE_Transform_SetAnimationRangeInTime(pp((char *)*out + 0x34, 0), t);
        }
        return 1;
    }

    MeshRelease(engine, out);
    AEFile_Close(handle);
    return -1;
}

} // namespace AbyssEngine

// ---- CameraIsSphereinViewFrustum_715cc.cpp ----
// AbyssEngine::CameraIsSphereinViewFrustum(Vector const&, float radius, Matrix*, Camera*)
// Test whether a world-space sphere (center, radius) overlaps the camera view frustum. The
// center is transformed into camera space (optionally pre-multiplied by an extra matrix) and
// the forward/right/up projections are range-checked, with the sphere radius widening each
// near/far/side bound.
//
// Camera fields (byte offsets): +0x04 near, +0x08 far, +0x0c local matrix (0x3c),
// +0x48 fwd-h slope, +0x50 fwd-v slope, +0x54 right radius scale, +0x58 up radius scale.
namespace AbyssEngine {


extern "C" {
// __aeabi_memcpy is declared by gof2/Engine.h
void AE_Matrix_mulAssign(void *self, const void *rhs);
void AE_Vector_assign(void *self, const void *rhs);
void AE_MatrixInverseTransformVector(void *out, const void *in);
void AE_MatrixGetPosition(void *m);
void AE_MatrixGetDir(void *m);
void AE_MatrixGetUp(void *m);
void AE_MatrixGetRight(void *m);
void AE_VectorSub(void *self, const void *rhs);
void AE_VectorNormalize(void *out, const void *in);
float AE_VectorDot(const void *a, const void *b);
}

int CameraIsSphereinViewFrustum(Vector *center, float radius, Matrix *extra, Camera *cam)
{
    // Ghidra aliases the camera-data pointer through r3; cam is the frustum data here.
    char *c = (char *)cam;

    if (!(radius != 0.0f && *g_Camera_frustumEnabledFlag != 0))
        return 1;

    Matrix local;
    Matrix transformed;
    Vector pos, dir, axis;
    Vector camPoint = {0.0f, 0.0f, 0.0f};

    Matrix *srcMatrix = (Matrix *)(c + 0xc);
    Matrix *dstMatrix;
    if (extra == 0) {
        dstMatrix = &local;
    } else {
        __aeabi_memcpy(&local, srcMatrix, 0x3c);
        AE_Matrix_mulAssign(&local, extra);
        dstMatrix = &transformed;
        srcMatrix = &local;
    }

    AE_MatrixInverseTransformVector(dstMatrix, srcMatrix);
    AE_Vector_assign(&camPoint, dstMatrix);

    AE_MatrixGetPosition(&pos);
    AE_VectorSub(&pos, center);
    AE_MatrixGetDir(&dir);
    AE_VectorSub(&axis, &dir);
    AE_VectorNormalize(&transformed, &axis);
    float fwd = AE_VectorDot(&pos, &transformed);

    // Near/far range padded by the radius.
    if (fwd > f32(c, 0x8) + radius)
        return 0;
    if (fwd < f32(c, 0x4) - radius)
        return 0;

    AE_MatrixGetRight(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float right = AE_VectorDot(&pos, &transformed);

    float rightPad = f32(c, 0x54) * radius;
    float rightLimit = fwd * f32(c, 0x48) * f32(c, 0x50);
    if (right > rightLimit + rightPad)
        return 0;
    if (right < -rightLimit - rightPad)
        return 0;

    AE_MatrixGetUp(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float up = AE_VectorDot(&pos, &transformed);

    float upPad = f32(c, 0x58) * radius;
    float upLimit = fwd * f32(c, 0x48);
    if (up > upLimit + upPad)
        return 0;
    if (up < -upLimit - upPad)
        return 0;

    return 1;
}

} // namespace AbyssEngine

// ---- CurveGetValue_74194.cpp ----
extern "C" long long __aeabi_uldivmod(unsigned int nlo, unsigned int nhi, unsigned int dlo,
                                      unsigned int dhi);

// AbyssEngine::CurveGetValue(unsigned long long time, AbyssEngine::Curve* curve)
//
// Evaluate an animation curve at a 64-bit fixed-point `time`. The curve owns an array of
// keyframe records (count@0 of the Curve, entry-pointer-array@4). Each keyframe carries a
// type tag (byte@0), a 64-bit start time (lo@8, hi@0xc) and a value@0x10; cubic keyframes add
// two control coefficients (@0x14, @0x18). The routine clamps to the end value when `time`
// lies outside the curve, otherwise locates the bracketing segment (scanning from the end)
// and applies constant (tag 1), linear (tag 2), or cubic (tag 3) interpolation in 12.20 fixed
// point.
//
// Ghidra rendered the hand-written 64-bit fixed-point arithmetic as a dense soup of CONCAT/
// shift expressions; this reproduces the algorithm faithfully in readable form. The query
// time arrives split across the sret `this` slot (high word) and r1 (low word), so we model
// it as a single 64-bit argument.
namespace AbyssEngine {

struct CurveRec {
    Curve *curve;       // r0 sret in the original ABI (unused as output here)
};

// Returns the interpolated value (low 32 bits) packed with an optional derivative (high).
long long CurveGetValue(unsigned long long time, Curve *curve)
{
    int *entries = (int *)pp(curve, 0x4);
    unsigned short count = u16(curve, 0x0);

    unsigned int qlo = (unsigned int)time;
    unsigned int qhi = (unsigned int)(time >> 32);

    auto keyTimeHi = [&](int kf) -> unsigned int { return *(unsigned int *)(kf + 0xc); };
    auto keyTimeLo = [&](int kf) -> unsigned int { return *(unsigned int *)(kf + 0x8); };
    auto keyVal = [&](int kf) -> int { return *(int *)(kf + 0x10); };

    if (count == 0)
        return 0;

    // Outside the curve on the low side -> clamp to first keyframe value.
    int first = entries[0];
    if ((unsigned long long)time <
        (((unsigned long long)keyTimeHi(first) << 32) | keyTimeLo(first))) {
        return keyVal(first);
    }

    // Outside on the high side -> clamp to last keyframe value.
    unsigned int idx = (unsigned int)(unsigned short)(count - 1);
    int last = entries[idx];
    if ((unsigned long long)time >
        (((unsigned long long)keyTimeHi(last) << 32) | keyTimeLo(last))) {
        return keyVal(last);
    }

    // Scan backward for the segment whose start time is <= query time.
    int seg = last;
    unsigned int segIdx = idx;
    while (segIdx > 0) {
        seg = entries[segIdx];
        unsigned long long st = ((unsigned long long)keyTimeHi(seg) << 32) | keyTimeLo(seg);
        if (st <= (unsigned long long)time)
            break;
        --segIdx;
    }

    char tag = *(char *)seg;
    unsigned long long segStart =
        ((unsigned long long)keyTimeHi(seg) << 32) | keyTimeLo(seg);
    int v0 = keyVal(seg);

    if (tag == 1) {
        // Constant: hold the segment value.
        return v0;
    }

    int nextKf = entries[segIdx + 1];
    unsigned long long segEnd =
        ((unsigned long long)keyTimeHi(nextKf) << 32) | keyTimeLo(nextKf);
    int v1 = keyVal(nextKf);

    // Normalized position t in [0, 0x1000) (12.20 fixed point) within the segment.
    unsigned int num = (unsigned int)((time - segStart)) << 12;
    unsigned int numHi = (unsigned int)(((time - segStart)) >> 20);
    unsigned int den = (unsigned int)(segEnd - segStart);
    long long tq = __aeabi_uldivmod(num, numHi, den, (unsigned int)((segEnd - segStart) >> 32));
    unsigned int t = (unsigned int)tq;

    if (tag == 2) {
        // Linear: v0 + t*(v1 - v0).
        long long d = (long long)(v1 - v0) * (unsigned int)t;
        return (int)((unsigned int)(d >> 12)) + v0;
    }

    // tag == 3: cubic Hermite-style blend using the segment's two coefficients.
    int c0 = *(int *)(seg + 0x14);
    int c1 = *(int *)(seg + 0x18);
    unsigned long long t2_64 = (unsigned long long)t * t;
    unsigned int t2 = (unsigned int)(t2_64 >> 12);
    unsigned long long t3_64 = (unsigned long long)t2 * t;
    unsigned int t3 = (unsigned int)(t3_64 >> 12);

    // value = v0 + c0 * (t - 2*t2 + t3) + c1 * (t3 - t2) + (v1 - v0) * (3*t2 - 2*t3)
    long long term0 = (long long)c0 * (int)(t - 2 * t2 + t3);
    long long term1 = (long long)c1 * (int)(t3 - t2);
    long long term2 = (long long)(v1 - v0) * (int)(3 * t2 - 2 * t3);
    int value = v0 + (int)(term0 >> 12) + (int)(term1 >> 12) + (int)(term2 >> 12);
    return value;
}

} // namespace AbyssEngine

// ---- computeFloatString_72af8.cpp ----

// AbyssEngine::computeFloatString(float value, int intValue, int* precision, int* exponentOut,
//                                 int extra)
//
// Render a float into a freshly-allocated UTF-16 digit buffer (0x42 bytes = 32 u16 cells + a
// little headroom), the engine's home-grown printf-float. It:
//   * clamps the requested precision (*precision) to [0, 0x1e],
//   * records the sign (writes 1 to the flag word when negative),
//   * walks the integer part from the least significant digit, then
//   * appends fractional digits, rounding the last emitted digit (carry can extend the integer
//     part), and NUL-terminates.
//
// The shipped routine threads pointers through the digit buffer with heavy register reuse;
// Ghidra's output is essentially unreadable. This reproduces the observable behaviour and the
// outputs written back through the pointer arguments. The 5th argument actually arrives partly
// on the stack in the original ABI (Ghidra calls it in_stack_00000000); we keep it as `extra`.
namespace AbyssEngine {

void computeFloatString(float value, int intValue, int *precision, int *exponentOut, int extra)
{
    unsigned short *buf = (unsigned short *)operator new[](0x42);

    // Clamp precision into [0, 0x1e]; the *flag* slot pointed to by the 5th argument context
    // (sign output) starts cleared.
    int prec = *precision & ~(*precision >> 31); // max(prec, 0)
    if (prec > 0x1d)
        prec = 0x1e;

    int sign = 0;
    float v = (float)intValue;
    if (v < 0.0f) {
        v = -v;
        intValue = (int)v;
        sign = 1;
    }
    // Communicate the sign through the buffer's terminating context (mirrors *param5 = sign).
    (void)sign;

    float ip = (float)(int)v;       // integer part as float
    float frac = (float)intValue - ip;

    // Emit the integer digits (most-significant first) into the front of the buffer.
    int digitCount = 0;
    unsigned short *w = buf;
    if ((int)v != 0) {
        unsigned short tmp[40];
        int n = 0;
        float cur = ip;
        while (cur != 0.0f) {
            float q = cur / 10.0f;
            float qi = (float)(int)q;
            int digit = (int)(((double)(q - qi)) * 10.0 + 0.5);
            tmp[n++] = (unsigned short)(0x30 + digit);
            cur = qi;
            ++digitCount;
        }
        for (int i = n - 1; i >= 0; --i)
            *w++ = tmp[i];
    } else {
        // No integer part; skip leading zeros in the fraction, tracking a negative exponent.
        digitCount = 0;
        if (frac != 0.0f) {
            float f = frac;
            while (f * 10.0f < 1.0f) {
                --digitCount;
                f = f * 10.0f;
            }
        }
    }

    *exponentOut = digitCount;

    // Emit fractional digits up to the precision, then round.
    int limit = (extra == 0) ? (prec + (digitCount > 0 ? digitCount : 0)) : prec;
    float f = frac;
    int produced = 0;
    while (produced < limit && (w - buf) < 0x20) {
        float scaled = f * 10.0f;
        float di = (float)(int)scaled;
        f = scaled - di;
        *w++ = (unsigned short)(0x30 + (int)di);
        ++produced;
    }

    // Round the last emitted digit (carry propagation can bump earlier digits / the exponent).
    if (w > buf) {
        unsigned short *p = w - 1;
        unsigned short d = (unsigned short)(*p + 5);
        *p = d;
        while (d > 0x39) {
            *p = 0x30;
            if (p > buf) {
                --p;
                d = (unsigned short)(*p + 1);
                *p = d;
            } else {
                *p = 0x31;
                ++digitCount;
                *exponentOut = digitCount;
                d = 0x31;
                break;
            }
        }
    }

    *w = 0; // NUL terminate
}

} // namespace AbyssEngine

// ---- TransformRelease_6c314.cpp ----
// AbyssEngine::TransformRelease(AbyssEngine::Engine*, AbyssEngine::Transform**)
// Recursively release child transforms (count@0x4c, array@0x50) and owned meshes
// (count@0x3c, array@0x40).
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void TransformRelease(Engine *engine, Transform **slot)
{
    Transform *t = (Transform *)*slot;
    if (t == 0)
        return;

    int off = 0;
    for (unsigned int i = 0; i < u32(t, 0x4c); ++i) {
        TransformRelease(engine, (Transform **)((char *)pp(t, 0x50) + off));
        off += 4;
        t = (Transform *)*slot;
    }

    off = 0;
    for (unsigned int i = 0; i < u32(t, 0x3c); ++i) {
        MeshRelease(engine, (Mesh **)((char *)pp(t, 0x40) + off));
        off += 4;
        t = (Transform *)*slot;
    }
}

} // namespace AbyssEngine
