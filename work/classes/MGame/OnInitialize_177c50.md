# MGame::OnInitialize
elf 0x177c50 body 1978
Sig: undefined __thiscall OnInitialize(MGame * this)

## decompile
```c

/* MGame::OnInitialize() */

void __thiscall MGame::OnInitialize(MGame *this)

{
  ulonglong uVar1;
  int iVar2;
  int iVar3;
  Ship *pSVar4;
  undefined4 uVar5;
  Station *pSVar6;
  float fVar7;
  uint uVar8;
  int *piVar9;
  uint uVar10;
  Engine *this_00;
  DialogueWindow *this_01;
  void *pvVar11;
  undefined4 *puVar12;
  uint *puVar13;
  SingleLevel *pSVar14;
  undefined4 *puVar15;
  undefined1 uVar16;
  Level *this_02;
  int *piVar17;
  int iVar18;
  String *pSVar19;
  code *pcVar20;
  int iVar21;
  Item *this_03;
  PlayerEgo *pPVar22;
  int *piVar23;
  ulonglong in_d16;
  
  piVar23 = *(int **)(DAT_00187df0 + 0x187c60);
  iVar2 = *piVar23;
  *(undefined1 *)&this[2].field_4C = 0;
  this_02 = (Level *)this->field_78;
  this->field_0C = 100;
  if (this_02 == (Level *)0x0) {
    iVar3 = Status::inAlienOrbit();
    piVar17 = this->field_4;
    if (iVar3 == 0) {
      Status::getSystem();
      iVar3 = SolarSystem::getTextureIndex();
      uVar16 = SUB41(&stack0xffffffcc,0);
      puVar13 = (uint *)(iVar3 + 0x2efeU & 0xffff);
    }
    else {
      uVar16 = SUB41(&stack0xffffffcc,0);
      puVar13 = (uint *)0x2f08;
    }
    AbyssEngine::PaintCanvas::TextureCreate((ushort)piVar17,puVar13,(bool)uVar16);
    AbyssEngine::PaintCanvas::ChangeCubeTexture((uint)this->field_4);
    piVar17 = *(int **)(DAT_00187df8 + 0x187cc6);
    Globals::startNewSoundResourceList();
    pcVar20 = *(code **)(DAT_00187dfc + 0x187cd4);
    (*pcVar20)(*piVar17,0x66);
    (*pcVar20)(*piVar17,0x68);
    (*pcVar20)(*piVar17,0x69);
    (*pcVar20)(*piVar17,0x6a);
    (*pcVar20)(*piVar17,0x6b);
    (*pcVar20)(*piVar17,0x67);
    (*pcVar20)(*piVar17,0x7e);
    (*pcVar20)(*piVar17,5);
    (*pcVar20)(*piVar17,0x18);
    (*pcVar20)(*piVar17,0x15);
    (*pcVar20)(*piVar17,0x12);
    (*pcVar20)(*piVar17,0x13);
    (*pcVar20)(*piVar17,0x14);
    (*pcVar20)(*piVar17,0x1c);
    (*pcVar20)(*piVar17,0x1d);
    (*pcVar20)(*piVar17,0x1b);
    (*pcVar20)(*piVar17,0x25);
    (*pcVar20)(*piVar17,0x1a);
    (*pcVar20)(*piVar17,0x2e);
    (*pcVar20)(*piVar17,0x2f);
    iVar3 = Status::getWingmen();
    if (iVar3 != 0) {
      Globals::addSoundResourceToList(*piVar17);
    }
    (*pcVar20)(*piVar17,0x3e);
    (*pcVar20)(*piVar17,0x3d);
    (*pcVar20)(*piVar17,0x24);
    iVar3 = Status::getCurrentCampaignMission();
    if (iVar3 < 2) {
      Globals::addSoundResourceToList(*piVar17);
      Globals::addSoundResourceToList(*piVar17);
    }
    iVar3 = Status::inAlienOrbit();
    if (iVar3 == 0) {
      Status::getSystem();
      iVar3 = SolarSystem::currentOrbitHasWarpGate();
      if (iVar3 != 0) {
        Globals::addSoundResourceToList(*piVar17);
      }
    }
    iVar3 = Status::getCurrentCampaignMission();
    if (iVar3 == 0) {
      (*pcVar20)(*piVar17,0x8f);
      (*pcVar20)(*piVar17,0x9d);
      (*pcVar20)(*piVar17,0x9e);
      (*pcVar20)(*piVar17,0xa1);
      (*pcVar20)(*piVar17,0xa0);
      iVar3 = *piVar17;
      uVar5 = 0x9f;
LAB_00187ddc:
      (*pcVar20)(iVar3,uVar5);
    }
    else {
      iVar3 = Status::getCurrentCampaignMission();
      if (iVar3 == 0xe) {
        iVar3 = *piVar17;
      }
      else {
        iVar3 = Status::getCurrentCampaignMission();
        if (iVar3 == 0x18) {
          iVar3 = *piVar17;
        }
        else {
          iVar3 = Status::getCurrentCampaignMission();
          if (iVar3 != 0x1d) {
            iVar3 = Status::getCurrentCampaignMission();
            if (iVar3 == 0x29) {
              (*pcVar20)(*piVar17,0x9b);
              (*pcVar20)(*piVar17,0x99);
              iVar3 = *piVar17;
              uVar5 = 0x9a;
              goto LAB_00187ddc;
            }
            goto LAB_00187e14;
          }
          iVar3 = *piVar17;
        }
      }
      Globals::addSoundResourceToList(iVar3);
    }
LAB_00187e14:
    Status::checkForLevelUp();
    this_02 = operator_new(0x2a0);
    Level::Level(this_02,3);
    this->field_78 = (int *)this_02;
  }
  iVar3 = Level::init((EVP_PKEY_CTX *)this_02);
  if (iVar3 == 0) {
    uVar5 = 100;
    goto LAB_001883f4;
  }
  reset();
  piVar17 = *(int **)(DAT_001881a0 + 0x187e46);
  iVar3 = *piVar17;
  if (-1 < *(int *)(iVar3 + 100)) {
    Player::setHitpoints(this->m_pPlayerEgo->m_pPlayer);
    iVar3 = *piVar17;
  }
  if (-1 < *(int *)(iVar3 + 0x5c)) {
    Player::setShieldHP(this->m_pPlayerEgo->m_pPlayer);
    iVar3 = *piVar17;
  }
  if (-1 < *(int *)(iVar3 + 0x60)) {
    Player::setArmorHP(this->m_pPlayerEgo->m_pPlayer);
    iVar3 = *piVar17;
  }
  if (-1 < *(int *)(iVar3 + 0x68)) {
    Player::setGammaHP((Player *)this->m_pPlayerEgo->m_pPlayer,*(int *)(iVar3 + 0x68));
  }
  PlayerEgo::resetLastHP();
  iVar3 = Status::getCurrentCampaignMission();
  if (iVar3 != 0x5f) {
    pcVar20 = *(code **)(DAT_001881a4 + 0x187eb6);
    pSVar4 = (Ship *)(*pcVar20)(*piVar17);
    uVar5 = Ship::getMaxHP(pSVar4);
    iVar3 = *piVar17;
    *(undefined4 *)(iVar3 + 100) = uVar5;
    (*pcVar20)(iVar3);
    uVar5 = Ship::getMaxShieldHP();
    iVar3 = *piVar17;
    *(undefined4 *)(iVar3 + 0x5c) = uVar5;
    pSVar4 = (Ship *)(*pcVar20)(iVar3);
    uVar5 = Ship::getMaxArmorHP(pSVar4);
    iVar18 = *piVar17;
    *(undefined4 *)(iVar18 + 0x68) = 100;
    *(undefined4 *)(iVar18 + 0x60) = uVar5;
    pSVar6 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar6);
    Status::getCurrentCampaignMission();
    fVar7 = (float)Status::getGammaRayDamagePerSecond(iVar18,iVar3);
    if (fVar7 == 0.0) {
      Player::setGammaHP((Player *)this->m_pPlayerEgo->m_pPlayer,100);
    }
  }
  iVar3 = Status::inAlienOrbit();
  if (iVar3 == 0) {
    pSVar6 = (Station *)Status::getStation();
    uVar5 = Station::getIndex(pSVar6);
    *(undefined4 *)(*piVar17 + 0x84) = uVar5;
  }
  uVar8 = AbyssEngine::ApplicationManager::GetCurrentTimeMillis();
  this[1].field_A4 = 0;
  this[1].field_AC = 0;
  uVar1 = in_d16 & 0xffff0000ffff0000 | (ulonglong)uVar8 & 0xffff;
  this->field_20 = (int)uVar1;
  this->field_24 = (int)(uVar1 >> 0x20);
  this->field_28 = (int)uVar1;
  this->field_2C = (int)(uVar1 >> 0x20);
  iVar3 = Radar::hasScanner();
  iVar18 = *piVar17;
  if (iVar3 != 0) {
    *(undefined4 *)(iVar18 + 0x11c) = 0;
  }
  *(undefined4 *)(iVar18 + 300) = 0;
  *(undefined4 *)(iVar18 + 0x134) = 0;
  *(undefined4 *)(iVar18 + 0x13c) = 0;
  *(undefined4 *)(iVar18 + 0x144) = 0;
  iVar3 = Status::getShip();
  iVar3 = Ship::getFirstEquipmentOfSort(iVar3);
  if (iVar3 != 0) {
    iVar18 = Item::getAttribute(iVar3);
    this[1].field_AC = iVar18;
    iVar3 = Item::getAttribute(iVar3);
    pSVar14 = this->m_pLevel;
    this[1].field_A8 = iVar3;
    Hud::setTimeExtender(SUB41(pSVar14,0),true,false,true);
  }
  iVar3 = Status::dlc1Won();
  if (((iVar3 != 0) && (iVar3 = Status::inAlienOrbit(), iVar3 != 0)) &&
     (iVar3 = Status::getCurrentCampaignMission(), iVar3 < 0x93)) {
    if ((this->m_pPlayerEgo != (PlayerEgo *)0x0) && (this->field_84 != (int *)0x0)) {
      this->m_pPlayerEgo->field_18 = this->field_84;
    }
    Level::createRadioMessage((int)this->field_78,8);
  }
  iVar3 = Status::inBlackMarketSystem();
  if (iVar3 == 0) {
    *(undefined2 *)(*piVar17 + 0x110) = 0;
  }
  else {
    if ((this->m_pPlayerEgo != (PlayerEgo *)0x0) && (this->field_84 != (int *)0x0)) {
      this->m_pPlayerEgo->field_18 = this->field_84;
    }
    if (*(char *)(*piVar17 + 0x110) == '\0') {
      piVar9 = this->field_78;
      if (*(char *)(*piVar17 + 0x111) == '\0') {
        piVar9 = (int *)Level::getEnemies();
        if (piVar9 != (int *)0x0) {
          iVar3 = *piVar9;
          iVar18 = 0;
          while (iVar3 != iVar18) {
            iVar21 = *(int *)(piVar9[1] + iVar18 * 4);
            iVar18 = iVar18 + 1;
            if (*(int *)(iVar21 + 0x28) == 8) {
              *(undefined1 *)(iVar21 + 0x25) = 0;
            }
          }
        }
        piVar9 = this->field_78;
        iVar3 = 9;
      }
      else {
        iVar3 = 0xd;
      }
      Level::createRadioMessage((int)piVar9,iVar3);
    }
    else {
      piVar9 = (int *)Level::getEnemies();
      if (piVar9 != (int *)0x0) {
        iVar3 = *piVar9;
        iVar18 = 0;
        while (iVar3 != iVar18) {
          iVar21 = *(int *)(piVar9[1] + iVar18 * 4);
          iVar18 = iVar18 + 1;
          if (*(int *)(iVar21 + 0x28) == 8) {
            *(undefined1 *)(iVar21 + 0x25) = 0;
          }
        }
      }
    }
  }
  iVar3 = Status::inAlienOrbit();
  if ((iVar3 == 0) && (iVar3 = Status::getCurrentCampaignMission(), iVar3 == 0x7d)) {
    iVar3 = *piVar17;
    pSVar6 = (Station *)Status::getStation();
    Station::getIndex(pSVar6);
    iVar3 = Status::isFreighterMissionStation(iVar3);
    if (iVar3 != 0) {
      Status::getCampaignMission();
      uVar8 = Mission::getStatusValue();
      iVar3 = *piVar17;
      pSVar6 = (Station *)Status::getStation();
      Station::getIndex(pSVar6);
      uVar10 = Status::getFreighterMissionStationBit(iVar3);
      if ((1 << (uVar10 & 0xff) & uVar8) == 0) {
        iVar3 = Status::getCampaignMission();
        Status::getCampaignMission();
        Mission::getStatusValue();
        iVar18 = *piVar17;
        pSVar6 = (Station *)Status::getStation();
        Station::getIndex(pSVar6);
        Status::getFreighterMissionStationBit(iVar18);
        Mission::setStatusValue(iVar3);
        if ((this->m_pPlayerEgo != (PlayerEgo *)0x0) && (this->field_84 != (int *)0x0)) {
          this->m_pPlayerEgo->field_18 = this->field_84;
        }
        Level::createRadioMessage((int)this->field_78,0x13);
      }
    }
  }
  pSVar4 = (Ship *)Status::getShip();
  puVar13 = (uint *)Ship::getEquipment(pSVar4,1);
  if (puVar13 != (uint *)0x0) {
    iVar3 = Status::getShip();
    iVar3 = Ship::hasEquipment(iVar3,*(int *)(*piVar17 + 0xf4));
    if (iVar3 == 0) {
      if (*(Item **)puVar13[1] != (Item *)0x0) {
        pPVar22 = this->m_pPlayerEgo;
        Item::getIndex(*(Item **)puVar13[1]);
        PlayerEgo::setCurrentSecondaryWeaponIndex((int)pPVar22);
        pSVar14 = this->m_pLevel;
LAB_001881ea:
        Hud::setCurrentSecondaryWeapon((Item *)pSVar14);
      }
    }
    else {
      for (uVar8 = 0; uVar8 < *puVar13; uVar8 = uVar8 + 1) {
        this_03 = *(Item **)(puVar13[1] + uVar8 * 4);
        if ((this_03 != (Item *)0x0) &&
           (iVar3 = Item::getIndex(this_03), iVar3 == *(int *)(*piVar17 + 0xf4))) {
          iVar3 = Level::getPlayer();
          Item::getIndex(this_03);
          PlayerEgo::setCurrentSecondaryWeaponIndex(iVar3);
          pSVar14 = this->m_pLevel;
          goto LAB_001881ea;
        }
      }
    }
  }
  iVar3 = *(int *)(DAT_00188448 + 0x1881f4);
  *(undefined2 *)&this[2].field_14 = 0;
  if (*(char *)(iVar3 + 0xf) == '\0') {
    iVar18 = Status::getCurrentCampaignMission();
    if (1 < iVar18) {
      fVar7 = (float)PlayerEgo::getPosition();
      FModSound::play(**(int **)(DAT_0018844c + 0x188250),(Vector *)this->m_pPlayerEgo->field_1C,
                      (Vector *)&stack0xffffffcc,fVar7);
    }
  }
  else {
    PlayerEgo::PlayEngineSound();
    puVar13 = (uint *)Level::getEnemies();
    if (puVar13 != (uint *)0x0) {
      for (uVar8 = 0; uVar8 < *puVar13; uVar8 = uVar8 + 1) {
        KIPlayer::PlayEngineSound(*(KIPlayer **)(puVar13[1] + uVar8 * 4));
      }
    }
  }
  this->field_50 = 0;
  Level::getStarSystem();
  StarSystem::initLight();
  Level::enableParticleEffects(SUB41(this->field_78,0),0.25 <= *(float *)(iVar3 + 0x28));
  Status::getShip();
  fVar7 = (float)Ship::getFireRateFactor();
  if (0.0 <= 1.0 - fVar7) {
    PlayerEgo::pitchAllPrimaryGuns(1.0 - fVar7);
  }
  iVar3 = Status::inAlienOrbit();
  if (iVar3 == 0) {
    pSVar6 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar6);
    uVar8 = iVar3 - 0x6d;
    if (uVar8 < 0x1a) {
      if ((1 << (uVar8 & 0xff) & DAT_00188440) == 0) {
        if (uVar8 != 2) goto LAB_00188306;
        iVar3 = Status::getCurrentCampaignMission();
        if (iVar3 < 0x5e) goto LAB_00188318;
      }
    }
    else {
LAB_00188306:
      if (2 < iVar3 - 0x66U) goto LAB_00188318;
    }
    Status::getStation();
    Station::visit();
  }
LAB_00188318:
  piVar17 = *(int **)(DAT_00188450 + 0x18831e);
  if (*piVar17 != -1) {
    Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_00188454 + 0x18832a));
  }
  iVar3 = DAT_00188458;
  *piVar17 = -1;
  FModSound::setDownPitch(SUB41(**(undefined4 **)(iVar3 + 0x18833e),0));
  this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::Engine::SetPostEffect(this_00,DAT_00188444,false);
  iVar3 = Status::getCurrentCampaignMission();
  if ((iVar3 == 0x9e) && (iVar3 = Level::getEnemies(), iVar3 != 0)) {
    iVar3 = Level::getEnemies();
    PlayerFighter::cloak(**(int **)(iVar3 + 4),true);
  }
  if (this->m_pDialogueWindow == (DialogueWindow *)0x0) {
    this_01 = operator_new(0x240);
    MenuTouchWindow::MenuTouchWindow((MenuTouchWindow *)this_01,1);
    this->m_pDialogueWindow = this_01;
  }
  pvVar11 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  iVar3 = DAT_00188464;
  puVar12 = (undefined4 *)(DAT_0018845c + 0x1883ac);
  puVar15 = (undefined4 *)(DAT_00188460 + 0x1883b0);
  this[2].field_5C = (int)pvVar11;
  pSVar19 = *(String **)*puVar12;
  uVar8 = **(uint **)(iVar3 + 0x1883b8);
  iVar3 = GameText::getText(*(int *)*puVar15);
  Globals::getLineArray
            (uVar8,pSVar19,iVar3,
             (Array *)(**(int **)(DAT_0018846c + 0x1883d6) +
                      *(int *)(**(int **)(DAT_00188468 + 0x1883d0) + 0x28) * -2));
  *(undefined1 *)&this->field_54 = 1;
  uVar5 = 0;
LAB_001883f4:
  if (*piVar23 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

```

