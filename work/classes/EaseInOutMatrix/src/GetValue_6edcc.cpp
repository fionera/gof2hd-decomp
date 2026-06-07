#include "class.h"

namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetValue() {
    return *(AEMath::Matrix *)((char *)this + 0x78);
}

} // namespace AbyssEngine
