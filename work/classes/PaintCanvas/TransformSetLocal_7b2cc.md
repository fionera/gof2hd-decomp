# PaintCanvas::TransformSetLocal
elf 0x7b2cc body 34
Sig: undefined __stdcall TransformSetLocal(uint param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformSetLocal(unsigned int, AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::PaintCanvas::TransformSetLocal(uint param_1,Matrix *param_2)

{
  Matrix *in_r2;
  
  if (*(Matrix **)(param_1 + 0x158) <= param_2) {
    return;
  }
  AEMath::Matrix::operator=(*(Matrix **)(*(int *)(param_1 + 0x15c) + (int)param_2 * 4),in_r2);
  return;
}

```

## target disasm
```
  0008b2cc: ldr.w r3,[r0,#0x158]
  0008b2d0: cmp r3,r1
  0008b2d2: it ls
  0008b2d4: bx.ls lr
  0008b2d6: push {r7,lr}
  0008b2d8: mov r7,sp
  0008b2da: ldr.w r0,[r0,#0x15c]
  0008b2de: ldr.w r0,[r0,r1,lsl #0x2]
  0008b2e2: mov r1,r2
  0008b2e4: blx 0x0006f148
  0008b2e8: pop.w {r7,lr}
  0008b2ec: bx lr
```