## target disasm
```
  00187c50: push {r4,r5,r6,r7,lr}
  00187c52: add r7,sp,#0xc
  00187c54: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00187c58: mov r10,r0
  00187c5a: ldr r0,[0x00187df0]
  00187c5c: add r0,pc
  00187c5e: ldr.w r9,[r0,#0x0]
  00187c62: ldr.w r0,[r9,#0x0]
  00187c66: str r0,[sp,#0x10]
  00187c68: movs r0,#0x0
  00187c6a: strb.w r0,[r10,#0x1dc]
  00187c6e: movs r0,#0x64
  00187c70: ldr.w r5,[r10,#0x78]
  00187c74: str.w r0,[r10,#0xc]
  00187c78: cmp r5,#0x0
  00187c7a: bne.w 0x00187e2e
  00187c7e: ldr r0,[0x00187df4]
  00187c80: add r0,pc
  00187c82: ldr r4,[r0,#0x0]
  00187c84: ldr r0,[r4,#0x0]
  00187c86: blx 0x000723d0
  00187c8a: ldr.w r5,[r10,#0x4]
  00187c8e: cbz r0,0x00187c9a
  00187c90: add r2,sp,#0x4
  00187c92: mov r0,r5
  00187c94: movw r1,#0x2f08
  00187c98: b 0x00187cb0
  00187c9a: ldr r0,[r4,#0x0]
  00187c9c: blx 0x00071a10
  00187ca0: blx 0x0007390c
  00187ca4: movw r1,#0x2efe
  00187ca8: add r0,r1
  00187caa: add r2,sp,#0x4
  00187cac: uxth r1,r0
  00187cae: mov r0,r5
  00187cb0: movs r3,#0x0
  00187cb2: blx 0x0006fbb0
  00187cb6: ldr r1,[sp,#0x4]
  00187cb8: ldr.w r0,[r10,#0x4]
  00187cbc: blx 0x00077ed8
  00187cc0: ldr r0,[0x00187df8]
  00187cc2: add r0,pc
  00187cc4: ldr r5,[r0,#0x0]
  00187cc6: ldr r0,[r5,#0x0]
  00187cc8: blx 0x00074f08
  00187ccc: ldr r0,[0x00187dfc]
  00187cce: movs r1,#0x66
  00187cd0: add r0,pc
  00187cd2: ldr r6,[r0,#0x0]
  00187cd4: ldr r0,[r5,#0x0]
  00187cd6: blx r6
  00187cd8: ldr r0,[r5,#0x0]
  00187cda: movs r1,#0x68
  00187cdc: blx r6
  00187cde: ldr r0,[r5,#0x0]
  00187ce0: movs r1,#0x69
  00187ce2: blx r6
  00187ce4: ldr r0,[r5,#0x0]
  00187ce6: movs r1,#0x6a
  00187ce8: blx r6
  00187cea: ldr r0,[r5,#0x0]
  00187cec: movs r1,#0x6b
  00187cee: blx r6
  00187cf0: ldr r0,[r5,#0x0]
  00187cf2: movs r1,#0x67
  00187cf4: blx r6
  00187cf6: ldr r0,[r5,#0x0]
  00187cf8: movs r1,#0x7e
  00187cfa: blx r6
  00187cfc: ldr r0,[r5,#0x0]
  00187cfe: movs r1,#0x5
  00187d00: blx r6
  00187d02: ldr r0,[r5,#0x0]
  00187d04: movs r1,#0x18
  00187d06: blx r6
  00187d08: ldr r0,[r5,#0x0]
  00187d0a: movs r1,#0x15
  00187d0c: blx r6
  00187d0e: ldr r0,[r5,#0x0]
  00187d10: movs r1,#0x12
  00187d12: blx r6
  00187d14: ldr r0,[r5,#0x0]
  00187d16: movs r1,#0x13
  00187d18: blx r6
  00187d1a: ldr r0,[r5,#0x0]
  00187d1c: movs r1,#0x14
  00187d1e: blx r6
  00187d20: ldr r0,[r5,#0x0]
  00187d22: movs r1,#0x1c
  00187d24: blx r6
  00187d26: ldr r0,[r5,#0x0]
  00187d28: movs r1,#0x1d
  00187d2a: blx r6
  00187d2c: ldr r0,[r5,#0x0]
  00187d2e: movs r1,#0x1b
  00187d30: blx r6
  00187d32: ldr r0,[r5,#0x0]
  00187d34: movs r1,#0x25
  00187d36: blx r6
  00187d38: ldr r0,[r5,#0x0]
  00187d3a: movs r1,#0x1a
  00187d3c: blx r6
  00187d3e: ldr r0,[r5,#0x0]
  00187d40: movs r1,#0x2e
  00187d42: blx r6
  00187d44: ldr r0,[r5,#0x0]
  00187d46: movs r1,#0x2f
  00187d48: blx r6
  00187d4a: ldr r0,[r4,#0x0]
  00187d4c: blx 0x000741dc
  00187d50: cbz r0,0x00187d5a
  00187d52: ldr r0,[r5,#0x0]
  00187d54: movs r1,#0x30
  00187d56: blx 0x00072298
  00187d5a: ldr r0,[r5,#0x0]
  00187d5c: movs r1,#0x3e
  00187d5e: blx r6
  00187d60: ldr r0,[r5,#0x0]
  00187d62: movs r1,#0x3d
  00187d64: blx r6
  00187d66: ldr r0,[r5,#0x0]
  00187d68: movs r1,#0x24
  00187d6a: blx r6
  00187d6c: ldr r0,[r4,#0x0]
  00187d6e: blx 0x00071770
  00187d72: cmp r0,#0x1
  00187d74: bgt 0x00187d86
  00187d76: ldr r0,[r5,#0x0]
  00187d78: movs r1,#0x9c
  00187d7a: blx 0x00072298
  00187d7e: ldr r0,[r5,#0x0]
  00187d80: movs r1,#0x9d
  00187d82: blx 0x00072298
  00187d86: ldr r0,[r4,#0x0]
  00187d88: blx 0x000723d0
  00187d8c: cbnz r0,0x00187da2
  00187d8e: ldr r0,[r4,#0x0]
  00187d90: blx 0x00071a10
  00187d94: blx 0x00073c18
  00187d98: cbz r0,0x00187da2
  00187d9a: ldr r0,[r5,#0x0]
  00187d9c: movs r1,#0x1f
  00187d9e: blx 0x00072298
  00187da2: ldr r0,[r4,#0x0]
  00187da4: blx 0x00071770
  00187da8: cbz r0,0x00187dba
  00187daa: ldr r0,[r4,#0x0]
  00187dac: blx 0x00071770
  00187db0: cmp r0,#0xe
  00187db2: bne 0x00187de0
  00187db4: ldr r0,[r5,#0x0]
  00187db6: movs r1,#0xf
  00187db8: b 0x00187e10
  00187dba: ldr r0,[r5,#0x0]
  00187dbc: movs r1,#0x8f
  00187dbe: blx r6
  00187dc0: ldr r0,[r5,#0x0]
  00187dc2: movs r1,#0x9d
  00187dc4: blx r6
  00187dc6: ldr r0,[r5,#0x0]
  00187dc8: movs r1,#0x9e
  00187dca: blx r6
  00187dcc: ldr r0,[r5,#0x0]
  00187dce: movs r1,#0xa1
  00187dd0: blx r6
  00187dd2: ldr r0,[r5,#0x0]
  00187dd4: movs r1,#0xa0
  00187dd6: blx r6
  00187dd8: ldr r0,[r5,#0x0]
  00187dda: movs r1,#0x9f
  00187ddc: blx r6
  00187dde: b 0x00187e14
  00187de0: ldr r0,[r4,#0x0]
  00187de2: blx 0x00071770
  00187de6: cmp r0,#0x18
  00187de8: bne 0x00187e00
  00187dea: ldr r0,[r5,#0x0]
  00187dec: movs r1,#0x22
  00187dee: b 0x00187e10
  00187e00: ldr r0,[r4,#0x0]
  00187e02: blx 0x00071770
  00187e06: cmp r0,#0x1d
  00187e08: bne.w 0x0018840a
  00187e0c: ldr r0,[r5,#0x0]
  00187e0e: movs r1,#0xe
  00187e10: blx 0x00072298
  00187e14: ldr r0,[r4,#0x0]
  00187e16: blx 0x00075274
  00187e1a: mov.w r0,#0x2a0
  00187e1e: blx 0x0006eb24
  00187e22: mov r5,r0
  00187e24: movs r1,#0x3
  00187e26: blx 0x0007201c
  00187e2a: str.w r5,[r10,#0x78]
  00187e2e: mov r0,r5
  00187e30: blx 0x00072028
  00187e34: cmp r0,#0x0
  00187e36: beq.w 0x00188046
  00187e3a: mov r0,r10
  00187e3c: blx 0x000780b8
  00187e40: ldr r0,[0x001881a0]
  00187e42: add r0,pc
  00187e44: ldr.w r8,[r0,#0x0]
  00187e48: ldr.w r0,[r8,#0x0]
  00187e4c: ldr r1,[r0,#0x64]
  00187e4e: cmp r1,#0x0
  00187e50: blt 0x00187e60
  00187e52: ldr.w r0,[r10,#0x58]
  00187e56: ldr r0,[r0,#0x0]
  00187e58: blx 0x000727e4
  00187e5c: ldr.w r0,[r8,#0x0]
  00187e60: ldr r1,[r0,#0x5c]
  00187e62: cmp r1,#0x0
  00187e64: blt 0x00187e74
  00187e66: ldr.w r0,[r10,#0x58]
  00187e6a: ldr r0,[r0,#0x0]
  00187e6c: blx 0x000726dc
  00187e70: ldr.w r0,[r8,#0x0]
  00187e74: ldr r1,[r0,#0x60]
  00187e76: cmp r1,#0x0
  00187e78: blt 0x00187e88
  00187e7a: ldr.w r0,[r10,#0x58]
  00187e7e: ldr r0,[r0,#0x0]
  00187e80: blx 0x00072c7c
  00187e84: ldr.w r0,[r8,#0x0]
  00187e88: ldr r1,[r0,#0x68]
  00187e8a: cmp r1,#0x0
  00187e8c: blt 0x00187e98
  00187e8e: ldr.w r0,[r10,#0x58]
  00187e92: ldr r0,[r0,#0x0]
  00187e94: blx 0x00076d44
  00187e98: ldr.w r0,[r10,#0x58]
  00187e9c: blx 0x000780c4
  00187ea0: ldr.w r0,[r8,#0x0]
  00187ea4: blx 0x00071770
  00187ea8: cmp r0,#0x5f
  00187eaa: beq 0x00187f1a
  00187eac: ldr r1,[0x001881a4]
  00187eae: ldr.w r0,[r8,#0x0]
  00187eb2: add r1,pc
  00187eb4: ldr r4,[r1,#0x0]
  00187eb6: blx r4
  00187eb8: blx 0x00073b04
  00187ebc: ldr.w r1,[r8,#0x0]
  00187ec0: str r0,[r1,#0x64]
  00187ec2: mov r0,r1
  00187ec4: blx r4
  00187ec6: blx 0x000722bc
  00187eca: ldr.w r1,[r8,#0x0]
  00187ece: str r0,[r1,#0x5c]
  00187ed0: mov r0,r1
  00187ed2: blx r4
  00187ed4: blx 0x00073b10
  00187ed8: ldr.w r5,[r8,#0x0]
  00187edc: movs r1,#0x64
  00187ede: str r1,[r5,#0x68]
  00187ee0: str r0,[r5,#0x60]
  00187ee2: mov r0,r5
  00187ee4: blx 0x00071c14
  00187ee8: blx 0x00071824
  00187eec: mov r6,r0
  00187eee: ldr.w r0,[r8,#0x0]
  00187ef2: blx 0x00071770
  00187ef6: mov r2,r0
  00187ef8: mov r0,r5
  00187efa: mov r1,r6
  00187efc: blx 0x000723dc
  00187f00: vmov s0,r0
  00187f04: vcmpe.f32 s0,#0
  00187f08: vmrs apsr,fpscr
  00187f0c: bne 0x00187f1a
  00187f0e: ldr.w r0,[r10,#0x58]
  00187f12: movs r1,#0x64
  00187f14: ldr r0,[r0,#0x0]
  00187f16: blx 0x00076d44
  00187f1a: ldr.w r0,[r8,#0x0]
  00187f1e: blx 0x000723d0
  00187f22: cbnz r0,0x00187f38
  00187f24: ldr.w r0,[r8,#0x0]
  00187f28: blx 0x00071c14
  00187f2c: blx 0x00071824
  00187f30: ldr.w r1,[r8,#0x0]
  00187f34: str.w r0,[r1,#0x84]
  00187f38: ldr r0,[0x001881a8]
  00187f3a: add r0,pc
  00187f3c: ldr r0,[r0,#0x0]
  00187f3e: ldr r0,[r0,#0x0]
  00187f40: blx 0x000713d4
  00187f44: vmov.32 d16[0x0],r0
  00187f48: movs r4,#0x0
  00187f4a: str.w r4,[r10,#0x16c]
  00187f4e: str.w r4,[r10,#0x174]
  00187f52: vmov.32 d16[0x1],r1
  00187f56: ldr.w r0,[r10,#0x80]
  00187f5a: add.w r1,r10,#0x20
  00187f5e: vmov d17,d16
  00187f62: vst1.64 {d16,d17},[r1]
  00187f66: blx 0x000743d4
  00187f6a: mov r1,r0
  00187f6c: ldr.w r0,[r8,#0x0]
  00187f70: cbz r1,0x00187f76
  00187f72: str.w r4,[r0,#0x11c]
  00187f76: str.w r4,[r0,#0x12c]
  00187f7a: str.w r4,[r0,#0x134]
  00187f7e: str.w r4,[r0,#0x13c]
  00187f82: str.w r4,[r0,#0x144]
  00187f86: blx 0x00071a58
  00187f8a: movs r1,#0x1a
  00187f8c: blx 0x0007228c
  00187f90: mov r5,r0
  00187f92: cbz r0,0x00187fc0
  00187f94: mov r0,r5
  00187f96: movs r1,#0x2a
  00187f98: blx 0x000718e4
  00187f9c: str.w r0,[r10,#0x174]
  00187fa0: mov r0,r5
  00187fa2: movs r1,#0x2b
  00187fa4: blx 0x000718e4
  00187fa8: ldr.w r1,[r10,#0x74]
  00187fac: movs r2,#0x0
  00187fae: str.w r0,[r10,#0x170]
  00187fb2: movs r0,#0x0
  00187fb4: str r0,[sp,#0x0]
  00187fb6: movs r3,#0x1
  00187fb8: mov r0,r1
  00187fba: movs r1,#0x1
  00187fbc: blx 0x000780d0
  00187fc0: ldr.w r0,[r8,#0x0]
  00187fc4: blx 0x000719a4
  00187fc8: cbz r0,0x00187ffa
  00187fca: ldr.w r0,[r8,#0x0]
  00187fce: blx 0x000723d0
  00187fd2: cbz r0,0x00187ffa
  00187fd4: ldr.w r0,[r8,#0x0]
  00187fd8: blx 0x00071770
  00187fdc: cmp r0,#0x92
  00187fde: bgt 0x00187ffa
  00187fe0: ldr.w r0,[r10,#0x58]
  00187fe4: cbz r0,0x00187fee
  00187fe6: ldr.w r1,[r10,#0x84]
  00187fea: cbz r1,0x00187fee
  00187fec: str r1,[r0,#0x18]
  00187fee: ldr.w r0,[r10,#0x78]
  00187ff2: movs r1,#0x8
  00187ff4: movs r2,#0x0
  00187ff6: blx 0x00072754
  00187ffa: ldr.w r0,[r8,#0x0]
  00187ffe: blx 0x00071998
  00188002: cbz r0,0x0018804a
  00188004: ldr.w r0,[r10,#0x58]
  00188008: cbz r0,0x00188012
  0018800a: ldr.w r1,[r10,#0x84]
  0018800e: cbz r1,0x00188012
  00188010: str r1,[r0,#0x18]
  00188012: ldr.w r1,[r8,#0x0]
  00188016: ldrb.w r0,[r1,#0x110]
  0018801a: cbz r0,0x00188056
  0018801c: ldr.w r0,[r10,#0x78]
  00188020: blx 0x00071eb4
  00188024: cbz r0,0x00188094
  00188026: ldr r1,[r0,#0x0]
  00188028: movs r2,#0x0
  0018802a: movs r3,#0x0
  0018802c: b 0x00188040
  0018802e: ldr r6,[r0,#0x4]
  00188030: ldr.w r6,[r6,r3,lsl #0x2]
  00188034: adds r3,#0x1
  00188036: ldr r5,[r6,#0x28]
  00188038: cmp r5,#0x8
  0018803a: it eq
  0018803c: strb.eq.w r2,[r6,#0x25]
  00188040: cmp r1,r3
  00188042: bne 0x0018802e
  00188044: b 0x00188094
  00188046: movs r0,#0x64
  00188048: b 0x001883f4
  0018804a: ldr.w r0,[r8,#0x0]
  0018804e: movs r1,#0x0
  00188050: strh.w r1,[r0,#0x110]
  00188054: b 0x00188094
  00188056: ldrb.w r1,[r1,#0x111]
  0018805a: ldr.w r0,[r10,#0x78]
  0018805e: cbz r1,0x00188064
  00188060: movs r1,#0xd
  00188062: b 0x0018808e
  00188064: blx 0x00071eb4
  00188068: cbz r0,0x00188088
  0018806a: ldr r1,[r0,#0x0]
  0018806c: movs r2,#0x0
  0018806e: movs r3,#0x0
  00188070: b 0x00188084
  00188072: ldr r6,[r0,#0x4]
  00188074: ldr.w r6,[r6,r3,lsl #0x2]
  00188078: adds r3,#0x1
  0018807a: ldr r5,[r6,#0x28]
  0018807c: cmp r5,#0x8
  0018807e: it eq
  00188080: strb.eq.w r2,[r6,#0x25]
  00188084: cmp r1,r3
  00188086: bne 0x00188072
  00188088: ldr.w r0,[r10,#0x78]
  0018808c: movs r1,#0x9
  0018808e: movs r2,#0x8
  00188090: blx 0x00072754
  00188094: ldr.w r0,[r8,#0x0]
  00188098: blx 0x000723d0
  0018809c: cmp r0,#0x0
  0018809e: bne 0x0018814a
  001880a0: ldr.w r0,[r8,#0x0]
  001880a4: blx 0x00071770
  001880a8: cmp r0,#0x7d
  001880aa: bne 0x0018814a
  001880ac: ldr.w r5,[r8,#0x0]
  001880b0: mov r0,r5
  001880b2: blx 0x00071c14
  001880b6: blx 0x00071824
  001880ba: mov r1,r0
  001880bc: mov r0,r5
  001880be: blx 0x00074650
  001880c2: cmp r0,#0x0
  001880c4: beq 0x0018814a
  001880c6: ldr.w r0,[r8,#0x0]
  001880ca: blx 0x0007285c
  001880ce: blx 0x0007288c
  001880d2: ldr.w r6,[r8,#0x0]
  001880d6: mov r5,r0
  001880d8: mov r0,r6
  001880da: blx 0x00071c14
  001880de: blx 0x00071824
  001880e2: mov r1,r0
  001880e4: mov r0,r6
  001880e6: blx 0x0007465c
  001880ea: movs r6,#0x1
  001880ec: lsl.w r0,r6,r0
  001880f0: tst r0,r5
  001880f2: bne 0x0018814a
  001880f4: ldr.w r0,[r8,#0x0]
  001880f8: blx 0x0007285c
  001880fc: mov r4,r0
  001880fe: ldr.w r0,[r8,#0x0]
  00188102: blx 0x0007285c
  00188106: blx 0x0007288c
  0018810a: ldr.w r5,[r8,#0x0]
  0018810e: mov r11,r0
  00188110: mov r0,r5
  00188112: blx 0x00071c14
  00188116: blx 0x00071824
  0018811a: mov r1,r0
  0018811c: mov r0,r5
  0018811e: blx 0x0007465c
  00188122: lsl.w r0,r6,r0
  00188126: orr.w r1,r0,r11
  0018812a: mov r0,r4
  0018812c: blx 0x00072898
  00188130: ldr.w r0,[r10,#0x58]
  00188134: cbz r0,0x0018813e
  00188136: ldr.w r1,[r10,#0x84]
  0018813a: cbz r1,0x0018813e
  0018813c: str r1,[r0,#0x18]
  0018813e: ldr.w r0,[r10,#0x78]
  00188142: movs r1,#0x13
  00188144: movs r2,#0x0
  00188146: blx 0x00072754
  0018814a: ldr.w r0,[r8,#0x0]
  0018814e: blx 0x00071a58
  00188152: movs r1,#0x1
  00188154: blx 0x00072118
  00188158: mov r5,r0
  0018815a: cmp r0,#0x0
  0018815c: beq 0x001881ee
  0018815e: ldr.w r0,[r8,#0x0]
  00188162: blx 0x00071a58
  00188166: ldr.w r1,[r8,#0x0]
  0018816a: movs r2,#0x1
  0018816c: ldr.w r1,[r1,#0xf4]
  00188170: blx 0x00073978
  00188174: cbz r0,0x001881ac
  00188176: movs r4,#0x0
  00188178: b 0x00188196
  0018817a: ldr r0,[r5,#0x4]
  0018817c: ldr.w r6,[r0,r4,lsl #0x2]
  00188180: cbz r6,0x00188194
  00188182: mov r0,r6
  00188184: blx 0x000718d8
  00188188: ldr.w r1,[r8,#0x0]
  0018818c: ldr.w r1,[r1,#0xf4]
  00188190: cmp r0,r1
  00188192: beq 0x001881cc
  00188194: adds r4,#0x1
  00188196: ldr r0,[r5,#0x0]
  00188198: cmp r4,r0
  0018819a: bcc 0x0018817a
  0018819c: b 0x001881ee
  001881ac: ldr r0,[r5,#0x4]
  001881ae: ldr r0,[r0,#0x0]
  001881b0: cbz r0,0x001881ee
  001881b2: ldr.w r6,[r10,#0x58]
  001881b6: blx 0x000718d8
  001881ba: mov r1,r0
  001881bc: mov r0,r6
  001881be: blx 0x000780dc
  001881c2: ldr r1,[r5,#0x4]
  001881c4: ldr.w r0,[r10,#0x74]
  001881c8: ldr r1,[r1,#0x0]
  001881ca: b 0x001881ea
  001881cc: ldr.w r0,[r10,#0x78]
  001881d0: blx 0x00072034
  001881d4: mov r5,r0
  001881d6: mov r0,r6
  001881d8: blx 0x000718d8
  001881dc: mov r1,r0
  001881de: mov r0,r5
  001881e0: blx 0x000780dc
  001881e4: ldr.w r0,[r10,#0x74]
  001881e8: mov r1,r6
  001881ea: blx 0x000780e8
  001881ee: ldr r0,[0x00188448]
  001881f0: add r0,pc
  001881f2: ldr r6,[r0,#0x0]
  001881f4: movs r0,#0x0
  001881f6: strh.w r0,[r10,#0x1a4]
  001881fa: ldrb r0,[r6,#0xf]
  001881fc: cbz r0,0x0018822a
  001881fe: ldr.w r0,[r10,#0x58]
  00188202: blx 0x00072b68
  00188206: ldr.w r0,[r10,#0x78]
  0018820a: blx 0x00071eb4
  0018820e: mov r5,r0
  00188210: cbz r0,0x0018825c
  00188212: movs r4,#0x0
  00188214: b 0x00188222
  00188216: ldr r0,[r5,#0x4]
  00188218: ldr.w r0,[r0,r4,lsl #0x2]
  0018821c: blx 0x000780f4
  00188220: adds r4,#0x1
  00188222: ldr r0,[r5,#0x0]
  00188224: cmp r4,r0
  00188226: bcc 0x00188216
  00188228: b 0x0018825c
  0018822a: ldr.w r0,[r8,#0x0]
  0018822e: blx 0x00071770
  00188232: cmp r0,#0x2
  00188234: blt 0x0018825c
  00188236: add r5,sp,#0x4
  00188238: ldr.w r1,[r10,#0x58]
  0018823c: mov r0,r5
  0018823e: blx 0x0007264c
  00188242: ldr r0,[0x0018844c]
  00188244: movs r2,#0x0
  00188246: ldr.w r1,[r10,#0x58]
  0018824a: movs r3,#0x0
  0018824c: add r0,pc
  0018824e: ldr r0,[r0,#0x0]
  00188250: ldr r1,[r1,#0x1c]
  00188252: str r2,[sp,#0x0]
  00188254: mov r2,r5
  00188256: ldr r0,[r0,#0x0]
  00188258: blx 0x00071548
  0018825c: ldr.w r0,[r10,#0x78]
  00188260: movs r5,#0x0
  00188262: str.w r5,[r10,#0x50]
  00188266: blx 0x000727c0
  0018826a: blx 0x000769c0
  0018826e: vmov.f32 s0,0x3e800000
  00188272: vldr.32 s2,[r6,#0x28]
  00188276: vldr.32 s4,[pc,#0x1c4]
  0018827a: movs r1,#0x0
  0018827c: ldr.w r0,[r10,#0x78]
  00188280: vcmpe.f32 s2,s0
  00188284: vmrs apsr,fpscr
  00188288: vcmpe.f32 s2,s4
  0018828c: it ge
  0018828e: mov.ge r1,#0x1
  00188290: vmrs apsr,fpscr
  00188294: it gt
  00188296: mov.gt r5,#0x1
  00188298: mov r2,r5
  0018829a: blx 0x00078100
  0018829e: ldr.w r0,[r8,#0x0]
  001882a2: blx 0x00071a58
  001882a6: blx 0x00073df8
  001882aa: vmov.f32 s0,0x3f800000
  001882ae: vmov s2,r0
  001882b2: vsub.f32 s0,s0,s2
  001882b6: vcmpe.f32 s0,#0
  001882ba: vmrs apsr,fpscr
  001882be: blt 0x001882cc
  001882c0: vmov r1,s0
  001882c4: ldr.w r0,[r10,#0x58]
  001882c8: blx 0x0007810c
  001882cc: ldr.w r0,[r8,#0x0]
  001882d0: blx 0x000723d0
  001882d4: cbnz r0,0x00188318
  001882d6: ldr.w r0,[r8,#0x0]
  001882da: blx 0x00071c14
  001882de: blx 0x00071824
  001882e2: sub.w r1,r0,#0x6d
  001882e6: cmp r1,#0x19
  001882e8: bhi 0x00188306
  001882ea: ldr r2,[0x00188440]
  001882ec: movs r3,#0x1
  001882ee: lsls r3,r1
  001882f0: tst r3,r2
  001882f2: bne 0x0018830c
  001882f4: cmp r1,#0x2
  001882f6: bne 0x00188306
  001882f8: ldr.w r0,[r8,#0x0]
  001882fc: blx 0x00071770
  00188300: cmp r0,#0x5e
  00188302: bge 0x0018830c
  00188304: b 0x00188318
  00188306: subs r0,#0x66
  00188308: cmp r0,#0x3
  0018830a: bcs 0x00188318
  0018830c: ldr.w r0,[r8,#0x0]
  00188310: blx 0x00071c14
  00188314: blx 0x0007504c
  00188318: ldr r0,[0x00188450]
  0018831a: add r0,pc
  0018831c: ldr r4,[r0,#0x0]
  0018831e: ldr r1,[r4,#0x0]
  00188320: adds r0,r1,#0x1
  00188322: beq 0x00188330
  00188324: ldr r0,[0x00188454]
  00188326: add r0,pc
  00188328: ldr r0,[r0,#0x0]
  0018832a: ldr r0,[r0,#0x0]
  0018832c: blx 0x00074f80
  00188330: ldr r0,[0x00188458]
  00188332: mov.w r1,#0xffffffff
  00188336: str r1,[r4,#0x0]
  00188338: movs r1,#0x0
  0018833a: add r0,pc
  0018833c: ldr r0,[r0,#0x0]
  0018833e: ldr r0,[r0,#0x0]
  00188340: blx 0x00074f74
  00188344: ldr.w r0,[r10,#0x8]
  00188348: blx 0x0007258c
  0018834c: ldr r1,[0x00188444]
  0018834e: movs r2,#0x0
  00188350: blx 0x000707f8
  00188354: ldr.w r0,[r8,#0x0]
  00188358: blx 0x00071770
  0018835c: cmp r0,#0x9e
  0018835e: bne 0x0018837e
  00188360: ldr.w r0,[r10,#0x78]
  00188364: blx 0x00071eb4
  00188368: cbz r0,0x0018837e
  0018836a: ldr.w r0,[r10,#0x78]
  0018836e: blx 0x00071eb4
  00188372: ldr r0,[r0,#0x4]
  00188374: movs r1,#0x1
  00188376: movs r2,#0x1
  00188378: ldr r0,[r0,#0x0]
  0018837a: blx 0x000757f0
  0018837e: ldr.w r0,[r10,#0x88]
  00188382: cbnz r0,0x00188398
  00188384: mov.w r0,#0x240
  00188388: blx 0x0006eb24
  0018838c: mov r5,r0
  0018838e: movs r1,#0x1
  00188390: blx 0x00075268
  00188394: str.w r5,[r10,#0x88]
  00188398: movs r0,#0xc
  0018839a: blx 0x0006eb24
  0018839e: mov r5,r0
  001883a0: blx 0x0006f628
  001883a4: ldr r0,[0x0018845c]
  001883a6: ldr r1,[0x00188460]
  001883a8: add r0,pc
  001883aa: ldr r2,[0x00188464]
  001883ac: add r1,pc
  001883ae: str.w r5,[r10,#0x1ec]
  001883b2: ldr r0,[r0,#0x0]
  001883b4: add r2,pc
  001883b6: ldr r1,[r1,#0x0]
  001883b8: ldr r2,[r2,#0x0]
  001883ba: ldr r5,[r0,#0x0]
  001883bc: ldr r0,[r1,#0x0]
  001883be: movs r1,#0xc4
  001883c0: ldr r6,[r2,#0x0]
  001883c2: blx 0x00072f70
  001883c6: mov r2,r0
  001883c8: ldr r0,[0x00188468]
  001883ca: ldr r1,[0x0018846c]
  001883cc: add r0,pc
  001883ce: ldr.w r3,[r10,#0x1ec]
  001883d2: add r1,pc
  001883d4: ldr r0,[r0,#0x0]
  001883d6: ldr r1,[r1,#0x0]
  001883d8: ldr r0,[r0,#0x0]
  001883da: ldr r1,[r1,#0x0]
  001883dc: ldr r0,[r0,#0x28]
  001883de: str r3,[sp,#0x0]
  001883e0: sub.w r3,r1,r0, lsl #0x1
  001883e4: mov r0,r6
  001883e6: mov r1,r5
  001883e8: blx 0x00076a44
  001883ec: movs r0,#0x1
  001883ee: strb.w r0,[r10,#0x54]
  001883f2: movs r0,#0x0
  001883f4: ldr r1,[sp,#0x10]
  001883f6: ldr.w r2,[r9,#0x0]
  001883fa: subs r1,r2,r1
  001883fc: ittt eq
  001883fe: add.eq sp,#0x14
  00188400: pop.eq.w {r8,r9,r10,r11}
  00188404: pop.eq {r4,r5,r6,r7,pc}
  00188406: blx 0x0006e824
  0018840a: ldr r0,[r4,#0x0]
  0018840c: blx 0x00071770
  00188410: cmp r0,#0x29
  00188412: bne.w 0x00187e14
  00188416: ldr r0,[r5,#0x0]
  00188418: movs r1,#0x9b
  0018841a: blx r6
  0018841c: ldr r0,[r5,#0x0]
  0018841e: movs r1,#0x99
  00188420: blx r6
  00188422: ldr r0,[r5,#0x0]
  00188424: movs r1,#0x9a
  00188426: b 0x00187ddc
```
