# PaintCanvas::TransformGetTriCount
elf 0x7b690 body 26
Sig: undefined __thiscall TransformGetTriCount(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformGetTriCount(unsigned int) */

void __thiscall AbyssEngine::PaintCanvas::TransformGetTriCount(PaintCanvas *this,uint param_1)

{
  if (*(uint *)(this + 0x158) <= param_1) {
    return;
  }
  (*(code *)(DAT_001ab2c4 + 0x1ab2c8))(this,*(undefined4 *)(*(int *)(this + 0x15c) + param_1 * 4));
  return;
}

```

## target disasm
```
  0008b690: ldr.w r2,[r0,#0x158]
  0008b694: cmp r2,r1
  0008b696: itt ls
  0008b698: mov.ls r0,#0x0
  0008b69a: bx.ls lr
  0008b69c: ldr.w r2,[r0,#0x15c]
  0008b6a0: ldr.w r1,[r2,r1,lsl #0x2]
  0008b6a4: b.w 0x001ab2b8
  001ab2b8: bx pc
```
