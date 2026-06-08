# Mesh::ReadEnhancedDataFromFile
elf 0x6bbec body 1750
Sig: undefined __thiscall ReadEnhancedDataFromFile(Mesh * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::Mesh::ReadEnhancedDataFromFile(unsigned int, unsigned int) */

void __thiscall AbyssEngine::Mesh::ReadEnhancedDataFromFile(Mesh *this,uint param_1,uint param_2)

{
  float fVar1;
  float fVar2;
  Transform *this_00;
  int iVar3;
  uint uVar4;
  void *pvVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  Mesh *pMVar9;
  float *pfVar10;
  Mesh *pMVar11;
  uint uVar12;
  int *piVar13;
  bool bVar14;
  bool bVar15;
  bool bVar16;
  float fVar17;
  float fVar18;
  undefined8 in_d4;
  undefined8 in_d5;
  undefined8 in_d6;
  undefined4 in_s14;
  longlong lVar19;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined4 uStack_48;
  int local_44;
  
  piVar13 = *(int **)(DAT_0007bf90 + 0x7bc04);
  _uStack_48 = CONCAT44(*piVar13,in_s14);
  local_60 = in_d4;
  local_58 = in_d5;
  local_50 = in_d6;
  this_00 = operator_new(0x180);
  Transform::Transform(this_00);
  iVar3 = AEFile::Read(4,this + 0x3c,param_1);
  if (iVar3 != 0) {
    pMVar9 = this + 0x40;
    iVar3 = AEFile::Read(4,pMVar9,param_1);
    if (iVar3 != 0) {
      pMVar11 = this + 0x44;
      iVar3 = AEFile::Read(4,pMVar11,param_1);
      if ((iVar3 != 0) && (iVar3 = AEFile::Read(4,this + 0x48,param_1), iVar3 != 0)) {
        fVar17 = *(float *)pMVar9;
        *(undefined4 *)pMVar9 = *(undefined4 *)pMVar11;
        *(float *)pMVar11 = -fVar17;
        iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 6),param_1);
        if (iVar3 != 0) {
          if (local_58._6_2_ == 1) {
            iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
            if (iVar3 == 0) goto LAB_0007c0f6;
            local_60 = CONCAT44(local_60._4_4_,piVar13);
            pfVar10 = *(float **)(DAT_0007bf98 + 0x7bd7e);
            for (uVar12 = 0; uVar12 < local_58._4_2_; uVar12 = uVar12 + 1) {
              iVar3 = AEFile::Read(4,&local_58,param_1);
              if (iVar3 == 0) goto LAB_0007c0f2;
              bVar14 = (float)local_58 < 0.0;
              bVar15 = (float)local_58 == 0.0;
              bVar16 = NAN((float)local_58);
              if (!bVar15 && !bVar14) {
                fVar17 = *pfVar10;
                bVar14 = fVar17 < (float)local_58;
                bVar15 = fVar17 == (float)local_58;
                bVar16 = NAN(fVar17) || NAN((float)local_58);
              }
              if (!bVar15 && bVar14 == bVar16) {
                *pfVar10 = (float)local_58;
              }
              iVar3 = AEFile::Read(0xc,&local_50,param_1);
              if (iVar3 == 0) goto LAB_0007c0f2;
              Transform::InsertKeyFrame((float *)this_00,7,(int)(float)local_58);
            }
            piVar13 = (int *)local_60;
          }
          else if (local_58._6_2_ == 0) {
            local_60 = CONCAT44(local_60._4_4_,piVar13);
            pfVar10 = *(float **)(DAT_0007bf94 + 0x7bca4);
            for (uVar12 = 0; uVar12 < 3; uVar12 = uVar12 + 1) {
              iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
              if (iVar3 == 0) goto LAB_0007c0f6;
              for (uVar8 = 0; uVar8 < local_58._4_2_; uVar8 = uVar8 + 1) {
                iVar3 = AEFile::Read(4,&local_50,param_1);
                if (iVar3 == 0) goto LAB_0007c0f2;
                bVar14 = (float)local_50 < 0.0;
                bVar15 = (float)local_50 == 0.0;
                bVar16 = NAN((float)local_50);
                if (!bVar15 && !bVar14) {
                  fVar17 = *pfVar10;
                  bVar14 = fVar17 < (float)local_50;
                  bVar15 = fVar17 == (float)local_50;
                  bVar16 = NAN(fVar17) || NAN((float)local_50);
                }
                if (!bVar15 && bVar14 == bVar16) {
                  *pfVar10 = (float)local_50;
                }
                iVar3 = AEFile::Read(4,&local_58,param_1);
                if (iVar3 == 0) goto LAB_0007c0f2;
                uVar4 = uVar12 & 0x7fffffff;
                if (uVar4 == 2) {
                  uVar4 = 2;
LAB_0007bd3c:
                  Transform::InsertKeyFrame((float *)this_00,(ulonglong)uVar4,(int)(float)local_50);
                }
                else {
                  if (uVar4 == 1) {
                    uVar4 = 4;
                    local_58 = CONCAT44(local_58._4_4_,-(float)local_58);
                    goto LAB_0007bd3c;
                  }
                  if (uVar4 == 0) {
                    uVar4 = 1;
                    goto LAB_0007bd3c;
                  }
                }
              }
              piVar13 = (int *)local_60;
            }
          }
          iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 6),param_1);
          if (iVar3 != 0) {
            local_60 = CONCAT44(local_60._4_4_,piVar13);
            if (local_58._6_2_ == 1) {
              iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
              if (iVar3 == 0) goto LAB_0007c0f6;
              pfVar10 = *(float **)(DAT_0007bfa0 + 0x7bee2);
              for (uVar12 = 0; uVar12 < local_58._4_2_; uVar12 = uVar12 + 1) {
                iVar3 = AEFile::Read(4,&local_58,param_1);
                if (iVar3 == 0) goto LAB_0007c0f2;
                bVar14 = (float)local_58 < 0.0;
                bVar15 = (float)local_58 == 0.0;
                bVar16 = NAN((float)local_58);
                if (!bVar15 && !bVar14) {
                  fVar17 = *pfVar10;
                  bVar14 = fVar17 < (float)local_58;
                  bVar15 = fVar17 == (float)local_58;
                  bVar16 = NAN(fVar17) || NAN((float)local_58);
                }
                if (!bVar15 && bVar14 == bVar16) {
                  *pfVar10 = (float)local_58;
                }
                iVar3 = AEFile::Read(0xc,&local_50,param_1);
                if (iVar3 == 0) goto LAB_0007c0f2;
                Transform::InsertKeyFrame((float *)this_00,0x1c0,(int)(float)local_58);
              }
            }
            else if (local_58._6_2_ == 0) {
              pfVar10 = *(float **)(DAT_0007bf9c + 0x7be18);
              for (uVar12 = 0; uVar12 < 3; uVar12 = uVar12 + 1) {
                iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
                if (iVar3 == 0) goto LAB_0007c0f2;
                for (uVar8 = 0; uVar8 < local_58._4_2_; uVar8 = uVar8 + 1) {
                  iVar3 = AEFile::Read(4,&local_50,param_1);
                  if (iVar3 == 0) goto LAB_0007c0f2;
                  bVar14 = (float)local_50 < 0.0;
                  bVar15 = (float)local_50 == 0.0;
                  bVar16 = NAN((float)local_50);
                  if (!bVar15 && !bVar14) {
                    fVar17 = *pfVar10;
                    bVar14 = fVar17 < (float)local_50;
                    bVar15 = fVar17 == (float)local_50;
                    bVar16 = NAN(fVar17) || NAN((float)local_50);
                  }
                  if (!bVar15 && bVar14 == bVar16) {
                    *pfVar10 = (float)local_50;
                  }
                  iVar3 = AEFile::Read(4,&local_58,param_1);
                  if (iVar3 == 0) goto LAB_0007c0f2;
                  uVar4 = uVar12 & 0x7fffffff;
                  if (uVar4 == 2) {
                    uVar4 = 0x100;
LAB_0007be9e:
                    Transform::InsertKeyFrame
                              ((float *)this_00,(ulonglong)uVar4,(int)(float)local_50);
                  }
                  else {
                    if (uVar4 == 1) {
                      uVar4 = 0x80;
                      goto LAB_0007be9e;
                    }
                    if (uVar4 == 0) {
                      uVar4 = 0x40;
                      goto LAB_0007be9e;
                    }
                  }
                }
              }
            }
            iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 6),param_1);
            piVar13 = (int *)local_60;
            if (iVar3 != 0) {
              if (local_58._6_2_ == 1) {
                iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
                if (iVar3 == 0) goto LAB_0007c0f6;
                pfVar10 = *(float **)(DAT_0007c304 + 0x7c052);
                for (uVar12 = 0; uVar12 < local_58._4_2_; uVar12 = uVar12 + 1) {
                  iVar3 = AEFile::Read(4,&local_58,param_1);
                  if (iVar3 == 0) goto LAB_0007c0f2;
                  bVar14 = (float)local_58 < 0.0;
                  bVar15 = (float)local_58 == 0.0;
                  bVar16 = NAN((float)local_58);
                  if (!bVar15 && !bVar14) {
                    fVar17 = *pfVar10;
                    bVar14 = fVar17 < (float)local_58;
                    bVar15 = fVar17 == (float)local_58;
                    bVar16 = NAN(fVar17) || NAN((float)local_58);
                  }
                  if (!bVar15 && bVar14 == bVar16) {
                    *pfVar10 = (float)local_58;
                  }
                  iVar3 = AEFile::Read(0xc,&local_50,param_1);
                  if (iVar3 == 0) goto LAB_0007c0f2;
                  Transform::InsertKeyFrame((float *)this_00,0x38,(int)(float)local_58);
                }
              }
              else if (local_58._6_2_ == 0) {
                pfVar10 = *(float **)(DAT_0007c2f0 + 0x7bf78);
                for (uVar12 = 0; uVar12 < 3; uVar12 = uVar12 + 1) {
                  iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
                  if (iVar3 == 0) goto LAB_0007c0f2;
                  for (uVar8 = 0; uVar8 < local_58._4_2_; uVar8 = uVar8 + 1) {
                    iVar3 = AEFile::Read(4,&local_50,param_1);
                    if (iVar3 == 0) goto LAB_0007c0f2;
                    bVar14 = (float)local_50 < 0.0;
                    bVar15 = (float)local_50 == 0.0;
                    bVar16 = NAN((float)local_50);
                    if (!bVar15 && !bVar14) {
                      fVar17 = *pfVar10;
                      bVar14 = fVar17 < (float)local_50;
                      bVar15 = fVar17 == (float)local_50;
                      bVar16 = NAN(fVar17) || NAN((float)local_50);
                    }
                    if (!bVar15 && bVar14 == bVar16) {
                      *pfVar10 = (float)local_50;
                    }
                    iVar3 = AEFile::Read(4,&local_58,param_1);
                    if (iVar3 == 0) goto LAB_0007c0f2;
                    uVar4 = uVar12 & 0x7fffffff;
                    if (uVar4 == 2) {
                      uVar4 = 0x20;
LAB_0007c012:
                      Transform::InsertKeyFrame
                                ((float *)this_00,(ulonglong)uVar4,(int)(float)local_50);
                    }
                    else {
                      if (uVar4 == 1) {
                        uVar4 = 0x10;
                        goto LAB_0007c012;
                      }
                      if (uVar4 == 0) {
                        uVar4 = 8;
                        goto LAB_0007c012;
                      }
                    }
                  }
                }
              }
              piVar13 = (int *)local_60;
              if ((param_2 & 0x18) != 0) {
                iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 6),param_1);
                if (iVar3 == 0) goto LAB_0007c0f6;
                if (local_58._6_2_ == 2) {
                  iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
                  if (iVar3 == 0) goto LAB_0007c0f6;
                  pfVar10 = *(float **)(DAT_0007c308 + 0x7c0ee);
                  for (uVar12 = 0; uVar12 < local_58._4_2_; uVar12 = uVar12 + 1) {
                    iVar3 = AEFile::Read(4,&local_50,param_1);
                    if (iVar3 == 0) goto LAB_0007c0f6;
                    bVar14 = (float)local_50 < 0.0;
                    bVar15 = (float)local_50 == 0.0;
                    bVar16 = NAN((float)local_50);
                    if (!bVar15 && !bVar14) {
                      fVar17 = *pfVar10;
                      bVar14 = fVar17 < (float)local_50;
                      bVar15 = fVar17 == (float)local_50;
                      bVar16 = NAN(fVar17) || NAN((float)local_50);
                    }
                    if (!bVar15 && bVar14 == bVar16) {
                      *pfVar10 = (float)local_50;
                    }
                    iVar3 = AEFile::Read(4,&local_58,param_1);
                    if (iVar3 == 0) goto LAB_0007c0f6;
                    Transform::InsertKeyFrame((float *)this_00,0x200,(int)(float)local_50);
                  }
                }
              }
              if ((param_2 & 0x10) != 0) {
                iVar3 = AEFile::Read(2,(void *)((int)&local_58 + 4),param_1);
                fVar2 = DAT_0007c2fc;
                fVar1 = DAT_0007c2f8;
                fVar17 = DAT_0007c2f4;
                if (iVar3 == 0) goto LAB_0007c0f6;
                if (local_58._4_2_ != 0) {
                  pfVar10 = *(float **)(DAT_0007c30c + 0x7c1ec);
                  for (uVar12 = 0; uVar12 < 7; uVar12 = uVar12 + 1) {
                    iVar3 = AEFile::Read(2,(void *)((int)&local_60 + 6),param_1);
                    if (iVar3 == 0) goto LAB_0007c0f6;
                    for (iVar3 = 0; iVar3 < local_60._6_2_; iVar3 = iVar3 + 1) {
                      iVar7 = AEFile::Read(4,&local_50,param_1);
                      if (iVar7 == 0) goto LAB_0007c0f6;
                      bVar14 = (float)local_50 < 0.0;
                      bVar15 = (float)local_50 == 0.0;
                      bVar16 = NAN((float)local_50);
                      if (!bVar15 && !bVar14) {
                        fVar18 = *pfVar10;
                        bVar14 = fVar18 < (float)local_50;
                        bVar15 = fVar18 == (float)local_50;
                        bVar16 = NAN(fVar18) || NAN((float)local_50);
                      }
                      if (!bVar15 && bVar14 == bVar16) {
                        *pfVar10 = (float)local_50;
                      }
                      iVar7 = AEFile::Read(4,&local_58,param_1);
                      if (iVar7 == 0) goto LAB_0007c0f6;
                      fVar18 = (float)local_58 / fVar17;
                      local_58 = CONCAT44(local_58._4_4_,fVar18);
                      uVar8 = 0x400;
                      switch(uVar12 & 0x7fffffff) {
                      case 0:
                        break;
                      case 1:
                        uVar8 = 0x800;
                        break;
                      case 2:
                        uVar8 = 0x2000;
                        break;
                      case 3:
                        uVar8 = 0x4000;
                        break;
                      default:
                        uVar8 = 0;
                        break;
                      case 6:
                        uVar8 = 0x40000;
                        local_58 = CONCAT44(local_58._4_4_,(fVar18 * fVar1) / fVar2);
                      }
                      Transform::InsertKeyFrame
                                ((float *)this_00,(ulonglong)uVar8,(int)(float)local_50);
                      this[0x85] = (Mesh)0x1;
                    }
                  }
                }
              }
              iVar3 = DAT_0007c310;
              if (*(int *)(this_00 + 0x11c) < 1) {
                pvVar5 = (void *)Transform::~Transform(this_00);
                operator_delete(pvVar5);
              }
              else {
                *(Transform **)(this + 0x34) = this_00;
                fVar17 = **(float **)(iVar3 + 0x7c19c);
                *(int *)(this_00 + 0xe8) = (int)fVar17;
                lVar19 = __aeabi_f2lz(fVar17);
                Transform::SetAnimationRangeInTime
                          (CONCAT44((int)((ulonglong)lVar19 >> 0x20),this_00),lVar19);
              }
              uVar6 = 1;
              goto LAB_0007c104;
            }
          }
        }
      }
    }
  }
  goto LAB_0007c0f6;
