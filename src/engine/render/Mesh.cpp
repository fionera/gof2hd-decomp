#include "engine/render/Mesh.h"
#include "engine/math/BSphere.h"
#include "engine/render/Engine.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "engine/file/AEFile.h"
#include "engine/core/AbyssEngine.h"

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

        this->vboByteSize = 0;
        this->vertexFormat = src->vertexFormat;
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

        this->shared = 1;
        this->hasAnimation = src->hasAnimation;
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
