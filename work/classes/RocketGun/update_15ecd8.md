# RocketGun::update
elf 0x15ecd8 body 1864
Sig: undefined __thiscall update(RocketGun * this, int param_1)

## decompile
```c

/* RocketGun::update(int) */

void __thiscall RocketGun::update(RocketGun *this,int param_1)

{
  float fVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  undefined8 *puVar5;
  undefined4 uVar6;
  undefined1 uVar7;
  int iVar8;
  int iVar9;
  Vector *pVVar10;
  int iVar11;
  code *pcVar12;
  uint uVar13;
  uint *puVar14;
  uint in_fpscr;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar19;
  AEMath aAStack_c4 [12];
  undefined4 local_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 uStack_a0;
  undefined4 local_98;
  undefined4 local_94;
  float fStack_90;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined8 local_60;
  float local_58;
  undefined4 local_50;
  undefined4 uStack_4c;
  float local_48;
  int local_44;
  
  piVar2 = *(int **)(DAT_0016f044 + 0x16ecf0);
  local_44 = *piVar2;
  local_50 = 0;
  uStack_4c = 0;
  local_48 = 0.0;
  Gun::update(*(int *)(this + 8));
  if ((this[0x1c] != (RocketGun)0x0) && (*(char *)(*(int *)(this + 8) + 0xa9) != '\0')) {
    piVar3 = (int *)Level::getPlayer();
    PlayerEgo::getPosition();
    local_60 = *(undefined8 *)(*(int *)(this + 8) + 0x7c);
    local_58 = *(float *)(*(int *)(this + 8) + 0x84) + DAT_0016f048;
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_6c,(Vector *)(*piVar3 + 4));
    AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_b8,(Vector *)&local_6c);
    AEGeometry::setPosition(*(AEGeometry **)(this + 0x18),(Vector *)&local_b8);
    fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 8) + 0x70),
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar15 = (float)VectorSignedToFloat((int)((fVar15 / DAT_0016f04c + 1.0) * DAT_0016f050),
                                        (byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::setScaling(fVar15 / DAT_0016f050,extraout_s1,DAT_0016f050);
    pVVar10 = *(Vector **)(this + 0x18);
    AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_6c);
    local_74 = 0x3f800000;
    local_78 = 0;
    local_70 = 0;
    AEGeometry::setDirection(pVVar10,(Vector *)&local_6c);
  }
  iVar4 = *(int *)(this + 8);
  this[0x1d] = *(RocketGun *)(iVar4 + 0xa9);
  iVar8 = DAT_0016f054;
  if (*(char *)(iVar4 + 0x4d) != '\0') {
    *(undefined4 *)(this + 0xc4) = 0xffffffff;
    *(undefined4 *)(this + 0xd4) = 0;
    *(undefined1 *)(iVar4 + 0x4d) = 0;
    puVar14 = *(uint **)(iVar8 + 0x16ee04);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar14);
    AbyssEngine::AEMath::MatrixSetTranslation
              ((Matrix *)&local_b8,
               *(float *)(*(int *)(*(int *)(this + 8) + 0xc) +
                          *(int *)(*(int *)(this + 8) + 0xa0) * 0xc + 8),extraout_s1_00,extraout_s2)
    ;
    puVar5 = (undefined8 *)
             (*(int *)(*(int *)(this + 8) + 0x18) + *(int *)(*(int *)(this + 8) + 0xa0) * 0xc);
    local_60 = *puVar5;
    local_58 = *(float *)(puVar5 + 1);
    uVar6 = AbyssEngine::PaintCanvas::TransformGetLocal(*puVar14);
    __aeabi_memcpy(&local_b8,uVar6,0x3c);
    local_6c = 0;
    uStack_68 = 0x3f800000;
    local_64 = 0;
    AbyssEngine::AEMath::VectorCross((Vector *)&local_78,(Vector *)&local_6c);
    AbyssEngine::AEMath::VectorNormalize(aAStack_c4,(Vector *)&local_78);
    pcVar12 = *(code **)(DAT_0016f058 + 0x16eea8);
    (*pcVar12)((Vector *)&local_78,aAStack_c4);
    AbyssEngine::AEMath::VectorCross((Vector *)aAStack_c4,(Vector *)&local_60);
    (*pcVar12)((Vector *)&local_6c,(Vector *)aAStack_c4);
    AbyssEngine::AEMath::VectorNormalize(aAStack_c4,(Vector *)&local_6c);
    (*pcVar12)((Vector *)&local_6c,aAStack_c4);
    local_a8 = local_74;
    local_98 = local_70;
    local_b8 = local_78;
    uStack_b4 = local_6c;
    local_b0 = (undefined4)local_60;
    local_a4 = uStack_68;
    uStack_a0 = local_60._4_4_;
    local_94 = local_64;
    fStack_90 = local_58;
    AbyssEngine::PaintCanvas::TransformSetLocal(*puVar14,*(Matrix **)(this + 0x10));
    if (*(int *)(this + 0xd8) == 0) {
      if ((*(int *)(this + 0xd0) - 4U < 2) || (*(int *)(this + 0xd0) == 0x28)) {
        ParticleSystemManager::enableSystemRender
                  (*(int *)(**(int **)(this + 0xb0) + 0x80),SUB41(*(undefined4 *)(this + 0xcc),0));
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(**(int **)(this + 0xb0) + 0x80),SUB41(*(undefined4 *)(this + 0xcc),0));
      }
      else {
        if (*(int *)(*(int *)(this + 8) + 0x58) == 0xe8) {
          uVar7 = (undefined1)*(undefined4 *)(this + 0xcc);
          iVar8 = *(int *)(**(int **)(this + 0xb0) + 0x9c);
        }
        else {
          uVar7 = (undefined1)*(undefined4 *)(this + 0xcc);
          iVar8 = *(int *)(**(int **)(this + 0xb0) + 0x84);
        }
        ParticleSystemManager::enableSystemEmit(iVar8,(bool)uVar7);
      }
    }
    else {
      iVar8 = *(int *)(*(int *)(this + 8) + 0xa0);
      in_fpscr = in_fpscr & 0xfffffff |
                 (uint)(*(float *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar8 * 0xc + 8) ==
                       DAT_0016f05c) << 0x1e;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        AbyssEngine::AEMath::Matrix::operator=
                  ((Matrix *)(*(int *)(*(int *)(this + 0xd8) + 4) + iVar8 * 0x3c),
                   (Matrix *)&local_b8);
        ParticleSystemManager::resetSystem
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4));
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(this + 0xe4),
                   SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(this + 0xdc) + 4) +
                          *(int *)(*(int *)(this + 8) + 0xa0) * 4),0));
        ParticleSystemManager::enableSystemRender
                  (*(int *)(this + 0xe4),
                   SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(this + 0xdc) + 4) +
                          *(int *)(*(int *)(this + 8) + 0xa0) * 4),0));
        *(undefined4 *)
         (*(int *)(*(int *)(this + 0xe0) + 4) + *(int *)(*(int *)(this + 8) + 0xa0) * 4) = 0;
      }
      else {
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(this + 0xe4),
                   SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xdc) + 4) + iVar8 * 4),0));
        ParticleSystemManager::resetSystem
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4));
        *(undefined4 *)
         (*(int *)(*(int *)(this + 0xe0) + 4) + *(int *)(*(int *)(this + 8) + 0xa0) * 4) = 0;
      }
    }
    iVar4 = *(int *)(this + 8);
    if ((*(int *)(iVar4 + 0x5c) == 0x28) && (0 < *(int *)(iVar4 + 0xa0))) {
      *(undefined1 *)(iVar4 + 0x4d) = 1;
      *(int *)(iVar4 + 0xa0) = *(int *)(iVar4 + 0xa0) + -1;
      (**(code **)(*(int *)this + 0x10))(this,param_1);
      Gun::update(*(int *)(this + 8));
      iVar4 = *(int *)(this + 8);
    }
  }
  if (((*(char *)(iVar4 + 0x4c) == '\0') || (*(int *)(iVar4 + 8) != 1)) ||
     (in_fpscr = in_fpscr & 0xfffffff |
                 (uint)(*(float *)(*(int *)(iVar4 + 0xc) + 8) == DAT_0016f440) << 0x1e,
     (byte)(in_fpscr >> 0x1e) == 0)) {
    if (((0 < *(int *)(this + 0xd4)) && (*(int *)(iVar4 + 8) == 1)) &&
       ((in_fpscr = in_fpscr & 0xfffffff |
                    (uint)(*(float *)(*(int *)(iVar4 + 0xc) + 8) == DAT_0016f440) << 0x1e,
        (byte)(in_fpscr >> 0x1e) != 0 &&
        (iVar8 = *(int *)(this + 0xd4) - param_1, *(int *)(this + 0xd4) = iVar8, iVar8 < 1)))) {
      *(undefined4 *)(this + 0xd4) = 0;
      ParticleSystemManager::enableSystemRender
                (*(int *)(this + 0xe4),SUB41(*(undefined4 *)(this + 0xcc),0));
    }
  }
  else {
    *(undefined1 *)(iVar4 + 0x4c) = 0;
    if ((*(int *)(this + 0xd0) - 4U < 2) || (*(int *)(this + 0xd0) == 0x28)) {
      if (*(int *)(this + 0xd4) == 0) {
        *(undefined4 *)(this + 0xd4) = 2000;
      }
      uVar7 = (undefined1)*(undefined4 *)(this + 0xcc);
      iVar8 = *(int *)(**(int **)(this + 0xb0) + 0x80);
    }
    else {
      uVar7 = (undefined1)*(undefined4 *)(this + 0xcc);
      if (*(int *)(iVar4 + 0x58) == 0xe8) {
        iVar8 = *(int *)(**(int **)(this + 0xb0) + 0x9c);
      }
      else {
        iVar8 = *(int *)(**(int **)(this + 0xb0) + 0x84);
      }
    }
    ParticleSystemManager::enableSystemEmit(iVar8,(bool)uVar7);
  }
  fVar1 = DAT_0016f444;
  fVar15 = DAT_0016f440;
  fVar16 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  iVar8 = 0;
  uVar13 = 0;
  fVar17 = fVar16;
  do {
    if (*(uint *)(*(int *)(this + 8) + 8) <= uVar13) {
      if (*piVar2 == local_44) {
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    iVar4 = *(int *)(*(int *)(this + 8) + 0xc);
    fVar17 = (float)AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar17);
    AbyssEngine::AEMath::operator*((Vector *)&local_60,fVar17);
    AbyssEngine::AEMath::operator+((Vector *)&local_b8,(Vector *)(iVar4 + iVar8));
    fVar17 = (float)AbyssEngine::AEMath::Vector::operator=((Vector *)&local_50,(Vector *)&local_b8);
    iVar4 = *(int *)(this + 8);
    if (*(char *)(iVar4 + 0x4c) != '\0') {
      in_fpscr = in_fpscr & 0xfffffff | (uint)(local_48 == fVar15) << 0x1e;
      fVar17 = local_48;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        if ((this[0xc0] != (RocketGun)0x0) &&
           ((iVar9 = *(int *)(this + 0xd8), iVar9 != 0 ||
            (iVar9 = *(int *)(*(int *)(iVar4 + 0x3c) + uVar13 * 4),
            iVar9 < *(int *)(iVar4 + 0x44) + -1000)))) {
          fVar17 = (float)seekEnemy((int)this,iVar9);
          iVar4 = *(int *)(this + 8);
        }
        if (*(int *)(iVar4 + 0x5c) == 0x28) {
          iVar9 = *(int *)(iVar4 + 0x44);
          iVar11 = *(int *)(iVar4 + 0x3c);
          uVar6 = __aeabi_uidiv(iVar9 * uVar13,*(undefined4 *)(iVar4 + 8));
          fVar17 = (float)VectorSignedToFloat(iVar9 - *(int *)(iVar11 + uVar13 * 4),
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar19 = (float)VectorUnsignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
          fVar17 = (float)AbyssEngine::AEMath::Sinf((fVar19 + fVar17) * fVar1);
          fVar18 = (float)VectorSignedToFloat(*(int *)(*(int *)(this + 8) + 0x44) -
                                              *(int *)(*(int *)(*(int *)(this + 8) + 0x3c) +
                                                      uVar13 * 4),(byte)(in_fpscr >> 0x16) & 3);
          fVar18 = (float)AbyssEngine::AEMath::Cosf((fVar19 + fVar18) * fVar1);
          AbyssEngine::AEMath::VectorCross
                    ((Vector *)&local_60,(Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar8));
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_b8,(Vector *)&local_60);
          pcVar12 = *(code **)(DAT_0016f448 + 0x16f2d2);
          (*pcVar12)(&local_6c,(AEMath *)&local_b8,fVar17 + fVar17);
          (*pcVar12)((Vector *)&local_60,&local_6c,fVar16);
          AbyssEngine::AEMath::Vector::operator+=
                    ((Vector *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar8),(Vector *)&local_60);
          (*pcVar12)(&local_6c,*(int *)(*(int *)(this + 8) + 0x24) + iVar8,fVar18 + fVar18);
          (*pcVar12)((Vector *)&local_60,&local_6c,fVar16);
          fVar17 = (float)AbyssEngine::AEMath::Vector::operator+=
                                    ((Vector *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar8),
                                     (Vector *)&local_60);
        }
      }
      else {
        if (*(int *)(this + 0xd8) == 0) goto LAB_0016f3c6;
        fVar17 = (float)ParticleSystemManager::enableSystemEmit
                                  (*(int *)(this + 0xe4),
                                   SUB41(*(undefined4 *)
                                          (*(int *)(*(int *)(this + 0xdc) + 4) + uVar13 * 4),0));
        if (*(int *)(*(int *)(*(int *)(this + 0xe0) + 4) + uVar13 * 4) == 0) {
          *(undefined4 *)(*(int *)(*(int *)(this + 0xe0) + 4) + uVar13 * 4) = 2000;
        }
      }
      if (*(int *)(this + 0xd8) != 0) {
        iVar4 = *(int *)(*(int *)(this + 8) + 0xc);
        fVar17 = (float)AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar17);
        AbyssEngine::AEMath::operator*((Vector *)&local_60,fVar17);
        AbyssEngine::AEMath::operator+((Vector *)&local_b8,(Vector *)(iVar4 + iVar8));
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_50,(Vector *)&local_b8);
        AbyssEngine::AEMath::MatrixSetTranslation
                  ((Matrix *)&local_b8,local_48,extraout_s1_01,extraout_s2_00);
        AbyssEngine::AEMath::VectorNormalize
                  ((AEMath *)&local_60,(Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar8));
        iVar4 = iVar8 * 5 + *(int *)(*(int *)(this + 0xd8) + 4);
        *(undefined4 *)(iVar4 + 8) = (undefined4)local_60;
        *(undefined4 *)(iVar4 + 0x18) = local_60._4_4_;
        *(float *)(iVar4 + 0x28) = local_58;
        fVar17 = (float)AbyssEngine::AEMath::MatrixSetTranslation
                                  ((Matrix *)&local_b8,local_48,extraout_s1_02,extraout_s2_01);
      }
    }
LAB_0016f3c6:
    if ((*(int *)(this + 0xe0) != 0) &&
       (in_fpscr = in_fpscr & 0xfffffff | (uint)(local_48 == fVar15) << 0x1e, fVar17 = local_48,
       (byte)(in_fpscr >> 0x1e) != 0)) {
      iVar4 = *(int *)(*(int *)(this + 0xe0) + 4);
      iVar9 = *(int *)(iVar4 + uVar13 * 4);
      if ((0 < iVar9) && (iVar9 = iVar9 - param_1, *(int *)(iVar4 + uVar13 * 4) = iVar9, iVar9 < 1))
      {
        *(undefined4 *)(iVar4 + uVar13 * 4) = 0;
        fVar17 = (float)ParticleSystemManager::enableSystemRender
                                  (*(int *)(this + 0xe4),
                                   SUB41(*(undefined4 *)
                                          (*(int *)(*(int *)(this + 0xdc) + 4) + uVar13 * 4),0));
      }
    }
    iVar8 = iVar8 + 0xc;
    uVar13 = uVar13 + 1;
  } while( true );
}

```

