# PaintCanvas::MaterialGetMaterial
elf 0x7c274 body 20
Sig: undefined __thiscall MaterialGetMaterial(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::MaterialGetMaterial(unsigned int) */

undefined4 __thiscall AbyssEngine::PaintCanvas::MaterialGetMaterial(PaintCanvas *this,uint param_1)

{
  undefined4 uVar1;
  
  if (param_1 < *(uint *)(this + 0x174)) {
    uVar1 = *(undefined4 *)(*(int *)(this + 0x178) + param_1 * 4);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

```

## target disasm
```
  0008c274: ldr.w r2,[r0,#0x174]
  0008c278: cmp r2,r1
  0008c27a: itee ls
  0008c27c: mov.ls r0,#0x0
  0008c27e: ldr.hi.w r0,[r0,#0x178]
  0008c282: ldr.hi.w r0,[r0,r1,lsl #0x2]
  0008c286: bx lr
```
