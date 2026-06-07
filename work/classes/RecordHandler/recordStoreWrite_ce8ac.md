# RecordHandler::recordStoreWrite
elf 0xce8ac body 3808
Sig: undefined __thiscall recordStoreWrite(RecordHandler * this, int param_1)

## decompile
```c

/* RecordHandler::recordStoreWrite(int) */

void __thiscall RecordHandler::recordStoreWrite(RecordHandler *this,int param_1)

{
  bool bVar1;
  int iVar2;
  AEFile *this_00;
  Mission *pMVar3;
  Ship *pSVar4;
  Station *this_01;
  uint *puVar5;
  Standing *this_02;
  int iVar6;
  Item *pIVar7;
  uint *puVar8;
  RecordHandler *this_03;
  uint *puVar9;
  undefined4 extraout_r3;
  undefined4 extraout_r3_00;
  undefined4 extraout_r3_01;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int *piVar13;
  int *piVar14;
  int iVar15;
  int *piVar16;
  float fVar17;
  uint in_stack_ffffffa8;
  uint local_40 [3];
  String aSStack_34 [12];
  int local_28;
  
  piVar16 = *(int **)(DAT_000dec58 + 0xde8c2);
  local_28 = *piVar16;
  AbyssEngine::String::String((String *)local_40,param_1);
  AbyssEngine::operator+(aSStack_34,this + 0x14);
  AbyssEngine::String::~String((String *)local_40);
  iVar2 = AEFile::FileExist(aSStack_34);
  if (iVar2 != 0) {
    AEFile::FileDelete(aSStack_34);
  }
  AEFile::OpenWrite(aSStack_34,local_40);
  iVar2 = Galaxy::getVisited((Galaxy *)**(undefined4 **)(DAT_000dec5c + 0xde8fe));
  AEFile::Write(0x87,local_40[0]);
  for (uVar10 = 0; uVar10 < 0x87; uVar10 = uVar10 + 1) {
    AEFile::Write(*(bool *)(iVar2 + uVar10),local_40[0]);
  }
  piVar13 = *(int **)(DAT_000dec60 + 0xde926);
  iVar2 = Status::getCredits();
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getRating((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  this_00 = (AEFile *)Status::getPlayingTime();
  AEFile::Write(this_00,CONCAT44(extraout_r3,local_40[0]),in_stack_ffffffa8);
  iVar2 = Status::getKills((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getMissionCount((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getLevel();
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getLastXP((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getGoodsProduced((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getStationsVisited((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getCurrentCampaignMission();
  AEFile::Write(iVar2,local_40[0]);
  pMVar3 = (Mission *)Status::getFreelanceMission();
  writeMission(this,pMVar3,local_40[0]);
  pMVar3 = (Mission *)Status::getCampaignMission();
  writeMission(this,pMVar3,local_40[0]);
  iVar2 = Status::getJumpgateUsed((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getCapturedCrates((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getBoughtEquipment((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  iVar2 = Status::getPirateKills((Status *)*piVar13);
  AEFile::Write(iVar2,local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x80),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x7c),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x84),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x88),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0x94),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar9 = *(uint **)(*piVar13 + 0x94);
    if (*puVar9 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar9[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x98),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar9 = *(uint **)(*piVar13 + 0x98);
    if (*puVar9 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar9[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(*(int *)(*piVar13 + 0x9c),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xa0),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xa4),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xa8),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0xac),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar9 = *(uint **)(*piVar13 + 0xac);
    if (*puVar9 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar9[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(*(int *)(*piVar13 + 0xb0),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0xb4),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar9 = *(uint **)(*piVar13 + 0xb4);
    if (*puVar9 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar9[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(*(int *)(*piVar13 + 0xb8),local_40[0]);
  AEFile::Write(*(AEFile **)(*piVar13 + 0xc0),CONCAT44(extraout_r3_00,local_40[0]),in_stack_ffffffa8
               );
  AEFile::Write(*(int *)(*piVar13 + 200),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xcc),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xd0),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xd4),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xd8),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xdc),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xe0),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xe4),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xe8),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0xec),local_40[0]);
  iVar2 = Achievements::getMedals((Achievements *)**(undefined4 **)(DAT_000dec64 + 0xdeb8e));
  AEFile::Write(0x2d,local_40[0]);
  for (uVar10 = 0; uVar10 < 0x2d; uVar10 = uVar10 + 1) {
    AEFile::Write(*(int *)(iVar2 + uVar10 * 4),local_40[0]);
  }
  Status::getShip();
  iVar2 = Ship::getIndex();
  AEFile::Write(iVar2,local_40[0]);
  pSVar4 = (Ship *)Status::getShip();
  iVar2 = Ship::getRace(pSVar4);
  AEFile::Write(iVar2,local_40[0]);
  pSVar4 = (Ship *)Status::getShip();
  puVar9 = (uint *)Ship::getEquipment(pSVar4);
  if (puVar9 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar9,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar9; uVar10 = uVar10 + 1) {
      pIVar7 = *(Item **)(puVar9[1] + uVar10 * 4);
      if (pIVar7 == (Item *)0x0) {
        AEFile::Write(-1,local_40[0]);
      }
      else {
        iVar2 = Item::getIndex(pIVar7);
        AEFile::Write(iVar2,local_40[0]);
        iVar2 = Item::getAmount();
        AEFile::Write(iVar2,local_40[0]);
        bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar9[1] + uVar10 * 4));
        AEFile::Write(bVar1,local_40[0]);
      }
    }
  }
  Status::getShip();
  puVar9 = (uint *)Ship::getCargo();
  if (puVar9 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar9,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar9; uVar10 = uVar10 + 1) {
      iVar2 = Item::getIndex(*(Item **)(puVar9[1] + uVar10 * 4));
      AEFile::Write(iVar2,local_40[0]);
      iVar2 = Item::getAmount();
      AEFile::Write(iVar2,local_40[0]);
      iVar2 = Item::getSinglePrice(*(Item **)(puVar9[1] + uVar10 * 4));
      AEFile::Write(iVar2,local_40[0]);
      bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar9[1] + uVar10 * 4));
      AEFile::Write(bVar1,local_40[0]);
    }
  }
  puVar9 = (uint *)Status::getStationStack((Status *)*piVar13);
  AEFile::Write(3,local_40[0]);
  for (uVar10 = 0; uVar10 < *puVar9 + 1; uVar10 = uVar10 + 1) {
    if (uVar10 == *puVar9) {
      this_01 = (Station *)Status::getStation();
    }
    else {
      this_01 = *(Station **)(puVar9[1] + uVar10 * 4);
    }
    if (this_01 == (Station *)0x0) {
      AEFile::Write(-1,local_40[0]);
    }
    else {
      iVar2 = Station::getIndex(this_01);
      AEFile::Write(iVar2,local_40[0]);
      puVar5 = (uint *)Station::getItems();
      if (puVar5 == (uint *)0x0) {
        AEFile::Write(0,local_40[0]);
      }
      else {
        AEFile::Write(*puVar5,local_40[0]);
        for (uVar11 = 0; uVar11 < *puVar5; uVar11 = uVar11 + 1) {
          iVar2 = Item::getIndex(*(Item **)(puVar5[1] + uVar11 * 4));
          AEFile::Write(iVar2,local_40[0]);
          iVar2 = Item::getAmount();
          AEFile::Write(iVar2,local_40[0]);
          iVar2 = Item::getSinglePrice(*(Item **)(puVar5[1] + uVar11 * 4));
          AEFile::Write(iVar2,local_40[0]);
          bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar5[1] + uVar11 * 4));
          AEFile::Write(bVar1,local_40[0]);
        }
      }
      puVar5 = (uint *)Station::getShips();
      if (puVar5 == (uint *)0x0) {
        AEFile::Write(0,local_40[0]);
      }
      else {
        AEFile::Write(*puVar5,local_40[0]);
        for (uVar11 = 0; uVar11 < *puVar5; uVar11 = uVar11 + 1) {
          iVar2 = Ship::getIndex();
          AEFile::Write(iVar2,local_40[0]);
          iVar2 = Ship::getRace(*(Ship **)(puVar5[1] + uVar11 * 4));
          AEFile::Write(iVar2,local_40[0]);
        }
      }
      puVar5 = (uint *)Station::getAgents(this_01);
      if (puVar5 == (uint *)0x0) {
        AEFile::Write(0,local_40[0]);
      }
      else {
        AEFile::Write(*puVar5,local_40[0]);
        for (uVar11 = 0; uVar11 < *puVar5; uVar11 = uVar11 + 1) {
          writeAgent(this,*(Agent **)(puVar5[1] + uVar11 * 4),local_40[0]);
        }
      }
      bVar1 = (bool)Station::hasAttackedFriends();
      AEFile::Write(bVar1,local_40[0]);
    }
  }
  this_02 = (Standing *)Status::getStanding();
  iVar2 = Standing::getStandings(this_02);
  AEFile::Write(2,local_40[0]);
  for (uVar10 = 0; uVar10 < 2; uVar10 = uVar10 + 1) {
    AEFile::Write(*(int *)(iVar2 + uVar10 * 4),local_40[0]);
  }
  puVar5 = (uint *)Status::getBluePrints((Status *)*piVar13);
  AEFile::Write(*puVar5,local_40[0]);
  for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
    piVar14 = *(int **)(puVar5[1] + uVar10 * 4);
    for (uVar11 = 0; uVar11 < *(uint *)*piVar14; uVar11 = uVar11 + 1) {
      AEFile::Write(*(int *)(((uint *)*piVar14)[1] + uVar11 * 4),local_40[0]);
    }
    AEFile::Write(piVar14[1],local_40[0]);
    AEFile::Write(SUB41(piVar14[2],0),local_40[0]);
    AEFile::Write(piVar14[3],local_40[0]);
    AEFile::Write(piVar14[4],local_40[0]);
    AEFile::Write((String *)(piVar14 + 5),local_40[0],true);
  }
  puVar5 = (uint *)Status::getPendingProducts((Status *)*piVar13);
  if (puVar5 == (uint *)0x0) {
    AEFile::Write(-1,local_40[0]);
  }
  else {
    iVar2 = 0;
    uVar10 = 0;
    while (*puVar5 != uVar10) {
      iVar15 = uVar10 * 4;
      uVar10 = uVar10 + 1;
      if (*(int *)(puVar5[1] + iVar15) != 0) {
        iVar2 = iVar2 + 1;
      }
    }
    if (iVar2 == 0) {
      AEFile::Write(-1,local_40[0]);
    }
    else {
      AEFile::Write(iVar2,local_40[0]);
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        iVar2 = *(int *)(puVar5[1] + uVar10 * 4);
        if (iVar2 != 0) {
          AEFile::Write(*(int *)(iVar2 + 0x14),local_40[0]);
          AEFile::Write(*(int *)(*(int *)(puVar5[1] + uVar10 * 4) + 0x10),local_40[0]);
          AEFile::Write(*(int *)(*(int *)(puVar5[1] + uVar10 * 4) + 0xc),local_40[0]);
          AEFile::Write(*(String **)(puVar5[1] + uVar10 * 4),local_40[0],true);
        }
      }
    }
  }
  if (*(int **)(*piVar13 + 0x24) == (int *)0x0) {
    AEFile::Write(-1,local_40[0]);
  }
  else {
    AEFile::Write(**(int **)(*piVar13 + 0x24),local_40[0]);
    uVar10 = 0;
    while( true ) {
      puVar5 = *(uint **)(*piVar13 + 0x24);
      if (*puVar5 <= uVar10) break;
      AEFile::Write(*(String **)(puVar5[1] + uVar10 * 4),local_40[0],true);
      uVar10 = uVar10 + 1;
    }
    AEFile::Write(*(int *)(*piVar13 + 0x2c),local_40[0]);
    AEFile::Write(*(int *)(*piVar13 + 0x30),local_40[0]);
    AEFile::Write(5,local_40[0]);
    for (uVar10 = 0; uVar10 < 5; uVar10 = uVar10 + 1) {
      AEFile::Write(*(int *)(*(int *)(*piVar13 + 0x28) + uVar10 * 4),local_40[0]);
    }
  }
  AEFile::Write(*(int *)(*piVar13 + 0x34),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0x38),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x38);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar5[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x40),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x40);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(int *)(puVar5[1] + uVar10 * 4),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x3c),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x3c);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(int *)(puVar5[1] + uVar10 * 4),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x48),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x48);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(int *)(puVar5[1] + uVar10 * 4),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x44),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x44);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(int *)(puVar5[1] + uVar10 * 4),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(**(int **)(*piVar13 + 0x4c),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)((Status *)*piVar13 + 0x4c);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(bool *)(puVar5[1] + uVar10),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  puVar5 = (uint *)Status::getAgents((Status *)*piVar13);
  AEFile::Write(*puVar5,local_40[0]);
  for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
    writeAgent(this,*(Agent **)(puVar5[1] + uVar10 * 4),local_40[0]);
  }
  iVar15 = *(int *)(DAT_000df45c + 0xdf0ac);
  AEFile::Write(*(bool *)(iVar15 + 8),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 9),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 10),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0xb),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0xc),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0xd),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0xe),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0xf),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x10),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x11),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x12),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x13),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x14),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x15),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x16),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x17),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x18),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x19),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1a),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1b),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1c),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1d),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1e),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x1f),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x20),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x22),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x21),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x23),local_40[0]);
  fVar17 = (float)AEFile::Write(*(bool *)(iVar15 + 0x24),local_40[0]);
  iVar2 = *(int *)(DAT_000df460 + 0xdf1d6);
  AEFile::Write(fVar17,*(uint *)(iVar2 + 0x2c));
  AEFile::Write(*(AEFile **)(*piVar13 + 0x100),CONCAT44(extraout_r3_01,local_40[0]),
                in_stack_ffffffa8);
  AEFile::Write(*(bool *)(iVar15 + 0x25),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x26),local_40[0]);
  if (*(int *)(*piVar13 + 0x8c) == 0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(1,local_40[0]);
    iVar6 = Ship::getIndex();
    AEFile::Write(iVar6,local_40[0]);
    iVar6 = Ship::getRace(*(Ship **)(*piVar13 + 0x8c));
    AEFile::Write(iVar6,local_40[0]);
    puVar5 = (uint *)Ship::getEquipment(*(Ship **)(*piVar13 + 0x8c));
    if (puVar5 == (uint *)0x0) {
      AEFile::Write(0,local_40[0]);
    }
    else {
      AEFile::Write(*puVar5,local_40[0]);
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        pIVar7 = *(Item **)(puVar5[1] + uVar10 * 4);
        if (pIVar7 == (Item *)0x0) {
          AEFile::Write(-1,local_40[0]);
        }
        else {
          iVar6 = Item::getIndex(pIVar7);
          AEFile::Write(iVar6,local_40[0]);
          iVar6 = Item::getAmount();
          AEFile::Write(iVar6,local_40[0]);
          bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar5[1] + uVar10 * 4));
          AEFile::Write(bVar1,local_40[0]);
        }
      }
    }
    puVar5 = (uint *)Ship::getCargo();
    if (puVar5 == (uint *)0x0) {
      AEFile::Write(0,local_40[0]);
    }
    else {
      AEFile::Write(*puVar5,local_40[0]);
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        iVar6 = Item::getIndex(*(Item **)(puVar5[1] + uVar10 * 4));
        AEFile::Write(iVar6,local_40[0]);
        iVar6 = Item::getAmount();
        AEFile::Write(iVar6,local_40[0]);
        iVar6 = Item::getSinglePrice(*(Item **)(puVar5[1] + uVar10 * 4));
        AEFile::Write(iVar6,local_40[0]);
        bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar5[1] + uVar10 * 4));
        AEFile::Write(bVar1,local_40[0]);
      }
    }
  }
  AEFile::Write(**(int **)(*piVar13 + 0x90),local_40[0]);
  uVar10 = 0;
  while( true ) {
    puVar5 = *(uint **)(*piVar13 + 0x90);
    if (*puVar5 <= uVar10) break;
    AEFile::Write(*(int *)(puVar5[1] + uVar10 * 4),local_40[0]);
    uVar10 = uVar10 + 1;
  }
  AEFile::Write(*(int *)(*piVar13 + 0x10c),local_40[0]);
  AEFile::Write(*(bool *)(*piVar13 + 0x110),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x114),local_40[0]);
  AEFile::Write(*(bool *)(*piVar13 + 0x111),local_40[0]);
  puVar5 = (uint *)Station::getItems();
  if (puVar5 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar5,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
      iVar6 = Item::getIndex(*(Item **)(puVar5[1] + uVar10 * 4));
      AEFile::Write(iVar6,local_40[0]);
      iVar6 = Item::getAmount();
      AEFile::Write(iVar6,local_40[0]);
      iVar6 = Item::getSinglePrice(*(Item **)(puVar5[1] + uVar10 * 4));
      AEFile::Write(iVar6,local_40[0]);
      bVar1 = (bool)Item::isUnsaleable(*(Item **)(puVar5[1] + uVar10 * 4));
      AEFile::Write(bVar1,local_40[0]);
    }
  }
  puVar5 = (uint *)Station::getShips();
  if (puVar5 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar5,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
      iVar6 = Ship::getIndex();
      AEFile::Write(iVar6,local_40[0]);
      iVar6 = Ship::getRace(*(Ship **)(puVar5[1] + uVar10 * 4));
      AEFile::Write(iVar6,local_40[0]);
    }
  }
  AEFile::Write(*(bool *)(iVar15 + 0x27),local_40[0]);
  AEFile::Write(*(bool *)(iVar2 + 0x35),local_40[0]);
  AEFile::Write(*(bool *)(iVar2 + 0x36),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0x54),local_40[0]);
  for (uVar10 = 0; uVar10 < **(uint **)(*piVar13 + 0x54); uVar10 = uVar10 + 1) {
    AEFile::Write(*(bool *)((*(uint **)(*piVar13 + 0x54))[1] + uVar10),local_40[0]);
  }
  AEFile::Write(DAT_000df7e4,local_40[0]);
  pSVar4 = (Ship *)Status::getShip();
  puVar8 = (uint *)Ship::getMods(pSVar4);
  if (puVar8 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar8,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar8; uVar10 = uVar10 + 1) {
      AEFile::Write(*(int *)(puVar8[1] + uVar10 * 4),local_40[0]);
    }
  }
  if (*(Ship **)(*piVar13 + 0x8c) == (Ship *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    puVar8 = (uint *)Ship::getMods(*(Ship **)(*piVar13 + 0x8c));
    if (puVar8 == (uint *)0x0) {
      AEFile::Write(0,local_40[0]);
    }
    else {
      AEFile::Write(*puVar8,local_40[0]);
      for (uVar10 = 0; uVar10 < *puVar8; uVar10 = uVar10 + 1) {
        AEFile::Write(*(int *)(puVar8[1] + uVar10 * 4),local_40[0]);
      }
    }
  }
  if (puVar5 == (uint *)0x0) {
    AEFile::Write(0,local_40[0]);
  }
  else {
    AEFile::Write(*puVar5,local_40[0]);
    for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
      puVar8 = (uint *)Ship::getMods(*(Ship **)(puVar5[1] + uVar10 * 4));
      if (puVar8 == (uint *)0x0) {
        AEFile::Write(0,local_40[0]);
      }
      else {
        AEFile::Write(*puVar8,local_40[0]);
        for (uVar11 = 0; uVar11 < *puVar8; uVar11 = uVar11 + 1) {
          AEFile::Write(*(int *)(puVar8[1] + uVar11 * 4),local_40[0]);
        }
      }
    }
  }
  for (uVar10 = 0; uVar10 < *puVar9 + 1; uVar10 = uVar10 + 1) {
    if (uVar10 == *puVar9) {
      iVar6 = Status::getStation();
    }
    else {
      iVar6 = *(int *)(puVar9[1] + uVar10 * 4);
    }
    if (iVar6 != 0) {
      puVar5 = (uint *)Station::getShips();
      if (puVar5 == (uint *)0x0) {
        AEFile::Write(0,local_40[0]);
      }
      else {
        AEFile::Write(*puVar5,local_40[0]);
        for (uVar11 = 0; uVar11 < *puVar5; uVar11 = uVar11 + 1) {
          iVar6 = Ship::getMods(*(Ship **)(puVar5[1] + uVar11 * 4));
          if (iVar6 == 0) {
            AEFile::Write(0,local_40[0]);
          }
          else {
            puVar8 = (uint *)Ship::getMods(*(Ship **)(puVar5[1] + uVar11 * 4));
            AEFile::Write(*puVar8,local_40[0]);
            puVar8 = (uint *)Ship::getMods(*(Ship **)(puVar5[1] + uVar11 * 4));
            for (uVar12 = 0; uVar12 < *puVar8; uVar12 = uVar12 + 1) {
              AEFile::Write(*(int *)(puVar8[1] + uVar12 * 4),local_40[0]);
            }
          }
        }
      }
    }
  }
  AEFile::Write(**(int **)*piVar13,local_40[0]);
  for (uVar10 = 0; uVar10 < **(uint **)*piVar13; uVar10 = uVar10 + 1) {
    this_03 = (RecordHandler *)(*(uint **)*piVar13)[1];
    writeWanted(this_03,*(Wanted **)(this_03 + uVar10 * 4),local_40[0]);
  }
  for (uVar10 = 0; uVar10 < 4; uVar10 = uVar10 + 1) {
    iVar6 = Status::getCollectedBounties((Status *)*piVar13,uVar10);
    AEFile::Write(iVar6,local_40[0]);
  }
  AEFile::Write(*(bool *)(iVar2 + 0x37),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x178),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x28),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x29),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2c),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2a),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2b),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2e),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2f),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x30),local_40[0]);
  AEFile::Write(**(int **)(*piVar13 + 0x58),local_40[0]);
  for (uVar10 = 0; uVar10 < **(uint **)(*piVar13 + 0x58); uVar10 = uVar10 + 1) {
    AEFile::Write(*(bool *)((*(uint **)(*piVar13 + 0x58))[1] + uVar10),local_40[0]);
  }
  AEFile::Write(*(bool *)(iVar15 + 0x31),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x2d),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x32),local_40[0]);
  AEFile::Write(*(int *)(*piVar13 + 0x118),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x33),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x34),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x35),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x36),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x37),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x38),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x39),local_40[0]);
  AEFile::Write(*(bool *)(iVar15 + 0x3a),local_40[0]);
  AEFile::Close(local_40[0]);
  addHash(this,param_1);
  **(int **)(DAT_000df83c + 0xdf77e) = param_1;
  saveOptions(this);
  AbyssEngine::String::~String(aSStack_34);
  iVar2 = *piVar16 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  000de8ac: push {r4,r5,r6,r7,lr}
  000de8ae: add r7,sp,#0xc
  000de8b0: push {r8,r9,r10,r11}
  000de8b4: sub sp,#0x34
  000de8b6: mov r9,r0
  000de8b8: ldr r0,[0x000dec58]
  000de8ba: add r6,sp,#0x18
  000de8bc: mov r11,r1
  000de8be: add r0,pc
  000de8c0: ldr.w r10,[r0,#0x0]
  000de8c4: ldr.w r0,[r10,#0x0]
  000de8c8: str r0,[sp,#0x30]
  000de8ca: mov r0,r6
  000de8cc: blx 0x0006f658
  000de8d0: add.w r1,r9,#0x14
  000de8d4: add r0,sp,#0x24
  000de8d6: mov r2,r6
  000de8d8: blx 0x0006ef98
  000de8dc: add r0,sp,#0x18
  000de8de: blx 0x0006ee30
  000de8e2: add r0,sp,#0x24
  000de8e4: blx 0x0006fff4
  000de8e8: cbz r0,0x000de8f0
  000de8ea: add r0,sp,#0x24
  000de8ec: blx 0x0007495c
  000de8f0: add r0,sp,#0x24
  000de8f2: add r1,sp,#0x18
  000de8f4: blx 0x0006f3dc
  000de8f8: ldr r0,[0x000dec5c]
  000de8fa: add r0,pc
  000de8fc: ldr r0,[r0,#0x0]
  000de8fe: ldr r0,[r0,#0x0]
  000de900: blx 0x00073480
  000de904: ldr r1,[sp,#0x18]
  000de906: mov r6,r0
  000de908: movs r0,#0x87
  000de90a: blx 0x00074a1c
  000de90e: movs r4,#0x0
  000de910: b 0x000de91c
  000de912: ldrb r0,[r6,r4]
  000de914: ldr r1,[sp,#0x18]
  000de916: blx 0x00074a28
  000de91a: adds r4,#0x1
  000de91c: cmp r4,#0x87
  000de91e: bcc 0x000de912
  000de920: ldr r0,[0x000dec60]
  000de922: add r0,pc
  000de924: ldr r5,[r0,#0x0]
  000de926: ldr r0,[r5,#0x0]
  000de928: blx 0x000733d8
  000de92c: ldr r1,[sp,#0x18]
  000de92e: blx 0x00074a1c
  000de932: ldr r0,[r5,#0x0]
  000de934: blx 0x00074a34
  000de938: ldr r1,[sp,#0x18]
  000de93a: blx 0x00074a1c
  000de93e: ldr r0,[r5,#0x0]
  000de940: blx 0x00071710
  000de944: ldr r2,[sp,#0x18]
  000de946: blx 0x00074a40
  000de94a: ldr r0,[r5,#0x0]
  000de94c: blx 0x00074a4c
  000de950: ldr r1,[sp,#0x18]
  000de952: blx 0x00074a1c
  000de956: ldr r0,[r5,#0x0]
  000de958: blx 0x00074a58
  000de95c: ldr r1,[sp,#0x18]
  000de95e: blx 0x00074a1c
  000de962: ldr r0,[r5,#0x0]
  000de964: blx 0x00071c5c
  000de968: ldr r1,[sp,#0x18]
  000de96a: blx 0x00074a1c
  000de96e: ldr r0,[r5,#0x0]
  000de970: blx 0x00074a64
  000de974: ldr r1,[sp,#0x18]
  000de976: blx 0x00074a1c
  000de97a: ldr r0,[r5,#0x0]
  000de97c: blx 0x00074a70
  000de980: ldr r1,[sp,#0x18]
  000de982: blx 0x00074a1c
  000de986: ldr r0,[r5,#0x0]
  000de988: blx 0x00074a7c
  000de98c: ldr r1,[sp,#0x18]
  000de98e: blx 0x00074a1c
  000de992: ldr r0,[r5,#0x0]
  000de994: blx 0x00071770
  000de998: ldr r1,[sp,#0x18]
  000de99a: blx 0x00074a1c
  000de99e: ldr r0,[r5,#0x0]
  000de9a0: blx 0x00073444
  000de9a4: mov r1,r0
  000de9a6: ldr r2,[sp,#0x18]
  000de9a8: mov r0,r9
  000de9aa: blx 0x00074a88
  000de9ae: ldr r0,[r5,#0x0]
  000de9b0: blx 0x0007285c
  000de9b4: mov r1,r0
  000de9b6: ldr r2,[sp,#0x18]
  000de9b8: mov r0,r9
  000de9ba: blx 0x00074a88
  000de9be: ldr r0,[r5,#0x0]
  000de9c0: blx 0x00074a94
  000de9c4: ldr r1,[sp,#0x18]
  000de9c6: blx 0x00074a1c
  000de9ca: ldr r0,[r5,#0x0]
  000de9cc: blx 0x00074aa0
  000de9d0: ldr r1,[sp,#0x18]
  000de9d2: blx 0x00074a1c
  000de9d6: ldr r0,[r5,#0x0]
  000de9d8: blx 0x00074aac
  000de9dc: ldr r1,[sp,#0x18]
  000de9de: blx 0x00074a1c
  000de9e2: ldr r0,[r5,#0x0]
  000de9e4: blx 0x00074ab8
  000de9e8: ldr r1,[sp,#0x18]
  000de9ea: blx 0x00074a1c
  000de9ee: ldr r0,[r5,#0x0]
  000de9f0: ldr r1,[sp,#0x18]
  000de9f2: ldr.w r0,[r0,#0x80]
  000de9f6: blx 0x00074a1c
  000de9fa: ldr r0,[r5,#0x0]
  000de9fc: ldr r1,[sp,#0x18]
  000de9fe: ldr r0,[r0,#0x7c]
  000dea00: blx 0x00074a1c
  000dea04: ldr r0,[r5,#0x0]
  000dea06: ldr r1,[sp,#0x18]
  000dea08: ldr.w r0,[r0,#0x84]
  000dea0c: blx 0x00074a1c
  000dea10: ldr r0,[r5,#0x0]
  000dea12: ldr r1,[sp,#0x18]
  000dea14: ldr.w r0,[r0,#0x88]
  000dea18: blx 0x00074a1c
  000dea1c: ldr r0,[r5,#0x0]
  000dea1e: ldr r1,[sp,#0x18]
  000dea20: ldr.w r0,[r0,#0x94]
  000dea24: ldr r0,[r0,#0x0]
  000dea26: blx 0x00074a1c
  000dea2a: movs r4,#0x0
  000dea2c: b 0x000dea3a
  000dea2e: ldr r0,[r1,#0x4]
  000dea30: ldr r1,[sp,#0x18]
  000dea32: ldrb r0,[r0,r4]
  000dea34: blx 0x00074a28
  000dea38: adds r4,#0x1
  000dea3a: ldr r0,[r5,#0x0]
  000dea3c: ldr.w r1,[r0,#0x94]
  000dea40: ldr r2,[r1,#0x0]
  000dea42: cmp r4,r2
  000dea44: bcc 0x000dea2e
  000dea46: ldr.w r0,[r0,#0x98]
  000dea4a: ldr r1,[sp,#0x18]
  000dea4c: ldr r0,[r0,#0x0]
  000dea4e: blx 0x00074a1c
  000dea52: movs r4,#0x0
  000dea54: b 0x000dea62
  000dea56: ldr r0,[r1,#0x4]
  000dea58: ldr r1,[sp,#0x18]
  000dea5a: ldrb r0,[r0,r4]
  000dea5c: blx 0x00074a28
  000dea60: adds r4,#0x1
  000dea62: ldr r0,[r5,#0x0]
  000dea64: ldr.w r1,[r0,#0x98]
  000dea68: ldr r2,[r1,#0x0]
  000dea6a: cmp r4,r2
  000dea6c: bcc 0x000dea56
  000dea6e: ldr r1,[sp,#0x18]
  000dea70: ldr.w r0,[r0,#0x9c]
  000dea74: blx 0x00074a1c
  000dea78: ldr r0,[r5,#0x0]
  000dea7a: ldr r1,[sp,#0x18]
  000dea7c: ldr.w r0,[r0,#0xa0]
  000dea80: blx 0x00074a1c
  000dea84: ldr r0,[r5,#0x0]
  000dea86: ldr r1,[sp,#0x18]
  000dea88: ldr.w r0,[r0,#0xa4]
  000dea8c: blx 0x00074a1c
  000dea90: ldr r0,[r5,#0x0]
  000dea92: ldr r1,[sp,#0x18]
  000dea94: ldr.w r0,[r0,#0xa8]
  000dea98: blx 0x00074a1c
  000dea9c: ldr r0,[r5,#0x0]
  000dea9e: ldr r1,[sp,#0x18]
  000deaa0: ldr.w r0,[r0,#0xac]
  000deaa4: ldr r0,[r0,#0x0]
  000deaa6: blx 0x00074a1c
  000deaaa: movs r4,#0x0
  000deaac: b 0x000deaba
  000deaae: ldr r0,[r1,#0x4]
  000deab0: ldr r1,[sp,#0x18]
  000deab2: ldrb r0,[r0,r4]
  000deab4: blx 0x00074a28
  000deab8: adds r4,#0x1
  000deaba: ldr r0,[r5,#0x0]
  000deabc: ldr.w r1,[r0,#0xac]
  000deac0: ldr r2,[r1,#0x0]
  000deac2: cmp r4,r2
  000deac4: bcc 0x000deaae
  000deac6: ldr r1,[sp,#0x18]
  000deac8: ldr.w r0,[r0,#0xb0]
  000deacc: blx 0x00074a1c
  000dead0: ldr r0,[r5,#0x0]
  000dead2: ldr r1,[sp,#0x18]
  000dead4: ldr.w r0,[r0,#0xb4]
  000dead8: ldr r0,[r0,#0x0]
  000deada: blx 0x00074a1c
  000deade: movs r4,#0x0
  000deae0: b 0x000deaee
  000deae2: ldr r0,[r1,#0x4]
  000deae4: ldr r1,[sp,#0x18]
  000deae6: ldrb r0,[r0,r4]
  000deae8: blx 0x00074a28
  000deaec: adds r4,#0x1
  000deaee: ldr r0,[r5,#0x0]
  000deaf0: ldr.w r1,[r0,#0xb4]
  000deaf4: ldr r2,[r1,#0x0]
  000deaf6: cmp r4,r2
  000deaf8: bcc 0x000deae2
  000deafa: ldr r1,[sp,#0x18]
  000deafc: ldr.w r0,[r0,#0xb8]
  000deb00: blx 0x00074a1c
  000deb04: ldr r0,[r5,#0x0]
  000deb06: ldr r2,[sp,#0x18]
  000deb08: ldrd r0,r1,[r0,#0xc0]
  000deb0c: blx 0x00074a40
  000deb10: ldr r0,[r5,#0x0]
  000deb12: ldr r1,[sp,#0x18]
  000deb14: ldr.w r0,[r0,#0xc8]
  000deb18: blx 0x00074a1c
  000deb1c: ldr r0,[r5,#0x0]
  000deb1e: ldr r1,[sp,#0x18]
  000deb20: ldr.w r0,[r0,#0xcc]
  000deb24: blx 0x00074a1c
  000deb28: ldr r0,[r5,#0x0]
  000deb2a: ldr r1,[sp,#0x18]
  000deb2c: ldr.w r0,[r0,#0xd0]
  000deb30: blx 0x00074a1c
  000deb34: ldr r0,[r5,#0x0]
  000deb36: ldr r1,[sp,#0x18]
  000deb38: ldr.w r0,[r0,#0xd4]
  000deb3c: blx 0x00074a1c
  000deb40: ldr r0,[r5,#0x0]
  000deb42: ldr r1,[sp,#0x18]
  000deb44: ldr.w r0,[r0,#0xd8]
  000deb48: blx 0x00074a1c
  000deb4c: ldr r0,[r5,#0x0]
  000deb4e: ldr r1,[sp,#0x18]
  000deb50: ldr.w r0,[r0,#0xdc]
  000deb54: blx 0x00074a1c
  000deb58: ldr r0,[r5,#0x0]
  000deb5a: ldr r1,[sp,#0x18]
  000deb5c: ldr.w r0,[r0,#0xe0]
  000deb60: blx 0x00074a1c
  000deb64: ldr r0,[r5,#0x0]
  000deb66: ldr r1,[sp,#0x18]
  000deb68: ldr.w r0,[r0,#0xe4]
  000deb6c: blx 0x00074a1c
  000deb70: ldr r0,[r5,#0x0]
  000deb72: ldr r1,[sp,#0x18]
  000deb74: ldr.w r0,[r0,#0xe8]
  000deb78: blx 0x00074a1c
  000deb7c: ldr r0,[r5,#0x0]
  000deb7e: ldr r1,[sp,#0x18]
  000deb80: ldr.w r0,[r0,#0xec]
  000deb84: blx 0x00074a1c
  000deb88: ldr r0,[0x000dec64]
  000deb8a: add r0,pc
  000deb8c: ldr r0,[r0,#0x0]
  000deb8e: ldr r0,[r0,#0x0]
  000deb90: blx 0x00074ac4
  000deb94: ldr r1,[sp,#0x18]
  000deb96: mov r6,r0
  000deb98: movs r0,#0x2d
  000deb9a: blx 0x00074a1c
  000deb9e: movs r4,#0x0
  000deba0: b 0x000debae
  000deba2: ldr.w r0,[r6,r4,lsl #0x2]
  000deba6: ldr r1,[sp,#0x18]
  000deba8: blx 0x00074a1c
  000debac: adds r4,#0x1
  000debae: cmp r4,#0x2d
  000debb0: bcc 0x000deba2
  000debb2: ldr r0,[r5,#0x0]
  000debb4: blx 0x00071a58
  000debb8: blx 0x000719c8
  000debbc: ldr r1,[sp,#0x18]
  000debbe: blx 0x00074a1c
  000debc2: ldr r0,[r5,#0x0]
  000debc4: blx 0x00071a58
  000debc8: blx 0x00073db0
  000debcc: ldr r1,[sp,#0x18]
  000debce: blx 0x00074a1c
  000debd2: ldr r0,[r5,#0x0]
  000debd4: blx 0x00071a58
  000debd8: blx 0x0007333c
  000debdc: mov r6,r0
  000debde: cbz r0,0x000dec34
  000debe0: ldr r1,[sp,#0x18]
  000debe2: ldr r0,[r6,#0x0]
  000debe4: blx 0x00074a1c
  000debe8: movs r4,#0x0
  000debea: b 0x000dec2c
  000debec: ldr r0,[r6,#0x4]
  000debee: ldr.w r0,[r0,r4,lsl #0x2]
  000debf2: cbz r0,0x000dec20
  000debf4: blx 0x000718d8
  000debf8: ldr r1,[sp,#0x18]
  000debfa: blx 0x00074a1c
  000debfe: ldr r0,[r6,#0x4]
  000dec00: ldr.w r0,[r0,r4,lsl #0x2]
  000dec04: blx 0x0007183c
  000dec08: ldr r1,[sp,#0x18]
  000dec0a: blx 0x00074a1c
  000dec0e: ldr r0,[r6,#0x4]
  000dec10: ldr.w r0,[r0,r4,lsl #0x2]
  000dec14: blx 0x00074ad0
  000dec18: ldr r1,[sp,#0x18]
  000dec1a: blx 0x00074a28
  000dec1e: b 0x000dec2a
  000dec20: ldr r1,[sp,#0x18]
  000dec22: mov.w r0,#0xffffffff
  000dec26: blx 0x00074a1c
  000dec2a: adds r4,#0x1
  000dec2c: ldr r0,[r6,#0x0]
  000dec2e: cmp r4,r0
  000dec30: bcc 0x000debec
  000dec32: b 0x000dec3c
  000dec34: ldr r1,[sp,#0x18]
  000dec36: movs r0,#0x0
  000dec38: blx 0x00074a1c
  000dec3c: ldr r0,[r5,#0x0]
  000dec3e: blx 0x00071a58
  000dec42: blx 0x00073750
  000dec46: mov r6,r0
  000dec48: cbz r0,0x000decb2
  000dec4a: ldr r1,[sp,#0x18]
  000dec4c: ldr r0,[r6,#0x0]
  000dec4e: blx 0x00074a1c
  000dec52: movs r4,#0x0
  000dec54: b 0x000decaa
  000dec68: ldr r0,[r6,#0x4]
  000dec6a: ldr.w r0,[r0,r4,lsl #0x2]
  000dec6e: blx 0x000718d8
  000dec72: ldr r1,[sp,#0x18]
  000dec74: blx 0x00074a1c
  000dec78: ldr r0,[r6,#0x4]
  000dec7a: ldr.w r0,[r0,r4,lsl #0x2]
  000dec7e: blx 0x0007183c
  000dec82: ldr r1,[sp,#0x18]
  000dec84: blx 0x00074a1c
  000dec88: ldr r0,[r6,#0x4]
  000dec8a: ldr.w r0,[r0,r4,lsl #0x2]
  000dec8e: blx 0x00071944
  000dec92: ldr r1,[sp,#0x18]
  000dec94: blx 0x00074a1c
  000dec98: ldr r0,[r6,#0x4]
  000dec9a: ldr.w r0,[r0,r4,lsl #0x2]
  000dec9e: blx 0x00074ad0
  000deca2: ldr r1,[sp,#0x18]
  000deca4: blx 0x00074a28
  000deca8: adds r4,#0x1
  000decaa: ldr r0,[r6,#0x0]
  000decac: cmp r4,r0
  000decae: bcc 0x000dec68
  000decb0: b 0x000decba
  000decb2: ldr r1,[sp,#0x18]
  000decb4: movs r0,#0x0
  000decb6: blx 0x00074a1c
  000decba: ldr r0,[r5,#0x0]
  000decbc: blx 0x00073c0c
  000decc0: ldr r1,[sp,#0x18]
  000decc2: mov r6,r0
  000decc4: movs r0,#0x3
  000decc6: blx 0x00074a1c
  000decca: movs r2,#0x0
  000deccc: str r6,[sp,#0x10]
  000decce: ldr r0,[r6,#0x0]
  000decd0: adds r1,r0,#0x1
  000decd2: cmp r2,r1
  000decd4: bcs.w 0x000dee06
  000decd8: cmp r2,r0
  000decda: str r2,[sp,#0x14]
  000decdc: bne 0x000dece8
  000decde: ldr r0,[r5,#0x0]
  000dece0: blx 0x00071c14
  000dece4: mov r8,r0
  000dece6: b 0x000decee
  000dece8: ldr r0,[r6,#0x4]
  000decea: ldr.w r8,[r0,r2,lsl #0x2]
  000decee: cmp.w r8,#0x0
  000decf2: beq 0x000ded60
  000decf4: mov r0,r8
  000decf6: blx 0x00071824
  000decfa: ldr r1,[sp,#0x18]
  000decfc: blx 0x00074a1c
  000ded00: mov r0,r8
  000ded02: blx 0x00071830
  000ded06: mov r6,r0
  000ded08: cbz r0,0x000ded72
  000ded0a: ldr r1,[sp,#0x18]
  000ded0c: ldr r0,[r6,#0x0]
  000ded0e: blx 0x00074a1c
  000ded12: movs r4,#0x0
  000ded14: b 0x000ded58
  000ded16: ldr r0,[r6,#0x4]
  000ded18: ldr.w r0,[r0,r4,lsl #0x2]
  000ded1c: blx 0x000718d8
  000ded20: ldr r1,[sp,#0x18]
  000ded22: blx 0x00074a1c
  000ded26: ldr r0,[r6,#0x4]
  000ded28: ldr.w r0,[r0,r4,lsl #0x2]
  000ded2c: blx 0x0007183c
  000ded30: ldr r1,[sp,#0x18]
  000ded32: blx 0x00074a1c
  000ded36: ldr r0,[r6,#0x4]
  000ded38: ldr.w r0,[r0,r4,lsl #0x2]
  000ded3c: blx 0x00071944
  000ded40: ldr r1,[sp,#0x18]
  000ded42: blx 0x00074a1c
  000ded46: ldr r0,[r6,#0x4]
  000ded48: ldr.w r0,[r0,r4,lsl #0x2]
  000ded4c: blx 0x00074ad0
  000ded50: ldr r1,[sp,#0x18]
  000ded52: blx 0x00074a28
  000ded56: adds r4,#0x1
  000ded58: ldr r0,[r6,#0x0]
  000ded5a: cmp r4,r0
  000ded5c: bcc 0x000ded16
  000ded5e: b 0x000ded7a
  000ded60: ldr r1,[sp,#0x18]
  000ded62: mov.w r0,#0xffffffff
  000ded66: blx 0x00074a1c
  000ded6a: ldr r2,[sp,#0x14]
  000ded6c: ldr r6,[sp,#0x10]
  000ded6e: adds r2,#0x1
  000ded70: b 0x000decce
  000ded72: ldr r1,[sp,#0x18]
  000ded74: movs r0,#0x0
  000ded76: blx 0x00074a1c
  000ded7a: mov r0,r8
  000ded7c: blx 0x00073708
  000ded80: mov r6,r0
  000ded82: cbz r0,0x000dedba
  000ded84: ldr r1,[sp,#0x18]
  000ded86: ldr r0,[r6,#0x0]
  000ded88: blx 0x00074a1c
  000ded8c: movs r4,#0x0
  000ded8e: b 0x000dedb2
  000ded90: ldr r0,[r6,#0x4]
  000ded92: ldr.w r0,[r0,r4,lsl #0x2]
  000ded96: blx 0x000719c8
  000ded9a: ldr r1,[sp,#0x18]
  000ded9c: blx 0x00074a1c
  000deda0: ldr r0,[r6,#0x4]
  000deda2: ldr.w r0,[r0,r4,lsl #0x2]
  000deda6: blx 0x00073db0
  000dedaa: ldr r1,[sp,#0x18]
  000dedac: blx 0x00074a1c
  000dedb0: adds r4,#0x1
  000dedb2: ldr r0,[r6,#0x0]
  000dedb4: cmp r4,r0
  000dedb6: bcc 0x000ded90
  000dedb8: b 0x000dedc2
  000dedba: ldr r1,[sp,#0x18]
  000dedbc: movs r0,#0x0
  000dedbe: blx 0x00074a1c
  000dedc2: mov r0,r8
  000dedc4: blx 0x00074104
  000dedc8: mov r6,r0
  000dedca: cbz r0,0x000dedf0
  000dedcc: ldr r1,[sp,#0x18]
  000dedce: ldr r0,[r6,#0x0]
  000dedd0: blx 0x00074a1c
  000dedd4: movs r4,#0x0
  000dedd6: b 0x000dede8
  000dedd8: ldr r0,[r6,#0x4]
  000dedda: ldr r2,[sp,#0x18]
  000deddc: ldr.w r1,[r0,r4,lsl #0x2]
  000dede0: mov r0,r9
  000dede2: blx 0x00074adc
  000dede6: adds r4,#0x1
  000dede8: ldr r0,[r6,#0x0]
  000dedea: cmp r4,r0
  000dedec: bcc 0x000dedd8
  000dedee: b 0x000dedf8
  000dedf0: ldr r1,[sp,#0x18]
  000dedf2: movs r0,#0x0
  000dedf4: blx 0x00074a1c
  000dedf8: mov r0,r8
  000dedfa: blx 0x00073f24
  000dedfe: ldr r1,[sp,#0x18]
  000dee00: blx 0x00074a28
  000dee04: b 0x000ded6a
  000dee06: ldr r0,[r5,#0x0]
  000dee08: blx 0x00071b84
  000dee0c: blx 0x00074ae8
  000dee10: ldr r1,[sp,#0x18]
  000dee12: mov r8,r0
  000dee14: movs r0,#0x2
  000dee16: blx 0x00074a1c
  000dee1a: movs r4,#0x0
  000dee1c: b 0x000dee2a
  000dee1e: ldr.w r0,[r8,r4,lsl #0x2]
  000dee22: ldr r1,[sp,#0x18]
  000dee24: blx 0x00074a1c
  000dee28: adds r4,#0x1
  000dee2a: cmp r4,#0x2
  000dee2c: bcc 0x000dee1e
  000dee2e: ldr r0,[r5,#0x0]
  000dee30: blx 0x000748a8
  000dee34: mov r6,r0
  000dee36: ldr r1,[sp,#0x18]
  000dee38: ldr r0,[r0,#0x0]
  000dee3a: blx 0x00074a1c
  000dee3e: movs r4,#0x0
  000dee40: str r6,[sp,#0x14]
  000dee42: b 0x000dee98
  000dee44: ldr r0,[r6,#0x4]
  000dee46: mov.w r8,#0x0
  000dee4a: ldr.w r6,[r0,r4,lsl #0x2]
  000dee4e: b 0x000dee60
  000dee50: ldr r0,[r0,#0x4]
  000dee52: ldr r1,[sp,#0x18]
  000dee54: ldr.w r0,[r0,r8,lsl #0x2]
  000dee58: blx 0x00074a1c
  000dee5c: add.w r8,r8,#0x1
  000dee60: ldr r0,[r6,#0x0]
  000dee62: ldr r1,[r0,#0x0]
  000dee64: cmp r8,r1
  000dee66: bcc 0x000dee50
  000dee68: ldr r1,[sp,#0x18]
  000dee6a: ldr r0,[r6,#0x4]
  000dee6c: blx 0x00074a1c
  000dee70: ldr r1,[sp,#0x18]
  000dee72: ldrb r0,[r6,#0x8]
  000dee74: blx 0x00074a28
  000dee78: ldr r1,[sp,#0x18]
  000dee7a: ldr r0,[r6,#0xc]
  000dee7c: blx 0x00074a1c
  000dee80: ldr r1,[sp,#0x18]
  000dee82: ldr r0,[r6,#0x10]
  000dee84: blx 0x00074a1c
  000dee88: ldr r1,[sp,#0x18]
  000dee8a: add.w r0,r6,#0x14
  000dee8e: movs r2,#0x1
  000dee90: blx 0x00074968
  000dee94: adds r4,#0x1
  000dee96: ldr r6,[sp,#0x14]
  000dee98: ldr r0,[r6,#0x0]
  000dee9a: cmp r4,r0
  000dee9c: bcc 0x000dee44
  000dee9e: ldr r0,[r5,#0x0]
  000deea0: blx 0x00074668
  000deea4: mov r6,r0
  000deea6: cbz r0,0x000def12
  000deea8: ldr r1,[r6,#0x0]
  000deeaa: movs r0,#0x0
  000deeac: movs r2,#0x0
  000deeae: b 0x000deebe
  000deeb0: ldr r3,[r6,#0x4]
  000deeb2: ldr.w r3,[r3,r2,lsl #0x2]
  000deeb6: adds r2,#0x1
  000deeb8: cmp r3,#0x0
  000deeba: it ne
  000deebc: add.ne r0,#0x1
  000deebe: cmp r1,r2
  000deec0: bne 0x000deeb0
  000deec2: ldr r1,[sp,#0x18]
  000deec4: cbz r0,0x000def1e
  000deec6: blx 0x00074a1c
  000deeca: movs r4,#0x0
  000deecc: b 0x000def0a
  000deece: ldr r0,[r6,#0x4]
  000deed0: ldr.w r0,[r0,r4,lsl #0x2]
  000deed4: cbz r0,0x000def08
  000deed6: ldr r1,[sp,#0x18]
  000deed8: ldr r0,[r0,#0x14]
  000deeda: blx 0x00074a1c
  000deede: ldr r0,[r6,#0x4]
  000deee0: ldr r1,[sp,#0x18]
  000deee2: ldr.w r0,[r0,r4,lsl #0x2]
  000deee6: ldr r0,[r0,#0x10]
  000deee8: blx 0x00074a1c
  000deeec: ldr r0,[r6,#0x4]
  000deeee: ldr r1,[sp,#0x18]
  000deef0: ldr.w r0,[r0,r4,lsl #0x2]
  000deef4: ldr r0,[r0,#0xc]
  000deef6: blx 0x00074a1c
  000deefa: ldr r0,[r6,#0x4]
  000deefc: ldr r1,[sp,#0x18]
  000deefe: ldr.w r0,[r0,r4,lsl #0x2]
  000def02: movs r2,#0x1
  000def04: blx 0x00074968
  000def08: adds r4,#0x1
  000def0a: ldr r0,[r6,#0x0]
  000def0c: cmp r4,r0
  000def0e: bcc 0x000deece
  000def10: b 0x000def26
  000def12: ldr r1,[sp,#0x18]
  000def14: mov.w r0,#0xffffffff
  000def18: blx 0x00074a1c
  000def1c: b 0x000def26
  000def1e: mov.w r0,#0xffffffff
  000def22: blx 0x00074a1c
  000def26: ldr r0,[r5,#0x0]
  000def28: ldr r0,[r0,#0x24]
  000def2a: cbz r0,0x000def86
  000def2c: ldr r1,[sp,#0x18]
  000def2e: ldr r0,[r0,#0x0]
  000def30: blx 0x00074a1c
  000def34: movs r4,#0x0
  000def36: b 0x000def48
  000def38: ldr r0,[r1,#0x4]
  000def3a: ldr r1,[sp,#0x18]
  000def3c: ldr.w r0,[r0,r4,lsl #0x2]
  000def40: movs r2,#0x1
  000def42: blx 0x00074968
  000def46: adds r4,#0x1
  000def48: ldr r0,[r5,#0x0]
  000def4a: ldr r1,[r0,#0x24]
  000def4c: ldr r2,[r1,#0x0]
  000def4e: cmp r4,r2
  000def50: bcc 0x000def38
  000def52: ldr r1,[sp,#0x18]
  000def54: ldr r0,[r0,#0x2c]
  000def56: blx 0x00074a1c
  000def5a: ldr r0,[r5,#0x0]
  000def5c: ldr r1,[sp,#0x18]
  000def5e: ldr r0,[r0,#0x30]
  000def60: blx 0x00074a1c
  000def64: ldr r1,[sp,#0x18]
  000def66: movs r0,#0x5
  000def68: blx 0x00074a1c
  000def6c: movs r4,#0x0
  000def6e: b 0x000def80
  000def70: ldr r0,[r5,#0x0]
  000def72: ldr r1,[sp,#0x18]
  000def74: ldr r0,[r0,#0x28]
  000def76: ldr.w r0,[r0,r4,lsl #0x2]
  000def7a: blx 0x00074a1c
  000def7e: adds r4,#0x1
  000def80: cmp r4,#0x4
  000def82: bls 0x000def70
  000def84: b 0x000def90
  000def86: ldr r1,[sp,#0x18]
  000def88: mov.w r0,#0xffffffff
  000def8c: blx 0x00074a1c
  000def90: ldr r0,[r5,#0x0]
  000def92: ldr r1,[sp,#0x18]
  000def94: ldr r0,[r0,#0x34]
  000def96: blx 0x00074a1c
  000def9a: ldr r0,[r5,#0x0]
  000def9c: ldr r1,[sp,#0x18]
  000def9e: ldr r0,[r0,#0x38]
  000defa0: ldr r0,[r0,#0x0]
  000defa2: blx 0x00074a1c
  000defa6: movs r4,#0x0
  000defa8: b 0x000defb6
  000defaa: ldr r0,[r1,#0x4]
  000defac: ldr r1,[sp,#0x18]
  000defae: ldrb r0,[r0,r4]
  000defb0: blx 0x00074a28
  000defb4: adds r4,#0x1
  000defb6: ldr r0,[r5,#0x0]
  000defb8: ldr r1,[r0,#0x38]
  000defba: ldr r2,[r1,#0x0]
  000defbc: cmp r4,r2
  000defbe: bcc 0x000defaa
  000defc0: ldr r0,[r0,#0x40]
  000defc2: ldr r1,[sp,#0x18]
  000defc4: ldr r0,[r0,#0x0]
  000defc6: blx 0x00074a1c
  000defca: movs r4,#0x0
  000defcc: b 0x000defdc
  000defce: ldr r0,[r1,#0x4]
  000defd0: ldr r1,[sp,#0x18]
  000defd2: ldr.w r0,[r0,r4,lsl #0x2]
  000defd6: blx 0x00074a1c
  000defda: adds r4,#0x1
  000defdc: ldr r0,[r5,#0x0]
  000defde: ldr r1,[r0,#0x40]
  000defe0: ldr r2,[r1,#0x0]
  000defe2: cmp r4,r2
  000defe4: bcc 0x000defce
  000defe6: ldr r0,[r0,#0x3c]
  000defe8: ldr r1,[sp,#0x18]
  000defea: ldr r0,[r0,#0x0]
  000defec: blx 0x00074a1c
  000deff0: movs r4,#0x0
  000deff2: b 0x000df002
  000deff4: ldr r0,[r1,#0x4]
  000deff6: ldr r1,[sp,#0x18]
  000deff8: ldr.w r0,[r0,r4,lsl #0x2]
  000deffc: blx 0x00074a1c
  000df000: adds r4,#0x1
  000df002: ldr r0,[r5,#0x0]
  000df004: ldr r1,[r0,#0x3c]
  000df006: ldr r2,[r1,#0x0]
  000df008: cmp r4,r2
  000df00a: bcc 0x000deff4
  000df00c: ldr r0,[r0,#0x48]
  000df00e: ldr r1,[sp,#0x18]
  000df010: ldr r0,[r0,#0x0]
  000df012: blx 0x00074a1c
  000df016: movs r4,#0x0
  000df018: b 0x000df028
  000df01a: ldr r0,[r1,#0x4]
  000df01c: ldr r1,[sp,#0x18]
  000df01e: ldr.w r0,[r0,r4,lsl #0x2]
  000df022: blx 0x00074a1c
  000df026: adds r4,#0x1
  000df028: ldr r0,[r5,#0x0]
  000df02a: ldr r1,[r0,#0x48]
  000df02c: ldr r2,[r1,#0x0]
  000df02e: cmp r4,r2
  000df030: bcc 0x000df01a
  000df032: ldr r0,[r0,#0x44]
  000df034: ldr r1,[sp,#0x18]
  000df036: ldr r0,[r0,#0x0]
  000df038: blx 0x00074a1c
  000df03c: movs r4,#0x0
  000df03e: b 0x000df04e
  000df040: ldr r0,[r1,#0x4]
  000df042: ldr r1,[sp,#0x18]
  000df044: ldr.w r0,[r0,r4,lsl #0x2]
  000df048: blx 0x00074a1c
  000df04c: adds r4,#0x1
  000df04e: ldr r0,[r5,#0x0]
  000df050: ldr r1,[r0,#0x44]
  000df052: ldr r2,[r1,#0x0]
  000df054: cmp r4,r2
  000df056: bcc 0x000df040
  000df058: ldr r0,[r0,#0x4c]
  000df05a: ldr r1,[sp,#0x18]
  000df05c: ldr r0,[r0,#0x0]
  000df05e: blx 0x00074a1c
  000df062: movs r4,#0x0
  000df064: b 0x000df072
  000df066: ldr r0,[r1,#0x4]
  000df068: ldr r1,[sp,#0x18]
  000df06a: ldrb r0,[r0,r4]
  000df06c: blx 0x00074a28
  000df070: adds r4,#0x1
  000df072: ldr r0,[r5,#0x0]
  000df074: ldr r1,[r0,#0x4c]
  000df076: ldr r2,[r1,#0x0]
  000df078: cmp r4,r2
  000df07a: bcc 0x000df066
  000df07c: blx 0x00071ab8
  000df080: mov r6,r0
  000df082: ldr r1,[sp,#0x18]
  000df084: ldr r0,[r0,#0x0]
  000df086: blx 0x00074a1c
  000df08a: movs r4,#0x0
  000df08c: b 0x000df09e
  000df08e: ldr r0,[r6,#0x4]
  000df090: ldr r2,[sp,#0x18]
  000df092: ldr.w r1,[r0,r4,lsl #0x2]
  000df096: mov r0,r9
  000df098: blx 0x00074adc
  000df09c: adds r4,#0x1
  000df09e: ldr r0,[r6,#0x0]
  000df0a0: cmp r4,r0
  000df0a2: bcc 0x000df08e
  000df0a4: ldr r0,[0x000df45c]
  000df0a6: ldr r1,[sp,#0x18]
  000df0a8: add r0,pc
  000df0aa: ldr.w r8,[r0,#0x0]
  000df0ae: ldrb.w r0,[r8,#0x8]
  000df0b2: blx 0x00074a28
  000df0b6: ldr r1,[sp,#0x18]
  000df0b8: ldrb.w r0,[r8,#0x9]
  000df0bc: blx 0x00074a28
  000df0c0: ldr r1,[sp,#0x18]
  000df0c2: ldrb.w r0,[r8,#0xa]
  000df0c6: blx 0x00074a28
  000df0ca: ldr r1,[sp,#0x18]
  000df0cc: ldrb.w r0,[r8,#0xb]
  000df0d0: blx 0x00074a28
  000df0d4: ldr r1,[sp,#0x18]
  000df0d6: ldrb.w r0,[r8,#0xc]
  000df0da: blx 0x00074a28
  000df0de: ldr r1,[sp,#0x18]
  000df0e0: ldrb.w r0,[r8,#0xd]
  000df0e4: blx 0x00074a28
  000df0e8: ldr r1,[sp,#0x18]
  000df0ea: ldrb.w r0,[r8,#0xe]
  000df0ee: blx 0x00074a28
  000df0f2: ldr r1,[sp,#0x18]
  000df0f4: ldrb.w r0,[r8,#0xf]
  000df0f8: blx 0x00074a28
  000df0fc: ldr r1,[sp,#0x18]
  000df0fe: ldrb.w r0,[r8,#0x10]
  000df102: blx 0x00074a28
  000df106: ldr r1,[sp,#0x18]
  000df108: ldrb.w r0,[r8,#0x11]
  000df10c: blx 0x00074a28
  000df110: ldr r1,[sp,#0x18]
  000df112: ldrb.w r0,[r8,#0x12]
  000df116: blx 0x00074a28
  000df11a: ldr r1,[sp,#0x18]
  000df11c: ldrb.w r0,[r8,#0x13]
  000df120: blx 0x00074a28
  000df124: ldr r1,[sp,#0x18]
  000df126: ldrb.w r0,[r8,#0x14]
  000df12a: blx 0x00074a28
  000df12e: ldr r1,[sp,#0x18]
  000df130: ldrb.w r0,[r8,#0x15]
  000df134: blx 0x00074a28
  000df138: ldr r1,[sp,#0x18]
  000df13a: ldrb.w r0,[r8,#0x16]
  000df13e: blx 0x00074a28
  000df142: ldr r1,[sp,#0x18]
  000df144: ldrb.w r0,[r8,#0x17]
  000df148: blx 0x00074a28
  000df14c: ldr r1,[sp,#0x18]
  000df14e: ldrb.w r0,[r8,#0x18]
  000df152: blx 0x00074a28
  000df156: ldr r1,[sp,#0x18]
  000df158: ldrb.w r0,[r8,#0x19]
  000df15c: blx 0x00074a28
  000df160: ldr r1,[sp,#0x18]
  000df162: ldrb.w r0,[r8,#0x1a]
  000df166: blx 0x00074a28
  000df16a: ldr r1,[sp,#0x18]
  000df16c: ldrb.w r0,[r8,#0x1b]
  000df170: blx 0x00074a28
  000df174: ldr r1,[sp,#0x18]
  000df176: ldrb.w r0,[r8,#0x1c]
  000df17a: blx 0x00074a28
  000df17e: ldr r1,[sp,#0x18]
  000df180: ldrb.w r0,[r8,#0x1d]
  000df184: blx 0x00074a28
  000df188: ldr r1,[sp,#0x18]
  000df18a: ldrb.w r0,[r8,#0x1e]
  000df18e: blx 0x00074a28
  000df192: ldr r1,[sp,#0x18]
  000df194: ldrb.w r0,[r8,#0x1f]
  000df198: blx 0x00074a28
  000df19c: ldr r1,[sp,#0x18]
  000df19e: ldrb.w r0,[r8,#0x20]
  000df1a2: blx 0x00074a28
  000df1a6: ldr r1,[sp,#0x18]
  000df1a8: ldrb.w r0,[r8,#0x22]
  000df1ac: blx 0x00074a28
  000df1b0: ldr r1,[sp,#0x18]
  000df1b2: ldrb.w r0,[r8,#0x21]
  000df1b6: blx 0x00074a28
  000df1ba: ldr r1,[sp,#0x18]
  000df1bc: ldrb.w r0,[r8,#0x23]
  000df1c0: blx 0x00074a28
  000df1c4: ldr r1,[sp,#0x18]
  000df1c6: ldrb.w r0,[r8,#0x24]
  000df1ca: blx 0x00074a28
  000df1ce: ldr r0,[0x000df460]
  000df1d0: ldr r1,[sp,#0x18]
  000df1d2: add r0,pc
  000df1d4: ldr r0,[r0,#0x0]
  000df1d6: str r0,[sp,#0xc]
  000df1d8: ldr r0,[r0,#0x2c]
  000df1da: blx 0x00074af4
  000df1de: ldr r0,[r5,#0x0]
  000df1e0: ldr r2,[sp,#0x18]
  000df1e2: ldrd r0,r1,[r0,#0x100]
  000df1e6: blx 0x00074a40
  000df1ea: ldr r1,[sp,#0x18]
  000df1ec: ldrb.w r0,[r8,#0x25]
  000df1f0: blx 0x00074a28
  000df1f4: ldr r1,[sp,#0x18]
  000df1f6: ldrb.w r0,[r8,#0x26]
  000df1fa: blx 0x00074a28
  000df1fe: ldr r0,[r5,#0x0]
  000df200: ldr r1,[sp,#0x18]
  000df202: ldr.w r0,[r0,#0x8c]
  000df206: cmp r0,#0x0
  000df208: beq 0x000df292
  000df20a: movs r0,#0x1
  000df20c: blx 0x00074a1c
  000df210: ldr r0,[r5,#0x0]
  000df212: ldr.w r0,[r0,#0x8c]
  000df216: blx 0x000719c8
  000df21a: ldr r1,[sp,#0x18]
  000df21c: blx 0x00074a1c
  000df220: ldr r0,[r5,#0x0]
  000df222: ldr.w r0,[r0,#0x8c]
  000df226: blx 0x00073db0
  000df22a: ldr r1,[sp,#0x18]
  000df22c: blx 0x00074a1c
  000df230: ldr r0,[r5,#0x0]
  000df232: ldr.w r0,[r0,#0x8c]
  000df236: blx 0x0007333c
  000df23a: mov r6,r0
  000df23c: cbz r0,0x000df29a
  000df23e: ldr r1,[sp,#0x18]
  000df240: ldr r0,[r6,#0x0]
  000df242: blx 0x00074a1c
  000df246: movs r4,#0x0
  000df248: b 0x000df28a
  000df24a: ldr r0,[r6,#0x4]
  000df24c: ldr.w r0,[r0,r4,lsl #0x2]
  000df250: cbz r0,0x000df27e
  000df252: blx 0x000718d8
  000df256: ldr r1,[sp,#0x18]
  000df258: blx 0x00074a1c
  000df25c: ldr r0,[r6,#0x4]
  000df25e: ldr.w r0,[r0,r4,lsl #0x2]
  000df262: blx 0x0007183c
  000df266: ldr r1,[sp,#0x18]
  000df268: blx 0x00074a1c
  000df26c: ldr r0,[r6,#0x4]
  000df26e: ldr.w r0,[r0,r4,lsl #0x2]
  000df272: blx 0x00074ad0
  000df276: ldr r1,[sp,#0x18]
  000df278: blx 0x00074a28
  000df27c: b 0x000df288
  000df27e: ldr r1,[sp,#0x18]
  000df280: mov.w r0,#0xffffffff
  000df284: blx 0x00074a1c
  000df288: adds r4,#0x1
  000df28a: ldr r0,[r6,#0x0]
  000df28c: cmp r4,r0
  000df28e: bcc 0x000df24a
  000df290: b 0x000df2a2
  000df292: movs r0,#0x0
  000df294: blx 0x00074a1c
  000df298: b 0x000df30e
  000df29a: ldr r1,[sp,#0x18]
  000df29c: movs r0,#0x0
  000df29e: blx 0x00074a1c
  000df2a2: ldr r0,[r5,#0x0]
  000df2a4: ldr.w r0,[r0,#0x8c]
  000df2a8: blx 0x00073750
  000df2ac: mov r6,r0
  000df2ae: cbz r0,0x000df306
  000df2b0: ldr r1,[sp,#0x18]
  000df2b2: ldr r0,[r6,#0x0]
  000df2b4: blx 0x00074a1c
  000df2b8: movs r4,#0x0
  000df2ba: b 0x000df2fe
  000df2bc: ldr r0,[r6,#0x4]
  000df2be: ldr.w r0,[r0,r4,lsl #0x2]
  000df2c2: blx 0x000718d8
  000df2c6: ldr r1,[sp,#0x18]
  000df2c8: blx 0x00074a1c
  000df2cc: ldr r0,[r6,#0x4]
  000df2ce: ldr.w r0,[r0,r4,lsl #0x2]
  000df2d2: blx 0x0007183c
  000df2d6: ldr r1,[sp,#0x18]
  000df2d8: blx 0x00074a1c
  000df2dc: ldr r0,[r6,#0x4]
  000df2de: ldr.w r0,[r0,r4,lsl #0x2]
  000df2e2: blx 0x00071944
  000df2e6: ldr r1,[sp,#0x18]
  000df2e8: blx 0x00074a1c
  000df2ec: ldr r0,[r6,#0x4]
  000df2ee: ldr.w r0,[r0,r4,lsl #0x2]
  000df2f2: blx 0x00074ad0
  000df2f6: ldr r1,[sp,#0x18]
  000df2f8: blx 0x00074a28
  000df2fc: adds r4,#0x1
  000df2fe: ldr r0,[r6,#0x0]
  000df300: cmp r4,r0
  000df302: bcc 0x000df2bc
  000df304: b 0x000df30e
  000df306: ldr r1,[sp,#0x18]
  000df308: movs r0,#0x0
  000df30a: blx 0x00074a1c
  000df30e: ldr r0,[r5,#0x0]
  000df310: ldr r1,[sp,#0x18]
  000df312: ldr.w r0,[r0,#0x90]
  000df316: ldr r0,[r0,#0x0]
  000df318: blx 0x00074a1c
  000df31c: movs r4,#0x0
  000df31e: b 0x000df32e
  000df320: ldr r0,[r1,#0x4]
  000df322: ldr r1,[sp,#0x18]
  000df324: ldr.w r0,[r0,r4,lsl #0x2]
  000df328: blx 0x00074a1c
  000df32c: adds r4,#0x1
  000df32e: ldr r0,[r5,#0x0]
  000df330: ldr.w r1,[r0,#0x90]
  000df334: ldr r2,[r1,#0x0]
  000df336: cmp r4,r2
  000df338: bcc 0x000df320
  000df33a: ldr r1,[sp,#0x18]
  000df33c: ldr.w r0,[r0,#0x10c]
  000df340: blx 0x00074a1c
  000df344: ldr r0,[r5,#0x0]
  000df346: ldr r1,[sp,#0x18]
  000df348: ldrb.w r0,[r0,#0x110]
  000df34c: blx 0x00074a28
  000df350: ldr r0,[r5,#0x0]
  000df352: ldr r1,[sp,#0x18]
  000df354: ldr.w r0,[r0,#0x114]
  000df358: blx 0x00074a1c
  000df35c: ldr r0,[r5,#0x0]
  000df35e: ldr r1,[sp,#0x18]
  000df360: ldrb.w r0,[r0,#0x111]
  000df364: blx 0x00074a28
  000df368: ldr r0,[r5,#0x0]
  000df36a: ldr.w r0,[r0,#0x14c]
  000df36e: blx 0x00071830
  000df372: mov r6,r0
  000df374: cbz r0,0x000df3cc
  000df376: ldr r1,[sp,#0x18]
  000df378: ldr r0,[r6,#0x0]
  000df37a: blx 0x00074a1c
  000df37e: movs r4,#0x0
  000df380: b 0x000df3c4
  000df382: ldr r0,[r6,#0x4]
  000df384: ldr.w r0,[r0,r4,lsl #0x2]
  000df388: blx 0x000718d8
  000df38c: ldr r1,[sp,#0x18]
  000df38e: blx 0x00074a1c
  000df392: ldr r0,[r6,#0x4]
  000df394: ldr.w r0,[r0,r4,lsl #0x2]
  000df398: blx 0x0007183c
  000df39c: ldr r1,[sp,#0x18]
  000df39e: blx 0x00074a1c
  000df3a2: ldr r0,[r6,#0x4]
  000df3a4: ldr.w r0,[r0,r4,lsl #0x2]
  000df3a8: blx 0x00071944
  000df3ac: ldr r1,[sp,#0x18]
  000df3ae: blx 0x00074a1c
  000df3b2: ldr r0,[r6,#0x4]
  000df3b4: ldr.w r0,[r0,r4,lsl #0x2]
  000df3b8: blx 0x00074ad0
  000df3bc: ldr r1,[sp,#0x18]
  000df3be: blx 0x00074a28
  000df3c2: adds r4,#0x1
  000df3c4: ldr r0,[r6,#0x0]
  000df3c6: cmp r4,r0
  000df3c8: bcc 0x000df382
  000df3ca: b 0x000df3d4
  000df3cc: ldr r1,[sp,#0x18]
  000df3ce: movs r0,#0x0
  000df3d0: blx 0x00074a1c
  000df3d4: ldr r0,[r5,#0x0]
  000df3d6: ldr.w r0,[r0,#0x14c]
  000df3da: blx 0x00073708
  000df3de: str r0,[sp,#0x14]
  000df3e0: ldr r0,[sp,#0x14]
  000df3e2: cbz r0,0x000df422
  000df3e4: ldrd r0,r1,[sp,#0x14]
  000df3e8: ldr r0,[r0,#0x0]
  000df3ea: blx 0x00074a1c
  000df3ee: movs r4,#0x0
  000df3f0: b 0x000df418
  000df3f2: ldr r0,[sp,#0x14]
  000df3f4: ldr r0,[r0,#0x4]
  000df3f6: ldr.w r0,[r0,r4,lsl #0x2]
  000df3fa: blx 0x000719c8
  000df3fe: ldr r1,[sp,#0x18]
  000df400: blx 0x00074a1c
  000df404: ldr r0,[sp,#0x14]
  000df406: ldr r0,[r0,#0x4]
  000df408: ldr.w r0,[r0,r4,lsl #0x2]
  000df40c: blx 0x00073db0
  000df410: ldr r1,[sp,#0x18]
  000df412: blx 0x00074a1c
  000df416: adds r4,#0x1
  000df418: ldr r0,[sp,#0x14]
  000df41a: ldr r0,[r0,#0x0]
  000df41c: cmp r4,r0
  000df41e: bcc 0x000df3f2
  000df420: b 0x000df42a
  000df422: ldr r1,[sp,#0x18]
  000df424: movs r0,#0x0
  000df426: blx 0x00074a1c
  000df42a: ldr r1,[sp,#0x18]
  000df42c: ldrb.w r0,[r8,#0x27]
  000df430: blx 0x00074a28
  000df434: ldr r0,[sp,#0xc]
  000df436: ldr r1,[sp,#0x18]
  000df438: ldrb.w r0,[r0,#0x35]
  000df43c: blx 0x00074a28
  000df440: ldr r0,[sp,#0xc]
  000df442: ldr r1,[sp,#0x18]
  000df444: ldrb.w r0,[r0,#0x36]
  000df448: blx 0x00074a28
  000df44c: ldr r0,[r5,#0x0]
  000df44e: ldr r1,[sp,#0x18]
  000df450: ldr r0,[r0,#0x54]
  000df452: ldr r0,[r0,#0x0]
  000df454: blx 0x00074a1c
  000df458: movs r4,#0x0
  000df45a: b 0x000df470
  000df464: ldr r0,[r0,#0x4]
  000df466: ldr r1,[sp,#0x18]
  000df468: ldrb r0,[r0,r4]
  000df46a: blx 0x00074a28
  000df46e: adds r4,#0x1
  000df470: ldr r0,[r5,#0x0]
  000df472: ldr r0,[r0,#0x54]
  000df474: ldr r1,[r0,#0x0]
  000df476: cmp r4,r1
  000df478: bcc 0x000df464
  000df47a: ldr r1,[sp,#0x18]
  000df47c: ldr r0,[0x000df7e4]
  000df47e: blx 0x00074a1c
  000df482: ldr r0,[r5,#0x0]
  000df484: blx 0x00071a58
  000df488: blx 0x00074b00
  000df48c: mov r6,r0
  000df48e: cbz r0,0x000df4b2
  000df490: ldr r1,[sp,#0x18]
  000df492: ldr r0,[r6,#0x0]
  000df494: blx 0x00074a1c
  000df498: movs r4,#0x0
  000df49a: b 0x000df4aa
  000df49c: ldr r0,[r6,#0x4]
  000df49e: ldr r1,[sp,#0x18]
  000df4a0: ldr.w r0,[r0,r4,lsl #0x2]
  000df4a4: blx 0x00074a1c
  000df4a8: adds r4,#0x1
  000df4aa: ldr r0,[r6,#0x0]
  000df4ac: cmp r4,r0
  000df4ae: bcc 0x000df49c
  000df4b0: b 0x000df4ba
  000df4b2: ldr r1,[sp,#0x18]
  000df4b4: movs r0,#0x0
  000df4b6: blx 0x00074a1c
  000df4ba: ldr r0,[r5,#0x0]
  000df4bc: ldr.w r0,[r0,#0x8c]
  000df4c0: cbz r0,0x000df4ec
  000df4c2: blx 0x00074b00
  000df4c6: mov r6,r0
  000df4c8: cbz r0,0x000df4f6
  000df4ca: ldr r1,[sp,#0x18]
  000df4cc: ldr r0,[r6,#0x0]
  000df4ce: blx 0x00074a1c
  000df4d2: movs r4,#0x0
  000df4d4: b 0x000df4e4
  000df4d6: ldr r0,[r6,#0x4]
  000df4d8: ldr r1,[sp,#0x18]
  000df4da: ldr.w r0,[r0,r4,lsl #0x2]
  000df4de: blx 0x00074a1c
  000df4e2: adds r4,#0x1
  000df4e4: ldr r0,[r6,#0x0]
  000df4e6: cmp r4,r0
  000df4e8: bcc 0x000df4d6
  000df4ea: b 0x000df4fe
  000df4ec: ldr r1,[sp,#0x18]
  000df4ee: movs r0,#0x0
  000df4f0: blx 0x00074a1c
  000df4f4: b 0x000df4fe
  000df4f6: ldr r1,[sp,#0x18]
  000df4f8: movs r0,#0x0
  000df4fa: blx 0x00074a1c
  000df4fe: ldr r0,[sp,#0x14]
  000df500: cbz r0,0x000df55e
  000df502: ldrd r0,r1,[sp,#0x14]
  000df506: ldr r0,[r0,#0x0]
  000df508: blx 0x00074a1c
  000df50c: movs r0,#0x0
  000df50e: b 0x000df550
  000df510: ldr r0,[sp,#0x14]
  000df512: ldr r1,[sp,#0x8]
  000df514: ldr r0,[r0,#0x4]
  000df516: ldr.w r0,[r0,r1,lsl #0x2]
  000df51a: blx 0x00074b00
  000df51e: mov r6,r0
  000df520: cbz r0,0x000df544
  000df522: ldr r1,[sp,#0x18]
  000df524: ldr r0,[r6,#0x0]
  000df526: blx 0x00074a1c
  000df52a: movs r4,#0x0
  000df52c: b 0x000df53c
  000df52e: ldr r0,[r6,#0x4]
  000df530: ldr r1,[sp,#0x18]
  000df532: ldr.w r0,[r0,r4,lsl #0x2]
  000df536: blx 0x00074a1c
  000df53a: adds r4,#0x1
  000df53c: ldr r0,[r6,#0x0]
  000df53e: cmp r4,r0
  000df540: bcc 0x000df52e
  000df542: b 0x000df54c
  000df544: ldr r1,[sp,#0x18]
  000df546: movs r0,#0x0
  000df548: blx 0x00074a1c
  000df54c: ldr r0,[sp,#0x8]
  000df54e: adds r0,#0x1
  000df550: str r0,[sp,#0x8]
  000df552: ldr r0,[sp,#0x14]
  000df554: ldr r1,[sp,#0x8]
  000df556: ldr r0,[r0,#0x0]
  000df558: cmp r1,r0
  000df55a: bcc 0x000df510
  000df55c: b 0x000df566
  000df55e: ldr r1,[sp,#0x18]
  000df560: movs r0,#0x0
  000df562: blx 0x00074a1c
  000df566: movs r0,#0x0
  000df568: b 0x000df614
  000df56a: ldr r1,[sp,#0x4]
  000df56c: cmp r1,r0
  000df56e: bne 0x000df578
  000df570: ldr r0,[r5,#0x0]
  000df572: blx 0x00071c14
  000df576: b 0x000df582
  000df578: ldr r0,[sp,#0x10]
  000df57a: ldr r1,[sp,#0x4]
  000df57c: ldr r0,[r0,#0x4]
  000df57e: ldr.w r0,[r0,r1,lsl #0x2]
  000df582: cmp r0,#0x0
  000df584: beq 0x000df610
  000df586: blx 0x00073708
  000df58a: str r0,[sp,#0x14]
  000df58c: ldr r0,[sp,#0x14]
  000df58e: cbz r0,0x000df608
  000df590: ldrd r0,r1,[sp,#0x14]
  000df594: ldr r0,[r0,#0x0]
  000df596: blx 0x00074a1c
  000df59a: movs r0,#0x0
  000df59c: b 0x000df5fa
  000df59e: ldr r0,[sp,#0x14]
  000df5a0: ldr r1,[sp,#0x8]
  000df5a2: ldr r0,[r0,#0x4]
  000df5a4: ldr.w r0,[r0,r1,lsl #0x2]
  000df5a8: blx 0x00074b00
  000df5ac: cbz r0,0x000df5ee
  000df5ae: ldr r0,[sp,#0x14]
  000df5b0: ldr r1,[sp,#0x8]
  000df5b2: ldr r0,[r0,#0x4]
  000df5b4: ldr.w r0,[r0,r1,lsl #0x2]
  000df5b8: blx 0x00074b00
  000df5bc: ldr r1,[sp,#0x18]
  000df5be: ldr r0,[r0,#0x0]
  000df5c0: blx 0x00074b0c
  000df5c4: ldr r0,[sp,#0x14]
  000df5c6: ldr r1,[sp,#0x8]
  000df5c8: ldr r0,[r0,#0x4]
  000df5ca: ldr.w r0,[r0,r1,lsl #0x2]
  000df5ce: blx 0x00074b00
  000df5d2: mov r6,r0
  000df5d4: movs r4,#0x0
  000df5d6: b 0x000df5e6
  000df5d8: ldr r0,[r6,#0x4]
  000df5da: ldr r1,[sp,#0x18]
  000df5dc: ldr.w r0,[r0,r4,lsl #0x2]
  000df5e0: blx 0x00074a1c
  000df5e4: adds r4,#0x1
  000df5e6: ldr r0,[r6,#0x0]
  000df5e8: cmp r4,r0
  000df5ea: bcc 0x000df5d8
  000df5ec: b 0x000df5f6
  000df5ee: ldr r1,[sp,#0x18]
  000df5f0: movs r0,#0x0
  000df5f2: blx 0x00074a1c
  000df5f6: ldr r0,[sp,#0x8]
  000df5f8: adds r0,#0x1
  000df5fa: str r0,[sp,#0x8]
  000df5fc: ldr r0,[sp,#0x14]
  000df5fe: ldr r1,[sp,#0x8]
  000df600: ldr r0,[r0,#0x0]
  000df602: cmp r1,r0
  000df604: bcc 0x000df59e
  000df606: b 0x000df610
  000df608: ldr r1,[sp,#0x18]
  000df60a: movs r0,#0x0
  000df60c: blx 0x00074a1c
  000df610: ldr r0,[sp,#0x4]
  000df612: adds r0,#0x1
  000df614: str r0,[sp,#0x4]
  000df616: ldr r0,[sp,#0x10]
  000df618: ldr r2,[sp,#0x4]
  000df61a: ldr r0,[r0,#0x0]
  000df61c: adds r1,r0,#0x1
  000df61e: cmp r2,r1
  000df620: bcc 0x000df56a
  000df622: ldr r0,[r5,#0x0]
  000df624: ldr r1,[sp,#0x18]
  000df626: ldr r0,[r0,#0x0]
  000df628: ldr r0,[r0,#0x0]
  000df62a: blx 0x00074a1c
  000df62e: movs r4,#0x0
  000df630: b 0x000df640
  000df632: ldr r0,[r0,#0x4]
  000df634: ldr r2,[sp,#0x18]
  000df636: ldr.w r1,[r0,r4,lsl #0x2]
  000df63a: blx 0x00074b18
  000df63e: adds r4,#0x1
  000df640: ldr r0,[r5,#0x0]
  000df642: ldr r0,[r0,#0x0]
  000df644: ldr r1,[r0,#0x0]
  000df646: cmp r4,r1
  000df648: bcc 0x000df632
  000df64a: movs r6,#0x0
  000df64c: b 0x000df65e
  000df64e: ldr r0,[r5,#0x0]
  000df650: mov r1,r6
  000df652: blx 0x00074b24
  000df656: ldr r1,[sp,#0x18]
  000df658: blx 0x00074a1c
  000df65c: adds r6,#0x1
  000df65e: cmp r6,#0x4
  000df660: bcc 0x000df64e
  000df662: ldr r0,[sp,#0xc]
  000df664: ldr r1,[sp,#0x18]
  000df666: ldrb.w r0,[r0,#0x37]
  000df66a: blx 0x00074a28
  000df66e: ldr r0,[r5,#0x0]
  000df670: ldr r1,[sp,#0x18]
  000df672: ldr.w r0,[r0,#0x178]
  000df676: blx 0x00074a1c
  000df67a: ldr r1,[sp,#0x18]
  000df67c: ldrb.w r0,[r8,#0x28]
  000df680: blx 0x00074a28
  000df684: ldr r1,[sp,#0x18]
  000df686: ldrb.w r0,[r8,#0x29]
  000df68a: blx 0x00074a28
  000df68e: ldr r1,[sp,#0x18]
  000df690: ldrb.w r0,[r8,#0x2c]
  000df694: blx 0x00074a28
  000df698: ldr r1,[sp,#0x18]
  000df69a: ldrb.w r0,[r8,#0x2a]
  000df69e: blx 0x00074a28
  000df6a2: ldr r1,[sp,#0x18]
  000df6a4: ldrb.w r0,[r8,#0x2b]
  000df6a8: blx 0x00074a28
  000df6ac: ldr r1,[sp,#0x18]
  000df6ae: ldrb.w r0,[r8,#0x2e]
  000df6b2: blx 0x00074a28
  000df6b6: ldr r1,[sp,#0x18]
  000df6b8: ldrb.w r0,[r8,#0x2f]
  000df6bc: blx 0x00074a28
  000df6c0: ldr r1,[sp,#0x18]
  000df6c2: ldrb.w r0,[r8,#0x30]
  000df6c6: blx 0x00074a28
  000df6ca: ldr r0,[r5,#0x0]
  000df6cc: ldr r1,[sp,#0x18]
  000df6ce: ldr r0,[r0,#0x58]
  000df6d0: ldr r0,[r0,#0x0]
  000df6d2: blx 0x00074a1c
  000df6d6: movs r4,#0x0
  000df6d8: b 0x000df6e6
  000df6da: ldr r0,[r0,#0x4]
  000df6dc: ldr r1,[sp,#0x18]
  000df6de: ldrb r0,[r0,r4]
  000df6e0: blx 0x00074a28
  000df6e4: adds r4,#0x1
  000df6e6: ldr r0,[r5,#0x0]
  000df6e8: ldr r0,[r0,#0x58]
  000df6ea: ldr r1,[r0,#0x0]
  000df6ec: cmp r4,r1
  000df6ee: bcc 0x000df6da
  000df6f0: ldr r1,[sp,#0x18]
  000df6f2: ldrb.w r0,[r8,#0x31]
  000df6f6: blx 0x00074a28
  000df6fa: ldr r1,[sp,#0x18]
  000df6fc: ldrb.w r0,[r8,#0x2d]
  000df700: blx 0x00074a28
  000df704: ldr r1,[sp,#0x18]
  000df706: ldrb.w r0,[r8,#0x32]
  000df70a: blx 0x00074a28
  000df70e: ldr r0,[r5,#0x0]
  000df710: ldr r1,[sp,#0x18]
  000df712: ldr.w r0,[r0,#0x118]
  000df716: blx 0x00074a1c
  000df71a: ldr r1,[sp,#0x18]
  000df71c: ldrb.w r0,[r8,#0x33]
  000df720: blx 0x00074a28
  000df724: ldr r1,[sp,#0x18]
  000df726: ldrb.w r0,[r8,#0x34]
  000df72a: blx 0x00074a28
  000df72e: ldr r1,[sp,#0x18]
  000df730: ldrb.w r0,[r8,#0x35]
  000df734: blx 0x00074a28
  000df738: ldr r1,[sp,#0x18]
  000df73a: ldrb.w r0,[r8,#0x36]
  000df73e: blx 0x00074a28
  000df742: ldr r1,[sp,#0x18]
  000df744: ldrb.w r0,[r8,#0x37]
  000df748: blx 0x00074a28
  000df74c: ldr r1,[sp,#0x18]
  000df74e: ldrb.w r0,[r8,#0x38]
  000df752: blx 0x00074a28
  000df756: ldr r1,[sp,#0x18]
  000df758: ldrb.w r0,[r8,#0x39]
  000df75c: blx 0x00074a28
  000df760: ldr r1,[sp,#0x18]
  000df762: ldrb.w r0,[r8,#0x3a]
  000df766: blx 0x00074a28
  000df76a: ldr r0,[sp,#0x18]
  000df76c: blx 0x0006ece0
  000df770: mov r0,r9
  000df772: mov r1,r11
  000df774: blx 0x00074b30
  000df778: ldr r0,[0x000df83c]
  000df77a: add r0,pc
  000df77c: ldr r0,[r0,#0x0]
  000df77e: str.w r11,[r0,#0x0]
  000df782: mov r0,r9
  000df784: blx 0x00071788
  000df788: add r0,sp,#0x24
  000df78a: blx 0x0006ee30
  000df78e: ldr r0,[sp,#0x30]
  000df790: ldr.w r1,[r10,#0x0]
  000df794: subs r0,r1,r0
  000df796: itttt eq
  000df798: mov.eq r0,#0x1
  000df79a: add.eq sp,#0x34
  000df79c: pop.eq.w {r8,r9,r10,r11}
  000df7a0: pop.eq {r4,r5,r6,r7,pc}
  000df7a2: blx 0x0006e824
```
