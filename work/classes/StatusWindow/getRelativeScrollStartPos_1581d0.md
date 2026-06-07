# StatusWindow::getRelativeScrollStartPos
elf 0x1581d0 body 42
Sig: undefined __thiscall getRelativeScrollStartPos(StatusWindow * this)

## decompile
```c

/* StatusWindow::getRelativeScrollStartPos() */

float __thiscall StatusWindow::getRelativeScrollStartPos(StatusWindow *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = DAT_001681fc;
  if (*(int *)(this + 0x38) < 1) {
    fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x38),(byte)(in_fpscr >> 0x16) & 3);
    fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x58),(byte)(in_fpscr >> 0x16) & 3);
    fVar1 = -fVar1 / fVar2;
  }
  return fVar1;
}

```

## target disasm
```
  001681d0: ldr r1,[r0,#0x38]
  001681d2: cmp r1,#0x0
  001681d4: ble 0x001681dc
  001681d6: vldr.32 s0,[pc,#0x24]
  001681da: b 0x001681f4
  001681dc: vmov s0,r1
  001681e0: vcvt.f32.s32 s0,s0
  001681e4: vldr.32 s2,[r0,#0x58]
  001681e8: vcvt.f32.s32 s2,s2
  001681ec: vneg.f32 s0,s0
  001681f0: vdiv.f32 s0,s0,s2
  001681f4: vmov r0,s0
  001681f8: bx lr
```
