#include "engine/render/ResourceMesh.h"
#include "game/core/String.h"

namespace AbyssEngine {
    ResourceMesh::~ResourceMesh() {
        delete[] name;
        name = nullptr;
    }
}
