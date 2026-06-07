# AEGeometry::getRightVector
elf 0xa45e0 body 26
Sig: undefined __stdcall getRightVector(void)

## decompile
```c

/* AEGeometry::getRightVector() */

void AEGeometry::getRightVector(void)

{
  Matrix *in_r0;
  int in_r1;
  
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r1 + 0x2c));
  AbyssEngine::AEMath::MatrixGetRight(in_r0);
  return;
}

```

## target disasm
```
  000b45e0: push {r4,r6,r7,lr}
  000b45e2: add r7,sp,#0x8
  000b45e4: ldr r2,[r1,#0xc]
  000b45e6: mov r4,r0
  000b45e8: ldr r0,[r1,#0x2c]
  000b45ea: mov r1,r2
  000b45ec: blx 0x000720c4
  000b45f0: mov r1,r0
  000b45f2: mov r0,r4
  000b45f4: blx 0x0006f4e4
  000b45f8: pop {r4,r6,r7,pc}
```
