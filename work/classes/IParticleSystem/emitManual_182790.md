# IParticleSystem::emitManual
elf 0x182790 body 928
Sig: undefined __stdcall emitManual(Vector param_1, int param_2, Vector * param_3, float param_4)

## decompile
```c

/* IParticleSystem::emitManual(AbyssEngine::AEMath::Vector, int, AbyssEngine::AEMath::Vector const*,
   float) */

void IParticleSystem::emitManual
               (float *param_1,undefined4 param_2,float param_3,undefined4 param_4,int param_5,
               int param_6,float param_7)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  code *pcVar8;
  int *piVar9;
  int iVar10;
  float *pfVar11;
  int iVar12;
  undefined4 uVar13;
  uint in_fpscr;
  uint uVar14;
  float fVar15;
  float fVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  Vector aVStack_b0 [12];
  undefined4 local_a4;
  float local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  float local_94;
  undefined4 uStack_90;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  int local_6c;
  
  iVar12 = DAT_00192b34;
  piVar9 = *(int **)(DAT_00192b30 + 0x1927a8);
  local_6c = *piVar9;
  local_98 = param_2;
  local_94 = param_3;
  uStack_90 = param_4;
  if (param_5 != -1) {
    iVar3 = *(int *)((int)param_1[0xf] + param_5 * 4);
    *(char *)((int)param_1[0x1b] + (int)param_1[0x14]) = (char)iVar3;
    iVar12 = *(int *)(iVar12 + 0x1927d2) + iVar3 * 0xa0;
    *(undefined4 *)((int)param_1[0x1a] + (int)param_1[0x14] * 4) = 0;
    local_7c = *(undefined4 *)(iVar12 + 0x88);
    local_74 = *(undefined4 *)(iVar12 + 0x8c);
    local_78 = *(undefined4 *)(iVar12 + 0x90);
    local_70 = *(undefined4 *)(iVar12 + 0x94);
    if ((int)((uint)*(byte *)((int)param_1 + 0x37) << 0x1e) < 0) {
      puVar4 = (undefined4 *)rotateUVs(param_1,(int)&local_7c,(float *)param_1[0x14]);
    }
    else {
      puVar4 = &local_7c;
    }
    iVar3 = *(int *)(iVar12 + 0x50);
    if (iVar3 == 0) {
      local_a4 = 0;
      local_a0 = 0.0;
      local_9c = 0;
    }
    else {
      pfVar11 = param_1 + 4;
      iVar10 = iVar3 << 1;
      pcVar8 = *(code **)(DAT_00192b38 + 0x19282c);
      iVar5 = (*pcVar8)(pfVar11,iVar10);
      iVar6 = (*pcVar8)(pfVar11,iVar10);
      fVar15 = (float)VectorSignedToFloat(iVar6 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      fVar16 = *(float *)(iVar12 + 0x5c);
      uVar18 = VectorSignedToFloat(iVar5 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (*pcVar8)(pfVar11,iVar10);
      local_a0 = fVar16 + fVar15;
      local_9c = VectorSignedToFloat(iVar5 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      local_a4 = uVar18;
    }
    AbyssEngine::AEMath::Vector::operator=
              ((Vector *)((int)param_1[0x19] + (int)param_1[0x14] * 0xc),(Vector *)&local_a4);
    if (param_6 != 0) {
      in_fpscr = in_fpscr & 0xfffffff | (uint)(*(float *)(iVar12 + 100) == 0.0) << 0x1e;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        AbyssEngine::AEMath::operator*((Vector *)&local_a4,*(float *)(iVar12 + 100));
        AbyssEngine::AEMath::Vector::operator-=
                  ((Vector *)((int)param_1[0x19] + (int)param_1[0x14] * 0xc),(Vector *)&local_a4);
      }
    }
    iVar3 = *(int *)(iVar12 + 0x48);
    if (iVar3 != 0) {
      iVar5 = AbyssEngine::AERandom::nextInt((int)(param_1 + 4));
      uVar18 = VectorSignedToFloat(iVar5 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = AbyssEngine::AERandom::nextInt((int)(param_1 + 4));
      local_9c = VectorSignedToFloat(iVar5 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      local_a0 = 0.0;
      local_a4 = uVar18;
      AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_98,(Vector *)&local_a4);
    }
    if (*(int *)(iVar12 + 0x4c) != 0) {
      iVar3 = AbyssEngine::AERandom::nextInt((int)(param_1 + 4));
      fVar15 = (float)VectorSignedToFloat(iVar3 - *(int *)(iVar12 + 0x4c),
                                          (byte)(in_fpscr >> 0x16) & 3);
      local_94 = local_94 + fVar15;
    }
    uVar1 = in_fpscr & 0xfffffff | (uint)(param_7 < 0.0) << 0x1f;
    uVar14 = uVar1 | (uint)NAN(param_7) << 0x1c;
    if ((byte)(uVar1 >> 0x1f) != ((byte)(uVar14 >> 0x1c) & 1)) {
      param_7 = *(float *)(iVar12 + 0x14);
    }
    piVar2 = (int *)(iVar12 + 0x18);
    if (*piVar2 == 0) {
      uVar20 = *(undefined4 *)(iVar12 + 0x1c);
      uVar21 = *(undefined4 *)(iVar12 + 0x20);
      iVar3 = *(int *)(iVar12 + 0x3c);
      uVar18 = *(undefined4 *)(iVar12 + 0x34);
      uVar13 = *puVar4;
      uVar7 = puVar4[1];
      uVar19 = puVar4[2];
      uVar17 = puVar4[3];
      if (*(float *)(iVar12 + 0x24) == 0.0) {
        local_a4 = 0;
        local_a0 = 0.0;
        local_9c = 0;
      }
      else {
        AbyssEngine::AEMath::operator*(*(float *)(iVar12 + 0x24),(Vector *)&local_a4);
      }
      (**(code **)((int)*param_1 + 0x18))
                (param_1,&local_98,param_7,uVar18,uVar13,uVar19,uVar7,uVar17,0 < iVar3,uVar20,uVar21
                 ,&local_a4);
    }
    else {
      pfVar11 = param_1 + 4;
      pcVar8 = *(code **)(DAT_00192b3c + 0x192966);
      uVar18 = (*pcVar8)(pfVar11);
      uVar13 = *(undefined4 *)(iVar12 + 0x34);
      iVar3 = *(int *)(iVar12 + 0x3c);
      uVar19 = *puVar4;
      uVar17 = puVar4[1];
      uVar21 = puVar4[2];
      uVar20 = puVar4[3];
      fVar22 = *(float *)(iVar12 + 0x1c);
      uVar7 = (*pcVar8)(pfVar11,*piVar2);
      fVar24 = *(float *)(iVar12 + 0x20);
      fVar23 = (float)VectorSignedToFloat(uVar18,(byte)(uVar14 >> 0x16) & 3);
      fVar16 = (float)VectorSignedToFloat(uVar7,(byte)(uVar14 >> 0x16) & 3);
      uVar18 = (*pcVar8)(pfVar11,*piVar2);
      fVar15 = (float)VectorSignedToFloat(uVar18,(byte)(uVar14 >> 0x16) & 3);
      if (*(float *)(iVar12 + 0x24) == 0.0) {
        local_a4 = 0;
        local_a0 = 0.0;
        local_9c = 0;
      }
      else {
        AbyssEngine::AEMath::operator*(*(float *)(iVar12 + 0x24),(Vector *)&local_a4);
      }
      (**(code **)((int)*param_1 + 0x18))
                (param_1,&local_98,param_7 + fVar23,uVar13,uVar19,uVar21,uVar17,uVar20,0 < iVar3,
                 fVar22 + fVar16,fVar24 + fVar15,&local_a4);
    }
    if (*(float *)(iVar12 + 100) != 0.0) {
      fVar15 = (float)AbyssEngine::AEMath::operator*(aVStack_b0,*(float *)(iVar12 + 100));
      AbyssEngine::AEMath::operator*((Vector *)&local_a4,fVar15);
      AbyssEngine::AEMath::Vector::operator+=
                ((Vector *)((int)param_1[0x19] + (int)param_1[0x14] * 0xc),(Vector *)&local_a4);
    }
    fVar15 = (float)((int)param_1[0x14] + 1);
    if ((int)param_1[0x12] <= (int)fVar15) {
      fVar15 = 0.0;
    }
    param_1[0x14] = fVar15;
  }
  if (*piVar9 != local_6c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00192790: push {r4,r5,r6,r7,lr}
  00192792: add r7,sp,#0xc
  00192794: push {r7,r8,r9,r10,r11}
  00192798: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  0019279c: sub sp,#0x78
  0019279e: ldr r4,[0x00192b30]
  001927a0: mov r11,r0
  001927a2: str r1,[sp,#0x48]
  001927a4: add r4,pc
  001927a6: ldr r0,[r7,#0x8]
  001927a8: strd r2,r3,[sp,#0x4c]
  001927ac: ldr r5,[r4,#0x0]
  001927ae: ldr r1,[r5,#0x0]
  001927b0: str r1,[sp,#0x74]
  001927b2: adds r1,r0,#0x1
  001927b4: beq.w 0x00192b14
  001927b8: ldr.w r1,[r11,#0x3c]
  001927bc: movs r6,#0x0
  001927be: ldr.w r2,[r11,#0x50]
  001927c2: ldr.w r3,[r11,#0x6c]
  001927c6: ldr.w r0,[r1,r0,lsl #0x2]
  001927ca: ldr r1,[0x00192b34]
  001927cc: strb r0,[r3,r2]
  001927ce: add r1,pc
  001927d0: ldr.w r2,[r11,#0x50]
  001927d4: add.w r0,r0,r0, lsl #0x2
  001927d8: ldr.w r3,[r11,#0x68]
  001927dc: ldr r1,[r1,#0x0]
  001927de: add.w r8,r1,r0, lsl #0x5
  001927e2: str.w r6,[r3,r2,lsl #0x2]
  001927e6: add.w r3,r8,#0x88
  001927ea: ldmia r3,{r0,r1,r2,r3}
  001927ec: strd r0,r2,[sp,#0x64]
  001927f0: strd r1,r3,[sp,#0x6c]
  001927f4: ldrb.w r0,[r11,#0x37]
  001927f8: str r5,[sp,#0x2c]
  001927fa: lsls r0,r0,#0x1e
  001927fc: bmi 0x00192802
  001927fe: add r0,sp,#0x64
  00192800: b 0x00192810
  00192802: ldr.w r2,[r11,#0x50]
  00192806: add r1,sp,#0x64
  00192808: add r3,sp,#0x54
  0019280a: mov r0,r11
  0019280c: blx 0x00078874
  00192810: ldr.w r10,[r8,#0x50]
  00192814: ldr r4,[r7,#0xc]
  00192816: cmp.w r10,#0x0
  0019281a: str r0,[sp,#0x28]
  0019281c: beq 0x0019287a
  0019281e: ldr r0,[0x00192b38]
  00192820: add.w r6,r11,#0x10
  00192824: lsl.w r5,r10,#0x1
  00192828: add r0,pc
  0019282a: mov r1,r5
  0019282c: ldr r4,[r0,#0x0]
  0019282e: mov r0,r6
  00192830: blx r4
  00192832: sub.w r9,r0,r10
  00192836: mov r0,r6
  00192838: mov r1,r5
  0019283a: blx r4
  0019283c: sub.w r0,r0,r10
  00192840: mov r1,r5
  00192842: vmov s0,r0
  00192846: mov r0,r6
  00192848: vcvt.f32.s32 s16,s0
  0019284c: vldr.32 s18,[r8,#0x5c]
  00192850: vmov s0,r9
  00192854: vcvt.f32.s32 s20,s0
  00192858: blx r4
  0019285a: sub.w r0,r0,r10
  0019285e: vadd.f32 s2,s18,s16
  00192862: ldr r4,[r7,#0xc]
  00192864: vmov s0,r0
  00192868: vcvt.f32.s32 s0,s0
  0019286c: vstr.32 s20,[sp,#0x3c]
  00192870: vstr.32 s0,[sp,#0x44]
  00192874: vstr.32 s2,[sp,#0x40]
  00192878: b 0x00192882
  0019287a: movs r0,#0x0
  0019287c: strd r0,r0,[sp,#0x3c]
  00192880: str r0,[sp,#0x44]
  00192882: ldr.w r0,[r11,#0x50]
  00192886: ldr.w r1,[r11,#0x64]
  0019288a: add.w r0,r0,r0, lsl #0x1
  0019288e: add.w r0,r1,r0, lsl #0x2
  00192892: add r1,sp,#0x3c
  00192894: blx 0x0006eb3c
  00192898: cbz r4,0x001928cc
  0019289a: vldr.32 s0,[r8,#0x64]
  0019289e: vcmpe.f32 s0,#0
  001928a2: vmrs apsr,fpscr
  001928a6: beq 0x001928cc
  001928a8: vmov r2,s0
  001928ac: add r5,sp,#0x3c
  001928ae: mov r1,r4
  001928b0: mov r0,r5
  001928b2: blx 0x0006ec74
  001928b6: ldr.w r0,[r11,#0x50]
  001928ba: ldr.w r1,[r11,#0x64]
  001928be: add.w r0,r0,r0, lsl #0x1
  001928c2: add.w r0,r1,r0, lsl #0x2
  001928c6: mov r1,r5
  001928c8: blx 0x00072694
  001928cc: ldr.w r4,[r8,#0x48]
  001928d0: cbz r4,0x00192910
  001928d2: add.w r5,r11,#0x10
  001928d6: lsls r6,r4,#0x1
  001928d8: mov r1,r6
  001928da: mov r0,r5
  001928dc: blx 0x00071848
  001928e0: subs r0,r0,r4
  001928e2: mov r1,r6
  001928e4: vmov s0,r0
  001928e8: mov r0,r5
  001928ea: vcvt.f32.s32 s16,s0
  001928ee: blx 0x00071848
  001928f2: subs r0,r0,r4
  001928f4: add r1,sp,#0x3c
  001928f6: vmov s0,r0
  001928fa: movs r0,#0x0
  001928fc: vcvt.f32.s32 s0,s0
  00192900: str r0,[sp,#0x40]
  00192902: add r0,sp,#0x48
  00192904: vstr.32 s16,[sp,#0x3c]
  00192908: vstr.32 s0,[sp,#0x44]
  0019290c: blx 0x00073534
  00192910: mov r4,r8
  00192912: vldr.32 s16,[r7,#0x10]
  00192916: ldr.w r1,[r4,#0x4c]!
  0019291a: cbz r1,0x0019293e
  0019291c: add.w r0,r11,#0x10
  00192920: lsls r1,r1,#0x1
  00192922: blx 0x00071848
  00192926: ldr r1,[r4,#0x0]
  00192928: subs r0,r0,r1
  0019292a: vmov s0,r0
  0019292e: vcvt.f32.s32 s0,s0
  00192932: vldr.32 s2,[sp,#0x4c]
  00192936: vadd.f32 s0,s2,s0
  0019293a: vstr.32 s0,[sp,#0x4c]
  0019293e: vcmpe.f32 s16,#0
  00192942: mov r4,r8
  00192944: vmrs apsr,fpscr
  00192948: add.w r5,r8,#0x64
  0019294c: it lt
  0019294e: vldr.lt.32 s16,[r8,#0x14]
  00192952: ldr.w r1,[r4,#0x18]!
  00192956: cmp r1,#0x0
  00192958: beq 0x001929e8
  0019295a: str r5,[sp,#0x24]
  0019295c: add.w r6,r11,#0x10
  00192960: ldr r0,[0x00192b3c]
  00192962: add r0,pc
  00192964: ldr.w r10,[r0,#0x0]
  00192968: mov r0,r6
  0019296a: blx r10
  0019296c: ldr r1,[sp,#0x28]
  0019296e: vmov s28,r0
  00192972: mov r0,r6
  00192974: ldr.w r9,[r8,#0x34]
  00192978: ldr.w r5,[r8,#0x3c]
  0019297c: vldr.32 s20,[r1]
  00192980: vldr.32 s18,[r1,#0x4]
  00192984: vldr.32 s24,[r1,#0x8]
  00192988: vldr.32 s22,[r1,#0xc]
  0019298c: ldr r1,[r4,#0x0]
  0019298e: vldr.32 s26,[r8,#0x1c]
  00192992: blx r10
  00192994: vldr.32 s30,[r8,#0x20]
  00192998: vcvt.f32.s32 s28,s28
  0019299c: ldr r1,[r4,#0x0]
  0019299e: vmov s0,r0
  001929a2: mov r0,r6
  001929a4: vcvt.f32.s32 s17,s0
  001929a8: blx r10
  001929aa: vmov s0,r0
  001929ae: vadd.f32 s28,s16,s28
  001929b2: vcvt.f32.s32 s0,s0
  001929b6: vadd.f32 s16,s26,s17
  001929ba: vadd.f32 s26,s30,s0
  001929be: vldr.32 s0,[r8,#0x24]
  001929c2: vcmpe.f32 s0,#0
  001929c6: vmrs apsr,fpscr
  001929ca: beq 0x00192a34
  001929cc: vmov r1,s0
  001929d0: ldr.w r0,[r11,#0x50]
  001929d4: ldr.w r2,[r11,#0x64]
  001929d8: add.w r0,r0,r0, lsl #0x1
  001929dc: add.w r2,r2,r0, lsl #0x2
  001929e0: add r0,sp,#0x3c
  001929e2: blx 0x00072658
  001929e6: b 0x00192a3c
  001929e8: ldr r0,[sp,#0x28]
  001929ea: vldr.32 s0,[r8,#0x24]
  001929ee: vldr.32 s26,[r8,#0x1c]
  001929f2: vldr.32 s28,[r8,#0x20]
  001929f6: vcmpe.f32 s0,#0
  001929fa: ldr.w r6,[r8,#0x3c]
  001929fe: vmrs apsr,fpscr
  00192a02: ldr.w r8,[r8,#0x34]
  00192a06: vldr.32 s20,[r0]
  00192a0a: vldr.32 s18,[r0,#0x4]
  00192a0e: vldr.32 s24,[r0,#0x8]
  00192a12: vldr.32 s22,[r0,#0xc]
  00192a16: beq 0x00192a7a
  00192a18: vmov r1,s0
  00192a1c: ldr.w r0,[r11,#0x50]
  00192a20: ldr.w r2,[r11,#0x64]
  00192a24: add.w r0,r0,r0, lsl #0x1
  00192a28: add.w r2,r2,r0, lsl #0x2
  00192a2c: add r0,sp,#0x3c
  00192a2e: blx 0x00072658
  00192a32: b 0x00192a82
  00192a34: movs r0,#0x0
  00192a36: strd r0,r0,[sp,#0x3c]
  00192a3a: str r0,[sp,#0x44]
  00192a3c: vmov r2,s28
  00192a40: ldr.w r0,[r11,#0x0]
  00192a44: cmp r5,#0x0
  00192a46: add r1,sp,#0x48
  00192a48: mov r3,r9
  00192a4a: ldr r6,[r0,#0x18]
  00192a4c: add r0,sp,#0x3c
  00192a4e: str r0,[sp,#0x1c]
  00192a50: mov.w r0,#0x0
  00192a54: vstr.32 s16,[sp,#0x14]
  00192a58: vstr.32 s26,[sp,#0x18]
  00192a5c: it gt
  00192a5e: mov.gt r0,#0x1
  00192a60: str r0,[sp,#0x10]
  00192a62: mov r0,r11
  00192a64: vstr.32 s20,[sp]
  00192a68: vstr.32 s24,[sp,#0x4]
  00192a6c: vstr.32 s18,[sp,#0x8]
  00192a70: vstr.32 s22,[sp,#0xc]
  00192a74: blx r6
  00192a76: ldr r5,[sp,#0x24]
  00192a78: b 0x00192abc
  00192a7a: movs r0,#0x0
  00192a7c: strd r0,r0,[sp,#0x3c]
  00192a80: str r0,[sp,#0x44]
  00192a82: vmov r2,s16
  00192a86: ldr.w r0,[r11,#0x0]
  00192a8a: cmp r6,#0x0
  00192a8c: add r1,sp,#0x48
  00192a8e: mov r3,r8
  00192a90: ldr r4,[r0,#0x18]
  00192a92: add r0,sp,#0x3c
  00192a94: str r0,[sp,#0x1c]
  00192a96: mov.w r0,#0x0
  00192a9a: vstr.32 s26,[sp,#0x14]
  00192a9e: vstr.32 s28,[sp,#0x18]
  00192aa2: it gt
  00192aa4: mov.gt r0,#0x1
  00192aa6: str r0,[sp,#0x10]
  00192aa8: mov r0,r11
  00192aaa: vstr.32 s20,[sp]
  00192aae: vstr.32 s24,[sp,#0x4]
  00192ab2: vstr.32 s18,[sp,#0x8]
  00192ab6: vstr.32 s22,[sp,#0xc]
  00192aba: blx r4
  00192abc: vldr.32 s0,[r5]
  00192ac0: vcmpe.f32 s0,#0
  00192ac4: vmrs apsr,fpscr
  00192ac8: beq 0x00192afe
  00192aca: vmov r2,s0
  00192ace: add r5,sp,#0x30
  00192ad0: add.w r1,r11,#0x1c
  00192ad4: mov r0,r5
  00192ad6: blx 0x0006ec74
  00192ada: add r6,sp,#0x3c
  00192adc: mov r1,r5
  00192ade: mov.w r2,#0x40000000
  00192ae2: mov r0,r6
  00192ae4: blx 0x0006ec74
  00192ae8: ldr.w r0,[r11,#0x50]
  00192aec: ldr.w r1,[r11,#0x64]
  00192af0: add.w r0,r0,r0, lsl #0x1
  00192af4: add.w r0,r1,r0, lsl #0x2
  00192af8: mov r1,r6
  00192afa: blx 0x00073534
  00192afe: ldr.w r1,[r11,#0x50]
  00192b02: ldr.w r0,[r11,#0x48]
  00192b06: adds r1,#0x1
  00192b08: cmp r1,r0
  00192b0a: it ge
  00192b0c: mov.ge r1,#0x0
  00192b0e: ldr r5,[sp,#0x2c]
  00192b10: str.w r1,[r11,#0x50]
  00192b14: ldr r0,[sp,#0x74]
  00192b16: ldr r1,[r5,#0x0]
  00192b18: subs r0,r1,r0
  00192b1a: itttt eq
  00192b1c: add.eq sp,#0x78
  00192b1e: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  00192b22: add.eq sp,#0x4
  00192b24: pop.eq.w {r8,r9,r10,r11}
  00192b28: it eq
  00192b2a: pop.eq {r4,r5,r6,r7,pc}
  00192b2c: blx 0x0006e824
```
