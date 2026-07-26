#include "engine/render/AEGeometry.h"
#include "engine/render/LodMeshMerger.h"
#include "engine/math/Transform.h"

uint32_t Transform_GetTransform(uint32_t tf);

Vector AEGeometry::getPosition() {
    Matrix &loc = *(Matrix *) this->canvas->TransformGetLocal(this->transform);
    return MatrixGetPosition(loc);
}

AEGeometry::~AEGeometry() {
    delete[] this->lodTransforms;
    this->lodTransforms = nullptr;
    delete[] this->lodMeshes;
    this->lodMeshes = nullptr;
    delete[] this->lodChildTransforms;
    this->lodChildTransforms = nullptr;
    delete[] this->lodChildMeshes;
    this->lodChildMeshes = nullptr;
    delete[] this->lodDistancesSq;
    this->lodDistancesSq = nullptr;
}

bool AEGeometry::hasLod() { return this->lodTransforms != nullptr; }

uint16_t AEGeometry::getID() { return this->mesh; }

uint8_t AEGeometry::isVisible() { return (uint8_t) this->visibility; }

void AEGeometry::setVisible(bool v) {
    // ASM: original writes the raw bool to two bytes (strb @0x49 then @0x48), not a normalized 0x101 strh.
    reinterpret_cast<uint8_t *>(&this->visibility)[0] = v;
    reinterpret_cast<uint8_t *>(&this->visibility)[1] = v;
}

void AEGeometry::DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b) {
    this->mergerIndex = (uint32_t) a;
    this->merger = b;
}

void AEGeometry::addChild(uint32_t child) {
    this->canvas->TransformAddChild(this->transform, child);
    uint32_t old = this->childTransform;
    if (old != 0xffffffffu)
        this->parentTransform = old;
    this->childTransform = child;
}

void AEGeometry::render() {
    if ((uint8_t) this->visibility == 0)
        return;
    this->canvas->DrawTransform(this->transform, nullptr);
}

void AEGeometry::translate(const Vector &v) {
    translate(v.x, v.y, v.z);
}

void AEGeometry::setScaling(float s) {
    setScaling(s, s, s);
}

Vector AEGeometry::getRotation() {
    return this->rotation;
}

void AEGeometry::setScaling(const Vector &v) {
    setScaling(v.x, v.y, v.z);
}

void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count) {
    this->lodMeshes = new uint16_t[count];
    this->lodDistancesSq = new unsigned long long[count];
    this->lodCount = count;
    this->lodTransforms = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        this->lodMeshes[i] = meshes[i];
        this->lodDistancesSq[i] = (unsigned long long) (long long) dists[i];
        this->canvas->TransformCreate(this->lodTransforms[i]);
        this->canvas->TransformAddMeshId(this->lodTransforms[i], meshIds[i]);
        unsigned long long v = this->lodDistancesSq[i];
        this->lodDistancesSq[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            this->canvas->TransformAddChild(this->lodTransforms[i], this->childTransform);
    }
}

void AEGeometry::setLodLastVisibleDistance(uint64_t d) {
    this->lastVisibleDistSq = d * d;
}

Vector AEGeometry::getRightVector() {
    Matrix &loc = *(Matrix *) this->canvas->TransformGetLocal(this->transform);
    return MatrixGetRight(loc);
}

Vector AEGeometry::getUpVector() {
    Matrix &loc = *(Matrix *) this->canvas->TransformGetLocal(this->transform);
    return MatrixGetUp(loc);
}

Matrix &AEGeometry::getMatrix() {
    return *(Matrix *) this->canvas->TransformGetLocal(this->transform);
}

Vector AEGeometry::getDirection() {
    Matrix &loc = *(Matrix *) this->canvas->TransformGetLocal(this->transform);
    return MatrixGetDir(loc);
}

Matrix &AEGeometry::getReferenceMatrix() {
    return this->referenceMatrix;
}

void AEGeometry::setMatrix(const Matrix &m) {
    AEGeomCanvas::TransformSetLocal(this->canvas, this->transform, (Matrix *) &m);
}

void AEGeometry::setRotationOrder(AbyssEngine::AEMath::RotationOrder order) {
    this->rotationOrder = order;
}

void AEGeometry::setPosition(float x, float y, float z) {
    AbyssEngine::AEMath::MatrixSetTranslation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform), x, y, z);
}

Vector AEGeometry::getScaling() {
    Vector v;
    v.x = this->scalingX;
    v.y = this->scalingY;
    v.z = this->scalingZ;
    return v;
}

