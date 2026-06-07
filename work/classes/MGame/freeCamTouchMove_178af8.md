# MGame::freeCamTouchMove
elf 0x178af8 body 552
Sig: undefined __stdcall freeCamTouchMove(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::freeCamTouchMove(int, int, void*) */

void MGame::freeCamTouchMove(int param_1,int param_2,void *param_3)

{
  float fVar1;
  int iVar2;
  Vector *this;
  int iVar3;
  int in_r3;
  int *piVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  undefined4 uVar7;
  undefined8 in_d4;
  ulonglong in_d5;
  undefined8 in_d6;
  undefined4 in_s14;
  uint uVar8;
  undefined4 uVar9;
  undefined8 local_60;
  ulonglong local_58;
  undefined8 local_50;
  undefined4 local_48;
  int local_44;
  
  piVar4 = *(int **)(DAT_00188d2c + 0x188b10);
  local_44 = *piVar4;
  local_60 = in_d4;
  local_58 = in_d5;
  local_50 = in_d6;
  local_48 = in_s14;
  iVar2 = PlayerEgo::isMining();
  if (iVar2 != 0) {
    *(undefined1 *)(param_1 + 0x111) = 1;
    if (*piVar4 - local_44 == 0) {
      (*(code *)(DAT_001ac7a4 + 0x1ac7a8))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*piVar4 - local_44);
  }
  *(undefined1 *)(param_1 + 0x111) = 0;
  if ((*(int *)(param_1 + 0x98) == 0) || (*(int *)(param_1 + 0x9c) == 0)) {
    iVar3 = (int)param_3 - *(int *)(param_1 + 0x128);
    iVar2 = param_2 - *(int *)(param_1 + 0x124);
    fVar5 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    fVar6 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(param_1 + 0x124) = param_2;
    *(void **)(param_1 + 0x128) = param_3;
    *(int *)(param_1 + 0x134) = iVar2;
    *(int *)(param_1 + 0x138) = iVar3;
    *(undefined4 *)(param_1 + 0x13c) = 0x3f800000;
    *(undefined4 *)(param_1 + 0x140) = 0x3f800000;
    *(float *)(param_1 + 0x118) = *(float *)(param_1 + 0x118) + fVar5;
    *(float *)(param_1 + 0x11c) = *(float *)(param_1 + 0x11c) + fVar6;
    if (*(int *)(param_1 + 0x98) == 0) {
      uVar9 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
      uVar7 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      iVar2 = 0xb0;
    }
    else {
      if (*(int *)(param_1 + 0x9c) != 0) goto LAB_00188bb6;
      uVar9 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
      uVar7 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      iVar2 = 0xa4;
    }
    local_48 = 0;
    local_50 = CONCAT44(uVar9,uVar7);
    AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + iVar2),(Vector *)&local_50);
    goto LAB_00188d06;
  }
LAB_00188bb6:
  local_60 = CONCAT44(local_60._4_4_,piVar4);
  this = (Vector *)(param_1 + 0xa4);
  AbyssEngine::AEMath::operator-((Vector *)&local_50,this);
  fVar5 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_50);
  if (*(int *)(param_1 + 0x98) == in_r3) {
    uVar8 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    uVar9 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    local_58 = (ulonglong)uVar8;
    local_60 = CONCAT44(uVar9,(int *)local_60);
    AbyssEngine::AEMath::operator-((Vector *)&local_50,(Vector *)((int)&local_60 + 4));
    fVar6 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_50);
    local_50 = CONCAT44(uVar8,uVar9);
LAB_00188c76:
    local_48 = 0;
    AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_50);
  }
  else {
    fVar6 = fVar5;
    if (*(int *)(param_1 + 0x9c) == in_r3) {
      uVar8 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
      uVar9 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      local_58 = (ulonglong)uVar8;
      local_60 = CONCAT44(uVar9,(int *)local_60);
      AbyssEngine::AEMath::operator-((Vector *)&local_50,(Vector *)((int)&local_60 + 4));
      fVar6 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_50);
      local_50 = CONCAT44(uVar8,uVar9);
      this = (Vector *)(param_1 + 0xb0);
      goto LAB_00188c76;
    }
  }
  fVar1 = DAT_00188d24;
  piVar4 = (int *)local_60;
  fVar5 = *(float *)(param_1 + 0xbc) + (fVar6 - fVar5) * DAT_00188d20;
  *(float *)(param_1 + 0xbc) = fVar5;
  if ((fVar1 < fVar5) || (fVar1 = DAT_00188d28, (int)((uint)(fVar5 < DAT_00188d28) << 0x1f) < 0)) {
    fVar5 = fVar1;
    *(float *)(param_1 + 0xbc) = fVar5;
  }
  TargetFollowCamera::zoomTarget(fVar5);
