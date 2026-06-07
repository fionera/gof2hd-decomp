# PaintCanvas::TransformSetColor
elf 0x7b410 body 20
Sig: undefined __thiscall TransformSetColor(PaintCanvas * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformSetColor(unsigned int, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TransformSetColor(PaintCanvas *this,uint param_1,uint param_2)

{
  PaintCanvas *pPVar1;
  
  pPVar1 = this + 0x158;
  if (param_1 < *(uint *)pPVar1) {
    this = *(PaintCanvas **)(*(int *)(this + 0x15c) + param_1 * 4);
  }
  if (param_1 < *(uint *)pPVar1) {
    *(uint *)(this + 0x48) = param_2;
  }
  return;
}

```

## target disasm
```
  0008b410: ldr.w r3,[r0,#0x158]
  0008b414: cmp r3,r1
  0008b416: ittt hi
  0008b418: ldr.hi.w r0,[r0,#0x15c]
  0008b41c: ldr.hi.w r0,[r0,r1,lsl #0x2]
  0008b420: str.hi r2,[r0,#0x48]
  0008b422: bx lr
```
