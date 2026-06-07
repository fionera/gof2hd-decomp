# PaintCanvas::CameraGetCurrentFactor1
elf 0x7b97a body 36
Sig: undefined __thiscall CameraGetCurrentFactor1(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraGetCurrentFactor1() */

undefined4 __thiscall AbyssEngine::PaintCanvas::CameraGetCurrentFactor1(PaintCanvas *this)

{
  undefined4 uVar1;
  
  if (*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) {
    uVar1 = *(undefined4 *)(*(int *)(*(int *)(this + 0x168) + *(uint *)(this + 0x170) * 4) + 0x48);
  }
  else {
    uVar1 = 0x3f800000;
  }
  return uVar1;
}

```

## target disasm
```
  0008b97a: ldr.w r2,[r0,#0x164]
  0008b97e: ldr.w r1,[r0,#0x170]
  0008b982: cmp r1,r2
  0008b984: bcs 0x0008b994
  0008b986: ldr.w r0,[r0,#0x168]
  0008b98a: ldr.w r0,[r0,r1,lsl #0x2]
  0008b98e: vldr.32 s0,[r0,#0x48]
  0008b992: b 0x0008b998
  0008b994: vmov.f32 s0,0x3f800000
  0008b998: vmov r0,s0
  0008b99c: bx lr
```
