# MenuTouchWindow::getRelativeScrollHeight
elf 0x125da4 body 78
Sig: undefined __thiscall getRelativeScrollHeight(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::getRelativeScrollHeight() */

float __thiscall MenuTouchWindow::getRelativeScrollHeight(MenuTouchWindow *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  
  iVar2 = *(int *)(this + 0x228);
  iVar3 = *(int *)(this + 0x22c);
  if (iVar3 < iVar2) {
    return DAT_00135df4;
  }
  iVar1 = *(int *)(this + 0x194);
  if (iVar1 < 1) {
    if (iVar2 - iVar3 <= iVar1) {
      fVar4 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_00135de4;
    }
    iVar2 = iVar1 + iVar3;
  }
  else {
    iVar2 = iVar2 - iVar1;
  }
  fVar4 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
LAB_00135de4:
  fVar5 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
  return fVar5 / fVar4;
}

```

## target disasm
```
  00135da4: ldrd r1,r2,[r0,#0x228]
  00135da8: cmp r1,r2
  00135daa: ble 0x00135db2
  00135dac: vldr.32 s0,[pc,#0x44]
  00135db0: b 0x00135dec
  00135db2: ldr.w r0,[r0,#0x194]
  00135db6: cmp r0,#0x1
  00135db8: blt 0x00135dc2
  00135dba: vmov s0,r2
  00135dbe: subs r0,r1,r0
  00135dc0: b 0x00135dce
  00135dc2: subs r3,r1,r2
  00135dc4: cmp r0,r3
  00135dc6: bge 0x00135dd8
  00135dc8: vmov s0,r2
  00135dcc: add r0,r2
  00135dce: vcvt.f32.s32 s0,s0
  00135dd2: vmov s2,r0
  00135dd6: b 0x00135de4
  00135dd8: vmov s0,r2
  00135ddc: vmov s2,r1
  00135de0: vcvt.f32.s32 s0,s0
  00135de4: vcvt.f32.s32 s2,s2
  00135de8: vdiv.f32 s0,s2,s0
  00135dec: vmov r0,s0
  00135df0: bx lr
```
