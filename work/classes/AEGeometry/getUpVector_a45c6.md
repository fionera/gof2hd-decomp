# AEGeometry::getUpVector
elf 0xa45c6 body 26
Sig: undefined __stdcall getUpVector(void)

## decompile
```c

/* AEGeometry::getUpVector() */

void AEGeometry::getUpVector(void)

{
  Matrix *in_r0;
  int in_r1;
  
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r1 + 0x2c));
  AbyssEngine::AEMath::MatrixGetUp(in_r0);
  return;
}

```

## target disasm
```
  000b45c6: push {r4,r6,r7,lr}
  000b45c8: add r7,sp,#0x8
  000b45ca: ldr r2,[r1,#0xc]
  000b45cc: mov r4,r0
  000b45ce: ldr r0,[r1,#0x2c]
  000b45d0: mov r1,r2
  000b45d2: blx 0x000720c4
  000b45d6: mov r1,r0
  000b45d8: mov r0,r4
  000b45da: blx 0x0006f4d8
  000b45de: pop {r4,r6,r7,pc}
```
