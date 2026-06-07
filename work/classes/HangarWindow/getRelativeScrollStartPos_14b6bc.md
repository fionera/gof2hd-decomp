# HangarWindow::getRelativeScrollStartPos
elf 0x14b6bc body 44
Sig: undefined __thiscall getRelativeScrollStartPos(HangarWindow * this)

## decompile
```c

/* HangarWindow::getRelativeScrollStartPos() */

float __thiscall HangarWindow::getRelativeScrollStartPos(HangarWindow *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = DAT_0015b6e8;
  if (*(int *)(this + 0xb4) < 1) {
    fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0xb4),(byte)(in_fpscr >> 0x16) & 3);
    fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0xd4),(byte)(in_fpscr >> 0x16) & 3);
    fVar1 = -fVar1 / fVar2;
  }
  return fVar1;
}

```

## target disasm
```
  0015b6bc: ldr.w r1,[r0,#0xb4]
  0015b6c0: cmp r1,#0x0
  0015b6c2: ble 0x0015b6ca
  0015b6c4: vldr.32 s0,[pc,#0x20]
  0015b6c8: b 0x0015b6e2
  0015b6ca: vmov s0,r1
  0015b6ce: vcvt.f32.s32 s0,s0
  0015b6d2: vldr.32 s2,[r0,#0xd4]
  0015b6d6: vcvt.f32.s32 s2,s2
  0015b6da: vneg.f32 s0,s0
  0015b6de: vdiv.f32 s0,s0,s2
  0015b6e2: vmov r0,s0
  0015b6e6: bx lr
```
