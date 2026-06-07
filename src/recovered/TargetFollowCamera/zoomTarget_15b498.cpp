#include "class.h"

// str.w r1,[r0,#0xb0]
extern "C" void TFC_zoomTarget(TargetFollowCamera *self, float v) {
    F<float>(self, 0xb0) = v;
}
