#include "quaternion.h"

namespace AbyssEngine {

Quaternion::Quaternion(const AEMath::Matrix &matrix) {
    Set(matrix);
}

} // namespace AbyssEngine