void AEGeometry::updateReferenceMatrix() {
    Matrix *loc = (Matrix *) this->canvas->TransformGetLocal(this->transform);
    this->referenceMatrix = *loc;
}

void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count) {
    this->lodMeshes = new uint16_t[count];
    this->lodDistancesSq = new unsigned long long[count];
    this->lodCount = count;
    this->lodTransforms = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        this->lodMeshes[i] = meshes[i];
        this->lodDistancesSq[i] = (unsigned long long) (long long) dists[i];
        this->canvas->TransformCreate(this->lodTransforms[i]);
        this->canvas->TransformAddMesh(this->lodTransforms[i], meshes[i], 0);
        unsigned long long v = this->lodDistancesSq[i];
        this->lodDistancesSq[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            this->canvas->TransformAddChild(this->lodTransforms[i], this->childTransform);
    }
}

Vector AEGeometry::getParentPosition() {
    PaintCanvas *cnvs = this->canvas;
    uint32_t tf = this->altTransform;
    if (tf == 0xffffffffu)
        tf = this->transform;
    Matrix &loc = *(Matrix *) cnvs->TransformGetLocal(tf);
    return MatrixGetPosition(loc);
}

void AEGeometry::setRotation(float x, float y, float z) {
    AbyssEngine::AEMath::MatrixSetRotation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform), x, y, z,
        (AbyssEngine::AEMath::RotationOrder) this->rotationOrder);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->scalingX, this->scalingY, this->scalingZ);
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
}

void AEGeometry::setRotation(const Vector &v) {
    setRotation(v.x, v.y, v.z);
}

void AEGeometry::setPosition(const Vector &v) {
    setPosition(v.x, v.y, v.z);
}

void AEGeometry::setLodChildMeshes(uint16_t *meshes) {
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildMeshes = new uint16_t[count];
        this->lodChildTransforms = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            this->lodChildMeshes[i] = meshes[i];
            this->canvas->TransformCreate(this->lodChildTransforms[i]);
            this->canvas->TransformAddMesh(this->lodChildTransforms[i], meshes[i], 0);
            this->canvas->TransformAddChild(this->lodTransforms[i], this->lodChildTransforms[i]);
            this->canvas->TransformRemoveChild(this->lodTransforms[i], this->childTransform);
            this->canvas->TransformAddChild(this->lodTransforms[i], this->childTransform);
            count = this->lodCount;
        }
    }
}

void AEGeometry::rotate(float x, float y, float z) {
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    AbyssEngine::AEMath::MatrixSetRotation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->rotation.x, this->rotation.y, this->rotation.z,
        (AbyssEngine::AEMath::RotationOrder) this->rotationOrder);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->scalingX, this->scalingY, this->scalingZ);
}

AEGeometry::AEGeometry(PaintCanvas *canvas) {
    this->cameraDelta = Vector{0, 0, 0};
    this->referenceMatrix = Matrix();
    this->mesh = 0;
    this->canvas = canvas;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);

    this->rotation = Vector{0.0f, 0.0f, 0.0f};
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lodDistancesSq = nullptr;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->meshId = 0xffffffff;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->merger = nullptr;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
}

void AEGeometry::setMesh(uint16_t mesh) {
    if (this->transform != 0)
        return this->canvas->TransformAddMesh(this->transform, mesh, false);
    return this->canvas->TransformCreate(mesh, this->transform);
}

void AEGeometry::translate(float x, float y, float z) {
    char buf[60];
    char src[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    memcpy(src, (const Matrix *) (uintptr_t) loc, 0x3c);
    AbyssEngine::AEMath::MatrixSetTranslation(*(Matrix *)buf, *(float *) (src + 0x0c) + z,
                                              *(float *) (src + 0x1c), *(float *) (src + 0x2c) + y);
    this->canvas->TransformSetLocal(this->transform, *(const Matrix *)buf);
    (void) x;
}

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag) {
    this->cameraDelta = Vector{0, 0, 0};
    this->referenceMatrix = Matrix();
    this->canvas = canvas;
    this->mesh = mesh;
    this->transform = 0;
    this->baseTransform = 0;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);
    canvas->MeshCreate(mesh, this->meshId, flag);
    canvas->TransformAddMeshId(this->baseTransform, this->meshId);

    this->rotation = Vector{0.0f, 0.0f, 0.0f};
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lodDistancesSq = nullptr;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)canvas, 0);
    this->referenceMatrix = *(Matrix *) loc;
    this->merger = nullptr;
}

void AEGeometry::setScaling(float x, float y, float z) {
    AbyssEngine::AEMath::MatrixSetRotation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->rotation.x, this->rotation.y, this->rotation.z,
        (AbyssEngine::AEMath::RotationOrder) this->rotationOrder);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform), x, y, z);
    this->scalingX = x;
    this->scalingY = y;
    this->scalingZ = z;
}

