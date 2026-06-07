#include "class.h"

// Guard on byte at 0x47; if clear, arm a small hit: shake timer 0x32, flag set,
// shake mode 2 at 0x120, set byte at 0x124.
extern "C" void TFC_hitSmall(TargetFollowCamera *self) {
    if (F<uint8_t>(self, 0x47) != 0) return;
    F<int>(self, 0x48) = 0x32;
    F<uint8_t>(self, 0x47) = 1;
    F<int>(self, 0x120) = 2;
    F<uint8_t>(self, 0x124) = 1;
}
