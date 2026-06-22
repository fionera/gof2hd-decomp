#include "engine/core/AbyssEngine.h"
#include "externs.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "platform/gl.h"
#include <cstdlib>

extern "C" {
void *__aeabi_memcpy(void *dst, const void *src, size_t n);

int __aeabi_uidiv(int num, int den);
}

namespace AbyssEngine {
    class FBOContainer {
    public:
        FBOContainer(Engine *engine, String name);

        void Create(int width, int height, bool a, bool linear);
    };
}

namespace {
    struct EngineArrayHeader {
        uint32_t count;
        void *data;
        uint32_t capacity;
    };

    template<class T>
    __attribute__ ((always_inline))

    inline void ArrayAddRaw(T item, void *arrayHeader) {
        EngineArrayHeader *a = (EngineArrayHeader *) arrayHeader;
        a->capacity = a->count + 1;
        a->data = realloc(a->data, (a->count + 1) * sizeof(T));
        ((T *) a->data)[a->count] = item;
        a->count = a->capacity;
    }

    template<class T>
    inline void ArrayAddCachedRaw(T item, void *arrayHeader) {
        EngineArrayHeader *a = (EngineArrayHeader *) arrayHeader;
        if (a->count >= a->capacity) {
            uint32_t oldCap = a->capacity;
            a->data = realloc(a->data, (size_t) oldCap * 2 * sizeof(T));
            __aeabi_memclr4((char *) a->data + oldCap * sizeof(T), oldCap * sizeof(T));
            a->capacity = oldCap * 2;
        }
        ((T *) a->data)[a->count] = item;
        a->count = a->count + 1;
    }
}

unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label) {
    (void) name;
    (void) label;
    return glIdentifier;
}

namespace AbyssEngine {
    void ImageFontSetYOffset(ImageFont *font, short yOffset) {
        if (font != 0)
            s16(font, 0x12) = yOffset;
    }
}

namespace AbyssEngine {
    int CameraIsPointinViewFrustum(const Vector &point, Matrix *extra, Camera *cam) {
        if (*g_Camera_frustumEnabledFlag == 0)
            return 1;

        char *c = (char *) cam;
        Matrix local;
        Matrix transformed;
        Vector pos, dir, axis;
        Vector camPoint = {0.0f, 0.0f, 0.0f};

        Matrix *srcMatrix = (Matrix *) (c + 0xc);
        Matrix *dstMatrix;
        if (extra == 0) {
            dstMatrix = &local;
        } else {
            __aeabi_memcpy(&local, srcMatrix, 0x3c);
            local *= *extra;
            dstMatrix = &transformed;
            srcMatrix = &local;
        }

        *(Vector *) dstMatrix = AEMath::MatrixInverseTransformVector(*srcMatrix, *(Vector *) dstMatrix);
        camPoint = *(Vector *) dstMatrix;

        pos = AEMath::MatrixGetPosition(*dstMatrix);
        pos -= point;
        dir = AEMath::MatrixGetDir(*dstMatrix);
        axis -= dir;
        Vector normAxis = AEMath::VectorNormalize(axis);
        float fwd = AEMath::VectorDot(pos, normAxis);

        if (fwd > f32(c, 0x8) || fwd < f32(c, 0x4))
            return 0;

        axis = AEMath::MatrixGetUp(*dstMatrix);
        normAxis = AEMath::VectorNormalize(axis);
        float up = AEMath::VectorDot(pos, normAxis);

        float hLimit = fwd * f32(c, 0x48);
        if (up > hLimit || up < -hLimit)
            return 0;

        axis = AEMath::MatrixGetRight(*dstMatrix);
        normAxis = AEMath::VectorNormalize(axis);
        float right = AEMath::VectorDot(pos, normAxis);

        float vLimit = hLimit * f32(c, 0x50);
        return (right <= vLimit && right >= -vLimit) ? 1 : 0;
    }
}

namespace AbyssEngine {
    Quaternion operator*(const Quaternion &a, float s) {
        float yv = a.y * s;
        float wv = a.w * s;
        return Quaternion(wv, 0.0f, yv, 0.0f);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const String &b) {
        String result(a);
        result += b;
        return result;
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys) {
        if (sys == 0)
            return;

        unsigned short count = u16(sys, 0x0);
        char *uvBase = (char *) pp(pp(sys, 0x8), 0x8);

        for (unsigned int idx = 0; (idx & 0xffff) < (unsigned int) (count << 3); idx += 8) {
            float *p = (float *) (uvBase + (idx & 0xffff) * 4);
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
}

extern "C" {
void glViewport(int, int, int, int);

void glEnable(unsigned int);

void glDisable(unsigned int);

void glLineWidth(float);

void glCullFace(unsigned int);

void glGetIntegerv(unsigned int, void *);
}

namespace AbyssEngine {
    void getAppVersion() {
        Engine *self = AE_getInitGLThis();
        char *c = (char *) self;
        int width = AE_getInitGLWidth();
        int height = AE_getInitGLHeight();

        self->fboContainer = 0;
        self->framebufferWidth = width;
        self->framebufferHeight = height;
        self->viewportWidth = width;
        self->viewportHeight = height;

        FileInterfaceAndroid *fileIface = new FileInterfaceAndroid();
        self->fileInterface = fileIface;
        AEFile::SetInterface((FileInterface *) fileIface);

        self->lastGlError = 0;
        u8(c, 0x2c) = 0;
        u8(c, 0x480) = 0;

        char *shaderFlag = g_Engine_shaderModeFlag;
        u32(c, 0x40c) = 0;
        self->ResetLightParam();
        glViewport(0, 0, self->viewportHeight, self->viewportWidth);

        if (*shaderFlag == 0) {
            glEnable(0x803a);
            glDisable(0xb50);
            glLineWidth(1.0f);
        } else {
            self->ShaderInit();
        }

        float zero3[3] = {0, 0, 0};
        *(Vector *) (c + 0x468) = *(const Vector *) zero3;
        u32(c, 0x378) = 0;
        *(Vector *) (c + 0x474) = *(const Vector *) zero3;
        u32(c, 0x37c) = 0;

        glEnable(0xb71);
        self->GlEnable(0xde1, true);
        glDisable(0xbe2);
        glCullFace(0x405);
        glEnable(0xb44);
        self->AfterGLInit();
        ((PaintCanvas *) *(void **) self->paintCanvas)->Initialize(false);

        self->maxTextureSize = 0;
        glGetIntegerv(0xd33, &self->maxTextureSize);

        if (*shaderFlag != 0 && *g_Engine_fboEnabledFlag != 0) {
            FBOContainer *fbo = new FBOContainer(self, String(""));
            self->fboContainer = fbo;
            fbo->Create((int) self->framebufferWidth, (int) self->framebufferHeight, true, false);
        }
    }
}

namespace AbyssEngine {
    void SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d, SpriteSystem *sys) {
        unsigned int count = 0;
        if (sys != 0)
            count = u16(sys, 0x0);
        if (sys == 0 || idx >= count)
            return;

        char flag = *g_SpriteSystem_uvFlipFlag;
        float *p = (float *) ((char *) pp(pp(sys, 0x8), 0x8) + (unsigned int) idx * 0x20);

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
}

namespace AbyssEngine {
    int ImageFontGetYOffset(ImageFont *font) {
        short v = (font == 0) ? (short) 0 : s16(font, 0x12);
        return (int) v;
    }
}

namespace AbyssEngine {
    Quaternion operator-(const Quaternion &a, const Quaternion &b) {
        float yv = a.y - b.y;
        float wv = a.w - b.w;
        return Quaternion(wv, 0.0f, yv, 0.0f);
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int vertexFormat, void **out);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int ImageCreateRegionFromFile(Engine *engine, const char *path, unsigned short index, Image2D *region) {
        if (engine == 0 || path == 0)
            return -4;

        char *r = (char *) region;
        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) (&handle)) == 0)
            return -1;

        char magic[8];
        for (int i = 0; i < 4; ++i) magic[i] = '*';
        if (AEFile::Read((uint32_t)(8), magic, handle) == 0)
            return -1;
        static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
        for (unsigned int k = 0; k < 8; ++k)
            if (want[k] != magic[k])
                return -1;

        unsigned short regionCount = 0;
        if (AEFile::Skip((uint32_t)(1), handle) == 0) return -1;
        if (AEFile::Read((uint32_t)(2), r + 0x08, handle) == 0) return -1;
        if (AEFile::Read((uint32_t)(2), r + 0x0a, handle) == 0) return -1;
        if (AEFile::Read((uint32_t)(2), &regionCount, handle) == 0) return -1;
        if (regionCount <= index) return -1;

        if (MeshCreate(engine, 4, 2, 0x13, (void **) region) != 1)
            return -2;

        unsigned char mode = 0;
        for (unsigned short i = 0; i < regionCount; ++i) {
            if (i == index) {
                if (AEFile::Read((uint32_t)(2), r + 0x0c, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), r + 0x0e, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), r + 0x10, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), r + 0x12, handle) == 0) goto fail;

                Mesh *mesh = *(Mesh **) r;
                float *pos = (float *) mesh->positions;

                float halfW = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x10), mode);
                pos[0] = 0;
                pos[1] = 0;
                pos[2] = 0;
                pos[4] = 0;
                pos[5] = 0;
                pos[3] = halfW;
                pos[6] = halfW;
                float halfH = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x12), mode);
                pos[8] = 0;
                pos[9] = 0;
                pos[11] = 0;
                pos[7] = halfH;
                pos[10] = halfH;

                double atlasH = (double) AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x0a), mode);
                double atlasW = (double) AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x08), mode);
                float offYs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) u16(r, 0x0e), mode);
                float offXs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) u16(r, 0x0c), mode);
                float offYu = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x0e), mode);
                float offXu = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) u16(r, 0x0c), mode);

                float *uv = (float *) mesh->texCoords;
                float u0 = offXu * (float) (1.0 / atlasW);
                float v0 = offYu * (float) (1.0 / atlasH);
                float v1 = (halfH + offYs) * (float) (1.0 / atlasH);
                float u1 = (halfW + offXs) * (float) (1.0 / atlasW);
                uv[0] = u0;
                uv[1] = v0;
                uv[2] = u1;
                uv[3] = v0;
                uv[4] = u1;
                uv[5] = v1;
                uv[6] = u0;
                uv[7] = v1;

                unsigned int *draw = (unsigned int *) mesh->indices;
                draw[0] = 0x20000;
                draw[1] = 1;
                draw[2] = 0;
            } else {
                if (AEFile::Skip((uint32_t)(8), handle) == 0)
                    goto fail;
            }
        }

        AEFile::Close(handle);
        return 1;

    fail:
        MeshRelease(engine, (Mesh **) region);
        return -1;
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int count) {
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
                    goto next;
                idx = idx + 1;
                code = *(unsigned short *) ((char *) pp(font, 0x4) + u * 2);
                if (code == str[i])
                    break;
            }
            {
                unsigned int found = (unsigned int) (unsigned short) (idx - 1);
                Mesh *glyph = *(Mesh **) ((char *) pp(font, 0xc) + found * 4);
                int w = (int) ((float *) glyph->positions)[3];
                int adv = (int) s16(font, 0x10) + w;
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
}