void AEGeometry::moveForward(float dist) {
    Vector direction = this->getDirection();
    Vector n = AbyssEngine::AEMath::VectorNormalize(direction);
    Vector pos = this->getPosition();
    pos.x = pos.x + n.x * dist;
    pos.y = pos.y + n.y * dist;
    pos.z = pos.z + n.z * dist;
    this->setPosition(pos);
}

void AEGeometry::updateLod(const Vector &camPos, float screenScale) {
    this->visibility = (this->visibility & 0xff00) | (this->visibility >> 8);

    char matrixCopy[60];
    memcpy(matrixCopy, this->canvas->TransformGetLocal(this->transform), 0x3c);

    uint32_t tf2 = this->altTransform;
    if (tf2 == 0xffffffffu) tf2 = this->transform;
    Vector pos = MatrixGetPosition(*(Matrix *)this->canvas->TransformGetLocal(tf2));
    this->cameraDelta = pos - camPos;

    float dx = this->cameraDelta.x, dy = this->cameraDelta.y, dz = this->cameraDelta.z;
    this->distSq = (unsigned long long) (dy * dy + dx * dx + dz * dz);

    unsigned long long lastVis = this->lastVisibleDistSq;
    bool visible;
    if (lastVis == 0) {
        visible = true;
    } else {
        visible = this->distSq < lastVis;
        this->visibility = (this->visibility & 0xff00) | (visible ? 1 : 0);
    }

    if (!visible) {
        this->currentLod = -1;
        return;
    }

    AbyssEngine::Transform *tgt = (AbyssEngine::Transform *)this->canvas->TransformGetTransform(this->transform);

    float factor = (screenScale <= 0.0625f) ? 0.75f : 1.0f;
    float detail = (0.03125f < screenScale) ? factor : 0.5f;

    int level = this->lodCount;

    while (level >= 1) {
        int idx = level - 1;
        float thresh = (float) this->lodDistancesSq[idx];
        float d = (float) this->distSq;
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }

        uint32_t lodTf = this->lodTransforms[idx];
        if (lodTf != this->transform) {
            this->canvas->TransformSetLocal(lodTf, *(Matrix *) matrixCopy);
            this->transform = this->lodTransforms[idx];
            int64_t animTime = tgt->currentTime;
            tgt = (AbyssEngine::Transform *)this->canvas->TransformGetTransform(this->transform);
            tgt->SetCurrentAnimationTime(animTime);
            tgt = (AbyssEngine::Transform *)this->canvas->TransformGetTransform(this->baseTransform);
            tgt->SetCurrentAnimationTime(animTime);
            this->currentLod = level;
            this->referenceMatrix = *(Matrix *) matrixCopy;
            if (this->merger != nullptr)
                this->merger->setLod(this->mergerIndex, (signed char) level);
        }
        return;
    }

    this->canvas->TransformSetLocal(this->baseTransform, *(Matrix *) matrixCopy);
    this->currentLod = 0;
    this->transform = this->baseTransform;
    if (this->merger != nullptr)
        this->merger->setLod(this->mergerIndex, 0);
}

void AEGeometry::setDirection(const Vector &dir, const Vector &up) {
    char local[60];
    memcpy(local, this->canvas->TransformGetLocal(this->transform), 0x3c);

    Vector newUp = up;
    Vector newRight = AbyssEngine::AEMath::VectorCross(newUp, dir);
    newRight = AbyssEngine::AEMath::VectorNormalize(newRight);
    newUp = AbyssEngine::AEMath::VectorCross(dir, newRight);
    newUp = AbyssEngine::AEMath::VectorNormalize(newUp);

    Matrix &m = *(Matrix *) local;
    m.m[0] = newRight.x;
    m.m[4] = newRight.y;
    m.m[8] = newRight.z;
    m.m[1] = newUp.x;
    m.m[5] = newUp.y;
    m.m[9] = newUp.z;
    m.m[2] = dir.x;
    m.m[6] = dir.y;
    m.m[10] = dir.z;

    this->canvas->TransformSetLocal(this->transform, m);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->scalingX, this->scalingY, this->scalingZ);
}

void AEGeometry::rotate(const Vector &v) {
    rotate(v.x, v.y, v.z);
}

void AEGeometry::setLodChildTransform(uint32_t param) {
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildTransforms = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            this->canvas->TransformAddChild(this->lodTransforms[i], param);
            count = this->lodCount;
        }
    }
}
