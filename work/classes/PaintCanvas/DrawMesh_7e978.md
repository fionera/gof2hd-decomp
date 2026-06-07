# PaintCanvas::DrawMesh
elf 0x7e978 body 1262
Sig: undefined __thiscall DrawMesh(PaintCanvas * this, Mesh * param_1, Matrix * param_2, Matrix * param_3, uint param_4, Matrix * param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix&,
   AbyssEngine::AEMath::Matrix&, unsigned int, AbyssEngine::AEMath::Matrix&) */

void __thiscall
AbyssEngine::PaintCanvas::DrawMesh
          (PaintCanvas *this,Mesh *param_1,Matrix *param_2,Matrix *param_3,uint param_4,
          Matrix *param_5)

{
  uint uVar1;
  byte bVar2;
  undefined8 uVar3;
  int *piVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  code *pcVar9;
  uint in_fpscr;
  uint uVar10;
  float fVar11;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s2;
  float extraout_s2_00;
  undefined8 uVar12;
  longlong lVar13;
  float extraout_s3;
  float fVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined8 uVar18;
  undefined1 auStack_2b8 [60];
  undefined1 auStack_27c [60];
  undefined1 auStack_240 [60];
  Matrix aMStack_204 [60];
  Matrix aMStack_1c8 [12];
  undefined4 local_1bc;
  undefined4 local_1ac;
  undefined4 local_19c;
  float local_188;
  float fStack_184;
  float local_180;
  undefined4 local_174;
  undefined4 local_170;
  undefined4 uStack_16c;
  undefined4 uStack_168;
  undefined4 uStack_164;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined4 local_150;
  undefined4 local_148 [5];
  undefined4 local_134;
  undefined4 local_130;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined4 local_110;
  undefined4 local_108 [5];
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined4 local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 uStack_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  int local_4c;
  
  piVar4 = *(int **)(DAT_0008ed10 + 0x8e996);
  local_4c = *piVar4;
  __aeabi_memcpy(&local_88,param_2,0x3c);
  __aeabi_memcpy(&local_c8,param_5,0x3c);
  uVar12 = DAT_0008ee78;
  uVar3 = DAT_0008ee70;
  uVar8 = param_4;
  if (*(int *)(param_1 + 0x34) != 0) {
    uVar7 = 0;
    uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar16 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar17 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar5 = (undefined4 *)((uint)local_108 | 4);
    *puVar5 = 0;
    puVar5[1] = uVar15;
    puVar5[2] = uVar16;
    puVar5[3] = uVar17;
    local_f0 = 0;
    uStack_e0 = uVar3;
    uStack_d8 = uVar12;
    puVar5 = (undefined4 *)((uint)local_148 | 4);
    *puVar5 = 0;
    puVar5[1] = uVar15;
    puVar5[2] = uVar16;
    puVar5[3] = uVar17;
    local_130 = 0;
    local_108[0] = 0x3f800000;
    local_f4 = 0x3f800000;
    uStack_120 = uVar3;
    uStack_118 = uVar12;
    local_148[0] = 0x3f800000;
    local_134 = 0x3f800000;
    local_d0 = 0x3f800000;
    local_110 = 0x3f800000;
    uStack_12c = uVar15;
    uStack_128 = uVar16;
    uStack_124 = uVar17;
    uStack_ec = uVar15;
    uStack_e8 = uVar16;
    uStack_e4 = uVar17;
    AEMath::MatrixSetTranslation
              ((Matrix *)&local_188,*(float *)(param_1 + 0x54),extraout_s1,extraout_s2);
    AEMath::MatrixSetTranslation
              ((Matrix *)&local_188,-*(float *)(param_1 + 0x54),extraout_s1_00,extraout_s2_00);
    puVar5 = (undefined4 *)((uint)&local_188 | 4);
    *puVar5 = uVar7;
    puVar5[1] = uVar15;
    puVar5[2] = uVar16;
    puVar5[3] = uVar17;
    uStack_160 = uVar3;
    uStack_158 = uVar12;
    local_188 = 1.0;
    local_174 = 0x3f800000;
    local_150 = 0x3f800000;
    local_170 = uVar7;
    uStack_16c = uVar15;
    uStack_168 = uVar16;
    uStack_164 = uVar17;
    AEMath::MatrixGetPosition(aMStack_204);
    AEMath::MatrixSetTranslation(aMStack_1c8,(Vector *)&local_188);
    __aeabi_memcpy(aMStack_1c8,*(int *)(param_1 + 0x34) + 0x5c,0x3c);
    pcVar9 = *(code **)(DAT_0008ee6c + 0x8eaa6);
    local_1bc = 0;
    local_1ac = 0;
    local_19c = 0;
    (*pcVar9)(auStack_2b8,param_2,(Matrix *)&local_188);
    (*pcVar9)(auStack_27c,auStack_2b8,local_148);
    (*pcVar9)(auStack_240,auStack_27c,aMStack_1c8);
    (*pcVar9)(aMStack_204,auStack_240,local_108);
    AEMath::Matrix::operator=((Matrix *)&local_88,aMStack_204);
    uVar7 = *(undefined4 *)(*(int *)(param_1 + 0x34) + 0x48);
    AEMath::Matrix::operator=((Matrix *)&local_c8,(Matrix *)(*(int *)(param_1 + 0x34) + 0x98));
    uVar8 = (int)(short)(ushort)(byte)((uint)uVar7 >> 8) * (int)(short)(ushort)(byte)(param_4 >> 8)
            & 0xffffff00U |
            (uint)((int)(short)((ushort)uVar7 & 0xff) * (int)(short)((ushort)param_4 & 0xff)) >> 8 |
            ((int)(short)(ushort)(byte)((uint)uVar7 >> 0x18) *
             (int)(short)(ushort)(byte)(param_4 >> 0x18) & 0xffffff00U) << 0x10 |
            ((int)(short)(ushort)(byte)((uint)uVar7 >> 0x10) *
             (int)(short)(ushort)(byte)(param_4 >> 0x10) & 0xffffff00U) << 8;
  }
  AEMath::MatrixTransformVector((Matrix *)local_148,(Vector *)&local_88);
  local_188 = *(float *)(param_1 + 0x48);
  fStack_184 = local_188;
  local_180 = local_188;
  AEMath::MatrixRotateVector((Matrix *)local_108,(Vector *)&local_88);
  AEMath::Vector::operator=((Vector *)&local_188,(Vector *)local_108);
  uVar3 = CONCAT44(fStack_184,local_188);
  uVar18 = FloatVectorNeg(uVar3,2,2);
  uVar12 = FloatVectorCompareGreaterThan(uVar3,0,2);
  lVar13 = VectorBitwiseSelect(uVar12,uVar3,uVar18);
  if ((float)((ulonglong)lVar13 >> 0x20) < (float)lVar13) {
    lVar13 = lVar13 << 0x20;
  }
  fVar11 = -local_180;
  if (0.0 < local_180) {
    fVar11 = local_180;
  }
  if (*(short *)(param_1 + 2) != 0) {
    fVar14 = (float)((ulonglong)lVar13 >> 0x20);
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar14 < fVar11) << 0x1f |
            (uint)(fVar14 == fVar11) << 0x1e;
    uVar10 = uVar1 | (uint)(NAN(fVar14) || NAN(fVar11)) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar10 >> 0x1c) & 1)) {
      fVar11 = fVar14;
    }
    iVar6 = CameraIsSphereinViewFrustum
                      (this,(Vector *)local_148,fVar11 * *(float *)(param_1 + 0x4c));
    if (iVar6 == 0) {
      *(int *)(this + 4) = *(int *)(this + 4) + 1;
      goto LAB_0008ee4c;
    }
    if (*(short *)(param_1 + 2) != 0) {
      if (*(int *)(param_1 + 0x30) == 0) {
        fVar11 = (float)VectorUnsignedToFloat(uVar8 >> 0x18,(byte)(uVar10 >> 0x16) & 3);
        fVar14 = (float)VectorUnsignedToFloat((uVar8 & 0xffffff) >> 0x10,(byte)(uVar10 >> 0x16) & 3)
        ;
        VectorUnsignedToFloat((uVar8 & 0xffff) >> 8,(byte)(uVar10 >> 0x16) & 3);
        VectorUnsignedToFloat(uVar8 & 0xff,(byte)(uVar10 >> 0x16) & 3);
        Engine::SetColor((*(float *)(this + 0x1fc) * fVar11) / DAT_0008ee80,extraout_s1_01,
                         (*(float *)(this + 0x200) * fVar14) / DAT_0008ee80,extraout_s3);
        AEMath::operator*((Matrix *)local_108,param_3);
        SetWorldViewMatrix((Matrix *)this);
        Engine::SetModelMatrix(*(Matrix **)(this + 0x34));
        Engine::SetUVMatrix(*(Engine **)(this + 0x34),(Matrix *)&local_c8);
        MeshDraw(*(Engine **)(this + 0x34),param_1);
        Engine::ResetUVMatrix(*(Engine **)(this + 0x34));
      }
      else {
        ArrayAddCached<AbyssEngine::Mesh*>(param_1,(Array *)(*(int *)(param_1 + 0x30) + 0x44));
        pcVar9 = *(code **)(DAT_0008ee84 + 0x8ec24);
        (*pcVar9)(local_88,local_84,local_80,local_7c,local_78,local_74,local_70,local_6c,uStack_68,
                  local_64,local_60,uStack_5c,local_58,uStack_54,uStack_50,
                  *(int *)(param_1 + 0x30) + 0x2c);
        (*pcVar9)(local_c8,local_c4,local_c0,local_bc,local_b8,local_b4,local_b0,uStack_ac,local_a8,
                  local_a4,uStack_a0,uStack_9c,local_98,uStack_94,uStack_90,
                  *(int *)(param_1 + 0x30) + 0x38);
        (*pcVar9)(*(undefined4 *)param_3,*(undefined4 *)(param_3 + 4),*(undefined4 *)(param_3 + 8),
                  *(undefined4 *)(param_3 + 0xc),*(undefined4 *)(param_3 + 0x10),
                  *(undefined4 *)(param_3 + 0x14),*(undefined4 *)(param_3 + 0x18),
                  *(undefined4 *)(param_3 + 0x1c),*(undefined4 *)(param_3 + 0x20),
                  *(undefined4 *)(param_3 + 0x24),*(undefined4 *)(param_3 + 0x28),
                  *(undefined4 *)(param_3 + 0x2c),*(undefined4 *)(param_3 + 0x30),
                  *(undefined4 *)(param_3 + 0x34),*(undefined4 *)(param_3 + 0x38),
                  *(int *)(param_1 + 0x30) + 0x5c);
        ArrayAddCached<unsigned_int>(uVar8,(Array *)(*(int *)(param_1 + 0x30) + 0x50));
      }
    }
  }
  if (*(int *)(param_1 + 0x34) != 0) {
    uVar8 = *(uint *)(*(int *)(param_1 + 0x34) + 0x3c);
    while (uVar8 = uVar8 - 1, uVar8 < 0x80000000) {
      DrawMesh(this,*(Mesh **)(*(int *)(*(int *)(param_1 + 0x34) + 0x40) + uVar8 * 4),
               (Matrix *)&local_88,param_3,param_4,(Matrix *)&local_c8);
    }
    for (uVar8 = 0; uVar8 < *(uint *)(*(int *)(param_1 + 0x34) + 0x4c); uVar8 = uVar8 + 1) {
      if ((*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) &&
         (iVar6 = Transform::InCameraVF
                            (*(Transform **)(*(int *)(*(int *)(param_1 + 0x34) + 0x50) + uVar8 * 4),
                             param_2,*(Camera **)
                                      (*(int *)(this + 0x168) + *(uint *)(this + 0x170) * 4)),
         iVar6 != 0)) {
        DrawTransform(this,*(Transform **)(*(int *)(*(int *)(param_1 + 0x34) + 0x50) + uVar8 * 4),
                      (Matrix *)&local_88,param_3);
      }
    }
  }