namespace AbyssEngine {
    struct Vec2 {
        float u, v;
    };

    Vec2 MeshIntersect(float qx, float qz, Mesh *mesh) {
        Vec2 out;
        unsigned int i = 0;

        for (;;) {
            if ((unsigned int) mesh->indexCount <= i) {
                out.u = -1.0f;
                out.v = -1.0f;
                return out;
            }

            float *pos = (float *) mesh->positions;
            unsigned short *idx = (unsigned short *) mesh->indices + i;
            float *a = pos + (unsigned int) idx[0] * 3;
            float *b = pos + (unsigned int) idx[1] * 3;
            float *c = pos + (unsigned int) idx[2] * 3;

            float ax = a[0], az = a[2];
            float bx = b[0], bz = b[2];
            float cx = c[0], cz = c[2];

            float minZ = az, maxZ = bz;
            if (bz < az) {
                minZ = bz;
                maxZ = az;
            }
            float minX = ax, maxX = bx;
            if (bx < ax) {
                minX = bx;
                maxX = ax;
            }
            if (cz > maxZ) maxZ = cz;
            if (cz < minZ) minZ = cz;
            if (cx > maxX) maxX = cx;
            if (cx < minX) minX = cx;

            if (maxZ >= qz && minX <= qx && maxX >= qx && minZ <= qz) {
                float ex = bx - ax, ez = bz - az;
                float len = sqrtf(ez * ez + ex * ex);
                float side = (ex / len) * qz + (-ez / len) * qx -
                             (az * (ex / len) + ax * (-ez / len));
                if (side <= 0.0f) {
                    ex = cx - bx;
                    ez = cz - bz;
                    len = sqrtf(ez * ez + ex * ex);
                    side = (ex / len) * qz + (-ez / len) * qx -
                           (bz * (ex / len) + bx * (-ez / len));
                    if (side <= 0.0f) {
                        ez = az - cz;
                        ex = ax - cx;
                        len = sqrtf(ez * ez + ex * ex);
                        side = (ex / len) * qz + (-ez / len) * qx -
                               (cz * (ex / len) + cx * (-ez / len));
                        if (side <= 0.0f) {
                            float *uv = (float *) mesh->texCoords;
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
}

extern "C" {
void glBindBuffer(unsigned int target, unsigned int buffer);

void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);

void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);

void glNormalPointer(unsigned int type, int stride, const void *ptr);

void glColorPointer(int size, unsigned int type, int stride, const void *ptr);

void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
}

namespace AbyssEngine {
    int MeshDraw(Engine *engine, Mesh *mesh) {
        if (mesh == 0 || (short) mesh->vertexCount == 0 || (mesh->vertexFormat & 1) == 0)
            return -4;

        unsigned char flags = mesh->vertexFormat;
        if (*g_Mesh_shaderPathFlag == 0 && mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            engine->AEClientState(0x8074, true);
            glVertexPointer(3, 0x1406, 0, 0);
            glBindBuffer(0x8893, mesh->indexVBO);

            if (flags & 2) {
                glBindBuffer(0x8892, mesh->texCoordVBO);
                engine->AEClientState(0x8078, true);
                glTexCoordPointer(2, 0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8078, false);
            }

            if (flags & 4) {
                glBindBuffer(0x8892, mesh->normalVBO);
                engine->AEClientState(0x8075, true);
                glNormalPointer(0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8075, false);
            }

            if (flags & 8) {
                glBindBuffer(0x8892, mesh->colorVBO);
                engine->AEClientState(0x8076, true);
                glColorPointer(4, 0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8076, false);
            }

            glDrawElements(4, (int) mesh->indexCount, 0x1403, 0);

            if (u8(engine, 0xfe) != 0) {
                int tris = __aeabi_uidiv((int) mesh->indexCount, 3);
                if (u8(engine, 0xfd) == 0) {
                    engine->drawCallCountA += 1;
                    engine->triangleCountA += tris;
                } else {
                    engine->triangleCountB += tris;
                    engine->drawCallCountB += 1;
                }
            }

            glBindBuffer(0x8892, 0);
            glBindBuffer(0x8893, 0);
        } else {
            engine->RenderMesh(mesh);
        }
        return 1;
    }
}

namespace AbyssEngine {
    void SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a, SpriteSystem *sys) {
        unsigned int count = 0;
        if (sys != 0)
            count = u16(sys, 0x0);
        if (sys == 0 || idx >= count)
            return;

        float *p = (float *) ((char *) pp(pp(sys, 0x8), 0xc) + (unsigned int) idx * 0x40);
        for (int v = 0; v < 4; ++v) {
            p[v * 4 + 0] = r;
            p[v * 4 + 1] = g;
            p[v * 4 + 2] = b;
            p[v * 4 + 3] = a;
        }
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    float CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam) {
        float ret = fov;
        if (cam != 0) {
            float *f = (float *) cam;
            f[1] = fov;
            f[2] = aspectNum;

            float s = AbyssEngine::AEMath::Sinf(f[0] * 0.5f);
            float c = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
            float scale = s / c;
            f[0x12] = scale;
            f[0x13] = (aspectDen / near) * scale;
            f[0x14] = aspectDen / near;

            float at = AbyssEngine::AEMath::ATanf((aspectDen / near) * scale);
            float ca = AbyssEngine::AEMath::Cosf(at);
            ret = 1.0f / ca;
            f[0x15] = ret;
        }
        return ret;
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start,
                          unsigned int len) {
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
                code = *(unsigned short *) ((char *) pp(font, 0x4) + u * 2);
                if (code == str[start & 0xffff]) {
                    found = true;
                    break;
                }
            }
            if (!found)
                continue;

            unsigned int gi = (unsigned int) (unsigned short) (idx - 1);
            Mesh *glyph = *(Mesh **) ((char *) pp(font, 0xc) + gi * 4);
            int w = (int) ((float *) glyph->positions)[3];
            int adv = (int) s16(font, 0x10) + w;
            int contrib = adv;
            if (w == 0xb)
                contrib = adv - 2;
            if (code != 0x20)
                contrib = adv;
            total += contrib;
        }
        return total;
    }
}

namespace AbyssEngine {
    void CurveRelease(Curve **slot) {
        Curve *curve = (Curve *) *slot;
        if (curve == 0)
            return;

        void *data = 0;
        for (unsigned int i = 0; (data = pp(curve, 0x4)), i < (unsigned int) u16(curve, 0x0); ++i) {
            char *entry = *(char **) ((char *) data + i * 4);
            char tag = entry[0];
            if (tag == 3 || tag == 2)
                operator delete(entry);
            else if (tag == 1)
                operator delete(entry);
            curve = (Curve *) *slot;
        }

        if (data != 0)
            operator delete[](data);
        curve = (Curve *) *slot;
        pp(curve, 0x4) = 0;

        if (*slot != 0)
            operator delete((void *) *slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    int MeshDraw(Engine * engine, Mesh * mesh);

    void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures) {
        if (canvas == 0 || mat == 0)
            return;

        char *m = (char *) mat;

        if (setTextures) {
            engine->SetTexturesExt(u32(m, 0x0), u32(m, 0x4), u32(m, 0x8), 0xffffffff);
        }
        engine->SetAddData(pp(m, 0x24), i32(m, 0x28));

        float ambient = f32(m, 0x68);
        if (ambient != -10.0f) {
            engine->LightSetGlobalSceneColorAmbient(ambient, 0.0f, 0.0f);
        }

        const float inv255 = 1.0f / 255.0f;
        int matOff = 0;
        for (unsigned int i = 0; i < u32(m, 0x44); ++i) {
            Matrix world;
            AE_AEMath_matMul(&world, (const Matrix *) ((char *) pp(m, 0x60) + matOff));
            ((PaintCanvas *) canvas)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) canvas);
            engine->SetModelMatrix(world);
            engine->SetUVMatrix(F<Matrix>(m, 0x3c + matOff));

            unsigned int packed = *(unsigned int *) ((char *) pp(m, 0x54) + i * 4);
            float ca = (float) ((packed >> 24) & 0xff);
            float cr = (float) ((packed >> 16) & 0xff);
            float cg = (float) ((packed >> 8) & 0xff);
            float cb = (float) (packed & 0xff);
            engine->SetColor(ca * inv255, cr * inv255, cg * inv255, cb * inv255);

            MeshDraw(engine, *(Mesh **) ((char *) pp(m, 0x48) + i * 4));
            matOff += 0x3c;
        }

        if (f32(m, 0x68) != -10.0f) {
            engine->LightSetGlobalSceneColorAmbient(f32(m, 0x68), 0.0f, 0.0f);
        }

        i32(m, 0x2c) = 0;
        i32(m, 0x5c) = 0;
        i32(m, 0x38) = 0;
        i32(m, 0x44) = 0;
        i32(m, 0x50) = 0;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void ImageFontRelease(Engine *engine, ImageFont **slot) {
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
            ImageFont *f = (ImageFont *) *slot;
            glyphs = pp(f, 0xc);
            if (u16(f, 0x0) <= i)
                break;
            MeshRelease(engine, (Mesh **) ((char *) glyphs + off));
            off += 4;
            ++i;
        }

        if (glyphs != 0)
            operator delete[](glyphs);
        pp(*slot, 0xc) = 0;