## target disasm
```
  0016ecd8: push {r4,r5,r6,r7,lr}
  0016ecda: add r7,sp,#0xc
  0016ecdc: push {r7,r8,r9,r10,r11}
  0016ece0: vpush {d8,d9,d10}
  0016ece4: sub sp,#0xa0
  0016ece6: mov r10,r0
  0016ece8: ldr r0,[0x0016f044]
  0016ecea: mov r4,r1
  0016ecec: add r0,pc
  0016ecee: ldr r0,[r0,#0x0]
  0016ecf0: str r0,[sp,#0x4]
  0016ecf2: ldr r0,[r0,#0x0]
  0016ecf4: str r0,[sp,#0x9c]
  0016ecf6: movs r0,#0x0
  0016ecf8: strd r0,r0,[sp,#0x90]
  0016ecfc: str r0,[sp,#0x98]
  0016ecfe: ldr.w r0,[r10,#0x8]
  0016ed02: blx 0x0007780c
  0016ed06: ldrb.w r0,[r10,#0x1c]
  0016ed0a: cmp r0,#0x0
  0016ed0c: beq 0x0016edda
  0016ed0e: ldr.w r0,[r10,#0x8]
  0016ed12: ldrb.w r0,[r0,#0xa9]
  0016ed16: cmp r0,#0x0
  0016ed18: beq 0x0016edda
  0016ed1a: ldr.w r0,[r10,#0xc]
  0016ed1e: blx 0x00072034
  0016ed22: add.w r8,sp,#0x28
  0016ed26: mov r5,r0
  0016ed28: mov r1,r5
  0016ed2a: mov r0,r8
  0016ed2c: blx 0x0007264c
  0016ed30: ldr.w r0,[r10,#0x8]
  0016ed34: add r6,sp,#0x74
  0016ed36: vldr.32 s0,[pc,#0x310]
  0016ed3a: add r2,sp,#0x80
  0016ed3c: ldr.w r1,[r0,#0x84]
  0016ed40: str r1,[sp,#0x88]
  0016ed42: vldr.32 s2,[sp,#0x88]
  0016ed46: vldr.64 d16,[r0,#0x7c]
  0016ed4a: vadd.f32 s0,s2,s0
  0016ed4e: vstr.64 d16,[sp,#0x80]
  0016ed52: vstr.32 s0,[sp,#0x88]
  0016ed56: ldr r0,[r5,#0x0]
  0016ed58: adds r1,r0,#0x4
  0016ed5a: mov r0,r6
  0016ed5c: blx 0x0006f694
  0016ed60: mov r0,r8
  0016ed62: mov r1,r6
  0016ed64: blx 0x00073534
  0016ed68: ldr.w r0,[r10,#0x18]
  0016ed6c: mov r1,r8
  0016ed6e: blx 0x00072148
  0016ed72: ldr.w r1,[r10,#0x8]
  0016ed76: ldr.w r0,[r10,#0x18]
  0016ed7a: vldr.32 s2,[pc,#0x2d0]
  0016ed7e: vldr.32 s0,[r1,#0x70]
  0016ed82: vcvt.f32.s32 s0,s0
  0016ed86: vdiv.f32 s0,s0,s2
  0016ed8a: vmov.f32 s2,0x3f800000
  0016ed8e: vadd.f32 s0,s0,s2
  0016ed92: vldr.32 s2,[pc,#0x2bc]
  0016ed96: vmul.f32 s0,s0,s2
  0016ed9a: vcvt.s32.f32 s0,s0
  0016ed9e: vcvt.f32.s32 s0,s0
  0016eda2: vdiv.f32 s0,s0,s2
  0016eda6: vmov r1,s0
  0016edaa: mov r2,r1
  0016edac: mov r3,r1
  0016edae: blx 0x000727b4
  0016edb2: ldr r0,[r5,#0x0]
  0016edb4: add.w r8,sp,#0x74
  0016edb8: ldr.w r6,[r10,#0x18]
  0016edbc: adds r1,r0,#0x4
  0016edbe: mov r0,r8
  0016edc0: blx 0x0006f4c0
  0016edc4: mov.w r0,#0x3f800000
  0016edc8: add r2,sp,#0x68
  0016edca: str r0,[sp,#0x6c]
  0016edcc: movs r0,#0x0
  0016edce: str r0,[sp,#0x68]
  0016edd0: mov r1,r8
  0016edd2: str r0,[sp,#0x70]
  0016edd4: mov r0,r6
  0016edd6: blx 0x000726ac
  0016edda: ldr.w r0,[r10,#0x8]
  0016edde: ldrb.w r1,[r0,#0xa9]
  0016ede2: strb.w r1,[r10,#0x1d]
  0016ede6: ldrb.w r1,[r0,#0x4d]
  0016edea: strd r4,r10,[sp,#0x10]
  0016edee: cmp r1,#0x0
  0016edf0: beq.w 0x0016f0aa
  0016edf4: ldr r1,[0x0016f054]
  0016edf6: mov.w r2,#0xffffffff
  0016edfa: movs r4,#0x0
  0016edfc: str.w r2,[r10,#0xc4]
  0016ee00: add r1,pc
  0016ee02: str.w r4,[r10,#0xd4]
  0016ee06: strb.w r4,[r0,#0x4d]
  0016ee0a: ldr.w r11,[r1,#0x0]
  0016ee0e: ldr.w r1,[r10,#0x10]
  0016ee12: ldr.w r0,[r11,#0x0]
  0016ee16: blx 0x000720c4
  0016ee1a: mov r1,r0
  0016ee1c: ldr.w r0,[r10,#0x8]
  0016ee20: ldr r3,[r0,#0xc]
  0016ee22: ldr.w r0,[r0,#0xa0]
  0016ee26: add.w r0,r0,r0, lsl #0x1
  0016ee2a: ldr.w r2,[r3,r0,lsl #0x2]
  0016ee2e: add.w r0,r3,r0, lsl #0x2
  0016ee32: ldr r3,[r0,#0x4]
  0016ee34: vldr.32 s0,[r0,#0x8]
  0016ee38: add r0,sp,#0x28
  0016ee3a: vstr.32 s0,[sp]
  0016ee3e: blx 0x0006f820
  0016ee42: ldr.w r0,[r10,#0x8]
  0016ee46: ldr r1,[r0,#0x18]
  0016ee48: ldr.w r0,[r0,#0xa0]
  0016ee4c: add.w r0,r0,r0, lsl #0x1
  0016ee50: add.w r0,r1,r0, lsl #0x2
  0016ee54: vldr.64 d16,[r0]
  0016ee58: ldr r0,[r0,#0x8]
  0016ee5a: str r0,[sp,#0x88]
  0016ee5c: vstr.64 d16,[sp,#0x80]
  0016ee60: ldr.w r1,[r10,#0x10]
  0016ee64: ldr.w r0,[r11,#0x0]
  0016ee68: blx 0x000720c4
  0016ee6c: add.w r9,sp,#0x28
  0016ee70: mov r1,r0
  0016ee72: movs r2,#0x3c
  0016ee74: mov r0,r9
  0016ee76: blx 0x0006f1e4
  0016ee7a: mov.w r0,#0x3f800000
  0016ee7e: add.w r10,sp,#0x74
  0016ee82: strd r4,r0,[sp,#0x74]
  0016ee86: add r5,sp,#0x80
  0016ee88: str r4,[sp,#0x7c]
  0016ee8a: add r4,sp,#0x68
  0016ee8c: mov r1,r10
  0016ee8e: mov r2,r5
  0016ee90: mov r0,r4
  0016ee92: blx 0x0006ec8c
  0016ee96: add r6,sp,#0x1c
  0016ee98: mov r1,r4
  0016ee9a: mov r0,r6
  0016ee9c: blx 0x0006ec80
  0016eea0: ldr r0,[0x0016f058]
  0016eea2: mov r1,r6
  0016eea4: add r0,pc
  0016eea6: ldr.w r8,[r0,#0x0]
  0016eeaa: mov r0,r4
  0016eeac: blx r8
  0016eeae: add r6,sp,#0x1c
  0016eeb0: mov r1,r5
  0016eeb2: mov r2,r4
  0016eeb4: mov r0,r6
  0016eeb6: blx 0x0006ec8c
  0016eeba: mov r0,r10
  0016eebc: mov r1,r6
  0016eebe: blx r8
  0016eec0: add r4,sp,#0x1c
  0016eec2: mov r1,r10
  0016eec4: mov r0,r4
  0016eec6: blx 0x0006ec80
  0016eeca: mov r0,r10
  0016eecc: mov r1,r4
  0016eece: ldr.w r10,[sp,#0x14]
  0016eed2: blx r8
  0016eed4: add r3,sp,#0x68
  0016eed6: ldmia r3,{r1,r2,r3}
  0016eed8: str r2,[sp,#0x38]
  0016eeda: ldr.w r0,[r11,#0x0]
  0016eede: ldrd r2,r6,[sp,#0x74]
  0016eee2: str r3,[sp,#0x48]
  0016eee4: add r3,sp,#0x80
  0016eee6: ldr r4,[sp,#0x7c]
  0016eee8: strd r1,r2,[sp,#0x28]
  0016eeec: ldmia r3,{r1,r2,r3}
  0016eeee: str r1,[sp,#0x30]
  0016eef0: strd r6,r2,[sp,#0x3c]
  0016eef4: mov r2,r9
  0016eef6: strd r4,r3,[sp,#0x4c]
  0016eefa: ldr.w r1,[r10,#0x10]
  0016eefe: blx 0x000721c0
  0016ef02: ldr.w r0,[r10,#0xd8]
  0016ef06: cbz r0,0x0016ef70
  0016ef08: ldr.w r1,[r10,#0x8]
  0016ef0c: vldr.32 s0,[pc,#0x14c]
  0016ef10: ldr r2,[r1,#0xc]
  0016ef12: ldr.w r1,[r1,#0xa0]
  0016ef16: add.w r3,r1,r1, lsl #0x1
  0016ef1a: add.w r2,r2,r3, lsl #0x2
  0016ef1e: vldr.32 s2,[r2,#0x8]
  0016ef22: vcmpe.f32 s2,s0
  0016ef26: vmrs apsr,fpscr
  0016ef2a: bne 0x0016efaa
  0016ef2c: ldr.w r2,[r10,#0xdc]
  0016ef30: movs r4,#0x0
  0016ef32: ldr.w r0,[r10,#0xe4]
  0016ef36: ldr r2,[r2,#0x4]
  0016ef38: ldr.w r1,[r2,r1,lsl #0x2]
  0016ef3c: movs r2,#0x0
  0016ef3e: blx 0x000723c4
  0016ef42: ldr.w r1,[r10,#0x8]
  0016ef46: ldr.w r2,[r10,#0xdc]
  0016ef4a: ldr.w r0,[r10,#0xe4]
  0016ef4e: ldr.w r1,[r1,#0xa0]
  0016ef52: ldr r2,[r2,#0x4]
  0016ef54: ldr.w r1,[r2,r1,lsl #0x2]
  0016ef58: blx 0x00077968
  0016ef5c: ldr.w r0,[r10,#0x8]
  0016ef60: ldr.w r1,[r10,#0xe0]
  0016ef64: ldr.w r0,[r0,#0xa0]
  0016ef68: ldr r1,[r1,#0x4]
  0016ef6a: str.w r4,[r1,r0,lsl #0x2]
  0016ef6e: b 0x0016efa6
  0016ef70: ldr.w r0,[r10,#0xd0]
  0016ef74: subs r1,r0,#0x4
  0016ef76: cmp r1,#0x2
  0016ef78: bcc 0x0016ef7e
  0016ef7a: cmp r0,#0x28
  0016ef7c: bne 0x0016f024
  0016ef7e: ldr.w r0,[r10,#0xb0]
  0016ef82: movs r2,#0x1
  0016ef84: ldr.w r1,[r10,#0xcc]
  0016ef88: ldr r0,[r0,#0x0]
  0016ef8a: ldr.w r0,[r0,#0x80]
  0016ef8e: blx 0x00072a24
  0016ef92: ldr.w r0,[r10,#0xb0]
  0016ef96: movs r2,#0x1
  0016ef98: ldr.w r1,[r10,#0xcc]
  0016ef9c: ldr r0,[r0,#0x0]
  0016ef9e: ldr.w r0,[r0,#0x80]
  0016efa2: blx 0x000723c4
  0016efa6: ldr r4,[sp,#0x10]
  0016efa8: b 0x0016f072
  0016efaa: ldr r0,[r0,#0x4]
  0016efac: rsb r1,r1,r1, lsl #0x4
  0016efb0: add.w r0,r0,r1, lsl #0x2
  0016efb4: add r1,sp,#0x28
  0016efb6: blx 0x0006f148
  0016efba: ldr.w r1,[r10,#0x8]
  0016efbe: ldr.w r2,[r10,#0xdc]
  0016efc2: ldr.w r0,[r10,#0xe4]
  0016efc6: ldr.w r1,[r1,#0xa0]
  0016efca: ldr r2,[r2,#0x4]
  0016efcc: ldr.w r1,[r2,r1,lsl #0x2]
  0016efd0: blx 0x00077968
  0016efd4: ldr.w r1,[r10,#0x8]
  0016efd8: ldr.w r2,[r10,#0xdc]
  0016efdc: ldr.w r0,[r10,#0xe4]
  0016efe0: ldr.w r1,[r1,#0xa0]
  0016efe4: ldr r2,[r2,#0x4]
  0016efe6: ldr.w r1,[r2,r1,lsl #0x2]
  0016efea: movs r2,#0x1
  0016efec: blx 0x000723c4
  0016eff0: ldr.w r1,[r10,#0x8]
  0016eff4: ldr.w r2,[r10,#0xdc]
  0016eff8: ldr.w r0,[r10,#0xe4]
  0016effc: ldr.w r1,[r1,#0xa0]
  0016f000: ldr r2,[r2,#0x4]
  0016f002: ldr.w r1,[r2,r1,lsl #0x2]
  0016f006: movs r2,#0x1
  0016f008: blx 0x00072a24
  0016f00c: ldr.w r0,[r10,#0x8]
  0016f010: movs r2,#0x0
  0016f012: ldr.w r1,[r10,#0xe0]
  0016f016: ldr r4,[sp,#0x10]
  0016f018: ldr.w r0,[r0,#0xa0]
  0016f01c: ldr r1,[r1,#0x4]
  0016f01e: str.w r2,[r1,r0,lsl #0x2]
  0016f022: b 0x0016f072
  0016f024: ldr.w r0,[r10,#0xb0]
  0016f028: ldr.w r1,[r10,#0x8]
  0016f02c: ldr r4,[sp,#0x10]
  0016f02e: ldr r0,[r0,#0x0]
  0016f030: ldr r2,[r1,#0x58]
  0016f032: cmp r2,#0xe8
  0016f034: bne 0x0016f060
  0016f036: ldr.w r1,[r10,#0xcc]
  0016f03a: movs r2,#0x1
  0016f03c: ldr.w r0,[r0,#0x9c]
  0016f040: b 0x0016f06e
  0016f060: ldr.w r1,[r10,#0xcc]
  0016f064: subs r2,#0xb3
  0016f066: ldr.w r0,[r0,#0x84]
  0016f06a: it ne
  0016f06c: mov.ne r2,#0x1
  0016f06e: blx 0x000723c4
  0016f072: ldr.w r0,[r10,#0x8]
  0016f076: ldr r1,[r0,#0x5c]
  0016f078: cmp r1,#0x28
  0016f07a: bne 0x0016f0aa
  0016f07c: ldr.w r1,[r0,#0xa0]
  0016f080: cmp r1,#0x1
  0016f082: blt 0x0016f0aa
  0016f084: movs r2,#0x1
  0016f086: subs r1,#0x1
  0016f088: strb.w r2,[r0,#0x4d]
  0016f08c: str.w r1,[r0,#0xa0]
  0016f090: mov r1,r4
  0016f092: ldr.w r0,[r10,#0x0]
  0016f096: ldr r2,[r0,#0x10]
  0016f098: mov r0,r10
  0016f09a: blx r2
  0016f09c: ldr.w r0,[r10,#0x8]
  0016f0a0: mov r1,r4
  0016f0a2: blx 0x0007780c
  0016f0a6: ldr.w r0,[r10,#0x8]
  0016f0aa: ldrb.w r1,[r0,#0x4c]
  0016f0ae: cbz r1,0x0016f0fc
  0016f0b0: ldr r1,[r0,#0x8]
  0016f0b2: cmp r1,#0x1
  0016f0b4: bne 0x0016f0fc
  0016f0b6: ldr r1,[r0,#0xc]
  0016f0b8: vldr.32 s0,[pc,#0x384]
  0016f0bc: vldr.32 s2,[r1,#0x8]
  0016f0c0: vcmpe.f32 s2,s0
  0016f0c4: vmrs apsr,fpscr
  0016f0c8: bne 0x0016f0fc
  0016f0ca: movs r1,#0x0
  0016f0cc: strb.w r1,[r0,#0x4c]
  0016f0d0: ldr.w r1,[r10,#0xd0]
  0016f0d4: subs r2,r1,#0x4
  0016f0d6: cmp r2,#0x2
  0016f0d8: bcc 0x0016f0de
  0016f0da: cmp r1,#0x28
  0016f0dc: bne 0x0016f13e
  0016f0de: ldr.w r0,[r10,#0xd4]
  0016f0e2: cbnz r0,0x0016f0ec
  0016f0e4: mov.w r0,#0x7d0
  0016f0e8: str.w r0,[r10,#0xd4]
  0016f0ec: ldr.w r0,[r10,#0xb0]
  0016f0f0: ldr.w r1,[r10,#0xcc]
  0016f0f4: ldr r0,[r0,#0x0]
  0016f0f6: ldr.w r0,[r0,#0x80]
  0016f0fa: b 0x0016f156
  0016f0fc: ldr.w r1,[r10,#0xd4]
  0016f100: cmp r1,#0x1
  0016f102: blt 0x0016f15c
  0016f104: ldr r2,[r0,#0x8]
  0016f106: cmp r2,#0x1
  0016f108: bne 0x0016f15c
  0016f10a: ldr r0,[r0,#0xc]
  0016f10c: vldr.32 s0,[pc,#0x330]
  0016f110: vldr.32 s2,[r0,#0x8]
  0016f114: vcmpe.f32 s2,s0
  0016f118: vmrs apsr,fpscr
  0016f11c: bne 0x0016f15c
  0016f11e: subs r0,r1,r4
  0016f120: str.w r0,[r10,#0xd4]
  0016f124: cmp r0,#0x0
  0016f126: bgt 0x0016f15c
  0016f128: ldr.w r1,[r10,#0xcc]
  0016f12c: movs r2,#0x0
  0016f12e: ldr.w r0,[r10,#0xe4]
  0016f132: str.w r2,[r10,#0xd4]
  0016f136: movs r2,#0x0
  0016f138: blx 0x00072a24
  0016f13c: b 0x0016f15c
  0016f13e: ldr.w r1,[r10,#0xb0]
  0016f142: ldr r2,[r0,#0x58]
  0016f144: ldr r0,[r1,#0x0]
  0016f146: cmp r2,#0xe8
  0016f148: ldr.w r1,[r10,#0xcc]
  0016f14c: ite eq
  0016f14e: ldr.eq.w r0,[r0,#0x9c]
  0016f152: ldr.ne.w r0,[r0,#0x84]
  0016f156: movs r2,#0x0
  0016f158: blx 0x000723c4
  0016f15c: vmov s0,r4
  0016f160: vldr.32 s16,[pc,#0x2dc]
  0016f164: vldr.32 s18,[pc,#0x2dc]
  0016f168: movs r4,#0x0
  0016f16a: vcvt.f32.s32 s0,s0
  0016f16e: mov.w r11,#0x0
  0016f172: mov.w r8,#0x0
  0016f176: vmov r0,s0
  0016f17a: str r0,[sp,#0x18]
  0016f17c: b 0x0016f414
  0016f17e: ldr r6,[r0,#0xc]
  0016f180: add.w r10,sp,#0x74
  0016f184: ldr r0,[r0,#0x18]
  0016f186: mov r9,r4
  0016f188: ldr r2,[sp,#0x18]
  0016f18a: add.w r1,r0,r11
  0016f18e: mov r0,r10
  0016f190: blx 0x0006ec74
  0016f194: add r5,sp,#0x80
  0016f196: mov r1,r10
  0016f198: mov.w r2,#0x3f000000
  0016f19c: ldr.w r10,[sp,#0x14]
  0016f1a0: mov r0,r5
  0016f1a2: blx 0x0006ec74
  0016f1a6: add.w r1,r6,r11
  0016f1aa: add r6,sp,#0x28
  0016f1ac: mov r2,r5
  0016f1ae: mov r0,r6
  0016f1b0: blx 0x0006f1cc
  0016f1b4: add r0,sp,#0x90
  0016f1b6: mov r1,r6
  0016f1b8: blx 0x0006eb3c
  0016f1bc: ldr.w r0,[r10,#0x8]
  0016f1c0: ldrb.w r1,[r0,#0x4c]
  0016f1c4: cmp r1,#0x0
  0016f1c6: beq.w 0x0016f3c6
  0016f1ca: vldr.32 s0,[sp,#0x98]
  0016f1ce: vcmpe.f32 s0,s16
  0016f1d2: vmrs apsr,fpscr
  0016f1d6: bne 0x0016f210
  0016f1d8: ldr.w r0,[r10,#0xd8]
  0016f1dc: cmp r0,#0x0
  0016f1de: beq.w 0x0016f3c6
  0016f1e2: ldr.w r1,[r10,#0xdc]
  0016f1e6: movs r2,#0x0
  0016f1e8: ldr.w r0,[r10,#0xe4]
  0016f1ec: ldr r1,[r1,#0x4]
  0016f1ee: ldr.w r1,[r1,r8,lsl #0x2]
  0016f1f2: blx 0x000723c4
  0016f1f6: ldr.w r0,[r10,#0xe0]
  0016f1fa: ldr r0,[r0,#0x4]
  0016f1fc: ldr.w r1,[r0,r8,lsl #0x2]
  0016f200: cmp r1,#0x0
  0016f202: bne.w 0x0016f32c
  0016f206: mov.w r1,#0x7d0
  0016f20a: str.w r1,[r0,r8,lsl #0x2]
  0016f20e: b 0x0016f32c
  0016f210: ldrb.w r1,[r10,#0xc0]
  0016f214: cbz r1,0x0016f238
  0016f216: ldr.w r1,[r10,#0xd8]
  0016f21a: cbnz r1,0x0016f22c
  0016f21c: ldr r1,[r0,#0x3c]
  0016f21e: ldr r2,[r0,#0x44]
  0016f220: ldr.w r1,[r1,r8,lsl #0x2]
  0016f224: sub.w r2,r2,#0x3e8
  0016f228: cmp r1,r2
  0016f22a: bge 0x0016f238
  0016f22c: mov r0,r10
  0016f22e: mov r2,r8
  0016f230: blx 0x00077974
  0016f234: ldr.w r0,[r10,#0x8]
  0016f238: ldr r1,[r0,#0x5c]
  0016f23a: cmp r1,#0x28
  0016f23c: bne 0x0016f32c
  0016f23e: ldr r4,[r0,#0x44]
  0016f240: ldr r1,[r0,#0x8]
  0016f242: ldr r6,[r0,#0x3c]
  0016f244: mul r0,r4,r8
  0016f248: blx 0x0006ec2c
  0016f24c: ldr.w r1,[r6,r8,lsl #0x2]
  0016f250: vmov s2,r0
  0016f254: subs r1,r4,r1
  0016f256: ldr r4,[sp,#0x18]
  0016f258: vmov s0,r1
  0016f25c: vcvt.f32.s32 s0,s0
  0016f260: vcvt.f32.u32 s20,s2
  0016f264: vadd.f32 s0,s20,s0
  0016f268: vmul.f32 s0,s0,s18
  0016f26c: vmov r0,s0
  0016f270: blx 0x0006f1a8
  0016f274: str r0,[sp,#0x8]
  0016f276: ldr.w r0,[r10,#0x8]
  0016f27a: ldr r1,[r0,#0x3c]
  0016f27c: ldr r0,[r0,#0x44]
  0016f27e: ldr.w r1,[r1,r8,lsl #0x2]
  0016f282: subs r0,r0,r1
  0016f284: vmov s0,r0
  0016f288: vcvt.f32.s32 s0,s0
  0016f28c: vadd.f32 s0,s20,s0
  0016f290: vmul.f32 s0,s0,s18
  0016f294: vmov r0,s0
  0016f298: blx 0x0006f49c
  0016f29c: str r0,[sp,#0xc]
  0016f29e: add r6,sp,#0x80
  0016f2a0: ldr.w r0,[r10,#0x8]
  0016f2a4: ldr r1,[r0,#0x18]
  0016f2a6: ldr r0,[r0,#0x24]
  0016f2a8: add r1,r11
  0016f2aa: add.w r2,r0,r11
  0016f2ae: mov r0,r6
  0016f2b0: blx 0x0006ec8c
  0016f2b4: add r5,sp,#0x28
  0016f2b6: mov r1,r6
  0016f2b8: mov r0,r5
  0016f2ba: blx 0x0006ec80
  0016f2be: ldr r0,[sp,#0x8]
  0016f2c0: add r6,sp,#0x74
  0016f2c2: mov r1,r5
  0016f2c4: vmov s0,r0
  0016f2c8: ldr r0,[0x0016f448]
  0016f2ca: vadd.f32 s0,s0,s0
  0016f2ce: add r0,pc
  0016f2d0: ldr.w r10,[r0,#0x0]
  0016f2d4: mov r0,r6
  0016f2d6: vmov r2,s0
  0016f2da: blx r10
  0016f2dc: add r5,sp,#0x80
  0016f2de: mov r1,r6
  0016f2e0: mov r2,r4
  0016f2e2: mov r0,r5
  0016f2e4: blx r10
  0016f2e6: ldr r0,[sp,#0x14]
  0016f2e8: mov r1,r5
  0016f2ea: ldr r0,[r0,#0x8]
  0016f2ec: ldr r0,[r0,#0xc]
  0016f2ee: add r0,r11
  0016f2f0: blx 0x00073534
  0016f2f4: ldr r0,[sp,#0xc]
  0016f2f6: vmov s0,r0
  0016f2fa: ldr r0,[sp,#0x14]
  0016f2fc: vadd.f32 s0,s0,s0
  0016f300: ldr r0,[r0,#0x8]
  0016f302: ldr r0,[r0,#0x24]
  0016f304: add.w r1,r0,r11
  0016f308: mov r0,r6
  0016f30a: vmov r2,s0
  0016f30e: blx r10
  0016f310: mov r0,r5
  0016f312: mov r1,r6
  0016f314: mov r2,r4
  0016f316: blx r10
  0016f318: ldr.w r10,[sp,#0x14]
  0016f31c: mov r1,r5
  0016f31e: mov r4,r9
  0016f320: ldr.w r0,[r10,#0x8]
  0016f324: ldr r0,[r0,#0xc]
  0016f326: add r0,r11
  0016f328: blx 0x00073534
  0016f32c: ldr.w r0,[r10,#0xd8]
  0016f330: cmp r0,#0x0
  0016f332: beq 0x0016f3c6
  0016f334: ldr.w r0,[r10,#0x8]
  0016f338: add r6,sp,#0x74
  0016f33a: ldr r2,[sp,#0x18]
  0016f33c: ldr.w r9,[r0,#0xc]
  0016f340: ldr r0,[r0,#0x18]
  0016f342: add.w r1,r0,r11
  0016f346: mov r0,r6
  0016f348: blx 0x0006ec74
  0016f34c: add r5,sp,#0x80
  0016f34e: mov r1,r6
  0016f350: mov.w r2,#0x3f000000
  0016f354: mov r0,r5
  0016f356: blx 0x0006ec74
  0016f35a: add r6,sp,#0x28
  0016f35c: add.w r1,r9,r11
  0016f360: mov r2,r5
  0016f362: mov r0,r6
  0016f364: blx 0x0006f1cc
  0016f368: add r0,sp,#0x90
  0016f36a: mov r1,r6
  0016f36c: blx 0x0006eb3c
  0016f370: ldr.w r0,[r10,#0xd8]
  0016f374: ldrd r2,r3,[sp,#0x90]
  0016f378: vldr.32 s0,[sp,#0x98]
  0016f37c: ldr r0,[r0,#0x4]
  0016f37e: vstr.32 s0,[sp]
  0016f382: adds r1,r0,r4
  0016f384: mov r0,r6
  0016f386: blx 0x0006f820
  0016f38a: ldr.w r0,[r10,#0x8]
  0016f38e: ldr r0,[r0,#0x18]
  0016f390: add.w r1,r0,r11
  0016f394: mov r0,r5
  0016f396: blx 0x0006ec80
  0016f39a: ldr.w r0,[r10,#0xd8]
  0016f39e: add.w r1,r11,r11, lsl #0x2
  0016f3a2: ldr r2,[sp,#0x80]
  0016f3a4: ldr r0,[r0,#0x4]
  0016f3a6: add r1,r0
  0016f3a8: str r2,[r1,#0x8]
  0016f3aa: ldr r2,[sp,#0x84]
  0016f3ac: str r2,[r1,#0x18]
  0016f3ae: ldr r2,[sp,#0x88]
  0016f3b0: str r2,[r1,#0x28]
  0016f3b2: adds r1,r0,r4
  0016f3b4: ldrd r2,r3,[sp,#0x90]
  0016f3b8: mov r0,r6
  0016f3ba: vldr.32 s0,[sp,#0x98]
  0016f3be: vstr.32 s0,[sp]
  0016f3c2: blx 0x0006f820
  0016f3c6: ldr.w r0,[r10,#0xe0]
  0016f3ca: cbz r0,0x0016f40a
  0016f3cc: vldr.32 s0,[sp,#0x98]
  0016f3d0: vcmpe.f32 s0,s16
  0016f3d4: vmrs apsr,fpscr
  0016f3d8: bne 0x0016f40a
  0016f3da: ldr r0,[r0,#0x4]
  0016f3dc: ldr.w r1,[r0,r8,lsl #0x2]
  0016f3e0: cmp r1,#0x1
  0016f3e2: blt 0x0016f40a
  0016f3e4: ldr r2,[sp,#0x10]
  0016f3e6: subs r1,r1,r2
  0016f3e8: str.w r1,[r0,r8,lsl #0x2]
  0016f3ec: cmp r1,#0x0
  0016f3ee: bgt 0x0016f40a
  0016f3f0: movs r1,#0x0
  0016f3f2: movs r2,#0x0
  0016f3f4: str.w r1,[r0,r8,lsl #0x2]
  0016f3f8: ldr.w r1,[r10,#0xdc]
  0016f3fc: ldr.w r0,[r10,#0xe4]
  0016f400: ldr r1,[r1,#0x4]
  0016f402: ldr.w r1,[r1,r8,lsl #0x2]
  0016f406: blx 0x00072a24
  0016f40a: adds r4,#0x3c
  0016f40c: add.w r11,r11,#0xc
  0016f410: add.w r8,r8,#0x1
  0016f414: ldr.w r0,[r10,#0x8]
  0016f418: ldr r1,[r0,#0x8]
  0016f41a: cmp r8,r1
  0016f41c: bcc.w 0x0016f17e
  0016f420: ldr r0,[sp,#0x9c]
  0016f422: ldr r1,[sp,#0x4]
  0016f424: ldr r1,[r1,#0x0]
  0016f426: subs r0,r1,r0
  0016f428: itttt eq
  0016f42a: add.eq sp,#0xa0
  0016f42c: vpop.eq {d8,d9,d10}
  0016f430: add.eq sp,#0x4
  0016f432: pop.eq.w {r8,r9,r10,r11}
  0016f436: it eq
  0016f438: pop.eq {r4,r5,r6,r7,pc}
  0016f43a: blx 0x0006e824
```
