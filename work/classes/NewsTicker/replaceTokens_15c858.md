# NewsTicker::replaceTokens
elf 0x15c858 body 4388
Sig: undefined __stdcall replaceTokens(String param_1)

## decompile
```c

/* NewsTicker::replaceTokens(AbyssEngine::String) */

void NewsTicker::replaceTokens(String *param_1,undefined4 param_2,String *param_3)

{
  int *piVar1;
  int iVar2;
  Station *pSVar3;
  undefined4 uVar4;
  void *pvVar5;
  String *pSVar6;
  uint uVar7;
  Array<Item*> *this;
  int iVar8;
  char *pcVar9;
  Galaxy *this_00;
  int *piVar10;
  Status *pSVar11;
  undefined4 uVar12;
  code *pcVar13;
  int iVar14;
  Item *this_01;
  undefined4 *puVar15;
  int *piVar16;
  undefined8 uVar17;
  String aSStack_52c [12];
  AbyssEngine aAStack_520 [12];
  String aSStack_514 [12];
  String aSStack_508 [12];
  String aSStack_4fc [12];
  String aSStack_4f0 [12];
  String aSStack_4e4 [12];
  String aSStack_4d8 [12];
  String aSStack_4cc [12];
  String aSStack_4c0 [12];
  String aSStack_4b4 [12];
  String aSStack_4a8 [12];
  String aSStack_49c [12];
  String aSStack_490 [12];
  String aSStack_484 [12];
  String aSStack_478 [12];
  String aSStack_46c [12];
  String aSStack_460 [12];
  String aSStack_454 [12];
  String aSStack_448 [12];
  String aSStack_43c [12];
  String aSStack_430 [12];
  String aSStack_424 [12];
  String aSStack_418 [12];
  String aSStack_40c [12];
  String aSStack_400 [12];
  String aSStack_3f4 [12];
  String aSStack_3e8 [12];
  String aSStack_3dc [12];
  String aSStack_3d0 [12];
  String aSStack_3c4 [12];
  String aSStack_3b8 [12];
  String aSStack_3ac [12];
  String aSStack_3a0 [12];
  String aSStack_394 [12];
  String aSStack_388 [12];
  String aSStack_37c [12];
  String aSStack_370 [12];
  String aSStack_364 [12];
  String aSStack_358 [12];
  String aSStack_34c [12];
  String aSStack_340 [12];
  String aSStack_334 [12];
  String aSStack_328 [12];
  String aSStack_31c [12];
  String aSStack_310 [12];
  int local_304 [3];
  String aSStack_2f8 [12];
  String aSStack_2ec [12];
  String aSStack_2e0 [12];
  String aSStack_2d4 [12];
  String aSStack_2c8 [12];
  String aSStack_2bc [12];
  String aSStack_2b0 [12];
  String aSStack_2a4 [12];
  String aSStack_298 [12];
  String aSStack_28c [12];
  String aSStack_280 [12];
  String aSStack_274 [12];
  String aSStack_268 [12];
  String aSStack_25c [12];
  String aSStack_250 [12];
  String aSStack_244 [12];
  String aSStack_238 [12];
  String aSStack_22c [12];
  String aSStack_220 [12];
  String aSStack_214 [12];
  String aSStack_208 [12];
  String aSStack_1fc [12];
  String aSStack_1f0 [12];
  String aSStack_1e4 [12];
  String aSStack_1d8 [12];
  String aSStack_1cc [12];
  String aSStack_1c0 [12];
  String aSStack_1b4 [12];
  String aSStack_1a8 [12];
  String aSStack_19c [12];
  String aSStack_190 [12];
  String aSStack_184 [12];
  String aSStack_178 [12];
  String aSStack_16c [12];
  String aSStack_160 [12];
  String aSStack_154 [12];
  String aSStack_148 [12];
  String aSStack_13c [12];
  String aSStack_130 [12];
  String aSStack_124 [12];
  String aSStack_118 [12];
  String aSStack_10c [12];
  String aSStack_100 [12];
  String aSStack_f4 [12];
  String aSStack_e8 [12];
  String aSStack_dc [12];
  String aSStack_d0 [12];
  String aSStack_c4 [12];
  String aSStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int iStack_28;
  
  piVar1 = *(int **)(DAT_0016cb38 + 0x16c872);
  puVar15 = *(undefined4 **)(DAT_0016cb3c + 0x16c874);
  iStack_28 = *piVar1;
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_34,param_3,false);
  AbyssEngine::String::String(aSStack_40,(char *)(DAT_0016cb40 + 0x16c896),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar2 != 0) {
    pSVar3 = (Station *)Globals::getRandomStation();
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_58,param_3,false);
    Station::getName();
    AbyssEngine::String::String(aSStack_64,aSStack_70,false);
    uVar4 = AbyssEngine::String::String(aSStack_7c,(char *)(DAT_0016cb48 + 0x16c900),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_58,aSStack_64,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016cb4c + 0x16c92c);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_7c);
    (*pcVar13)(aSStack_64);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_58);
    pSVar11 = (Status *)*puVar15;
    AbyssEngine::String::String(aSStack_88,param_3,false);
    AbyssEngine::String::String(aSStack_94,(char *)(DAT_0016cb50 + 0x16c960),false);
    iVar2 = Status::stringHasToken(pSVar11,aSStack_88,aSStack_94);
    AbyssEngine::String::~String(aSStack_94);
    AbyssEngine::String::~String(aSStack_88);
    if (iVar2 != 0) {
      this_00 = (Galaxy *)**(undefined4 **)(DAT_0016cb54 + 0x16c992);
      iVar2 = Station::getSystem(pSVar3);
      Galaxy::getSystem(this_00,iVar2);
      uVar12 = *puVar15;
      AbyssEngine::String::String(aSStack_a0,param_3,false);
      SolarSystem::getName();
      AbyssEngine::String::String(aSStack_ac,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0016cb58 + 0x16c9d2),false);
      Status::replaceHash(aSStack_4c,uVar12,aSStack_a0,aSStack_ac,uVar4);
      AbyssEngine::String::operator=(param_3,aSStack_4c);
      (*pcVar13)(aSStack_4c);
      (*pcVar13)(aSStack_b8);
      (*pcVar13)(aSStack_ac);
      (*pcVar13)(aSStack_70);
      (*pcVar13)(aSStack_a0);
    }
    if (pSVar3 != (Station *)0x0) {
      pvVar5 = (void *)Station::~Station(pSVar3);
      operator_delete(pvVar5);
    }
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_c4,param_3,false);
  AbyssEngine::String::String(aSStack_d0,(char *)(DAT_0016cb5c + 0x16ca3c),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_c4,aSStack_d0);
  AbyssEngine::String::~String(aSStack_d0);
  AbyssEngine::String::~String(aSStack_c4);
  if (iVar2 != 0) {
    pSVar11 = (Status *)*puVar15;
    AbyssEngine::String::String(aSStack_dc,param_3,false);
    AbyssEngine::String::String(aSStack_e8,(char *)(DAT_0016cb60 + 0x16ca80),false);
    iVar2 = Status::stringHasToken(pSVar11,aSStack_dc,aSStack_e8);
    AbyssEngine::String::~String(aSStack_e8);
    AbyssEngine::String::~String(aSStack_dc);
    if (iVar2 == 0) {
      Globals::getRandomSystemForDrinks();
    }
    else {
      Status::getSystem();
      uVar12 = *puVar15;
      AbyssEngine::String::String(aSStack_f4,param_3,false);
      iVar2 = **(int **)(DAT_0016cb64 + 0x16cacc);
      SolarSystem::getIndex();
      pSVar6 = (String *)GameText::getText(iVar2);
      AbyssEngine::String::String(aSStack_100,pSVar6,false);
      uVar4 = AbyssEngine::String::String(aSStack_10c,(char *)(DAT_0016cb68 + 0x16caf0),false);
      Status::replaceHash(aSStack_4c,uVar12,aSStack_f4,aSStack_100,uVar4);
      AbyssEngine::String::operator=(param_3,aSStack_4c);
      pcVar13 = *(code **)(DAT_0016cb6c + 0x16cb1c);
      (*pcVar13)(aSStack_4c);
      (*pcVar13)(aSStack_10c);
      (*pcVar13)(aSStack_100);
      (*pcVar13)(aSStack_f4);
    }
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_118,param_3,false);
    SolarSystem::getName();
    AbyssEngine::String::String(aSStack_124,aSStack_70,false);
    uVar4 = AbyssEngine::String::String(aSStack_130,(char *)(DAT_0016ce10 + 0x16cbac),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_118,aSStack_124,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016ce14 + 0x16cbd8);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_130);
    (*pcVar13)(aSStack_124);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_118);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_13c,param_3,false);
  AbyssEngine::String::String(aSStack_148,(char *)(DAT_0016ce18 + 0x16cc0c),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_13c,aSStack_148);
  AbyssEngine::String::~String(aSStack_148);
  AbyssEngine::String::~String(aSStack_13c);
  if (iVar2 != 0) {
    uVar4 = **(undefined4 **)(DAT_0016ce1c + 0x16cc38);
    Status::getSystem();
    SolarSystem::getRace();
    Globals::getRandomName((int)aSStack_4c,SUB41(uVar4,0));
    AbyssEngine::String::String(aSStack_70,(char *)(DAT_0016ce20 + 0x16cc5a),false);
    iVar2 = AbyssEngine::String::IndexOf(aSStack_4c);
    AbyssEngine::String::~String(aSStack_70);
    if (-1 < iVar2) {
      AbyssEngine::String::SubString((uint)aSStack_70,(uint)aSStack_4c);
      AbyssEngine::String::operator=(aSStack_4c,aSStack_70);
      AbyssEngine::String::~String(aSStack_70);
    }
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_154,param_3,false);
    AbyssEngine::String::String(aSStack_160,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_16c,(char *)(DAT_0016ce24 + 0x16ccc0),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_154,aSStack_160,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    pcVar13 = *(code **)(DAT_0016ce28 + 0x16cce6);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_16c);
    (*pcVar13)(aSStack_160);
    (*pcVar13)(aSStack_154);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_178,param_3,false);
  AbyssEngine::String::String(aSStack_184,(char *)(DAT_0016ce2c + 0x16cd12),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_178,aSStack_184);
  AbyssEngine::String::~String(aSStack_184);
  AbyssEngine::String::~String(aSStack_178);
  if (iVar2 != 0) {
    iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0016ce30 + 0x16cd3c));
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_190,param_3,false);
    iVar14 = **(int **)(DAT_0016ce34 + 0x16cd5a);
    Globals::getRandomEnemyFighter((Globals *)**(undefined4 **)(DAT_0016ce38 + 0x16cd5c),iVar2);
    pSVar6 = (String *)GameText::getText(iVar14);
    AbyssEngine::String::String(aSStack_19c,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_1a8,(char *)(DAT_0016ce3c + 0x16cd82),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_190,aSStack_19c,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016ce40 + 0x16cda8);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_1a8);
    (*pcVar13)(aSStack_19c);
    (*pcVar13)(aSStack_190);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_1b4,param_3,false);
  AbyssEngine::String::String(aSStack_1c0,(char *)(DAT_0016ce44 + 0x16cdce),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_1b4,aSStack_1c0);
  AbyssEngine::String::~String(aSStack_1c0);
  AbyssEngine::String::~String(aSStack_1b4);
  if (iVar2 != 0) {
    piVar10 = *(int **)(DAT_0016ce48 + 0x16cdf8);
    uVar7 = AbyssEngine::AERandom::nextInt(*piVar10);
    if (uVar7 < 4) {
      pcVar9 = *(char **)(DAT_0016ce4c + 0x16ce08 + uVar7 * 4);
    }
    else {
      pcVar9 = (char *)(DAT_0016d1a0 + 0x16ce5a);
      if (uVar7 != 4) {
        pcVar9 = (char *)(DAT_0016d19c + 0x16ce58);
      }
    }
    AbyssEngine::String::String(aSStack_4c,pcVar9,false);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_1cc,param_3,false);
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
    AbyssEngine::String::String(aSStack_1e4,iVar2);
    AbyssEngine::operator+(aSStack_1d8,aSStack_4c);
    uVar4 = AbyssEngine::String::String(aSStack_1f0,(char *)(DAT_0016d1a4 + 0x16ce96),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_1cc,aSStack_1d8,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    pcVar13 = *(code **)(DAT_0016d1a8 + 0x16cebc);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_1f0);
    (*pcVar13)(aSStack_1d8);
    (*pcVar13)(aSStack_1e4);
    (*pcVar13)(aSStack_1cc);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_1fc,param_3,false);
  AbyssEngine::String::String(aSStack_208,(char *)(DAT_0016d1ac + 0x16ceec),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_1fc,aSStack_208);
  AbyssEngine::String::~String(aSStack_208);
  AbyssEngine::String::~String(aSStack_1fc);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_214,param_3,false);
    iVar2 = **(int **)(DAT_0016d1b0 + 0x16cf22);
    AbyssEngine::AERandom::nextInt(**(int **)(DAT_0016d1b4 + 0x16cf24));
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_220,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_22c,(char *)(DAT_0016d1b8 + 0x16cf4a),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_214,aSStack_220,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016d1bc + 0x16cf70);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_22c);
    (*pcVar13)(aSStack_220);
    (*pcVar13)(aSStack_214);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_238,param_3,false);
  AbyssEngine::String::String(aSStack_244,(char *)(DAT_0016d1c0 + 0x16cf96),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_238,aSStack_244);
  AbyssEngine::String::~String(aSStack_244);
  AbyssEngine::String::~String(aSStack_238);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_250,param_3,false);
    iVar2 = DAT_0016d1cc;
    iVar14 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0016d1c8 + 0x16cfd2));
    __aeabi_idivmod(iVar14 + iVar2 + 1000,1000);
    Layout::formatNumber((int)aSStack_70);
    AbyssEngine::String::String(aSStack_25c,aSStack_70,false);
    uVar4 = AbyssEngine::String::String(aSStack_268,(char *)(DAT_0016d1d0 + 0x16d00c),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_250,aSStack_25c,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016d1d4 + 0x16d032);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_268);
    (*pcVar13)(aSStack_25c);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_250);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_274,param_3,false);
  AbyssEngine::String::String(aSStack_280,(char *)(DAT_0016d1d8 + 0x16d05e),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_274,aSStack_280);
  AbyssEngine::String::~String(aSStack_280);
  AbyssEngine::String::~String(aSStack_274);
  if (iVar2 != 0) {
    iVar2 = **(int **)(DAT_0016d1dc + 0x16d088);
    AbyssEngine::AERandom::nextInt(**(int **)(DAT_0016d1e0 + 0x16d08a));
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_4c,pSVar6,false);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_28c,param_3,false);
    AbyssEngine::String::String(aSStack_298,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_2a4,(char *)(DAT_0016d1e4 + 0x16d0cc),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_28c,aSStack_298,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    pcVar13 = *(code **)(DAT_0016d1e8 + 0x16d0f2);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_2a4);
    (*pcVar13)(aSStack_298);
    (*pcVar13)(aSStack_28c);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_2b0,param_3,false);
  AbyssEngine::String::String(aSStack_2bc,(char *)(DAT_0016d1ec + 0x16d11e),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_2b0,aSStack_2bc);
  AbyssEngine::String::~String(aSStack_2bc);
  AbyssEngine::String::~String(aSStack_2b0);
  if (iVar2 != 0) {
    piVar10 = *(int **)(DAT_0016d1f0 + 0x16d14a);
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
    AbyssEngine::String::String(aSStack_4c);
    AbyssEngine::String::String(aSStack_70);
    if (iVar2 < 0x28) {
      pSVar6 = (String *)GameText::getText(**(int **)(DAT_0016d1f4 + 0x16d16c));
      AbyssEngine::String::operator=(aSStack_4c,pSVar6);
    }
    else {
      this = operator_new(0xc);
      Array<Item*>::Array(this);
      piVar16 = *(int **)(DAT_0016d1f8 + 0x16d198);
      for (uVar7 = 0; uVar7 < *(uint *)*piVar16; uVar7 = uVar7 + 1) {
        this_01 = *(Item **)(((uint *)*piVar16)[1] + uVar7 * 4);
        iVar2 = Item::getAttribute((int)this_01);
        if (((iVar2 == 1) && (iVar2 = Item::getIngredients(), iVar2 == 0)) &&
           (iVar2 = Item::getType(this_01), iVar2 != 4)) {
          ArrayAdd<Item*>(this_01,(Array *)this);
        }
      }
      iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
      Item::getIndex(*(Item **)(*(int *)(this + 4) + iVar2 * 4));
      pSVar6 = (String *)GameText::getText(**(int **)(DAT_0016d260 + 0x16d250));
      AbyssEngine::String::operator=(aSStack_4c,pSVar6);
      pvVar5 = (void *)Array<Item*>::~Array(this);
      operator_delete(pvVar5);
    }
    uVar17 = Status::getPlayingTime();
    FUN_001aae54((int)uVar17,(int)((ulonglong)uVar17 >> 0x20),DAT_0016d2ac,0);
    Layout::formatNumber((int)aSStack_1e4);
    AbyssEngine::String::operator=(aSStack_70,aSStack_1e4);
    AbyssEngine::String::~String(aSStack_1e4);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_2c8,param_3,false);
    AbyssEngine::String::String(aSStack_2d4,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_2e0,(char *)(DAT_0016d2e8 + 0x16d2e0),false);
    Status::replaceHash(aSStack_1e4,uVar12,aSStack_2c8,aSStack_2d4,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_1e4);
    pcVar13 = *(code **)(DAT_0016d328 + 0x16d308);
    (*pcVar13)(aSStack_1e4);
    (*pcVar13)(aSStack_2e0);
    (*pcVar13)(aSStack_2d4);
    (*pcVar13)(aSStack_2c8);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_2ec,param_3,false);
    AbyssEngine::String::String(aSStack_310,aSStack_70,false);
    AbyssEngine::String::String(aSStack_31c,(char *)(DAT_0016d348 + 0x16d33e),false);
    AbyssEngine::operator+((String *)local_304,aSStack_310);
    GameText::getText(**(int **)(DAT_0016d73c + 0x16d35c));
    AbyssEngine::operator+(aSStack_2f8,(String *)local_304);
    uVar4 = AbyssEngine::String::String(aSStack_328,(char *)(DAT_0016d740 + 0x16d376),false);
    Status::replaceHash(aSStack_1e4,uVar12,aSStack_2ec,aSStack_2f8,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_1e4);
    (*pcVar13)(aSStack_1e4);
    (*pcVar13)(aSStack_328);
    (*pcVar13)(aSStack_2f8);
    (*pcVar13)(local_304);
    (*pcVar13)(aSStack_31c);
    (*pcVar13)(aSStack_310);
    (*pcVar13)(aSStack_2ec);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_334,param_3,false);
  AbyssEngine::String::String(aSStack_340,(char *)(DAT_0016d744 + 0x16d3ce),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_334,aSStack_340);
  AbyssEngine::String::~String(aSStack_340);
  AbyssEngine::String::~String(aSStack_334);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_34c,param_3,false);
    piVar10 = *(int **)(DAT_0016d748 + 0x16d408);
    piVar16 = *(int **)(DAT_0016d74c + 0x16d40a);
    iVar2 = *piVar10;
    AbyssEngine::AERandom::nextInt(*piVar16);
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_358,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_364,(char *)(DAT_0016d750 + 0x16d438),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_34c,aSStack_358,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016d754 + 0x16d45e);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_364);
    (*pcVar13)(aSStack_358);
    (*pcVar13)(aSStack_34c);
    AbyssEngine::AERandom::nextInt(*piVar16);
    Globals::getRandomName((int)aSStack_4c,SUB41(**(undefined4 **)(DAT_0016d758 + 0x16d484),0));
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_370,param_3,false);
    AbyssEngine::String::String(aSStack_37c,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_388,(char *)(DAT_0016d75c + 0x16d4ae),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_370,aSStack_37c,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_388);
    (*pcVar13)(aSStack_37c);
    (*pcVar13)(aSStack_370);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_394,param_3,false);
    iVar2 = *piVar10;
    AbyssEngine::AERandom::nextInt(*piVar16);
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_3a0,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_3ac,(char *)(DAT_0016d760 + 0x16d516),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_394,aSStack_3a0,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_3ac);
    (*pcVar13)(aSStack_3a0);
    (*pcVar13)(aSStack_394);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_3b8,param_3,false);
  AbyssEngine::String::String(aSStack_3c4,(char *)(DAT_0016d764 + 0x16d562),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_3b8,aSStack_3c4);
  AbyssEngine::String::~String(aSStack_3c4);
  AbyssEngine::String::~String(aSStack_3b8);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_3d0,param_3,false);
    piVar10 = *(int **)(DAT_0016d768 + 0x16d59c);
    piVar16 = *(int **)(DAT_0016d76c + 0x16d59e);
    iVar2 = *piVar10;
    AbyssEngine::AERandom::nextInt(*piVar16);
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_3dc,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_3e8,(char *)(DAT_0016d770 + 0x16d5cc),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_3d0,aSStack_3dc,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016d774 + 0x16d5f2);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_3e8);
    (*pcVar13)(aSStack_3dc);
    (*pcVar13)(aSStack_3d0);
    AbyssEngine::AERandom::nextInt(*piVar16);
    Globals::getRandomName((int)aSStack_4c,SUB41(**(undefined4 **)(DAT_0016d778 + 0x16d618),0));
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_3f4,param_3,false);
    AbyssEngine::String::String(aSStack_400,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_40c,(char *)(DAT_0016d77c + 0x16d642),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_3f4,aSStack_400,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_40c);
    (*pcVar13)(aSStack_400);
    (*pcVar13)(aSStack_3f4);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_418,param_3,false);
    iVar2 = *piVar10;
    AbyssEngine::AERandom::nextInt(*piVar16);
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_424,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_430,(char *)(DAT_0016d780 + 0x16d6aa),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_418,aSStack_424,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_430);
    (*pcVar13)(aSStack_424);
    (*pcVar13)(aSStack_418);
    (*pcVar13)(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_43c,param_3,false);
  AbyssEngine::String::String(aSStack_448,(char *)(DAT_0016d784 + 0x16d6f6),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_43c,aSStack_448);
  AbyssEngine::String::~String(aSStack_448);
  AbyssEngine::String::~String(aSStack_43c);
  if (iVar2 != 0) {
    piVar16 = *(int **)(DAT_0016d788 + 0x16d720);
    iVar2 = Galaxy::getSystems((Galaxy *)*piVar16);
    iVar14 = -1;
    piVar10 = *(int **)(DAT_0016d78c + 0x16d736);
    while (iVar14 < 0) {
      AbyssEngine::AERandom::nextInt(*piVar10);
      iVar8 = SolarSystem::getRace();
      if (iVar8 == 2) {
        iVar14 = SolarSystem::getIndex();
      }
    }
    SolarSystem::getStations(*(SolarSystem **)(*(int *)(iVar2 + 4) + iVar14 * 4));
    iVar2 = *piVar16;
    AbyssEngine::AERandom::nextInt(*piVar10);
    pSVar3 = (Station *)Galaxy::getStation(iVar2);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_454,param_3,false);
    Station::getName();
    AbyssEngine::String::String(aSStack_460,aSStack_70,false);
    uVar4 = AbyssEngine::String::String(aSStack_46c,(char *)(DAT_0016dbb4 + 0x16d80a),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_454,aSStack_460,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016dbb8 + 0x16d830);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_46c);
    (*pcVar13)(aSStack_460);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_454);
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
    __aeabi_idivmod(iVar2 + DAT_0016dbc4,1000);
    Layout::formatNumber((int)aSStack_4c);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_478,param_3,false);
    AbyssEngine::String::String(aSStack_484,aSStack_4c,false);
    uVar4 = AbyssEngine::String::String(aSStack_490,(char *)(DAT_0016dbc8 + 0x16d894),false);
    Status::replaceHash(aSStack_70,uVar12,aSStack_478,aSStack_484,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_70);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_490);
    (*pcVar13)(aSStack_484);
    (*pcVar13)(aSStack_478);
    if (pSVar3 != (Station *)0x0) {
      pvVar5 = (void *)Station::~Station(pSVar3);
      operator_delete(pvVar5);
    }
    AbyssEngine::String::~String(aSStack_4c);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_49c,param_3,false);
  AbyssEngine::String::String(aSStack_4a8,(char *)(DAT_0016dbcc + 0x16d8f2),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_49c,aSStack_4a8);
  AbyssEngine::String::~String(aSStack_4a8);
  AbyssEngine::String::~String(aSStack_49c);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_4b4,param_3,false);
    piVar10 = *(int **)(DAT_0016dbd0 + 0x16d928);
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
    AbyssEngine::String::String(aSStack_4c0,iVar2 + 10);
    uVar4 = AbyssEngine::String::String(aSStack_4cc,(char *)(DAT_0016dbd4 + 0x16d944),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_4b4,aSStack_4c0,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016dbd8 + 0x16d96a);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_4cc);
    (*pcVar13)(aSStack_4c0);
    (*pcVar13)(aSStack_4b4);
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_4d8,param_3,false);
    iVar2 = **(int **)(DAT_0016dbdc + 0x16d990);
    AbyssEngine::AERandom::nextInt(*piVar10);
    pSVar6 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_4e4,pSVar6,false);
    uVar4 = AbyssEngine::String::String(aSStack_4f0,(char *)(DAT_0016dbe0 + 0x16d9b6),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_4d8,aSStack_4e4,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_4f0);
    (*pcVar13)(aSStack_4e4);
    (*pcVar13)(aSStack_4d8);
  }
  pSVar11 = (Status *)*puVar15;
  AbyssEngine::String::String(aSStack_4fc,param_3,false);
  AbyssEngine::String::String(aSStack_508,(char *)(DAT_0016dbe4 + 0x16d9fc),false);
  iVar2 = Status::stringHasToken(pSVar11,aSStack_4fc,aSStack_508);
  AbyssEngine::String::~String(aSStack_508);
  AbyssEngine::String::~String(aSStack_4fc);
  if (iVar2 != 0) {
    uVar12 = *puVar15;
    AbyssEngine::String::String(aSStack_514,param_3,false);
    local_304[0] = 2;
    iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0016dbe8 + 0x16da36));
    AbyssEngine::String::String(aSStack_1e4,iVar2);
    AbyssEngine::String::String(aSStack_70,aSStack_1e4,false);
    AbyssEngine::operator+(aAStack_520,local_304,aSStack_70);
    uVar4 = AbyssEngine::String::String(aSStack_52c,(char *)(DAT_0016dbec + 0x16da64),false);
    Status::replaceHash(aSStack_4c,uVar12,aSStack_514,aAStack_520,uVar4);
    AbyssEngine::String::operator=(param_3,aSStack_4c);
    pcVar13 = *(code **)(DAT_0016dbf0 + 0x16da8a);
    (*pcVar13)(aSStack_4c);
    (*pcVar13)(aSStack_52c);
    (*pcVar13)(aAStack_520);
    (*pcVar13)(aSStack_70);
    (*pcVar13)(aSStack_1e4);
    (*pcVar13)(aSStack_514);
  }
  AbyssEngine::String::String(param_1,param_3,false);
  if (*piVar1 != iStack_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016c858: push {r4,r5,r6,r7,lr}
  0016c85a: add r7,sp,#0xc
  0016c85c: push {r8,r9,r10,r11}
  0016c860: sub.w sp,sp,#0x518
  0016c864: sub sp,#0x4
  0016c866: str r0,[sp,#0xc]
  0016c868: mov r10,r2
  0016c86a: ldr r0,[0x0016cb38]
  0016c86c: ldr r1,[0x0016cb3c]
  0016c86e: add r0,pc
  0016c870: add r1,pc
  0016c872: ldr r0,[r0,#0x0]
  0016c874: ldr.w r8,[r1,#0x0]
  0016c878: mov r1,r2
  0016c87a: str r0,[sp,#0x10]
  0016c87c: movs r2,#0x0
  0016c87e: ldr r0,[r0,#0x0]
  0016c880: ldr.w r5,[r8,#0x0]
  0016c884: str.w r0,[r7,#-0x20]
  0016c888: sub.w r0,r7,#0x2c
  0016c88c: blx 0x0006f028
  0016c890: ldr r1,[0x0016cb40]
  0016c892: add r1,pc
  0016c894: sub.w r0,r7,#0x38
  0016c898: movs r2,#0x0
  0016c89a: blx 0x0006ee18
  0016c89e: sub.w r1,r7,#0x2c
  0016c8a2: sub.w r2,r7,#0x38
  0016c8a6: mov r0,r5
  0016c8a8: blx 0x00077890
  0016c8ac: mov r5,r0
  0016c8ae: sub.w r0,r7,#0x38
  0016c8b2: blx 0x0006ee30
  0016c8b6: sub.w r0,r7,#0x2c
  0016c8ba: blx 0x0006ee30
  0016c8be: cmp r5,#0x0
  0016c8c0: beq.w 0x0016ca26
  0016c8c4: ldr r0,[0x0016cb44]
  0016c8c6: add r0,pc
  0016c8c8: ldr r0,[r0,#0x0]
  0016c8ca: ldr r0,[r0,#0x0]
  0016c8cc: blx 0x000737d4
  0016c8d0: mov r9,r0
  0016c8d2: sub.w r0,r7,#0x50
  0016c8d6: mov r1,r10
  0016c8d8: movs r2,#0x0
  0016c8da: ldr.w r5,[r8,#0x0]
  0016c8de: blx 0x0006f028
  0016c8e2: sub.w r0,r7,#0x68
  0016c8e6: mov r1,r9
  0016c8e8: blx 0x000736a8
  0016c8ec: sub.w r0,r7,#0x5c
  0016c8f0: sub.w r1,r7,#0x68
  0016c8f4: movs r2,#0x0
  0016c8f6: blx 0x0006f028
  0016c8fa: ldr r1,[0x0016cb48]
  0016c8fc: add r1,pc
  0016c8fe: sub.w r0,r7,#0x74
  0016c902: movs r2,#0x0
  0016c904: blx 0x0006ee18
  0016c908: str r0,[sp,#0x0]
  0016c90a: sub.w r0,r7,#0x44
  0016c90e: sub.w r2,r7,#0x50
  0016c912: sub.w r3,r7,#0x5c
  0016c916: mov r1,r5
  0016c918: blx 0x000739b4
  0016c91c: sub.w r1,r7,#0x44
  0016c920: mov r0,r10
  0016c922: blx 0x0006f2b0
  0016c926: ldr r0,[0x0016cb4c]
  0016c928: add r0,pc
  0016c92a: ldr r6,[r0,#0x0]
  0016c92c: sub.w r0,r7,#0x44
  0016c930: blx r6
  0016c932: sub.w r0,r7,#0x74
  0016c936: blx r6
  0016c938: sub.w r0,r7,#0x5c
  0016c93c: blx r6
  0016c93e: sub.w r0,r7,#0x68
  0016c942: blx r6
  0016c944: sub.w r0,r7,#0x50
  0016c948: blx r6
  0016c94a: sub.w r0,r7,#0x80
  0016c94e: mov r1,r10
  0016c950: movs r2,#0x0
  0016c952: ldr.w r5,[r8,#0x0]
  0016c956: blx 0x0006f028
  0016c95a: ldr r1,[0x0016cb50]
  0016c95c: add r1,pc
  0016c95e: sub.w r0,r7,#0x8c
  0016c962: movs r2,#0x0
  0016c964: blx 0x0006ee18
  0016c968: sub.w r1,r7,#0x80
  0016c96c: sub.w r2,r7,#0x8c
  0016c970: mov r0,r5
  0016c972: blx 0x00077890
  0016c976: mov r5,r0
  0016c978: sub.w r0,r7,#0x8c
  0016c97c: blx 0x0006ee30
  0016c980: sub.w r0,r7,#0x80
  0016c984: blx 0x0006ee30
  0016c988: cmp r5,#0x0
  0016c98a: beq 0x0016ca16
  0016c98c: ldr r0,[0x0016cb54]
  0016c98e: add r0,pc
  0016c990: ldr r0,[r0,#0x0]
  0016c992: ldr r4,[r0,#0x0]
  0016c994: mov r0,r9
  0016c996: blx 0x0007189c
  0016c99a: mov r1,r0
  0016c99c: mov r0,r4
  0016c99e: blx 0x00073684
  0016c9a2: mov r4,r0
  0016c9a4: sub.w r0,r7,#0x98
  0016c9a8: mov r1,r10
  0016c9aa: movs r2,#0x0
  0016c9ac: ldr.w r5,[r8,#0x0]
  0016c9b0: blx 0x0006f028
  0016c9b4: sub.w r0,r7,#0x68
  0016c9b8: mov r1,r4
  0016c9ba: blx 0x00071cc8
  0016c9be: sub.w r0,r7,#0xa4
  0016c9c2: sub.w r1,r7,#0x68
  0016c9c6: movs r2,#0x0
  0016c9c8: blx 0x0006f028
  0016c9cc: ldr r1,[0x0016cb58]
  0016c9ce: add r1,pc
  0016c9d0: sub.w r0,r7,#0xb0
  0016c9d4: movs r2,#0x0
  0016c9d6: blx 0x0006ee18
  0016c9da: str r0,[sp,#0x0]
  0016c9dc: sub.w r0,r7,#0x44
  0016c9e0: sub.w r2,r7,#0x98
  0016c9e4: sub.w r3,r7,#0xa4
  0016c9e8: mov r1,r5
  0016c9ea: blx 0x000739b4
  0016c9ee: sub.w r1,r7,#0x44
  0016c9f2: mov r0,r10
  0016c9f4: blx 0x0006f2b0
  0016c9f8: sub.w r0,r7,#0x44
  0016c9fc: blx r6
  0016c9fe: sub.w r0,r7,#0xb0
  0016ca02: blx r6
  0016ca04: sub.w r0,r7,#0xa4
  0016ca08: blx r6
  0016ca0a: sub.w r0,r7,#0x68
  0016ca0e: blx r6
  0016ca10: sub.w r0,r7,#0x98
  0016ca14: blx r6
  0016ca16: cmp.w r9,#0x0
  0016ca1a: beq 0x0016ca26
  0016ca1c: mov r0,r9
  0016ca1e: blx 0x0007360c
  0016ca22: blx 0x0006eb48
  0016ca26: sub.w r0,r7,#0xbc
  0016ca2a: mov r1,r10
  0016ca2c: movs r2,#0x0
  0016ca2e: ldr.w r5,[r8,#0x0]
  0016ca32: blx 0x0006f028
  0016ca36: ldr r1,[0x0016cb5c]
  0016ca38: add r1,pc
  0016ca3a: sub.w r0,r7,#0xc8
  0016ca3e: movs r2,#0x0
  0016ca40: blx 0x0006ee18
  0016ca44: sub.w r1,r7,#0xbc
  0016ca48: sub.w r2,r7,#0xc8
  0016ca4c: mov r0,r5
  0016ca4e: blx 0x00077890
  0016ca52: mov r5,r0
  0016ca54: sub.w r0,r7,#0xc8
  0016ca58: blx 0x0006ee30
  0016ca5c: sub.w r0,r7,#0xbc
  0016ca60: blx 0x0006ee30
  0016ca64: cmp r5,#0x0
  0016ca66: beq.w 0x0016cbf6
  0016ca6a: sub.w r0,r7,#0xd4
  0016ca6e: mov r1,r10
  0016ca70: movs r2,#0x0
  0016ca72: ldr.w r5,[r8,#0x0]
  0016ca76: blx 0x0006f028
  0016ca7a: ldr r1,[0x0016cb60]
  0016ca7c: add r1,pc
  0016ca7e: sub.w r0,r7,#0xe0
  0016ca82: movs r2,#0x0
  0016ca84: blx 0x0006ee18
  0016ca88: sub.w r1,r7,#0xd4
  0016ca8c: sub.w r2,r7,#0xe0
  0016ca90: mov r0,r5
  0016ca92: blx 0x00077890
  0016ca96: mov r5,r0
  0016ca98: sub.w r0,r7,#0xe0
  0016ca9c: blx 0x0006ee30
  0016caa0: sub.w r0,r7,#0xd4
  0016caa4: blx 0x0006ee30
  0016caa8: cmp r5,#0x0
  0016caaa: beq 0x0016cb70
  0016caac: ldr.w r0,[r8,#0x0]
  0016cab0: blx 0x00071a10
  0016cab4: mov r5,r0
  0016cab6: sub.w r0,r7,#0xec
  0016caba: mov r1,r10
  0016cabc: movs r2,#0x0
  0016cabe: ldr.w r9,[r8,#0x0]
  0016cac2: blx 0x0006f028
  0016cac6: ldr r0,[0x0016cb64]
  0016cac8: add r0,pc
  0016caca: ldr r0,[r0,#0x0]
  0016cacc: ldr r6,[r0,#0x0]
  0016cace: mov r0,r5
  0016cad0: blx 0x00071a7c
  0016cad4: addw r1,r0,#0x57e
  0016cad8: mov r0,r6
  0016cada: blx 0x00072f70
  0016cade: mov r1,r0
  0016cae0: sub.w r0,r7,#0xf8
  0016cae4: movs r2,#0x0
  0016cae6: blx 0x0006f028
  0016caea: ldr r1,[0x0016cb68]
  0016caec: add r1,pc
  0016caee: addw r0,sp,#0x434
  0016caf2: movs r2,#0x0
  0016caf4: blx 0x0006ee18
  0016caf8: str r0,[sp,#0x0]
  0016cafa: sub.w r0,r7,#0x44
  0016cafe: sub.w r2,r7,#0xec
  0016cb02: sub.w r3,r7,#0xf8
  0016cb06: mov r1,r9
  0016cb08: blx 0x000739b4
  0016cb0c: sub.w r1,r7,#0x44
  0016cb10: mov r0,r10
  0016cb12: blx 0x0006f2b0
  0016cb16: ldr r0,[0x0016cb6c]
  0016cb18: add r0,pc
  0016cb1a: ldr r4,[r0,#0x0]
  0016cb1c: sub.w r0,r7,#0x44
  0016cb20: blx r4
  0016cb22: addw r0,sp,#0x434
  0016cb26: blx r4
  0016cb28: sub.w r0,r7,#0xf8
  0016cb2c: blx r4
  0016cb2e: sub.w r0,r7,#0xec
  0016cb32: blx r4
  0016cb34: b 0x0016cb7e
  0016cb70: ldr r0,[0x0016ce0c]
  0016cb72: add r0,pc
  0016cb74: ldr r0,[r0,#0x0]
  0016cb76: ldr r0,[r0,#0x0]
  0016cb78: blx 0x0007789c
  0016cb7c: mov r5,r0
  0016cb7e: add.w r0,sp,#0x428
  0016cb82: mov r1,r10
  0016cb84: movs r2,#0x0
  0016cb86: ldr.w r6,[r8,#0x0]
  0016cb8a: blx 0x0006f028
  0016cb8e: sub.w r0,r7,#0x68
  0016cb92: mov r1,r5
  0016cb94: blx 0x00071cc8
  0016cb98: addw r0,sp,#0x41c
  0016cb9c: sub.w r1,r7,#0x68
  0016cba0: movs r2,#0x0
  0016cba2: blx 0x0006f028
  0016cba6: ldr r1,[0x0016ce10]
  0016cba8: add r1,pc
  0016cbaa: add.w r0,sp,#0x410
  0016cbae: movs r2,#0x0
  0016cbb0: blx 0x0006ee18
  0016cbb4: str r0,[sp,#0x0]
  0016cbb6: sub.w r0,r7,#0x44
  0016cbba: add.w r2,sp,#0x428
  0016cbbe: addw r3,sp,#0x41c
  0016cbc2: mov r1,r6
  0016cbc4: blx 0x000739b4
  0016cbc8: sub.w r1,r7,#0x44
  0016cbcc: mov r0,r10
  0016cbce: blx 0x0006f2b0
  0016cbd2: ldr r0,[0x0016ce14]
  0016cbd4: add r0,pc
  0016cbd6: ldr r4,[r0,#0x0]
  0016cbd8: sub.w r0,r7,#0x44
  0016cbdc: blx r4
  0016cbde: add.w r0,sp,#0x410
  0016cbe2: blx r4
  0016cbe4: addw r0,sp,#0x41c
  0016cbe8: blx r4
  0016cbea: sub.w r0,r7,#0x68
  0016cbee: blx r4
  0016cbf0: add.w r0,sp,#0x428
  0016cbf4: blx r4
  0016cbf6: addw r0,sp,#0x404
  0016cbfa: mov r1,r10
  0016cbfc: movs r2,#0x0
  0016cbfe: ldr.w r5,[r8,#0x0]
  0016cc02: blx 0x0006f028
  0016cc06: ldr r1,[0x0016ce18]
  0016cc08: add r1,pc
  0016cc0a: add r0,sp,#0x3f8
  0016cc0c: movs r2,#0x0
  0016cc0e: blx 0x0006ee18
  0016cc12: addw r1,sp,#0x404
  0016cc16: add r2,sp,#0x3f8
  0016cc18: mov r0,r5
  0016cc1a: blx 0x00077890
  0016cc1e: mov r5,r0
  0016cc20: add r0,sp,#0x3f8
  0016cc22: blx 0x0006ee30
  0016cc26: addw r0,sp,#0x404
  0016cc2a: blx 0x0006ee30
  0016cc2e: cmp r5,#0x0
  0016cc30: beq 0x0016ccfe
  0016cc32: ldr r0,[0x0016ce1c]
  0016cc34: add r0,pc
  0016cc36: ldr r0,[r0,#0x0]
  0016cc38: ldr r4,[r0,#0x0]
  0016cc3a: ldr.w r0,[r8,#0x0]
  0016cc3e: blx 0x00071a10
  0016cc42: blx 0x00071950
  0016cc46: mov r2,r0
  0016cc48: sub.w r0,r7,#0x44
  0016cc4c: mov r1,r4
  0016cc4e: movs r3,#0x1
  0016cc50: blx 0x00071b24
  0016cc54: ldr r1,[0x0016ce20]
  0016cc56: add r1,pc
  0016cc58: sub.w r0,r7,#0x68
  0016cc5c: movs r2,#0x0
  0016cc5e: blx 0x0006ee18
  0016cc62: sub.w r0,r7,#0x44
  0016cc66: sub.w r1,r7,#0x68
  0016cc6a: blx 0x0006f3a0
  0016cc6e: mov r5,r0
  0016cc70: sub.w r0,r7,#0x68
  0016cc74: blx 0x0006ee30
  0016cc78: cmp r5,#0x0
  0016cc7a: blt 0x0016cca0
  0016cc7c: sub.w r0,r7,#0x68
  0016cc80: sub.w r1,r7,#0x44
  0016cc84: movs r2,#0x0
  0016cc86: mov r3,r5
  0016cc88: blx 0x0006f604
  0016cc8c: sub.w r0,r7,#0x44
  0016cc90: sub.w r1,r7,#0x68
  0016cc94: blx 0x0006f2b0
  0016cc98: sub.w r0,r7,#0x68
  0016cc9c: blx 0x0006ee30
  0016cca0: ldr.w r5,[r8,#0x0]
  0016cca4: add r0,sp,#0x3ec
  0016cca6: mov r1,r10
  0016cca8: movs r2,#0x0
  0016ccaa: blx 0x0006f028
  0016ccae: add r0,sp,#0x3e0
  0016ccb0: sub.w r1,r7,#0x44
  0016ccb4: movs r2,#0x0
  0016ccb6: blx 0x0006f028
  0016ccba: ldr r1,[0x0016ce24]
  0016ccbc: add r1,pc
  0016ccbe: add r0,sp,#0x3d4
  0016ccc0: movs r2,#0x0
  0016ccc2: blx 0x0006ee18
  0016ccc6: str r0,[sp,#0x0]
  0016ccc8: sub.w r0,r7,#0x68
  0016cccc: add r2,sp,#0x3ec
  0016ccce: add r3,sp,#0x3e0
  0016ccd0: mov r1,r5
  0016ccd2: blx 0x000739b4
  0016ccd6: sub.w r1,r7,#0x68
  0016ccda: mov r0,r10
  0016ccdc: blx 0x0006f2b0
  0016cce0: ldr r0,[0x0016ce28]
  0016cce2: add r0,pc
  0016cce4: ldr r4,[r0,#0x0]
  0016cce6: sub.w r0,r7,#0x68
  0016ccea: blx r4
  0016ccec: add r0,sp,#0x3d4
  0016ccee: blx r4
  0016ccf0: add r0,sp,#0x3e0
  0016ccf2: blx r4
  0016ccf4: add r0,sp,#0x3ec
  0016ccf6: blx r4
  0016ccf8: sub.w r0,r7,#0x44
  0016ccfc: blx r4
  0016ccfe: add r0,sp,#0x3c8
  0016cd00: mov r1,r10
  0016cd02: movs r2,#0x0
  0016cd04: ldr.w r5,[r8,#0x0]
  0016cd08: blx 0x0006f028
  0016cd0c: ldr r1,[0x0016ce2c]
  0016cd0e: add r1,pc
  0016cd10: add r0,sp,#0x3bc
  0016cd12: movs r2,#0x0
  0016cd14: blx 0x0006ee18
  0016cd18: add r1,sp,#0x3c8
  0016cd1a: add r2,sp,#0x3bc
  0016cd1c: mov r0,r5
  0016cd1e: blx 0x00077890
  0016cd22: mov r5,r0
  0016cd24: add r0,sp,#0x3bc
  0016cd26: blx 0x0006ee30
  0016cd2a: add r0,sp,#0x3c8
  0016cd2c: blx 0x0006ee30
  0016cd30: cmp r5,#0x0
  0016cd32: beq 0x0016cdba
  0016cd34: ldr r0,[0x0016ce30]
  0016cd36: movs r1,#0x4
  0016cd38: add r0,pc
  0016cd3a: ldr r0,[r0,#0x0]
  0016cd3c: ldr r0,[r0,#0x0]
  0016cd3e: blx 0x00071848
  0016cd42: mov r4,r0
  0016cd44: add r0,sp,#0x3b0
  0016cd46: mov r1,r10
  0016cd48: movs r2,#0x0
  0016cd4a: ldr.w r5,[r8,#0x0]
  0016cd4e: blx 0x0006f028
  0016cd52: ldr r0,[0x0016ce34]
  0016cd54: ldr r1,[0x0016ce38]
  0016cd56: add r0,pc
  0016cd58: add r1,pc
  0016cd5a: ldr r0,[r0,#0x0]
  0016cd5c: ldr r1,[r1,#0x0]
  0016cd5e: ldr r6,[r0,#0x0]
  0016cd60: ldr r0,[r1,#0x0]
  0016cd62: mov r1,r4
  0016cd64: blx 0x00071a34
  0016cd68: addw r1,r0,#0x391
  0016cd6c: mov r0,r6
  0016cd6e: blx 0x00072f70
  0016cd72: mov r1,r0
  0016cd74: add r0,sp,#0x3a4
  0016cd76: movs r2,#0x0
  0016cd78: blx 0x0006f028
  0016cd7c: ldr r1,[0x0016ce3c]
  0016cd7e: add r1,pc
  0016cd80: add r0,sp,#0x398
  0016cd82: movs r2,#0x0
  0016cd84: blx 0x0006ee18
  0016cd88: str r0,[sp,#0x0]
  0016cd8a: sub.w r0,r7,#0x44
  0016cd8e: add r2,sp,#0x3b0
  0016cd90: add r3,sp,#0x3a4
  0016cd92: mov r1,r5
  0016cd94: blx 0x000739b4
  0016cd98: sub.w r1,r7,#0x44
  0016cd9c: mov r0,r10
  0016cd9e: blx 0x0006f2b0
  0016cda2: ldr r0,[0x0016ce40]
  0016cda4: add r0,pc
  0016cda6: ldr r4,[r0,#0x0]
  0016cda8: sub.w r0,r7,#0x44
  0016cdac: blx r4
  0016cdae: add r0,sp,#0x398
  0016cdb0: blx r4
  0016cdb2: add r0,sp,#0x3a4
  0016cdb4: blx r4
  0016cdb6: add r0,sp,#0x3b0
  0016cdb8: blx r4
  0016cdba: add r0,sp,#0x38c
  0016cdbc: mov r1,r10
  0016cdbe: movs r2,#0x0
  0016cdc0: ldr.w r5,[r8,#0x0]
  0016cdc4: blx 0x0006f028
  0016cdc8: ldr r1,[0x0016ce44]
  0016cdca: add r1,pc
  0016cdcc: add r0,sp,#0x380
  0016cdce: movs r2,#0x0
  0016cdd0: blx 0x0006ee18
  0016cdd4: add r1,sp,#0x38c
  0016cdd6: add r2,sp,#0x380
  0016cdd8: mov r0,r5
  0016cdda: blx 0x00077890
  0016cdde: mov r5,r0
  0016cde0: add r0,sp,#0x380
  0016cde2: blx 0x0006ee30
  0016cde6: add r0,sp,#0x38c
  0016cde8: blx 0x0006ee30
  0016cdec: cmp r5,#0x0
  0016cdee: beq 0x0016ced8
  0016cdf0: ldr r0,[0x0016ce48]
  0016cdf2: movs r1,#0x6
  0016cdf4: add r0,pc
  0016cdf6: ldr r4,[r0,#0x0]
  0016cdf8: ldr r0,[r4,#0x0]
  0016cdfa: blx 0x00071848
  0016cdfe: cmp r0,#0x4
  0016ce00: bcs 0x0016ce50
  0016ce02: ldr r1,[0x0016ce4c]
  0016ce04: add r1,pc
  0016ce06: ldr.w r1,[r1,r0,lsl #0x2]
  0016ce0a: b 0x0016ce5c
  0016ce50: ldr r0,[0x0016d19c]
  0016ce52: ldr r1,[0x0016d1a0]
  0016ce54: add r0,pc
  0016ce56: add r1,pc
  0016ce58: it ne
  0016ce5a: mov.ne r1,r0
  0016ce5c: sub.w r0,r7,#0x44
  0016ce60: movs r2,#0x0
  0016ce62: blx 0x0006ee18
  0016ce66: ldr.w r5,[r8,#0x0]
  0016ce6a: add r0,sp,#0x374
  0016ce6c: mov r1,r10
  0016ce6e: movs r2,#0x0
  0016ce70: blx 0x0006f028
  0016ce74: ldr r0,[r4,#0x0]
  0016ce76: movs r1,#0xa
  0016ce78: blx 0x00071848
  0016ce7c: mov r1,r0
  0016ce7e: add r0,sp,#0x35c
  0016ce80: blx 0x0006f658
  0016ce84: add r0,sp,#0x368
  0016ce86: sub.w r1,r7,#0x44
  0016ce8a: add r2,sp,#0x35c
  0016ce8c: blx 0x0006ef98
  0016ce90: ldr r1,[0x0016d1a4]
  0016ce92: add r1,pc
  0016ce94: add r0,sp,#0x350
  0016ce96: movs r2,#0x0
  0016ce98: blx 0x0006ee18
  0016ce9c: str r0,[sp,#0x0]
  0016ce9e: sub.w r0,r7,#0x68
  0016cea2: add r2,sp,#0x374
  0016cea4: add r3,sp,#0x368
  0016cea6: mov r1,r5
  0016cea8: blx 0x000739b4
  0016ceac: sub.w r1,r7,#0x68
  0016ceb0: mov r0,r10
  0016ceb2: blx 0x0006f2b0
  0016ceb6: ldr r0,[0x0016d1a8]
  0016ceb8: add r0,pc
  0016ceba: ldr r4,[r0,#0x0]
  0016cebc: sub.w r0,r7,#0x68
  0016cec0: blx r4
  0016cec2: add r0,sp,#0x350
  0016cec4: blx r4
  0016cec6: add r0,sp,#0x368
  0016cec8: blx r4
  0016ceca: add r0,sp,#0x35c
  0016cecc: blx r4
  0016cece: add r0,sp,#0x374
  0016ced0: blx r4
  0016ced2: sub.w r0,r7,#0x44
  0016ced6: blx r4
  0016ced8: add r0,sp,#0x344
  0016ceda: mov r1,r10
  0016cedc: movs r2,#0x0
  0016cede: ldr.w r5,[r8,#0x0]
  0016cee2: blx 0x0006f028
  0016cee6: ldr r1,[0x0016d1ac]
  0016cee8: add r1,pc
  0016ceea: add r0,sp,#0x338
  0016ceec: movs r2,#0x0
  0016ceee: blx 0x0006ee18
  0016cef2: add r1,sp,#0x344
  0016cef4: add r2,sp,#0x338
  0016cef6: mov r0,r5
  0016cef8: blx 0x00077890
  0016cefc: mov r5,r0
  0016cefe: add r0,sp,#0x338
  0016cf00: blx 0x0006ee30
  0016cf04: add r0,sp,#0x344
  0016cf06: blx 0x0006ee30
  0016cf0a: cbz r5,0x0016cf82
  0016cf0c: add r0,sp,#0x32c
  0016cf0e: mov r1,r10
  0016cf10: movs r2,#0x0
  0016cf12: ldr.w r5,[r8,#0x0]
  0016cf16: blx 0x0006f028
  0016cf1a: ldr r0,[0x0016d1b0]
  0016cf1c: ldr r1,[0x0016d1b4]
  0016cf1e: add r0,pc
  0016cf20: add r1,pc
  0016cf22: ldr r0,[r0,#0x0]
  0016cf24: ldr r1,[r1,#0x0]
  0016cf26: ldr r6,[r0,#0x0]
  0016cf28: ldr r0,[r1,#0x0]
  0016cf2a: movs r1,#0x5
  0016cf2c: blx 0x00071848
  0016cf30: addw r1,r0,#0xca5
  0016cf34: mov r0,r6
  0016cf36: blx 0x00072f70
  0016cf3a: mov r1,r0
  0016cf3c: add r0,sp,#0x320
  0016cf3e: movs r2,#0x0
  0016cf40: blx 0x0006f028
  0016cf44: ldr r1,[0x0016d1b8]
  0016cf46: add r1,pc
  0016cf48: add r0,sp,#0x314
  0016cf4a: movs r2,#0x0
  0016cf4c: blx 0x0006ee18
  0016cf50: str r0,[sp,#0x0]
  0016cf52: sub.w r0,r7,#0x44
  0016cf56: add r2,sp,#0x32c
  0016cf58: add r3,sp,#0x320
  0016cf5a: mov r1,r5
  0016cf5c: blx 0x000739b4
  0016cf60: sub.w r1,r7,#0x44
  0016cf64: mov r0,r10
  0016cf66: blx 0x0006f2b0
  0016cf6a: ldr r0,[0x0016d1bc]
  0016cf6c: add r0,pc
  0016cf6e: ldr r4,[r0,#0x0]
  0016cf70: sub.w r0,r7,#0x44
  0016cf74: blx r4
  0016cf76: add r0,sp,#0x314
  0016cf78: blx r4
  0016cf7a: add r0,sp,#0x320
  0016cf7c: blx r4
  0016cf7e: add r0,sp,#0x32c
  0016cf80: blx r4
  0016cf82: add r0,sp,#0x308
  0016cf84: mov r1,r10
  0016cf86: movs r2,#0x0
  0016cf88: ldr.w r5,[r8,#0x0]
  0016cf8c: blx 0x0006f028
  0016cf90: ldr r1,[0x0016d1c0]
  0016cf92: add r1,pc
  0016cf94: add r0,sp,#0x2fc
  0016cf96: movs r2,#0x0
  0016cf98: blx 0x0006ee18
  0016cf9c: add r1,sp,#0x308
  0016cf9e: add r2,sp,#0x2fc
  0016cfa0: mov r0,r5
  0016cfa2: blx 0x00077890
  0016cfa6: mov r5,r0
  0016cfa8: add r0,sp,#0x2fc
  0016cfaa: blx 0x0006ee30
  0016cfae: add r0,sp,#0x308
  0016cfb0: blx 0x0006ee30
  0016cfb4: cmp r5,#0x0
  0016cfb6: beq 0x0016d04a
  0016cfb8: add r0,sp,#0x2f0
  0016cfba: mov r1,r10
  0016cfbc: movs r2,#0x0
  0016cfbe: ldr.w r5,[r8,#0x0]
  0016cfc2: blx 0x0006f028
  0016cfc6: ldr r0,[0x0016d1c4]
  0016cfc8: ldr r1,[0x0016d1c8]
  0016cfca: add r0,pc
  0016cfcc: ldr r4,[0x0016d1cc]
  0016cfce: add r1,pc
  0016cfd0: ldr r0,[r0,#0x0]
  0016cfd2: ldr r1,[r1,#0x0]
  0016cfd4: ldr r6,[r0,#0x0]
  0016cfd6: ldr r0,[r1,#0x0]
  0016cfd8: mov r1,r4
  0016cfda: blx 0x00071848
  0016cfde: add r0,r4
  0016cfe0: mov.w r1,#0x3e8
  0016cfe4: add.w r4,r0,#0x3e8
  0016cfe8: mov r0,r4
  0016cfea: blx 0x0006f514
  0016cfee: subs r2,r4,r1
  0016cff0: sub.w r0,r7,#0x68
  0016cff4: mov r1,r6
  0016cff6: blx 0x00074df4
  0016cffa: add r0,sp,#0x2e4
  0016cffc: sub.w r1,r7,#0x68
  0016d000: movs r2,#0x0
  0016d002: blx 0x0006f028
  0016d006: ldr r1,[0x0016d1d0]
  0016d008: add r1,pc
  0016d00a: add r0,sp,#0x2d8
  0016d00c: movs r2,#0x0
  0016d00e: blx 0x0006ee18
  0016d012: str r0,[sp,#0x0]
  0016d014: sub.w r0,r7,#0x44
  0016d018: add r2,sp,#0x2f0
  0016d01a: add r3,sp,#0x2e4
  0016d01c: mov r1,r5
  0016d01e: blx 0x000739b4
  0016d022: sub.w r1,r7,#0x44
  0016d026: mov r0,r10
  0016d028: blx 0x0006f2b0
  0016d02c: ldr r0,[0x0016d1d4]
  0016d02e: add r0,pc
  0016d030: ldr r4,[r0,#0x0]
  0016d032: sub.w r0,r7,#0x44
  0016d036: blx r4
  0016d038: add r0,sp,#0x2d8
  0016d03a: blx r4
  0016d03c: add r0,sp,#0x2e4
  0016d03e: blx r4
  0016d040: sub.w r0,r7,#0x68
  0016d044: blx r4
  0016d046: add r0,sp,#0x2f0
  0016d048: blx r4
  0016d04a: add r0,sp,#0x2cc
  0016d04c: mov r1,r10
  0016d04e: movs r2,#0x0
  0016d050: ldr.w r5,[r8,#0x0]
  0016d054: blx 0x0006f028
  0016d058: ldr r1,[0x0016d1d8]
  0016d05a: add r1,pc
  0016d05c: add r0,sp,#0x2c0
  0016d05e: movs r2,#0x0
  0016d060: blx 0x0006ee18
  0016d064: add r1,sp,#0x2cc
  0016d066: add r2,sp,#0x2c0
  0016d068: mov r0,r5
  0016d06a: blx 0x00077890
  0016d06e: mov r5,r0
  0016d070: add r0,sp,#0x2c0
  0016d072: blx 0x0006ee30
  0016d076: add r0,sp,#0x2cc
  0016d078: blx 0x0006ee30
  0016d07c: cmp r5,#0x0
  0016d07e: beq 0x0016d10a
  0016d080: ldr r0,[0x0016d1dc]
  0016d082: ldr r1,[0x0016d1e0]
  0016d084: add r0,pc
  0016d086: add r1,pc
  0016d088: ldr r0,[r0,#0x0]
  0016d08a: ldr r1,[r1,#0x0]
  0016d08c: ldr r4,[r0,#0x0]
  0016d08e: ldr r0,[r1,#0x0]
  0016d090: movs r1,#0x4
  0016d092: blx 0x00071848
  0016d096: addw r1,r0,#0x4ff
  0016d09a: mov r0,r4
  0016d09c: blx 0x00072f70
  0016d0a0: mov r1,r0
  0016d0a2: sub.w r0,r7,#0x44
  0016d0a6: movs r2,#0x0
  0016d0a8: blx 0x0006f028
  0016d0ac: ldr.w r5,[r8,#0x0]
  0016d0b0: add r0,sp,#0x2b4
  0016d0b2: mov r1,r10
  0016d0b4: movs r2,#0x0
  0016d0b6: blx 0x0006f028
  0016d0ba: add r0,sp,#0x2a8
  0016d0bc: sub.w r1,r7,#0x44
  0016d0c0: movs r2,#0x0
  0016d0c2: blx 0x0006f028
  0016d0c6: ldr r1,[0x0016d1e4]
  0016d0c8: add r1,pc
  0016d0ca: add r0,sp,#0x29c
  0016d0cc: movs r2,#0x0
  0016d0ce: blx 0x0006ee18
  0016d0d2: str r0,[sp,#0x0]
  0016d0d4: sub.w r0,r7,#0x68
  0016d0d8: add r2,sp,#0x2b4
  0016d0da: add r3,sp,#0x2a8
  0016d0dc: mov r1,r5
  0016d0de: blx 0x000739b4
  0016d0e2: sub.w r1,r7,#0x68
  0016d0e6: mov r0,r10
  0016d0e8: blx 0x0006f2b0
  0016d0ec: ldr r0,[0x0016d1e8]
  0016d0ee: add r0,pc
  0016d0f0: ldr r4,[r0,#0x0]
  0016d0f2: sub.w r0,r7,#0x68
  0016d0f6: blx r4
  0016d0f8: add r0,sp,#0x29c
  0016d0fa: blx r4
  0016d0fc: add r0,sp,#0x2a8
  0016d0fe: blx r4
  0016d100: add r0,sp,#0x2b4
  0016d102: blx r4
  0016d104: sub.w r0,r7,#0x44
  0016d108: blx r4
  0016d10a: add r0,sp,#0x290
  0016d10c: mov r1,r10
  0016d10e: movs r2,#0x0
  0016d110: ldr.w r5,[r8,#0x0]
  0016d114: blx 0x0006f028
  0016d118: ldr r1,[0x0016d1ec]
  0016d11a: add r1,pc
  0016d11c: add r0,sp,#0x284
  0016d11e: movs r2,#0x0
  0016d120: blx 0x0006ee18
  0016d124: add r1,sp,#0x290
  0016d126: add r2,sp,#0x284
  0016d128: mov r0,r5
  0016d12a: blx 0x00077890
  0016d12e: mov r5,r0
  0016d130: add r0,sp,#0x284
  0016d132: blx 0x0006ee30
  0016d136: add r0,sp,#0x290
  0016d138: blx 0x0006ee30
  0016d13c: cmp r5,#0x0
  0016d13e: beq.w 0x0016d3ba
  0016d142: ldr r0,[0x0016d1f0]
  0016d144: movs r1,#0x64
  0016d146: add r0,pc
  0016d148: ldr r4,[r0,#0x0]
  0016d14a: ldr r0,[r4,#0x0]
  0016d14c: blx 0x00071848
  0016d150: mov r6,r0
  0016d152: sub.w r0,r7,#0x44
  0016d156: blx 0x0006efbc
  0016d15a: sub.w r0,r7,#0x68
  0016d15e: blx 0x0006efbc
  0016d162: cmp r6,#0x27
  0016d164: bgt 0x0016d182
  0016d166: ldr r0,[0x0016d1f4]
  0016d168: add r0,pc
  0016d16a: ldr r0,[r0,#0x0]
  0016d16c: ldr r0,[r0,#0x0]
  0016d16e: movw r1,#0x39a
  0016d172: blx 0x00072f70
  0016d176: mov r1,r0
  0016d178: sub.w r0,r7,#0x44
  0016d17c: blx 0x0006f2b0
  0016d180: b 0x0016d278
  0016d182: movs r0,#0xc
  0016d184: blx 0x0006eb24
  0016d188: mov r11,r0
  0016d18a: str r6,[sp,#0x8]
  0016d18c: blx 0x00071860
  0016d190: ldr r0,[0x0016d1f8]
  0016d192: movs r5,#0x0
  0016d194: add r0,pc
  0016d196: ldr.w r9,[r0,#0x0]
  0016d19a: b 0x0016d22a
  0016d1fc: ldr r0,[r0,#0x4]
  0016d1fe: ldr.w r6,[r0,r5,lsl #0x2]
  0016d202: mov r0,r6
  0016d204: movs r1,#0x3c
  0016d206: blx 0x000718e4
  0016d20a: cmp r0,#0x1
  0016d20c: bne 0x0016d228
  0016d20e: mov r0,r6
  0016d210: blx 0x00071914
  0016d214: cbnz r0,0x0016d228
  0016d216: mov r0,r6
  0016d218: blx 0x000718fc
  0016d21c: cmp r0,#0x4
  0016d21e: beq 0x0016d228
  0016d220: mov r0,r6
  0016d222: mov r1,r11
  0016d224: blx 0x000718b4
  0016d228: adds r5,#0x1
  0016d22a: ldr.w r0,[r9,#0x0]
  0016d22e: ldr r1,[r0,#0x0]
  0016d230: cmp r5,r1
  0016d232: bcc 0x0016d1fc
  0016d234: ldr.w r1,[r11,#0x0]
  0016d238: ldr r0,[r4,#0x0]
  0016d23a: blx 0x00071848
  0016d23e: ldr.w r1,[r11,#0x4]
  0016d242: ldr.w r0,[r1,r0,lsl #0x2]
  0016d246: blx 0x000718d8
  0016d24a: ldr r1,[0x0016d260]
  0016d24c: add r1,pc
  0016d24e: ldr r1,[r1,#0x0]
  0016d250: ldr r2,[r1,#0x0]
  0016d252: addw r1,r0,#0x4fa
  0016d256: mov r0,r2
  0016d258: blx 0x00072f70
  0016d25c: mov r1,r0
  0016d25e: b 0x0016d264
  0016d264: sub.w r0,r7,#0x44
  0016d268: blx 0x0006f2b0
  0016d26c: mov r0,r11
  0016d26e: blx 0x0007342c
  0016d272: blx 0x0006eb48
  0016d276: ldr r6,[sp,#0x8]
  0016d278: ldr r0,[0x0016d28c]
  0016d27a: add r0,pc
  0016d27c: ldr r0,[r0,#0x0]
  0016d27e: ldr r5,[r0,#0x0]
  0016d280: ldr.w r0,[r8,#0x0]
  0016d284: blx 0x00071710
  0016d288: b 0x0016d290
  0016d290: ldr r2,[0x0016d2ac]
  0016d292: movs r3,#0x0
  0016d294: bl 0x001aae54
  0016d298: movs r1,#0xc
  0016d29a: cmp r6,#0x28
  0016d29c: it lt
  0016d29e: mov.lt r1,#0x8c
  0016d2a0: adds r2,r0,r1
  0016d2a2: add r0,sp,#0x35c
  0016d2a4: mov r1,r5
  0016d2a6: blx 0x00074df4
  0016d2aa: b 0x0016d2b0
  0016d2b0: sub.w r0,r7,#0x68
  0016d2b4: add r1,sp,#0x35c
  0016d2b6: blx 0x0006f2b0
  0016d2ba: add r0,sp,#0x35c
  0016d2bc: blx 0x0006ee30
  0016d2c0: ldr.w r5,[r8,#0x0]
  0016d2c4: add r0,sp,#0x278
  0016d2c6: mov r1,r10
  0016d2c8: movs r2,#0x0
  0016d2ca: blx 0x0006f028
  0016d2ce: add r0,sp,#0x26c
  0016d2d0: sub.w r1,r7,#0x44
  0016d2d4: movs r2,#0x0
  0016d2d6: blx 0x0006f028
  0016d2da: ldr r1,[0x0016d2e8]
  0016d2dc: add r1,pc
  0016d2de: add r0,sp,#0x260
  0016d2e0: movs r2,#0x0
  0016d2e2: blx 0x0006ee18
  0016d2e6: b 0x0016d2ec
  0016d2ec: str r0,[sp,#0x0]
  0016d2ee: add r0,sp,#0x35c
  0016d2f0: add r2,sp,#0x278
  0016d2f2: add r3,sp,#0x26c
  0016d2f4: mov r1,r5
  0016d2f6: blx 0x000739b4
  0016d2fa: add r1,sp,#0x35c
  0016d2fc: mov r0,r10
  0016d2fe: blx 0x0006f2b0
  0016d302: ldr r0,[0x0016d328]
  0016d304: add r0,pc
  0016d306: ldr r4,[r0,#0x0]
  0016d308: add r0,sp,#0x35c
  0016d30a: blx r4
  0016d30c: add r0,sp,#0x260
  0016d30e: blx r4
  0016d310: add r0,sp,#0x26c
  0016d312: blx r4
  0016d314: add r0,sp,#0x278
  0016d316: blx r4
  0016d318: ldr.w r5,[r8,#0x0]
  0016d31c: add r0,sp,#0x254
  0016d31e: mov r1,r10
  0016d320: movs r2,#0x0
  0016d322: blx 0x0006f028
  0016d326: b 0x0016d32c
  0016d32c: add r0,sp,#0x230
  0016d32e: sub.w r1,r7,#0x68
  0016d332: movs r2,#0x0
  0016d334: blx 0x0006f028
  0016d338: ldr r1,[0x0016d348]
  0016d33a: add r1,pc
  0016d33c: add r0,sp,#0x224
  0016d33e: movs r2,#0x0
  0016d340: blx 0x0006ee18
  0016d344: b 0x0016d34c
  0016d34c: add r0,sp,#0x23c
  0016d34e: add r1,sp,#0x230
  0016d350: add r2,sp,#0x224
  0016d352: blx 0x0006ef98
  0016d356: ldr r0,[0x0016d73c]
  0016d358: add r0,pc
  0016d35a: ldr r0,[r0,#0x0]
  0016d35c: ldr r0,[r0,#0x0]
  0016d35e: movw r1,#0xcaa
  0016d362: blx 0x00072f70
  0016d366: mov r2,r0
  0016d368: add r0,sp,#0x248
  0016d36a: add r1,sp,#0x23c
  0016d36c: blx 0x0006ef98
  0016d370: ldr r1,[0x0016d740]
  0016d372: add r1,pc
  0016d374: add r0,sp,#0x218
  0016d376: movs r2,#0x0
  0016d378: blx 0x0006ee18
  0016d37c: str r0,[sp,#0x0]
  0016d37e: add r0,sp,#0x35c
  0016d380: add r2,sp,#0x254
  0016d382: add r3,sp,#0x248
  0016d384: mov r1,r5
  0016d386: blx 0x000739b4
  0016d38a: add r1,sp,#0x35c
  0016d38c: mov r0,r10
  0016d38e: blx 0x0006f2b0
  0016d392: add r0,sp,#0x35c
  0016d394: blx r4
  0016d396: add r0,sp,#0x218
  0016d398: blx r4
  0016d39a: add r0,sp,#0x248
  0016d39c: blx r4
  0016d39e: add r0,sp,#0x23c
  0016d3a0: blx r4
  0016d3a2: add r0,sp,#0x224
  0016d3a4: blx r4
  0016d3a6: add r0,sp,#0x230
  0016d3a8: blx r4
  0016d3aa: add r0,sp,#0x254
  0016d3ac: blx r4
  0016d3ae: sub.w r0,r7,#0x68
  0016d3b2: blx r4
  0016d3b4: sub.w r0,r7,#0x44
  0016d3b8: blx r4
  0016d3ba: add r0,sp,#0x20c
  0016d3bc: mov r1,r10
  0016d3be: movs r2,#0x0
  0016d3c0: ldr.w r5,[r8,#0x0]
  0016d3c4: blx 0x0006f028
  0016d3c8: ldr r1,[0x0016d744]
  0016d3ca: add r1,pc
  0016d3cc: add r0,sp,#0x200
  0016d3ce: movs r2,#0x0
  0016d3d0: blx 0x0006ee18
  0016d3d4: add r1,sp,#0x20c
  0016d3d6: add r2,sp,#0x200
  0016d3d8: mov r0,r5
  0016d3da: blx 0x00077890
  0016d3de: mov r5,r0
  0016d3e0: add r0,sp,#0x200
  0016d3e2: blx 0x0006ee30
  0016d3e6: add r0,sp,#0x20c
  0016d3e8: blx 0x0006ee30
  0016d3ec: cmp r5,#0x0
  0016d3ee: beq.w 0x0016d54e
  0016d3f2: add r0,sp,#0x1f4
  0016d3f4: mov r1,r10
  0016d3f6: movs r2,#0x0
  0016d3f8: ldr.w r5,[r8,#0x0]
  0016d3fc: blx 0x0006f028
  0016d400: ldr r0,[0x0016d748]
  0016d402: ldr r1,[0x0016d74c]
  0016d404: add r0,pc
  0016d406: add r1,pc
  0016d408: ldr.w r9,[r0,#0x0]
  0016d40c: ldr.w r11,[r1,#0x0]
  0016d410: ldr.w r6,[r9,#0x0]
  0016d414: ldr.w r0,[r11,#0x0]
  0016d418: movs r1,#0x4
  0016d41a: blx 0x00071848
  0016d41e: addw r1,r0,#0xcab
  0016d422: mov r0,r6
  0016d424: blx 0x00072f70
  0016d428: mov r1,r0
  0016d42a: add r0,sp,#0x1e8
  0016d42c: movs r2,#0x0
  0016d42e: blx 0x0006f028
  0016d432: ldr r1,[0x0016d750]
  0016d434: add r1,pc
  0016d436: add r0,sp,#0x1dc
  0016d438: movs r2,#0x0
  0016d43a: blx 0x0006ee18
  0016d43e: str r0,[sp,#0x0]
  0016d440: sub.w r0,r7,#0x44
  0016d444: add r2,sp,#0x1f4
  0016d446: add r3,sp,#0x1e8
  0016d448: mov r1,r5
  0016d44a: blx 0x000739b4
  0016d44e: sub.w r1,r7,#0x44
  0016d452: mov r0,r10
  0016d454: blx 0x0006f2b0
  0016d458: ldr r0,[0x0016d754]
  0016d45a: add r0,pc
  0016d45c: ldr r4,[r0,#0x0]
  0016d45e: sub.w r0,r7,#0x44
  0016d462: blx r4
  0016d464: add r0,sp,#0x1dc
  0016d466: blx r4
  0016d468: add r0,sp,#0x1e8
  0016d46a: blx r4
  0016d46c: add r0,sp,#0x1f4
  0016d46e: blx r4
  0016d470: ldr.w r0,[r11,#0x0]
  0016d474: movs r1,#0x8
  0016d476: blx 0x00071848
  0016d47a: mov r2,r0
  0016d47c: ldr r0,[0x0016d758]
  0016d47e: movs r3,#0x1
  0016d480: add r0,pc
  0016d482: ldr r0,[r0,#0x0]
  0016d484: ldr r1,[r0,#0x0]
  0016d486: sub.w r0,r7,#0x44
  0016d48a: blx 0x00071b24
  0016d48e: ldr.w r5,[r8,#0x0]
  0016d492: add r0,sp,#0x1d0
  0016d494: mov r1,r10
  0016d496: movs r2,#0x0
  0016d498: blx 0x0006f028
  0016d49c: add r0,sp,#0x1c4
  0016d49e: sub.w r1,r7,#0x44
  0016d4a2: movs r2,#0x0
  0016d4a4: blx 0x0006f028
  0016d4a8: ldr r1,[0x0016d75c]
  0016d4aa: add r1,pc
  0016d4ac: add r0,sp,#0x1b8
  0016d4ae: movs r2,#0x0
  0016d4b0: blx 0x0006ee18
  0016d4b4: str r0,[sp,#0x0]
  0016d4b6: sub.w r0,r7,#0x68
  0016d4ba: add r2,sp,#0x1d0
  0016d4bc: add r3,sp,#0x1c4
  0016d4be: mov r1,r5
  0016d4c0: blx 0x000739b4
  0016d4c4: sub.w r1,r7,#0x68
  0016d4c8: mov r0,r10
  0016d4ca: blx 0x0006f2b0
  0016d4ce: sub.w r0,r7,#0x68
  0016d4d2: blx r4
  0016d4d4: add r0,sp,#0x1b8
  0016d4d6: blx r4
  0016d4d8: add r0,sp,#0x1c4
  0016d4da: blx r4
  0016d4dc: add r0,sp,#0x1d0
  0016d4de: blx r4
  0016d4e0: ldr.w r5,[r8,#0x0]
  0016d4e4: add r0,sp,#0x1ac
  0016d4e6: mov r1,r10
  0016d4e8: movs r2,#0x0
  0016d4ea: blx 0x0006f028
  0016d4ee: ldr.w r6,[r9,#0x0]
  0016d4f2: ldr.w r0,[r11,#0x0]
  0016d4f6: movs r1,#0x4
  0016d4f8: blx 0x00071848
  0016d4fc: addw r1,r0,#0xcaf
  0016d500: mov r0,r6
  0016d502: blx 0x00072f70
  0016d506: mov r1,r0
  0016d508: add r0,sp,#0x1a0
  0016d50a: movs r2,#0x0
  0016d50c: blx 0x0006f028
  0016d510: ldr r1,[0x0016d760]
  0016d512: add r1,pc
  0016d514: add r0,sp,#0x194
  0016d516: movs r2,#0x0
  0016d518: blx 0x0006ee18
  0016d51c: str r0,[sp,#0x0]
  0016d51e: sub.w r0,r7,#0x68
  0016d522: add r2,sp,#0x1ac
  0016d524: add r3,sp,#0x1a0
  0016d526: mov r1,r5
  0016d528: blx 0x000739b4
  0016d52c: sub.w r1,r7,#0x68
  0016d530: mov r0,r10
  0016d532: blx 0x0006f2b0
  0016d536: sub.w r0,r7,#0x68
  0016d53a: blx r4
  0016d53c: add r0,sp,#0x194
  0016d53e: blx r4
  0016d540: add r0,sp,#0x1a0
  0016d542: blx r4
  0016d544: add r0,sp,#0x1ac
  0016d546: blx r4
  0016d548: sub.w r0,r7,#0x44
  0016d54c: blx r4
  0016d54e: add r0,sp,#0x188
  0016d550: mov r1,r10
  0016d552: movs r2,#0x0
  0016d554: ldr.w r5,[r8,#0x0]
  0016d558: blx 0x0006f028
  0016d55c: ldr r1,[0x0016d764]
  0016d55e: add r1,pc
  0016d560: add r0,sp,#0x17c
  0016d562: movs r2,#0x0
  0016d564: blx 0x0006ee18
  0016d568: add r1,sp,#0x188
  0016d56a: add r2,sp,#0x17c
  0016d56c: mov r0,r5
  0016d56e: blx 0x00077890
  0016d572: mov r5,r0
  0016d574: add r0,sp,#0x17c
  0016d576: blx 0x0006ee30
  0016d57a: add r0,sp,#0x188
  0016d57c: blx 0x0006ee30
  0016d580: cmp r5,#0x0
  0016d582: beq.w 0x0016d6e2
  0016d586: add r0,sp,#0x170
  0016d588: mov r1,r10
  0016d58a: movs r2,#0x0
  0016d58c: ldr.w r5,[r8,#0x0]
  0016d590: blx 0x0006f028
  0016d594: ldr r0,[0x0016d768]
  0016d596: ldr r1,[0x0016d76c]
  0016d598: add r0,pc
  0016d59a: add r1,pc
  0016d59c: ldr.w r9,[r0,#0x0]
  0016d5a0: ldr.w r11,[r1,#0x0]
  0016d5a4: ldr.w r6,[r9,#0x0]
  0016d5a8: ldr.w r0,[r11,#0x0]
  0016d5ac: movs r1,#0x4
  0016d5ae: blx 0x00071848
  0016d5b2: add.w r1,r0,#0x196
  0016d5b6: mov r0,r6
  0016d5b8: blx 0x00072f70
  0016d5bc: mov r1,r0
  0016d5be: add r0,sp,#0x164
  0016d5c0: movs r2,#0x0
  0016d5c2: blx 0x0006f028
  0016d5c6: ldr r1,[0x0016d770]
  0016d5c8: add r1,pc
  0016d5ca: add r0,sp,#0x158
  0016d5cc: movs r2,#0x0
  0016d5ce: blx 0x0006ee18
  0016d5d2: str r0,[sp,#0x0]
  0016d5d4: sub.w r0,r7,#0x44
  0016d5d8: add r2,sp,#0x170
  0016d5da: add r3,sp,#0x164
  0016d5dc: mov r1,r5
  0016d5de: blx 0x000739b4
  0016d5e2: sub.w r1,r7,#0x44
  0016d5e6: mov r0,r10
  0016d5e8: blx 0x0006f2b0
  0016d5ec: ldr r0,[0x0016d774]
  0016d5ee: add r0,pc
  0016d5f0: ldr r4,[r0,#0x0]
  0016d5f2: sub.w r0,r7,#0x44
  0016d5f6: blx r4
  0016d5f8: add r0,sp,#0x158
  0016d5fa: blx r4
  0016d5fc: add r0,sp,#0x164
  0016d5fe: blx r4
  0016d600: add r0,sp,#0x170
  0016d602: blx r4
  0016d604: ldr.w r0,[r11,#0x0]
  0016d608: movs r1,#0x8
  0016d60a: blx 0x00071848
  0016d60e: mov r2,r0
  0016d610: ldr r0,[0x0016d778]
  0016d612: movs r3,#0x1
  0016d614: add r0,pc
  0016d616: ldr r0,[r0,#0x0]
  0016d618: ldr r1,[r0,#0x0]
  0016d61a: sub.w r0,r7,#0x44
  0016d61e: blx 0x00071b24
  0016d622: ldr.w r5,[r8,#0x0]
  0016d626: add r0,sp,#0x14c
  0016d628: mov r1,r10
  0016d62a: movs r2,#0x0
  0016d62c: blx 0x0006f028
  0016d630: add r0,sp,#0x140
  0016d632: sub.w r1,r7,#0x44
  0016d636: movs r2,#0x0
  0016d638: blx 0x0006f028
  0016d63c: ldr r1,[0x0016d77c]
  0016d63e: add r1,pc
  0016d640: add r0,sp,#0x134
  0016d642: movs r2,#0x0
  0016d644: blx 0x0006ee18
  0016d648: str r0,[sp,#0x0]
  0016d64a: sub.w r0,r7,#0x68
  0016d64e: add r2,sp,#0x14c
  0016d650: add r3,sp,#0x140
  0016d652: mov r1,r5
  0016d654: blx 0x000739b4
  0016d658: sub.w r1,r7,#0x68
  0016d65c: mov r0,r10
  0016d65e: blx 0x0006f2b0
  0016d662: sub.w r0,r7,#0x68
  0016d666: blx r4
  0016d668: add r0,sp,#0x134
  0016d66a: blx r4
  0016d66c: add r0,sp,#0x140
  0016d66e: blx r4
  0016d670: add r0,sp,#0x14c
  0016d672: blx r4
  0016d674: ldr.w r5,[r8,#0x0]
  0016d678: add r0,sp,#0x128
  0016d67a: mov r1,r10
  0016d67c: movs r2,#0x0
  0016d67e: blx 0x0006f028
  0016d682: ldr.w r6,[r9,#0x0]
  0016d686: ldr.w r0,[r11,#0x0]
  0016d68a: movs r1,#0x5
  0016d68c: blx 0x00071848
  0016d690: addw r1,r0,#0xcb3
  0016d694: mov r0,r6
  0016d696: blx 0x00072f70
  0016d69a: mov r1,r0
  0016d69c: add r0,sp,#0x11c
  0016d69e: movs r2,#0x0
  0016d6a0: blx 0x0006f028
  0016d6a4: ldr r1,[0x0016d780]
  0016d6a6: add r1,pc
  0016d6a8: add r0,sp,#0x110
  0016d6aa: movs r2,#0x0
  0016d6ac: blx 0x0006ee18
  0016d6b0: str r0,[sp,#0x0]
  0016d6b2: sub.w r0,r7,#0x68
  0016d6b6: add r2,sp,#0x128
  0016d6b8: add r3,sp,#0x11c
  0016d6ba: mov r1,r5
  0016d6bc: blx 0x000739b4
  0016d6c0: sub.w r1,r7,#0x68
  0016d6c4: mov r0,r10
  0016d6c6: blx 0x0006f2b0
  0016d6ca: sub.w r0,r7,#0x68
  0016d6ce: blx r4
  0016d6d0: add r0,sp,#0x110
  0016d6d2: blx r4
  0016d6d4: add r0,sp,#0x11c
  0016d6d6: blx r4
  0016d6d8: add r0,sp,#0x128
  0016d6da: blx r4
  0016d6dc: sub.w r0,r7,#0x44
  0016d6e0: blx r4
  0016d6e2: add r0,sp,#0x104
  0016d6e4: mov r1,r10
  0016d6e6: movs r2,#0x0
  0016d6e8: ldr.w r5,[r8,#0x0]
  0016d6ec: blx 0x0006f028
  0016d6f0: ldr r1,[0x0016d784]
  0016d6f2: add r1,pc
  0016d6f4: add r0,sp,#0xf8
  0016d6f6: movs r2,#0x0
  0016d6f8: blx 0x0006ee18
  0016d6fc: add r1,sp,#0x104
  0016d6fe: add r2,sp,#0xf8
  0016d700: mov r0,r5
  0016d702: blx 0x00077890
  0016d706: mov r5,r0
  0016d708: add r0,sp,#0xf8
  0016d70a: blx 0x0006ee30
  0016d70e: add r0,sp,#0x104
  0016d710: blx 0x0006ee30
  0016d714: cmp r5,#0x0
  0016d716: beq.w 0x0016d8de
  0016d71a: ldr r0,[0x0016d788]
  0016d71c: add r0,pc
  0016d71e: ldr.w r11,[r0,#0x0]
  0016d722: ldr.w r0,[r11,#0x0]
  0016d726: blx 0x00071884
  0016d72a: mov r5,r0
  0016d72c: ldr r0,[0x0016d78c]
  0016d72e: mov.w r6,#0xffffffff
  0016d732: add r0,pc
  0016d734: ldr.w r9,[r0,#0x0]
  0016d738: b 0x0016d7b2
  0016d790: ldr r1,[r5,#0x0]
  0016d792: ldr.w r0,[r9,#0x0]
  0016d796: blx 0x00071848
  0016d79a: ldr r1,[r5,#0x4]
  0016d79c: ldr.w r4,[r1,r0,lsl #0x2]
  0016d7a0: mov r0,r4
  0016d7a2: blx 0x00071950
  0016d7a6: cmp r0,#0x2
  0016d7a8: bne 0x0016d7b2
  0016d7aa: mov r0,r4
  0016d7ac: blx 0x00071a7c
  0016d7b0: mov r6,r0
  0016d7b2: cmp.w r6,#0xffffffff
  0016d7b6: ble 0x0016d790
  0016d7b8: ldr r0,[r5,#0x4]
  0016d7ba: ldr.w r0,[r0,r6,lsl #0x2]
  0016d7be: blx 0x00071a88
  0016d7c2: mov r4,r0
  0016d7c4: ldr r1,[r0,#0x0]
  0016d7c6: ldr.w r0,[r9,#0x0]
  0016d7ca: ldr.w r5,[r11,#0x0]
  0016d7ce: blx 0x00071848
  0016d7d2: ldr r1,[r4,#0x4]
  0016d7d4: ldr.w r1,[r1,r0,lsl #0x2]
  0016d7d8: mov r0,r5
  0016d7da: blx 0x00071c44
  0016d7de: mov r11,r0
  0016d7e0: add r0,sp,#0xec
  0016d7e2: mov r1,r10
  0016d7e4: movs r2,#0x0
  0016d7e6: ldr.w r5,[r8,#0x0]
  0016d7ea: blx 0x0006f028
  0016d7ee: sub.w r0,r7,#0x68
  0016d7f2: mov r1,r11
  0016d7f4: blx 0x000736a8
  0016d7f8: add r0,sp,#0xe0
  0016d7fa: sub.w r1,r7,#0x68
  0016d7fe: movs r2,#0x0
  0016d800: blx 0x0006f028
  0016d804: ldr r1,[0x0016dbb4]
  0016d806: add r1,pc
  0016d808: add r0,sp,#0xd4
  0016d80a: movs r2,#0x0
  0016d80c: blx 0x0006ee18
  0016d810: str r0,[sp,#0x0]
  0016d812: sub.w r0,r7,#0x44
  0016d816: add r2,sp,#0xec
  0016d818: add r3,sp,#0xe0
  0016d81a: mov r1,r5
  0016d81c: blx 0x000739b4
  0016d820: sub.w r1,r7,#0x44
  0016d824: mov r0,r10
  0016d826: blx 0x0006f2b0
  0016d82a: ldr r0,[0x0016dbb8]
  0016d82c: add r0,pc
  0016d82e: ldr r6,[r0,#0x0]
  0016d830: sub.w r0,r7,#0x44
  0016d834: blx r6
  0016d836: add r0,sp,#0xd4
  0016d838: blx r6
  0016d83a: add r0,sp,#0xe0
  0016d83c: blx r6
  0016d83e: sub.w r0,r7,#0x68
  0016d842: blx r6
  0016d844: add r0,sp,#0xec
  0016d846: blx r6
  0016d848: ldr r0,[0x0016dbbc]
  0016d84a: ldr r1,[0x0016dbc0]
  0016d84c: add r0,pc
  0016d84e: ldr r0,[r0,#0x0]
  0016d850: ldr r4,[r0,#0x0]
  0016d852: ldr.w r0,[r9,#0x0]
  0016d856: blx 0x00071848
  0016d85a: ldr r1,[0x0016dbc4]
  0016d85c: adds r5,r0,r1
  0016d85e: mov.w r1,#0x3e8
  0016d862: mov r0,r5
  0016d864: blx 0x0006f514
  0016d868: subs r2,r5,r1
  0016d86a: sub.w r0,r7,#0x44
  0016d86e: mov r1,r4
  0016d870: blx 0x00074df4
  0016d874: ldr.w r5,[r8,#0x0]
  0016d878: add r0,sp,#0xc8
  0016d87a: mov r1,r10
  0016d87c: movs r2,#0x0
  0016d87e: blx 0x0006f028
  0016d882: add r0,sp,#0xbc
  0016d884: sub.w r1,r7,#0x44
  0016d888: movs r2,#0x0
  0016d88a: blx 0x0006f028
  0016d88e: ldr r1,[0x0016dbc8]
  0016d890: add r1,pc
  0016d892: add r0,sp,#0xb0
  0016d894: movs r2,#0x0
  0016d896: blx 0x0006ee18
  0016d89a: str r0,[sp,#0x0]
  0016d89c: sub.w r0,r7,#0x68
  0016d8a0: add r2,sp,#0xc8
  0016d8a2: add r3,sp,#0xbc
  0016d8a4: mov r1,r5
  0016d8a6: blx 0x000739b4
  0016d8aa: sub.w r1,r7,#0x68
  0016d8ae: mov r0,r10
  0016d8b0: blx 0x0006f2b0
  0016d8b4: sub.w r0,r7,#0x68
  0016d8b8: blx r6
  0016d8ba: add r0,sp,#0xb0
  0016d8bc: blx r6
  0016d8be: add r0,sp,#0xbc
  0016d8c0: blx r6
  0016d8c2: add r0,sp,#0xc8
  0016d8c4: blx r6
  0016d8c6: cmp.w r11,#0x0
  0016d8ca: beq 0x0016d8d6
  0016d8cc: mov r0,r11
  0016d8ce: blx 0x0007360c
  0016d8d2: blx 0x0006eb48
  0016d8d6: sub.w r0,r7,#0x44
  0016d8da: blx 0x0006ee30
  0016d8de: add r0,sp,#0xa4
  0016d8e0: mov r1,r10
  0016d8e2: movs r2,#0x0
  0016d8e4: ldr.w r5,[r8,#0x0]
  0016d8e8: blx 0x0006f028
  0016d8ec: ldr r1,[0x0016dbcc]
  0016d8ee: add r1,pc
  0016d8f0: add r0,sp,#0x98
  0016d8f2: movs r2,#0x0
  0016d8f4: blx 0x0006ee18
  0016d8f8: add r1,sp,#0xa4
  0016d8fa: add r2,sp,#0x98
  0016d8fc: mov r0,r5
  0016d8fe: blx 0x00077890
  0016d902: mov r5,r0
  0016d904: add r0,sp,#0x98
  0016d906: blx 0x0006ee30
  0016d90a: add r0,sp,#0xa4
  0016d90c: blx 0x0006ee30
  0016d910: cmp r5,#0x0
  0016d912: beq 0x0016d9e8
  0016d914: add r0,sp,#0x8c
  0016d916: mov r1,r10
  0016d918: movs r2,#0x0
  0016d91a: ldr.w r5,[r8,#0x0]
  0016d91e: blx 0x0006f028
  0016d922: ldr r0,[0x0016dbd0]
  0016d924: add r0,pc
  0016d926: ldr.w r9,[r0,#0x0]
  0016d92a: ldr.w r0,[r9,#0x0]
  0016d92e: movs r1,#0x50
  0016d930: blx 0x00071848
  0016d934: add.w r1,r0,#0xa
  0016d938: add r0,sp,#0x80
  0016d93a: blx 0x0006f658
  0016d93e: ldr r1,[0x0016dbd4]
  0016d940: add r1,pc
  0016d942: add r0,sp,#0x74
  0016d944: movs r2,#0x0
  0016d946: blx 0x0006ee18
  0016d94a: str r0,[sp,#0x0]
  0016d94c: sub.w r0,r7,#0x44
  0016d950: add r2,sp,#0x8c
  0016d952: add r3,sp,#0x80
  0016d954: mov r1,r5
  0016d956: blx 0x000739b4
  0016d95a: sub.w r1,r7,#0x44
  0016d95e: mov r0,r10
  0016d960: blx 0x0006f2b0
  0016d964: ldr r0,[0x0016dbd8]
  0016d966: add r0,pc
  0016d968: ldr r4,[r0,#0x0]
  0016d96a: sub.w r0,r7,#0x44
  0016d96e: blx r4
  0016d970: add r0,sp,#0x74
  0016d972: blx r4
  0016d974: add r0,sp,#0x80
  0016d976: blx r4
  0016d978: add r0,sp,#0x8c
  0016d97a: blx r4
  0016d97c: add r0,sp,#0x68
  0016d97e: mov r1,r10
  0016d980: movs r2,#0x0
  0016d982: ldr.w r5,[r8,#0x0]
  0016d986: blx 0x0006f028
  0016d98a: ldr r0,[0x0016dbdc]
  0016d98c: add r0,pc
  0016d98e: ldr r0,[r0,#0x0]
  0016d990: ldr r6,[r0,#0x0]
  0016d992: ldr.w r0,[r9,#0x0]
  0016d996: movs r1,#0x6
  0016d998: blx 0x00071848
  0016d99c: addw r1,r0,#0xcb8
  0016d9a0: mov r0,r6
  0016d9a2: blx 0x00072f70
  0016d9a6: mov r1,r0
  0016d9a8: add r0,sp,#0x5c
  0016d9aa: movs r2,#0x0
  0016d9ac: blx 0x0006f028
  0016d9b0: ldr r1,[0x0016dbe0]
  0016d9b2: add r1,pc
  0016d9b4: add r0,sp,#0x50
  0016d9b6: movs r2,#0x0
  0016d9b8: blx 0x0006ee18
  0016d9bc: str r0,[sp,#0x0]
  0016d9be: sub.w r0,r7,#0x44
  0016d9c2: add r2,sp,#0x68
  0016d9c4: add r3,sp,#0x5c
  0016d9c6: mov r1,r5
  0016d9c8: blx 0x000739b4
  0016d9cc: sub.w r1,r7,#0x44
  0016d9d0: mov r0,r10
  0016d9d2: blx 0x0006f2b0
  0016d9d6: sub.w r0,r7,#0x44
  0016d9da: blx r4
  0016d9dc: add r0,sp,#0x50
  0016d9de: blx r4
  0016d9e0: add r0,sp,#0x5c
  0016d9e2: blx r4
  0016d9e4: add r0,sp,#0x68
  0016d9e6: blx r4
  0016d9e8: add r0,sp,#0x44
  0016d9ea: mov r1,r10
  0016d9ec: movs r2,#0x0
  0016d9ee: ldr.w r5,[r8,#0x0]
  0016d9f2: blx 0x0006f028
  0016d9f6: ldr r1,[0x0016dbe4]
  0016d9f8: add r1,pc
  0016d9fa: add r0,sp,#0x38
  0016d9fc: movs r2,#0x0
  0016d9fe: blx 0x0006ee18
  0016da02: add r1,sp,#0x44
  0016da04: add r2,sp,#0x38
  0016da06: mov r0,r5
  0016da08: blx 0x00077890
  0016da0c: mov r5,r0
  0016da0e: add r0,sp,#0x38
  0016da10: blx 0x0006ee30
  0016da14: add r0,sp,#0x44
  0016da16: blx 0x0006ee30
  0016da1a: cmp r5,#0x0
  0016da1c: beq 0x0016daa6
  0016da1e: add r0,sp,#0x2c
  0016da20: mov r1,r10
  0016da22: movs r2,#0x0
  0016da24: ldr.w r5,[r8,#0x0]
  0016da28: blx 0x0006f028
  0016da2c: ldr r0,[0x0016dbe8]
  0016da2e: movs r1,#0x2
  0016da30: str r1,[sp,#0x23c]
  0016da32: add r0,pc
  0016da34: ldr r0,[r0,#0x0]
  0016da36: ldr r0,[r0,#0x0]
  0016da38: movs r1,#0x30
  0016da3a: blx 0x00071848
  0016da3e: mov r1,r0
  0016da40: add r0,sp,#0x35c
  0016da42: blx 0x0006f658
  0016da46: sub.w r0,r7,#0x68
  0016da4a: add r1,sp,#0x35c
  0016da4c: movs r2,#0x0
  0016da4e: blx 0x0006f028
  0016da52: add r0,sp,#0x20
  0016da54: add r1,sp,#0x23c
  0016da56: sub.w r2,r7,#0x68
  0016da5a: blx 0x0007690c
  0016da5e: ldr r1,[0x0016dbec]
  0016da60: add r1,pc
  0016da62: add r0,sp,#0x14
  0016da64: movs r2,#0x0
  0016da66: blx 0x0006ee18
  0016da6a: str r0,[sp,#0x0]
  0016da6c: sub.w r0,r7,#0x44
  0016da70: add r2,sp,#0x2c
  0016da72: add r3,sp,#0x20
  0016da74: mov r1,r5
  0016da76: blx 0x000739b4
  0016da7a: sub.w r1,r7,#0x44
  0016da7e: mov r0,r10
  0016da80: blx 0x0006f2b0
  0016da84: ldr r0,[0x0016dbf0]
  0016da86: add r0,pc
  0016da88: ldr r4,[r0,#0x0]
  0016da8a: sub.w r0,r7,#0x44
  0016da8e: blx r4
  0016da90: add r0,sp,#0x14
  0016da92: blx r4
  0016da94: add r0,sp,#0x20
  0016da96: blx r4
  0016da98: sub.w r0,r7,#0x68
  0016da9c: blx r4
  0016da9e: add r0,sp,#0x35c
  0016daa0: blx r4
  0016daa2: add r0,sp,#0x2c
  0016daa4: blx r4
  0016daa6: ldr r0,[sp,#0xc]
  0016daa8: mov r1,r10
  0016daaa: movs r2,#0x0
  0016daac: blx 0x0006f028
  0016dab0: ldr.w r0,[r7,#-0x20]
  0016dab4: ldr r1,[sp,#0x10]
  0016dab6: ldr r1,[r1,#0x0]
  0016dab8: subs r0,r1,r0
  0016daba: itttt eq
  0016dabc: add.eq.w sp,sp,#0x518
  0016dac0: add.eq sp,#0x4
  0016dac2: pop.eq.w {r8,r9,r10,r11}
  0016dac6: pop.eq {r4,r5,r6,r7,pc}
  0016dac8: blx 0x0006e824
```
