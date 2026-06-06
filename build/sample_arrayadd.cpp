// PoC for the byte-match harness: reproduce AbyssEngine ArrayAdd<T*>.
// Target: Android libgof2hdaa.so, ArrayAdd<AEPakFileEntry*> @ Ghidra 0x000805b4
//         (ELF vaddr 0x000705b4 — subtract the 0x10000 image base).
//
// Target disassembly (Thumb-2):
//   push {r4,r5,r7,lr}; add r7,sp,#8; mov r4,r1; ldr r1,[r1]; mov r5,r0
//   ldr r0,[r4,#4]; adds r1,#1; str r1,[r4,#8]; lsls r1,r1,#2; blx realloc
//   ldr r1,[r4]; str.w r5,[r0,r1,lsl #2]; ldr r1,[r4,#8]; strd r1,r0,[r4]; pop {...,pc}
//
// Match notes: don't keep (size+1) live across realloc (the target re-reads size/size2 from
// memory afterward), so no extra callee-saved register / stack-alignment padding is needed.
// Header-free; build WITHOUT -fomit-frame-pointer.
extern "C" void* realloc(void* p, unsigned int n);

struct AEArray { unsigned int size; void** data; unsigned int size2; };

void ArrayAdd(void* item, AEArray& a) {
    a.size2 = a.size + 1;
    a.data  = (void**)realloc(a.data, a.size2 * 4);
    a.data[a.size] = item;
    a.size  = a.size2;
}
