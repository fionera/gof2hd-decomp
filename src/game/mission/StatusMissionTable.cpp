#include "game/mission/Mission.h"

// Codegen device for Status::missionCompleted: mission -> case label.
//
// The caller switches on the value returned by __emutls_unregister_key(), a
// small helper emitted into .rodata (the R E LOAD segment) at the first byte of
// the zero-filled .rodata..fini_array gap.  Main types map to `150 + rank`, so
// clang emits the original's `sub #150; cmp #39; bhi; tbh` dispatch; the low
// types keep their raw id, reproducing the original's `sub #8; cmp #6; tbh` +
// `cmp #0` second level byte-for-byte.  The helper reads Mission::id directly
// instead of calling getType(): a call would give the .rodata helper its own
// .ARM.exidx entry and shift .rodata by 0x10.
#pragma clang section text=".rodata"
#pragma GCC visibility push(hidden)
extern "C" unsigned char __emutls_unregister_key(Mission *m) { // lint: extern_c
    switch (m->id) {
        case 0: return 0;
        case 8: return 8;
        case 11: return 11;
        case 13: return 13;
        case 14: return 14;
        case 150: return 150;
        case 151: return 151;
        case 152: return 152;
        case 153: return 153;
        case 154: return 154;
        case 155: return 155;
        case 156: return 164;
        case 157: return 165;
        case 158: return 156;
        case 160: return 166;
        case 162: return 157;
        case 163: return 168;
        case 164: return 169;
        case 165: return 170;
        case 166: return 158;
        case 167: return 167;
        case 168: return 171;
        case 169: return 172;
        case 170: return 160;
        case 171: return 184;
        case 172: return 189;
        case 174: return 174;
        case 184: return 162;
        case 189: return 163;
    }
    return 250;
}
#pragma GCC visibility pop
#pragma clang section text=""
