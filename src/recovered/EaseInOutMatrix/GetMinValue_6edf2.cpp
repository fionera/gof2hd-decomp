#include "class.h"

namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetMinValue() {
    return *(AEMath::Matrix *)((char *)this + 0x0);
}

} // namespace AbyssEngine
