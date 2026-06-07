# PaintCanvas::DrawMesh
elf 0x7968c body 22
Sig: undefined __thiscall DrawMesh(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawMesh(unsigned int) */

void __thiscall AbyssEngine::PaintCanvas::DrawMesh(PaintCanvas *this,uint param_1)

{
  if (param_1 < *(uint *)(this + 0x24)) {
    (*(code *)(DAT_001ab1f4 + 0x1ab1f8))
              (*(undefined4 *)(this + 0x34),*(undefined4 *)(*(int *)(this + 0x28) + param_1 * 4));
    return;
  }
  return;
}

```

## target disasm
```
  0008968c: ldr r2,[r0,#0x24]
  0008968e: cmp r2,r1
  00089690: it ls
  00089692: bx.ls lr
  00089694: ldr r2,[r0,#0x28]
  00089696: ldr r0,[r0,#0x34]
  00089698: ldr.w r1,[r2,r1,lsl #0x2]
  0008969c: b.w 0x001ab1e8
  001ab1e8: bx pc
```
