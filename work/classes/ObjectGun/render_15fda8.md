# ObjectGun::render
elf 0x15fda8 body 1396
Sig: undefined __thiscall render(ObjectGun * this)

## decompile
```c

/* ObjectGun::render() */

void __thiscall ObjectGun::render(ObjectGun *this)

{
  float fVar1;
  float fVar2;
  float fVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  int iVar6;
  Matrix *pMVar7;
  Vector *pVVar8;
  Vector *this_00;
  int iVar9;
  float *pfVar10;
  undefined4 *puVar11;
  int iVar12;
  Vector *pVVar13;
  uint uVar14;
  uint uVar15;
  uint *puVar16;
  int *piVar17;
  Vector *this_01;
  code *pcVar18;
  int iVar19;
  uint in_fpscr;
  float fVar20;
  float extraout_s0;
  float fVar21;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s1_09;
  float extraout_s1_10;
  float extraout_s1_11;
  float extraout_s1_12;
  float extraout_s1_13;
  float extraout_s1_14;
  float extraout_s1_15;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float extraout_s2_07;
  float extraout_s2_08;
  float extraout_s2_09;
  float extraout_s2_10;
  float fVar22;
  float extraout_s2_11;
  float extraout_s2_12;
  float extraout_s2_13;
  float extraout_s2_14;
  float fVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  Matrix aMStack_134 [12];
  AEMath aAStack_128 [12];
  float local_11c;
  float local_118;
  float local_114;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_cc;
  undefined4 local_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined4 local_a8;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined4 local_68;
  int local_64;
  
  piVar17 = *(int **)(DAT_001700c4 + 0x16fdbe);
  local_64 = *piVar17;
  Gun::render();
  iVar6 = *(int *)(this + 8);
  fVar21 = extraout_s1;
  fVar22 = extraout_s2;
  if (*(int *)(iVar6 + 0x5c) == 0x19) {
    for (uVar14 = 0; uVar14 < *(uint *)(iVar6 + 8); uVar14 = uVar14 + 1) {
      if (*(char *)(*(int *)(iVar6 + 0x40) + uVar14) != '\0') {
        Explosion::render();
        iVar6 = *(int *)(this + 8);
        fVar21 = extraout_s1_00;
        fVar22 = extraout_s2_00;
      }
    }
  }
  uVar5 = DAT_00170340;
  uVar4 = DAT_00170338;
  if (*(char *)(iVar6 + 0x4c) != '\0') {
    uVar24 = 0;
    uVar25 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar26 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar27 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar11 = (undefined4 *)((uint)&local_a0 | 4);
    *puVar11 = 0;
    puVar11[1] = uVar25;
    puVar11[2] = uVar26;
    puVar11[3] = uVar27;
    local_88 = 0;
    local_78 = uVar4;
    uStack_70 = uVar5;
    local_a0 = 0x3f800000;
    local_8c = 0x3f800000;
    local_68 = 0x3f800000;
    uStack_84 = uVar25;
    local_80 = uVar26;
    local_7c = uVar27;
    if (*(int *)(iVar6 + 0x5c) == 8) {
      uVar14 = **(uint **)(DAT_001700c8 + 0x16fe38);
      AbyssEngine::PaintCanvas::CameraGetCurrent();
      pMVar7 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetLocal(uVar14);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_a0,pMVar7);
      fVar21 = extraout_s1_01;
      fVar22 = extraout_s2_01;
      if (this[0x4c] != (ObjectGun)0x0) {
        uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_c0 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_bc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar11 = (undefined4 *)((uint)&local_e0 | 4);
        *puVar11 = 0;
        puVar11[1] = uStack_c4;
        puVar11[2] = uStack_c0;
        puVar11[3] = uStack_bc;
        local_c8 = 0;
        local_b8 = uVar4;
        uStack_b0 = uVar5;
        local_e0 = 0x3f800000;
        local_cc = 0x3f800000;
        local_a8 = 0x3f800000;
        AbyssEngine::AEMath::MatrixSetRotation
                  ((Matrix *)&local_11c,*(float *)(this + 0x48),extraout_s1_01,extraout_s2_01);
        AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_e0,(Matrix *)&local_11c);
        AbyssEngine::AEMath::Matrix::operator*=((Matrix *)&local_a0,(Matrix *)&local_e0);
        fVar21 = extraout_s1_02;
        fVar22 = extraout_s2_02;
      }
    }
    fVar3 = DAT_001700d8;
    fVar2 = DAT_001700d4;
    fVar1 = DAT_001700d0;
    pVVar8 = (Vector *)(this + 0x68);
    this_00 = (Vector *)(this + 0x5c);
    this_01 = (Vector *)(this + 0x50);
    iVar19 = 0;
    puVar11 = (undefined4 *)((uint)&local_e0 | 4);
    iVar6 = 0;
    for (uVar14 = 0; iVar9 = *(int *)(this + 8), uVar14 < *(uint *)(iVar9 + 8); uVar14 = uVar14 + 1)
    {
      pfVar10 = (float *)(*(int *)(iVar9 + 0xc) + iVar19);
      in_fpscr = in_fpscr & 0xfffffff | (uint)(*pfVar10 == fVar1) << 0x1e;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_e0,pfVar10[2],fVar21,fVar22);
        AbyssEngine::AEMath::VectorNormalize
                  ((AEMath *)&local_e0,(Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar19));
        AbyssEngine::AEMath::Vector::operator=(this_01,(Vector *)&local_e0);
        if (this[0x4c] == (ObjectGun)0x0) {
          if (this[0x24] == (ObjectGun)0x0) {
            if (((*(int *)(this + 0xc) == 0) || (iVar9 = Level::getPlayer(), iVar9 == 0)) ||
               (iVar9 = Gun::isPlayerGun(), iVar9 == 0)) {
              local_dc = 0x3f800000;
              local_e0 = 0;
              local_d8 = 0;
              pVVar13 = (Vector *)&local_e0;
            }
            else {
              pVVar13 = (Vector *)(*(int *)(*(int *)(this + 8) + 0x24) + iVar19);
            }
            AbyssEngine::AEMath::Vector::operator=(this_00,pVVar13);
            AbyssEngine::AEMath::VectorCross((Vector *)&local_e0,this_00);
            pcVar18 = *(code **)(DAT_0017034c + 0x17022e);
            (*pcVar18)(pVVar8,(Matrix *)&local_e0);
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e0,pVVar8);
            (*pcVar18)(pVVar8,(Matrix *)&local_e0);
            AbyssEngine::AEMath::VectorCross((Vector *)&local_e0,this_01);
            (*pcVar18)(this_00,(Matrix *)&local_e0);
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e0,this_00);
            (*pcVar18)(this_00,(Matrix *)&local_e0);
            *(undefined4 *)(this + 0x74) = *(undefined4 *)(this + 0x68);
            *(undefined4 *)(this + 0x78) = *(undefined4 *)(this + 0x5c);
            *(undefined4 *)(this + 0x7c) = *(undefined4 *)(this + 0x50);
            *(undefined4 *)(this + 0x84) = *(undefined4 *)(this + 0x6c);
            *(undefined4 *)(this + 0x88) = *(undefined4 *)(this + 0x60);
            *(undefined4 *)(this + 0x8c) = *(undefined4 *)(this + 0x54);
            *(undefined4 *)(this + 0x94) = *(undefined4 *)(this + 0x70);
            *(undefined4 *)(this + 0x98) = *(undefined4 *)(this + 100);
            *(undefined4 *)(this + 0x9c) = *(undefined4 *)(this + 0x58);
            iVar9 = *(int *)(*(int *)(*(int *)(this + 8) + 0x3c) + uVar14 * 4);
            if (iVar9 < 1000) {
              fVar21 = (float)VectorSignedToFloat(iVar9,(byte)(in_fpscr >> 0x16) & 3);
              AbyssEngine::AEMath::MatrixSetScaling
                        ((Matrix *)&local_e0,fVar21 / fVar2,extraout_s1_13,extraout_s2_12);
              fVar21 = extraout_s1_14;
              fVar22 = extraout_s2_13;
            }
            else {
              fVar21 = extraout_s1_13;
              fVar22 = extraout_s2_12;
              if (*(char *)(*(int *)(this + 8) + 0x4d) != '\0') {
                AbyssEngine::AEMath::MatrixSetScaling
                          ((Matrix *)&local_e0,extraout_s0_00,extraout_s1_13,extraout_s2_12);
                iVar9 = (**(code **)(*(int *)this + 0x20))(this);
                fVar21 = extraout_s1_15;
                fVar22 = extraout_s2_14;
                if (iVar9 == 0) {
                  *(undefined1 *)(*(int *)(this + 8) + 0x4d) = 0;
                }
              }
            }
          }
          else {
            iVar9 = Level::getPlayer();
            local_e0 = 0x3f800000;
            *puVar11 = uVar24;
            puVar11[1] = uVar25;
            puVar11[2] = uVar26;
            puVar11[3] = uVar27;
            local_cc = 0x3f800000;
            local_b8 = uVar4;
            uStack_b0 = uVar5;
            local_a8 = 0x3f800000;
            local_c8 = uVar24;
            uStack_c4 = uVar25;
            uStack_c0 = uVar26;
            uStack_bc = uVar27;
            AbyssEngine::AEMath::MatrixSetRotation
                      ((Matrix *)&local_11c,extraout_s0,extraout_s1_05,extraout_s2_05);
            AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(iVar9 + 0x40),(Matrix *)&local_e0);
            AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x74),(Matrix *)(iVar9 + 0x40))
            ;
            AbyssEngine::AEMath::MatrixSetTranslation
                      ((Matrix *)&local_11c,
                       *(float *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar19 + 8),extraout_s1_06,
                       extraout_s2_06);
            AbyssEngine::AEMath::MatrixGetDir(aMStack_134);
            fVar21 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_128,(Vector *)aMStack_134);
            AbyssEngine::AEMath::operator*((Vector *)&local_11c,fVar21);
            AbyssEngine::AEMath::Vector::operator=
                      ((Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar19),(Vector *)&local_11c
                      );
            *(undefined4 *)(iVar9 + 0x7c) = 0;
            *(undefined4 *)(iVar9 + 0x80) = 0;
            AbyssEngine::AEMath::MatrixSetRotation
                      ((Matrix *)&local_11c,*(float *)(this + 0x20),extraout_s1_07,extraout_s2_07);
            AbyssEngine::PaintCanvas::TransformSetLocal
                      (**(uint **)(DAT_00170348 + 0x1700bc),*(Matrix **)(this + 0x14));
            fVar21 = extraout_s1_08;
            fVar22 = extraout_s2_08;
          }
        }
        else {
          AbyssEngine::AEMath::operator-((AEMath *)&local_11c,this_01);
          iVar9 = *(int *)(this + 8);
          fVar21 = extraout_s1_03;
          fVar22 = extraout_s2_03;
          if (2 < *(int *)(iVar9 + 0x58) - 0xb4U) {
            uVar15 = **(uint **)(DAT_001700dc + 0x16ff52);
            AbyssEngine::PaintCanvas::CameraGetCurrent();
            pMVar7 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetLocal(uVar15);
            AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_a0,pMVar7);
            AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_e0);
            AbyssEngine::AEMath::Vector::operator=((Vector *)&local_11c,(Vector *)&local_e0);
            iVar9 = *(int *)(this + 8);
            fVar21 = extraout_s1_04;
            fVar22 = extraout_s2_04;
          }
          *(undefined4 *)(this + 0x74) = local_a0;
          *(undefined4 *)(this + 0x78) = uStack_9c;
          *(undefined4 *)(this + 0x84) = local_90;
          *(undefined4 *)(this + 0x88) = local_8c;
          *(undefined4 *)(this + 0x94) = local_80;
          *(undefined4 *)(this + 0x98) = local_7c;
          *(float *)(this + 0x7c) = -local_11c;
          *(float *)(this + 0x8c) = -local_118;
          fVar20 = -local_114;
          *(float *)(this + 0x9c) = fVar20;
          iVar12 = *(int *)(*(int *)(iVar9 + 0x3c) + uVar14 * 4);
          if (iVar12 < 1000) {
            fVar20 = (float)VectorSignedToFloat(iVar12,(byte)(in_fpscr >> 0x16) & 3);
            fVar20 = fVar20 / fVar2;
          }
          else if (*(char *)(iVar9 + 0x4d) == '\0') goto LAB_001700fc;
          AbyssEngine::AEMath::MatrixSetScaling((Matrix *)&local_e0,fVar20,fVar21,fVar22);
          fVar21 = extraout_s1_09;
          fVar22 = extraout_s2_09;
        }
LAB_001700fc:
        if (**(char **)(DAT_00170350 + 0x170102) != '\0') {
          AbyssEngine::AEMath::MatrixSetScaling
                    ((Matrix *)&local_e0,*(float *)(this + 0x44),fVar21,fVar22);
          fVar21 = extraout_s1_10;
          fVar22 = extraout_s2_10;
        }
        if (*(int *)(*(int *)(this + 8) + 0x5c) == 0xb) {
          pfVar10 = *(float **)(*(int *)(*(int *)(*(int *)(this + 8) + 0xac) + 4) + uVar14 * 4);
          if ((pfVar10 != (float *)0x0) && (0 < *(int *)(this + 0x34))) {
            AbyssEngine::AEMath::MatrixSetRotation((Matrix *)&local_e0,pfVar10[2],fVar21,fVar22);
            fVar21 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x34),
                                                (byte)(in_fpscr >> 0x16) & 3);
            in_fpscr = in_fpscr & 0xfffffff;
            fVar22 = fVar21 * fVar3;
            fVar20 = -(fVar21 * fVar3);
            fVar21 = fVar22;
            if (*pfVar10 < 0.0) {
              fVar21 = fVar20;
            }
            fVar23 = fVar22;
            if (pfVar10[1] < 0.0) {
              fVar23 = fVar20;
            }
            if (pfVar10[2] < 0.0) {
              fVar22 = fVar20;
            }
            fVar22 = pfVar10[2] + fVar22;
            *pfVar10 = *pfVar10 + fVar21;
            pfVar10[1] = pfVar10[1] + fVar23;
            pfVar10[2] = fVar22;
            fVar21 = extraout_s1_11;
          }
          AbyssEngine::AEMath::MatrixSetScaling
                    ((Matrix *)&local_e0,*(float *)(this + 0x44),fVar21,fVar22);
        }
        puVar16 = *(uint **)(DAT_00170354 + 0x1701cc);
        AbyssEngine::PaintCanvas::TransformSetLocal(*puVar16,*(Matrix **)(this + 0x10));
        AbyssEngine::PaintCanvas::DrawTransform(*puVar16,*(Matrix **)(this + 0x10));
        fVar21 = extraout_s1_12;
        fVar22 = extraout_s2_11;
      }
      else {
        iVar6 = iVar6 + 1;
      }
      iVar19 = iVar19 + 0xc;
    }
    if (*(int *)(iVar9 + 0x14) <= iVar6) {
      *(undefined1 *)(iVar9 + 0x4c) = 0;
    }
  }
  if ((this[0x1d] != (ObjectGun)0x0) && (*(int *)(this + 0x18) != 0)) {
    AEGeometry::render();
  }
  *(undefined4 *)(this + 0x34) = 0;
  if (*piVar17 == local_64) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0016fda8: push {r4,r5,r6,r7,lr}
  0016fdaa: add r7,sp,#0xc
  0016fdac: push {r7,r8,r9,r10,r11}
  0016fdb0: vpush {d8,d9,d10,d11,d12,d13,d14}
  0016fdb4: sub sp,#0x100
  0016fdb6: mov r4,r0
  0016fdb8: ldr r0,[0x001700c4]
  0016fdba: add r0,pc
  0016fdbc: ldr r6,[r0,#0x0]
  0016fdbe: ldr r0,[r6,#0x0]
  0016fdc0: str r0,[sp,#0xfc]
  0016fdc2: ldr r0,[r4,#0x8]
  0016fdc4: blx 0x000779bc
  0016fdc8: ldr r0,[r4,#0x8]
  0016fdca: ldr r1,[r0,#0x5c]
  0016fdcc: cmp r1,#0x19
  0016fdce: bne 0x0016fdf0
  0016fdd0: movs r5,#0x0
  0016fdd2: b 0x0016fdea
  0016fdd4: ldr r1,[r0,#0x40]
  0016fdd6: ldrb r1,[r1,r5]
  0016fdd8: cbz r1,0x0016fde8
  0016fdda: ldr r0,[r4,#0x2c]
  0016fddc: ldr r0,[r0,#0x4]
  0016fdde: ldr.w r0,[r0,r5,lsl #0x2]
  0016fde2: blx 0x00072d18
  0016fde6: ldr r0,[r4,#0x8]
  0016fde8: adds r5,#0x1
  0016fdea: ldr r1,[r0,#0x8]
  0016fdec: cmp r5,r1
  0016fdee: bcc 0x0016fdd4
  0016fdf0: ldrb.w r1,[r0,#0x4c]
  0016fdf4: cmp r1,#0x0
  0016fdf6: beq.w 0x0017030c
  0016fdfa: adr.w r1,0x170338
  0016fdfe: vmov.i32 q4,#0x0
  0016fe02: add r2,sp,#0xc0
  0016fe04: vld1.64 {d10,d11},[r1]
  0016fe08: orr r1,r2,#0x4
  0016fe0c: str r6,[sp,#0x4]
  0016fe0e: vst1.32 {d8,d9},[r1]
  0016fe12: add.w r1,r2,#0x18
  0016fe16: vst1.64 {d8,d9},[r1]
  0016fe1a: add.w r1,r2,#0x28
  0016fe1e: vst1.64 {d10,d11},[r1]
  0016fe22: mov.w r1,#0x3f800000
  0016fe26: str r1,[sp,#0xc0]
  0016fe28: str r1,[sp,#0xd4]
  0016fe2a: str r1,[sp,#0xf8]
  0016fe2c: ldr r0,[r0,#0x5c]
  0016fe2e: cmp r0,#0x8
  0016fe30: bne 0x0016fea4
  0016fe32: ldr r0,[0x001700c8]
  0016fe34: add r0,pc
  0016fe36: ldr r0,[r0,#0x0]
  0016fe38: ldr r5,[r0,#0x0]
  0016fe3a: mov r0,r5
  0016fe3c: blx 0x000717f4
  0016fe40: mov r1,r0
  0016fe42: mov r0,r5
  0016fe44: blx 0x0006ff1c
  0016fe48: mov r1,r0
  0016fe4a: add r0,sp,#0xc0
  0016fe4c: blx 0x0006f148
  0016fe50: ldrb.w r0,[r4,#0x4c]
  0016fe54: cbz r0,0x0016fea4
  0016fe56: add r5,sp,#0x80
  0016fe58: vmov.i32 q8,#0x0
  0016fe5c: orr r0,r5,#0x4
  0016fe60: add r6,sp,#0x44
  0016fe62: ldr r3,[0x001700cc]
  0016fe64: mov r1,r5
  0016fe66: vst1.32 {d16,d17},[r0]
  0016fe6a: add.w r0,r5,#0x18
  0016fe6e: movs r2,#0x0
  0016fe70: vst1.64 {d16,d17},[r0]
  0016fe74: add.w r0,r5,#0x28
  0016fe78: vst1.64 {d10,d11},[r0]
  0016fe7c: mov.w r0,#0x3f800000
  0016fe80: str r0,[sp,#0x80]
  0016fe82: str r0,[sp,#0x94]
  0016fe84: str r0,[sp,#0xb8]
  0016fe86: mov r0,r6
  0016fe88: vldr.32 s0,[r4,#0x48]
  0016fe8c: vstr.32 s0,[sp]
  0016fe90: blx 0x00072094
  0016fe94: mov r0,r5
  0016fe96: mov r1,r6
  0016fe98: blx 0x0006f148
  0016fe9c: add r0,sp,#0xc0
  0016fe9e: mov r1,r5
  0016fea0: blx 0x0006f4a8
  0016fea4: add.w r0,r4,#0x68
  0016fea8: str r0,[sp,#0x8]
  0016feaa: add.w r0,r4,#0x5c
  0016feae: add r6,sp,#0x80
  0016feb0: str r0,[sp,#0x1c]
  0016feb2: add.w r0,r6,#0x28
  0016feb6: str r0,[sp,#0x18]
  0016feb8: add.w r0,r6,#0x18
  0016febc: add.w r8,r4,#0x50
  0016fec0: vldr.32 s24,[pc,#0x20c]
  0016fec4: vldr.32 s26,[pc,#0x20c]
  0016fec8: mov.w r11,#0x0
  0016fecc: vldr.32 s28,[pc,#0x208]
  0016fed0: mov.w r10,#0x0
  0016fed4: str r0,[sp,#0x14]
  0016fed6: orr r0,r6,#0x4
  0016feda: mov.w r9,#0x0
  0016fede: str r0,[sp,#0x10]
  0016fee0: add.w r0,r4,#0x74
  0016fee4: str r0,[sp,#0x28]
  0016fee6: str.w r8,[sp,#0x20]
  0016feea: ldr r0,[r4,#0x8]
  0016feec: ldr r1,[r0,#0x8]
  0016feee: cmp r10,r1
  0016fef0: bcs.w 0x001702fe
  0016fef4: ldr r0,[r0,#0xc]
  0016fef6: add r0,r11
  0016fef8: vldr.32 s0,[r0]
  0016fefc: vcmpe.f32 s0,s24
  0016ff00: vmrs apsr,fpscr
  0016ff04: beq 0x0016ffe4
  0016ff06: vmov r2,s0
  0016ff0a: ldr r3,[r0,#0x4]
  0016ff0c: ldr r1,[sp,#0x28]
  0016ff0e: vldr.32 s0,[r0,#0x8]
  0016ff12: mov r0,r6
  0016ff14: vstr.32 s0,[sp]
  0016ff18: blx 0x0006f820
  0016ff1c: ldr r0,[r4,#0x8]
  0016ff1e: ldr r0,[r0,#0x18]
  0016ff20: add.w r1,r0,r11
  0016ff24: mov r0,r6
  0016ff26: blx 0x0006ec80
  0016ff2a: mov r0,r8
  0016ff2c: mov r1,r6
  0016ff2e: blx 0x0006eb3c
  0016ff32: ldrb.w r0,[r4,#0x4c]
  0016ff36: cmp r0,#0x0
  0016ff38: beq 0x0016ffea
  0016ff3a: add r0,sp,#0x44
  0016ff3c: mov r1,r8
  0016ff3e: blx 0x0006f4cc
  0016ff42: ldr r0,[r4,#0x8]
  0016ff44: ldr r1,[r0,#0x58]
  0016ff46: subs r1,#0xb4
  0016ff48: cmp r1,#0x3
  0016ff4a: bcc 0x0016ff7e
  0016ff4c: ldr r0,[0x001700dc]
  0016ff4e: add r0,pc
  0016ff50: ldr r0,[r0,#0x0]
  0016ff52: ldr r5,[r0,#0x0]
  0016ff54: mov r0,r5
  0016ff56: blx 0x000717f4
  0016ff5a: mov r1,r0
  0016ff5c: mov r0,r5
  0016ff5e: blx 0x0006ff1c
  0016ff62: add r5,sp,#0xc0
  0016ff64: mov r1,r0
  0016ff66: mov r0,r5
  0016ff68: blx 0x0006f148
  0016ff6c: mov r0,r6
  0016ff6e: mov r1,r5
  0016ff70: blx 0x0006f4c0
  0016ff74: add r0,sp,#0x44
  0016ff76: mov r1,r6
  0016ff78: blx 0x0006eb3c
  0016ff7c: ldr r0,[r4,#0x8]
  0016ff7e: ldr r2,[sp,#0xd0]
  0016ff80: ldr r3,[sp,#0xe0]
  0016ff82: ldrd r1,r5,[sp,#0xc0]
  0016ff86: strd r1,r5,[r4,#0x74]
  0016ff8a: ldr r1,[sp,#0xd4]
  0016ff8c: strd r2,r1,[r4,#0x84]
  0016ff90: ldr r1,[sp,#0xe4]
  0016ff92: strd r3,r1,[r4,#0x94]
  0016ff96: vldr.32 s0,[sp,#0x44]
  0016ff9a: vneg.f32 s0,s0
  0016ff9e: vstr.32 s0,[r4,#0x7c]
  0016ffa2: vldr.32 s0,[sp,#0x48]
  0016ffa6: vneg.f32 s0,s0
  0016ffaa: vstr.32 s0,[r4,#0x8c]
  0016ffae: vldr.32 s0,[sp,#0x4c]
  0016ffb2: vneg.f32 s0,s0
  0016ffb6: vstr.32 s0,[r4,#0x9c]
  0016ffba: ldr r1,[r0,#0x3c]
  0016ffbc: ldr.w r1,[r1,r10,lsl #0x2]
  0016ffc0: cmp.w r1,#0x3e8
  0016ffc4: bge.w 0x001700e0
  0016ffc8: vmov s0,r1
  0016ffcc: mov r0,r6
  0016ffce: vcvt.f32.s32 s0,s0
  0016ffd2: ldr r1,[sp,#0x28]
  0016ffd4: vdiv.f32 s0,s0,s26
  0016ffd8: vmov r2,s0
  0016ffdc: vstr.32 s0,[sp]
  0016ffe0: mov r3,r2
  0016ffe2: b 0x001700f8
  0016ffe4: add.w r9,r9,#0x1
  0016ffe8: b 0x001701de
  0016ffea: ldrb.w r1,[r4,#0x24]
  0016ffee: ldr r0,[r4,#0xc]
  0016fff0: cmp r1,#0x0
  0016fff2: str.w r9,[sp,#0x24]
  0016fff6: beq.w 0x001701e8
  0016fffa: blx 0x00072034
  0016fffe: mov r8,r0
  00170000: ldr r0,[sp,#0x10]
  00170002: mov.w r1,#0x3f800000
  00170006: str r1,[sp,#0x80]
  00170008: vst1.32 {d8,d9},[r0]
  0017000c: ldr r0,[sp,#0x14]
  0017000e: str r1,[sp,#0x94]
  00170010: vst1.64 {d8,d9},[r0]
  00170014: ldr r0,[sp,#0x18]
  00170016: vst1.32 {d10,d11},[r0]
  0017001a: movs r0,#0x0
  0017001c: str r1,[sp,#0xb8]
  0017001e: mov r1,r6
  00170020: ldrd r2,r3,[r8,#0x7c]
  00170024: str r0,[sp,#0x0]
  00170026: add r0,sp,#0x44
  00170028: blx 0x00072094
  0017002c: add.w r5,r8,#0x40
  00170030: mov r1,r6
  00170032: mov r0,r5
  00170034: blx 0x0006f4a8
  00170038: ldr.w r9,[sp,#0x28]
  0017003c: mov r1,r5
  0017003e: mov r0,r9
  00170040: blx 0x0006f148
  00170044: ldr r0,[r4,#0x8]
  00170046: mov r1,r9
  00170048: ldr r0,[r0,#0xc]
  0017004a: ldr.w r2,[r0,r11]
  0017004e: add r0,r11
  00170050: ldr r3,[r0,#0x4]
  00170052: vldr.32 s0,[r0,#0x8]
  00170056: add r0,sp,#0x44
  00170058: vstr.32 s0,[sp]
  0017005c: blx 0x0006f820
  00170060: add r5,sp,#0x2c
  00170062: mov r1,r9
  00170064: mov r0,r5
  00170066: blx 0x0006f4c0
  0017006a: add.w r9,sp,#0x38
  0017006e: mov r1,r5
  00170070: mov r0,r9
  00170072: blx 0x0006ec80
  00170076: ldr r0,[r4,#0x8]
  00170078: add r5,sp,#0x44
  0017007a: mov r1,r9
  0017007c: ldr.w r9,[sp,#0x24]
  00170080: ldr r2,[r0,#0x50]
  00170082: mov r0,r5
  00170084: blx 0x0006ec74
  00170088: ldr r0,[r4,#0x8]
  0017008a: mov r1,r5
  0017008c: ldr r0,[r0,#0x18]
  0017008e: add r0,r11
  00170090: blx 0x0006eb3c
  00170094: movs r0,#0x0
  00170096: mov r1,r6
  00170098: strd r0,r0,[r8,#0x7c]
  0017009c: mov r0,r5
  0017009e: vldr.32 s0,[r4,#0x20]
  001700a2: movs r2,#0x0
  001700a4: movs r3,#0x0
  001700a6: ldr.w r8,[sp,#0x20]
  001700aa: vstr.32 s0,[sp]
  001700ae: blx 0x00072094
  001700b2: ldr r0,[0x00170348]
  001700b4: mov r2,r6
  001700b6: ldr r1,[r4,#0x14]
  001700b8: add r0,pc
  001700ba: ldr r0,[r0,#0x0]
  001700bc: ldr r0,[r0,#0x0]
  001700be: blx 0x000721c0
  001700c2: b 0x001700fc
  001700e0: ldrb.w r0,[r0,#0x4d]
  001700e4: cbz r0,0x001700fc
  001700e6: ldr r1,[sp,#0x28]
  001700e8: mov.w r0,#0x3f800000
  001700ec: str r0,[sp,#0x0]
  001700ee: mov r0,r6
  001700f0: mov.w r2,#0x3f800000
  001700f4: mov.w r3,#0x3f800000
  001700f8: blx 0x0006f814
  001700fc: ldr r0,[0x00170350]
  001700fe: add r0,pc
  00170100: ldr r0,[r0,#0x0]
  00170102: ldrb r0,[r0,#0x0]
  00170104: cbz r0,0x0017011a
  00170106: ldrd r2,r3,[r4,#0x3c]
  0017010a: mov r0,r6
  0017010c: ldr r1,[sp,#0x28]
  0017010e: vldr.32 s0,[r4,#0x44]
  00170112: vstr.32 s0,[sp]
  00170116: blx 0x0006f814
  0017011a: ldr r0,[r4,#0x8]
  0017011c: ldr r1,[r0,#0x5c]
  0017011e: cmp r1,#0xb
  00170120: bne 0x001701c4
  00170122: ldr.w r0,[r0,#0xac]
  00170126: ldr r0,[r0,#0x4]
  00170128: ldr.w r5,[r0,r10,lsl #0x2]
  0017012c: cmp r5,#0x0
  0017012e: beq 0x001701b0
  00170130: ldr r0,[r4,#0x34]
  00170132: cmp r0,#0x1
  00170134: blt 0x001701b0
  00170136: ldrd r2,r3,[r5,#0x0]
  0017013a: mov r0,r6
  0017013c: ldr r1,[sp,#0x28]
  0017013e: vldr.32 s0,[r5,#0x8]
  00170142: vstr.32 s0,[sp]
  00170146: blx 0x00072094
  0017014a: vldr.32 s0,[r4,#0x34]
  0017014e: vcvt.f32.s32 s0,s0
  00170152: vldr.32 s4,[r5]
  00170156: vldr.32 s6,[r5,#0x4]
  0017015a: vcmpe.f32 s4,#0
  0017015e: vldr.32 s8,[r5,#0x8]
  00170162: vmrs apsr,fpscr
  00170166: vcmpe.f32 s6,#0
  0017016a: vmul.f32 s2,s0,s28
  0017016e: vnmul.f32 s0,s0,s28
  00170172: vmov.f32 s10,s2
  00170176: vmov.f32 s12,s2
  0017017a: it mi
  0017017c: vmov.mi.f32 s10,s0
  00170180: vmrs apsr,fpscr
  00170184: vcmpe.f32 s8,#0
  00170188: it mi
  0017018a: vmov.mi.f32 s12,s0
  0017018e: vmrs apsr,fpscr
  00170192: it mi
  00170194: vmov.mi.f32 s2,s0
  00170198: vadd.f32 s0,s4,s10
  0017019c: vadd.f32 s4,s6,s12
  001701a0: vadd.f32 s2,s8,s2
  001701a4: vstr.32 s0,[r5]
  001701a8: vstr.32 s4,[r5,#0x4]
  001701ac: vstr.32 s2,[r5,#0x8]
  001701b0: ldrd r2,r3,[r4,#0x3c]
  001701b4: mov r0,r6
  001701b6: ldr r1,[sp,#0x28]
  001701b8: vldr.32 s0,[r4,#0x44]
  001701bc: vstr.32 s0,[sp]
  001701c0: blx 0x0006f814
  001701c4: ldr r0,[0x00170354]
  001701c6: ldr r1,[r4,#0x10]
  001701c8: add r0,pc
  001701ca: ldr r2,[sp,#0x28]
  001701cc: ldr r5,[r0,#0x0]
  001701ce: ldr r0,[r5,#0x0]
  001701d0: blx 0x000721c0
  001701d4: ldr r1,[r4,#0x10]
  001701d6: movs r2,#0x0
  001701d8: ldr r0,[r5,#0x0]
  001701da: blx 0x0007306c
  001701de: add.w r11,r11,#0xc
  001701e2: add.w r10,r10,#0x1
  001701e6: b 0x0016feea
  001701e8: cbz r0,0x00170202
  001701ea: blx 0x00072034
  001701ee: cbz r0,0x00170202
  001701f0: ldr r0,[r4,#0x8]
  001701f2: blx 0x0007798c
  001701f6: cbz r0,0x00170202
  001701f8: ldr r0,[r4,#0x8]
  001701fa: ldr r0,[r0,#0x24]
  001701fc: add.w r1,r0,r11
  00170200: b 0x00170210
  00170202: mov.w r0,#0x3f800000
  00170206: mov r1,r6
  00170208: str r0,[sp,#0x84]
  0017020a: movs r0,#0x0
  0017020c: str r0,[sp,#0x80]
  0017020e: str r0,[sp,#0x88]
  00170210: ldr r0,[sp,#0x1c]
  00170212: blx 0x0006eb3c
  00170216: ldr r5,[sp,#0x1c]
  00170218: mov r0,r6
  0017021a: mov r2,r8
  0017021c: mov r1,r5
  0017021e: blx 0x0006ec8c
  00170222: ldr r0,[0x0017034c]
  00170224: mov r8,r6
  00170226: ldr r6,[sp,#0x8]
  00170228: mov r1,r8
  0017022a: add r0,pc
  0017022c: ldr.w r9,[r0,#0x0]
  00170230: mov r0,r6
  00170232: blx r9
  00170234: mov r0,r8
  00170236: mov r1,r6
  00170238: blx 0x0006ec80
  0017023c: mov r0,r6
  0017023e: mov r1,r8
  00170240: blx r9
  00170242: ldr r1,[sp,#0x20]
  00170244: mov r0,r8
  00170246: mov r2,r6
  00170248: blx 0x0006ec8c
  0017024c: mov r0,r5
  0017024e: mov r1,r8
  00170250: blx r9
  00170252: mov r0,r8
  00170254: mov r1,r5
  00170256: blx 0x0006ec80
  0017025a: mov r0,r5
  0017025c: mov r1,r8
  0017025e: blx r9
  00170260: ldrd r6,lr,[r4,#0x50]
  00170264: ldr r0,[r4,#0x58]
  00170266: str r0,[sp,#0xc]
  00170268: ldrd r5,r3,[r4,#0x5c]
  0017026c: ldrd r12,r1,[r4,#0x64]
  00170270: ldrd r9,r2,[r4,#0x6c]
  00170274: ldr r0,[r4,#0x8]
  00170276: str r1,[r4,#0x74]
  00170278: str r5,[r4,#0x78]
  0017027a: str r6,[r4,#0x7c]
  0017027c: strd r9,r3,[r4,#0x84]
  00170280: str.w lr,[r4,#0x8c]
  00170284: strd r2,r12,[r4,#0x94]
  00170288: ldr r1,[sp,#0xc]
  0017028a: str.w r1,[r4,#0x9c]
  0017028e: ldr r1,[r0,#0x3c]
  00170290: ldr.w r1,[r1,r10,lsl #0x2]
  00170294: cmp.w r1,#0x3e8
  00170298: bge 0x001702c0
  0017029a: vmov s0,r1
  0017029e: mov r0,r8
  001702a0: vcvt.f32.s32 s0,s0
  001702a4: ldr r1,[sp,#0x28]
  001702a6: vdiv.f32 s0,s0,s26
  001702aa: vmov r2,s0
  001702ae: vstr.32 s0,[sp]
  001702b2: mov r3,r2
  001702b4: blx 0x0006f814
  001702b8: mov r6,r8
  001702ba: ldrd r8,r9,[sp,#0x20]
  001702be: b 0x001700fc
  001702c0: ldrb.w r0,[r0,#0x4d]
  001702c4: mov r6,r8
  001702c6: ldrd r8,r9,[sp,#0x20]
  001702ca: cmp r0,#0x0
  001702cc: beq.w 0x001700fc
  001702d0: ldr r1,[sp,#0x28]
  001702d2: mov.w r0,#0x3f800000
  001702d6: str r0,[sp,#0x0]
  001702d8: mov r0,r6
  001702da: mov.w r2,#0x3f800000
  001702de: mov.w r3,#0x3f800000
  001702e2: blx 0x0006f814
  001702e6: ldr r0,[r4,#0x0]
  001702e8: ldr r1,[r0,#0x20]
  001702ea: mov r0,r4
  001702ec: blx r1
  001702ee: cmp r0,#0x0
  001702f0: bne.w 0x001700fc
  001702f4: ldr r0,[r4,#0x8]
  001702f6: movs r1,#0x0
  001702f8: strb.w r1,[r0,#0x4d]
  001702fc: b 0x001700fc
  001702fe: ldr r1,[r0,#0x14]
  00170300: cmp r9,r1
  00170302: itt ge
  00170304: mov.ge r1,#0x0
  00170306: strb.ge.w r1,[r0,#0x4c]
  0017030a: ldr r6,[sp,#0x4]
  0017030c: ldrb r0,[r4,#0x1d]
  0017030e: cbz r0,0x00170318
  00170310: ldr r0,[r4,#0x18]
  00170312: cbz r0,0x00170318
  00170314: blx 0x00072238
  00170318: movs r0,#0x0
  0017031a: str r0,[r4,#0x34]
  0017031c: ldr r0,[sp,#0xfc]
  0017031e: ldr r1,[r6,#0x0]
  00170320: subs r0,r1,r0
  00170322: itttt eq
  00170324: add.eq sp,#0x100
  00170326: vpop.eq {d8,d9,d10,d11,d12,d13,d14}
  0017032a: add.eq sp,#0x4
  0017032c: pop.eq.w {r8,r9,r10,r11}
  00170330: it eq
  00170332: pop.eq {r4,r5,r6,r7,pc}
  00170334: blx 0x0006e824
```
