# Transform::InsertKeyFrame_old
elf 0x7640c body 3778
Sig: undefined __stdcall InsertKeyFrame_old(float * param_1, longlong param_2, int param_3)

## decompile
```c

/* AbyssEngine::Transform::InsertKeyFrame_old(float const*, long long, int) */

void AbyssEngine::Transform::InsertKeyFrame_old(float *param_1,longlong param_2,int param_3)

{
  KeyFrame *this;
  int iVar1;
  uint uVar2;
  int iVar3;
  Vector *this_00;
  float *in_r1;
  Array *pAVar4;
  Vector *pVVar5;
  Vector *pVVar6;
  Vector *pVVar7;
  Vector *pVVar8;
  Vector *pVVar9;
  int iVar10;
  float fVar11;
  uint uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  uint uVar16;
  int iVar17;
  uint uVar18;
  float fVar19;
  KeyFrame *pKVar20;
  uint uVar21;
  float *pfVar22;
  code *pcVar23;
  uint uVar24;
  int *piVar25;
  float *pfVar26;
  code *pcVar27;
  uint uVar28;
  int iVar29;
  bool bVar30;
  undefined8 uVar31;
  AEMath aAStack_40 [12];
  int local_34;
  
  uVar18 = (uint)((ulonglong)param_2 >> 0x20);
  uVar12 = (uint)param_2;
  pfVar22 = param_1 + 0x48;
  fVar19 = 0.0;
  piVar25 = *(int **)(DAT_0008678c + 0x8642e);
  local_34 = *piVar25;
  **(int **)(DAT_00086788 + 0x8642c) = **(int **)(DAT_00086788 + 0x8642c) + 1;
  fVar13 = param_1[0x3f];
  bVar30 = (uint)param_1[0x3e] < (uint)param_3;
  fVar11 = (float)(param_3 >> 0x1f);
  if ((int)(((int)fVar13 - (int)fVar11) - (uint)bVar30) < 0 !=
      (SBORROW4((int)fVar13,(int)fVar11) != SBORROW4((int)fVar13 - (int)fVar11,(uint)bVar30))) {
    param_1[0x3e] = (float)param_3;
    param_1[0x3f] = fVar11;
  }
  pAVar4 = (Array *)(param_1 + 0x47);
  while (((uint)fVar19 < (uint)param_1[0x47] &&
         (iVar1 = *(int *)((int)*pfVar22 + (int)fVar19 * 4), iVar3 = *(int *)(iVar1 + 0x54),
         bVar30 = *(uint *)(iVar1 + 0x50) < (uint)param_3,
         (int)((iVar3 - (int)fVar11) - (uint)bVar30) < 0 !=
         (SBORROW4(iVar3,(int)fVar11) != SBORROW4(iVar3 - (int)fVar11,(uint)bVar30))))) {
    fVar19 = (float)((int)fVar19 + 1);
  }
  this = operator_new(0x60);
  KeyFrame::KeyFrame(this);
  if (fVar19 != 0.0) {
    pcVar23 = *(code **)(DAT_00086790 + 0x864aa);
    (*pcVar23)(this,*(undefined4 *)((int)*pfVar22 + (int)fVar19 * 4 + -4));
    (*pcVar23)(this + 0xc,*(int *)((int)*pfVar22 + (int)fVar19 * 4 + -4) + 0xc);
    (*pcVar23)(this + 0x18,*(int *)((int)*pfVar22 + (int)fVar19 * 4 + -4) + 0x18);
    iVar1 = *(int *)((int)*pfVar22 + (int)fVar19 * 4 + -4);
    uVar31 = *(undefined8 *)(iVar1 + 0x58);
    *(undefined8 *)(this + 0x50) = *(undefined8 *)(iVar1 + 0x50);
    *(undefined8 *)(this + 0x58) = uVar31;
  }
  uVar2 = uVar12 - 1;
  iVar1 = uVar18 - (uVar12 == 0);
  if ((uint)-iVar1 < (uint)(uVar2 < 8)) {
                    /* WARNING: Could not recover jumptable at 0x00086612. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&UNK_00086616 + (uint)*(byte *)(uVar12 + 0x86615) * 2))
              (uVar2,-(uint)(uVar2 >= 8) - iVar1,0,this);
    return;
  }
  if (param_2 == 0x10) {
    fVar13 = *in_r1;
    *(float *)(this + 0x10) = fVar13;
LAB_0008669c:
    if (param_1[0x39] < fVar13) {
      param_1[0x39] = fVar13;
    }
  }
  else {
    if (param_2 == 0x20) {
      fVar13 = *in_r1;
      *(float *)(this + 0x14) = fVar13;
      goto LAB_0008669c;
    }
    if (param_2 == 0x38) {
      fVar13 = *in_r1;
      fVar14 = in_r1[1];
      fVar15 = in_r1[2];
      *(float *)(this + 0xc) = fVar13;
      fVar19 = fVar14;
      if (fVar14 < fVar13) {
        fVar19 = fVar13;
      }
      *(float *)(this + 0x10) = fVar14;
      fVar13 = fVar15;
      if (fVar15 < fVar19) {
        fVar13 = fVar19;
      }
      *(float *)(this + 0x14) = fVar15;
      goto LAB_0008669c;
    }
    if (param_2 == 0x40) {
      *(float *)(this + 0x18) = *in_r1;
    }
    else if (param_2 == 0x80) {
      *(float *)(this + 0x1c) = *in_r1;
    }
    else if (param_2 == 0x100) {
      *(float *)(this + 0x20) = *in_r1;
    }
    else if (param_2 == 0x1c0) {
      fVar13 = *in_r1;
      fVar19 = in_r1[1];
      fVar14 = in_r1[2];
      pKVar20 = this + 0x18;
LAB_000866dc:
      *(float *)pKVar20 = fVar13;
      *(float *)(pKVar20 + 4) = fVar19;
      *(float *)(pKVar20 + 8) = fVar14;
    }
    else if (param_2 == 0x200) {
      *(float *)(this + 0x48) = *in_r1 / DAT_00086794;
    }
    else if (param_2 == 0x400) {
      *(float *)(this + 0x24) = -*in_r1;
    }
    else if (param_2 == 0x800) {
      *(float *)(this + 0x28) = *in_r1;
    }
    else if (param_2 == 0x1000) {
      *(float *)(this + 0x2c) = *in_r1;
    }
    else if (param_2 == 0x1c00) {
      *(float *)(this + 0x24) = -*in_r1;
      fVar13 = in_r1[2];
      *(float *)(this + 0x28) = in_r1[1];
      *(float *)(this + 0x2c) = fVar13;
    }
    else if (param_2 == 0x2000) {
      *(float *)(this + 0x30) = *in_r1;
    }
    else if (param_2 == 0x4000) {
      *(float *)(this + 0x34) = -*in_r1;
    }
    else {
      if (param_2 == 0x8000) {
        fVar13 = *in_r1;
      }
      else {
        if (param_2 != 0xe000) {
          if (param_2 == 0x10000) {
            *(float *)(this + 0x3c) = *in_r1;
          }
          else if (param_2 == 0x20000) {
            *(float *)(this + 0x40) = *in_r1;
          }
          else if (param_2 == 0x40000) {
            *(float *)(this + 0x44) = *in_r1;
          }
          else if (param_2 == 0x70000) {
            pKVar20 = this + 0x3c;
            fVar13 = *in_r1;
            fVar19 = in_r1[1];
            fVar14 = in_r1[2];
            goto LAB_000866dc;
          }
          goto LAB_000867a0;
        }
        *(float *)(this + 0x30) = *in_r1;
        *(float *)(this + 0x34) = -in_r1[1];
        fVar13 = in_r1[2];
      }
      *(float *)(this + 0x38) = fVar13;
    }
  }
LAB_000867a0:
  *(int *)(this + 0x50) = param_3;
  *(float *)(this + 0x54) = fVar11;
  pKVar20 = this + 0x58;
  *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar18;
  *(uint *)pKVar20 = *(uint *)pKVar20 | uVar12;
  uVar2 = *(uint *)pAVar4;
  if (uVar2 == 0) {
    iVar1 = *piVar25 - local_34;
    if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(iVar1,*piVar25);
    }
    (*(code *)(DAT_001ab164 + 0x1ab168))();
    return;
  }
  pVVar5 = (Vector *)(this + 0x3c);
  iVar1 = 0;
  pVVar6 = (Vector *)(this + 0x30);
  pVVar7 = (Vector *)(this + 0x24);
  pVVar8 = (Vector *)(this + 0x18);
  pVVar9 = (Vector *)(this + 0xc);
  for (uVar16 = 0; uVar16 < uVar2; uVar16 = uVar16 + 1) {
    iVar3 = *(int *)((int)*pfVar22 + iVar1);
    uVar21 = *(uint *)(iVar3 + 0x50);
    iVar10 = *(int *)(iVar3 + 0x54);
    if ((int)((iVar10 - (int)fVar11) - (uint)(uVar21 < (uint)param_3)) < 0 ==
        (SBORROW4(iVar10,(int)fVar11) !=
        SBORROW4(iVar10 - (int)fVar11,(uint)(uVar21 < (uint)param_3)))) break;
    if (uVar16 != 0) {
      uVar2 = *(uint *)(iVar3 + 0x58);
      uVar28 = *(uint *)(iVar3 + 0x5c);
      if ((uVar2 & uVar12) == 0 && (uVar28 & uVar18) == 0) {
        iVar17 = *(int *)((int)*pfVar22 + iVar1 + -4);
        uVar24 = *(uint *)(iVar17 + 0x50);
        iVar29 = *(int *)(iVar17 + 0x54);
        fVar13 = (float)__aeabi_l2f(uVar21 - uVar24,(iVar10 - iVar29) - (uint)(uVar21 < uVar24));
        fVar19 = (float)__aeabi_l2f(param_3 - uVar24,
                                    ((int)fVar11 - iVar29) - (uint)((uint)param_3 < uVar24));
        fVar13 = fVar13 / fVar19;
        *(uint *)(iVar3 + 0x58) = uVar2 | uVar12;
        *(uint *)(iVar3 + 0x5c) = uVar28 | uVar18;
        uVar2 = uVar12 - 1;
        iVar10 = uVar18 - (uVar12 == 0);
        if ((uint)-iVar10 < (uint)(uVar2 < 8)) {
                    /* WARNING: Could not recover jumptable at 0x000869c2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*(code *)(&DAT_000869c6 + (uint)*(ushort *)(&DAT_000869c6 + uVar2 * 2) * 2))
                    (uVar2,-(uint)(uVar2 >= 8) - iVar10,uVar16);
          return;
        }
        if (param_2 == 0x10) {
          *(float *)(iVar3 + 0x10) =
               *(float *)(iVar17 + 0x10) +
               fVar13 * (*(float *)(this + 0x10) - *(float *)(iVar17 + 0x10));
        }
        else if (param_2 == 0x20) {
          *(float *)(iVar3 + 0x14) =
               *(float *)(iVar17 + 0x14) +
               fVar13 * (*(float *)(this + 0x14) - *(float *)(iVar17 + 0x14));
        }
        else if (param_2 == 0x38) {
          AEMath::VectorLerp(aAStack_40,(Vector *)(iVar17 + 0xc),pVVar9,fVar13);
          this_00 = (Vector *)(*(int *)((int)*pfVar22 + iVar1) + 0xc);
LAB_00086c1c:
          AEMath::Vector::operator=(this_00,(Vector *)aAStack_40);
        }
        else if (param_2 == 0x40) {
          *(float *)(iVar3 + 0x18) =
               *(float *)(iVar17 + 0x18) + fVar13 * (*(float *)pVVar8 - *(float *)(iVar17 + 0x18));
        }
        else if (param_2 == 0x80) {
          *(float *)(iVar3 + 0x1c) =
               *(float *)(iVar17 + 0x1c) +
               fVar13 * (*(float *)(this + 0x1c) - *(float *)(iVar17 + 0x1c));
        }
        else if (param_2 == 0x100) {
          *(float *)(iVar3 + 0x20) =
               *(float *)(iVar17 + 0x20) +
               fVar13 * (*(float *)(this + 0x20) - *(float *)(iVar17 + 0x20));
        }
        else {
          if (param_2 == 0x1c0) {
            AEMath::VectorLerp(aAStack_40,(Vector *)(iVar17 + 0x18),pVVar8,fVar13);
            this_00 = (Vector *)(*(int *)((int)*pfVar22 + iVar1) + 0x18);
            goto LAB_00086c1c;
          }
          if (param_2 == 0x200) {
            *(float *)(iVar3 + 0x48) =
                 *(float *)(iVar17 + 0x48) +
                 fVar13 * (*(float *)(this + 0x48) - *(float *)(iVar17 + 0x48));
          }
          else if (param_2 == 0x400) {
            *(float *)(iVar3 + 0x24) =
                 *(float *)(iVar17 + 0x24) + fVar13 * (*(float *)pVVar7 - *(float *)(iVar17 + 0x24))
            ;
          }
          else if (param_2 == 0x800) {
            *(float *)(iVar3 + 0x28) =
                 *(float *)(iVar17 + 0x28) +
                 fVar13 * (*(float *)(this + 0x28) - *(float *)(iVar17 + 0x28));
          }
          else if (param_2 == 0x1000) {
            *(float *)(iVar3 + 0x2c) =
                 *(float *)(iVar17 + 0x2c) +
                 fVar13 * (*(float *)(this + 0x2c) - *(float *)(iVar17 + 0x2c));
          }
          else {
            if (param_2 == 0x1c00) {
              AEMath::VectorLerp(aAStack_40,(Vector *)(iVar17 + 0x24),pVVar7,fVar13);
              this_00 = (Vector *)(*(int *)((int)*pfVar22 + iVar1) + 0x24);
              goto LAB_00086c1c;
            }
            if (param_2 == 0x2000) {
              *(float *)(iVar3 + 0x30) =
                   *(float *)(iVar17 + 0x30) +
                   fVar13 * (*(float *)pVVar6 - *(float *)(iVar17 + 0x30));
            }
            else if (param_2 == 0x4000) {
              *(float *)(iVar3 + 0x34) =
                   *(float *)(iVar17 + 0x34) +
                   fVar13 * (*(float *)(this + 0x34) - *(float *)(iVar17 + 0x34));
            }
            else if (param_2 == 0x8000) {
              *(float *)(iVar3 + 0x38) =
                   *(float *)(iVar17 + 0x38) +
                   fVar13 * (*(float *)(this + 0x38) - *(float *)(iVar17 + 0x38));
            }
            else {
              if (param_2 == 0xe000) {
                AEMath::VectorLerp(aAStack_40,(Vector *)(iVar17 + 0x30),pVVar6,fVar13);
                this_00 = (Vector *)(*(int *)((int)*pfVar22 + iVar1) + 0x30);
                goto LAB_00086c1c;
              }
              if (param_2 == 0x10000) {
                *(float *)(iVar3 + 0x3c) =
                     *(float *)(iVar17 + 0x3c) +
                     fVar13 * (*(float *)pVVar5 - *(float *)(iVar17 + 0x3c));
              }
              else if (param_2 == 0x20000) {
                *(float *)(iVar3 + 0x40) =
                     *(float *)(iVar17 + 0x40) +
                     fVar13 * (*(float *)(this + 0x40) - *(float *)(iVar17 + 0x40));
              }
              else if (param_2 == 0x40000) {
                *(float *)(iVar3 + 0x44) =
                     *(float *)(iVar17 + 0x44) +
                     fVar13 * (*(float *)(this + 0x44) - *(float *)(iVar17 + 0x44));
              }
              else if (param_2 == 0x70000) {
                AEMath::VectorLerp(aAStack_40,(Vector *)(iVar17 + 0x3c),pVVar5,fVar13);
                this_00 = (Vector *)(*(int *)((int)*pfVar22 + iVar1) + 0x3c);
                goto LAB_00086c1c;
              }
            }
          }
        }
      }
    }
    iVar1 = iVar1 + 4;
    uVar2 = *(uint *)pAVar4;
  }
  if (uVar16 == uVar2) {
    uVar16 = *(uint *)pKVar20;
    if ((uVar16 & 0x40) == 0) {
      *(undefined4 *)pVVar8 = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x18);
    }
    if (-1 < (int)(uVar16 << 0x18)) {
      *(undefined4 *)(this + 0x1c) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x1c);
    }
    if (-1 < (int)(uVar16 << 0x17)) {
      *(undefined4 *)(this + 0x20) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x20);
    }
    if ((uVar16 & 1) == 0) {
      *(undefined4 *)this = **(undefined4 **)((int)*pfVar22 + uVar2 * 4 + -4);
    }
    if (-1 < (int)(uVar16 << 0x1e)) {
      *(undefined4 *)(this + 4) = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 4);
    }
    if (-1 < (int)(uVar16 << 0x1d)) {
      *(undefined4 *)(this + 8) = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 8);
    }
    if (-1 < (int)(uVar16 << 0x1c)) {
      *(undefined4 *)pVVar9 = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0xc);
    }
    if (-1 < (int)(uVar16 << 0x1b)) {
      *(undefined4 *)(this + 0x10) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x10);
    }
    if (-1 < (int)(uVar16 << 0x1a)) {
      *(undefined4 *)(this + 0x14) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x14);
    }
    if (-1 < (int)(uVar16 << 0x16)) {
      *(undefined4 *)(this + 0x48) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x48);
    }
    if (-1 < (int)(uVar16 << 0xf)) {
      *(undefined4 *)pVVar5 = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x3c);
    }
    if (-1 < (int)(uVar16 << 0xe)) {
      *(undefined4 *)(this + 0x40) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x40);
    }
    if (-1 < (int)(uVar16 << 0xd)) {
      *(undefined4 *)(this + 0x44) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x44);
    }
    if (-1 < (int)(uVar16 << 0x15)) {
      *(undefined4 *)pVVar7 = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x24);
    }
    if (-1 < (int)(uVar16 << 0x14)) {
      *(undefined4 *)(this + 0x28) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x28);
    }
    if (-1 < (int)(uVar16 << 0x13)) {
      *(undefined4 *)(this + 0x2c) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x2c);
    }
    if (-1 < (int)(uVar16 << 0x12)) {
      *(undefined4 *)pVVar6 = *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x30);
    }
    if (-1 < (int)(uVar16 << 0x11)) {
      *(undefined4 *)(this + 0x34) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x34);
    }
    if (-1 < (int)(uVar16 << 0x10)) {
      *(undefined4 *)(this + 0x38) =
           *(undefined4 *)(*(int *)((int)*pfVar22 + uVar2 * 4 + -4) + 0x38);
    }
    *(uint *)pKVar20 = uVar16 | uVar12;
    *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar18;
    ArrayAdd<AbyssEngine::KeyFrame*>(this,pAVar4);
    goto LAB_000873c6;
  }
  pfVar26 = *(float **)((int)*pfVar22 + iVar1);
  fVar19 = pfVar26[0x14];
  fVar13 = pfVar26[0x15];
  if (fVar13 == fVar11 && fVar19 == (float)param_3) {
    fVar11 = pfVar26[0x16];
    fVar13 = *(float *)pVVar8;
    if (((uint)fVar11 & 0x40) != 0) {
      fVar13 = fVar13 + pfVar26[6];
    }
    pfVar26[6] = fVar13;
    fVar13 = *(float *)(this + 0x1c);
    if ((int)fVar11 << 0x18 < 0) {
      fVar13 = fVar13 + pfVar26[7];
    }
    pfVar26[7] = fVar13;
    fVar13 = *(float *)(this + 0x20);
    if ((int)fVar11 << 0x17 < 0) {
      fVar13 = fVar13 + pfVar26[8];
    }
    pfVar26[8] = fVar13;
    fVar13 = *(float *)this;
    if (((uint)fVar11 & 1) != 0) {
      fVar13 = fVar13 + *pfVar26;
    }
    *pfVar26 = fVar13;
    fVar13 = *(float *)(this + 4);
    if ((int)fVar11 << 0x1e < 0) {
      fVar13 = fVar13 + pfVar26[1];
    }
    pfVar26[1] = fVar13;
    fVar13 = *(float *)(this + 8);
    if ((int)fVar11 << 0x1d < 0) {
      fVar13 = fVar13 + pfVar26[2];
    }
    pfVar26[2] = fVar13;
    fVar13 = *(float *)pVVar9;
    if ((int)fVar11 << 0x1c < 0) {
      fVar13 = fVar13 * pfVar26[3];
    }
    pfVar26[3] = fVar13;
    fVar13 = *(float *)(this + 0x10);
    if ((int)fVar11 << 0x1b < 0) {
      fVar13 = fVar13 * pfVar26[4];
    }
    pfVar26[4] = fVar13;
    fVar13 = *(float *)(this + 0x14);
    if ((int)fVar11 << 0x1a < 0) {
      fVar13 = fVar13 * pfVar26[5];
    }
    pfVar26[5] = fVar13;
    pfVar26[0x12] = *(float *)(this + 0x48);
    fVar13 = *(float *)(this + 0x3c);
    if ((int)fVar11 << 0xf < 0) {
      fVar13 = fVar13 + pfVar26[0xf];
    }
    pfVar26[0xf] = fVar13;
    fVar13 = *(float *)(this + 0x40);
    if ((int)fVar11 << 0xe < 0) {
      fVar13 = fVar13 + pfVar26[0x10];
    }
    pfVar26[0x10] = fVar13;
    fVar13 = *(float *)(this + 0x44);
    if ((int)fVar11 << 0xd < 0) {
      fVar13 = fVar13 + pfVar26[0x11];
    }
    pfVar26[0x11] = fVar13;
    fVar13 = *(float *)pVVar7;
    if ((int)fVar11 << 0x15 < 0) {
      fVar13 = fVar13 + pfVar26[9];
    }
    pfVar26[9] = fVar13;
    fVar13 = *(float *)(this + 0x28);
    if ((int)fVar11 << 0x14 < 0) {
      fVar13 = fVar13 + pfVar26[10];
    }
    pfVar26[10] = fVar13;
    fVar13 = *(float *)(this + 0x2c);
    if ((int)fVar11 << 0x13 < 0) {
      fVar13 = fVar13 + pfVar26[0xb];
    }
    pfVar26[0xb] = fVar13;
    fVar13 = *(float *)pVVar6;
    if ((int)fVar11 << 0x12 < 0) {
      fVar13 = fVar13 * pfVar26[0xc];
    }
    pfVar26[0xc] = fVar13;
    fVar13 = *(float *)(this + 0x34);
    if ((int)fVar11 << 0x11 < 0) {
      fVar13 = fVar13 * pfVar26[0xd];
    }
    pfVar26[0xd] = fVar13;
    fVar13 = *(float *)(this + 0x38);
    if ((int)fVar11 << 0x10 < 0) {
      fVar13 = fVar13 * pfVar26[0xe];
    }
    pfVar26[0xe] = fVar13;
    pfVar26[0x16] = (float)((uint)fVar11 | uVar12);
    pfVar26[0x17] = (float)((uint)pfVar26[0x17] | uVar18);
    *(uint *)pKVar20 = *(uint *)pKVar20 | uVar12;
    *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar18;
    UpdateKeyFrames((Transform *)param_1,this,uVar16);
    operator_delete(this);
    goto LAB_000873c6;
  }
  if (uVar16 == 0) {
    InsertKeyFrame((Transform *)param_1,this,0);
    UpdateKeyFrames((Transform *)param_1,this,0);
    goto LAB_000873c6;
  }
  iVar3 = *(int *)((int)*pfVar22 + iVar1 + -4);
  fVar14 = *(float *)(iVar3 + 0x50);
  iVar10 = *(int *)(iVar3 + 0x54);
  fVar11 = *(float *)(this + 0x50);
  fVar11 = (float)__aeabi_l2f((int)fVar11 - (int)fVar14,
                              (*(int *)(this + 0x54) - iVar10) - (uint)((uint)fVar11 < (uint)fVar14)
                             );
  fVar13 = (float)__aeabi_l2f((int)fVar19 - (int)fVar14,
                              ((int)fVar13 - iVar10) - (uint)((uint)fVar19 < (uint)fVar14));
  fVar11 = fVar11 / fVar13;
  pcVar27 = *(code **)(DAT_000873f0 + 0x87078);
  (*pcVar27)(aAStack_40,iVar3 + 0x18,pfVar26 + 6,fVar11);
  pcVar23 = *(code **)(DAT_000873f4 + 0x8708c);
  (*pcVar23)(pVVar8,aAStack_40);
  (*pcVar27)(aAStack_40,*(undefined4 *)((int)*pfVar22 + iVar1 + -4),
             *(undefined4 *)((int)*pfVar22 + iVar1),fVar11);
  (*pcVar23)(this,aAStack_40);
  (*pcVar27)(aAStack_40,*(int *)((int)*pfVar22 + iVar1 + -4) + 0xc,
             *(int *)((int)*pfVar22 + iVar1) + 0xc,fVar11);
  (*pcVar23)(pVVar9,aAStack_40);
  iVar10 = *(int *)((int)*pfVar22 + iVar1);
  iVar3 = *(int *)((int)*pfVar22 + iVar1 + -4);
  fVar13 = *(float *)(iVar3 + 0x48);
  *(float *)(this + 0x48) = fVar13 + fVar11 * (*(float *)(iVar10 + 0x48) - fVar13);
  (*pcVar27)(aAStack_40,iVar3 + 0x3c,iVar10 + 0x3c,fVar11);
  (*pcVar23)(pVVar5,aAStack_40);
  (*pcVar27)(aAStack_40,*(int *)((int)*pfVar22 + iVar1 + -4) + 0x24,
             *(int *)((int)*pfVar22 + iVar1) + 0x24,fVar11);
  (*pcVar23)(pVVar7,aAStack_40);
  (*pcVar27)(aAStack_40,*(int *)((int)*pfVar22 + iVar1 + -4) + 0x30,
             *(int *)((int)*pfVar22 + iVar1) + 0x30,fVar11);
  (*pcVar23)(pVVar6,aAStack_40);
  iVar1 = *(int *)((int)*pfVar22 + iVar1 + -4);
  uVar2 = *(uint *)(iVar1 + 0x5c);
  *(uint *)pKVar20 = *(uint *)(iVar1 + 0x58) | *(uint *)pKVar20;
  *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar2;
  uVar2 = uVar12 - 1;
  iVar1 = uVar18 - (uVar12 == 0);
  if ((uint)-iVar1 < (uint)(uVar2 < 8)) {
                    /* WARNING: Could not recover jumptable at 0x0008728c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&UNK_00087290 + (uint)*(byte *)(uVar12 + 0x8728f) * 2))
              (uVar2,-(uint)(uVar2 >= 8) - iVar1,uVar16,7 - uVar2);
    return;
  }
  if (param_2 == 0x10) {
    *(float *)(this + 0x10) = *in_r1;
  }
  else if (param_2 == 0x20) {
    *(float *)(this + 0x14) = *in_r1;
  }
  else if (param_2 == 0x38) {
    pKVar20 = this + 0xc;
LAB_00087306:
    fVar11 = in_r1[1];
    fVar13 = in_r1[2];
    *(float *)pKVar20 = *in_r1;
    *(float *)(pKVar20 + 4) = fVar11;
    *(float *)(pKVar20 + 8) = fVar13;
  }
  else {
    if (param_2 == 0x40) goto LAB_00087392;
    if (param_2 == 0x80) {
      *(float *)(this + 0x1c) = *in_r1;
    }
    else if (param_2 == 0x100) {
      *(float *)(this + 0x20) = *in_r1;
    }
    else {
      if (param_2 == 0x1c0) {
        pKVar20 = this + 0x18;
        goto LAB_00087306;
      }
      if (param_2 == 0x200) {
        *(float *)(this + 0x48) = *in_r1;
      }
      else if (param_2 == 0x400) {
        *(float *)pVVar7 = -*in_r1;
      }
      else if (param_2 == 0x800) {
        *(float *)(this + 0x28) = *in_r1;
      }
      else if (param_2 == 0x1000) {
        *(float *)(this + 0x2c) = *in_r1;
      }
      else if (param_2 == 0x1c00) {
        *(float *)(this + 0x24) = -*in_r1;
        fVar11 = in_r1[2];
        *(float *)(this + 0x28) = in_r1[1];
        *(float *)(this + 0x2c) = fVar11;
      }
      else {
        pVVar8 = pVVar6;
        if (param_2 == 0x2000) {
LAB_00087392:
          *(float *)pVVar8 = *in_r1;
        }
        else if (param_2 == 0x4000) {
          *(float *)(this + 0x34) = -*in_r1;
        }
        else {
          if (param_2 == 0x8000) {
            fVar11 = *in_r1;
          }
          else {
            if (param_2 != 0xe000) {
              pVVar8 = pVVar5;
              if (param_2 == 0x10000) goto LAB_00087392;
              if (param_2 == 0x20000) {
                *(float *)(this + 0x40) = -*in_r1;
              }
              else {
                if (param_2 == 0x40000) {
                  fVar11 = *in_r1;
                }
                else {
                  if (param_2 != 0x70000) goto LAB_000873b0;
                  *(float *)(this + 0x3c) = *in_r1;
                  *(float *)(this + 0x40) = -in_r1[1];
                  fVar11 = in_r1[2];
                }
                *(float *)(this + 0x44) = fVar11;
              }
              goto LAB_000873b0;
            }
            *(float *)(this + 0x30) = *in_r1;
            *(float *)(this + 0x34) = -in_r1[1];
            fVar11 = in_r1[2];
          }
          *(float *)(this + 0x38) = fVar11;
        }
      }
    }
  }
LAB_000873b0:
  InsertKeyFrame((Transform *)param_1,this,uVar16);
  UpdateKeyFrames((Transform *)param_1,this,uVar16);
LAB_000873c6:
  if (*piVar25 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008640c: push {r4,r5,r6,r7,lr}
  0008640e: add r7,sp,#0xc
  00086410: push {r7,r8,r9,r10,r11}
  00086414: vpush {d8}
  00086418: sub sp,#0x68
  0008641a: str r1,[sp,#0x28]
  0008641c: mov r11,r2
  0008641e: ldr r2,[0x00086788]
  00086420: add.w r5,r0,#0x120
  00086424: ldr r1,[0x0008678c]
  00086426: movs r4,#0x0
  00086428: add r2,pc
  0008642a: add r1,pc
  0008642c: ldr r6,[r2,#0x0]
  0008642e: ldr.w r9,[r1,#0x0]
  00086432: ldr r1,[r6,#0x0]
  00086434: ldr.w r2,[r9,#0x0]
  00086438: adds r1,#0x1
  0008643a: str r1,[r6,#0x0]
  0008643c: str r2,[sp,#0x64]
  0008643e: mov r6,r0
  00086440: ldr.w r1,[r6,#0xf8]!
  00086444: ldr.w r10,[r7,#0x8]
  00086448: ldr r2,[r6,#0x4]
  0008644a: subs.w r1,r1,r10
  0008644e: sbcs.w r1,r2,r10, asr #0x1f
  00086452: itt lt
  00086454: asr.lt.w r1,r10, asr #0x1f
  00086458: strd.lt r10,r1,[r6,#0x0]
  0008645c: ldr.w r6,[r0,#0x11c]
  00086460: add.w r1,r0,#0x11c
  00086464: str r1,[sp,#0x4c]
  00086466: b 0x0008647e
  00086468: ldr r1,[r5,#0x0]
  0008646a: ldr.w r1,[r1,r4,lsl #0x2]
  0008646e: ldrd r1,r2,[r1,#0x50]
  00086472: subs.w r1,r1,r10
  00086476: sbcs.w r1,r2,r10, asr #0x1f
  0008647a: bge 0x00086482
  0008647c: adds r4,#0x1
  0008647e: cmp r4,r6
  00086480: bcc 0x00086468
  00086482: str r5,[sp,#0x48]
  00086484: mov r8,r3
  00086486: str r0,[sp,#0x24]
  00086488: movs r0,#0x60
  0008648a: blx 0x0006eb24
  0008648e: str r0,[sp,#0x38]
  00086490: blx 0x0006f838
  00086494: ldr.w r12,[sp,#0x48]
  00086498: cbz r4,0x000864fc
  0008649a: ldr.w r0,[r12,#0x0]
  0008649e: ldr r2,[0x00086790]
  000864a0: add.w r0,r0,r4, lsl #0x2
  000864a4: ldr r6,[sp,#0x38]
  000864a6: add r2,pc
  000864a8: ldr.w r1,[r0,#-0x4]
  000864ac: ldr r5,[r2,#0x0]
  000864ae: mov r0,r6
  000864b0: blx r5
  000864b2: ldr r0,[sp,#0x48]
  000864b4: ldr r0,[r0,#0x0]
  000864b6: add.w r0,r0,r4, lsl #0x2
  000864ba: ldr.w r0,[r0,#-0x4]
  000864be: add.w r1,r0,#0xc
  000864c2: add.w r0,r6,#0xc
  000864c6: blx r5
  000864c8: ldr r0,[sp,#0x48]
  000864ca: ldr r0,[r0,#0x0]
  000864cc: add.w r0,r0,r4, lsl #0x2
  000864d0: ldr.w r0,[r0,#-0x4]
  000864d4: add.w r1,r0,#0x18
  000864d8: add.w r0,r6,#0x18
  000864dc: blx r5
  000864de: ldr.w r12,[sp,#0x48]
  000864e2: ldr.w r0,[r12,#0x0]
  000864e6: add.w r0,r0,r4, lsl #0x2
  000864ea: ldr.w r0,[r0,#-0x4]
  000864ee: adds r0,#0x50
  000864f0: vld1.64 {d16,d17},[r0]
  000864f4: add.w r0,r6,#0x50
  000864f8: vst1.64 {d16,d17},[r0]
  000864fc: subs.w r0,r11,#0x1
  00086500: sbc r1,r8,#0x0
  00086504: rsbs.w r3,r0,#0x7
  00086508: mov.w r2,#0x0
  0008650c: mov lr,r8
  0008650e: sbcs.w r1,r2,r1
  00086512: bcs 0x00086610
  00086514: eor r0,r11,#0x10
  00086518: orrs.w r0,r0,lr
  0008651c: beq.w 0x0008664c
  00086520: eor r0,r11,#0x20
  00086524: orrs.w r0,r0,lr
  00086528: beq.w 0x0008665a
  0008652c: eor r0,r11,#0x38
  00086530: orrs.w r0,r0,lr
  00086534: beq.w 0x00086668
  00086538: eor r0,r11,#0x40
  0008653c: orrs.w r0,r0,lr
  00086540: beq.w 0x000866b4
  00086544: eor r0,r11,#0x80
  00086548: orrs.w r0,r0,lr
  0008654c: beq.w 0x000866be
  00086550: eor r0,r11,#0x100
  00086554: orrs.w r0,r0,lr
  00086558: beq.w 0x000866c8
  0008655c: eor r0,r11,#0x1c0
  00086560: orrs.w r0,r0,lr
  00086564: beq.w 0x000866d2
  00086568: eor r0,r11,#0x200
  0008656c: orrs.w r0,r0,lr
  00086570: beq.w 0x000866e0
  00086574: eor r0,r11,#0x400
  00086578: orrs.w r0,r0,lr
  0008657c: beq.w 0x000866f6
  00086580: eor r0,r11,#0x800
  00086584: orrs.w r0,r0,lr
  00086588: beq.w 0x00086708
  0008658c: eor r0,r11,#0x1000
  00086590: orrs.w r0,r0,lr
  00086594: beq.w 0x00086712
  00086598: eor r0,r11,#0x1c00
  0008659c: orrs.w r0,r0,lr
  000865a0: beq.w 0x0008671c
  000865a4: eor r0,r11,#0x2000
  000865a8: orrs.w r0,r0,lr
  000865ac: beq.w 0x00086736
  000865b0: eor r0,r11,#0x4000
  000865b4: orrs.w r0,r0,lr
  000865b8: beq.w 0x00086740
  000865bc: eor r0,r11,#0x8000
  000865c0: orrs.w r0,r0,lr
  000865c4: beq.w 0x00086752
  000865c8: eor r0,r11,#0xe000
  000865cc: orrs.w r0,r0,lr
  000865d0: beq.w 0x0008675a
  000865d4: eor r0,r11,#0x10000
  000865d8: orrs.w r0,r0,lr
  000865dc: beq.w 0x00086774
  000865e0: eor r0,r11,#0x20000
  000865e4: orrs.w r0,r0,lr
  000865e8: beq.w 0x0008677e
  000865ec: eor r0,r11,#0x40000
  000865f0: orrs.w r0,r0,lr
  000865f4: beq.w 0x00086798
  000865f8: ldr r3,[sp,#0x38]
  000865fa: eor r0,r11,#0x70000
  000865fe: orrs.w r0,r0,lr
  00086602: bne.w 0x000867a0
  00086606: ldr r2,[sp,#0x28]
  00086608: add.w r4,r3,#0x3c
  0008660c: ldmia r2,{r0,r1,r2}
  0008660e: b 0x000866dc
  00086610: ldr r3,[sp,#0x38]
  00086612: tbb [pc,r0]
  0008664c: ldr r0,[sp,#0x28]
  0008664e: ldr r3,[sp,#0x38]
  00086650: vldr.32 s0,[r0]
  00086654: vstr.32 s0,[r3,#0x10]
  00086658: b 0x0008669c
  0008665a: ldr r0,[sp,#0x28]
  0008665c: ldr r3,[sp,#0x38]
  0008665e: vldr.32 s0,[r0]
  00086662: vstr.32 s0,[r3,#0x14]
  00086666: b 0x0008669c
  00086668: ldr r2,[sp,#0x28]
  0008666a: ldmia r2,{r0,r1,r2}
  0008666c: ldr r3,[sp,#0x38]
  0008666e: vmov s0,r0
  00086672: vmov s2,r1
  00086676: str r0,[r3,#0xc]
  00086678: vcmpe.f32 s0,s2
  0008667c: vmrs apsr,fpscr
  00086680: it gt
  00086682: vmov.gt.f32 s2,s0
  00086686: vmov s0,r2
  0008668a: str r1,[r3,#0x10]
  0008668c: vcmpe.f32 s2,s0
  00086690: vmrs apsr,fpscr
  00086694: it gt
  00086696: vmov.gt.f32 s0,s2
  0008669a: str r2,[r3,#0x14]
  0008669c: ldr r0,[sp,#0x24]
  0008669e: vldr.32 s2,[r0,#0xe4]
  000866a2: vcmpe.f32 s0,s2
  000866a6: vmrs apsr,fpscr
  000866aa: itt gt
  000866ac: ldr.gt r0,[sp,#0x24]
  000866ae: vstr.gt.32 s0,[r0,#0xe4]
  000866b2: b 0x000867a0
  000866b4: ldr r0,[sp,#0x28]
  000866b6: ldr r3,[sp,#0x38]
  000866b8: ldr r0,[r0,#0x0]
  000866ba: str r0,[r3,#0x18]
  000866bc: b 0x000867a0
  000866be: ldr r0,[sp,#0x28]
  000866c0: ldr r3,[sp,#0x38]
  000866c2: ldr r0,[r0,#0x0]
  000866c4: str r0,[r3,#0x1c]
  000866c6: b 0x000867a0
  000866c8: ldr r0,[sp,#0x28]
  000866ca: ldr r3,[sp,#0x38]
  000866cc: ldr r0,[r0,#0x0]
  000866ce: str r0,[r3,#0x20]
  000866d0: b 0x000867a0
  000866d2: ldr r2,[sp,#0x28]
  000866d4: ldmia r2,{r0,r1,r2}
  000866d6: ldr r3,[sp,#0x38]
  000866d8: add.w r4,r3,#0x18
  000866dc: stmia r4!,{r0,r1,r2}
  000866de: b 0x000867a0
  000866e0: ldr r0,[sp,#0x28]
  000866e2: vldr.32 s0,[pc,#0xb0]
  000866e6: ldr r3,[sp,#0x38]
  000866e8: vldr.32 s2,[r0]
  000866ec: vdiv.f32 s0,s2,s0
  000866f0: vstr.32 s0,[r3,#0x48]
  000866f4: b 0x000867a0
  000866f6: ldr r0,[sp,#0x28]
  000866f8: ldr r3,[sp,#0x38]
  000866fa: vldr.32 s0,[r0]
  000866fe: vneg.f32 s0,s0
  00086702: vstr.32 s0,[r3,#0x24]
  00086706: b 0x000867a0
  00086708: ldr r0,[sp,#0x28]
  0008670a: ldr r3,[sp,#0x38]
  0008670c: ldr r0,[r0,#0x0]
  0008670e: str r0,[r3,#0x28]
  00086710: b 0x000867a0
  00086712: ldr r0,[sp,#0x28]
  00086714: ldr r3,[sp,#0x38]
  00086716: ldr r0,[r0,#0x0]
  00086718: str r0,[r3,#0x2c]
  0008671a: b 0x000867a0
  0008671c: ldr r1,[sp,#0x28]
  0008671e: ldr r3,[sp,#0x38]
  00086720: vldr.32 s0,[r1]
  00086724: vneg.f32 s0,s0
  00086728: vstr.32 s0,[r3,#0x24]
  0008672c: ldrd r0,r1,[r1,#0x4]
  00086730: strd r0,r1,[r3,#0x28]
  00086734: b 0x000867a0
  00086736: ldr r0,[sp,#0x28]
  00086738: ldr r3,[sp,#0x38]
  0008673a: ldr r0,[r0,#0x0]
  0008673c: str r0,[r3,#0x30]
  0008673e: b 0x000867a0
  00086740: ldr r0,[sp,#0x28]
  00086742: ldr r3,[sp,#0x38]
  00086744: vldr.32 s0,[r0]
  00086748: vneg.f32 s0,s0
  0008674c: vstr.32 s0,[r3,#0x34]
  00086750: b 0x000867a0
  00086752: ldr r0,[sp,#0x28]
  00086754: ldr r3,[sp,#0x38]
  00086756: ldr r0,[r0,#0x0]
  00086758: b 0x00086770
  0008675a: ldr r1,[sp,#0x28]
  0008675c: ldr r3,[sp,#0x38]
  0008675e: ldr r0,[r1,#0x0]
  00086760: str r0,[r3,#0x30]
  00086762: vldr.32 s0,[r1,#0x4]
  00086766: vneg.f32 s0,s0
  0008676a: vstr.32 s0,[r3,#0x34]
  0008676e: ldr r0,[r1,#0x8]
  00086770: str r0,[r3,#0x38]
  00086772: b 0x000867a0
  00086774: ldr r0,[sp,#0x28]
  00086776: ldr r3,[sp,#0x38]
  00086778: ldr r0,[r0,#0x0]
  0008677a: str r0,[r3,#0x3c]
  0008677c: b 0x000867a0
  0008677e: ldr r0,[sp,#0x28]
  00086780: ldr r3,[sp,#0x38]
  00086782: ldr r0,[r0,#0x0]
  00086784: str r0,[r3,#0x40]
  00086786: b 0x000867a0
  00086798: ldr r0,[sp,#0x28]
  0008679a: ldr r3,[sp,#0x38]
  0008679c: ldr r0,[r0,#0x0]
  0008679e: str r0,[r3,#0x44]
  000867a0: mov r4,r3
  000867a2: asr.w r0,r10, asr #0x1f
  000867a6: str.w r10,[r4,#0x50]!
  000867aa: str r0,[sp,#0x34]
  000867ac: str r0,[r4,#0x4]
  000867ae: mov r2,r4
  000867b0: ldr r0,[r4,#0xc]
  000867b2: ldr.w r1,[r2,#0x8]!
  000867b6: orr.w r0,r0,lr
  000867ba: str r0,[r4,#0xc]
  000867bc: orr.w r0,r1,r11
  000867c0: str r0,[r2,#0x0]
  000867c2: ldr r0,[sp,#0x4c]
  000867c4: ldr.w r8,[r0,#0x0]
  000867c8: cmp.w r8,#0x0
  000867cc: beq.w 0x00086e34
  000867d0: add.w r1,r3,#0x3c
  000867d4: strd r4,r2,[sp,#0x4]
  000867d8: str.w r9,[sp,#0x10]
  000867dc: movs r5,#0x0
  000867de: str r1,[sp,#0xc]
  000867e0: add.w r1,r3,#0x30
  000867e4: str r1,[sp,#0x1c]
  000867e6: add.w r1,r3,#0x24
  000867ea: str r1,[sp,#0x18]
  000867ec: add.w r1,r3,#0x18
  000867f0: movs r2,#0x0
  000867f2: str r1,[sp,#0x14]
  000867f4: add.w r1,r3,#0xc
  000867f8: str r1,[sp,#0x20]
  000867fa: strd lr,r11,[sp,#0x2c]
  000867fe: str.w r10,[sp,#0x50]
  00086802: cmp r2,r8
  00086804: bcs.w 0x00086c88
  00086808: ldr.w r3,[r12,#0x0]
  0008680c: ldr r0,[r3,r5]
  0008680e: str r0,[sp,#0x54]
  00086810: ldrd r4,r1,[r0,#0x50]
  00086814: subs.w r6,r4,r10
  00086818: sbcs.w r6,r1,r10, asr #0x1f
  0008681c: bge.w 0x00086c88
  00086820: cmp r2,#0x0
  00086822: beq.w 0x00086c28
  00086826: ldr.w r9,[sp,#0x54]
  0008682a: mov r8,r5
  0008682c: ldr.w r6,[r9,#0x58]!
  00086830: ldr.w r10,[r9,#0x4]
  00086834: and.w r0,r6,r11
  00086838: and.w r5,r10,lr
  0008683c: orrs r0,r5
  0008683e: mov r5,r8
  00086840: bne.w 0x00086c28
  00086844: adds r0,r3,r5
  00086846: str r2,[sp,#0x44]
  00086848: ldr.w r2,[r0,#-0x4]
  0008684c: str r2,[sp,#0x3c]
  0008684e: ldrd r5,r11,[r2,#0x50]
  00086852: subs r0,r4,r5
  00086854: sbc.w r1,r1,r11
  00086858: blx 0x0006f7d8
  0008685c: str r0,[sp,#0x40]
  0008685e: ldr r0,[sp,#0x50]
  00086860: ldr r1,[sp,#0x34]
  00086862: subs r0,r0,r5
  00086864: sbc.w r1,r1,r11
  00086868: ldrd r4,r11,[sp,#0x2c]
  0008686c: blx 0x0006f7d8
  00086870: vmov s0,r0
  00086874: ldr r0,[sp,#0x40]
  00086876: orr.w r1,r6,r11
  0008687a: mov lr,r4
  0008687c: vmov s2,r0
  00086880: orr.w r0,r10,r4
  00086884: vdiv.f32 s0,s2,s0
  00086888: strd r1,r0,[r9,#0x0]
  0008688c: subs.w r0,r11,#0x1
  00086890: sbc r1,r4,#0x0
  00086894: rsbs.w r2,r0,#0x7
  00086898: mov.w r2,#0x0
  0008689c: sbcs.w r1,r2,r1
  000868a0: bcs.w 0x000869bc
  000868a4: ldr r1,[sp,#0x38]
  000868a6: eor r0,r11,#0x10
  000868aa: ldrd r2,r12,[sp,#0x44]
  000868ae: orrs.w r0,r0,lr
  000868b2: mov r5,r8
  000868b4: beq.w 0x00086a60
  000868b8: eor r0,r11,#0x20
  000868bc: orrs.w r0,r0,lr
  000868c0: beq.w 0x00086a7a
  000868c4: eor r0,r11,#0x38
  000868c8: orrs.w r0,r0,lr
  000868cc: beq.w 0x00086a94
  000868d0: eor r0,r11,#0x40
  000868d4: orrs.w r0,r0,lr
  000868d8: beq.w 0x00086ab4
  000868dc: eor r0,r11,#0x80
  000868e0: orrs.w r0,r0,lr
  000868e4: beq.w 0x00086ad0
  000868e8: eor r0,r11,#0x100
  000868ec: orrs.w r0,r0,lr
  000868f0: beq.w 0x00086aea
  000868f4: eor r0,r11,#0x1c0
  000868f8: orrs.w r0,r0,lr
  000868fc: beq.w 0x00086b04
  00086900: eor r0,r11,#0x200
  00086904: orrs.w r0,r0,lr
  00086908: beq.w 0x00086b24
  0008690c: eor r0,r11,#0x400
  00086910: orrs.w r0,r0,lr
  00086914: beq.w 0x00086b3e
  00086918: eor r0,r11,#0x800
  0008691c: orrs.w r0,r0,lr
  00086920: beq.w 0x00086b5a
  00086924: eor r0,r11,#0x1000
  00086928: orrs.w r0,r0,lr
  0008692c: beq.w 0x00086b74
  00086930: eor r0,r11,#0x1c00
  00086934: orrs.w r0,r0,lr
  00086938: beq.w 0x00086b8e
  0008693c: eor r0,r11,#0x2000
  00086940: orrs.w r0,r0,lr
  00086944: beq.w 0x00086bae
  00086948: eor r0,r11,#0x4000
  0008694c: orrs.w r0,r0,lr
  00086950: beq.w 0x00086bca
  00086954: eor r0,r11,#0x8000
  00086958: orrs.w r0,r0,lr
  0008695c: beq.w 0x00086be4
  00086960: eor r0,r11,#0xe000
  00086964: orrs.w r0,r0,lr
  00086968: beq.w 0x00086bfe
  0008696c: eor r0,r11,#0x10000
  00086970: orrs.w r0,r0,lr
  00086974: beq.w 0x00086c38
  00086978: eor r0,r11,#0x20000
  0008697c: orrs.w r0,r0,lr
  00086980: beq.w 0x00086c54
  00086984: eor r0,r11,#0x40000
  00086988: orrs.w r0,r0,lr
  0008698c: beq.w 0x00086c6e
  00086990: eor r0,r11,#0x70000
  00086994: orrs.w r0,r0,lr
  00086998: bne.w 0x00086c28
  0008699c: vmov r3,s0
  000869a0: ldr r0,[sp,#0x3c]
  000869a2: add r6,sp,#0x58
  000869a4: ldr r2,[sp,#0xc]
  000869a6: add.w r1,r0,#0x3c
  000869aa: mov r4,lr
  000869ac: mov r0,r6
  000869ae: blx 0x0006f808
  000869b2: ldr r0,[sp,#0x48]
  000869b4: ldr r0,[r0,#0x0]
  000869b6: ldr r0,[r0,r5]
  000869b8: adds r0,#0x3c
  000869ba: b 0x00086c1c
  000869bc: ldrd r2,r12,[sp,#0x44]
  000869c0: mov r5,r8
  000869c2: tbh [pc,r0]
  00086a60: ldr r0,[sp,#0x3c]
  00086a62: vldr.32 s4,[r1,#0x10]
  00086a66: vldr.32 s2,[r0,#0x10]
  00086a6a: ldr r0,[sp,#0x54]
  00086a6c: vsub.f32 s4,s4,s2
  00086a70: vmla.f32 s2,s0,s4
  00086a74: vstr.32 s2,[r0,#0x10]
  00086a78: b 0x00086c28
  00086a7a: ldr r0,[sp,#0x3c]
  00086a7c: vldr.32 s4,[r1,#0x14]
  00086a80: vldr.32 s2,[r0,#0x14]
  00086a84: ldr r0,[sp,#0x54]
  00086a86: vsub.f32 s4,s4,s2
  00086a8a: vmla.f32 s2,s0,s4
  00086a8e: vstr.32 s2,[r0,#0x14]
  00086a92: b 0x00086c28
  00086a94: vmov r3,s0
  00086a98: ldr r0,[sp,#0x3c]
  00086a9a: add r6,sp,#0x58
  00086a9c: ldr r2,[sp,#0x20]
  00086a9e: add.w r1,r0,#0xc
  00086aa2: mov r4,lr
  00086aa4: mov r0,r6
  00086aa6: blx 0x0006f808
  00086aaa: ldr r0,[sp,#0x48]
  00086aac: ldr r0,[r0,#0x0]
  00086aae: ldr r0,[r0,r5]
  00086ab0: adds r0,#0xc
  00086ab2: b 0x00086c1c
  00086ab4: ldr r0,[sp,#0x3c]
  00086ab6: vldr.32 s2,[r0,#0x18]
  00086aba: ldr r0,[sp,#0x14]
  00086abc: vldr.32 s4,[r0]
  00086ac0: ldr r0,[sp,#0x54]
  00086ac2: vsub.f32 s4,s4,s2
  00086ac6: vmla.f32 s2,s0,s4
  00086aca: vstr.32 s2,[r0,#0x18]
  00086ace: b 0x00086c28
  00086ad0: ldr r0,[sp,#0x3c]
  00086ad2: vldr.32 s4,[r1,#0x1c]
  00086ad6: vldr.32 s2,[r0,#0x1c]
  00086ada: ldr r0,[sp,#0x54]
  00086adc: vsub.f32 s4,s4,s2
  00086ae0: vmla.f32 s2,s0,s4
  00086ae4: vstr.32 s2,[r0,#0x1c]
  00086ae8: b 0x00086c28
  00086aea: ldr r0,[sp,#0x3c]
  00086aec: vldr.32 s4,[r1,#0x20]
  00086af0: vldr.32 s2,[r0,#0x20]
  00086af4: ldr r0,[sp,#0x54]
  00086af6: vsub.f32 s4,s4,s2
  00086afa: vmla.f32 s2,s0,s4
  00086afe: vstr.32 s2,[r0,#0x20]
  00086b02: b 0x00086c28
  00086b04: vmov r3,s0
  00086b08: ldr r0,[sp,#0x3c]
  00086b0a: add r6,sp,#0x58
  00086b0c: ldr r2,[sp,#0x14]
  00086b0e: add.w r1,r0,#0x18
  00086b12: mov r4,lr
  00086b14: mov r0,r6
  00086b16: blx 0x0006f808
  00086b1a: ldr r0,[sp,#0x48]
  00086b1c: ldr r0,[r0,#0x0]
  00086b1e: ldr r0,[r0,r5]
  00086b20: adds r0,#0x18
  00086b22: b 0x00086c1c
  00086b24: ldr r0,[sp,#0x3c]
  00086b26: vldr.32 s4,[r1,#0x48]
  00086b2a: vldr.32 s2,[r0,#0x48]
  00086b2e: ldr r0,[sp,#0x54]
  00086b30: vsub.f32 s4,s4,s2
  00086b34: vmla.f32 s2,s0,s4
  00086b38: vstr.32 s2,[r0,#0x48]
  00086b3c: b 0x00086c28
  00086b3e: ldr r0,[sp,#0x3c]
  00086b40: vldr.32 s2,[r0,#0x24]
  00086b44: ldr r0,[sp,#0x18]
  00086b46: vldr.32 s4,[r0]
  00086b4a: ldr r0,[sp,#0x54]
  00086b4c: vsub.f32 s4,s4,s2
  00086b50: vmla.f32 s2,s0,s4
  00086b54: vstr.32 s2,[r0,#0x24]
  00086b58: b 0x00086c28
  00086b5a: ldr r0,[sp,#0x3c]
  00086b5c: vldr.32 s4,[r1,#0x28]
  00086b60: vldr.32 s2,[r0,#0x28]
  00086b64: ldr r0,[sp,#0x54]
  00086b66: vsub.f32 s4,s4,s2
  00086b6a: vmla.f32 s2,s0,s4
  00086b6e: vstr.32 s2,[r0,#0x28]
  00086b72: b 0x00086c28
  00086b74: ldr r0,[sp,#0x3c]
  00086b76: vldr.32 s4,[r1,#0x2c]
  00086b7a: vldr.32 s2,[r0,#0x2c]
  00086b7e: ldr r0,[sp,#0x54]
  00086b80: vsub.f32 s4,s4,s2
  00086b84: vmla.f32 s2,s0,s4
  00086b88: vstr.32 s2,[r0,#0x2c]
  00086b8c: b 0x00086c28
  00086b8e: vmov r3,s0
  00086b92: ldr r0,[sp,#0x3c]
  00086b94: add r6,sp,#0x58
  00086b96: ldr r2,[sp,#0x18]
  00086b98: add.w r1,r0,#0x24
  00086b9c: mov r4,lr
  00086b9e: mov r0,r6
  00086ba0: blx 0x0006f808
  00086ba4: ldr r0,[sp,#0x48]
  00086ba6: ldr r0,[r0,#0x0]
  00086ba8: ldr r0,[r0,r5]
  00086baa: adds r0,#0x24
  00086bac: b 0x00086c1c
  00086bae: ldr r0,[sp,#0x3c]
  00086bb0: vldr.32 s2,[r0,#0x30]
  00086bb4: ldr r0,[sp,#0x1c]
  00086bb6: vldr.32 s4,[r0]
  00086bba: ldr r0,[sp,#0x54]
  00086bbc: vsub.f32 s4,s4,s2
  00086bc0: vmla.f32 s2,s0,s4
  00086bc4: vstr.32 s2,[r0,#0x30]
  00086bc8: b 0x00086c28
  00086bca: ldr r0,[sp,#0x3c]
  00086bcc: vldr.32 s4,[r1,#0x34]
  00086bd0: vldr.32 s2,[r0,#0x34]
  00086bd4: ldr r0,[sp,#0x54]
  00086bd6: vsub.f32 s4,s4,s2
  00086bda: vmla.f32 s2,s0,s4
  00086bde: vstr.32 s2,[r0,#0x34]
  00086be2: b 0x00086c28
  00086be4: ldr r0,[sp,#0x3c]
  00086be6: vldr.32 s4,[r1,#0x38]
  00086bea: vldr.32 s2,[r0,#0x38]
  00086bee: ldr r0,[sp,#0x54]
  00086bf0: vsub.f32 s4,s4,s2
  00086bf4: vmla.f32 s2,s0,s4
  00086bf8: vstr.32 s2,[r0,#0x38]
  00086bfc: b 0x00086c28
  00086bfe: vmov r3,s0
  00086c02: ldr r0,[sp,#0x3c]
  00086c04: add r6,sp,#0x58
  00086c06: ldr r2,[sp,#0x1c]
  00086c08: add.w r1,r0,#0x30
  00086c0c: mov r4,lr
  00086c0e: mov r0,r6
  00086c10: blx 0x0006f808
  00086c14: ldr r0,[sp,#0x48]
  00086c16: ldr r0,[r0,#0x0]
  00086c18: ldr r0,[r0,r5]
  00086c1a: adds r0,#0x30
  00086c1c: mov r1,r6
  00086c1e: blx 0x0006eb3c
  00086c22: mov lr,r4
  00086c24: ldrd r2,r12,[sp,#0x44]
  00086c28: ldr r0,[sp,#0x4c]
  00086c2a: adds r5,#0x4
  00086c2c: adds r2,#0x1
  00086c2e: ldr.w r10,[sp,#0x50]
  00086c32: ldr.w r8,[r0,#0x0]
  00086c36: b 0x00086802
  00086c38: ldr r0,[sp,#0x3c]
  00086c3a: vldr.32 s2,[r0,#0x3c]
  00086c3e: ldr r0,[sp,#0xc]
  00086c40: vldr.32 s4,[r0]
  00086c44: ldr r0,[sp,#0x54]
  00086c46: vsub.f32 s4,s4,s2
  00086c4a: vmla.f32 s2,s0,s4
  00086c4e: vstr.32 s2,[r0,#0x3c]
  00086c52: b 0x00086c28
  00086c54: ldr r0,[sp,#0x3c]
  00086c56: vldr.32 s4,[r1,#0x40]
  00086c5a: vldr.32 s2,[r0,#0x40]
  00086c5e: ldr r0,[sp,#0x54]
  00086c60: vsub.f32 s4,s4,s2
  00086c64: vmla.f32 s2,s0,s4
  00086c68: vstr.32 s2,[r0,#0x40]
  00086c6c: b 0x00086c28
  00086c6e: ldr r0,[sp,#0x3c]
  00086c70: vldr.32 s4,[r1,#0x44]
  00086c74: vldr.32 s2,[r0,#0x44]
  00086c78: ldr r0,[sp,#0x54]
  00086c7a: vsub.f32 s4,s4,s2
  00086c7e: vmla.f32 s2,s0,s4
  00086c82: vstr.32 s2,[r0,#0x44]
  00086c86: b 0x00086c28
  00086c88: cmp r2,r8
  00086c8a: bne.w 0x00086e5c
  00086c8e: ldr r5,[sp,#0x8]
  00086c90: ldrd r2,r1,[r5,#0x0]
  00086c94: tst r2,#0x40
  00086c98: bne 0x00086cac
  00086c9a: ldr.w r3,[r12,#0x0]
  00086c9e: ldr r4,[sp,#0x14]
  00086ca0: add.w r3,r3,r8, lsl #0x2
  00086ca4: ldr.w r3,[r3,#-0x4]
  00086ca8: ldr r3,[r3,#0x18]
  00086caa: str r3,[r4,#0x0]
  00086cac: ldr r0,[sp,#0x38]
  00086cae: lsls r3,r2,#0x18
  00086cb0: bmi 0x00086cc2
  00086cb2: ldr.w r3,[r12,#0x0]
  00086cb6: add.w r3,r3,r8, lsl #0x2
  00086cba: ldr.w r3,[r3,#-0x4]
  00086cbe: ldr r3,[r3,#0x1c]
  00086cc0: str r3,[r0,#0x1c]
  00086cc2: ldr r4,[sp,#0x10]
  00086cc4: lsls r3,r2,#0x17
  00086cc6: bmi 0x00086cd8
  00086cc8: ldr.w r3,[r12,#0x0]
  00086ccc: add.w r3,r3,r8, lsl #0x2
  00086cd0: ldr.w r3,[r3,#-0x4]
  00086cd4: ldr r3,[r3,#0x20]
  00086cd6: str r3,[r0,#0x20]
  00086cd8: lsls r3,r2,#0x1f
  00086cda: bne 0x00086cec
  00086cdc: ldr.w r3,[r12,#0x0]
  00086ce0: add.w r3,r3,r8, lsl #0x2
  00086ce4: ldr.w r3,[r3,#-0x4]
  00086ce8: ldr r3,[r3,#0x0]
  00086cea: str r3,[r0,#0x0]
  00086cec: lsls r3,r2,#0x1e
  00086cee: bmi 0x00086d00
  00086cf0: ldr.w r3,[r12,#0x0]
  00086cf4: add.w r3,r3,r8, lsl #0x2
  00086cf8: ldr.w r3,[r3,#-0x4]
  00086cfc: ldr r3,[r3,#0x4]
  00086cfe: str r3,[r0,#0x4]
  00086d00: lsls r3,r2,#0x1d
  00086d02: bmi 0x00086d14
  00086d04: ldr.w r3,[r12,#0x0]
  00086d08: add.w r3,r3,r8, lsl #0x2
  00086d0c: ldr.w r3,[r3,#-0x4]
  00086d10: ldr r3,[r3,#0x8]
  00086d12: str r3,[r0,#0x8]
  00086d14: lsls r3,r2,#0x1c
  00086d16: bmi 0x00086d2a
  00086d18: ldr.w r3,[r12,#0x0]
  00086d1c: ldr r6,[sp,#0x20]
  00086d1e: add.w r3,r3,r8, lsl #0x2
  00086d22: ldr.w r3,[r3,#-0x4]
  00086d26: ldr r3,[r3,#0xc]
  00086d28: str r3,[r6,#0x0]
  00086d2a: lsls r3,r2,#0x1b
  00086d2c: bmi 0x00086d3e
  00086d2e: ldr.w r3,[r12,#0x0]
  00086d32: add.w r3,r3,r8, lsl #0x2
  00086d36: ldr.w r3,[r3,#-0x4]
  00086d3a: ldr r3,[r3,#0x10]
  00086d3c: str r3,[r0,#0x10]
  00086d3e: lsls r3,r2,#0x1a
  00086d40: bmi 0x00086d52
  00086d42: ldr.w r3,[r12,#0x0]
  00086d46: add.w r3,r3,r8, lsl #0x2
  00086d4a: ldr.w r3,[r3,#-0x4]
  00086d4e: ldr r3,[r3,#0x14]
  00086d50: str r3,[r0,#0x14]
  00086d52: lsls r3,r2,#0x16
  00086d54: bmi 0x00086d66
  00086d56: ldr.w r3,[r12,#0x0]
  00086d5a: add.w r3,r3,r8, lsl #0x2
  00086d5e: ldr.w r3,[r3,#-0x4]
  00086d62: ldr r3,[r3,#0x48]
  00086d64: str r3,[r0,#0x48]
  00086d66: lsls r3,r2,#0xf
  00086d68: bmi 0x00086d7c
  00086d6a: ldr.w r3,[r12,#0x0]
  00086d6e: ldr r6,[sp,#0xc]
  00086d70: add.w r3,r3,r8, lsl #0x2
  00086d74: ldr.w r3,[r3,#-0x4]
  00086d78: ldr r3,[r3,#0x3c]
  00086d7a: str r3,[r6,#0x0]
  00086d7c: lsls r3,r2,#0xe
  00086d7e: bmi 0x00086d90
  00086d80: ldr.w r3,[r12,#0x0]
  00086d84: add.w r3,r3,r8, lsl #0x2
  00086d88: ldr.w r3,[r3,#-0x4]
  00086d8c: ldr r3,[r3,#0x40]
  00086d8e: str r3,[r0,#0x40]
  00086d90: lsls r3,r2,#0xd
  00086d92: bmi 0x00086da4
  00086d94: ldr.w r3,[r12,#0x0]
  00086d98: add.w r3,r3,r8, lsl #0x2
  00086d9c: ldr.w r3,[r3,#-0x4]
  00086da0: ldr r3,[r3,#0x44]
  00086da2: str r3,[r0,#0x44]
  00086da4: lsls r3,r2,#0x15
  00086da6: bmi 0x00086dba
  00086da8: ldr.w r3,[r12,#0x0]
  00086dac: ldr r6,[sp,#0x18]
  00086dae: add.w r3,r3,r8, lsl #0x2
  00086db2: ldr.w r3,[r3,#-0x4]
  00086db6: ldr r3,[r3,#0x24]
  00086db8: str r3,[r6,#0x0]
  00086dba: lsls r3,r2,#0x14
  00086dbc: bmi 0x00086dce
  00086dbe: ldr.w r3,[r12,#0x0]
  00086dc2: add.w r3,r3,r8, lsl #0x2
  00086dc6: ldr.w r3,[r3,#-0x4]
  00086dca: ldr r3,[r3,#0x28]
  00086dcc: str r3,[r0,#0x28]
  00086dce: lsls r3,r2,#0x13
  00086dd0: bmi 0x00086de2
  00086dd2: ldr.w r3,[r12,#0x0]
  00086dd6: add.w r3,r3,r8, lsl #0x2
  00086dda: ldr.w r3,[r3,#-0x4]
  00086dde: ldr r3,[r3,#0x2c]
  00086de0: str r3,[r0,#0x2c]
  00086de2: lsls r3,r2,#0x12
  00086de4: bmi 0x00086df8
  00086de6: ldr.w r3,[r12,#0x0]
  00086dea: ldr r6,[sp,#0x1c]
  00086dec: add.w r3,r3,r8, lsl #0x2
  00086df0: ldr.w r3,[r3,#-0x4]
  00086df4: ldr r3,[r3,#0x30]
  00086df6: str r3,[r6,#0x0]
  00086df8: lsls r3,r2,#0x11
  00086dfa: bmi 0x00086e0c
  00086dfc: ldr.w r3,[r12,#0x0]
  00086e00: add.w r3,r3,r8, lsl #0x2
  00086e04: ldr.w r3,[r3,#-0x4]
  00086e08: ldr r3,[r3,#0x34]
  00086e0a: str r3,[r0,#0x34]
  00086e0c: lsls r3,r2,#0x10
  00086e0e: bmi 0x00086e20
  00086e10: ldr.w r3,[r12,#0x0]
  00086e14: add.w r3,r3,r8, lsl #0x2
  00086e18: ldr.w r3,[r3,#-0x4]
  00086e1c: ldr r3,[r3,#0x38]
  00086e1e: str r3,[r0,#0x38]
  00086e20: orr.w r1,r1,lr
  00086e24: orr.w r2,r2,r11
  00086e28: strd r2,r1,[r5,#0x0]
  00086e2c: ldr r1,[sp,#0x4c]
  00086e2e: blx 0x0006f844
  00086e32: b 0x000873c6
  00086e34: ldr r0,[sp,#0x64]
  00086e36: ldr.w r1,[r9,#0x0]
  00086e3a: subs r0,r1,r0
  00086e3c: itttt eq
  00086e3e: ldr.eq r0,[sp,#0x38]
  00086e40: ldr.eq r1,[sp,#0x4c]
  00086e42: add.eq sp,#0x68
  00086e44: vpop.eq {d8}
  00086e48: itttt eq
  00086e4a: add.eq sp,#0x4
  00086e4c: pop.eq.w {r8,r9,r10,r11}
  00086e50: pop.eq.w {r4,r5,r6,r7,lr}
  00086e54: b.eq.w 0x001ab158
  00086e58: blx 0x0006e824
  00086e5c: ldr.w r0,[r12,#0x0]
  00086e60: mov r6,r5
  00086e62: ldr.w r9,[r0,r5]
  00086e66: ldrd r5,r4,[r9,#0x50]
  00086e6a: eor.w r1,r4,r10, asr #0x1f
  00086e6e: eor.w r3,r5,r10
  00086e72: orrs r1,r3
  00086e74: bne.w 0x00087028
  00086e78: ldr r3,[sp,#0x14]
  00086e7a: mov r0,r9
  00086e7c: ldr.w r1,[r0,#0x58]!
  00086e80: vldr.32 s0,[r3]
  00086e84: tst r1,#0x40
  00086e88: ldr r6,[r0,#0x4]
  00086e8a: itt ne
  00086e8c: vldr.ne.32 s2,[r9,#0x18]
  00086e90: vadd.ne.f32 s0,s0,s2
  00086e94: ldr r5,[sp,#0x38]
  00086e96: ldr r4,[sp,#0x10]
  00086e98: lsls r3,r1,#0x18
  00086e9a: vstr.32 s0,[r9,#0x18]
  00086e9e: orr.w r6,r6,lr
  00086ea2: vldr.32 s0,[r5,#0x1c]
  00086ea6: itt mi
  00086ea8: vldr.mi.32 s2,[r9,#0x1c]
  00086eac: vadd.mi.f32 s0,s0,s2
  00086eb0: lsls r3,r1,#0x17
  00086eb2: vstr.32 s0,[r9,#0x1c]
  00086eb6: vldr.32 s0,[r5,#0x20]
  00086eba: itt mi
  00086ebc: vldr.mi.32 s2,[r9,#0x20]
  00086ec0: vadd.mi.f32 s0,s0,s2
  00086ec4: lsls r3,r1,#0x1f
  00086ec6: vstr.32 s0,[r9,#0x20]
  00086eca: vldr.32 s0,[r5]
  00086ece: itt ne
  00086ed0: vldr.ne.32 s2,[r9]
  00086ed4: vadd.ne.f32 s0,s0,s2
  00086ed8: lsls r3,r1,#0x1e
  00086eda: vstr.32 s0,[r9]
  00086ede: vldr.32 s0,[r5,#0x4]
  00086ee2: itt mi
  00086ee4: vldr.mi.32 s2,[r9,#0x4]
  00086ee8: vadd.mi.f32 s0,s0,s2
  00086eec: lsls r3,r1,#0x1d
  00086eee: vstr.32 s0,[r9,#0x4]
  00086ef2: vldr.32 s0,[r5,#0x8]
  00086ef6: itt mi
  00086ef8: vldr.mi.32 s2,[r9,#0x8]
  00086efc: vadd.mi.f32 s0,s0,s2
  00086f00: ldr r3,[sp,#0x20]
  00086f02: vstr.32 s0,[r9,#0x8]
  00086f06: vldr.32 s0,[r3]
  00086f0a: lsls r3,r1,#0x1c
  00086f0c: itt mi
  00086f0e: vldr.mi.32 s2,[r9,#0xc]
  00086f12: vmul.mi.f32 s0,s0,s2
  00086f16: lsls r3,r1,#0x1b
  00086f18: vstr.32 s0,[r9,#0xc]
  00086f1c: vldr.32 s0,[r5,#0x10]
  00086f20: itt mi
  00086f22: vldr.mi.32 s2,[r9,#0x10]
  00086f26: vmul.mi.f32 s0,s0,s2
  00086f2a: lsls r3,r1,#0x1a
  00086f2c: vstr.32 s0,[r9,#0x10]
  00086f30: vldr.32 s0,[r5,#0x14]
  00086f34: itt mi
  00086f36: vldr.mi.32 s2,[r9,#0x14]
  00086f3a: vmul.mi.f32 s0,s0,s2
  00086f3e: vstr.32 s0,[r9,#0x14]
  00086f42: ldr r3,[r5,#0x48]
  00086f44: str.w r3,[r9,#0x48]
  00086f48: lsls r3,r1,#0xf
  00086f4a: vldr.32 s0,[r5,#0x3c]
  00086f4e: itt mi
  00086f50: vldr.mi.32 s2,[r9,#0x3c]
  00086f54: vadd.mi.f32 s0,s0,s2
  00086f58: lsls r3,r1,#0xe
  00086f5a: vstr.32 s0,[r9,#0x3c]
  00086f5e: vldr.32 s0,[r5,#0x40]
  00086f62: itt mi
  00086f64: vldr.mi.32 s2,[r9,#0x40]
  00086f68: vadd.mi.f32 s0,s0,s2
  00086f6c: lsls r3,r1,#0xd
  00086f6e: vstr.32 s0,[r9,#0x40]
  00086f72: vldr.32 s0,[r5,#0x44]
  00086f76: itt mi
  00086f78: vldr.mi.32 s2,[r9,#0x44]
  00086f7c: vadd.mi.f32 s0,s0,s2
  00086f80: ldr r3,[sp,#0x18]
  00086f82: vstr.32 s0,[r9,#0x44]
  00086f86: vldr.32 s0,[r3]
  00086f8a: lsls r3,r1,#0x15
  00086f8c: itt mi
  00086f8e: vldr.mi.32 s2,[r9,#0x24]
  00086f92: vadd.mi.f32 s0,s0,s2
  00086f96: lsls r3,r1,#0x14
  00086f98: vstr.32 s0,[r9,#0x24]
  00086f9c: vldr.32 s0,[r5,#0x28]
  00086fa0: itt mi
  00086fa2: vldr.mi.32 s2,[r9,#0x28]
  00086fa6: vadd.mi.f32 s0,s0,s2
  00086faa: lsls r3,r1,#0x13
  00086fac: vstr.32 s0,[r9,#0x28]
  00086fb0: vldr.32 s0,[r5,#0x2c]
  00086fb4: itt mi
  00086fb6: vldr.mi.32 s2,[r9,#0x2c]
  00086fba: vadd.mi.f32 s0,s0,s2
  00086fbe: ldr r3,[sp,#0x1c]
  00086fc0: vstr.32 s0,[r9,#0x2c]
  00086fc4: vldr.32 s0,[r3]
  00086fc8: lsls r3,r1,#0x12
  00086fca: itt mi
  00086fcc: vldr.mi.32 s2,[r9,#0x30]
  00086fd0: vmul.mi.f32 s0,s0,s2
  00086fd4: lsls r3,r1,#0x11
  00086fd6: vstr.32 s0,[r9,#0x30]
  00086fda: vldr.32 s0,[r5,#0x34]
  00086fde: itt mi
  00086fe0: vldr.mi.32 s2,[r9,#0x34]
  00086fe4: vmul.mi.f32 s0,s0,s2
  00086fe8: lsls r3,r1,#0x10
  00086fea: vstr.32 s0,[r9,#0x34]
  00086fee: vldr.32 s0,[r5,#0x38]
  00086ff2: itt mi
  00086ff4: vldr.mi.32 s2,[r9,#0x38]
  00086ff8: vmul.mi.f32 s0,s0,s2
  00086ffc: ldr r3,[sp,#0x30]
  00086ffe: vstr.32 s0,[r9,#0x38]
  00087002: orrs r1,r3
  00087004: strd r1,r6,[r0,#0x0]
  00087008: ldr r6,[sp,#0x8]
  0008700a: ldrd r0,r1,[r6,#0x0]
  0008700e: orr.w r1,r1,lr
  00087012: orrs r0,r3
  00087014: strd r0,r1,[r6,#0x0]
  00087018: mov r1,r5
  0008701a: ldr r0,[sp,#0x24]
  0008701c: blx 0x0006f85c
  00087020: mov r0,r5
  00087022: blx 0x0006eb48
  00087026: b 0x000873c6
  00087028: cmp r2,#0x0
  0008702a: beq.w 0x00087268
  0008702e: add r0,r6
  00087030: str r2,[sp,#0x44]
  00087032: mov r10,r6
  00087034: ldr.w r8,[r0,#-0x4]
  00087038: ldr r0,[sp,#0x4]
  0008703a: ldrd r6,r11,[r8,#0x50]
  0008703e: ldrd r0,r1,[r0,#0x0]
  00087042: subs r0,r0,r6
  00087044: sbc.w r1,r1,r11
  00087048: blx 0x0006f7d8
  0008704c: str r0,[sp,#0x54]
  0008704e: subs r0,r5,r6
  00087050: sbc.w r1,r4,r11
  00087054: blx 0x0006f7d8
  00087058: vmov s0,r0
  0008705c: ldr r0,[sp,#0x54]
  0008705e: add.w r1,r8,#0x18
  00087062: add.w r2,r9,#0x18
  00087066: add.w r8,sp,#0x58
  0008706a: vmov s2,r0
  0008706e: ldr r0,[0x000873f0]
  00087070: vdiv.f32 s16,s2,s0
  00087074: add r0,pc
  00087076: ldr.w r9,[r0,#0x0]
  0008707a: mov r0,r8
  0008707c: vmov r11,s16
  00087080: mov r3,r11
  00087082: blx r9
  00087084: ldr r0,[0x000873f4]
  00087086: mov r1,r8
  00087088: add r0,pc
  0008708a: ldr r4,[r0,#0x0]
  0008708c: ldr r0,[sp,#0x14]
  0008708e: blx r4
  00087090: ldr r5,[sp,#0x48]
  00087092: add.w r8,sp,#0x58
  00087096: mov r3,r11
  00087098: ldr r0,[r5,#0x0]
  0008709a: ldr.w r2,[r0,r10]
  0008709e: add r0,r10
  000870a0: ldr.w r1,[r0,#-0x4]
  000870a4: mov r0,r8
  000870a6: blx r9
  000870a8: ldr r6,[sp,#0x38]
  000870aa: mov r1,r8
  000870ac: mov r0,r6
  000870ae: blx r4
  000870b0: ldr r0,[r5,#0x0]
  000870b2: add.w r8,sp,#0x58
  000870b6: mov r3,r11
  000870b8: add.w r1,r0,r10
  000870bc: ldr.w r0,[r0,r10]
  000870c0: ldr.w r1,[r1,#-0x4]
  000870c4: add.w r2,r0,#0xc
  000870c8: adds r1,#0xc
  000870ca: mov r0,r8
  000870cc: blx r9
  000870ce: ldr r0,[sp,#0x20]
  000870d0: mov r1,r8
  000870d2: blx r4
  000870d4: ldr r0,[r5,#0x0]
  000870d6: add.w r8,sp,#0x58
  000870da: mov r3,r11
  000870dc: ldr.w r2,[r0,r10]
  000870e0: add r0,r10
  000870e2: ldr.w r0,[r0,#-0x4]
  000870e6: vldr.32 s2,[r2,#0x48]
  000870ea: add.w r1,r0,#0x3c
  000870ee: vldr.32 s0,[r0,#0x48]
  000870f2: adds r2,#0x3c
  000870f4: mov r0,r8
  000870f6: vsub.f32 s2,s2,s0
  000870fa: vmla.f32 s0,s16,s2
  000870fe: vstr.32 s0,[r6,#0x48]
  00087102: blx r9
  00087104: ldr r0,[sp,#0xc]
  00087106: mov r1,r8
  00087108: blx r4
  0008710a: ldr r0,[r5,#0x0]
  0008710c: add.w r8,sp,#0x58
  00087110: mov r3,r11
  00087112: ldr.w r2,[r0,r10]
  00087116: add r0,r10
  00087118: ldr.w r0,[r0,#-0x4]
  0008711c: adds r2,#0x24
  0008711e: add.w r1,r0,#0x24
  00087122: mov r0,r8
  00087124: blx r9
  00087126: ldr r0,[sp,#0x18]
  00087128: mov r1,r8
  0008712a: blx r4
  0008712c: ldr r0,[r5,#0x0]
  0008712e: add.w r8,sp,#0x58
  00087132: mov r3,r11
  00087134: ldr r6,[sp,#0x2c]
  00087136: ldr.w r2,[r0,r10]
  0008713a: add r0,r10
  0008713c: ldr.w r0,[r0,#-0x4]
  00087140: adds r2,#0x30
  00087142: add.w r1,r0,#0x30
  00087146: mov r0,r8
  00087148: blx r9
  0008714a: ldr r0,[sp,#0x1c]
  0008714c: mov r1,r8
  0008714e: blx r4
  00087150: ldr r0,[r5,#0x0]
  00087152: ldr r5,[sp,#0x8]
  00087154: add r0,r10
  00087156: ldrd r1,r2,[r5,#0x0]
  0008715a: ldr.w r0,[r0,#-0x4]
  0008715e: ldrd r0,r3,[r0,#0x58]
  00087162: orrs r2,r3
  00087164: orrs r0,r1
  00087166: strd r0,r2,[r5,#0x0]
  0008716a: ldr r5,[sp,#0x30]
  0008716c: movs r2,#0x0
  0008716e: subs r0,r5,#0x1
  00087170: sbc r1,r6,#0x0
  00087174: rsbs.w r3,r0,#0x7
  00087178: sbcs.w r1,r2,r1
  0008717c: bcs.w 0x00087284
  00087180: ldr r4,[sp,#0x10]
  00087182: eor r0,r5,#0x10
  00087186: ldr r2,[sp,#0x44]
  00087188: orrs r0,r6
  0008718a: beq.w 0x000872c6
  0008718e: ldr.w r8,[sp,#0x38]
  00087192: eor r0,r5,#0x20
  00087196: orrs r0,r6
  00087198: beq.w 0x000872d4
  0008719c: eor r0,r5,#0x38
  000871a0: orrs r0,r6
  000871a2: beq.w 0x000872de
  000871a6: eor r0,r5,#0x40
  000871aa: orrs r0,r6
  000871ac: beq.w 0x000872e6
  000871b0: eor r0,r5,#0x80
  000871b4: orrs r0,r6
  000871b6: beq.w 0x000872ec
  000871ba: eor r0,r5,#0x100
  000871be: orrs r0,r6
  000871c0: beq.w 0x000872f6
  000871c4: eor r0,r5,#0x1c0
  000871c8: orrs r0,r6
  000871ca: beq.w 0x00087300
  000871ce: eor r0,r5,#0x200
  000871d2: orrs r0,r6
  000871d4: beq.w 0x0008730e
  000871d8: eor r0,r5,#0x400
  000871dc: orrs r0,r6
  000871de: beq.w 0x00087318
  000871e2: eor r0,r5,#0x800
  000871e6: orrs r0,r6
  000871e8: beq.w 0x0008732a
  000871ec: eor r0,r5,#0x1000
  000871f0: orrs r0,r6
  000871f2: beq.w 0x00087334
  000871f6: eor r0,r5,#0x1c00
  000871fa: orrs r0,r6
  000871fc: beq.w 0x0008733e
  00087200: eor r0,r5,#0x2000
  00087204: orrs r0,r6
  00087206: beq.w 0x00087356
  0008720a: eor r0,r5,#0x4000
  0008720e: orrs r0,r6
  00087210: beq.w 0x0008735c
  00087214: eor r0,r5,#0x8000
  00087218: orrs r0,r6
  0008721a: beq.w 0x0008736c
  0008721e: eor r0,r5,#0xe000
  00087222: orrs r0,r6
  00087224: beq.w 0x00087372
  00087228: eor r0,r5,#0x10000
  0008722c: orrs r0,r6
  0008722e: beq.w 0x0008738e
  00087232: eor r0,r5,#0x20000
  00087236: orrs r0,r6
  00087238: beq.w 0x00087398
  0008723c: eor r0,r5,#0x40000
  00087240: orrs r0,r6
  00087242: beq.w 0x000873a8
  00087246: eor r0,r5,#0x70000
  0008724a: orrs r0,r6
  0008724c: bne.w 0x000873b0
  00087250: ldr r1,[sp,#0x28]
  00087252: ldr r0,[r1,#0x0]
  00087254: str.w r0,[r8,#0x3c]
  00087258: vldr.32 s0,[r1,#0x4]
  0008725c: vneg.f32 s0,s0
  00087260: vstr.32 s0,[r8,#0x40]
  00087264: ldr r0,[r1,#0x8]
  00087266: b 0x000873ac
  00087268: ldr r4,[sp,#0x24]
  0008726a: movs r2,#0x0
  0008726c: ldr r5,[sp,#0x38]
  0008726e: mov r0,r4
  00087270: mov r1,r5
  00087272: blx 0x0006f850
  00087276: mov r0,r4
  00087278: mov r1,r5
  0008727a: movs r2,#0x0
  0008727c: blx 0x0006f85c
  00087280: ldr r4,[sp,#0x10]
  00087282: b 0x000873c6
  00087284: ldr r4,[sp,#0x10]
  00087286: ldr.w r8,[sp,#0x38]
  0008728a: ldr r2,[sp,#0x44]
  0008728c: tbb [pc,r0]
  000872c6: ldr r0,[sp,#0x28]
  000872c8: ldr.w r8,[sp,#0x38]
  000872cc: ldr r0,[r0,#0x0]
  000872ce: str.w r0,[r8,#0x10]
  000872d2: b 0x000873b0
  000872d4: ldr r0,[sp,#0x28]
  000872d6: ldr r0,[r0,#0x0]
  000872d8: str.w r0,[r8,#0x14]
  000872dc: b 0x000873b0
  000872de: ldr r3,[sp,#0x28]
  000872e0: add.w r12,r8,#0xc
  000872e4: b 0x00087306
  000872e6: ldr r0,[sp,#0x28]
  000872e8: ldr r1,[sp,#0x14]
  000872ea: b 0x00087392
  000872ec: ldr r0,[sp,#0x28]
  000872ee: ldr r0,[r0,#0x0]
  000872f0: str.w r0,[r8,#0x1c]
  000872f4: b 0x000873b0
  000872f6: ldr r0,[sp,#0x28]
  000872f8: ldr r0,[r0,#0x0]
  000872fa: str.w r0,[r8,#0x20]
  000872fe: b 0x000873b0
  00087300: ldr r3,[sp,#0x28]
  00087302: add.w r12,r8,#0x18
  00087306: ldmia r3,{r0,r1,r3}
  00087308: stm.w r12!,{r0,r1,r3}
  0008730c: b 0x000873b0
  0008730e: ldr r0,[sp,#0x28]
  00087310: ldr r0,[r0,#0x0]
  00087312: str.w r0,[r8,#0x48]
  00087316: b 0x000873b0
  00087318: ldr r0,[sp,#0x28]
  0008731a: vldr.32 s0,[r0]
  0008731e: ldr r0,[sp,#0x18]
  00087320: vneg.f32 s0,s0
  00087324: vstr.32 s0,[r0]
  00087328: b 0x000873b0
  0008732a: ldr r0,[sp,#0x28]
  0008732c: ldr r0,[r0,#0x0]
  0008732e: str.w r0,[r8,#0x28]
  00087332: b 0x000873b0
  00087334: ldr r0,[sp,#0x28]
  00087336: ldr r0,[r0,#0x0]
  00087338: str.w r0,[r8,#0x2c]
  0008733c: b 0x000873b0
  0008733e: ldr r1,[sp,#0x28]
  00087340: vldr.32 s0,[r1]
  00087344: vneg.f32 s0,s0
  00087348: vstr.32 s0,[r8,#0x24]
  0008734c: ldrd r0,r1,[r1,#0x4]
  00087350: strd r0,r1,[r8,#0x28]
  00087354: b 0x000873b0
  00087356: ldr r0,[sp,#0x28]
  00087358: ldr r1,[sp,#0x1c]
  0008735a: b 0x00087392
  0008735c: ldr r0,[sp,#0x28]
  0008735e: vldr.32 s0,[r0]
  00087362: vneg.f32 s0,s0
  00087366: vstr.32 s0,[r8,#0x34]
  0008736a: b 0x000873b0
  0008736c: ldr r0,[sp,#0x28]
  0008736e: ldr r0,[r0,#0x0]
  00087370: b 0x00087388
  00087372: ldr r1,[sp,#0x28]
  00087374: ldr r0,[r1,#0x0]
  00087376: str.w r0,[r8,#0x30]
  0008737a: vldr.32 s0,[r1,#0x4]
  0008737e: vneg.f32 s0,s0
  00087382: vstr.32 s0,[r8,#0x34]
  00087386: ldr r0,[r1,#0x8]
  00087388: str.w r0,[r8,#0x38]
  0008738c: b 0x000873b0
  0008738e: ldr r0,[sp,#0x28]
  00087390: ldr r1,[sp,#0xc]
  00087392: ldr r0,[r0,#0x0]
  00087394: str r0,[r1,#0x0]
  00087396: b 0x000873b0
  00087398: ldr r0,[sp,#0x28]
  0008739a: vldr.32 s0,[r0]
  0008739e: vneg.f32 s0,s0
  000873a2: vstr.32 s0,[r8,#0x40]
  000873a6: b 0x000873b0
  000873a8: ldr r0,[sp,#0x28]
  000873aa: ldr r0,[r0,#0x0]
  000873ac: str.w r0,[r8,#0x44]
  000873b0: ldr r5,[sp,#0x24]
  000873b2: mov r1,r8
  000873b4: mov r6,r2
  000873b6: mov r0,r5
  000873b8: blx 0x0006f850
  000873bc: mov r0,r5
  000873be: mov r1,r8
  000873c0: mov r2,r6
  000873c2: blx 0x0006f85c
  000873c6: ldr r0,[sp,#0x64]
  000873c8: ldr r1,[r4,#0x0]
  000873ca: subs r0,r1,r0
  000873cc: itttt eq
  000873ce: add.eq sp,#0x68
  000873d0: vpop.eq {d8}
  000873d4: add.eq sp,#0x4
  000873d6: pop.eq.w {r8,r9,r10,r11}
  000873da: it eq
  000873dc: pop.eq {r4,r5,r6,r7,pc}
  000873de: blx 0x0006e824
  001ab158: bx pc
```
