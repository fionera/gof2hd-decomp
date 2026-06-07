#include "class.h"

// Guard on byte at 0x47; if clear, arm a big hit: shake timer 1000, flag set,
// shake mode 6 at 0x120, clear byte at 0x124.
extern "C" void TFC_hit(TargetFollowCamera *self) {
    if (F<uint8_t>(self, 0x47) != 0) return;
    F<int>(self, 0x48) = 1000;
    F<uint8_t>(self, 0x47) = 1;
    F<int>(self, 0x120) = 6;
    F<uint8_t>(self, 0x124) = 0;
}
