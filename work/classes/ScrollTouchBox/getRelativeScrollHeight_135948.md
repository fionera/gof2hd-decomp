# ScrollTouchBox::getRelativeScrollHeight
elf 0x135948 body 76
Sig: undefined __thiscall getRelativeScrollHeight(ScrollTouchBox * this)

## decompile
```c

/* ScrollTouchBox::getRelativeScrollHeight() */

float __thiscall ScrollTouchBox::getRelativeScrollHeight(ScrollTouchBox *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  
  iVar2 = *(int *)(this + 0x10);
  iVar3 = *(int *)(this + 0x18);
  if (iVar3 < iVar2) {
    return DAT_00145994;
  }
  iVar1 = *(int *)(this + 0x34);
  if (iVar1 < 1) {
    if (iVar2 - iVar3 <= iVar1) {
      fVar4 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_00145986;
    }
    iVar2 = iVar1 + iVar3;
  }
  else {
    iVar2 = iVar2 - iVar1;
  }
  fVar4 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
LAB_00145986:
  fVar5 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
  return fVar5 / fVar4;
}

```

## target disasm
```
  00145948: ldr r1,[r0,#0x10]
  0014594a: ldr r2,[r0,#0x18]
  0014594c: cmp r1,r2
  0014594e: ble 0x00145956
  00145950: vldr.32 s0,[pc,#0x40]
  00145954: b 0x0014598e
  00145956: ldr r0,[r0,#0x34]
  00145958: cmp r0,#0x1
  0014595a: blt 0x00145964
  0014595c: vmov s0,r2
  00145960: subs r0,r1,r0
  00145962: b 0x00145970
  00145964: subs r3,r1,r2
  00145966: cmp r0,r3
  00145968: bge 0x0014597a
  0014596a: vmov s0,r2
  0014596e: add r0,r2
  00145970: vcvt.f32.s32 s0,s0
  00145974: vmov s2,r0
  00145978: b 0x00145986
  0014597a: vmov s0,r2
  0014597e: vmov s2,r1
  00145982: vcvt.f32.s32 s0,s0
  00145986: vcvt.f32.s32 s2,s2
  0014598a: vdiv.f32 s0,s2,s0
  0014598e: vmov r0,s0
  00145992: bx lr
```
