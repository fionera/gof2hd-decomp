# Transform::InternUpdate
elf 0x75048 body 2024
Sig: undefined __stdcall InternUpdate(longlong param_1, bool param_2)

## decompile
```c

/* AbyssEngine::Transform::InternUpdate(long long, bool) */

void AbyssEngine::Transform::InternUpdate(longlong param_1,bool param_2)

{
  bool bVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined4 uVar11;
  int *piVar12;
  int iVar13;
  int *piVar14;
  float fVar15;
  float fVar16;
  undefined4 *puVar17;
  Quaternion *this;
  int iVar18;
  uint uVar19;
  undefined4 extraout_r1;
  undefined4 extraout_r1_00;
  undefined4 extraout_r1_01;
  undefined4 uVar20;
  int in_r3;
  int iVar21;
  uint uVar22;
  uint uVar23;
  Matrix *pMVar24;
  int iVar25;
  code *pcVar26;
  int iVar27;
  int *piVar28;
  BSphere *this_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float fVar29;
  float fVar30;
  float fVar31;
  float fVar32;
  undefined4 uVar33;
  float fVar34;
  int in_stack_00000000;
  undefined1 auStack_2f8 [60];
  undefined1 auStack_2bc [60];
  undefined1 auStack_280 [60];
  Matrix aMStack_244 [60];
  undefined4 local_208 [5];
  undefined4 local_1f4;
  undefined4 local_1f0;
  float fStack_1ec;
  undefined4 uStack_1e8;
  float fStack_1e4;
  undefined8 uStack_1e0;
  undefined8 uStack_1d8;
  undefined4 local_1d0;
  undefined4 local_1c8 [5];
  undefined4 local_1b4;
  undefined4 local_1b0;
  float fStack_1ac;
  undefined4 uStack_1a8;
  float fStack_1a4;
  undefined8 uStack_1a0;
  undefined8 uStack_198;
  undefined4 local_190;
  undefined4 local_188 [5];
  undefined4 local_174;
  undefined4 local_170;
  float fStack_16c;
  undefined4 uStack_168;
  float fStack_164;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined4 local_150;
  undefined4 local_148 [3];
  float local_13c;
  undefined4 local_134;
  undefined4 local_130;
  float fStack_12c;
  undefined4 uStack_128;
  float fStack_124;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined4 local_110;
  undefined4 local_108 [3];
  undefined4 local_fc;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  float fStack_e4;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined4 local_d0;
  undefined4 local_c8 [3];
  undefined4 local_bc;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  float fStack_a4;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined4 local_90;
  undefined4 local_88;
  float fStack_84;
  undefined4 local_80;
  undefined4 local_7c;
  float local_78;
  undefined4 local_74;
  float local_70;
  float fStack_6c;
  undefined4 local_68;
  float local_64;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  int local_4c;
  
  iVar13 = (int)param_1;
  uVar22 = (uint)param_2;
  piVar28 = *(int **)(DAT_000852e0 + 0x85064);
  local_4c = *piVar28;
  if (*(int *)(iVar13 + 0x11c) != 0) {
    uVar23 = *(uint *)(iVar13 + 0x100);
    iVar27 = *(int *)(iVar13 + 0x104);
    iVar18 = (*(int **)(iVar13 + 0x120))[*(int *)(iVar13 + 0x11c) + -1];
    bVar1 = (int)((iVar27 - in_r3) - (uint)(uVar23 < uVar22)) < 0 !=
            (SBORROW4(iVar27,in_r3) != SBORROW4(iVar27 - in_r3,(uint)(uVar23 < uVar22)));
    if (bVar1) {
      iVar27 = in_r3;
    }
    uVar19 = *(uint *)(iVar18 + 0x50);
    iVar18 = *(int *)(iVar18 + 0x54);
    if (bVar1) {
      uVar23 = uVar22;
    }
    if ((int)((iVar18 - iVar27) - (uint)(uVar19 < uVar23)) < 0 !=
        (SBORROW4(iVar18,iVar27) != SBORROW4(iVar18 - iVar27,(uint)(uVar19 < uVar23)))) {
      uVar23 = uVar19;
      iVar27 = iVar18;
    }
    iVar18 = -1;
    piVar12 = *(int **)(iVar13 + 0x120);
    do {
      piVar14 = piVar12;
      iVar21 = *piVar14;
      iVar18 = iVar18 + 1;
      uVar22 = *(uint *)(iVar21 + 0x50);
      iVar25 = *(int *)(iVar21 + 0x54);
      piVar12 = piVar14 + 1;
    } while ((int)((iVar25 - iVar27) - (uint)(uVar22 < uVar23)) < 0 !=
             (SBORROW4(iVar25,iVar27) != SBORROW4(iVar25 - iVar27,(uint)(uVar22 < uVar23))));
    *(int *)(iVar13 + 0x118) = iVar18;
    uStack_58 = DAT_00085870;
    local_60 = DAT_00085868;
    if (iVar18 == 0) {
      fStack_6c = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      local_68 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      local_64 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar17 = (undefined4 *)((uint)&local_88 | 4);
      *puVar17 = 0;
      puVar17[1] = fStack_6c;
      puVar17[2] = local_68;
      puVar17[3] = local_64;
      local_70 = 0.0;
      local_88 = 0x3f800000;
      local_74 = 0x3f800000;
      local_50 = 0x3f800000;
      AEMath::Matrix::operator=((Matrix *)(iVar13 + 0x98),(Matrix *)&local_88);
      if (**(char **)(DAT_000852e8 + 0x852d0) == '\0') {
        *(undefined4 *)(iVar13 + 0xac) = DAT_000852ec;
      }
      goto LAB_00085828;
    }
    if (uVar23 == 0 && iVar27 == 0) {
      this = (Quaternion *)
             Quaternion::Quaternion
                       ((Quaternion *)&local_88,*(undefined4 *)(iVar21 + 0x18),
                        *(undefined4 *)(iVar21 + 0x1c),*(undefined4 *)(iVar21 + 0x20));
      *(undefined4 *)(iVar13 + 0x128) = local_88;
      *(float *)(iVar13 + 300) = fStack_84;
      *(undefined4 *)(iVar13 + 0x130) = local_80;
      *(undefined4 *)(iVar13 + 0x134) = local_7c;
      Quaternion::~Quaternion(this);
      pcVar26 = *(code **)(DAT_000856b4 + 0x8531a);
      (*pcVar26)(iVar13 + 0x138,
                 *(undefined4 *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4));
      (*pcVar26)(iVar13 + 0x144,
                 *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) + 0xc);
      iVar27 = *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4);
      *(undefined4 *)(iVar13 + 0x178) = *(undefined4 *)(iVar27 + 0x48);
      Quaternion::Quaternion
                ((Quaternion *)&local_88,*(undefined4 *)(iVar27 + 0x3c),
                 *(undefined4 *)(iVar27 + 0x40),*(undefined4 *)(iVar27 + 0x44));
      *(undefined4 *)(iVar13 + 0x150) = local_88;
      *(float *)(iVar13 + 0x154) = fStack_84;
      *(undefined4 *)(iVar13 + 0x158) = local_80;
      *(undefined4 *)(iVar13 + 0x15c) = local_7c;
      Quaternion::~Quaternion((Quaternion *)&local_88);
      (*pcVar26)(iVar13 + 0x160,
                 *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) + 0x24);
      (*pcVar26)(iVar13 + 0x16c,
                 *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) + 0x30);
    }
    else {
      iVar18 = piVar14[-1];
      uVar19 = *(uint *)(iVar18 + 0x50);
      iVar21 = *(int *)(iVar18 + 0x54);
      Quaternion::Quaternion
                ((Quaternion *)local_c8,*(undefined4 *)(iVar18 + 0x18),
                 *(undefined4 *)(iVar18 + 0x1c),*(undefined4 *)(iVar18 + 0x20));
      fVar15 = (float)__aeabi_l2f(uVar23 - uVar19,(iVar27 - iVar21) - (uint)(uVar23 < uVar19));
      fVar16 = (float)__aeabi_l2f(uVar22 - uVar19,(iVar25 - iVar21) - (uint)(uVar22 < uVar19));
      iVar27 = *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4);
      Quaternion::Quaternion
                ((Quaternion *)local_108,*(undefined4 *)(iVar27 + 0x18),
                 *(undefined4 *)(iVar27 + 0x1c),*(undefined4 *)(iVar27 + 0x20));
      fVar15 = fVar15 / fVar16;
      Quaternion::Lerp((Quaternion *)&local_88,(Quaternion *)local_c8,(Quaternion *)local_108,fVar16
                      );
      puVar17 = (undefined4 *)(DAT_000852e4 + 0x85158);
      *(undefined4 *)(iVar13 + 0x128) = local_88;
      *(float *)(iVar13 + 300) = fStack_84;
      *(undefined4 *)(iVar13 + 0x130) = local_80;
      *(undefined4 *)(iVar13 + 0x134) = local_7c;
      pcVar26 = (code *)*puVar17;
      (*pcVar26)((Quaternion *)&local_88);
      (*pcVar26)(local_108);
      fVar16 = (float)(*pcVar26)(local_c8);
      AEMath::VectorLerp((AEMath *)&local_88,
                         *(Vector **)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4 + -4),
                         *(Vector **)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4),
                         fVar16);
      fVar16 = (float)AEMath::Vector::operator=((Vector *)(iVar13 + 0x138),(Vector *)&local_88);
      AEMath::VectorLerp((AEMath *)&local_88,
                         (Vector *)
                         (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4 + -4) +
                         0xc),(Vector *)
                              (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) +
                              0xc),fVar16);
      AEMath::Vector::operator=((Vector *)(iVar13 + 0x144),(Vector *)&local_88);
      iVar27 = *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4 + -4);
      *(float *)(iVar13 + 0x178) =
           *(float *)(iVar27 + 0x48) +
           fVar15 * (*(float *)(*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) +
                               0x48) - *(float *)(iVar27 + 0x48));
      Quaternion::Quaternion
                ((Quaternion *)local_c8,*(undefined4 *)(iVar27 + 0x3c),
                 *(undefined4 *)(iVar27 + 0x40),*(undefined4 *)(iVar27 + 0x44));
      iVar27 = *(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4);
      fVar15 = (float)Quaternion::Quaternion
                                ((Quaternion *)local_108,*(undefined4 *)(iVar27 + 0x3c),
                                 *(undefined4 *)(iVar27 + 0x40),*(undefined4 *)(iVar27 + 0x44));
      Quaternion::Lerp((Quaternion *)&local_88,(Quaternion *)local_c8,(Quaternion *)local_108,fVar15
                      );
      *(undefined4 *)(iVar13 + 0x150) = local_88;
      *(float *)(iVar13 + 0x154) = fStack_84;
      *(undefined4 *)(iVar13 + 0x158) = local_80;
      *(undefined4 *)(iVar13 + 0x15c) = local_7c;
      (*pcVar26)((Quaternion *)&local_88);
      (*pcVar26)(local_108);
      fVar15 = (float)(*pcVar26)(local_c8);
      AEMath::VectorLerp((AEMath *)&local_88,
                         (Vector *)
                         (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4 + -4) +
                         0x24),(Vector *)
                               (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) +
                               0x24),fVar15);
      fVar15 = (float)AEMath::Vector::operator=((Vector *)(iVar13 + 0x160),(Vector *)&local_88);
      AEMath::VectorLerp((AEMath *)&local_88,
                         (Vector *)
                         (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4 + -4) +
                         0x30),(Vector *)
                               (*(int *)(*(int *)(iVar13 + 0x120) + *(int *)(iVar13 + 0x118) * 4) +
                               0x30),fVar15);
      AEMath::Vector::operator=((Vector *)(iVar13 + 0x16c),(Vector *)&local_88);
    }
    uVar10 = DAT_00085870;
    uVar9 = DAT_00085868;
    uVar20 = 0;
    fVar32 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar33 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    fVar34 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    fVar15 = *(float *)(iVar13 + 0x178) * DAT_000856b8;
    uVar22 = (uint)(0.0 < fVar15) * (int)fVar15;
    *(uint *)(iVar13 + 0x48) = uVar22 | uVar22 * 0x10000 | uVar22 * 0x1000000 | uVar22 * 0x100;
    puVar17 = (undefined4 *)((uint)&local_88 | 4);
    *puVar17 = 0;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    local_70 = 0.0;
    local_60 = uVar9;
    uStack_58 = uVar10;
    local_88 = 0x3f800000;
    local_74 = 0x3f800000;
    local_50 = 0x3f800000;
    fStack_6c = fVar32;
    local_68 = uVar33;
    local_64 = fVar34;
    Quaternion::Convert((Quaternion *)(iVar13 + 0x128),(Matrix *)&local_88);
    *(undefined4 *)(iVar13 + 0x5c) = local_88;
    *(undefined4 *)(iVar13 + 0x60) = local_80;
    *(float *)(iVar13 + 100) = -fStack_84;
    *(undefined4 *)(iVar13 + 0x68) = local_7c;
    *(undefined4 *)(iVar13 + 0x6c) = local_68;
    *(undefined4 *)(iVar13 + 0x70) = (undefined4)local_60;
    *(float *)(iVar13 + 0x74) = -local_64;
    *(undefined4 *)(iVar13 + 0x78) = local_60._4_4_;
    *(float *)(iVar13 + 0x7c) = -local_78;
    *(float *)(iVar13 + 0x80) = -local_70;
    *(undefined4 *)(iVar13 + 0x84) = local_74;
    *(float *)(iVar13 + 0x88) = -fStack_6c;
    AEMath::MatrixSetScaling((Matrix *)local_c8,*(float *)(iVar13 + 0x14c),extraout_s1,-local_70);
    AEMath::MatrixSetTranslation
              ((Matrix *)local_c8,*(float *)(iVar13 + 0x140),extraout_s1_00,extraout_s2);
    fVar16 = *(float *)(iVar13 + 0x150);
    fVar29 = *(float *)(iVar13 + 0x154);
    fVar30 = *(float *)(iVar13 + 0x158);
    fVar31 = *(float *)(iVar13 + 0x15c);
    *(undefined4 *)(iVar13 + 0xc4) = 0;
    *(undefined4 *)(iVar13 + 0xb4) = 0;
    *(undefined4 *)(iVar13 + 0xa4) = 0;
    puVar17 = (undefined4 *)((uint)local_c8 | 4);
    fVar6 = fVar29 * fVar29 + fVar30 * fVar30;
    fVar7 = fVar16 * fVar16 + fVar30 * fVar30;
    fVar8 = fVar16 * fVar16 + fVar29 * fVar29;
    fVar2 = fVar16 * fVar29 - fVar30 * fVar31;
    fVar3 = fVar16 * fVar29 + fVar30 * fVar31;
    fVar15 = fVar29 * fVar30 - fVar16 * fVar31;
    fVar4 = fVar16 * fVar30 + fVar29 * fVar31;
    fVar5 = fVar16 * fVar30 - fVar29 * fVar31;
    fVar16 = fVar29 * fVar30 + fVar16 * fVar31;
    fVar7 = 1.0 - (fVar7 + fVar7);
    *(float *)(iVar13 + 0x98) = 1.0 - (fVar6 + fVar6);
    *(float *)(iVar13 + 0x9c) = fVar2 + fVar2;
    *(float *)(iVar13 + 0xa0) = fVar4 + fVar4;
    *(float *)(iVar13 + 0xa8) = fVar3 + fVar3;
    *(float *)(iVar13 + 0xac) = fVar7;
    *(float *)(iVar13 + 0xb0) = fVar15 + fVar15;
    *(float *)(iVar13 + 0xb8) = fVar5 + fVar5;
    *(float *)(iVar13 + 0xbc) = fVar16 + fVar16;
    *(float *)(iVar13 + 0xc0) = 1.0 - (fVar8 + fVar8);
    *puVar17 = uVar20;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    uStack_a0 = uVar9;
    uStack_98 = uVar10;
    puVar17 = (undefined4 *)((uint)local_108 | 4);
    *puVar17 = uVar20;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    uStack_e0 = uVar9;
    uStack_d8 = uVar10;
    puVar17 = (undefined4 *)((uint)local_148 | 4);
    local_c8[0] = 0x3f800000;
    *puVar17 = uVar20;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    uStack_120 = uVar9;
    uStack_118 = uVar10;
    puVar17 = (undefined4 *)((uint)local_188 | 4);
    *puVar17 = uVar20;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    uStack_160 = uVar9;
    uStack_158 = uVar10;
    puVar17 = (undefined4 *)((uint)local_1c8 | 4);
    *puVar17 = uVar20;
    puVar17[1] = fVar32;
    puVar17[2] = uVar33;
    puVar17[3] = fVar34;
    uStack_1a0 = uVar9;
    uStack_198 = uVar10;
    local_b4 = 0x3f800000;
    local_90 = 0x3f800000;
    local_108[0] = 0x3f800000;
    local_f4 = 0x3f800000;
    local_148[0] = 0x3f800000;
    local_134 = 0x3f800000;
    local_188[0] = 0x3f800000;
    local_174 = 0x3f800000;
    local_1c8[0] = 0x3f800000;
    local_1b4 = 0x3f800000;
    local_bc = 0xbf000000;
    uStack_ac = 0xbf000000;
    local_d0 = 0x3f800000;
    local_110 = 0x3f800000;
    local_150 = 0x3f800000;
    local_190 = 0x3f800000;
    local_13c = *(float *)(iVar13 + 0x16c) * *(float *)(iVar13 + 0x160) + 0.5;
    local_fc = 0x3f000000;
    fStack_12c = *(float *)(iVar13 + 0x170) * *(float *)(iVar13 + 0x164) + 0.5;
    uStack_ec = 0x3f000000;
    local_1b0 = uVar20;
    fStack_1ac = fVar32;
    uStack_1a8 = uVar33;
    fStack_1a4 = fVar34;
    local_170 = uVar20;
    fStack_16c = fVar32;
    uStack_168 = uVar33;
    fStack_164 = fVar34;
    local_130 = uVar20;
    uStack_128 = uVar33;
    fStack_124 = fVar34;
    local_f0 = uVar20;
    uStack_e8 = uVar33;
    fStack_e4 = fVar34;
    local_b0 = uVar20;
    uStack_a8 = uVar33;
    fStack_a4 = fVar34;
    AEMath::MatrixSetScaling((Matrix *)local_208,fStack_12c,fVar7,*(float *)(iVar13 + 0x164));
    AEMath::operator*(aMStack_244,(Matrix *)local_108);
    AEMath::operator*((Matrix *)local_208,aMStack_244);
    AEMath::Matrix::operator=((Matrix *)local_1c8,(Matrix *)local_208);
    iVar27 = DAT_00085880;
    uVar11 = DAT_00085878;
    if (**(char **)(DAT_0008587c + 0x8568a) == '\0') {
      puVar17 = (undefined4 *)((uint)local_208 | 4);
      *puVar17 = uVar20;
      puVar17[1] = fVar32;
      puVar17[2] = uVar33;
      puVar17[3] = fVar34;
      uStack_1e0 = uVar9;
      uStack_1d8 = uVar10;
      local_208[0] = 0x3f800000;
      local_1d0 = 0x3f800000;
      pcVar26 = *(code **)(iVar27 + 0x856d4);
      local_1f4 = uVar11;
      local_1f0 = uVar20;
      fStack_1ec = fVar32;
      uStack_1e8 = uVar33;
      fStack_1e4 = fVar34;
      (*pcVar26)(auStack_2f8,local_1c8,local_148);
      (*pcVar26)(auStack_2bc,auStack_2f8,local_188);
      (*pcVar26)(auStack_280,auStack_2bc,(Matrix *)local_208);
      pMVar24 = aMStack_244;
      (*pcVar26)(pMVar24,auStack_280,local_c8);
    }
    else {
      pcVar26 = *(code **)(DAT_00085884 + 0x8569a);
      (*pcVar26)(auStack_280,local_1c8,local_148);
      (*pcVar26)(aMStack_244,auStack_280,local_188);
      pMVar24 = (Matrix *)local_208;
      (*pcVar26)(pMVar24,aMStack_244,local_c8);
    }
    AEMath::Matrix::operator=((Matrix *)(iVar13 + 0x98),pMVar24);
  }
  if (in_stack_00000000 == 0) {
    fStack_84 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    local_80 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    local_7c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    this_00 = (BSphere *)(iVar13 + 0xd4);
    local_88 = 0;
    local_78 = 1.0;
    AEMath::BSphere::operator=(this_00,(BSphere *)&local_88);
    uVar20 = extraout_r1;
    for (uVar22 = 0; uVar22 < *(uint *)(iVar13 + 0x3c); uVar22 = uVar22 + 1) {
      iVar27 = *(int *)(*(int *)(iVar13 + 0x40) + uVar22 * 4);
      if ((iVar27 == 0) || (*(int *)(iVar27 + 0x34) == 0)) {
        local_88 = *(undefined4 *)(iVar27 + 0x3c);
        fStack_84 = *(float *)(iVar27 + 0x40);
        local_80 = *(undefined4 *)(iVar27 + 0x44);
        local_7c = *(undefined4 *)(iVar27 + 0x48);
        local_78 = *(float *)(iVar27 + 0x4c);
      }
      else {
        InternUpdate(CONCAT44(iVar27,*(int *)(iVar27 + 0x34)),param_2);
        iVar27 = *(int *)(*(int *)(iVar13 + 0x40) + uVar22 * 4);
        local_88 = *(undefined4 *)(iVar27 + 0x3c);
        fStack_84 = *(float *)(iVar27 + 0x40);
        local_80 = *(undefined4 *)(iVar27 + 0x44);
        local_7c = *(undefined4 *)(iVar27 + 0x48);
        local_78 = *(float *)(iVar27 + 0x4c);
        AEMath::MatrixTransformVector
                  ((Matrix *)local_c8,
                   (Vector *)
                   (*(int *)(*(int *)(*(int *)(iVar13 + 0x40) + uVar22 * 4) + 0x34) + 0x5c));
        AEMath::Vector::operator=((Vector *)&local_88,(Vector *)local_c8);
      }
      AEMath::BSphere::Merge(this_00,(BSphere *)&local_88);
      uVar20 = extraout_r1_00;
    }
    for (uVar22 = 0; uVar22 < *(uint *)(iVar13 + 0x4c); uVar22 = uVar22 + 1) {
      InternUpdate(CONCAT44(uVar20,*(undefined4 *)(*(int *)(iVar13 + 0x50) + uVar22 * 4)),param_2);
      iVar27 = *(int *)(*(int *)(iVar13 + 0x50) + uVar22 * 4);
      local_88 = *(undefined4 *)(iVar27 + 0xd4);
      fStack_84 = *(float *)(iVar27 + 0xd8);
      local_80 = *(undefined4 *)(iVar27 + 0xdc);
      local_7c = *(undefined4 *)(iVar27 + 0xe0);
      local_78 = *(float *)(iVar27 + 0xe4);
      AEMath::MatrixTransformVector
                ((Matrix *)local_c8,
                 (Vector *)(*(int *)(*(int *)(iVar13 + 0x50) + uVar22 * 4) + 0x5c));
      AEMath::Vector::operator=((Vector *)&local_88,(Vector *)local_c8);
      AEMath::BSphere::Merge(this_00,(BSphere *)&local_88);
      uVar20 = extraout_r1_01;
    }
  }
LAB_00085828:
  if (*piVar28 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00085048: push {r4,r5,r6,r7,lr}
  0008504a: add r7,sp,#0xc
  0008504c: push {r7,r8,r9,r10,r11}
  00085050: vpush {d8,d9,d10,d11}
  00085054: sub.w sp,sp,#0x2c0
  00085058: strd r2,r3,[sp,#0x8]
  0008505c: mov r9,r0
  0008505e: ldr r0,[0x000852e0]
  00085060: add r0,pc
  00085062: ldr.w r10,[r0,#0x0]
  00085066: ldr.w r0,[r10,#0x0]
  0008506a: str r0,[sp,#0x2bc]
  0008506c: ldr.w r1,[r9,#0x11c]
  00085070: cmp r1,#0x0
  00085072: beq.w 0x00085716
  00085076: ldr.w r0,[r9,#0x120]
  0008507a: movs r2,#0x0
  0008507c: ldrd r5,r8,[r9,#0x100]
  00085080: ldr r4,[sp,#0x8]
  00085082: add.w r1,r0,r1, lsl #0x2
  00085086: ldr r6,[sp,#0xc]
  00085088: subs r3,r5,r4
  0008508a: ldr.w r1,[r1,#-0x4]
  0008508e: sbcs.w r3,r8,r6
  00085092: mov.w r3,#0x0
  00085096: it lt
  00085098: mov.lt r3,#0x1
  0008509a: cmp r3,#0x0
  0008509c: it ne
  0008509e: mov.ne r8,r6
  000850a0: ldrd r1,r3,[r1,#0x50]
  000850a4: it ne
  000850a6: mov.ne r5,r4
  000850a8: subs r6,r1,r5
  000850aa: sbcs.w r6,r3,r8
  000850ae: it lt
  000850b0: mov.lt r2,#0x1
  000850b2: cmp r2,#0x0
  000850b4: itt ne
  000850b6: mov.ne r8,r3
  000850b8: mov.ne r5,r1
  000850ba: mov.w r1,#0xffffffff
  000850be: ldmia r0!,{r3}
  000850c0: adds r1,#0x1
  000850c2: ldrd r4,r6,[r3,#0x50]
  000850c6: subs r2,r4,r5
  000850c8: sbcs.w r2,r6,r8
  000850cc: blt 0x000850be
  000850ce: cmp r1,#0x0
  000850d0: str.w r1,[r9,#0x118]
  000850d4: beq.w 0x00085292
  000850d8: orrs.w r1,r5,r8
  000850dc: beq.w 0x000852f0
  000850e0: ldr.w r0,[r0,#-0x8]
  000850e4: str.w r10,[sp,#0x4]
  000850e8: add.w r3,r0,#0x18
  000850ec: ldrd r10,r11,[r0,#0x50]
  000850f0: add r0,sp,#0x240
  000850f2: ldmia r3,{r1,r2,r3}
  000850f4: blx 0x0006f7f0
  000850f8: subs.w r0,r5,r10
  000850fc: sbc.w r1,r8,r11
  00085100: blx 0x0006f7d8
  00085104: mov r8,r0
  00085106: subs.w r0,r4,r10
  0008510a: sbc.w r1,r6,r11
  0008510e: blx 0x0006f7d8
  00085112: mov r5,r0
  00085114: ldr.w r0,[r9,#0x118]
  00085118: ldr.w r1,[r9,#0x120]
  0008511c: ldr.w r0,[r1,r0,lsl #0x2]
  00085120: add.w r3,r0,#0x18
  00085124: ldmia r3,{r1,r2,r3}
  00085126: add r0,sp,#0x200
  00085128: blx 0x0006f7f0
  0008512c: vmov s0,r5
  00085130: vmov s2,r8
  00085134: vdiv.f32 s16,s2,s0
  00085138: vmov r4,s16
  0008513c: add r5,sp,#0x280
  0008513e: add r1,sp,#0x240
  00085140: add r2,sp,#0x200
  00085142: mov r3,r4
  00085144: mov r0,r5
  00085146: blx 0x0006f7fc
  0008514a: ldr r0,[0x000852e4]
  0008514c: add.w r8,r9,#0x128
  00085150: vld1.64 {d16,d17},[r5]
  00085154: add r0,pc
  00085156: vst1.64 {d16,d17},[r8]
  0008515a: ldr r6,[r0,#0x0]
  0008515c: mov r0,r5
  0008515e: blx r6
  00085160: add r0,sp,#0x200
  00085162: blx r6
  00085164: add r0,sp,#0x240
  00085166: blx r6
  00085168: ldr.w r0,[r9,#0x118]
  0008516c: add r5,sp,#0x280
  0008516e: ldr.w r1,[r9,#0x120]
  00085172: mov r3,r4
  00085174: ldr.w r2,[r1,r0,lsl #0x2]
  00085178: add.w r0,r1,r0, lsl #0x2
  0008517c: ldr.w r1,[r0,#-0x4]
  00085180: mov r0,r5
  00085182: blx 0x0006f808
  00085186: add.w r0,r9,#0x138
  0008518a: mov r1,r5
  0008518c: blx 0x0006eb3c
  00085190: ldr.w r0,[r9,#0x118]
  00085194: add r5,sp,#0x280
  00085196: ldr.w r1,[r9,#0x120]
  0008519a: mov r3,r4
  0008519c: ldr.w r2,[r1,r0,lsl #0x2]
  000851a0: add.w r0,r1,r0, lsl #0x2
  000851a4: ldr.w r0,[r0,#-0x4]
  000851a8: adds r2,#0xc
  000851aa: add.w r1,r0,#0xc
  000851ae: mov r0,r5
  000851b0: blx 0x0006f808
  000851b4: add.w r0,r9,#0x144
  000851b8: mov r1,r5
  000851ba: blx 0x0006eb3c
  000851be: ldr.w r0,[r9,#0x118]
  000851c2: ldr.w r1,[r9,#0x120]
  000851c6: ldr.w r2,[r1,r0,lsl #0x2]
  000851ca: add.w r0,r1,r0, lsl #0x2
  000851ce: ldr.w r0,[r0,#-0x4]
  000851d2: vldr.32 s0,[r2,#0x48]
  000851d6: add.w r3,r0,#0x3c
  000851da: vldr.32 s2,[r0,#0x48]
  000851de: add r0,sp,#0x240
  000851e0: vsub.f32 s0,s0,s2
  000851e4: vmla.f32 s2,s16,s0
  000851e8: vstr.32 s2,[r9,#0x178]
  000851ec: ldmia r3,{r1,r2,r3}
  000851ee: blx 0x0006f7f0
  000851f2: ldr.w r0,[r9,#0x118]
  000851f6: ldr.w r1,[r9,#0x120]
  000851fa: ldr.w r0,[r1,r0,lsl #0x2]
  000851fe: add.w r3,r0,#0x3c
  00085202: ldmia r3,{r1,r2,r3}
  00085204: add r0,sp,#0x200
  00085206: blx 0x0006f7f0
  0008520a: ldr.w r10,[sp,#0x4]
  0008520e: add r5,sp,#0x280
  00085210: add r1,sp,#0x240
  00085212: add r2,sp,#0x200
  00085214: mov r3,r4
  00085216: mov r0,r5
  00085218: blx 0x0006f7fc
  0008521c: vld1.64 {d16,d17},[r5]
  00085220: add.w r0,r9,#0x150
  00085224: vst1.64 {d16,d17},[r0]
  00085228: mov r0,r5
  0008522a: blx r6
  0008522c: add r0,sp,#0x200
  0008522e: blx r6
  00085230: add r0,sp,#0x240
  00085232: blx r6
  00085234: ldr.w r0,[r9,#0x118]
  00085238: add r5,sp,#0x280
  0008523a: ldr.w r1,[r9,#0x120]
  0008523e: mov r3,r4
  00085240: ldr.w r2,[r1,r0,lsl #0x2]
  00085244: add.w r0,r1,r0, lsl #0x2
  00085248: ldr.w r0,[r0,#-0x4]
  0008524c: adds r2,#0x24
  0008524e: add.w r1,r0,#0x24
  00085252: mov r0,r5
  00085254: blx 0x0006f808
  00085258: add.w r0,r9,#0x160
  0008525c: mov r1,r5
  0008525e: blx 0x0006eb3c
  00085262: ldr.w r0,[r9,#0x118]
  00085266: add r5,sp,#0x280
  00085268: ldr.w r1,[r9,#0x120]
  0008526c: mov r3,r4
  0008526e: ldr.w r2,[r1,r0,lsl #0x2]
  00085272: add.w r0,r1,r0, lsl #0x2
  00085276: ldr.w r0,[r0,#-0x4]
  0008527a: adds r2,#0x30
  0008527c: add.w r1,r0,#0x30
  00085280: mov r0,r5
  00085282: blx 0x0006f808
  00085286: add.w r0,r9,#0x16c
  0008528a: mov r1,r5
  0008528c: blx 0x0006eb3c
  00085290: b 0x00085390
  00085292: adr.w r0,0x85868
  00085296: vmov.i32 q8,#0x0
  0008529a: add r1,sp,#0x280
  0008529c: vld1.64 {d18,d19},[r0]
  000852a0: orr r0,r1,#0x4
  000852a4: vst1.32 {d16,d17},[r0]
  000852a8: add.w r0,r1,#0x18
  000852ac: vst1.64 {d16,d17},[r0]
  000852b0: add.w r0,r1,#0x28
  000852b4: vst1.64 {d18,d19},[r0]
  000852b8: mov.w r0,#0x3f800000
  000852bc: str r0,[sp,#0x280]
  000852be: str r0,[sp,#0x294]
  000852c0: str r0,[sp,#0x2b8]
  000852c2: add.w r0,r9,#0x98
  000852c6: blx 0x0006f148
  000852ca: ldr r0,[0x000852e8]
  000852cc: add r0,pc
  000852ce: ldr r0,[r0,#0x0]
  000852d0: ldrb r0,[r0,#0x0]
  000852d2: cmp r0,#0x0
  000852d4: bne.w 0x00085828
  000852d8: ldr r0,[0x000852ec]
  000852da: str.w r0,[r9,#0xac]
  000852de: b 0x00085828
  000852f0: adds r3,#0x18
  000852f2: add r5,sp,#0x280
  000852f4: ldmia r3,{r1,r2,r3}
  000852f6: mov r0,r5
  000852f8: blx 0x0006f7f0
  000852fc: vld1.64 {d16,d17},[r5]
  00085300: add.w r8,r9,#0x128
  00085304: vst1.64 {d16,d17},[r8]
  00085308: blx 0x0006f13c
  0008530c: ldr r0,[0x000856b4]
  0008530e: ldr.w r1,[r9,#0x118]
  00085312: ldr.w r2,[r9,#0x120]
  00085316: add r0,pc
  00085318: ldr r4,[r0,#0x0]
  0008531a: add.w r0,r9,#0x138
  0008531e: ldr.w r1,[r2,r1,lsl #0x2]
  00085322: blx r4
  00085324: ldr.w r0,[r9,#0x118]
  00085328: ldr.w r1,[r9,#0x120]
  0008532c: ldr.w r1,[r1,r0,lsl #0x2]
  00085330: add.w r0,r9,#0x144
  00085334: adds r1,#0xc
  00085336: blx r4
  00085338: ldr.w r0,[r9,#0x118]
  0008533c: ldr.w r1,[r9,#0x120]
  00085340: ldr.w r0,[r1,r0,lsl #0x2]
  00085344: add.w r3,r0,#0x3c
  00085348: ldr r1,[r0,#0x48]
  0008534a: mov r0,r5
  0008534c: str.w r1,[r9,#0x178]
  00085350: ldmia r3,{r1,r2,r3}
  00085352: blx 0x0006f7f0
  00085356: vld1.64 {d16,d17},[r5]
  0008535a: add.w r0,r9,#0x150
  0008535e: vst1.64 {d16,d17},[r0]
  00085362: mov r0,r5
  00085364: blx 0x0006f13c
  00085368: ldr.w r0,[r9,#0x118]
  0008536c: ldr.w r1,[r9,#0x120]
  00085370: ldr.w r1,[r1,r0,lsl #0x2]
  00085374: add.w r0,r9,#0x160
  00085378: adds r1,#0x24
  0008537a: blx r4
  0008537c: ldr.w r0,[r9,#0x118]
  00085380: ldr.w r1,[r9,#0x120]
  00085384: ldr.w r1,[r1,r0,lsl #0x2]
  00085388: add.w r0,r9,#0x16c
  0008538c: adds r1,#0x30
  0008538e: blx r4
  00085390: vldr.32 s0,[pc,#0x324]
  00085394: adr.w r0,0x85868
  00085398: vldr.32 s2,[r9,#0x178]
  0008539c: vmov.i32 q4,#0x0
  000853a0: vld1.64 {d10,d11},[r0]
  000853a4: mov.w r6,#0x3f800000
  000853a8: vmul.f32 s0,s2,s0
  000853ac: vcvt.u32.f32 s0,s0
  000853b0: vmov r0,s0
  000853b4: orr.w r1,r0,r0, lsl #0x10
  000853b8: orr.w r1,r1,r0, lsl #0x18
  000853bc: orr.w r0,r1,r0, lsl #0x8
  000853c0: add r1,sp,#0x280
  000853c2: str.w r0,[r9,#0x48]
  000853c6: orr r0,r1,#0x4
  000853ca: vst1.32 {d8,d9},[r0]
  000853ce: add.w r0,r1,#0x18
  000853d2: vst1.64 {d8,d9},[r0]
  000853d6: add.w r0,r1,#0x28
  000853da: vst1.64 {d10,d11},[r0]
  000853de: mov r0,r8
  000853e0: str r6,[sp,#0x280]
  000853e2: str r6,[sp,#0x294]
  000853e4: str r6,[sp,#0x2b8]
  000853e6: blx 0x0006f160
  000853ea: ldr r1,[sp,#0x288]
  000853ec: add.w r3,r9,#0x68
  000853f0: ldr r0,[sp,#0x280]
  000853f2: add.w r4,r9,#0x5c
  000853f6: strd r0,r1,[r9,#0x5c]
  000853fa: vldr.32 s0,[sp,#0x284]
  000853fe: vneg.f32 s0,s0
  00085402: vstr.32 s0,[r9,#0x64]
  00085406: ldr r1,[sp,#0x2a0]
  00085408: ldr r2,[sp,#0x2a8]
  0008540a: ldr r0,[sp,#0x28c]
  0008540c: stmia r3!,{r0,r1,r2}
  0008540e: mov r1,r4
  00085410: vldr.32 s0,[sp,#0x2a4]
  00085414: vneg.f32 s0,s0
  00085418: vstr.32 s0,[r9,#0x74]
  0008541c: ldr r0,[sp,#0x2ac]
  0008541e: str.w r0,[r9,#0x78]
  00085422: vldr.32 s0,[sp,#0x290]
  00085426: vldr.32 s2,[sp,#0x298]
  0008542a: vneg.f32 s0,s0
  0008542e: vneg.f32 s2,s2
  00085432: vstr.32 s0,[r9,#0x7c]
  00085436: vstr.32 s2,[r9,#0x80]
  0008543a: ldr r0,[sp,#0x294]
  0008543c: str.w r0,[r9,#0x84]
  00085440: add r0,sp,#0x240
  00085442: vldr.32 s0,[sp,#0x29c]
  00085446: vneg.f32 s0,s0
  0008544a: vstr.32 s0,[r9,#0x88]
  0008544e: ldrd r2,r3,[r9,#0x144]
  00085452: vldr.32 s0,[r9,#0x14c]
  00085456: vstr.32 s0,[sp]
  0008545a: blx 0x0006f814
  0008545e: add r5,sp,#0x240
  00085460: ldrd r2,r3,[r9,#0x138]
  00085464: vldr.32 s0,[r9,#0x140]
  00085468: mov r1,r4
  0008546a: mov r0,r5
  0008546c: vstr.32 s0,[sp]
  00085470: blx 0x0006f820
  00085474: vldr.32 s0,[r9,#0x150]
  00085478: movs r0,#0x0
  0008547a: vldr.32 s2,[r9,#0x154]
  0008547e: add r4,sp,#0x200
  00085480: vldr.32 s4,[r9,#0x158]
  00085484: vmul.f32 s12,s0,s0
  00085488: vmul.f32 s10,s2,s2
  0008548c: vldr.32 s6,[r9,#0x15c]
  00085490: vmul.f32 s8,s4,s4
  00085494: str.w r0,[r9,#0xc4]
  00085498: vmul.f32 s14,s4,s6
  0008549c: str.w r0,[r9,#0xb4]
  000854a0: vmul.f32 s1,s0,s2
  000854a4: str.w r0,[r9,#0xa4]
  000854a8: vmul.f32 s3,s2,s6
  000854ac: orr r0,r5,#0x4
  000854b0: vmul.f32 s5,s0,s4
  000854b4: vcvt.f64.f32 d18,s12
  000854b8: vcvt.f64.f32 d16,s8
  000854bc: vcvt.f64.f32 d17,s10
  000854c0: vmul.f32 s0,s0,s6
  000854c4: vmul.f32 s2,s2,s4
  000854c8: vadd.f64 d25,d17,d16
  000854cc: vadd.f64 d16,d18,d16
  000854d0: vadd.f64 d17,d18,d17
  000854d4: vcvt.f64.f32 d23,s0
  000854d8: vcvt.f64.f32 d19,s14
  000854dc: vcvt.f64.f32 d20,s1
  000854e0: vcvt.f64.f32 d21,s3
  000854e4: vcvt.f64.f32 d22,s5
  000854e8: vcvt.f64.f32 d24,s2
  000854ec: vsub.f64 d18,d20,d19
  000854f0: vadd.f64 d19,d20,d19
  000854f4: vsub.f64 d20,d24,d23
  000854f8: vadd.f64 d26,d22,d21
  000854fc: vsub.f64 d21,d22,d21
  00085500: vadd.f64 d22,d24,d23
  00085504: vadd.f64 d23,d25,d25
  00085508: vadd.f64 d16,d16,d16
  0008550c: vadd.f64 d17,d17,d17
  00085510: vmov.f64 d24,0x3ff0000000000000
  00085514: vadd.f64 d18,d18,d18
  00085518: vadd.f64 d25,d26,d26
  0008551c: vadd.f64 d19,d19,d19
  00085520: vadd.f64 d20,d20,d20
  00085524: vadd.f64 d21,d21,d21
  00085528: vadd.f64 d22,d22,d22
  0008552c: vsub.f64 d23,d24,d23
  00085530: vsub.f64 d16,d24,d16
  00085534: vsub.f64 d17,d24,d17
  00085538: vcvt.f32.f64 s14,d23
  0008553c: vcvt.f32.f64 s2,d18
  00085540: vcvt.f32.f64 s4,d25
  00085544: vcvt.f32.f64 s6,d19
  00085548: vcvt.f32.f64 s8,d20
  0008554c: vcvt.f32.f64 s10,d21
  00085550: vcvt.f32.f64 s12,d22
  00085554: vcvt.f32.f64 s1,d16
  00085558: vcvt.f32.f64 s3,d17
  0008555c: vstr.32 s14,[r9,#0x98]
  00085560: vmov.f32 s0,0x3f000000
  00085564: vstr.32 s2,[r9,#0x9c]
  00085568: vstr.32 s4,[r9,#0xa0]
  0008556c: vstr.32 s6,[r9,#0xa8]
  00085570: vstr.32 s1,[r9,#0xac]
  00085574: vstr.32 s8,[r9,#0xb0]
  00085578: vstr.32 s10,[r9,#0xb8]
  0008557c: vstr.32 s12,[r9,#0xbc]
  00085580: vmov.f32 s8,s0
  00085584: vstr.32 s3,[r9,#0xc0]
  00085588: vst1.32 {d8,d9},[r0]
  0008558c: add.w r0,r5,#0x18
  00085590: vst1.64 {d8,d9},[r0]
  00085594: add.w r0,r5,#0x28
  00085598: add r5,sp,#0x140
  0008559a: vst1.64 {d10,d11},[r0]
  0008559e: orr r0,r4,#0x4
  000855a2: vst1.32 {d8,d9},[r0]
  000855a6: add.w r0,r4,#0x18
  000855aa: vst1.64 {d8,d9},[r0]
  000855ae: add.w r0,r4,#0x28
  000855b2: vst1.64 {d10,d11},[r0]
  000855b6: add r0,sp,#0x1c0
  000855b8: orr r1,r0,#0x4
  000855bc: str r6,[sp,#0x240]
  000855be: vst1.32 {d8,d9},[r1]
  000855c2: add.w r1,r0,#0x18
  000855c6: adds r0,#0x28
  000855c8: vst1.64 {d8,d9},[r1]
  000855cc: add r1,sp,#0x180
  000855ce: vst1.64 {d10,d11},[r0]
  000855d2: orr r0,r1,#0x4
  000855d6: vst1.32 {d8,d9},[r0]
  000855da: add.w r0,r1,#0x18
  000855de: vst1.64 {d8,d9},[r0]
  000855e2: add.w r0,r1,#0x28
  000855e6: vst1.64 {d10,d11},[r0]
  000855ea: orr r0,r5,#0x4
  000855ee: vst1.32 {d8,d9},[r0]
  000855f2: add.w r0,r5,#0x18
  000855f6: vst1.64 {d8,d9},[r0]
  000855fa: add.w r0,r5,#0x28
  000855fe: vst1.64 {d10,d11},[r0]
  00085602: mov.w r0,#0xbf000000
  00085606: str r6,[sp,#0x254]
  00085608: str r6,[sp,#0x278]
  0008560a: str r6,[sp,#0x200]
  0008560c: str r6,[sp,#0x214]
  0008560e: str r6,[sp,#0x1c0]
  00085610: str r6,[sp,#0x1d4]
  00085612: str r6,[sp,#0x180]
  00085614: str r6,[sp,#0x194]
  00085616: str r6,[sp,#0x140]
  00085618: str r6,[sp,#0x154]
  0008561a: str r0,[sp,#0x24c]
  0008561c: str r0,[sp,#0x25c]
  0008561e: mov.w r0,#0x3f000000
  00085622: str r6,[sp,#0x238]
  00085624: str r6,[sp,#0x1f8]
  00085626: str r6,[sp,#0x1b8]
  00085628: str r6,[sp,#0x178]
  0008562a: vldr.32 s4,[r9,#0x16c]
  0008562e: vldr.32 s6,[r9,#0x170]
  00085632: vmov r2,s4
  00085636: vldr.32 s2,[r9,#0x160]
  0008563a: vmov r3,s6
  0008563e: vmla.f32 s8,s4,s2
  00085642: vstr.32 s8,[sp,#0x1cc]
  00085646: vldr.32 s2,[r9,#0x164]
  0008564a: str r0,[sp,#0x20c]
  0008564c: vmla.f32 s0,s6,s2
  00085650: str r0,[sp,#0x21c]
  00085652: add r0,sp,#0x100
  00085654: str r6,[sp,#0x0]
  00085656: vstr.32 s0,[sp,#0x1dc]
  0008565a: blx 0x0006f814
  0008565e: add.w r8,sp,#0xc4
  00085662: add.w r2,r9,#0x98
  00085666: mov r1,r4
  00085668: mov r0,r8
  0008566a: mov r11,r2
  0008566c: blx 0x0006f82c
  00085670: add r4,sp,#0x100
  00085672: add r2,sp,#0x240
  00085674: mov r1,r8
  00085676: mov r0,r4
  00085678: blx 0x0006f82c
  0008567c: mov r0,r5
  0008567e: mov r1,r4
  00085680: blx 0x0006f148
  00085684: ldr r0,[0x0008587c]
  00085686: add r0,pc
  00085688: ldr r0,[r0,#0x0]
  0008568a: ldrb r0,[r0,#0x0]
  0008568c: cbz r0,0x000856bc
  0008568e: ldr r0,[0x00085884]
  00085690: add r5,sp,#0x88
  00085692: add r1,sp,#0x140
  00085694: add r2,sp,#0x1c0
  00085696: add r0,pc
  00085698: ldr r4,[r0,#0x0]
  0008569a: mov r0,r5
  0008569c: blx r4
  0008569e: add r6,sp,#0xc4
  000856a0: add r2,sp,#0x180
  000856a2: mov r1,r5
  000856a4: mov r0,r6
  000856a6: blx r4
  000856a8: add r5,sp,#0x100
  000856aa: add r2,sp,#0x240
  000856ac: mov r1,r6
  000856ae: mov r0,r5
  000856b0: blx r4
  000856b2: b 0x0008570e
  000856bc: orr r2,r4,#0x4
  000856c0: ldr r0,[0x00085880]
  000856c2: ldr r1,[0x00085878]
  000856c4: add.w r8,sp,#0x10
  000856c8: vst1.32 {d8,d9},[r2]
  000856cc: add.w r2,r4,#0x18
  000856d0: add r0,pc
  000856d2: vst1.64 {d8,d9},[r2]
  000856d6: add.w r2,r4,#0x28
  000856da: vst1.64 {d10,d11},[r2]
  000856de: add r2,sp,#0x1c0
  000856e0: str r6,[sp,#0x100]
  000856e2: str r6,[sp,#0x138]
  000856e4: ldr r6,[r0,#0x0]
  000856e6: mov r0,r8
  000856e8: str r1,[sp,#0x114]
  000856ea: add r1,sp,#0x140
  000856ec: blx r6
  000856ee: add r5,sp,#0x4c
  000856f0: add r2,sp,#0x180
  000856f2: mov r1,r8
  000856f4: mov r0,r5
  000856f6: blx r6
  000856f8: add.w r8,sp,#0x88
  000856fc: mov r1,r5
  000856fe: mov r2,r4
  00085700: mov r0,r8
  00085702: blx r6
  00085704: add r5,sp,#0xc4
  00085706: add r2,sp,#0x240
  00085708: mov r1,r8
  0008570a: mov r0,r5
  0008570c: blx r6
  0008570e: mov r0,r11
  00085710: mov r1,r5
  00085712: blx 0x0006f148
  00085716: ldr r0,[r7,#0x8]
  00085718: cmp r0,#0x0
  0008571a: bne.w 0x00085828
  0008571e: add r5,sp,#0x280
  00085720: vmov.i32 q8,#0x0
  00085724: add.w r11,r9,#0xd4
  00085728: mov.w r1,#0x3f800000
  0008572c: mov r0,r5
  0008572e: str.w r10,[sp,#0x4]
  00085732: vst1.64 {d16,d17},[r0]!
  00085736: str r1,[r0,#0x0]
  00085738: mov r0,r11
  0008573a: mov r1,r5
  0008573c: blx 0x0006eb18
  00085740: add.w r8,sp,#0x240
  00085744: movs r4,#0x0
  00085746: mov.w r10,#0x0
  0008574a: b 0x000857b8
  0008574c: ldr.w r0,[r9,#0x40]
  00085750: ldr.w r1,[r0,r10,lsl #0x2]
  00085754: cbz r1,0x0008579a
  00085756: ldr r0,[r1,#0x34]
  00085758: cbz r0,0x0008579a
  0008575a: str r4,[sp,#0x0]
  0008575c: ldrd r2,r3,[sp,#0x8]
  00085760: blx 0x0006f7e4
  00085764: ldr.w r0,[r9,#0x40]
  00085768: ldr.w r1,[r0,r10,lsl #0x2]
  0008576c: add.w r2,r1,#0x3c
  00085770: vld1.32 {d16,d17},[r2]
  00085774: mov r2,r5
  00085776: ldr r1,[r1,#0x4c]
  00085778: vst1.64 {d16,d17},[r2]!
  0008577c: str r1,[r2,#0x0]
  0008577e: mov r2,r5
  00085780: ldr.w r0,[r0,r10,lsl #0x2]
  00085784: ldr r0,[r0,#0x34]
  00085786: add.w r1,r0,#0x5c
  0008578a: mov r0,r8
  0008578c: blx 0x0006f688
  00085790: mov r0,r5
  00085792: mov r1,r8
  00085794: blx 0x0006eb3c
  00085798: b 0x000857ac
  0008579a: add.w r0,r1,#0x3c
  0008579e: vld1.32 {d16,d17},[r0]
  000857a2: ldr r0,[r1,#0x4c]
  000857a4: mov r1,r5
  000857a6: vst1.64 {d16,d17},[r1]!
  000857aa: str r0,[r1,#0x0]
  000857ac: mov r0,r11
  000857ae: mov r1,r5
  000857b0: blx 0x0006eca4
  000857b4: add.w r10,r10,#0x1
  000857b8: ldr.w r0,[r9,#0x3c]
  000857bc: cmp r10,r0
  000857be: bcc 0x0008574c
  000857c0: ldr.w r10,[sp,#0x4]
  000857c4: add r4,sp,#0x240
  000857c6: mov.w r8,#0x0
  000857ca: movs r6,#0x0
  000857cc: b 0x00085820
  000857ce: ldr.w r0,[r9,#0x50]
  000857d2: ldr.w r0,[r0,r6,lsl #0x2]
  000857d6: str.w r8,[sp,#0x0]
  000857da: ldrd r2,r3,[sp,#0x8]
  000857de: blx 0x0006f7e4
  000857e2: ldr.w r0,[r9,#0x50]
  000857e6: ldr.w r1,[r0,r6,lsl #0x2]
  000857ea: add.w r2,r1,#0xd4
  000857ee: vld1.32 {d16,d17},[r2]
  000857f2: mov r2,r5
  000857f4: ldr.w r1,[r1,#0xe4]
  000857f8: vst1.64 {d16,d17},[r2]!
  000857fc: str r1,[r2,#0x0]
  000857fe: mov r2,r5
  00085800: ldr.w r0,[r0,r6,lsl #0x2]
  00085804: add.w r1,r0,#0x5c
  00085808: mov r0,r4
  0008580a: blx 0x0006f688
  0008580e: mov r0,r5
  00085810: mov r1,r4
  00085812: blx 0x0006eb3c
  00085816: mov r0,r11
  00085818: mov r1,r5
  0008581a: blx 0x0006eca4
  0008581e: adds r6,#0x1
  00085820: ldr.w r0,[r9,#0x4c]
  00085824: cmp r6,r0
  00085826: bcc 0x000857ce
  00085828: ldr r0,[sp,#0x2bc]
  0008582a: ldr.w r1,[r10,#0x0]
  0008582e: subs r0,r1,r0
  00085830: itttt eq
  00085832: add.eq.w sp,sp,#0x2c0
  00085836: vpop.eq {d8,d9,d10,d11}
  0008583a: add.eq sp,#0x4
  0008583c: pop.eq.w {r8,r9,r10,r11}
  00085840: it eq
  00085842: pop.eq {r4,r5,r6,r7,pc}
  00085844: blx 0x0006e824
```