        if (*slot != 0)
            operator delete((void *) *slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    extern "C" {
    char *g_uvFlipFlag;
    char *g_tangentEnabled;
    }

    int MeshReadData(Engine *engine, const unsigned int &handleRef, unsigned int flags, Mesh **slot,
                     Material *mat);

    int MeshReadData(Engine *engine, const unsigned int &handleRef, unsigned int flags, Mesh **slot,
                     Material *mat) {
        unsigned int handle = handleRef;
        unsigned int subBit = flags & 0x1a;
        unsigned char mode = 0;

        if (subBit != 0) {
            if (AEFile::Read((uint32_t)(0xc), &(*slot)->pivotX, handle) == 0)
                return -1;
        }

        Mesh *m = *slot;

        if (m->vertexFormat & 0x10) {
            if (AEFile::Read((uint32_t)(2), &m->indexCount, handle) == 0)
                return -1;
            void *idx = ::operator new[]((unsigned int) m->indexCount << 1);
            (*slot)->indices = idx;
            if (AEFile::Read((uint32_t)((unsigned int) (*slot)->indexCount << 1), (*slot)->indices, handle) == 0)
                return -1;
            m = *slot;
        }

        if (AEFile::Read((uint32_t)(2), &m->vertexCount, handle) == 0)
            return -1;

        float minv[3] = {1e30f, 1e30f, 1e30f};
        float maxv[3] = {-1e30f, -1e30f, -1e30f};
        bool compressedPos = (int) (flags << 0x1d) < 0;
        unsigned int vcount = (*slot)->vertexCount;

        if (compressedPos) {
            void *raw = ::operator new[](vcount * 6);
            if (AEFile::Read((uint32_t)(vcount * 6), raw, handle) == 0) {
                ::operator delete[](raw);
                return -1;
            }
            m = *slot;
            void *pos = ::operator new[](vcount * 0xc);
            m->positions = pos;
            m = *slot;
            int dst = 0;
            unsigned int n3 = vcount * 3;
            for (unsigned int i = 0; i < n3; ++i) {
                int axis = (int) __aeabi_uidiv((int) i, 3);
                axis = (int) i - axis * 3;
                float v = AbyssEngine::AEMath::VectorSignedToFloat((int) *(short *) ((char *) raw + i * 2), mode);
                *(float *) ((char *) m->positions + dst) = v;
                dst += 4;
                if (v < minv[axis]) minv[axis] = v;
                if (maxv[axis] < v) maxv[axis] = v;
            }
            ::operator delete[](raw);
        } else if ((flags & 3) == 0) {
            if ((flags & 0x18) != 0) {
                m = *slot;
                void *pos = ::operator new[](vcount * 0xc);
                m->positions = pos;
                if (AEFile::Read((uint32_t)((*slot)->vertexCount * 0xc), (*slot)->positions, handle) == 0)
                    return -1;
            }
        } else {
            void *raw = ::operator new[](vcount * 0xc);
            if (AEFile::Read((uint32_t)(vcount * 0xc), raw, handle) == 0) {
                ::operator delete[](raw);
                return -1;
            }
            m = *slot;
            void *pos = ::operator new[](vcount * 0xc);
            m->positions = pos;
            m = *slot;
            int dst = 0;
            unsigned int n3 = vcount * 3;
            for (unsigned int i = 0; i < n3; ++i) {
                int axis = (int) __aeabi_uidiv((int) i, 3);
                axis = (int) i - axis * 3;
                float v = AbyssEngine::AEMath::VectorSignedToFloat(*(int *) ((char *) raw + i * 4), mode);
                *(float *) ((char *) m->positions + dst) = v;
                dst += 4;
                if (v < minv[axis]) minv[axis] = v;
                if (maxv[axis] < v) maxv[axis] = v;
            }
            ::operator delete[](raw);
        }

        float center[3];
        center[0] = (maxv[0] + minv[0]) * 0.5f;
        center[1] = (maxv[1] + minv[1]) * 0.5f;
        center[2] = (maxv[2] + minv[2]) * 0.5f;
        *(Vector *) &(*slot)->boundsCenterX = *(const Vector *) center;
        float halfDiag[3] = {minv[0], minv[1], minv[2]};
        *(Vector *) center -= *(const Vector *) halfDiag;
        (*slot)->boundsRadius = AEMath::VectorLength(*(const Vector *) center);

        m = *slot;

        if (m->vertexFormat & 2) {
            if (compressedPos) {
                void *raw = ::operator new[](vcount << 2);
                if (AEFile::Read((uint32_t)(vcount << 2), raw, handle) == 0) {
                    ::operator delete[](raw);
                    return -1;
                }
                m = *slot;
                void *uv = ::operator new[](vcount << 3);
                m->texCoords = uv;
                m = *slot;
                char flip = *g_uvFlipFlag;
                int dst = 0;
                const double scale = 1.0 / 32767.0;
                for (unsigned int i = 0; i < (vcount << 1); i += 2) {
                    double u = (double) AbyssEngine::AEMath::VectorSignedToFloat(
                                   (int) *(short *) ((char *) raw + i * 2), mode) * scale;
                    float *p = (float *) ((char *) m->texCoords + dst);
                    dst += 8;
                    p[0] = (float) u;
                    double v = (double) AbyssEngine::AEMath::VectorSignedToFloat(
                                   (int) *(short *) ((char *) raw + i * 2 + 2), mode) * scale;
                    double vv = (flip == 0) ? v : (1.0 - v);
                    p[1] = (float) vv;
                }
                ::operator delete[](raw);
            } else if ((flags & 0x18) != 0) {
                void *uv = ::operator new[](vcount << 3);
                m->texCoords = uv;
                if (AEFile::Read((uint32_t)((*slot)->vertexCount << 3), (*slot)->texCoords, handle) == 0)
                    return -1;
                if (*g_uvFlipFlag != 0) {
                    m = *slot;
                    int off = 4;
                    for (unsigned int i = 0; i < (unsigned int) (m->vertexCount << 1); i += 2) {
                        float *p = (float *) ((char *) m->texCoords + off);
                        off += 8;
                        *p = 1.0f - *p;
                    }
                }
            }
        }

        m = *slot;

        if (m->vertexFormat & 4) {
            if (compressedPos) {
                void *raw = ::operator new[](vcount * 6);
                if (AEFile::Read((uint32_t)(vcount * 6), raw, handle) == 0) {
                    ::operator delete[](raw);
                    return -1;
                }
                m = *slot;
                void *nrm = ::operator new[](vcount * 0xc);
                m->normals = nrm;
                m = *slot;
                const double scale = 1.0 / 32767.0;
                short *s = (short *) raw;
                int off = 4;
                for (unsigned int i = 0; i < vcount * 3; i += 3) {
                    float nx = (float) ((double) AbyssEngine::AEMath::VectorSignedToFloat((int) s[0], mode) * scale);
                    float ny = (float) ((double) AbyssEngine::AEMath::VectorSignedToFloat((int) s[1], mode) * scale);
                    float nz = (float) ((double) AbyssEngine::AEMath::VectorSignedToFloat((int) s[2], mode) * scale);
                    float len2 = nx * nx + ny * ny + nz * nz;
                    float len = sqrtf(len2);
                    char *base = (char *) m->normals;
                    if (len != 0.0f) {
                        nx /= len;
                        ny /= len;
                        nz /= len;
                        if (nx < -1.0f) nx = -1.0f;
                        if (nx > 1.0f) nx = 1.0f;
                        if (ny < -1.0f) ny = -1.0f;
                        if (ny > 1.0f) ny = 1.0f;
                        if (nz < -1.0f) nz = -1.0f;
                        if (nz > 1.0f) nz = 1.0f;
                        ((float *) (base + off))[-1] = ny;
                        *(float *) (base + off) = nx;
                        *(float *) (base + off + 4) = nz;
                    } else {
                        *(unsigned int *) (base + off) = 0x3f800000;
                        *(unsigned int *) (base + off - 4) = 0;
                        *(float *) (base + off + 4) = 1.0f;
                    }
                    s += 3;
                    off += 0xc;
                    m = *slot;
                }
                ::operator delete[](raw);
            } else if ((flags & 0x18) != 0) {
                void *nrm = ::operator new[](vcount * 0xc);
                m->normals = nrm;
                if (AEFile::Read((uint32_t)((*slot)->vertexCount * 0xc), (*slot)->normals, handle) == 0)
                    return -1;
            }

            if (*g_tangentEnabled != 0) {
                m = *slot;
                void *tan = ::operator new[](vcount * 0xc);
                m->tangents = tan;
                m = *slot;
                void *bin = ::operator new[](vcount * 0xc);
                m->binormals = bin;
                m = *slot;

                unsigned int triCount = (unsigned int) __aeabi_uidiv((int) (unsigned short) m->indexCount, 3);
                void *accum = ::operator new[](vcount * 0xc);

                for (unsigned int b = 0; b < vcount * 0xc; ++b) ((char *) accum)[b] = 0;

                int triOff = 0;
                for (unsigned int t = 0; t < triCount; ++t) {
                    m = *slot;
                    char *posBase = (char *) m->positions;
                    char *uvBase = (char *) m->texCoords;
                    char *idxBase = (char *) m->indices + triOff;
                    unsigned int i0 = (unsigned int) *(unsigned short *) (idxBase);
                    unsigned int i2 = (unsigned int) *(unsigned short *) (idxBase + 4);
                    unsigned int i1 = (unsigned int) *(unsigned short *) (idxBase + 2);

                    float uv0v = *(float *) (uvBase + (i0 << 3 | 4));
                    float uv0u = *(float *) (uvBase + i0 * 8);
                    float dv1 = *(float *) (uvBase + (i2 << 3 | 4)) - uv0v;
                    float dv2 = *(float *) (uvBase + (i1 << 3 | 4)) - uv0v;
                    float *p0 = (float *) (posBase + i0 * 0xc);
                    float *p2 = (float *) (posBase + i2 * 0xc);
                    float *p1 = (float *) (posBase + i1 * 0xc);
                    float denom = (*(float *) (uvBase + i1 * 8) - uv0u) * dv1 -
                                  (*(float *) (uvBase + i2 * 8) - uv0u) * dv2;
                    float r = (denom != 0.0f) ? (1.0f / denom) : 0.0f;
                    float tng[3];
                    tng[1] = ((p1[1] - p0[1]) * dv1 - (p2[1] - p0[1]) * dv2) * r;
                    tng[0] = ((p1[0] - p0[0]) * dv1 - (p2[0] - p0[0]) * dv2) * r;
                    tng[2] = ((p1[2] - p0[2]) * dv1 - (p2[2] - p0[2]) * dv2) * r;
                    float *a = (float *) ((char *) accum + i0 * 0xc);
                    a[0] += tng[0];
                    a[1] += tng[1];
                    a[2] += tng[2];
                    a = (float *) ((char *) accum + i1 * 0xc);
                    a[0] += tng[0];
                    a[1] += tng[1];
                    a[2] += tng[2];
                    a = (float *) ((char *) accum + i2 * 0xc);
                    a[0] += tng[0];
                    a[1] += tng[1];
                    a[2] += tng[2];
                    triOff += 6;
                }

                int off = 0;
                for (unsigned int v = 0; v < vcount; ++v) {
                    m = *slot;
                    char *nrmBase = (char *) m->normals;
                    float nrm[3];
                    nrm[0] = *(float *) (nrmBase + off);
                    nrm[1] = *(float *) (nrmBase + off + 4);
                    nrm[2] = *(float *) (nrmBase + off + 8);
                    float tg[3];
                    tg[0] = *(float *) ((char *) accum + off);
                    tg[1] = *(float *) ((char *) accum + off + 4);
                    tg[2] = *(float *) ((char *) accum + off + 8);
                    float d = AEMath::VectorDot(*(const Vector *) nrm, *(const Vector *) tg);
                    float scaled[3] = {nrm[0], nrm[1], nrm[2]};
                    *(Vector *) scaled *= d;
                    *(Vector *) tg -= *(const Vector *) scaled;
                    float tanOut[3];
                    *(Vector *) tanOut = AEMath::VectorNormalize(*(const Vector *) tg);
                    char *tb = (char *) (*slot)->tangents + off;
                    *(float *) (tb) = tanOut[0];
                    *(float *) (tb + 4) = tanOut[1];
                    *(float *) (tb + 8) = tanOut[2];
                    float binOut[3] = {tg[0], tg[1], tg[2]};
                    *(Vector *) binOut = AEMath::VectorCross(*(const Vector *) binOut, *(const Vector *) nrm);
                    char *bb = (char *) (*slot)->binormals + off;
                    *(float *) (bb) = binOut[0];
                    *(float *) (bb + 4) = binOut[1];
                    *(float *) (bb + 8) = binOut[2];
                    off += 0xc;
                }
                ::operator delete[](accum);
            }
        }

        m = *slot;

        if (m->vertexFormat & 8) {
            if (compressedPos) {
                void *raw = ::operator new[](vcount << 2);
                if (AEFile::Read((uint32_t)(vcount << 2), raw, handle) == 0) {
                    ::operator delete[](raw);
                    return -1;
                }
                m = *slot;
                void *col = ::operator new[](vcount << 4);
                m->colors = col;
                m = *slot;
                const float inv = 255.0f;
                int dst = 0;
                for (unsigned int i = 0; i < (vcount << 2); ++i) {
                    float c = AbyssEngine::AEMath::VectorUnsignedToFloat(
                        (unsigned int) *((unsigned char *) raw + i), mode);
                    *(float *) ((char *) m->colors + dst) = c / inv;
                    dst += 4;
                }
                ::operator delete[](raw);
            } else if ((flags & 0x18) != 0) {
                void *col = ::operator new[](vcount << 4);
                m->colors = col;
                if (AEFile::Read((uint32_t)((*slot)->vertexCount << 4), (*slot)->colors, handle) == 0)
                    return -1;
            }
        }

        if (subBit != 0) {
            if ((*slot)->ReadEnhancedDataFromFile(handle, flags) == 0)
                return -1;
            unsigned short childCount = 0;
            if (AEFile::Read((uint32_t)(2), &childCount, handle) == 0)
                return -1;
            Transform *xf = (*slot)->animation;
            if (xf != 0)
                ((AEMath::BSphere *) &(*slot)->boundsCenterX)->Merge(xf->bounds());
            for (unsigned int c = 0; c < childCount; ++c) {
                char *child = (char *) ::operator new(0x88);
                for (int b = 0; b < 0x88; ++b) child[b] = 0;
                Mesh *childPtr = (Mesh *) child;
                childPtr->boundsRadiusSq = 1.0f;
                childPtr->vboEligible = 1;
                childPtr->vertexFormat = (*slot)->vertexFormat;
                childPtr->material = (*slot)->material;
                if (MeshReadData(engine, handleRef, flags, &childPtr, mat) == -1)
                    return -1;
                ((AEMath::BSphere *) &(*slot)->boundsCenterX)->Merge(
                    *(const AEMath::BSphere *) &childPtr->boundsCenterX);
                ArrayAddRaw<Mesh *>(childPtr, &(*slot)->animation->meshes);
            }
        }

        return 1;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const long long &b) {
        String result(a);
        String num;
        num.ctor_longlong(b);
        result += num;
        return result;
    }
}

namespace AbyssEngine {
    void TransformRelease(Engine * engine, Transform * *slot);

