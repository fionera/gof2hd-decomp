# StatusWindow::getRelativeScrollHeight
elf 0x158200 body 76
Sig: undefined __thiscall getRelativeScrollHeight(StatusWindow * this)

## decompile
```c

/* StatusWindow::getRelativeScrollHeight() */

float __thiscall StatusWindow::getRelativeScrollHeight(StatusWindow *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  
  iVar2 = *(int *)(this + 0x58);
  iVar3 = *(int *)(this + 0x5c);
  if (iVar2 < iVar3) {
    return DAT_0016824c;
  }
  iVar1 = *(int *)(this + 0x38);
  if (iVar1 < 1) {
    if (iVar3 - iVar2 <= iVar1) {
      fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_0016823e;
    }
    iVar3 = iVar1 + iVar2;
  }
  else {
    iVar3 = iVar3 - iVar1;
  }
  fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
LAB_0016823e:
  fVar5 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  return fVar5 / fVar4;
}

```

## target disasm
```
  00168200: ldrd r1,r2,[r0,#0x58]
  00168204: cmp r2,r1
  00168206: ble 0x0016820e
  00168208: vldr.32 s0,[pc,#0x40]
  0016820c: b 0x00168246
  0016820e: ldr r0,[r0,#0x38]
  00168210: cmp r0,#0x1
  00168212: blt 0x0016821c
  00168214: vmov s0,r1
  00168218: subs r0,r2,r0
  0016821a: b 0x00168228
  0016821c: subs r3,r2,r1
  0016821e: cmp r0,r3
  00168220: bge 0x00168232
  00168222: vmov s0,r1
  00168226: add r0,r1
  00168228: vcvt.f32.s32 s0,s0
  0016822c: vmov s2,r0
  00168230: b 0x0016823e
  00168232: vmov s0,r1
  00168236: vmov s2,r2
  0016823a: vcvt.f32.s32 s0,s0
  0016823e: vcvt.f32.s32 s2,s2
  00168242: vdiv.f32 s0,s2,s0
  00168246: vmov r0,s0
  0016824a: bx lr
```
