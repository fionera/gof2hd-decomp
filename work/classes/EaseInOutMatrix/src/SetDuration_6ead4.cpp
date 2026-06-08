#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetDuration(int duration) {
    this->f_f0 = (float)duration;
}

} // namespace AbyssEngine
