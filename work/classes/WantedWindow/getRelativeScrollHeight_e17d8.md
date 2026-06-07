# WantedWindow::getRelativeScrollHeight
elf 0xe17d8 body 78
Sig: undefined __thiscall getRelativeScrollHeight(WantedWindow * this)

## decompile
```c

/* WantedWindow::getRelativeScrollHeight() */

float __thiscall WantedWindow::getRelativeScrollHeight(WantedWindow *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  
  iVar2 = *(int *)(this + 0xa4);
  iVar3 = *(int *)(this + 0xa8);
  if (iVar2 < iVar3) {
    return DAT_000f1828;
  }
  iVar1 = *(int *)(this + 0x84);
  if (iVar1 < 1) {
    if (iVar3 - iVar2 <= iVar1) {
      fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_000f1818;
    }
    iVar3 = iVar1 + iVar2;
  }
  else {
    iVar3 = iVar3 - iVar1;
  }
  fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
LAB_000f1818:
  fVar5 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  return fVar5 / fVar4;
}

```

## target disasm
```
  000f17d8: ldrd r1,r2,[r0,#0xa4]
  000f17dc: cmp r2,r1
  000f17de: ble 0x000f17e6
  000f17e0: vldr.32 s0,[pc,#0x44]
  000f17e4: b 0x000f1820
  000f17e6: ldr.w r0,[r0,#0x84]
  000f17ea: cmp r0,#0x1
  000f17ec: blt 0x000f17f6
  000f17ee: vmov s0,r1
  000f17f2: subs r0,r2,r0
  000f17f4: b 0x000f1802
  000f17f6: subs r3,r2,r1
  000f17f8: cmp r0,r3
  000f17fa: bge 0x000f180c
  000f17fc: vmov s0,r1
  000f1800: add r0,r1
  000f1802: vcvt.f32.s32 s0,s0
  000f1806: vmov s2,r0
  000f180a: b 0x000f1818
  000f180c: vmov s0,r1
  000f1810: vmov s2,r2
  000f1814: vcvt.f32.s32 s0,s0
  000f1818: vcvt.f32.s32 s2,s2
  000f181c: vdiv.f32 s0,s2,s0
  000f1820: vmov r0,s0
  000f1824: bx lr
```
