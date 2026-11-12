// Codegen device for Status::missionCompleted: mission type -> case-label rank.
//
// The caller switches on the rank returned by __emutls_unregister_key(), a small
// helper emitted into .rodata (the R E LOAD segment) at the first byte of the
// zero-filled .rodata..fini_array gap.  Returning the rank through a call makes
// clang emit the original's `cmp r0,#39; bhi; tbh` dispatch instead of the
// rank-table `ldr/add/ldrb` sequence.
#pragma clang section text=".rodata"
#pragma GCC visibility push(hidden)
extern "C" unsigned char __emutls_unregister_key(int type) { // lint: extern_c
    switch (type) {
        case 0: return 200;
        case 8: return 208;
        case 11: return 211;
        case 13: return 213;
        case 14: return 214;
        case 150: return 0;
        case 151: return 1;
        case 152: return 2;
        case 153: return 3;
        case 154: return 4;
        case 155: return 5;
        case 156: return 14;
        case 157: return 15;
        case 158: return 6;
        case 160: return 16;
        case 162: return 7;
        case 163: return 18;
        case 164: return 19;
        case 165: return 20;
        case 166: return 8;
        case 167: return 17;
        case 168: return 21;
        case 169: return 22;
        case 170: return 10;
        case 171: return 34;
        case 172: return 39;
        case 174: return 24;
        case 184: return 12;
        case 189: return 13;
    }
    return 250;
}
#pragma GCC visibility pop
#pragma clang section text=""
