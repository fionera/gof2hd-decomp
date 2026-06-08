#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetDuration(int duration) {
    f32(this, 0xf0) = (float)duration;
}

} // namespace AbyssEngine
