# PaintCanvas::MeshGetPointer
elf 0x7a5bc body 16
Sig: undefined __thiscall MeshGetPointer(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshGetPointer(unsigned int) */

undefined4 __thiscall AbyssEngine::PaintCanvas::MeshGetPointer(PaintCanvas *this,uint param_1)

{
  undefined4 uVar1;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    uVar1 = *(undefined4 *)(*(int *)(this + 0x28) + param_1 * 4);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

```

## target disasm
```
  0008a5bc: ldr r2,[r0,#0x24]
  0008a5be: cmp r2,r1
  0008a5c0: itee ls
  0008a5c2: mov.ls r0,#0x0
  0008a5c4: ldr.hi r0,[r0,#0x28]
  0008a5c6: ldr.hi.w r0,[r0,r1,lsl #0x2]
  0008a5ca: bx lr
```
