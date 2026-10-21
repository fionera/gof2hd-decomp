#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace {
    struct CurveKeyframe {
        int8_t tag;  // 1 = step, 2 = linear, 3 = cubic Hermite
        uint8_t pad0x1[7];
        unsigned long long time;
        int32_t value;
        int32_t c0;
        int32_t c1;
    };

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
    static_assert(offsetof(CurveKeyframe, tag) == 0x0, "CurveKeyframe.tag");
    static_assert(offsetof(CurveKeyframe, time) == 0x8, "CurveKeyframe.time");
    static_assert(offsetof(CurveKeyframe, value) == 0x10, "CurveKeyframe.value");
    static_assert(offsetof(CurveKeyframe, c0) == 0x14, "CurveKeyframe.c0");
    static_assert(offsetof(CurveKeyframe, c1) == 0x18, "CurveKeyframe.c1");
#endif
}

namespace AbyssEngine {
    // Defined in EngineFlags.cpp (separate TU). These MUST be declared in
    // namespace AbyssEngine (external linkage) so they bind to the real
    // definitions there; declaring them in an anonymous namespace would give
    // them internal linkage and leave them undefined in this TU.
    extern char *g_Camera_frustumEnabledFlag;
    extern char *g_Engine_fboEnabledFlag;
    extern char *g_Engine_shaderModeFlag;
    extern char *g_GameText_arabicEnabledFlag;
    extern char *g_MeshIntersect_flipVFlag;
    extern char *g_Mesh_extraArraysFlag;
    extern char *g_Mesh_keepCpuCopyFlag;
    extern char *g_Mesh_shaderPathFlag;
    extern char *g_Mesh_tangentDelFlag;
    extern char *g_Mesh_tangentEnabledFlag;
    extern int *g_Mesh_vboByteCounter;
    extern char *g_Mesh_vboEnabledFlag;
    extern char *g_SpriteSystem_tangentFlag;
    extern char *g_SpriteSystem_uvFlipFlag;

    int currentUsedShaderIndex;

    float currentFps;
    float debugTouch[4];
    int fpsCounter;
    int SwapCounter;
    unsigned char firstRender;
    int loadTexture;
    float currentLODBias;
    int CubeMapSetIndex;
    int debugModusIndex;
    unsigned char firstRenderGlow;
    unsigned char performanceTest;
    long long performanceTime;
    float timeBetweenFrames;
    unsigned char BiasErrorOutputFlag;
    unsigned char performanceTestEnded;
    int orientationChangeTimer;
    long long performanceFrameCounter;
    long long performanceModulSwitches;
    long long performanceOverallModulSwitches;
    int lauf;
    float i_fov;
    float i_zFar;
    float i_zNear;
    float fpsTimer;
    int loadMesh;
    long long triDrawn;
    unsigned char ShaderSet;

    Engine *AE_getInitGLThis();

    int AE_getInitGLWidth();

    int AE_getInitGLHeight();

    void AE_SpriteSystem_pushMatrix(unsigned int m0, unsigned int m1, unsigned int m2, unsigned int m3,
                                    unsigned int m4, unsigned int m5, unsigned int m6, unsigned int m7,
                                    unsigned int m8, unsigned int m9, unsigned int m10, unsigned int m11,
                                    unsigned int m12, unsigned int m13, unsigned int m14, int dst);
}

namespace {
    struct EngineArrayHeader {
        uint32_t count;
        unsigned char *data;
        uint32_t capacity;
    };

    template<class T>
    inline void ArrayAddCachedRaw(T item, EngineArrayHeader *arrayHeader) {
        EngineArrayHeader *a = arrayHeader;
        if (a->count >= a->capacity) {
            uint32_t oldCap = a->capacity;
            a->data = (unsigned char *) realloc(a->data, (size_t) oldCap * 2 * sizeof(T));
            memset((T *) a->data + oldCap, 0, oldCap * sizeof(T));
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
            font->yOffset = yOffset;
    }
}

namespace AbyssEngine {
    int CameraIsPointinViewFrustum(const Vector &point, Matrix *extra, Camera *cam) {
        if (Engine::vfc == 0)
            return 1;

        const Matrix &view = cam->projection;
        // Point in camera space (computed but unused; a plain float[3] in the original).
        float camPoint[3] = {0.0f, 0.0f, 0.0f};
        if (extra != 0) {
            Matrix combined = view;
            combined *= *extra;
            *(Vector *) camPoint = AEMath::MatrixInverseTransformVector(combined, point);
        } else {
            *(Vector *) camPoint = AEMath::MatrixInverseTransformVector(view, point);
        }

        Vector toPoint = point - AEMath::MatrixGetPosition(view);
        float depth = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(-AEMath::MatrixGetDir(view)));
        if (depth > cam->position[2] || depth < cam->position[1])
            return 0;

        float vertical = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(AEMath::MatrixGetUp(view)));
        float vLimit = depth * cam->frustumTanHalfFov;
        if (vertical > vLimit || vertical < -(depth * cam->frustumTanHalfFov))
            return 0;

        float horizontal = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(AEMath::MatrixGetRight(view)));
        return !(horizontal > vLimit * cam->frustumAspect) && !(horizontal < -(vLimit * cam->frustumAspect));
    }
}

