# PaintCanvas::DrawRegion2D
elf 0x78538 body 686
Sig: undefined __thiscall DrawRegion2D(PaintCanvas * this, uint param_1, int param_2, int param_3, int param_4, int param_5, float param_6, int param_7, int param_8, int param_9, int param_10)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawRegion2D(unsigned int, int, int, int, int, float, int, int, int,
   int) */

void __thiscall
AbyssEngine::PaintCanvas::DrawRegion2D
          (PaintCanvas *this,uint param_1,int param_2,int param_3,int param_4,int param_5,
          float param_6,int param_7,int param_8,int param_9,int param_10)

{
  undefined4 uVar1;
  uint uVar2;
  int *piVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  float *pfVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  uint in_fpscr;
  float fVar10;
  float extraout_s0;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  int in_stack_00000018;
  Matrix aMStack_16c [60];
  undefined4 local_130 [5];
  undefined4 local_11c;
  undefined4 local_118;
  undefined4 uStack_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined4 local_f8;
  undefined4 local_f0;
  uint uStack_ec;
  undefined4 local_e8;
  undefined4 uStack_e4;
  uint local_e0;
  undefined4 uStack_dc;
  undefined4 local_d8;
  undefined4 uStack_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined4 local_c8;
  undefined8 local_c4;
  undefined8 uStack_bc;
  undefined4 local_b0 [3];
  undefined4 local_a4;
  undefined4 local_9c;
  undefined4 local_98;
  float fStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined4 local_78;
  undefined4 local_70 [3];
  undefined4 local_64;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined4 local_38;
  int local_34;
  
  piVar3 = *(int **)(DAT_0008880c + 0x88552);
  local_34 = *piVar3;
  if (param_1 < *(uint *)(this + 0x14c)) {
    SetTexture((uint)this,*(uint *)(*(int *)(*(int *)(this + 0x150) + param_1 * 4) + 4));
    piVar6 = *(int **)(*(int *)(this + 0x150) + param_1 * 4);
    uStack_114 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_110 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_10c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    iVar5 = *piVar6;
    fVar10 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined1 *)(piVar6 + 5) = 1;
    puVar4 = *(undefined4 **)(iVar5 + 4);
    fVar12 = (float)VectorSignedToFloat(param_5,(byte)(in_fpscr >> 0x16) & 3);
    puVar4[3] = fVar10;
    puVar4[4] = 0;
    puVar4[5] = 0;
    puVar4[6] = fVar10;
    puVar4[7] = fVar12;
    puVar4[8] = 0;
    puVar4[9] = 0;
    puVar4[10] = fVar12;
    puVar4[0xb] = 0;
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    fVar13 = (float)VectorUnsignedToFloat
                              ((uint)*(ushort *)((int)piVar6 + 10),(byte)(in_fpscr >> 0x16) & 3);
    fVar14 = (float)VectorUnsignedToFloat
                              ((uint)*(ushort *)(piVar6 + 2),(byte)(in_fpscr >> 0x16) & 3);
    fVar16 = (float)VectorUnsignedToFloat
                              ((uint)*(ushort *)(piVar6 + 3),(byte)(in_fpscr >> 0x16) & 3);
    fVar15 = (float)VectorUnsignedToFloat
                              ((uint)*(ushort *)((int)piVar6 + 0xe),(byte)(in_fpscr >> 0x16) & 3);
    fVar18 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = (float)VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    fVar11 = (float)VectorSignedToFloat((int)(short)param_2,(byte)(in_fpscr >> 0x16) & 3);
    pfVar7 = *(float **)(iVar5 + 8);
    fVar17 = (fVar18 + fVar16) * (1.0 / fVar14);
    fVar18 = (fVar19 + fVar15) * (1.0 / fVar13);
    fVar11 = (fVar10 + fVar16 + fVar11) * (1.0 / fVar14);
    fVar10 = (fVar12 + fVar15 + fVar19) * (1.0 / fVar13);
    *pfVar7 = fVar17;
    pfVar7[1] = fVar18;
    pfVar7[2] = fVar11;
    pfVar7[3] = fVar18;
    pfVar7[4] = fVar11;
    pfVar7[5] = fVar10;
    pfVar7[6] = fVar17;
    pfVar7[7] = fVar10;
    uVar1 = DAT_00088808;
    uStack_40 = DAT_00088800;
    uStack_48 = DAT_000887f8;
    uStack_bc = DAT_000887f0;
    local_c4 = DAT_000887e8;
    puVar4 = *(undefined4 **)(iVar5 + 0x2c);
    *puVar4 = 0x20000;
    puVar9 = (undefined4 *)((uint)local_70 | 4);
    puVar4[1] = 1;
    puVar8 = (undefined4 *)((uint)local_b0 | 4);
    puVar4[2] = uVar1;
    *puVar9 = 0;
    puVar9[1] = uStack_114;
    puVar9[2] = uStack_110;
    puVar9[3] = uStack_10c;
    local_58 = 0;
    *puVar8 = 0;
    puVar8[1] = uStack_114;
    puVar8[2] = uStack_110;
    puVar8[3] = uStack_10c;
    local_98 = 0;
    uStack_88 = uStack_48;
    uStack_80 = uStack_40;
    local_d8 = 0;
    local_70[0] = 0x3f800000;
    local_5c = 0x3f800000;
    local_b0[0] = 0x3f800000;
    local_9c = 0x3f800000;
    local_38 = 0x3f800000;
    local_78 = 0x3f800000;
    local_e8 = 0;
    uStack_e4 = 0;
    puVar4 = (undefined4 *)((uint)local_130 | 4);
    *puVar4 = 0;
    puVar4[1] = uStack_114;
    puVar4[2] = uStack_110;
    puVar4[3] = uStack_10c;
    local_118 = 0;
    uStack_108 = uStack_48;
    uStack_100 = uStack_40;
    local_64 = VectorSignedToFloat(-param_8,(byte)(in_fpscr >> 0x16) & 3);
    uStack_54 = VectorSignedToFloat(-param_9,(byte)(in_fpscr >> 0x16) & 3);
    local_a4 = VectorSignedToFloat(param_10 + param_8,(byte)(in_fpscr >> 0x16) & 3);
    fStack_94 = (float)VectorSignedToFloat(in_stack_00000018 + param_9,(byte)(in_fpscr >> 0x16) & 3)
    ;
    local_130[0] = 0x3f800000;
    local_11c = 0x3f800000;
    local_f8 = 0x3f800000;
    uStack_d4 = uStack_114;
    uStack_d0 = uStack_110;
    uStack_cc = uStack_10c;
    uStack_90 = uStack_110;
    uStack_8c = uStack_10c;
    uStack_50 = uStack_110;
    uStack_4c = uStack_10c;
    uVar2 = AEMath::Sinf(fStack_94);
    local_f0 = AEMath::Cosf(extraout_s0);
    local_c8 = 0x3f800000;
    uStack_ec = uVar2 ^ 0x80000000;
    local_e0 = uVar2;
    uStack_dc = local_f0;
    AEMath::operator*(aMStack_16c,(Matrix *)&local_f0);
    AEMath::Matrix::operator=((Matrix *)local_130,aMStack_16c);
    AEMath::operator*(aMStack_16c,(Matrix *)local_b0);
    AEMath::Matrix::operator=((Matrix *)local_130,aMStack_16c);
    SetWorldViewMatrix((Matrix *)this);
    glDisable(0xb44);
    MeshDraw(*(Engine **)(this + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(this + 0x150) + param_1 * 4));
    param_6 = (float)glEnable(0xb44);
  }
  if (*piVar3 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_6);
  }
  return;
}

