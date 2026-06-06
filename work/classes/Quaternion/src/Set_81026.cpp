#include "quaternion.h"

namespace AbyssEngine {

void Quaternion::Set(AEMath::Vector angles) {
    Set(angles.x, angles.y, angles.z);
}

namespace {

__attribute__((used)) void Set_81026_trailer() {}

}

} // namespace AbyssEngine
