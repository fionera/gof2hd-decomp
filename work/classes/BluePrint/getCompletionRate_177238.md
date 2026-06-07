# BluePrint::getCompletionRate
elf 0x177238 body 92
Sig: undefined __thiscall getCompletionRate(BluePrint * this)

## decompile
```c

/* BluePrint::getCompletionRate() */

float __thiscall BluePrint::getCompletionRate(BluePrint *this)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  piVar2 = (int *)getQuantityList(this);
  iVar3 = 0;
  fVar5 = DAT_00187294;
  while (*piVar2 != iVar3) {
    iVar4 = *(int *)(piVar2[1] + iVar3 * 4);
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + 1;
    fVar6 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)VectorSignedToFloat(iVar4 - *(int *)((*(undefined4 **)this)[1] + iVar1),
                                       (byte)(in_fpscr >> 0x16) & 3);
    fVar8 = (float)VectorUnsignedToFloat(**(undefined4 **)this,(byte)(in_fpscr >> 0x16) & 3);
    fVar5 = fVar5 + (fVar7 / fVar6) / fVar8;
  }
  return fVar5;
}

```

## target disasm
```
  00187238: push {r4,r5,r6,r7,lr}
  0018723a: add r7,sp,#0xc
  0018723c: push.w r11
  00187240: mov r6,r0
  00187242: blx 0x00073960
  00187246: ldr r1,[r0,#0x0]
  00187248: movs r2,#0x0
  0018724a: vldr.32 s0,[pc,#0x48]
  0018724e: b 0x00187286
  00187250: ldr r3,[r6,#0x0]
  00187252: ldr r4,[r0,#0x4]
  00187254: ldr r5,[r3,#0x4]
  00187256: ldr.w r4,[r4,r2,lsl #0x2]
  0018725a: ldr.w r5,[r5,r2,lsl #0x2]
  0018725e: adds r2,#0x1
  00187260: vmov s2,r4
  00187264: subs r5,r4,r5
  00187266: vcvt.f32.s32 s2,s2
  0018726a: vmov s4,r5
  0018726e: vcvt.f32.s32 s4,s4
  00187272: vdiv.f32 s2,s4,s2
  00187276: vldr.32 s4,[r3]
  0018727a: vcvt.f32.u32 s4,s4
  0018727e: vdiv.f32 s2,s2,s4
  00187282: vadd.f32 s0,s0,s2
  00187286: cmp r1,r2
  00187288: bne 0x00187250
  0018728a: vmov r0,s0
  0018728e: pop.w r11
  00187292: pop {r4,r5,r6,r7,pc}
```
