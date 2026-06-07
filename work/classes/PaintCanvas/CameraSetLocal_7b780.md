# PaintCanvas::CameraSetLocal
elf 0x7b780 body 36
Sig: undefined __stdcall CameraSetLocal(uint param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraSetLocal(unsigned int, AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::PaintCanvas::CameraSetLocal(uint param_1,Matrix *param_2)

{
  Matrix *in_r2;
  
  if (*(Matrix **)(param_1 + 0x164) <= param_2) {
    return;
  }
  AEMath::Matrix::operator=
            ((Matrix *)(*(int *)(*(int *)(param_1 + 0x168) + (int)param_2 * 4) + 0xc),in_r2);
  return;
}

```

## target disasm
```
  0008b780: ldr.w r3,[r0,#0x164]
  0008b784: cmp r3,r1
  0008b786: it ls
  0008b788: bx.ls lr
  0008b78a: push {r7,lr}
  0008b78c: mov r7,sp
  0008b78e: ldr.w r0,[r0,#0x168]
  0008b792: ldr.w r0,[r0,r1,lsl #0x2]
  0008b796: mov r1,r2
  0008b798: adds r0,#0xc
  0008b79a: blx 0x0006f148
  0008b79e: pop.w {r7,lr}
  0008b7a2: bx lr
```