LAB_0007c0f2:
  piVar13 = (int *)local_60;
LAB_0007c0f6:
  pvVar5 = (void *)Transform::~Transform(this_00);
  operator_delete(pvVar5);
  uVar6 = 0xffffffff;
LAB_0007c104:
  if (*piVar13 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

```

## target disasm
```
  0007bbec: push {r4,r5,r6,r7,lr}
  0007bbee: add r7,sp,#0xc
  0007bbf0: push {r7,r8,r9,r10,r11}
  0007bbf4: vpush {d2,d3,d4,d5,d6,d7,d8,d9,d10}
  0007bbf8: mov r9,r0
  0007bbfa: ldr r0,[0x0007bf90]
  0007bbfc: mov r4,r2
  0007bbfe: mov r10,r1
  0007bc00: add r0,pc
  0007bc02: ldr.w r8,[r0,#0x0]
  0007bc06: ldr.w r0,[r8,#0x0]
  0007bc0a: str r0,[sp,#0x2c]
  0007bc0c: mov.w r0,#0x180
  0007bc10: blx 0x0006eb24
  0007bc14: mov r11,r0
  0007bc16: blx 0x0006eb84
  0007bc1a: add.w r1,r9,#0x3c
  0007bc1e: movs r0,#0x4
  0007bc20: mov r2,r10
  0007bc22: blx 0x0006eb90
  0007bc26: cmp r0,#0x0
  0007bc28: beq.w 0x0007c0f6
  0007bc2c: add.w r5,r9,#0x40
  0007bc30: movs r0,#0x4
  0007bc32: mov r2,r10
  0007bc34: mov r1,r5
  0007bc36: blx 0x0006eb90
  0007bc3a: cmp r0,#0x0
  0007bc3c: beq.w 0x0007c0f6
  0007bc40: add.w r6,r9,#0x44
  0007bc44: movs r0,#0x4
  0007bc46: mov r2,r10
  0007bc48: mov r1,r6
  0007bc4a: blx 0x0006eb90
  0007bc4e: cmp r0,#0x0
  0007bc50: beq.w 0x0007c0f6
  0007bc54: add.w r1,r9,#0x48
  0007bc58: movs r0,#0x4
  0007bc5a: mov r2,r10
  0007bc5c: blx 0x0006eb90
  0007bc60: cmp r0,#0x0
  0007bc62: beq.w 0x0007c0f6
  0007bc66: vldr.32 s0,[r5]
  0007bc6a: sub.w r1,r7,#0x4a
  0007bc6e: ldr r0,[r6,#0x0]
  0007bc70: mov r2,r10
  0007bc72: vneg.f32 s0,s0
  0007bc76: str r0,[r5,#0x0]
  0007bc78: movs r0,#0x2
  0007bc7a: vstr.32 s0,[r6]
  0007bc7e: blx 0x0006eb90
  0007bc82: cmp r0,#0x0
  0007bc84: beq.w 0x0007c0f6
  0007bc88: ldrsh.w r0,[r7,#-0x4a]
  0007bc8c: cmp r0,#0x1
  0007bc8e: beq 0x0007bd60
  0007bc90: cmp r0,#0x0
  0007bc92: bne.w 0x0007bde6
  0007bc96: ldr r0,[0x0007bf94]
  0007bc98: add r1,sp,#0x1c
  0007bc9a: movs r6,#0x0
  0007bc9c: str.w r8,[sp,#0x10]
  0007bca0: add r0,pc
  0007bca2: ldr r5,[r0,#0x0]
  0007bca4: b 0x0007bd5a
  0007bca6: movs r0,#0x2
  0007bca8: mov r2,r10
  0007bcaa: blx 0x0006eb90
  0007bcae: cmp r0,#0x0
  0007bcb0: beq.w 0x0007c0f6
  0007bcb4: mov.w r8,#0x0
  0007bcb8: b 0x0007bd4a
  0007bcba: add r1,sp,#0x20
  0007bcbc: movs r0,#0x4
  0007bcbe: mov r2,r10
  0007bcc0: blx 0x0006eb90
  0007bcc4: cmp r0,#0x0
  0007bcc6: beq.w 0x0007c0f2
  0007bcca: vldr.32 s0,[sp,#0x20]
  0007bcce: add r1,sp,#0x18
  0007bcd0: movs r0,#0x4
  0007bcd2: mov r2,r10
  0007bcd4: vcmpe.f32 s0,#0
  0007bcd8: vmrs apsr,fpscr
  0007bcdc: itttt gt
  0007bcde: vldr.gt.32 s2,[r5]
  0007bce2: vcmpe.gt.f32 s2,s0
  0007bce6: vmrs.gt apsr,fpscr
  0007bcea: vstr.gt.32 s0,[r5]
  0007bcee: blx 0x0006eb90
  0007bcf2: cmp r0,#0x0
  0007bcf4: beq.w 0x0007c0f2
  0007bcf8: bic r0,r6,#0x80000000
  0007bcfc: cmp r0,#0x2
  0007bcfe: beq 0x0007bd12
  0007bd00: cmp r0,#0x1
  0007bd02: beq 0x0007bd22
  0007bd04: cbnz r0,0x0007bd46
  0007bd06: vldr.32 s0,[sp,#0x20]
  0007bd0a: add r1,sp,#0x18
  0007bd0c: mov r0,r11
  0007bd0e: movs r2,#0x1
  0007bd10: b 0x0007bd1c
  0007bd12: vldr.32 s0,[sp,#0x20]
  0007bd16: mov r0,r11
  0007bd18: add r1,sp,#0x18
  0007bd1a: movs r2,#0x2
  0007bd1c: vcvt.s32.f32 s0,s0
  0007bd20: b 0x0007bd3c
  0007bd22: vldr.32 s0,[sp,#0x18]
  0007bd26: mov r0,r11
  0007bd28: vldr.32 s2,[sp,#0x20]
  0007bd2c: add r1,sp,#0x18
  0007bd2e: vneg.f32 s0,s0
  0007bd32: movs r2,#0x4
  0007bd34: vstr.32 s0,[sp,#0x18]
  0007bd38: vcvt.s32.f32 s0,s2
  0007bd3c: vstr.32 s0,[sp]
  0007bd40: movs r3,#0x0
  0007bd42: blx 0x0006eb9c
  0007bd46: add.w r8,r8,#0x1
  0007bd4a: ldrh.w r0,[sp,#0x1c]
  0007bd4e: cmp r8,r0
  0007bd50: bcc 0x0007bcba
  0007bd52: adds r6,#0x1
  0007bd54: ldr.w r8,[sp,#0x10]
  0007bd58: add r1,sp,#0x1c
  0007bd5a: cmp r6,#0x2
  0007bd5c: bls 0x0007bca6
  0007bd5e: b 0x0007bde6
  0007bd60: add r1,sp,#0x1c
  0007bd62: movs r0,#0x2
  0007bd64: mov r2,r10
  0007bd66: blx 0x0006eb90
  0007bd6a: cmp r0,#0x0
  0007bd6c: beq.w 0x0007c0f6
  0007bd70: str.w r8,[sp,#0x10]
  0007bd74: add r6,sp,#0x20
  0007bd76: ldr r0,[0x0007bf98]
  0007bd78: movs r5,#0x0
  0007bd7a: add r0,pc
  0007bd7c: ldr.w r8,[r0,#0x0]
  0007bd80: b 0x0007bdda
  0007bd82: add r1,sp,#0x18
  0007bd84: movs r0,#0x4
  0007bd86: mov r2,r10
  0007bd88: blx 0x0006eb90
  0007bd8c: cmp r0,#0x0
  0007bd8e: beq.w 0x0007c0f2
  0007bd92: vldr.32 s0,[sp,#0x18]
  0007bd96: movs r0,#0xc
  0007bd98: mov r1,r6
  0007bd9a: mov r2,r10
  0007bd9c: vcmpe.f32 s0,#0
  0007bda0: vmrs apsr,fpscr
  0007bda4: itttt gt
  0007bda6: vldr.gt.32 s2,[r8]
  0007bdaa: vcmpe.gt.f32 s2,s0
  0007bdae: vmrs.gt apsr,fpscr
  0007bdb2: vstr.gt.32 s0,[r8]
  0007bdb6: blx 0x0006eb90
  0007bdba: cmp r0,#0x0
  0007bdbc: beq.w 0x0007c0f2
  0007bdc0: vldr.32 s0,[sp,#0x18]
  0007bdc4: mov r0,r11
  0007bdc6: mov r1,r6
  0007bdc8: movs r2,#0x7
  0007bdca: vcvt.s32.f32 s0,s0
  0007bdce: movs r3,#0x0
  0007bdd0: vstr.32 s0,[sp]
  0007bdd4: blx 0x0006eb9c
  0007bdd8: adds r5,#0x1
  0007bdda: ldrh.w r0,[sp,#0x1c]
  0007bdde: cmp r5,r0
  0007bde0: bcc 0x0007bd82
  0007bde2: ldr.w r8,[sp,#0x10]
  0007bde6: sub.w r1,r7,#0x4a
  0007bdea: movs r0,#0x2
  0007bdec: mov r2,r10
  0007bdee: blx 0x0006eb90
  0007bdf2: cmp r0,#0x0
  0007bdf4: beq.w 0x0007c0f6
  0007bdf8: ldrsh.w r0,[r7,#-0x4a]
  0007bdfc: str.w r8,[sp,#0x10]
  0007be00: cmp r0,#0x1
  0007be02: beq 0x0007bec4
  0007be04: cmp r0,#0x0
  0007be06: str r4,[sp,#0xc]
  0007be08: bne.w 0x0007bf48
  0007be0c: ldr r0,[0x0007bf9c]
  0007be0e: add r1,sp,#0x1c
  0007be10: mov.w r8,#0x0
  0007be14: add r0,pc
  0007be16: ldr r5,[r0,#0x0]
  0007be18: b 0x0007bebc
  0007be1a: movs r0,#0x2
  0007be1c: mov r2,r10
  0007be1e: mov r4,r1
  0007be20: blx 0x0006eb90
  0007be24: cmp r0,#0x0
  0007be26: beq.w 0x0007c0f2
  0007be2a: movs r6,#0x0
  0007be2c: b 0x0007beae
  0007be2e: add r1,sp,#0x20
  0007be30: movs r0,#0x4
  0007be32: mov r2,r10
  0007be34: blx 0x0006eb90
  0007be38: cmp r0,#0x0
  0007be3a: beq.w 0x0007c0f2
  0007be3e: vldr.32 s0,[sp,#0x20]
  0007be42: add r1,sp,#0x18
  0007be44: movs r0,#0x4
  0007be46: mov r2,r10
  0007be48: vcmpe.f32 s0,#0
  0007be4c: vmrs apsr,fpscr
  0007be50: itttt gt
  0007be52: vldr.gt.32 s2,[r5]
  0007be56: vcmpe.gt.f32 s2,s0
  0007be5a: vmrs.gt apsr,fpscr
  0007be5e: vstr.gt.32 s0,[r5]
  0007be62: blx 0x0006eb90
  0007be66: cmp r0,#0x0
  0007be68: beq.w 0x0007c0f2
  0007be6c: bic r0,r8,#0x80000000
  0007be70: cmp r0,#0x2
  0007be72: beq 0x0007be86
  0007be74: cmp r0,#0x1
  0007be76: beq 0x0007be94
  0007be78: cbnz r0,0x0007beac
  0007be7a: vldr.32 s0,[sp,#0x20]
  0007be7e: add r1,sp,#0x18
  0007be80: mov r0,r11
  0007be82: movs r2,#0x40
  0007be84: b 0x0007be9e
  0007be86: vldr.32 s0,[sp,#0x20]
  0007be8a: mov r0,r11
  0007be8c: add r1,sp,#0x18
  0007be8e: mov.w r2,#0x100
  0007be92: b 0x0007be9e
  0007be94: vldr.32 s0,[sp,#0x20]
  0007be98: mov r0,r11
  0007be9a: add r1,sp,#0x18
  0007be9c: movs r2,#0x80
  0007be9e: vcvt.s32.f32 s0,s0
  0007bea2: vstr.32 s0,[sp]
  0007bea6: movs r3,#0x0
  0007bea8: blx 0x0006eb9c
  0007beac: adds r6,#0x1
  0007beae: ldrh.w r0,[sp,#0x1c]
  0007beb2: cmp r6,r0
  0007beb4: bcc 0x0007be2e
  0007beb6: add.w r8,r8,#0x1
  0007beba: mov r1,r4
  0007bebc: cmp.w r8,#0x2
  0007bec0: bls 0x0007be1a
  0007bec2: b 0x0007bf48
  0007bec4: add r1,sp,#0x1c
  0007bec6: movs r0,#0x2
  0007bec8: mov r2,r10
  0007beca: blx 0x0006eb90
  0007bece: cmp r0,#0x0
  0007bed0: beq.w 0x0007c0f6
  0007bed4: str r4,[sp,#0xc]
  0007bed6: add r4,sp,#0x18
  0007bed8: ldr r0,[0x0007bfa0]
  0007beda: add r6,sp,#0x20
  0007bedc: movs r5,#0x0
  0007bede: add r0,pc
  0007bee0: ldr.w r8,[r0,#0x0]
  0007bee4: b 0x0007bf40
  0007bee6: movs r0,#0x4
  0007bee8: mov r1,r4
  0007beea: mov r2,r10
  0007beec: blx 0x0006eb90
  0007bef0: cmp r0,#0x0
  0007bef2: beq.w 0x0007c0f2
  0007bef6: vldr.32 s0,[sp,#0x18]
  0007befa: movs r0,#0xc
  0007befc: mov r1,r6
  0007befe: mov r2,r10
  0007bf00: vcmpe.f32 s0,#0
  0007bf04: vmrs apsr,fpscr
  0007bf08: itttt gt
  0007bf0a: vldr.gt.32 s2,[r8]
  0007bf0e: vcmpe.gt.f32 s2,s0
  0007bf12: vmrs.gt apsr,fpscr
  0007bf16: vstr.gt.32 s0,[r8]
  0007bf1a: blx 0x0006eb90
  0007bf1e: cmp r0,#0x0
  0007bf20: beq.w 0x0007c0f2
  0007bf24: vldr.32 s0,[sp,#0x18]
  0007bf28: mov r0,r11
  0007bf2a: mov r1,r6
  0007bf2c: mov.w r2,#0x1c0
  0007bf30: vcvt.s32.f32 s0,s0
  0007bf34: movs r3,#0x0
  0007bf36: vstr.32 s0,[sp]
  0007bf3a: blx 0x0006eb9c
  0007bf3e: adds r5,#0x1
  0007bf40: ldrh.w r0,[sp,#0x1c]
  0007bf44: cmp r5,r0
  0007bf46: bcc 0x0007bee6
  0007bf48: sub.w r1,r7,#0x4a
  0007bf4c: movs r0,#0x2
  0007bf4e: mov r2,r10
  0007bf50: blx 0x0006eb90
  0007bf54: ldr.w r8,[sp,#0x10]
  0007bf58: cmp r0,#0x0
  0007bf5a: beq.w 0x0007c0f6
  0007bf5e: ldrsh.w r0,[r7,#-0x4a]
  0007bf62: cmp r0,#0x1
  0007bf64: beq 0x0007c038
  0007bf66: cmp r0,#0x0
  0007bf68: bne.w 0x0007c0b0
  0007bf6c: ldr r0,[0x0007c2f0]
  0007bf6e: add r1,sp,#0x1c
  0007bf70: mov.w r8,#0x0
  0007bf74: add r0,pc
  0007bf76: ldr r5,[r0,#0x0]
  0007bf78: b 0x0007c030
  0007bf7a: movs r0,#0x2
  0007bf7c: mov r2,r10
  0007bf7e: mov r4,r1
  0007bf80: blx 0x0006eb90
  0007bf84: cmp r0,#0x0
  0007bf86: beq.w 0x0007c0f2
  0007bf8a: movs r6,#0x0
  0007bf8c: b 0x0007c022
  0007bfa4: add r1,sp,#0x20
  0007bfa6: movs r0,#0x4
  0007bfa8: mov r2,r10
  0007bfaa: blx 0x0006eb90
  0007bfae: cmp r0,#0x0
  0007bfb0: beq.w 0x0007c0f2
  0007bfb4: vldr.32 s0,[sp,#0x20]
  0007bfb8: add r1,sp,#0x18
  0007bfba: movs r0,#0x4
  0007bfbc: mov r2,r10
  0007bfbe: vcmpe.f32 s0,#0
  0007bfc2: vmrs apsr,fpscr
  0007bfc6: itttt gt
  0007bfc8: vldr.gt.32 s2,[r5]
  0007bfcc: vcmpe.gt.f32 s2,s0
  0007bfd0: vmrs.gt apsr,fpscr
  0007bfd4: vstr.gt.32 s0,[r5]
  0007bfd8: blx 0x0006eb90
  0007bfdc: cmp r0,#0x0
  0007bfde: beq.w 0x0007c0f2
  0007bfe2: bic r0,r8,#0x80000000
  0007bfe6: cmp r0,#0x2
  0007bfe8: beq 0x0007bffc
  0007bfea: cmp r0,#0x1
  0007bfec: beq 0x0007c008
  0007bfee: cbnz r0,0x0007c020
  0007bff0: vldr.32 s0,[sp,#0x20]
  0007bff4: add r1,sp,#0x18
  0007bff6: mov r0,r11
  0007bff8: movs r2,#0x8
  0007bffa: b 0x0007c012
  0007bffc: vldr.32 s0,[sp,#0x20]
  0007c000: mov r0,r11
  0007c002: add r1,sp,#0x18
  0007c004: movs r2,#0x20
  0007c006: b 0x0007c012
  0007c008: vldr.32 s0,[sp,#0x20]
  0007c00c: mov r0,r11
  0007c00e: add r1,sp,#0x18
  0007c010: movs r2,#0x10
  0007c012: vcvt.s32.f32 s0,s0
  0007c016: vstr.32 s0,[sp]
  0007c01a: movs r3,#0x0
  0007c01c: blx 0x0006eb9c
  0007c020: adds r6,#0x1
  0007c022: ldrh.w r0,[sp,#0x1c]
  0007c026: cmp r6,r0
  0007c028: bcc 0x0007bfa4
  0007c02a: add.w r8,r8,#0x1
  0007c02e: mov r1,r4
  0007c030: cmp.w r8,#0x2
  0007c034: bls 0x0007bf7a
  0007c036: b 0x0007c0b0
  0007c038: add r1,sp,#0x1c
  0007c03a: movs r0,#0x2
  0007c03c: mov r2,r10
  0007c03e: blx 0x0006eb90
  0007c042: cmp r0,#0x0
  0007c044: beq 0x0007c0f6
  0007c046: ldr r0,[0x0007c304]
  0007c048: add r4,sp,#0x18
  0007c04a: add r6,sp,#0x20
  0007c04c: movs r5,#0x0
  0007c04e: add r0,pc
  0007c050: ldr.w r8,[r0,#0x0]
  0007c054: b 0x0007c0a8
  0007c056: movs r0,#0x4
  0007c058: mov r1,r4
  0007c05a: mov r2,r10
  0007c05c: blx 0x0006eb90
  0007c060: cmp r0,#0x0
  0007c062: beq 0x0007c0f2
  0007c064: vldr.32 s0,[sp,#0x18]
  0007c068: movs r0,#0xc
  0007c06a: mov r1,r6
  0007c06c: mov r2,r10
  0007c06e: vcmpe.f32 s0,#0
  0007c072: vmrs apsr,fpscr
  0007c076: itttt gt
  0007c078: vldr.gt.32 s2,[r8]
  0007c07c: vcmpe.gt.f32 s2,s0
  0007c080: vmrs.gt apsr,fpscr
  0007c084: vstr.gt.32 s0,[r8]
  0007c088: blx 0x0006eb90
  0007c08c: cbz r0,0x0007c0f2
  0007c08e: vldr.32 s0,[sp,#0x18]
  0007c092: mov r0,r11
  0007c094: mov r1,r6
  0007c096: movs r2,#0x38
  0007c098: vcvt.s32.f32 s0,s0
  0007c09c: movs r3,#0x0
  0007c09e: vstr.32 s0,[sp]
  0007c0a2: blx 0x0006eb9c
  0007c0a6: adds r5,#0x1
  0007c0a8: ldrh.w r0,[sp,#0x1c]
  0007c0ac: cmp r5,r0
  0007c0ae: bcc 0x0007c056
  0007c0b0: ldr r5,[sp,#0xc]
  0007c0b2: ldr.w r8,[sp,#0x10]
  0007c0b6: tst r5,#0x18
  0007c0ba: beq 0x0007c184
  0007c0bc: sub.w r1,r7,#0x4a
  0007c0c0: movs r0,#0x2
  0007c0c2: mov r2,r10
  0007c0c4: blx 0x0006eb90
  0007c0c8: cbz r0,0x0007c0f6
  0007c0ca: ldrh.w r0,[r7,#-0x4a]
  0007c0ce: cmp r0,#0x2
  0007c0d0: bne 0x0007c184
  0007c0d2: add r1,sp,#0x1c
  0007c0d4: movs r0,#0x2
  0007c0d6: mov r2,r10
  0007c0d8: mov r4,r8
  0007c0da: blx 0x0006eb90
  0007c0de: cmp r0,#0x0
  0007c0e0: beq.w 0x0007c2dc
  0007c0e4: ldr r0,[0x0007c308]
  0007c0e6: add r6,sp,#0x18
  0007c0e8: movs r5,#0x0
  0007c0ea: add r0,pc
  0007c0ec: ldr.w r8,[r0,#0x0]
  0007c0f0: b 0x0007c178
  0007c0f2: ldr.w r8,[sp,#0x10]
  0007c0f6: mov r0,r11
  0007c0f8: blx 0x0006eba8
  0007c0fc: blx 0x0006eb48
  0007c100: mov.w r0,#0xffffffff
  0007c104: ldr r1,[sp,#0x2c]
  0007c106: ldr.w r2,[r8,#0x0]
  0007c10a: subs r1,r2,r1
  0007c10c: itttt eq
  0007c10e: vpop.eq {d2,d3,d4,d5,d6,d7,d8,d9,d10}
  0007c112: add.eq sp,#0x4
  0007c114: pop.eq.w {r8,r9,r10,r11}
  0007c118: pop.eq {r4,r5,r6,r7,pc}
  0007c11a: blx 0x0006e824
  0007c11e: add r1,sp,#0x20
  0007c120: movs r0,#0x4
  0007c122: mov r2,r10
  0007c124: blx 0x0006eb90
  0007c128: cmp r0,#0x0
  0007c12a: beq.w 0x0007c2dc
  0007c12e: vldr.32 s0,[sp,#0x20]
  0007c132: movs r0,#0x4
  0007c134: mov r1,r6
  0007c136: mov r2,r10
  0007c138: vcmpe.f32 s0,#0
  0007c13c: vmrs apsr,fpscr
  0007c140: itttt gt
  0007c142: vldr.gt.32 s2,[r8]
  0007c146: vcmpe.gt.f32 s2,s0
  0007c14a: vmrs.gt apsr,fpscr
  0007c14e: vstr.gt.32 s0,[r8]
  0007c152: blx 0x0006eb90
  0007c156: cmp r0,#0x0
  0007c158: beq.w 0x0007c2dc
  0007c15c: vldr.32 s0,[sp,#0x20]
  0007c160: mov r0,r11
  0007c162: mov r1,r6
  0007c164: mov.w r2,#0x200
  0007c168: vcvt.s32.f32 s0,s0
  0007c16c: movs r3,#0x0
  0007c16e: vstr.32 s0,[sp]
  0007c172: blx 0x0006eb9c
  0007c176: adds r5,#0x1
  0007c178: ldrh.w r0,[sp,#0x1c]
  0007c17c: cmp r5,r0
  0007c17e: bcc 0x0007c11e
  0007c180: ldr r5,[sp,#0xc]
  0007c182: mov r8,r4
  0007c184: ands r0,r5,#0x10
  0007c188: bne 0x0007c1c4
  0007c18a: ldr.w r0,[r11,#0x11c]
  0007c18e: cmp r0,#0x1
  0007c190: blt 0x0007c1f6
  0007c192: ldr r0,[0x0007c310]
  0007c194: str.w r11,[r9,#0x34]
  0007c198: add r0,pc
  0007c19a: ldr r5,[0x0007c300]
  0007c19c: ldr r0,[r0,#0x0]
  0007c19e: vldr.32 s0,[r0]
  0007c1a2: vmov r0,s0
  0007c1a6: vcvt.s32.f32 s0,s0
  0007c1aa: vstr.32 s0,[r11,#0xe8]
  0007c1ae: blx 0x0006ebb4
  0007c1b2: mov r2,r0
  0007c1b4: movs r0,#0x0
  0007c1b6: strd r5,r0,[sp,#0x0]
  0007c1ba: mov r0,r11
  0007c1bc: mov r3,r1
  0007c1be: blx 0x0006ebc0
  0007c1c2: b 0x0007c200
  0007c1c4: add r1,sp,#0x1c
  0007c1c6: movs r0,#0x2
  0007c1c8: mov r2,r10
  0007c1ca: blx 0x0006eb90
  0007c1ce: cmp r0,#0x0
  0007c1d0: beq 0x0007c0f6
  0007c1d2: ldrh.w r0,[sp,#0x1c]
  0007c1d6: mov r4,r8
  0007c1d8: cmp r0,#0x0
  0007c1da: beq.w 0x0007c2d8
  0007c1de: ldr r0,[0x0007c30c]
  0007c1e0: mov.w r8,#0x0
  0007c1e4: vldr.32 s16,[pc,#0x10c]
  0007c1e8: add r0,pc
  0007c1ea: vldr.32 s18,[pc,#0x10c]
  0007c1ee: vldr.32 s20,[pc,#0x10c]
  0007c1f2: ldr r5,[r0,#0x0]
  0007c1f4: b 0x0007c2d0
  0007c1f6: mov r0,r11
  0007c1f8: blx 0x0006eba8
  0007c1fc: blx 0x0006eb48
  0007c200: movs r0,#0x1
  0007c202: b 0x0007c104
  0007c204: sub.w r1,r7,#0x52
  0007c208: movs r0,#0x2
  0007c20a: mov r2,r10
  0007c20c: blx 0x0006eb90
  0007c210: cmp r0,#0x0
  0007c212: beq 0x0007c2dc
  0007c214: movs r6,#0x0
  0007c216: b 0x0007c2c4
  0007c218: add r1,sp,#0x20
  0007c21a: movs r0,#0x4
  0007c21c: mov r2,r10
  0007c21e: blx 0x0006eb90
  0007c222: cmp r0,#0x0
  0007c224: beq 0x0007c2dc
  0007c226: vldr.32 s0,[sp,#0x20]
  0007c22a: add r1,sp,#0x18
  0007c22c: movs r0,#0x4
  0007c22e: mov r2,r10
  0007c230: vcmpe.f32 s0,#0
  0007c234: vmrs apsr,fpscr
  0007c238: itttt gt
  0007c23a: vldr.gt.32 s2,[r5]
  0007c23e: vcmpe.gt.f32 s2,s0
  0007c242: vmrs.gt apsr,fpscr
  0007c246: vstr.gt.32 s0,[r5]
  0007c24a: blx 0x0006eb90
  0007c24e: cmp r0,#0x0
  0007c250: beq 0x0007c2dc
  0007c252: vldr.32 s0,[sp,#0x18]
  0007c256: bic r0,r8,#0x80000000
  0007c25a: cmp r0,#0x6
  0007c25c: vdiv.f32 s0,s0,s16
  0007c260: vstr.32 s0,[sp,#0x18]
  0007c264: bhi 0x0007c290
  0007c266: movs r3,#0x0
  0007c268: mov.w r2,#0x400
  0007c26c: tbb [pc,r0]
  0007c278: movs r3,#0x0
  0007c27a: mov.w r2,#0x800
  0007c27e: b 0x0007c2a8
  0007c280: movs r3,#0x0
  0007c282: mov.w r2,#0x2000
  0007c286: b 0x0007c2a8
  0007c288: movs r3,#0x0
  0007c28a: mov.w r2,#0x4000
  0007c28e: b 0x0007c2a8
  0007c290: movs r2,#0x0
  0007c292: movs r3,#0x0
  0007c294: b 0x0007c2a8
  0007c296: vmul.f32 s0,s0,s18
  0007c29a: movs r3,#0x0
  0007c29c: mov.w r2,#0x40000
  0007c2a0: vdiv.f32 s0,s0,s20
  0007c2a4: vstr.32 s0,[sp,#0x18]
  0007c2a8: vldr.32 s0,[sp,#0x20]
  0007c2ac: add r1,sp,#0x18
  0007c2ae: mov r0,r11
  0007c2b0: vcvt.s32.f32 s0,s0
  0007c2b4: vstr.32 s0,[sp]
  0007c2b8: blx 0x0006eb9c
  0007c2bc: movs r0,#0x1
  0007c2be: adds r6,#0x1
  0007c2c0: strb.w r0,[r9,#0x85]
  0007c2c4: ldrsh.w r0,[r7,#-0x52]
  0007c2c8: cmp r6,r0
  0007c2ca: blt 0x0007c218
  0007c2cc: add.w r8,r8,#0x1
  0007c2d0: cmp.w r8,#0x6
  0007c2d4: bls.w 0x0007c204
  0007c2d8: mov r8,r4
  0007c2da: b 0x0007c18a
  0007c2dc: mov r8,r4
  0007c2de: b 0x0007c0f6
```
