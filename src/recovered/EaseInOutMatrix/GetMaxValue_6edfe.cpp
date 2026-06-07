#include "class.h"

namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetMaxValue() {
    return *(AEMath::Matrix *)((char *)this + 0xb4);
}

} // namespace AbyssEngine
