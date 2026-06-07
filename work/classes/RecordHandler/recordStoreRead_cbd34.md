# RecordHandler::recordStoreRead
elf 0xcbd34 body 5014
Sig: undefined __thiscall recordStoreRead(RecordHandler * this, int param_1)

## decompile
```c

/* RecordHandler::recordStoreRead(int) */

void __thiscall RecordHandler::recordStoreRead(RecordHandler *this,int param_1)

{
  int iVar1;
  uint uVar2;
  GameRecord *this_00;
  undefined4 uVar3;
  Array<bool> *pAVar4;
  void *pvVar5;
  Array<Item*> *pAVar6;
  int *piVar7;
  Array<Station*> *this_01;
  Station *this_02;
  int *piVar8;
  Standing *this_03;
  Array<BluePrint*> *this_04;
  undefined4 *puVar9;
  BluePrint *this_05;
  Array<PendingProduct*> *this_06;
  PendingProduct *pPVar10;
  Array *pAVar11;
  String *this_07;
  Array<Agent*> *pAVar12;
  Array<Ship*> *pAVar13;
  int iVar14;
  Array<Wanted*> *this_08;
  int *piVar15;
  int iVar16;
  RecordHandler *pRVar17;
  int *piVar18;
  uint uVar19;
  GameRecord *pGVar20;
  Item *pIVar21;
  uint local_ac;
  int local_a8;
  uint local_a4;
  int local_a0;
  uint local_9c;
  uint local_98;
  uint local_94;
  uint local_90;
  String aSStack_8c [12];
  int local_80;
  uint local_7c;
  uint local_78;
  uint local_74;
  uint local_70;
  uint local_6c;
  uint local_68;
  uint local_64;
  uint local_60 [8];
  RecordHandler *local_40 [3];
  String aSStack_34 [12];
  int local_28;
  
  piVar18 = *(int **)(DAT_000dc094 + 0xdbd48);
  local_28 = *piVar18;
  AbyssEngine::String::String((String *)local_40,param_1);
  AbyssEngine::operator+(aSStack_34,this + 0x14);
  AbyssEngine::String::~String((String *)local_40);
  iVar1 = AEFile::FileExist(aSStack_34);
  if (iVar1 != 0) {
    uVar2 = AEFile::OpenRead(aSStack_34,local_60 + 7);
    iVar1 = checkHash(uVar2);
    AEFile::Close(local_60[7]);
    if (iVar1 != 0) {
      AEFile::OpenRead(aSStack_34,local_60 + 7);
      this_00 = operator_new(0x1c8);
      GameRecord::GameRecord(this_00);
      pGVar20 = this_00 + 4;
      *(int *)pGVar20 = 0;
      AEFile::Read((int *)pGVar20,local_60[7]);
      for (uVar2 = 0; uVar2 < *(uint *)pGVar20; uVar2 = uVar2 + 1) {
        AEFile::Read((bool *)(*(int *)this_00 + uVar2),local_60[7]);
      }
      AEFile::Read((int *)(this_00 + 8),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xc),local_60[7]);
      AEFile::Read((longlong *)(this_00 + 0x10),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x18),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x1c),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x20),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x24),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x28),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x3c),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x40),local_60[7]);
      uVar3 = readMission(this,local_60[7]);
      *(undefined4 *)(this_00 + 0x54) = uVar3;
      uVar3 = readMission(this,local_60[7]);
      *(undefined4 *)(this_00 + 0x58) = uVar3;
      AEFile::Read((int *)(this_00 + 0x30),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x34),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x38),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x2c),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x44),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x48),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x4c),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x50),local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x68) = pAVar4;
      local_60[6] = 0;
      AEFile::Read((int *)(local_60 + 6),local_60[7]);
      ArraySetLength<bool>(local_60[6],*(Array **)(this_00 + 0x68));
      for (iVar1 = 0; iVar1 < (int)local_60[6]; iVar1 = iVar1 + 1) {
        AEFile::Read((bool *)(*(int *)(*(int *)(this_00 + 0x68) + 4) + iVar1),local_60[7]);
      }
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x6c) = pAVar4;
      local_60[5] = 0;
      AEFile::Read((int *)(local_60 + 5),local_60[7]);
      ArraySetLength<bool>(local_60[5],*(Array **)(this_00 + 0x6c));
      for (iVar1 = 0; iVar1 < (int)local_60[5]; iVar1 = iVar1 + 1) {
        AEFile::Read((bool *)(*(int *)(*(int *)(this_00 + 0x6c) + 4) + iVar1),local_60[7]);
      }
      AEFile::Read((int *)(this_00 + 0x74),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x78),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x7c),local_60[7]);
      AEFile::Read((int *)(this_00 + 0x80),local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x84) = pAVar4;
      local_60[4] = 0;
      AEFile::Read((int *)(local_60 + 4),local_60[7]);
      ArraySetLength<bool>(local_60[4],*(Array **)(this_00 + 0x84));
      for (iVar1 = 0; iVar1 < (int)local_60[4]; iVar1 = iVar1 + 1) {
        AEFile::Read((bool *)(*(int *)(*(int *)(this_00 + 0x84) + 4) + iVar1),local_60[7]);
      }
      AEFile::Read((int *)(this_00 + 0x88),local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x8c) = pAVar4;
      local_60[3] = 0;
      AEFile::Read((int *)(local_60 + 3),local_60[7]);
      ArraySetLength<bool>(local_60[3],*(Array **)(this_00 + 0x8c));
      for (iVar1 = 0; iVar1 < (int)local_60[3]; iVar1 = iVar1 + 1) {
        AEFile::Read((bool *)(*(int *)(*(int *)(this_00 + 0x8c) + 4) + iVar1),local_60[7]);
      }
      AEFile::Read((int *)(this_00 + 0x90),local_60[7]);
      AEFile::Read((longlong *)(this_00 + 0x98),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xa0),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xa4),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xa8),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xac),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xb0),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xb4),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xb8),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xbc),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xc0),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xc4),local_60[7]);
      pGVar20 = this_00 + 100;
      AEFile::Read((int *)pGVar20,local_60[7]);
      uVar19 = *(uint *)pGVar20;
      uVar2 = (uint)((ulonglong)uVar19 * 4);
      if ((int)((ulonglong)uVar19 * 4 >> 0x20) != 0) {
        uVar2 = 0xffffffff;
      }
      pvVar5 = operator_new__(uVar2);
      iVar16 = 0;
      *(void **)(this_00 + 0x60) = pvVar5;
      for (iVar1 = 0; iVar1 < (int)uVar19; iVar1 = iVar1 + 1) {
        AEFile::Read((int *)(*(int *)(this_00 + 0x60) + iVar16),local_60[7]);
        iVar16 = iVar16 + 4;
        uVar19 = *(uint *)pGVar20;
      }
      local_60[2] = 0;
      AEFile::Read((int *)(local_60 + 2),local_60[7]);
      piVar15 = *(int **)(DAT_000dc448 + 0xdc0c4);
      uVar3 = Ship::makeShip(*(int *)(*(int *)(*piVar15 + 4) + local_60[2] * 4));
      *(undefined4 *)(this_00 + 0x130) = uVar3;
      local_60[1] = 0;
      AEFile::Read((int *)(local_60 + 1),local_60[7]);
      Ship::setRace(*(int *)(this_00 + 0x130));
      local_60[0] = 0;
      AEFile::Read((int *)local_60,local_60[7]);
      if (0 < (int)local_60[0]) {
        pAVar6 = operator_new(0xc);
        Array<Item*>::Array(pAVar6);
        ArraySetLength<Item*>(local_60[0],(Array *)pAVar6);
        piVar7 = *(int **)(DAT_000dc44c + 0xdc124);
        for (iVar1 = 0; iVar1 < (int)local_60[0]; iVar1 = iVar1 + 1) {
          local_40[0] = (RecordHandler *)0x0;
          AEFile::Read((int *)local_40,local_60[7]);
          if (local_40[0] == (RecordHandler *)0xffffffff) {
            *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = 0;
          }
          else {
            local_64 = 0;
            AEFile::Read((int *)&local_64,local_60[7]);
            uVar2 = local_64;
            pIVar21 = *(Item **)(*(int *)(*piVar7 + 4) + (int)local_40[0] * 4);
            Item::getMaxPrice(pIVar21);
            uVar3 = Item::makeItem((int)pIVar21,uVar2);
            *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
            local_68 = local_68 & 0xffffff00;
            AEFile::Read((bool *)&local_68,local_60[7]);
            Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
          }
        }
        Ship::replaceEquipment(*(Ship **)(this_00 + 0x130),(Array *)pAVar6);
      }
      local_64 = 0;
      AEFile::Read((int *)&local_64,local_60[7]);
      if (0 < (int)local_64) {
        pAVar6 = operator_new(0xc);
        Array<Item*>::Array(pAVar6);
        ArraySetLength<Item*>(local_64,(Array *)pAVar6);
        piVar7 = *(int **)(DAT_000dc450 + 0xdc1d2);
        for (iVar1 = 0; iVar1 < (int)local_64; iVar1 = iVar1 + 1) {
          local_6c = 0;
          AEFile::Read((int *)local_40,local_60[7]);
          AEFile::Read((int *)&local_68,local_60[7]);
          AEFile::Read((int *)&local_6c,local_60[7]);
          local_70 = local_70 & 0xffffff00;
          AEFile::Read((bool *)&local_70,local_60[7]);
          uVar2 = local_68;
          pIVar21 = *(Item **)(*(int *)(*piVar7 + 4) + (int)local_40[0] * 4);
          Item::getMaxPrice(pIVar21);
          uVar3 = Item::makeItem((int)pIVar21,uVar2);
          *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
          Item::setPrice(*(int *)(*(int *)(pAVar6 + 4) + iVar1 * 4));
          Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
        }
        Ship::replaceCargo(*(Ship **)(this_00 + 0x130),(Array *)pAVar6);
      }
      this_01 = operator_new(0xc);
      Array<Station*>::Array(this_01);
      local_68 = 0;
      AEFile::Read((int *)&local_68,local_60[7]);
      ArraySetLength<Station*>(local_68,(Array *)this_01);
      uVar2 = *(uint *)this_01;
      piVar7 = *(int **)(DAT_000dc454 + 0xdc288);
      for (uVar19 = 0; uVar19 < uVar2 + 1; uVar19 = uVar19 + 1) {
        local_40[0] = (RecordHandler *)0x0;
        AEFile::Read((int *)local_40,local_60[7]);
        if (local_40[0] == (RecordHandler *)0xffffffff) {
          this_02 = (Station *)0x0;
        }
        else {
          this_02 = (Station *)Galaxy::getStation(*piVar7);
          AEFile::Read((int *)&local_64,local_60[7]);
          if (0 < (int)local_64) {
            pAVar6 = operator_new(0xc);
            Array<Item*>::Array(pAVar6);
            ArraySetLength<Item*>(local_64,(Array *)pAVar6);
            for (iVar1 = 0; iVar1 < (int)local_64; iVar1 = iVar1 + 1) {
              local_74 = 0;
              AEFile::Read((int *)&local_6c,local_60[7]);
              AEFile::Read((int *)&local_70,local_60[7]);
              AEFile::Read((int *)&local_74,local_60[7]);
              local_78 = local_78 & 0xffffff00;
              AEFile::Read((bool *)&local_78,local_60[7]);
              uVar2 = local_70;
              pIVar21 = *(Item **)(*(int *)(**(int **)(DAT_000dc458 + 0xdc304) + 4) + local_6c * 4);
              Item::getMaxPrice(pIVar21);
              uVar3 = Item::makeItem((int)pIVar21,uVar2);
              *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
              Item::setPrice(*(int *)(*(int *)(pAVar6 + 4) + iVar1 * 4));
              Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
            }
            Station::setItems(this_02,(Array *)pAVar6,false);
          }
          local_6c = 0;
          AEFile::Read((int *)&local_6c,local_60[7]);
          if (0 < (int)local_6c) {
            pAVar13 = operator_new(0xc);
            Array<Ship*>::Array(pAVar13);
            ArraySetLength<Ship*>(local_6c,(Array *)pAVar13);
            for (iVar1 = 0; iVar1 < (int)local_6c; iVar1 = iVar1 + 1) {
              local_70 = 0;
              AEFile::Read((int *)&local_70,local_60[7]);
              uVar3 = Ship::makeShip(*(int *)(*(int *)(*piVar15 + 4) + local_70 * 4));
              *(undefined4 *)(*(int *)(pAVar13 + 4) + iVar1 * 4) = uVar3;
              local_74 = 0;
              AEFile::Read((int *)&local_74,local_60[7]);
              Ship::setRace(*(int *)(*(int *)(pAVar13 + 4) + iVar1 * 4));
            }
            Station::setShips((Array *)this_02,SUB41(pAVar13,0));
          }
          local_70 = 0;
          AEFile::Read((int *)&local_70,local_60[7]);
          if (0 < (int)local_70) {
            pAVar12 = operator_new(0xc);
            Array<Agent*>::Array(pAVar12);
            ArraySetLength<Agent*>(local_70,(Array *)pAVar12);
            for (iVar1 = 0; iVar1 < (int)local_70; iVar1 = iVar1 + 1) {
              uVar3 = readAgent(this,local_60[7]);
              *(undefined4 *)(*(int *)(pAVar12 + 4) + iVar1 * 4) = uVar3;
            }
            Station::setAgents(this_02,(Array *)pAVar12);
          }
          local_74 = local_74 & 0xffffff00;
          AEFile::Read((bool *)&local_74,local_60[7]);
          Station::setAttackedFriends(this_02,local_74._0_1_);
        }
        uVar2 = *(uint *)this_01;
        if (uVar19 == uVar2) {
          *(Station **)(this_00 + 0x138) = this_02;
        }
        else {
          *(Station **)(*(int *)(this_01 + 4) + uVar19 * 4) = this_02;
        }
      }
      *(Array<Station*> **)(this_00 + 0x5c) = this_01;
      local_6c = 0;
      AEFile::Read((int *)&local_6c,local_60[7]);
      uVar2 = local_6c;
      uVar19 = (uint)((ulonglong)local_6c * 4);
      if ((int)((ulonglong)local_6c * 4 >> 0x20) != 0) {
        uVar19 = 0xffffffff;
      }
      piVar8 = operator_new__(uVar19);
      piVar7 = piVar8;
      for (iVar1 = 0; iVar1 < (int)uVar2; iVar1 = iVar1 + 1) {
        AEFile::Read(piVar7,local_60[7]);
        piVar7 = piVar7 + 1;
        uVar2 = local_6c;
      }
      this_03 = operator_new(8);
      Standing::Standing(this_03);
      *(Standing **)(this_00 + 0x13c) = this_03;
      Standing::setStandings(this_03,piVar8);
      this_04 = operator_new(0xc);
      Array<BluePrint*>::Array(this_04);
      local_70 = 0;
      AEFile::Read((int *)&local_70,local_60[7]);
      ArraySetLength<BluePrint*>(local_70,(Array *)this_04);
      puVar9 = *(undefined4 **)(DAT_000dc878 + 0xdc4fa);
      for (uVar2 = 0; uVar2 < *(uint *)this_04; uVar2 = uVar2 + 1) {
        this_05 = operator_new(0x2c);
        iVar1 = Status::getBluePrints((Status *)*puVar9);
        iVar1 = BluePrint::getIndex(*(BluePrint **)(*(int *)(iVar1 + 4) + uVar2 * 4));
        BluePrint::BluePrint(this_05,iVar1);
        iVar1 = 0;
        *(BluePrint **)(*(int *)(this_04 + 4) + uVar2 * 4) = this_05;
        piVar7 = *(int **)(*(int *)(this_04 + 4) + uVar2 * 4);
        for (uVar19 = 0; uVar19 < *(uint *)*piVar7; uVar19 = uVar19 + 1) {
          AEFile::Read((int *)(((uint *)*piVar7)[1] + iVar1),local_60[7]);
          iVar1 = iVar1 + 4;
        }
        AEFile::Read(piVar7 + 1,local_60[7]);
        AEFile::Read((bool *)(piVar7 + 2),local_60[7]);
        AEFile::Read(piVar7 + 3,local_60[7]);
        AEFile::Read(piVar7 + 4,local_60[7]);
        AbyssEngine::String::String((String *)local_40);
        AEFile::Read((String *)local_40,local_60[7],true);
        AbyssEngine::String::operator=((String *)(piVar7 + 5),(String *)local_40);
        AbyssEngine::String::~String((String *)local_40);
      }
      *(Array<BluePrint*> **)(this_00 + 0x140) = this_04;
      local_74 = 0;
      AEFile::Read((int *)&local_74,local_60[7]);
      if ((int)local_74 < 1) {
        *(undefined4 *)(this_00 + 0x144) = 0;
      }
      else {
        this_06 = operator_new(0xc);
        Array<PendingProduct*>::Array(this_06);
        ArraySetLength<PendingProduct*>(local_74,(Array *)this_06);
        for (uVar2 = 0; uVar2 < *(uint *)this_06; uVar2 = uVar2 + 1) {
          local_80 = 0;
          AEFile::Read((int *)&local_78,local_60[7]);
          AEFile::Read((int *)&local_7c,local_60[7]);
          AEFile::Read(&local_80,local_60[7]);
          AbyssEngine::String::String((String *)local_40);
          AEFile::Read((String *)local_40,local_60[7],true);
          pPVar10 = operator_new(0x18);
          uVar19 = local_78;
          AbyssEngine::String::String(aSStack_8c,(String *)local_40,false);
          PendingProduct::PendingProduct(pPVar10,uVar19,aSStack_8c,local_80,local_7c);
          *(PendingProduct **)(*(int *)(this_06 + 4) + uVar2 * 4) = pPVar10;
          AbyssEngine::String::~String(aSStack_8c);
          AbyssEngine::String::~String((String *)local_40);
        }
        *(Array<PendingProduct*> **)(this_00 + 0x144) = this_06;
      }
      local_78 = 0;
      AEFile::Read((int *)&local_78,local_60[7]);
      if ((int)local_78 < 1) {
        *(undefined4 *)(this_00 + 0x14c) = 0;
      }
      else {
        pAVar11 = operator_new(0xc);
        Array<AbyssEngine::String*>::Array();
        ArraySetLength<AbyssEngine::String*>(local_78,pAVar11);
        for (iVar1 = 0; iVar1 < (int)local_78; iVar1 = iVar1 + 1) {
          AbyssEngine::String::String((String *)local_40);
          AEFile::Read((String *)local_40,local_60[7],true);
          this_07 = operator_new(0xc);
          AbyssEngine::String::String(this_07,(String *)local_40,false);
          *(String **)(*(int *)(pAVar11 + 4) + iVar1 * 4) = this_07;
          AbyssEngine::String::~String((String *)local_40);
        }
        *(Array **)(this_00 + 0x14c) = pAVar11;
        AEFile::Read((int *)(this_00 + 0x150),local_60[7]);
        AEFile::Read((int *)(this_00 + 0x154),local_60[7]);
        local_40[0] = (RecordHandler *)0x0;
        AEFile::Read((int *)local_40,local_60[7]);
        pRVar17 = local_40[0];
        uVar2 = (uint)(ZEXT48(local_40[0]) * 4);
        if ((int)(ZEXT48(local_40[0]) * 4 >> 0x20) != 0) {
          uVar2 = 0xffffffff;
        }
        pvVar5 = operator_new__(uVar2);
        iVar16 = 0;
        *(void **)(this_00 + 0x158) = pvVar5;
        for (iVar1 = 0; iVar1 < (int)pRVar17; iVar1 = iVar1 + 1) {
          AEFile::Read((int *)(*(int *)(this_00 + 0x158) + iVar16),local_60[7]);
          iVar16 = iVar16 + 4;
          pRVar17 = local_40[0];
        }
      }
      AEFile::Read((int *)(this_00 + 0x15c),local_60[7]);
      local_40[0] = (RecordHandler *)0x0;
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x160) = pAVar4;
      ArraySetLength<bool>((uint)local_40[0],(Array *)pAVar4);
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x160); uVar2 = uVar2 + 1) {
        AEFile::Read((bool *)((*(uint **)(this_00 + 0x160))[1] + uVar2),local_60[7]);
      }
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar11 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this_00 + 0x168) = pAVar11;
      ArraySetLength<int>((uint)local_40[0],pAVar11);
      iVar1 = 0;
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x168); uVar2 = uVar2 + 1) {
        AEFile::Read((int *)((*(uint **)(this_00 + 0x168))[1] + iVar1),local_60[7]);
        iVar1 = iVar1 + 4;
      }
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar11 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this_00 + 0x164) = pAVar11;
      ArraySetLength<int>((uint)local_40[0],pAVar11);
      iVar1 = 0;
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x164); uVar2 = uVar2 + 1) {
        AEFile::Read((int *)((*(uint **)(this_00 + 0x164))[1] + iVar1),local_60[7]);
        iVar1 = iVar1 + 4;
      }
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar11 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this_00 + 0x170) = pAVar11;
      ArraySetLength<int>((uint)local_40[0],pAVar11);
      iVar1 = 0;
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x170); uVar2 = uVar2 + 1) {
        AEFile::Read((int *)((*(uint **)(this_00 + 0x170))[1] + iVar1),local_60[7]);
        iVar1 = iVar1 + 4;
      }
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar11 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this_00 + 0x16c) = pAVar11;
      ArraySetLength<int>((uint)local_40[0],pAVar11);
      iVar1 = 0;
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x16c); uVar2 = uVar2 + 1) {
        AEFile::Read((int *)((*(uint **)(this_00 + 0x16c))[1] + iVar1),local_60[7]);
        iVar1 = iVar1 + 4;
      }
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x174) = pAVar4;
      ArraySetLength<bool>((uint)local_40[0],(Array *)pAVar4);
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x174); uVar2 = uVar2 + 1) {
        AEFile::Read((bool *)((*(uint **)(this_00 + 0x174))[1] + uVar2),local_60[7]);
      }
      local_7c = 0;
      AEFile::Read((int *)&local_7c,local_60[7]);
      pAVar12 = operator_new(0xc);
      Array<Agent*>::Array(pAVar12);
      *(Array<Agent*> **)(this_00 + 0x148) = pAVar12;
      ArraySetLength<Agent*>(local_7c,(Array *)pAVar12);
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x148); uVar2 = uVar2 + 1) {
        uVar3 = readAgent(this,local_60[7]);
        *(undefined4 *)(*(int *)(*(int *)(this_00 + 0x148) + 4) + uVar2 * 4) = uVar3;
      }
      AEFile::Read((bool *)(this_00 + 0xe4),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xe5),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xe6),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xe7),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xe8),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xe9),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xea),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xeb),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xec),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xed),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xee),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xef),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf0),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf1),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf2),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf3),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf4),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf5),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf6),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf7),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf8),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xf9),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xfa),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xfb),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xfc),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xfe),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xfd),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xff),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0x100),local_60[7]);
      AEFile::Read((float *)(this_00 + 0x11c),local_60[7]);
      AEFile::Read((longlong *)(this_00 + 200),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0x101),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0x102),local_60[7]);
      local_80 = 0;
      AEFile::Read(&local_80,local_60[7]);
      if (0 < local_80) {
        local_90 = 0;
        AEFile::Read((int *)&local_90,local_60[7]);
        uVar3 = Ship::makeShip(*(int *)(*(int *)(*piVar15 + 4) + local_90 * 4));
        *(undefined4 *)(this_00 + 0x134) = uVar3;
        local_94 = 0;
        AEFile::Read((int *)&local_94,local_60[7]);
        Ship::setRace(*(int *)(this_00 + 0x134));
        local_98 = 0;
        AEFile::Read((int *)&local_98,local_60[7]);
        if (0 < (int)local_98) {
          pAVar6 = operator_new(0xc);
          Array<Item*>::Array(pAVar6);
          ArraySetLength<Item*>(local_98,(Array *)pAVar6);
          piVar7 = *(int **)(DAT_000dce48 + 0xdcaa6);
          for (iVar1 = 0; iVar1 < (int)local_98; iVar1 = iVar1 + 1) {
            local_9c = 0;
            AEFile::Read((int *)&local_9c,local_60[7]);
            if (local_9c == 0xffffffff) {
              *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = 0;
            }
            else {
              local_a0 = 0;
              AEFile::Read(&local_a0,local_60[7]);
              iVar16 = local_a0;
              pIVar21 = *(Item **)(*(int *)(*piVar7 + 4) + local_9c * 4);
              Item::getMaxPrice(pIVar21);
              uVar3 = Item::makeItem((int)pIVar21,iVar16);
              *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
              local_a4 = local_a4 & 0xffffff00;
              AEFile::Read((bool *)&local_a4,local_60[7]);
              Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
            }
          }
          Ship::replaceEquipment(*(Ship **)(this_00 + 0x134),(Array *)pAVar6);
        }
        local_9c = 0;
        AEFile::Read((int *)&local_9c,local_60[7]);
        if (0 < (int)local_9c) {
          pAVar6 = operator_new(0xc);
          Array<Item*>::Array(pAVar6);
          ArraySetLength<Item*>(local_9c,(Array *)pAVar6);
          piVar7 = *(int **)(DAT_000dce4c + 0xdcb5a);
          for (iVar1 = 0; iVar1 < (int)local_9c; iVar1 = iVar1 + 1) {
            local_a8 = 0;
            AEFile::Read(&local_a0,local_60[7]);
            AEFile::Read((int *)&local_a4,local_60[7]);
            AEFile::Read(&local_a8,local_60[7]);
            local_ac = local_ac & 0xffffff00;
            AEFile::Read((bool *)&local_ac,local_60[7]);
            uVar2 = local_a4;
            pIVar21 = *(Item **)(*(int *)(*piVar7 + 4) + local_a0 * 4);
            Item::getMaxPrice(pIVar21);
            uVar3 = Item::makeItem((int)pIVar21,uVar2);
            *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
            Item::setPrice(*(int *)(*(int *)(pAVar6 + 4) + iVar1 * 4));
            Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
          }
          Ship::replaceCargo(*(Ship **)(this_00 + 0x134),(Array *)pAVar6);
        }
      }
      pvVar5 = operator_new(0xc);
      Array<int>::Array();
      *(void **)(this_00 + 0x70) = pvVar5;
      local_90 = 0;
      AEFile::Read((int *)&local_90,local_60[7]);
      ArraySetLength<int>(local_90,*(Array **)(this_00 + 0x70));
      iVar16 = 0;
      for (iVar1 = 0; iVar1 < (int)local_90; iVar1 = iVar1 + 1) {
        AEFile::Read((int *)(*(int *)(*(int *)(this_00 + 0x70) + 4) + iVar16),local_60[7]);
        iVar16 = iVar16 + 4;
      }
      AEFile::Read((int *)(this_00 + 0xd0),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xd4),local_60[7]);
      AEFile::Read((int *)(this_00 + 0xd8),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0xdc),local_60[7]);
      local_94 = 0;
      AEFile::Read((int *)&local_94,local_60[7]);
      if (0 < (int)local_94) {
        pAVar6 = operator_new(0xc);
        Array<Item*>::Array(pAVar6);
        ArraySetLength<Item*>(local_94,(Array *)pAVar6);
        piVar7 = *(int **)(DAT_000dce50 + 0xdcc74);
        for (iVar1 = 0; iVar1 < (int)local_94; iVar1 = iVar1 + 1) {
          local_a0 = 0;
          AEFile::Read((int *)&local_98,local_60[7]);
          AEFile::Read((int *)&local_9c,local_60[7]);
          AEFile::Read(&local_a0,local_60[7]);
          local_a4 = local_a4 & 0xffffff00;
          AEFile::Read((bool *)&local_a4,local_60[7]);
          uVar2 = local_9c;
          pIVar21 = *(Item **)(*(int *)(*piVar7 + 4) + local_98 * 4);
          Item::getMaxPrice(pIVar21);
          uVar3 = Item::makeItem((int)pIVar21,uVar2);
          *(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4) = uVar3;
          Item::setPrice(*(int *)(*(int *)(pAVar6 + 4) + iVar1 * 4));
          Item::setUnsaleable(SUB41(*(undefined4 *)(*(int *)(pAVar6 + 4) + iVar1 * 4),0));
        }
        *(Array<Item*> **)(this_00 + 0x180) = pAVar6;
      }
      local_98 = 0;
      AEFile::Read((int *)&local_98,local_60[7]);
      if (0 < (int)local_98) {
        pAVar13 = operator_new(0xc);
        Array<Ship*>::Array(pAVar13);
        ArraySetLength<Ship*>(local_98,(Array *)pAVar13);
        for (iVar1 = 0; iVar1 < (int)local_98; iVar1 = iVar1 + 1) {
          local_9c = 0;
          AEFile::Read((int *)&local_9c,local_60[7]);
          uVar3 = Ship::makeShip(*(int *)(*(int *)(*piVar15 + 4) + local_9c * 4));
          *(undefined4 *)(*(int *)(pAVar13 + 4) + iVar1 * 4) = uVar3;
          local_a0 = 0;
          AEFile::Read(&local_a0,local_60[7]);
          Ship::setRace(*(int *)(*(int *)(pAVar13 + 4) + iVar1 * 4));
        }
        *(Array<Ship*> **)(this_00 + 0x184) = pAVar13;
      }
      AEFile::Read((bool *)(this_00 + 0x103),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0x115),local_60[7]);
      AEFile::Read((bool *)(this_00 + 0x116),local_60[7]);
      AEFile::Read((int *)local_40,local_60[7]);
      pAVar4 = operator_new(0xc);
      Array<bool>::Array(pAVar4);
      *(Array<bool> **)(this_00 + 0x178) = pAVar4;
      ArraySetLength<bool>((uint)local_40[0],(Array *)pAVar4);
      for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x178); uVar2 = uVar2 + 1) {
        AEFile::Read((bool *)((*(uint **)(this_00 + 0x178))[1] + uVar2),local_60[7]);
      }
      AEFile::Read((int *)(this_00 + 0x1bc),local_60[7]);
      if (*(int *)(this_00 + 0x1bc) == DAT_000dce54) {
        local_9c = 0;
        AEFile::Read((int *)&local_9c,local_60[7]);
        if (0 < (int)local_9c) {
          pAVar11 = operator_new(0xc);
          Array<int>::Array();
          ArraySetLength<int>(local_9c,pAVar11);
          iVar16 = 0;
          for (iVar1 = 0; iVar1 < (int)local_9c; iVar1 = iVar1 + 1) {
            AEFile::Read((int *)(*(int *)(pAVar11 + 4) + iVar16),local_60[7]);
            iVar16 = iVar16 + 4;
          }
          Ship::setMods(*(Ship **)(this_00 + 0x130),pAVar11);
        }
        AEFile::Read((int *)&local_9c,local_60[7]);
        if (0 < (int)local_9c) {
          pAVar11 = operator_new(0xc);
          Array<int>::Array();
          ArraySetLength<int>(local_9c,pAVar11);
          iVar16 = 0;
          for (iVar1 = 0; iVar1 < (int)local_9c; iVar1 = iVar1 + 1) {
            AEFile::Read((int *)(*(int *)(pAVar11 + 4) + iVar16),local_60[7]);
            iVar16 = iVar16 + 4;
          }
          Ship::setMods(*(Ship **)(this_00 + 0x134),pAVar11);
        }
        local_a0 = 0;
        AEFile::Read(&local_a0,local_60[7]);
        if (0 < local_a0) {
          for (iVar1 = 0; iVar1 < local_a0; iVar1 = iVar1 + 1) {
            local_9c = 0;
            AEFile::Read((int *)&local_9c,local_60[7]);
            if (0 < (int)local_9c) {
              pAVar11 = operator_new(0xc);
              Array<int>::Array();
              ArraySetLength<int>(local_9c,pAVar11);
              iVar14 = 0;
              for (iVar16 = 0; iVar16 < (int)local_9c; iVar16 = iVar16 + 1) {
                AEFile::Read((int *)(*(int *)(pAVar11 + 4) + iVar14),local_60[7]);
                iVar14 = iVar14 + 4;
              }
              Ship::setMods(*(Ship **)(*(int *)(*(int *)(this_00 + 0x184) + 4) + iVar1 * 4),pAVar11)
              ;
            }
          }
        }
        for (uVar2 = 0; uVar2 < *(uint *)this_01 + 1; uVar2 = uVar2 + 1) {
          pGVar20 = this_00 + 0x138;
          if (uVar2 != *(uint *)this_01) {
            pGVar20 = (GameRecord *)(*(int *)(this_01 + 4) + uVar2 * 4);
          }
          if (*(int *)pGVar20 != 0) {
            local_a4 = 0;
            AEFile::Read((int *)&local_a4,local_60[7]);
            if (0 < (int)local_a4) {
              for (iVar1 = 0; iVar1 < (int)local_a4; iVar1 = iVar1 + 1) {
                local_a8 = 0;
                AEFile::Read(&local_a8,local_60[7]);
                for (iVar16 = 0; iVar16 < local_a8; iVar16 = iVar16 + 1) {
                  AEFile::Read((int *)&local_ac,local_60[7]);
                  iVar14 = Station::getShips();
                  Ship::addMod(*(Ship **)(*(int *)(iVar14 + 4) + iVar1 * 4),local_ac);
                }
              }
            }
          }
        }
        local_40[0] = (RecordHandler *)0x0;
        AEFile::Read((int *)local_40,local_60[7]);
        if (0 < (int)local_40[0]) {
          this_08 = operator_new(0xc);
          Array<Wanted*>::Array(this_08);
          *(Array<Wanted*> **)(this_00 + 0x1b4) = this_08;
          ArraySetLength<Wanted*>((uint)local_40[0],(Array *)this_08);
          for (iVar1 = 0; iVar1 < (int)local_40[0]; iVar1 = iVar1 + 1) {
            uVar3 = readWanted(local_40[0],local_60[7]);
            *(undefined4 *)(*(int *)(*(int *)(this_00 + 0x1b4) + 4) + iVar1 * 4) = uVar3;
          }
        }
        pGVar20 = this_00 + 0x1a4;
        for (uVar2 = 0; uVar2 < 4; uVar2 = uVar2 + 1) {
          AEFile::Read((int *)pGVar20,local_60[7]);
          pGVar20 = pGVar20 + 4;
        }
        AEFile::Read((bool *)(this_00 + 0x117),local_60[7]);
        AEFile::Read((int *)(this_00 + 0x1b8),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x104),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x105),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x108),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x106),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x107),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10a),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10b),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10c),local_60[7]);
        AEFile::Read((int *)local_40,local_60[7]);
        pAVar4 = operator_new(0xc);
        Array<bool>::Array(pAVar4);
        *(Array<bool> **)(this_00 + 0x17c) = pAVar4;
        ArraySetLength<bool>((uint)local_40[0],(Array *)pAVar4);
        for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 0x17c); uVar2 = uVar2 + 1) {
          AEFile::Read((bool *)((*(uint **)(this_00 + 0x17c))[1] + uVar2),local_60[7]);
        }
        AEFile::Read((bool *)(this_00 + 0x119),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x109),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x11a),local_60[7]);
        AEFile::Read((int *)(this_00 + 0xe0),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10d),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10e),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x10f),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x110),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x111),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x113),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x112),local_60[7]);
        AEFile::Read((bool *)(this_00 + 0x114),local_60[7]);
        AEFile::Close(local_60[7]);
      }
      else {
        AEFile::Close(local_60[7]);
      }
    }
  }
  AbyssEngine::String::~String(aSStack_34);
  iVar1 = *piVar18 - local_28;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000dbd34: push {r4,r5,r6,r7,lr}
  000dbd36: add r7,sp,#0xc
  000dbd38: push {r8,r9,r10,r11}
  000dbd3c: sub sp,#0xac
  000dbd3e: mov r9,r0
  000dbd40: ldr r0,[0x000dc094]
  000dbd42: add r4,sp,#0x90
  000dbd44: add r0,pc
  000dbd46: ldr r6,[r0,#0x0]
  000dbd48: ldr r0,[r6,#0x0]
  000dbd4a: str r0,[sp,#0xa8]
  000dbd4c: mov r0,r4
  000dbd4e: blx 0x0006f658
  000dbd52: add.w r1,r9,#0x14
  000dbd56: add r0,sp,#0x9c
  000dbd58: mov r2,r4
  000dbd5a: blx 0x0006ef98
  000dbd5e: add r0,sp,#0x90
  000dbd60: blx 0x0006ee30
  000dbd64: add r0,sp,#0x9c
  000dbd66: blx 0x0006fff4
  000dbd6a: cmp r0,#0x0
  000dbd6c: beq.w 0x000dbf68
  000dbd70: add r0,sp,#0x9c
  000dbd72: add r1,sp,#0x8c
  000dbd74: blx 0x0006f34c
  000dbd78: ldr r1,[sp,#0x8c]
  000dbd7a: blx 0x00074848
  000dbd7e: mov r4,r0
  000dbd80: ldr r0,[sp,#0x8c]
  000dbd82: blx 0x0006ece0
  000dbd86: cmp r4,#0x0
  000dbd88: beq.w 0x000dbf68
  000dbd8c: add r0,sp,#0x9c
  000dbd8e: add r1,sp,#0x8c
  000dbd90: blx 0x0006f34c
  000dbd94: mov.w r0,#0x1c8
  000dbd98: blx 0x0006eb24
  000dbd9c: mov r11,r0
  000dbd9e: blx 0x00074800
  000dbda2: movs r0,#0x0
  000dbda4: mov r5,r11
  000dbda6: ldr r1,[sp,#0x8c]
  000dbda8: str.w r0,[r5,#0x4]!
  000dbdac: mov r0,r5
  000dbdae: blx 0x00074818
  000dbdb2: movs r4,#0x0
  000dbdb4: b 0x000dbdc4
  000dbdb6: ldr.w r0,[r11,#0x0]
  000dbdba: ldr r1,[sp,#0x8c]
  000dbdbc: add r0,r4
  000dbdbe: blx 0x00074854
  000dbdc2: adds r4,#0x1
  000dbdc4: ldr r0,[r5,#0x0]
  000dbdc6: cmp r4,r0
  000dbdc8: bcc 0x000dbdb6
  000dbdca: ldr r1,[sp,#0x8c]
  000dbdcc: add.w r0,r11,#0x8
  000dbdd0: blx 0x00074818
  000dbdd4: ldr r1,[sp,#0x8c]
  000dbdd6: add.w r0,r11,#0xc
  000dbdda: blx 0x00074818
  000dbdde: ldr r1,[sp,#0x8c]
  000dbde0: add.w r0,r11,#0x10
  000dbde4: blx 0x0007480c
  000dbde8: ldr r1,[sp,#0x8c]
  000dbdea: add.w r0,r11,#0x18
  000dbdee: blx 0x00074818
  000dbdf2: ldr r1,[sp,#0x8c]
  000dbdf4: add.w r0,r11,#0x1c
  000dbdf8: blx 0x00074818
  000dbdfc: ldr r1,[sp,#0x8c]
  000dbdfe: add.w r0,r11,#0x20
  000dbe02: blx 0x00074818
  000dbe06: ldr r1,[sp,#0x8c]
  000dbe08: add.w r0,r11,#0x24
  000dbe0c: blx 0x00074818
  000dbe10: ldr r1,[sp,#0x8c]
  000dbe12: add.w r0,r11,#0x28
  000dbe16: blx 0x00074818
  000dbe1a: ldr r1,[sp,#0x8c]
  000dbe1c: add.w r0,r11,#0x3c
  000dbe20: blx 0x00074818
  000dbe24: ldr r1,[sp,#0x8c]
  000dbe26: add.w r0,r11,#0x40
  000dbe2a: blx 0x00074818
  000dbe2e: ldr r1,[sp,#0x8c]
  000dbe30: mov r0,r9
  000dbe32: blx 0x00074860
  000dbe36: ldr r1,[sp,#0x8c]
  000dbe38: str.w r0,[r11,#0x54]
  000dbe3c: mov r0,r9
  000dbe3e: blx 0x00074860
  000dbe42: ldr r1,[sp,#0x8c]
  000dbe44: str.w r0,[r11,#0x58]
  000dbe48: add.w r0,r11,#0x30
  000dbe4c: blx 0x00074818
  000dbe50: ldr r1,[sp,#0x8c]
  000dbe52: add.w r0,r11,#0x34
  000dbe56: blx 0x00074818
  000dbe5a: ldr r1,[sp,#0x8c]
  000dbe5c: add.w r0,r11,#0x38
  000dbe60: blx 0x00074818
  000dbe64: ldr r1,[sp,#0x8c]
  000dbe66: add.w r0,r11,#0x2c
  000dbe6a: blx 0x00074818
  000dbe6e: ldr r1,[sp,#0x8c]
  000dbe70: add.w r0,r11,#0x44
  000dbe74: blx 0x00074818
  000dbe78: ldr r1,[sp,#0x8c]
  000dbe7a: add.w r0,r11,#0x48
  000dbe7e: blx 0x00074818
  000dbe82: ldr r1,[sp,#0x8c]
  000dbe84: add.w r0,r11,#0x4c
  000dbe88: blx 0x00074818
  000dbe8c: ldr r1,[sp,#0x8c]
  000dbe8e: add.w r0,r11,#0x50
  000dbe92: blx 0x00074818
  000dbe96: movs r0,#0xc
  000dbe98: blx 0x0006eb24
  000dbe9c: mov r5,r0
  000dbe9e: blx 0x000735a0
  000dbea2: ldr r1,[sp,#0x8c]
  000dbea4: movs r0,#0x0
  000dbea6: str.w r5,[r11,#0x68]
  000dbeaa: str r0,[sp,#0x88]
  000dbeac: add r0,sp,#0x88
  000dbeae: blx 0x00074818
  000dbeb2: ldr r0,[sp,#0x88]
  000dbeb4: ldr.w r1,[r11,#0x68]
  000dbeb8: blx 0x000735c4
  000dbebc: movs r4,#0x0
  000dbebe: b 0x000dbed0
  000dbec0: ldr.w r0,[r11,#0x68]
  000dbec4: ldr r1,[sp,#0x8c]
  000dbec6: ldr r0,[r0,#0x4]
  000dbec8: add r0,r4
  000dbeca: blx 0x00074854
  000dbece: adds r4,#0x1
  000dbed0: ldr r0,[sp,#0x88]
  000dbed2: cmp r4,r0
  000dbed4: blt 0x000dbec0
  000dbed6: movs r0,#0xc
  000dbed8: blx 0x0006eb24
  000dbedc: mov r5,r0
  000dbede: blx 0x000735a0
  000dbee2: ldr r1,[sp,#0x8c]
  000dbee4: movs r0,#0x0
  000dbee6: str.w r5,[r11,#0x6c]
  000dbeea: str r0,[sp,#0x84]
  000dbeec: add r0,sp,#0x84
  000dbeee: blx 0x00074818
  000dbef2: ldr r0,[sp,#0x84]
  000dbef4: ldr.w r1,[r11,#0x6c]
  000dbef8: blx 0x000735c4
  000dbefc: movs r4,#0x0
  000dbefe: b 0x000dbf10
  000dbf00: ldr.w r0,[r11,#0x6c]
  000dbf04: ldr r1,[sp,#0x8c]
  000dbf06: ldr r0,[r0,#0x4]
  000dbf08: add r0,r4
  000dbf0a: blx 0x00074854
  000dbf0e: adds r4,#0x1
  000dbf10: ldr r0,[sp,#0x84]
  000dbf12: cmp r4,r0
  000dbf14: blt 0x000dbf00
  000dbf16: ldr r1,[sp,#0x8c]
  000dbf18: add.w r0,r11,#0x74
  000dbf1c: blx 0x00074818
  000dbf20: ldr r1,[sp,#0x8c]
  000dbf22: add.w r0,r11,#0x78
  000dbf26: blx 0x00074818
  000dbf2a: ldr r1,[sp,#0x8c]
  000dbf2c: add.w r0,r11,#0x7c
  000dbf30: blx 0x00074818
  000dbf34: ldr r1,[sp,#0x8c]
  000dbf36: add.w r0,r11,#0x80
  000dbf3a: blx 0x00074818
  000dbf3e: movs r0,#0xc
  000dbf40: blx 0x0006eb24
  000dbf44: mov r5,r0
  000dbf46: blx 0x000735a0
  000dbf4a: ldr r1,[sp,#0x8c]
  000dbf4c: movs r0,#0x0
  000dbf4e: str.w r5,[r11,#0x84]
  000dbf52: str r0,[sp,#0x80]
  000dbf54: add r0,sp,#0x80
  000dbf56: blx 0x00074818
  000dbf5a: ldr r0,[sp,#0x80]
  000dbf5c: ldr.w r1,[r11,#0x84]
  000dbf60: blx 0x000735c4
  000dbf64: movs r4,#0x0
  000dbf66: b 0x000dbf98
  000dbf68: mov.w r11,#0x0
  000dbf6c: add r0,sp,#0x9c
  000dbf6e: blx 0x0006ee30
  000dbf72: ldr r0,[sp,#0xa8]
  000dbf74: ldr r1,[r6,#0x0]
  000dbf76: subs r0,r1,r0
  000dbf78: itttt eq
  000dbf7a: mov.eq r0,r11
  000dbf7c: add.eq sp,#0xac
  000dbf7e: pop.eq.w {r8,r9,r10,r11}
  000dbf82: pop.eq {r4,r5,r6,r7,pc}
  000dbf84: blx 0x0006e824
  000dbf88: ldr.w r0,[r11,#0x84]
  000dbf8c: ldr r1,[sp,#0x8c]
  000dbf8e: ldr r0,[r0,#0x4]
  000dbf90: add r0,r4
  000dbf92: blx 0x00074854
  000dbf96: adds r4,#0x1
  000dbf98: ldr r0,[sp,#0x80]
  000dbf9a: cmp r4,r0
  000dbf9c: blt 0x000dbf88
  000dbf9e: ldr r1,[sp,#0x8c]
  000dbfa0: add.w r0,r11,#0x88
  000dbfa4: blx 0x00074818
  000dbfa8: movs r0,#0xc
  000dbfaa: blx 0x0006eb24
  000dbfae: mov r5,r0
  000dbfb0: str r6,[sp,#0x20]
  000dbfb2: blx 0x000735a0
  000dbfb6: ldr r1,[sp,#0x8c]
  000dbfb8: movs r0,#0x0
  000dbfba: str.w r5,[r11,#0x8c]
  000dbfbe: str r0,[sp,#0x7c]
  000dbfc0: add r0,sp,#0x7c
  000dbfc2: blx 0x00074818
  000dbfc6: ldr r0,[sp,#0x7c]
  000dbfc8: ldr.w r1,[r11,#0x8c]
  000dbfcc: blx 0x000735c4
  000dbfd0: movs r4,#0x0
  000dbfd2: b 0x000dbfe4
  000dbfd4: ldr.w r0,[r11,#0x8c]
  000dbfd8: ldr r1,[sp,#0x8c]
  000dbfda: ldr r0,[r0,#0x4]
  000dbfdc: add r0,r4
  000dbfde: blx 0x00074854
  000dbfe2: adds r4,#0x1
  000dbfe4: ldr r0,[sp,#0x7c]
  000dbfe6: cmp r4,r0
  000dbfe8: blt 0x000dbfd4
  000dbfea: ldr r1,[sp,#0x8c]
  000dbfec: add.w r0,r11,#0x90
  000dbff0: blx 0x00074818
  000dbff4: ldr r1,[sp,#0x8c]
  000dbff6: add.w r0,r11,#0x98
  000dbffa: blx 0x0007480c
  000dbffe: ldr r1,[sp,#0x8c]
  000dc000: add.w r0,r11,#0xa0
  000dc004: blx 0x00074818
  000dc008: ldr r1,[sp,#0x8c]
  000dc00a: add.w r0,r11,#0xa4
  000dc00e: blx 0x00074818
  000dc012: ldr r1,[sp,#0x8c]
  000dc014: add.w r0,r11,#0xa8
  000dc018: blx 0x00074818
  000dc01c: ldr r1,[sp,#0x8c]
  000dc01e: add.w r0,r11,#0xac
  000dc022: blx 0x00074818
  000dc026: ldr r1,[sp,#0x8c]
  000dc028: add.w r0,r11,#0xb0
  000dc02c: blx 0x00074818
  000dc030: ldr r1,[sp,#0x8c]
  000dc032: add.w r0,r11,#0xb4
  000dc036: blx 0x00074818
  000dc03a: ldr r1,[sp,#0x8c]
  000dc03c: add.w r0,r11,#0xb8
  000dc040: blx 0x00074818
  000dc044: ldr r1,[sp,#0x8c]
  000dc046: add.w r0,r11,#0xbc
  000dc04a: blx 0x00074818
  000dc04e: ldr r1,[sp,#0x8c]
  000dc050: add.w r0,r11,#0xc0
  000dc054: blx 0x00074818
  000dc058: ldr r1,[sp,#0x8c]
  000dc05a: add.w r0,r11,#0xc4
  000dc05e: blx 0x00074818
  000dc062: ldr r1,[sp,#0x8c]
  000dc064: add.w r8,r11,#0x64
  000dc068: mov r0,r8
  000dc06a: blx 0x00074818
  000dc06e: ldr.w r6,[r8,#0x0]
  000dc072: movs r0,#0x4
  000dc074: umull r0,r1,r6,r0
  000dc078: cmp r1,#0x0
  000dc07a: it ne
  000dc07c: mov.ne r1,#0x1
  000dc07e: cmp r1,#0x0
  000dc080: it ne
  000dc082: mov.ne.w r0,#0xffffffff
  000dc086: blx 0x0006ec08
  000dc08a: movs r5,#0x0
  000dc08c: movs r4,#0x0
  000dc08e: str.w r0,[r11,#0x60]
  000dc092: b 0x000dc0ac
  000dc098: ldr.w r0,[r11,#0x60]
  000dc09c: ldr r1,[sp,#0x8c]
  000dc09e: add r0,r5
  000dc0a0: blx 0x00074818
  000dc0a4: adds r5,#0x4
  000dc0a6: ldr.w r6,[r8,#0x0]
  000dc0aa: adds r4,#0x1
  000dc0ac: cmp r4,r6
  000dc0ae: blt 0x000dc098
  000dc0b0: ldr r1,[sp,#0x8c]
  000dc0b2: movs r0,#0x0
  000dc0b4: str r0,[sp,#0x78]
  000dc0b6: add r0,sp,#0x78
  000dc0b8: blx 0x00074818
  000dc0bc: ldr r0,[0x000dc448]
  000dc0be: ldr r1,[sp,#0x78]
  000dc0c0: add r0,pc
  000dc0c2: ldr r4,[r0,#0x0]
  000dc0c4: ldr r0,[r4,#0x0]
  000dc0c6: ldr r0,[r0,#0x4]
  000dc0c8: ldr.w r0,[r0,r1,lsl #0x2]
  000dc0cc: mov.w r1,#0xffffffff
  000dc0d0: blx 0x000719d4
  000dc0d4: ldr r1,[sp,#0x8c]
  000dc0d6: str.w r0,[r11,#0x130]
  000dc0da: movs r0,#0x0
  000dc0dc: str r0,[sp,#0x74]
  000dc0de: add r0,sp,#0x74
  000dc0e0: blx 0x00074818
  000dc0e4: ldr r1,[sp,#0x74]
  000dc0e6: ldr.w r0,[r11,#0x130]
  000dc0ea: blx 0x000719ec
  000dc0ee: ldr r1,[sp,#0x8c]
  000dc0f0: movs r0,#0x0
  000dc0f2: str r0,[sp,#0x70]
  000dc0f4: add r0,sp,#0x70
  000dc0f6: blx 0x00074818
  000dc0fa: ldr r0,[sp,#0x70]
  000dc0fc: cmp r0,#0x1
  000dc0fe: blt 0x000dc1a0
  000dc100: movs r0,#0xc
  000dc102: blx 0x0006eb24
  000dc106: mov r6,r0
  000dc108: blx 0x00071860
  000dc10c: ldr r0,[sp,#0x70]
  000dc10e: mov r1,r6
  000dc110: blx 0x0007186c
  000dc114: str r4,[sp,#0x18]
  000dc116: add r4,sp,#0x90
  000dc118: ldr r0,[0x000dc44c]
  000dc11a: movs r5,#0x0
  000dc11c: mov.w r8,#0x0
  000dc120: add r0,pc
  000dc122: ldr r0,[r0,#0x0]
  000dc124: str r0,[sp,#0x1c]
  000dc126: b 0x000dc18e
  000dc128: ldr r1,[sp,#0x8c]
  000dc12a: str r5,[sp,#0x90]
  000dc12c: mov r0,r4
  000dc12e: blx 0x00074818
  000dc132: ldr r0,[sp,#0x90]
  000dc134: adds r0,#0x1
  000dc136: beq 0x000dc184
  000dc138: ldr r1,[sp,#0x8c]
  000dc13a: str r5,[sp,#0x6c]
  000dc13c: add r0,sp,#0x6c
  000dc13e: blx 0x00074818
  000dc142: ldr r0,[sp,#0x1c]
  000dc144: ldr r1,[sp,#0x90]
  000dc146: ldr r5,[sp,#0x6c]
  000dc148: ldr r0,[r0,#0x0]
  000dc14a: ldr r0,[r0,#0x4]
  000dc14c: ldr.w r10,[r0,r1,lsl #0x2]
  000dc150: mov r0,r10
  000dc152: blx 0x00071cbc
  000dc156: mov r2,r0
  000dc158: mov r0,r10
  000dc15a: mov r1,r5
  000dc15c: blx 0x0007486c
  000dc160: ldr r1,[r6,#0x4]
  000dc162: movs r5,#0x0
  000dc164: str.w r0,[r1,r8,lsl #0x2]
  000dc168: ldr r1,[sp,#0x8c]
  000dc16a: strb.w r5,[sp,#0x68]
  000dc16e: add r0,sp,#0x68
  000dc170: blx 0x00074854
  000dc174: ldr r0,[r6,#0x4]
  000dc176: ldrb.w r1,[sp,#0x68]
  000dc17a: ldr.w r0,[r0,r8,lsl #0x2]
  000dc17e: blx 0x00073330
  000dc182: b 0x000dc18a
  000dc184: ldr r0,[r6,#0x4]
  000dc186: str.w r5,[r0,r8,lsl #0x2]
  000dc18a: add.w r8,r8,#0x1
  000dc18e: ldr r0,[sp,#0x70]
  000dc190: cmp r8,r0
  000dc192: blt 0x000dc128
  000dc194: ldr.w r0,[r11,#0x130]
  000dc198: mov r1,r6
  000dc19a: blx 0x00074878
  000dc19e: ldr r4,[sp,#0x18]
  000dc1a0: ldr r1,[sp,#0x8c]
  000dc1a2: movs r0,#0x0
  000dc1a4: str r0,[sp,#0x6c]
  000dc1a6: add r0,sp,#0x6c
  000dc1a8: blx 0x00074818
  000dc1ac: ldr r0,[sp,#0x6c]
  000dc1ae: cmp r0,#0x1
  000dc1b0: blt 0x000dc250
  000dc1b2: movs r0,#0xc
  000dc1b4: blx 0x0006eb24
  000dc1b8: mov r5,r0
  000dc1ba: blx 0x00071860
  000dc1be: ldr r0,[sp,#0x6c]
  000dc1c0: mov r1,r5
  000dc1c2: blx 0x0007186c
  000dc1c6: ldr r0,[0x000dc450]
  000dc1c8: movs r6,#0x0
  000dc1ca: mov.w r8,#0x0
  000dc1ce: add r0,pc
  000dc1d0: ldr r0,[r0,#0x0]
  000dc1d2: str r0,[sp,#0x1c]
  000dc1d4: b 0x000dc240
  000dc1d6: ldr r1,[sp,#0x8c]
  000dc1d8: str r6,[sp,#0x64]
  000dc1da: add r0,sp,#0x90
  000dc1dc: blx 0x00074818
  000dc1e0: ldr r1,[sp,#0x8c]
  000dc1e2: add r0,sp,#0x68
  000dc1e4: blx 0x00074818
  000dc1e8: ldr r1,[sp,#0x8c]
  000dc1ea: add r0,sp,#0x64
  000dc1ec: blx 0x00074818
  000dc1f0: ldr r1,[sp,#0x8c]
  000dc1f2: strb.w r6,[sp,#0x60]
  000dc1f6: add r0,sp,#0x60
  000dc1f8: blx 0x00074854
  000dc1fc: ldr r0,[sp,#0x1c]
  000dc1fe: ldr r1,[sp,#0x90]
  000dc200: ldr r6,[sp,#0x68]
  000dc202: ldr r0,[r0,#0x0]
  000dc204: ldr r0,[r0,#0x4]
  000dc206: ldr.w r10,[r0,r1,lsl #0x2]
  000dc20a: mov r0,r10
  000dc20c: blx 0x00071cbc
  000dc210: mov r2,r0
  000dc212: mov r0,r10
  000dc214: mov r1,r6
  000dc216: blx 0x0007486c
  000dc21a: ldr r1,[r5,#0x4]
  000dc21c: str.w r0,[r1,r8,lsl #0x2]
  000dc220: ldr r0,[r5,#0x4]
  000dc222: ldr r1,[sp,#0x64]
  000dc224: ldr.w r0,[r0,r8,lsl #0x2]
  000dc228: blx 0x00073864
  000dc22c: ldr r0,[r5,#0x4]
  000dc22e: ldrb.w r1,[sp,#0x60]
  000dc232: ldr.w r0,[r0,r8,lsl #0x2]
  000dc236: blx 0x00073330
  000dc23a: add.w r8,r8,#0x1
  000dc23e: movs r6,#0x0
  000dc240: ldr r0,[sp,#0x6c]
  000dc242: cmp r8,r0
  000dc244: blt 0x000dc1d6
  000dc246: ldr.w r0,[r11,#0x130]
  000dc24a: mov r1,r5
  000dc24c: blx 0x00074884
  000dc250: movs r0,#0xc
  000dc252: str r4,[sp,#0x18]
  000dc254: blx 0x0006eb24
  000dc258: mov r4,r0
  000dc25a: blx 0x00073594
  000dc25e: ldr r1,[sp,#0x8c]
  000dc260: movs r0,#0x0
  000dc262: str r0,[sp,#0x68]
  000dc264: add r0,sp,#0x68
  000dc266: blx 0x00074818
  000dc26a: ldr r0,[sp,#0x68]
  000dc26c: mov r1,r4
  000dc26e: blx 0x000735b8
  000dc272: add.w r1,r11,#0x138
  000dc276: str r1,[sp,#0xc]
  000dc278: ldr r0,[r4,#0x0]
  000dc27a: add.w r10,sp,#0x90
  000dc27e: ldr r1,[0x000dc454]
  000dc280: movs r6,#0x0
  000dc282: movs r2,#0x0
  000dc284: add r1,pc
  000dc286: ldr r1,[r1,#0x0]
  000dc288: str r1,[sp,#0x14]
  000dc28a: b 0x000dc464
  000dc28c: ldr r1,[sp,#0x8c]
  000dc28e: str r6,[sp,#0x90]
  000dc290: mov r0,r10
  000dc292: blx 0x00074818
  000dc296: ldr r1,[sp,#0x90]
  000dc298: adds r0,r1,#0x1
  000dc29a: beq.w 0x000dc438
  000dc29e: ldr r0,[sp,#0x14]
  000dc2a0: ldr r0,[r0,#0x0]
  000dc2a2: blx 0x00071c44
  000dc2a6: str r0,[sp,#0x10]
  000dc2a8: ldr r1,[sp,#0x8c]
  000dc2aa: add r0,sp,#0x6c
  000dc2ac: blx 0x00074818
  000dc2b0: ldr r0,[sp,#0x6c]
  000dc2b2: cmp r0,#0x1
  000dc2b4: blt 0x000dc35a
  000dc2b6: movs r0,#0xc
  000dc2b8: blx 0x0006eb24
  000dc2bc: mov r5,r0
  000dc2be: blx 0x00071860
  000dc2c2: ldr r0,[sp,#0x6c]
  000dc2c4: mov r1,r5
  000dc2c6: blx 0x0007186c
  000dc2ca: str r4,[sp,#0x8]
  000dc2cc: mov r6,r5
  000dc2ce: movs r4,#0x0
  000dc2d0: mov.w r10,#0x0
  000dc2d4: b 0x000dc342
  000dc2d6: ldr r1,[sp,#0x8c]
  000dc2d8: str r4,[sp,#0x5c]
  000dc2da: add r0,sp,#0x64
  000dc2dc: blx 0x00074818
  000dc2e0: ldr r1,[sp,#0x8c]
  000dc2e2: add r0,sp,#0x60
  000dc2e4: blx 0x00074818
  000dc2e8: ldr r1,[sp,#0x8c]
  000dc2ea: add r0,sp,#0x5c
  000dc2ec: blx 0x00074818
  000dc2f0: ldr r1,[sp,#0x8c]
  000dc2f2: strb.w r4,[sp,#0x58]
  000dc2f6: add r0,sp,#0x58
  000dc2f8: blx 0x00074854
  000dc2fc: ldr r0,[0x000dc458]
  000dc2fe: ldr r1,[sp,#0x64]
  000dc300: add r0,pc
  000dc302: ldr r5,[sp,#0x60]
  000dc304: ldr r0,[r0,#0x0]
  000dc306: ldr r0,[r0,#0x0]
  000dc308: ldr r0,[r0,#0x4]
  000dc30a: ldr.w r8,[r0,r1,lsl #0x2]
  000dc30e: mov r0,r8
  000dc310: blx 0x00071cbc
  000dc314: mov r2,r0
  000dc316: mov r0,r8
  000dc318: mov r1,r5
  000dc31a: blx 0x0007486c
  000dc31e: ldr r1,[r6,#0x4]
  000dc320: str.w r0,[r1,r10,lsl #0x2]
  000dc324: ldr r0,[r6,#0x4]
  000dc326: ldr r1,[sp,#0x5c]
  000dc328: ldr.w r0,[r0,r10,lsl #0x2]
  000dc32c: blx 0x00073864
  000dc330: ldr r0,[r6,#0x4]
  000dc332: ldrb.w r1,[sp,#0x58]
  000dc336: ldr.w r0,[r0,r10,lsl #0x2]
  000dc33a: blx 0x00073330
  000dc33e: add.w r10,r10,#0x1
  000dc342: ldr r0,[sp,#0x6c]
  000dc344: cmp r10,r0
  000dc346: blt 0x000dc2d6
  000dc348: ldr r0,[sp,#0x10]
  000dc34a: mov r1,r6
  000dc34c: movs r2,#0x0
  000dc34e: blx 0x000736e4
  000dc352: ldr r4,[sp,#0x8]
  000dc354: add.w r10,sp,#0x90
  000dc358: movs r6,#0x0
  000dc35a: ldr r1,[sp,#0x8c]
  000dc35c: str r6,[sp,#0x64]
  000dc35e: add r0,sp,#0x64
  000dc360: blx 0x00074818
  000dc364: ldr r0,[sp,#0x64]
  000dc366: cmp r0,#0x1
  000dc368: blt 0x000dc3d2
  000dc36a: movs r0,#0xc
  000dc36c: blx 0x0006eb24
  000dc370: mov r8,r0
  000dc372: blx 0x000719b0
  000dc376: ldr r0,[sp,#0x64]
  000dc378: mov r1,r8
  000dc37a: blx 0x00071a28
  000dc37e: movs r5,#0x0
  000dc380: b 0x000dc3c2
  000dc382: ldr r1,[sp,#0x8c]
  000dc384: str r6,[sp,#0x60]
  000dc386: add r0,sp,#0x60
  000dc388: blx 0x00074818
  000dc38c: ldr r0,[sp,#0x18]
  000dc38e: ldr r1,[sp,#0x60]
  000dc390: ldr r0,[r0,#0x0]
  000dc392: ldr r0,[r0,#0x4]
  000dc394: ldr.w r0,[r0,r1,lsl #0x2]
  000dc398: mov.w r1,#0xffffffff
  000dc39c: blx 0x000719d4
  000dc3a0: ldr.w r1,[r8,#0x4]
  000dc3a4: str.w r0,[r1,r5,lsl #0x2]
  000dc3a8: ldr r1,[sp,#0x8c]
  000dc3aa: str r6,[sp,#0x5c]
  000dc3ac: add r0,sp,#0x5c
  000dc3ae: blx 0x00074818
  000dc3b2: ldr.w r0,[r8,#0x4]
  000dc3b6: ldr r1,[sp,#0x5c]
  000dc3b8: ldr.w r0,[r0,r5,lsl #0x2]
  000dc3bc: blx 0x000719ec
  000dc3c0: adds r5,#0x1
  000dc3c2: ldr r0,[sp,#0x64]
  000dc3c4: cmp r5,r0
  000dc3c6: blt 0x000dc382
  000dc3c8: ldr r0,[sp,#0x10]
  000dc3ca: mov r1,r8
  000dc3cc: movs r2,#0x0
  000dc3ce: blx 0x000736fc
  000dc3d2: ldr r1,[sp,#0x8c]
  000dc3d4: str r6,[sp,#0x60]
  000dc3d6: add r0,sp,#0x60
  000dc3d8: blx 0x00074818
  000dc3dc: ldr r0,[sp,#0x60]
  000dc3de: cmp r0,#0x1
  000dc3e0: blt 0x000dc41e
  000dc3e2: movs r0,#0xc
  000dc3e4: blx 0x0006eb24
  000dc3e8: mov r5,r0
  000dc3ea: blx 0x00071ad0
  000dc3ee: ldr r0,[sp,#0x60]
  000dc3f0: mov r1,r5
  000dc3f2: blx 0x00071adc
  000dc3f6: mov r6,r4
  000dc3f8: movs r4,#0x0
  000dc3fa: b 0x000dc40c
  000dc3fc: ldr r1,[sp,#0x8c]
  000dc3fe: mov r0,r9
  000dc400: blx 0x00074890
  000dc404: ldr r1,[r5,#0x4]
  000dc406: str.w r0,[r1,r4,lsl #0x2]
  000dc40a: adds r4,#0x1
  000dc40c: ldr r0,[sp,#0x60]
  000dc40e: cmp r4,r0
  000dc410: blt 0x000dc3fc
  000dc412: ldr r0,[sp,#0x10]
  000dc414: mov r1,r5
  000dc416: blx 0x00073720
  000dc41a: mov r4,r6
  000dc41c: movs r6,#0x0
  000dc41e: ldr r1,[sp,#0x8c]
  000dc420: strb.w r6,[sp,#0x5c]
  000dc424: add r0,sp,#0x5c
  000dc426: blx 0x00074854
  000dc42a: ldrb.w r1,[sp,#0x5c]
  000dc42e: ldr r0,[sp,#0x10]
  000dc430: blx 0x00074470
  000dc434: ldr r3,[sp,#0x10]
  000dc436: b 0x000dc43a
  000dc438: movs r3,#0x0
  000dc43a: ldr r0,[r4,#0x0]
  000dc43c: ldr r2,[sp,#0x1c]
  000dc43e: cmp r2,r0
  000dc440: bne 0x000dc45c
  000dc442: ldr r1,[sp,#0xc]
  000dc444: str r3,[r1,#0x0]
  000dc446: b 0x000dc462
  000dc45c: ldr r1,[r4,#0x4]
  000dc45e: str.w r3,[r1,r2,lsl #0x2]
  000dc462: adds r2,#0x1
  000dc464: adds r0,#0x1
  000dc466: str r2,[sp,#0x1c]
  000dc468: cmp r2,r0
  000dc46a: bcc.w 0x000dc28c
  000dc46e: ldr r1,[sp,#0x8c]
  000dc470: movs r0,#0x0
  000dc472: str r4,[sp,#0x8]
  000dc474: str.w r4,[r11,#0x5c]
  000dc478: str r0,[sp,#0x64]
  000dc47a: add r0,sp,#0x64
  000dc47c: blx 0x00074818
  000dc480: ldr.w r10,[sp,#0x64]
  000dc484: movs r0,#0x4
  000dc486: umull r0,r1,r10,r0
  000dc48a: cmp r1,#0x0
  000dc48c: it ne
  000dc48e: mov.ne r1,#0x1
  000dc490: cmp r1,#0x0
  000dc492: it ne
  000dc494: mov.ne.w r0,#0xffffffff
  000dc498: blx 0x0006ec08
  000dc49c: mov r8,r0
  000dc49e: movs r6,#0x0
  000dc4a0: mov r5,r0
  000dc4a2: b 0x000dc4b4
  000dc4a4: ldr r1,[sp,#0x8c]
  000dc4a6: mov r0,r5
  000dc4a8: blx 0x00074818
  000dc4ac: ldr.w r10,[sp,#0x64]
  000dc4b0: adds r5,#0x4
  000dc4b2: adds r6,#0x1
  000dc4b4: cmp r6,r10
  000dc4b6: blt 0x000dc4a4
  000dc4b8: movs r0,#0x8
  000dc4ba: blx 0x0006eb24
  000dc4be: mov r5,r0
  000dc4c0: blx 0x00073aec
  000dc4c4: str.w r5,[r11,#0x13c]
  000dc4c8: mov r0,r5
  000dc4ca: mov r1,r8
  000dc4cc: blx 0x0007489c
  000dc4d0: movs r0,#0xc
  000dc4d2: blx 0x0006eb24
  000dc4d6: mov r8,r0
  000dc4d8: blx 0x00073a8c
  000dc4dc: ldr r1,[sp,#0x8c]
  000dc4de: movs r0,#0x0
  000dc4e0: str r0,[sp,#0x60]
  000dc4e2: add r0,sp,#0x60
  000dc4e4: blx 0x00074818
  000dc4e8: ldr r0,[sp,#0x60]
  000dc4ea: mov r1,r8
  000dc4ec: blx 0x00073a98
  000dc4f0: ldr r0,[0x000dc878]
  000dc4f2: mov.w r10,#0x0
  000dc4f6: add r0,pc
  000dc4f8: ldr r0,[r0,#0x0]
  000dc4fa: str r0,[sp,#0x1c]
  000dc4fc: b 0x000dc596
  000dc4fe: movs r0,#0x2c
  000dc500: blx 0x0006eb24
  000dc504: mov r5,r0
  000dc506: ldr r0,[sp,#0x1c]
  000dc508: ldr r0,[r0,#0x0]
  000dc50a: blx 0x000748a8
  000dc50e: ldr r0,[r0,#0x4]
  000dc510: ldr.w r0,[r0,r10,lsl #0x2]
  000dc514: blx 0x000733fc
  000dc518: mov r1,r0
  000dc51a: mov r0,r5
  000dc51c: blx 0x00073aa4
  000dc520: ldr.w r0,[r8,#0x4]
  000dc524: movs r6,#0x0
  000dc526: movs r4,#0x0
  000dc528: str.w r5,[r0,r10,lsl #0x2]
  000dc52c: ldr.w r0,[r8,#0x4]
  000dc530: ldr.w r5,[r0,r10,lsl #0x2]
  000dc534: b 0x000dc544
  000dc536: ldr r0,[r0,#0x4]
  000dc538: ldr r1,[sp,#0x8c]
  000dc53a: add r0,r6
  000dc53c: blx 0x00074818
  000dc540: adds r6,#0x4
  000dc542: adds r4,#0x1
  000dc544: ldr r0,[r5,#0x0]
  000dc546: ldr r1,[r0,#0x0]
  000dc548: cmp r4,r1
  000dc54a: bcc 0x000dc536
  000dc54c: ldr r1,[sp,#0x8c]
  000dc54e: adds r0,r5,#0x4
  000dc550: blx 0x00074818
  000dc554: ldr r1,[sp,#0x8c]
  000dc556: add.w r0,r5,#0x8
  000dc55a: blx 0x00074854
  000dc55e: ldr r1,[sp,#0x8c]
  000dc560: add.w r0,r5,#0xc
  000dc564: blx 0x00074818
  000dc568: ldr r1,[sp,#0x8c]
  000dc56a: add.w r0,r5,#0x10
  000dc56e: blx 0x00074818
  000dc572: add r0,sp,#0x90
  000dc574: blx 0x0006efbc
  000dc578: ldr r1,[sp,#0x8c]
  000dc57a: add r0,sp,#0x90
  000dc57c: movs r2,#0x1
  000dc57e: blx 0x00074824
  000dc582: add.w r0,r5,#0x14
  000dc586: add r1,sp,#0x90
  000dc588: blx 0x0006f2b0
  000dc58c: add r0,sp,#0x90
  000dc58e: blx 0x0006ee30
  000dc592: add.w r10,r10,#0x1
  000dc596: ldr.w r0,[r8,#0x0]
  000dc59a: cmp r10,r0
  000dc59c: bcc 0x000dc4fe
  000dc59e: ldr r1,[sp,#0x8c]
  000dc5a0: movs r0,#0x0
  000dc5a2: str.w r8,[r11,#0x140]
  000dc5a6: str r0,[sp,#0x5c]
  000dc5a8: add r0,sp,#0x5c
  000dc5aa: blx 0x00074818
  000dc5ae: ldr r0,[sp,#0x5c]
  000dc5b0: cmp r0,#0x1
  000dc5b2: blt 0x000dc642
  000dc5b4: movs r0,#0xc
  000dc5b6: blx 0x0006eb24
  000dc5ba: mov r4,r0
  000dc5bc: blx 0x00073630
  000dc5c0: ldr r0,[sp,#0x5c]
  000dc5c2: mov r1,r4
  000dc5c4: blx 0x000748b4
  000dc5c8: movs r6,#0x0
  000dc5ca: mov.w r10,#0x0
  000dc5ce: b 0x000dc636
  000dc5d0: ldr r1,[sp,#0x8c]
  000dc5d2: str r6,[sp,#0x50]
  000dc5d4: add r0,sp,#0x58
  000dc5d6: blx 0x00074818
  000dc5da: ldr r1,[sp,#0x8c]
  000dc5dc: add r0,sp,#0x54
  000dc5de: blx 0x00074818
  000dc5e2: ldr r1,[sp,#0x8c]
  000dc5e4: add r0,sp,#0x50
  000dc5e6: blx 0x00074818
  000dc5ea: add r0,sp,#0x90
  000dc5ec: blx 0x0006efbc
  000dc5f0: ldr r1,[sp,#0x8c]
  000dc5f2: add r0,sp,#0x90
  000dc5f4: movs r2,#0x1
  000dc5f6: blx 0x00074824
  000dc5fa: movs r0,#0x18
  000dc5fc: blx 0x0006eb24
  000dc600: ldr.w r8,[sp,#0x58]
  000dc604: mov r5,r0
  000dc606: add r0,sp,#0x44
  000dc608: add r1,sp,#0x90
  000dc60a: movs r2,#0x0
  000dc60c: blx 0x0006f028
  000dc610: ldrd r3,r0,[sp,#0x50]
  000dc614: add r2,sp,#0x44
  000dc616: str r0,[sp,#0x0]
  000dc618: mov r0,r5
  000dc61a: mov r1,r8
  000dc61c: blx 0x000748c0
  000dc620: ldr r0,[r4,#0x4]
  000dc622: str.w r5,[r0,r10,lsl #0x2]
  000dc626: add r0,sp,#0x44
  000dc628: blx 0x0006ee30
  000dc62c: add r0,sp,#0x90
  000dc62e: blx 0x0006ee30
  000dc632: add.w r10,r10,#0x1
  000dc636: ldr r0,[r4,#0x0]
  000dc638: cmp r10,r0
  000dc63a: bcc 0x000dc5d0
  000dc63c: str.w r4,[r11,#0x144]
  000dc640: b 0x000dc648
  000dc642: movs r0,#0x0
  000dc644: str.w r0,[r11,#0x144]
  000dc648: ldr r1,[sp,#0x8c]
  000dc64a: movs r0,#0x0
  000dc64c: str r0,[sp,#0x58]
  000dc64e: add r0,sp,#0x58
  000dc650: blx 0x00074818
  000dc654: ldr r0,[sp,#0x58]
  000dc656: cmp r0,#0x1
  000dc658: blt 0x000dc70e
  000dc65a: movs r0,#0xc
  000dc65c: blx 0x0006eb24
  000dc660: mov r8,r0
  000dc662: blx 0x0006f628
  000dc666: ldr r0,[sp,#0x58]
  000dc668: mov r1,r8
  000dc66a: blx 0x0006fe08
  000dc66e: add r6,sp,#0x90
  000dc670: mov.w r10,#0x0
  000dc674: b 0x000dc6a8
  000dc676: mov r0,r6
  000dc678: blx 0x0006efbc
  000dc67c: ldr r1,[sp,#0x8c]
  000dc67e: mov r0,r6
  000dc680: movs r2,#0x1
  000dc682: blx 0x00074824
  000dc686: movs r0,#0xc
  000dc688: blx 0x0006eb24
  000dc68c: mov r5,r0
  000dc68e: mov r1,r6
  000dc690: movs r2,#0x0
  000dc692: blx 0x0006f028
  000dc696: ldr.w r0,[r8,#0x4]
  000dc69a: str.w r5,[r0,r10,lsl #0x2]
  000dc69e: mov r0,r6
  000dc6a0: blx 0x0006ee30
  000dc6a4: add.w r10,r10,#0x1
  000dc6a8: ldr r0,[sp,#0x58]
  000dc6aa: cmp r10,r0
  000dc6ac: blt 0x000dc676
  000dc6ae: ldr r1,[sp,#0x8c]
  000dc6b0: str.w r8,[r11,#0x14c]
  000dc6b4: add.w r0,r11,#0x150
  000dc6b8: blx 0x00074818
  000dc6bc: ldr r1,[sp,#0x8c]
  000dc6be: add.w r0,r11,#0x154
  000dc6c2: blx 0x00074818
  000dc6c6: ldr r1,[sp,#0x8c]
  000dc6c8: movs r0,#0x0
  000dc6ca: str r0,[sp,#0x90]
  000dc6cc: add r0,sp,#0x90
  000dc6ce: blx 0x00074818
  000dc6d2: ldr r5,[sp,#0x90]
  000dc6d4: movs r0,#0x4
  000dc6d6: umull r0,r1,r5,r0
  000dc6da: cmp r1,#0x0
  000dc6dc: it ne
  000dc6de: mov.ne r1,#0x1
  000dc6e0: cmp r1,#0x0
  000dc6e2: it ne
  000dc6e4: mov.ne.w r0,#0xffffffff
  000dc6e8: blx 0x0006ec08
  000dc6ec: movs r6,#0x0
  000dc6ee: movs r4,#0x0
  000dc6f0: str.w r0,[r11,#0x158]
  000dc6f4: b 0x000dc708
  000dc6f6: ldr.w r0,[r11,#0x158]
  000dc6fa: ldr r1,[sp,#0x8c]
  000dc6fc: add r0,r6
  000dc6fe: blx 0x00074818
  000dc702: ldr r5,[sp,#0x90]
  000dc704: adds r6,#0x4
  000dc706: adds r4,#0x1
  000dc708: cmp r4,r5
  000dc70a: blt 0x000dc6f6
  000dc70c: b 0x000dc714
  000dc70e: movs r0,#0x0
  000dc710: str.w r0,[r11,#0x14c]
  000dc714: ldr r1,[sp,#0x8c]
  000dc716: add.w r0,r11,#0x15c
  000dc71a: blx 0x00074818
  000dc71e: ldr r1,[sp,#0x8c]
  000dc720: movs r0,#0x0
  000dc722: str r0,[sp,#0x90]
  000dc724: add r0,sp,#0x90
  000dc726: blx 0x00074818
  000dc72a: movs r0,#0xc
  000dc72c: blx 0x0006eb24
  000dc730: mov r5,r0
  000dc732: blx 0x000735a0
  000dc736: ldr r0,[sp,#0x90]
  000dc738: str.w r5,[r11,#0x160]
  000dc73c: mov r1,r5
  000dc73e: blx 0x000735c4
  000dc742: movs r4,#0x0
  000dc744: b 0x000dc752
  000dc746: ldr r0,[r0,#0x4]
  000dc748: ldr r1,[sp,#0x8c]
  000dc74a: add r0,r4
  000dc74c: blx 0x00074854
  000dc750: adds r4,#0x1
  000dc752: ldr.w r0,[r11,#0x160]
  000dc756: ldr r1,[r0,#0x0]
  000dc758: cmp r4,r1
  000dc75a: bcc 0x000dc746
  000dc75c: ldr r1,[sp,#0x8c]
  000dc75e: add r0,sp,#0x90
  000dc760: blx 0x00074818
  000dc764: movs r0,#0xc
  000dc766: blx 0x0006eb24
  000dc76a: mov r5,r0
  000dc76c: blx 0x000701f8
  000dc770: ldr r0,[sp,#0x90]
  000dc772: str.w r5,[r11,#0x168]
  000dc776: mov r1,r5
  000dc778: blx 0x00071a4c
  000dc77c: movs r5,#0x0
  000dc77e: movs r4,#0x0
  000dc780: b 0x000dc790
  000dc782: ldr r0,[r0,#0x4]
  000dc784: ldr r1,[sp,#0x8c]
  000dc786: add r0,r5
  000dc788: blx 0x00074818
  000dc78c: adds r5,#0x4
  000dc78e: adds r4,#0x1
  000dc790: ldr.w r0,[r11,#0x168]
  000dc794: ldr r1,[r0,#0x0]
  000dc796: cmp r4,r1
  000dc798: bcc 0x000dc782
  000dc79a: ldr r1,[sp,#0x8c]
  000dc79c: add r0,sp,#0x90
  000dc79e: blx 0x00074818
  000dc7a2: movs r0,#0xc
  000dc7a4: blx 0x0006eb24
  000dc7a8: mov r5,r0
  000dc7aa: blx 0x000701f8
  000dc7ae: ldr r0,[sp,#0x90]
  000dc7b0: str.w r5,[r11,#0x164]
  000dc7b4: mov r1,r5
  000dc7b6: blx 0x00071a4c
  000dc7ba: movs r5,#0x0
  000dc7bc: movs r4,#0x0
  000dc7be: b 0x000dc7ce
  000dc7c0: ldr r0,[r0,#0x4]
  000dc7c2: ldr r1,[sp,#0x8c]
  000dc7c4: add r0,r5
  000dc7c6: blx 0x00074818
  000dc7ca: adds r5,#0x4
  000dc7cc: adds r4,#0x1
  000dc7ce: ldr.w r0,[r11,#0x164]
  000dc7d2: ldr r1,[r0,#0x0]
  000dc7d4: cmp r4,r1
  000dc7d6: bcc 0x000dc7c0
  000dc7d8: ldr r1,[sp,#0x8c]
  000dc7da: add r0,sp,#0x90
  000dc7dc: blx 0x00074818
  000dc7e0: movs r0,#0xc
  000dc7e2: blx 0x0006eb24
  000dc7e6: mov r5,r0
  000dc7e8: blx 0x000701f8
  000dc7ec: ldr r0,[sp,#0x90]
  000dc7ee: str.w r5,[r11,#0x170]
  000dc7f2: mov r1,r5
  000dc7f4: blx 0x00071a4c
  000dc7f8: movs r5,#0x0
  000dc7fa: movs r4,#0x0
  000dc7fc: b 0x000dc80c
  000dc7fe: ldr r0,[r0,#0x4]
  000dc800: ldr r1,[sp,#0x8c]
  000dc802: add r0,r5
  000dc804: blx 0x00074818
  000dc808: adds r5,#0x4
  000dc80a: adds r4,#0x1
  000dc80c: ldr.w r0,[r11,#0x170]
  000dc810: ldr r1,[r0,#0x0]
  000dc812: cmp r4,r1
  000dc814: bcc 0x000dc7fe
  000dc816: ldr r1,[sp,#0x8c]
  000dc818: add r0,sp,#0x90
  000dc81a: blx 0x00074818
  000dc81e: movs r0,#0xc
  000dc820: blx 0x0006eb24
  000dc824: mov r5,r0
  000dc826: blx 0x000701f8
  000dc82a: ldr r0,[sp,#0x90]
  000dc82c: str.w r5,[r11,#0x16c]
  000dc830: mov r1,r5
  000dc832: blx 0x00071a4c
  000dc836: movs r5,#0x0
  000dc838: movs r4,#0x0
  000dc83a: b 0x000dc84a
  000dc83c: ldr r0,[r0,#0x4]
  000dc83e: ldr r1,[sp,#0x8c]
  000dc840: add r0,r5
  000dc842: blx 0x00074818
  000dc846: adds r5,#0x4
  000dc848: adds r4,#0x1
  000dc84a: ldr.w r0,[r11,#0x16c]
  000dc84e: ldr r1,[r0,#0x0]
  000dc850: cmp r4,r1
  000dc852: bcc 0x000dc83c
  000dc854: ldr r1,[sp,#0x8c]
  000dc856: add r0,sp,#0x90
  000dc858: blx 0x00074818
  000dc85c: movs r0,#0xc
  000dc85e: blx 0x0006eb24
  000dc862: mov r5,r0
  000dc864: blx 0x000735a0
  000dc868: ldr r0,[sp,#0x90]
  000dc86a: str.w r5,[r11,#0x174]
  000dc86e: mov r1,r5
  000dc870: blx 0x000735c4
  000dc874: movs r4,#0x0
  000dc876: b 0x000dc888
  000dc87c: ldr r0,[r0,#0x4]
  000dc87e: ldr r1,[sp,#0x8c]
  000dc880: add r0,r4
  000dc882: blx 0x00074854
  000dc886: adds r4,#0x1
  000dc888: ldr.w r0,[r11,#0x174]
  000dc88c: ldr r1,[r0,#0x0]
  000dc88e: cmp r4,r1
  000dc890: bcc 0x000dc87c
  000dc892: ldr r1,[sp,#0x8c]
  000dc894: movs r0,#0x0
  000dc896: str r0,[sp,#0x54]
  000dc898: add r0,sp,#0x54
  000dc89a: blx 0x00074818
  000dc89e: movs r0,#0xc
  000dc8a0: blx 0x0006eb24
  000dc8a4: mov r5,r0
  000dc8a6: blx 0x00071ad0
  000dc8aa: ldr r0,[sp,#0x54]
  000dc8ac: str.w r5,[r11,#0x148]
  000dc8b0: mov r1,r5
  000dc8b2: blx 0x00071adc
  000dc8b6: movs r4,#0x0
  000dc8b8: b 0x000dc8ce
  000dc8ba: ldr r1,[sp,#0x8c]
  000dc8bc: mov r0,r9
  000dc8be: blx 0x00074890
  000dc8c2: ldr.w r1,[r11,#0x148]
  000dc8c6: ldr r1,[r1,#0x4]
  000dc8c8: str.w r0,[r1,r4,lsl #0x2]
  000dc8cc: adds r4,#0x1
  000dc8ce: ldr.w r0,[r11,#0x148]
  000dc8d2: ldr r0,[r0,#0x0]
  000dc8d4: cmp r4,r0
  000dc8d6: bcc 0x000dc8ba
  000dc8d8: ldr r1,[sp,#0x8c]
  000dc8da: add.w r0,r11,#0xe4
  000dc8de: blx 0x00074854
  000dc8e2: ldr r1,[sp,#0x8c]
  000dc8e4: add.w r0,r11,#0xe5
  000dc8e8: blx 0x00074854
  000dc8ec: ldr r1,[sp,#0x8c]
  000dc8ee: add.w r0,r11,#0xe6
  000dc8f2: blx 0x00074854
  000dc8f6: ldr r1,[sp,#0x8c]
  000dc8f8: add.w r0,r11,#0xe7
  000dc8fc: blx 0x00074854
  000dc900: ldr r1,[sp,#0x8c]
  000dc902: add.w r0,r11,#0xe8
  000dc906: blx 0x00074854
  000dc90a: ldr r1,[sp,#0x8c]
  000dc90c: add.w r0,r11,#0xe9
  000dc910: blx 0x00074854
  000dc914: ldr r1,[sp,#0x8c]
  000dc916: add.w r0,r11,#0xea
  000dc91a: blx 0x00074854
  000dc91e: ldr r1,[sp,#0x8c]
  000dc920: add.w r0,r11,#0xeb
  000dc924: blx 0x00074854
  000dc928: ldr r1,[sp,#0x8c]
  000dc92a: add.w r0,r11,#0xec
  000dc92e: blx 0x00074854
  000dc932: ldr r1,[sp,#0x8c]
  000dc934: add.w r0,r11,#0xed
  000dc938: blx 0x00074854
  000dc93c: ldr r1,[sp,#0x8c]
  000dc93e: add.w r0,r11,#0xee
  000dc942: blx 0x00074854
  000dc946: ldr r1,[sp,#0x8c]
  000dc948: add.w r0,r11,#0xef
  000dc94c: blx 0x00074854
  000dc950: ldr r1,[sp,#0x8c]
  000dc952: add.w r0,r11,#0xf0
  000dc956: blx 0x00074854
  000dc95a: ldr r1,[sp,#0x8c]
  000dc95c: add.w r0,r11,#0xf1
  000dc960: blx 0x00074854
  000dc964: ldr r1,[sp,#0x8c]
  000dc966: add.w r0,r11,#0xf2
  000dc96a: blx 0x00074854
  000dc96e: ldr r1,[sp,#0x8c]
  000dc970: add.w r0,r11,#0xf3
  000dc974: blx 0x00074854
  000dc978: ldr r1,[sp,#0x8c]
  000dc97a: add.w r0,r11,#0xf4
  000dc97e: blx 0x00074854
  000dc982: ldr r1,[sp,#0x8c]
  000dc984: add.w r0,r11,#0xf5
  000dc988: blx 0x00074854
  000dc98c: ldr r1,[sp,#0x8c]
  000dc98e: add.w r0,r11,#0xf6
  000dc992: blx 0x00074854
  000dc996: ldr r1,[sp,#0x8c]
  000dc998: add.w r0,r11,#0xf7
  000dc99c: blx 0x00074854
  000dc9a0: ldr r1,[sp,#0x8c]
  000dc9a2: add.w r0,r11,#0xf8
  000dc9a6: blx 0x00074854
  000dc9aa: ldr r1,[sp,#0x8c]
  000dc9ac: add.w r0,r11,#0xf9
  000dc9b0: blx 0x00074854
  000dc9b4: ldr r1,[sp,#0x8c]
  000dc9b6: add.w r0,r11,#0xfa
  000dc9ba: blx 0x00074854
  000dc9be: ldr r1,[sp,#0x8c]
  000dc9c0: add.w r0,r11,#0xfb
  000dc9c4: blx 0x00074854
  000dc9c8: ldr r1,[sp,#0x8c]
  000dc9ca: add.w r0,r11,#0xfc
  000dc9ce: blx 0x00074854
  000dc9d2: ldr r1,[sp,#0x8c]
  000dc9d4: add.w r0,r11,#0xfe
  000dc9d8: blx 0x00074854
  000dc9dc: ldr r1,[sp,#0x8c]
  000dc9de: add.w r0,r11,#0xfd
  000dc9e2: blx 0x00074854
  000dc9e6: ldr r1,[sp,#0x8c]
  000dc9e8: add.w r0,r11,#0xff
  000dc9ec: blx 0x00074854
  000dc9f0: ldr r1,[sp,#0x8c]
  000dc9f2: add.w r0,r11,#0x100
  000dc9f6: blx 0x00074854
  000dc9fa: ldr r1,[sp,#0x8c]
  000dc9fc: add.w r0,r11,#0x11c
  000dca00: blx 0x00074830
  000dca04: ldr r1,[sp,#0x8c]
  000dca06: add.w r0,r11,#0xc8
  000dca0a: blx 0x0007480c
  000dca0e: ldr r1,[sp,#0x8c]
  000dca10: addw r0,r11,#0x101
  000dca14: blx 0x00074854
  000dca18: ldr r1,[sp,#0x8c]
  000dca1a: add.w r0,r11,#0x102
  000dca1e: blx 0x00074854
  000dca22: ldr r1,[sp,#0x8c]
  000dca24: movs r0,#0x0
  000dca26: str r0,[sp,#0x50]
  000dca28: add r0,sp,#0x50
  000dca2a: blx 0x00074818
  000dca2e: ldr r0,[sp,#0x50]
  000dca30: cmp r0,#0x1
  000dca32: blt.w 0x000dcbd6
  000dca36: ldr r1,[sp,#0x8c]
  000dca38: movs r0,#0x0
  000dca3a: str r0,[sp,#0x40]
  000dca3c: add r0,sp,#0x40
  000dca3e: blx 0x00074818
  000dca42: ldr r0,[sp,#0x18]
  000dca44: ldr r1,[sp,#0x40]
  000dca46: ldr r0,[r0,#0x0]
  000dca48: ldr r0,[r0,#0x4]
  000dca4a: ldr.w r0,[r0,r1,lsl #0x2]
  000dca4e: mov.w r1,#0xffffffff
  000dca52: blx 0x000719d4
  000dca56: ldr r1,[sp,#0x8c]
  000dca58: str.w r0,[r11,#0x134]
  000dca5c: movs r0,#0x0
  000dca5e: str r0,[sp,#0x3c]
  000dca60: add r0,sp,#0x3c
  000dca62: blx 0x00074818
  000dca66: ldr r1,[sp,#0x3c]
  000dca68: ldr.w r0,[r11,#0x134]
  000dca6c: blx 0x000719ec
  000dca70: ldr r1,[sp,#0x8c]
  000dca72: movs r0,#0x0
  000dca74: str r0,[sp,#0x38]
  000dca76: add r0,sp,#0x38
  000dca78: blx 0x00074818
  000dca7c: ldr r0,[sp,#0x38]
  000dca7e: cmp r0,#0x1
  000dca80: blt 0x000dcb28
  000dca82: movs r0,#0xc
  000dca84: blx 0x0006eb24
  000dca88: mov r9,r0
  000dca8a: blx 0x00071860
  000dca8e: ldr r0,[sp,#0x38]
  000dca90: mov r1,r9
  000dca92: blx 0x0007186c
  000dca96: ldr r0,[0x000dce48]
  000dca98: add r4,sp,#0x34
  000dca9a: mov.w r8,#0x0
  000dca9e: mov.w r10,#0x0
  000dcaa2: add r0,pc
  000dcaa4: ldr r0,[r0,#0x0]
  000dcaa6: str r0,[sp,#0x1c]
  000dcaa8: b 0x000dcb18
  000dcaaa: ldr r1,[sp,#0x8c]
  000dcaac: str.w r8,[sp,#0x34]
  000dcab0: mov r0,r4
  000dcab2: blx 0x00074818
  000dcab6: ldr r0,[sp,#0x34]
  000dcab8: adds r0,#0x1
  000dcaba: beq 0x000dcb0c
  000dcabc: ldr r1,[sp,#0x8c]
  000dcabe: str.w r8,[sp,#0x30]
  000dcac2: add r0,sp,#0x30
  000dcac4: blx 0x00074818
  000dcac8: ldr r0,[sp,#0x1c]
  000dcaca: ldr r1,[sp,#0x34]
  000dcacc: ldr r6,[sp,#0x30]
  000dcace: ldr r0,[r0,#0x0]
  000dcad0: ldr r0,[r0,#0x4]
  000dcad2: ldr.w r5,[r0,r1,lsl #0x2]
  000dcad6: mov r0,r5
  000dcad8: blx 0x00071cbc
  000dcadc: mov r2,r0
  000dcade: mov r0,r5
  000dcae0: mov r1,r6
  000dcae2: blx 0x0007486c
  000dcae6: ldr.w r1,[r9,#0x4]
  000dcaea: str.w r0,[r1,r10,lsl #0x2]
  000dcaee: ldr r1,[sp,#0x8c]
  000dcaf0: strb.w r8,[sp,#0x2c]
  000dcaf4: add r0,sp,#0x2c
  000dcaf6: blx 0x00074854
  000dcafa: ldr.w r0,[r9,#0x4]
  000dcafe: ldrb.w r1,[sp,#0x2c]
  000dcb02: ldr.w r0,[r0,r10,lsl #0x2]
  000dcb06: blx 0x00073330
  000dcb0a: b 0x000dcb14
  000dcb0c: ldr.w r0,[r9,#0x4]
  000dcb10: str.w r8,[r0,r10,lsl #0x2]
  000dcb14: add.w r10,r10,#0x1
  000dcb18: ldr r0,[sp,#0x38]
  000dcb1a: cmp r10,r0
  000dcb1c: blt 0x000dcaaa
  000dcb1e: ldr.w r0,[r11,#0x134]
  000dcb22: mov r1,r9
  000dcb24: blx 0x00074878
  000dcb28: ldr r1,[sp,#0x8c]
  000dcb2a: movs r0,#0x0
  000dcb2c: str r0,[sp,#0x34]
  000dcb2e: add r0,sp,#0x34
  000dcb30: blx 0x00074818
  000dcb34: ldr r0,[sp,#0x34]
  000dcb36: cmp r0,#0x1
  000dcb38: blt 0x000dcbd6
  000dcb3a: movs r0,#0xc
  000dcb3c: blx 0x0006eb24
  000dcb40: mov r5,r0
  000dcb42: blx 0x00071860
  000dcb46: ldr r0,[sp,#0x34]
  000dcb48: mov r1,r5
  000dcb4a: blx 0x0007186c
  000dcb4e: ldr r0,[0x000dce4c]
  000dcb50: movs r4,#0x0
  000dcb52: mov.w r8,#0x0
  000dcb56: add r0,pc
  000dcb58: ldr.w r9,[r0,#0x0]
  000dcb5c: b 0x000dcbc6
  000dcb5e: ldr r1,[sp,#0x8c]
  000dcb60: str r4,[sp,#0x28]
  000dcb62: add r0,sp,#0x30
  000dcb64: blx 0x00074818
  000dcb68: ldr r1,[sp,#0x8c]
  000dcb6a: add r0,sp,#0x2c
  000dcb6c: blx 0x00074818
  000dcb70: ldr r1,[sp,#0x8c]
  000dcb72: add r0,sp,#0x28
  000dcb74: blx 0x00074818
  000dcb78: ldr r1,[sp,#0x8c]
  000dcb7a: strb.w r4,[sp,#0x24]
  000dcb7e: add r0,sp,#0x24
  000dcb80: blx 0x00074854
  000dcb84: ldr.w r0,[r9,#0x0]
  000dcb88: ldr r1,[sp,#0x30]
  000dcb8a: ldr r6,[sp,#0x2c]
  000dcb8c: ldr r0,[r0,#0x4]
  000dcb8e: ldr.w r10,[r0,r1,lsl #0x2]
  000dcb92: mov r0,r10
  000dcb94: blx 0x00071cbc
  000dcb98: mov r2,r0
  000dcb9a: mov r0,r10
  000dcb9c: mov r1,r6
  000dcb9e: blx 0x0007486c
  000dcba2: ldr r1,[r5,#0x4]
  000dcba4: str.w r0,[r1,r8,lsl #0x2]
  000dcba8: ldr r0,[r5,#0x4]
  000dcbaa: ldr r1,[sp,#0x28]
  000dcbac: ldr.w r0,[r0,r8,lsl #0x2]
  000dcbb0: blx 0x00073864
  000dcbb4: ldr r0,[r5,#0x4]
  000dcbb6: ldrb.w r1,[sp,#0x24]
  000dcbba: ldr.w r0,[r0,r8,lsl #0x2]
  000dcbbe: blx 0x00073330
  000dcbc2: add.w r8,r8,#0x1
  000dcbc6: ldr r0,[sp,#0x34]
  000dcbc8: cmp r8,r0
  000dcbca: blt 0x000dcb5e
  000dcbcc: ldr.w r0,[r11,#0x134]
  000dcbd0: mov r1,r5
  000dcbd2: blx 0x00074884
  000dcbd6: movs r0,#0xc
  000dcbd8: blx 0x0006eb24
  000dcbdc: mov r5,r0
  000dcbde: blx 0x000701f8
  000dcbe2: ldr r1,[sp,#0x8c]
  000dcbe4: movs r0,#0x0
  000dcbe6: str.w r5,[r11,#0x70]
  000dcbea: str r0,[sp,#0x40]
  000dcbec: add r0,sp,#0x40
  000dcbee: blx 0x00074818
  000dcbf2: ldr r0,[sp,#0x40]
  000dcbf4: ldr.w r1,[r11,#0x70]
  000dcbf8: blx 0x00071a4c
  000dcbfc: movs r5,#0x0
  000dcbfe: movs r4,#0x0
  000dcc00: b 0x000dcc14
  000dcc02: ldr.w r0,[r11,#0x70]
  000dcc06: ldr r1,[sp,#0x8c]
  000dcc08: ldr r0,[r0,#0x4]
  000dcc0a: add r0,r5
  000dcc0c: blx 0x00074818
  000dcc10: adds r5,#0x4
  000dcc12: adds r4,#0x1
  000dcc14: ldr r0,[sp,#0x40]
  000dcc16: cmp r4,r0
  000dcc18: blt 0x000dcc02
  000dcc1a: ldr r1,[sp,#0x8c]
  000dcc1c: add.w r0,r11,#0xd0
  000dcc20: blx 0x00074818
  000dcc24: ldr r1,[sp,#0x8c]
  000dcc26: add.w r0,r11,#0xd4
  000dcc2a: blx 0x00074854
  000dcc2e: ldr r1,[sp,#0x8c]
  000dcc30: add.w r0,r11,#0xd8
  000dcc34: blx 0x00074818
  000dcc38: ldr r1,[sp,#0x8c]
  000dcc3a: add.w r0,r11,#0xdc
  000dcc3e: blx 0x00074854
  000dcc42: ldr r1,[sp,#0x8c]
  000dcc44: movs r0,#0x0
  000dcc46: str r0,[sp,#0x3c]
  000dcc48: add r0,sp,#0x3c
  000dcc4a: blx 0x00074818
  000dcc4e: ldr r0,[sp,#0x3c]
  000dcc50: cmp r0,#0x1
  000dcc52: blt 0x000dccea
  000dcc54: movs r0,#0xc
  000dcc56: blx 0x0006eb24
  000dcc5a: mov r5,r0
  000dcc5c: blx 0x00071860
  000dcc60: ldr r0,[sp,#0x3c]
  000dcc62: mov r1,r5
  000dcc64: blx 0x0007186c
  000dcc68: ldr r0,[0x000dce50]
  000dcc6a: movs r4,#0x0
  000dcc6c: mov.w r8,#0x0
  000dcc70: add r0,pc
  000dcc72: ldr.w r9,[r0,#0x0]
  000dcc76: b 0x000dcce0
  000dcc78: ldr r1,[sp,#0x8c]
  000dcc7a: str r4,[sp,#0x30]
  000dcc7c: add r0,sp,#0x38
  000dcc7e: blx 0x00074818
  000dcc82: ldr r1,[sp,#0x8c]
  000dcc84: add r0,sp,#0x34
  000dcc86: blx 0x00074818
  000dcc8a: ldr r1,[sp,#0x8c]
  000dcc8c: add r0,sp,#0x30
  000dcc8e: blx 0x00074818
  000dcc92: ldr r1,[sp,#0x8c]
  000dcc94: strb.w r4,[sp,#0x2c]
  000dcc98: add r0,sp,#0x2c
  000dcc9a: blx 0x00074854
  000dcc9e: ldr.w r0,[r9,#0x0]
  000dcca2: ldr r1,[sp,#0x38]
  000dcca4: ldr r6,[sp,#0x34]
  000dcca6: ldr r0,[r0,#0x4]
  000dcca8: ldr.w r10,[r0,r1,lsl #0x2]
  000dccac: mov r0,r10
  000dccae: blx 0x00071cbc
  000dccb2: mov r2,r0
  000dccb4: mov r0,r10
  000dccb6: mov r1,r6
  000dccb8: blx 0x0007486c
  000dccbc: ldr r1,[r5,#0x4]
  000dccbe: str.w r0,[r1,r8,lsl #0x2]
  000dccc2: ldr r0,[r5,#0x4]
  000dccc4: ldr r1,[sp,#0x30]
  000dccc6: ldr.w r0,[r0,r8,lsl #0x2]
  000dccca: blx 0x00073864
  000dccce: ldr r0,[r5,#0x4]
  000dccd0: ldrb.w r1,[sp,#0x2c]
  000dccd4: ldr.w r0,[r0,r8,lsl #0x2]
  000dccd8: blx 0x00073330
  000dccdc: add.w r8,r8,#0x1
  000dcce0: ldr r0,[sp,#0x3c]
  000dcce2: cmp r8,r0
  000dcce4: blt 0x000dcc78
  000dcce6: str.w r5,[r11,#0x180]
  000dccea: ldr r1,[sp,#0x8c]
  000dccec: movs r0,#0x0
  000dccee: str r0,[sp,#0x38]
  000dccf0: add r0,sp,#0x38
  000dccf2: blx 0x00074818
  000dccf6: ldr r0,[sp,#0x38]
  000dccf8: cmp r0,#0x1
  000dccfa: blt 0x000dcd6a
  000dccfc: movs r0,#0xc
  000dccfe: blx 0x0006eb24
  000dcd02: mov r5,r0
  000dcd04: blx 0x000719b0
  000dcd08: ldr r0,[sp,#0x38]
  000dcd0a: mov r1,r5
  000dcd0c: blx 0x00071a28
  000dcd10: add.w r9,sp,#0x34
  000dcd14: add.w r8,sp,#0x30
  000dcd18: mov.w r10,#0x0
  000dcd1c: movs r6,#0x0
  000dcd1e: b 0x000dcd60
  000dcd20: ldr r1,[sp,#0x8c]
  000dcd22: str.w r10,[sp,#0x34]
  000dcd26: mov r0,r9
  000dcd28: blx 0x00074818
  000dcd2c: ldr r0,[sp,#0x18]
  000dcd2e: ldr r1,[sp,#0x34]
  000dcd30: ldr r0,[r0,#0x0]
  000dcd32: ldr r0,[r0,#0x4]
  000dcd34: ldr.w r0,[r0,r1,lsl #0x2]
  000dcd38: mov.w r1,#0xffffffff
  000dcd3c: blx 0x000719d4
  000dcd40: ldr r1,[r5,#0x4]
  000dcd42: str.w r0,[r1,r6,lsl #0x2]
  000dcd46: ldr r1,[sp,#0x8c]
  000dcd48: str.w r10,[sp,#0x30]
  000dcd4c: mov r0,r8
  000dcd4e: blx 0x00074818
  000dcd52: ldr r0,[r5,#0x4]
  000dcd54: ldr r1,[sp,#0x30]
  000dcd56: ldr.w r0,[r0,r6,lsl #0x2]
  000dcd5a: blx 0x000719ec
  000dcd5e: adds r6,#0x1
  000dcd60: ldr r0,[sp,#0x38]
  000dcd62: cmp r6,r0
  000dcd64: blt 0x000dcd20
  000dcd66: str.w r5,[r11,#0x184]
  000dcd6a: ldr r1,[sp,#0x8c]
  000dcd6c: addw r0,r11,#0x103
  000dcd70: blx 0x00074854
  000dcd74: ldr r1,[sp,#0x8c]
  000dcd76: addw r0,r11,#0x115
  000dcd7a: blx 0x00074854
  000dcd7e: ldr r1,[sp,#0x8c]
  000dcd80: add.w r0,r11,#0x116
  000dcd84: blx 0x00074854
  000dcd88: ldr r1,[sp,#0x8c]
  000dcd8a: add r0,sp,#0x90
  000dcd8c: blx 0x00074818
  000dcd90: movs r0,#0xc
  000dcd92: blx 0x0006eb24
  000dcd96: mov r5,r0
  000dcd98: blx 0x000735a0
  000dcd9c: ldr r0,[sp,#0x90]
  000dcd9e: str.w r5,[r11,#0x178]
  000dcda2: mov r1,r5
  000dcda4: blx 0x000735c4
  000dcda8: movs r4,#0x0
  000dcdaa: b 0x000dcdb8
  000dcdac: ldr r0,[r0,#0x4]
  000dcdae: ldr r1,[sp,#0x8c]
  000dcdb0: add r0,r4
  000dcdb2: blx 0x00074854
  000dcdb6: adds r4,#0x1
  000dcdb8: ldr.w r0,[r11,#0x178]
  000dcdbc: ldr r1,[r0,#0x0]
  000dcdbe: cmp r4,r1
  000dcdc0: bcc 0x000dcdac
  000dcdc2: ldr r1,[sp,#0x8c]
  000dcdc4: add.w r5,r11,#0x1bc
  000dcdc8: mov r0,r5
  000dcdca: blx 0x00074818
  000dcdce: ldr r0,[0x000dce54]
  000dcdd0: ldr r1,[r5,#0x0]
  000dcdd2: cmp r1,r0
  000dcdd4: bne 0x000dce96
  000dcdd6: ldr r1,[sp,#0x8c]
  000dcdd8: movs r0,#0x0
  000dcdda: str r0,[sp,#0x34]
  000dcddc: add r0,sp,#0x34
  000dcdde: blx 0x00074818
  000dcde2: ldr r0,[sp,#0x34]
  000dcde4: cmp r0,#0x1
  000dcde6: blt 0x000dce20
  000dcde8: movs r0,#0xc
  000dcdea: blx 0x0006eb24
  000dcdee: mov r5,r0
  000dcdf0: blx 0x000701f8
  000dcdf4: ldr r0,[sp,#0x34]
  000dcdf6: mov r1,r5
  000dcdf8: blx 0x00071a4c
  000dcdfc: movs r6,#0x0
  000dcdfe: movs r4,#0x0
  000dce00: b 0x000dce10
  000dce02: ldr r0,[r5,#0x4]
  000dce04: ldr r1,[sp,#0x8c]
  000dce06: add r0,r6
  000dce08: blx 0x00074818
  000dce0c: adds r6,#0x4
  000dce0e: adds r4,#0x1
  000dce10: ldr r0,[sp,#0x34]
  000dce12: cmp r4,r0
  000dce14: blt 0x000dce02
  000dce16: ldr.w r0,[r11,#0x130]
  000dce1a: mov r1,r5
  000dce1c: blx 0x000748cc
  000dce20: ldr r1,[sp,#0x8c]
  000dce22: add r0,sp,#0x34
  000dce24: blx 0x00074818
  000dce28: ldr r0,[sp,#0x34]
  000dce2a: cmp r0,#0x1
  000dce2c: blt 0x000dce76
  000dce2e: movs r0,#0xc
  000dce30: blx 0x0006eb24
  000dce34: mov r5,r0
  000dce36: blx 0x000701f8
  000dce3a: ldr r0,[sp,#0x34]
  000dce3c: mov r1,r5
  000dce3e: blx 0x00071a4c
  000dce42: movs r6,#0x0
  000dce44: movs r4,#0x0
  000dce46: b 0x000dce66
  000dce58: ldr r0,[r5,#0x4]
  000dce5a: ldr r1,[sp,#0x8c]
  000dce5c: add r0,r6
  000dce5e: blx 0x00074818
  000dce62: adds r6,#0x4
  000dce64: adds r4,#0x1
  000dce66: ldr r0,[sp,#0x34]
  000dce68: cmp r4,r0
  000dce6a: blt 0x000dce58
  000dce6c: ldr.w r0,[r11,#0x134]
  000dce70: mov r1,r5
  000dce72: blx 0x000748cc
  000dce76: ldr r1,[sp,#0x8c]
  000dce78: movs r0,#0x0
  000dce7a: str r0,[sp,#0x30]
  000dce7c: add r0,sp,#0x30
  000dce7e: blx 0x00074818
  000dce82: ldr r0,[sp,#0x30]
  000dce84: cmp r0,#0x1
  000dce86: blt 0x000dcefc
  000dce88: add.w r9,sp,#0x34
  000dce8c: mov.w r8,#0x0
  000dce90: mov.w r10,#0x0
  000dce94: b 0x000dcef8
  000dce96: ldr r0,[sp,#0x8c]
  000dce98: blx 0x0006ece0
  000dce9c: ldr r6,[sp,#0x20]
  000dce9e: b.w 0x000dbf6c
  000dcea2: ldr r1,[sp,#0x8c]
  000dcea4: str.w r8,[sp,#0x34]
  000dcea8: mov r0,r9
  000dceaa: blx 0x00074818
  000dceae: ldr r0,[sp,#0x34]
  000dceb0: cmp r0,#0x1
  000dceb2: blt 0x000dcef2
  000dceb4: movs r0,#0xc
  000dceb6: blx 0x0006eb24
  000dceba: mov r5,r0
  000dcebc: blx 0x000701f8
  000dcec0: ldr r0,[sp,#0x34]
  000dcec2: mov r1,r5
  000dcec4: blx 0x00071a4c
  000dcec8: movs r6,#0x0
  000dceca: movs r4,#0x0
  000dcecc: b 0x000dcedc
  000dcece: ldr r0,[r5,#0x4]
  000dced0: ldr r1,[sp,#0x8c]
  000dced2: add r0,r6
  000dced4: blx 0x00074818
  000dced8: adds r6,#0x4
  000dceda: adds r4,#0x1
  000dcedc: ldr r0,[sp,#0x34]
  000dcede: cmp r4,r0
  000dcee0: blt 0x000dcece
  000dcee2: ldr.w r0,[r11,#0x184]
  000dcee6: ldr r0,[r0,#0x4]
  000dcee8: ldr.w r0,[r0,r10,lsl #0x2]
  000dceec: mov r1,r5
  000dceee: blx 0x000748cc
  000dcef2: ldr r0,[sp,#0x30]
  000dcef4: add.w r10,r10,#0x1
  000dcef8: cmp r10,r0
  000dcefa: blt 0x000dcea2
  000dcefc: add r6,sp,#0x24
  000dcefe: movs r4,#0x0
  000dcf00: mov.w r8,#0x0
  000dcf04: b 0x000dcf70
  000dcf06: cmp r8,r0
  000dcf08: ldr r0,[sp,#0xc]
  000dcf0a: ittt ne
  000dcf0c: ldr.ne r0,[sp,#0x8]
  000dcf0e: ldr.ne r0,[r0,#0x4]
  000dcf10: add.ne.w r0,r0,r8, lsl #0x2
  000dcf14: ldr r5,[r0,#0x0]
  000dcf16: cbz r5,0x000dcf6c
  000dcf18: ldr r1,[sp,#0x8c]
  000dcf1a: str r4,[sp,#0x2c]
  000dcf1c: add r0,sp,#0x2c
  000dcf1e: blx 0x00074818
  000dcf22: ldr r0,[sp,#0x2c]
  000dcf24: cmp r0,#0x1
  000dcf26: blt 0x000dcf6c
  000dcf28: mov.w r9,#0x0
  000dcf2c: b 0x000dcf68
  000dcf2e: ldr r1,[sp,#0x8c]
  000dcf30: str r4,[sp,#0x28]
  000dcf32: add r0,sp,#0x28
  000dcf34: blx 0x00074818
  000dcf38: mov.w r10,#0x0
  000dcf3c: b 0x000dcf5c
  000dcf3e: ldr r1,[sp,#0x8c]
  000dcf40: mov r0,r6
  000dcf42: blx 0x00074818
  000dcf46: mov r0,r5
  000dcf48: blx 0x00073708
  000dcf4c: ldr r0,[r0,#0x4]
  000dcf4e: ldr r1,[sp,#0x24]
  000dcf50: ldr.w r0,[r0,r9,lsl #0x2]
  000dcf54: blx 0x000748d8
  000dcf58: add.w r10,r10,#0x1
  000dcf5c: ldr r0,[sp,#0x28]
  000dcf5e: cmp r10,r0
  000dcf60: blt 0x000dcf3e
  000dcf62: ldr r0,[sp,#0x2c]
  000dcf64: add.w r9,r9,#0x1
  000dcf68: cmp r9,r0
  000dcf6a: blt 0x000dcf2e
  000dcf6c: add.w r8,r8,#0x1
  000dcf70: ldr r0,[sp,#0x8]
  000dcf72: ldr r0,[r0,#0x0]
  000dcf74: adds r1,r0,#0x1
  000dcf76: cmp r8,r1
  000dcf78: bcc 0x000dcf06
  000dcf7a: ldr r1,[sp,#0x8c]
  000dcf7c: movs r0,#0x0
  000dcf7e: str r0,[sp,#0x90]
  000dcf80: add r0,sp,#0x90
  000dcf82: blx 0x00074818
  000dcf86: ldr r0,[sp,#0x90]
  000dcf88: cmp r0,#0x1
  000dcf8a: blt 0x000dcfc0
  000dcf8c: movs r0,#0xc
  000dcf8e: blx 0x0006eb24
  000dcf92: mov r5,r0
  000dcf94: blx 0x000748e4
  000dcf98: ldr r0,[sp,#0x90]
  000dcf9a: str.w r5,[r11,#0x1b4]
  000dcf9e: mov r1,r5
  000dcfa0: blx 0x000748f0
  000dcfa4: movs r4,#0x0
  000dcfa6: b 0x000dcfba
  000dcfa8: ldr r1,[sp,#0x8c]
  000dcfaa: blx 0x000748fc
  000dcfae: ldr.w r1,[r11,#0x1b4]
  000dcfb2: ldr r1,[r1,#0x4]
  000dcfb4: str.w r0,[r1,r4,lsl #0x2]
  000dcfb8: adds r4,#0x1
  000dcfba: ldr r0,[sp,#0x90]
  000dcfbc: cmp r4,r0
  000dcfbe: blt 0x000dcfa8
  000dcfc0: add.w r5,r11,#0x1a4
  000dcfc4: movs r4,#0x0
  000dcfc6: b 0x000dcfd4
  000dcfc8: ldr r1,[sp,#0x8c]
  000dcfca: mov r0,r5
  000dcfcc: blx 0x00074818
  000dcfd0: adds r5,#0x4
  000dcfd2: adds r4,#0x1
  000dcfd4: cmp r4,#0x4
  000dcfd6: bcc 0x000dcfc8
  000dcfd8: ldr r1,[sp,#0x8c]
  000dcfda: addw r0,r11,#0x117
  000dcfde: blx 0x00074854
  000dcfe2: ldr r1,[sp,#0x8c]
  000dcfe4: add.w r0,r11,#0x1b8
  000dcfe8: blx 0x00074818
  000dcfec: ldr r1,[sp,#0x8c]
  000dcfee: add.w r0,r11,#0x104
  000dcff2: blx 0x00074854
  000dcff6: ldr r1,[sp,#0x8c]
  000dcff8: addw r0,r11,#0x105
  000dcffc: blx 0x00074854
  000dd000: ldr r1,[sp,#0x8c]
  000dd002: add.w r0,r11,#0x108
  000dd006: blx 0x00074854
  000dd00a: ldr r1,[sp,#0x8c]
  000dd00c: add.w r0,r11,#0x106
  000dd010: blx 0x00074854
  000dd014: ldr r1,[sp,#0x8c]
  000dd016: addw r0,r11,#0x107
  000dd01a: blx 0x00074854
  000dd01e: ldr r1,[sp,#0x8c]
  000dd020: add.w r0,r11,#0x10a
  000dd024: blx 0x00074854
  000dd028: ldr r1,[sp,#0x8c]
  000dd02a: addw r0,r11,#0x10b
  000dd02e: blx 0x00074854
  000dd032: ldr r1,[sp,#0x8c]
  000dd034: add.w r0,r11,#0x10c
  000dd038: blx 0x00074854
  000dd03c: ldr r1,[sp,#0x8c]
  000dd03e: add r0,sp,#0x90
  000dd040: blx 0x00074818
  000dd044: movs r0,#0xc
  000dd046: blx 0x0006eb24
  000dd04a: mov r5,r0
  000dd04c: blx 0x000735a0
  000dd050: ldr r0,[sp,#0x90]
  000dd052: str.w r5,[r11,#0x17c]
  000dd056: mov r1,r5
  000dd058: blx 0x000735c4
  000dd05c: movs r4,#0x0
  000dd05e: b 0x000dd06c
  000dd060: ldr r0,[r0,#0x4]
  000dd062: ldr r1,[sp,#0x8c]
  000dd064: add r0,r4
  000dd066: blx 0x00074854
  000dd06a: adds r4,#0x1
  000dd06c: ldr.w r0,[r11,#0x17c]
  000dd070: ldr r1,[r0,#0x0]
  000dd072: cmp r4,r1
  000dd074: bcc 0x000dd060
  000dd076: ldr r1,[sp,#0x8c]
  000dd078: addw r0,r11,#0x119
  000dd07c: blx 0x00074854
  000dd080: ldr r1,[sp,#0x8c]
  000dd082: addw r0,r11,#0x109
  000dd086: blx 0x00074854
  000dd08a: ldr r1,[sp,#0x8c]
  000dd08c: add.w r0,r11,#0x11a
  000dd090: blx 0x00074854
  000dd094: ldr r1,[sp,#0x8c]
  000dd096: add.w r0,r11,#0xe0
  000dd09a: blx 0x00074818
  000dd09e: ldr r1,[sp,#0x8c]
  000dd0a0: addw r0,r11,#0x10d
  000dd0a4: blx 0x00074854
  000dd0a8: ldr r1,[sp,#0x8c]
  000dd0aa: add.w r0,r11,#0x10e
  000dd0ae: blx 0x00074854
  000dd0b2: ldr r1,[sp,#0x8c]
  000dd0b4: addw r0,r11,#0x10f
  000dd0b8: blx 0x00074854
  000dd0bc: ldr r1,[sp,#0x8c]
  000dd0be: add.w r0,r11,#0x110
  000dd0c2: blx 0x00074854
  000dd0c6: ldr r1,[sp,#0x8c]
  000dd0c8: addw r0,r11,#0x111
  000dd0cc: blx 0x00074854
  000dd0d0: ldr r1,[sp,#0x8c]
  000dd0d2: addw r0,r11,#0x113
  000dd0d6: blx 0x00074854
  000dd0da: ldr r1,[sp,#0x8c]
  000dd0dc: add.w r0,r11,#0x112
  000dd0e0: blx 0x00074854
  000dd0e4: ldr r1,[sp,#0x8c]
  000dd0e6: add.w r0,r11,#0x114
  000dd0ea: blx 0x00074854
  000dd0ee: ldr r0,[sp,#0x8c]
  000dd0f0: blx 0x0006ece0
  000dd0f4: b 0x000dce9c
```
