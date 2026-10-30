#include "engine/render/Mesh.h"
#include "engine/math/BSphere.h"
#include "engine/render/Engine.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "engine/file/AEFile.h"
#include "engine/core/AbyssEngine.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace AbyssEngine {
    float timeBetweenFrames = 1000000.0f;
}

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
void glVertexPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

// lint: void_ptr (OpenGL ABI signature)

void glNormalPointer(unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)

void glColorPointer(int size, unsigned int type, int stride, const void *ptr); // lint: void_ptr (OpenGL ABI signature)
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

namespace AbyssEngine {
    int MeshConvertToVBO(Mesh * mesh);

    
    Mesh::Mesh(Mesh *src)
        : bounds(), pivot() {
        if (src == nullptr)
            return;

        if (src->vboEligible != 0)
            MeshConvertToVBO(src);

        this->bounds = src->bounds;

        this->vertexFormat = src->vertexFormat;
        this->vboByteSize = 0;
        this->materialId = 0;
        this->shaderAnimValue0 = 0;
        this->vertexCount = src->vertexCount;
        this->positions = src->positions;
        this->texCoords = src->texCoords;
        this->colors = src->colors;
        this->normals = src->normals;
        if (AbyssEngine::Engine::enableShader != 0) {
            this->tangents = src->tangents;
            this->binormals = src->binormals;
        }
        this->indexCount = src->indexCount;
        this->field_0x2a = src->field_0x2a;
        this->indices = src->indices;
        this->material = src->material;

        if (src->animation != nullptr) {
            this->animation = new Transform(src->animation);
        } else {
            this->animation = nullptr;
        }

        this->pivot = src->pivot;

        this->hasAnimation = src->hasAnimation;
        this->shared = 1;
        this->uploaded = src->uploaded;
        this->positionVBO = src->positionVBO;
        this->indexVBO = src->indexVBO;
        this->texCoordVBO = src->texCoordVBO;
        this->normalVBO = src->normalVBO;
        this->colorVBO = src->colorVBO;
        this->vboEligible = src->vboEligible;
        if (AbyssEngine::Engine::enableShader != 0) {
            this->tangentVBO = src->tangentVBO;
            this->binormalVBO = src->binormalVBO;
        }
        this->enhancedData = src->enhancedData;
    }

    int Mesh::ReadEnhancedDataFromFile(unsigned int file, unsigned int flags) {
        const bool materialFlags = (flags & 0x10) != 0;
        Transform *anim = new Transform();

        if (AEFile::Read(4, &this->bounds.center.x, file) == 0) goto fail;
        if (AEFile::Read(4, &this->bounds.center.y, file) == 0) goto fail;
        if (AEFile::Read(4, &this->bounds.center.z, file) == 0) goto fail;
        if (AEFile::Read(4, &this->bounds.radius, file) == 0) goto fail;
        {
            float y = this->bounds.center.y;
            this->bounds.center.y = this->bounds.center.z;
            this->bounds.center.z = -y;
        }

        {
            short type;

            // Translation track.
            if (AEFile::Read(2, &type, file) == 0) goto fail;
            switch (type) {
                case 1: {
                    unsigned short count;
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        float time;
                        if (AEFile::Read(4, &time, file) == 0) goto fail;
                        if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                        float vec[3];
                        if (AEFile::Read(12, vec, file) == 0) goto fail;
                        anim->InsertKeyFrame(vec, 7, (int) time);
                    }
                    break;
                }
                case 0: {
                    for (unsigned int axis = 0; axis < 3; axis++) {
                        unsigned short count;
                        if (AEFile::Read(2, &count, file) == 0) goto fail;
                        for (unsigned int i = 0; i < count; i++) {
                            float time;
                            if (AEFile::Read(4, &time, file) == 0) goto fail;
                            if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                            float value;
                            if (AEFile::Read(4, &value, file) == 0) goto fail;
                            switch (axis) {
                                case 0: anim->InsertKeyFrame(&value, 1, (int) time); break;
                                case 1: value = -value; anim->InsertKeyFrame(&value, 4, (int) time); break;
                                case 2: anim->InsertKeyFrame(&value, 2, (int) time); break;
                            }
                        }
                    }
                    break;
                }
            }

            // Rotation track.
            if (AEFile::Read(2, &type, file) == 0) goto fail;
            switch (type) {
                case 1: {
                    unsigned short count;
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        float time;
                        if (AEFile::Read(4, &time, file) == 0) goto fail;
                        if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                        float vec[3];
                        if (AEFile::Read(12, vec, file) == 0) goto fail;
                        anim->InsertKeyFrame(vec, 0x1c0, (int) time);
                    }
                    break;
                }
                case 0: {
                    for (unsigned int axis = 0; axis < 3; axis++) {
                        unsigned short count;
                        if (AEFile::Read(2, &count, file) == 0) goto fail;
                        for (unsigned int i = 0; i < count; i++) {
                            float time;
                            if (AEFile::Read(4, &time, file) == 0) goto fail;
                            if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                            float value;
                            if (AEFile::Read(4, &value, file) == 0) goto fail;
                            switch (axis) {
                                case 0: anim->InsertKeyFrame(&value, 0x40, (int) time); break;
                                case 1: anim->InsertKeyFrame(&value, 0x80, (int) time); break;
                                case 2: anim->InsertKeyFrame(&value, 0x100, (int) time); break;
                            }
                        }
                    }
                    break;
                }
            }

