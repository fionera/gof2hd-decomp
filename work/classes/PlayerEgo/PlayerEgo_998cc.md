# PlayerEgo::PlayerEgo
elf 0x998cc body 1890
Sig: undefined __thiscall PlayerEgo(PlayerEgo * this, Player * param_1)

## decompile
```c

/* PlayerEgo::PlayerEgo(Player*) */

PlayerEgo * __thiscall PlayerEgo::PlayerEgo(PlayerEgo *this,Player *param_1)

{
  uint uVar1;
  uint uVar2;
  undefined1 uVar3;
  MovingStars *this_00;
  Player *this_01;
  int *piVar4;
  Ship *pSVar5;
  int iVar6;
  undefined4 uVar7;
  Item *pIVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  AEGeometry *pAVar11;
  AEGeometry *this_02;
  void *pvVar12;
  undefined4 *puVar13;
  int iVar14;
  code *pcVar15;
  code *pcVar16;
  uint in_fpscr;
  float fVar17;
  int *piVar18;
  float fVar19;
  float fVar20;
  int *piVar21;
  SingleLevel *pSVar22;
  SingleLevel *pSVar23;
  SingleLevel *pSVar24;
  
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)&this[1].field_1C);
  piVar4 = (int *)0x0;
  pSVar22 = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  pSVar23 = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  pSVar24 = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this[4].field_4 = (int *)0x0;
  this[4].field_8 = (int *)pSVar22;
  this[4].m_pLevel = pSVar23;
  this[4].field_10 = (int *)pSVar24;
  this[6].field_8 = (int *)0x0;
  this[6].m_pLevel = pSVar22;
  this[6].field_10 = (int *)pSVar23;
  this[6].field_14 = (int *)pSVar24;
  this[9].field_4 = (int *)0x0;
  this[9].field_8 = (int *)0x0;
  this[9].m_pLevel = (SingleLevel *)0x0;
  this[9].field_20 = 0;
  this[10].m_pPlayer = 0;
  this[10].field_4 = (int *)0x0;
  this[0xc].field_18 = (int *)0x0;
  this[0xc].field_1C = 0;
  this[0xc].field_20 = 0;
  this[0xf].field_8 = (int *)0x0;
  this[0xf].m_pLevel = (SingleLevel *)0x0;
  this[0xf].field_10 = (int *)0x0;
  this[4].field_14 = (int *)0x0;
  this[4].field_18 = (int *)0x0;
  this[6].field_18 = (int *)0x0;
  this[6].field_1C = 0;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x13));
  this[0x16].field_20 = (int)piVar4;
  this[0x17].m_pPlayer = (int)pSVar22;
  this[0x17].field_4 = (int *)pSVar23;
  this[0x17].field_8 = (int *)pSVar24;
  this[0x15].field_20 = 0;
  this[0x16].m_pPlayer = 0;
  this[0x16].field_4 = (int *)0x0;
  this[0x17].m_pLevel = (SingleLevel *)0x0;
  this[0x17].field_10 = (int *)0x0;
  this[5].field_14 = (int *)&DAT_00000005;
  this[5].field_18 = (int *)0x1388;
  this[5].field_1C = 20000;
  this->field_4 = (int *)0x0;
  this[6].m_pPlayer = -1;
  this[6].field_4 = (int *)0x0;
  this[1].field_4 = (int *)0x0;
  this[3].field_1C = 0;
  this[3].field_20 = 0;
  this[4].m_pPlayer = 0;
  this[6].field_20 = (int)piVar4;
  this[7].m_pPlayer = (int)pSVar22;
  this[7].field_4 = (int *)pSVar23;
  this[7].field_8 = (int *)pSVar24;
  this->field_14 = (int *)0x0;
  this->field_18 = (int *)0x0;
  this[8].field_8 = piVar4;
  this[8].m_pLevel = pSVar22;
  this[8].field_10 = (int *)pSVar23;
  this[8].field_14 = (int *)pSVar24;
  this[7].field_1C = (int)piVar4;
  this[7].field_20 = (int)pSVar22;
  this[8].m_pPlayer = (int)pSVar23;
  this[8].field_4 = (int *)pSVar24;
  this[7].m_pLevel = (SingleLevel *)0x0;
  this[7].field_10 = (int *)0xffffffff;
  this[7].field_14 = (int *)0x0;
  *(undefined1 *)&this[1].m_pPlayer = 0;
  this[9].field_10 = (int *)0x0;
  this[0x10].field_14 = (int *)0x0;
  *(undefined1 *)&this[9].field_14 = 0;
  this[9].field_18 = (int *)0x0;
  *(undefined1 *)&this[9].field_1C = 0;
  *(undefined1 *)&this[10].field_8 = 0;
  *(undefined4 *)((int)&this[8].field_18 + 1) = 0;
  *(undefined4 *)((int)&this[8].field_14 + 1) = 0;
  *(undefined1 *)((int)&this[9].m_pPlayer + 2) = 0;
  *(undefined2 *)&this[9].m_pPlayer = 0;
  this[8].field_20 = 0;
  this[10].m_pLevel = (SingleLevel *)0xffffffff;
  this[10].field_10 = (int *)0x0;
  this[10].field_14 = (int *)0x0;
  this[0xb].field_10 = (int *)0x0;
  *(undefined1 *)&this[0xb].field_14 = 0;
  *(undefined1 *)((int)&this[0xb].field_14 + 1) = 0;
  this[0xc].field_14 = (int *)0x0;
  *(undefined1 *)&this[0xd].m_pPlayer = 0;
  this[0xd].field_1C = 0;
  *(undefined1 *)&this[0xd].field_20 = 0;
  *(undefined1 *)((int)&this[1].m_pPlayer + 1) = 0;
  this[0xf].field_14 = (int *)0x0;
  *(undefined1 *)&this[0xf].field_18 = 0;
  this->field_8 = (int *)0x0;
  this[0x10].m_pLevel = (SingleLevel *)0x0;
  this[0x15].field_4 = (int *)0x0;
  *(undefined1 *)&this[0x15].field_14 = 0;
  this[1].field_8 = (int *)0x0;
  this[1].m_pLevel = (SingleLevel *)0x0;
  *(undefined4 *)((int)&this[1].field_10 + 1) = 0;
  *(undefined4 *)((int)&this[1].m_pLevel + 1) = 0;
  this[0xb].field_18 = (int *)0x0;
  this[0xb].field_1C = 0;
  *(undefined2 *)&this[0xb].field_20 = 0;
  this[0xc].m_pPlayer = (int)piVar4;
  this[0xc].field_4 = (int *)pSVar22;
  this[0xc].field_8 = (int *)pSVar23;
  this[0xc].m_pLevel = pSVar24;
  *(undefined1 *)&this[0xc].field_10 = 0;
  this[0xd].field_4 = piVar4;
  this[0xd].field_8 = (int *)pSVar22;
  this[0xd].m_pLevel = pSVar23;
  this[0xd].field_10 = (int *)pSVar24;
  iVar6 = DAT_000a9d80;
  *(undefined4 *)((int)&this[0xd].field_14 + 3) = 0;
  *(undefined4 *)((int)&this[0xd].field_10 + 3) = 0;
  this[0xe].m_pPlayer = 0;
  this[0xe].field_4 = (int *)0x0;
  *(undefined4 *)((int)&this[0xe].field_8 + 1) = 0;
  *(undefined4 *)((int)&this[0xe].field_4 + 1) = 0;
  this[0xe].field_1C = (int)piVar4;
  this[0xe].field_20 = (int)pSVar22;
  this[0xf].m_pPlayer = (int)pSVar23;
  this[0xf].field_4 = (int *)pSVar24;
  this[0xe].field_10 = piVar4;
  this[0xe].field_14 = (int *)pSVar22;
  this[0xe].field_18 = (int *)pSVar23;
  this[0xe].field_1C = (int)pSVar24;
  this[0x15].m_pLevel = (SingleLevel *)0xffffffff;
  this[0x15].field_10 = (int *)0xffffffff;
  *(undefined1 *)((int)&this[0x15].field_14 + 1) = 1;
  *(undefined1 *)((int)&this[0x16].field_14 + 2) = 1;
  *(undefined1 *)((int)&this[0x16].field_14 + 3) = 1;
  *(undefined1 *)((int)&this[0x16].field_14 + 1) = 0;
  *(undefined1 *)((int)&this[0x15].field_14 + 2) = 0;
  *(undefined1 *)&this[0x16].field_18 = 0;
  this[0x16].field_1C = 0;
  this[0x17].field_14 = (int *)0x0;
  this[0xb].field_8 = (int *)0x0;
  *(undefined1 *)&this[10].field_18 = 0;
  *(undefined1 *)((int)&this[0x17].field_18 + 1) = 1;
  this[0xb].m_pLevel = (SingleLevel *)0x0;
  this[3].field_10 = (int *)0x0;
  this[3].field_14 = (int *)0x0;
  this[0x15].field_18 = (int *)0x0;
  this[0x15].field_1C = 0;
  this[4].field_1C = 0;
  *(undefined1 *)((int)&this[0x17].field_18 + 2) = 0;
  *(undefined1 *)((int)&this[0x19].field_14 + 1) = 0;
  *(undefined1 *)((int)&this[4].field_20 + 2) = 0;
  *(undefined1 *)&this[0x18].field_10 = 0;
  this[10].field_1C = (int)piVar4;
  this[10].field_20 = (int)pSVar22;
  this[0xb].m_pPlayer = (int)pSVar23;
  this[0xb].field_4 = (int *)pSVar24;
  this[0x18].field_14 = (int *)0x7d0;
  *(undefined1 *)&this[4].field_20 = 0;
  *(undefined1 *)((int)&this[4].field_20 + 1) = 0;
  this[0x18].field_1C = 0;
  this[0x18].field_20 = iVar6;
  this[0x19].m_pPlayer = 0;
  this[0x11].field_14 = (int *)0x0;
  this[0x11].field_18 = (int *)0x0;
  this[0x14].field_18 = (int *)0x0;
  this[0x14].field_1C = 0;
  this[0x14].field_20 = 0;
  *(undefined1 *)((int)&this[0x15].m_pPlayer + 1) = 0;
  *(undefined1 *)((int)&this[0x12].field_20 + 1) = 0;
  *(undefined1 *)&this[0x16].m_pLevel = 0;
  this[0x10].field_18 = piVar4;
  this[0x10].field_1C = (int)pSVar22;
  this[0x10].field_20 = (int)pSVar23;
  this[0x11].m_pPlayer = (int)pSVar24;
  this->m_pPlayer = (int)param_1;
  Player::setPlayShootSound(param_1,true,2);
  this[7].m_pLevel = (SingleLevel *)0x1;
  this_00 = operator_new(0x1c);
  MovingStars::MovingStars(this_00);
  this_01 = (Player *)this->m_pPlayer;
  this[6].field_20 = (int)this_00;
  piVar4 = (int *)Player::getCombinedHP(this_01);
  iVar6 = DAT_000a9d88;
  puVar13 = (undefined4 *)(DAT_000a9d84 + 0xa9b04);
  this[8].field_10 = piVar4;
  puVar13 = (undefined4 *)*puVar13;
  this[5].field_4 = (int *)0x40000000;
  this[5].field_8 = (int *)&LAB_3f800000;
  pcVar16 = *(code **)(iVar6 + 0xa9b0a);
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  iVar6 = Ship::getBoostDelay(pSVar5);
  this[5].field_1C = iVar6;
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  uVar7 = Ship::getBoostSpeed(pSVar5);
  fVar19 = DAT_000a9d8c;
  fVar17 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
  this[5].field_14 = (int *)((int)(fVar17 / DAT_000a9d8c + fVar17 / DAT_000a9d8c) + 2);
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  piVar4 = (int *)Ship::getBoostTime(pSVar5);
  this[5].field_18 = piVar4;
  *(bool *)((int)&this[9].m_pPlayer + 2) = 0 < this[5].field_1C;
  iVar6 = (*pcVar16)(*puVar13);
  pIVar8 = (Item *)Ship::getFirstEquipmentOfSort(iVar6);
  if (pIVar8 != (Item *)0x0) {
    iVar6 = Item::getIndex(pIVar8);
    switch(iVar6) {
    case 0x47:
      iVar6 = 0x26;
      break;
    case 0x48:
      iVar6 = 0x27;
      break;
    case 0x49:
      iVar6 = 0x28;
      break;
    case 0x4a:
      iVar6 = 0x29;
      break;
    default:
      if (iVar6 == 0xc3) {
        iVar6 = 0x44e;
        break;
      }
      goto LAB_000a9bb0;
    }
    this[5].field_20 = iVar6;
LAB_000a9bb0:
    Globals::addSoundResourceToList(**(int **)(DAT_000a9d90 + 0xa9bb6));
  }
  pSVar5 = (Ship *)Status::getShip();
  piVar4 = (int *)Ship::getHandling(pSVar5);
  this[9].field_10 = piVar4;
  Status::getShip();
  iVar6 = Ship::getIndex();
  if (iVar6 == 0x2a) {
    iVar6 = 0x450;
  }
  else {
    Status::getShip();
    iVar6 = Ship::getIndex();
    if (iVar6 == 0x2b) {
      iVar6 = 0x452;
    }
    else {
      Status::getShip();
      iVar6 = Ship::getIndex();
      if (iVar6 == 0x28) {
        iVar6 = 0x453;
      }
      else {
        piVar4 = this[9].field_10;
        uVar1 = in_fpscr & 0xfffffff;
        uVar2 = uVar1 | (uint)((float)piVar4 < DAT_000a9d94) << 0x1f;
        in_fpscr = uVar2 | (uint)(NAN((float)piVar4) || NAN(DAT_000a9d94)) << 0x1c;
        if ((byte)(uVar2 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
          iVar6 = 0x2d;
        }
        else {
          uVar2 = uVar1 | (uint)((float)piVar4 < DAT_000a9d98) << 0x1f;
          in_fpscr = uVar2 | (uint)(NAN((float)piVar4) || NAN(DAT_000a9d98)) << 0x1c;
          if ((byte)(uVar2 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
            iVar6 = 0x2c;
          }
          else {
            uVar1 = uVar1 | (uint)((float)piVar4 < DAT_000a9d9c) << 0x1f;
            in_fpscr = uVar1 | (uint)(NAN((float)piVar4) || NAN(DAT_000a9d9c)) << 0x1c;
            if ((byte)(uVar1 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
              iVar6 = 0x2b;
            }
            else {
              iVar6 = 0x2a;
            }
          }
        }
      }
    }
  }
  this->field_1C = iVar6;
  piVar4 = *(int **)(DAT_000a9da0 + 0xa9c5e);
  Globals::addSoundResourceToList(*piVar4);
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  uVar7 = Ship::getAgility(pSVar5);
  piVar18 = (int *)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
  piVar21 = this[9].field_10;
  this[0x10].field_14 = piVar18;
  this[9].field_10 = (int *)(((float)piVar21 + (float)piVar21 * ((float)piVar18 / fVar19)) * 20.0);
  uVar3 = Status::hardCoreMode();
  *(undefined1 *)((int)&this[0xf].field_18 + 1) = uVar3;
  (*pcVar16)(*puVar13);
  uVar7 = Ship::getMaxShieldHP();
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  uVar9 = Ship::getShieldRegen(pSVar5);
  fVar17 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
  fVar20 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
  this[0x19].field_4 = (int *)0xffffffff;
  this[0xe].field_20 = (int)(fVar20 / (fVar17 / fVar19));
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  iVar6 = Ship::hasCloakIntegrated(pSVar5);
  if (iVar6 == 0) {
    iVar6 = Status::getShip();
    iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
  }
  else {
    iVar6 = *(int *)(*(int *)(**(int **)(DAT_000aa088 + 0xa9cee) + 4) + 0x17c);
  }
  this[0xc].m_pPlayer = iVar6;
  this[0xe].field_10 = (int *)0xffffffff;
  if (iVar6 == 0) {
    this[0xe].field_18 = (int *)0x0;
  }
  else {
    piVar18 = (int *)Item::getAttribute(iVar6);
    iVar6 = this[0xc].m_pPlayer;
    this[0xe].field_18 = piVar18;
    if (iVar6 != 0) {
      iVar6 = Item::getAttribute(iVar6);
      iVar14 = this[0xc].m_pPlayer;
      this[0xe].field_1C = iVar6;
      if (iVar14 != 0) {
        Globals::addSoundResourceToList(*piVar4);
      }
      goto LAB_000a9d4e;
    }
  }
  this[0xe].field_1C = 0;
LAB_000a9d4e:
  iVar6 = Status::getShip();
  iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
  if (iVar6 == 0) {
    this[0xe].field_8 = (int *)0x0;
    Status::getShip();
    iVar6 = Ship::hasJumpDriveIntegrated();
    if (iVar6 != 0) {
      piVar18 = (int *)Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_000aa08c + 0xa9dc0) + 4)
                                                  + 0x154));
      this[0xe].field_8 = piVar18;
    }
  }
  else {
    piVar18 = (int *)Item::getAttribute(iVar6);
    this[0xe].field_8 = piVar18;
    Globals::addSoundResourceToList(*piVar4);
    Globals::addSoundResourceToList(*piVar4);
  }
  iVar6 = Status::getShip();
  iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
  if (iVar6 != 0) {
    pcVar15 = *(code **)(DAT_000aa090 + 0xa9de8);
    (*pcVar15)(*piVar4,1);
    (*pcVar15)(*piVar4,3);
    (*pcVar15)(*piVar4,2);
  }
  iVar6 = Status::getShip();
  iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
  if (iVar6 != 0) {
    Globals::addSoundResourceToList(*piVar4);
  }
  iVar6 = Status::getShip();
  iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
  if (iVar6 != 0) {
    Globals::addSoundResourceToList(*piVar4);
  }
  pSVar5 = (Ship *)(*pcVar16)(*puVar13);
  uVar3 = Ship::hasVolatileGoods(pSVar5);
  *(undefined1 *)&this[0x19].field_14 = uVar3;
  (*pcVar16)(*puVar13);
  piVar4 = (int *)Ship::getFreeSpace();
  this[0x10].field_10 = piVar4;
  iVar6 = (*pcVar16)(*puVar13);
  iVar14 = Ship::getFirstEquipmentOfSort(iVar6);
  iVar6 = DAT_000aa098;
  puVar13 = (undefined4 *)(DAT_000aa094 + 0xa9e68);
  *(bool *)((int)&this[0x19].field_14 + 2) = iVar14 != 0;
  puVar13 = (undefined4 *)*puVar13;
  pcVar16 = *(code **)(iVar6 + 0xa9e6e);
  (*pcVar16)(*puVar13,0x4c0,&this[0xf].field_20);
  (*pcVar16)(*puVar13,0x4ce,this + 0x10);
  (*pcVar16)(*puVar13,0x1f5e,&this[0x10].field_4);
  (*pcVar16)(*puVar13,0x1f5d,&this[0x10].field_8);
  (*pcVar16)(*puVar13,0x548,&this[0xf].field_1C);
  *(undefined1 *)&this[5].field_10 = 0;
  uVar3 = Status::inBlackMarketSystem();
  fVar19 = DAT_000aa070;
  piVar4 = this[9].field_10;
  *(undefined1 *)&this[0x17].field_18 = uVar3;
  iVar6 = DAT_000aa07c;
  fVar17 = DAT_000aa074;
  this->field_20 = 0;
  this[0x12].m_pPlayer = iVar6;
  this[0x11].field_20 = (int)((((float)piVar4 * fVar19) / fVar17) * DAT_000aa078);
  for (iVar6 = 0x290; iVar14 = DAT_000aa0a0, iVar6 != 0x2a4; iVar6 = iVar6 + 4) {
    *(undefined4 *)((int)&this->m_pPlayer + iVar6) = 0;
  }
  puVar10 = (undefined4 *)(DAT_000aa09c + 0xa9f10);
  *(undefined1 *)&this[0x12].field_20 = 0;
  this[0x12].field_1C = 0;
  piVar4 = (int *)*puVar10;
  this[0x16].field_10 = (int *)0x0;
  *(undefined1 *)&this[0x16].field_14 = 0;
  iVar6 = *piVar4;
  this[0x11].field_4 = (int *)0x0;
  this[0x11].field_8 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  this[0x11].m_pLevel = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  this[0x11].field_10 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this[0x17].field_1C = 0;
  this[0x17].field_20 = (int)*(int **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  this[0x18].m_pPlayer = (int)*(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  this[0x18].field_4 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar14 = *(int *)(iVar14 + 0xa9f20);
  *(undefined1 *)&this[3].field_18 = 1;
  this[0x18].m_pLevel = (SingleLevel *)0x0;
  this[5].m_pPlayer = -1;
  *(undefined1 *)((int)&this[0x19].field_14 + 3) = 0;
  *(bool *)&this[0x18].field_18 = iVar6 == 0;
  fVar19 = *(float *)(iVar14 + 0x2c);
  if (fVar19 <= 0.0) {
    piVar4 = (int *)0x1388;
  }
  else if (fVar19 <= DAT_000aa080) {
    piVar4 = (int *)0x1b58;
  }
  else {
    piVar4 = (int *)0x2ee0;
    if (fVar19 <= DAT_000aa084) {
      piVar4 = (int *)0x2328;
    }
  }
  this[0x18].field_8 = piVar4;
  iVar6 = Status::inSupernovaSystem();
  if ((iVar6 != 0) || (iVar6 = Status::inSupernovaOrbit(), iVar6 != 0)) {
    iVar6 = Status::getShip();
    iVar6 = Ship::getFirstEquipmentOfSort(iVar6);
    if (iVar6 != 0) {
      iVar6 = Status::getShip();
      pIVar8 = (Item *)Ship::getFirstEquipmentOfSort(iVar6);
      iVar6 = Item::getIndex(pIVar8);
      iVar14 = 0x8d4;
      if (iVar6 == 0xcd) {
        iVar14 = 0x8d5;
      }
      this[5].m_pPlayer = iVar14;
      *(undefined1 *)&this[1].field_14 = 1;
      pAVar11 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar11,(PaintCanvas *)*puVar13);
      this[1].field_10 = (int *)pAVar11;
      pAVar11 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar11,0x4973,(PaintCanvas *)*puVar13,false);
      this_02 = operator_new(0xc0);
      AEGeometry::AEGeometry(this_02,0x4972,(PaintCanvas *)*puVar13,false);
      AEGeometry::addChild((uint)this[1].field_10);
      pvVar12 = (void *)AEGeometry::~AEGeometry(pAVar11);
      operator_delete(pvVar12);
      AEGeometry::addChild((uint)this[1].field_10);
      pvVar12 = (void *)AEGeometry::~AEGeometry(this_02);
      operator_delete(pvVar12);
    }
  }
  return this;
}

```

