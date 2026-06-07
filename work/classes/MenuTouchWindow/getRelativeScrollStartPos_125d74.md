# MenuTouchWindow::getRelativeScrollStartPos
elf 0x125d74 body 44
Sig: undefined __thiscall getRelativeScrollStartPos(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::getRelativeScrollStartPos() */

float __thiscall MenuTouchWindow::getRelativeScrollStartPos(MenuTouchWindow *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = DAT_00135da0;
  if (*(int *)(this + 0x194) < 1) {
    fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x194),(byte)(in_fpscr >> 0x16) & 3);
    fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x22c),(byte)(in_fpscr >> 0x16) & 3);
    fVar1 = -fVar1 / fVar2;
  }
  return fVar1;
}

```

## target disasm
```
  00135d74: ldr.w r1,[r0,#0x194]
  00135d78: cmp r1,#0x0
  00135d7a: ble 0x00135d82
  00135d7c: vldr.32 s0,[pc,#0x20]
  00135d80: b 0x00135d9a
  00135d82: vmov s0,r1
  00135d86: vcvt.f32.s32 s0,s0
  00135d8a: vldr.32 s2,[r0,#0x22c]
  00135d8e: vcvt.f32.s32 s2,s2
  00135d92: vneg.f32 s0,s0
  00135d96: vdiv.f32 s0,s0,s2
  00135d9a: vmov r0,s0
  00135d9e: bx lr
```
