# Level::initParticleSystems
elf 0xbd6f8 body 1702
Sig: undefined __thiscall initParticleSystems(Level * this)

## decompile
```c

/* Level::initParticleSystems() */

void __thiscall Level::initParticleSystems(Level *this)

{
  double dVar1;
  double dVar2;
  undefined8 uVar3;
  Array *pAVar4;
  undefined4 uVar5;
  int iVar6;
  SolarSystem *this_00;
  uint *puVar7;
  undefined4 uVar8;
  uint uVar9;
  EVP_PKEY_CTX *pEVar10;
  EVP_PKEY_CTX *extraout_r1;
  EVP_PKEY_CTX *extraout_r1_00;
  EVP_PKEY_CTX *extraout_r1_01;
  EVP_PKEY_CTX *extraout_r1_02;
  EVP_PKEY_CTX *ctx;
  undefined4 *puVar11;
  undefined4 uVar12;
  KIPlayer *this_01;
  int iVar13;
  code *pcVar14;
  uint uVar15;
  uint *puVar16;
  int *piVar17;
  uint in_fpscr;
  float fVar18;
  undefined4 uVar19;
  float fVar20;
  undefined4 uVar21;
  float fVar22;
  undefined4 uVar23;
  float fVar24;
  double dVar25;
  undefined4 uVar26;
  double dVar27;
  undefined4 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined4 local_268 [5];
  undefined4 local_254;
  undefined4 local_250;
  undefined4 uStack_24c;
  undefined4 uStack_248;
  undefined4 uStack_244;
  undefined8 uStack_240;
  undefined8 uStack_238;
  undefined4 local_230;
  undefined4 local_228 [5];
  undefined4 local_214;
  undefined4 local_210;
  undefined4 uStack_20c;
  undefined4 uStack_208;
  undefined4 uStack_204;
  undefined8 uStack_200;
  undefined8 uStack_1f8;
  undefined4 local_1f0;
  undefined4 local_1e8 [5];
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 uStack_1cc;
  undefined4 uStack_1c8;
  undefined4 uStack_1c4;
  undefined8 uStack_1c0;
  undefined8 uStack_1b8;
  undefined4 local_1b0;
  undefined4 local_1a8 [5];
  undefined4 local_194;
  undefined4 local_190;
  undefined4 uStack_18c;
  undefined4 uStack_188;
  undefined4 uStack_184;
  undefined8 uStack_180;
  undefined8 uStack_178;
  undefined4 local_170;
  undefined4 local_168 [5];
  undefined4 local_154;
  undefined4 local_150;
  undefined4 uStack_14c;
  undefined4 uStack_148;
  undefined4 uStack_144;
  undefined8 uStack_140;
  undefined8 uStack_138;
  undefined4 local_130;
  undefined4 local_128 [5];
  undefined4 local_114;
  undefined4 local_110;
  undefined4 uStack_10c;
  undefined4 uStack_108;
  undefined4 uStack_104;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined4 local_f0;
  float local_e8 [5];
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined4 local_b0;
  float local_a8;
  float local_a4;
  float local_a0;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined4 local_70;
  int local_6c;
  
  piVar17 = *(int **)(DAT_000cda44 + 0xcd710);
  local_6c = *piVar17;
  if (*(int *)(this + 0xf0) != 0) {
    if (*(int *)(this + 0xa4) != 0) {
      pAVar4 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this + 0xa8) = pAVar4;
      ArraySetLength<int>(**(uint **)(this + 0xa4),pAVar4);
      uVar28 = DAT_000cda74;
      uVar26 = DAT_000cda70;
      uVar12 = DAT_000cda6c;
      uVar5 = DAT_000cda68;
      dVar2 = DAT_000cda60;
      fVar20 = DAT_000cda5c;
      fVar18 = DAT_000cda58;
      dVar1 = DAT_000cda50;
      dVar25 = 1.5;
      dVar27 = 1.0;
      puVar11 = (undefined4 *)(*(int *)(DAT_000cda48 + 0xcd75e) + 0x12b4);
      for (uVar15 = 0; uVar15 < **(uint **)(this + 0xa4); uVar15 = uVar15 + 1) {
        uVar8 = ParticleSystemManager::addSystem
                          (*(undefined4 *)(this + 0x80),**(int **)(this + 0xf0) + 4,uVar15 + 0x1d,0)
        ;
        *(undefined4 *)(*(int *)(*(int *)(this + 0xa8) + 4) + uVar15 * 4) = uVar8;
        AEGeometry::getPosition();
        puVar11[-7] = local_a8;
        AEGeometry::getPosition();
        puVar11[-6] = local_a4;
        AEGeometry::getScaling();
        in_fpscr = in_fpscr & 0xfffffff;
        if (dVar27 <= (double)local_a8 * dVar25) {
          uVar8 = 0x50;
        }
        else {
          AEGeometry::getScaling();
          uVar8 = (undefined4)(longlong)((double)local_e8[0] * dVar25 * dVar1);
        }
        puVar11[-0x1b] = uVar8;
        AEGeometry::getScaling();
        puVar11[-0x20] = local_a8 * fVar18;
        puVar11[-0x21] = 0x14;
        puVar11[-0x1a] = 0x41000000;
        puVar11[-0x14] = DAT_000cda78;
        AEGeometry::getPosition();
        puVar11[-5] = local_a0;
        AEGeometry::getScaling();
        in_fpscr = in_fpscr & 0xfffffff;
        fVar22 = fVar20;
        if ((double)local_a8 * dVar25 < dVar27) {
          AEGeometry::getScaling();
          fVar22 = (float)((double)local_e8[0] * dVar25 * dVar2);
        }
        uVar8 = DAT_000cda7c;
        puVar11[-9] = fVar22;
        puVar11[-0x18] = uVar8;
        puVar11[-0x17] = 0;
        puVar11[-0xc] = DAT_000cda80;
        Status::getShip();
        iVar6 = Ship::getIndex();
        uVar9 = *(int *)(DAT_000cda84 + 0xcd8f6 + iVar6 * 4) - 1;
        uVar8 = uVar5;
        uVar19 = uVar12;
        uVar21 = uVar26;
        uVar23 = uVar28;
        if (uVar9 < 9) {
          uVar8 = *(undefined4 *)(DAT_000cda8c + 0xcd912 + uVar9 * 4);
          uVar21 = *(undefined4 *)(DAT_000cda88 + 0xcd910 + uVar9 * 4);
          uVar19 = *(undefined4 *)(DAT_000cda90 + 0xcd91a + uVar9 * 4);
          uVar23 = *(undefined4 *)(DAT_000cda94 + 0xcd91c + uVar9 * 4);
        }
        puVar11[-3] = uVar23;
        puVar11[-2] = uVar21;
        puVar11[-1] = uVar19;
        *puVar11 = uVar8;
        puVar11 = puVar11 + 0x28;
      }
    }
    uVar12 = *(undefined4 *)(this + 0x88);
    puVar16 = *(uint **)(DAT_000cda98 + 0xcd964);
    uVar15 = *puVar16;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar5 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar15);
    uVar5 = ParticleSystemManager::addSystem(uVar12,uVar5,4,0);
    *(undefined4 *)(this + 100) = uVar5;
    iVar6 = Status::getSystem();
    if (iVar6 != 0) {
      this_00 = (SolarSystem *)Status::getSystem();
      iVar6 = SolarSystem::hasPirateBase(this_00);
      if ((iVar6 != 0) && (puVar7 = *(uint **)(this + 0xf8), puVar7 != (uint *)0x0)) {
        for (uVar15 = 0; uVar15 < *puVar7; uVar15 = uVar15 + 1) {
          this_01 = *(KIPlayer **)(puVar7[1] + uVar15 * 4);
          if (this_01 != (KIPlayer *)0x0) {
            iVar6 = KIPlayer::getType(this_01);
            if (iVar6 == 0x37a3) {
              AEGeometry::updateReferenceMatrix();
              uVar12 = *(undefined4 *)(this + 0x7c);
              uVar5 = AEGeometry::getReferenceMatrix();
              ParticleSystemManager::addSystem(uVar12,uVar5,8,0);
              break;
            }
            puVar7 = *(uint **)(this + 0xf8);
          }
        }
      }
    }
    uVar15 = *puVar16;
    uVar12 = *(undefined4 *)(this + 0x7c);
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar5 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar15);
    uVar5 = ParticleSystemManager::addSystem(uVar12,uVar5,7,0);
    *(undefined4 *)(this + 0x284) = uVar5;
    iVar6 = Status::inAlienOrbit();
    uVar5 = DAT_000cde20;
    if (iVar6 == 0) {
      iVar13 = *(int *)(DAT_000cdaa0 + 0xcda26);
      *(undefined4 *)(iVar13 + 0x534) = DAT_000cde24;
      *(undefined4 *)(iVar13 + 0x538) = uVar5;
      Status::getSystem();
      iVar6 = SolarSystem::getTextureIndex();
      uVar15 = *(uint *)(DAT_000cde30 + 0xcdacc + iVar6 * 4);
      iVar6 = Status::inSupernovaSystem();
      fVar18 = DAT_000cde28;
      if (iVar6 != 0) {
        fVar18 = 0.5;
      }
      fVar20 = (float)VectorSignedToFloat(uVar15 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
      fVar22 = (float)VectorSignedToFloat((uVar15 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
      uVar9 = 0xbb;
      fVar24 = (float)VectorSignedToFloat((uVar15 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
      if (iVar6 != 0) {
        uVar9 = 0xff;
      }
      uVar15 = uVar9 | (int)(fVar18 * fVar22) << 0x10 | (int)(fVar18 * fVar20) << 0x18 |
               (int)(fVar18 * fVar24) << 8;
      *(uint *)(iVar13 + 0x494) = uVar15;
      *(uint *)(iVar13 + 0x498) = uVar15;
    }
    else {
      iVar6 = *(int *)(DAT_000cdaa0 + 0xcda26);
      uVar15 = DAT_000cdaa4 << 8;
      *(uint *)(iVar6 + 0x534) = *(uint *)(iVar6 + 0x534) & 0xff | DAT_000cdaa4 << 8;
      *(uint *)(iVar6 + 0x494) = *(uint *)(iVar6 + 0x494) & 0xff | uVar15;
    }
  }
  iVar6 = Status::inSupernovaSystem();
  if (((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x59)) ||
     (iVar6 = Status::getCurrentCampaignMission(), 0x9d < iVar6)) {
    pEVar10 = (EVP_PKEY_CTX *)0x0;
    iVar6 = *(int *)(DAT_000cde3c + 0xcdbd8);
    *(undefined4 *)(iVar6 + 0x4b8) = 0;
    *(undefined4 *)(iVar6 + 0x4bc) = 0;
    *(undefined4 *)(iVar6 + 0x4c0) = 0;
  }
  else {
    StarSystem::getLightDirection();
    AbyssEngine::AEMath::VectorNormalize((AEMath *)local_e8,(Vector *)local_128);
    AbyssEngine::AEMath::operator-((AEMath *)&local_a8,(Vector *)local_e8);
    local_a4 = local_a4 * DAT_000cde2c;
    local_a0 = local_a0 * DAT_000cde2c;
    iVar6 = *(int *)(DAT_000cde38 + 0xcdbb6);
    *(float *)(iVar6 + 0x4b8) = local_a8 * DAT_000cde2c;
    pEVar10 = (EVP_PKEY_CTX *)(iVar6 + 0x4bc);
    *(float *)pEVar10 = local_a4;
    *(float *)(iVar6 + 0x4c0) = local_a0;
  }
  if (*(ParticleSystemManager **)(this + 0x80) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x80),pEVar10);
    pEVar10 = extraout_r1;
  }
  if (*(ParticleSystemManager **)(this + 0x7c) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x7c),pEVar10);
    pEVar10 = extraout_r1_00;
  }
  if (*(ParticleSystemManager **)(this + 0x88) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x88),pEVar10);
    pEVar10 = extraout_r1_01;
  }
  if (*(ParticleSystemManager **)(this + 0x8c) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x8c),pEVar10);
    pEVar10 = extraout_r1_02;
  }
  if (*(ParticleSystemManager **)(this + 0x98) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x98),pEVar10);
  }
  iVar6 = DAT_000cde40;
  uVar3 = DAT_000cde18;
  uVar30 = DAT_000cde10;
  uVar5 = 0;
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar26 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar28 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar11 = (undefined4 *)((uint)&local_a8 | 4);
  *puVar11 = 0;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  local_90 = 0;
  uStack_80 = uVar30;
  uStack_78 = uVar3;
  local_a8 = 1.0;
  local_94 = 0x3f800000;
  local_70 = 0x3f800000;
  pcVar14 = *(code **)(iVar6 + 0xcdc38);
  uStack_8c = uVar12;
  uStack_88 = uVar26;
  uStack_84 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),&local_a8,10,0);
  *(undefined4 *)(this + 0x38) = uVar8;
  puVar11 = (undefined4 *)((uint)local_e8 | 4);
  *puVar11 = uVar5;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  uStack_c0 = uVar30;
  uStack_b8 = uVar3;
  local_e8[0] = 1.0;
  local_d4 = 0x3f800000;
  local_b0 = 0x3f800000;
  local_d0 = uVar5;
  uStack_cc = uVar12;
  uStack_c8 = uVar26;
  uStack_c4 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),local_e8,0xb,0);
  *(undefined4 *)(this + 0x3c) = uVar8;
  puVar11 = (undefined4 *)((uint)local_128 | 4);
  *puVar11 = uVar5;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  uStack_100 = uVar30;
  uStack_f8 = uVar3;
  local_128[0] = 0x3f800000;
  local_114 = 0x3f800000;
  local_f0 = 0x3f800000;
  local_110 = uVar5;
  uStack_10c = uVar12;
  uStack_108 = uVar26;
  uStack_104 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),local_128,0x14,0);
  *(undefined4 *)(this + 0x48) = uVar8;
  puVar11 = (undefined4 *)((uint)local_168 | 4);
  *puVar11 = uVar5;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  uStack_140 = uVar30;
  uStack_138 = uVar3;
  local_168[0] = 0x3f800000;
  local_154 = 0x3f800000;
  local_130 = 0x3f800000;
  local_150 = uVar5;
  uStack_14c = uVar12;
  uStack_148 = uVar26;
  uStack_144 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),local_168,0x15,0);
  *(undefined4 *)(this + 0x34) = uVar8;
  puVar11 = (undefined4 *)((uint)local_1a8 | 4);
  *puVar11 = uVar5;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  uStack_180 = uVar30;
  uStack_178 = uVar3;
  local_1a8[0] = 0x3f800000;
  local_194 = 0x3f800000;
  local_170 = 0x3f800000;
  local_190 = uVar5;
  uStack_18c = uVar12;
  uStack_188 = uVar26;
  uStack_184 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),local_1a8,0x16,0);
  *(undefined4 *)(this + 0x50) = uVar8;
  puVar11 = (undefined4 *)((uint)local_1e8 | 4);
  *puVar11 = uVar5;
  puVar11[1] = uVar12;
  puVar11[2] = uVar26;
  puVar11[3] = uVar28;
  uStack_1c0 = uVar30;
  uStack_1b8 = uVar3;
  local_1e8[0] = 0x3f800000;
  local_1d4 = 0x3f800000;
  local_1b0 = 0x3f800000;
  local_1d0 = uVar5;
  uStack_1cc = uVar12;
  uStack_1c8 = uVar26;
  uStack_1c4 = uVar28;
  uVar8 = (*pcVar14)(*(undefined4 *)(this + 0x74),local_1e8,0x17,0);
  *(undefined4 *)(this + 0x54) = uVar8;
  uVar29 = Status::getCurrentCampaignMission();
  pEVar10 = (EVP_PKEY_CTX *)((ulonglong)uVar29 >> 0x20);
  if ((int)uVar29 == 0x50) {
    puVar11 = (undefined4 *)((uint)local_228 | 4);
    *puVar11 = uVar5;
    puVar11[1] = uVar12;
    puVar11[2] = uVar26;
    puVar11[3] = uVar28;
    uStack_200 = uVar30;
    uStack_1f8 = uVar3;
    local_228[0] = 0x3f800000;
    local_214 = 0x3f800000;
    local_1f0 = 0x3f800000;
    local_210 = uVar5;
    uStack_20c = uVar12;
    uStack_208 = uVar26;
    uStack_204 = uVar28;
    uVar8 = ParticleSystemManager::addSystem(*(undefined4 *)(this + 0x74),local_228,0x18,0);
    *(undefined4 *)(this + 0x58) = uVar8;
    puVar11 = (undefined4 *)((uint)local_268 | 4);
    *puVar11 = uVar5;
    puVar11[1] = uVar12;
    puVar11[2] = uVar26;
    puVar11[3] = uVar28;
    uStack_240 = uVar30;
    uStack_238 = uVar3;
    local_268[0] = 0x3f800000;
    local_254 = 0x3f800000;
    local_230 = 0x3f800000;
    local_250 = uVar5;
    uStack_24c = uVar12;
    uStack_248 = uVar26;
    uStack_244 = uVar28;
    uVar30 = ParticleSystemManager::addSystem(*(undefined4 *)(this + 0x74),local_268,0x18,0);
    pEVar10 = (EVP_PKEY_CTX *)((ulonglong)uVar30 >> 0x20);
    *(int *)(this + 0x5c) = (int)uVar30;
  }
  ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x74),pEVar10);
  ParticleSystemManager::enableSystemEmit
            (*(int *)(this + 0x74),SUB41(*(undefined4 *)(this + 0x50),0));
  ParticleSystemManager::enableSystemEmit
            (*(int *)(this + 0x74),SUB41(*(undefined4 *)(this + 0x54),0));
  iVar6 = Status::getCurrentCampaignMission();
  if (iVar6 == 0x50) {
    ParticleSystemManager::enableSystemEmit
              (*(int *)(this + 0x74),SUB41(*(undefined4 *)(this + 0x58),0));
    ParticleSystemManager::enableSystemEmit
              (*(int *)(this + 0x74),SUB41(*(undefined4 *)(this + 0x5c),0));
  }
  pcVar14 = *(code **)(DAT_000cde44 + 0xcddcc);
  (*pcVar14)(*(undefined4 *)(this + 0x9c));
  (*pcVar14)(*(undefined4 *)(this + 0x78));
  (*pcVar14)(*(undefined4 *)(this + 0x84));
  if (*(ParticleSystemManager **)(this + 0x94) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::init(*(ParticleSystemManager **)(this + 0x94),ctx);
  }
  if (*piVar17 == local_6c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```