LAB_00188d06:
  if (*piVar4 == local_44) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00188af8: push {r4,r5,r6,r7,lr}
  00188afa: add r7,sp,#0xc
  00188afc: push {r7,r8,r9,r10,r11}
  00188b00: vpush {d3,d4,d5,d6,d7,d8,d9,d10}
  00188b04: mov r4,r0
  00188b06: ldr r0,[0x00188d2c]
  00188b08: mov r9,r3
  00188b0a: mov r5,r2
  00188b0c: add r0,pc
  00188b0e: mov r8,r1
  00188b10: ldr r6,[r0,#0x0]
  00188b12: ldr r0,[r6,#0x0]
  00188b14: str r0,[sp,#0x24]
  00188b16: ldr r0,[r4,#0x58]
  00188b18: blx 0x00077adc
  00188b1c: cbz r0,0x00188b48
  00188b1e: movs r0,#0x1
  00188b20: strb.w r0,[r4,#0x111]
  00188b24: ldr r0,[sp,#0x24]
  00188b26: ldr r1,[r6,#0x0]
  00188b28: subs r0,r1,r0
  00188b2a: itttt eq
  00188b2c: mov.eq r0,r4
  00188b2e: mov.eq r3,r9
  00188b30: vpop.eq {d3,d4,d5,d6,d7,d8,d9,d10}
  00188b34: add.eq sp,#0x4
  00188b36: ittt eq
  00188b38: pop.eq.w {r8,r9,r10,r11}
  00188b3c: pop.eq.w {r4,r5,r6,r7,lr}
  00188b40: b.eq.w 0x001ac798
  00188b44: blx 0x0006e824
  00188b48: ldr.w r0,[r4,#0x98]
  00188b4c: movs r1,#0x0
  00188b4e: strb.w r1,[r4,#0x111]
  00188b52: cbz r0,0x00188b5c
  00188b54: mov r10,r4
  00188b56: ldr.w r1,[r10,#0x9c]!
  00188b5a: cbnz r1,0x00188bb6
  00188b5c: ldrd r1,r2,[r4,#0x124]
  00188b60: cmp r0,#0x0
  00188b62: sub.w r2,r5,r2
  00188b66: mov.w r3,#0x3f800000
  00188b6a: sub.w r1,r8,r1
  00188b6e: add.w r12,r4,#0x134
  00188b72: vmov s2,r2
  00188b76: vmov s0,r1
  00188b7a: vcvt.f32.s32 s0,s0
  00188b7e: vcvt.f32.s32 s2,s2
  00188b82: vldr.32 s4,[r4,#0x118]
  00188b86: vldr.32 s6,[r4,#0x11c]
  00188b8a: strd r8,r5,[r4,#0x124]
  00188b8e: stm.w r12!,{r1,r2,r3}
  00188b92: str.w r3,[r4,#0x140]
  00188b96: vadd.f32 s0,s4,s0
  00188b9a: vadd.f32 s2,s6,s2
  00188b9e: vstr.32 s0,[r4,#0x118]
  00188ba2: vstr.32 s2,[r4,#0x11c]
  00188ba6: beq.w 0x00188cc6
  00188baa: mov r10,r4
  00188bac: ldr.w r0,[r10,#0x9c]!
  00188bb0: cmp r0,#0x0
  00188bb2: beq.w 0x00188ce0
  00188bb6: str r6,[sp,#0x8]
  00188bb8: add.w r6,r4,#0xb0
  00188bbc: add.w r11,sp,#0x18
  00188bc0: add.w r1,r4,#0xa4
  00188bc4: mov r2,r6
  00188bc6: str r1,[sp,#0x4]
  00188bc8: mov r0,r11
  00188bca: blx 0x0006ec38
  00188bce: mov r0,r11
  00188bd0: blx 0x0006ec44
  00188bd4: vmov s16,r0
  00188bd8: ldr.w r0,[r4,#0x98]
  00188bdc: cmp r0,r9
  00188bde: beq 0x00188c32
  00188be0: vmov.f32 s0,s16
  00188be4: ldr.w r0,[r10,#0x0]
  00188be8: ldr r2,[sp,#0x4]
  00188bea: cmp r0,r9
  00188bec: bne 0x00188c7e
  00188bee: vmov s0,r5
  00188bf2: add r5,sp,#0x18
  00188bf4: add r1,sp,#0xc
  00188bf6: vcvt.f32.s32 s18,s0
  00188bfa: mov r0,r5
  00188bfc: vmov s0,r8
  00188c00: mov.w r8,#0x0
  00188c04: vcvt.f32.s32 s20,s0
  00188c08: str.w r8,[sp,#0x14]
  00188c0c: vstr.32 s18,[sp,#0x10]
  00188c10: vstr.32 s20,[sp,#0xc]
  00188c14: blx 0x0006ec38
  00188c18: mov r0,r5
  00188c1a: blx 0x0006ec44
  00188c1e: add r1,sp,#0x18
  00188c20: mov r5,r0
  00188c22: mov r0,r6
  00188c24: vstr.32 s18,[sp,#0x1c]
  00188c28: vstr.32 s20,[sp,#0x18]
  00188c2c: str.w r8,[sp,#0x20]
  00188c30: b 0x00188c76
  00188c32: vmov s0,r5
  00188c36: add r5,sp,#0x18
  00188c38: add r1,sp,#0xc
  00188c3a: mov r2,r6
  00188c3c: vcvt.f32.s32 s18,s0
  00188c40: mov r0,r5
  00188c42: vmov s0,r8
  00188c46: mov.w r8,#0x0
  00188c4a: vcvt.f32.s32 s20,s0
  00188c4e: str.w r8,[sp,#0x14]
  00188c52: vstr.32 s18,[sp,#0x10]
  00188c56: vstr.32 s20,[sp,#0xc]
  00188c5a: blx 0x0006ec38
  00188c5e: mov r0,r5
  00188c60: blx 0x0006ec44
  00188c64: mov r5,r0
  00188c66: vstr.32 s18,[sp,#0x1c]
  00188c6a: vstr.32 s20,[sp,#0x18]
  00188c6e: add r1,sp,#0x18
  00188c70: str.w r8,[sp,#0x20]
  00188c74: ldr r0,[sp,#0x4]
  00188c76: blx 0x0006eb3c
  00188c7a: vmov s0,r5
  00188c7e: vsub.f32 s2,s0,s16
  00188c82: vldr.32 s4,[pc,#0x9c]
  00188c86: vldr.32 s0,[r4,#0xbc]
  00188c8a: ldr r6,[sp,#0x8]
  00188c8c: vmla.f32 s0,s2,s4
  00188c90: vldr.32 s2,[pc,#0x90]
  00188c94: vcmpe.f32 s0,s2
  00188c98: vstr.32 s0,[r4,#0xbc]
  00188c9c: vmrs apsr,fpscr
  00188ca0: bgt 0x00188cb0
  00188ca2: vldr.32 s2,[pc,#0x84]
  00188ca6: vcmpe.f32 s0,s2
  00188caa: vmrs apsr,fpscr
  00188cae: bpl 0x00188cb8
  00188cb0: vmov.f32 s0,s2
  00188cb4: vstr.32 s2,[r4,#0xbc]
  00188cb8: vmov r1,s0
  00188cbc: ldr.w r0,[r4,#0xf4]
  00188cc0: blx 0x00078178
  00188cc4: b 0x00188d06
  00188cc6: vmov s0,r5
  00188cca: movs r0,#0x0
  00188ccc: vmov s2,r8
  00188cd0: vcvt.f32.s32 s0,s0
  00188cd4: vcvt.f32.s32 s2,s2
  00188cd8: str r0,[sp,#0x20]
  00188cda: add.w r0,r4,#0xb0
  00188cde: b 0x00188cf8
  00188ce0: vmov s0,r5
  00188ce4: movs r0,#0x0
  00188ce6: vmov s2,r8
  00188cea: vcvt.f32.s32 s0,s0
  00188cee: vcvt.f32.s32 s2,s2
  00188cf2: str r0,[sp,#0x20]
  00188cf4: add.w r0,r4,#0xa4
  00188cf8: vstr.32 s0,[sp,#0x1c]
  00188cfc: vstr.32 s2,[sp,#0x18]
  00188d00: add r1,sp,#0x18
  00188d02: blx 0x0006eb3c
  00188d06: ldr r0,[sp,#0x24]
  00188d08: ldr r1,[r6,#0x0]
  00188d0a: subs r0,r1,r0
  00188d0c: itttt eq
  00188d0e: vpop.eq {d3,d4,d5,d6,d7,d8,d9,d10}
  00188d12: add.eq sp,#0x4
  00188d14: pop.eq.w {r8,r9,r10,r11}
  00188d18: pop.eq {r4,r5,r6,r7,pc}
  00188d1a: blx 0x0006e824
  001ac798: bx pc
```
