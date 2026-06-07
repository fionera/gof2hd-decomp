# Generator::createMission
elf 0x971b8 body 1832
Sig: undefined __thiscall createMission(Generator * this, Agent * param_1, Array * param_2)

## decompile
```c

/* Generator::createMission(Agent*, Array<SolarSystem*>*) */

void __thiscall Generator::createMission(Generator *this,Agent *param_1,Array *param_2)

{
  bool bVar1;
  Generator *pGVar2;
  int iVar3;
  SolarSystem *pSVar4;
  uint uVar5;
  undefined1 *puVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  Station *pSVar12;
  float fVar13;
  Standing *this_00;
  Mission *this_01;
  int extraout_r1;
  int extraout_r1_00;
  undefined4 extraout_r1_01;
  int extraout_r1_02;
  int extraout_r1_03;
  int extraout_r1_04;
  int *piVar14;
  Galaxy *this_02;
  int *piVar15;
  SolarSystem *pSVar16;
  int *piVar17;
  int iVar18;
  uint in_fpscr;
  float fVar19;
  float fVar20;
  int local_98;
  int local_8c;
  uint local_84;
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  int local_4c;
  
  piVar14 = *(int **)(DAT_000a7500 + 0xa71d0);
  local_4c = *piVar14;
  pGVar2 = (Generator *)Agent::getStation(param_1);
  local_8c = generateStationIndex(pGVar2,param_2,(int)pGVar2);
  piVar15 = *(int **)(DAT_000a7504 + 0xa71ea);
  Status::getSystem();
  iVar3 = SolarSystem::getIndex();
  if (iVar3 == 0xf) {
    pSVar4 = (SolarSystem *)Status::getSystem();
    iVar3 = SolarSystem::getStations(pSVar4);
    iVar3 = **(int **)(iVar3 + 4);
    local_8c = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a7508 + 0xa720c));
    local_8c = local_8c + iVar3;
  }
  uVar5 = Agent::getRace(param_1);
  iVar3 = 999;
  bVar1 = false;
  piVar17 = *(int **)(*piVar15 + 0x50);
  piVar15 = *(int **)(DAT_000a750c + 0xa7244);
  uVar10 = 0;
LAB_000a72d4:
  if (!bVar1) {
    bVar1 = iVar3 < 1;
    uVar9 = AbyssEngine::AERandom::nextInt(*piVar15);
    iVar3 = iVar3 + -1;
    uVar10 = uVar9;
    if ((uVar9 != 8) && ((uVar9 != 10 || (uVar10 = 10, uVar5 < 4)))) {
      if (piVar17 != (int *)0x0) {
        puVar6 = (undefined1 *)piVar17[1];
        if (puVar6[uVar9] != '\0') {
          iVar7 = *piVar17;
          iVar18 = 0;
          for (iVar11 = 0; iVar7 != iVar11; iVar11 = iVar11 + 1) {
            iVar18 = iVar18 + (uint)(byte)puVar6[iVar11];
          }
          uVar10 = uVar9;
          if (iVar18 == 0xe) {
            for (; iVar7 != 0; iVar7 = iVar7 + -1) {
              *puVar6 = 0;
              puVar6 = puVar6 + 1;
            }
          }
          goto LAB_000a72d4;
        }
        puVar6[uVar9] = 1;
      }
      if (uVar9 == 0xc) {
        Status::getSystem();
        iVar7 = SolarSystem::getIndex();
        bVar1 = true;
        uVar10 = 0xc;
        if (iVar7 != 0x19) {
          uVar10 = uVar9;
        }
      }
      else {
        bVar1 = true;
        uVar10 = uVar9;
      }
    }
    goto LAB_000a72d4;
  }
  iVar7 = Status::getCurrentCampaignMission();
  iVar3 = local_8c;
  if ((iVar7 < 0x10) ||
     ((uVar10 == 0xf && (*(char *)(*(int *)(DAT_000a7510 + 0xa72f2) + 0x37) == '\0')))) {
    uVar8 = AbyssEngine::AERandom::nextInt(*piVar15);
    local_84 = 0xb;
    switch(uVar8) {
    case 0:
      break;
    case 1:
      local_84 = 0;
      break;
    case 2:
      local_84 = 7;
      break;
    case 3:
      local_84 = 4;
      break;
    case 4:
      goto switchD_000a7310_caseD_4;
    default:
      goto switchD_000a7310_default;
    }
  }
  else {
switchD_000a7310_default:
    local_84 = uVar10;
    if (uVar10 == 0xc) {
switchD_000a7310_caseD_4:
      iVar3 = Agent::getStation(param_1);
      local_84 = 0xc;
    }
  }
  iVar7 = Agent::getOffer(param_1);
  if (iVar7 == 5) {
    iVar3 = Agent::getStation(param_1);
    local_84 = 8;
  }
  else {
    if ((local_84 < 0xf) && ((1 << (local_84 & 0xff) & 0x4801U) != 0)) {
      while( true ) {
        pSVar12 = (Station *)Status::getStation();
        iVar7 = Station::getIndex(pSVar12);
        if (iVar3 != iVar7) break;
        pGVar2 = (Generator *)Agent::getStation(param_1);
        iVar3 = generateStationIndex(pGVar2,param_2,(int)pGVar2);
      }
    }
    if ((uVar5 < 4) && (local_84 == 0xd)) {
      Agent::getSystem(param_1);
      iVar7 = SolarSystem::getRoutes();
      if (iVar7 == 0) {
        iVar7 = AbyssEngine::AERandom::nextInt(*piVar15);
        local_84 = 4;
        if (iVar7 == 0) {
          local_84 = 1;
        }
      }
      else {
LAB_000a73ac:
        bVar1 = false;
        while (!bVar1) {
          pGVar2 = (Generator *)Agent::getStation(param_1);
          iVar3 = generateStationIndex(pGVar2,param_2,(int)pGVar2);
          uVar10 = 0;
          while( true ) {
            if (*(uint *)param_2 <= uVar10) goto LAB_000a73ac;
            iVar7 = SolarSystem::stationIsInSystem
                              (*(SolarSystem **)(*(int *)(param_2 + 4) + uVar10 * 4),iVar3);
            if ((iVar7 != 0) && (uVar9 = SolarSystem::getRace(), uVar9 == uVar5)) break;
            uVar10 = uVar10 + 1;
          }
          bVar1 = true;
        }
        local_84 = 0xd;
      }
    }
  }
  Agent::getName();
  iVar7 = Status::getCurrentCampaignMission();
  if (iVar7 < 0x10) {
    iVar7 = AbyssEngine::AERandom::nextInt(*piVar15);
  }
  else {
    iVar7 = AbyssEngine::AERandom::nextInt(*piVar15);
  }
  if (local_84 == 8) {
    iVar7 = **(int **)(DAT_000a7514 + 0xa742c);
    do {
      do {
        do {
          uVar10 = AbyssEngine::AERandom::nextInt(*piVar15);
          iVar18 = uVar10 + 0x61;
          iVar11 = Item::getOccurence(*(Item **)(*(int *)(iVar7 + 4) + iVar18 * 4));
        } while (iVar11 == 0);
        iVar11 = Item::getSinglePrice(*(Item **)(*(int *)(iVar7 + 4) + iVar18 * 4));
      } while (iVar11 == 0);
      iVar11 = Item::getIngredients();
    } while (((((uVar10 & 0xfffffffe) == 0x78 || iVar18 == 0x75) ||
              ((uVar10 & 0xfffffffe) == 0x12 || iVar18 == 0x83)) ||
             (iVar18 == 0xa4 || iVar18 == 0xaf)) || (iVar11 != 0));
    local_8c = AbyssEngine::AERandom::nextInt(*piVar15);
    iVar7 = Item::getTecLevel(*(Item **)(*(int *)(iVar7 + 4) + iVar18 * 4));
    local_8c = local_8c + 5;
    goto switchD_000a74bc_caseD_1;
  }
  iVar7 = iVar7 + 1;
  local_8c = 0;
  iVar18 = 0;
  switch(local_84) {
  case 0:
    iVar18 = AbyssEngine::AERandom::nextInt(*piVar15);
    fVar13 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
    local_8c = (int)((fVar13 / 10.0) * DAT_000a795c) + 5;
    break;
  case 1:
  case 4:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
    break;
  case 2:
    uVar8 = AbyssEngine::AERandom::nextInt(*piVar15);
    fVar13 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
    goto LAB_000a75fa;
  case 3:
  case 5:
    iVar18 = 0x75;
    fVar13 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
    if (local_84 == 3) {
      iVar18 = 0x74;
    }
    local_8c = (int)((fVar13 / 10.0) * 8.0) + 2;
    break;
  case 0xb:
    fVar13 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
    fVar13 = (fVar13 / 10.0) * 18.0;
LAB_000a75fa:
    local_8c = (int)fVar13 + 2;
LAB_000a7606:
    iVar18 = 0;
    break;
  default:
    if (local_84 == 0xf) {
      uVar8 = AbyssEngine::AERandom::nextInt(*piVar15);
      uVar10 = 0;
      fVar13 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
      local_8c = (int)fVar13 + 0x1e;
      do {
        if (*(uint *)param_2 <= uVar10) {
          iVar18 = 0;
          goto switchD_000a74bc_caseD_1;
        }
        iVar11 = SolarSystem::stationIsInSystem
                           (*(SolarSystem **)(*(int *)(param_2 + 4) + uVar10 * 4),iVar3);
        uVar10 = uVar10 + 1;
      } while (iVar11 == 0);
      this_02 = (Galaxy *)**(undefined4 **)(DAT_000a7924 + 0xa7564);
      pSVar12 = (Station *)Status::getStation();
      iVar11 = Galaxy::getAsteroidProbabilities(this_02,pSVar12);
      iVar18 = AbyssEngine::AERandom::nextInt(*piVar15);
      iVar18 = *(int *)(iVar11 + iVar18 * 8);
      break;
    }
    goto LAB_000a7606;
  }
switchD_000a74bc_caseD_1:
  piVar17 = *(int **)(DAT_000a7978 + 0xa7612);
  pSVar4 = (SolarSystem *)*piVar17;
  if (9 < iVar7) {
    iVar7 = 10;
  }
  pSVar12 = (Station *)Status::getStation();
  iVar11 = Station::getSystem(pSVar12);
  pSVar16 = *(SolarSystem **)(*(int *)(param_2 + 4) + iVar11 * 4);
  pSVar12 = (Station *)Galaxy::getStation(*piVar17);
  Station::getSystem(pSVar12);
  fVar13 = (float)Galaxy::distance(pSVar4,pSVar16);
  fVar19 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
  fVar19 = (float)VectorSignedToFloat((int)((fVar19 / 10.0) * DAT_000a7964) + 0x5dc,
                                      (byte)(in_fpscr >> 0x16) & 3);
  fVar19 = (fVar13 / DAT_000a7960 + 1.0) * fVar19;
  fVar13 = DAT_000a7968;
  if ((local_84 == 9) || (fVar13 = DAT_000a796c, local_84 == 7)) {
    fVar19 = fVar19 * fVar13;
  }
  else if (local_84 == 8) {
    iVar11 = Item::getMaxPrice(*(Item **)(*(int *)(**(int **)(DAT_000a797c + 0xa786c) + 4) +
                                         iVar18 * 4));
    fVar19 = (float)VectorSignedToFloat(local_8c * iVar11,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = fVar19 * DAT_000a7974;
  }
  else if (local_84 == 3 || local_84 == 5) {
    fVar19 = fVar19 + fVar19;
  }
  else if (local_84 == 0xb) {
    fVar13 = (float)VectorSignedToFloat(local_8c,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = fVar19 * DAT_000a7970 + ((fVar19 * DAT_000a7970) / 5.0) * fVar13;
  }
  iVar11 = Status::getLevel();
  fVar13 = (float)VectorSignedToFloat(iVar11 * iVar11 * iVar11 * 10,(byte)(in_fpscr >> 0x16) & 3);
  fVar19 = fVar19 + fVar13;
  if ((local_84 | 4) == 0xc) {
    local_98 = 0;
  }
  else {
    this_00 = (Standing *)Status::getStanding();
    iVar11 = Agent::getRace(param_1);
    fVar13 = (float)Standing::getMissionBonus(this_00,iVar11);
    iVar11 = (int)(fVar19 * fVar13);
    __aeabi_idivmod(iVar11,0x32);
    local_98 = extraout_r1 + iVar11;
    __aeabi_idivmod(local_98,0x32);
    if (extraout_r1_00 != 0) {
      local_98 = iVar11 - extraout_r1;
    }
  }
  __aeabi_idivmod((int)fVar19,0x32);
  fVar20 = (float)VectorSignedToFloat(extraout_r1_01,(byte)(in_fpscr >> 0x16) & 3);
  __aeabi_idivmod((int)(fVar19 + fVar20),0x32);
  fVar13 = fVar19 - fVar20;
  if (extraout_r1_02 == 0) {
    fVar13 = fVar19 + fVar20;
  }
  this_01 = operator_new(0x78);
  AbyssEngine::String::String(aSStack_64,aSStack_58,false);
  uVar8 = Agent::getImageParts(param_1);
  Mission::Mission(this_01,local_84,aSStack_64,uVar8,uVar5,(int)fVar13,iVar3,iVar7);
  AbyssEngine::String::~String(aSStack_64);
  __aeabi_idiv((int)fVar13,10);
  uVar8 = AbyssEngine::AERandom::nextInt(*piVar15);
  fVar19 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
  iVar7 = (int)(fVar13 / 10.0 + fVar19);
  if (local_84 == 8) {
    fVar13 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
    iVar7 = (int)(fVar13 * 0.5);
  }
  else if (local_84 == 6) {
    Globals::getRandomName((int)aSStack_70,SUB41(**(undefined4 **)(DAT_000a7980 + 0xa77ee),0));
    Mission::setTargetName(this_01,aSStack_70);
    AbyssEngine::String::~String(aSStack_70);
  }
  __aeabi_idivmod(iVar7,0x32);
  iVar11 = extraout_r1_03 + iVar7;
  __aeabi_idivmod(iVar11,0x32);
  if (extraout_r1_04 != 0) {
    iVar11 = iVar7 - extraout_r1_03;
  }
  Mission::setCosts(this_01,iVar11);
  Mission::setProductionGoods(this_01,iVar18,local_8c);
  Mission::setBonus(this_01,local_98);
  uVar10 = 0;
  do {
    if (*(uint *)param_2 <= uVar10) {
LAB_000a78aa:
      AbyssEngine::String::~String(aSStack_58);
      iVar3 = *piVar14 - local_4c;
      if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(iVar3);
      }
      return;
    }
    iVar7 = SolarSystem::stationIsInSystem
                      (*(SolarSystem **)(*(int *)(param_2 + 4) + uVar10 * 4),iVar3);
    if (iVar7 != 0) {
      SolarSystem::getName();
      Mission::setTargetSystemName(this_01);
      AbyssEngine::String::~String(aSStack_7c);
      goto LAB_000a78aa;
    }
    uVar10 = uVar10 + 1;
  } while( true );
}

```

