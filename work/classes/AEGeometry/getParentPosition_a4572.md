# AEGeometry::getParentPosition
elf 0xa4572 body 32
Sig: undefined __stdcall getParentPosition(void)

## decompile
```c

/* AEGeometry::getParentPosition() */

void AEGeometry::getParentPosition(void)

{
  Matrix *in_r0;
  int in_r1;
  
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r1 + 0x2c));
  AbyssEngine::AEMath::MatrixGetPosition(in_r0);
  return;
}

```

## target disasm
```
  000b4572: push {r4,r6,r7,lr}
  000b4574: add r7,sp,#0x8
  000b4576: mov r2,r1
  000b4578: ldr r1,[r1,#0x24]
  000b457a: mov r4,r0
  000b457c: ldr r0,[r2,#0x2c]
  000b457e: adds r3,r1,#0x1
  000b4580: it eq
  000b4582: ldr.eq r1,[r2,#0xc]
  000b4584: blx 0x000720c4
  000b4588: mov r1,r0
  000b458a: mov r0,r4
  000b458c: blx 0x0006f16c
  000b4590: pop {r4,r6,r7,pc}
```
