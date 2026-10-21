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

    int currentUsedShaderIndex = -1;

    int currentFps;
    float debugTouch[4];
    int fpsCounter;
    int SwapCounter;
    unsigned char firstRender = 1;
    int loadTexture;
    float currentLODBias;
    int CubeMapSetIndex;
    int debugModusIndex;
    unsigned char firstRenderGlow = 1;
    unsigned char performanceTest;
    long long performanceTime;
    float timeBetweenFrames = 1000000.0f;
    unsigned char BiasErrorOutputFlag = 1;
    unsigned char performanceTestEnded;
    int orientationChangeTimer;
    long long performanceFrameCounter;
    long long performanceModulSwitches;
    long long performanceOverallModulSwitches;
    int lauf;
    float i_fov = -1.0f;
    float i_zFar;
    float i_zNear;
    int fpsTimer;
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
    Quaternion operator-(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
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
    void CurveRelease(Curve **slot) {
        Curve *curve = (Curve *) *slot;
        if (curve == 0)
            return;

        CurveKeyframe **data = 0;
        for (unsigned int i = 0; (data = (CurveKeyframe **) curve->entries),
                                 i < (unsigned int) curve->count; ++i) {
            CurveKeyframe *entry = data[i];
            int8_t tag = (int8_t) entry->tag;
            switch (tag) {
                case 3:
                    operator delete(entry);
                    break;
                case 2:
                    operator delete(entry);
                    break;
                case 1:
                    operator delete(entry);
                    break;
            }
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
    int MeshCreate(Engine * /*engine*/, unsigned short vertexCount, unsigned short triCount,
                   signed char vertexFormat, Mesh **out) {
        if (vertexCount < 4 || triCount == 0 || (vertexFormat & 1) == 0)
            return -4;

        Mesh *m = new Mesh();
        *out = m;
        m->vertexFormat = vertexFormat;
        m->vertexCount = vertexCount;
        m->indexCount = triCount * 3;
        m->field_0x2a = triCount;

        float *f = new float[vertexCount * 3];
        m->positions = (unsigned char *) f;
        memset(f, 0, vertexCount * 3 * sizeof(float));

        if (vertexFormat & 0x10) {
            unsigned short *idx = new unsigned short[triCount * 3];
            (*out)->indices = (unsigned char *) idx;
            memset(idx, 0, triCount * 3 * sizeof(unsigned short));
        }
        if (vertexFormat & 2) {
            f = new float[vertexCount * 2];
            m->texCoords = (unsigned char *) f;
            memset(f, 0, vertexCount * 2 * sizeof(float));
        }
        if (vertexFormat & 4) {
            f = new float[vertexCount * 3];
            m->normals = (unsigned char *) f;
            memset(f, 0, vertexCount * 3 * sizeof(float));
            if (Engine::enableShader) {
                f = new float[vertexCount * 3];
                m->tangents = (unsigned char *) f;
                memset(f, 0, vertexCount * 3 * sizeof(float));
                f = new float[vertexCount * 3];
                m->binormals = (unsigned char *) f;
                memset(f, 0, vertexCount * 3 * sizeof(float));
            }
        }
        if (vertexFormat & 8) {
            f = new float[vertexCount * 4];
            m->colors = (unsigned char *) f;
            memset(f, 0, vertexCount * 4 * sizeof(float));
        }

        return 1;
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
    String operator+(const String &a) {
        return String(a);
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
    Quaternion operator+(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
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

        char magic[8] = "*******";
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
        const char *sigMesh = "AEMesh";
        const char *sigV2 = "V2AEMesh";
        const char *sigV3 = "V3AEMesh";
        const char *sigV4 = "V4AEMesh";
        const char *sigV5 = "V5AEMesh";
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