## target disasm
```
  000a98cc: push {r4,r5,r6,r7,lr}
  000a98ce: add r7,sp,#0xc
  000a98d0: push {r7,r8,r9,r10,r11}
  000a98d4: vpush {d8,d9}
  000a98d8: mov r4,r0
  000a98da: adds r0,#0x40
  000a98dc: mov r8,r1
  000a98de: blx 0x0006f118
  000a98e2: vmov.i32 q4,#0x0
  000a98e6: add.w r0,r4,#0x94
  000a98ea: movs r6,#0x0
  000a98ec: vst1.32 {d8,d9},[r0]
  000a98f0: add.w r0,r4,#0xe0
  000a98f4: vst1.32 {d8,d9},[r0]
  000a98f8: add.w r0,r4,#0x2ac
  000a98fc: strd r6,r6,[r4,#0x148]
  000a9900: str.w r6,[r4,#0x150]
  000a9904: strd r6,r6,[r4,#0x164]
  000a9908: str.w r6,[r4,#0x16c]
  000a990c: strd r6,r6,[r4,#0x1c8]
  000a9910: str.w r6,[r4,#0x1d0]
  000a9914: strd r6,r6,[r4,#0x224]
  000a9918: str.w r6,[r4,#0x22c]
  000a991c: strd r6,r6,[r4,#0xa4]
  000a9920: strd r6,r6,[r4,#0xf0]
  000a9924: blx 0x0006f118
  000a9928: add.w r1,r4,#0x338
  000a992c: movw r2,#0x1388
  000a9930: movw r3,#0x4e20
  000a9934: add.w r12,r4,#0xc8
  000a9938: vst1.32 {d8,d9},[r1]
  000a993c: movs r1,#0x5
  000a993e: add.w r11,r4,#0x1b0
  000a9942: add.w r10,r4,#0x208
  000a9946: strd r6,r6,[r4,#0x314]
  000a994a: movs r5,#0x1
  000a994c: str.w r6,[r4,#0x31c]
  000a9950: strd r6,r6,[r4,#0x348]
  000a9954: stm.w r12!,{r1,r2,r3}
  000a9958: add.w r2,r4,#0xf8
  000a995c: mov.w r1,#0xffffffff
  000a9960: str r6,[r4,#0x4]
  000a9962: strd r1,r6,[r4,#0xd8]
  000a9966: str r6,[r4,#0x28]
  000a9968: strd r6,r6,[r4,#0x88]
  000a996c: str.w r6,[r4,#0x90]
  000a9970: vst1.64 {d8,d9},[r2]
  000a9974: add.w r2,r4,#0x128
  000a9978: strd r6,r6,[r4,#0x14]
  000a997c: vst1.64 {d8,d9},[r2]
  000a9980: add.w r2,r4,#0x118
  000a9984: vst1.64 {d8,d9},[r2]
  000a9988: add.w r2,r4,#0x1d8
  000a998c: strd r6,r1,[r4,#0x108]
  000a9990: str.w r6,[r4,#0x110]
  000a9994: strb.w r6,[r4,#0x24]
  000a9998: str.w r6,[r4,#0x154]
  000a999c: str.w r6,[r4,#0x254]
  000a99a0: strb.w r6,[r4,#0x158]
  000a99a4: str.w r6,[r4,#0x15c]
  000a99a8: strb.w r6,[r4,#0x160]
  000a99ac: strb.w r6,[r4,#0x170]
  000a99b0: str.w r6,[r4,#0x139]
  000a99b4: str.w r6,[r4,#0x135]
  000a99b8: strb.w r6,[r4,#0x146]
  000a99bc: strh.w r6,[r4,#0x144]
  000a99c0: str.w r6,[r4,#0x140]
  000a99c4: strd r1,r6,[r4,#0x174]
  000a99c8: str.w r6,[r4,#0x17c]
  000a99cc: str.w r6,[r4,#0x19c]
  000a99d0: strb.w r6,[r4,#0x1a0]
  000a99d4: strb.w r6,[r4,#0x1a1]
  000a99d8: str.w r6,[r4,#0x1c4]
  000a99dc: strb.w r6,[r4,#0x1d4]
  000a99e0: str.w r6,[r4,#0x1f0]
  000a99e4: strb.w r6,[r4,#0x1f4]
  000a99e8: strb.w r6,[r4,#0x25]
  000a99ec: str.w r6,[r4,#0x230]
  000a99f0: strb.w r6,[r4,#0x234]
  000a99f4: str r6,[r4,#0x8]
  000a99f6: str.w r6,[r4,#0x24c]
  000a99fa: str.w r6,[r4,#0x2f8]
  000a99fe: strb.w r6,[r4,#0x308]
  000a9a02: strd r6,r6,[r4,#0x2c]
  000a9a06: str.w r6,[r4,#0x35]
  000a9a0a: str.w r6,[r4,#0x31]
  000a9a0e: strd r6,r6,[r4,#0x1a4]
  000a9a12: strh.w r6,[r4,#0x1ac]
  000a9a16: vst1.64 {d8,d9},[r11]
  000a9a1a: strb.w r6,[r4,#0x1c0]
  000a9a1e: vst1.64 {d8,d9},[r2]
  000a9a22: add.w r2,r4,#0x214
  000a9a26: ldr r0,[0x000a9d80]
  000a9a28: str.w r6,[r4,#0x1eb]
  000a9a2c: str.w r6,[r4,#0x1e7]
  000a9a30: strd r6,r6,[r4,#0x1f8]
  000a9a34: str.w r6,[r4,#0x201]
  000a9a38: str.w r6,[r4,#0x1fd]
  000a9a3c: vst1.32 {d8,d9},[r2]
  000a9a40: movs r2,#0x2
  000a9a42: vst1.64 {d8,d9},[r10]
  000a9a46: strd r1,r1,[r4,#0x300]
  000a9a4a: add.w r1,r4,#0x184
  000a9a4e: strb.w r5,[r4,#0x309]
  000a9a52: strb.w r5,[r4,#0x32e]
  000a9a56: strb.w r5,[r4,#0x32f]
  000a9a5a: strb.w r6,[r4,#0x32d]
  000a9a5e: strb.w r6,[r4,#0x30a]
  000a9a62: strb.w r6,[r4,#0x330]
  000a9a66: str.w r6,[r4,#0x334]
  000a9a6a: str.w r6,[r4,#0x350]
  000a9a6e: str.w r6,[r4,#0x194]
  000a9a72: strb.w r6,[r4,#0x180]
  000a9a76: strb.w r5,[r4,#0x355]
  000a9a7a: str.w r6,[r4,#0x198]
  000a9a7e: strd r6,r6,[r4,#0x7c]
  000a9a82: strd r6,r6,[r4,#0x30c]
  000a9a86: str.w r6,[r4,#0xac]
  000a9a8a: strb.w r6,[r4,#0x356]
  000a9a8e: strb.w r6,[r4,#0x399]
  000a9a92: strb.w r6,[r4,#0xb2]
  000a9a96: strb.w r6,[r4,#0x370]
  000a9a9a: vst1.32 {d8,d9},[r1]
  000a9a9e: mov.w r1,#0x7d0
  000a9aa2: str.w r1,[r4,#0x374]
  000a9aa6: movs r1,#0x1
  000a9aa8: strb.w r6,[r4,#0xb0]
  000a9aac: strb.w r6,[r4,#0xb1]
  000a9ab0: strd r6,r0,[r4,#0x37c]
  000a9ab4: add.w r0,r4,#0x258
  000a9ab8: str.w r6,[r4,#0x384]
  000a9abc: strd r6,r6,[r4,#0x278]
  000a9ac0: strd r6,r6,[r4,#0x2e8]
  000a9ac4: str.w r6,[r4,#0x2f0]
  000a9ac8: strb.w r6,[r4,#0x2f5]
  000a9acc: strb.w r6,[r4,#0x2a9]
  000a9ad0: strb.w r6,[r4,#0x324]
  000a9ad4: vst1.64 {d8,d9},[r0]
  000a9ad8: mov r0,r8
  000a9ada: str.w r8,[r4,#0x0]
  000a9ade: blx 0x00072244
  000a9ae2: movs r0,#0x1c
  000a9ae4: str.w r5,[r4,#0x108]
  000a9ae8: blx 0x0006eb24
  000a9aec: mov r5,r0
  000a9aee: blx 0x00072250
  000a9af2: ldr r0,[r4,#0x0]
  000a9af4: str.w r5,[r4,#0xf8]
  000a9af8: blx 0x0007225c
  000a9afc: ldr r1,[0x000a9d84]
  000a9afe: ldr r2,[0x000a9d88]
  000a9b00: add r1,pc
  000a9b02: str.w r0,[r4,#0x130]
  000a9b06: add r2,pc
  000a9b08: mov.w r0,#0x40000000
  000a9b0c: ldr.w r9,[r1,#0x0]
  000a9b10: mov.w r1,#0x3f800000
  000a9b14: strd r0,r1,[r4,#0xb8]
  000a9b18: ldr.w r8,[r2,#0x0]
  000a9b1c: ldr.w r0,[r9,#0x0]
  000a9b20: blx r8
  000a9b22: blx 0x00072268
  000a9b26: str.w r0,[r4,#0xd0]
  000a9b2a: ldr.w r0,[r9,#0x0]
  000a9b2e: blx r8
  000a9b30: blx 0x00072274
  000a9b34: vmov s0,r0
  000a9b38: vldr.32 s16,[pc,#0x250]
  000a9b3c: vcvt.f32.s32 s0,s0
  000a9b40: vdiv.f32 s0,s0,s16
  000a9b44: vadd.f32 s0,s0,s0
  000a9b48: vcvt.s32.f32 s0,s0
  000a9b4c: vmov r0,s0
  000a9b50: adds r0,#0x2
  000a9b52: str.w r0,[r4,#0xc8]
  000a9b56: ldr.w r0,[r9,#0x0]
  000a9b5a: blx r8
  000a9b5c: blx 0x00072280
  000a9b60: ldr.w r1,[r4,#0xd0]
  000a9b64: str.w r0,[r4,#0xcc]
  000a9b68: cmp r1,#0x0
  000a9b6a: it gt
  000a9b6c: mov.gt r6,#0x1
  000a9b6e: strb.w r6,[r4,#0x146]
  000a9b72: ldr.w r0,[r9,#0x0]
  000a9b76: blx r8
  000a9b78: movs r1,#0xe
  000a9b7a: blx 0x0007228c
  000a9b7e: cbz r0,0x000a9bbc
  000a9b80: blx 0x000718d8
  000a9b84: sub.w r1,r0,#0x47
  000a9b88: cmp r1,#0x3
  000a9b8a: bhi 0x000a9b98
  000a9b8c: tbb [pc,r1]
  000a9b94: movs r1,#0x26
  000a9b96: b 0x000a9bac
  000a9b98: cmp r0,#0xc3
  000a9b9a: bne 0x000a9c4a
  000a9b9c: movw r1,#0x44e
  000a9ba0: b 0x000a9bac
  000a9ba2: movs r1,#0x27
  000a9ba4: b 0x000a9bac
  000a9ba6: movs r1,#0x28
  000a9ba8: b 0x000a9bac
  000a9baa: movs r1,#0x29
  000a9bac: str.w r1,[r4,#0xd4]
  000a9bb0: ldr r0,[0x000a9d90]
  000a9bb2: add r0,pc
  000a9bb4: ldr r0,[r0,#0x0]
  000a9bb6: ldr r0,[r0,#0x0]
  000a9bb8: blx 0x00072298
  000a9bbc: ldr.w r0,[r9,#0x0]
  000a9bc0: blx 0x00071a58
  000a9bc4: blx 0x000722a4
  000a9bc8: str.w r0,[r4,#0x154]
  000a9bcc: ldr.w r0,[r9,#0x0]
  000a9bd0: blx 0x00071a58
  000a9bd4: blx 0x000719c8
  000a9bd8: cmp r0,#0x2a
  000a9bda: bne 0x000a9be4
  000a9bdc: mov r6,r11
  000a9bde: mov.w r1,#0x450
  000a9be2: b 0x000a9c56
  000a9be4: ldr.w r0,[r9,#0x0]
  000a9be8: blx 0x00071a58
  000a9bec: blx 0x000719c8
  000a9bf0: cmp r0,#0x2b
  000a9bf2: bne 0x000a9bfc
  000a9bf4: mov r6,r11
  000a9bf6: movw r1,#0x452
  000a9bfa: b 0x000a9c56
  000a9bfc: ldr.w r0,[r9,#0x0]
  000a9c00: blx 0x00071a58
  000a9c04: blx 0x000719c8
  000a9c08: cmp r0,#0x28
  000a9c0a: bne 0x000a9c14
  000a9c0c: mov r6,r11
  000a9c0e: movw r1,#0x453
  000a9c12: b 0x000a9c56
  000a9c14: vldr.32 s2,[pc,#0x17c]
  000a9c18: mov r6,r11
  000a9c1a: vldr.32 s0,[r4,#0x154]
  000a9c1e: vcmpe.f32 s0,s2
  000a9c22: vmrs apsr,fpscr
  000a9c26: bge 0x000a9c50
  000a9c28: vldr.32 s2,[pc,#0x16c]
  000a9c2c: vcmpe.f32 s0,s2
  000a9c30: vmrs apsr,fpscr
  000a9c34: bge 0x000a9c54
  000a9c36: vldr.32 s2,[pc,#0x164]
  000a9c3a: vcmpe.f32 s0,s2
  000a9c3e: vmrs apsr,fpscr
  000a9c42: ite lt
  000a9c44: mov.lt r1,#0x2a
  000a9c46: mov.ge r1,#0x2b
  000a9c48: b 0x000a9c56
  000a9c4a: ldr.w r1,[r4,#0xd4]
  000a9c4e: b 0x000a9bb0
  000a9c50: movs r1,#0x2d
  000a9c52: b 0x000a9c56
  000a9c54: movs r1,#0x2c
  000a9c56: str r1,[r4,#0x1c]
  000a9c58: ldr r0,[0x000a9da0]
  000a9c5a: add r0,pc
  000a9c5c: ldr.w r11,[r0,#0x0]
  000a9c60: ldr.w r0,[r11,#0x0]
  000a9c64: blx 0x00072298
  000a9c68: ldr.w r0,[r9,#0x0]
  000a9c6c: blx r8
  000a9c6e: blx 0x000722b0
  000a9c72: vmov s0,r0
  000a9c76: vmov.f32 s6,0x41a00000
  000a9c7a: vcvt.f32.s32 s0,s0
  000a9c7e: vldr.32 s4,[r4,#0x154]
  000a9c82: vdiv.f32 s2,s0,s16
  000a9c86: vmla.f32 s4,s4,s2
  000a9c8a: vstr.32 s0,[r4,#0x254]
  000a9c8e: vmul.f32 s2,s4,s6
  000a9c92: vstr.32 s2,[r4,#0x154]
  000a9c96: ldr.w r0,[r9,#0x0]
  000a9c9a: blx 0x0007192c
  000a9c9e: strb.w r0,[r4,#0x235]
  000a9ca2: ldr.w r0,[r9,#0x0]
  000a9ca6: blx r8
  000a9ca8: blx 0x000722bc
  000a9cac: mov r5,r0
  000a9cae: ldr.w r0,[r9,#0x0]
  000a9cb2: blx r8
  000a9cb4: blx 0x000722c8
  000a9cb8: vmov s0,r0
  000a9cbc: mov.w r0,#0xffffffff
  000a9cc0: vmov s2,r5
  000a9cc4: vcvt.f32.s32 s0,s0
  000a9cc8: vcvt.f32.s32 s2,s2
  000a9ccc: str.w r0,[r4,#0x388]
  000a9cd0: vdiv.f32 s0,s0,s16
  000a9cd4: vdiv.f32 s0,s2,s0
  000a9cd8: vstr.32 s0,[r4,#0x218]
  000a9cdc: ldr.w r0,[r9,#0x0]
  000a9ce0: blx r8
  000a9ce2: blx 0x000722d4
  000a9ce6: cbz r0,0x000a9cf8
  000a9ce8: ldr r0,[0x000aa088]
  000a9cea: add r0,pc
  000a9cec: ldr r0,[r0,#0x0]
  000a9cee: ldr r0,[r0,#0x0]
  000a9cf0: ldr r0,[r0,#0x4]
  000a9cf2: ldr.w r0,[r0,#0x17c]
  000a9cf6: b 0x000a9d06
  000a9cf8: ldr.w r0,[r9,#0x0]
  000a9cfc: blx 0x00071a58
  000a9d00: movs r1,#0x15
  000a9d02: blx 0x0007228c
  000a9d06: cmp r0,#0x0
  000a9d08: mov.w r1,#0xffffffff
  000a9d0c: str r0,[r6,#0x0]
  000a9d0e: str.w r1,[r10,#0x0]
  000a9d12: beq 0x000a9d42
  000a9d14: movs r1,#0x23
  000a9d16: blx 0x000718e4
  000a9d1a: mov r1,r0
  000a9d1c: ldr.w r0,[r4,#0x1b0]
  000a9d20: str.w r1,[r4,#0x210]
  000a9d24: cbz r0,0x000a9d48
  000a9d26: movs r1,#0x24
  000a9d28: blx 0x000718e4
  000a9d2c: ldr.w r1,[r4,#0x1b0]
  000a9d30: str.w r0,[r4,#0x214]
  000a9d34: cbz r1,0x000a9d4e
  000a9d36: ldr.w r0,[r11,#0x0]
  000a9d3a: movs r1,#0x1e
  000a9d3c: blx 0x00072298
  000a9d40: b 0x000a9d4e
  000a9d42: movs r0,#0x0
  000a9d44: str.w r0,[r4,#0x210]
  000a9d48: movs r0,#0x0
  000a9d4a: str.w r0,[r4,#0x214]
  000a9d4e: ldr.w r0,[r9,#0x0]
  000a9d52: blx 0x00071a58
  000a9d56: movs r1,#0x12
  000a9d58: blx 0x0007228c
  000a9d5c: cbz r0,0x000a9da4
  000a9d5e: movs r1,#0x25
  000a9d60: blx 0x000718e4
  000a9d64: str.w r0,[r4,#0x200]
  000a9d68: movs r1,#0x20
  000a9d6a: ldr.w r0,[r11,#0x0]
  000a9d6e: blx 0x00072298
  000a9d72: ldr.w r0,[r11,#0x0]
  000a9d76: movs r1,#0x21
  000a9d78: blx 0x00072298
  000a9d7c: b 0x000a9dd0
  000a9da4: movs r0,#0x0
  000a9da6: str.w r0,[r4,#0x200]
  000a9daa: ldr.w r0,[r9,#0x0]
  000a9dae: blx 0x00071a58
  000a9db2: blx 0x000719bc
  000a9db6: cbz r0,0x000a9dd0
  000a9db8: ldr r0,[0x000aa08c]
  000a9dba: movs r1,#0x25
  000a9dbc: add r0,pc
  000a9dbe: ldr r0,[r0,#0x0]
  000a9dc0: ldr r0,[r0,#0x0]
  000a9dc2: ldr r0,[r0,#0x4]
  000a9dc4: ldr.w r0,[r0,#0x154]
  000a9dc8: blx 0x000718e4
  000a9dcc: str.w r0,[r4,#0x200]
  000a9dd0: ldr.w r0,[r9,#0x0]
  000a9dd4: blx 0x00071a58
  000a9dd8: movs r1,#0x13
  000a9dda: blx 0x0007228c
  000a9dde: cbz r0,0x000a9dfe
  000a9de0: ldr r0,[0x000aa090]
  000a9de2: movs r1,#0x1
  000a9de4: add r0,pc
  000a9de6: ldr r5,[r0,#0x0]
  000a9de8: ldr.w r0,[r11,#0x0]
  000a9dec: blx r5
  000a9dee: ldr.w r0,[r11,#0x0]
  000a9df2: movs r1,#0x3
  000a9df4: blx r5
  000a9df6: ldr.w r0,[r11,#0x0]
  000a9dfa: movs r1,#0x2
  000a9dfc: blx r5
  000a9dfe: ldr.w r0,[r9,#0x0]
  000a9e02: blx 0x00071a58
  000a9e06: movs r1,#0x9
  000a9e08: blx 0x0007228c
  000a9e0c: cbz r0,0x000a9e18
  000a9e0e: ldr.w r0,[r11,#0x0]
  000a9e12: movs r1,#0x19
  000a9e14: blx 0x00072298
  000a9e18: ldr.w r0,[r9,#0x0]
  000a9e1c: blx 0x00071a58
  000a9e20: movs r1,#0xa
  000a9e22: blx 0x0007228c
  000a9e26: cbz r0,0x000a9e32
  000a9e28: ldr.w r0,[r11,#0x0]
  000a9e2c: movs r1,#0x17
  000a9e2e: blx 0x00072298
  000a9e32: ldr.w r0,[r9,#0x0]
  000a9e36: blx r8
  000a9e38: blx 0x000722e0
  000a9e3c: strb.w r0,[r4,#0x398]
  000a9e40: ldr.w r0,[r9,#0x0]
  000a9e44: blx r8
  000a9e46: blx 0x000722ec
  000a9e4a: str.w r0,[r4,#0x250]
  000a9e4e: ldr.w r0,[r9,#0x0]
  000a9e52: blx r8
  000a9e54: movs r1,#0x2b
  000a9e56: blx 0x0007228c
  000a9e5a: ldr r1,[0x000aa094]
  000a9e5c: cmp r0,#0x0
  000a9e5e: ldr r3,[0x000aa098]
  000a9e60: it ne
  000a9e62: mov.ne r0,#0x1
  000a9e64: add r1,pc
  000a9e66: strb.w r0,[r4,#0x39a]
  000a9e6a: add r3,pc
  000a9e6c: add.w r2,r4,#0x23c
  000a9e70: ldr.w r8,[r1,#0x0]
  000a9e74: mov.w r1,#0x4c0
  000a9e78: ldr r5,[r3,#0x0]
  000a9e7a: ldr.w r0,[r8,#0x0]
  000a9e7e: blx r5
  000a9e80: ldr.w r0,[r8,#0x0]
  000a9e84: add.w r2,r4,#0x240
  000a9e88: movw r1,#0x4ce
  000a9e8c: blx r5
  000a9e8e: ldr.w r0,[r8,#0x0]
  000a9e92: add.w r2,r4,#0x244
  000a9e96: movw r1,#0x1f5e
  000a9e9a: blx r5
  000a9e9c: ldr.w r0,[r8,#0x0]
  000a9ea0: add.w r2,r4,#0x248
  000a9ea4: movw r1,#0x1f5d
  000a9ea8: blx r5
  000a9eaa: ldr.w r0,[r8,#0x0]
  000a9eae: add.w r2,r4,#0x238
  000a9eb2: mov.w r1,#0x548
  000a9eb6: blx r5
  000a9eb8: movs r5,#0x0
  000a9eba: strb.w r5,[r4,#0xc4]
  000a9ebe: ldr.w r0,[r9,#0x0]
  000a9ec2: blx 0x00071998
  000a9ec6: vldr.32 s2,[r4,#0x154]
  000a9eca: vldr.32 s0,[pc,#0x1a4]
  000a9ece: strb.w r0,[r4,#0x354]
  000a9ed2: mov.w r0,#0x290
  000a9ed6: vmul.f32 s0,s2,s0
  000a9eda: vldr.32 s2,[pc,#0x198]
  000a9ede: ldr r1,[0x000aa07c]
  000a9ee0: str r5,[r4,#0x20]
  000a9ee2: str.w r1,[r4,#0x288]
  000a9ee6: vdiv.f32 s0,s0,s2
  000a9eea: vldr.32 s2,[pc,#0x18c]
  000a9eee: vmul.f32 s0,s0,s2
  000a9ef2: vstr.32 s0,[r4,#0x284]
  000a9ef6: b 0x000a9efc
  000a9ef8: str r5,[r4,r0]
  000a9efa: adds r0,#0x4
  000a9efc: cmp.w r0,#0x2a4
  000a9f00: bne 0x000a9ef8
  000a9f02: ldr r0,[0x000aa09c]
  000a9f04: vmov.i32 q8,#0x0
  000a9f08: movs r2,#0x0
  000a9f0a: ldr r1,[0x000aa0a0]
  000a9f0c: add r0,pc
  000a9f0e: strb.w r2,[r4,#0x2a8]
  000a9f12: str.w r2,[r4,#0x2a4]
  000a9f16: add.w r3,r4,#0x268
  000a9f1a: ldr r0,[r0,#0x0]
  000a9f1c: add r1,pc
  000a9f1e: str.w r2,[r4,#0x328]
  000a9f22: strb.w r2,[r4,#0x32c]
  000a9f26: ldr r0,[r0,#0x0]
  000a9f28: vst1.64 {d16,d17},[r3]
  000a9f2c: add.w r3,r4,#0x358
  000a9f30: clz r0,r0
  000a9f34: vst1.64 {d16,d17},[r3]
  000a9f38: movs r3,#0x1
  000a9f3a: lsrs r0,r0,#0x5
  000a9f3c: ldr r1,[r1,#0x0]
  000a9f3e: strb.w r3,[r4,#0x84]
  000a9f42: mov.w r3,#0xffffffff
  000a9f46: str.w r2,[r4,#0x36c]
  000a9f4a: str.w r3,[r4,#0xb4]
  000a9f4e: strb.w r2,[r4,#0x39b]
  000a9f52: strb.w r0,[r4,#0x378]
  000a9f56: vldr.32 s0,[r1,#0x2c]
  000a9f5a: vcmpe.f32 s0,#0
  000a9f5e: vmrs apsr,fpscr
  000a9f62: bls 0x000a9f8a
  000a9f64: vldr.32 s2,[pc,#0x118]
  000a9f68: vcmpe.f32 s0,s2
  000a9f6c: vmrs apsr,fpscr
  000a9f70: bls 0x000a9f90
  000a9f72: vldr.32 s2,[pc,#0x110]
  000a9f76: movw r0,#0x2ee0
  000a9f7a: vcmpe.f32 s0,s2
  000a9f7e: vmrs apsr,fpscr
  000a9f82: it ls
  000a9f84: movw.ls r0,#0x2328
  000a9f88: b 0x000a9f94
  000a9f8a: movw r0,#0x1388
  000a9f8e: b 0x000a9f94
  000a9f90: movw r0,#0x1b58
  000a9f94: str.w r0,[r4,#0x368]
  000a9f98: ldr.w r0,[r9,#0x0]
  000a9f9c: blx 0x00071878
  000a9fa0: cbnz r0,0x000a9fae
  000a9fa2: ldr.w r0,[r9,#0x0]
  000a9fa6: blx 0x000722f8
  000a9faa: cmp r0,#0x0
  000a9fac: beq 0x000aa04a
  000a9fae: ldr.w r0,[r9,#0x0]
  000a9fb2: blx 0x00071a58
  000a9fb6: movs r1,#0x26
  000a9fb8: blx 0x0007228c
  000a9fbc: cmp r0,#0x0
  000a9fbe: beq 0x000aa04a
  000a9fc0: ldr.w r0,[r9,#0x0]
  000a9fc4: blx 0x00071a58
  000a9fc8: movs r1,#0x26
  000a9fca: blx 0x0007228c
  000a9fce: blx 0x000718d8
  000a9fd2: movw r1,#0x8d4
  000a9fd6: cmp r0,#0xcd
  000a9fd8: it eq
  000a9fda: movw.eq r1,#0x8d5
  000a9fde: movs r0,#0x1
  000a9fe0: str.w r1,[r4,#0xb4]
  000a9fe4: strb.w r0,[r4,#0x38]
  000a9fe8: movs r0,#0xc0
  000a9fea: blx 0x0006eb24
  000a9fee: ldr.w r1,[r8,#0x0]
  000a9ff2: mov r5,r0
  000a9ff4: blx 0x000720f4
  000a9ff8: movs r0,#0xc0
  000a9ffa: str r5,[r4,#0x34]
  000a9ffc: blx 0x0006eb24
  000aa000: mov r5,r0
  000aa002: ldr.w r2,[r8,#0x0]
  000aa006: movw r1,#0x4973
  000aa00a: movs r3,#0x0
  000aa00c: blx 0x0007207c
  000aa010: movs r0,#0xc0
  000aa012: blx 0x0006eb24
  000aa016: mov r6,r0
  000aa018: ldr.w r2,[r8,#0x0]
  000aa01c: movw r1,#0x4972
  000aa020: movs r3,#0x0
  000aa022: blx 0x0007207c
  000aa026: ldr r1,[r5,#0xc]
  000aa028: ldr r0,[r4,#0x34]
  000aa02a: blx 0x000720e8
  000aa02e: mov r0,r5
  000aa030: blx 0x00071fc8
  000aa034: blx 0x0006eb48
  000aa038: ldr r1,[r6,#0xc]
  000aa03a: ldr r0,[r4,#0x34]
  000aa03c: blx 0x000720e8
  000aa040: mov r0,r6
  000aa042: blx 0x00071fc8
  000aa046: blx 0x0006eb48
  000aa04a: mov r0,r4
  000aa04c: vpop {d8,d9}
  000aa050: add sp,#0x4
  000aa052: pop.w {r8,r9,r10,r11}
  000aa056: pop {r4,r5,r6,r7,pc}
```
