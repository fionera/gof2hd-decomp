# AEGeometry::updateReferenceMatrix
elf 0xa4a6a body 28
Sig: undefined __stdcall updateReferenceMatrix(void)

## decompile
```c

/* AEGeometry::updateReferenceMatrix() */

void AEGeometry::updateReferenceMatrix(void)

{
  int in_r0;
  undefined4 uVar1;
  
  uVar1 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  (*(code *)(DAT_001abdb4 + 0x1abdb8))(in_r0 + 0x84,uVar1);
  return;
}

```

## target disasm
```
  000b4a6a: push {r4,r6,r7,lr}
  000b4a6c: add r7,sp,#0x8
  000b4a6e: mov r4,r0
  000b4a70: ldr r1,[r0,#0xc]
  000b4a72: ldr r0,[r0,#0x2c]
  000b4a74: blx 0x000720c4
  000b4a78: mov r1,r0
  000b4a7a: add.w r0,r4,#0x84
  000b4a7e: pop.w {r4,r6,r7,lr}
  000b4a82: b.w 0x001abda8
```