    void MeshRelease(Engine *engine, Mesh **slot) {
        if (engine != 0 && *slot != 0) {
            TransformRelease(engine, &(*slot)->animation);

            typedef void (*FreeFn)(Engine *, Mesh **);
            extern void *g_MeshRelease_freeFn;
            ((FreeFn) g_MeshRelease_freeFn)(engine, slot);
        }
    }
}

namespace AbyssEngine {
    String operator+(const float &a, const String &b) {
        String result;
        result.ctor_float(a);
        result += b;
        return result;
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int fmt, void **out);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out) {
        unsigned int n = count;
        if (n == 0)
            return -4;

        char *sys = (char *) operator new(0x14);
        pp(sys, 0x4) = 0;
        pp(sys, 0x8) = 0;
        pp(sys, 0x10) = 0;
        *out = (SpriteSystem *) sys;
        u16(sys, 0x0) = count;

        unsigned int triCount = (n << 1) & 0xffff;
        int rc = MeshCreate(engine, (unsigned short) ((n & 0x3fff) << 2), (unsigned short) triCount,
                            0x1f, (void **) (sys + 0x10));
        char *s = (char *) *out;
        if (rc != 1) {
            MeshRelease(engine, (Mesh **) (s + 0x10));
            if (pp(*out, 0x4) != 0)
                operator delete[](pp(*out, 0x4));
            pp(*out, 0x4) = 0;
            if (pp(*out, 0x8) != 0)
                operator delete[](pp(*out, 0x8));
            pp(*out, 0x8) = 0;
            if (*out != 0)
                operator delete((void *) *out);
            *out = 0;
            return -1;
        }

        Mesh *mesh = (Mesh *) pp(s, 0x10);
        int *indexArr = (int *) mesh->indices;

        void *posCpu = operator new[](n * 0xc);
        pp(s, 0x4) = posCpu;
        __aeabi_memclr4(pp(*out, 0x4), n * 0xc);

        s = (char *) *out;
        u8(s, 0xc) = sharedSize ? 1 : 0;
        if (sharedSize) {
            void *sz = operator new[](2);
            pp(s, 0x8) = sz;
            *(unsigned short *) pp(*out, 0x8) = 0;
        } else {
            void *sz = operator new[]((n << 1));
            pp(s, 0x8) = sz;
            __aeabi_memclr(pp(*out, 0x8), (n << 1));
        }

        unsigned short base = 0;
        char *idx = (char *) indexArr;
        for (unsigned int off = 0; (off & 0xffff) < n * 6; off += 6) {
            unsigned short *p = (unsigned short *) (idx + (off & 0xffff) * 2);
            p[0] = base;
            p[1] = base | 1;
            p[2] = base | 2;
            p[3] = base;
            p[4] = base | 2;
            p[5] = base | 3;
            base = base + 4;
        }

        Mesh *m = (Mesh *) pp(*out, 0x10);
        unsigned int vcount = (unsigned int) (unsigned short) m->vertexCount;

        float *colors = (float *) m->colors;
        for (unsigned int i = 0; i <= vcount * 4 && vcount * 4 - i != 0; ++i)
            colors[i] = 1.0f;

        char tangent = *g_SpriteSystem_tangentFlag;
        int vo = 0;
        char *normals = (char *) m->normals;
        char *tangents = (char *) m->tangents;
        char *binormals = (char *) m->binormals;
        for (unsigned int k = vcount; k != 0; --k) {
            float *nrm = (float *) (normals + vo);
            nrm[0] = 0.0f;
            nrm[1] = 0.0f;
            nrm[2] = 1.0f;
            if (tangent != 0) {
                float *tan = (float *) (tangents + vo);
                tan[0] = 1.0f;
                tan[1] = 0.0f;
                tan[2] = 0.0f;
                float *bin = (float *) (binormals + vo);
                bin[0] = 0.0f;
                bin[1] = 1.0f;
                bin[2] = 0.0f;
            }
            vo += 0xc;
        }

        return 1;
    }
}

namespace AbyssEngine {
    void ImageRelease(Image * *slot);

    int ImageCreateFromFile(Engine *engine, const char *path, Image **out) {
        if (engine == 0 || path == 0)
            return -4;

        char *img = (char *) ::operator new(0x14);
        u8(img, 0x8) = 0;
        u32(img, 0x0) = 0;
        u32(img, 0x4) = 0;
        u32(img, 0xc) = 0;
        *out = (Image *) img;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) (&handle)) == 0) {
            if (*out != 0)
                ::operator delete((void *) *out);
            *out = 0;
            return -1;
        }

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
            char *im = (char *) *out;
            unsigned char fmt = 0;
            unsigned short palCount = 0;
            if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), im + 0x0, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), im + 0x2, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &palCount, handle) == 0) goto fail;

            AEFile::Skip((uint32_t)((unsigned int) palCount << 3), handle);

            if (fmt & 2)
                u8(im, 0x8) = 1;

            unsigned int dataLen = 0;
            switch (fmt) {
                case 1:
                case 3:
                case 0x81: {
                    unsigned int sz = (unsigned int) u16(im, 0x0) * (unsigned int) u16(im, 0x2) * 4;
                    void *p = ::operator new[](sz);
                    pp(im, 0xc) = p;
                    im = (char *) *out;
                    if (AEFile::Read((uint32_t)(sz), pp(im, 0xc), handle) == 0) goto fail;
                    im = (char *) *out;
                    u32(im, 0x10) = sz;
                    {
                        unsigned int v = (unsigned int) (int) (signed char) fmt;
                        unsigned int code = ((int) v < 0) ? ((v > 0x7fffffff) ? 6u : v) : 3u;
                        if ((int) v >= 0) code = 3;
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

            AEFile::Close(handle);
            return 1;
        }

    fail:
        ImageRelease(out);
        AEFile::Close(handle);
        return -1;
    }
}

