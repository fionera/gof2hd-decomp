# Radar::update
elf 0x12de60 body 222
Sig: undefined __stdcall update(Vector param_1)

## decompile
```c

/* Radar::update(AbyssEngine::AEMath::Vector) */

void Radar::update(int param_1,float param_2,float param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  int iVar2;
  int *piVar3;
  Matrix aMStack_3c [12];
  float local_30;
  float local_2c;
  undefined4 local_28;
  int local_24;
  
  piVar3 = *(int **)(DAT_0013df40 + 0x13de78);
  local_24 = *piVar3;
  local_30 = param_2;
  local_2c = param_3;
  local_28 = param_4;
  AbyssEngine::AEMath::MatrixTransformVector(aMStack_3c,(Vector *)(param_1 + 0x1d0));
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x154),(Vector *)aMStack_3c);
  puVar1 = *(undefined4 **)(DAT_0013df44 + 0x13deb6);
  *(float *)(param_1 + 0x158) = -*(float *)(param_1 + 0x158);
  *(float *)(param_1 + 0x15c) = -*(float *)(param_1 + 0x15c);
  iVar2 = AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar1,(Vector *)&local_30);
  *(char *)(param_1 + 0x11c) = (char)iVar2;
  *(int *)(param_1 + 0xfc) = (int)local_30;
  *(int *)(param_1 + 0x100) = (int)local_2c;
  if (iVar2 == 0) {
    elipsoidIntersect((Vector *)aMStack_3c,param_1,(int)local_30,(int)local_2c,
                      *(undefined4 *)(param_1 + 0x154),*(undefined4 *)(param_1 + 0x158),
                      *(undefined4 *)(param_1 + 0x15c));
    AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x154),(Vector *)aMStack_3c);
    *(int *)(param_1 + 0xfc) = (int)*(float *)(param_1 + 0x154);
    *(int *)(param_1 + 0x100) = (int)*(float *)(param_1 + 0x158);
  }
  if (*piVar3 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0013de60: push {r4,r5,r6,r7,lr}
  0013de62: add r7,sp,#0xc
  0013de64: push {r8,r9,r11}
  0013de68: sub sp,#0x28
  0013de6a: mov r4,r0
  0013de6c: ldr r0,[0x0013df40]
  0013de6e: strd r1,r2,[sp,#0x18]
  0013de72: add r5,sp,#0x18
  0013de74: add r0,pc
  0013de76: add r6,sp,#0xc
  0013de78: add.w r1,r4,#0x1d0
  0013de7c: mov r2,r5
  0013de7e: ldr.w r9,[r0,#0x0]
  0013de82: ldr.w r0,[r9,#0x0]
  0013de86: str r3,[sp,#0x20]
  0013de88: str r0,[sp,#0x24]
  0013de8a: mov r0,r6
  0013de8c: blx 0x0006f688
  0013de90: add.w r8,r4,#0x154
  0013de94: mov r1,r6
  0013de96: mov r0,r8
  0013de98: blx 0x0006eb3c
  0013de9c: vldr.32 s0,[r4,#0x158]
  0013dea0: mov r1,r5
  0013dea2: vldr.32 s2,[r4,#0x15c]
  0013dea6: mov r2,r5
  0013dea8: vneg.f32 s0,s0
  0013deac: ldr r0,[0x0013df44]
  0013deae: vneg.f32 s2,s2
  0013deb2: add r0,pc
  0013deb4: ldr r0,[r0,#0x0]
  0013deb6: vstr.32 s0,[r4,#0x158]
  0013deba: vstr.32 s2,[r4,#0x15c]
  0013debe: ldr r0,[r0,#0x0]
  0013dec0: blx 0x000729c4
  0013dec4: strb.w r0,[r4,#0x11c]
  0013dec8: cmp r0,#0x0
  0013deca: vldr.32 s0,[sp,#0x18]
  0013dece: vcvt.s32.f32 s2,s0
  0013ded2: vstr.32 s2,[r4,#0xfc]
  0013ded6: vldr.32 s2,[sp,#0x1c]
  0013deda: vcvt.s32.f32 s4,s2
  0013dede: vcvt.s32.f32 s0,s0
  0013dee2: vcvt.s32.f32 s2,s2
  0013dee6: vstr.32 s4,[r4,#0x100]
  0013deea: bne 0x0013df28
  0013deec: vmov r2,s0
  0013def0: add.w r6,r4,#0x154
  0013def4: vmov r3,s2
  0013def8: add r5,sp,#0xc
  0013defa: ldmia r6,{r0,r1,r6}
  0013defc: stm sp,{r0,r1,r6}
  0013df00: mov r0,r5
  0013df02: mov r1,r4
  0013df04: blx 0x00076774
  0013df08: mov r0,r8
  0013df0a: mov r1,r5
  0013df0c: blx 0x0006eb3c
  0013df10: vldr.32 s0,[r4,#0x154]
  0013df14: vldr.32 s2,[r4,#0x158]
  0013df18: vcvt.s32.f32 s0,s0
  0013df1c: vcvt.s32.f32 s2,s2
  0013df20: vstr.32 s0,[r4,#0xfc]
  0013df24: vstr.32 s2,[r4,#0x100]
  0013df28: ldr r0,[sp,#0x24]
  0013df2a: ldr.w r1,[r9,#0x0]
  0013df2e: subs r0,r1,r0
  0013df30: ittt eq
  0013df32: add.eq sp,#0x28
  0013df34: pop.eq.w {r8,r9,r11}
  0013df38: pop.eq {r4,r5,r6,r7,pc}
  0013df3a: blx 0x0006e824
```
