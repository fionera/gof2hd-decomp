# Trail::update
elf 0x15bfac body 462
Sig: undefined __stdcall update(float param_1, float param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* Trail::update(float, float, float, float, float, float) */

void Trail::update(float param_1,float param_2,float param_3,float param_4,float param_5,
                  float param_6)

{
  int in_r0;
  int *piVar1;
  float in_r1;
  int *piVar2;
  float in_r2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  float in_r3;
  int iVar6;
  int *piVar7;
  uint *puVar8;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float fVar9;
  float fVar10;
  float extraout_s2;
  float in_stack_00000000;
  float in_stack_00000004;
  float in_stack_00000008;
  Matrix aMStack_70 [60];
  int local_34;
  
  piVar7 = *(int **)(DAT_0016c17c + 0x16bfca);
  local_34 = *piVar7;
  fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
  piVar1 = *(int **)(in_r0 + 0x18);
  *piVar1 = (int)(in_r1 - fVar9);
  piVar1[1] = (int)in_r2;
  piVar1[2] = (int)in_r3;
  fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
  piVar1[3] = (int)(fVar9 + in_r1);
  piVar1[4] = (int)in_r2;
  piVar1[5] = (int)in_r3;
  fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
  piVar1[6] = (int)(in_stack_00000000 - fVar9);
  piVar1[7] = (int)in_stack_00000004;
  piVar1[8] = (int)in_stack_00000008;
  fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
  piVar1[9] = (int)(fVar9 + in_stack_00000000);
  piVar1[10] = (int)in_stack_00000004;
  piVar1[0xb] = (int)in_stack_00000008;
  piVar2 = piVar1 + *(int *)(in_r0 + 0x20);
  for (iVar3 = *(int *)(in_r0 + 0x20) + -1; 10 < iVar3; iVar3 = iVar3 + -6) {
    piVar2[-2] = piVar2[-8];
    piVar2[-1] = piVar2[-7];
    *(undefined8 *)(piVar2 + -6) = *(undefined8 *)(piVar2 + -0xc);
    *(undefined8 *)(piVar2 + -4) = *(undefined8 *)(piVar2 + -10);
    piVar2 = piVar2 + -6;
  }
  iVar3 = 0;
  iVar4 = 0;
  while (iVar4 < *(int *)(in_r0 + 0x20)) {
    puVar5 = (undefined4 *)((int)piVar1 + iVar3);
    iVar4 = iVar4 + 3;
    fVar9 = (float)VectorSignedToFloat(*puVar5,(byte)(in_fpscr >> 0x16) & 3);
    iVar6 = *(int *)(in_r0 + 0x1c);
    *(int *)(iVar6 + iVar3) = (int)(fVar9 - in_r1);
    iVar6 = iVar6 + iVar3;
    iVar3 = iVar3 + 0xc;
    fVar9 = (float)VectorSignedToFloat(puVar5[1],(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(iVar6 + 4) = (int)(fVar9 - in_r2);
    fVar9 = (float)VectorSignedToFloat(puVar5[2],(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(iVar6 + 8) = (int)(fVar9 - in_r3);
  }
  iVar3 = 0;
  puVar8 = *(uint **)(DAT_0016c180 + 0x16c100);
  for (iVar4 = 0; iVar4 < (*(int *)(in_r0 + 0x24) + 1) * 2; iVar4 = iVar4 + 1) {
    puVar5 = (undefined4 *)(*(int *)(in_r0 + 0x1c) + iVar3);
    VectorSignedToFloat(*puVar5,(byte)(in_fpscr >> 0x16) & 3);
    fVar9 = (float)VectorSignedToFloat(puVar5[2],(byte)(in_fpscr >> 0x16) & 3);
    fVar10 = (float)VectorSignedToFloat(puVar5[1],(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::PaintCanvas::MeshSetPoint
              (*puVar8,(ushort)*(undefined4 *)(in_r0 + 0x10),fVar9,param_2,fVar10);
    iVar3 = iVar3 + 0xc;
    param_2 = extraout_s1;
  }
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar8);
  AbyssEngine::AEMath::MatrixSetTranslation(aMStack_70,extraout_s0,extraout_s1_00,extraout_s2);
  if (*piVar7 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016bfac: push {r4,r5,r6,r7,lr}
  0016bfae: add r7,sp,#0xc
  0016bfb0: push {r7,r8,r9,r10,r11}
  0016bfb4: vpush {d8}
  0016bfb8: sub sp,#0x48
  0016bfba: mov r6,r0
  0016bfbc: ldr r0,[0x0016c17c]
  0016bfbe: vmov s16,r3
  0016bfc2: vldr.32 s10,[r7,#0x10]
  0016bfc6: add r0,pc
  0016bfc8: mov r9,r2
  0016bfca: mov r10,r1
  0016bfcc: mvn r12,#0x17
  0016bfd0: ldr.w r8,[r0,#0x0]
  0016bfd4: ldr.w r0,[r8,#0x0]
  0016bfd8: str r0,[sp,#0x44]
  0016bfda: vldr.32 s0,[r6,#0xc]
  0016bfde: vcvt.f32.s32 s2,s0
  0016bfe2: ldr r0,[r6,#0x18]
  0016bfe4: vmov s0,r1
  0016bfe8: vsub.f32 s4,s0,s2
  0016bfec: vmov s2,r2
  0016bff0: vcvt.s32.f32 s6,s2
  0016bff4: vcvt.s32.f32 s8,s16
  0016bff8: vcvt.s32.f32 s4,s4
  0016bffc: vstr.32 s4,[r0]
  0016c000: vstr.32 s6,[r0,#0x4]
  0016c004: vstr.32 s8,[r0,#0x8]
  0016c008: vldr.32 s4,[r6,#0xc]
  0016c00c: vcvt.f32.s32 s4,s4
  0016c010: vcvt.s32.f32 s6,s2
  0016c014: vcvt.s32.f32 s8,s16
  0016c018: vadd.f32 s4,s4,s0
  0016c01c: vcvt.s32.f32 s4,s4
  0016c020: vstr.32 s4,[r0,#0xc]
  0016c024: vstr.32 s6,[r0,#0x10]
  0016c028: vstr.32 s8,[r0,#0x14]
  0016c02c: vldr.32 s4,[r6,#0xc]
  0016c030: vldr.32 s6,[r7,#0x8]
  0016c034: vcvt.f32.s32 s4,s4
  0016c038: vldr.32 s8,[r7,#0xc]
  0016c03c: vcvt.s32.f32 s12,s8
  0016c040: vcvt.s32.f32 s14,s10
  0016c044: vsub.f32 s4,s6,s4
  0016c048: vcvt.s32.f32 s4,s4
  0016c04c: vstr.32 s4,[r0,#0x18]
  0016c050: vstr.32 s12,[r0,#0x1c]
  0016c054: vstr.32 s14,[r0,#0x20]
  0016c058: vldr.32 s4,[r6,#0xc]
  0016c05c: vcvt.f32.s32 s4,s4
  0016c060: vadd.f32 s4,s4,s6
  0016c064: vcvt.s32.f32 s6,s8
  0016c068: vcvt.s32.f32 s8,s10
  0016c06c: vcvt.s32.f32 s4,s4
  0016c070: vstr.32 s4,[r0,#0x24]
  0016c074: vstr.32 s6,[r0,#0x28]
  0016c078: vstr.32 s8,[r0,#0x2c]
  0016c07c: ldr r2,[r6,#0x20]
  0016c07e: add.w r1,r0,r2, lsl #0x2
  0016c082: subs r2,#0x1
  0016c084: subs r1,#0x18
  0016c086: b 0x0016c09e
  0016c088: sub.w r3,r1,#0x18
  0016c08c: ldrd r5,r4,[r1,#-0x8]
  0016c090: vld1.32 {d16,d17},[r3]
  0016c094: subs r2,#0x6
  0016c096: strd r5,r4,[r1,#0x10]
  0016c09a: vst1.32 {d16,d17},[r1],r12
  0016c09e: cmp r2,#0xa
  0016c0a0: bgt 0x0016c088
  0016c0a2: movs r1,#0x0
  0016c0a4: movs r2,#0x0
  0016c0a6: b 0x0016c0f0
  0016c0a8: adds r3,r0,r1
  0016c0aa: adds r2,#0x3
  0016c0ac: vldr.32 s4,[r3]
  0016c0b0: vcvt.f32.s32 s4,s4
  0016c0b4: ldr r5,[r6,#0x1c]
  0016c0b6: vsub.f32 s4,s4,s0
  0016c0ba: vcvt.s32.f32 s4,s4
  0016c0be: vmov r4,s4
  0016c0c2: str r4,[r5,r1]
  0016c0c4: add r5,r1
  0016c0c6: adds r1,#0xc
  0016c0c8: vldr.32 s4,[r3,#0x4]
  0016c0cc: vcvt.f32.s32 s4,s4
  0016c0d0: vsub.f32 s4,s4,s2
  0016c0d4: vcvt.s32.f32 s4,s4
  0016c0d8: vstr.32 s4,[r5,#0x4]
  0016c0dc: vldr.32 s4,[r3,#0x8]
  0016c0e0: vcvt.f32.s32 s4,s4
  0016c0e4: vsub.f32 s4,s4,s16
  0016c0e8: vcvt.s32.f32 s4,s4
  0016c0ec: vstr.32 s4,[r5,#0x8]
  0016c0f0: ldr r3,[r6,#0x20]
  0016c0f2: cmp r2,r3
  0016c0f4: blt 0x0016c0a8
  0016c0f6: ldr r0,[0x0016c180]
  0016c0f8: movs r4,#0x0
  0016c0fa: movs r5,#0x0
  0016c0fc: add r0,pc
  0016c0fe: ldr.w r11,[r0,#0x0]
  0016c102: b 0x0016c138
  0016c104: ldr r2,[r6,#0x1c]
  0016c106: ldr r1,[r6,#0x10]
  0016c108: add r2,r4
  0016c10a: vldr.32 s0,[r2]
  0016c10e: vldr.32 s2,[r2,#0x4]
  0016c112: vldr.32 s4,[r2,#0x8]
  0016c116: vcvt.f32.s32 s0,s0
  0016c11a: uxth r2,r5
  0016c11c: vmov r3,s0
  0016c120: vcvt.f32.s32 s0,s4
  0016c124: vcvt.f32.s32 s2,s2
  0016c128: vstr.32 s0,[sp,#0x4]
  0016c12c: vstr.32 s2,[sp]
  0016c130: blx 0x000777f4
  0016c134: adds r4,#0xc
  0016c136: adds r5,#0x1
  0016c138: ldr r1,[r6,#0x24]
  0016c13a: ldr.w r0,[r11,#0x0]
  0016c13e: adds r1,#0x1
  0016c140: cmp.w r5,r1, lsl #0x1
  0016c144: blt 0x0016c104
  0016c146: ldr r1,[r6,#0x14]
  0016c148: blx 0x000720c4
  0016c14c: mov r1,r0
  0016c14e: add r0,sp,#0x8
  0016c150: mov r2,r10
  0016c152: mov r3,r9
  0016c154: vstr.32 s16,[sp]
  0016c158: blx 0x0006f820
  0016c15c: ldr r0,[sp,#0x44]
  0016c15e: ldr.w r1,[r8,#0x0]
  0016c162: subs r0,r1,r0
  0016c164: itttt eq
  0016c166: add.eq sp,#0x48
  0016c168: vpop.eq {d8}
  0016c16c: add.eq sp,#0x4
  0016c16e: pop.eq.w {r8,r9,r10,r11}
  0016c172: it eq
  0016c174: pop.eq {r4,r5,r6,r7,pc}
  0016c176: blx 0x0006e824
```