namespace AbyssEngine {
    void ImageRelease(Image **slot) {
        if (*slot != 0) {
            operator delete[](pp(*slot, 0xc));
            pp(*slot, 0xc) = 0;
            if (*slot != 0)
                operator delete((void *) *slot);
            *slot = 0;
        }
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void Image2DRelease(Engine *engine, Image2D **slot) {
        if (*slot != 0) {
            MeshRelease(engine, (Mesh **) *slot);
            if (*slot != 0)
                operator delete((void *) *slot);
            *slot = 0;
        }
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine * /*engine*/, unsigned short vertexCount, unsigned short triCount,
                   signed char vertexFormat, Mesh **out) {
        if (vertexCount < 4 || triCount == 0 || (vertexFormat & 1) == 0)
            return -4;

        Mesh *m = (Mesh *) operator new(0x88);

        __aeabi_memclr4(m, 0x88);
        m->boundsRadiusSq = 1.0f;

        *out = m;
        m->vertexCount = (short) vertexCount;
        m->vertexFormat = vertexFormat;
        m->indexCount = (short) (triCount + (triCount << 1));
        m->field_0x2a = (short) triCount;

        unsigned int posBytes = (unsigned int) vertexCount * 0xc;
        void *p = operator new[](posBytes);
        m->positions = p;
        __aeabi_memclr4(p, posBytes);

        if (vertexFormat & 0x10) {
            p = operator new[]((unsigned int) triCount * 6);
            m->indices = p;
            __aeabi_memclr(p, (unsigned int) triCount * 6);
        }
        if (vertexFormat & 2) {
            p = operator new[]((unsigned int) vertexCount << 3);
            m->texCoords = p;
            __aeabi_memclr4(p, (unsigned int) vertexCount << 3);
        }
        if (vertexFormat & 4) {
            p = operator new[](posBytes);
            m->normals = p;
            __aeabi_memclr4(p, posBytes);
            if (*g_Mesh_tangentEnabledFlag != 0) {
                p = operator new[](posBytes);
                m->tangents = p;
                __aeabi_memclr4(p, posBytes);
                p = operator new[](posBytes);
                m->binormals = p;
                __aeabi_memclr4(p, posBytes);
            }
        }
        if (vertexFormat & 8) {
            p = operator new[]((unsigned int) vertexCount << 4);
            m->colors = p;
            __aeabi_memclr4(p, (unsigned int) vertexCount << 4);
        }

        return 1;
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    float CameraSetPerspective(float p1, float aspectNum, float fov, float aspectDen, float near,
                               Camera *cam) {
        float ret = p1;
        if (cam != 0) {
            float *f = (float *) cam;
            f[0] = p1;
            f[1] = aspectNum;
            f[2] = fov;

            float s = AbyssEngine::AEMath::Sinf(f[0] * 0.5f);
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
}

namespace AbyssEngine {
    void ImageFontSetSpacing(ImageFont *font, short spacing) {
        if (font != 0)
            s16(font, 0x10) = spacing;
    }
}

namespace AbyssEngine {
    int ImageFontGetHeight(ImageFont *font) {
        if (font != 0) {
            void *p = pp(font, 0xc);
            Mesh *q = (Mesh *) pp(p, 0x0);
            float *r = (float *) q->positions;
            int v = (int) r[7];
            if (v == 0x18)
                v = 0x13;
            return v;
        }
        return 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const int &b) {
        String result(a);
        String num;
        num.ctor_int(b);
        result += num;
        return result;
    }
}

namespace AbyssEngine {
    int ImageFontGetSpacing(ImageFont *font) {
        short v = (font == 0) ? (short) 0 : s16(font, 0x10);
        return (int) v;
    }
}

namespace AbyssEngine {
    String operator+(const String &a) {
        return String(a);
    }
}

namespace AbyssEngine {
    int ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x,
                            int y, PaintCanvas *canvas, Engine *engine, bool flag);

    void ImageFontDrawString(ImageFont *font, const unsigned short *str, int x, int y,
                             PaintCanvas *canvas, Engine *engine, bool flag) {
        if (font != 0 && str != 0) {
            unsigned short i = 0;
            unsigned int len;
            do {
                len = (unsigned int) i;
                i = i + 1;
            } while (str[len] != 0);
            ImageFontDrawString(font, str, len, x, y, canvas, engine, flag);
        }
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *text, unsigned int len);

    int ImageFontGetHeight(ImageFont * font);
    int MeshDraw(Engine * engine, Mesh * mesh);

    int ImageFontDrawString(ImageFont *font, const unsigned short *text, unsigned int len, int x, int y,
                            PaintCanvas *canvas, Engine *engine, bool rtl) {
        if (text == 0 || font == 0)
            return 0;

        char *f = (char *) font;
        char *pc = (char *) canvas;
        char *en = (char *) engine;
        unsigned char mode = 0;

        int w = ImageFontGetWidth(font, text, len);
        if (w + x < 0)
            return 0;

        int top = (int) (short) s16(f, 0x12);
        int h = ImageFontGetHeight(font);
        int dispW = engine->GetDisplayWidth();
        int bottom = dispW;
        if (x <= dispW) {
            top = top + y;
            bottom = h + top;
        }
        int side = (x <= dispW) ? bottom : (dispW - x);

        if ((side < 0) != (dispW < x))
            return 0;
        if (top > (int) engine->GetDisplayHeight())
            return 0;

        int step = -1;
        int idx = (int) len - 1;
        bool batched = (u8(pc, 0x1c) != 0) || rtl;
        if (batched) {
            step = 1;
            idx = 0;
        }

        bool shaderMode = (engine->shaderModeFlag != 0);
        if (shaderMode) {
            if (*g_GameText_arabicEnabledFlag != 0 && GameText::getLanguage() == 9 &&
                GameText::isNonArabicString(text, len) != 0) {
                idx = 0;
                step = 1;
            }
        }

        float baseY = AbyssEngine::AEMath::VectorSignedToFloat(top - 2, mode);

        for (unsigned int i = 0; i < len; ++i) {
            unsigned int slot = 0;
            unsigned short glyphCount = u16(f, 0x0);
            bool found = false;
            while (slot < glyphCount) {
                if (*(unsigned short *) (*(int *) (f + 4) + slot * 2) == text[idx]) {
                    found = true;
                    break;
                }
                ++slot;
            }

            if (found) {
                Mesh *glyphMesh = *(Mesh **) (*(int *) (f + 0xc) + slot * 4);
                int advance = (int) ((float *) glyphMesh->positions)[3];

                if (x + advance >= 0 && x <= (int) engine->GetDisplayWidth()) {
                    if (!shaderMode) {
                        ((PaintCanvas *) canvas)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) canvas);
                        MeshDraw(engine, *(Mesh **) (*(int *) (f + 0xc) + slot * 4));
                    } else {
                        int spr = *(int *) (pc + 8);
                        int n = *(int *) (pc + 0xc);
                        float fx = AbyssEngine::AEMath::VectorSignedToFloat(x, mode);
                        float *vsrc = (float *) glyphMesh->positions;
                        float *vdst = (float *) (*(int *) (spr + 4) + n * 0x30);
                        vdst[0] = vsrc[0] + fx;
                        vdst[1] = vsrc[1] + baseY;
                        vdst[3] = vsrc[3] + fx;
                        vdst[4] = vsrc[4] + baseY;
                        vdst[6] = vsrc[6] + fx;
                        vdst[7] = vsrc[7] + baseY;
                        vdst[9] = vsrc[9] + fx;
                        vdst[10] = vsrc[10] + baseY;

                        unsigned int *csrc = (unsigned int *) glyphMesh->texCoords;
                        unsigned int *cdst = (unsigned int *) (*(int *) (spr + 8) + (n << 5));
                        for (int k = 0; k < 8; ++k) cdst[k] = csrc[k];

                        unsigned int *mdst = (unsigned int *) (*(int *) (spr + 0xc) + (n << 6));
                        for (int r = 0; r < 4; ++r) {
                            mdst[r * 4 + 0] = u32(en, 0xd0);
                            mdst[r * 4 + 1] = u32(en, 0xd4);
                            mdst[r * 4 + 2] = u32(en, 0xd8);
                            mdst[r * 4 + 3] = u32(en, 0xdc);
                        }
                        *(int *) (pc + 0xc) = n + 1;

                        if (n > 0x62) {
                            s16(*(void **) (pc + 8), 0x28) = (short) ((n + 1) * 6);
                            ((PaintCanvas *) canvas)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) canvas);
                            MeshDraw(engine, *(Mesh **) (pc + 8));
                            *(int *) (pc + 0xc) = 0;
                        }
                    }
                }

                int adv = (int) (short) s16(f, 0x10) + advance;
                int eff = adv;
                if (text[idx] == 0x20)
                    eff = adv - 2;
                if (advance != 0xb)
                    eff = adv;
                x += eff;
            }

            idx += step;
        }

        if (shaderMode) {
            int n = *(int *) (pc + 0xc);
            if (n > 0) {
                s16(*(void **) (pc + 8), 0x28) = (short) ((n + (n << 1)) * 2);
                ((PaintCanvas *) canvas)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) canvas);
                MeshDraw(engine, *(Mesh **) (pc + 8));
                *(int *) (pc + 0xc) = 0;
            }
        }

        return 1;
    }
}

namespace AbyssEngine {
    String operator+(const int &a, const String &b) {
        String result;
        result.ctor_int(a);
        result += b;
        return result;
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int vertexFormat, void **out);

    void ImageFontRelease(Engine * engine, ImageFont * *slot);

    static void buildGlyphQuad(Mesh *mesh, unsigned int offX, unsigned int offY, unsigned int sizeX,
                               unsigned int sizeY, unsigned int atlasW, unsigned int atlasH,
                               unsigned char mode) {
        float *pos = (float *) mesh->positions;
        float halfW = AbyssEngine::AEMath::VectorUnsignedToFloat(sizeX, mode);
        pos[0] = 0;
        pos[1] = 0;
        pos[2] = 0;
        pos[4] = 0;
        pos[5] = 0;
        pos[3] = halfW;
        pos[6] = halfW;
        float halfH = AbyssEngine::AEMath::VectorUnsignedToFloat(offY, mode);
        pos[8] = 0;
        pos[9] = 0;
        pos[11] = 0;
        pos[7] = halfH;
        pos[10] = halfH;

        double aH = (double) AbyssEngine::AEMath::VectorUnsignedToFloat(atlasW, mode);
        double aW = (double) AbyssEngine::AEMath::VectorUnsignedToFloat(atlasH, mode);
        float offYs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) sizeY, mode);
        float offXs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) sizeX, mode);
        float u0 = AbyssEngine::AEMath::VectorUnsignedToFloat(offX, mode) * (float) (1.0 / aW);
        float v0 = AbyssEngine::AEMath::VectorUnsignedToFloat(offX, mode) * (float) (1.0 / aH);

        float *uv = (float *) mesh->texCoords;
        float v1 = (halfH + offYs) * (float) (1.0 / aH);
        float u1 = (halfW + offXs) * (float) (1.0 / aW);
        uv[0] = u0;
        uv[1] = v0;
        uv[2] = u1;
        uv[3] = v0;
        uv[4] = u1;
        uv[5] = v1;
        uv[6] = u0;
        uv[7] = v1;

        unsigned int *draw = (unsigned int *) mesh->indices;
        draw[0] = 0x20000;
        draw[1] = 1;
        draw[2] = 0;
    }

    int ImageCreateFontFromFile(Engine *engine, const char *path, unsigned short index, ImageFont **out) {
        if (engine == 0 || path == 0)
            return -4;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) (&handle)) == 0)
            return -1;

        char magic[8];
        for (int i = 0; i < 4; ++i) magic[i] = '*';
        if (AEFile::Read((uint32_t)(8), magic, handle) == 0) {
            ImageFontRelease(engine, out);
            return -1;
        }
        static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
        for (unsigned int k = 0; k < 8; ++k)
            if (want[k] != magic[k]) {
                ImageFontRelease(engine, out);
                return -1;
            }

        unsigned char mode = 0;
        unsigned char fmt = 0;
        unsigned short atlasH = 0, atlasW = 0, fontCount = 0;
        unsigned short glyphCount = 0;
        if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
        if (AEFile::Read((uint32_t)(2), &atlasH, handle) == 0) goto fail;
        if (AEFile::Read((uint32_t)(2), &atlasW, handle) == 0) goto fail;
        if (AEFile::Read((uint32_t)(2), &fontCount, handle) == 0) goto fail;
        if (AEFile::Skip((uint32_t)((unsigned int) fontCount << 3), handle) == 0) goto fail;

        {
            unsigned int fb = (unsigned int) fmt - 3;
            if (fb < 0x1f) {
                unsigned int extra = 0;
                if (((1u << (fb & 0xff)) & 0u) != 0 || (fmt >= 0x24)) {
                    if (AEFile::Read((uint32_t)(4), &extra, handle) == 0) goto fail;
                } else if (fb == 0) {
                    extra = (unsigned int) atlasW * (unsigned int) atlasH * 4;
                }
                if (AEFile::Skip((uint32_t)(extra), handle) == 0) goto fail;
            }
        }

        if (AEFile::Read((uint32_t)(2), &glyphCount, handle) == 0) goto fail;
        if (glyphCount <= index) goto fail;

        {
            char *font = (char *) ::operator new(0x14);
            u16(font, 0x0) = 0;
            *(unsigned long long *) (font + 4) = 0;
            *(unsigned long long *) (font + 0xc) = 0;
            *out = (ImageFont *) font;
        }

        for (unsigned short g = 0; g < glyphCount; ++g) {
            if (g == index) {
                char *font = (char *) *out;
                if (AEFile::Read((uint32_t)(2), font + 0x0, handle) == 0) goto fail;
                void *codes = ::operator new[]((unsigned int) u16(font, 0x0) << 1);
                pp(font, 0x4) = codes;
                if (AEFile::Read((uint32_t)((unsigned int) u16((char *) *out, 0x0) << 1), pp((char *) *out, 0x4),
                                 handle) == 0)
                    goto fail;
                font = (char *) *out;
                void *meshes = ::operator new[]((unsigned int) u16(font, 0x0) << 2);
                pp(font, 0xc) = meshes;

                unsigned int n = u16((char *) *out, 0x0);
                for (unsigned int gi = 0; gi < n; ++gi) {
                    void **meshSlot = (void **) (*(int *) (pp((char *) *out, 0xc)) + gi * 4);
                    if (MeshCreate(engine, 4, 2, 0x13, meshSlot) != 1)
                        goto fail;
                    unsigned short sizeY = 0, sizeX = 0, offX = 0, offY = 0;
                    if (AEFile::Read((uint32_t)(2), &sizeY, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &sizeX, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &offX, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &offY, handle) == 0) goto fail;
                    Mesh *mesh = *(Mesh **) (*(int *) (pp((char *) *out, 0xc)) + gi * 4);
                    buildGlyphQuad(mesh, offX, offY, sizeX, sizeY, atlasW, atlasH, mode);
                }
            } else {
                unsigned short subCount = 0;
                if (AEFile::Read((uint32_t)(2), &subCount, handle) == 0) goto fail;
                if (AEFile::Skip((uint32_t)((unsigned int) subCount * 10), handle) == 0) goto fail;
            }
        }

        AEFile::Close(handle);
        return 1;

    fail:
        ImageFontRelease(engine, out);
        return -1;
    }
}