## target disasm
```
  000cd6f8: push {r4,r5,r6,r7,lr}
  000cd6fa: add r7,sp,#0xc
  000cd6fc: push {r7,r8,r9,r10,r11}
  000cd700: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  000cd704: sub.w sp,sp,#0x208
  000cd708: mov r4,r0
  000cd70a: ldr r0,[0x000cda44]
  000cd70c: add r0,pc
  000cd70e: ldr.w r10,[r0,#0x0]
  000cd712: ldr.w r0,[r10,#0x0]
  000cd716: str r0,[sp,#0x204]
  000cd718: ldr.w r0,[r4,#0xf0]
  000cd71c: cmp r0,#0x0
  000cd71e: beq.w 0x000cdb4a
  000cd722: ldr.w r0,[r4,#0xa4]
  000cd726: str.w r10,[sp,#0x4]
  000cd72a: cmp r0,#0x0
  000cd72c: beq.w 0x000cd95a
  000cd730: movs r0,#0xc
  000cd732: blx 0x0006eb24
  000cd736: mov r5,r0
  000cd738: blx 0x000701f8
  000cd73c: ldr.w r0,[r4,#0xa4]
  000cd740: mov r1,r5
  000cd742: str.w r5,[r4,#0xa8]
  000cd746: ldr r0,[r0,#0x0]
  000cd748: blx 0x00071a4c
  000cd74c: ldr r0,[0x000cda48]
  000cd74e: movw r1,#0x12b4
  000cd752: vmov.f64 d8,0x3ff8000000000000
  000cd756: add.w r10,sp,#0x1c8
  000cd75a: add r0,pc
  000cd75c: movs r6,#0x0
  000cd75e: mov.w r9,#0x0
  000cd762: ldr r0,[r0,#0x0]
  000cd764: vmov.f64 d9,0x3ff0000000000000
  000cd768: adds r5,r0,r1
  000cd76a: ldr r0,[0x000cda4c]
  000cd76c: add r0,pc
  000cd76e: vldr.64 d10,[pc,#0x2e0]
  000cd772: ldr.w r8,[r0,#0x0]
  000cd776: vldr.32 s22,[pc,#0x2e0]
  000cd77a: vldr.32 s24,[pc,#0x2e0]
  000cd77e: vldr.64 d13,[pc,#0x2e0]
  000cd782: vldr.32 s28,[pc,#0x2e4]
  000cd786: vldr.32 s30,[pc,#0x2e4]
  000cd78a: vldr.32 s23,[pc,#0x2e4]
  000cd78e: vldr.32 s25,[pc,#0x2e4]
  000cd792: b 0x000cd94e
  000cd794: ldr.w r1,[r4,#0xf0]
  000cd798: add.w r11,r9,#0x1d
  000cd79c: ldr.w r0,[r4,#0x80]
  000cd7a0: movs r3,#0x0
  000cd7a2: mov r2,r11
  000cd7a4: ldr r1,[r1,#0x0]
  000cd7a6: adds r1,#0x4
  000cd7a8: blx 0x000724d8
  000cd7ac: ldr.w r1,[r4,#0xa8]
  000cd7b0: ldr r1,[r1,#0x4]
  000cd7b2: str.w r0,[r1,r9,lsl #0x2]
  000cd7b6: ldr.w r0,[r4,#0xa4]
  000cd7ba: ldr r0,[r0,#0x4]
  000cd7bc: ldr.w r1,[r0,r9,lsl #0x2]
  000cd7c0: mov r0,r10
  000cd7c2: blx 0x000720b8
  000cd7c6: ldr r0,[sp,#0x1c8]
  000cd7c8: str.w r0,[r5,#-0x1c]
  000cd7cc: ldr.w r0,[r4,#0xa4]
  000cd7d0: ldr r0,[r0,#0x4]
  000cd7d2: ldr.w r1,[r0,r9,lsl #0x2]
  000cd7d6: mov r0,r10
  000cd7d8: blx 0x000720b8
  000cd7dc: ldr r0,[sp,#0x1cc]
  000cd7de: str.w r0,[r5,#-0x18]
  000cd7e2: ldr.w r0,[r4,#0xa4]
  000cd7e6: ldr r0,[r0,#0x4]
  000cd7e8: ldr.w r1,[r0,r9,lsl #0x2]
  000cd7ec: mov r0,r10
  000cd7ee: blx 0x00072760
  000cd7f2: vldr.32 s0,[sp,#0x1c8]
  000cd7f6: vcvt.f64.f32 d16,s0
  000cd7fa: vmul.f64 d16,d16,d8
  000cd7fe: vcmpe.f64 d16,d9
  000cd802: vmrs apsr,fpscr
  000cd806: bpl 0x000cd832
  000cd808: ldr.w r0,[r4,#0xa4]
  000cd80c: ldr r0,[r0,#0x4]
  000cd80e: ldr.w r1,[r0,r9,lsl #0x2]
  000cd812: add r0,sp,#0x188
  000cd814: blx 0x00072760
  000cd818: vldr.32 s0,[sp,#0x188]
  000cd81c: vcvt.f64.f32 d16,s0
  000cd820: vmul.f64 d16,d16,d8
  000cd824: vmul.f64 d16,d16,d10
  000cd828: vcvt.s32.f64 s0,d16
  000cd82c: vmov r0,s0
  000cd830: b 0x000cd834
  000cd832: movs r0,#0x50
  000cd834: str.w r0,[r5,#-0x6c]
  000cd838: ldr.w r0,[r4,#0xa4]
  000cd83c: ldr r0,[r0,#0x4]
  000cd83e: ldr.w r1,[r0,r9,lsl #0x2]
  000cd842: mov r0,r10
  000cd844: blx 0x00072760
  000cd848: vldr.32 s0,[sp,#0x1c8]
  000cd84c: movs r0,#0x14
  000cd84e: vmul.f32 s0,s0,s22
  000cd852: vstr.32 s0,[r5,#-0x80]
  000cd856: str.w r0,[r5,#-0x84]
  000cd85a: mov.w r0,#0x41000000
  000cd85e: str.w r0,[r5,#-0x68]
  000cd862: ldr r0,[0x000cda78]
  000cd864: str.w r0,[r5,#-0x50]
  000cd868: ldr.w r0,[r4,#0xa4]
  000cd86c: ldr r0,[r0,#0x4]
  000cd86e: ldr.w r1,[r0,r9,lsl #0x2]
  000cd872: mov r0,r10
  000cd874: blx 0x000720b8
  000cd878: ldr r0,[sp,#0x1d0]
  000cd87a: str.w r0,[r5,#-0x14]
  000cd87e: ldr.w r0,[r4,#0xa4]
  000cd882: ldr r0,[r0,#0x4]
  000cd884: ldr.w r1,[r0,r9,lsl #0x2]
  000cd888: mov r0,r10
  000cd88a: blx 0x00072760
  000cd88e: vldr.32 s0,[sp,#0x1c8]
  000cd892: vcvt.f64.f32 d16,s0
  000cd896: vmul.f64 d16,d16,d8
  000cd89a: vmov.f32 s0,s24
  000cd89e: vcmpe.f64 d16,d9
  000cd8a2: vmrs apsr,fpscr
  000cd8a6: bpl 0x000cd8cc
  000cd8a8: ldr.w r0,[r4,#0xa4]
  000cd8ac: ldr r0,[r0,#0x4]
  000cd8ae: ldr.w r1,[r0,r9,lsl #0x2]
  000cd8b2: add r0,sp,#0x188
  000cd8b4: blx 0x00072760
  000cd8b8: vldr.32 s0,[sp,#0x188]
  000cd8bc: vcvt.f64.f32 d16,s0
  000cd8c0: vmul.f64 d16,d16,d8
  000cd8c4: vmul.f64 d16,d16,d13
  000cd8c8: vcvt.f32.f64 s0,d16
  000cd8cc: ldr r0,[0x000cda7c]
  000cd8ce: vstr.32 s0,[r5,#-0x24]
  000cd8d2: strd r0,r6,[r5,#-0x60]
  000cd8d6: ldr r0,[0x000cda80]
  000cd8d8: str.w r0,[r5,#-0x30]
  000cd8dc: ldr.w r0,[r8,#0x0]
  000cd8e0: blx 0x00071a58
  000cd8e4: blx 0x000719c8
  000cd8e8: ldr r1,[0x000cda84]
  000cd8ea: vmov.f32 s6,s25
  000cd8ee: vmov.f32 s4,s23
  000cd8f2: add r1,pc
  000cd8f4: vmov.f32 s2,s30
  000cd8f8: vmov.f32 s0,s28
  000cd8fc: ldr.w r0,[r1,r0,lsl #0x2]
  000cd900: subs r0,#0x1
  000cd902: cmp r0,#0x8
  000cd904: bhi 0x000cd938
  000cd906: ldr r1,[0x000cda88]
  000cd908: ldr r2,[0x000cda8c]
  000cd90a: ldr r3,[0x000cda90]
  000cd90c: add r1,pc
  000cd90e: add r2,pc
  000cd910: ldr r6,[0x000cda94]
  000cd912: add.w r2,r2,r0, lsl #0x2
  000cd916: add r3,pc
  000cd918: add r6,pc
  000cd91a: add.w r1,r1,r0, lsl #0x2
  000cd91e: vldr.32 s0,[r2]
  000cd922: add.w r2,r3,r0, lsl #0x2
  000cd926: add.w r0,r6,r0, lsl #0x2
  000cd92a: vldr.32 s4,[r1]
  000cd92e: vldr.32 s2,[r2]
  000cd932: movs r6,#0x0
  000cd934: vldr.32 s6,[r0]
  000cd938: vstr.32 s6,[r5,#-0xc]
  000cd93c: sub.w r9,r11,#0x1c
  000cd940: vstr.32 s4,[r5,#-0x8]
  000cd944: vstr.32 s2,[r5,#-0x4]
  000cd948: vstr.32 s0,[r5]
  000cd94c: adds r5,#0xa0
  000cd94e: ldr.w r0,[r4,#0xa4]
  000cd952: ldr r0,[r0,#0x0]
  000cd954: cmp r9,r0
  000cd956: bcc.w 0x000cd794
  000cd95a: ldr r0,[0x000cda98]
  000cd95c: ldr.w r5,[r4,#0x88]
  000cd960: add r0,pc
  000cd962: ldr.w r9,[r0,#0x0]
  000cd966: ldr.w r6,[r9,#0x0]
  000cd96a: mov r0,r6
  000cd96c: blx 0x000717f4
  000cd970: mov r1,r0
  000cd972: mov r0,r6
  000cd974: blx 0x0006ff1c
  000cd978: mov r1,r0
  000cd97a: mov r0,r5
  000cd97c: movs r2,#0x4
  000cd97e: movs r3,#0x0
  000cd980: blx 0x000724d8
  000cd984: ldr r1,[0x000cda9c]
  000cd986: str r0,[r4,#0x64]
  000cd988: add r1,pc
  000cd98a: ldr.w r8,[r1,#0x0]
  000cd98e: ldr.w r0,[r8,#0x0]
  000cd992: blx 0x00071a10
  000cd996: cbz r0,0x000cd9ee
  000cd998: ldr.w r0,[r8,#0x0]
  000cd99c: blx 0x00071a10
  000cd9a0: blx 0x00073ee8
  000cd9a4: cbz r0,0x000cd9ee
  000cd9a6: ldr.w r0,[r4,#0xf8]
  000cd9aa: cbz r0,0x000cd9ee
  000cd9ac: movs r6,#0x0
  000cd9ae: movw r10,#0x37a3
  000cd9b2: b 0x000cd9cc
  000cd9b4: ldr r1,[r0,#0x4]
  000cd9b6: ldr.w r5,[r1,r6,lsl #0x2]
  000cd9ba: cbz r5,0x000cd9ca
  000cd9bc: mov r0,r5
  000cd9be: blx 0x00072010
  000cd9c2: cmp r0,r10
  000cd9c4: beq 0x000cd9d4
  000cd9c6: ldr.w r0,[r4,#0xf8]
  000cd9ca: adds r6,#0x1
  000cd9cc: ldr r1,[r0,#0x0]
  000cd9ce: cmp r6,r1
  000cd9d0: bcc 0x000cd9b4
  000cd9d2: b 0x000cd9ee
  000cd9d4: ldr r0,[r5,#0x8]
  000cd9d6: blx 0x000742b4
  000cd9da: ldr r0,[r5,#0x8]
  000cd9dc: ldr r6,[r4,#0x7c]
  000cd9de: blx 0x00072d00
  000cd9e2: mov r1,r0
  000cd9e4: mov r0,r6
  000cd9e6: movs r2,#0x8
  000cd9e8: movs r3,#0x0
  000cd9ea: blx 0x000724d8
  000cd9ee: ldr.w r6,[r9,#0x0]
  000cd9f2: ldr r5,[r4,#0x7c]
  000cd9f4: mov r0,r6
  000cd9f6: blx 0x000717f4
  000cd9fa: mov r1,r0
  000cd9fc: mov r0,r6
  000cd9fe: blx 0x0006ff1c
  000cda02: mov r1,r0
  000cda04: mov r0,r5
  000cda06: movs r2,#0x7
  000cda08: movs r3,#0x0
  000cda0a: blx 0x000724d8
  000cda0e: str.w r0,[r4,#0x284]
  000cda12: ldr.w r0,[r8,#0x0]
  000cda16: blx 0x000723d0
  000cda1a: cmp r0,#0x0
  000cda1c: ldr.w r10,[sp,#0x4]
  000cda20: ldr r0,[0x000cdaa0]
  000cda22: add r0,pc
  000cda24: beq 0x000cdaa8
  000cda26: ldr r0,[r0,#0x0]
  000cda28: ldr r3,[0x000cdaa4]
  000cda2a: ldr.w r1,[r0,#0x494]
  000cda2e: ldr.w r2,[r0,#0x534]
  000cda32: bfi r1,r3,#0x8,#0x18
  000cda36: bfi r2,r3,#0x8,#0x18
  000cda3a: str.w r2,[r0,#0x534]
  000cda3e: str.w r1,[r0,#0x494]
  000cda42: b 0x000cdb4a
  000cdaa8: ldr r5,[r0,#0x0]
  000cdaaa: ldr.w r0,[r8,#0x0]
  000cdaae: ldr r2,[0x000cde24]
  000cdab0: ldr r1,[0x000cde20]
  000cdab2: str.w r2,[r5,#0x534]
  000cdab6: str.w r1,[r5,#0x538]
  000cdaba: blx 0x00071a10
  000cdabe: blx 0x0007390c
  000cdac2: ldr r2,[0x000cde30]
  000cdac4: ldr.w r1,[r8,#0x0]
  000cdac8: add r2,pc
  000cdaca: ldr.w r6,[r2,r0,lsl #0x2]
  000cdace: mov r0,r1
  000cdad0: blx 0x00071878
  000cdad4: vmov.f32 s2,0x3f000000
  000cdad8: vldr.32 s0,[pc,#0x34c]
  000cdadc: cmp r0,#0x0
  000cdade: lsr.w r1,r6,#0x18
  000cdae2: it ne
  000cdae4: vmov.ne.f32 s0,s2
  000cdae8: ubfx r2,r6,#0x10,#0x8
  000cdaec: vmov s2,r1
  000cdaf0: cmp r0,#0x0
  000cdaf2: vmov s4,r2
  000cdaf6: vcvt.f32.s32 s2,s2
  000cdafa: vcvt.f32.s32 s4,s4
  000cdafe: ubfx r1,r6,#0x8,#0x8
  000cdb02: vmov s6,r1
  000cdb06: mov.w r1,#0xbb
  000cdb0a: vcvt.f32.s32 s6,s6
  000cdb0e: vmul.f32 s2,s0,s2
  000cdb12: vmul.f32 s4,s0,s4
  000cdb16: vmul.f32 s0,s0,s6
  000cdb1a: vcvt.s32.f32 s2,s2
  000cdb1e: it ne
  000cdb20: mov.ne r1,#0xff
  000cdb22: vcvt.s32.f32 s4,s4
  000cdb26: vcvt.s32.f32 s0,s0
  000cdb2a: vmov r0,s4
  000cdb2e: orr.w r0,r1,r0, lsl #0x10
  000cdb32: vmov r1,s2
  000cdb36: orr.w r0,r0,r1, lsl #0x18
  000cdb3a: vmov r1,s0
  000cdb3e: orr.w r0,r0,r1, lsl #0x8
  000cdb42: str.w r0,[r5,#0x494]
  000cdb46: str.w r0,[r5,#0x498]
  000cdb4a: ldr r0,[0x000cde34]
  000cdb4c: add r0,pc
  000cdb4e: ldr.w r9,[r0,#0x0]
  000cdb52: ldr.w r0,[r9,#0x0]
  000cdb56: blx 0x00071878
  000cdb5a: cbz r0,0x000cdbd0
  000cdb5c: ldr.w r0,[r9,#0x0]
  000cdb60: blx 0x00071770
  000cdb64: cmp r0,#0x59
  000cdb66: beq 0x000cdbd0
  000cdb68: ldr.w r0,[r9,#0x0]
  000cdb6c: blx 0x00071770
  000cdb70: cmp r0,#0x9d
  000cdb72: bgt 0x000cdbd0
  000cdb74: add.w r8,sp,#0x148
  000cdb78: ldr.w r1,[r4,#0xec]
  000cdb7c: mov r0,r8
  000cdb7e: blx 0x000727cc
  000cdb82: add r6,sp,#0x188
  000cdb84: mov r1,r8
  000cdb86: mov r0,r6
  000cdb88: blx 0x0006ec80
  000cdb8c: add r0,sp,#0x1c8
  000cdb8e: mov r1,r6
  000cdb90: blx 0x0006f4cc
  000cdb94: vldr.32 s0,[pc,#0x294]
  000cdb98: vldr.32 s2,[sp,#0x1c8]
  000cdb9c: vldr.32 s4,[sp,#0x1cc]
  000cdba0: vldr.32 s6,[sp,#0x1d0]
  000cdba4: vmul.f32 s2,s2,s0
  000cdba8: ldr r0,[0x000cde38]
  000cdbaa: vmul.f32 s4,s4,s0
  000cdbae: vmul.f32 s0,s6,s0
  000cdbb2: add r0,pc
  000cdbb4: ldr r0,[r0,#0x0]
  000cdbb6: add.w r1,r0,#0x4b8
  000cdbba: vstr.32 s2,[r1]
  000cdbbe: addw r1,r0,#0x4bc
  000cdbc2: add.w r0,r0,#0x4c0
  000cdbc6: vstr.32 s4,[r1]
  000cdbca: vstr.32 s0,[r0]
  000cdbce: b 0x000cdbe4
  000cdbd0: ldr r0,[0x000cde3c]
  000cdbd2: movs r1,#0x0
  000cdbd4: add r0,pc
  000cdbd6: ldr r0,[r0,#0x0]
  000cdbd8: str.w r1,[r0,#0x4b8]
  000cdbdc: str.w r1,[r0,#0x4bc]
  000cdbe0: str.w r1,[r0,#0x4c0]
  000cdbe4: ldr.w r0,[r4,#0x80]
  000cdbe8: cbz r0,0x000cdbee
  000cdbea: blx 0x000742c0
  000cdbee: ldr r0,[r4,#0x7c]
  000cdbf0: cbz r0,0x000cdbf6
  000cdbf2: blx 0x000742c0
  000cdbf6: ldr.w r0,[r4,#0x88]
  000cdbfa: cbz r0,0x000cdc00
  000cdbfc: blx 0x000742c0
  000cdc00: ldr.w r0,[r4,#0x8c]
  000cdc04: cbz r0,0x000cdc0a
  000cdc06: blx 0x000742c0
  000cdc0a: ldr.w r0,[r4,#0x98]
  000cdc0e: cbz r0,0x000cdc14
  000cdc10: blx 0x000742c0
  000cdc14: adr r0,[0xcde10]
  000cdc16: vmov.i32 q4,#0x0
  000cdc1a: add r1,sp,#0x1c8
  000cdc1c: vld1.64 {d10,d11},[r0]
  000cdc20: orr r0,r1,#0x4
  000cdc24: mov.w r6,#0x3f800000
  000cdc28: ldr r2,[0x000cde40]
  000cdc2a: movs r3,#0x0
  000cdc2c: vst1.32 {d8,d9},[r0]
  000cdc30: add.w r0,r1,#0x18
  000cdc34: add r2,pc
  000cdc36: vst1.64 {d8,d9},[r0]
  000cdc3a: add.w r0,r1,#0x28
  000cdc3e: vst1.64 {d10,d11},[r0]
  000cdc42: str r6,[sp,#0x1c8]
  000cdc44: str r6,[sp,#0x1dc]
  000cdc46: str r6,[sp,#0x200]
  000cdc48: ldr r5,[r2,#0x0]
  000cdc4a: movs r2,#0xa
  000cdc4c: ldr r0,[r4,#0x74]
  000cdc4e: blx r5
  000cdc50: add r1,sp,#0x188
  000cdc52: str r0,[r4,#0x38]
  000cdc54: orr r0,r1,#0x4
  000cdc58: movs r2,#0xb
  000cdc5a: movs r3,#0x0
  000cdc5c: vst1.32 {d8,d9},[r0]
  000cdc60: add.w r0,r1,#0x18
  000cdc64: vst1.64 {d8,d9},[r0]
  000cdc68: add.w r0,r1,#0x28
  000cdc6c: vst1.64 {d10,d11},[r0]
  000cdc70: str r6,[sp,#0x188]
  000cdc72: str r6,[sp,#0x19c]
  000cdc74: str r6,[sp,#0x1c0]
  000cdc76: ldr r0,[r4,#0x74]
  000cdc78: blx r5
  000cdc7a: add r1,sp,#0x148
  000cdc7c: str r0,[r4,#0x3c]
  000cdc7e: orr r0,r1,#0x4
  000cdc82: movs r2,#0x14
  000cdc84: movs r3,#0x0
  000cdc86: vst1.32 {d8,d9},[r0]
  000cdc8a: add.w r0,r1,#0x18
  000cdc8e: vst1.64 {d8,d9},[r0]
  000cdc92: add.w r0,r1,#0x28
  000cdc96: vst1.64 {d10,d11},[r0]
  000cdc9a: str r6,[sp,#0x148]
  000cdc9c: str r6,[sp,#0x15c]
  000cdc9e: str r6,[sp,#0x180]
  000cdca0: ldr r0,[r4,#0x74]
  000cdca2: blx r5
  000cdca4: add r1,sp,#0x108
  000cdca6: str r0,[r4,#0x48]
  000cdca8: orr r0,r1,#0x4
  000cdcac: movs r2,#0x15
  000cdcae: movs r3,#0x0
  000cdcb0: vst1.32 {d8,d9},[r0]
  000cdcb4: add.w r0,r1,#0x18
  000cdcb8: vst1.64 {d8,d9},[r0]
  000cdcbc: add.w r0,r1,#0x28
  000cdcc0: vst1.64 {d10,d11},[r0]
  000cdcc4: str r6,[sp,#0x108]
  000cdcc6: str r6,[sp,#0x11c]
  000cdcc8: str r6,[sp,#0x140]
  000cdcca: ldr r0,[r4,#0x74]
  000cdccc: blx r5
  000cdcce: add r1,sp,#0xc8
  000cdcd0: str r0,[r4,#0x34]
  000cdcd2: orr r0,r1,#0x4
  000cdcd6: movs r2,#0x16
  000cdcd8: movs r3,#0x0
  000cdcda: vst1.32 {d8,d9},[r0]
  000cdcde: add.w r0,r1,#0x18
  000cdce2: vst1.64 {d8,d9},[r0]
  000cdce6: add.w r0,r1,#0x28
  000cdcea: vst1.64 {d10,d11},[r0]
  000cdcee: str r6,[sp,#0xc8]
  000cdcf0: str r6,[sp,#0xdc]
  000cdcf2: str r6,[sp,#0x100]
  000cdcf4: ldr r0,[r4,#0x74]
  000cdcf6: blx r5
  000cdcf8: add r1,sp,#0x88
  000cdcfa: str r0,[r4,#0x50]
  000cdcfc: orr r0,r1,#0x4
  000cdd00: movs r2,#0x17
  000cdd02: movs r3,#0x0
  000cdd04: vst1.32 {d8,d9},[r0]
  000cdd08: add.w r0,r1,#0x18
  000cdd0c: vst1.64 {d8,d9},[r0]
  000cdd10: add.w r0,r1,#0x28
  000cdd14: vst1.64 {d10,d11},[r0]
  000cdd18: str r6,[sp,#0x88]
  000cdd1a: str r6,[sp,#0x9c]
  000cdd1c: str r6,[sp,#0xc0]
  000cdd1e: ldr r0,[r4,#0x74]
  000cdd20: blx r5
  000cdd22: str r0,[r4,#0x54]
  000cdd24: ldr.w r0,[r9,#0x0]
  000cdd28: blx 0x00071770
  000cdd2c: cmp r0,#0x50
  000cdd2e: bne 0x000cdd88
  000cdd30: add r1,sp,#0x48
  000cdd32: movs r2,#0x18
  000cdd34: orr r0,r1,#0x4
  000cdd38: movs r3,#0x0
  000cdd3a: vst1.32 {d8,d9},[r0]
  000cdd3e: add.w r0,r1,#0x18
  000cdd42: vst1.64 {d8,d9},[r0]
  000cdd46: add.w r0,r1,#0x28
  000cdd4a: vst1.64 {d10,d11},[r0]
  000cdd4e: str r6,[sp,#0x48]
  000cdd50: str r6,[sp,#0x5c]
  000cdd52: str r6,[sp,#0x80]
  000cdd54: ldr r0,[r4,#0x74]
  000cdd56: blx 0x000724d8
  000cdd5a: add r1,sp,#0x8
  000cdd5c: str r0,[r4,#0x58]
  000cdd5e: orr r0,r1,#0x4
  000cdd62: movs r2,#0x18
  000cdd64: movs r3,#0x0
  000cdd66: vst1.32 {d8,d9},[r0]
  000cdd6a: add.w r0,r1,#0x18
  000cdd6e: vst1.64 {d8,d9},[r0]
  000cdd72: add.w r0,r1,#0x28
  000cdd76: vst1.64 {d10,d11},[r0]
  000cdd7a: str r6,[sp,#0x8]
  000cdd7c: str r6,[sp,#0x1c]
  000cdd7e: str r6,[sp,#0x40]
  000cdd80: ldr r0,[r4,#0x74]
  000cdd82: blx 0x000724d8
  000cdd86: str r0,[r4,#0x5c]
  000cdd88: ldr r0,[r4,#0x74]
  000cdd8a: blx 0x000742c0
  000cdd8e: ldr r1,[r4,#0x50]
  000cdd90: movs r2,#0x0
  000cdd92: ldr r0,[r4,#0x74]
  000cdd94: blx 0x000723c4
  000cdd98: ldr r1,[r4,#0x54]
  000cdd9a: movs r2,#0x0
  000cdd9c: ldr r0,[r4,#0x74]
  000cdd9e: blx 0x000723c4
  000cdda2: ldr.w r0,[r9,#0x0]
  000cdda6: blx 0x00071770
  000cddaa: cmp r0,#0x50
  000cddac: bne 0x000cddc2
  000cddae: ldr r1,[r4,#0x58]
  000cddb0: movs r2,#0x0
  000cddb2: ldr r0,[r4,#0x74]
  000cddb4: blx 0x000723c4
  000cddb8: ldr r1,[r4,#0x5c]
  000cddba: movs r2,#0x0
  000cddbc: ldr r0,[r4,#0x74]
  000cddbe: blx 0x000723c4
  000cddc2: ldr r1,[0x000cde44]
  000cddc4: ldr.w r0,[r4,#0x9c]
  000cddc8: add r1,pc
  000cddca: ldr r5,[r1,#0x0]
  000cddcc: blx r5
  000cddce: ldr r0,[r4,#0x78]
  000cddd0: blx r5
  000cddd2: ldr.w r0,[r4,#0x84]
  000cddd6: blx r5
  000cddd8: ldr.w r0,[r4,#0x94]
  000cdddc: cbz r0,0x000cdde2
  000cddde: blx 0x000742c0
  000cdde2: ldr r0,[sp,#0x204]
  000cdde4: ldr.w r1,[r10,#0x0]
  000cdde8: subs r0,r1,r0
  000cddea: itttt eq
  000cddec: add.eq.w sp,sp,#0x208
  000cddf0: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  000cddf4: add.eq sp,#0x4
  000cddf6: pop.eq.w {r8,r9,r10,r11}
  000cddfa: it eq
  000cddfc: pop.eq {r4,r5,r6,r7,pc}
  000cddfe: blx 0x0006e824
```
