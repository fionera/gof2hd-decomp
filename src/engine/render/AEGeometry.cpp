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
    return Vector(this->rotationX, this->rotationY, this->rotationZ);
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
    this->canvas->TransformSetLocal(this->transform, m);
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
    this->rotationX = x;
    this->rotationY = y;
    this->rotationZ = z;
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
    this->rotationX += x;
    this->rotationY += y;
    this->rotationZ += z;
    AbyssEngine::AEMath::MatrixSetRotation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->rotationX, this->rotationY, this->rotationZ,
        (AbyssEngine::AEMath::RotationOrder) this->rotationOrder);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->scalingX, this->scalingY, this->scalingZ);
}

AEGeometry::AEGeometry(PaintCanvas *canvas)
    : cameraDelta() {
    this->canvas = canvas;
    this->mesh = 0;
    canvas->TransformCreate(this->baseTransform);

    this->rotationX = 0.0f;
    this->rotationY = 0.0f;
    this->rotationZ = 0.0f;
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lastVisibleDistSq = 0;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->lodDistancesSq = nullptr;
    this->meshHandle = 0xffffffff;
    this->merger = nullptr;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->meshId = 0xffffffff;
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
    Matrix local = *(const Matrix *) this->canvas->TransformGetLocal(this->transform);
    AbyssEngine::AEMath::MatrixSetTranslation(local, local.m[3] + x, local.m[7] + y, local.m[11] + z);
    this->canvas->TransformSetLocal(this->transform, local);
}

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
    : cameraDelta() {
    this->mesh = mesh;
    this->canvas = canvas;
    this->transform = 0;
    this->baseTransform = 0;
    canvas->TransformCreate(this->baseTransform);
    canvas->MeshCreate(mesh, this->meshId, flag);
    canvas->TransformAddMeshId(this->baseTransform, this->meshId);

    this->rotationX = 0.0f;
    this->rotationY = 0.0f;
    this->rotationZ = 0.0f;
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lastVisibleDistSq = 0;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->lodDistancesSq = nullptr;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
    this->referenceMatrix = *(const Matrix *) canvas->TransformGetLocal(this->transform);
    this->merger = nullptr;
}

void AEGeometry::setScaling(float x, float y, float z) {
    AbyssEngine::AEMath::MatrixSetRotation(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform),
        this->rotationX, this->rotationY, this->rotationZ,
        (AbyssEngine::AEMath::RotationOrder) this->rotationOrder);
    AbyssEngine::AEMath::MatrixSetScaling(
        *(Matrix *) this->canvas->TransformGetLocal(this->transform), x, y, z);
    this->scalingX = x;
    this->scalingY = y;
    this->scalingZ = z;
}

void AEGeometry::moveForward(float dist) {
    Vector n = AbyssEngine::AEMath::VectorNormalize(this->getDirection());
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
    this->cameraDelta = MatrixGetPosition(*(Matrix *)this->canvas->TransformGetLocal(tf2)) - camPos;

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

    float factor = (screenScale <= 0.66f) ? 0.75f : 1.0f;
    float detail = 0.5f;
    if (!(screenScale <= 0.33f))
        detail = factor;

    int64_t animTime = tgt->currentTime;
    for (int i = this->lodCount - 1; i >= 0; i--) {
        if (detail * (float) this->lodDistancesSq[i] < (float) this->distSq) {
            if (this->lodTransforms[i] != this->transform) {
                this->canvas->TransformSetLocal(this->lodTransforms[i], *(Matrix *) matrixCopy);
                this->transform = this->lodTransforms[i];
                tgt = (AbyssEngine::Transform *) this->canvas->TransformGetTransform(this->lodTransforms[i]);
                tgt->SetCurrentAnimationTime(animTime);
                tgt = (AbyssEngine::Transform *) this->canvas->TransformGetTransform(this->baseTransform);
                tgt->SetCurrentAnimationTime(animTime);
                this->currentLod = i + 1;
                this->referenceMatrix = *(Matrix *) matrixCopy;
                if (this->merger != nullptr)
                    this->merger->setLod(this->mergerIndex, (signed char) (i + 1));
            }
            return;
        }
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