```

## target disasm
```
  00088538: push {r4,r5,r6,r7,lr}
  0008853a: add r7,sp,#0xc
  0008853c: push {r7,r8,r9,r10,r11}
  00088540: vpush {d8}
  00088544: sub sp,#0x148
  00088546: mov r8,r0
  00088548: ldr r0,[0x0008880c]
  0008854a: mov r6,r2
  0008854c: mov r2,r1
  0008854e: add r0,pc
  00088550: mov r4,r3
  00088552: ldr r1,[r0,#0x0]
  00088554: ldr r0,[r1,#0x0]
  00088556: str r0,[sp,#0x144]
  00088558: ldr.w r0,[r8,#0x14c]
  0008855c: cmp r0,r2
  0008855e: bls.w 0x000887ca
  00088562: ldrd r9,r11,[r7,#0x8]
  00088566: mov r5,r2
  00088568: ldr.w r0,[r8,#0x150]
  0008856c: str r1,[sp,#0x8]
  0008856e: str r2,[sp,#0x4]
  00088570: ldr.w r0,[r0,r2,lsl #0x2]
  00088574: mov.w r2,#0xffffffff
  00088578: ldr.w r10,[r7,#0x18]
  0008857c: vldr.32 s16,[r7,#0x10]
  00088580: ldr r1,[r0,#0x4]
  00088582: mov r0,r8
  00088584: blx 0x0006fa6c
  00088588: ldr.w r0,[r8,#0x150]
  0008858c: vmov s0,r9
  00088590: mov.w r12,#0x1
  00088594: vmov s2,r11
  00088598: vmov.f32 s4,0x3f800000
  0008859c: ldr.w r3,[r0,r5,lsl #0x2]
  000885a0: movs r0,#0x0
  000885a2: vmov s14,r4
  000885a6: vmov s12,r6
  000885aa: add r5,sp,#0x108
  000885ac: vmov.i32 q8,#0x0
  000885b0: add r4,sp,#0xc8
  000885b2: ldr r2,[r3,#0x0]
  000885b4: vcvt.f32.s32 s0,s0
  000885b8: strb.w r12,[r3,#0x14]
  000885bc: ldr r1,[r2,#0x4]
  000885be: vcvt.f32.s32 s2,s2
  000885c2: vstr.32 s0,[r1,#0xc]
  000885c6: strd r0,r0,[r1,#0x10]
  000885ca: vstr.32 s0,[r1,#0x18]
  000885ce: vstr.32 s2,[r1,#0x1c]
  000885d2: strd r0,r0,[r1,#0x20]
  000885d6: vstr.32 s2,[r1,#0x28]
  000885da: str r0,[r1,#0x2c]
  000885dc: strd r0,r0,[r1,#0x0]
  000885e0: str r0,[r1,#0x8]
  000885e2: ldrh r1,[r3,#0xa]
  000885e4: vmov s6,r1
  000885e8: vcvt.f32.u32 s6,s6
  000885ec: ldrh r1,[r3,#0x8]
  000885ee: vmov s8,r1
  000885f2: vcvt.f32.u32 s8,s8
  000885f6: ldrh r1,[r3,#0xc]
  000885f8: vdiv.f32 s6,s4,s6
  000885fc: vmov s10,r1
  00088600: vdiv.f32 s4,s4,s8
  00088604: vcvt.f32.u32 s10,s10
  00088608: ldrh r1,[r3,#0xe]
  0008860a: vmov s8,r1
  0008860e: sxth r1,r6
  00088610: vmov s1,r1
  00088614: ldr r1,[r7,#0x20]
  00088616: vcvt.f32.u32 s8,s8
  0008861a: ldr r6,[r7,#0x14]
  0008861c: vcvt.f32.s32 s12,s12
  00088620: add r1,r10
  00088622: vcvt.f32.s32 s14,s14
  00088626: vcvt.f32.s32 s1,s1
  0008862a: ldr r3,[r2,#0x8]
  0008862c: vadd.f32 s0,s0,s10
  00088630: vadd.f32 s2,s2,s8
  00088634: vadd.f32 s10,s12,s10
  00088638: vadd.f32 s8,s14,s8
  0008863c: vadd.f32 s0,s0,s1
  00088640: vadd.f32 s2,s2,s14
  00088644: vmul.f32 s10,s10,s4
  00088648: vmul.f32 s8,s8,s6
  0008864c: vmul.f32 s4,s0,s4
  00088650: vmov s0,r1
  00088654: vmul.f32 s2,s2,s6
  00088658: vstr.32 s10,[r3]
  0008865c: vstr.32 s8,[r3,#0x4]
  00088660: vstr.32 s4,[r3,#0x8]
  00088664: vstr.32 s8,[r3,#0xc]
  00088668: vstr.32 s4,[r3,#0x10]
  0008866c: vstr.32 s2,[r3,#0x14]
  00088670: vstr.32 s10,[r3,#0x18]
  00088674: vstr.32 s2,[r3,#0x1c]
  00088678: adr r3,[0x887e8]
  0008867a: ldr r1,[r2,#0x2c]
  0008867c: ldr r2,[r7,#0x1c]
  0008867e: vld1.64 {d18,d19},[r3]
  00088682: adr r3,[0x887f8]
  00088684: add r2,r6
  00088686: rsbs r6,r6
  00088688: vld1.64 {d20,d21},[r3]
  0008868c: rsb.w r3,r10,#0x0
  00088690: vmov s2,r2
  00088694: mov.w r10,#0x3f800000
  00088698: vmov s4,r3
  0008869c: mov.w r3,#0x20000
  000886a0: ldr r2,[0x00088808]
  000886a2: vmov s6,r6
  000886a6: str r3,[r1,#0x0]
  000886a8: orr r6,r5,#0x4
  000886ac: str.w r12,[r1,#0x4]
  000886b0: orr r3,r4,#0x4
  000886b4: str r2,[r1,#0x8]
  000886b6: add.w r1,r5,#0x18
  000886ba: vst1.32 {d16,d17},[r6]
  000886be: add r6,sp,#0x48
  000886c0: vst1.64 {d16,d17},[r1]
  000886c4: add.w r1,r5,#0x28
  000886c8: add r5,sp,#0x88
  000886ca: vst1.64 {d20,d21},[r1]
  000886ce: add.w r1,r4,#0x18
  000886d2: vst1.32 {d16,d17},[r3]
  000886d6: vst1.64 {d16,d17},[r1]
  000886da: add.w r1,r4,#0x28
  000886de: vmov r4,s16
  000886e2: vst1.64 {d20,d21},[r1]
  000886e6: add.w r1,r5,#0x18
  000886ea: vst1.64 {d16,d17},[r1]
  000886ee: add.w r1,r5,#0x2c
  000886f2: str.w r10,[sp,#0x108]
  000886f6: str.w r10,[sp,#0x11c]
  000886fa: str.w r10,[sp,#0xc8]
  000886fe: str.w r10,[sp,#0xdc]
  00088702: str.w r10,[sp,#0x140]
  00088706: str.w r10,[sp,#0x100]
  0008870a: vst1.32 {d18,d19},[r1]
  0008870e: strd r0,r0,[sp,#0x90]
  00088712: orr r0,r6,#0x4
  00088716: vst1.32 {d16,d17},[r0]
  0008871a: add.w r0,r6,#0x18
  0008871e: vst1.64 {d16,d17},[r0]
  00088722: add.w r0,r6,#0x28
  00088726: vst1.64 {d20,d21},[r0]
  0008872a: vcvt.f32.s32 s6,s6
  0008872e: vcvt.f32.s32 s4,s4
  00088732: mov r0,r4
  00088734: vcvt.f32.s32 s2,s2
  00088738: vcvt.f32.s32 s0,s0
  0008873c: str.w r10,[sp,#0x48]
  00088740: str.w r10,[sp,#0x5c]
  00088744: str.w r10,[sp,#0x80]
  00088748: vstr.32 s6,[sp,#0x114]
  0008874c: vstr.32 s4,[sp,#0x124]
  00088750: vstr.32 s2,[sp,#0xd4]
  00088754: vstr.32 s0,[sp,#0xe4]
  00088758: blx 0x0006f1a8
  0008875c: mov r9,r0
  0008875e: eor r11,r0,#0x80000000
  00088762: mov r0,r4
  00088764: blx 0x0006f49c
  00088768: add r4,sp,#0xc
  0008876a: add r2,sp,#0x108
  0008876c: strd r0,r11,[sp,#0x88]
  00088770: mov r1,r5
  00088772: strd r9,r0,[sp,#0x98]
  00088776: mov r0,r4
  00088778: str.w r10,[sp,#0xb0]
  0008877c: blx 0x0006f82c
  00088780: mov r0,r6
  00088782: mov r1,r4
  00088784: blx 0x0006f148
  00088788: add r4,sp,#0xc
  0008878a: add r1,sp,#0xc8
  0008878c: mov r2,r6
  0008878e: mov r0,r4
  00088790: blx 0x0006f82c
  00088794: mov r0,r6
  00088796: mov r1,r4
  00088798: blx 0x0006f148
  0008879c: mov r0,r8
  0008879e: mov r1,r6
  000887a0: blx 0x0006f55c
  000887a4: movw r0,#0xb44
  000887a8: blx 0x0006edc4
  000887ac: ldr.w r1,[r8,#0x150]
  000887b0: ldr r2,[sp,#0x4]
  000887b2: ldr.w r0,[r8,#0x34]
  000887b6: ldr.w r1,[r1,r2,lsl #0x2]
  000887ba: ldr r1,[r1,#0x0]
  000887bc: blx 0x0006f568
  000887c0: movw r0,#0xb44
  000887c4: blx 0x0006edb8
  000887c8: ldr r1,[sp,#0x8]
  000887ca: ldr r0,[sp,#0x144]
  000887cc: ldr r1,[r1,#0x0]
  000887ce: subs r0,r1,r0
  000887d0: itttt eq
  000887d2: add.eq sp,#0x148
  000887d4: vpop.eq {d8}
  000887d8: add.eq sp,#0x4
  000887da: pop.eq.w {r8,r9,r10,r11}
  000887de: it eq
  000887e0: pop.eq {r4,r5,r6,r7,pc}
  000887e2: blx 0x0006e824
```