namespace AbyssEngine {
    Quaternion operator+(const Quaternion &a, const Quaternion &b) {
        float yv = a.y + b.y;
        float wv = a.w + b.w;
        return Quaternion(wv, 0.0f, yv, 0.0f);
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllSize(short size, SpriteSystem *sys) {
        if (sys == 0)
            return;

        short *sizes = (short *) pp(sys, 0x8);
        if (u8(sys, 0xc) != 0) {
            sizes[0] = size;
            return;
        }
        unsigned int count = u16(sys, 0x0);
        for (unsigned int i = 0; i < count; ++i)
            sizes[i] = size;
    }
}

namespace AbyssEngine {
    float MODF(float value, float *intPart) {
        float ip = (float) (int) value;
        *intPart = ip;
        return value - ip;
    }
}

namespace AbyssEngine {
    int MeshDraw(Engine * engine, Mesh * mesh);

    static inline unsigned int f2u(float f) {
        union {
            float f;
            unsigned int u;
        } c;
        c.f = f;
        return c.u;
    }

    void SpriteSystemDraw(Engine *engine, const Matrix &view, const Matrix &world, SpriteSystem *sys) {
        if (sys == 0)
            return;

        Mesh *mesh = (Mesh *) pp(sys, 0x10);
        float *vbuf = (float *) mesh->positions;

        Matrix mv;
        AE_AEMath_matMul(&mv, &view);

        unsigned short count = u16(sys, 0x0);
        unsigned char sharedSize = u8(sys, 0xc);
        float *posCpu = (float *) pp(sys, 0x4);
        short *sizeCpu = (short *) pp(sys, 0x8);
        const float *W = world;

        unsigned short sizeIdx = 0;
        unsigned short posIdx = 0;
        for (unsigned int q = 0; (q & 0xffff) < (unsigned int) count * 0xc; q += 0xc) {
            float *p = posCpu + (unsigned int) posIdx;
            posIdx = posIdx + 3;
            float px = p[0], py = p[1], pz = p[2];

            unsigned int si = sizeIdx;
            sizeIdx = (unsigned short) (((sharedSize ^ 1) & 0xff) + sizeIdx);
            float half = (float) ((int) sizeCpu[si] >> 1);

            float cx = W[0xc / 4 + 0] + W[4 / 4] * py + W[0] * px + W[8 / 4] * pz;
            float cy = W[0x1c / 4] + py * W[0x14 / 4] + px * W[0x10 / 4] + pz * W[0x18 / 4];
            float cz = W[0x2c / 4] + py * W[0x24 / 4] + px * W[0x20 / 4] + pz * W[0x28 / 4];

            float left = cx - half, right = cx + half;
            float bottom = cy - half, top = cy + half;

            float *out = vbuf + q;
            out[0] = left;
            out[1] = bottom;
            out[2] = cz;
            out[3] = right;
            out[4] = bottom;
            out[5] = cz;
            out[6] = right;
            out[7] = top;
            out[8] = cz;
            out[9] = left;
            out[10] = top;
            out[11] = cz;
        }

        if (mesh->material == 0) {
            MeshDraw(engine, mesh);
        } else {
            void *batch = mesh->material;
            ArrayAddCachedRaw<Mesh *>(mesh, (char *) batch + 0x44);

            const float *v = view;
            AE_SpriteSystem_pushMatrix(f2u(v[0]), f2u(v[1]), f2u(v[2]), f2u(v[3]), f2u(v[4]), f2u(v[5]),
                                       f2u(v[6]), f2u(v[7]), f2u(v[8]), f2u(v[9]), f2u(v[10]), f2u(v[11]),
                                       f2u(v[12]), f2u(v[13]), f2u(v[14]), (int) (intptr_t) batch + 0x2c);
            unsigned int one = 0x3f800000;
            unsigned int negOne = 0xbf800000;
            AE_SpriteSystem_pushMatrix(one, 0, 0, 0, 0, negOne, 0, 0, 0, 0, one, 0,
                                       one, one, one, (int) (intptr_t) batch + 0x38);
            const float *w = world;
            AE_SpriteSystem_pushMatrix(f2u(w[0]), f2u(w[1]), f2u(w[2]), f2u(w[3]), f2u(w[4]), f2u(w[5]),
                                       f2u(w[6]), f2u(w[7]), f2u(w[8]), f2u(w[9]), f2u(w[10]), f2u(w[11]),
                                       f2u(w[12]), f2u(w[13]), f2u(w[14]), (int) (intptr_t) batch + 0x5c);
            ArrayAddCachedRaw<unsigned int>(0xffffffff, (char *) batch + 0x50);
        }
    }
}

extern "C" {
void glGenBuffers(int n, void *buffers);

void glBindBuffer(unsigned int target, unsigned int buffer);

void glBufferData(unsigned int target, unsigned int size, const void *data, unsigned int usage);

void glDeleteBuffers(int n, const void *buffers);

unsigned int glGetError();
}

namespace AbyssEngine {
    __attribute__ ((always_inline))

    static inline void freeArray(void *&arr) {
        if (arr != 0)
            operator delete[](arr);
        arr = 0;
    }

    int MeshConvertToVBOIntern(Mesh *m) {
        if (m == 0 || *g_Mesh_vboEnabledFlag == 0)
            return -4;
        if (m->uploaded != 0 || (short) m->indexCount == 0)
            return -4;

        unsigned int vcount = m->vertexCount;

        void *colArr = m->colors;
        void *tanArr = m->tangents;
        void *binArr = m->binormals;

        glGenBuffers(1, &m->positionVBO);
        glBindBuffer(0x8892, m->positionVBO);
        glBufferData(0x8892, vcount * 0xc, m->positions, 0x88e4);

        unsigned char flags = m->vertexFormat;
        if (flags & 2) {
            glGenBuffers(1, &m->texCoordVBO);
            glBindBuffer(0x8892, m->texCoordVBO);
            glBufferData(0x8892, vcount << 3, m->texCoords, 0x88e4);
            m->vboByteSize += (int) (vcount * 8);

            glGenBuffers(1, &m->indexVBO);
            glBindBuffer(0x8893, m->indexVBO);
            glBufferData(0x8893, (unsigned int) m->indexCount << 1, m->indices, 0x88e4);
            m->vboByteSize += (int) (m->indexCount * 2);
            flags = m->vertexFormat;
        }

        if (flags & 4) {
            glGenBuffers(1, &m->normalVBO);
            glBindBuffer(0x8892, m->normalVBO);
            glBufferData(0x8892, vcount * 0xc, m->normals, 0x88e4);
            m->vboByteSize += (int) (vcount * 0xc);
            if (*g_Mesh_tangentEnabledFlag != 0) {
                glGenBuffers(1, &m->tangentVBO);
                glBindBuffer(0x8892, m->tangentVBO);
                glBufferData(0x8892, vcount * 0xc, tanArr, 0x88e4);
                m->vboByteSize += (int) (vcount * 0xc);
                glGenBuffers(1, &m->binormalVBO);
                glBindBuffer(0x8892, m->binormalVBO);
                glBufferData(0x8892, vcount * 0xc, binArr, 0x88e4);
                m->vboByteSize += (int) (vcount * 0xc);
            }
        }

        if (m->vertexFormat & 8) {
            glGenBuffers(1, &m->colorVBO);
            glBindBuffer(0x8892, m->colorVBO);
            glBufferData(0x8892, vcount << 4, colArr, 0x88e4);
            m->vboByteSize += (int) (vcount * 0x10);
        }

        glBindBuffer(0x8892, 0);
        glBindBuffer(0x8893, 0);

        if (glGetError() == 0) {
            if (*g_Mesh_keepCpuCopyFlag == 0) {
                freeArray(m->positions);
                freeArray(m->texCoords);
                freeArray(m->indices);
                freeArray(m->normals);
                freeArray(m->colors);
                freeArray(m->tangents);
                freeArray(m->binormals);
            }
            m->uploaded = 1;
            *g_Mesh_vboByteCounter += m->vboByteSize;
            return 1;
        }

        if (m->uploaded != 0) {
            glDeleteBuffers(1, &m->positionVBO);
            glDeleteBuffers(1, &m->indexVBO);
            unsigned char f = m->vertexFormat;
            if (f & 2) {
                glDeleteBuffers(1, &m->texCoordVBO);
                f = m->vertexFormat;
            }
            if (f & 4) {
                glDeleteBuffers(1, &m->normalVBO);
                if (*g_Mesh_tangentDelFlag != 0) {
                    glDeleteBuffers(1, &m->tangentVBO);
                    glDeleteBuffers(1, &m->binormalVBO);
                }
            }
            if (m->vertexFormat & 8)
                glDeleteBuffers(1, &m->colorVBO);
        }
        m->vboByteSize = 0;
        return -1;
    }
}

namespace AbyssEngine {
    int MeshConvertToVBO(Mesh * mesh);

