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
        : boundsCenterX(0.0f), boundsCenterY(0.0f), boundsCenterZ(0.0f), boundsRadius(0.0f),
          boundsRadiusSq(1.0f), pivotX(0.0f), pivotY(0.0f), pivotZ(0.0f) {
        if (src == nullptr)
            return;

        if (src->vboEligible != 0)
            MeshConvertToVBO(src);

        // boundsCenter..boundsRadiusSq is the original's AEMath::BSphere member, pivot a Vector.
        *reinterpret_cast<AEMath::BSphere *>(&this->boundsCenterX) =
                *reinterpret_cast<const AEMath::BSphere *>(&src->boundsCenterX);

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

        *reinterpret_cast<AEMath::Vector *>(&this->pivotX) = *reinterpret_cast<const AEMath::Vector *>(&src->pivotX);

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

    namespace {
        // The smallest positive keyframe time seen becomes the engine's frame step.
        inline void trackFrameStep(float time) {
            if (time > 0.0f && AbyssEngine::timeBetweenFrames > time)
                AbyssEngine::timeBetweenFrames = time;
        }
    }

    int Mesh::ReadEnhancedDataFromFile(unsigned int file, unsigned int flags) {
        Transform *anim = new Transform();

        if (AEFile::Read(4, &this->boundsCenterX, file) == 0) goto fail;
        if (AEFile::Read(4, &this->boundsCenterY, file) == 0) goto fail;
        if (AEFile::Read(4, &this->boundsCenterZ, file) == 0) goto fail;
        if (AEFile::Read(4, &this->boundsRadius, file) == 0) goto fail;
        {
            float y = this->boundsCenterY;
            this->boundsCenterY = this->boundsCenterZ;
            this->boundsCenterZ = -y;
        }

        {
            short type;
            unsigned short count;
            // Two scratch slots shared by every track: packed tracks read (time, xyz), per-axis
            // tracks read (time, value) into them the other way around.
            float scalar;
            float vec[3];

            // Translation track: either packed xyz keys or one track per axis (y/z swapped, z negated).
            if (AEFile::Read(2, &type, file) == 0) goto fail;
            switch (type) {
                case 1: {
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                for (unsigned int i = 0; i < count; i++) {
                    if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                    trackFrameStep(scalar);
                    if (AEFile::Read(12, vec, file) == 0) goto fail;
                    anim->InsertKeyFrame(vec, 7, (int) scalar);
                }
                    break;
                }
                case 0: {
                for (unsigned int axis = 0; axis < 3; axis++) {
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        if (AEFile::Read(4, vec, file) == 0) goto fail;
                        trackFrameStep(vec[0]);
                        if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                        switch (axis) {
                            case 0:
                                anim->InsertKeyFrame(&scalar, 1, (int) vec[0]);
                                break;
                            case 1:
                                scalar = -scalar;
                                anim->InsertKeyFrame(&scalar, 4, (int) vec[0]);
                                break;
                            case 2:
                                anim->InsertKeyFrame(&scalar, 2, (int) vec[0]);
                                break;
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
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                for (unsigned int i = 0; i < count; i++) {
                    if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                    trackFrameStep(scalar);
                    if (AEFile::Read(12, vec, file) == 0) goto fail;
                    anim->InsertKeyFrame(vec, 0x1c0, (int) scalar);
                }
                    break;
                }
                case 0: {
                for (unsigned int axis = 0; axis < 3; axis++) {
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        if (AEFile::Read(4, vec, file) == 0) goto fail;
                        trackFrameStep(vec[0]);
                        if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                        switch (axis) {
                            case 0:
                                anim->InsertKeyFrame(&scalar, 0x40, (int) vec[0]);
                                break;
                            case 1:
                                anim->InsertKeyFrame(&scalar, 0x80, (int) vec[0]);
                                break;
                            case 2:
                                anim->InsertKeyFrame(&scalar, 0x100, (int) vec[0]);
                                break;
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
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                for (unsigned int i = 0; i < count; i++) {
                    if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                    trackFrameStep(scalar);
                    if (AEFile::Read(12, vec, file) == 0) goto fail;
                    anim->InsertKeyFrame(vec, 0x38, (int) scalar);
                }
                    break;
                }
                case 0: {
                for (unsigned int axis = 0; axis < 3; axis++) {
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        if (AEFile::Read(4, vec, file) == 0) goto fail;
                        trackFrameStep(vec[0]);
                        if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                        switch (axis) {
                            case 0:
                                anim->InsertKeyFrame(&scalar, 8, (int) vec[0]);
                                break;
                            case 1:
                                anim->InsertKeyFrame(&scalar, 0x10, (int) vec[0]);
                                break;
                            case 2:
                                anim->InsertKeyFrame(&scalar, 0x20, (int) vec[0]);
                                break;
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
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (unsigned int i = 0; i < count; i++) {
                        if (AEFile::Read(4, vec, file) == 0) goto fail;
                        trackFrameStep(vec[0]);
                        if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                        anim->InsertKeyFrame(&scalar, 0x200, (int) vec[0]);
                    }
                }
            }

            // Material/shader animation tracks.
            if ((flags & 0x10) != 0) {
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                if (count != 0) {
                    for (unsigned int channel = 0; channel < 7; channel++) {
                        short keys;
                        if (AEFile::Read(2, &keys, file) == 0) goto fail;
                        for (int i = 0; i < keys; i++) {
                            if (AEFile::Read(4, vec, file) == 0) goto fail;
                            trackFrameStep(vec[0]);
                            if (AEFile::Read(4, &scalar, file) == 0) goto fail;
                            scalar = scalar / 100.0f;
                            long long channelFlags;
                            switch (channel) {
                                case 0:
                                    channelFlags = 0x400;
                                    break;
                                case 1:
                                    channelFlags = 0x800;
                                    break;
                                case 2:
                                    channelFlags = 0x2000;
                                    break;
                                case 3:
                                    channelFlags = 0x4000;
                                    break;
                                case 6:
                                    scalar = scalar * 6.2831855f / 360.0f;
                                    channelFlags = 0x40000;
                                    break;
                                default:
                                    channelFlags = 0;
                                    break;
                            }
                            anim->InsertKeyFrame(&scalar, channelFlags, (int) vec[0]);
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
            ((AEMath::BSphere *) &(*slot)->boundsCenterX)->Merge((*slot)->animation->boundingSphere);
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
                if (subMeshCount < 2) {
                    if (MeshReadData(engine, handle, fmt, out, mat) == -1)
                        goto fail;
                } else {
                    // Several sub-meshes: each becomes a child mesh of a fresh animation transform.
                    (*out)->animation = new Transform();
                    for (unsigned int i = 0; i < subMeshCount; i++) {
                        Mesh *child = new Mesh();
                        child->vboEligible = 1;
                        child->vertexFormat = (*out)->vertexFormat;
                        child->material = mat;
                        if (MeshReadData(engine, handle, fmt, &child, mat) == -1)
                            goto fail;
                        ((AEMath::BSphere *) &(*out)->boundsCenterX)->Merge(
                                *(const AEMath::BSphere *) &child->boundsCenterX);
                        ArrayAdd(child, (*out)->animation->meshes);
                    }
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

        unsigned char *pos = m->positions;
        unsigned char *tex = m->texCoords;
        unsigned char *col = m->colors;
        unsigned char *nrm = m->normals;
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
            float maxZ = az > bz ? az : bz;
            float minZ = az > bz ? bz : az;
            float maxX = ax > bx ? ax : bx;
            float minX = ax > bx ? bx : ax;
            maxZ = maxZ > cz ? maxZ : cz;
            minZ = minZ > cz ? cz : minZ;
            maxX = maxX > cx ? maxX : cx;
            minX = minX > cx ? cx : minX;
            if (maxZ < qz || minX > qx || maxX < qx || minZ > qz)
                continue;

            // The point must lie on the inner side of all three edges.
            float ez = bz - az, ex = bx - ax;
            float len = sqrtf(ez * ez + ex * ex);
            float nx = ex / len, nz = -ez / len;
            if (nx * qz + nz * qx - (az * nx + ax * nz) > 0.0f)
                continue;
            ez = cz - bz;
            ex = cx - bx;
            len = sqrtf(ez * ez + ex * ex);
            nx = ex / len;
            nz = -ez / len;
            if (nx * qz + nz * qx - (bz * nx + bx * nz) > 0.0f)
                continue;
            ez = az - cz;
            ex = ax - cx;
            len = sqrtf(ez * ez + ex * ex);
            nx = ex / len;
            nz = -ez / len;
            if (nx * qz + nz * qx - (cz * nx + cx * nz) > 0.0f)
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

