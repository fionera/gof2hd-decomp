# Level::init
elf 0xadf94 body 1480
Sig: int __stdcall init(EVP_PKEY_CTX * ctx)

## decompile
```c

/* Level::init() */

int Level::init(EVP_PKEY_CTX *ctx)

{
  bool bVar1;
  int iVar2;
  void *pvVar3;
  LODManager *this;
  ParticleSystemManager *pPVar4;
  undefined4 *puVar5;
  int iVar6;
  Mission *pMVar7;
  SolarSystem *this_00;
  uint *puVar8;
  undefined4 uVar9;
  uint uVar10;
  int iVar11;
  undefined4 *puVar12;
  int *piVar13;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float fVar14;
  float extraout_s1;
  float extraout_s1_00;
  float fVar15;
  float extraout_s1_01;
  float extraout_s2;
  float extraout_s2_00;
  float fVar16;
  float extraout_s2_01;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  float local_34;
  float local_30;
  float local_2c;
  int local_28;
  
  piVar13 = *(int **)(DAT_000be25c + 0xbdfa6);
  local_28 = *piVar13;
  iVar2 = *(int *)(ctx + 0x134);
  if (iVar2 == 0) {
    ctx[0x68] = (EVP_PKEY_CTX)0x0;
    ctx[0x1b0] = (EVP_PKEY_CTX)0x0;
    *(undefined2 *)(ctx + 0x189) = 0;
    if (*(Route **)(ctx + 0x108) != (Route *)0x0) {
      pvVar3 = (void *)Route::~Route(*(Route **)(ctx + 0x108));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(ctx + 0x108) = 0;
    if (*(Route **)(ctx + 0x110) != (Route *)0x0) {
      pvVar3 = (void *)Route::~Route(*(Route **)(ctx + 0x110));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(ctx + 0x110) = 0;
    if (*(Route **)(ctx + 0x10c) != (Route *)0x0) {
      pvVar3 = (void *)Route::~Route(*(Route **)(ctx + 0x10c));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(ctx + 0x10c) = 0;
    if (*(Objective **)(ctx + 0x2c) != (Objective *)0x0) {
      pvVar3 = (void *)Objective::~Objective(*(Objective **)(ctx + 0x2c));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(ctx + 0x2c) = 0;
    if (*(Objective **)(ctx + 0x28) != (Objective *)0x0) {
      pvVar3 = (void *)Objective::~Objective(*(Objective **)(ctx + 0x28));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(ctx + 0x28) = 0;
    this = operator_new(0x14);
    LODManager::LODManager(this);
    *(LODManager **)ctx = this;
    pPVar4 = operator_new(100);
    puVar12 = *(undefined4 **)(DAT_000be260 + 0xbe042);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x4e85,0,0xffff,0);
    iVar2 = DAT_000be264;
    *(ParticleSystemManager **)(ctx + 0x78) = pPVar4;
    puVar5 = *(undefined4 **)(iVar2 + 0xbe064);
    iVar2 = Status::inAlienOrbit();
    bVar1 = false;
    if (iVar2 == 0) {
      Status::getSystem();
      iVar2 = SolarSystem::getTextureIndex();
      bVar1 = iVar2 == 0xc;
    }
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x6a72,0,0xffff,0);
    *(ParticleSystemManager **)(ctx + 0x84) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x4e83,1,0xffff,0);
    *(ParticleSystemManager **)(ctx + 0x74) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x4e7a,1,0x4e7a,1);
    *(ParticleSystemManager **)(ctx + 0x80) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x5e20,1,0x5e20,1);
    *(ParticleSystemManager **)(ctx + 0x98) = pPVar4;
    pPVar4 = operator_new(100);
    uVar9 = 0x4e7f;
    if (bVar1) {
      uVar9 = 0x4ea9;
    }
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,uVar9,1,0,0);
    *(ParticleSystemManager **)(ctx + 0x7c) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x4e7c,0,0x4e7c,0);
    *(ParticleSystemManager **)(ctx + 0x88) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x6a7c,1,0x6a7c,1);
    *(ParticleSystemManager **)(ctx + 0x8c) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x6ab9,1,0xffff,0);
    *(ParticleSystemManager **)(ctx + 0x9c) = pPVar4;
    pPVar4 = operator_new(100);
    ParticleSystemManager::ParticleSystemManager(pPVar4,*puVar12,1,0x6aaf,1,0xffff,0);
    *(ParticleSystemManager **)(ctx + 0x94) = pPVar4;
    createSpace((Level *)ctx);
    if (*(int *)(ctx + 0xc0) == 3) {
      createPlayer();
      fVar14 = extraout_s0;
      fVar15 = extraout_s1;
      fVar16 = extraout_s2;
      if ((**(char **)(DAT_000be268 + 0xbe206) == '\0') || (*(int *)(*(int *)(ctx + 0xf0) + 8) == 0)
         ) {
LAB_000be272:
        PlayerEgo::setPosition(fVar14,fVar15,fVar16);
      }
      else {
        iVar2 = Status::getStationStack((Status *)*puVar5);
        if ((iVar2 == 0) ||
           ((*(int *)(*(int *)(iVar2 + 4) + 4) == 0 || (iVar6 = Status::getSystem(), iVar6 == 0))))
        {
LAB_000be242:
          (**(code **)(**(int **)(*(int *)(*(int *)(ctx + 0x100) + 4) + 8) + 0x28))(&local_34);
          fVar14 = extraout_s0_00;
          fVar15 = extraout_s1_00;
          fVar16 = extraout_s2_00;
          goto LAB_000be272;
        }
        Status::getSystem();
        iVar6 = SolarSystem::currentOrbitHasWarpGate();
        if (iVar6 != 0) goto LAB_000be242;
        this_00 = (SolarSystem *)Status::getSystem();
        puVar8 = (uint *)SolarSystem::getStations(this_00);
        fVar14 = extraout_s0_02;
        for (uVar10 = 0; uVar10 < *puVar8; uVar10 = uVar10 + 1) {
          iVar11 = *(int *)(puVar8[1] + uVar10 * 4);
          iVar6 = Station::getIndex(*(Station **)(*(int *)(iVar2 + 4) + 4));
          fVar14 = extraout_s0_03;
          if (iVar11 == iVar6) goto LAB_000be4d2;
        }
        uVar10 = 0xffffffff;
LAB_000be4d2:
        local_34 = 0.0;
        local_30 = 0.0;
        local_2c = DAT_000be590;
        if (-1 < (int)uVar10) {
          StarSystem::getPlanets(*(StarSystem **)(ctx + 0xec));
          AEGeometry::getPosition();
          fVar14 = (float)AbyssEngine::AEMath::Vector::operator=
                                    ((Vector *)&local_34,(Vector *)&local_40);
        }
        AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_34,fVar14);
        PlayerEgo::setPosition(*(undefined4 *)(ctx + 0xf0),local_34,local_30,local_2c);
        local_34 = -local_34;
        local_30 = -local_30;
        local_2c = -local_2c;
        AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_40,(Vector *)&local_34);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_34,(Vector *)&local_40);
        local_40 = 0;
        uStack_3c = 0x3f800000;
        local_38 = 0;
        AEGeometry::setDirection(*(Vector **)(*(int *)(ctx + 0xf0) + 8),(Vector *)&local_34);
      }
      iVar2 = Status::getCurrentCampaignMission();
      if (iVar2 == 1) {
        PlayerEgo::setPosition(extraout_s0_01,extraout_s1_01,extraout_s2_01);
      }
    }
    iVar2 = *(int *)(ctx + 0x134);
  }
  if (iVar2 != 1) {
    *(int *)(ctx + 0x134) = iVar2 + 1;
    iVar2 = 0;
    goto LAB_000be486;
  }
  if (*(int *)(ctx + 0xc0) != 4 && *(int *)(ctx + 0xc0) != 0x17) {
    createAsteroids((Level *)ctx);
    createGasClouds((Level *)ctx);
  }
  puVar5 = *(undefined4 **)(DAT_000be5a4 + 0xbe2b6);
  pMVar7 = (Mission *)Status::getMission();
  if (pMVar7 == (Mission *)0x0) {
    Status::setMission((Mission *)*puVar5);
  }
  iVar2 = *(int *)(ctx + 0xc0);
  if (iVar2 == 3) {
    iVar2 = Mission::isEmpty();
    if ((iVar2 == 0) && (iVar2 = Mission::isCampaignMission(pMVar7), iVar2 != 0)) {
      iVar2 = *(int *)(ctx + 0xc0);
      if (iVar2 != 3) goto LAB_000be392;
      iVar2 = Status::gameWon();
      if ((iVar2 != 0) &&
         (*(char *)(*(int *)(DAT_000be5ac + 0xbe304) + 0x37) == '\0' &&
          *(char *)(*(int *)(DAT_000be5ac + 0xbe304) + 0x35) == '\0')) goto LAB_000be312;
      iVar2 = *(int *)(ctx + 0xc0);
      if (iVar2 != 3) goto LAB_000be392;
      Status::getMission();
      iVar2 = Mission::isEmpty();
      if (iVar2 == 0) {
        pMVar7 = (Mission *)Status::getMission();
        iVar2 = Mission::isCampaignMission(pMVar7);
        if (iVar2 != 0) {
          createCampaignMission((Level *)ctx);
        }
      }
    }
    else {
LAB_000be312:
      createMission((Level *)ctx);
      iVar2 = Status::inBlackMarketSystem();
      if ((**(char **)(DAT_000be5b0 + 0xbe326) != '\0') && (iVar2 != 0)) {
        uVar9 = *(undefined4 *)(ctx + 0xf0);
        PlayerEgo::getPosition();
        local_40 = 0;
        uStack_3c = 0;
        local_38 = DAT_000be5a0;
        AbyssEngine::AEMath::operator+((Vector *)&local_4c,(Vector *)&local_34);
        PlayerEgo::setPosition(uVar9,local_4c,uStack_48,uStack_44);
      }
    }
  }
  else {
LAB_000be392:
    createScene((Level *)ctx);
    *(int *)(ctx + 0xc0) = iVar2;
  }
  createStaticObjects((Level *)ctx);
  iVar2 = *(int *)(ctx + 0xc0);
  if ((iVar2 != 0x17 && iVar2 != 4) &&
     ((iVar2 != 2 || (iVar2 = Status::getCurrentCampaignMission(), iVar2 != 0x2b)))) {
    createSentryGuns((Level *)ctx);
    createFighterTurrets((Level *)ctx);
    createWingmen((Level *)ctx);
  }
  assignGuns((Level *)ctx);
  if (*(int *)(ctx + 0xc0) != 3) {
    *(undefined4 *)(ctx + 0xc0) = 3;
  }
  connectPlayers();
  if (*(Route **)(ctx + 0xf0) != (Route *)0x0) {
    PlayerEgo::setRoute(*(Route **)(ctx + 0xf0));
  }
  puVar8 = *(uint **)(ctx + 0xf8);
  uVar9 = 0;
  if (puVar8 == (uint *)0x0) {
LAB_000be45a:
    iVar2 = 0;
  }
  else {
    iVar2 = 0;
    for (uVar10 = 0; uVar10 < *puVar8; uVar10 = uVar10 + 1) {
      iVar6 = *(int *)(puVar8[1] + uVar10 * 4);
      if (((*(char *)(iVar6 + 0x41) == '\0') && (*(char *)(iVar6 + 0x71) == '\0')) &&
         (*(char *)(iVar6 + 0x3f) == '\0')) {
        iVar6 = KIPlayer::isWingMan();
        puVar8 = *(uint **)(ctx + 0xf8);
        if (((iVar6 == 0) &&
            (iVar6 = *(int *)(puVar8[1] + uVar10 * 4), *(char *)(iVar6 + 0x44) == '\0')) &&
           (*(char *)(iVar6 + 0x3c) == '\0')) {
          iVar2 = iVar2 + (*(byte *)(iVar6 + 0x3d) ^ 1);
        }
      }
    }
    if (puVar8 == (uint *)0x0) goto LAB_000be45a;
    iVar2 = iVar2 - *(int *)(ctx + 0x120);
  }
  *(undefined4 *)(ctx + 0x128) = 0;
  *(int *)(ctx + 0x118) = iVar2;
  if (*(undefined4 **)(ctx + 0xfc) != (undefined4 *)0x0) {
    uVar9 = **(undefined4 **)(ctx + 0xfc);
  }
  *(undefined4 *)(ctx + 0x184) = 0;
  ctx[0x188] = (EVP_PKEY_CTX)0x0;
  *(undefined4 *)(ctx + 0x1c) = 0;
  *(undefined4 *)(ctx + 0x128) = uVar9;
  *(undefined4 *)(ctx + 300) = 0;
  ctx[0x13c] = (EVP_PKEY_CTX)0x0;
  iVar2 = 1;
  ctx[0x70] = (EVP_PKEY_CTX)0x1;
LAB_000be486:
  if (*piVar13 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar2;
}

```
## target disasm
```
  000bdf94: push {r4,r5,r6,r7,lr}
  000bdf96: add r7,sp,#0xc
  000bdf98: push {r8,r9,r10,r11}
  000bdf9c: sub sp,#0x3c
  000bdf9e: mov r4,r0
  000bdfa0: ldr r0,[0x000be25c]
  000bdfa2: add r0,pc
  000bdfa4: ldr.w r11,[r0,#0x0]
  000bdfa8: ldr.w r0,[r11,#0x0]
  000bdfac: str r0,[sp,#0x38]
  000bdfae: ldr.w r0,[r4,#0x134]
  000bdfb2: cmp r0,#0x0
  000bdfb4: bne.w 0x000be294
  000bdfb8: ldr.w r0,[r4,#0x108]
  000bdfbc: movs r5,#0x0
  000bdfbe: strb.w r5,[r4,#0x68]
  000bdfc2: cmp r0,#0x0
  000bdfc4: strb.w r5,[r4,#0x1b0]
  000bdfc8: strh.w r5,[r4,#0x189]
  000bdfcc: beq 0x000bdfd6
  000bdfce: blx 0x00072310
  000bdfd2: blx 0x0006eb48
  000bdfd6: ldr.w r0,[r4,#0x110]
  000bdfda: str.w r5,[r4,#0x108]
  000bdfde: cbz r0,0x000bdfe8
  000bdfe0: blx 0x00072310
  000bdfe4: blx 0x0006eb48
  000bdfe8: ldr.w r0,[r4,#0x10c]
  000bdfec: movs r5,#0x0
  000bdfee: str.w r5,[r4,#0x110]
  000bdff2: cbz r0,0x000bdffc
  000bdff4: blx 0x00072310
  000bdff8: blx 0x0006eb48
  000bdffc: ldr r0,[r4,#0x2c]
  000bdffe: str.w r5,[r4,#0x10c]
  000be002: cbz r0,0x000be00c
  000be004: blx 0x00073b34
  000be008: blx 0x0006eb48
  000be00c: ldr r0,[r4,#0x28]
  000be00e: movs r5,#0x0
  000be010: str r5,[r4,#0x2c]
  000be012: cbz r0,0x000be01c
  000be014: blx 0x00073b34
  000be018: blx 0x0006eb48
  000be01c: movs r0,#0x14
  000be01e: str r5,[r4,#0x28]
  000be020: blx 0x0006eb24
  000be024: mov r5,r0
  000be026: blx 0x00073bdc
  000be02a: movs r0,#0x64
  000be02c: str.w r11,[sp,#0xc]
  000be030: str r5,[r4,#0x0]
  000be032: blx 0x0006eb24
  000be036: mov r5,r0
  000be038: ldr r0,[0x000be260]
  000be03a: movw r10,#0xffff
  000be03e: add r0,pc
  000be040: ldr.w r8,[r0,#0x0]
  000be044: ldr.w r1,[r8,#0x0]
  000be048: movs r0,#0x0
  000be04a: movs r2,#0x1
  000be04c: strd r0,r10,[sp,#0x0]
  000be050: movw r3,#0x4e85
  000be054: str r0,[sp,#0x8]
  000be056: mov r0,r5
  000be058: blx 0x00073be8
  000be05c: ldr r0,[0x000be264]
  000be05e: str r5,[r4,#0x78]
  000be060: add r0,pc
  000be062: ldr r0,[r0,#0x0]
  000be064: str r0,[sp,#0x10]
  000be066: ldr r0,[r0,#0x0]
  000be068: blx 0x000723d0
  000be06c: mov.w r9,#0x0
  000be070: cmp r0,#0x0
  000be072: mov.w r11,#0x0
  000be076: bne 0x000be08e
  000be078: ldr r0,[sp,#0x10]
  000be07a: ldr r0,[r0,#0x0]
  000be07c: blx 0x00071a10
  000be080: blx 0x0007390c
  000be084: subs r0,#0xc
  000be086: clz r0,r0
  000be08a: lsr.w r11,r0,#0x5
  000be08e: movs r0,#0x64
  000be090: blx 0x0006eb24
  000be094: ldr.w r1,[r8,#0x0]
  000be098: mov r5,r0
  000be09a: movs r2,#0x1
  000be09c: movw r3,#0x6a72
  000be0a0: strd r9,r10,[sp,#0x0]
  000be0a4: movs r6,#0x1
  000be0a6: str.w r9,[sp,#0x8]
  000be0aa: blx 0x00073be8
  000be0ae: movs r0,#0x64
  000be0b0: str.w r5,[r4,#0x84]
  000be0b4: blx 0x0006eb24
  000be0b8: ldr.w r1,[r8,#0x0]
  000be0bc: mov r5,r0
  000be0be: movs r0,#0x0
  000be0c0: strd r6,r10,[sp,#0x0]
  000be0c4: str r0,[sp,#0x8]
  000be0c6: mov r0,r5
  000be0c8: movs r2,#0x1
  000be0ca: movw r3,#0x4e83
  000be0ce: blx 0x00073be8
  000be0d2: movs r0,#0x64
  000be0d4: str r5,[r4,#0x74]
  000be0d6: blx 0x0006eb24
  000be0da: ldr.w r1,[r8,#0x0]
  000be0de: mov r5,r0
  000be0e0: movs r6,#0x1
  000be0e2: movw r0,#0x4e7a
  000be0e6: strd r6,r0,[sp,#0x0]
  000be0ea: mov r0,r5
  000be0ec: movs r2,#0x1
  000be0ee: movw r3,#0x4e7a
  000be0f2: str r6,[sp,#0x8]
  000be0f4: blx 0x00073be8
  000be0f8: movs r0,#0x64
  000be0fa: str.w r5,[r4,#0x80]
  000be0fe: blx 0x0006eb24
  000be102: ldr.w r1,[r8,#0x0]
  000be106: mov r5,r0
  000be108: movw r0,#0x5e20
  000be10c: movs r2,#0x1
  000be10e: strd r6,r0,[sp,#0x0]
  000be112: mov r0,r5
  000be114: movw r3,#0x5e20
  000be118: str r6,[sp,#0x8]
  000be11a: blx 0x00073be8
  000be11e: movs r0,#0x64
  000be120: str.w r5,[r4,#0x98]
  000be124: blx 0x0006eb24
  000be128: ldr.w r1,[r8,#0x0]
  000be12c: mov r5,r0
  000be12e: movs r6,#0x0
  000be130: movs r0,#0x1
  000be132: movw r3,#0x4e7f
  000be136: strd r0,r6,[sp,#0x0]
  000be13a: str r6,[sp,#0x8]
  000be13c: cmp.w r11,#0x0
  000be140: it ne
  000be142: movw.ne r3,#0x4ea9
  000be146: mov r0,r5
  000be148: movs r2,#0x1
  000be14a: blx 0x00073be8
  000be14e: movs r0,#0x64
  000be150: str r5,[r4,#0x7c]
  000be152: blx 0x0006eb24
  000be156: ldr.w r1,[r8,#0x0]
  000be15a: mov r5,r0
  000be15c: movw r0,#0x4e7c
  000be160: movs r2,#0x1
  000be162: strd r6,r0,[sp,#0x0]
  000be166: mov r0,r5
  000be168: movw r3,#0x4e7c
  000be16c: str r6,[sp,#0x8]
  000be16e: movs r6,#0x1
  000be170: blx 0x00073be8
  000be174: ldr.w r11,[sp,#0xc]
  000be178: movs r0,#0x64
  000be17a: str.w r5,[r4,#0x88]
  000be17e: blx 0x0006eb24
  000be182: ldr.w r1,[r8,#0x0]
  000be186: mov r5,r0
  000be188: movw r0,#0x6a7c
  000be18c: movs r2,#0x1
  000be18e: strd r6,r0,[sp,#0x0]
  000be192: mov r0,r5
  000be194: movw r3,#0x6a7c
  000be198: str r6,[sp,#0x8]
  000be19a: blx 0x00073be8
  000be19e: movs r0,#0x64
  000be1a0: str.w r5,[r4,#0x8c]
  000be1a4: blx 0x0006eb24
  000be1a8: ldr.w r1,[r8,#0x0]
  000be1ac: mov r5,r0
  000be1ae: movs r6,#0x1
  000be1b0: mov.w r9,#0x0
  000be1b4: movs r2,#0x1
  000be1b6: movw r3,#0x6ab9
  000be1ba: strd r6,r10,[sp,#0x0]
  000be1be: str.w r9,[sp,#0x8]
  000be1c2: blx 0x00073be8
  000be1c6: movs r0,#0x64
  000be1c8: str.w r5,[r4,#0x9c]
  000be1cc: blx 0x0006eb24
  000be1d0: ldr.w r1,[r8,#0x0]
  000be1d4: mov r5,r0
  000be1d6: movs r2,#0x1
  000be1d8: movw r3,#0x6aaf
  000be1dc: strd r6,r10,[sp,#0x0]
  000be1e0: str.w r9,[sp,#0x8]
  000be1e4: blx 0x00073be8
  000be1e8: mov r0,r4
  000be1ea: str.w r5,[r4,#0x94]
  000be1ee: blx 0x00073bf4
  000be1f2: ldr.w r0,[r4,#0xc0]
  000be1f6: cmp r0,#0x3
  000be1f8: bne 0x000be290
  000be1fa: mov r0,r4
  000be1fc: blx 0x00073c00
  000be200: ldr r0,[0x000be268]
  000be202: add r0,pc
  000be204: ldr r1,[r0,#0x0]
  000be206: ldr.w r0,[r4,#0xf0]
  000be20a: ldrb r1,[r1,#0x0]
  000be20c: cbz r1,0x000be26c
  000be20e: ldr r1,[r0,#0x8]
  000be210: cbz r1,0x000be26c
  000be212: ldr r0,[sp,#0x10]
  000be214: ldr r0,[r0,#0x0]
  000be216: blx 0x00073c0c
  000be21a: mov r10,r0
  000be21c: cbz r0,0x000be242
  000be21e: ldr.w r0,[r10,#0x4]
  000be222: ldr r0,[r0,#0x4]
  000be224: cbz r0,0x000be242
  000be226: ldr r0,[sp,#0x10]
  000be228: ldr r0,[r0,#0x0]
  000be22a: blx 0x00071a10
  000be22e: cbz r0,0x000be242
  000be230: ldr r0,[sp,#0x10]
  000be232: ldr r0,[r0,#0x0]
  000be234: blx 0x00071a10
  000be238: blx 0x00073c18
  000be23c: cmp r0,#0x0
  000be23e: beq.w 0x000be49c
  000be242: ldr.w r0,[r4,#0x100]
  000be246: ldr r0,[r0,#0x4]
  000be248: ldr r1,[r0,#0x8]
  000be24a: ldr r0,[r1,#0x0]
  000be24c: ldr r2,[r0,#0x28]
  000be24e: add r0,sp,#0x2c
  000be250: blx r2
  000be252: add r3,sp,#0x2c
  000be254: ldmia r3,{r1,r2,r3}
  000be256: ldr.w r0,[r4,#0xf0]
  000be25a: b 0x000be272
  000be26c: ldr r1,[0x000be594]
  000be26e: ldr r3,[0x000be598]
  000be270: mov r2,r1
  000be272: blx 0x000725bc
  000be276: ldr r0,[sp,#0x10]
  000be278: ldr r0,[r0,#0x0]
  000be27a: blx 0x00071770
  000be27e: cmp r0,#0x1
  000be280: bne 0x000be290
  000be282: ldr r3,[0x000be59c]
  000be284: movs r1,#0x0
  000be286: ldr.w r0,[r4,#0xf0]
  000be28a: movs r2,#0x0
  000be28c: blx 0x000725bc
  000be290: ldr.w r0,[r4,#0x134]
  000be294: cmp r0,#0x1
  000be296: bne 0x000be35c
  000be298: ldr.w r0,[r4,#0xc0]
  000be29c: cmp r0,#0x4
  000be29e: it ne
  000be2a0: cmp.ne r0,#0x17
  000be2a2: beq 0x000be2b0
  000be2a4: mov r0,r4
  000be2a6: blx 0x00073c24
  000be2aa: mov r0,r4
  000be2ac: blx 0x00073c30
  000be2b0: ldr r0,[0x000be5a4]
  000be2b2: add r0,pc
  000be2b4: ldr.w r8,[r0,#0x0]
  000be2b8: ldr.w r0,[r8,#0x0]
  000be2bc: blx 0x00072850
  000be2c0: mov r5,r0
  000be2c2: cbnz r0,0x000be2d4
  000be2c4: ldr r0,[0x000be5a8]
  000be2c6: add r0,pc
  000be2c8: ldr r0,[r0,#0x0]
  000be2ca: ldr r1,[r0,#0x0]
  000be2cc: ldr.w r0,[r8,#0x0]
  000be2d0: blx 0x00073c3c
  000be2d4: ldr.w r6,[r4,#0xc0]
  000be2d8: cmp r6,#0x3
  000be2da: bne 0x000be392
  000be2dc: mov r0,r5
  000be2de: blx 0x00072868
  000be2e2: cbnz r0,0x000be312
  000be2e4: mov r0,r5
  000be2e6: blx 0x0007372c
  000be2ea: cbz r0,0x000be312
  000be2ec: ldr.w r6,[r4,#0xc0]
  000be2f0: cmp r6,#0x3
  000be2f2: bne 0x000be392
  000be2f4: ldr.w r0,[r8,#0x0]
  000be2f8: blx 0x00073c48
  000be2fc: cbz r0,0x000be366
  000be2fe: ldr r0,[0x000be5ac]
  000be300: add r0,pc
  000be302: ldr r0,[r0,#0x0]
  000be304: ldrb.w r1,[r0,#0x35]
  000be308: ldrb.w r0,[r0,#0x37]
  000be30c: orrs r0,r1
  000be30e: lsls r0,r0,#0x18
  000be310: bne 0x000be366
  000be312: mov r0,r4
  000be314: blx 0x00073c54
  000be318: ldr.w r0,[r8,#0x0]
  000be31c: blx 0x00071998
  000be320: ldr r1,[0x000be5b0]
  000be322: add r1,pc
  000be324: ldr r1,[r1,#0x0]
  000be326: ldrb r1,[r1,#0x0]
  000be328: cbz r1,0x000be39c
  000be32a: cbz r0,0x000be39c
  000be32c: ldr.w r5,[r4,#0xf0]
  000be330: add.w r9,sp,#0x2c
  000be334: mov r0,r9
  000be336: mov r1,r5
  000be338: blx 0x0007264c
  000be33c: movs r1,#0x0
  000be33e: ldr r0,[0x000be5a0]
  000be340: strd r1,r1,[sp,#0x20]
  000be344: add r2,sp,#0x20
  000be346: str r0,[sp,#0x28]
  000be348: add r0,sp,#0x14
  000be34a: mov r1,r9
  000be34c: blx 0x0006f1cc
  000be350: add r3,sp,#0x14
  000be352: mov r0,r5
  000be354: ldmia r3,{r1,r2,r3}
  000be356: blx 0x00072cb8
  000be35a: b 0x000be39c
  000be35c: adds r0,#0x1
  000be35e: str.w r0,[r4,#0x134]
  000be362: movs r0,#0x0
  000be364: b 0x000be486
  000be366: ldr.w r6,[r4,#0xc0]
  000be36a: cmp r6,#0x3
  000be36c: bne 0x000be392
  000be36e: ldr.w r0,[r8,#0x0]
  000be372: blx 0x00072850
  000be376: blx 0x00072868
  000be37a: cbnz r0,0x000be39c
  000be37c: ldr.w r0,[r8,#0x0]
  000be380: blx 0x00072850
  000be384: blx 0x0007372c
  000be388: cbz r0,0x000be39c
  000be38a: mov r0,r4
  000be38c: blx 0x00073c60
  000be390: b 0x000be39c
  000be392: mov r0,r4
  000be394: blx 0x00073c6c
  000be398: str.w r6,[r4,#0xc0]
  000be39c: mov r0,r4
  000be39e: blx 0x00073c78
  000be3a2: ldr.w r0,[r4,#0xc0]
  000be3a6: cmp r0,#0x17
  000be3a8: it ne
  000be3aa: cmp.ne r0,#0x4
  000be3ac: beq 0x000be3d0
  000be3ae: cmp r0,#0x2
  000be3b0: bne 0x000be3be
  000be3b2: ldr.w r0,[r8,#0x0]
  000be3b6: blx 0x00071770
  000be3ba: cmp r0,#0x2b
  000be3bc: beq 0x000be3d0
  000be3be: mov r0,r4
  000be3c0: blx 0x00073c84
  000be3c4: mov r0,r4
  000be3c6: blx 0x00073c90
  000be3ca: mov r0,r4
  000be3cc: blx 0x00073c9c
  000be3d0: mov r0,r4
  000be3d2: blx 0x00073ca8
  000be3d6: ldr.w r0,[r4,#0xc0]
  000be3da: cmp r0,#0x3
  000be3dc: itt ne
  000be3de: mov.ne r0,#0x3
  000be3e0: str.w.ne r0,[r4,#0xc0]
  000be3e4: mov r0,r4
  000be3e6: blx 0x00073cb4
  000be3ea: ldr.w r0,[r4,#0xf0]
  000be3ee: cbz r0,0x000be3f8
  000be3f0: ldr.w r1,[r4,#0x108]
  000be3f4: blx 0x00073cc0
  000be3f8: ldr.w r1,[r4,#0xf8]
  000be3fc: movs r5,#0x0
  000be3fe: cbz r1,0x000be45a
  000be400: mov.w r9,#0x0
  000be404: movs r6,#0x0
  000be406: b 0x000be448
  000be408: ldr r0,[r1,#0x4]
  000be40a: ldr.w r0,[r0,r6,lsl #0x2]
  000be40e: ldrb.w r2,[r0,#0x41]
  000be412: cbnz r2,0x000be446
  000be414: ldrb.w r2,[r0,#0x71]
  000be418: cbnz r2,0x000be446
  000be41a: ldrb.w r2,[r0,#0x3f]
  000be41e: cbnz r2,0x000be446
  000be420: blx 0x00072ed4
  000be424: ldr.w r1,[r4,#0xf8]
  000be428: cbnz r0,0x000be446
  000be42a: ldr r0,[r1,#0x4]
  000be42c: ldr.w r0,[r0,r6,lsl #0x2]
  000be430: ldrb.w r2,[r0,#0x44]
  000be434: cbnz r2,0x000be446
  000be436: ldrb.w r2,[r0,#0x3c]
  000be43a: cbnz r2,0x000be446
  000be43c: ldrb.w r0,[r0,#0x3d]
  000be440: eor r0,r0,#0x1
  000be444: add r9,r0
  000be446: adds r6,#0x1
  000be448: ldr r0,[r1,#0x0]
  000be44a: cmp r6,r0
  000be44c: bcc 0x000be408
  000be44e: cbz r1,0x000be45a
  000be450: ldr.w r0,[r4,#0x120]
  000be454: sub.w r0,r9,r0
  000be458: b 0x000be45c
  000be45a: movs r0,#0x0
  000be45c: str.w r5,[r4,#0x128]
  000be460: ldr.w r1,[r4,#0xfc]
  000be464: str.w r0,[r4,#0x118]
  000be468: cbz r1,0x000be46c
  000be46a: ldr r5,[r1,#0x0]
  000be46c: movs r0,#0x0
  000be46e: str.w r0,[r4,#0x184]
  000be472: strb.w r0,[r4,#0x188]
  000be476: str r0,[r4,#0x1c]
  000be478: strd r5,r0,[r4,#0x128]
  000be47c: strb.w r0,[r4,#0x13c]
  000be480: movs r0,#0x1
  000be482: strb.w r0,[r4,#0x70]
  000be486: ldr r1,[sp,#0x38]
  000be488: ldr.w r2,[r11,#0x0]
  000be48c: subs r1,r2,r1
  000be48e: ittt eq
  000be490: add.eq sp,#0x3c
  000be492: pop.eq.w {r8,r9,r10,r11}
  000be496: pop.eq {r4,r5,r6,r7,pc}
  000be498: blx 0x0006e824
  000be49c: ldr r0,[sp,#0x10]
  000be49e: ldr r0,[r0,#0x0]
  000be4a0: blx 0x00071a10
  000be4a4: blx 0x00071a88
  000be4a8: mov r8,r0
  000be4aa: movs r5,#0x0
  000be4ac: b 0x000be4c6
  000be4ae: ldr.w r0,[r8,#0x4]
  000be4b2: ldr.w r1,[r10,#0x4]
  000be4b6: ldr.w r6,[r0,r5,lsl #0x2]
  000be4ba: ldr r0,[r1,#0x4]
  000be4bc: blx 0x00071824
  000be4c0: cmp r6,r0
  000be4c2: beq 0x000be4d2
  000be4c4: adds r5,#0x1
  000be4c6: ldr.w r0,[r8,#0x0]
  000be4ca: cmp r5,r0
  000be4cc: bcc 0x000be4ae
  000be4ce: mov.w r5,#0xffffffff
  000be4d2: mov.w r10,#0x0
  000be4d6: cmp r5,#0x0
  000be4d8: ldr r0,[0x000be590]
  000be4da: strd r10,r10,[sp,#0x2c]
  000be4de: str r0,[sp,#0x34]
  000be4e0: blt 0x000be502
  000be4e2: ldr.w r0,[r4,#0xec]
  000be4e6: blx 0x00073ccc
  000be4ea: ldr r0,[r0,#0x4]
  000be4ec: add.w r0,r0,r5, lsl #0x2
  000be4f0: add r5,sp,#0x20
  000be4f2: ldr r1,[r0,#0x4]
  000be4f4: mov r0,r5
  000be4f6: blx 0x000720b8
  000be4fa: add r0,sp,#0x2c
  000be4fc: mov r1,r5
  000be4fe: blx 0x0006eb3c
  000be502: add r5,sp,#0x2c
  000be504: mov.w r1,#0x40800000
  000be508: mov r0,r5
  000be50a: blx 0x00072628
  000be50e: add r3,sp,#0x2c
  000be510: ldmia r3,{r1,r2,r3}
  000be512: ldr.w r0,[r4,#0xf0]
  000be516: blx 0x00072cb8
  000be51a: vldr.32 s0,[sp,#0x2c]
  000be51e: add r6,sp,#0x20
  000be520: vldr.32 s2,[sp,#0x30]
  000be524: mov r1,r5
  000be526: vldr.32 s4,[sp,#0x34]
  000be52a: vneg.f32 s0,s0
  000be52e: vneg.f32 s2,s2
  000be532: mov r0,r6
  000be534: vneg.f32 s4,s4
  000be538: vstr.32 s0,[sp,#0x2c]
  000be53c: vstr.32 s2,[sp,#0x30]
  000be540: vstr.32 s4,[sp,#0x34]
  000be544: blx 0x0006ec80
  000be548: mov r0,r5
  000be54a: mov r1,r6
  000be54c: blx 0x0006eb3c
  000be550: ldr.w r0,[r4,#0xf0]
  000be554: mov.w r1,#0x3f800000
  000be558: add r2,sp,#0x20
  000be55a: ldr r0,[r0,#0x8]
  000be55c: strd r10,r1,[sp,#0x20]
  000be560: mov r1,r5
  000be562: str.w r10,[sp,#0x28]
  000be566: blx 0x000726ac
  000be56a: b 0x000be276
```