    int TransformConvertToVBO(Transform *t) {
        if (t != 0) {
            for (unsigned int i = 0; i < t->meshes.size_; ++i)
                MeshConvertToVBO(t->meshes.data_[i]);
            for (unsigned int i = 0; i < t->children.size_; ++i)
                TransformConvertToVBO(t->children.data_[i]);
        }
        return 1;
    }
}

namespace AbyssEngine {
    void ImageFontCheckString(ImageFont * /*font*/, const unsigned short * /*str*/, unsigned int count) {
        unsigned short i = 0;
        unsigned int v;
        do {
            v = (unsigned int) i;
            i = i + 1;
        } while (v < count);
    }
}

namespace AbyssEngine {
    typedef void (*ImageCallback)(Image *, void *);

    int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user,
                                    unsigned int *outIds, float scale, AELoadedTexture *outTex,
                                    bool flag);

    int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user,
                              unsigned int *outIds, bool /*flag*/, float scale) {
        TextureCreateFromFileIntern(engine, path, cb, user, outIds, scale,
                                    (AELoadedTexture *) 0, false);
        return 1;
    }
}

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

    char *g_cubemapEnabledFlag;
    char *g_texEnvFlag;
    char *g_clampFlag;
    float *g_anisoMaxPtr;
    char *g_labelObjectsFlag;
    }

    int ImageCreateFromFile(Engine *engine, const char *path, Image **out);

    void ImageRelease(Image * *slot);

    void SetFXMaterial(BlendMode /*mode*/) {
    }

    int GenerateCompressedTexture(Image * /*image*/) {
        return 1;
    }

    __attribute__ ((always_inline))

    static inline void uploadCompressedChain(char *img, unsigned int glFmt, int divShift, int minBlock) {
        unsigned int w = (unsigned int) u16(img, 0x0);
        unsigned int h = (unsigned int) u16(img, 0x2);
        int level = 0;
        for (unsigned int off = 0; off < u32(img, 0x10);) {
            unsigned int blockSz = (w * h) >> divShift;
            if ((int) blockSz < minBlock)
                blockSz = (unsigned int) minBlock;
            glCompressedTexImage2D(0xde1, level, glFmt, (int) w, (int) h, 0, (int) blockSz,
                                   (char *) pp(img, 0xc) + off);
            off += blockSz;
            unsigned int nw = 1, nh = 1;
            if (h >> 1 > 1) nh = h >> 1;
            if (w >> 1 > 1) nw = w >> 1;
            ++level;
            w = nw;
            h = nh;
        }
    }

    int TextureCreateFromFileIntern(Engine *engine, const char *path, void (*cb)(Image *, void *),
                                    void *user, unsigned int *outIds, float aniso,
                                    AELoadedTexture *outTex, bool /*flag*/) {
        Image *imgPtr = 0;
        *outIds = 0;
        engine->lastGlError = glGetError();

        if (ImageCreateFromFile(engine, path, &imgPtr) != 1)
            return -4;
        if (cb != 0)
            cb(imgPtr, user);

        glGenTextures(1, outIds);
        char *img = (char *) imgPtr;
        int format = (int) u32(img, 0x4);

        if (format == 6) {
            if (*g_cubemapEnabledFlag != 0) {
                glBindTexture(0x8513, *outIds);
                glTexParameteri(0x8513, 0x2800, 0x2601);
                glTexParameteri(0x8513, 0x2801, 0x2601);
                int faceH = __aeabi_uidiv((int) u16(img, 0x2), 6);
                unsigned int w = u16(img, 0x0);
                const unsigned int faces[6] = {0x8517, 0x8516, 0x8519, 0x8515, 0x851a, 0x8518};
                int faceBytes = faceH * (int) w;
                for (int fi = 0; fi < 6; ++fi) {
                    glTexImage2D(faces[fi], 0, 0x1908, (int) w, faceH, 0, 0x1908, 0x1401,
                                 (char *) pp(img, 0xc) + faceBytes * 4 * fi);
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

            if (u8(img, 0x8) == 0) {
                if (engine->linearFilterFlag != 0) {
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

            int w = (int) u16(img, 0x0);
            int h = (int) u16(img, 0x2);
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
                case 4:
                    if (u8(img, 0x8) == 0)
                        glCompressedTexImage2D(0xde1, 0, 0x8c03, w, h, 0, (int) u32(img, 0x10), pp(img, 0xc));
                    else
                        uploadCompressedChain(img, 0x8c03, 1, 0x10);
                    break;
                case 5:
                    if (u8(img, 0x8) == 0)
                        glCompressedTexImage2D(0xde1, 0, 0x8c02, w, h, 0, (int) u32(img, 0x10), pp(img, 0xc));
                    else
                        uploadCompressedChain(img, 0x8c02, 1, 0x10);
                    break;
                case 7:
                    if (u8(img, 0x8) == 0)
                        glCompressedTexImage2D(0xde1, 0, 0x8c93, w, h, 0, (int) u32(img, 0x10), pp(img, 0xc));
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
                        glCompressedTexImage2D(0xde1, 0, glFmt, w, h, 0, (int) u32(img, 0x10), pp(img, 0xc));
                    else
                        uploadCompressedChain(img, glFmt, 1, 0x10);
                    break;
                }
                case 0xb:
                    if (u8(img, 0x8) == 0)
                        glCompressedTexImage2D(0xde1, 0, 0x8d64, w, h, 0, (int) u32(img, 0x10), pp(img, 0xc));
                    else
                        uploadCompressedChain(img, 0x8d64, 1, 8);
                    break;
                default:
                    break;
            }
        }

        if (outTex == 0) {
            int err = (int) glGetError();
            engine->lastGlError = (unsigned int) err;
            if (err != 0) {
                String tmp(path);
                engine->lastErrorPath.assign(&tmp);
                glDeleteTextures(1, outIds);
                ImageRelease(&imgPtr);
                return -4;
            }
            AELabelObject(0x1702, *outIds, path);
        } else {
            char *tex = (char *) outTex;
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
            engine->textureByteCounter += (int) u32(tex, 0x18);
        }

        ImageRelease(&imgPtr);
        return 1;
    }
}

namespace AbyssEngine {
    String operator+(const long long &a, const String &b) {
        String result;
        result.ctor_longlong(a);
        result += b;
        return result;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void SpriteSystemRelease(Engine *engine, SpriteSystem **slot) {
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

        MeshRelease(engine, (Mesh **) ((char *) *slot + 0x10));

        if (*slot != 0)
            operator delete((void *) *slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    int CurveCreate(void **src, unsigned short count, Curve **out) {
        Curve *curve = (Curve *) operator new(8);
        u32(curve, 0x4) = 0;
        *out = curve;
        u16(curve, 0x0) = count;

        void *data = operator new[]((unsigned int) count << 2);
        pp(curve, 0x4) = data;
        __aeabi_memcpy4(data, src, (unsigned int) count << 2);
        return 1;
    }
}

namespace AbyssEngine {
    bool operator==(const String &a, const String &b) {
        String tmp(a);
        int cmp = tmp.Compare_str(const_cast<String *>(&b));
        return cmp != 0;
    }
}

namespace AbyssEngine {
    __attribute__ ((always_inline))

    static inline void releaseArray(void *&arr) {
        if (arr != 0)
            operator delete[](arr);
        arr = 0;
    }

    void MeshReleaseIntern(Engine * /*engine*/, Mesh **slot) {
        Mesh *m = *slot;
        if (m == 0)
            return;

        if (m->shared == 0) {
            if (m->uploaded != 0) {
                glDeleteBuffers(1, &m->positionVBO);
                glDeleteBuffers(1, &m->indexVBO);
                unsigned char flags = m->vertexFormat;
                if (flags & 2) {
                    glDeleteBuffers(1, &m->texCoordVBO);
                    flags = m->vertexFormat;
                }
                if (flags & 4) {
                    glDeleteBuffers(1, &m->normalVBO);
                    if (*g_Mesh_tangentEnabledFlag != 0) {
                        glDeleteBuffers(1, &m->tangentVBO);
                        glDeleteBuffers(1, &m->binormalVBO);
                    }
                }
                if (m->vertexFormat & 8)
                    glDeleteBuffers(1, &m->colorVBO);
            }

            releaseArray(m->indices);
            releaseArray(m->positions);
            releaseArray(m->texCoords);
            releaseArray(m->colors);
            releaseArray(m->normals);

            if (*g_Mesh_extraArraysFlag != 0) {
                releaseArray(m->tangents);
                releaseArray(m->binormals);
            }
        }

        delete m->animation;
        m->animation = 0;

        operator delete((void *) m);
        *slot = 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const float &b) {
        String result(a);
        String num;
        num.ctor_float(b);
        result += num;
        return result;
    }
}

namespace AbyssEngine {
    int MeshConvertToVBOIntern(Mesh * mesh);
    int TransformConvertToVBO(Transform * t);

    int MeshConvertToVBO(Mesh *mesh) {
        int result = -4;
        if (mesh != 0 && *g_Mesh_vboEnabledFlag != 0) {
            if (mesh->uploaded != 0 || mesh->vboEligible == 0)
                return -4;
            MeshConvertToVBOIntern(mesh);
            TransformConvertToVBO(mesh->animation);
            result = 1;
        }
        return result;
    }
}

namespace AbyssEngine {
    void esMatrixMultiply(ESMatrix *out, ESMatrix *a, ESMatrix *b) {
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
}

namespace AbyssEngine {
    int MeshReadData(Engine *engine, const unsigned int &handle, unsigned int flags, Mesh **slot,
                     Material *mat);

    void MeshRelease(Engine * engine, Mesh * *slot);

    static void initMesh(Mesh *m) {
        for (int i = 0; i < 0x88; ++i) ((char *) m)[i] = 0;

        m->boundsRadiusSq = 1.0f;
    }

    int MeshCreateFromFile(Engine *engine, const char *path, Mesh **out, Material *mat) {
        if (engine == 0 || path == 0)
            return -4;

        Mesh *m = (Mesh *) ::operator new(0x88);
        initMesh(m);
        *out = m;
        m->material = mat;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, &m->positionVBO) == 0) {
            if (*out != 0)
                ::operator delete((void *) *out);
            *out = 0;
            return -1;
        }

        char magic[7];
        for (int i = 0; i < 4; ++i) magic[i] = '*';
        if (AEFile::Read((uint32_t)(7), magic, handle) == 0) {
            MeshRelease(engine, out);
            AEFile::Close(handle);
            return -1;
        }

        static const char sig0[7] = {'A', 'E', 'M', 'e', 's', 'h', 0};
        static const char sig1[7] = {'A', 'E', 'A', 'n', 'i', 'm', 0};
        static const char sig2[7] = {'A', 'E', 'S', 'k', 'i', 'n', 0};
        static const char sig3[7] = {'A', 'E', 'L', 'o', 'd', '0', 0};
        static const char sig4[7] = {'A', 'E', 'M', 'e', 's', '2', 0};
        unsigned int fmt = 0x1f;
        for (int i = 0; i < 7; ++i) {
            char ch = magic[i];
            if (sig0[i] != ch) fmt &= ~1u;
            if (sig1[i] != ch) fmt &= ~8u;
            if (sig2[i] != ch) fmt &= ~0x10u;
            if (sig3[i] != ch) fmt &= ~4u;
            if (sig4[i] != ch) fmt &= ~2u;
        }

        if (fmt == 0) {
            MeshRelease(engine, out);
            AEFile::Close(handle);
            return -1;
        }

        if ((fmt & 0x1b) != 0) {
            unsigned short ver = 0;
            if (AEFile::Read((uint32_t)(2), &ver, handle) == 0) {
                MeshRelease(engine, out);
                AEFile::Close(handle);
                return -1;
            }
        }

        if (AEFile::Read((uint32_t)(1), &(*out)->vertexFormat, handle) == 0 || (*out)->vertexFormat == 0) {
            MeshRelease(engine, out);
            AEFile::Close(handle);
            return -1;
        }

        bool ok = false;
        if ((fmt & 0x1a) == 0) {
            if (MeshReadData(engine, handle, fmt, out, mat) != -1)
                ok = true;
        } else {
            unsigned short subCount = 0;
            if (AEFile::Read((uint32_t)(2), &subCount, handle) == 0) {
                MeshRelease(engine, out);
                AEFile::Close(handle);
                return -1;
            }
            if (subCount < 2) {
                if (MeshReadData(engine, handle, fmt, out, mat) != -1)
                    ok = true;
            } else {
                (*out)->animation = new Transform();
                for (unsigned int s = 0; s < subCount; ++s) {
                    Mesh *childPtr = (Mesh *) ::operator new(0x88);
                    initMesh(childPtr);
                    childPtr->vboEligible = 1;
                    childPtr->vertexFormat = (*out)->vertexFormat;
                    childPtr->material = mat;
                    if (MeshReadData(engine, handle, fmt, &childPtr, mat) == -1) {
                        MeshRelease(engine, out);
                        AEFile::Close(handle);
                        return -1;
                    }
                    ((AEMath::BSphere *) &(*out)->boundsCenterX)->Merge(
                        *(const AEMath::BSphere *) &childPtr->boundsCenterX);
                    ArrayAddRaw<Mesh *>(childPtr, &(*out)->animation->meshes);
                }
                ok = true;
            }
        }

        if (ok) {
            AEFile::Close(handle);
            Transform *xf = (*out)->animation;
            if (xf != 0) {
                xf->CollectAnimationData();
                long long t = __aeabi_f2lz(0.0f);
                xf->SetAnimationRangeInTime(t, t);
            }
            return 1;
        }

        MeshRelease(engine, out);
        AEFile::Close(handle);
        return -1;
    }
}

namespace AbyssEngine {
    int CameraIsSphereinViewFrustum(const Vector &center, float radius, Matrix *extra, Camera *cam) {
        char *c = (char *) cam;

        if (!(radius != 0.0f && *g_Camera_frustumEnabledFlag != 0))
            return 1;

        Matrix local;
        Matrix transformed;
        Vector pos, dir, axis;
        Vector camPoint = {0.0f, 0.0f, 0.0f};

        Matrix *srcMatrix = (Matrix *) (c + 0xc);
        Matrix *dstMatrix;
        if (extra == 0) {
            dstMatrix = &local;
        } else {
            __aeabi_memcpy(&local, srcMatrix, 0x3c);
            local *= *extra;
            dstMatrix = &transformed;
            srcMatrix = &local;
        }

        *(Vector *) dstMatrix = AEMath::MatrixInverseTransformVector(*srcMatrix, *(Vector *) dstMatrix);
        camPoint = *(Vector *) dstMatrix;

        pos = AEMath::MatrixGetPosition(*dstMatrix);
        pos -= center;
        dir = AEMath::MatrixGetDir(*dstMatrix);
        axis -= dir;
        Vector normAxis = AEMath::VectorNormalize(axis);
        float fwd = AEMath::VectorDot(pos, normAxis);

        if (fwd > f32(c, 0x8) + radius)
            return 0;
        if (fwd < f32(c, 0x4) - radius)
            return 0;

        axis = AEMath::MatrixGetRight(*dstMatrix);
        normAxis = AEMath::VectorNormalize(axis);
        float right = AEMath::VectorDot(pos, normAxis);

        float rightPad = f32(c, 0x54) * radius;
        float rightLimit = fwd * f32(c, 0x48) * f32(c, 0x50);
        if (right > rightLimit + rightPad)
            return 0;
        if (right < -rightLimit - rightPad)
            return 0;

        axis = AEMath::MatrixGetUp(*dstMatrix);
        normAxis = AEMath::VectorNormalize(axis);
        float up = AEMath::VectorDot(pos, normAxis);

        float upPad = f32(c, 0x58) * radius;
        float upLimit = fwd * f32(c, 0x48);
        if (up > upLimit + upPad)
            return 0;
        if (up < -upLimit - upPad)
            return 0;

        return 1;
    }
}

namespace AbyssEngine {
    struct CurveRec {
        Curve *curve;
    };

    long long CurveGetValue(unsigned long long time, Curve *curve) {
        int *entries = (int *) pp(curve, 0x4);
        unsigned short count = u16(curve, 0x0);

        unsigned int qlo = (unsigned int) time;
        unsigned int qhi = (unsigned int) (time >> 32);

        auto keyTimeHi = [&](int kf) -> unsigned int { return *(unsigned int *) (kf + 0xc); };
        auto keyTimeLo = [&](int kf) -> unsigned int { return *(unsigned int *) (kf + 0x8); };
        auto keyVal = [&](int kf) -> int { return *(int *) (kf + 0x10); };

        if (count == 0)
            return 0;

        int first = entries[0];
        if ((unsigned long long) time <
            (((unsigned long long) keyTimeHi(first) << 32) | keyTimeLo(first))) {
            return keyVal(first);
        }

        unsigned int idx = (unsigned int) (unsigned short) (count - 1);
        int last = entries[idx];
        if ((unsigned long long) time >
            (((unsigned long long) keyTimeHi(last) << 32) | keyTimeLo(last))) {
            return keyVal(last);
        }

        int seg = last;
        unsigned int segIdx = idx;
        while (segIdx > 0) {
            seg = entries[segIdx];
            unsigned long long st = ((unsigned long long) keyTimeHi(seg) << 32) | keyTimeLo(seg);
            if (st <= (unsigned long long) time)
                break;
            --segIdx;
        }

        char tag = *(char *) seg;
        unsigned long long segStart =
                ((unsigned long long) keyTimeHi(seg) << 32) | keyTimeLo(seg);
        int v0 = keyVal(seg);

        if (tag == 1) {
            return v0;
        }

        int nextKf = entries[segIdx + 1];
        unsigned long long segEnd =
                ((unsigned long long) keyTimeHi(nextKf) << 32) | keyTimeLo(nextKf);
        int v1 = keyVal(nextKf);

        unsigned int num = (unsigned int) ((time - segStart)) << 12;
        unsigned int numHi = (unsigned int) (((time - segStart)) >> 20);
        unsigned int den = (unsigned int) (segEnd - segStart);
        long long tq = __aeabi_uldivmod(num, numHi, den, (unsigned int) ((segEnd - segStart) >> 32));
        unsigned int t = (unsigned int) tq;

        if (tag == 2) {
            long long d = (long long) (v1 - v0) * (unsigned int) t;
            return (int) ((unsigned int) (d >> 12)) + v0;
        }

        int c0 = *(int *) (seg + 0x14);
        int c1 = *(int *) (seg + 0x18);
        unsigned long long t2_64 = (unsigned long long) t * t;
        unsigned int t2 = (unsigned int) (t2_64 >> 12);
        unsigned long long t3_64 = (unsigned long long) t2 * t;
        unsigned int t3 = (unsigned int) (t3_64 >> 12);

        long long term0 = (long long) c0 * (int) (t - 2 * t2 + t3);
        long long term1 = (long long) c1 * (int) (t3 - t2);
        long long term2 = (long long) (v1 - v0) * (int) (3 * t2 - 2 * t3);
        int value = v0 + (int) (term0 >> 12) + (int) (term1 >> 12) + (int) (term2 >> 12);
        return value;
    }
}

namespace AbyssEngine {
    void computeFloatString(float value, int intValue, int *precision, int *exponentOut, int extra) {
        unsigned short *buf = (unsigned short *) operator new[](0x42);

        int prec = *precision & ~(*precision >> 31);
        if (prec > 0x1d)
            prec = 0x1e;

        int sign = 0;
        float v = (float) intValue;
        if (v < 0.0f) {
            v = -v;
            intValue = (int) v;
            sign = 1;
        }

        (void) sign;

        float ip = (float) (int) v;
        float frac = (float) intValue - ip;

        int digitCount = 0;
        unsigned short *w = buf;
        if ((int) v != 0) {
            unsigned short tmp[40];
            int n = 0;
            float cur = ip;
            while (cur != 0.0f) {
                float q = cur / 10.0f;
                float qi = (float) (int) q;
                int digit = (int) (((double) (q - qi)) * 10.0 + 0.5);
                tmp[n++] = (unsigned short) (0x30 + digit);
                cur = qi;
                ++digitCount;
            }
            for (int i = n - 1; i >= 0; --i)
                *w++ = tmp[i];
        } else {
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

        int limit = (extra == 0) ? (prec + (digitCount > 0 ? digitCount : 0)) : prec;
        float f = frac;
        int produced = 0;
        while (produced < limit && (w - buf) < 0x20) {
            float scaled = f * 10.0f;
            float di = (float) (int) scaled;
            f = scaled - di;
            *w++ = (unsigned short) (0x30 + (int) di);
            ++produced;
        }

        if (w > buf) {
            unsigned short *p = w - 1;
            unsigned short d = (unsigned short) (*p + 5);
            *p = d;
            while (d > 0x39) {
                *p = 0x30;
                if (p > buf) {
                    --p;
                    d = (unsigned short) (*p + 1);
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

        *w = 0;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void TransformRelease(Engine *engine, Transform **slot) {
        Transform *t = (Transform *) *slot;
        if (t == 0)
            return;

        for (unsigned int i = 0; i < t->children.size_; ++i) {
            TransformRelease(engine, &t->children.data_[i]);
            t = (Transform *) *slot;
        }

        for (unsigned int i = 0; i < t->meshes.size_; ++i) {
            MeshRelease(engine, &t->meshes.data_[i]);
            t = (Transform *) *slot;
        }
    }
}

int GetStringLength(AbyssEngine::String str) {
    const unsigned short *p = str.GetAEWChar();
    while (*p)
        ++p;
    return static_cast<int>(p - str.GetAEWChar());
}
