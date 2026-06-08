# AbyssEngine::SpriteSystemDraw
elf 0x86ddc body 636
Sig: undefined __stdcall SpriteSystemDraw(Engine * param_1, Matrix * param_2, Matrix * param_3, SpriteSystem * param_4)

## decompile
```c

/* AbyssEngine::SpriteSystemDraw(AbyssEngine::Engine*, AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&, AbyssEngine::SpriteSystem*) */

void AbyssEngine::SpriteSystemDraw
               (Engine *param_1,Matrix *param_2,Matrix *param_3,SpriteSystem *param_4)

{
  SpriteSystem SVar1;
  ushort uVar2;
  uint uVar3;
  int *piVar4;
  float *pfVar5;
  uint uVar6;
  Mesh *pMVar7;
  ushort uVar8;
  ushort uVar9;
  int iVar10;
  code *pcVar11;
  int iVar12;
  int iVar13;
  uint in_fpscr;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  Matrix aMStack_64 [60];
  int local_28;
  
  piVar4 = *(int **)(DAT_0009705c + 0x96df2);
  local_28 = *piVar4;
  if (param_4 != (SpriteSystem *)0x0) {
    iVar10 = *(int *)(*(int *)(param_4 + 0x10) + 4);
    AEMath::operator*(aMStack_64,param_2);
    uVar2 = *(ushort *)param_4;
    SVar1 = param_4[0xc];
    uVar9 = 0;
    iVar13 = *(int *)(param_4 + 4);
    iVar12 = *(int *)(param_4 + 8);
    uVar8 = 0;
    for (uVar3 = 0; uVar3 = uVar3 & 0xffff, uVar3 < (uint)uVar2 * 0xc; uVar3 = uVar3 + 0xc) {
      pfVar5 = (float *)(iVar13 + (uint)uVar8 * 4);
      uVar8 = uVar8 + 3;
      fVar18 = pfVar5[1];
      fVar17 = *pfVar5;
      fVar19 = pfVar5[2];
      uVar6 = (uint)uVar9;
      uVar9 = ((byte)SVar1 ^ 1) + uVar9;
      pfVar5 = (float *)(iVar10 + uVar3 * 4);
      fVar16 = (float)VectorSignedToFloat((int)*(short *)(iVar12 + uVar6 * 2) >> 1,
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar15 = *(float *)(param_3 + 0xc) +
               *(float *)(param_3 + 4) * fVar18 + *(float *)param_3 * fVar17 +
               *(float *)(param_3 + 8) * fVar19;
      fVar14 = *(float *)(param_3 + 0x1c) +
               fVar18 * *(float *)(param_3 + 0x14) + fVar17 * *(float *)(param_3 + 0x10) +
               fVar19 * *(float *)(param_3 + 0x18);
      fVar17 = *(float *)(param_3 + 0x2c) +
               fVar18 * *(float *)(param_3 + 0x24) + fVar17 * *(float *)(param_3 + 0x20) +
               fVar19 * *(float *)(param_3 + 0x28);
      fVar18 = fVar15 + fVar16;
      fVar19 = fVar14 - fVar16;
      fVar15 = fVar15 - fVar16;
      fVar14 = fVar14 + fVar16;
      *pfVar5 = fVar15;
      pfVar5[1] = fVar19;
      pfVar5[2] = fVar17;
      pfVar5[3] = fVar18;
      pfVar5[4] = fVar19;
      pfVar5[5] = fVar17;
      pfVar5[6] = fVar18;
      pfVar5[7] = fVar14;
      pfVar5[8] = fVar17;
      pfVar5[9] = fVar15;
      pfVar5[10] = fVar14;
      pfVar5[0xb] = fVar17;
    }
    pMVar7 = *(Mesh **)(param_4 + 0x10);
    if (*(int *)(pMVar7 + 0x30) == 0) {
      MeshDraw(param_1,pMVar7);
    }
    else {
      ArrayAddCached<AbyssEngine::Mesh*>(pMVar7,(Array *)(*(int *)(pMVar7 + 0x30) + 0x44));
      pcVar11 = *(code **)(DAT_00097060 + 0x96f7c);
      (*pcVar11)(*(undefined4 *)param_2,*(undefined4 *)(param_2 + 4),*(undefined4 *)(param_2 + 8),
                 *(undefined4 *)(param_2 + 0xc),*(undefined4 *)(param_2 + 0x10),
                 *(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_2 + 0x18),
                 *(undefined4 *)(param_2 + 0x1c),*(undefined4 *)(param_2 + 0x20),
                 *(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_2 + 0x28),
                 *(undefined4 *)(param_2 + 0x2c),*(undefined4 *)(param_2 + 0x30),
                 *(undefined4 *)(param_2 + 0x34),*(undefined4 *)(param_2 + 0x38),
                 *(int *)(*(int *)(param_4 + 0x10) + 0x30) + 0x2c);
      (*pcVar11)(0x3f800000,0,0,0,0,DAT_00097058,0,0,0,0,0x3f800000,0,0x3f800000,0x3f800000,
                 0x3f800000,*(int *)(*(int *)(param_4 + 0x10) + 0x30) + 0x38);
      (*pcVar11)(*(undefined4 *)param_3,*(undefined4 *)(param_3 + 4),*(undefined4 *)(param_3 + 8),
                 *(undefined4 *)(param_3 + 0xc),*(undefined4 *)(param_3 + 0x10),
                 *(undefined4 *)(param_3 + 0x14),*(undefined4 *)(param_3 + 0x18),
                 *(undefined4 *)(param_3 + 0x1c),*(undefined4 *)(param_3 + 0x20),
                 *(undefined4 *)(param_3 + 0x24),*(undefined4 *)(param_3 + 0x28),
                 *(undefined4 *)(param_3 + 0x2c),*(undefined4 *)(param_3 + 0x30),
                 *(undefined4 *)(param_3 + 0x34),*(undefined4 *)(param_3 + 0x38),
                 *(int *)(*(int *)(param_4 + 0x10) + 0x30) + 0x5c);
      ArrayAddCached<unsigned_int>
                (0xffffffff,(Array *)(*(int *)(*(int *)(param_4 + 0x10) + 0x30) + 0x50));
    }
  }
  if (*piVar4 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00096ddc: push {r4,r5,r6,r7,lr}
  00096dde: add r7,sp,#0xc
  00096de0: push {r8,r9,r10,r11}
  00096de4: sub sp,#0x94
  00096de6: mov r11,r1
  00096de8: ldr r1,[0x0009705c]
  00096dea: mov r4,r3
  00096dec: mov r5,r2
  00096dee: add r1,pc
  00096df0: cmp r3,#0x0
  00096df2: ldr r1,[r1,#0x0]
  00096df4: ldr r2,[r1,#0x0]
  00096df6: str r2,[sp,#0x90]
  00096df8: beq.w 0x00097044
  00096dfc: str r0,[sp,#0x48]
  00096dfe: mov r2,r5
  00096e00: ldr r0,[r4,#0x10]
  00096e02: str r1,[sp,#0x50]
  00096e04: mov r1,r11
  00096e06: ldr.w r8,[r0,#0x4]
  00096e0a: add r0,sp,#0x54
  00096e0c: blx 0x0006f82c
  00096e10: ldrh r0,[r4,#0x0]
  00096e12: mov.w r12,#0x3
  00096e16: ldrb r2,[r4,#0xc]
  00096e18: movs r6,#0x0
  00096e1a: ldr.w lr,[r4,#0x4]
  00096e1e: add.w r0,r0,r0, lsl #0x1
  00096e22: ldr.w r10,[r4,#0x8]
  00096e26: eor r2,r2,#0x1
  00096e2a: str r4,[sp,#0x4c]
  00096e2c: lsls r3,r0,#0x2
  00096e2e: movs r0,#0x0
  00096e30: movs r4,#0x0
  00096e32: b 0x00096f06
  00096e34: uxth.w r9,r4
  00096e38: vldr.32 s2,[r5,#0x4]
  00096e3c: add.w r1,lr,r9, lsl #0x2
  00096e40: vldr.32 s14,[r5,#0x14]
  00096e44: vldr.32 s1,[r5,#0x24]
  00096e48: uxtah r4,r12,r4
  00096e4c: vldr.32 s10,[r1,#0x4]
  00096e50: vldr.32 s0,[r5]
  00096e54: vmul.f32 s2,s2,s10
  00096e58: vldr.32 s8,[r1]
  00096e5c: vmul.f32 s14,s10,s14
  00096e60: vldr.32 s12,[r1,#0x8]
  00096e64: vmul.f32 s10,s10,s1
  00096e68: vldr.32 s1,[r5,#0x10]
  00096e6c: vldr.32 s4,[r5,#0x8]
  00096e70: uxth r1,r6
  00096e72: ldrsh.w r1,[r10,r1,lsl #0x1]
  00096e76: uxtah r6,r2,r6
  00096e7a: vldr.32 s6,[r5,#0xc]
  00096e7e: vmla.f32 s2,s0,s8
  00096e82: vldr.32 s0,[r5,#0x20]
  00096e86: vmla.f32 s14,s8,s1
  00096e8a: vmla.f32 s10,s8,s0
  00096e8e: vldr.32 s0,[r5,#0x18]
  00096e92: asrs r1,r1,#0x1
  00096e94: vmla.f32 s2,s4,s12
  00096e98: vmla.f32 s14,s12,s0
  00096e9c: vldr.32 s0,[r5,#0x28]
  00096ea0: vmov s4,r1
  00096ea4: add.w r1,r8,r0, lsl #0x2
  00096ea8: vmla.f32 s10,s12,s0
  00096eac: vldr.32 s0,[r5,#0x1c]
  00096eb0: vcvt.f32.s32 s4,s4
  00096eb4: adds r0,#0xc
  00096eb6: vadd.f32 s2,s6,s2
  00096eba: vldr.32 s6,[r5,#0x2c]
  00096ebe: vadd.f32 s0,s0,s14
  00096ec2: vadd.f32 s6,s6,s10
  00096ec6: vadd.f32 s8,s2,s4
  00096eca: vsub.f32 s10,s0,s4
  00096ece: vsub.f32 s2,s2,s4
  00096ed2: vadd.f32 s0,s0,s4
  00096ed6: vstr.32 s2,[r1]
  00096eda: vstr.32 s10,[r1,#0x4]
  00096ede: vstr.32 s6,[r1,#0x8]
  00096ee2: vstr.32 s8,[r1,#0xc]
  00096ee6: vstr.32 s10,[r1,#0x10]
  00096eea: vstr.32 s6,[r1,#0x14]
  00096eee: vstr.32 s8,[r1,#0x18]
  00096ef2: vstr.32 s0,[r1,#0x1c]
  00096ef6: vstr.32 s6,[r1,#0x20]
  00096efa: vstr.32 s2,[r1,#0x24]
  00096efe: vstr.32 s0,[r1,#0x28]
  00096f02: vstr.32 s6,[r1,#0x2c]
  00096f06: uxth r0,r0
  00096f08: cmp r0,r3
  00096f0a: bcc 0x00096e34
  00096f0c: ldr r4,[sp,#0x4c]
  00096f0e: ldr r2,[r4,#0x10]
  00096f10: ldr r0,[r2,#0x30]
  00096f12: cmp r0,#0x0
  00096f14: beq.w 0x0009703a
  00096f18: add.w r1,r0,#0x44
  00096f1c: mov r0,r2
  00096f1e: blx 0x0006ffa0
  00096f22: ldr.w r0,[r11,#0x0]
  00096f26: str r0,[sp,#0x44]
  00096f28: ldr.w r0,[r11,#0x4]
  00096f2c: str r0,[sp,#0x40]
  00096f2e: ldr.w r0,[r11,#0x8]
  00096f32: str r0,[sp,#0x3c]
  00096f34: ldr.w r0,[r11,#0xc]
  00096f38: str r0,[sp,#0x38]
  00096f3a: ldr.w r0,[r11,#0x10]
  00096f3e: str r0,[sp,#0x34]
  00096f40: ldr.w r0,[r11,#0x14]
  00096f44: str r0,[sp,#0x30]
  00096f46: ldrd r9,r10,[r11,#0x18]
  00096f4a: ldrd r6,lr,[r11,#0x20]
  00096f4e: ldrd r8,r2,[r11,#0x28]
  00096f52: add.w r11,r11,#0x30
  00096f56: ldm.w r11,{r0,r1,r11}
  00096f5a: ldr.w r12,[r4,#0x10]
  00096f5e: ldr.w r3,[r12,#0x30]
  00096f62: add.w r12,sp,#0x20
  00096f66: stm r12,{r0,r1,r11}
  00096f6a: adds r3,#0x2c
  00096f6c: str r3,[sp,#0x2c]
  00096f6e: strd r8,r2,[sp,#0x18]
  00096f72: ldr r0,[0x00097060]
  00096f74: strd r6,lr,[sp,#0x10]
  00096f78: add r0,pc
  00096f7a: strd r9,r10,[sp,#0x8]
  00096f7e: ldr.w r8,[r0,#0x0]
  00096f82: ldr r0,[sp,#0x30]
  00096f84: str r0,[sp,#0x4]
  00096f86: ldr r0,[sp,#0x34]
  00096f88: str r0,[sp,#0x0]
  00096f8a: ldrd r1,r0,[sp,#0x40]
  00096f8e: ldrd r3,r2,[sp,#0x38]
  00096f92: blx r8
  00096f94: ldr r0,[r4,#0x10]
  00096f96: mov.w r2,#0x3f800000
  00096f9a: ldr r1,[0x00097058]
  00096f9c: movs r3,#0x0
  00096f9e: str.w r8,[sp,#0x48]
  00096fa2: mov r6,r8
  00096fa4: ldr r0,[r0,#0x30]
  00096fa6: strd r2,r2,[sp,#0x20]
  00096faa: adds r0,#0x38
  00096fac: strd r2,r0,[sp,#0x28]
  00096fb0: movs r0,#0x0
  00096fb2: strd r0,r1,[sp,#0x0]
  00096fb6: movs r1,#0x0
  00096fb8: strd r0,r0,[sp,#0x8]
  00096fbc: strd r0,r0,[sp,#0x10]
  00096fc0: strd r2,r0,[sp,#0x18]
  00096fc4: mov.w r0,#0x3f800000
  00096fc8: movs r2,#0x0
  00096fca: blx r8
  00096fcc: ldr r0,[r5,#0x0]
  00096fce: add.w r11,r5,#0x20
  00096fd2: str r0,[sp,#0x44]
  00096fd4: ldr r0,[r5,#0x4]
  00096fd6: str r0,[sp,#0x40]
  00096fd8: ldr r0,[r5,#0x8]
  00096fda: str r0,[sp,#0x3c]
  00096fdc: ldr r0,[r5,#0xc]
  00096fde: str r0,[sp,#0x38]
  00096fe0: ldrd r6,r0,[r5,#0x10]
  00096fe4: str r0,[sp,#0x34]
  00096fe6: ldrd r12,lr,[r5,#0x18]
  00096fea: ldm.w r11,{r9,r10,r11}
  00096fee: ldrd r3,r0,[r5,#0x2c]
  00096ff2: ldrd r1,r5,[r5,#0x34]
  00096ff6: ldr.w r8,[r4,#0x10]
  00096ffa: ldr.w r2,[r8,#0x30]
  00096ffe: add.w r8,sp,#0x20
  00097002: stm.w r8!,{r0,r1,r5}
  00097006: add r1,sp,#0x4
  00097008: adds r2,#0x5c
  0009700a: ldr r0,[sp,#0x34]
  0009700c: str r2,[sp,#0x2c]
  0009700e: str r6,[sp,#0x0]
  00097010: stm r1,{r0,r12,lr}
  00097014: add r0,sp,#0x10
  00097016: stm r0,{r9,r10,r11}
  0009701a: str r3,[sp,#0x1c]
  0009701c: ldrd r1,r0,[sp,#0x40]
  00097020: ldrd r3,r2,[sp,#0x38]
  00097024: ldr r6,[sp,#0x48]
  00097026: blx r6
  00097028: ldr r0,[r4,#0x10]
  0009702a: ldr r0,[r0,#0x30]
  0009702c: add.w r1,r0,#0x50
  00097030: mov.w r0,#0xffffffff
  00097034: blx 0x0006ffac
  00097038: b 0x00097042
  0009703a: ldr r0,[sp,#0x48]
  0009703c: mov r1,r2
  0009703e: blx 0x0006f568
  00097042: ldr r1,[sp,#0x50]
  00097044: ldr r0,[sp,#0x90]
  00097046: ldr r1,[r1,#0x0]
  00097048: subs r0,r1,r0
  0009704a: ittt eq
  0009704c: add.eq sp,#0x94
  0009704e: pop.eq.w {r8,r9,r10,r11}
  00097052: pop.eq {r4,r5,r6,r7,pc}
  00097054: blx 0x0006e824
```
