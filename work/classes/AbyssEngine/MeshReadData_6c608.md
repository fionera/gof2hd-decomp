# AbyssEngine::MeshReadData
elf 0x6c608 body 2572
Sig: undefined __stdcall MeshReadData(Engine * param_1, uint * param_2, uint param_3, Mesh * * param_4, Material * param_5)

## decompile
```c

/* AbyssEngine::MeshReadData(AbyssEngine::Engine*, unsigned int const&, unsigned int,
   AbyssEngine::Mesh**, AbyssEngine::Material*) */

void AbyssEngine::MeshReadData
               (Engine *param_1,uint *param_2,uint param_3,Mesh **param_4,Material *param_5)

{
  byte *pbVar1;
  char cVar2;
  ushort uVar3;
  byte bVar4;
  int iVar5;
  Mesh *pMVar6;
  void *pvVar7;
  int iVar8;
  void *pvVar9;
  int iVar10;
  undefined4 uVar11;
  float *pfVar12;
  int extraout_r1;
  uint uVar13;
  float *pfVar14;
  uint uVar15;
  float *pfVar16;
  float *pfVar17;
  code *pcVar18;
  int iVar19;
  uint uVar20;
  int iVar21;
  uint uVar22;
  short *psVar23;
  uint uVar24;
  float *pfVar25;
  uint uVar26;
  uint uVar27;
  int *piVar28;
  int iVar29;
  uint in_fpscr;
  float fVar30;
  float fVar31;
  float fVar32;
  float fVar33;
  double dVar34;
  undefined4 uVar38;
  undefined4 uVar40;
  undefined1 in_q4 [16];
  undefined1 auVar39 [12];
  undefined1 auVar35 [16];
  undefined1 auVar36 [16];
  undefined1 auVar37 [16];
  undefined4 uVar41;
  float fVar42;
  float fVar43;
  double dVar44;
  double dVar45;
  double dVar46;
  Vector aVStack_bc [12];
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  float local_9c;
  undefined8 local_98;
  float local_90;
  Mesh *local_88;
  float local_84;
  float local_80;
  float local_7c;
  float local_78;
  float local_74;
  float local_70;
  float local_6c;
  float local_68;
  int local_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  
  uStack_60 = in_q4._0_8_;
  uStack_58 = in_q4._8_8_;
  uVar26 = param_3 & 0x1a;
  piVar28 = *(int **)(DAT_0007c9e8 + 0x7c624);
  local_64 = *piVar28;
  if ((uVar26 == 0) || (iVar5 = AEFile::Read(0xc,*param_4 + 0x50,*param_2), iVar5 != 0)) {
    pMVar6 = *param_4;
    if ((int)((uint)(byte)*pMVar6 << 0x1b) < 0) {
      iVar5 = AEFile::Read(2,pMVar6 + 0x28,*param_2);
      if (iVar5 != 0) {
        pMVar6 = *param_4;
        pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 0x28) << 1);
        *(void **)(pMVar6 + 0x2c) = pvVar7;
        iVar5 = AEFile::Read((uint)*(ushort *)(*param_4 + 0x28) << 1,*(void **)(*param_4 + 0x2c),
                             *param_2);
        if (iVar5 != 0) {
          pMVar6 = *param_4;
          goto LAB_0007c68c;
        }
      }
    }
    else {
LAB_0007c68c:
      iVar5 = AEFile::Read(2,pMVar6 + 2,*param_2);
      auVar39 = in_q4._4_12_;
      if (iVar5 != 0) {
        local_6c = DAT_0007c9ec;
        local_68 = DAT_0007c9ec;
        local_74 = DAT_0007c9f0;
        local_70 = DAT_0007c9ec;
        iVar5 = param_3 << 0x1d;
        local_7c = DAT_0007c9f0;
        local_78 = DAT_0007c9f0;
        if (iVar5 < 0) {
          pMVar6 = *param_4;
          pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 6);
          iVar8 = AEFile::Read((uint)*(ushort *)(pMVar6 + 2) * 6,pvVar7,*param_2);
          if (iVar8 != 0) {
            pMVar6 = *param_4;
            pvVar9 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
            *(void **)(pMVar6 + 4) = pvVar9;
            pfVar16 = &local_7c;
            pMVar6 = *param_4;
            pfVar25 = &local_70;
            iVar8 = 0;
            iVar19 = 0;
            uVar3 = *(ushort *)(pMVar6 + 2);
            while (iVar21 = __aeabi_uidiv(iVar19,3), (uint)uVar3 * 3 != iVar19) {
              iVar10 = iVar19 * 2;
              iVar19 = iVar19 + 1;
              pfVar12 = pfVar25 + iVar21 * -3;
              pfVar17 = pfVar16 + iVar21 * -3;
              pfVar25 = pfVar25 + 1;
              pfVar16 = pfVar16 + 1;
              fVar31 = (float)VectorSignedToFloat((int)*(short *)((int)pvVar7 + iVar10),
                                                  (byte)(in_fpscr >> 0x16) & 3);
              pfVar14 = (float *)(*(int *)(pMVar6 + 4) + iVar8);
              iVar8 = iVar8 + 4;
              *pfVar14 = fVar31;
              in_fpscr = in_fpscr & 0xfffffff;
              if (fVar31 < *pfVar12) {
                *pfVar12 = fVar31;
              }
              if (*pfVar17 < fVar31) {
                *pfVar17 = fVar31;
              }
            }
LAB_0007c834:
            operator_delete__(pvVar7);
            auVar39 = in_q4._4_12_;
            goto LAB_0007c844;
          }
        }
        else {
          if ((param_3 & 3) == 0) {
            if ((param_3 & 0x18) != 0) {
              pMVar6 = *param_4;
              pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
              *(void **)(pMVar6 + 4) = pvVar7;
              iVar8 = AEFile::Read((uint)*(ushort *)(*param_4 + 2) * 0xc,*(void **)(*param_4 + 4),
                                   *param_2);
              auVar39 = in_q4._4_12_;
              if (iVar8 == 0) goto LAB_0007cbdc;
            }
LAB_0007c844:
            fVar42 = local_68;
            fVar33 = local_74;
            fVar32 = local_78;
            fVar31 = local_7c;
            auVar35._4_12_ = auVar39;
            auVar35._0_4_ = local_70;
            auVar36._12_4_ = auVar39._8_4_;
            auVar36._0_8_ = auVar35._0_8_;
            auVar36._8_4_ = local_6c;
            local_88 = (Mesh *)((local_7c + local_70) * 0.5);
            local_84 = (local_78 + local_6c) * 0.5;
            local_80 = (local_74 + local_68) * 0.5;
            AEMath::Vector::operator=((Vector *)(*param_4 + 0x3c),(Vector *)&local_88);
            local_98 = CONCAT44(fVar32,fVar31);
            local_a0 = auVar36._8_4_;
            local_a4 = auVar36._0_4_;
            local_90 = fVar33;
            local_9c = fVar42;
            AEMath::operator-((Vector *)&local_88,(Vector *)&local_98);
            uVar11 = AEMath::VectorLength((Vector *)&local_88);
            pMVar6 = *param_4;
            *(undefined4 *)(pMVar6 + 0x48) = uVar11;
            if ((int)((uint)(byte)*pMVar6 << 0x1e) < 0) {
              if (iVar5 == 0) {
                if ((param_3 & 0x18) != 0) {
                  pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 3);
                  *(void **)(pMVar6 + 8) = pvVar7;
                  iVar8 = AEFile::Read((uint)*(ushort *)(*param_4 + 2) << 3,*(void **)(*param_4 + 8)
                                       ,*param_2);
                  if (iVar8 == 0) goto LAB_0007cbdc;
                  if (**(char **)(DAT_0007ca00 + 0x7c9d2) != '\0') {
                    pMVar6 = *param_4;
                    iVar8 = 4;
                    uVar3 = *(ushort *)(pMVar6 + 2);
                    for (uVar22 = 0; uVar22 < (uint)uVar3 << 1; uVar22 = uVar22 + 2) {
                      pfVar25 = (float *)(*(int *)(pMVar6 + 8) + iVar8);
                      iVar8 = iVar8 + 8;
                      *pfVar25 = 1.0 - *pfVar25;
                    }
                  }
                }
              }
              else {
                pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 2);
                iVar8 = AEFile::Read((uint)*(ushort *)(pMVar6 + 2) << 2,pvVar7,*param_2);
                if (iVar8 == 0) goto LAB_0007cbd8;
                pMVar6 = *param_4;
                pvVar9 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 3);
                iVar8 = DAT_0007c9f4;
                *(void **)(pMVar6 + 8) = pvVar9;
                dVar34 = DAT_0007c9f8;
                iVar19 = 0;
                pMVar6 = *param_4;
                uVar22 = 0;
                uVar3 = *(ushort *)(pMVar6 + 2);
                cVar2 = **(char **)(iVar8 + 0x7c904);
                while (uVar22 < (uint)uVar3 << 1) {
                  iVar8 = uVar22 * 2;
                  iVar21 = uVar22 * 2;
                  uVar22 = uVar22 + 2;
                  dVar44 = (double)VectorSignedToFloat((int)*(short *)((int)pvVar7 + iVar8),
                                                       (byte)(in_fpscr >> 0x16) & 3);
                  pfVar25 = (float *)(*(int *)(pMVar6 + 8) + iVar19);
                  iVar19 = iVar19 + 8;
                  *pfVar25 = (float)(dVar44 * dVar34);
                  dVar45 = (double)VectorSignedToFloat((int)*(short *)((int)pvVar7 + iVar21 + 2),
                                                       (byte)(in_fpscr >> 0x16) & 3);
                  dVar45 = dVar45 * dVar34;
                  dVar44 = 1.0 - dVar45;
                  if (cVar2 == '\0') {
                    dVar44 = dVar45;
                  }
                  pfVar25[1] = (float)dVar44;
                }
                operator_delete__(pvVar7);
              }
            }
            pMVar6 = *param_4;
            if ((int)((uint)(byte)*pMVar6 << 0x1d) < 0) {
              if (iVar5 == 0) {
                if ((param_3 & 0x18) != 0) {
                  pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
                  *(void **)(pMVar6 + 0x10) = pvVar7;
                  iVar8 = AEFile::Read((uint)*(ushort *)(*param_4 + 2) * 0xc,
                                       *(void **)(*param_4 + 0x10),*param_2);
                  if (iVar8 == 0) {
                    uVar11 = 0xffffffff;
                    goto LAB_0007cbe0;
                  }
                }
              }
              else {
                pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 6);
                iVar8 = AEFile::Read((uint)*(ushort *)(pMVar6 + 2) * 6,pvVar7,*param_2);
                if (iVar8 == 0) goto LAB_0007cbd8;
                pMVar6 = *param_4;
                pvVar9 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
                *(void **)(pMVar6 + 0x10) = pvVar9;
                pMVar6 = *param_4;
                psVar23 = (short *)((int)pvVar7 + 4);
                iVar8 = 4;
                auVar36._8_4_ = DAT_0007ca10;
                auVar36._0_8_ = DAT_0007ca08;
                for (uVar22 = 0; uVar22 < (uint)*(ushort *)(pMVar6 + 2) * 3; uVar22 = uVar22 + 3) {
                  dVar44 = (double)VectorSignedToFloat((int)*(short *)((int)pvVar7 + uVar22 * 2 + 2)
                                                       ,(byte)(in_fpscr >> 0x16) & 3);
                  dVar34 = auVar36._0_8_;
                  dVar45 = (double)VectorSignedToFloat((int)psVar23[-2],(byte)(in_fpscr >> 0x16) & 3
                                                      );
                  dVar46 = (double)VectorSignedToFloat((int)*psVar23,(byte)(in_fpscr >> 0x16) & 3);
                  fVar33 = (float)(dVar44 * dVar34);
                  fVar42 = (float)(dVar45 * dVar34);
                  fVar43 = (float)(dVar46 * dVar34);
                  fVar32 = fVar33 * fVar33 + fVar42 * fVar42 + fVar43 * fVar43;
                  fVar31 = SQRT(fVar32);
                  uVar20 = in_fpscr & 0xfffffff;
                  if (NAN(fVar31)) {
                    fVar31 = (float)sqrtf(fVar32);
                  }
                  in_fpscr = uVar20 & 0xfffffff | (uint)(fVar31 == 0.0) << 0x1e;
                  if ((byte)(in_fpscr >> 0x1e) == 0) {
                    fVar33 = fVar33 / fVar31;
                    fVar42 = fVar42 / fVar31;
                    fVar43 = fVar43 / fVar31;
                    fVar31 = fVar33;
                    if (fVar33 < -1.0) {
                      fVar31 = -1.0;
                    }
                    fVar32 = fVar42;
                    if (fVar42 < -1.0) {
                      fVar32 = -1.0;
                    }
                    fVar30 = fVar43;
                    if (fVar43 < -1.0) {
                      fVar30 = -1.0;
                    }
                    pMVar6 = *param_4;
                    if (1.0 < fVar33) {
                      fVar31 = 1.0;
                    }
                    uVar20 = uVar20 & 0xfffffff | (uint)(fVar43 < 1.0) << 0x1f |
                             (uint)(fVar43 == 1.0) << 0x1e;
                    in_fpscr = uVar20 | (uint)NAN(fVar43) << 0x1c;
                    if (1.0 < fVar42) {
                      fVar32 = 1.0;
                    }
                    bVar4 = (byte)(uVar20 >> 0x18);
                    iVar19 = *(int *)(pMVar6 + 0x10);
                    if (!(bool)(bVar4 >> 6 & 1) && bVar4 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                      fVar30 = 1.0;
                    }
                    ((float *)(iVar19 + iVar8))[-1] = fVar32;
                    *(float *)(iVar19 + iVar8) = fVar31;
                  }
                  else {
                    fVar30 = auVar36._8_4_;
                    pMVar6 = *param_4;
                    iVar19 = *(int *)(pMVar6 + 0x10);
                    *(undefined4 *)(iVar19 + iVar8) = 0x3f800000;
                    *(undefined4 *)(iVar19 + iVar8 + -4) = 0;
                  }
                  iVar19 = iVar19 + iVar8;
                  psVar23 = psVar23 + 3;
                  iVar8 = iVar8 + 0xc;
                  *(float *)(iVar19 + 4) = fVar30;
                }
                operator_delete__(pvVar7);
              }
              if (**(char **)(DAT_0007cf88 + 0x7cc36) != '\0') {
                pMVar6 = *param_4;
                pcVar18 = *(code **)(DAT_0007cf8c + 0x7cc4a);
                uVar11 = (*pcVar18)((uint)*(ushort *)(pMVar6 + 2) * 0xc);
                *(undefined4 *)(pMVar6 + 0x14) = uVar11;
                pMVar6 = *param_4;
                uVar11 = (*pcVar18)((uint)*(ushort *)(pMVar6 + 2) * 0xc);
                *(undefined4 *)(pMVar6 + 0x18) = uVar11;
                pMVar6 = *param_4;
                uVar22 = __aeabi_uidiv(*(undefined2 *)(pMVar6 + 0x28),3);
                uVar20 = (uint)*(ushort *)(pMVar6 + 2);
                iVar8 = (*pcVar18)(uVar20 * 0xc);
                auVar39 = auVar36._4_12_;
                if (uVar20 != 0) {
                  iVar19 = uVar20 * 0xc + -0xc;
                  __aeabi_uidivmod(iVar19,0xc);
                  __aeabi_memclr4(iVar8,(iVar19 - extraout_r1) + 0xc);
                  auVar39 = auVar36._4_12_;
                }
                auVar37._4_12_ = auVar39;
                auVar37._0_4_ = 0x3f800000;
                iVar19 = 0;
                pcVar18 = *(code **)(DAT_0007d04c + 0x7ccac);
                for (uVar27 = 0; uVar27 < uVar22; uVar27 = uVar27 + 1) {
                  pMVar6 = *param_4;
                  iVar29 = *(int *)(pMVar6 + 4);
                  iVar21 = *(int *)(pMVar6 + 8);
                  iVar10 = *(int *)(pMVar6 + 0x2c) + iVar19;
                  uVar15 = (uint)*(ushort *)(*(int *)(pMVar6 + 0x2c) + iVar19);
                  uVar13 = (uint)*(ushort *)(iVar10 + 4);
                  uVar24 = (uint)*(ushort *)(iVar10 + 2);
                  fVar31 = *(float *)((uVar15 << 3 | 4) + iVar21);
                  fVar33 = *(float *)(iVar21 + uVar15 * 8);
                  fVar32 = *(float *)((uVar13 << 3 | 4) + iVar21) - fVar31;
                  fVar31 = *(float *)((uVar24 << 3 | 4) + iVar21) - fVar31;
                  pfVar16 = (float *)(iVar29 + uVar15 * 0xc);
                  pfVar25 = (float *)(iVar29 + uVar13 * 0xc);
                  pfVar17 = (float *)(iVar29 + uVar24 * 0xc);
                  local_80 = auVar37._0_4_ /
                             ((*(float *)(iVar21 + uVar24 * 8) - fVar33) * fVar32 -
                             (*(float *)(iVar21 + uVar13 * 8) - fVar33) * fVar31);
                  local_84 = ((pfVar17[1] - pfVar16[1]) * fVar32 -
                             (pfVar25[1] - pfVar16[1]) * fVar31) * local_80;
                  local_88 = (Mesh *)(((*pfVar17 - *pfVar16) * fVar32 -
                                      (*pfVar25 - *pfVar16) * fVar31) * local_80);
                  local_80 = ((pfVar17[2] - pfVar16[2]) * fVar32 -
                             (pfVar25[2] - pfVar16[2]) * fVar31) * local_80;
                  (*pcVar18)(iVar8 + uVar15 * 0xc,&local_88);
                  (*pcVar18)(iVar8 + uVar24 * 0xc,&local_88);
                  (*pcVar18)(iVar8 + uVar13 * 0xc,&local_88);
                  iVar19 = iVar19 + 6;
                }
                iVar19 = 0;
                for (; uVar20 != 0; uVar20 = uVar20 - 1) {
                  local_88 = *(Mesh **)(*(int *)(*param_4 + 0x10) + iVar19);
                  iVar21 = *(int *)(*param_4 + 0x10) + iVar19;
                  local_84 = *(float *)(iVar21 + 4);
                  local_80 = *(float *)(iVar21 + 8);
                  local_98 = *(undefined8 *)(iVar8 + iVar19);
                  local_90 = *(float *)((undefined8 *)(iVar8 + iVar19) + 1);
                  fVar31 = (float)AEMath::VectorDot((Vector *)&local_88,(Vector *)&local_98);
                  AEMath::operator*(aVStack_bc,fVar31);
                  AEMath::operator-((Vector *)&local_b0,(Vector *)&local_98);
                  AEMath::VectorNormalize((AEMath *)&local_a4,(Vector *)&local_b0);
                  iVar21 = *(int *)(*param_4 + 0x14);
                  *(undefined4 *)(iVar21 + iVar19) = local_a4;
                  iVar21 = iVar21 + iVar19;
                  *(undefined4 *)(iVar21 + 4) = local_a0;
                  *(float *)(iVar21 + 8) = local_9c;
                  AEMath::VectorCross((Vector *)&local_b0,(Vector *)&local_88);
                  iVar21 = *(int *)(*param_4 + 0x18);
                  *(undefined4 *)(iVar21 + iVar19) = local_b0;
                  iVar21 = iVar21 + iVar19;
                  iVar19 = iVar19 + 0xc;
                  *(undefined4 *)(iVar21 + 4) = local_ac;
                  *(undefined4 *)(iVar21 + 8) = local_a8;
                }
              }
            }
            pMVar6 = *param_4;
            if ((int)((uint)(byte)*pMVar6 << 0x1c) < 0) {
              if (iVar5 != 0) {
                pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 2);
                iVar5 = AEFile::Read((uint)*(ushort *)(pMVar6 + 2) << 2,pvVar7,*param_2);
                if (iVar5 == 0) goto LAB_0007cbd8;
                pMVar6 = *param_4;
                pvVar9 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 4);
                *(void **)(pMVar6 + 0xc) = pvVar9;
                fVar31 = DAT_0007d048;
                iVar5 = 0;
                pMVar6 = *param_4;
                uVar22 = 0;
                uVar3 = *(ushort *)(pMVar6 + 2);
                while (uVar22 < (uint)uVar3 << 2) {
                  pbVar1 = (byte *)((int)pvVar7 + uVar22);
                  uVar22 = uVar22 + 1;
                  fVar32 = (float)VectorUnsignedToFloat((uint)*pbVar1,(byte)(in_fpscr >> 0x16) & 3);
                  pfVar25 = (float *)(*(int *)(pMVar6 + 0xc) + iVar5);
                  iVar5 = iVar5 + 4;
                  *pfVar25 = fVar32 / fVar31;
                }
                operator_delete__(pvVar7);
                goto LAB_0007cf28;
              }
              if ((param_3 & 0x18) == 0) goto LAB_0007cf28;
              pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) << 4);
              *(void **)(pMVar6 + 0xc) = pvVar7;
              iVar5 = AEFile::Read((uint)*(ushort *)(*param_4 + 2) << 4,*(void **)(*param_4 + 0xc),
                                   *param_2);
              uVar11 = 0xffffffff;
              if (iVar5 != 0) {
                uVar11 = 1;
              }
              if ((uVar26 == 0) || (iVar5 == 0)) goto LAB_0007cbe0;
LAB_0007cf34:
              iVar5 = Mesh::ReadEnhancedDataFromFile(*param_4,*param_2,param_3);
              if ((iVar5 == 0) || (iVar5 = AEFile::Read(2,&local_98,*param_2), iVar5 == 0))
              goto LAB_0007cbdc;
              iVar5 = *(int *)(*param_4 + 0x34);
              if (iVar5 != 0) {
                AEMath::BSphere::Merge((BSphere *)(*param_4 + 0x3c),(BSphere *)(iVar5 + 0xd4));
              }
              uVar26 = (uint)(ushort)local_98;
              if (uVar26 != 0) {
                uVar11 = 0;
                uVar38 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
                uVar40 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
                uVar41 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
                for (uVar22 = 0; uVar22 < uVar26; uVar22 = uVar22 + 1) {
                  local_88 = operator_new(0x88);
                  *(undefined4 *)(local_88 + 0x82) = 0;
                  *(undefined4 *)(local_88 + 0x7e) = 0;
                  local_88[0x38] = (Mesh)0x0;
                  *(undefined2 *)(local_88 + 0x22) = 0;
                  *local_88 = (Mesh)0x0;
                  *(undefined4 *)(local_88 + 0x4c) = 0x3f800000;
                  *(undefined4 *)(local_88 + 0x50) = 0;
                  *(undefined4 *)(local_88 + 0x54) = 0;
                  *(undefined4 *)(local_88 + 0x59) = 0;
                  *(undefined4 *)(local_88 + 0x55) = 0;
                  *(undefined4 *)(local_88 + 0x70) = uVar11;
                  *(undefined4 *)(local_88 + 0x74) = uVar38;
                  *(undefined4 *)(local_88 + 0x78) = uVar40;
                  *(undefined4 *)(local_88 + 0x7c) = uVar41;
                  *(undefined4 *)(local_88 + 0x60) = uVar11;
                  *(undefined4 *)(local_88 + 100) = uVar38;
                  *(undefined4 *)(local_88 + 0x68) = uVar40;
                  *(undefined4 *)(local_88 + 0x6c) = uVar41;
                  *(undefined4 *)(local_88 + 0x28) = uVar11;
                  *(undefined4 *)(local_88 + 0x2c) = uVar38;
                  *(undefined4 *)(local_88 + 0x30) = uVar40;
                  *(undefined4 *)(local_88 + 0x34) = uVar41;
                  *(undefined4 *)(local_88 + 0x12) = uVar11;
                  *(undefined4 *)(local_88 + 0x16) = uVar38;
                  *(undefined4 *)(local_88 + 0x1a) = uVar40;
                  *(undefined4 *)(local_88 + 0x1e) = uVar41;
                  *(undefined4 *)(local_88 + 2) = uVar11;
                  *(undefined4 *)(local_88 + 6) = uVar38;
                  *(undefined4 *)(local_88 + 10) = uVar40;
                  *(undefined4 *)(local_88 + 0xe) = uVar41;
                  *(undefined4 *)(local_88 + 0x3c) = uVar11;
                  *(undefined4 *)(local_88 + 0x40) = uVar38;
                  *(undefined4 *)(local_88 + 0x44) = uVar40;
                  *(undefined4 *)(local_88 + 0x48) = uVar41;
                  local_88[0x84] = (Mesh)0x1;
                  pMVar6 = *param_4;
                  *local_88 = *pMVar6;
                  *(undefined4 *)(local_88 + 0x30) = *(undefined4 *)(pMVar6 + 0x30);
                  iVar5 = MeshReadData(param_1,param_2,param_3,&local_88,param_5);
                  if (iVar5 == -1) goto LAB_0007cbdc;
                  AEMath::BSphere::Merge((BSphere *)(*param_4 + 0x3c),(BSphere *)(local_88 + 0x3c));
                  ArrayAdd<AbyssEngine::Mesh*>(local_88,(Array *)(*(int *)(*param_4 + 0x34) + 0x3c))
                  ;
                  uVar26 = (uint)(ushort)local_98;
                }
              }
            }
            else {
LAB_0007cf28:
              if (uVar26 != 0) goto LAB_0007cf34;
            }
            uVar11 = 1;
            goto LAB_0007cbe0;
          }
          pMVar6 = *param_4;
          pvVar7 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
          iVar8 = AEFile::Read((uint)*(ushort *)(pMVar6 + 2) * 0xc,pvVar7,*param_2);
          if (iVar8 != 0) {
            pMVar6 = *param_4;
            pvVar9 = operator_new__((uint)*(ushort *)(pMVar6 + 2) * 0xc);
            *(void **)(pMVar6 + 4) = pvVar9;
            pfVar16 = &local_7c;
            pMVar6 = *param_4;
            pfVar25 = &local_70;
            iVar19 = 0;
            iVar21 = 0;
            iVar8 = (uint)*(ushort *)(pMVar6 + 2) * 3;
            while (iVar10 = __aeabi_uidiv(iVar21,3), iVar8 != 0) {
              iVar8 = iVar8 + -1;
              iVar21 = iVar21 + 1;
              pfVar12 = pfVar25 + iVar10 * -3;
              pfVar17 = pfVar16 + iVar10 * -3;
              pfVar25 = pfVar25 + 1;
              fVar31 = (float)VectorSignedToFloat(*(undefined4 *)((int)pvVar7 + iVar19),
                                                  (byte)(in_fpscr >> 0x16) & 3);
              pfVar16 = pfVar16 + 1;
              pfVar14 = (float *)(*(int *)(pMVar6 + 4) + iVar19);
              iVar19 = iVar19 + 4;
              *pfVar14 = fVar31;
              in_fpscr = in_fpscr & 0xfffffff;
              if (fVar31 < *pfVar12) {
                *pfVar12 = fVar31;
              }
              if (*pfVar17 < fVar31) {
                *pfVar17 = fVar31;
              }
            }
            goto LAB_0007c834;
          }
        }
LAB_0007cbd8:
        operator_delete__(pvVar7);
      }
    }
  }
LAB_0007cbdc:
  uVar11 = 0xffffffff;
LAB_0007cbe0:
  if (*piVar28 == local_64) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar11);
}

```

