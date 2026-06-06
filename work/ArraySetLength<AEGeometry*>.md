# ArraySetLength<AEGeometry*>

ELF vaddr 0xa721c (Ghidra 0xb721c), body 52 bytes
Signature: void __stdcall ArraySetLength<AEGeometry*>(uint param_1, Array * param_2)

## Ghidra decompile
```c

/* void ArraySetLength<AEGeometry*>(unsigned int, Array<AEGeometry*>&) */

void ArraySetLength<AEGeometry*>(uint param_1,Array *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  
  if (*(uint *)(param_2 + 8) == param_1) {
    uVar1 = *(undefined4 *)(param_2 + 4);
    uVar2 = param_1;
  }
  else {
    uVar2 = param_1;
    if (param_1 == 0) {
      uVar2 = 1;
    }
    *(uint *)(param_2 + 8) = uVar2;
    uVar1 = realloc(*(undefined4 *)(param_2 + 4),uVar2 << 2);
    uVar2 = *(uint *)(param_2 + 8);
    *(undefined4 *)(param_2 + 4) = uVar1;
  }
  __aeabi_memclr4(uVar1,uVar2 << 2);
  *(uint *)param_2 = param_1;
  return;
}

```

## Target disassembly
```
  000b721c: push {r4,r5,r7,lr}
  000b721e: add r7,sp,#0x8
  000b7220: mov r5,r0
  000b7222: ldr r0,[r1,#0x8]
  000b7224: mov r4,r1
  000b7226: cmp r0,r5
  000b7228: bne 0x000b7230
  000b722a: ldr r0,[r4,#0x4]
  000b722c: mov r1,r5
  000b722e: b 0x000b7246
  000b7230: ldr r0,[r4,#0x4]
  000b7232: mov r1,r5
  000b7234: cmp r5,#0x0
  000b7236: it eq
  000b7238: mov.eq r1,#0x1
  000b723a: str r1,[r4,#0x8]
  000b723c: lsls r1,r1,#0x2
  000b723e: blx 0x0006ecc8
  000b7242: ldr r1,[r4,#0x8]
  000b7244: str r0,[r4,#0x4]
  000b7246: lsls r1,r1,#0x2
  000b7248: blx 0x0006ec14
  000b724c: str r5,[r4,#0x0]
  000b724e: pop {r4,r5,r7,pc}
```

## Callees
  <EXTERNAL>::__aeabi_memclr4
  <EXTERNAL>::realloc

## Verify
```
python3 tools/gofdiff.py --so ../_work/bins/android_2.0.16_libgof2hdaa.so --vaddr a721c --n 68 --obj build/obj/<tu>.o --sym ArraySetLength<AEGeometry*>
```