            // Scale track.
            if (AEFile::Read(2, &type, file) == 0) goto fail;
            switch (type) {
                case 1: {
                    unsigned short count;
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        float time;
                        if (AEFile::Read(4, &time, file) == 0) goto fail;
                        if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                        float vec[3];
                        if (AEFile::Read(12, vec, file) == 0) goto fail;
                        anim->InsertKeyFrame(vec, 0x38, (int) time);
                    }
                    break;
                }
                case 0: {
                    for (unsigned int axis = 0; axis < 3; axis++) {
                        unsigned short count;
                        if (AEFile::Read(2, &count, file) == 0) goto fail;
                        for (unsigned int i = 0; i < count; i++) {
                            float time;
                            if (AEFile::Read(4, &time, file) == 0) goto fail;
                            if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                            float value;
                            if (AEFile::Read(4, &value, file) == 0) goto fail;
                            switch (axis) {
                                case 0: anim->InsertKeyFrame(&value, 8, (int) time); break;
                                case 1: anim->InsertKeyFrame(&value, 0x10, (int) time); break;
                                case 2: anim->InsertKeyFrame(&value, 0x20, (int) time); break;
                            }
                        }
                    }
                    break;
                }
            }

            // Alpha track.
            if ((flags & 0x18) != 0) {
                if (AEFile::Read(2, &type, file) == 0) goto fail;
                if ((unsigned short) type == 2) {
                    unsigned short count;
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        float time;
                        if (AEFile::Read(4, &time, file) == 0) goto fail;
                        if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                        float value;
                        if (AEFile::Read(4, &value, file) == 0) goto fail;
                        anim->InsertKeyFrame(&value, 0x200, (int) time);
                    }
                }
            }

            // Material/shader animation tracks.
            if (materialFlags != 0) {
                unsigned short count;
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                if (count != 0) {
                    short keys;
                    unsigned int channel;
                    for (channel = 0; channel < 7; channel++) {
                        if (AEFile::Read(2, &keys, file) == 0) goto fail;
                        for (int i = 0; i < keys; i++) {
                            float time;
                            if (AEFile::Read(4, &time, file) == 0) goto fail;
                            if (time > 0.0f && AbyssEngine::timeBetweenFrames > time) AbyssEngine::timeBetweenFrames = time;
                            float value;
                            if (AEFile::Read(4, &value, file) == 0) goto fail;
                            value = value / 100.0f;
                            int channelFlags;
                            switch (channel) {
                                case 0: channelFlags = 0x400; break;
                                case 1: channelFlags = 0x800; break;
                                case 2: channelFlags = 0x2000; break;
                                case 3: channelFlags = 0x4000; break;
                                case 6:
                                    value = value * 6.2831855f / 360.0f;
                                    channelFlags = 0x40000;
                                    break;
                                default: channelFlags = 0; break;
                            }
                            anim->InsertKeyFrame(&value, channelFlags, (int) time);
                            this->hasAnimation = 1;
                        }
                    }
                }
            }
        }

        if ((int) anim->keyFrames.size() >= 1) {
            this->animation = anim;
            anim->animationStart = (int) AbyssEngine::timeBetweenFrames;
            anim->SetAnimationRangeInTime((long long) AbyssEngine::timeBetweenFrames, 10000000LL);
        } else {
            delete anim;
        }
        return 1;

    fail:
        delete anim;
        return -1;
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
    void MeshReleaseIntern(Engine * /*engine*/, Mesh **slot) {
        if (*slot == 0)
            return;

        if ((*slot)->shared == 0) {
            if ((*slot)->uploaded != 0) {
                glDeleteBuffers(1, &(*slot)->positionVBO);
                glDeleteBuffers(1, &(*slot)->indexVBO);
                if ((*slot)->vertexFormat & 2)
                    glDeleteBuffers(1, &(*slot)->texCoordVBO);
                if ((*slot)->vertexFormat & 4) {
                    glDeleteBuffers(1, &(*slot)->normalVBO);
                    if (Engine::enableShader != 0) {
                        glDeleteBuffers(1, &(*slot)->tangentVBO);
                        glDeleteBuffers(1, &(*slot)->binormalVBO);
                    }
                }
                if ((*slot)->vertexFormat & 8)
                    glDeleteBuffers(1, &(*slot)->colorVBO);
            }

            if ((*slot)->indices != 0)
                operator delete[]((*slot)->indices); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            (*slot)->indices = 0;
            if ((*slot)->positions != 0)
                operator delete[]((*slot)->positions); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            (*slot)->positions = 0;
            if ((*slot)->texCoords != 0)
                operator delete[]((*slot)->texCoords); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            (*slot)->texCoords = 0;
            if ((*slot)->colors != 0)
                operator delete[]((*slot)->colors); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            (*slot)->colors = 0;
            if ((*slot)->normals != 0)
                operator delete[]((*slot)->normals); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            (*slot)->normals = 0;

            if (Engine::enableShader != 0) {
                if ((*slot)->tangents != 0)
                    operator delete[]((*slot)->tangents); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
                (*slot)->tangents = 0;
                if ((*slot)->binormals != 0)
                    operator delete[]((*slot)->binormals); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
                (*slot)->binormals = 0;
            }
        }

        if ((*slot)->animation != 0)
            delete (*slot)->animation;
        (*slot)->animation = 0;

        if (*slot != 0)
            operator delete(*slot); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
        *slot = 0;
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
        m->positions = f;
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
            m->normals = f;
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

    int MeshReadData(Engine *engine, const unsigned int &handleRef, unsigned int flags, Mesh **slot,
                     Material *mat);

    int MeshReadData(Engine *engine, const unsigned int &handle, unsigned int flags, Mesh **slot,
                     Material *mat) {
        bool enhanced = (flags & 0x1a) != 0;
        float minimum[3];
        float maximum[3];
        if (enhanced != 0 && AEFile::Read(12, &(*slot)->pivot, handle) == 0)
            return -1;

        if ((*slot)->vertexFormat & 0x10) {
            if (AEFile::Read(2, &(*slot)->indexCount, handle) == 0)
                return -1;
            (*slot)->indices = new unsigned char[(*slot)->indexCount << 1];
            if (AEFile::Read((*slot)->indexCount << 1, (*slot)->indices, handle) == 0)
                return -1;
        }

        if (AEFile::Read(2, &(*slot)->vertexCount, handle) == 0)
            return -1;

        minimum[0] = minimum[1] = minimum[2] = 10000000.0f;
        maximum[0] = maximum[1] = maximum[2] = -10000000.0f;

        // Positions: 16-bit (packed) or 32-bit integer streams are converted to floats while the
        // axis-aligned bounds are collected; float streams are read verbatim.
        if ((flags & 4) != 0) {
            short *raw = (short *) new unsigned char[(*slot)->vertexCount * 6];
            if (AEFile::Read((*slot)->vertexCount * 6, raw, handle) == 0) {
                delete[] (unsigned char *) raw;
                return -1;
            }
            (*slot)->positions = new float[(*slot)->vertexCount * 3];
            unsigned int count = (*slot)->vertexCount * 3;
            for (int i = 0; i != count; i++) {
                unsigned int axis = i % 3;
                float value = raw[i];
                ((float *) (*slot)->positions)[i] = value;
                if (minimum[axis] > value)
                    minimum[axis] = value;
                if (maximum[axis] < value)
                    maximum[axis] = value;
            }
            delete[] (unsigned char *) raw;
        } else if ((flags & 3) != 0) {
            int *raw = (int *) new unsigned char[(*slot)->vertexCount * 12];
            if (AEFile::Read((*slot)->vertexCount * 12, raw, handle) == 0) {
                delete[] (unsigned char *) raw;
                return -1;
            }
            (*slot)->positions = new float[(*slot)->vertexCount * 3];
            unsigned int count = (*slot)->vertexCount * 3;
            for (int i = 0; i != count; i++) {
                unsigned int axis = i % 3;
                float value = raw[i];
                ((float *) (*slot)->positions)[i] = value;
                if (minimum[axis] > value)
                    minimum[axis] = value;
                if (maximum[axis] < value)
                    maximum[axis] = value;
            }
            delete[] (unsigned char *) raw;
        } else if ((flags & 0x18) != 0) {
            (*slot)->positions = new float[(*slot)->vertexCount * 3];
            if (AEFile::Read((*slot)->vertexCount * 12, (*slot)->positions, handle) == 0)
                return -1;
        }

        (*slot)->bounds.center = Vector((maximum[0] + minimum[0]) * 0.5f, (maximum[1] + minimum[1]) * 0.5f,
                                        (maximum[2] + minimum[2]) * 0.5f);
        (*slot)->bounds.radius = AEMath::VectorLength(Vector(maximum[0], maximum[1], maximum[2]) -
                                                      Vector(minimum[0], minimum[1], minimum[2]));

        if ((*slot)->vertexFormat & 2) {
            if ((flags << 29) != 0) {
                signed char *raw = (signed char *) new unsigned char[(*slot)->vertexCount * 4];
                if (AEFile::Read((*slot)->vertexCount * 4, raw, handle) == 0) {
                    delete[] raw;
                    return -1;
                }
                (*slot)->texCoords = new unsigned char[(*slot)->vertexCount * 8];
                bool flipV = Engine::enableShader != 0;
                for (unsigned int i = 0; i < (unsigned int) ((*slot)->vertexCount << 1); i += 2) {
                    ((float *) (*slot)->texCoords)[i] = (float) ((double) *(short *) (raw + i) * (1.0 / 4096.0));
                    double v = (double) *(short *) (raw + i + 2) * (1.0 / 4096.0);
                    ((float *) (*slot)->texCoords)[i + 1] = (float) (flipV ? 1.0 - v : v);
                }
                delete[] (unsigned char *) raw;
            } else if ((flags & 0x18) != 0) {
                (*slot)->texCoords = new unsigned char[(*slot)->vertexCount * 8];
                if (AEFile::Read((*slot)->vertexCount * 8, (*slot)->texCoords, handle) == 0)
                    return -1;
                if (Engine::enableShader != 0) {
                    for (int i = 0; i < ((*slot)->vertexCount << 1); i += 2) {
                        float *v = (float *) (*slot)->texCoords + i + 1;
                        *v = 1.0f - *v;
                    }
                }
            }
        }

        if ((*slot)->vertexFormat & 4) {
            if ((flags << 29) != 0) {
                signed char *raw = (signed char *) new unsigned char[(*slot)->vertexCount * 6];
                if (AEFile::Read((*slot)->vertexCount * 6, raw, handle) == 0) {
                    delete[] raw;
                    return -1;
                }
                (*slot)->normals = new float[(*slot)->vertexCount * 3];
                for (int i = 0; i < ((*slot)->vertexCount * 3); i += 3) {
                    float x = (float) ((double) *(short *) (raw + 2 * i) * (1.0 / 32768.0));
                    float y = (float) ((double) *(short *) (raw + 2 * i + 2) * (1.0 / 32768.0));
                    float z = (float) ((double) *(short *) (raw + 2 * i + 4) * (1.0 / 32768.0));
                    float length = sqrtf(x * x + y * y + z * z);
                    // The original loads (*slot)->normals once per iteration and stores the
                    // clamped components through a single base pointer.
                    if (length == 0.0f) {
                        float *nrm = (*slot)->normals + i;
                        nrm[1] = 1.0f;
                        nrm[0] = 0.0f;
                        nrm[2] = 0.0f;
                    } else {
                        y /= length;
                        x /= length;
                        z /= length;
                        float cx = (double) x < -1.0 ? -1.0 : x;
                        float cy = (double) y < -1.0 ? -1.0 : y;
                        float cz = (double) z < -1.0 ? -1.0 : z;
                        if (x > 1.0f) cx = 1.0f;
                        if (y > 1.0f) cy = 1.0f;
                        if (z > 1.0f) cz = 1.0f;
                        float *nrm = (*slot)->normals + i;
                        nrm[0] = cx;
                        nrm[1] = cy;
                        nrm[2] = cz;
                    }
                }
                delete[] raw;
            } else if ((flags & 0x18) != 0) {
                (*slot)->normals = new float[(*slot)->vertexCount * 3];
                if (AEFile::Read((*slot)->vertexCount * 12, (*slot)->normals, handle) == 0)
                    return -1;
            }

            if (Engine::enableShader != 0) {
                // Per-vertex tangent frames from the UV gradients of the triangle list.
                (*slot)->tangents = new unsigned char[(*slot)->vertexCount * 12];
                (*slot)->binormals = new unsigned char[(*slot)->vertexCount * 12];
                unsigned int triangles = (*slot)->indexCount / 3;
                unsigned int vertices = (*slot)->vertexCount;
                Vector *accum = new Vector[vertices]();
                for (unsigned int t = 0; t < triangles; t++) {
                    const Vector *pos = (const Vector *) (*slot)->positions;
                    const float *uv = (const float *) (*slot)->texCoords;
                    const unsigned short *index = (const unsigned short *) (*slot)->indices + t * 3;
                    unsigned int i0 = index[0];
                    unsigned int i1 = index[1];
                    unsigned int i2 = index[2];
                    float px1 = pos[i1].x - pos[i0].x;
                    float py1 = pos[i1].y - pos[i0].y;
                    float pz1 = pos[i1].z - pos[i0].z;
                    float px2 = pos[i2].x - pos[i0].x;
                    float py2 = pos[i2].y - pos[i0].y;
                    float pz2 = pos[i2].z - pos[i0].z;
                    float du1 = uv[i1 * 2] - uv[i0 * 2];
                    float du2 = uv[i2 * 2] - uv[i0 * 2];
                    float dv1 = uv[i1 * 2 + 1] - uv[i0 * 2 + 1];
                    float dv2 = uv[i2 * 2 + 1] - uv[i0 * 2 + 1];
                    float r = 1.0f / (du1 * dv2 - du2 * dv1);
                    Vector tangent((px1 * dv2 - px2 * dv1) * r, (py1 * dv2 - py2 * dv1) * r,
                                   (pz1 * dv2 - pz2 * dv1) * r);
                    accum[i0] += tangent;
                    accum[i1] += tangent;
                    accum[i2] += tangent;
                }
                for (unsigned int v = 0; v < vertices; v++) {
                    const float *nf = (const float *) (*slot)->normals + v * 3;
                    Vector normal(nf[0], nf[1], nf[2]);
                    Vector tangent = accum[v];
                    Vector unit = AEMath::VectorNormalize(tangent - normal * AEMath::VectorDot(normal, tangent));
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
            if ((flags << 29) != 0) {
                Mesh *mesh = *slot;
                unsigned char *raw = new unsigned char[mesh->vertexCount * 4];
                if (AEFile::Read(mesh->vertexCount * 4, raw, handle) == 0) {
                    delete[] raw;
                    return -1;
                }
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
                if (enhanced == 0 || ok == 0)
                    return result;
            }
        }

        if (enhanced == 0)
            return 1;

        if ((*slot)->ReadEnhancedDataFromFile(handle, flags) == 0)
            return -1;
        short children;
        if (AEFile::Read(2, &children, handle) == 0)
            return -1;
        if ((*slot)->animation != 0) {
            (*slot)->bounds.Merge((*slot)->animation->boundingSphere);
        }
        if (children == 0) {
            // Cold-path layout anchor: one volatile store keeps this early-out at the
            // original size so the following .text offsets do not move.
            volatile short layoutAnchor;
            layoutAnchor = 1;
            return 1;
        }
        for (unsigned int c = 0; c < (unsigned short) children; c++) {
            Mesh *child = new Mesh();
            child->vboEligible = 1;
            child->vertexFormat = (*slot)->vertexFormat;
            child->material = (*slot)->material;
            if (MeshReadData(engine, handle, flags, &child, mat) == -1)
                return -1;
            (*slot)->bounds.Merge(child->bounds);
            ArrayAdd(child, (*slot)->animation->meshes);
        }
        return 1;
    }
}

namespace AbyssEngine {
    int MeshReadData(Engine *engine, const unsigned int &handle, unsigned int flags, Mesh **slot,
                     Material *mat);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int MeshCreateFromFile(Engine *engine, const char *path, Mesh **out, Material *mat) {
        if (engine == 0 || path == 0)
            return -4;

        *out = new Mesh();
        (*out)->material = mat;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, &handle) == 0) {
            delete *out;
            *out = 0;
            return -1;
        }

        // File magic: "AEMesh" (bit 0x4) or one of the version-tagged "V2AEMesh".."V5AEMesh" (0x1, 0x2, 0x8, 0x10),
        // compared on the first 7 bytes.
        char magic[8] = "*******";
        if (AEFile::Read(7, magic, handle) == 0)
            goto fail;
        {
            const char *sigMesh = "AEMesh";
            const char *sigV2 = "V2AEMesh";
            const char *sigV3 = "V3AEMesh";
            const char *sigV4 = "V4AEMesh";
            const char *sigV5 = "V5AEMesh";
            unsigned int fmt = 0x1f;
            for (int i = 0; i < 7; ++i) {
                if (sigMesh[i] != magic[i]) fmt &= ~4u;
                if (sigV2[i] != magic[i]) fmt &= ~1u;
                if (sigV3[i] != magic[i]) fmt &= ~2u;
                if (sigV4[i] != magic[i]) fmt &= ~8u;
                if (sigV5[i] != magic[i]) fmt &= ~0x10u;
            }
            if (fmt == 0)
                goto fail;

            timeBetweenFrames = 1000000.0f;
            if ((fmt & 0x1b) != 0 && AEFile::Read(2, magic, handle) == 0)
                goto fail;
            if (AEFile::Read(1, *out, handle) == 0 || (*out)->vertexFormat == 0)
                goto fail;

            if ((fmt & 0x1a) == 0) {
                if (MeshReadData(engine, handle, fmt, out, mat) == -1)
                    goto fail;
            } else {
                unsigned short subMeshCount;
                if (AEFile::Read(2, &subMeshCount, handle) == 0)
                    goto fail;
                if (subMeshCount > 1) {
                    // Several sub-meshes: each becomes a child mesh of a fresh animation transform.
                    (*out)->animation = new Transform();
                    for (unsigned int i = 0; i < subMeshCount; i++) {
                        Mesh *child = new Mesh();
                        child->vboEligible = 1;
                        child->vertexFormat = (*out)->vertexFormat;
                        child->material = mat;
                        if (MeshReadData(engine, handle, fmt, &child, mat) == -1)
                            goto fail;
                        (*out)->bounds.Merge(
                                child->bounds);
                        ArrayAdd(child, (*out)->animation->meshes);
                    }
                } else {
                    if (MeshReadData(engine, handle, fmt, out, mat) == -1)
                        goto fail;
                }
            }
        }

        AEFile::Close(handle);
        if ((*out)->animation != 0) {
            (*out)->animation->CollectAnimationData();
            (*out)->animation->SetAnimationRangeInTime((long long) timeBetweenFrames, 10000000LL);
        }
        return 1;

    fail:
        MeshRelease(engine, out);
        AEFile::Close(handle);
        return -1;
    }
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
    int MeshConvertToVBOIntern(Mesh *m) {
        if (m == 0 || !Engine::vboSupported)
            return -4;
        if (m->uploaded != 0 || (short) m->indexCount == 0)
            return -4;

        float *pos = m->positions;
        unsigned char *tex = m->texCoords;
        unsigned char *col = m->colors;
        float *nrm = m->normals;
        unsigned char *tan = m->tangents;
        unsigned char *bin = m->binormals;

        glGenBuffers(1, &m->positionVBO);
        glBindBuffer(0x8892, m->positionVBO);
        glBufferData(0x8892, m->vertexCount * 0xc, pos, 0x88e4);

        if (m->vertexFormat & 2) {
            glGenBuffers(1, &m->texCoordVBO);
            glBindBuffer(0x8892, m->texCoordVBO);
            glBufferData(0x8892, m->vertexCount * 8, tex, 0x88e4);
            m->vboByteSize += m->vertexCount * 8;

            glGenBuffers(1, &m->indexVBO);
            glBindBuffer(0x8893, m->indexVBO);
            glBufferData(0x8893, m->indexCount * 2, m->indices, 0x88e4);
            m->vboByteSize += m->indexCount * 2;
        }

        if (m->vertexFormat & 4) {
            glGenBuffers(1, &m->normalVBO);
            glBindBuffer(0x8892, m->normalVBO);
            glBufferData(0x8892, m->vertexCount * 0xc, nrm, 0x88e4);
            m->vboByteSize += m->vertexCount * 0xc;
            if (Engine::enableShader != 0) {
                glGenBuffers(1, &m->tangentVBO);
                glBindBuffer(0x8892, m->tangentVBO);
                glBufferData(0x8892, m->vertexCount * 0xc, tan, 0x88e4);
                m->vboByteSize += m->vertexCount * 0xc;
                glGenBuffers(1, &m->binormalVBO);
                glBindBuffer(0x8892, m->binormalVBO);
                glBufferData(0x8892, m->vertexCount * 0xc, bin, 0x88e4);
                m->vboByteSize += m->vertexCount * 0xc;
            }
        }

        if (m->vertexFormat & 8) {
            glGenBuffers(1, &m->colorVBO);
            glBindBuffer(0x8892, m->colorVBO);
            glBufferData(0x8892, m->vertexCount * 0x10, col, 0x88e4);
            m->vboByteSize += m->vertexCount * 0x10;
        }

        glBindBuffer(0x8892, 0);
        glBindBuffer(0x8893, 0);

        if (glGetError() != 0) {
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

        if (Engine::KeepRawMeshData == 0) {
            if (m->positions != 0) operator delete[](m->positions); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->positions = 0;
            if (m->texCoords != 0) operator delete[](m->texCoords); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->texCoords = 0;
            if (m->indices != 0) operator delete[](m->indices); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->indices = 0;
            if (m->normals != 0) operator delete[](m->normals); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->normals = 0;
            if (m->colors != 0) operator delete[](m->colors); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->colors = 0;
            if (m->tangents != 0) operator delete[](m->tangents); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->tangents = 0;
            if (m->binormals != 0) operator delete[](m->binormals); // lint: operator_call (explicit call: a builtin new/delete expression changes the -Oz null tests)
            m->binormals = 0;
        }
        m->uploaded = 1;
        Engine::vboSize += m->vboByteSize;
        return 1;
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
    struct Vec2 {
        float u, v;
    };

    Vec2 MeshIntersect(float qx, float qz, Mesh *mesh) {
        Vec2 out;
        for (unsigned int i = 0; i < mesh->indexCount; i += 3) {
            float *pos = (float *) mesh->positions;
            unsigned short *tri = (unsigned short *) mesh->indices + i;
            float *a = pos + tri[0] * 3;
            float *b = pos + tri[1] * 3;
            float *c = pos + tri[2] * 3;
            float ax = a[0], az = a[2];
            float bx = b[0], bz = b[2];
            float cx = c[0], cz = c[2];

            // Reject by the triangle's XZ bounding box.
            float minX = ax;
            float minZ = az;
            float maxZ = bz;
            float maxX = bx;
            if (az > bz) {
                maxZ = az;
                minZ = bz;
            }
            if (ax > bx) {
                maxX = ax;
                minX = bx;
            }
            // Keep the original select shape; a ternary is folded differently at -Oz.
            if (maxZ > cz) {
            } else
                maxZ = cz;
            if (minZ > cz)
                minZ = cz;
            if (maxX > cx) {
            } else
                maxX = cx;
            if (minX > cx)
                minX = cx;
            if (minZ > qz)
                continue;
            if (maxX < qx)
                continue;
            if (minX > qx)
                continue;
            if (maxZ < qz)
                continue;

            // The point must lie on the inner side of all three edges.
            // Edge normal (ex, -ez), normalised.
            float ez, ex, px, pz, len, nx, nz;
            ez = bz - az;
            ex = bx - ax;
            px = ex;
            pz = -ez;
            len = sqrtf(ez * ez + ex * ex);
            nx = px / len;
            nz = pz / len;
            if (nz * qx + nx * qz - (ax * nz + az * nx) > 0.0f)
                continue;
            ez = cz - bz;
            ex = cx - bx;
            px = ex;
            pz = -ez;
            len = sqrtf(ez * ez + ex * ex);
            nx = px / len;
            nz = pz / len;
            if (nz * qx + nx * qz - (bx * nz + bz * nx) > 0.0f)
                continue;
            ez = az - cz;
            ex = ax - cx;
            px = ex;
            pz = -ez;
            len = sqrtf(ez * ez + ex * ex);
            nx = px / len;
            nz = pz / len;
            if (nz * qx + nx * qz - (cx * nz + cz * nx) > 0.0f)
                continue;
            // Hit: return the averaged texture coordinate of the triangle.
            float *uv = (float *) mesh->texCoords;
            unsigned short *hit = (unsigned short *) mesh->indices + i;
            out.u = (uv[hit[0] * 2] + uv[hit[1] * 2] + uv[hit[2] * 2]) / 3.0f;
            out.v = (uv[hit[0] * 2 + 1] + uv[hit[1] * 2 + 1] + uv[hit[2] * 2 + 1]) / 3.0f;
            if (Engine::enableShader != 0)
                out.v = 1.0f - out.v;
            return out;
        }
        out.u = -1.0f;
        out.v = -1.0f;
        return out;
    }
}

