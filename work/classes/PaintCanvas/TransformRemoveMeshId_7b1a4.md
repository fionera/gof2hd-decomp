# PaintCanvas::TransformRemoveMeshId
elf 0x7b1a4 body 36
Sig: undefined __thiscall TransformRemoveMeshId(PaintCanvas * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformRemoveMeshId(unsigned int, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TransformRemoveMeshId(PaintCanvas *this,uint param_1,uint param_2)

{
  bool bVar1;
  bool bVar2;
  
  bVar2 = param_1 <= *(uint *)(this + 0x158);
  bVar1 = *(uint *)(this + 0x158) == param_1;
  if (bVar2 && !bVar1) {
    bVar2 = param_2 <= *(uint *)(this + 0x24);
    bVar1 = *(uint *)(this + 0x24) == param_2;
  }
  if (!bVar2 || bVar1) {
    return;
  }
  (*(code *)(DAT_001ab2a4 + 0x1ab2a8))
            (*(undefined4 *)(*(int *)(this + 0x28) + param_2 * 4),
             *(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0x3c);
  return;
}

```

## target disasm
```
  0008b1a4: ldr.w r3,[r0,#0x158]
  0008b1a8: cmp r3,r1
  0008b1aa: itte hi
  0008b1ac: ldr.hi r3,[r0,#0x24]
  0008b1ae: cmp.hi r3,r2
  0008b1b0: bx.ls lr
  0008b1b2: ldr r3,[r0,#0x28]
  0008b1b4: ldr.w r0,[r0,#0x15c]
  0008b1b8: ldr.w r1,[r0,r1,lsl #0x2]
  0008b1bc: ldr.w r0,[r3,r2,lsl #0x2]
  0008b1c0: adds r1,#0x3c
  0008b1c2: b.w 0x001ab298
  001ab298: bx pc
```