## target disasm
```
  0007c608: push {r4,r5,r6,r7,lr}
  0007c60a: add r7,sp,#0xc
  0007c60c: push {r7,r8,r9,r10,r11}
  0007c610: vpush {d8,d9,d10,d11,d12,d13,d14}
  0007c614: sub sp,#0x88
  0007c616: mov r5,r0
  0007c618: ldr r0,[0x0007c9e8]
  0007c61a: ands r8,r2,#0x1a
  0007c61e: mov r9,r3
  0007c620: add r0,pc
  0007c622: mov r10,r2
  0007c624: mov r6,r1
  0007c626: ldr.w r11,[r0,#0x0]
  0007c62a: ldr.w r0,[r11,#0x0]
  0007c62e: str r0,[sp,#0x84]
  0007c630: beq 0x0007c648
  0007c632: ldr.w r0,[r9,#0x0]
  0007c636: ldr r2,[r6,#0x0]
  0007c638: add.w r1,r0,#0x50
  0007c63c: movs r0,#0xc
  0007c63e: blx 0x0006eb90
  0007c642: cmp r0,#0x0
  0007c644: beq.w 0x0007cbdc
  0007c648: ldr.w r0,[r9,#0x0]
  0007c64c: ldrb r1,[r0,#0x0]
  0007c64e: lsls r1,r1,#0x1b
  0007c650: bpl 0x0007c68c
  0007c652: ldr r2,[r6,#0x0]
  0007c654: add.w r1,r0,#0x28
  0007c658: movs r0,#0x2
  0007c65a: blx 0x0006eb90
  0007c65e: cmp r0,#0x0
  0007c660: beq.w 0x0007cbdc
  0007c664: ldr.w r4,[r9,#0x0]
  0007c668: ldrh r0,[r4,#0x28]
  0007c66a: lsls r0,r0,#0x1
  0007c66c: blx 0x0006ec08
  0007c670: str r0,[r4,#0x2c]
  0007c672: ldr.w r0,[r9,#0x0]
  0007c676: ldr r2,[r6,#0x0]
  0007c678: ldrh r3,[r0,#0x28]
  0007c67a: ldr r1,[r0,#0x2c]
  0007c67c: lsls r0,r3,#0x1
  0007c67e: blx 0x0006eb90
  0007c682: cmp r0,#0x0
  0007c684: beq.w 0x0007cbdc
  0007c688: ldr.w r0,[r9,#0x0]
  0007c68c: ldr r2,[r6,#0x0]
  0007c68e: adds r1,r0,#0x2
  0007c690: movs r0,#0x2
  0007c692: blx 0x0006eb90
  0007c696: cmp r0,#0x0
  0007c698: beq.w 0x0007cbdc
  0007c69c: ldr r0,[0x0007c9ec]
  0007c69e: ldr r1,[0x0007c9f0]
  0007c6a0: strd r0,r0,[sp,#0x7c]
  0007c6a4: strd r1,r0,[sp,#0x74]
  0007c6a8: lsls.w r0,r10,#0x1d
  0007c6ac: strd r1,r1,[sp,#0x6c]
  0007c6b0: str.w r9,[sp,#0x28]
  0007c6b4: str r6,[sp,#0x18]
  0007c6b6: strd r8,r0,[sp,#0x8]
  0007c6ba: bmi 0x0007c780
  0007c6bc: lsls.w r0,r10,#0x1e
  0007c6c0: beq.w 0x0007c96e
  0007c6c4: ldr.w r4,[r9,#0x0]
  0007c6c8: ldrh r0,[r4,#0x2]
  0007c6ca: add.w r0,r0,r0, lsl #0x1
  0007c6ce: lsls r0,r0,#0x2
  0007c6d0: blx 0x0006ec08
  0007c6d4: mov r8,r0
  0007c6d6: ldrh r0,[r4,#0x2]
  0007c6d8: ldr r2,[r6,#0x0]
  0007c6da: mov r1,r8
  0007c6dc: add.w r0,r0,r0, lsl #0x1
  0007c6e0: lsls r0,r0,#0x2
  0007c6e2: blx 0x0006eb90
  0007c6e6: cmp r0,#0x0
  0007c6e8: beq.w 0x0007cbd6
  0007c6ec: str r5,[sp,#0x4]
  0007c6ee: strd r11,r10,[sp,#0x10]
  0007c6f2: ldr.w r4,[r9,#0x0]
  0007c6f6: ldrh r0,[r4,#0x2]
  0007c6f8: add.w r0,r0,r0, lsl #0x1
  0007c6fc: lsls r0,r0,#0x2
  0007c6fe: blx 0x0006ec08
  0007c702: str r0,[r4,#0x4]
  0007c704: add.w r10,sp,#0x6c
  0007c708: ldr.w r9,[r9,#0x0]
  0007c70c: add r4,sp,#0x78
  0007c70e: movs r5,#0x0
  0007c710: movs r6,#0x0
  0007c712: ldrh.w r0,[r9,#0x2]
  0007c716: add.w r11,r0,r0, lsl #0x1
  0007c71a: b 0x0007c770
  0007c71c: add.w r2,r8,r5
  0007c720: sub.w r0,r0,r0, lsl #0x2
  0007c724: sub.w r11,r11,#0x1
  0007c728: adds r6,#0x1
  0007c72a: vldr.32 s0,[r2]
  0007c72e: add.w r1,r4,r0, lsl #0x2
  0007c732: add.w r0,r10,r0, lsl #0x2
  0007c736: adds r4,#0x4
  0007c738: vcvt.f32.s32 s0,s0
  0007c73c: ldr.w r2,[r9,#0x4]
  0007c740: add.w r10,r10,#0x4
  0007c744: add r2,r5
  0007c746: adds r5,#0x4
  0007c748: vstr.32 s0,[r2]
  0007c74c: vldr.32 s2,[r1]
  0007c750: vcmpe.f32 s2,s0
  0007c754: vmrs apsr,fpscr
  0007c758: it gt
  0007c75a: vstr.gt.32 s0,[r1]
  0007c75e: vldr.32 s2,[r0]
  0007c762: vcmpe.f32 s2,s0
  0007c766: vmrs apsr,fpscr
  0007c76a: it mi
  0007c76c: vstr.mi.32 s0,[r0]
  0007c770: mov r0,r6
  0007c772: movs r1,#0x3
  0007c774: blx 0x0006ec2c
  0007c778: cmp.w r11,#0x0
  0007c77c: bne 0x0007c71c
  0007c77e: b 0x0007c834
  0007c780: ldr.w r4,[r9,#0x0]
  0007c784: ldrh r0,[r4,#0x2]
  0007c786: add.w r0,r0,r0, lsl #0x1
  0007c78a: lsls r0,r0,#0x1
  0007c78c: blx 0x0006ec08
  0007c790: mov r8,r0
  0007c792: ldrh r0,[r4,#0x2]
  0007c794: ldr r2,[r6,#0x0]
  0007c796: mov r1,r8
  0007c798: add.w r0,r0,r0, lsl #0x1
  0007c79c: lsls r0,r0,#0x1
  0007c79e: blx 0x0006eb90
  0007c7a2: cmp r0,#0x0
  0007c7a4: beq.w 0x0007cbd6
  0007c7a8: str r5,[sp,#0x4]
  0007c7aa: strd r11,r10,[sp,#0x10]
  0007c7ae: ldr.w r4,[r9,#0x0]
  0007c7b2: ldrh r0,[r4,#0x2]
  0007c7b4: add.w r0,r0,r0, lsl #0x1
  0007c7b8: lsls r0,r0,#0x2
  0007c7ba: blx 0x0006ec08
  0007c7be: str r0,[r4,#0x4]
  0007c7c0: add.w r11,sp,#0x6c
  0007c7c4: ldr.w r9,[r9,#0x0]
  0007c7c8: add r5,sp,#0x78
  0007c7ca: movs r4,#0x0
  0007c7cc: movs r6,#0x0
  0007c7ce: ldrh.w r0,[r9,#0x2]
  0007c7d2: add.w r10,r0,r0, lsl #0x1
  0007c7d6: b 0x0007c828
  0007c7d8: ldrsh.w r2,[r8,r6,lsl #0x1]
  0007c7dc: sub.w r0,r0,r0, lsl #0x2
  0007c7e0: adds r6,#0x1
  0007c7e2: add.w r1,r5,r0, lsl #0x2
  0007c7e6: add.w r0,r11,r0, lsl #0x2
  0007c7ea: adds r5,#0x4
  0007c7ec: add.w r11,r11,#0x4
  0007c7f0: vmov s0,r2
  0007c7f4: vcvt.f32.s32 s0,s0
  0007c7f8: ldr.w r2,[r9,#0x4]
  0007c7fc: add r2,r4
  0007c7fe: adds r4,#0x4
  0007c800: vstr.32 s0,[r2]
  0007c804: vldr.32 s2,[r1]
  0007c808: vcmpe.f32 s2,s0
  0007c80c: vmrs apsr,fpscr
  0007c810: it gt
  0007c812: vstr.gt.32 s0,[r1]
  0007c816: vldr.32 s2,[r0]
  0007c81a: vcmpe.f32 s2,s0
  0007c81e: vmrs apsr,fpscr
  0007c822: it mi
  0007c824: vstr.mi.32 s0,[r0]
  0007c828: mov r0,r6
  0007c82a: movs r1,#0x3
  0007c82c: blx 0x0006ec2c
  0007c830: cmp r10,r6
  0007c832: bne 0x0007c7d8
  0007c834: mov r0,r8
  0007c836: blx 0x0006ebfc
  0007c83a: ldr.w r9,[sp,#0x28]
  0007c83e: ldrd r11,r10,[sp,#0x10]
  0007c842: ldr r6,[sp,#0x18]
  0007c844: vldr.32 s16,[sp,#0x78]
  0007c848: vmov.f32 s0,0x3f000000
  0007c84c: vldr.32 s22,[sp,#0x6c]
  0007c850: add r1,sp,#0x60
  0007c852: vldr.32 s18,[sp,#0x7c]
  0007c856: vldr.32 s24,[sp,#0x70]
  0007c85a: vadd.f32 s2,s22,s16
  0007c85e: vldr.32 s20,[sp,#0x80]
  0007c862: vldr.32 s26,[sp,#0x74]
  0007c866: vadd.f32 s4,s24,s18
  0007c86a: vadd.f32 s6,s26,s20
  0007c86e: vmul.f32 s2,s2,s0
  0007c872: vmul.f32 s4,s4,s0
  0007c876: vmul.f32 s0,s6,s0
  0007c87a: vstr.32 s2,[sp,#0x60]
  0007c87e: vstr.32 s4,[sp,#0x64]
  0007c882: vstr.32 s0,[sp,#0x68]
  0007c886: ldr.w r0,[r9,#0x0]
  0007c88a: adds r0,#0x3c
  0007c88c: blx 0x0006eb3c
  0007c890: add r5,sp,#0x60
  0007c892: add r1,sp,#0x50
  0007c894: add r2,sp,#0x44
  0007c896: vstr.32 s24,[sp,#0x54]
  0007c89a: mov r0,r5
  0007c89c: vstr.32 s22,[sp,#0x50]
  0007c8a0: vstr.32 s18,[sp,#0x48]
  0007c8a4: vstr.32 s16,[sp,#0x44]
  0007c8a8: vstr.32 s26,[sp,#0x58]
  0007c8ac: vstr.32 s20,[sp,#0x4c]
  0007c8b0: blx 0x0006ec38
  0007c8b4: mov r0,r5
  0007c8b6: blx 0x0006ec44
  0007c8ba: ldr.w r4,[r9,#0x0]
  0007c8be: str r0,[r4,#0x48]
  0007c8c0: ldrb r0,[r4,#0x0]
  0007c8c2: lsls r0,r0,#0x1e
  0007c8c4: bpl.w 0x0007ca2c
  0007c8c8: ldr r0,[sp,#0xc]
  0007c8ca: cmp r0,#0x0
  0007c8cc: beq 0x0007c9a6
  0007c8ce: ldrh r0,[r4,#0x2]
  0007c8d0: lsls r0,r0,#0x2
  0007c8d2: blx 0x0006ec08
  0007c8d6: mov r8,r0
  0007c8d8: ldrh r0,[r4,#0x2]
  0007c8da: ldr r2,[r6,#0x0]
  0007c8dc: mov r1,r8
  0007c8de: lsls r0,r0,#0x2
  0007c8e0: blx 0x0006eb90
  0007c8e4: cmp r0,#0x0
  0007c8e6: beq.w 0x0007cbd6
  0007c8ea: ldr.w r4,[r9,#0x0]
  0007c8ee: ldrh r0,[r4,#0x2]
  0007c8f0: lsls r0,r0,#0x3
  0007c8f2: blx 0x0006ec08
  0007c8f6: ldr r1,[0x0007c9f4]
  0007c8f8: vmov.f64 d17,0x3ff0000000000000
  0007c8fc: str r0,[r4,#0x8]
  0007c8fe: movs r3,#0x0
  0007c900: add r1,pc
  0007c902: ldr.w r0,[r9,#0x0]
  0007c906: movs r6,#0x0
  0007c908: ldr r1,[r1,#0x0]
  0007c90a: ldrh r2,[r0,#0x2]
  0007c90c: vldr.64 d16,[pc,#0xe8]
  0007c910: ldrb r1,[r1,#0x0]
  0007c912: lsls r2,r2,#0x1
  0007c914: b 0x0007c962
  0007c916: ldrsh.w r5,[r8,r6,lsl #0x1]
  0007c91a: add.w r4,r8,r6, lsl #0x1
  0007c91e: cmp r1,#0x0
  0007c920: add.w r6,r6,#0x2
  0007c924: vmov s0,r5
  0007c928: vcvt.f64.s32 d18,s0
  0007c92c: ldr r5,[r0,#0x8]
  0007c92e: add r5,r3
  0007c930: add.w r3,r3,#0x8
  0007c934: vmul.f64 d18,d18,d16
  0007c938: vcvt.f32.f64 s0,d18
  0007c93c: vstr.32 s0,[r5]
  0007c940: ldrsh.w r4,[r4,#0x2]
  0007c944: vmov s0,r4
  0007c948: vcvt.f64.s32 d18,s0
  0007c94c: vmul.f64 d18,d18,d16
  0007c950: vsub.f64 d19,d17,d18
  0007c954: it eq
  0007c956: vmov.eq.f64 d19,d18
  0007c95a: vcvt.f32.f64 s0,d19
  0007c95e: vstr.32 s0,[r5,#0x4]
  0007c962: cmp r6,r2
  0007c964: bcc 0x0007c916
  0007c966: mov r0,r8
  0007c968: blx 0x0006ebfc
  0007c96c: b 0x0007ca2c
  0007c96e: tst r10,#0x18
  0007c972: str r5,[sp,#0x4]
  0007c974: beq.w 0x0007c844
  0007c978: ldr.w r4,[r9,#0x0]
  0007c97c: ldrh r0,[r4,#0x2]
  0007c97e: add.w r0,r0,r0, lsl #0x1
  0007c982: lsls r0,r0,#0x2
  0007c984: blx 0x0006ec08
  0007c988: str r0,[r4,#0x4]
  0007c98a: ldr.w r0,[r9,#0x0]
  0007c98e: ldr r2,[r6,#0x0]
  0007c990: ldrh r3,[r0,#0x2]
  0007c992: ldr r1,[r0,#0x4]
  0007c994: add.w r0,r3,r3, lsl #0x1
  0007c998: lsls r0,r0,#0x2
  0007c99a: blx 0x0006eb90
  0007c99e: cmp r0,#0x0
  0007c9a0: bne.w 0x0007c844
  0007c9a4: b 0x0007cbdc
  0007c9a6: tst r10,#0x18
  0007c9aa: beq 0x0007ca2c
  0007c9ac: ldrh r0,[r4,#0x2]
  0007c9ae: lsls r0,r0,#0x3
  0007c9b0: blx 0x0006ec08
  0007c9b4: str r0,[r4,#0x8]
  0007c9b6: ldr.w r0,[r9,#0x0]
  0007c9ba: ldr r2,[r6,#0x0]
  0007c9bc: ldrh r3,[r0,#0x2]
  0007c9be: ldr r1,[r0,#0x8]
  0007c9c0: lsls r0,r3,#0x3
  0007c9c2: blx 0x0006eb90
  0007c9c6: cmp r0,#0x0
  0007c9c8: beq.w 0x0007cbdc
  0007c9cc: ldr r0,[0x0007ca00]
  0007c9ce: add r0,pc
  0007c9d0: ldr r0,[r0,#0x0]
  0007c9d2: ldrb r0,[r0,#0x0]
  0007c9d4: cbz r0,0x0007ca2c
  0007c9d6: ldr.w r0,[r9,#0x0]
  0007c9da: vmov.f32 s0,0x3f800000
  0007c9de: movs r2,#0x0
  0007c9e0: movs r3,#0x4
  0007c9e2: ldrh r1,[r0,#0x2]
  0007c9e4: lsls r1,r1,#0x1
  0007c9e6: b 0x0007ca28
  0007ca14: ldr r6,[r0,#0x8]
  0007ca16: adds r2,#0x2
  0007ca18: add r6,r3
  0007ca1a: adds r3,#0x8
  0007ca1c: vldr.32 s2,[r6]
  0007ca20: vsub.f32 s2,s0,s2
  0007ca24: vstr.32 s2,[r6]
  0007ca28: cmp r2,r1
  0007ca2a: bcc 0x0007ca14
  0007ca2c: strd r11,r10,[sp,#0x10]
  0007ca30: ldr.w r4,[r9,#0x0]
  0007ca34: ldrb r0,[r4,#0x0]
  0007ca36: lsls r0,r0,#0x1d
  0007ca38: bpl.w 0x0007ce6c
  0007ca3c: ldr r0,[sp,#0xc]
  0007ca3e: cmp r0,#0x0
  0007ca40: beq.w 0x0007cbfe
  0007ca44: ldrh r0,[r4,#0x2]
  0007ca46: add.w r0,r0,r0, lsl #0x1
  0007ca4a: lsls r0,r0,#0x1
  0007ca4c: blx 0x0006ec08
  0007ca50: mov r8,r0
  0007ca52: ldrh r0,[r4,#0x2]
  0007ca54: ldr r1,[sp,#0x18]
  0007ca56: add.w r0,r0,r0, lsl #0x1
  0007ca5a: ldr r2,[r1,#0x0]
  0007ca5c: mov r1,r8
  0007ca5e: lsls r0,r0,#0x1
  0007ca60: blx 0x0006eb90
  0007ca64: ldr r5,[sp,#0x28]
  0007ca66: cmp r0,#0x0
  0007ca68: ldr.w r11,[sp,#0x10]
  0007ca6c: beq.w 0x0007cbd6
  0007ca70: ldr r4,[r5,#0x0]
  0007ca72: ldrh r0,[r4,#0x2]
  0007ca74: add.w r0,r0,r0, lsl #0x1
  0007ca78: lsls r0,r0,#0x2
  0007ca7a: blx 0x0006ec08
  0007ca7e: vmov.f32 s22,0x3f800000
  0007ca82: str r0,[r4,#0x10]
  0007ca84: vmov.f64 d10,-0x4010000000000000
  0007ca88: ldr r0,[r5,#0x0]
  0007ca8a: add.w r6,r8,#0x4
  0007ca8e: add.w r10,r8,#0x2
  0007ca92: movs r4,#0x4
  0007ca94: mov.w r11,#0x3f800000
  0007ca98: mov.w r9,#0x0
  0007ca9c: vldr.64 d8,[pc,#-0x98]
  0007caa0: vldr.32 s18,[pc,#-0x94]
  0007caa4: b 0x0007cbc2
  0007caa6: ldrsh.w r0,[r10,r9,lsl #0x1]
  0007caaa: vmov s0,r0
  0007caae: vcvt.f64.s32 d16,s0
  0007cab2: ldrsh.w r0,[r6,#-0x4]
  0007cab6: ldrsh.w r1,[r6,#0x0]
  0007caba: vmov s0,r0
  0007cabe: vmul.f64 d16,d16,d8
  0007cac2: vcvt.f64.s32 d17,s0
  0007cac6: vmov s0,r1
  0007caca: vmul.f64 d17,d17,d8
  0007cace: vcvt.f64.s32 d18,s0
  0007cad2: vcvt.f32.f64 s26,d16
  0007cad6: vmul.f64 d18,d18,d8
  0007cada: vcvt.f32.f64 s24,d17
  0007cade: vmul.f32 s2,s26,s26
  0007cae2: vcvt.f32.f64 s28,d18
  0007cae6: vmla.f32 s2,s24,s24
  0007caea: vmla.f32 s2,s28,s28
  0007caee: vsqrt.f32 s0,s2
  0007caf2: vcmpe.f32 s0,s0
  0007caf6: vmrs apsr,fpscr
  0007cafa: bvc 0x0007cb08
  0007cafc: vmov r0,s2
  0007cb00: blx 0x0006ec50
  0007cb04: vmov s0,r0
  0007cb08: vcmpe.f32 s0,#0
  0007cb0c: vmrs apsr,fpscr
  0007cb10: bne 0x0007cb2a
  0007cb12: ldr r0,[sp,#0x28]
  0007cb14: movs r3,#0x0
  0007cb16: vmov.f32 s0,s18
  0007cb1a: ldr r0,[r0,#0x0]
  0007cb1c: ldr r1,[r0,#0x10]
  0007cb1e: adds r2,r1,r4
  0007cb20: str.w r11,[r1,r4]
  0007cb24: str.w r3,[r2,#-0x4]
  0007cb28: b 0x0007cbb4
  0007cb2a: vdiv.f32 s2,s26,s0
  0007cb2e: vdiv.f32 s4,s24,s0
  0007cb32: vcvt.f64.f32 d16,s2
  0007cb36: vcmpe.f64 d16,d10
  0007cb3a: vmrs apsr,fpscr
  0007cb3e: vdiv.f32 s6,s28,s0
  0007cb42: it mi
  0007cb44: vmov.mi.f64 d16,d10
  0007cb48: vcvt.f64.f32 d17,s4
  0007cb4c: vcmpe.f64 d17,d10
  0007cb50: vmrs apsr,fpscr
  0007cb54: vcvt.f64.f32 d18,s6
  0007cb58: vcmpe.f64 d18,d10
  0007cb5c: vcvt.f32.f64 s8,d16
  0007cb60: it mi
  0007cb62: vmov.mi.f64 d17,d10
  0007cb66: vmrs apsr,fpscr
  0007cb6a: vcmpe.f32 s2,s22
  0007cb6e: vcvt.f32.f64 s10,d17
  0007cb72: it mi
  0007cb74: vmov.mi.f64 d18,d10
  0007cb78: vmrs apsr,fpscr
  0007cb7c: ldr r0,[sp,#0x28]
  0007cb7e: vcmpe.f32 s4,s22
  0007cb82: ldr r0,[r0,#0x0]
  0007cb84: vcvt.f32.f64 s0,d18
  0007cb88: it gt
  0007cb8a: vmov.gt.f32 s8,s22
  0007cb8e: vmrs apsr,fpscr
  0007cb92: vcmpe.f32 s6,s22
  0007cb96: it gt
  0007cb98: vmov.gt.f32 s10,s22
  0007cb9c: vmrs apsr,fpscr
  0007cba0: ldr r1,[r0,#0x10]
  0007cba2: add.w r2,r1,r4
  0007cba6: it gt
  0007cba8: vmov.gt.f32 s0,s22
  0007cbac: vstr.32 s10,[r2,#-0x4]
  0007cbb0: vstr.32 s8,[r2]
  0007cbb4: add r1,r4
  0007cbb6: adds r6,#0x6
  0007cbb8: adds r4,#0xc
  0007cbba: add.w r9,r9,#0x3
  0007cbbe: vstr.32 s0,[r1,#0x4]
  0007cbc2: ldrh r0,[r0,#0x2]
  0007cbc4: add.w r0,r0,r0, lsl #0x1
  0007cbc8: cmp r9,r0
  0007cbca: bcc.w 0x0007caa6
  0007cbce: mov r0,r8
  0007cbd0: blx 0x0006ebfc
  0007cbd4: b 0x0007cc30
  0007cbd6: mov r0,r8
  0007cbd8: blx 0x0006ebfc
  0007cbdc: mov.w r0,#0xffffffff
  0007cbe0: ldr r1,[sp,#0x84]
  0007cbe2: ldr.w r2,[r11,#0x0]
  0007cbe6: subs r1,r2,r1
  0007cbe8: itttt eq
  0007cbea: add.eq sp,#0x88
  0007cbec: vpop.eq {d8,d9,d10,d11,d12,d13,d14}
  0007cbf0: add.eq sp,#0x4
  0007cbf2: pop.eq.w {r8,r9,r10,r11}
  0007cbf6: it eq
  0007cbf8: pop.eq {r4,r5,r6,r7,pc}
  0007cbfa: blx 0x0006e824
  0007cbfe: ldr r0,[sp,#0x14]
  0007cc00: tst r0,#0x18
  0007cc04: beq 0x0007cc30
  0007cc06: ldrh r0,[r4,#0x2]
  0007cc08: add.w r0,r0,r0, lsl #0x1
  0007cc0c: lsls r0,r0,#0x2
  0007cc0e: blx 0x0006ec08
  0007cc12: str r0,[r4,#0x10]
  0007cc14: ldr r0,[sp,#0x28]
  0007cc16: ldr r1,[sp,#0x18]
  0007cc18: ldr r0,[r0,#0x0]
  0007cc1a: ldr r2,[r1,#0x0]
  0007cc1c: ldrh r3,[r0,#0x2]
  0007cc1e: ldr r1,[r0,#0x10]
  0007cc20: add.w r0,r3,r3, lsl #0x1
  0007cc24: lsls r0,r0,#0x2
  0007cc26: blx 0x0006eb90
  0007cc2a: cmp r0,#0x0
  0007cc2c: beq.w 0x0007d03e
  0007cc30: ldr r0,[0x0007cf88]
  0007cc32: add r0,pc
  0007cc34: ldr r0,[r0,#0x0]
  0007cc36: ldrb r0,[r0,#0x0]
  0007cc38: cmp r0,#0x0
  0007cc3a: beq.w 0x0007ce6c
  0007cc3e: ldr r6,[sp,#0x28]
  0007cc40: ldr r5,[r6,#0x0]
  0007cc42: ldr r0,[0x0007cf8c]
  0007cc44: ldrh r1,[r5,#0x2]
  0007cc46: add r0,pc
  0007cc48: ldr r4,[r0,#0x0]
  0007cc4a: add.w r0,r1,r1, lsl #0x1
  0007cc4e: lsls r0,r0,#0x2
  0007cc50: blx r4
  0007cc52: str r0,[r5,#0x14]
  0007cc54: ldr r5,[r6,#0x0]
  0007cc56: ldrh r0,[r5,#0x2]
  0007cc58: add.w r0,r0,r0, lsl #0x1
  0007cc5c: lsls r0,r0,#0x2
  0007cc5e: blx r4
  0007cc60: str r0,[r5,#0x18]
  0007cc62: movs r1,#0x3
  0007cc64: ldr r5,[r6,#0x0]
  0007cc66: ldrh r0,[r5,#0x28]
  0007cc68: blx 0x0006ec2c
  0007cc6c: ldrh r5,[r5,#0x2]
  0007cc6e: str r0,[sp,#0x1c]
  0007cc70: add.w r0,r5,r5, lsl #0x1
  0007cc74: lsls r6,r0,#0x2
  0007cc76: mov r0,r6
  0007cc78: blx r4
  0007cc7a: mov r4,r0
  0007cc7c: cmp r5,#0x0
  0007cc7e: str r5,[sp,#0x24]
  0007cc80: beq 0x0007cc98
  0007cc82: subs r6,#0xc
  0007cc84: movs r1,#0xc
  0007cc86: mov r0,r6
  0007cc88: blx 0x0006ec5c
  0007cc8c: subs r0,r6,r1
  0007cc8e: add.w r1,r0,#0xc
  0007cc92: mov r0,r4
  0007cc94: blx 0x0006ec14
  0007cc98: str r4,[sp,#0x20]
  0007cc9a: vmov.f32 s16,0x3f800000
  0007cc9e: ldr r0,[0x0007d04c]
  0007cca0: mov.w r11,#0x0
  0007cca4: mov.w r8,#0x0
  0007cca8: add r0,pc
  0007ccaa: ldr.w r9,[r0,#0x0]
  0007ccae: b 0x0007cdc2
  0007ccb0: ldr r0,[sp,#0x28]
  0007ccb2: ldr r1,[r0,#0x0]
  0007ccb4: movs r0,#0x4
  0007ccb6: ldrd r12,r2,[r1,#0x4]
  0007ccba: ldr r3,[r1,#0x2c]
  0007ccbc: add.w r6,r3,r11
  0007ccc0: ldrh.w r3,[r3,r11]
  0007ccc4: ldrh r1,[r6,#0x4]
  0007ccc6: ldrh r6,[r6,#0x2]
  0007ccc8: orr.w r4,r0,r3, lsl #0x3
  0007cccc: add.w r5,r2,r1, lsl #0x3
  0007ccd0: add r4,r2
  0007ccd2: add.w r10,r6,r6, lsl #0x1
  0007ccd6: vldr.32 s0,[r5]
  0007ccda: orr.w r5,r0,r6, lsl #0x3
  0007ccde: vldr.32 s2,[r4]
  0007cce2: orr.w r4,r0,r1, lsl #0x3
  0007cce6: add r4,r2
  0007cce8: add r5,r2
  0007ccea: vldr.32 s4,[r4]
  0007ccee: add.w r4,r2,r3, lsl #0x3
  0007ccf2: add.w r2,r2,r6, lsl #0x3
  0007ccf6: vldr.32 s10,[r5]
  0007ccfa: vldr.32 s6,[r4]
  0007ccfe: vsub.f32 s4,s4,s2
  0007cd02: vldr.32 s8,[r2]
  0007cd06: vsub.f32 s2,s10,s2
  0007cd0a: vsub.f32 s0,s0,s6
  0007cd0e: add.w r2,r3,r3, lsl #0x1
  0007cd12: add.w r6,r1,r1, lsl #0x1
  0007cd16: vsub.f32 s8,s8,s6
  0007cd1a: add.w r3,r12,r2, lsl #0x2
  0007cd1e: ldr r4,[sp,#0x20]
  0007cd20: add.w r0,r12,r6, lsl #0x2
  0007cd24: add r5,sp,#0x60
  0007cd26: vldr.32 s10,[r3,#0x8]
  0007cd2a: vldr.32 s3,[r0]
  0007cd2e: mov r1,r5
  0007cd30: vldr.32 s5,[r0,#0x4]
  0007cd34: vldr.32 s7,[r0,#0x8]
  0007cd38: add.w r0,r4,r2, lsl #0x2
  0007cd3c: vmul.f32 s6,s8,s4
  0007cd40: vldr.32 s8,[r3,#0x4]
  0007cd44: vmls.f32 s6,s0,s2
  0007cd48: vldr.32 s0,[r3]
  0007cd4c: add.w r3,r12,r10, lsl #0x2
  0007cd50: vldr.32 s12,[r3]
  0007cd54: vldr.32 s14,[r3,#0x4]
  0007cd58: vldr.32 s1,[r3,#0x8]
  0007cd5c: vsub.f32 s12,s12,s0
  0007cd60: vsub.f32 s14,s14,s8
  0007cd64: vsub.f32 s1,s1,s10
  0007cd68: vdiv.f32 s6,s16,s6
  0007cd6c: vsub.f32 s8,s5,s8
  0007cd70: vsub.f32 s0,s3,s0
  0007cd74: vsub.f32 s10,s7,s10
  0007cd78: vmul.f32 s14,s14,s4
  0007cd7c: vmul.f32 s12,s12,s4
  0007cd80: vmul.f32 s4,s1,s4
  0007cd84: vmls.f32 s14,s8,s2
  0007cd88: vmls.f32 s12,s0,s2
  0007cd8c: vmls.f32 s4,s10,s2
  0007cd90: vmul.f32 s0,s14,s6
  0007cd94: vmul.f32 s2,s12,s6
  0007cd98: vmul.f32 s4,s4,s6
  0007cd9c: vstr.32 s0,[sp,#0x64]
  0007cda0: vstr.32 s2,[sp,#0x60]
  0007cda4: vstr.32 s4,[sp,#0x68]
  0007cda8: blx r9
  0007cdaa: add.w r0,r4,r10, lsl #0x2
  0007cdae: mov r1,r5
  0007cdb0: blx r9
  0007cdb2: add.w r0,r4,r6, lsl #0x2
  0007cdb6: mov r1,r5
  0007cdb8: blx r9
  0007cdba: add.w r11,r11,#0x6
  0007cdbe: add.w r8,r8,#0x1
  0007cdc2: ldr r0,[sp,#0x1c]
  0007cdc4: cmp r8,r0
  0007cdc6: bcc.w 0x0007ccb0
  0007cdca: ldrd r11,r5,[sp,#0x24]
  0007cdce: movs r4,#0x0
  0007cdd0: b 0x0007ce66
  0007cdd2: ldr r0,[r5,#0x0]
  0007cdd4: add.w r8,sp,#0x50
  0007cdd8: str.w r11,[sp,#0x24]
  0007cddc: add.w r11,sp,#0x60
  0007cde0: ldr r0,[r0,#0x10]
  0007cde2: ldr r1,[r0,r4]
  0007cde4: add r0,r4
  0007cde6: ldrd r2,r0,[r0,#0x4]
  0007cdea: strd r1,r2,[sp,#0x60]
  0007cdee: mov r1,r8
  0007cdf0: str r0,[sp,#0x68]
  0007cdf2: ldr r0,[sp,#0x20]
  0007cdf4: add r0,r4
  0007cdf6: vldr.64 d16,[r0]
  0007cdfa: ldr r0,[r0,#0x8]
  0007cdfc: str r0,[sp,#0x58]
  0007cdfe: mov r0,r11
  0007ce00: vstr.64 d16,[sp,#0x50]
  0007ce04: blx 0x0006ec68
  0007ce08: add.w r10,sp,#0x2c
  0007ce0c: mov r2,r0
  0007ce0e: mov r1,r11
  0007ce10: mov r0,r10
  0007ce12: blx 0x0006ec74
  0007ce16: add.w r9,sp,#0x38
  0007ce1a: mov r1,r8
  0007ce1c: mov r2,r10
  0007ce1e: mov r0,r9
  0007ce20: blx 0x0006ec38
  0007ce24: add r6,sp,#0x44
  0007ce26: mov r1,r9
  0007ce28: mov r0,r6
  0007ce2a: blx 0x0006ec80
  0007ce2e: ldr r0,[r5,#0x0]
  0007ce30: mov r2,r6
  0007ce32: ldr r1,[sp,#0x44]
  0007ce34: ldr r0,[r0,#0x14]
  0007ce36: str r1,[r0,r4]
  0007ce38: add r0,r4
  0007ce3a: ldr r1,[sp,#0x48]
  0007ce3c: str r1,[r0,#0x4]
  0007ce3e: ldr r1,[sp,#0x4c]
  0007ce40: str r1,[r0,#0x8]
  0007ce42: mov r0,r9
  0007ce44: mov r1,r11
  0007ce46: blx 0x0006ec8c
  0007ce4a: ldr r0,[r5,#0x0]
  0007ce4c: ldr r1,[sp,#0x38]
  0007ce4e: ldr.w r11,[sp,#0x24]
  0007ce52: ldr r0,[r0,#0x18]
  0007ce54: sub.w r11,r11,#0x1
  0007ce58: str r1,[r0,r4]
  0007ce5a: add r0,r4
  0007ce5c: adds r4,#0xc
  0007ce5e: ldr r1,[sp,#0x3c]
  0007ce60: str r1,[r0,#0x4]
  0007ce62: ldr r1,[sp,#0x40]
  0007ce64: str r1,[r0,#0x8]
  0007ce66: cmp.w r11,#0x0
  0007ce6a: bne 0x0007cdd2
  0007ce6c: ldr.w r8,[sp,#0x28]
  0007ce70: ldr.w r9,[sp,#0x14]
  0007ce74: ldr.w r4,[r8,#0x0]
  0007ce78: ldrb r0,[r4,#0x0]
  0007ce7a: lsls r0,r0,#0x1c
  0007ce7c: bpl 0x0007cf28
  0007ce7e: ldr r0,[sp,#0xc]
  0007ce80: cbz r0,0x0007ceea
  0007ce82: ldrh r0,[r4,#0x2]
  0007ce84: lsls r0,r0,#0x2
  0007ce86: blx 0x0006ec08
  0007ce8a: ldr r1,[sp,#0x18]
  0007ce8c: mov r5,r0
  0007ce8e: ldrh r0,[r4,#0x2]
  0007ce90: ldr r2,[r1,#0x0]
  0007ce92: mov r1,r5
  0007ce94: lsls r0,r0,#0x2
  0007ce96: blx 0x0006eb90
  0007ce9a: ldr.w r11,[sp,#0x10]
  0007ce9e: cmp r0,#0x0
  0007cea0: beq.w 0x0007d03a
  0007cea4: ldr.w r4,[r8,#0x0]
  0007cea8: ldrh r0,[r4,#0x2]
  0007ceaa: lsls r0,r0,#0x4
  0007ceac: blx 0x0006ec08
  0007ceb0: str r0,[r4,#0xc]
  0007ceb2: movs r2,#0x0
  0007ceb4: ldr.w r0,[r8,#0x0]
  0007ceb8: movs r3,#0x0
  0007ceba: vldr.32 s0,[pc,#0x18c]
  0007cebe: ldrh r1,[r0,#0x2]
  0007cec0: lsls r1,r1,#0x2
  0007cec2: b 0x0007cede
  0007cec4: ldrb r6,[r5,r3]
  0007cec6: adds r3,#0x1
  0007cec8: vmov s2,r6
  0007cecc: vcvt.f32.u32 s2,s2
  0007ced0: ldr r6,[r0,#0xc]
  0007ced2: add r6,r2
  0007ced4: adds r2,#0x4
  0007ced6: vdiv.f32 s2,s2,s0
  0007ceda: vstr.32 s2,[r6]
  0007cede: cmp r3,r1
  0007cee0: bcc 0x0007cec4
  0007cee2: mov r0,r5
  0007cee4: blx 0x0006ebfc
  0007cee8: b 0x0007cf28
  0007ceea: tst r9,#0x18
  0007ceee: beq 0x0007cf28
  0007cef0: ldrh r0,[r4,#0x2]
  0007cef2: lsls r0,r0,#0x4
  0007cef4: blx 0x0006ec08
  0007cef8: str r0,[r4,#0xc]
  0007cefa: ldr.w r0,[r8,#0x0]
  0007cefe: ldr r1,[sp,#0x18]
  0007cf00: ldrh r3,[r0,#0x2]
  0007cf02: ldr r2,[r1,#0x0]
  0007cf04: ldr r1,[r0,#0xc]
  0007cf06: lsls r0,r3,#0x4
  0007cf08: blx 0x0006eb90
  0007cf0c: mov r1,r0
  0007cf0e: mov.w r0,#0xffffffff
  0007cf12: cmp r1,#0x0
  0007cf14: it ne
  0007cf16: mov.ne r0,#0x1
  0007cf18: ldr r2,[sp,#0x8]
  0007cf1a: cmp r2,#0x0
  0007cf1c: beq.w 0x0007d042
  0007cf20: ldr.w r11,[sp,#0x10]
  0007cf24: cbnz r1,0x0007cf34
  0007cf26: b 0x0007cbe0
  0007cf28: ldr r0,[sp,#0x8]
  0007cf2a: ldr.w r11,[sp,#0x10]
  0007cf2e: cmp r0,#0x0
  0007cf30: beq.w 0x0007d036
  0007cf34: ldr r4,[sp,#0x18]
  0007cf36: mov r2,r9
  0007cf38: ldr.w r0,[r8,#0x0]
  0007cf3c: ldr r1,[r4,#0x0]
  0007cf3e: blx 0x0006ec98
  0007cf42: cmp r0,#0x0
  0007cf44: beq.w 0x0007cbdc
  0007cf48: ldr r2,[r4,#0x0]
  0007cf4a: add r1,sp,#0x50
  0007cf4c: movs r0,#0x2
  0007cf4e: blx 0x0006eb90
  0007cf52: cmp r0,#0x0
  0007cf54: beq.w 0x0007cbdc
  0007cf58: ldr.w r0,[r8,#0x0]
  0007cf5c: ldr r1,[r0,#0x34]
  0007cf5e: cbz r1,0x0007cf68
  0007cf60: adds r0,#0x3c
  0007cf62: adds r1,#0xd4
  0007cf64: blx 0x0006eca4
  0007cf68: ldrh.w r0,[sp,#0x50]
  0007cf6c: cmp r0,#0x0
  0007cf6e: beq 0x0007d036
  0007cf70: vmov.i32 q4,#0x0
  0007cf74: ldr.w r8,[sp,#0x28]
  0007cf78: add r5,sp,#0x60
  0007cf7a: movs r6,#0x0
  0007cf7c: mov.w r9,#0x3f800000
  0007cf80: mov.w r10,#0x1
  0007cf84: movs r4,#0x0
  0007cf86: b 0x0007d030
  0007cf90: movs r0,#0x88
  0007cf92: blx 0x0006eb24
  0007cf96: add.w r1,r0,#0x70
  0007cf9a: str.w r6,[r0,#0x82]
  0007cf9e: str.w r6,[r0,#0x7e]
  0007cfa2: mov r3,r5
  0007cfa4: strb.w r6,[r0,#0x38]
  0007cfa8: strh r6,[r0,#0x22]
  0007cfaa: strb r6,[r0,#0x0]
  0007cfac: strd r9,r6,[r0,#0x4c]
  0007cfb0: str r6,[r0,#0x54]
  0007cfb2: str.w r6,[r0,#0x59]
  0007cfb6: str.w r6,[r0,#0x55]
  0007cfba: vst1.32 {d8,d9},[r1]
  0007cfbe: add.w r1,r0,#0x60
  0007cfc2: vst1.32 {d8,d9},[r1]
  0007cfc6: add.w r1,r0,#0x28
  0007cfca: vst1.32 {d8,d9},[r1]
  0007cfce: add.w r1,r0,#0x12
  0007cfd2: vst1.16 {d8,d9},[r1]
  0007cfd6: adds r1,r0,#0x2
  0007cfd8: vst1.16 {d8,d9},[r1]
  0007cfdc: add.w r1,r0,#0x3c
  0007cfe0: vst1.32 {d8,d9},[r1]
  0007cfe4: str r0,[sp,#0x60]
  0007cfe6: strb.w r10,[r0,#0x84]
  0007cfea: ldr.w r1,[r8,#0x0]
  0007cfee: ldrb r2,[r1,#0x0]
  0007cff0: strb r2,[r0,#0x0]
  0007cff2: ldr r1,[r1,#0x30]
  0007cff4: str r1,[r0,#0x30]
  0007cff6: ldr r0,[r7,#0x8]
  0007cff8: str r0,[sp,#0x0]
  0007cffa: ldr r0,[sp,#0x4]
  0007cffc: ldrd r2,r1,[sp,#0x14]
  0007d000: blx 0x0006ecb0
  0007d004: adds r0,#0x1
  0007d006: beq.w 0x0007cbdc
  0007d00a: ldr r0,[sp,#0x60]
  0007d00c: ldr.w r2,[r8,#0x0]
  0007d010: add.w r1,r0,#0x3c
  0007d014: add.w r0,r2,#0x3c
  0007d018: blx 0x0006eca4
  0007d01c: ldr.w r1,[r8,#0x0]
  0007d020: ldr r0,[sp,#0x60]
  0007d022: ldr r1,[r1,#0x34]
  0007d024: adds r1,#0x3c
  0007d026: blx 0x0006ecbc
  0007d02a: ldrh.w r0,[sp,#0x50]
  0007d02e: adds r4,#0x1
  0007d030: uxth r0,r0
  0007d032: cmp r4,r0
  0007d034: bcc 0x0007cf90
  0007d036: movs r0,#0x1
  0007d038: b 0x0007cbe0
  0007d03a: mov r0,r5
  0007d03c: b 0x0007cbd8
  0007d03e: mov.w r0,#0xffffffff
  0007d042: ldr.w r11,[sp,#0x10]
  0007d046: b 0x0007cbe0
```
