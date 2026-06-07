# PaintCanvas::ClearBuffer
elf 0x7ca84 body 36
Sig: undefined __stdcall ClearBuffer(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::ClearBuffer(unsigned int) */

void AbyssEngine::PaintCanvas::ClearBuffer(uint param_1)

{
  glEnable(0xb71);
  glDepthMask(1);
  (*(code *)(DAT_001ab374 + 0x1ab378))(*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## target disasm
```
  0008ca84: push {r4,r5,r7,lr}
  0008ca86: add r7,sp,#0x8
  0008ca88: mov r5,r0
  0008ca8a: movw r0,#0xb71
  0008ca8e: mov r4,r1
  0008ca90: blx 0x0006edb8
  0008ca94: movs r0,#0x1
  0008ca96: blx 0x0006eec0
  0008ca9a: ldr r0,[r5,#0x34]
  0008ca9c: mov r1,r4
  0008ca9e: pop.w {r4,r5,r7,lr}
  0008caa2: b.w 0x001ab368
  001ab368: bx pc
```