## target disasm
```
  000a71b8: push {r4,r5,r6,r7,lr}
  000a71ba: add r7,sp,#0xc
  000a71bc: push {r7,r8,r9,r10,r11}
  000a71c0: vpush {d8,d9,d10,d11}
  000a71c4: sub sp,#0x60
  000a71c6: ldr r0,[0x000a7500]
  000a71c8: mov r8,r2
  000a71ca: mov r9,r1
  000a71cc: add r0,pc
  000a71ce: ldr r4,[r0,#0x0]
  000a71d0: ldr r0,[r4,#0x0]
  000a71d2: str r0,[sp,#0x5c]
  000a71d4: mov r0,r1
  000a71d6: blx 0x00071ac4
  000a71da: mov r1,r8
  000a71dc: mov r2,r0
  000a71de: blx 0x00071c08
  000a71e2: mov r5,r0
  000a71e4: ldr r0,[0x000a7504]
  000a71e6: add r0,pc
  000a71e8: ldr r6,[r0,#0x0]
  000a71ea: ldr r0,[r6,#0x0]
  000a71ec: blx 0x00071a10
  000a71f0: blx 0x00071a7c
  000a71f4: cmp r0,#0xf
  000a71f6: str r4,[sp,#0x14]
  000a71f8: bne 0x000a721c
  000a71fa: ldr r0,[r6,#0x0]
  000a71fc: blx 0x00071a10
  000a7200: blx 0x00071a88
  000a7204: ldr r1,[0x000a7508]
  000a7206: ldr r2,[r0,#0x4]
  000a7208: add r1,pc
  000a720a: ldr r1,[r1,#0x0]
  000a720c: ldr r5,[r2,#0x0]
  000a720e: ldr r0,[r1,#0x0]
  000a7210: movs r1,#0x4
  000a7212: blx 0x00071848
  000a7216: add r0,r5
  000a7218: str r0,[sp,#0x1c]
  000a721a: b 0x000a721e
  000a721c: str r5,[sp,#0x1c]
  000a721e: mov r0,r9
  000a7220: str.w r9,[sp,#0x18]
  000a7224: blx 0x00071bd8
  000a7228: strd r6,r0,[sp,#0x20]
  000a722c: movs r5,#0x0
  000a722e: ldr r0,[r6,#0x0]
  000a7230: movw r9,#0x3e7
  000a7234: movs r2,#0x0
  000a7236: mov.w r11,#0x0
  000a723a: ldr.w r10,[r0,#0x50]
  000a723e: ldr r0,[0x000a750c]
  000a7240: add r0,pc
  000a7242: ldr r0,[r0,#0x0]
  000a7244: str r0,[sp,#0x28]
  000a7246: b 0x000a72d4
  000a7248: mov.w r11,#0x0
  000a724c: cmp.w r9,#0x1
  000a7250: it lt
  000a7252: mov.lt.w r11,#0x1
  000a7256: ldr r0,[sp,#0x28]
  000a7258: movs r1,#0xf
  000a725a: ldr r0,[r0,#0x0]
  000a725c: blx 0x00071848
  000a7260: sub.w r9,r9,#0x1
  000a7264: mov r6,r0
  000a7266: cmp r0,#0x8
  000a7268: beq 0x000a72d2
  000a726a: cmp r6,#0xa
  000a726c: bne 0x000a7276
  000a726e: ldr r0,[sp,#0x24]
  000a7270: movs r2,#0xa
  000a7272: cmp r0,#0x3
  000a7274: bhi 0x000a72d4
  000a7276: cmp.w r10,#0x0
  000a727a: beq 0x000a72b0
  000a727c: ldr.w r0,[r10,#0x4]
  000a7280: ldrb r1,[r0,r6]
  000a7282: cbz r1,0x000a72ac
  000a7284: ldr.w r1,[r10,#0x0]
  000a7288: movs r2,#0x0
  000a728a: movs r3,#0x0
  000a728c: b 0x000a7294
  000a728e: ldrb r4,[r0,r2]
  000a7290: adds r2,#0x1
  000a7292: add r3,r4
  000a7294: cmp r1,r2
  000a7296: bne 0x000a728e
  000a7298: cmp r3,#0xe
  000a729a: mov r2,r6
  000a729c: bne 0x000a72d4
  000a729e: b 0x000a72a6
  000a72a0: strb.w r5,[r0],#0x1
  000a72a4: subs r1,#0x1
  000a72a6: cmp r1,#0x0
  000a72a8: bne 0x000a72a0
  000a72aa: b 0x000a72d2
  000a72ac: movs r1,#0x1
  000a72ae: strb r1,[r0,r6]
  000a72b0: cmp r6,#0xc
  000a72b2: bne 0x000a72ce
  000a72b4: ldr r0,[sp,#0x20]
  000a72b6: ldr r0,[r0,#0x0]
  000a72b8: blx 0x00071a10
  000a72bc: blx 0x00071a7c
  000a72c0: movs r2,#0xc
  000a72c2: cmp r0,#0x19
  000a72c4: mov.w r11,#0x1
  000a72c8: it ne
  000a72ca: mov.ne r2,r6
  000a72cc: b 0x000a72d4
  000a72ce: mov.w r11,#0x1
  000a72d2: mov r2,r6
  000a72d4: lsls.w r0,r11,#0x1f
  000a72d8: beq 0x000a7248
  000a72da: ldr r6,[sp,#0x20]
  000a72dc: mov r4,r2
  000a72de: ldr r0,[r6,#0x0]
  000a72e0: blx 0x00071770
  000a72e4: cmp r0,#0x10
  000a72e6: blt 0x000a72f8
  000a72e8: cmp r4,#0xf
  000a72ea: bne 0x000a731e
  000a72ec: ldr r0,[0x000a7510]
  000a72ee: add r0,pc
  000a72f0: ldr r0,[r0,#0x0]
  000a72f2: ldrb.w r0,[r0,#0x37]
  000a72f6: cbnz r0,0x000a731e
  000a72f8: ldr r0,[sp,#0x28]
  000a72fa: movs r1,#0x5
  000a72fc: ldr r0,[r0,#0x0]
  000a72fe: blx 0x00071848
  000a7302: cmp r0,#0x4
  000a7304: bhi 0x000a731e
  000a7306: ldrd r9,r4,[sp,#0x18]
  000a730a: movs r2,#0xb
  000a730c: ldr.w r11,[sp,#0x24]
  000a7310: tbb [pc,r0]
  000a731a: movs r2,#0x0
  000a731c: b 0x000a733e
  000a731e: mov r2,r4
  000a7320: cmp r4,#0xc
  000a7322: ldrd r9,r4,[sp,#0x18]
  000a7326: ldr.w r11,[sp,#0x24]
  000a732a: bne 0x000a733e
  000a732c: mov r0,r9
  000a732e: blx 0x00071ac4
  000a7332: mov r4,r0
  000a7334: movs r2,#0xc
  000a7336: b 0x000a733e
  000a7338: movs r2,#0x7
  000a733a: b 0x000a733e
  000a733c: movs r2,#0x4
  000a733e: mov r0,r9
  000a7340: str r2,[sp,#0x24]
  000a7342: blx 0x00071ae8
  000a7346: cmp r0,#0x5
  000a7348: bne 0x000a7356
  000a734a: mov r0,r9
  000a734c: blx 0x00071ac4
  000a7350: mov r4,r0
  000a7352: movs r0,#0x8
  000a7354: b 0x000a73f8
  000a7356: ldr r1,[sp,#0x24]
  000a7358: cmp r1,#0xe
  000a735a: bhi 0x000a7388
  000a735c: movs r0,#0x1
  000a735e: lsls r0,r1
  000a7360: movw r1,#0x4801
  000a7364: tst r0,r1
  000a7366: bne 0x000a737a
  000a7368: b 0x000a7388
  000a736a: mov r0,r9
  000a736c: blx 0x00071ac4
  000a7370: mov r1,r8
  000a7372: mov r2,r0
  000a7374: blx 0x00071c08
  000a7378: mov r4,r0
  000a737a: ldr r0,[r6,#0x0]
  000a737c: blx 0x00071c14
  000a7380: blx 0x00071824
  000a7384: cmp r4,r0
  000a7386: beq 0x000a736a
  000a7388: cmp.w r11,#0x3
  000a738c: bhi 0x000a73fa
  000a738e: ldr r0,[sp,#0x24]
  000a7390: cmp r0,#0xd
  000a7392: bne 0x000a73fa
  000a7394: mov r0,r9
  000a7396: blx 0x00071c20
  000a739a: ldr.w r1,[r8,#0x4]
  000a739e: ldr.w r0,[r1,r0,lsl #0x2]
  000a73a2: blx 0x00071aac
  000a73a6: cmp r0,#0x0
  000a73a8: beq.w 0x000a7586
  000a73ac: movs r0,#0x0
  000a73ae: b 0x000a73f2
  000a73b0: mov r0,r9
  000a73b2: blx 0x00071ac4
  000a73b6: mov r1,r8
  000a73b8: mov r2,r0
  000a73ba: blx 0x00071c08
  000a73be: mov r4,r0
  000a73c0: movs r5,#0x0
  000a73c2: b 0x000a73c6
  000a73c4: adds r5,#0x1
  000a73c6: ldr.w r0,[r8,#0x0]
  000a73ca: cmp r5,r0
  000a73cc: bcs 0x000a73ac
  000a73ce: ldr.w r0,[r8,#0x4]
  000a73d2: mov r1,r4
  000a73d4: ldr.w r0,[r0,r5,lsl #0x2]
  000a73d8: blx 0x00071a94
  000a73dc: cmp r0,#0x0
  000a73de: beq 0x000a73c4
  000a73e0: ldr.w r0,[r8,#0x4]
  000a73e4: ldr.w r0,[r0,r5,lsl #0x2]
  000a73e8: blx 0x00071950
  000a73ec: cmp r0,r11
  000a73ee: bne 0x000a73c4
  000a73f0: movs r0,#0x1
  000a73f2: lsls r0,r0,#0x1f
  000a73f4: beq 0x000a73b0
  000a73f6: movs r0,#0xd
  000a73f8: str r0,[sp,#0x24]
  000a73fa: add r0,sp,#0x50
  000a73fc: mov r1,r9
  000a73fe: blx 0x00071c2c
  000a7402: ldr r0,[r6,#0x0]
  000a7404: blx 0x00071770
  000a7408: mov r1,r0
  000a740a: ldr r0,[sp,#0x28]
  000a740c: cmp r1,#0xf
  000a740e: ldr r0,[r0,#0x0]
  000a7410: bgt 0x000a741a
  000a7412: movs r1,#0x2
  000a7414: blx 0x00071848
  000a7418: b 0x000a7420
  000a741a: movs r1,#0x9
  000a741c: blx 0x00071848
  000a7420: ldr r1,[sp,#0x24]
  000a7422: cmp r1,#0x8
  000a7424: bne 0x000a74ae
  000a7426: ldr r0,[0x000a7514]
  000a7428: add r0,pc
  000a742a: ldr r0,[r0,#0x0]
  000a742c: ldr r5,[r0,#0x0]
  000a742e: ldr r0,[sp,#0x28]
  000a7430: ldr r1,[r5,#0x0]
  000a7432: ldr r0,[r0,#0x0]
  000a7434: subs r1,#0x61
  000a7436: blx 0x00071848
  000a743a: mov r6,r0
  000a743c: ldr r0,[r5,#0x4]
  000a743e: add.w r10,r6,#0x61
  000a7442: ldr.w r0,[r0,r10,lsl #0x2]
  000a7446: blx 0x000718f0
  000a744a: cmp r0,#0x0
  000a744c: beq 0x000a742e
  000a744e: ldr r0,[r5,#0x4]
  000a7450: ldr.w r0,[r0,r10,lsl #0x2]
  000a7454: blx 0x00071944
  000a7458: cmp r0,#0x0
  000a745a: beq 0x000a742e
  000a745c: ldr r0,[r5,#0x4]
  000a745e: ldr.w r0,[r0,r10,lsl #0x2]
  000a7462: blx 0x00071914
  000a7466: bic r1,r6,#0x1
  000a746a: cmp r1,#0x78
  000a746c: it ne
  000a746e: cmp.ne.w r10,#0x75
  000a7472: beq 0x000a742e
  000a7474: cmp r1,#0x12
  000a7476: it ne
  000a7478: cmp.ne.w r10,#0x83
  000a747c: beq 0x000a742e
  000a747e: cmp.w r10,#0xa4
  000a7482: it ne
  000a7484: cmp.ne.w r10,#0xaf
  000a7488: beq 0x000a742e
  000a748a: cmp r0,#0x0
  000a748c: bne 0x000a742e
  000a748e: ldr r0,[sp,#0x28]
  000a7490: ldr r0,[r0,#0x0]
  000a7492: movs r1,#0xf
  000a7494: blx 0x00071848
  000a7498: mov r6,r0
  000a749a: ldr r0,[r5,#0x4]
  000a749c: ldr.w r0,[r0,r10,lsl #0x2]
  000a74a0: blx 0x00071920
  000a74a4: mov r1,r0
  000a74a6: adds r0,r6,#0x5
  000a74a8: str r0,[sp,#0x1c]
  000a74aa: mov r6,r1
  000a74ac: b 0x000a760a
  000a74ae: adds r6,r0,#0x1
  000a74b0: movs r0,#0x0
  000a74b2: cmp r1,#0xb
  000a74b4: str r0,[sp,#0x1c]
  000a74b6: bhi 0x000a7518
  000a74b8: mov.w r10,#0x0
  000a74bc: tbb [pc,r1]
  000a74cc: vmov s2,r6
  000a74d0: mov.w r10,#0x75
  000a74d4: vmov.f32 s0,0x41200000
  000a74d8: vcvt.f32.s32 s2,s2
  000a74dc: vdiv.f32 s0,s2,s0
  000a74e0: vmov.f32 s2,0x41000000
  000a74e4: vmul.f32 s0,s0,s2
  000a74e8: vcvt.s32.f32 s0,s0
  000a74ec: ldr r0,[sp,#0x24]
  000a74ee: cmp r0,#0x3
  000a74f0: it eq
  000a74f2: mov.eq.w r10,#0x74
  000a74f6: vmov r0,s0
  000a74fa: adds r0,#0x2
  000a74fc: b 0x000a75ca
  000a7518: cmp r1,#0xf
  000a751a: bne 0x000a7606
  000a751c: ldr r0,[sp,#0x28]
  000a751e: ldr r0,[r0,#0x0]
  000a7520: movs r1,#0x5a
  000a7522: blx 0x00071848
  000a7526: vmov s0,r0
  000a752a: str r6,[sp,#0x18]
  000a752c: movs r5,#0x0
  000a752e: vcvt.f32.s32 s0,s0
  000a7532: vcvt.s32.f32 s0,s0
  000a7536: ldr r6,[sp,#0x20]
  000a7538: vmov r0,s0
  000a753c: adds r0,#0x1e
  000a753e: str r0,[sp,#0x1c]
  000a7540: ldr.w r0,[r8,#0x0]
  000a7544: cmp r5,r0
  000a7546: bcs.w 0x000a7904
  000a754a: ldr.w r0,[r8,#0x4]
  000a754e: ldr.w r0,[r0,r5,lsl #0x2]
  000a7552: mov r1,r4
  000a7554: blx 0x00071a94
  000a7558: adds r5,#0x1
  000a755a: cmp r0,#0x0
  000a755c: beq 0x000a7540
  000a755e: ldr r0,[0x000a7924]
  000a7560: add r0,pc
  000a7562: ldr r0,[r0,#0x0]
  000a7564: ldr r5,[r0,#0x0]
  000a7566: ldr r0,[r6,#0x0]
  000a7568: blx 0x00071c14
  000a756c: mov r1,r0
  000a756e: mov r0,r5
  000a7570: blx 0x00071c38
  000a7574: mov r6,r0
  000a7576: ldr r0,[sp,#0x28]
  000a7578: ldr r0,[r0,#0x0]
  000a757a: movs r1,#0x3
  000a757c: blx 0x00071848
  000a7580: ldr.w r10,[r6,r0,lsl #0x3]
  000a7584: b 0x000a7908
  000a7586: ldr r0,[sp,#0x28]
  000a7588: movs r1,#0x2
  000a758a: ldr r0,[r0,#0x0]
  000a758c: blx 0x00071848
  000a7590: movs r1,#0x4
  000a7592: cmp r0,#0x0
  000a7594: it eq
  000a7596: mov.eq r1,#0x1
  000a7598: str r1,[sp,#0x24]
  000a759a: b 0x000a73fa
  000a759c: ldr r0,[sp,#0x28]
  000a759e: ldr r0,[r0,#0x0]
  000a75a0: movs r1,#0x7
  000a75a2: blx 0x00071848
  000a75a6: vmov s2,r6
  000a75aa: mov r10,r0
  000a75ac: vmov.f32 s0,0x41200000
  000a75b0: vcvt.f32.s32 s2,s2
  000a75b4: vdiv.f32 s0,s2,s0
  000a75b8: vldr.32 s2,[pc,#0x3a0]
  000a75bc: vmul.f32 s0,s0,s2
  000a75c0: vcvt.s32.f32 s0,s0
  000a75c4: vmov r0,s0
  000a75c8: adds r0,#0x5
  000a75ca: str r0,[sp,#0x1c]
  000a75cc: b 0x000a760a
  000a75ce: ldr r0,[sp,#0x28]
  000a75d0: ldr r0,[r0,#0x0]
  000a75d2: movs r1,#0x4
  000a75d4: blx 0x00071848
  000a75d8: vmov s0,r0
  000a75dc: vcvt.f32.s32 s0,s0
  000a75e0: b 0x000a75fa
  000a75e2: vmov s2,r6
  000a75e6: vmov.f32 s0,0x41200000
  000a75ea: vcvt.f32.s32 s2,s2
  000a75ee: vdiv.f32 s0,s2,s0
  000a75f2: vmov.f32 s2,0x41900000
  000a75f6: vmul.f32 s0,s0,s2
  000a75fa: vcvt.s32.f32 s0,s0
  000a75fe: vmov r0,s0
  000a7602: adds r0,#0x2
  000a7604: str r0,[sp,#0x1c]
  000a7606: mov.w r10,#0x0
  000a760a: ldr r0,[0x000a7978]
  000a760c: cmp r6,#0xa
  000a760e: add r0,pc
  000a7610: ldr r5,[r0,#0x0]
  000a7612: ldr r0,[r5,#0x0]
  000a7614: str r0,[sp,#0x10]
  000a7616: it ge
  000a7618: mov.ge r6,#0xa
  000a761a: ldr r0,[sp,#0x20]
  000a761c: str r6,[sp,#0x18]
  000a761e: ldr r0,[r0,#0x0]
  000a7620: blx 0x00071c14
  000a7624: blx 0x0007189c
  000a7628: ldr.w r1,[r8,#0x4]
  000a762c: ldr.w r6,[r1,r0,lsl #0x2]
  000a7630: ldr r0,[r5,#0x0]
  000a7632: mov r1,r4
  000a7634: blx 0x00071c44
  000a7638: blx 0x0007189c
  000a763c: ldr.w r1,[r8,#0x4]
  000a7640: ldr.w r2,[r1,r0,lsl #0x2]
  000a7644: ldr r0,[sp,#0x10]
  000a7646: mov r1,r6
  000a7648: blx 0x00071c50
  000a764c: ldr r5,[sp,#0x18]
  000a764e: vmov.f32 s16,0x41200000
  000a7652: vmov s0,r0
  000a7656: vldr.32 s4,[pc,#0x308]
  000a765a: vmov s2,r5
  000a765e: vdiv.f32 s0,s0,s4
  000a7662: vcvt.f32.s32 s2,s2
  000a7666: vldr.32 s4,[pc,#0x2fc]
  000a766a: vdiv.f32 s2,s2,s16
  000a766e: vmul.f32 s2,s2,s4
  000a7672: vmov.f32 s4,0x3f800000
  000a7676: vcvt.s32.f32 s2,s2
  000a767a: vadd.f32 s0,s0,s4
  000a767e: vmov r0,s2
  000a7682: addw r0,r0,#0x5dc
  000a7686: vmov s2,r0
  000a768a: vcvt.f32.s32 s2,s2
  000a768e: ldr r0,[sp,#0x24]
  000a7690: cmp r0,#0x9
  000a7692: vmul.f32 s18,s0,s2
  000a7696: beq 0x000a76a4
  000a7698: cmp r0,#0x7
  000a769a: bne.w 0x000a7862
  000a769e: vldr.32 s0,[pc,#0x2cc]
  000a76a2: b 0x000a76a8
  000a76a4: vldr.32 s0,[pc,#0x2c0]
  000a76a8: vmul.f32 s18,s18,s0
  000a76ac: ldr r0,[sp,#0x20]
  000a76ae: ldr r0,[r0,#0x0]
  000a76b0: blx 0x00071c5c
  000a76b4: mul r1,r0,r0
  000a76b8: muls r0,r1
  000a76ba: add.w r0,r0,r0, lsl #0x2
  000a76be: lsls r0,r0,#0x1
  000a76c0: vmov s0,r0
  000a76c4: vcvt.f32.s32 s0,s0
  000a76c8: ldr r0,[sp,#0x24]
  000a76ca: orr r0,r0,#0x4
  000a76ce: cmp r0,#0xc
  000a76d0: vadd.f32 s18,s18,s0
  000a76d4: bne 0x000a76dc
  000a76d6: movs r0,#0x0
  000a76d8: str r0,[sp,#0x10]
  000a76da: b 0x000a7724
  000a76dc: ldr r0,[sp,#0x20]
  000a76de: ldr r0,[r0,#0x0]
  000a76e0: blx 0x00071b84
  000a76e4: mov r6,r0
  000a76e6: mov r0,r9
  000a76e8: blx 0x00071bd8
  000a76ec: mov r1,r0
  000a76ee: mov r0,r6
  000a76f0: blx 0x00071c68
  000a76f4: vmov s0,r0
  000a76f8: movs r1,#0x32
  000a76fa: vmul.f32 s0,s18,s0
  000a76fe: vcvt.s32.f32 s0,s0
  000a7702: vmov r6,s0
  000a7706: mov r0,r6
  000a7708: blx 0x0006f514
  000a770c: adds r5,r1,r6
  000a770e: str r1,[sp,#0x20]
  000a7710: movs r1,#0x32
  000a7712: mov r0,r5
  000a7714: blx 0x0006f514
  000a7718: ldr r0,[sp,#0x20]
  000a771a: cmp r1,#0x0
  000a771c: it ne
  000a771e: sub.ne r5,r6,r0
  000a7720: str r5,[sp,#0x10]
  000a7722: ldr r5,[sp,#0x18]
  000a7724: vcvt.s32.f32 s0,s18
  000a7728: movs r1,#0x32
  000a772a: vmov r0,s0
  000a772e: blx 0x0006f514
  000a7732: vmov s0,r1
  000a7736: movs r1,#0x32
  000a7738: vcvt.f32.s32 s20,s0
  000a773c: vadd.f32 s22,s18,s20
  000a7740: vcvt.s32.f32 s0,s22
  000a7744: vmov r0,s0
  000a7748: blx 0x0006f514
  000a774c: vsub.f32 s18,s18,s20
  000a7750: cmp r1,#0x0
  000a7752: it eq
  000a7754: vmov.eq.f32 s18,s22
  000a7758: movs r0,#0x78
  000a775a: blx 0x0006eb24
  000a775e: str r0,[sp,#0x20]
  000a7760: add r0,sp,#0x44
  000a7762: add r1,sp,#0x50
  000a7764: movs r2,#0x0
  000a7766: blx 0x0006f028
  000a776a: mov r0,r9
  000a776c: ldr.w r9,[sp,#0x20]
  000a7770: blx 0x00071c74
  000a7774: vcvt.s32.f32 s20,s18
  000a7778: mov r3,r0
  000a777a: vcvt.s32.f32 s0,s18
  000a777e: strd r4,r5,[sp,#0x8]
  000a7782: ldr r1,[sp,#0x24]
  000a7784: add r2,sp,#0x44
  000a7786: mov r0,r9
  000a7788: str.w r11,[sp,#0x0]
  000a778c: vstr.32 s0,[sp,#0x4]
  000a7790: blx 0x00071c80
  000a7794: add r0,sp,#0x44
  000a7796: vmov r6,s20
  000a779a: blx 0x0006ee30
  000a779e: mov r0,r6
  000a77a0: movs r1,#0xa
  000a77a2: blx 0x0007198c
  000a77a6: mov r1,r0
  000a77a8: ldr r0,[sp,#0x28]
  000a77aa: ldr r0,[r0,#0x0]
  000a77ac: blx 0x00071848
  000a77b0: vdiv.f32 s0,s18,s16
  000a77b4: vmov s2,r0
  000a77b8: vcvt.f32.s32 s2,s2
  000a77bc: vadd.f32 s0,s0,s2
  000a77c0: vcvt.s32.f32 s0,s0
  000a77c4: ldr r0,[sp,#0x24]
  000a77c6: cmp r0,#0x8
  000a77c8: vmov r5,s0
  000a77cc: bne 0x000a77e4
  000a77ce: vmov.f32 s2,0x3f000000
  000a77d2: vcvt.f32.s32 s0,s0
  000a77d6: vmul.f32 s0,s0,s2
  000a77da: vcvt.s32.f32 s0,s0
  000a77de: vmov r5,s0
  000a77e2: b 0x000a7808
  000a77e4: cmp r0,#0x6
  000a77e6: bne 0x000a7808
  000a77e8: ldr r0,[0x000a7980]
  000a77ea: add r0,pc
  000a77ec: ldr r0,[r0,#0x0]
  000a77ee: ldr r1,[r0,#0x0]
  000a77f0: add r0,sp,#0x38
  000a77f2: movs r2,#0x0
  000a77f4: movs r3,#0x1
  000a77f6: blx 0x00071b24
  000a77fa: add r1,sp,#0x38
  000a77fc: mov r0,r9
  000a77fe: blx 0x00071c8c
  000a7802: add r0,sp,#0x38
  000a7804: blx 0x0006ee30
  000a7808: mov r0,r5
  000a780a: movs r1,#0x32
  000a780c: blx 0x0006f514
  000a7810: adds r6,r1,r5
  000a7812: mov r9,r1
  000a7814: movs r1,#0x32
  000a7816: mov r0,r6
  000a7818: blx 0x0006f514
  000a781c: cmp r1,#0x0
  000a781e: it ne
  000a7820: sub.ne.w r6,r5,r9
  000a7824: ldr.w r9,[sp,#0x20]
  000a7828: mov r1,r6
  000a782a: mov r0,r9
  000a782c: blx 0x00071c98
  000a7830: ldr r2,[sp,#0x1c]
  000a7832: mov r0,r9
  000a7834: mov r1,r10
  000a7836: blx 0x00071ca4
  000a783a: ldr r1,[sp,#0x10]
  000a783c: mov r0,r9
  000a783e: blx 0x00071cb0
  000a7842: movs r5,#0x0
  000a7844: b 0x000a7858
  000a7846: ldr.w r0,[r8,#0x4]
  000a784a: ldr.w r0,[r0,r5,lsl #0x2]
  000a784e: mov r1,r4
  000a7850: blx 0x00071a94
  000a7854: cbnz r0,0x000a788e
  000a7856: adds r5,#0x1
  000a7858: ldr.w r0,[r8,#0x0]
  000a785c: cmp r5,r0
  000a785e: bcc 0x000a7846
  000a7860: b 0x000a78aa
  000a7862: cmp r0,#0x8
  000a7864: bne 0x000a78d0
  000a7866: ldr r0,[0x000a797c]
  000a7868: add r0,pc
  000a786a: ldr r0,[r0,#0x0]
  000a786c: ldr r0,[r0,#0x0]
  000a786e: ldr r0,[r0,#0x4]
  000a7870: ldr.w r0,[r0,r10,lsl #0x2]
  000a7874: blx 0x00071cbc
  000a7878: ldr r1,[sp,#0x1c]
  000a787a: vldr.32 s2,[pc,#0xf8]
  000a787e: muls r0,r1
  000a7880: vmov s0,r0
  000a7884: vcvt.f32.s32 s0,s0
  000a7888: vmul.f32 s18,s0,s2
  000a788c: b 0x000a76ac
  000a788e: ldr.w r0,[r8,#0x4]
  000a7892: ldr.w r1,[r0,r5,lsl #0x2]
  000a7896: add r0,sp,#0x2c
  000a7898: blx 0x00071cc8
  000a789c: add r1,sp,#0x2c
  000a789e: mov r0,r9
  000a78a0: blx 0x00071cd4
  000a78a4: add r0,sp,#0x2c
  000a78a6: blx 0x0006ee30
  000a78aa: add r0,sp,#0x50
  000a78ac: blx 0x0006ee30
  000a78b0: ldr r0,[sp,#0x5c]
  000a78b2: ldr r1,[sp,#0x14]
  000a78b4: ldr r1,[r1,#0x0]
  000a78b6: subs r0,r1,r0
  000a78b8: itttt eq
  000a78ba: mov.eq r0,r9
  000a78bc: add.eq sp,#0x60
  000a78be: vpop.eq {d8,d9,d10,d11}
  000a78c2: add.eq sp,#0x4
  000a78c4: itt eq
  000a78c6: pop.eq.w {r8,r9,r10,r11}
  000a78ca: pop.eq {r4,r5,r6,r7,pc}
  000a78cc: blx 0x0006e824
  000a78d0: cmp r0,#0x3
  000a78d2: it ne
  000a78d4: cmp.ne r0,#0x5
  000a78d6: bne 0x000a78de
  000a78d8: vadd.f32 s18,s18,s18
  000a78dc: b 0x000a76ac
  000a78de: cmp r0,#0xb
  000a78e0: bne.w 0x000a76ac
  000a78e4: vldr.32 s0,[pc,#0x88]
  000a78e8: vmov.f32 s2,0x40a00000
  000a78ec: ldr r0,[sp,#0x1c]
  000a78ee: vmul.f32 s18,s18,s0
  000a78f2: vdiv.f32 s0,s18,s2
  000a78f6: vmov s2,r0
  000a78fa: vcvt.f32.s32 s2,s2
  000a78fe: vmla.f32 s18,s0,s2
  000a7902: b 0x000a76ac
  000a7904: mov.w r10,#0x0
  000a7908: ldr r6,[sp,#0x18]
  000a790a: b 0x000a760a
```
