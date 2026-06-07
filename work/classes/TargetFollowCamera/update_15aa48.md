# TargetFollowCamera::update
elf 0x15aa48 body 2030
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* TargetFollowCamera::update(int) */

void TargetFollowCamera::update(int param_1)

{
  bool bVar1;
  char cVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  longlong lVar5;
  byte bVar6;
  undefined1 auVar7 [16];
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  undefined4 *puVar11;
  int iVar12;
  int iVar13;
  uint in_r1;
  Matrix *pMVar14;
  Vector *pVVar15;
  code *pcVar16;
  int iVar17;
  Vector *pVVar18;
  int *piVar19;
  uint in_fpscr;
  uint uVar20;
  uint uVar21;
  float fVar22;
  float fVar23;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s2_00;
  float fVar24;
  float fVar25;
  double dVar26;
  double dVar27;
  double dVar28;
  double dVar29;
  double dVar30;
  double dVar31;
  Vector aVStack_140 [12];
  undefined4 local_134;
  undefined4 uStack_130;
  undefined4 uStack_12c;
  undefined8 local_f8;
  undefined4 local_f0;
  undefined8 local_e8;
  undefined4 local_e0;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined4 local_b0;
  undefined8 local_a8;
  undefined4 local_a0;
  Matrix aMStack_98 [12];
  undefined4 local_8c;
  undefined4 local_7c;
  undefined4 local_6c;
  int local_5c;
  
  piVar19 = *(int **)(DAT_0016abb4 + 0x16aa60);
  local_5c = *piVar19;
  if (*(char *)(param_1 + 0x138) == '\0') {
    if ((0 < (int)in_r1) && (*(char *)(param_1 + 0x46) != '\0')) {
      uVar10 = AEGeometry::getMatrix();
      __aeabi_memcpy(aMStack_98,uVar10,0x3c);
      uVar9 = DAT_0016b270;
      uVar8 = DAT_0016b268;
      if (*(char *)(param_1 + 0x45) == '\0') {
        if (*(char *)(param_1 + 0x44) == '\0') {
          if (*(char *)(param_1 + 0xf0) == '\0') {
            local_a8 = *(undefined8 *)(param_1 + 0x14);
            local_a0 = *(undefined4 *)(param_1 + 0x1c);
            local_f8 = *(undefined8 *)(param_1 + 8);
            local_f0 = *(undefined4 *)(param_1 + 0x10);
            AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_e8);
            AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x20),(Vector *)&local_e8);
            uVar9 = DAT_0016b270;
            uVar8 = DAT_0016b268;
            pVVar18 = (Vector *)(param_1 + 8);
            pVVar15 = (Vector *)(param_1 + 0x14);
            if (*(char *)(param_1 + 0x4c) != '\0') {
              uStack_cc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
              uStack_c8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
              uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
              puVar11 = (undefined4 *)((uint)&local_e8 | 4);
              *puVar11 = 0;
              puVar11[1] = uStack_cc;
              puVar11[2] = uStack_c8;
              puVar11[3] = uStack_c4;
              local_d0 = 0;
              uStack_c0 = uVar8;
              uStack_b8 = uVar9;
              local_e8 = CONCAT44(local_e8._4_4_,0x3f800000);
              local_d4 = 0x3f800000;
              local_b0 = 0x3f800000;
              AbyssEngine::AEMath::MatrixSetRotation
                        (&local_134,(Matrix *)&local_e8,*(undefined4 *)(param_1 + 0x50),
                         *(undefined4 *)(param_1 + 0x54),*(undefined4 *)(param_1 + 0x58),2);
              AbyssEngine::AEMath::Matrix::operator*=(aMStack_98,(Matrix *)&local_e8);
              fVar24 = (float)AbyssEngine::AEMath::VectorLength((Vector *)(param_1 + 0x38));
              in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar24 == *(float *)(param_1 + 0xb0)) << 0x1e
              ;
              if ((byte)(in_fpscr >> 0x1e) == 0) {
                AbyssEngine::AEMath::Vector::operator*=
                          ((Vector *)(param_1 + 0x38),*(float *)(param_1 + 0xb0) / fVar24);
              }
            }
            AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_e8,(Vector *)aMStack_98);
            pcVar16 = *(code **)(DAT_0016b27c + 0x16b104);
            (*pcVar16)(pVVar18,(Matrix *)&local_e8);
            AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_e8,(Vector *)aMStack_98);
            (*pcVar16)(pVVar15,(Matrix *)&local_e8);
            AbyssEngine::AEMath::operator-((Vector *)&local_e8,pVVar15);
            uVar3 = (longlong)(int)in_r1 * (longlong)(int)in_r1;
            iVar13 = (int)(uVar3 >> 0x20);
            uVar4 = (uVar3 & 0xffffffff) * (ulonglong)in_r1;
            lVar5 = (uVar4 & 0xffffffff) * (ulonglong)in_r1;
            iVar12 = iVar13 * in_r1 + (int)uVar3 * ((int)in_r1 >> 0x1f) + (int)(uVar4 >> 0x20);
            dVar28 = (double)__aeabi_l2d((int)lVar5,
                                         iVar12 * in_r1 +
                                         (int)uVar4 * ((int)in_r1 >> 0x1f) +
                                         (int)((ulonglong)lVar5 >> 0x20));
            dVar29 = (double)__aeabi_l2d((int)uVar4,iVar12);
            auVar7 = *(undefined1 (*) [16])(param_1 + 0x60);
            dVar30 = *(double *)(param_1 + 0x70);
            dVar26 = *(double *)(param_1 + 0x78);
            dVar27 = (double)__aeabi_l2d((int)uVar3,iVar13);
            dVar31 = (double)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
            fVar24 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
            AbyssEngine::AEMath::Vector::operator*=
                      ((Vector *)&local_e8,
                       (float)((*(double *)(param_1 + 0x80) +
                               auVar7._8_8_ * dVar29 + auVar7._0_8_ * dVar28 + dVar30 * dVar27 +
                               dVar26 * dVar31) * (double)(1.0 / fVar24)));
            AbyssEngine::AEMath::operator+((Vector *)&local_134,(Vector *)&local_a8);
            (*pcVar16)(pVVar15,(Vector *)&local_134);
            AbyssEngine::AEMath::operator-((Vector *)&local_134,pVVar18);
            AbyssEngine::AEMath::Vector::operator*=
                      ((Vector *)&local_134,
                       (float)((*(double *)(param_1 + 0xa8) +
                               SUB168(*(undefined1 (*) [16])(param_1 + 0x88),8) * dVar29 +
                               SUB168(*(undefined1 (*) [16])(param_1 + 0x88),0) * dVar28 +
                               *(double *)(param_1 + 0x98) * dVar27 +
                               *(double *)(param_1 + 0xa0) * dVar31) * (double)(1.0 / fVar24)));
            if (*(char *)(param_1 + 0x100) != '\0') {
              fVar24 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_134);
              fVar24 = fVar24 + *(float *)(param_1 + 0x104);
              bVar1 = fVar24 < DAT_0016b278;
              in_fpscr = in_fpscr & 0xfffffff;
              *(float *)(param_1 + 0x104) = fVar24;
              *(bool *)(param_1 + 0x100) = bVar1;
            }
            pMVar14 = (Matrix *)aVStack_140;
            AbyssEngine::AEMath::operator+((Vector *)pMVar14,(Vector *)&local_f8);
          }
          else {
            AbyssEngine::AEMath::MatrixIdentity((AEMath *)&local_e8,aMStack_98);
            AbyssEngine::AEMath::Matrix::operator=(aMStack_98,(Matrix *)&local_e8);
            AbyssEngine::AEMath::MatrixSetRotation
                      ((Matrix *)&local_e8,extraout_s0,extraout_s1_00,extraout_s2_00);
            AbyssEngine::AEMath::operator*((Matrix *)&local_e8,(Matrix *)(param_1 + 0xb4));
            AbyssEngine::AEMath::Matrix::operator=(aMStack_98,(Matrix *)&local_e8);
            local_e8 = *(undefined8 *)(param_1 + 8);
            local_e0 = *(undefined4 *)(param_1 + 0x10);
            AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_134,(Vector *)aMStack_98);
            pVVar18 = (Vector *)(param_1 + 8);
            AbyssEngine::AEMath::Vector::operator=(pVVar18,(Vector *)&local_134);
            in_fpscr = in_fpscr & 0xfffffff;
            uVar21 = in_fpscr | (uint)(*(float *)(param_1 + 0x108) == 0.0) << 0x1e;
            if (((byte)(uVar21 >> 0x1e) != 0) ||
               (uVar21 = in_fpscr, *(float *)(param_1 + 0x104) < *(float *)(param_1 + 0x108) * 1.5))
            {
              AbyssEngine::AEMath::operator-((Vector *)&local_134,pVVar18);
              uVar21 = uVar21 & 0xfffffff | (uint)(*(float *)(param_1 + 0x108) == 0.0) << 0x1e;
              if ((byte)(uVar21 >> 0x1e) != 0) {
                uVar10 = AbyssEngine::AEMath::VectorLength((Vector *)&local_134);
                *(undefined4 *)(param_1 + 0x108) = uVar10;
              }
              iVar12 = in_r1 * in_r1 * in_r1;
              dVar28 = (double)VectorSignedToFloat(iVar12,(byte)(uVar21 >> 0x16) & 3);
              dVar31 = (double)VectorSignedToFloat(iVar12 * in_r1,(byte)(uVar21 >> 0x16) & 3);
              dVar29 = (double)VectorSignedToFloat(in_r1 * in_r1,(byte)(uVar21 >> 0x16) & 3);
              dVar30 = (double)VectorSignedToFloat(in_r1,(byte)(uVar21 >> 0x16) & 3);
              fVar24 = (float)VectorSignedToFloat(in_r1,(byte)(uVar21 >> 0x16) & 3);
              AbyssEngine::AEMath::Vector::operator*=
                        ((Vector *)&local_134,
                         (float)((*(double *)(param_1 + 0xa8) +
                                 *(double *)(param_1 + 0x90) * dVar28 +
                                 *(double *)(param_1 + 0x88) * dVar31 +
                                 *(double *)(param_1 + 0x98) * dVar29 +
                                 *(double *)(param_1 + 0xa0) * dVar30) * (double)(1.0 / fVar24)));
              fVar24 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_134);
              fVar24 = fVar24 + *(float *)(param_1 + 0x104);
              fVar22 = *(float *)(param_1 + 0x108) * 0.75;
              *(float *)(param_1 + 0x104) = fVar24;
              uVar21 = uVar21 & 0xfffffff | (uint)(fVar24 < fVar22) << 0x1f |
                       (uint)(fVar24 == fVar22) << 0x1e;
              in_fpscr = uVar21 | (uint)(NAN(fVar24) || NAN(fVar22)) << 0x1c;
              bVar6 = (byte)(uVar21 >> 0x18);
              *(bool *)(param_1 + 0x100) =
                   !(bool)(bVar6 >> 6 & 1) && bVar6 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1);
              AbyssEngine::AEMath::operator+((Vector *)&local_a8,(Vector *)&local_e8);
              AbyssEngine::AEMath::Vector::operator=(pVVar18,(Vector *)&local_a8);
            }
            local_8c = *(undefined4 *)(param_1 + 8);
            local_7c = *(undefined4 *)(param_1 + 0xc);
            local_6c = *(undefined4 *)(param_1 + 0x10);
            AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_134);
            AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x20),(Vector *)&local_134);
            AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_134);
            pMVar14 = (Matrix *)&local_a8;
            AbyssEngine::AEMath::MatrixTransformVector(pMVar14,(Vector *)aMStack_98);
            pVVar18 = (Vector *)(param_1 + 0x14);
          }
          AbyssEngine::AEMath::Vector::operator=(pVVar18,(Vector *)pMVar14);
        }
        else {
          *(undefined1 *)(param_1 + 0x100) = 0;
          AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_e8);
          AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x14),(Vector *)&local_e8);
          *(float *)(param_1 + 0x14) = *(float *)(param_1 + 0x14) - *(float *)(param_1 + 8);
          *(float *)(param_1 + 0x18) = *(float *)(param_1 + 0x18) - *(float *)(param_1 + 0xc);
          *(float *)(param_1 + 0x1c) = *(float *)(param_1 + 0x1c) - *(float *)(param_1 + 0x10);
        }
      }
      else {
        if (*(char *)(param_1 + 0x10c) == '\0') {
          uStack_cc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
          uStack_c8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
          uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
          puVar11 = (undefined4 *)((uint)&local_e8 | 4);
          *puVar11 = 0;
          puVar11[1] = uStack_cc;
          puVar11[2] = uStack_c8;
          puVar11[3] = uStack_c4;
          local_d0 = 0;
          uStack_c0 = uVar8;
          uStack_b8 = uVar9;
          local_e8 = CONCAT44(local_e8._4_4_,0x3f800000);
          local_d4 = 0x3f800000;
          local_b0 = 0x3f800000;
        }
        else {
          __aeabi_memcpy(&local_e8,aMStack_98,0x3c);
        }
        AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_134);
        AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x20),(Vector *)&local_134);
        AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_e8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x14),(Vector *)&local_e8);
        *(undefined1 *)(param_1 + 0x100) = 0;
      }
      if (*(char *)(param_1 + 0xf0) != '\0') {
        AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_e8);
        pcVar16 = *(code **)(DAT_0016b014 + 0x16ac3c);
        (*pcVar16)((Vector *)(param_1 + 8),(Matrix *)&local_e8);
        AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_e8);
        (*pcVar16)(param_1 + 0x20,(Matrix *)&local_e8);
        AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_134);
        AbyssEngine::AEMath::operator-((Vector *)&local_e8,(Vector *)(param_1 + 8));
        (*pcVar16)(param_1 + 0x14,(Vector *)&local_e8);
      }
      if (*(char *)(param_1 + 0x47) != '\0') {
        iVar12 = *(int *)(param_1 + 0x48) - in_r1;
        *(int *)(param_1 + 0x48) = iVar12;
        if (iVar12 < 1) {
          *(undefined1 *)(param_1 + 0x47) = 0;
        }
        iVar12 = *(int *)(param_1 + 0x120);
        puVar11 = *(undefined4 **)(DAT_0016b018 + 0x16ac9c);
        pcVar16 = *(code **)(DAT_0016b01c + 0x16aca2);
        fVar24 = DAT_0016b020;
        if (*(char *)(param_1 + 0xf0) == '\0') {
          fVar24 = 1.0;
        }
        iVar13 = (*pcVar16)(*puVar11,iVar12 << 1);
        fVar22 = (float)VectorSignedToFloat(iVar13 - iVar12,(byte)(in_fpscr >> 0x16) & 3);
        iVar13 = *(int *)(param_1 + 0x120);
        *(float *)(param_1 + 8) = *(float *)(param_1 + 8) + fVar24 * fVar22;
        iVar12 = (*pcVar16)(*puVar11,iVar13 << 1);
        fVar22 = (float)VectorSignedToFloat(iVar12 - iVar13,(byte)(in_fpscr >> 0x16) & 3);
        iVar13 = *(int *)(param_1 + 0x120);
        *(float *)(param_1 + 0xc) = *(float *)(param_1 + 0xc) + fVar24 * fVar22;
        iVar12 = (*pcVar16)(*puVar11,iVar13 << 1);
        fVar22 = (float)VectorSignedToFloat(iVar12 - iVar13,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(param_1 + 0x10) = *(float *)(param_1 + 0x10) + fVar24 * fVar22;
      }
      fVar24 = *(float *)(param_1 + 0x110);
      uVar21 = in_fpscr & 0xfffffff | (uint)(fVar24 < 0.0) << 0x1f | (uint)(fVar24 == 0.0) << 0x1e;
      uVar20 = uVar21 | (uint)NAN(fVar24) << 0x1c;
      bVar6 = (byte)(uVar21 >> 0x18);
      if (!(bool)(bVar6 >> 6 & 1) && bVar6 >> 7 == ((byte)(uVar20 >> 0x1c) & 1)) {
        iVar17 = *(int *)(param_1 + 0x114);
        cVar2 = *(char *)(param_1 + 0xf0);
        puVar11 = *(undefined4 **)(DAT_0016b024 + 0x16ad2e);
        pcVar16 = *(code **)(DAT_0016b028 + 0x16ad34);
        iVar13 = iVar17 << 1;
        iVar12 = (*pcVar16)(*puVar11,iVar13);
        fVar23 = (float)VectorSignedToFloat(iVar12 - iVar17,(byte)(uVar20 >> 0x16) & 3);
        fVar22 = DAT_0016b02c;
        if (cVar2 == '\0') {
          fVar22 = 1.0;
        }
        fVar25 = *(float *)(param_1 + 0x110);
        *(float *)(param_1 + 0x14) = *(float *)(param_1 + 0x14) + fVar22 * fVar24 * fVar23;
        iVar12 = (*pcVar16)(*puVar11,iVar13);
        fVar24 = (float)VectorSignedToFloat(iVar12 - iVar17,(byte)(uVar20 >> 0x16) & 3);
        fVar23 = *(float *)(param_1 + 0x110);
        *(float *)(param_1 + 0x18) = *(float *)(param_1 + 0x18) + fVar22 * fVar25 * fVar24;
        iVar12 = (*pcVar16)(*puVar11,iVar13);
        fVar24 = (float)VectorSignedToFloat(iVar12 - iVar17,(byte)(uVar20 >> 0x16) & 3);
        *(float *)(param_1 + 0x1c) = *(float *)(param_1 + 0x1c) + fVar22 * fVar23 * fVar24;
      }
      AbyssEngine::AEMath::MatrixGetLookAt
                ((AEMath *)&local_e8,(Vector *)(param_1 + 8),(Vector *)(param_1 + 0x14),
                 (Vector *)(param_1 + 0x20));
      AbyssEngine::AEMath::Matrix::operator=(aMStack_98,(Matrix *)&local_e8);
      uStack_b8 = DAT_0016b270;
      uStack_c0 = DAT_0016b268;
      uStack_cc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uStack_c8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar11 = (undefined4 *)((uint)&local_e8 | 4);
      *puVar11 = 0;
      puVar11[1] = uStack_cc;
      puVar11[2] = uStack_c8;
      puVar11[3] = uStack_c4;
      local_d0 = 0;
      local_e8 = CONCAT44(local_e8._4_4_,0x3f800000);
      local_d4 = 0x3f800000;
      local_b0 = 0x3f800000;
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)&local_134,*(float *)(param_1 + 0x130),extraout_s1,extraout_s2);
      AbyssEngine::AEMath::Matrix::operator*=(aMStack_98,(Matrix *)&local_e8);
      AbyssEngine::PaintCanvas::CameraSetLocal
                (**(uint **)(DAT_0016b030 + 0x16ae32),*(Matrix **)param_1);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(param_1 + 0x13c),aMStack_98);
    }
  }
  else {
    AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_134);
    *(undefined4 *)(param_1 + 8) = local_134;
    *(undefined4 *)(param_1 + 0xc) = uStack_130;
    *(undefined4 *)(param_1 + 0x10) = uStack_12c;
    AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 8),(Vector *)&local_134);
    AbyssEngine::PaintCanvas::CameraSetLocal
              (**(uint **)(DAT_0016abb8 + 0x16aa9a),*(Matrix **)param_1);
    if (*(int *)(param_1 + 4) != 0) {
      uVar10 = AEGeometry::getMatrix();
      __aeabi_memcpy(aMStack_98,uVar10,0x3c);
      uVar9 = DAT_0016b270;
      uVar8 = DAT_0016b268;
      if (*(char *)(param_1 + 0x10c) == '\0') {
        uStack_cc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_c8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar11 = (undefined4 *)((uint)&local_e8 | 4);
        *puVar11 = 0;
        puVar11[1] = uStack_cc;
        puVar11[2] = uStack_c8;
        puVar11[3] = uStack_c4;
        local_d0 = 0;
        uStack_c0 = uVar8;
        uStack_b8 = uVar9;
        local_e8 = CONCAT44(local_e8._4_4_,0x3f800000);
        local_d4 = 0x3f800000;
        local_b0 = 0x3f800000;
      }
      else {
        __aeabi_memcpy(&local_e8,aMStack_98,0x3c);
      }
      AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_a8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x20),(Vector *)&local_a8);
      AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_e8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x14),(Vector *)&local_e8);
    }
  }
  if (*piVar19 != local_5c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016aa48: push {r4,r5,r6,r7,lr}
  0016aa4a: add r7,sp,#0xc
  0016aa4c: push {r7,r8,r9,r10,r11}
  0016aa50: vpush {d8,d9,d10,d11,d12,d13}
  0016aa54: sub sp,#0x100
  0016aa56: mov r4,r0
  0016aa58: ldr r0,[0x0016abb4]
  0016aa5a: mov r11,r1
  0016aa5c: add r0,pc
  0016aa5e: ldr.w r9,[r0,#0x0]
  0016aa62: ldr.w r0,[r9,#0x0]
  0016aa66: str r0,[sp,#0xfc]
  0016aa68: ldrb.w r0,[r4,#0x138]
  0016aa6c: cbz r0,0x0016aac8
  0016aa6e: add.w r5,r4,#0x13c
  0016aa72: add r6,sp,#0x24
  0016aa74: mov r0,r6
  0016aa76: mov r1,r5
  0016aa78: blx 0x0006f16c
  0016aa7c: add r2,sp,#0x24
  0016aa7e: add.w r3,r4,#0x8
  0016aa82: ldmia r2,{r0,r1,r2}
  0016aa84: stmia r3!,{r0,r1,r2}
  0016aa86: add.w r0,r4,#0x8
  0016aa8a: mov r1,r6
  0016aa8c: blx 0x0006eb3c
  0016aa90: ldr r0,[0x0016abb8]
  0016aa92: mov r2,r5
  0016aa94: ldr r1,[r4,#0x0]
  0016aa96: add r0,pc
  0016aa98: ldr r0,[r0,#0x0]
  0016aa9a: ldr r0,[r0,#0x0]
  0016aa9c: blx 0x000720a0
  0016aaa0: ldr r0,[r4,#0x4]
  0016aaa2: cmp r0,#0x0
  0016aaa4: beq.w 0x0016ae42
  0016aaa8: blx 0x000721cc
  0016aaac: mov r1,r0
  0016aaae: add r0,sp,#0xc0
  0016aab0: movs r2,#0x3c
  0016aab2: blx 0x0006f1e4
  0016aab6: ldrb.w r0,[r4,#0x10c]
  0016aaba: cbz r0,0x0016ab04
  0016aabc: add r0,sp,#0x70
  0016aabe: add r1,sp,#0xc0
  0016aac0: movs r2,#0x3c
  0016aac2: blx 0x0006f1e4
  0016aac6: b 0x0016ab34
  0016aac8: cmp.w r11,#0x1
  0016aacc: blt.w 0x0016ae42
  0016aad0: ldrb.w r0,[r4,#0x46]
  0016aad4: cmp r0,#0x0
  0016aad6: beq.w 0x0016ae42
  0016aada: ldr r0,[r4,#0x4]
  0016aadc: blx 0x000721cc
  0016aae0: mov r1,r0
  0016aae2: add r0,sp,#0xc0
  0016aae4: movs r2,#0x3c
  0016aae6: blx 0x0006f1e4
  0016aaea: ldrb.w r0,[r4,#0x45]
  0016aaee: cbz r0,0x0016ab5e
  0016aaf0: ldrb.w r0,[r4,#0x10c]
  0016aaf4: cmp r0,#0x0
  0016aaf6: beq 0x0016abbc
  0016aaf8: add r0,sp,#0x70
  0016aafa: add r1,sp,#0xc0
  0016aafc: movs r2,#0x3c
  0016aafe: blx 0x0006f1e4
  0016ab02: b 0x0016abec
  0016ab04: adr.w r0,0x16b268
  0016ab08: vmov.i32 q8,#0x0
  0016ab0c: add r1,sp,#0x70
  0016ab0e: vld1.64 {d18,d19},[r0]
  0016ab12: orr r0,r1,#0x4
  0016ab16: vst1.32 {d16,d17},[r0]
  0016ab1a: add.w r0,r1,#0x18
  0016ab1e: vst1.64 {d16,d17},[r0]
  0016ab22: add.w r0,r1,#0x28
  0016ab26: vst1.64 {d18,d19},[r0]
  0016ab2a: mov.w r0,#0x3f800000
  0016ab2e: str r0,[sp,#0x70]
  0016ab30: str r0,[sp,#0x84]
  0016ab32: str r0,[sp,#0xa8]
  0016ab34: add r5,sp,#0xb0
  0016ab36: add r1,sp,#0x70
  0016ab38: mov r0,r5
  0016ab3a: blx 0x0006f4d8
  0016ab3e: add.w r0,r4,#0x20
  0016ab42: mov r1,r5
  0016ab44: blx 0x0006eb3c
  0016ab48: add r5,sp,#0x70
  0016ab4a: add r1,sp,#0xc0
  0016ab4c: mov r0,r5
  0016ab4e: blx 0x0006f16c
  0016ab52: add.w r0,r4,#0x14
  0016ab56: mov r1,r5
  0016ab58: blx 0x0006eb3c
  0016ab5c: b 0x0016ae42
  0016ab5e: ldrb.w r0,[r4,#0x44]
  0016ab62: cmp r0,#0x0
  0016ab64: beq.w 0x0016ae60
  0016ab68: add r6,sp,#0x70
  0016ab6a: movs r0,#0x0
  0016ab6c: add r1,sp,#0xc0
  0016ab6e: strb.w r0,[r4,#0x100]
  0016ab72: mov r0,r6
  0016ab74: blx 0x0006f16c
  0016ab78: add.w r0,r4,#0x14
  0016ab7c: mov r1,r6
  0016ab7e: blx 0x0006eb3c
  0016ab82: vldr.32 s0,[r4,#0x8]
  0016ab86: vldr.32 s6,[r4,#0x14]
  0016ab8a: vldr.32 s2,[r4,#0xc]
  0016ab8e: vldr.32 s8,[r4,#0x18]
  0016ab92: vsub.f32 s0,s6,s0
  0016ab96: vldr.32 s4,[r4,#0x10]
  0016ab9a: vldr.32 s10,[r4,#0x1c]
  0016ab9e: vsub.f32 s2,s8,s2
  0016aba2: vsub.f32 s4,s10,s4
  0016aba6: vstr.32 s0,[r4,#0x14]
  0016abaa: vstr.32 s2,[r4,#0x18]
  0016abae: vstr.32 s4,[r4,#0x1c]
  0016abb2: b 0x0016ac1a
  0016abbc: adr.w r0,0x16b268
  0016abc0: vmov.i32 q8,#0x0
  0016abc4: add r1,sp,#0x70
  0016abc6: vld1.64 {d18,d19},[r0]
  0016abca: orr r0,r1,#0x4
  0016abce: vst1.32 {d16,d17},[r0]
  0016abd2: add.w r0,r1,#0x18
  0016abd6: vst1.64 {d16,d17},[r0]
  0016abda: add.w r0,r1,#0x28
  0016abde: vst1.64 {d18,d19},[r0]
  0016abe2: mov.w r0,#0x3f800000
  0016abe6: str r0,[sp,#0x70]
  0016abe8: str r0,[sp,#0x84]
  0016abea: str r0,[sp,#0xa8]
  0016abec: add r6,sp,#0x24
  0016abee: add r1,sp,#0x70
  0016abf0: mov r0,r6
  0016abf2: blx 0x0006f4d8
  0016abf6: add.w r0,r4,#0x20
  0016abfa: mov r1,r6
  0016abfc: blx 0x0006eb3c
  0016ac00: add r6,sp,#0x70
  0016ac02: add r1,sp,#0xc0
  0016ac04: mov r0,r6
  0016ac06: blx 0x0006f16c
  0016ac0a: add.w r0,r4,#0x14
  0016ac0e: mov r1,r6
  0016ac10: blx 0x0006eb3c
  0016ac14: movs r0,#0x0
  0016ac16: strb.w r0,[r4,#0x100]
  0016ac1a: ldrb.w r0,[r4,#0xf0]
  0016ac1e: cbz r0,0x0016ac76
  0016ac20: add.w r10,sp,#0x70
  0016ac24: add.w r8,sp,#0xc0
  0016ac28: mov r0,r10
  0016ac2a: mov r1,r8
  0016ac2c: blx 0x0006f16c
  0016ac30: ldr r0,[0x0016b014]
  0016ac32: add.w r5,r4,#0x8
  0016ac36: mov r1,r10
  0016ac38: add r0,pc
  0016ac3a: ldr r6,[r0,#0x0]
  0016ac3c: mov r0,r5
  0016ac3e: blx r6
  0016ac40: add.w r10,sp,#0x70
  0016ac44: mov r1,r8
  0016ac46: mov r0,r10
  0016ac48: blx 0x0006f4d8
  0016ac4c: add.w r0,r4,#0x20
  0016ac50: mov r1,r10
  0016ac52: blx r6
  0016ac54: add.w r10,sp,#0x24
  0016ac58: mov r1,r8
  0016ac5a: mov r0,r10
  0016ac5c: blx 0x0006f4c0
  0016ac60: add.w r8,sp,#0x70
  0016ac64: mov r1,r5
  0016ac66: mov r2,r10
  0016ac68: mov r0,r8
  0016ac6a: blx 0x0006ec38
  0016ac6e: add.w r0,r4,#0x14
  0016ac72: mov r1,r8
  0016ac74: blx r6
  0016ac76: ldrb.w r0,[r4,#0x47]
  0016ac7a: cmp r0,#0x0
  0016ac7c: beq 0x0016ad18
  0016ac7e: ldr r0,[r4,#0x48]
  0016ac80: sub.w r0,r0,r11
  0016ac84: str r0,[r4,#0x48]
  0016ac86: cmp r0,#0x0
  0016ac88: bgt 0x0016ac90
  0016ac8a: movs r0,#0x0
  0016ac8c: strb.w r0,[r4,#0x47]
  0016ac90: ldr r0,[0x0016b018]
  0016ac92: vmov.f32 s0,0x3f800000
  0016ac96: ldr r1,[0x0016b01c]
  0016ac98: add r0,pc
  0016ac9a: ldr.w r5,[r4,#0x120]
  0016ac9e: add r1,pc
  0016aca0: ldrb.w r2,[r4,#0xf0]
  0016aca4: ldr.w r8,[r0,#0x0]
  0016aca8: ldr r6,[r1,#0x0]
  0016acaa: lsls r1,r5,#0x1
  0016acac: vldr.32 s16,[pc,#0x370]
  0016acb0: cmp r2,#0x0
  0016acb2: ldr.w r0,[r8,#0x0]
  0016acb6: it eq
  0016acb8: vmov.eq.f32 s16,s0
  0016acbc: blx r6
  0016acbe: subs r0,r0,r5
  0016acc0: vmov s0,r0
  0016acc4: vcvt.f32.s32 s0,s0
  0016acc8: vldr.32 s2,[r4,#0x8]
  0016accc: ldr.w r5,[r4,#0x120]
  0016acd0: lsls r1,r5,#0x1
  0016acd2: vmla.f32 s2,s16,s0
  0016acd6: vstr.32 s2,[r4,#0x8]
  0016acda: ldr.w r0,[r8,#0x0]
  0016acde: blx r6
  0016ace0: subs r0,r0,r5
  0016ace2: vmov s0,r0
  0016ace6: vcvt.f32.s32 s0,s0
  0016acea: vldr.32 s2,[r4,#0xc]
  0016acee: ldr.w r5,[r4,#0x120]
  0016acf2: lsls r1,r5,#0x1
  0016acf4: vmla.f32 s2,s16,s0
  0016acf8: vstr.32 s2,[r4,#0xc]
  0016acfc: ldr.w r0,[r8,#0x0]
  0016ad00: blx r6
  0016ad02: subs r0,r0,r5
  0016ad04: vmov s0,r0
  0016ad08: vcvt.f32.s32 s0,s0
  0016ad0c: vldr.32 s2,[r4,#0x10]
  0016ad10: vmla.f32 s2,s16,s0
  0016ad14: vstr.32 s2,[r4,#0x10]
  0016ad18: vldr.32 s16,[r4,#0x110]
  0016ad1c: vcmpe.f32 s16,#0
  0016ad20: vmrs apsr,fpscr
  0016ad24: ble 0x0016adc0
  0016ad26: ldr r0,[0x0016b024]
  0016ad28: ldr r1,[0x0016b028]
  0016ad2a: add r0,pc
  0016ad2c: ldr.w r6,[r4,#0x114]
  0016ad30: add r1,pc
  0016ad32: ldrb.w r11,[r4,#0xf0]
  0016ad36: ldr.w r8,[r0,#0x0]
  0016ad3a: ldr.w r10,[r1,#0x0]
  0016ad3e: lsls r5,r6,#0x1
  0016ad40: mov r1,r5
  0016ad42: ldr.w r0,[r8,#0x0]
  0016ad46: blx r10
  0016ad48: subs r0,r0,r6
  0016ad4a: vmov.f32 s2,0x3f800000
  0016ad4e: vldr.32 s18,[pc,#0x2dc]
  0016ad52: cmp.w r11,#0x0
  0016ad56: vmov s0,r0
  0016ad5a: mov r1,r5
  0016ad5c: vcvt.f32.s32 s0,s0
  0016ad60: it eq
  0016ad62: vmov.eq.f32 s18,s2
  0016ad66: vldr.32 s2,[r4,#0x14]
  0016ad6a: vmul.f32 s0,s16,s0
  0016ad6e: vldr.32 s16,[r4,#0x110]
  0016ad72: vmla.f32 s2,s18,s0
  0016ad76: vstr.32 s2,[r4,#0x14]
  0016ad7a: ldr.w r0,[r8,#0x0]
  0016ad7e: blx r10
  0016ad80: subs r0,r0,r6
  0016ad82: mov r1,r5
  0016ad84: vmov s0,r0
  0016ad88: vcvt.f32.s32 s0,s0
  0016ad8c: vldr.32 s2,[r4,#0x18]
  0016ad90: vmul.f32 s0,s16,s0
  0016ad94: vldr.32 s16,[r4,#0x110]
  0016ad98: vmla.f32 s2,s18,s0
  0016ad9c: vstr.32 s2,[r4,#0x18]
  0016ada0: ldr.w r0,[r8,#0x0]
  0016ada4: blx r10
  0016ada6: subs r0,r0,r6
  0016ada8: vmov s0,r0
  0016adac: vcvt.f32.s32 s0,s0
  0016adb0: vldr.32 s2,[r4,#0x1c]
  0016adb4: vmul.f32 s0,s16,s0
  0016adb8: vmla.f32 s2,s18,s0
  0016adbc: vstr.32 s2,[r4,#0x1c]
  0016adc0: add r6,sp,#0x70
  0016adc2: add.w r1,r4,#0x8
  0016adc6: add.w r2,r4,#0x14
  0016adca: add.w r3,r4,#0x20
  0016adce: mov r0,r6
  0016add0: blx 0x000725e0
  0016add4: add r5,sp,#0xc0
  0016add6: mov r1,r6
  0016add8: mov r0,r5
  0016adda: blx 0x0006f148
  0016adde: adr.w r0,0x16b268
  0016ade2: vmov.i32 q8,#0x0
  0016ade6: vld1.64 {d18,d19},[r0]
  0016adea: orr r0,r6,#0x4
  0016adee: mov r1,r6
  0016adf0: movs r2,#0x0
  0016adf2: vst1.32 {d16,d17},[r0]
  0016adf6: add.w r0,r6,#0x18
  0016adfa: movs r3,#0x0
  0016adfc: vst1.64 {d16,d17},[r0]
  0016ae00: add.w r0,r6,#0x28
  0016ae04: vst1.64 {d18,d19},[r0]
  0016ae08: mov.w r0,#0x3f800000
  0016ae0c: str r0,[sp,#0x70]
  0016ae0e: str r0,[sp,#0x84]
  0016ae10: str r0,[sp,#0xa8]
  0016ae12: add r0,sp,#0x24
  0016ae14: vldr.32 s0,[r4,#0x130]
  0016ae18: vstr.32 s0,[sp]
  0016ae1c: blx 0x00072094
  0016ae20: mov r0,r5
  0016ae22: mov r1,r6
  0016ae24: blx 0x0006f4a8
  0016ae28: ldr r0,[0x0016b030]
  0016ae2a: mov r2,r5
  0016ae2c: ldr r1,[r4,#0x0]
  0016ae2e: add r0,pc
  0016ae30: ldr r0,[r0,#0x0]
  0016ae32: ldr r0,[r0,#0x0]
  0016ae34: blx 0x000720a0
  0016ae38: add.w r0,r4,#0x13c
  0016ae3c: mov r1,r5
  0016ae3e: blx 0x0006f148
  0016ae42: ldr r0,[sp,#0xfc]
  0016ae44: ldr.w r1,[r9,#0x0]
  0016ae48: subs r0,r1,r0
  0016ae4a: itttt eq
  0016ae4c: add.eq sp,#0x100
  0016ae4e: vpop.eq {d8,d9,d10,d11,d12,d13}
  0016ae52: add.eq sp,#0x4
  0016ae54: pop.eq.w {r8,r9,r10,r11}
  0016ae58: it eq
  0016ae5a: pop.eq {r4,r5,r6,r7,pc}
  0016ae5c: blx 0x0006e824
  0016ae60: ldrb.w r0,[r4,#0xf0]
  0016ae64: cmp r0,#0x0
  0016ae66: beq.w 0x0016b034
  0016ae6a: add.w r8,sp,#0x70
  0016ae6e: add r6,sp,#0xc0
  0016ae70: mov r0,r8
  0016ae72: mov r1,r6
  0016ae74: blx 0x0006fb2c
  0016ae78: mov r0,r6
  0016ae7a: mov r1,r8
  0016ae7c: blx 0x0006f148
  0016ae80: movs r0,#0x0
  0016ae82: ldr r3,[0x0016b260]
  0016ae84: str r0,[sp,#0x0]
  0016ae86: add r0,sp,#0x70
  0016ae88: mov r1,r6
  0016ae8a: movs r2,#0x0
  0016ae8c: blx 0x00072094
  0016ae90: add.w r8,sp,#0x70
  0016ae94: add.w r1,r4,#0xb4
  0016ae98: mov r2,r6
  0016ae9a: mov r0,r8
  0016ae9c: blx 0x0006f82c
  0016aea0: mov r0,r6
  0016aea2: mov r1,r8
  0016aea4: blx 0x0006f148
  0016aea8: add.w r8,sp,#0x24
  0016aeac: vldr.64 d16,[r4,#0x8]
  0016aeb0: ldr r0,[r4,#0x10]
  0016aeb2: add.w r2,r4,#0xf4
  0016aeb6: str r0,[sp,#0x78]
  0016aeb8: mov r0,r8
  0016aeba: mov r1,r6
  0016aebc: vstr.64 d16,[sp,#0x70]
  0016aec0: blx 0x0006f688
  0016aec4: add.w r6,r4,#0x8
  0016aec8: mov r1,r8
  0016aeca: mov r0,r6
  0016aecc: blx 0x0006eb3c
  0016aed0: vldr.32 s0,[r4,#0x108]
  0016aed4: vcmpe.f32 s0,#0
  0016aed8: vmrs apsr,fpscr
  0016aedc: beq 0x0016aef4
  0016aede: vmov.f32 s2,0x3fc00000
  0016aee2: vldr.32 s4,[r4,#0x104]
  0016aee6: vmul.f32 s0,s0,s2
  0016aeea: vcmpe.f32 s4,s0
  0016aeee: vmrs apsr,fpscr
  0016aef2: bpl 0x0016afd0
  0016aef4: add r0,sp,#0x24
  0016aef6: add r2,sp,#0x70
  0016aef8: mov r1,r6
  0016aefa: blx 0x0006ec38
  0016aefe: vldr.32 s0,[r4,#0x108]
  0016af02: vcmpe.f32 s0,#0
  0016af06: vmrs apsr,fpscr
  0016af0a: bne 0x0016af16
  0016af0c: add r0,sp,#0x24
  0016af0e: blx 0x0006ec44
  0016af12: str.w r0,[r4,#0x108]
  0016af16: mul r0,r11,r11
  0016af1a: add.w r2,r4,#0x88
  0016af1e: vmov.f32 s2,0x3f800000
  0016af22: add.w r8,sp,#0x24
  0016af26: mul r1,r0,r11
  0016af2a: vmov s0,r1
  0016af2e: mul r1,r1,r11
  0016af32: vcvt.f64.s32 d16,s0
  0016af36: vldmia r2!,{d17,d18,d19,d20}
  0016af3a: vmov s0,r1
  0016af3e: vmul.f64 d16,d18,d16
  0016af42: vcvt.f64.s32 d18,s0
  0016af46: vmla.f64 d16,d17,d18
  0016af4a: vmov s0,r0
  0016af4e: mov r0,r8
  0016af50: vcvt.f64.s32 d17,s0
  0016af54: vmla.f64 d16,d19,d17
  0016af58: vmov s0,r11
  0016af5c: vcvt.f64.s32 d17,s0
  0016af60: vcvt.f32.s32 s0,s0
  0016af64: vmla.f64 d16,d20,d17
  0016af68: vdiv.f32 s0,s2,s0
  0016af6c: vldr.64 d17,[r4,#0xa8]
  0016af70: vadd.f64 d16,d17,d16
  0016af74: vcvt.f64.f32 d17,s0
  0016af78: vmul.f64 d16,d16,d17
  0016af7c: vcvt.f32.f64 s0,d16
  0016af80: vmov r1,s0
  0016af84: blx 0x00072628
  0016af88: mov r0,r8
  0016af8a: blx 0x0006ec44
  0016af8e: vmov.f32 s0,0x3f400000
  0016af92: vldr.32 s4,[r4,#0x108]
  0016af96: vmov s6,r0
  0016af9a: vldr.32 s2,[r4,#0x104]
  0016af9e: movs r0,#0x0
  0016afa0: add r5,sp,#0xb0
  0016afa2: vadd.f32 s2,s6,s2
  0016afa6: add r1,sp,#0x70
  0016afa8: mov r2,r8
  0016afaa: vmul.f32 s0,s4,s0
  0016afae: vstr.32 s2,[r4,#0x104]
  0016afb2: vcmpe.f32 s2,s0
  0016afb6: vmrs apsr,fpscr
  0016afba: it gt
  0016afbc: mov.gt r0,#0x1
  0016afbe: strb.w r0,[r4,#0x100]
  0016afc2: mov r0,r5
  0016afc4: blx 0x0006f1cc
  0016afc8: mov r0,r6
  0016afca: mov r1,r5
  0016afcc: blx 0x0006eb3c
  0016afd0: add.w r2,r4,#0x8
  0016afd4: add.w r8,sp,#0x24
  0016afd8: add r6,sp,#0xc0
  0016afda: ldmia r2,{r0,r1,r2}
  0016afdc: str r0,[sp,#0xcc]
  0016afde: mov r0,r8
  0016afe0: str r1,[sp,#0xdc]
  0016afe2: mov r1,r6
  0016afe4: str r2,[sp,#0xec]
  0016afe6: blx 0x0006f4d8
  0016afea: add.w r0,r4,#0x20
  0016afee: mov r1,r8
  0016aff0: blx 0x0006eb3c
  0016aff4: add.w r8,sp,#0x24
  0016aff8: mov r1,r6
  0016affa: mov r0,r8
  0016affc: blx 0x0006f4c0
  0016b000: add r5,sp,#0xb0
  0016b002: mov r1,r6
  0016b004: mov r2,r8
  0016b006: mov r0,r5
  0016b008: blx 0x0006f688
  0016b00c: add.w r0,r4,#0x14
  0016b010: mov r1,r5
  0016b012: b 0x0016b258
  0016b034: vldr.64 d16,[r4,#0x14]
  0016b038: add r6,sp,#0x70
  0016b03a: ldr r0,[r4,#0x1c]
  0016b03c: add r1,sp,#0xc0
  0016b03e: str r0,[sp,#0xb8]
  0016b040: vstr.64 d16,[sp,#0xb0]
  0016b044: vldr.64 d16,[r4,#0x8]
  0016b048: ldr r0,[r4,#0x10]
  0016b04a: str r0,[sp,#0x68]
  0016b04c: mov r0,r6
  0016b04e: vstr.64 d16,[sp,#0x60]
  0016b052: blx 0x0006f4d8
  0016b056: add.w r0,r4,#0x20
  0016b05a: mov r1,r6
  0016b05c: blx 0x0006eb3c
  0016b060: ldrb.w r0,[r4,#0x4c]
  0016b064: add.w r8,r4,#0x8
  0016b068: add.w r5,r4,#0x14
  0016b06c: str r5,[sp,#0x14]
  0016b06e: cbz r0,0x0016b0e8
  0016b070: adr r0,[0x16b268]
  0016b072: vmov.i32 q8,#0x0
  0016b076: vld1.64 {d18,d19},[r0]
  0016b07a: orr r0,r6,#0x4
  0016b07e: mov r1,r6
  0016b080: vst1.32 {d16,d17},[r0]
  0016b084: add.w r0,r6,#0x18
  0016b088: vst1.64 {d16,d17},[r0]
  0016b08c: add.w r0,r6,#0x28
  0016b090: vst1.64 {d18,d19},[r0]
  0016b094: mov.w r0,#0x3f800000
  0016b098: str r0,[sp,#0x70]
  0016b09a: str r0,[sp,#0x84]
  0016b09c: str r0,[sp,#0xa8]
  0016b09e: movs r0,#0x2
  0016b0a0: ldrd r2,r3,[r4,#0x50]
  0016b0a4: vldr.32 s0,[r4,#0x58]
  0016b0a8: str r0,[sp,#0x4]
  0016b0aa: add r0,sp,#0x24
  0016b0ac: vstr.32 s0,[sp]
  0016b0b0: blx 0x00073060
  0016b0b4: add r0,sp,#0xc0
  0016b0b6: mov r1,r6
  0016b0b8: blx 0x0006f4a8
  0016b0bc: add.w r6,r4,#0x38
  0016b0c0: mov r0,r6
  0016b0c2: blx 0x0006ec44
  0016b0c6: vldr.32 s0,[r4,#0xb0]
  0016b0ca: vmov s2,r0
  0016b0ce: vcmpe.f32 s2,s0
  0016b0d2: vmrs apsr,fpscr
  0016b0d6: beq 0x0016b0ec
  0016b0d8: vdiv.f32 s0,s0,s2
  0016b0dc: mov r0,r6
  0016b0de: vmov r1,s0
  0016b0e2: blx 0x00072628
  0016b0e6: b 0x0016b0ec
  0016b0e8: add.w r6,r4,#0x38
  0016b0ec: add r5,sp,#0x70
  0016b0ee: add r1,sp,#0xc0
  0016b0f0: mov r2,r6
  0016b0f2: mov r0,r5
  0016b0f4: blx 0x0006f688
  0016b0f8: ldr r0,[0x0016b27c]
  0016b0fa: mov r1,r5
  0016b0fc: str.w r8,[sp,#0xc]
  0016b100: add r0,pc
  0016b102: ldr.w r10,[r0,#0x0]
  0016b106: mov r0,r8
  0016b108: blx r10
  0016b10a: add.w r8,sp,#0x70
  0016b10e: add.w r2,r4,#0x2c
  0016b112: add r1,sp,#0xc0
  0016b114: str.w r10,[sp,#0x10]
  0016b118: mov r0,r8
  0016b11a: blx 0x0006f688
  0016b11e: ldr r5,[sp,#0x14]
  0016b120: mov r1,r8
  0016b122: mov r0,r5
  0016b124: blx r10
  0016b126: add r0,sp,#0x70
  0016b128: add r2,sp,#0xb0
  0016b12a: mov r1,r5
  0016b12c: blx 0x0006ec38
  0016b130: smull r8,r6,r11,r11
  0016b134: asr.w r3,r11, asr #0x1f
  0016b138: umull r10,r1,r8,r11
  0016b13c: mla r1,r8,r3,r1
  0016b140: umull r0,r2,r10,r11
  0016b144: mla r5,r6,r11,r1
  0016b148: mla r2,r10,r3,r2
  0016b14c: mla r1,r5,r11,r2
  0016b150: blx 0x00077794
  0016b154: vmov d8,r0,r1
  0016b158: mov r0,r10
  0016b15a: mov r1,r5
  0016b15c: ldr.w r10,[sp,#0xc]
  0016b160: blx 0x00077794
  0016b164: vmov d9,r0,r1
  0016b168: vldr.64 d17,[r4,#0x68]
  0016b16c: vldr.64 d16,[r4,#0x60]
  0016b170: mov r0,r8
  0016b172: mov r1,r6
  0016b174: vmul.f64 d12,d17,d9
  0016b178: vldr.64 d10,[r4,#0x70]
  0016b17c: vldr.64 d11,[r4,#0x78]
  0016b180: vmla.f64 d12,d16,d8
  0016b184: blx 0x00077794
  0016b188: vmov d13,r0,r1
  0016b18c: add r5,sp,#0x70
  0016b18e: vmov s0,r11
  0016b192: vmla.f64 d12,d10,d13
  0016b196: mov r0,r5
  0016b198: vcvt.f64.s32 d10,s0
  0016b19c: vcvt.f32.s32 s0,s0
  0016b1a0: vldr.64 d16,[r4,#0x80]
  0016b1a4: vmov.f32 s2,0x3f800000
  0016b1a8: vmla.f64 d12,d11,d10
  0016b1ac: vdiv.f32 s0,s2,s0
  0016b1b0: vadd.f64 d16,d16,d12
  0016b1b4: vcvt.f64.f32 d11,s0
  0016b1b8: vmul.f64 d16,d16,d11
  0016b1bc: vcvt.f32.f64 s0,d16
  0016b1c0: vmov r1,s0
  0016b1c4: blx 0x00072628
  0016b1c8: add r6,sp,#0x24
  0016b1ca: add r1,sp,#0xb0
  0016b1cc: mov r2,r5
  0016b1ce: mov r0,r6
  0016b1d0: blx 0x0006f1cc
  0016b1d4: ldr r0,[sp,#0x14]
  0016b1d6: mov r1,r6
  0016b1d8: ldr r2,[sp,#0x10]
  0016b1da: blx r2
  0016b1dc: add r6,sp,#0x24
  0016b1de: add r2,sp,#0x60
  0016b1e0: mov r1,r10
  0016b1e2: mov r0,r6
  0016b1e4: blx 0x0006ec38
  0016b1e8: add.w r0,r4,#0x88
  0016b1ec: vldmia r0!,{d16,d17,d18,d19,d20}
  0016b1f0: mov r0,r6
  0016b1f2: vmul.f64 d17,d17,d9
  0016b1f6: vmla.f64 d17,d16,d8
  0016b1fa: vmla.f64 d17,d18,d13
  0016b1fe: vmla.f64 d17,d19,d10
  0016b202: vadd.f64 d16,d20,d17
  0016b206: vmul.f64 d16,d16,d11
  0016b20a: vcvt.f32.f64 s0,d16
  0016b20e: vmov r1,s0
  0016b212: blx 0x00072628
  0016b216: ldrb.w r0,[r4,#0x100]
  0016b21a: cbz r0,0x0016b248
  0016b21c: add r0,sp,#0x24
  0016b21e: blx 0x0006ec44
  0016b222: vmov s2,r0
  0016b226: vldr.32 s0,[r4,#0x104]
  0016b22a: movs r0,#0x0
  0016b22c: vadd.f32 s0,s2,s0
  0016b230: vldr.32 s2,[pc,#0x44]
  0016b234: vcmpe.f32 s0,s2
  0016b238: vstr.32 s0,[r4,#0x104]
  0016b23c: vmrs apsr,fpscr
  0016b240: it mi
  0016b242: mov.mi r0,#0x1
  0016b244: strb.w r0,[r4,#0x100]
  0016b248: add r6,sp,#0x18
  0016b24a: add r1,sp,#0x60
  0016b24c: add r2,sp,#0x24
  0016b24e: mov r0,r6
  0016b250: blx 0x0006f1cc
  0016b254: mov r0,r10
  0016b256: mov r1,r6
  0016b258: blx 0x0006eb3c
  0016b25c: b 0x0016ac1a
```
