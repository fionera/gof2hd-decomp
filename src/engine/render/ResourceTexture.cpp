#include "engine/render/ResourceTexture.h"
#include "game/core/String.h"

namespace AbyssEngine {
    ResourceTexture::~ResourceTexture() {
        delete[] this->name;
        this->name = nullptr;
    }
}
