# HangarWindow::getRelativeScrollHeight
elf 0x14b6ec body 78
Sig: undefined __thiscall getRelativeScrollHeight(HangarWindow * this)

## decompile
```c

/* HangarWindow::getRelativeScrollHeight() */

float __thiscall HangarWindow::getRelativeScrollHeight(HangarWindow *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  
  iVar2 = *(int *)(this + 0xd4);
  iVar3 = *(int *)(this + 0xd8);
  if (iVar2 < iVar3) {
    return DAT_0015b73c;
  }
  iVar1 = *(int *)(this + 0xb4);
  if (iVar1 < 1) {
    if (iVar3 - iVar2 <= iVar1) {
      fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_0015b72c;
    }
    iVar3 = iVar1 + iVar2;
  }
  else {
    iVar3 = iVar3 - iVar1;
  }
  fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
LAB_0015b72c:
  fVar5 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  return fVar5 / fVar4;
}

```

## target disasm
```
  0015b6ec: ldrd r1,r2,[r0,#0xd4]
  0015b6f0: cmp r2,r1
  0015b6f2: ble 0x0015b6fa
  0015b6f4: vldr.32 s0,[pc,#0x44]
  0015b6f8: b 0x0015b734
  0015b6fa: ldr.w r0,[r0,#0xb4]
  0015b6fe: cmp r0,#0x1
  0015b700: blt 0x0015b70a
  0015b702: vmov s0,r1
  0015b706: subs r0,r2,r0
  0015b708: b 0x0015b716
  0015b70a: subs r3,r2,r1
  0015b70c: cmp r0,r3
  0015b70e: bge 0x0015b720
  0015b710: vmov s0,r1
  0015b714: add r0,r1
  0015b716: vcvt.f32.s32 s0,s0
  0015b71a: vmov s2,r0
  0015b71e: b 0x0015b72c
  0015b720: vmov s0,r1
  0015b724: vmov s2,r2
  0015b728: vcvt.f32.s32 s0,s0
  0015b72c: vcvt.f32.s32 s2,s2
  0015b730: vdiv.f32 s0,s2,s0
  0015b734: vmov r0,s0
  0015b738: bx lr
```
