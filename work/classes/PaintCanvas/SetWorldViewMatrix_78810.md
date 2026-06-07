# PaintCanvas::SetWorldViewMatrix
elf 0x78810 body 8
Sig: undefined __stdcall SetWorldViewMatrix(Matrix * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetWorldViewMatrix(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::PaintCanvas::SetWorldViewMatrix(Matrix *param_1)

{
  (*(code *)(DAT_001ab1c4 + 0x1ab1c8))(*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## target disasm
```
  00088810: ldr r0,[r0,#0x34]
  00088812: b.w 0x001ab1b8
  001ab1b8: bx pc
```
