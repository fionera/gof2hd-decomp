#include "class.h"

// push {r7,lr}; mov r7,sp; add.w r1,r1,#0x13c; movs r2,#0x3c; blx __aeabi_memcpy; pop {r7,pc}
// getLocal() returns the local matrix (this+0x13c, 0x3c=60 bytes) by value. Returning
// a 60-byte aggregate uses sret (r0 = hidden dest, r1 = this); clang copies it with
// __aeabi_memcpy and needs no register saved across the call.
struct Mat60 { float m[15]; };

extern "C" Mat60 TFC_getLocal(TargetFollowCamera *self) {
    return *(Mat60 *)((char *)self + 0x13c);
}