LAB_0008ee4c:
  if (*piVar4 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008e978: push {r4,r5,r6,r7,lr}
  0008e97a: add r7,sp,#0xc
  0008e97c: push {r7,r8,r9,r10,r11}
  0008e980: vpush {d8,d9,d10,d11}
  0008e984: sub.w sp,sp,#0x2c8
  0008e988: str r3,[sp,#0x54]
  0008e98a: mov r8,r0
  0008e98c: ldr r0,[0x0008ed10]
  0008e98e: mov r10,r1
  0008e990: mov r1,r2
  0008e992: add r0,pc
  0008e994: ldr r0,[r0,#0x0]
  0008e996: str r0,[sp,#0x4c]
  0008e998: ldr r0,[r0,#0x0]
  0008e99a: str r0,[sp,#0x2c4]
  0008e99c: add r0,sp,#0x288
  0008e99e: str r2,[sp,#0x50]
  0008e9a0: movs r2,#0x3c
  0008e9a2: blx 0x0006f1e4
  0008e9a6: ldr r1,[r7,#0xc]
  0008e9a8: add r0,sp,#0x248
  0008e9aa: movs r2,#0x3c
  0008e9ac: blx 0x0006f1e4
  0008e9b0: ldr r4,[r7,#0x8]
  0008e9b2: ldr.w r0,[r10,#0x34]
  0008e9b6: cmp r0,#0x0
  0008e9b8: mov r6,r4
  0008e9ba: beq.w 0x0008eb2a
  0008e9be: adr.w r0,0x8ee70
  0008e9c2: vmov.i32 q4,#0x0
  0008e9c6: add r1,sp,#0x208
  0008e9c8: vld1.64 {d10,d11},[r0]
  0008e9cc: orr r0,r1,#0x4
  0008e9d0: add r2,sp,#0x1c8
  0008e9d2: mov.w r5,#0x3f800000
  0008e9d6: mov r9,r4
  0008e9d8: vst1.32 {d8,d9},[r0]
  0008e9dc: add.w r0,r1,#0x18
  0008e9e0: mov r4,r2
  0008e9e2: vst1.64 {d8,d9},[r0]
  0008e9e6: add.w r0,r1,#0x28
  0008e9ea: vst1.64 {d10,d11},[r0]
  0008e9ee: orr r0,r2,#0x4
  0008e9f2: vst1.32 {d8,d9},[r0]
  0008e9f6: add.w r0,r2,#0x18
  0008e9fa: vst1.64 {d8,d9},[r0]
  0008e9fe: add.w r0,r2,#0x28
  0008ea02: str r5,[sp,#0x208]
  0008ea04: str r5,[sp,#0x21c]
  0008ea06: vst1.64 {d10,d11},[r0]
  0008ea0a: str r5,[sp,#0x1c8]
  0008ea0c: str r5,[sp,#0x1dc]
  0008ea0e: str r5,[sp,#0x240]
  0008ea10: str r5,[sp,#0x200]
  0008ea12: ldr.w r2,[r10,#0x50]
  0008ea16: ldr.w r0,[r10,#0x58]
  0008ea1a: vldr.32 s0,[r10,#0x54]
  0008ea1e: eor r2,r2,#0x80000000
  0008ea22: eor r3,r0,#0x80000000
  0008ea26: add r0,sp,#0x188
  0008ea28: vstr.32 s0,[sp]
  0008ea2c: blx 0x0006f820
  0008ea30: vldr.32 s0,[r10,#0x54]
  0008ea34: add r6,sp,#0x188
  0008ea36: ldr.w r3,[r10,#0x58]
  0008ea3a: mov r1,r4
  0008ea3c: vneg.f32 s0,s0
  0008ea40: ldr.w r2,[r10,#0x50]
  0008ea44: mov r0,r6
  0008ea46: mov r4,r9
  0008ea48: vstr.32 s0,[sp]
  0008ea4c: blx 0x0006f820
  0008ea50: orr r0,r6,#0x4
  0008ea54: vst1.32 {d8,d9},[r0]
  0008ea58: add.w r0,r6,#0x18
  0008ea5c: vst1.64 {d8,d9},[r0]
  0008ea60: add.w r0,r6,#0x28
  0008ea64: vst1.64 {d10,d11},[r0]
  0008ea68: str r5,[sp,#0x188]
  0008ea6a: str r5,[sp,#0x19c]
  0008ea6c: str r5,[sp,#0x1c0]
  0008ea6e: add r5,sp,#0x10c
  0008ea70: ldr.w r0,[r10,#0x34]
  0008ea74: add.w r1,r0,#0x5c
  0008ea78: mov r0,r5
  0008ea7a: blx 0x0006f16c
  0008ea7e: add r0,sp,#0x148
  0008ea80: mov r1,r6
  0008ea82: mov r2,r5
  0008ea84: blx 0x0006f1d8
  0008ea88: ldr.w r0,[r10,#0x34]
  0008ea8c: movs r2,#0x3c
  0008ea8e: add.w r1,r0,#0x5c
  0008ea92: add r0,sp,#0x148
  0008ea94: blx 0x0006f1e4
  0008ea98: ldr r0,[0x0008ee6c]
  0008ea9a: add.w r11,sp,#0x58
  0008ea9e: ldr r1,[sp,#0x50]
  0008eaa0: mov r2,r6
  0008eaa2: add r0,pc
  0008eaa4: ldr.w r9,[r0,#0x0]
  0008eaa8: movs r0,#0x0
  0008eaaa: str r0,[sp,#0x154]
  0008eaac: str r0,[sp,#0x164]
  0008eaae: str r0,[sp,#0x174]
  0008eab0: mov r0,r11
  0008eab2: blx r9
  0008eab4: add r5,sp,#0x94
  0008eab6: add r2,sp,#0x1c8
  0008eab8: mov r1,r11
  0008eaba: mov r0,r5
  0008eabc: blx r9
  0008eabe: add r6,sp,#0xd0
  0008eac0: add r2,sp,#0x148
  0008eac2: mov r1,r5
  0008eac4: mov r0,r6
  0008eac6: blx r9
  0008eac8: add r5,sp,#0x10c
  0008eaca: add r2,sp,#0x208
  0008eacc: mov r1,r6
  0008eace: mov r0,r5
  0008ead0: blx r9
  0008ead2: add r0,sp,#0x288
  0008ead4: mov r1,r5
  0008ead6: blx 0x0006f148
  0008eada: ldr.w r0,[r10,#0x34]
  0008eade: add.w r1,r0,#0x98
  0008eae2: ldr r5,[r0,#0x48]
  0008eae4: add r0,sp,#0x248
  0008eae6: blx 0x0006f148
  0008eaea: uxtb r6,r4
  0008eaec: uxtb r0,r5
  0008eaee: ubfx r12,r4,#0x10,#0x8
  0008eaf2: ubfx r3,r4,#0x8,#0x8
  0008eaf6: smulbb r0,r0,r6
  0008eafa: ubfx r6,r5,#0x8,#0x8
  0008eafe: lsrs r1,r4,#0x18
  0008eb00: lsrs r2,r5,#0x18
  0008eb02: smulbb r1,r2,r1
  0008eb06: ubfx r2,r5,#0x10,#0x8
  0008eb0a: smulbb r3,r6,r3
  0008eb0e: smulbb r2,r2,r12
  0008eb12: bic r1,r1,#0xff
  0008eb16: bic r3,r3,#0xff
  0008eb1a: orr.w r0,r3,r0, lsr #0x8
  0008eb1e: orr.w r0,r0,r1, lsl #0x10
  0008eb22: bic r1,r2,#0xff
  0008eb26: orr.w r6,r0,r1, lsl #0x8
  0008eb2a: add.w r9,sp,#0x288
  0008eb2e: add.w r2,r10,#0x3c
  0008eb32: add r0,sp,#0x1c8
  0008eb34: mov r1,r9
  0008eb36: blx 0x0006f688
  0008eb3a: ldr.w r0,[r10,#0x48]
  0008eb3e: add r5,sp,#0x188
  0008eb40: add.w r11,sp,#0x208
  0008eb44: mov r1,r9
  0008eb46: mov r2,r5
  0008eb48: strd r0,r0,[sp,#0x188]
  0008eb4c: str r0,[sp,#0x190]
  0008eb4e: mov r0,r11
  0008eb50: blx 0x0006f694
  0008eb54: mov r0,r5
  0008eb56: mov r1,r11
  0008eb58: blx 0x0006eb3c
  0008eb5c: vldr.64 d16,[sp,#0x188]
  0008eb60: vneg.f32 d17,d16
  0008eb64: vldr.32 s4,[sp,#0x190]
  0008eb68: vcgt.f32 d1,d16,#0
  0008eb6c: vbsl d1,d16,d17
  0008eb70: vneg.f32 s0,s4
  0008eb74: vcmpe.f32 s2,s3
  0008eb78: vmrs apsr,fpscr
  0008eb7c: vcmpe.f32 s4,#0
  0008eb80: it gt
  0008eb82: vmov.gt.f32 s3,s2
  0008eb86: vmrs apsr,fpscr
  0008eb8a: it gt
  0008eb8c: vmov.gt.f32 s0,s4
  0008eb90: ldrh.w r0,[r10,#0x2]
  0008eb94: cmp r0,#0x0
  0008eb96: beq.w 0x0008edca
  0008eb9a: vcmpe.f32 s3,s0
  0008eb9e: add r1,sp,#0x1c8
  0008eba0: vmrs apsr,fpscr
  0008eba4: mov r0,r8
  0008eba6: it gt
  0008eba8: vmov.gt.f32 s0,s3
  0008ebac: vldr.32 s2,[r10,#0x4c]
  0008ebb0: vmul.f32 s0,s0,s2
  0008ebb4: vmov r2,s0
  0008ebb8: blx 0x0006ff94
  0008ebbc: cmp r0,#0x0
  0008ebbe: beq.w 0x0008ed02
  0008ebc2: ldrh.w r0,[r10,#0x2]
  0008ebc6: cmp r0,#0x0
  0008ebc8: beq.w 0x0008edca
  0008ebcc: ldr.w r0,[r10,#0x30]
  0008ebd0: cmp r0,#0x0
  0008ebd2: beq.w 0x0008ed14
  0008ebd6: add.w r1,r0,#0x44
  0008ebda: mov r0,r10
  0008ebdc: blx 0x0006ffa0
  0008ebe0: ldr r0,[sp,#0x288]
  0008ebe2: add r2,sp,#0x2b8
  0008ebe4: str r0,[sp,#0x48]
  0008ebe6: add r4,sp,#0x20
  0008ebe8: ldr r0,[sp,#0x28c]
  0008ebea: str r0,[sp,#0x44]
  0008ebec: ldr r0,[sp,#0x290]
  0008ebee: str r0,[sp,#0x40]
  0008ebf0: ldr r0,[sp,#0x294]
  0008ebf2: str r0,[sp,#0x3c]
  0008ebf4: ldr r0,[sp,#0x298]
  0008ebf6: str r0,[sp,#0x38]
  0008ebf8: ldr r0,[sp,#0x29c]
  0008ebfa: str r0,[sp,#0x34]
  0008ebfc: ldr r0,[sp,#0x2a0]
  0008ebfe: str r0,[sp,#0x30]
  0008ec00: ldr.w r5,[r10,#0x30]
  0008ec04: ldrd r11,r9,[sp,#0x2a4]
  0008ec08: ldr.w lr,[sp,#0x2ac]
  0008ec0c: adds r5,#0x2c
  0008ec0e: ldrd r12,r3,[sp,#0x2b0]
  0008ec12: ldmia r2,{r0,r1,r2}
  0008ec14: stmia r4!,{r0,r1,r2,r5}
  0008ec16: strd r12,r3,[sp,#0x18]
  0008ec1a: ldr r0,[0x0008ee84]
  0008ec1c: str.w lr,[sp,#0x14]
  0008ec20: add r0,pc
  0008ec22: strd r11,r9,[sp,#0xc]
  0008ec26: ldr r4,[r0,#0x0]
  0008ec28: ldr r0,[sp,#0x30]
  0008ec2a: str r0,[sp,#0x8]
  0008ec2c: ldr r0,[sp,#0x34]
  0008ec2e: str r0,[sp,#0x4]
  0008ec30: ldr r0,[sp,#0x38]
  0008ec32: str r0,[sp,#0x0]
  0008ec34: ldrd r1,r0,[sp,#0x44]
  0008ec38: ldrd r3,r2,[sp,#0x3c]
  0008ec3c: blx r4
  0008ec3e: ldr r0,[sp,#0x248]
  0008ec40: add.w lr,sp,#0x26c
  0008ec44: str r0,[sp,#0x48]
  0008ec46: add r2,sp,#0x278
  0008ec48: ldr r0,[sp,#0x24c]
  0008ec4a: str r0,[sp,#0x44]
  0008ec4c: ldr r0,[sp,#0x250]
  0008ec4e: str r0,[sp,#0x40]
  0008ec50: ldr r0,[sp,#0x254]
  0008ec52: str r0,[sp,#0x3c]
  0008ec54: ldr r0,[sp,#0x258]
  0008ec56: str r0,[sp,#0x38]
  0008ec58: ldr r0,[sp,#0x25c]
  0008ec5a: str r0,[sp,#0x34]
  0008ec5c: ldr.w r5,[r10,#0x30]
  0008ec60: ldrd r9,r11,[sp,#0x260]
  0008ec64: ldr r0,[sp,#0x268]
  0008ec66: adds r5,#0x38
  0008ec68: str r0,[sp,#0x30]
  0008ec6a: ldm.w lr,{r3,r12,lr}
  0008ec6e: ldmia r2,{r0,r1,r2}
  0008ec70: str r0,[sp,#0x20]
  0008ec72: ldr r0,[sp,#0x38]
  0008ec74: str r0,[sp,#0x0]
  0008ec76: ldr r0,[sp,#0x34]
  0008ec78: str r1,[sp,#0x24]
  0008ec7a: add r1,sp,#0x4
  0008ec7c: str r2,[sp,#0x28]
  0008ec7e: str r5,[sp,#0x2c]
  0008ec80: stm r1,{r0,r9,r11}
  0008ec84: add r1,sp,#0x10
  0008ec86: ldr r0,[sp,#0x30]
  0008ec88: stm r1,{r0,r3,r12,lr}
  0008ec8c: ldrd r1,r0,[sp,#0x44]
  0008ec90: ldrd r3,r2,[sp,#0x3c]
  0008ec94: blx r4
  0008ec96: ldr r2,[sp,#0x54]
  0008ec98: add.w lr,r2,#0x10
  0008ec9c: ldr r0,[r2,#0x0]
  0008ec9e: str r0,[sp,#0x48]
  0008eca0: ldr r0,[r2,#0x4]
  0008eca2: str r0,[sp,#0x44]
  0008eca4: ldr r0,[r2,#0x8]
  0008eca6: str r0,[sp,#0x40]
  0008eca8: ldr r0,[r2,#0xc]
  0008ecaa: str r0,[sp,#0x3c]
  0008ecac: ldm.w lr,{r5,r12,lr}
  0008ecb0: ldrd r9,r11,[r2,#0x1c]
  0008ecb4: ldr r0,[r2,#0x24]
  0008ecb6: str r0,[sp,#0x38]
  0008ecb8: ldr r0,[r2,#0x28]
  0008ecba: str r0,[sp,#0x34]
  0008ecbc: ldr r0,[r2,#0x2c]
  0008ecbe: adds r2,#0x30
  0008ecc0: str r0,[sp,#0x30]
  0008ecc2: ldmia r2,{r0,r1,r2}
  0008ecc4: ldr.w r3,[r10,#0x30]
  0008ecc8: str r0,[sp,#0x20]
  0008ecca: adds r3,#0x5c
  0008eccc: str r1,[sp,#0x24]
  0008ecce: str r2,[sp,#0x28]
  0008ecd0: str r3,[sp,#0x2c]
  0008ecd2: stm sp,{r5,r12,lr}
  0008ecd6: strd r9,r11,[sp,#0xc]
  0008ecda: ldr r0,[sp,#0x38]
  0008ecdc: str r0,[sp,#0x14]
  0008ecde: ldr r0,[sp,#0x34]
  0008ece0: str r0,[sp,#0x18]
  0008ece2: ldr r0,[sp,#0x30]
  0008ece4: str r0,[sp,#0x1c]
  0008ece6: ldrd r1,r0,[sp,#0x44]
  0008ecea: ldrd r3,r2,[sp,#0x3c]
  0008ecee: blx r4
  0008ecf0: ldr.w r0,[r10,#0x30]
  0008ecf4: ldr r4,[r7,#0x8]
  0008ecf6: add.w r1,r0,#0x50
  0008ecfa: mov r0,r6
  0008ecfc: blx 0x0006ffac
  0008ed00: b 0x0008edca
  0008ed02: ldr.w r0,[r8,#0x4]
  0008ed06: adds r0,#0x1
  0008ed08: str.w r0,[r8,#0x4]
  0008ed0c: b 0x0008ee4c
  0008ed14: lsrs r0,r6,#0x18
  0008ed16: vmov s0,r0
  0008ed1a: vcvt.f32.u32 s0,s0
  0008ed1e: vldr.32 s2,[r8,#0x1fc]
  0008ed22: ubfx r0,r6,#0x10,#0x8
  0008ed26: ubfx r1,r6,#0x8,#0x8
  0008ed2a: vmov s4,r0
  0008ed2e: uxtb r0,r6
  0008ed30: vmov s6,r1
  0008ed34: vcvt.f32.u32 s4,s4
  0008ed38: vcvt.f32.u32 s6,s6
  0008ed3c: vmov s8,r0
  0008ed40: vmul.f32 s0,s2,s0
  0008ed44: vcvt.f32.u32 s8,s8
  0008ed48: vldr.32 s2,[r8,#0x200]
  0008ed4c: vldr.32 s10,[r8,#0x204]
  0008ed50: vldr.32 s12,[r8,#0x208]
  0008ed54: vmul.f32 s2,s2,s4
  0008ed58: vldr.32 s4,[pc,#0x124]
  0008ed5c: vmul.f32 s6,s10,s6
  0008ed60: ldr.w r0,[r8,#0x34]
  0008ed64: vdiv.f32 s0,s0,s4
  0008ed68: vdiv.f32 s2,s2,s4
  0008ed6c: vdiv.f32 s6,s6,s4
  0008ed70: vmul.f32 s8,s12,s8
  0008ed74: vmov r1,s0
  0008ed78: vmov r2,s2
  0008ed7c: vmov r3,s6
  0008ed80: vdiv.f32 s4,s8,s4
  0008ed84: vstr.32 s4,[sp]
  0008ed88: blx 0x0006fad8
  0008ed8c: add r6,sp,#0x208
  0008ed8e: add.w r9,sp,#0x288
  0008ed92: ldr r1,[sp,#0x54]
  0008ed94: mov r0,r6
  0008ed96: mov r2,r9
  0008ed98: blx 0x0006f82c
  0008ed9c: mov r0,r8
  0008ed9e: mov r1,r6
  0008eda0: blx 0x0006f55c
  0008eda4: ldr.w r0,[r8,#0x34]
  0008eda8: mov r1,r9
  0008edaa: blx 0x0006ffb8
  0008edae: ldr.w r0,[r8,#0x34]
  0008edb2: add r1,sp,#0x248
  0008edb4: blx 0x0006ffc4
  0008edb8: ldr.w r0,[r8,#0x34]
  0008edbc: mov r1,r10
  0008edbe: blx 0x0006f568
  0008edc2: ldr.w r0,[r8,#0x34]
  0008edc6: blx 0x0006ffd0
  0008edca: ldr.w r0,[r10,#0x34]
  0008edce: cbz r0,0x0008ee4c
  0008edd0: ldr r0,[r0,#0x3c]
  0008edd2: mov r11,r4
  0008edd4: ldr r5,[sp,#0x54]
  0008edd6: add.w r9,sp,#0x288
  0008edda: subs r6,r0,#0x1
  0008eddc: add r4,sp,#0x248
  0008edde: b 0x0008edfa
  0008ede0: ldr.w r0,[r10,#0x34]
  0008ede4: mov r2,r9
  0008ede6: mov r3,r5
  0008ede8: ldr r0,[r0,#0x40]
  0008edea: ldr.w r1,[r0,r6,lsl #0x2]
  0008edee: mov r0,r8
  0008edf0: strd r11,r4,[sp,#0x0]
  0008edf4: blx 0x0006fb74
  0008edf8: subs r6,#0x1
  0008edfa: cmp.w r6,#0xffffffff
  0008edfe: bgt 0x0008ede0
  0008ee00: ldr r4,[sp,#0x50]
  0008ee02: add.w r9,sp,#0x288
  0008ee06: movs r5,#0x0
  0008ee08: b 0x0008ee42
  0008ee0a: ldr.w r2,[r8,#0x164]
  0008ee0e: ldr.w r1,[r8,#0x170]
  0008ee12: cmp r1,r2
  0008ee14: bcs 0x0008ee40
  0008ee16: ldr.w r2,[r8,#0x168]
  0008ee1a: ldr r0,[r0,#0x50]
  0008ee1c: ldr.w r2,[r2,r1,lsl #0x2]
  0008ee20: mov r1,r4
  0008ee22: ldr.w r0,[r0,r5,lsl #0x2]
  0008ee26: blx 0x0006fb44
  0008ee2a: cbz r0,0x0008ee40
  0008ee2c: ldr.w r0,[r10,#0x34]
  0008ee30: mov r2,r9
  0008ee32: ldr r3,[sp,#0x54]
  0008ee34: ldr r0,[r0,#0x50]
  0008ee36: ldr.w r1,[r0,r5,lsl #0x2]
  0008ee3a: mov r0,r8
  0008ee3c: blx 0x0006fb68
  0008ee40: adds r5,#0x1
  0008ee42: ldr.w r0,[r10,#0x34]
  0008ee46: ldr r1,[r0,#0x4c]
  0008ee48: cmp r5,r1
  0008ee4a: bcc 0x0008ee0a
  0008ee4c: ldr r0,[sp,#0x2c4]
  0008ee4e: ldr r1,[sp,#0x4c]
  0008ee50: ldr r1,[r1,#0x0]
  0008ee52: subs r0,r1,r0
  0008ee54: itttt eq
  0008ee56: add.eq.w sp,sp,#0x2c8
  0008ee5a: vpop.eq {d8,d9,d10,d11}
  0008ee5e: add.eq sp,#0x4
  0008ee60: pop.eq.w {r8,r9,r10,r11}
  0008ee64: it eq
  0008ee66: pop.eq {r4,r5,r6,r7,pc}
  0008ee68: blx 0x0006e824
```