namespace AbyssEngine {
    Quaternion operator*(const Quaternion &a, float s) {
        return Quaternion(a.x * s, a.y * s, a.z * s, a.w * s);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys) {
        if (sys == 0)
            return;

        unsigned short count = sys->count;
        float *uvBase = (float *) sys->mesh->texCoords;

        for (unsigned short idx = 0; idx < (count << 3); idx += 8) {
            float *p = uvBase + idx;
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

namespace AbyssEngine {
    void getAppVersion() {
        Engine *self = AE_getInitGLThis();
        int width = AE_getInitGLWidth();
        int height = AE_getInitGLHeight();

        self->refractFBO = 0;
        self->displayWidth = width;
        self->displayHeight = height;
        self->viewportWidth = width;
        self->viewportHeight = height;

        FileInterfaceAndroid *fileIface = new FileInterfaceAndroid();
        self->fileInterface = fileIface;
        AEFile::SetInterface((FileInterface *) fileIface);

        self->lastGlError = 0;
        self->vibrationSupported = 0;
        self->hasVibration = false;

        char *shaderFlag = g_Engine_shaderModeFlag;
        self->viewFramebuffer = 0;
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
        self->field_0x468 = *(const Vector *) zero3;
        self->lightDirty[0] = 0;
        self->field_0x474 = *(const Vector *) zero3;
        self->lightDirty[1] = 0;

        glEnable(0xb71);
        self->GlEnable(0xde1, true);
        glDisable(0xbe2);
        glCullFace(0x405);
        glEnable(0xb44);
        self->AfterGLInit();
        (*(PaintCanvas **) self->appManager)->Initialize(false);

        self->maxTextureSize = 0;
        glGetIntegerv(0xd33, reinterpret_cast<GLint *>(&self->maxTextureSize));

        if (*shaderFlag != 0 && *g_Engine_fboEnabledFlag != 0) {
            FBOContainer *fbo = new FBOContainer(self, String(""));
            self->refractFBO = fbo;
            fbo->Create((int) self->displayWidth, (int) self->displayHeight, true, false);
        }
    }
}

namespace AbyssEngine {
    void SpriteSystemSetUv(unsigned short idx, float u0, float v0, float u1, float v1, SpriteSystem *sys) {
        unsigned int count = 0;
        if (sys != 0)
            count = sys->count;
        if (sys == 0 || idx >= count)
            return;

        unsigned char shader = Engine::enableShader;
        float *p = (float *) sys->mesh->texCoords + (unsigned int) idx * 8;
        p[0] = u0;
        if (shader) {
            p[1] = v0;
            p[2] = u1;
            p[3] = v0;
            p[4] = u1;
            p[5] = v1;
            p[6] = u0;
            p[7] = v1;
        } else {
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

namespace AbyssEngine {
    short ImageFontGetYOffset(ImageFont *font) {
        if (font == 0)
            return 0;
        return font->yOffset;
    }
}

namespace AbyssEngine {
    Quaternion operator-(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int vertexFormat,
                   void **out); // lint: void_ptr (exported MeshCreate symbol; void** baked into mangled name)

    void MeshRelease(Engine * engine, Mesh * *slot);

    int ImageCreateRegionFromFile(Engine *engine, const char *path, unsigned short index, Image2D *region) {
        if (engine == 0 || path == 0)
            return -4;

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
        if (AEFile::Read((uint32_t)(2), &region->atlasW, handle) == 0) return -1;
        if (AEFile::Read((uint32_t)(2), &region->atlasH, handle) == 0) return -1;
        if (AEFile::Read((uint32_t)(2), &regionCount, handle) == 0) return -1;
        if (regionCount <= index) return -1;

        if (MeshCreate(engine, 4, 2, 0x13, (void **) region) != 1) // lint: void_ptr (cast to exported MeshCreate void** param)
            // lint: void_ptr (cast to exported MeshCreate void** param)
            return -2;

        unsigned char mode = 0;
        for (unsigned short i = 0; i < regionCount; ++i) {
            if (i == index) {
                if (AEFile::Read((uint32_t)(2), &region->offX, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &region->offY, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &region->sizeX, handle) == 0) goto fail;
                if (AEFile::Read((uint32_t)(2), &region->sizeY, handle) == 0) goto fail;

                Mesh *mesh = region->mesh;
                float *pos = (float *) mesh->positions;

                float halfW = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->sizeX, mode);
                pos[0] = 0;
                pos[1] = 0;
                pos[2] = 0;
                pos[4] = 0;
                pos[5] = 0;
                pos[3] = halfW;
                pos[6] = halfW;
                float halfH = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->sizeY, mode);
                pos[8] = 0;
                pos[9] = 0;
                pos[11] = 0;
                pos[7] = halfH;
                pos[10] = halfH;

                double atlasH = (double)
                        AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->atlasH, mode);
                double atlasW = (double)
                        AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->atlasW, mode);
                float offYs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) region->offY, mode);
                float offXs = AbyssEngine::AEMath::VectorSignedToFloat((int) (short) region->offX, mode);
                float offYu = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->offY, mode);
                float offXu = AbyssEngine::AEMath::VectorUnsignedToFloat((unsigned int) region->offX, mode);

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

        unsigned short glyphCount = font->glyphCount;
        for (unsigned short i = 0; i < count; ++i) {
            unsigned short idx = 0;
            unsigned short code;
            for (;;) {
                unsigned int u = idx;
                if (glyphCount <= u)
                    goto next;
                idx = idx + 1;
                code = font->codes[u];
                if (code == str[i])
                    break;
            }
            {
                unsigned int found = (unsigned int) (unsigned short) (idx - 1);
                Mesh *glyph = font->glyphMeshes[found];
                int w = (int) ((float *) glyph->positions)[3];
                int adv = (int) font->spacing + w;
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
                            if (Engine::enableShader != 0)
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

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
void glVertexPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

// lint: void_ptr (OpenGL ABI signature)

void glNormalPointer(unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

void glColorPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)
}

namespace AbyssEngine {
    int MeshDraw(Engine *engine, Mesh *mesh) {
        if (mesh == 0 || (short) mesh->vertexCount == 0 || (mesh->vertexFormat & 1) == 0)
            return -4;

        if (Engine::enableShader == 0 && mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            engine->AEClientState(0x8074, true);
            glVertexPointer(3, 0x1406, 0, 0);
            glBindBuffer(0x8893, mesh->indexVBO);

            if (mesh->vertexFormat & 2) {
                glBindBuffer(0x8892, mesh->texCoordVBO);
                engine->AEClientState(0x8078, true);
                glTexCoordPointer(2, 0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8078, false);
            }

            if (mesh->vertexFormat & 4) {
                glBindBuffer(0x8892, mesh->normalVBO);
                engine->AEClientState(0x8075, true);
                glNormalPointer(0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8075, false);
            }

            if (mesh->vertexFormat & 8) {
                glBindBuffer(0x8892, mesh->colorVBO);
                engine->AEClientState(0x8076, true);
                glColorPointer(4, 0x1406, 0, 0);
            } else {
                engine->AEClientState(0x8076, false);
            }

            glDrawElements(4, (int) mesh->indexCount, 0x1403, 0);

            // Engine byte 0xfe enables the draw statistics, byte 0xfd selects the bucket.
            if (engine->field_0xfe != 0) {
                int tris = (int) ((unsigned) mesh->indexCount / (unsigned) 3);
                if (engine->statsEnabled != 0) {
                    engine->triangleCountB += tris;
                    engine->drawCallCountB += 1;
                } else {
                    engine->triangleCountA += tris;
                    engine->drawCallCountA += 1;
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
            count = sys->count;
        if (sys == 0 || idx >= count)
            return;

        float *p = (float *) sys->mesh->colors + (unsigned int) idx * 16;
        p[0] = r;
        p[1] = g;
        p[2] = b;
        p[3] = a;
        p[4] = r;
        p[5] = g;
        p[6] = b;
        p[7] = a;
        p[8] = r;
        p[9] = g;
        p[10] = b;
        p[11] = a;
        p[12] = r;
        p[13] = g;
        p[14] = b;
        p[15] = a;
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    void CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam) {
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
            f[0x15] = 1.0f / AbyssEngine::AEMath::Cosf(at);
        }
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start,
                          unsigned int len) {
        if (font == 0 || str == 0)
            return 0;

        unsigned int end = len + start;
        unsigned short glyphCount = font->glyphCount;
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
                code = font->codes[u];
                if (code == str[start & 0xffff]) {
                    found = true;
                    break;
                }
            }
            if (!found)
                continue;

            unsigned int gi = (unsigned int) (unsigned short) (idx - 1);
            Mesh *glyph = font->glyphMeshes[gi];
            int w = (int) ((float *) glyph->positions)[3];
            int adv = (int) font->spacing + w;
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

        CurveKeyframe **data = 0;
        for (unsigned int i = 0; (data = (CurveKeyframe **) curve->entries),
                                 i < (unsigned int) curve->count; ++i) {
            CurveKeyframe *entry = data[i];
            int8_t tag = (int8_t) entry->tag;
            if (tag == 3 || tag == 2)
                operator delete(entry);
            else if (tag == 1)
                operator delete(entry);
            curve = (Curve *) *slot;
        }

        if (data != 0) {
            operator delete[](data);
            curve = (Curve *) *slot;
        }
        curve->entries = 0;

        if (*slot != 0)
            operator delete(*slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    int MeshDraw(Engine * engine, Mesh * mesh);

    void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures) {
        if (canvas == 0 || mat == 0)
            return;

        if (setTextures) {
            engine->SetTexturesExt(mat->textures[0], mat->textures[1], mat->textures[2], 0xffffffff);
        }
        engine->SetAddData(mat->addData, (int) mat->addDataSize);

        // Remember the light's ambient colour so it can be restored after drawing.
        LightColor saved = {0.0f, 0.0f, 0.0f, 0.0f};
        if (mat->ambientColor.x != -10.0f) {
            saved = engine->lightAmbient;
            engine->LightSetLightColorAmbient(mat->ambientColor.x, mat->ambientColor.y, mat->ambientColor.z, 0x4000);
        }

        for (unsigned int i = 0; i < mat->meshes.count; ++i) {
            Matrix world = mat->arr_5c.data_[i] * mat->arr_2c.data_[i];
            canvas->SetWorldViewMatrix(world);
            engine->SetModelMatrix(mat->arr_2c.data_[i]);
            engine->SetUVMatrix(mat->arr_38.data_[i]);

            unsigned int packed = mat->arr_50.data_[i];
            engine->SetColor((float) ((packed >> 24) / 255.0), (float) (((packed >> 16) & 0xff) / 255.0),
                             (float) (((packed >> 8) & 0xff) / 255.0), (float) ((packed & 0xff) / 255.0));

            MeshDraw(engine, mat->meshes.data_[i]);
        }

        if (mat->ambientColor.x != -10.0f) {
            engine->LightSetLightColorAmbient(saved.r, saved.g, saved.b, 0x4000);
        }

        mat->arr_5c.count = 0;
        mat->arr_2c.count = 0;
        mat->arr_38.count = 0;
        mat->meshes.count = 0;
        mat->arr_50.count = 0;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void ImageFontRelease(Engine *engine, ImageFont **slot) {
        if (*slot == 0)
            return;

        uint16_t *table = (*slot)->codes;
        if (table != 0)
            operator delete[](table);
        (*slot)->codes = 0;

        unsigned int i = 0;
        Mesh **glyphs;
        ImageFont *f;
        for (;;) {
            f = *slot;
            glyphs = f->glyphMeshes;
            if (f->glyphCount <= i)
                break;
            MeshRelease(engine, &glyphs[i]);
            ++i;
        }

        if (glyphs != 0) {
            operator delete[](glyphs);
            f = *slot;
        }
        f->glyphMeshes = 0;

        if (*slot != 0)
            operator delete(*slot);
        *slot = 0;
    }
}

namespace AbyssEngine {

    int MeshReadData(Engine *engine, const unsigned int &handleRef, unsigned int flags, Mesh **slot,
                     Material *mat);

    int MeshReadData(Engine *engine, const unsigned int &handle, unsigned int flags, Mesh **slot,
                     Material *mat) {
        unsigned int enhanced = flags & 0x1a;
        if (enhanced != 0 && AEFile::Read(12, &(*slot)->pivotX, handle) == 0)
            return -1;

        if ((*slot)->vertexFormat & 0x10) {
            if (AEFile::Read(2, &(*slot)->indexCount, handle) == 0)
                return -1;
            Mesh *mesh = *slot;
            mesh->indices = new unsigned char[mesh->indexCount << 1];
            if (AEFile::Read((*slot)->indexCount << 1, (*slot)->indices, handle) == 0)
                return -1;
        }

        if (AEFile::Read(2, &(*slot)->vertexCount, handle) == 0)
            return -1;

        float minimum[3];
        float maximum[3];
        minimum[0] = minimum[1] = minimum[2] = 10000000.0f;
        maximum[0] = maximum[1] = maximum[2] = -10000000.0f;
        unsigned int packed = flags & 4;

        // Positions: 16-bit (packed) or 32-bit integer streams are converted to floats while the
        // axis-aligned bounds are collected; float streams are read verbatim.
        if (packed != 0) {
            Mesh *mesh = *slot;
            short *raw = (short *) new unsigned char[mesh->vertexCount * 6];
            if (AEFile::Read(mesh->vertexCount * 6, raw, handle) == 0) {
                delete[] (unsigned char *) raw;
                return -1;
            }
            mesh = *slot;
            mesh->positions = new unsigned char[mesh->vertexCount * 12];
            mesh = *slot;
            unsigned int count = mesh->vertexCount * 3;
            for (unsigned int i = 0; i != count; i++) {
                unsigned int axis = i % 3;
                float value = raw[i];
                ((float *) mesh->positions)[i] = value;
                if (minimum[axis] > value)
                    minimum[axis] = value;
                if (maximum[axis] < value)
                    maximum[axis] = value;
            }
            delete[] (unsigned char *) raw;
        } else if ((flags & 3) != 0) {
            Mesh *mesh = *slot;
            int *raw = (int *) new unsigned char[mesh->vertexCount * 12];
            if (AEFile::Read(mesh->vertexCount * 12, raw, handle) == 0) {
                delete[] (unsigned char *) raw;
                return -1;
            }
            mesh = *slot;
            mesh->positions = new unsigned char[mesh->vertexCount * 12];
            mesh = *slot;
            unsigned int count = mesh->vertexCount * 3;
            for (unsigned int i = 0; i != count; i++) {
                unsigned int axis = i % 3;
                float value = raw[i];
                ((float *) mesh->positions)[i] = value;
                if (minimum[axis] > value)
                    minimum[axis] = value;
                if (maximum[axis] < value)
                    maximum[axis] = value;
            }
            delete[] (unsigned char *) raw;
        } else if ((flags & 0x18) != 0) {
            Mesh *mesh = *slot;
            mesh->positions = new unsigned char[mesh->vertexCount * 12];
            if (AEFile::Read((*slot)->vertexCount * 12, (*slot)->positions, handle) == 0)
                return -1;
        }

        Vector center = {(maximum[0] + minimum[0]) * 0.5f, (maximum[1] + minimum[1]) * 0.5f,
                         (maximum[2] + minimum[2]) * 0.5f};
        *(Vector *) &(*slot)->boundsCenterX = center;
        Vector high = {maximum[0], maximum[1], maximum[2]};
        Vector low = {minimum[0], minimum[1], minimum[2]};
        (*slot)->boundsRadius = AEMath::VectorLength(high - low);

        if ((*slot)->vertexFormat & 2) {
            if (packed != 0) {
                Mesh *mesh = *slot;
                short *raw = (short *) new unsigned char[mesh->vertexCount * 4];
                if (AEFile::Read(mesh->vertexCount * 4, raw, handle) == 0) {
                    delete[] (unsigned char *) raw;
                    return -1;
                }
                mesh = *slot;
                mesh->texCoords = new unsigned char[mesh->vertexCount * 8];
                mesh = *slot;
                bool flipV = Engine::enableShader != 0;
                for (unsigned int i = 0; i < (unsigned int) (mesh->vertexCount << 1); i += 2) {
                    float *uv = (float *) mesh->texCoords + i;
                    uv[0] = (float) ((double) raw[i] * (1.0 / 4096.0));
                    double v = (double) raw[i + 1] * (1.0 / 4096.0);
                    uv[1] = (float) (flipV ? 1.0 - v : v);
                }
                delete[] (unsigned char *) raw;
            } else if ((flags & 0x18) != 0) {
                Mesh *mesh = *slot;
                mesh->texCoords = new unsigned char[mesh->vertexCount * 8];
                if (AEFile::Read((*slot)->vertexCount * 8, (*slot)->texCoords, handle) == 0)
                    return -1;
                if (Engine::enableShader != 0) {
                    mesh = *slot;
                    for (unsigned int i = 0; i < (unsigned int) (mesh->vertexCount << 1); i += 2) {
                        float *v = (float *) mesh->texCoords + i + 1;
                        *v = 1.0f - *v;
                    }
                }
            }
        }

        if ((*slot)->vertexFormat & 4) {
            if (packed != 0) {
                Mesh *mesh = *slot;
                short *raw = (short *) new unsigned char[mesh->vertexCount * 6];
                if (AEFile::Read(mesh->vertexCount * 6, raw, handle) == 0) {
                    delete[] (unsigned char *) raw;
                    return -1;
                }
                mesh = *slot;
                mesh->normals = new unsigned char[mesh->vertexCount * 12];
                for (unsigned int i = 0; i < (unsigned int) ((*slot)->vertexCount * 3); i += 3) {
                    float x = (float) ((double) raw[i] * (1.0 / 32768.0));
                    float y = (float) ((double) raw[i + 1] * (1.0 / 32768.0));
                    float z = (float) ((double) raw[i + 2] * (1.0 / 32768.0));
                    float length = sqrtf(x * x + y * y + z * z);
                    float *normal = (float *) (*slot)->normals + i;
                    if (length == 0.0f) {
                        normal[1] = 1.0f;
                        normal[0] = 0.0f;
                        normal[2] = 0.0f;
                    } else {
                        x /= length;
                        y /= length;
                        z /= length;
                        float cx = (double) x < -1.0 ? -1.0f : x;
                        float cy = (double) y < -1.0 ? -1.0f : y;
                        float cz = (double) z < -1.0 ? -1.0f : z;
                        if (x > 1.0f)
                            cx = 1.0f;
                        if (y > 1.0f)
                            cy = 1.0f;
                        if (z > 1.0f)
                            cz = 1.0f;
                        normal[0] = cx;
                        normal[1] = cy;
                        normal[2] = cz;
                    }
                }
                delete[] (unsigned char *) raw;
            } else if ((flags & 0x18) != 0) {
                Mesh *mesh = *slot;
                mesh->normals = new unsigned char[mesh->vertexCount * 12];
                if (AEFile::Read((*slot)->vertexCount * 12, (*slot)->normals, handle) == 0)
                    return -1;
            }

            if (Engine::enableShader != 0) {
                // Per-vertex tangent frames from the UV gradients of the triangle list.
                Mesh *mesh = *slot;
                mesh->tangents = new unsigned char[mesh->vertexCount * 12];
                mesh = *slot;
                mesh->binormals = new unsigned char[mesh->vertexCount * 12];
                mesh = *slot;
                unsigned int triangles = mesh->indexCount / 3;
                unsigned int vertices = mesh->vertexCount;
                Vector *accum = (Vector *) new unsigned char[vertices * 12];
                for (unsigned int v = 0; v < vertices; v++) {
                    accum[v].x = 0.0f;
                    accum[v].y = 0.0f;
                    accum[v].z = 0.0f;
                }
                for (unsigned int t = 0; t < triangles; t++) {
                    mesh = *slot;
                    const Vector *pos = (const Vector *) mesh->positions;
                    const float *uv = (const float *) mesh->texCoords;
                    const unsigned short *index = (const unsigned short *) mesh->indices + t * 3;
                    unsigned int i0 = index[0];
                    unsigned int i1 = index[1];
                    unsigned int i2 = index[2];
                    float du1 = uv[i1 * 2] - uv[i0 * 2];
                    float dv1 = uv[i1 * 2 + 1] - uv[i0 * 2 + 1];
                    float du2 = uv[i2 * 2] - uv[i0 * 2];
                    float dv2 = uv[i2 * 2 + 1] - uv[i0 * 2 + 1];
                    float r = 1.0f / (du1 * dv2 - du2 * dv1);
                    Vector tangent;
                    tangent.x = ((pos[i1].x - pos[i0].x) * dv2 - (pos[i2].x - pos[i0].x) * dv1) * r;
                    tangent.y = ((pos[i1].y - pos[i0].y) * dv2 - (pos[i2].y - pos[i0].y) * dv1) * r;
                    tangent.z = ((pos[i1].z - pos[i0].z) * dv2 - (pos[i2].z - pos[i0].z) * dv1) * r;
                    accum[i0] += tangent;
                    accum[i1] += tangent;
                    accum[i2] += tangent;
                }
                for (unsigned int v = 0; v < vertices; v++) {
                    Vector normal = ((const Vector *) (*slot)->normals)[v];
                    Vector tangent = accum[v];
                    Vector projected = tangent - normal * AEMath::VectorDot(normal, tangent);
                    Vector unit = AEMath::VectorNormalize(projected);
                    Vector *outTangent = (Vector *) (*slot)->tangents + v;
                    outTangent->x = unit.x;
                    outTangent->y = unit.y;
                    outTangent->z = unit.z;
                    Vector binormal = AEMath::VectorCross(normal, unit);
                    Vector *outBinormal = (Vector *) (*slot)->binormals + v;
                    outBinormal->x = binormal.x;
                    outBinormal->y = binormal.y;
                    outBinormal->z = binormal.z;
                }
            }
        }

        if ((*slot)->vertexFormat & 8) {
            if (packed != 0) {
                Mesh *mesh = *slot;
                unsigned char *raw = new unsigned char[mesh->vertexCount * 4];
                if (AEFile::Read(mesh->vertexCount * 4, raw, handle) == 0) {
                    delete[] raw;
                    return -1;
                }
                mesh = *slot;
                mesh->colors = new unsigned char[mesh->vertexCount * 16];
                mesh = *slot;
                for (unsigned int i = 0; i < (unsigned int) (mesh->vertexCount << 2); i++)
                    ((float *) mesh->colors)[i] = (float) raw[i] / 255.0f;
                delete[] raw;
            } else if ((flags & 0x18) != 0) {
                Mesh *mesh = *slot;
                mesh->colors = new unsigned char[mesh->vertexCount * 16];
                unsigned int ok = AEFile::Read((*slot)->vertexCount * 16, (*slot)->colors, handle);
                int result = ok != 0 ? 1 : -1;
                if (enhanced == 0)
                    return result;
                if (ok == 0)
                    return result;
            }
        }

        if (enhanced == 0)
            return 1;

        if ((*slot)->ReadEnhancedDataFromFile(handle, flags) == 0)
            return -1;
        unsigned short children;
        if (AEFile::Read(2, &children, handle) == 0)
            return -1;
        if ((*slot)->animation != 0)
            ((AEMath::BSphere *) &(*slot)->boundsCenterX)->Merge((*slot)->animation->bounds());
        for (unsigned int c = 0; c < children; c++) {
            Mesh *child = new Mesh();
            child->vboEligible = 1;
            child->vertexFormat = (*slot)->vertexFormat;
            child->material = (*slot)->material;
            if (MeshReadData(engine, handle, flags, &child, mat) == -1)
                return -1;
            ((AEMath::BSphere *) &(*slot)->boundsCenterX)->Merge(*(const AEMath::BSphere *) &child->boundsCenterX);
            ArrayAdd(child, (*slot)->animation->meshes);
        }
        return 1;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const long long &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    void TransformRelease(Engine * engine, Transform * *slot);

    void MeshReleaseIntern(Engine *engine, Mesh **slot);

    void MeshRelease(Engine *engine, Mesh **slot) {
        if (engine != 0 && *slot != 0) {
            TransformRelease(engine, &(*slot)->animation);

            MeshReleaseIntern(engine, slot);
        }
    }
}

namespace AbyssEngine {
    String operator+(const float &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int fmt,
                   void **out); // lint: void_ptr (exported MeshCreate symbol; void** baked into mangled name)

    void MeshRelease(Engine * engine, Mesh * *slot);

    int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out) {
        unsigned int n = count;
        if (n == 0)
            return -4;

        SpriteSystem *sys = (SpriteSystem *) operator new(0x14);
        sys->posCpu = 0;
        sys->sizeCpu = 0;
        sys->mesh = 0;
        *out = sys;
        sys->count = count;

        unsigned int triCount = (n << 1) & 0xffff;
        int rc = MeshCreate(engine, (unsigned short) ((n & 0x3fff) << 2), (unsigned short) triCount,
                            0x1f, (void **) &sys->mesh); // lint: void_ptr (cast to exported MeshCreate void** param)
        SpriteSystem *s = *out;
        if (rc != 1) {
            MeshRelease(engine, &s->mesh);
            if (s->posCpu != 0)
                operator delete[]((*out)->posCpu);
            (*out)->posCpu = 0;
            if ((*out)->sizeCpu != 0)
                operator delete[]((*out)->sizeCpu);
            (*out)->sizeCpu = 0;
            if (*out != 0)
                operator delete(*out);
            *out = 0;
            return -1;
        }

        Mesh *mesh = s->mesh;
        int *indexArr = (int *) mesh->indices;

        unsigned char *posCpu = (unsigned char *) operator new[](n * 0xc);
        s->posCpu = (float *) posCpu;
        memset((*out)->posCpu, 0, n * 0xc);

        s = *out;
        s->sharedSize = sharedSize ? 1 : 0;
        if (sharedSize) {
            unsigned char *sz = (unsigned char *) operator new[](2);
            s->sizeCpu = (int16_t *) sz;
            *(unsigned short *) (*out)->sizeCpu = 0;
        } else {
            unsigned char *sz = (unsigned char *) operator new[]((n << 1));
            s->sizeCpu = (int16_t *) sz;
            memset((*out)->sizeCpu, 0, (n << 1));
        }

        unsigned short base = 0;
        unsigned short *idx = (unsigned short *) indexArr;
        for (unsigned short off = 0; off < n * 6; off += 6) {
            unsigned short *p = idx + off;
            p[0] = base;
            p[1] = base | 1;
            p[2] = base | 2;
            p[3] = base;
            p[4] = base | 2;
            p[5] = base | 3;
            base = base + 4;
        }

        Mesh *m = (*out)->mesh;
        unsigned int vcount = (unsigned int) (unsigned short) m->vertexCount;

        float *colors = (float *) m->colors;
        for (unsigned int i = 0; i <= vcount * 4 && vcount * 4 - i != 0; ++i)
            colors[i] = 1.0f;

        char tangent = Engine::enableShader;
        float *normals = (float *) m->normals;
        float *tangents = (float *) m->tangents;
        float *binormals = (float *) m->binormals;
        unsigned int vi = 0;
        for (unsigned int k = vcount; k != 0; --k) {
            float *nrm = normals + vi;
            nrm[0] = 0.0f;
            nrm[1] = 0.0f;
            nrm[2] = 1.0f;
            if (tangent != 0) {
                float *tan = tangents + vi;
                tan[0] = 1.0f;
                tan[1] = 0.0f;
                tan[2] = 0.0f;
                float *bin = binormals + vi;
                bin[0] = 0.0f;
                bin[1] = 1.0f;
                bin[2] = 0.0f;
            }
            vi += 3;
        }

        return 1;
    }
}

namespace AbyssEngine {
    void ImageRelease(Image * *slot);

    int ImageCreateFromFile(Engine *engine, const char *path, Image **out) {
        if (engine == 0 || path == 0)
            return -4;

        Image *img = (Image *) ::operator new(0x14);
        img->hasMipmaps = 0;
        *(uint32_t *) &img->width = 0;
        img->format = 0;
        img->data = 0;
        *out = img;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) (&handle)) == 0) {
            if (*out != 0)
                ::operator delete(*out);
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
            Image *im = *out;
            unsigned char fmt = 0;
            unsigned short palCount = 0;
            if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &im->width, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &im->height, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &palCount, handle) == 0) goto fail;

            AEFile::Skip((uint32_t)((unsigned int) palCount << 3), handle);

            if (fmt & 2)
                im->hasMipmaps = 1;

            unsigned int dataLen = 0;
            switch (fmt) {
                case 1:
                case 3:
                case 0x81: {
                    unsigned int sz = (unsigned int) im->width * (unsigned int) im->height * 4;
                    unsigned char *p = (unsigned char *) ::operator new[](sz);
                    im->data = p;
                    im = *out;
                    if (AEFile::Read((uint32_t)(sz), im->data, handle) == 0) goto fail;
                    im = *out;
                    im->dataLen = sz;
                    {
                        unsigned int v = (unsigned int) (int) (signed char) fmt;
                        unsigned int code = ((int) v < 0) ? ((v > 0x7fffffff) ? 6u : v) : 3u;
                        if ((int) v >= 0) code = 3;
                        im->format = code;
                    }
                    break;
                }
                case 0xd:
                case 0xf:
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = (unsigned char *) ::operator new[](dataLen);
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 4;
                    im->dataLen = dataLen;
                    break;
                case 0x10:
                case 0x12:
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = (unsigned char *) ::operator new[](dataLen);
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 5;
                    im->dataLen = dataLen;
                    break;
                case 0x11:
                case 0x13:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 7;
                    im->data = (unsigned char *) ::operator new[](im->dataLen);
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x14:
                case 0x16:
                case 0x17:
                case 0x40:
                case 0x42:
                    if (fmt == 0x40)
                        im->hasMipmaps = 0;
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = (unsigned char *) ::operator new[](dataLen);
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 0xb;
                    im->dataLen = dataLen;
                    if (fmt == 0x17)
                        im->hasMipmaps = 0;
                    break;
                case 0x20:
                case 0x22:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 8;
                    im->data = (unsigned char *) ::operator new[](im->dataLen);
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x21:
                case 0x23:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 9;
                    im->data = (unsigned char *) ::operator new[](im->dataLen);
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x24:
                case 0x26:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 10;
                    im->data = (unsigned char *) ::operator new[](im->dataLen);
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
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
            operator delete[]((*slot)->data);
            (*slot)->data = 0;
            if (*slot != 0)
                operator delete(*slot);
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
                operator delete(*slot);
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

        memset(m, 0, 0x88);
        m->boundsRadiusSq = 1.0f;

        *out = m;
        m->vertexCount = (short) vertexCount;
        m->vertexFormat = vertexFormat;
        m->indexCount = (short) (triCount + (triCount << 1));
        m->field_0x2a = (short) triCount;

        unsigned int posBytes = (unsigned int) vertexCount * 0xc;
        unsigned char *p = (unsigned char *) operator new[](posBytes);
        m->positions = p;
        memset(p, 0, posBytes);

        if (vertexFormat & 0x10) {
            p = (unsigned char *) operator new[]((unsigned int) triCount * 6);
            m->indices = p;
            memset(p, 0, (unsigned int) triCount * 6);
        }
        if (vertexFormat & 2) {
            p = (unsigned char *) operator new[]((unsigned int) vertexCount << 3);
            m->texCoords = p;
            memset(p, 0, (unsigned int) vertexCount << 3);
        }
        if (vertexFormat & 4) {
            p = (unsigned char *) operator new[](posBytes);
            m->normals = p;
            memset(p, 0, posBytes);
            if (*g_Mesh_tangentEnabledFlag != 0) {
                p = (unsigned char *) operator new[](posBytes);
                m->tangents = p;
                memset(p, 0, posBytes);
                p = (unsigned char *) operator new[](posBytes);
                m->binormals = p;
                memset(p, 0, posBytes);
            }
        }
        if (vertexFormat & 8) {
            p = (unsigned char *) operator new[]((unsigned int) vertexCount << 4);
            m->colors = p;
            memset(p, 0, (unsigned int) vertexCount << 4);
        }

        return 1;
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    void CameraSetPerspective(float p1, float aspectNum, float fov, float aspectDen, float near,
                              Camera *cam) {
        if (cam != 0) {
            float *f = (float *) cam;
            f[0] = p1;
            f[1] = aspectNum;
            f[2] = fov;

            float half = p1 * 0.5f;
            float s = AbyssEngine::AEMath::Sinf(half);
            float c = AbyssEngine::AEMath::Cosf(half);
            f[0x12] = s / c;
            f[0x13] = (aspectDen / near) * (s / c);
            f[0x14] = aspectDen / near;

            f[0x16] = 1.0f / AbyssEngine::AEMath::Cosf(half);
            f[0x15] = 1.0f / AbyssEngine::AEMath::Cosf(AbyssEngine::AEMath::ATanf(f[0x12] * f[0x14]));
        }
    }
}

namespace AbyssEngine {
    void ImageFontSetSpacing(ImageFont *font, short spacing) {
        if (font != 0)
            font->spacing = spacing;
    }
}

namespace AbyssEngine {
    int ImageFontGetHeight(ImageFont *font) {
        if (font != 0) {
            Mesh **p = font->glyphMeshes;
            Mesh *q = p[0];
            float *r = (float *) q->positions;
            int v = (int) r[7];
            if (v == 0x18)
                v -= 5; // ASM: original subtracts 5 (subeq) rather than moving 19 (moveq); v==0x18 here
            return v;
        }
        return 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const int &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    short ImageFontGetSpacing(ImageFont *font) {
        short v = (font == 0) ? (short) 0 : font->spacing;
        return (int) v;
    }
}

namespace AbyssEngine {
    String operator+(const String &a) {
        return String(a);
    }
}

namespace AbyssEngine {
    void ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x,
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

    void ImageFontDrawString(ImageFont *font, const unsigned short *text, unsigned int len, int x, int y,
                             PaintCanvas *canvas, Engine *engine, bool rtl) {
        bool valid = text != 0 && font != 0;

        if (engine->statsBucketFlag == 0) {
            // Immediate mode: one draw call per glyph mesh, translated by the world-view matrix.
            if (!valid)
                return;
            if (ImageFontGetWidth(font, text, len) + x < 0)
                return;
            int top = font->yOffset;
            int height = ImageFontGetHeight(font);
            if ((int) engine->GetDisplayWidth() < x)
                return;
            top += y;
            if (height + top < 0)
                return;
            if (top > (int) engine->GetDisplayHeight())
                return;

            AEMath::Matrix placement;
            float glyphY = (float) (top - 2);
            placement.m[3] = (float) x;
            placement.m[7] = glyphY;
            bool forward = (canvas->field_0x1c | rtl) != 0;
            int step = forward ? 1 : -1;
            int index = forward ? 0 : len - 1;

            for (unsigned short i = 0; i < len; i++) {
                for (unsigned short slot = 0; slot < font->glyphCount; slot++) {
                    if (font->codes[slot] != text[index])
                        continue;
                    placement.m[7] = glyphY;
                    placement.m[3] = (float) x;
                    int advance = (int) ((float *) font->glyphMeshes[slot]->positions)[3];
                    if (x + advance >= 0 && x <= (int) engine->GetDisplayWidth()) {
                        canvas->SetWorldViewMatrix(placement);
                        MeshDraw(engine, font->glyphMeshes[slot]);
                    }
                    int width = font->spacing + advance;
                    int stepX = width;
                    if (text[index] == ' ')
                        stepX = width - 2;
                    if (advance != 11)
                        stepX = width;
                    x += stepX;
                    break;
                }
                index += step;
            }
            return;
        }

        // Batched mode: glyph quads are appended to the canvas' 2D quad mesh and flushed per 100.
        if (!valid)
            return;
        if (ImageFontGetWidth(font, text, len) + x < 0)
            return;
        int top = font->yOffset;
        int height = ImageFontGetHeight(font);
        if ((int) engine->GetDisplayWidth() < x)
            return;
        top += y;
        if (height + top < 0)
            return;
        if (top > (int) engine->GetDisplayHeight())
            return;

        bool forward = (canvas->field_0x1c | rtl) != 0;
        int step = forward ? 1 : -1;
        int index = forward ? 0 : len - 1;
        if (Engine::enableReverseFlag != 0 && GameText::getLanguage() == 9 &&
            GameText::isNonArabicString(text, len) != 0) {
            index = 0;
            step = 1;
        }

        float glyphY = (float) (top - 2);
        for (unsigned short i = 0; i < len; i++) {
            for (unsigned short slot = 0; slot < font->glyphCount; slot++) {
                if (font->codes[slot] != text[index])
                    continue;
                int advance = (int) ((float *) font->glyphMeshes[slot]->positions)[3];
                if (x + advance >= 0 && x <= (int) engine->GetDisplayWidth()) {
                    Mesh *batch = (Mesh *) canvas->quad2dMesh;
                    int n = canvas->field_0xc;
                    Mesh *glyph = font->glyphMeshes[slot];
                    float glyphX = (float) x;
                    float *src = (float *) glyph->positions;
                    float *dst = (float *) batch->positions;
                    dst[n * 12 + 0] = src[0] + glyphX;
                    dst[n * 12 + 1] = src[1] + glyphY;
                    dst[n * 12 + 3] = src[3] + glyphX;
                    dst[n * 12 + 4] = src[4] + glyphY;
                    dst[n * 12 + 6] = src[6] + glyphX;
                    dst[n * 12 + 7] = src[7] + glyphY;
                    dst[n * 12 + 9] = src[9] + glyphX;
                    dst[n * 12 + 10] = src[10] + glyphY;

                    unsigned int *uvSrc = (unsigned int *) glyph->texCoords;
                    unsigned int *uvDst = (unsigned int *) batch->texCoords;
                    uvDst[n * 8 + 0] = uvSrc[0];
                    uvDst[n * 8 + 1] = uvSrc[1];
                    uvDst[n * 8 + 2] = uvSrc[2];
                    uvDst[n * 8 + 3] = uvSrc[3];
                    uvDst[n * 8 + 4] = uvSrc[4];
                    uvDst[n * 8 + 5] = uvSrc[5];
                    uvDst[n * 8 + 6] = uvSrc[6];
                    uvDst[n * 8 + 7] = uvSrc[7];

                    float *colors = (float *) batch->colors;
                    colors[n * 16 + 0] = engine->glColor[0];
                    colors[n * 16 + 1] = engine->glColor[1];
                    colors[n * 16 + 2] = engine->glColor[2];
                    colors[n * 16 + 3] = engine->glColor[3];
                    colors[n * 16 + 4] = engine->glColor[0];
                    colors[n * 16 + 5] = engine->glColor[1];
                    colors[n * 16 + 6] = engine->glColor[2];
                    colors[n * 16 + 7] = engine->glColor[3];
                    colors[n * 16 + 8] = engine->glColor[0];
                    colors[n * 16 + 9] = engine->glColor[1];
                    colors[n * 16 + 10] = engine->glColor[2];
                    colors[n * 16 + 11] = engine->glColor[3];
                    colors[n * 16 + 12] = engine->glColor[0];
                    colors[n * 16 + 13] = engine->glColor[1];
                    colors[n * 16 + 14] = engine->glColor[2];
                    colors[n * 16 + 15] = engine->glColor[3];
                    canvas->field_0xc = n + 1;

                    if (n >= 99) {
                        batch->indexCount = (short) ((n + 1) * 6);
                        AEMath::Matrix identity;
                        canvas->SetWorldViewMatrix(identity);
                        MeshDraw(engine, (Mesh *) canvas->quad2dMesh);
                        canvas->field_0xc = 0;
                    }
                }
                int width = font->spacing + advance;
                int stepX = width;
                if (text[index] == ' ')
                    stepX = width - 2;
                if (advance != 11)
                    stepX = width;
                x += stepX;
                break;
            }
            index += step;
        }

        int pending = canvas->field_0xc;
        if (pending > 0) {
            ((Mesh *) canvas->quad2dMesh)->indexCount = (short) (pending * 6);
            AEMath::Matrix identity;
            canvas->SetWorldViewMatrix(identity);
            MeshDraw(engine, (Mesh *) canvas->quad2dMesh);
            canvas->field_0xc = 0;
        }
    }
}

namespace AbyssEngine {
    String operator+(const int &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   unsigned int vertexFormat,
                   void **out); // lint: void_ptr (exported MeshCreate symbol; void** baked into mangled name)

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
            ImageFont *font = (ImageFont *) ::operator new(0x14);
            font->glyphCount = 0;
            *(unsigned long long *) &font->codes = 0;
            *(unsigned long long *) &font->glyphMeshes = 0;
            *out = font;
        }

        for (unsigned short g = 0; g < glyphCount; ++g) {
            if (g == index) {
                ImageFont *font = *out;
                if (AEFile::Read((uint32_t)(2), &font->glyphCount, handle) == 0) goto fail;
                unsigned char *codes = (unsigned char *) ::operator new[]((unsigned int) font->glyphCount << 1);
                font->codes = (uint16_t *) codes;
                if (AEFile::Read((uint32_t)((unsigned int) (*out)->glyphCount << 1), (*out)->codes,
                                 handle) == 0)
                    goto fail;
                font = *out;
                unsigned char *meshes = (unsigned char *) ::operator new[]((unsigned int) font->glyphCount << 2);
                font->glyphMeshes = (Mesh **) meshes;

                unsigned int n = (*out)->glyphCount;
                for (unsigned int gi = 0; gi < n; ++gi) {
                    if (MeshCreate(engine, 4, 2, 0x13, (void **) &(*out)->glyphMeshes[gi]) != 1) // lint: void_ptr (cast to exported MeshCreate void** param)
                        // lint: void_ptr (cast to exported MeshCreate void** param)
                        goto fail;
                    unsigned short sizeY = 0, sizeX = 0, offX = 0, offY = 0;
                    if (AEFile::Read((uint32_t)(2), &sizeY, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &sizeX, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &offX, handle) == 0) goto fail;
                    if (AEFile::Read((uint32_t)(2), &offY, handle) == 0) goto fail;
                    Mesh *mesh = (*out)->glyphMeshes[gi];
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
        return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllSize(short size, SpriteSystem *sys) {
        if (sys == 0)
            return;

        short *sizes = sys->sizeCpu;
        if (sys->sharedSize != 0) {
            sizes[0] = size;
            return;
        }
        for (unsigned int i = 0; i < sys->count; ++i)
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
        unsigned int u;
        memcpy(&u, &f, sizeof(u));
        return u;
    }

    void SpriteSystemDraw(Engine *engine, const Matrix &view, const Matrix &world, SpriteSystem *sys) {
        if (sys == 0)
            return;

        float *vertices = (float *) sys->mesh->positions;
        Matrix combined = view * world;  // computed by the original as well; the quads use `world`

        unsigned int floats = sys->count * 12;
        unsigned char sizeStep = sys->sharedSize ^ 1;
        const float *positions = sys->posCpu;
        const short *sizes = sys->sizeCpu;
        unsigned short positionIndex = 0;
        unsigned short sizeIndex = 0;
        for (unsigned short q = 0; q < floats; q += 12) {
            const float *p = positions + positionIndex;
            positionIndex += 3;
            float half = (float) (sizes[sizeIndex] >> 1);
            sizeIndex += sizeStep;

            float x = world.m[0] * p[0] + world.m[1] * p[1] + world.m[2] * p[2] + world.m[3];
            float y = p[0] * world.m[4] + p[1] * world.m[5] + p[2] * world.m[6] + world.m[7];
            float z = p[0] * world.m[8] + p[1] * world.m[9] + p[2] * world.m[10] + world.m[11];

            float *quad = vertices + q;
            quad[0] = x - half;
            quad[1] = y - half;
            quad[2] = z;
            quad[3] = x + half;
            quad[4] = y - half;
            quad[5] = z;
            quad[6] = x + half;
            quad[7] = y + half;
            quad[8] = z;
            quad[9] = x - half;
            quad[10] = y + half;
            quad[11] = z;
        }

        Mesh *mesh = sys->mesh;
        if (mesh->material == 0) {
            MeshDraw(engine, mesh);
            return;
        }
        ArrayAddCached(mesh, mesh->material->meshes);
        ArrayAddCached(view, sys->mesh->material->arr_2c);
        Matrix flipY;
        flipY.m[5] = -1.0f;
        ArrayAddCached(flipY, sys->mesh->material->arr_38);
        ArrayAddCached(world, sys->mesh->material->arr_5c);
        ArrayAddCached(0xffffffffu, sys->mesh->material->arr_50);
    }
}

namespace AbyssEngine {
    int MeshConvertToVBOIntern(Mesh *m) {
        if (m == 0 || !Engine::vboSupported)
            return -4;
        if (m->uploaded != 0 || (short) m->indexCount == 0)
            return -4;

        unsigned int vcount = m->vertexCount;

        float *colArr = (float *) m->colors;
        float *tanArr = (float *) m->tangents;
        float *binArr = (float *) m->binormals;

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
            if (Engine::enableShader != 0) {
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
            if (Engine::KeepRawMeshData == 0) {
                if (m->positions != 0) operator delete[](m->positions);
                m->positions = 0;
                if (m->texCoords != 0) operator delete[](m->texCoords);
                m->texCoords = 0;
                if (m->indices != 0) operator delete[](m->indices);
                m->indices = 0;
                if (m->normals != 0) operator delete[](m->normals);
                m->normals = 0;
                if (m->colors != 0) operator delete[](m->colors);
                m->colors = 0;
                if (m->tangents != 0) operator delete[](m->tangents);
                m->tangents = 0;
                if (m->binormals != 0) operator delete[](m->binormals);
                m->binormals = 0;
            }
            m->uploaded = 1;
            Engine::vboSize += m->vboByteSize;
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
                if (Engine::enableShader != 0) {
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
            for (unsigned int i = 0; i < t->meshes.count; ++i)
                MeshConvertToVBO(t->meshes.data_[i]);
            for (unsigned int i = 0; i < t->children.count; ++i)
                TransformConvertToVBO(t->children.data_[i]);
        }
        return 1;
    }
}

namespace AbyssEngine {
    int ImageFontCheckString(ImageFont * /*font*/, const unsigned short * /*str*/, unsigned int count) {
        unsigned int i = 0;
        unsigned short v;
        do {
            v = (unsigned short) i;
            i = i + 1;
        } while (v < count);
        return (int) i;
    }
}

namespace AbyssEngine {
    typedef void (*ImageCallback)(Image *, void *); // lint: void_ptr (exported texture-load callback signature)

    int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user, // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    unsigned int *outIds, float scale, AELoadedTexture *outTex,
                                    bool flag);

    int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user, // lint: void_ptr (exported TextureCreateFromFile signature)
                              // lint: void_ptr (exported TextureCreateFromFile signature)
                              unsigned int *outIds, bool flag, float scale) {
        TextureCreateFromFileIntern(engine, path, cb, user, outIds, scale,
                                    (AELoadedTexture *) 0, flag);
        return 1;
    }
}

namespace AbyssEngine {
    extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    void glTexEnvi(unsigned int t, unsigned int p, int v);
    }


    int ImageCreateFromFile(Engine *engine, const char *path, Image **out);

    void ImageRelease(Image * *slot);

    void SetFXMaterial(BlendMode /*mode*/) {
    }

    int GenerateCompressedTexture(Image * /*image*/) {
        return 1;
    }

    int TextureCreateFromFileIntern(Engine *engine, const char *path, void (*cb)(Image *, void *), // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    void *user, unsigned int *outIds, float scale, // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    AELoadedTexture *outTex, bool registerTexture) {
        Image *image = 0;
        *outIds = 0;
        engine->lastGlError = glGetError();

        int result = ImageCreateFromFile(engine, path, &image);
        if (result != 1)
            return result;
        if (cb != 0)
            cb(image, user);

        glGenTextures(1, outIds);
        if (image->format == 6) {
            if (Engine::enableShader != 0) {
                glBindTexture(0x8513, *outIds);
                glTexParameteri(0x8513, 0x2800, 0x2601);
                glTexParameteri(0x8513, 0x2801, 0x2601);
                int faceHeight = image->height / 6;
                int faceWidth = image->width;
                glTexImage2D(0x8517, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data);
                int facePixels = faceHeight * faceWidth;
                glTexImage2D(0x8516, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 4);
                glTexImage2D(0x8519, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 8);
                glTexImage2D(0x8515, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 12);
                glTexImage2D(0x851a, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 16);
                glTexImage2D(0x8518, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 20);
            }
        } else {
            glBindTexture(0xde1, *outIds);
            glPixelStorei(0xcf5, 1);
            if (Engine::enableShader == 0)
                glTexEnvi(0x2300, 0x2200, 0x2100);
            if (Engine::clampTextures) {
                glTexParameteri(0xde1, 0x2802, 0x812f);
                glTexParameteri(0xde1, 0x2803, 0x812f);
            } else {
                glTexParameteri(0xde1, 0x2802, 0x2901);
                glTexParameteri(0xde1, 0x2803, 0x2901);
            }
            if (image->hasMipmaps) {
                if (Engine::AnisotropyValue > 0.0f) {
                    glTexParameterf(0xde1, 0x84fe, Engine::AnisotropyValue);
                    glTexParameteri(0xde1, 0x2801, 0x2703);
                } else {
                    glTexParameteri(0xde1, 0x2801, 0x2703);
                    glTexParameteri(0xde1, 0x2800, 0x2601);
                }
            } else if (engine->linearFilterFlag) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            } else {
                glTexParameteri(0xde1, 0x2800, 0x2600);
                glTexParameteri(0xde1, 0x2801, 0x2600);
            }
            if (Engine::clampTextures) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            }

            switch (image->format) {
            case 1:
                glTexImage2D(0xde1, 0, 0x1909, image->width, image->height, 0, 0x1909, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 2:
                glTexImage2D(0xde1, 0, 0x1907, image->width, image->height, 0, 0x1907, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 3:
                glTexImage2D(0xde1, 0, 0x1908, image->width, image->height, 0, 0x1908, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 4: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c03, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blocks = width < 16 ? 2 : width >> 3;
                    unsigned int rows = height < 8 ? 16 : (height << 1) & ~7u;
                    unsigned int blockBytes = rows * blocks;
                    glCompressedTexImage2D(0xde1, level, 0x8c03, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 5: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c02, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blocks = height < 8 ? 2 : height >> 2;
                    unsigned int rows = width < 8 ? 16 : (width << 1) & ~7u;
                    unsigned int blockBytes = rows * blocks;
                    glCompressedTexImage2D(0xde1, level, 0x8c02, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 7: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c93, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = width * height;
                    if (blockBytes <= 16)
                        blockBytes = 16;
                    glCompressedTexImage2D(0xde1, level, 0x8c93, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 8:
            case 9:
            case 10: {
                unsigned int format = 0x83f0;
                if (image->format == 9)
                    format = 0x83f2;
                if (image->format == 10)
                    format = 0x83f3;
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, format, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = height * width;
                    if (image->format - 9 <= 1) {
                        if ((int) blockBytes <= 16)
                            blockBytes = 16;
                    } else if (blockBytes > 15) {
                        blockBytes >>= 1;
                    } else {
                        blockBytes = image->format == 8 ? 8 : 16;
                    }
                    glCompressedTexImage2D(0xde1, level, format, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 11: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8d64, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = (width * height) >> 1;
                    if (width < 8)
                        blockBytes = 8;
                    glCompressedTexImage2D(0xde1, level, 0x8d64, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            default:
                break;
            }
        }

        if (registerTexture) {
            if (outTex != 0) {
                outTex->valid = 1;
                outTex->isCube = image->format == 6;
                if (glGetError() != 0) {
                    outTex->glId = 0xffffffff;
                    outTex->byteSize = 0;
                    glDeleteTextures(1, outIds);
                } else {
                    AELabelObject(0x1702, *outIds, path);
                    outTex->glId = *outIds;
                    outTex->byteSize = image->dataLen;
                    ++Engine::ImageCount;
                }
                engine->textureByteCounter += outTex->byteSize;
            } else {
                AELoadedTexture *texture = new AELoadedTexture;
                texture->scale = scale;
                texture->valid = 1;
                texture->isCube = image->format == 6;
                texture->name.Set(path);
                if (glGetError() != 0) {
                    glDeleteTextures(1, outIds);
                    texture->byteSize = 0;
                    texture->glId = 0xffffffff;
                } else {
                    AELabelObject(0x1702, *outIds, path);
                    texture->glId = *outIds;
                    texture->byteSize = image->dataLen;
                    ++Engine::ImageCount;
                }
                engine->textureByteCounter += texture->byteSize;
                ArrayAdd(texture, engine->appManager->paintCanvas->cubeTextures);
                *outIds = engine->appManager->paintCanvas->cubeTextures.count - 1;
            }
        } else {
            engine->lastGlError = glGetError();
            if (engine->lastGlError != 0) {
                engine->lastErrorPath = String(path);
                glDeleteTextures(1, outIds);
                ImageRelease(&image);
                return -4;
            }
            AELabelObject(0x1702, *outIds, path);
        }

        ImageRelease(&image);
        return 1;
    }
}

namespace AbyssEngine {
    String operator+(const long long &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void SpriteSystemRelease(Engine *engine, SpriteSystem **slot) {
        if (*slot == 0)
            return;

        unsigned char *p = (unsigned char *) (*slot)->posCpu;
        if (p != 0)
            operator delete[](p);
        (*slot)->posCpu = 0;

        p = (unsigned char *) (*slot)->sizeCpu;
        if (p != 0)
            operator delete[](p);
        (*slot)->sizeCpu = 0;

        MeshRelease(engine, &(*slot)->mesh);

        if (*slot != 0)
            operator delete(*slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    int CurveCreate(void **src, unsigned short count, Curve **out) { // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        Curve *curve = (Curve *) operator new(8);
        curve->entries = 0;
        *out = curve;
        curve->count = count;

        unsigned char *data = (unsigned char *) operator new[]((unsigned int) count << 2);
        curve->entries = data;
        memcpy(data, src, (unsigned int) count << 2);
        return 1;
    }
}

namespace AbyssEngine {
    bool operator==(const String &a, const String &b) {
        String copy(a);
        return copy.Compare(b) == 0;
    }
}

namespace AbyssEngine {
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
                    if (Engine::enableShader != 0) {
                        glDeleteBuffers(1, &m->tangentVBO);
                        glDeleteBuffers(1, &m->binormalVBO);
                    }
                }
                if (m->vertexFormat & 8)
                    glDeleteBuffers(1, &m->colorVBO);
            }

            if (m->indices != 0) operator delete[](m->indices);
            m->indices = 0;
            if (m->positions != 0) operator delete[](m->positions);
            m->positions = 0;
            if (m->texCoords != 0) operator delete[](m->texCoords);
            m->texCoords = 0;
            if (m->colors != 0) operator delete[](m->colors);
            m->colors = 0;
            if (m->normals != 0) operator delete[](m->normals);
            m->normals = 0;

            if (Engine::enableShader != 0) {
                if (m->tangents != 0) operator delete[](m->tangents);
                m->tangents = 0;
                if (m->binormals != 0) operator delete[](m->binormals);
                m->binormals = 0;
            }
        }

        delete m->animation;
        m->animation = 0;

        operator delete(m);
        *slot = 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const float &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    int MeshConvertToVBOIntern(Mesh * mesh);
    int TransformConvertToVBO(Transform * t);

    int MeshConvertToVBO(Mesh *mesh) {
        int result = -4;
        if (mesh != 0 && Engine::vboSupported) {
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

        memcpy(out, local, 0x40);
    }
}

namespace AbyssEngine {
    int MeshReadData(Engine *engine, const unsigned int &handle, unsigned int flags, Mesh **slot,
                     Material *mat);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int MeshCreateFromFile(Engine *engine, const char *path, Mesh **out, Material *mat) {
        if (engine == 0 || path == 0)
            return -4;

        Mesh *m = (Mesh *) ::operator new(0x88);
        memset(m, 0, 0x88);
        m->boundsRadiusSq = 1.0f;
        *out = m;
        m->material = mat;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, &m->positionVBO) == 0) {
            if (*out != 0)
                ::operator delete(*out);
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

        // Mesh-file format magic (verified vs original signature table @0x1da3d0:
        // "AEMesh\0", "V2AEMesh\0".."V5AEMesh\0"). The 7-byte magic is matched
        // against "AEMesh\0" and the first 7 bytes of the version-tagged variants;
        // each full match keeps its fmt bit set. Bits: AEMesh=0x4, V2=0x1, V3=0x2,
        // V4=0x8, V5=0x10 (downstream: 0x1b => read version short, 0x1a => submeshes).
        static const char sigMesh[7] = {'A', 'E', 'M', 'e', 's', 'h', 0};
        static const char sigV2[7] = {'V', '2', 'A', 'E', 'M', 'e', 's'};
        static const char sigV3[7] = {'V', '3', 'A', 'E', 'M', 'e', 's'};
        static const char sigV4[7] = {'V', '4', 'A', 'E', 'M', 'e', 's'};
        static const char sigV5[7] = {'V', '5', 'A', 'E', 'M', 'e', 's'};
        unsigned int fmt = 0x1f;
        for (int i = 0; i < 7; ++i) {
            char ch = magic[i];
            if (sigMesh[i] != ch) fmt &= ~4u;
            if (sigV2[i] != ch) fmt &= ~1u;
            if (sigV3[i] != ch) fmt &= ~2u;
            if (sigV4[i] != ch) fmt &= ~8u;
            if (sigV5[i] != ch) fmt &= ~0x10u;
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
                    memset(childPtr, 0, 0x88);
                    childPtr->boundsRadiusSq = 1.0f;
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
                    {
                        EngineArrayHeader *a = (EngineArrayHeader *) &(*out)->animation->meshes;
                        a->capacity = a->count + 1;
                        a->data = (unsigned char *) realloc(a->data, (a->count + 1) * sizeof(Mesh *));
                        ((Mesh **) a->data)[a->count] = childPtr;
                        a->count = a->capacity;
                    }
                }
                ok = true;
            }
        }

        if (ok) {
            AEFile::Close(handle);
            Transform *xf = (*out)->animation;
            if (xf != 0) {
                xf->CollectAnimationData();
                long long t = (long long) (0.0f);
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
        if (!(radius != 0.0f && Engine::vfc != 0))
            return 1;

        const Matrix &view = cam->projection;
        // Point in camera space (computed but unused; a plain float[3] in the original).
        float camPoint[3] = {0.0f, 0.0f, 0.0f};
        if (extra != 0) {
            Matrix combined = view;
            combined *= *extra;
            *(Vector *) camPoint = AEMath::MatrixInverseTransformVector(combined, center);
        } else {
            *(Vector *) camPoint = AEMath::MatrixInverseTransformVector(view, center);
        }

        Vector toCenter = center - AEMath::MatrixGetPosition(view);
        float depth = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(-AEMath::MatrixGetDir(view)));
        if (depth > cam->position[2] + radius)
            return 0;
        if (depth < cam->position[1] - radius)
            return 0;

        float horizontal = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(AEMath::MatrixGetRight(view)));
        float hLimit = depth * cam->frustumTanHalfFov * cam->frustumAspect;
        float hPad = cam->frustumInvCosX * radius;
        if (horizontal > hLimit + hPad)
            return 0;
        if (horizontal < -hLimit - hPad)
            return 0;

        float vertical = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(AEMath::MatrixGetUp(view)));
        float vLimit = depth * cam->frustumTanHalfFov;
        float vPad = cam->frustumInvCosY * radius;
        if (vertical > vLimit + vPad || vertical < -vLimit - vPad)
            return 0;
        return 1;
    }
}

namespace AbyssEngine {
    struct CurveRec {
        Curve *curve;
    };

    int CurveGetValue(unsigned long long time, Curve *curve) {
        CurveKeyframe **keys = (CurveKeyframe **) curve->entries;
        if (keys[0]->time >= time)
            return keys[0]->value;
        int last = curve->count - 1;
        if (time >= keys[last]->time)
            return keys[last]->value;

        unsigned short index;
        CurveKeyframe *key;
        do {
            index = (unsigned short) last;
            key = keys[index];
            last--;
        } while (time < key->time);

        if (key->tag == 3) {
            CurveKeyframe *next = keys[index + 1];
            int endValue = next->value;
            long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
            long long t2 = (t * t) >> 12;
            long long t3 = (t2 * t) >> 12;
            int startValue = key->value;
            return (int) (((t - 2 * t2 + t3) * key->c0) >> 12) + (int) (((t3 - t2) * key->c1) >> 12) +
                   (int) (((3 * t2 - 2 * t3) * (endValue - startValue)) >> 12) + startValue;
        }
        if (key->tag == 2) {
            CurveKeyframe *next = keys[index + 1];
            long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
            return (int) ((t * (next->value - key->value)) >> 12) + key->value;
        }
        if (key->tag == 1)
            return key->value;
        return 0;
    }
}

namespace AbyssEngine {
    unsigned short *computeFloatString(float value, int precision, int *decimalPoint, int *negative, int fixed) {
        unsigned short *buf = new unsigned short[33];
        if (precision < 0)
            precision = 0;
        if (precision >= 30)
            precision = 30;
        *negative = 0;
        if (value < 0.0f) {
            *negative = 1;
            value = -value;
        }
        float integral;
        int exponent = 0;
        unsigned short *out;

        float fraction = MODF(value, &integral);
        if (integral != 0.0f) {
            unsigned short *end = buf + 32;
            unsigned short *p = buf + 31;
            while (integral != 0.0f) {
                float digit = MODF(integral / 10.0f, &integral);
                exponent++;
                *p-- = (unsigned short) ((int) ((digit + 0.03) * 10.0) + '0');
            }
            unsigned short *src = p + 1;
            out = buf;
            while (src < end)
                *out++ = *src++;
        } else {
            if (fraction > 0.0f) {
                while (fraction * 10.0f < 1.0f) {
                    exponent--;
                    fraction *= 10.0f;
                }
            }
            out = buf;
        }
        *decimalPoint = exponent;

        unsigned short *limit = buf + precision;
        if (fixed == 0)
            limit += exponent;
        if (limit < buf) {
            *buf = 0;
            return buf;
        }
        while (out <= limit && out < buf + 32) {
            float digit;
            fraction = MODF(fraction * 10.0f, &digit);
            *out++ = (unsigned short) ((int) digit + '0');
        }
        if (limit >= buf + 32) {
            buf[31] = 0;
            return buf;
        }

        unsigned short *p = limit;
        unsigned short v = (unsigned short) (*limit + 5);
        *limit = v;
        while (v > '9') {
            *p = '0';
            if (p > buf) {
                p--;
                v = (unsigned short) (*p + 1);
                *p = v;
            } else {
                *p = '1';
                exponent++;
                *decimalPoint = exponent;
                if (fixed == 0) {
                    if (limit > buf)
                        *limit = '0';
                    limit++;
                }
                v = *p;
            }
        }
        *limit = 0;
        return buf;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void TransformRelease(Engine *engine, Transform **slot) {
        Transform *t = (Transform *) *slot;
        if (t == 0)
            return;

        for (unsigned int i = 0; i < t->children.count; ++i) {
            TransformRelease(engine, &t->children.data_[i]);
            t = (Transform *) *slot;
        }

        for (unsigned int i = 0; i < t->meshes.count; ++i) {
            MeshRelease(engine, &t->meshes.data_[i]);
            t = (Transform *) *slot;
        }
    }
}

int GetStringLength(AbyssEngine::String str) {
    const unsigned short *p = str.GetAEWChar();
    while (*p++) {
    }
    return static_cast<int>(p - str.GetAEWChar()) - 1;
}
