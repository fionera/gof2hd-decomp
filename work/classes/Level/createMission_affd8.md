# Level::createMission
elf 0xaffd8 body 16166
Sig: undefined __thiscall createMission(Level * this)

## decompile
```c

/* Level::createMission() */

void __thiscall Level::createMission(Level *this)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  byte bVar5;
  bool bVar6;
  bool bVar7;
  double dVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  PlayerFixedObject PVar13;
  Mission *pMVar14;
  int iVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  int iVar18;
  Array<KIPlayer*> *pAVar19;
  undefined4 *puVar20;
  code *pcVar21;
  int iVar22;
  Station *pSVar23;
  int iVar24;
  SolarSystem *pSVar25;
  int iVar26;
  Route *pRVar27;
  Standing *pSVar28;
  int iVar29;
  Level *pLVar30;
  Level *pLVar31;
  int iVar32;
  Wanted *this_00;
  int iVar33;
  int iVar34;
  Waypoint *pWVar35;
  String *pSVar36;
  undefined4 *puVar37;
  Objective *pOVar38;
  Route *pRVar39;
  Agent *pAVar40;
  Mission *pMVar41;
  uint uVar42;
  int iVar43;
  void *pvVar44;
  Generator *this_01;
  FileRead *this_02;
  Array *pAVar45;
  int *piVar46;
  uint *puVar47;
  void *__child_stack;
  void *__child_stack_00;
  Route *pRVar48;
  void *__child_stack_01;
  int iVar49;
  int __flags;
  int __flags_00;
  int __flags_01;
  int __flags_02;
  int __flags_03;
  int __flags_04;
  int __flags_05;
  int iVar50;
  void *__arg;
  void *__arg_00;
  void *__arg_01;
  void *__arg_02;
  void *__arg_03;
  void *__arg_04;
  void *__arg_05;
  int iVar51;
  KIPlayer *pKVar52;
  PlayerTurret *pPVar53;
  uint uVar54;
  Player *pPVar55;
  int *piVar56;
  code *pcVar57;
  uint uVar58;
  int *piVar59;
  Globals *this_03;
  float *pfVar60;
  PlayerFixedObject *pPVar61;
  int iVar62;
  int *piVar63;
  int iVar64;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float fVar65;
  float extraout_s1;
  float extraout_s1_00;
  float fVar66;
  float extraout_s2;
  float extraout_s2_00;
  float fVar67;
  float fVar68;
  undefined4 uVar69;
  float fVar70;
  float fVar71;
  float fVar72;
  float fVar73;
  double dVar74;
  int local_1ac;
  int local_18c;
  Vector *local_17c;
  int local_178;
  String aSStack_170 [12];
  String aSStack_164 [12];
  String aSStack_158 [12];
  String aSStack_14c [12];
  String aSStack_140 [12];
  undefined4 local_134;
  undefined4 uStack_130;
  undefined4 local_12c;
  float local_128;
  float local_124;
  float local_120;
  float local_11c;
  float local_118;
  float local_114;
  float local_110;
  float local_10c;
  float local_108;
  String aSStack_104 [12];
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  float local_ec;
  float local_e8;
  float local_e4;
  int local_e0;
  float fStack_dc;
  int local_d8;
  undefined4 uStack_d4;
  undefined8 local_d0;
  float local_b8;
  undefined8 local_b4;
  int local_ac;
  int local_a8;
  int iStack_a4;
  ulonglong local_a0;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined4 local_80;
  float local_7c;
  float local_78;
  float local_74;
  undefined8 local_70;
  int local_68;
  int local_64;
  
  piVar63 = *(int **)(DAT_000c036c + 0xbfff0);
  piVar56 = *(int **)(DAT_000c0370 + 0xbfff2);
  local_64 = *piVar63;
  pMVar14 = (Mission *)Status::getMission();
  if (pMVar14 == (Mission *)0x0) goto LAB_000c0194;
  AEGeometry::getPosition();
  iVar15 = Status::inAlienOrbit();
  if (iVar15 != 0) {
    uVar16 = Status::getLevel();
    uVar58 = 2;
    piVar56 = *(int **)(DAT_000c0374 + 0xc003e);
    uVar17 = AbyssEngine::AERandom::nextInt(*piVar56);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    fVar67 = (float)VectorSignedToFloat(uVar17,(byte)(in_fpscr >> 0x16) & 3);
    fVar66 = fVar66 * 0.5 + -1.0;
    in_fpscr = in_fpscr & 0xfffffff;
    if (2.0 <= fVar66 + fVar67) {
      uVar16 = AbyssEngine::AERandom::nextInt(*piVar56);
      fVar67 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
      uVar58 = (uint)(fVar66 + fVar67);
    }
    iVar15 = Status::getCurrentCampaignMission();
    iVar18 = Status::getCurrentCampaignMission();
    if (iVar18 == 0x21) {
      uVar58 = 2;
    }
    if (iVar15 == 0x44) {
      uVar58 = 2;
    }
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(uVar58,(Array *)pAVar19);
    puVar20 = *(undefined4 **)(DAT_000c0378 + 0xc00d6);
    pcVar21 = *(code **)(DAT_000c037c + 0xc00e0);
    for (iVar15 = 0; iVar15 < (int)uVar58; iVar15 = iVar15 + 1) {
      iVar18 = Globals::getRandomEnemyFighter((Globals *)*puVar20,9);
      uVar17 = createShip(this,9,0,iVar18,(Waypoint *)0x0,true,false);
      uVar16 = DAT_000c0380;
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar17;
      piVar59 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4);
      iVar18 = (*pcVar21)(*piVar56,uVar16);
      iVar22 = (*pcVar21)(*piVar56,DAT_000c0384);
      uVar17 = VectorSignedToFloat(iVar18 + -60000,(byte)(in_fpscr >> 0x16) & 3);
      uVar69 = VectorSignedToFloat(iVar22 + -40000,(byte)(in_fpscr >> 0x16) & 3);
      iVar18 = (*pcVar21)(*piVar56,DAT_000c0380);
      uVar16 = VectorSignedToFloat(iVar18 + -60000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar59 + 0x48))(piVar59,uVar17,uVar69,uVar16);
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) + 4),0));
    }
    goto switchD_000c02a8_caseD_8;
  }
  iVar15 = Mission::isEmpty();
  if (iVar15 != 0) {
    piVar59 = *(int **)(DAT_000c0388 + 0xc01c2);
    AbyssEngine::AERandom::reset();
    bVar6 = false;
    *(undefined4 *)(this + 0xc0) = 0;
    createScene(this);
    *(undefined4 *)(this + 0xc0) = 3;
    Status::getSystem();
    iVar15 = SolarSystem::getIndex();
    if ((iVar15 == 0xf) && (iVar15 = Status::getCurrentCampaignMission(), iVar15 < 0x10)) {
      bVar6 = true;
    }
    pSVar23 = (Station *)Status::getStation();
    iVar15 = Station::getIndex(pSVar23);
    iVar18 = AbyssEngine::AERandom::nextInt(*piVar59);
    pMVar14 = (Mission *)Status::getFreelanceMission();
    if ((pMVar14 == (Mission *)0x0) ||
       (((iVar22 = Mission::getType(), iVar22 != 0 && (iVar22 = Mission::getType(), iVar22 != 0xb))
        && (iVar22 = Mission::getType(), iVar22 != 0xf)))) {
      iVar22 = 0;
    }
    else {
      uVar16 = Mission::getDifficulty(pMVar14);
      fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
      iVar22 = (int)((fVar66 / 10.0) * 5.0);
    }
    iVar24 = Status::getSystem();
    if (iVar24 != 0) {
      pSVar25 = (SolarSystem *)Status::getSystem();
      iVar24 = SolarSystem::hasPirateBase(pSVar25);
      if (iVar24 != 0) {
        iVar22 = AbyssEngine::AERandom::nextInt(*piVar59);
        iVar22 = iVar22 + 2;
      }
    }
    pSVar25 = (SolarSystem *)Status::getSystem();
    iVar24 = SolarSystem::getSecurityLevel(pSVar25);
    uVar58 = Status::hardCoreMode();
    iVar24 = iVar24 - (uVar58 & 0 < iVar24);
    if ((bVar6) && (iVar26 = Status::hardCoreMode(), iVar26 == 0)) {
      bVar1 = false;
    }
    else {
      if (iVar24 == 0) {
        iVar26 = 0x5a;
      }
      else if (iVar24 == 1) {
        iVar26 = 0x41;
      }
      else {
        iVar26 = 10;
        if (iVar24 == 2) {
          iVar26 = 0x23;
        }
      }
      bVar1 = iVar18 < iVar26;
    }
    local_e0 = AbyssEngine::AERandom::nextInt(*piVar59);
    local_e0 = local_e0 + -50000;
    fStack_dc = 0.0;
    local_d8 = AbyssEngine::AERandom::nextInt(*piVar59);
    local_d8 = local_d8 + 50000;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,&local_e0,3);
    *(Route **)(this + 0x180) = pRVar27;
    Status::getSystem();
    iVar18 = SolarSystem::getRace();
    iVar26 = AbyssEngine::AERandom::nextInt(*piVar59);
    if (iVar26 < 0x4b) {
      local_1ac = 8;
    }
    else {
      pSVar28 = (Standing *)Status::getStanding();
      local_1ac = Standing::getEnemyRace(pSVar28,iVar18);
    }
    fVar66 = DAT_000c06dc;
    if (bVar1) {
      uVar16 = AbyssEngine::AERandom::nextInt(*piVar59);
      fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    }
    *(int *)(this + 0x16c) = (int)fVar66;
    if (0 < (int)fVar66) {
      iVar26 = Status::hardCoreMode();
      if (iVar26 == 0) {
        iVar26 = *(int *)(this + 0x16c);
      }
      else {
        iVar26 = AbyssEngine::AERandom::nextInt(*piVar59);
        iVar26 = iVar26 + 2;
        *(int *)(this + 0x16c) = iVar26;
      }
      fVar66 = (float)VectorSignedToFloat(iVar26,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x16c) =
           (int)(fVar66 + (*(float *)(*(int *)(DAT_000c06e0 + 0xc05a6) + 0x2c) + -0.5) * fVar66);
      iVar26 = Status::getLevel();
      *(int *)(this + 0x16c) =
           *(int *)(this + 0x16c) + ((int)(iVar26 + ((uint)(iVar26 >> 0x1f) >> 0x1e)) >> 2);
    }
    iVar26 = *(int *)(DAT_000c06e4 + 0xc05e0);
    if (((bool)(iVar24 == 3 & bVar1)) &&
       (in_fpscr = in_fpscr & 0xfffffff, *(float *)(iVar26 + 0x2c) < 1.0)) {
      iVar29 = AbyssEngine::AERandom::nextInt(*piVar59);
      *(int *)(this + 0x16c) = iVar29 + 1;
    }
    if (iVar15 == 0x4e) {
      iVar15 = 0;
      *(undefined4 *)(this + 0x164) = 0;
      *(undefined4 *)(this + 0x168) = 0;
    }
    else {
      pcVar21 = *(code **)(DAT_000c06e8 + 0xc062a);
      uVar16 = (*pcVar21)(*piVar59,2);
      *(undefined4 *)(this + 0x164) = uVar16;
      uVar16 = (*pcVar21)(*piVar59,5);
      *(undefined4 *)(this + 0x168) = uVar16;
      iVar15 = (*pcVar21)(*piVar59,2);
    }
    pLVar30 = this + 0x168;
    pLVar31 = this + 0x164;
    if (bVar6) {
      iVar24 = AbyssEngine::AERandom::nextInt(*piVar59);
    }
    *(int *)(this + 0x160) =
         iVar24 + iVar15 +
         ((int)(*(int *)(this + 0x168) + ((uint)(*(int *)(this + 0x168) >> 0x1f) >> 0x1e)) >> 2);
    if (((bVar1) && (iVar15 = Status::getCurrentCampaignMission(), 0x1f < iVar15)) &&
       (iVar15 = AbyssEngine::AERandom::nextInt(*piVar59), iVar15 < 8)) {
      *(undefined4 *)(this + 0x160) = 9;
      *(undefined4 *)(this + 0x16c) = 9;
    }
    Status::getStation();
    iVar15 = Station::stationHasPirateBase();
    fVar66 = DAT_000c06dc;
    if (iVar15 != 0) {
      fVar66 = (*(float *)(iVar26 + 0x2c) + -0.5) * 5.0 + 5.0;
    }
    iVar29 = (int)fVar66;
    iVar24 = Status::getSystem();
    if (iVar24 != 0) {
      pSVar25 = (SolarSystem *)Status::getSystem();
      iVar24 = SolarSystem::hasPirateBase(pSVar25);
      if (iVar24 != 0) {
        iVar22 = AbyssEngine::AERandom::nextInt(*piVar59);
        iVar22 = iVar22 + 2;
        iVar24 = Status::hardCoreMode();
        if (iVar24 != 0) {
          iVar24 = AbyssEngine::AERandom::nextInt(*piVar59);
          iVar22 = iVar24 + iVar22 * 2;
        }
        bVar1 = false;
        *(undefined4 *)(this + 0x16c) = 0;
      }
    }
    pSVar23 = (Station *)Status::getStation();
    iVar24 = Station::stationHasHiddenBlueprint(pSVar23,true);
    if (**(char **)(DAT_000c0a74 + 0xc074a) == '\0') {
      pSVar23 = (Station *)Status::getStation();
      iVar32 = Station::isAttackedByAliens(pSVar23);
      if (iVar32 != 0) goto LAB_000c075a;
    }
    else {
LAB_000c075a:
      iVar32 = Status::getCurrentCampaignMission();
      if (iVar32 != 0x2a) {
        iVar32 = *(int *)(this + 0x168);
        if (iVar32 < 3) {
          iVar32 = 2;
        }
        *(int *)(this + 0x168) = iVar32;
        iVar32 = AbyssEngine::AERandom::nextInt(*piVar59);
        *(int *)(this + 0x16c) = iVar32 + 2;
        if ((*(int *)(this + 0x100) != 0) &&
           (piVar46 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc), piVar46 != (int *)0x0))
        {
          uVar16 = *(undefined4 *)(this + 0x180);
          (**(code **)(*piVar46 + 0x28))(&local_f8);
          Route::setNewCoords(uVar16,local_f8,uStack_f4,uStack_f0);
        }
        bVar1 = true;
        local_1ac = 9;
      }
    }
    this_00 = (Wanted *)Status::getWantedInCurrentOrbit((Status *)*piVar56);
    if (this_00 == (Wanted *)0x0) {
      bVar6 = false;
    }
    else {
      if (2 < *(int *)(this + 0x160)) {
        *(undefined4 *)(this + 0x160) = 2;
      }
      bVar6 = true;
    }
    iVar32 = Status::getCurrentCampaignMission();
    fVar66 = DAT_000c06dc;
    if ((100 < iVar32) && (iVar32 = Status::getCurrentCampaignMission(), iVar32 < 0x91)) {
      uVar16 = Status::getCurrentCampaignMission();
      fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
      fVar66 = (fVar66 / DAT_000c0a78) * 15.0 + 5.0;
    }
    uVar58 = Status::hardCoreMode();
    iVar32 = (int)fVar66 << (uVar58 & 0xff);
    if ((iVar32 < 1) || (iVar33 = AbyssEngine::AERandom::nextInt(*piVar59), iVar32 <= iVar33)) {
      iVar32 = 0;
    }
    else {
      iVar32 = AbyssEngine::AERandom::nextInt(*piVar59);
      uVar58 = Status::hardCoreMode();
      iVar32 = iVar32 + 2 << (uVar58 & 0xff);
    }
    if (((pMVar14 == (Mission *)0x0) || (iVar33 = Mission::getType(), iVar33 != 0xd)) ||
       (*(char *)(*piVar56 + 0xf1) != '\0')) {
LAB_000c089c:
      Status::getStation();
      iVar33 = Station::hasAttackedFriends();
      iVar34 = *(int *)(this + 0x160);
      if (iVar33 != 0) {
        if (iVar34 < 8) {
          iVar34 = 7;
        }
        *(int *)(this + 0x160) = iVar34;
        *(undefined2 *)(this + 0x189) = 0x101;
      }
      bVar2 = false;
      if (*(int *)(this + 0x164) + iVar34 + *(int *)(this + 0x168) + *(int *)(this + 0x16c) + iVar22
          == 0) {
        uVar16 = 4;
        goto LAB_000c08d4;
      }
    }
    else {
      iVar33 = Mission::getTargetStation();
      pSVar23 = (Station *)Status::getStation();
      iVar34 = Station::getIndex(pSVar23);
      if (iVar33 != iVar34) goto LAB_000c089c;
      *(undefined4 *)(this + 0x164) = 0;
      *(undefined4 *)(this + 0x168) = 0;
      iVar22 = 0;
      *(undefined4 *)(this + 0x16c) = 0;
      bVar1 = false;
      uVar16 = 6;
      if (*(char *)(*piVar56 + 0xf0) == '\0') {
        uVar16 = 7;
      }
      bVar2 = *(char *)(*piVar56 + 0xf0) == '\0';
LAB_000c08d4:
      *(undefined4 *)(this + 0x160) = uVar16;
    }
    pSVar23 = (Station *)Status::getStation();
    iVar33 = Station::getIndex(pSVar23);
    if (iVar33 - 100U < 9) {
      uVar58 = 1 << (iVar33 - 100U & 0xff);
      if ((uVar58 & 0x1c) == 0) {
        if ((uVar58 & 0x103) == 0) goto LAB_000c08fe;
        goto LAB_000c0902;
      }
      *(undefined4 *)(this + 0x160) = 0;
      iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
      iVar33 = iVar33 + 3;
      bVar1 = true;
      local_1ac = 8;
LAB_000c0928:
      *(undefined4 *)(this + 0x164) = 0;
      *(undefined4 *)(this + 0x168) = 0;
      *(int *)(this + 0x16c) = iVar33;
    }
    else {
LAB_000c08fe:
      if (iVar33 == 10) {
LAB_000c0902:
        iVar33 = 0;
        *(undefined4 *)(this + 0x160) = 0;
        bVar1 = false;
        goto LAB_000c0928;
      }
    }
    iVar33 = Status::getCurrentCampaignMission();
    if (((iVar33 == 0x24) || (iVar33 = Status::getCurrentCampaignMission(), iVar33 == 0x25)) &&
       (iVar33 = Status::getSystem(), iVar33 != 0)) {
      Status::getSystem();
      iVar33 = SolarSystem::getIndex();
      if (iVar33 == 5) {
        *(undefined4 *)(this + 0x16c) = 0;
        bVar1 = false;
        *(undefined4 *)(this + 0x160) = 0;
      }
    }
    iVar33 = Status::getCurrentCampaignMission();
    if (((0x29 < iVar33) && (iVar33 = Status::getCurrentCampaignMission(), iVar33 < 0x2c)) &&
       (iVar33 = Status::inAlienOrbit(), iVar33 == 0)) {
      iVar22 = 0;
      bVar1 = false;
      *(undefined4 *)(this + 0x16c) = 0;
    }
    iVar33 = Status::inBlackMarketSystem();
    if (iVar33 != 0) {
      iVar22 = 0;
      *(undefined4 *)(this + 0x160) = 0;
      iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
      *(int *)(this + 0x16c) = iVar33 + 6;
      iVar33 = Status::hardCoreMode();
      if (iVar33 != 0) {
        iVar34 = *(int *)(this + 0x16c);
        iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
        *(int *)(this + 0x16c) = iVar33 + iVar34 * 2;
      }
      *(undefined4 *)pLVar30 = 0;
      *(undefined4 *)pLVar31 = 0;
      bVar1 = true;
      local_1ac = 8;
    }
    iVar33 = Status::inPirateLootOrbit();
    if (iVar33 != 0) {
      iVar22 = 0;
      *(undefined4 *)(this + 0x160) = 0;
      iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
      *(int *)(this + 0x16c) = iVar33 + 10;
      iVar33 = Status::hardCoreMode();
      if (iVar33 != 0) {
        iVar34 = *(int *)(this + 0x16c);
        iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
        *(int *)(this + 0x16c) = iVar33 + iVar34 * 2;
      }
      *(undefined4 *)pLVar30 = 0;
      *(undefined4 *)pLVar31 = 0;
      bVar1 = true;
      local_1ac = 8;
    }
    pSVar23 = (Station *)Status::getStation();
    iVar33 = Station::getIndex(pSVar23);
    if (iVar33 == 0x6c) {
      if (*(int *)(*piVar56 + 0x114) == 0) {
        *(undefined4 *)(this + 0x160) = 0;
        this[0x288] = (Level)0x1;
        iVar15 = Status::hardCoreMode();
        uVar16 = 6;
        if (iVar15 != 0) {
          uVar16 = 8;
        }
        *(undefined4 *)(this + 0x164) = 0;
        *(undefined4 *)(this + 0x168) = 0;
        *(undefined4 *)(this + 0x16c) = uVar16;
        pAVar19 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar19);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
        ArraySetLength<KIPlayer*>(*(int *)(this + 0x16c) + 4,(Array *)pAVar19);
        piVar59 = *(int **)(DAT_000c1124 + 0xc0f82);
        for (iVar15 = 0; uVar17 = DAT_000c112c, uVar16 = DAT_000c1128, iVar15 != 4;
            iVar15 = iVar15 + 1) {
          uVar16 = createStaticObject(this,(Waypoint *)0x0,0x37a3,true);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar16;
          pSVar36 = (String *)GameText::getText(*piVar59);
          AbyssEngine::String::operator=
                    ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) + 0x18),
                     pSVar36);
        }
        (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
                  ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c112c,DAT_000c1130,
                   DAT_000c1128);
        piVar46 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
        (**(code **)(*piVar46 + 0x48))(piVar46,DAT_000c1134,DAT_000c1138,DAT_000c113c);
        piVar46 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
        (**(code **)(*piVar46 + 0x48))(piVar46,DAT_000c1140,uVar17,uVar16);
        piVar46 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc);
        (**(code **)(*piVar46 + 0x48))(piVar46,DAT_000c1144,DAT_000c1148,DAT_000c114c);
        puVar20 = *(undefined4 **)(DAT_000c1150 + 0xc1024);
        for (uVar58 = 4; uVar58 < **(uint **)(this + 0xf8); uVar58 = uVar58 + 1) {
          iVar15 = Globals::getRandomEnemyFighter((Globals *)*puVar20,8);
          uVar16 = createShip(this,8,0,iVar15,(Waypoint *)0x0,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
        }
        pMVar14 = operator_new(0x78);
        pSVar36 = (String *)GameText::getText(*piVar59);
        AbyssEngine::String::String(aSStack_104,pSVar36,false);
        Mission::Mission(pMVar14,4,aSStack_104,DAT_000c1154 + 0xc107c,4,0,0x6c,0);
        AbyssEngine::String::~String(aSStack_104);
        Status::setFreelanceMission((Mission *)*piVar56);
        Status::setMission((Mission *)*piVar56);
        pOVar38 = operator_new(0x1c);
        Objective::Objective(pOVar38,0x12,0,**(int **)(this + 0xf8),this);
        goto LAB_000c16cc;
      }
      bVar1 = false;
      *(undefined4 *)(this + 0x16c) = 0;
    }
    iVar33 = Status::getCampaignMission();
    if (iVar33 != 0) {
      Status::getCampaignMission();
      iVar33 = Mission::getType();
      if ((iVar33 == 0xa3) && (puVar47 = *(uint **)(*piVar56 + 0x90), puVar47 != (uint *)0x0)) {
        for (uVar58 = 0; uVar58 < *puVar47; uVar58 = uVar58 + 1) {
          iVar34 = *(int *)(puVar47[1] + uVar58 * 4);
          pSVar23 = (Station *)Status::getStation();
          iVar33 = Station::getIndex(pSVar23);
          if (iVar34 == iVar33) {
            bVar3 = true;
            goto LAB_000c0aa4;
          }
          puVar47 = *(uint **)(*piVar56 + 0x90);
        }
      }
    }
    bVar3 = false;
LAB_000c0aa4:
    pcVar21 = *(code **)(DAT_000c0e68 + 0xc0ab2);
    iVar33 = (*pcVar21)(*piVar59,20000);
    local_70 = CONCAT44(local_70._4_4_,iVar33 + -10000);
    iVar33 = (*pcVar21)(*piVar59,20000);
    local_70 = CONCAT44(iVar33 + -10000,(int)local_70);
    local_68 = (*pcVar21)(*piVar59,30000);
    local_68 = local_68 + 20000;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_70,3);
    if (bVar3) {
      *(undefined4 *)(this + 0x16c) = 0;
      *(undefined4 *)(this + 0x164) = 0;
      pAVar19 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar19);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
      ArraySetLength<KIPlayer*>(*(int *)(this + 0x160) + 8,(Array *)pAVar19);
      iVar15 = (*pcVar21)(*piVar59,30000);
      iVar15 = iVar15 + DAT_000c0e6c;
      iVar22 = (*pcVar21)(*piVar59,2);
      iVar24 = -1;
      if (iVar22 == 0) {
        iVar24 = 1;
      }
      local_7c = (float)(iVar15 * iVar24);
      iVar15 = (*pcVar21)(*piVar59,3000);
      local_78 = (float)(iVar15 + -6000);
      iVar15 = (*pcVar21)(*piVar59,50000);
      local_74 = (float)(iVar15 + DAT_000c0e70);
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_7c,3);
      *(Route **)(this + 0x108) = pRVar27;
      iVar15 = Route::clone((__fn *)pRVar27,__child_stack,__flags,__arg);
      *(int *)(this + 0x110) = iVar15;
      Route::setLoop(SUB41(iVar15,0));
      pWVar35 = (Waypoint *)Route::getWaypoint();
      iVar15 = 0xf;
      if (iVar18 == 1) {
        iVar15 = 0xd;
      }
      uVar16 = createShip(this,iVar18,1,iVar15,pWVar35,true,false);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar16;
      PlayerFixedObject::setMoving
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      uVar16 = VectorSignedToFloat(local_7c,(byte)(in_fpscr >> 0x16) & 3);
      uVar17 = VectorSignedToFloat(local_78,(byte)(in_fpscr >> 0x16) & 3);
      uVar69 = VectorSignedToFloat(local_74,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),uVar16,uVar17,uVar69);
      if (*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50) != 0) {
        iVar15 = AbyssEngine::AERandom::nextInt(*piVar59);
        if (iVar15 - 3U < 3) {
          iVar15 = *(int *)(DAT_000c0e74 + 0xc0c20 + (iVar15 - 3U) * 4);
        }
        piVar56 = *(int **)(DAT_000c0e78 + 0xc0c32);
        **(int **)(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50) + 4) = iVar15;
        pSVar36 = (String *)GameText::getText(*piVar56);
        AbyssEngine::String::operator=
                  ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar36);
      }
      puVar20 = *(undefined4 **)(DAT_000c0e7c + 0xc0c58);
      for (iVar15 = 1; uStack_88 = DAT_000c0e88, uStack_90 = DAT_000c0e80, iVar15 != 6;
          iVar15 = iVar15 + 1) {
        iVar22 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar18);
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,iVar18,0,iVar22,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar16;
        pKVar52 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar15 * 4);
        pRVar27 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                        *(void **)(*(int *)(this + 0xf8) + 4),__flags_00,__arg_00);
        KIPlayer::setRoute(pKVar52,pRVar27);
      }
      local_98 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar37 = (undefined4 *)((uint)&local_b8 | 4);
      *puVar37 = 0;
      puVar37[1] = *(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      puVar37[2] = local_98;
      puVar37[3] = uStack_94;
      local_a0 = (ulonglong)*(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4) << 0x20;
      local_b8 = 1.0;
      iStack_a4 = 0x3f800000;
      local_80 = 0x3f800000;
      local_110 = 0.0;
      local_10c = 0.0;
      local_108 = 0.0;
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x28))(&local_11c);
      iVar24 = 0x18;
      iVar15 = DAT_000c0e90 + 0xc0cfe;
      for (iVar22 = 0; iVar22 != 0x38; iVar22 = iVar22 + 0x1c) {
        uVar16 = createStaticObject(this,(Waypoint *)0x0,0x381b,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24) = uVar16;
        pPVar53 = *(PlayerTurret **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24);
        Player::setMaxHitpoints(*(Player **)(pPVar53 + 4),1000);
        iVar26 = iVar15 + iVar18 * 0x38 + iVar22;
        uVar16 = *(undefined4 *)(iVar26 + 4);
        uVar17 = *(undefined4 *)(iVar26 + 8);
        uVar69 = *(undefined4 *)(iVar26 + 0xc);
        local_134 = *(undefined4 *)(iVar26 + 0x10);
        uStack_130 = *(undefined4 *)(iVar26 + 0x14);
        local_12c = *(undefined4 *)(iVar26 + 0x18);
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_128,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_110,(Vector *)&local_128);
        AEGeometry::setRotation(*(AEGeometry **)(pPVar53 + 8),(Vector *)&local_110);
        local_134 = uVar16;
        uStack_130 = uVar17;
        local_12c = uVar69;
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_128,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_110,(Vector *)&local_128);
        local_128 = local_11c + local_110;
        local_124 = local_118 + local_10c;
        local_120 = local_114 + local_108;
        fVar66 = (float)(**(code **)(*(int *)pPVar53 + 0x44))(pPVar53,(Matrix *)&local_128);
        PlayerTurret::setScaling(pPVar53,fVar66);
        iVar24 = iVar24 + 4;
        *(int *)(pPVar53 + 0x28) = iVar18;
      }
      for (iVar15 = 8; iVar15 < *(int *)(this + 0x160) + 8; iVar15 = iVar15 + 1) {
        iVar22 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar18);
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,iVar18,0,iVar22,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar16;
      }
      goto switchD_000c02a8_caseD_8;
    }
    iVar33 = Status::getCampaignMission();
    if (iVar33 == 0) {
LAB_000c0e98:
      uVar58 = 0;
    }
    else {
      Status::getCampaignMission();
      iVar33 = Mission::getType();
      if (iVar33 != 0xa7) goto LAB_000c0e98;
      pSVar23 = (Station *)Status::getStation();
      iVar33 = Station::getIndex(pSVar23);
      Status::getCampaignMission();
      iVar34 = Mission::getTargetStation();
      uVar58 = (uint)(iVar33 == iVar34);
    }
    if ((iVar18 == 0) && (0 < *(int *)pLVar30)) {
      iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
      if (iVar33 < 0x1e) {
        iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
        bVar3 = false;
        if (iVar33 < 0x1e) {
          iVar33 = Status::getCurrentCampaignMission();
          bVar4 = 0x67 < iVar33;
          bVar7 = true;
          goto LAB_000c29ee;
        }
        bVar7 = true;
      }
      else {
        bVar3 = false;
LAB_000c14a2:
        bVar7 = false;
      }
LAB_000c29e8:
      bVar4 = false;
    }
    else {
      bVar3 = false;
      if (iVar18 != 1) {
        bVar7 = false;
        goto LAB_000c29e8;
      }
      bVar7 = false;
      bVar4 = false;
      if (0 < *(int *)pLVar30) {
        iVar33 = AbyssEngine::AERandom::nextInt(*piVar59);
        bVar7 = false;
        bVar3 = false;
        bVar4 = false;
        if (iVar33 < 0x1e) {
          iVar33 = Status::getCurrentCampaignMission();
          bVar3 = 0x8c < iVar33;
          goto LAB_000c14a2;
        }
      }
    }
LAB_000c29ee:
    iVar33 = Status::inSupernovaSystem();
    if (iVar33 != 0) {
      *(undefined4 *)(this + 0x160) = 0;
      *(undefined4 *)(this + 0x164) = 0;
      bVar7 = false;
      *(undefined4 *)(this + 0x168) = 0;
      *(undefined4 *)(this + 0x16c) = 0;
      bVar1 = false;
    }
    if (bVar6) {
      iVar33 = Wanted::getNumWingmen(this_00);
      *(int *)(this + 0x160) = iVar33 + *(int *)(this + 0x160) + 1;
    }
    iVar33 = 7;
    if (bVar4) {
      iVar33 = 8;
    }
    if (!bVar7) {
      iVar33 = 0;
    }
    iVar34 = 0;
    if (bVar3) {
      iVar34 = 5;
    }
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>
              (iVar29 + iVar15 + iVar24 + iVar32 + iVar22 + uVar58 + iVar34 + iVar33 +
               *(int *)(this + 0x160) + *(int *)(this + 0x164) + *(int *)(this + 0x168) +
               *(int *)(this + 0x16c),(Array *)pAVar19);
    for (iVar49 = 0; iVar43 = *(int *)(this + 0x160), iVar49 < iVar43; iVar49 = iVar49 + 1) {
      if (iVar49 == 0 && bVar6) {
        uVar42 = Wanted::getRace(this_00);
        if (uVar42 < 4) {
          iVar43 = Wanted::getRace(this_00);
        }
        else {
          iVar43 = 8;
        }
        iVar64 = Wanted::getShip(this_00);
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,iVar43,0,iVar64,pWVar35,true,false);
        **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar16;
        iVar64 = Wanted::getHitpoints(this_00);
        iVar62 = Status::getLevel();
        if (iVar62 < 0x15) {
          iVar62 = Status::getLevel();
        }
        else {
          iVar62 = 0x14;
        }
        iVar50 = Status::gameWon();
        if (iVar50 == 0) {
          iVar50 = Status::getCurrentCampaignMission();
        }
        else {
          iVar50 = 0x2d;
        }
        fVar66 = (float)VectorSignedToFloat(iVar62 * 0xf + iVar64 + iVar50 * 4,
                                            (byte)(in_fpscr >> 0x16) & 3);
        iVar64 = (int)(fVar66 + (*(float *)(iVar26 + 0x2c) + -0.5) * fVar66);
        Player::setMaxHitpoints(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),iVar64);
        Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
        Wanted::getName();
        AbyssEngine::String::operator=
                  ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),(String *)&local_b8);
        AbyssEngine::String::~String((String *)&local_b8);
        puVar20 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
        if ((Array<int> *)*puVar20 != (Array<int> *)0x0) {
          pvVar44 = (void *)Array<int>::~Array((Array<int> *)*puVar20);
          operator_delete(pvVar44);
          puVar20 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
        }
        *puVar20 = 0;
        pvVar44 = operator_new(0xc);
        Array<int>::Array();
        *(void **)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50) = pvVar44;
        iVar62 = Wanted::getLoot(this_00);
        ArrayAdd<int>(iVar62,*(Array **)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50));
        iVar62 = Wanted::getLootAmount(this_00);
        ArrayAdd<int>(iVar62,*(Array **)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50));
        *(undefined1 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x42) = 1;
        iVar62 = Wanted::getIndex(this_00);
        piVar56 = (int *)**(int **)(*(int *)(this + 0xf8) + 4);
        piVar56[0x12] = iVar62;
        (**(code **)(*piVar56 + 0x1c))(piVar56,DAT_000c2de4);
        for (iVar62 = 1; iVar50 = Wanted::getNumWingmen(this_00), iVar62 <= iVar50;
            iVar62 = iVar62 + 1) {
          iVar50 = Globals::getRandomEnemyFighter
                             ((Globals *)**(undefined4 **)(DAT_000c2de8 + 0xc2c9c),iVar43);
          pWVar35 = (Waypoint *)Route::getWaypoint();
          uVar16 = createShip(this,iVar43,0,iVar50,pWVar35,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) = uVar16;
          Player::setMaxHitpoints
                    (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) + 4),
                     iVar64 / 2);
          Player::setHitpoints
                    (*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) + 4));
        }
      }
      else if ((!bVar6) || (iVar43 = Wanted::getNumWingmen(this_00), iVar43 < iVar49)) {
        iVar43 = Globals::getRandomEnemyFighter
                           ((Globals *)**(undefined4 **)(DAT_000c2de0 + 0xc2b08),iVar18);
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,iVar18,0,iVar43,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar49 * 4) = uVar16;
      }
    }
    puVar20 = *(undefined4 **)(DAT_000c2dec + 0xc2d18);
    iVar26 = iVar43;
    while( true ) {
      iVar49 = *(int *)pLVar31;
      iVar64 = iVar49 + iVar43;
      if (iVar64 <= iVar26) break;
      iVar49 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar18);
      uVar16 = createShip(this,iVar18,0,iVar49,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4) = uVar16;
      KIPlayer::setDead();
      iVar49 = (*pcVar21)(*piVar59,DAT_000c2df0);
      local_b8 = (float)(iVar49 + DAT_000c2df4);
      iVar49 = (*pcVar21)(*piVar59,DAT_000c2df8);
      local_b4 = CONCAT44(local_b4._4_4_,iVar49 + DAT_000c2dfc);
      iVar49 = (*pcVar21)(*piVar59,DAT_000c2e00);
      local_b4 = CONCAT44(iVar49 + 50000,(float)local_b4);
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_b8,3);
      KIPlayer::setRoute(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4),pRVar27);
      KIPlayer::setJumper(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4),true);
      iVar43 = *(int *)(this + 0x160);
      iVar26 = iVar26 + 1;
    }
    iVar62 = iVar64 * 4;
    iVar26 = 0;
    if (iVar18 != 2) {
      iVar26 = iVar18;
    }
    while( true ) {
      uVar12 = DAT_000c35a0;
      uVar11 = DAT_000c3598;
      uVar16 = DAT_000c3178;
      iVar51 = iVar49 + iVar43;
      iVar50 = *(int *)pLVar30;
      if (iVar51 + iVar50 <= iVar64) break;
      iVar49 = iVar64 - iVar51;
      if (iVar49 == 0 && bVar7) {
        if (bVar4) {
          iVar49 = (*pcVar21)(*piVar59,DAT_000c3178);
          fVar67 = (float)VectorSignedToFloat(iVar49 + -40000,(byte)(in_fpscr >> 0x16) & 3);
          iVar49 = (*pcVar21)(*piVar59,40000);
          fVar68 = (float)VectorSignedToFloat(iVar49 + -20000,(byte)(in_fpscr >> 0x16) & 3);
          iVar49 = (*pcVar21)(*piVar59,uVar16);
          fVar66 = (float)VectorSignedToFloat(iVar49 + DAT_000c317c,(byte)(in_fpscr >> 0x16) & 3);
          local_b4 = CONCAT44(fVar66,fVar68);
          local_b8 = fVar67;
          pWVar35 = operator_new(0x138);
          Waypoint::Waypoint(pWVar35,(int)fVar67,(int)fVar68,(int)fVar66,(Route *)0x0);
          uVar16 = createStaticObject(this,pWVar35,0x4974,true);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) = uVar16;
          KIPlayer::setSpacePoints
                    (*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62),(Array *)0x0);
          piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
          (**(code **)(*piVar56 + 0x44))(piVar56,&local_b8);
          PlayerFixedObject::setMoving
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62),false);
          Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) + 4))
          ;
          piVar56 = (int *)(*(int *)(this + 0xf8) + 4);
          puVar37 = (undefined4 *)(*(int *)(*piVar56 + iVar62) + 0x50);
          if ((Array<int> *)*puVar37 == (Array<int> *)0x0) {
            piVar46 = (int *)(*piVar56 + iVar64 * 4);
          }
          else {
            pvVar44 = (void *)Array<int>::~Array((Array<int> *)*puVar37);
            operator_delete(pvVar44);
            piVar56 = (int *)(*(int *)(this + 0xf8) + 4);
            piVar46 = (int *)(*piVar56 + iVar62);
            puVar37 = (undefined4 *)(*(int *)(*piVar56 + iVar62) + 0x50);
          }
          *puVar37 = 0;
          *(undefined1 *)(*piVar46 + 0x70) = 0;
        }
        else {
          uVar16 = createShip(this,iVar18,1,0xe,(Waypoint *)0x0,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) = uVar16;
          piVar56 = (int *)(*(int *)(this + 0xf8) + 4);
        }
        PlayerFixedObject::setMoving(*(PlayerFixedObject **)(*piVar56 + iVar62),false);
        uVar16 = DAT_000c3590;
        piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
        iVar49 = (*pcVar21)(*piVar59);
        uVar17 = VectorSignedToFloat(iVar49 + -40000,(byte)(in_fpscr >> 0x16) & 3);
        iVar49 = (*pcVar21)(*piVar59,10000);
        uVar69 = VectorSignedToFloat(iVar49 + -5000,(byte)(in_fpscr >> 0x16) & 3);
        iVar49 = (*pcVar21)(*piVar59,uVar16);
        uVar16 = VectorSignedToFloat(iVar49 + 40000,(byte)(in_fpscr >> 0x16) & 3);
        (**(code **)(*piVar56 + 0x48))(piVar56,uVar17,uVar69,uVar16);
      }
      else {
        if (iVar64 != iVar51) {
          iVar49 = 1;
        }
        if (iVar49 == 0 && bVar3) {
          iVar49 = (*pcVar21)(*piVar59,DAT_000c3178);
          fVar67 = (float)VectorSignedToFloat(iVar49 + -40000,(byte)(in_fpscr >> 0x16) & 3);
          iVar49 = (*pcVar21)(*piVar59,40000);
          fVar68 = (float)VectorSignedToFloat(iVar49 + -20000,(byte)(in_fpscr >> 0x16) & 3);
          iVar49 = (*pcVar21)(*piVar59,uVar16);
          fVar66 = (float)VectorSignedToFloat(iVar49 + 60000,(byte)(in_fpscr >> 0x16) & 3);
          local_b4 = CONCAT44(fVar66,fVar68);
          local_b8 = fVar67;
          pWVar35 = operator_new(0x138);
          Waypoint::Waypoint(pWVar35,(int)fVar67,(int)fVar68,(int)fVar66,(Route *)0x0);
          uVar16 = createStaticObject(this,pWVar35,0x4a6b,true);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) = uVar16;
          KIPlayer::setSpacePoints
                    (*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62),(Array *)0x0);
          piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
          (**(code **)(*piVar56 + 0x44))(piVar56,&local_b8);
          PlayerFixedObject::setMoving
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62),false);
          Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) + 4))
          ;
          uVar16 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
          pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c3184 + 0xc301e));
          AbyssEngine::String::String(aSStack_140,pSVar36,false);
          PlayerFixedObject::setName(uVar16,aSStack_140);
          AbyssEngine::String::~String(aSStack_140);
          iVar49 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
          puVar37 = (undefined4 *)(iVar49 + 0x50);
          *(undefined1 *)(iVar49 + 0x70) = 0;
          if ((Array<int> *)*puVar37 != (Array<int> *)0x0) {
            pvVar44 = (void *)Array<int>::~Array((Array<int> *)*puVar37);
            operator_delete(pvVar44);
            puVar37 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) + 0x50);
          }
          *puVar37 = 0;
        }
        else {
          iVar50 = (*pcVar21)(*piVar59,100);
          iVar51 = 0xf;
          iVar43 = iVar26;
          iVar49 = 2;
          if (0x1d < iVar50) {
            iVar43 = iVar18;
            iVar49 = iVar18;
          }
          if (iVar18 == 0) {
            iVar43 = iVar49;
          }
          if (iVar43 == 1) {
            iVar51 = 0xd;
          }
          uVar16 = createShip(this,iVar43,1,iVar51,(Waypoint *)0x0,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62) = uVar16;
          PlayerFixedObject::setMoving
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62),true);
          piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62);
          iVar49 = (*pcVar21)(*piVar59,60000);
          iVar49 = iVar49 + DAT_000c3188;
          iVar43 = (*pcVar21)(*piVar59,2);
          iVar50 = -1;
          if (iVar43 == 0) {
            iVar50 = 1;
          }
          iVar43 = (*pcVar21)(*piVar59,40000);
          uVar17 = VectorSignedToFloat(iVar50 * iVar49,(byte)(in_fpscr >> 0x16) & 3);
          uVar69 = VectorSignedToFloat(iVar43 + -20000,(byte)(in_fpscr >> 0x16) & 3);
          iVar49 = (*pcVar21)(*piVar59,DAT_000c318c);
          uVar16 = VectorSignedToFloat(iVar49 + DAT_000c3188,(byte)(in_fpscr >> 0x16) & 3);
          (**(code **)(*piVar56 + 0x48))(piVar56,uVar17,uVar69,uVar16);
        }
      }
      iVar43 = *(int *)(this + 0x160);
      iVar49 = *(int *)(this + 0x164);
      iVar62 = iVar62 + 4;
      iVar64 = iVar64 + 1;
    }
    if (iVar33 != 0) {
      local_98 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar37 = (undefined4 *)((uint)&local_b8 | 4);
      *puVar37 = 0;
      puVar37[1] = *(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      puVar37[2] = local_98;
      puVar37[3] = uStack_94;
      local_a0 = (ulonglong)*(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4) << 0x20;
      uStack_90 = uVar11;
      uStack_88 = uVar12;
      local_b8 = 1.0;
      iStack_a4 = 0x3f800000;
      local_80 = 0x3f800000;
      local_7c = 0.0;
      local_78 = 0.0;
      local_74 = 0.0;
      (**(code **)(**(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar51 * 4) + 0x28))(&local_110)
      ;
      iVar26 = 0;
      iVar43 = *(int *)(this + 0x160);
      iVar49 = *(int *)(this + 0x164);
      iVar50 = *(int *)(this + 0x168);
      for (iVar64 = iVar49 + iVar43 + iVar50; iVar64 < iVar43 + iVar33 + iVar49 + iVar50;
          iVar64 = iVar64 + 1) {
        uVar16 = createStaticObject(this,(Waypoint *)0x0,0x1a74,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar64 * 4) = uVar16;
        pPVar53 = *(PlayerTurret **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar64 * 4);
        Player::setMaxHitpoints(*(Player **)(pPVar53 + 4),1000);
        if (bVar4) {
          iVar49 = DAT_000c35a8 + 0xc32d0;
        }
        else {
          iVar49 = DAT_000c35ac + 0xc32d6;
        }
        iVar43 = iVar49 + iVar26;
        fVar66 = *(float *)(iVar43 + 8);
        local_124 = *(float *)(iVar43 + 0x10);
        local_128 = *(float *)(iVar43 + 0xc);
        fVar67 = *(float *)(iVar43 + 4);
        local_120 = *(float *)(iVar43 + 0x14);
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_11c,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_7c,(Vector *)&local_11c);
        AEGeometry::setRotation(*(AEGeometry **)(pPVar53 + 8),(Vector *)&local_7c);
        local_128 = *(float *)(iVar49 + iVar26);
        local_124 = fVar67;
        local_120 = fVar66;
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_11c,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_7c,(Vector *)&local_11c);
        local_11c = local_110 + local_7c;
        local_118 = local_10c + local_78;
        local_114 = local_108 + local_74;
        fVar66 = (float)(**(code **)(*(int *)pPVar53 + 0x44))(pPVar53,(Matrix *)&local_11c);
        PlayerTurret::setScaling(pPVar53,fVar66);
        *(undefined4 *)(pPVar53 + 0x50) = 0;
        iVar26 = iVar26 + 0x18;
        *(int *)(pPVar53 + 0x28) = iVar18;
        iVar43 = *(int *)(this + 0x160);
        iVar49 = *(int *)(this + 0x164);
        iVar50 = *(int *)(this + 0x168);
      }
    }
    uVar12 = DAT_000c35a0;
    uVar11 = DAT_000c3598;
    if (bVar3) {
      local_98 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar37 = (undefined4 *)((uint)&local_b8 | 4);
      *puVar37 = 0;
      puVar37[1] = *(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      puVar37[2] = local_98;
      puVar37[3] = uStack_94;
      local_a0 = (ulonglong)*(uint *)((undefined1  [16])0x0 + (undefined1  [16])0x4) << 0x20;
      uStack_90 = uVar11;
      uStack_88 = uVar12;
      local_b8 = 1.0;
      iStack_a4 = 0x3f800000;
      local_80 = 0x3f800000;
      local_7c = 0.0;
      local_78 = 0.0;
      local_74 = 0.0;
      (**(code **)(**(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + (iVar43 + iVar49) * 4) + 0x28))
                (&local_110);
      iVar43 = *(int *)(this + 0x160);
      iVar49 = *(int *)(this + 0x164);
      iVar50 = *(int *)(this + 0x168);
      pfVar60 = (float *)(DAT_000c35b4 + 0xc33fc);
      for (iVar18 = iVar49 + iVar43 + iVar50; iVar18 < iVar34 + iVar43 + iVar49 + iVar50;
          iVar18 = iVar18 + 1) {
        uVar16 = createStaticObject(this,(Waypoint *)0x0,0x1a76,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 * 4) = uVar16;
        pPVar53 = *(PlayerTurret **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 * 4);
        Player::setMaxHitpoints(*(Player **)(pPVar53 + 4),1000);
        local_128 = pfVar60[3];
        local_124 = pfVar60[4];
        local_120 = pfVar60[5];
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_11c,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_7c,(Vector *)&local_11c);
        AEGeometry::setRotation(*(AEGeometry **)(pPVar53 + 8),(Vector *)&local_7c);
        local_128 = *pfVar60;
        local_124 = pfVar60[1];
        local_120 = pfVar60[2];
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_11c,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_7c,(Vector *)&local_11c);
        local_11c = local_110 + local_7c;
        local_118 = local_10c + local_78;
        local_114 = local_108 + local_74;
        fVar66 = (float)(**(code **)(*(int *)pPVar53 + 0x44))(pPVar53,(Matrix *)&local_11c);
        PlayerTurret::setScaling(pPVar53,fVar66);
        *(undefined4 *)(pPVar53 + 0x50) = 0;
        *(undefined4 *)(pPVar53 + 0x28) = 1;
        pfVar60 = pfVar60 + 6;
        iVar43 = *(int *)(this + 0x160);
        iVar49 = *(int *)(this + 0x164);
        iVar50 = *(int *)(this + 0x168);
      }
    }
    if (bVar1) {
      iVar64 = Globals::getRandomEnemyFighter((Globals *)*puVar20,local_1ac);
      iVar18 = Status::hardCoreMode();
      if ((local_1ac == 8) && (**(char **)(DAT_000c3ad0 + 0xc3784) != '\0')) {
        iVar26 = AbyssEngine::AERandom::nextInt(*piVar59);
        iVar49 = 0x14;
        if (iVar18 != 0) {
          iVar49 = 0x28;
        }
        iVar18 = Status::getLevel();
        if (iVar26 < iVar18 + iVar49) {
          fVar66 = (float)PlayerEgo::getPosition();
          AbyssEngine::AEMath::operator/((Vector *)&local_b8,fVar66);
          AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_b8);
          Route::setNewCoords(*(undefined4 *)(this + 0x180),*(undefined4 *)(this + 0x18c),
                              *(undefined4 *)(this + 400),*(undefined4 *)(this + 0x194));
        }
      }
      iVar43 = *(int *)(this + 0x160);
      iVar49 = *(int *)(this + 0x164);
      iVar50 = *(int *)(this + 0x168);
      iVar62 = iVar33 + iVar34 + iVar43 + iVar49 + iVar50;
      while( true ) {
        iVar18 = *(int *)(this + 0x16c);
        iVar26 = iVar33 + iVar34 + iVar43 + iVar49 + iVar50 + iVar18;
        if (iVar26 <= iVar62) break;
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,local_1ac,0,iVar64,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) = uVar16;
        iVar18 = Status::inPirateLootOrbit();
        if (iVar18 != 0) {
          iVar18 = *(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) + 4);
          Player::getMaxHitpoints();
          Player::setHitpoints(iVar18);
          piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4);
          (**(code **)(*piVar56 + 0x1c))(piVar56,DAT_000c3ad8);
          PlayerFighter::setRotate
                    (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4),3);
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_b8);
          iVar18 = (*pcVar21)(*piVar59,50000);
          iVar26 = (*pcVar21)(*piVar59,2);
          iVar49 = -1;
          if (iVar26 == 0) {
            iVar49 = 1;
          }
          fVar66 = (float)VectorSignedToFloat(iVar49 * iVar18 + 20000,(byte)(in_fpscr >> 0x16) & 3);
          *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar66;
          iVar18 = (*pcVar21)(*piVar59,50000);
          iVar26 = (*pcVar21)(*piVar59,2);
          iVar49 = -1;
          if (iVar26 == 0) {
            iVar49 = 1;
          }
          fVar66 = (float)VectorSignedToFloat(iVar49 * iVar18 + 10000,(byte)(in_fpscr >> 0x16) & 3);
          *(float *)(this + 400) = *(float *)(this + 400) + fVar66;
          iVar18 = (*pcVar21)(*piVar59,50000);
          iVar26 = (*pcVar21)(*piVar59,2);
          iVar49 = -1;
          if (iVar26 == 0) {
            iVar49 = 1;
          }
          fVar66 = (float)VectorSignedToFloat(iVar49 * iVar18 + 20000,(byte)(in_fpscr >> 0x16) & 3);
          *(float *)(this + 0x194) = *(float *)(this + 0x194) + fVar66;
          piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4);
          (**(code **)(*piVar56 + 0x44))(piVar56,(Vector *)(this + 0x18c));
        }
        iVar62 = iVar62 + 1;
        iVar43 = *(int *)(this + 0x160);
        iVar49 = *(int *)(this + 0x164);
        iVar50 = *(int *)(this + 0x168);
      }
    }
    else {
      iVar18 = *(int *)(this + 0x16c);
      iVar26 = iVar33 + iVar34 + iVar43 + iVar49 + iVar50 + iVar18;
    }
    fVar67 = DAT_000c3ae0;
    fVar66 = DAT_000c3adc;
    local_17c = (Vector *)(this + 0x18c);
    iVar64 = iVar33 + iVar34;
    for (; iVar62 = DAT_000c3aec, iVar51 = iVar64 + iVar22 + iVar43 + iVar49 + iVar50 + iVar18,
        iVar26 < iVar51; iVar26 = iVar26 + 1) {
      iVar18 = Globals::getRandomEnemyFighter((Globals *)*puVar20,8);
      uVar16 = createShip(this,8,0,iVar18,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4) = uVar16;
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=(local_17c,(Vector *)&local_b8);
      uVar16 = DAT_000c3ae4;
      fVar70 = *(float *)(this + 0x18c);
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4);
      uVar17 = (*pcVar21)(*piVar59);
      fVar71 = *(float *)(this + 400);
      uVar69 = (*pcVar21)(*piVar59,DAT_000c3ae8);
      fVar72 = *(float *)(this + 0x194);
      fVar65 = (float)VectorSignedToFloat(uVar17,(byte)(in_fpscr >> 0x16) & 3);
      fVar73 = (float)VectorSignedToFloat(uVar69,(byte)(in_fpscr >> 0x16) & 3);
      uVar16 = (*pcVar21)(*piVar59,uVar16);
      fVar68 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar56 + 0x48))
                (piVar56,fVar70 + fVar66 + fVar65,fVar71 + fVar67 + fVar73,fVar72 + fVar66 + fVar68)
      ;
      iVar43 = *(int *)(this + 0x160);
      iVar49 = *(int *)(this + 0x164);
      iVar50 = *(int *)(this + 0x168);
      iVar18 = *(int *)(this + 0x16c);
    }
    if (iVar15 != 0) {
      local_b8 = 0.0;
      local_b4 = 0;
      for (; iVar26 = iVar64 + iVar15 + iVar22 + iVar43 + iVar49 + iVar50 + iVar18, iVar51 < iVar26;
          iVar51 = iVar51 + 1) {
        pSVar23 = (Station *)Status::getStation();
        iVar18 = Station::getPirateStationIndex(pSVar23);
        local_7c = *(float *)(&UNK_000c3a96 + iVar18 * 0xc + iVar62);
        iVar18 = iVar18 * 0xc + iVar62;
        local_78 = *(float *)(&UNK_000c3a96 + iVar18 + 4);
        local_74 = *(float *)(&UNK_000c3a96 + iVar18 + 8);
        pRVar27 = operator_new(0x18);
        Route::Route(pRVar27,(int *)&local_7c,3);
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createStaticObject(this,pWVar35,0x37a3,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar51 * 4) = uVar16;
        piVar56 = (int *)Route::getWaypoint();
        (**(code **)(*piVar56 + 0x28))((Vector *)&local_110,piVar56);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_b8,(Vector *)&local_110);
        KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar51 * 4));
        pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c3dfc + 0xc3b72));
        AbyssEngine::String::operator=
                  ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar51 * 4) + 0x18),
                   pSVar36);
        iVar43 = *(int *)(this + 0x160);
        iVar49 = *(int *)(this + 0x164);
        iVar50 = *(int *)(this + 0x168);
        iVar18 = *(int *)(this + 0x16c);
      }
      for (; iVar26 < iVar43 + iVar29 + iVar64 + iVar15 + iVar22 + iVar49 + iVar50 + iVar18;
          iVar26 = iVar26 + 1) {
        iVar18 = Globals::getRandomEnemyFighter((Globals *)*puVar20,8);
        uVar16 = createShip(this,8,0,iVar18,(Waypoint *)0x0,true,false);
        fVar66 = local_b8;
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4) = uVar16;
        iVar18 = (*pcVar21)(*piVar59,20000);
        iVar49 = (*pcVar21)(*piVar59,2);
        fVar67 = (float)local_b4;
        iVar43 = (*pcVar21)(*piVar59,20000);
        iVar62 = (*pcVar21)(*piVar59,2);
        iVar50 = -1;
        if (iVar62 == 0) {
          iVar50 = 1;
        }
        iVar62 = -1;
        if (iVar49 == 0) {
          iVar62 = 1;
        }
        fVar68 = local_b4._4_4_;
        iVar49 = (*pcVar21)(*piVar59,20000);
        fVar71 = (float)VectorSignedToFloat(iVar62 * iVar18 + 10000,(byte)(in_fpscr >> 0x16) & 3);
        fVar70 = (float)VectorSignedToFloat(iVar50 * iVar43 + 10000,(byte)(in_fpscr >> 0x16) & 3);
        iVar18 = (*pcVar21)(*piVar59,2);
        iVar43 = -1;
        if (iVar18 == 0) {
          iVar43 = 1;
        }
        fVar65 = (float)VectorSignedToFloat(iVar43 * iVar49 + 10000,(byte)(in_fpscr >> 0x16) & 3);
        uVar17 = VectorSignedToFloat((int)(fVar66 + fVar71),(byte)(in_fpscr >> 0x16) & 3);
        uVar69 = VectorSignedToFloat((int)(fVar67 + fVar70),(byte)(in_fpscr >> 0x16) & 3);
        piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4);
        uVar16 = VectorSignedToFloat((int)(fVar68 + fVar65),(byte)(in_fpscr >> 0x16) & 3);
        (**(code **)(*piVar56 + 0x48))(piVar56,uVar17,uVar69,uVar16);
        KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4));
        *(undefined1 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4) + 0x12f) = 1;
        iVar43 = *(int *)(this + 0x160);
        iVar49 = *(int *)(this + 0x164);
        iVar50 = *(int *)(this + 0x168);
        iVar18 = *(int *)(this + 0x16c);
      }
    }
    if (0 < iVar32) {
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=(local_17c,(Vector *)&local_b8);
      iVar18 = (*pcVar21)(*piVar59,50000);
      iVar26 = (*pcVar21)(*piVar59,2);
      iVar49 = -1;
      if (iVar26 == 0) {
        iVar49 = 1;
      }
      iVar43 = -1;
      fVar66 = (float)VectorSignedToFloat(iVar49 * iVar18 + 20000,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar66;
      iVar18 = (*pcVar21)(*piVar59,50000);
      iVar26 = (*pcVar21)(*piVar59,2);
      iVar49 = -1;
      if (iVar26 == 0) {
        iVar49 = 1;
      }
      fVar66 = (float)VectorSignedToFloat(iVar49 * iVar18 + 10000,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 400) = *(float *)(this + 400) + fVar66;
      iVar18 = (*pcVar21)(*piVar59,50000);
      iVar26 = (*pcVar21)(*piVar59,2);
      if (iVar26 == 0) {
        iVar43 = 1;
      }
      fVar66 = (float)VectorSignedToFloat(iVar43 * iVar18 + 20000,(byte)(in_fpscr >> 0x16) & 3);
      fVar67 = *(float *)(this + 0x194);
      *(float *)(this + 0x194) = fVar67 + fVar66;
      local_b8 = (float)(int)*(float *)(this + 0x18c);
      local_b4 = CONCAT44((int)(fVar67 + fVar66),(int)*(float *)(this + 400));
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_b8,3);
      iVar18 = *(int *)(this + 0x160);
      iVar26 = *(int *)(this + 0x164);
      iVar49 = *(int *)(this + 0x168);
      iVar43 = *(int *)(this + 0x16c);
      iVar50 = iVar15 + iVar64 + iVar29;
      for (iVar62 = iVar22 + iVar50 + iVar18 + iVar26 + iVar49 + iVar43;
          iVar62 < iVar18 + iVar50 + iVar32 + iVar22 + iVar26 + iVar49 + iVar43; iVar62 = iVar62 + 1
          ) {
        pWVar35 = (Waypoint *)Route::getWaypoint();
        uVar16 = createShip(this,10,0,0x2c,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) = uVar16;
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar62 * 4) + 4),0));
        iVar18 = *(int *)(this + 0x160);
        iVar26 = *(int *)(this + 0x164);
        iVar49 = *(int *)(this + 0x168);
        iVar43 = *(int *)(this + 0x16c);
      }
      pvVar44 = (void *)Route::~Route(pRVar27);
      operator_delete(pvVar44);
    }
    if (uVar58 != 0) {
      *(int *)(this + 0xac) =
           *(int *)(this + 0x160) + iVar15 + iVar64 + iVar29 + iVar32 + iVar22 +
           *(int *)(this + 0x164) + *(int *)(this + 0x168) + *(int *)(this + 0x16c);
      pWVar35 = operator_new(0x138);
      Waypoint::Waypoint(pWVar35,DAT_000c423c,0,20000,(Route *)0x0);
      uVar16 = createStaticObject(this,pWVar35,0x4a88,true);
      iVar18 = *(int *)(this + 0xac);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 * 4) = uVar16;
      PlayerFixedObject::setDockingType
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 * 4),1);
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)
                  (*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4),false);
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4);
      (**(code **)(*piVar56 + 0x44))(piVar56,this + 200);
      Player::setAlwaysFriend
                (*(Player **)
                  (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4) + 4),
                 true);
      uVar16 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4);
      pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c4240 + 0xc3f12));
      AbyssEngine::String::String(aSStack_14c,pSVar36,false);
      PlayerFixedObject::setName(uVar16,aSStack_14c);
      AbyssEngine::String::~String(aSStack_14c);
    }
    if (iVar24 != 0) {
      iVar43 = *(int *)(this + 0x160);
      iVar26 = *(int *)(this + 0x164);
      iVar64 = *(int *)(this + 0x168);
      iVar49 = *(int *)(this + 0x16c);
      pSVar23 = (Station *)Status::getStation();
      iVar18 = Station::getHiddenBlueprintIndex(pSVar23);
      iVar24 = 0xf;
      if (iVar18 == 0) {
        iVar24 = 0xd;
      }
      uVar16 = createShip(this,*(int *)(DAT_000c42d4 + 0xc3f86 + iVar18 * 4),1,iVar24,
                          (Waypoint *)0x0,true,false);
      iVar49 = iVar32 + iVar29 + iVar15 + iVar22 + uVar58 + iVar34 + iVar33 + iVar43 + iVar26 +
               iVar64 + iVar49;
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar49 * 4) = uVar16;
      pPVar61 = *(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar49 * 4);
      PlayerFixedObject::setDockingType(pPVar61,3);
      pSVar23 = (Station *)Status::getStation();
      PVar13 = (PlayerFixedObject)Station::stationHasHiddenBlueprint(pSVar23,false);
      pPVar61[0x70] = PVar13;
      (**(code **)(*(int *)pPVar61 + 0x44))(pPVar61,this + 200);
      PlayerFixedObject::setDeadButSelectable(pPVar61);
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar49 * 4);
      iVar15 = DAT_000c42d8 + 0xc4006 + iVar18 * 0xc;
      (**(code **)(*piVar56 + 0x48))
                (piVar56,*(undefined4 *)(DAT_000c42d8 + 0xc4006 + iVar18 * 0xc),
                 *(undefined4 *)(iVar15 + 4),*(undefined4 *)(iVar15 + 8));
      if (*(Array<int> **)(pPVar61 + 0x50) != (Array<int> *)0x0) {
        pvVar44 = (void *)Array<int>::~Array(*(Array<int> **)(pPVar61 + 0x50));
        operator_delete(pvVar44);
      }
      *(undefined4 *)(pPVar61 + 0x50) = 0;
      pSVar23 = (Station *)Status::getStation();
      iVar15 = Station::stationHasHiddenBlueprint(pSVar23,false);
      if (iVar15 != 0) {
        this_01 = operator_new(1);
        Generator::Generator(this_01);
        uVar16 = Generator::getLootList((int)this_01,0x73);
        *(undefined4 *)(pPVar61 + 0x50) = uVar16;
        pvVar44 = (void *)Generator::~Generator(this_01);
        operator_delete(pvVar44);
      }
      this_02 = operator_new(1);
      FileRead::FileRead(this_02);
      pAVar45 = (Array *)FileRead::loadSpacePoints
                                   (this_02,*(int *)(DAT_000c42dc + 0xc406e + iVar18 * 4),-1);
      pvVar44 = (void *)FileRead::~FileRead(this_02);
      operator_delete(pvVar44);
      KIPlayer::setSpacePoints((KIPlayer *)pPVar61,pAVar45);
    }
    if (bVar2) {
      pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c42e0 + 0xc409c));
      AbyssEngine::String::operator=
                ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar36);
    }
    goto switchD_000c02a8_caseD_8;
  }
  Status::getSystem();
  iVar15 = SolarSystem::getRace();
  piVar56 = *(int **)(DAT_000c038c + 0xc0276);
  iVar18 = AbyssEngine::AERandom::nextInt(*piVar56);
  if (iVar18 < 0x4b) {
    iVar18 = 8;
  }
  else {
    pSVar28 = (Standing *)Status::getStanding();
    iVar18 = Standing::getEnemyRace(pSVar28,iVar15);
  }
  iVar22 = Mission::getType();
  uVar10 = DAT_000c2acc;
  uVar9 = DAT_000c2478;
  uVar69 = DAT_000c1d90;
  uVar17 = DAT_000c15f8;
  uVar16 = DAT_000c0384;
  switch(iVar22) {
  case 1:
    pcVar21 = *(code **)(DAT_000c15f4 + 0xc14ba);
    iVar22 = (*pcVar21)(*piVar56);
    local_b8 = (float)(iVar22 + -50000);
    local_b4 = 0xc35000000000;
    local_ac = (*pcVar21)(*piVar56,uVar17);
    local_ac = local_ac + -50000;
    local_a8 = 0;
    iStack_a4 = DAT_000c15fc;
    iVar22 = (*pcVar21)(*piVar56,uVar17);
    local_a0 = (ulonglong)(iVar22 - 50000);
    local_98 = uVar17;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,9);
    *(Route **)(this + 0x110) = pRVar27;
    pMVar14 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar14);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    piVar59 = (int *)(DAT_000c1600 + 0xc1530);
    iVar22 = AbyssEngine::AERandom::nextInt(*piVar56);
    fVar66 = (float)VectorSignedToFloat((int)((fVar66 / 10.0) * 5.0) + 3,
                                        (byte)(in_fpscr >> 0x16) & 3);
    uVar58 = (uint)(fVar66 + (*(float *)(*piVar59 + 0x2c) + -0.5) * fVar66);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(uVar58 + iVar22 + 3,(Array *)pAVar19);
    puVar20 = *(undefined4 **)(DAT_000c1604 + 0xc1596);
    for (iVar22 = 0; uVar42 = uVar58, iVar22 < (int)uVar58; iVar22 = iVar22 + 1) {
      iVar24 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar18);
      uVar16 = createShip(this,iVar18,0,iVar24,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) = uVar16;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) + 4),0));
    }
    for (; uVar42 < **(uint **)(this + 0xf8); uVar42 = uVar42 + 1) {
      iVar18 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar15);
      pRVar27 = *(Route **)(this + 0x110);
      iVar22 = *piVar56;
      Route::length();
      iVar22 = AbyssEngine::AERandom::nextInt(iVar22);
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar22);
      uVar16 = createShip(this,iVar15,0,iVar18,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4) = uVar16;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4) + 4),true);
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,7,uVar58,this);
LAB_000c16cc:
    *(Objective **)(this + 0x28) = pOVar38;
    break;
  case 2:
    local_e0 = 0;
    fStack_dc = 0.0;
    local_d8 = 0;
    iVar24 = AbyssEngine::AERandom::nextInt(*piVar56);
    iVar22 = DAT_000c19ec;
    fVar66 = (float)VectorSignedToFloat(DAT_000c19ec - iVar24,(byte)(in_fpscr >> 0x16) & 3);
    local_b8 = (float)(int)fVar66;
    local_b4 = local_b4 & 0xffffffff00000000;
    iVar24 = AbyssEngine::AERandom::nextInt(*piVar56);
    fVar66 = (float)VectorSignedToFloat(iVar22 - iVar24,(byte)(in_fpscr >> 0x16) & 3);
    local_ac = 0;
    local_a8 = 0;
    iStack_a4 = 0;
    local_b4 = CONCAT44((int)fVar66,(float)local_b4);
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,6);
    *(Route **)(this + 0x110) = pRVar27;
    iVar22 = Mission::getProductionGoodAmount();
    uVar16 = Mission::getDifficulty(pMVar14);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    iVar29 = (int)((fVar66 / 10.0) * 4.0);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    iVar26 = iVar29 + 2;
    ArraySetLength<KIPlayer*>(iVar26 + iVar22,(Array *)pAVar19);
    local_178 = 0;
    for (iVar24 = 0; fVar66 = DAT_000c19f8, iVar24 < iVar26; iVar24 = iVar24 + 1) {
      iVar32 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c19f0 + 0xc179a),iVar18);
      uVar16 = createShip(this,iVar18,0,iVar32,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4) = uVar16;
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4);
      pcVar21 = *(code **)(DAT_000c19f4 + 0xc17d2);
      iVar32 = (*pcVar21)(*(undefined4 *)(this + 0x110),0);
      iVar34 = *(int *)(iVar32 + 0x124);
      iVar32 = (*pcVar21)(*(undefined4 *)(this + 0x110),0);
      iVar33 = *(int *)(iVar32 + 0x128);
      iVar32 = (*pcVar21)(*(undefined4 *)(this + 0x110),0);
      uVar16 = VectorSignedToFloat(local_178 + iVar34,(byte)(in_fpscr >> 0x16) & 3);
      uVar17 = VectorSignedToFloat(local_178 + iVar33,(byte)(in_fpscr >> 0x16) & 3);
      uVar69 = VectorSignedToFloat(*(int *)(iVar32 + 300) + local_178,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar56 + 0x48))(piVar56,uVar16,uVar17,uVar69);
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4) + 4),0));
      pKVar52 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar24 * 4);
      pRVar27 = (Route *)Route::clone(*(__fn **)(this + 0x110),*(void **)(*(int *)(this + 0xf8) + 4)
                                      ,__flags_02,__arg_02);
      KIPlayer::setRoute(pKVar52,pRVar27);
      iVar32 = KIPlayer::getRoute(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4));
      Route::reachWaypoint(iVar32);
      local_178 = local_178 + 2000;
    }
    iVar18 = iVar29 * 4 + 8;
    puVar20 = *(undefined4 **)(DAT_000c19fc + 0xc18a8);
    for (iVar24 = 0; (uint)(iVar26 + iVar24) < **(uint **)(this + 0xf8); iVar24 = iVar24 + 1) {
      iVar29 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar15);
      uVar16 = createShip(this,iVar15,0,iVar29,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 + iVar24 * 4) = uVar16;
      Player::setAlwaysFriend
                (*(Player **)
                  (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 + iVar24 * 4) + 4),true);
      (**(code **)(**(int **)((*(uint **)(this + 0xfc))[1] +
                             (iVar24 + (**(uint **)(this + 0xfc) >> 1)) * 4) + 0x28))
                ((Vector *)&local_70);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)&local_70);
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 + iVar24 * 4);
      (**(code **)(*piVar56 + 0x48))(piVar56,local_e0,fStack_dc + fVar66,local_d8);
      iVar29 = *(int *)(*(int *)(this + 0xf8) + 4) + iVar18;
      iVar32 = *(int *)(iVar29 + iVar24 * 4);
      *(undefined4 *)(iVar32 + 0x50) = 0;
      *(undefined1 *)(iVar32 + 0x4c) = 0;
      piVar56 = *(int **)(iVar29 + iVar24 * 4);
      fVar67 = (float)(**(code **)(*piVar56 + 0x1c))(piVar56,0);
      KIPlayer::setRotationSpeed
                (*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 + iVar24 * 4),fVar67);
      iVar29 = *(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18 + iVar24 * 4) + 4);
      Player::getMaxHitpoints();
      Player::setHitpoints(iVar29);
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x12,0,iVar26,this);
    *(Objective **)(this + 0x28) = pOVar38;
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x12,iVar26,iVar26 + iVar22,this);
    goto LAB_000c374c;
  case 3:
  case 5:
    pcVar21 = *(code **)(DAT_000c0390 + 0xc02d8);
    iVar15 = (*pcVar21)(*piVar56);
    iVar18 = (*pcVar21)(*piVar56,2);
    iVar22 = -1;
    if (iVar18 == 0) {
      iVar22 = 1;
    }
    iVar24 = -1;
    local_b8 = (float)(iVar22 * (iVar15 + 40000));
    local_b4 = (ulonglong)(uint)local_b4._4_4_ << 0x20;
    iVar15 = (*pcVar21)(*piVar56,uVar16);
    iVar18 = (*pcVar21)(*piVar56,2);
    if (iVar18 == 0) {
      iVar24 = 1;
    }
    local_b4 = CONCAT44(iVar24 * (iVar15 + 40000),(float)local_b4);
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,3);
    *(Route **)(this + 0x108) = pRVar27;
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    uVar58 = Mission::getProductionGoodAmount();
    ArraySetLength<KIPlayer*>(uVar58,*(Array **)(this + 0xf8));
    uVar58 = 0;
    while( true ) {
      uVar42 = **(uint **)(this + 0xf8);
      if (uVar42 <= uVar58) break;
      iVar15 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c06d0 + 0xc039c),8);
      pWVar35 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x108),0);
      uVar16 = createShip(this,8,0,iVar15,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4));
      uVar58 = uVar58 + 1;
    }
    PlayerFighter::setMissionCrate
              (*(PlayerFighter **)((*(uint **)(this + 0xf8))[1] + uVar42 * 4 + -4),true);
    pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c06d4 + 0xc0400));
    AbyssEngine::String::operator=
              ((String *)
               (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 0x18),
               pSVar36);
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0xb,**(int **)(this + 0xf8) + -1,this);
    *(Objective **)(this + 0x28) = pOVar38;
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0xc,**(int **)(this + 0xf8) + -1,this);
    *(Objective **)(this + 0x2c) = pOVar38;
    break;
  case 4:
    iVar15 = AbyssEngine::AERandom::nextInt(*piVar56);
    if (iVar15 == 0) {
      local_b8 = (float)(int)*(float *)(this + 200);
      local_b4 = CONCAT44((int)*(float *)(this + 0xd0),(int)*(float *)(this + 0xcc));
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_b8,3);
      *(Route **)(this + 0x108) = pRVar27;
    }
    else {
      pLVar30 = (Level *)AbyssEngine::AERandom::nextInt(*piVar56);
      uVar16 = createRoute(pLVar30,(int)(pLVar30 + 2));
      *(undefined4 *)(this + 0x108) = uVar16;
    }
    pMVar14 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar14);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    fVar66 = (float)VectorSignedToFloat((int)((fVar66 / 10.0) * 5.0) + 2,
                                        (byte)(in_fpscr >> 0x16) & 3);
    uVar42 = (uint)(fVar66 + (*(float *)(*(int *)(DAT_000c42e4 + 0xc410e) + 0x2c) + -0.5) * fVar66);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(uVar42,(Array *)pAVar19);
    puVar20 = *(undefined4 **)(DAT_000c42e8 + 0xc416a);
    for (uVar58 = 0; uVar58 < **(uint **)(this + 0xf8); uVar58 = uVar58 + 1) {
      iVar15 = Globals::getRandomEnemyFighter((Globals *)*puVar20,8);
      pRVar27 = *(Route **)(this + 0x108);
      iVar18 = *piVar56;
      Route::length();
      iVar18 = AbyssEngine::AERandom::nextInt(iVar18);
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar18);
      uVar16 = createShip(this,8,0,iVar15,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4));
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x12,0,uVar42,this);
    *(Objective **)(this + 0x28) = pOVar38;
    break;
  case 6:
    pcVar21 = *(code **)(DAT_000c1d8c + 0xc1a08);
    iVar15 = (*pcVar21)(*piVar56);
    iVar18 = (*pcVar21)(*piVar56,2);
    iVar22 = -1;
    if (iVar18 == 0) {
      iVar22 = 1;
    }
    iVar24 = -1;
    local_b8 = (float)(iVar22 * (iVar15 + 60000));
    local_b4 = (ulonglong)(uint)local_b4._4_4_ << 0x20;
    iVar15 = (*pcVar21)(*piVar56,uVar69);
    iVar18 = (*pcVar21)(*piVar56,2);
    if (iVar18 == 0) {
      iVar24 = 1;
    }
    local_b4 = CONCAT44(iVar24 * (iVar15 + 60000),(float)local_b4);
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,3);
    *(Route **)(this + 0x108) = pRVar27;
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar19);
    iVar15 = Globals::getRandomEnemyFighter((Globals *)**(undefined4 **)(DAT_000c1d94 + 0xc1a96),8);
    pWVar35 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x108),0);
    uVar16 = createShip(this,8,0,iVar15,pWVar35,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar16;
    KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
    pPVar55 = *(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
    iVar15 = Player::getMaxHitpoints();
    Player::setMaxHitpoints(pPVar55,iVar15 * 3);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c1d98);
    PlayerFighter::setRotate((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),3);
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,1,0,this);
    *(Objective **)(this + 0x28) = pOVar38;
    break;
  case 7:
    pcVar21 = *(code **)(DAT_000c1d9c + 0xc1b2e);
    iVar15 = (*pcVar21)(*piVar56,40000);
    local_b8 = (float)(iVar15 + -20000);
    iVar15 = (*pcVar21)(*piVar56,20000);
    local_b4 = CONCAT44(local_b4._4_4_,iVar15 + -10000);
    iVar15 = (*pcVar21)(*piVar56,30000);
    local_b4 = CONCAT44(iVar15 + 40000,(float)local_b4);
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,3);
    pMVar14 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar14);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    iVar15 = (int)(fVar66 / 10.0 + fVar66 / 10.0);
    pMVar14 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar14);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>((int)((fVar66 / 10.0) * 20.0) + iVar15 + 0xf,(Array *)pAVar19);
    uVar58 = 0;
    while( true ) {
      puVar47 = *(uint **)(this + 0xf8);
      uVar42 = *puVar47;
      uVar54 = uVar42 - iVar15;
      if (uVar54 <= uVar58) break;
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,0);
      uVar16 = createStaticObject(this,pWVar35,0x4215,true);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) + 4),0));
      uVar58 = uVar58 + 1;
    }
    piVar56 = *(int **)(DAT_000c1da0 + 0xc1c30);
    for (; uVar54 < uVar42; uVar54 = uVar54 + 1) {
      iVar18 = Globals::getRandomEnemyFighter((Globals *)*piVar56,8);
      uVar16 = createShip(this,8,0,iVar18,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar54 * 4) = uVar16;
      puVar47 = *(uint **)(this + 0xf8);
      uVar42 = *puVar47;
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,7,*puVar47 - iVar15,this);
    iVar15 = DAT_000c1da4;
    *(Objective **)(this + 0x28) = pOVar38;
    *(int *)(this + 0x130) = iVar15 + 1000;
    Globals::addSoundResourceToList(*piVar56);
    break;
  case 8:
  case 0xb:
  case 0xd:
  case 0xe:
    break;
  case 9:
    local_a8 = 0;
    local_b8 = 1.4013e-41;
    local_98 = DAT_000c1dac;
    local_b4 = (ulonglong)DAT_000c1da8 << 0x20;
    local_ac = 10000;
    iStack_a4 = DAT_000c1db0;
    local_a0 = 10000;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,9);
    *(Route **)(this + 0x110) = pRVar27;
    pMVar41 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar41);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    iVar15 = *(int *)(DAT_000c1db4 + 0xc1cfa);
    fVar66 = (float)VectorSignedToFloat((int)((fVar66 / 10.0) * 5.0) + 3,
                                        (byte)(in_fpscr >> 0x16) & 3);
    iVar26 = (int)(fVar66 + (*(float *)(iVar15 + 0x2c) + -0.5) * fVar66);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(iVar26 + 5U,(Array *)pAVar19);
    pSVar28 = (Standing *)Status::getStanding();
    iVar18 = Mission::getClientRace(pMVar14);
    iVar18 = Standing::getEnemyRace(pSVar28,iVar18);
    iVar22 = Mission::getClientRace(pMVar14);
    iVar24 = 0xf;
    if (iVar22 < 4) {
      local_18c = Mission::getClientRace(pMVar14);
      if (local_18c == 1) {
        iVar24 = 0xd;
      }
    }
    else {
      local_18c = 0;
    }
    fVar67 = DAT_000c35c8;
    dVar8 = DAT_000c35c0;
    fVar66 = DAT_000c35bc;
    puVar20 = *(undefined4 **)(DAT_000c35b8 + 0xc34fe);
    for (uVar58 = 0; uVar58 < **(uint **)(this + 0xf8); uVar58 = uVar58 + 1) {
      if ((int)uVar58 < iVar26) {
        iVar22 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar18);
        iVar29 = *piVar56;
        pRVar27 = *(Route **)(this + 0x110);
        Route::length();
        iVar29 = AbyssEngine::AERandom::nextInt(iVar29);
        pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar29);
        uVar16 = createShip(this,iVar18,0,iVar22,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
        KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4));
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) + 4),0));
      }
      else {
        uVar16 = createShip(this,local_18c,1,iVar24,(Waypoint *)0x0,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) = uVar16;
        pPVar55 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) + 4);
        iVar22 = Status::getLevel();
        if (iVar22 < 0x15) {
          iVar22 = Status::getLevel();
        }
        else {
          iVar22 = 0x14;
        }
        fVar68 = (float)VectorSignedToFloat(iVar22 << 1,(byte)(in_fpscr >> 0x16) & 3);
        iVar22 = Status::gameWon();
        if (iVar22 == 0) {
          iVar22 = Status::getCurrentCampaignMission();
        }
        else {
          iVar22 = 0x2d;
        }
        fVar71 = (float)VectorSignedToFloat(iVar22 << 1,(byte)(in_fpscr >> 0x16) & 3);
        dVar74 = (double)*(float *)(iVar15 + 0x2c);
        uVar42 = in_fpscr & 0xfffffff | (uint)(dVar74 < dVar8) << 0x1f |
                 (uint)(dVar74 == dVar8) << 0x1e;
        in_fpscr = uVar42 | (uint)(NAN(dVar74) || NAN(dVar8)) << 0x1c;
        bVar5 = (byte)(uVar42 >> 0x18);
        fVar70 = 1.0;
        if (!(bool)(bVar5 >> 6 & 1) && bVar5 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
          fVar70 = fVar67;
        }
        Player::setMaxHitpoints(pPVar55,(int)((fVar68 + fVar66 + fVar71) * fVar70));
        PlayerFixedObject::setMoving
                  (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4),true);
        Player::setAlwaysFriend
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4) + 4),true
                  );
        iVar22 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar58 * 4);
        *(undefined4 *)(iVar22 + 0x50) = 0;
        *(undefined1 *)(iVar22 + 0x4c) = 0;
      }
    }
    piVar56 = *(int **)((*(uint **)(this + 0xf8))[1] + iVar26 * 4);
    (**(code **)(*piVar56 + 0x48))(piVar56,DAT_000c3a98,DAT_000c3a9c,DAT_000c3aa0);
    uVar16 = DAT_000c3aa4;
    piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4 + 4);
    (**(code **)(*piVar56 + 0x48))(piVar56,DAT_000c3aa8,DAT_000c3aa4,DAT_000c3aac);
    piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4 + 8);
    (**(code **)(*piVar56 + 0x48))(piVar56,DAT_000c3ab0,DAT_000c3ab4,DAT_000c3ab8);
    piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4 + 0xc);
    (**(code **)(*piVar56 + 0x48))(piVar56,DAT_000c3abc,DAT_000c3ac0,DAT_000c3ac4);
    piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar26 * 4 + 0x10);
    (**(code **)(*piVar56 + 0x48))(piVar56,uVar16,DAT_000c3ac8,DAT_000c3acc);
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x12,0,iVar26,this);
    *(Objective **)(this + 0x28) = pOVar38;
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x12,iVar26,iVar26 + 5U,this);
    goto LAB_000c374c;
  case 10:
    pcVar21 = *(code **)(DAT_000c20fc + 0xc1dc2);
    iVar15 = (*pcVar21)(*piVar56,5000);
    local_b8 = (float)(iVar15 + -0x9c4);
    iVar15 = (*pcVar21)(*piVar56,5000);
    local_b4 = CONCAT44(local_b4._4_4_,iVar15 + -0x9c4);
    iVar15 = (*pcVar21)(*piVar56,30000);
    local_b4 = CONCAT44(iVar15 + DAT_000c2100,(float)local_b4);
    local_ac = (*pcVar21)(*piVar56,5000);
    local_ac = local_ac + -0x9c4;
    local_a8 = (*pcVar21)(*piVar56,5000);
    local_a8 = local_a8 + -0x9c4;
    iStack_a4 = (*pcVar21)(*piVar56,30000);
    iStack_a4 = iStack_a4 + DAT_000c2104;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,6);
    *(Route **)(this + 0x108) = pRVar27;
    iVar18 = AbyssEngine::AERandom::nextInt(*piVar56);
    pMVar41 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar41);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    iVar22 = *(int *)(DAT_000c2108 + 0xc1e6a);
    fVar67 = *(float *)(iVar22 + 0x2c);
    pSVar28 = (Standing *)Status::getStanding();
    iVar15 = Mission::getClientRace(pMVar14);
    iVar24 = Standing::getEnemyRace(pSVar28,iVar15);
    fVar66 = (float)VectorSignedToFloat((int)((fVar66 / 10.0) * 5.0) + 3,
                                        (byte)(in_fpscr >> 0x16) & 3);
    iVar15 = 0;
    if (iVar24 != 8) {
      iVar15 = iVar24;
    }
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    uVar58 = iVar18 + 2;
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>((int)(fVar66 + (fVar67 + -0.5) * fVar66) + uVar58,(Array *)pAVar19);
    fVar67 = DAT_000c211c;
    fVar66 = DAT_000c2118;
    dVar8 = DAT_000c2110;
    iVar18 = 0xf;
    if (iVar15 == 1) {
      iVar18 = 0xd;
    }
    for (iVar24 = 0; iVar24 < (int)uVar58; iVar24 = iVar24 + 1) {
      pcVar57 = *(code **)(DAT_000c2120 + 0xc1f14);
      pWVar35 = (Waypoint *)(*pcVar57)(*(undefined4 *)(this + 0x108),1);
      uVar16 = createShip(this,iVar15,1,iVar18,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4) = uVar16;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4) + 4),0));
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4),false);
      piVar59 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4);
      iVar26 = (*pcVar57)(*(undefined4 *)(this + 0x108),1);
      iVar26 = *(int *)(iVar26 + 0x124);
      iVar29 = (*pcVar21)(*piVar56,20000);
      iVar32 = (*pcVar57)(*(undefined4 *)(this + 0x108),1);
      iVar34 = *(int *)(iVar32 + 0x128);
      iVar32 = (*pcVar21)(*piVar56,20000);
      iVar33 = (*pcVar57)(*(undefined4 *)(this + 0x108),1);
      iVar33 = *(int *)(iVar33 + 300);
      uVar69 = VectorSignedToFloat(iVar26 + iVar29 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      uVar17 = VectorSignedToFloat(iVar32 + iVar34 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      iVar26 = (*pcVar21)(*piVar56,20000);
      uVar16 = VectorSignedToFloat(iVar26 + iVar33 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar59 + 0x48))(piVar59,uVar69,uVar17,uVar16);
      pPVar55 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar24 * 4) + 4);
      uVar16 = Player::getMaxHitpoints();
      fVar70 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
      dVar74 = (double)*(float *)(iVar22 + 0x2c);
      uVar42 = in_fpscr & 0xfffffff | (uint)(dVar74 < dVar8) << 0x1f |
               (uint)(dVar74 == dVar8) << 0x1e;
      in_fpscr = uVar42 | (uint)(NAN(dVar74) || NAN(dVar8)) << 0x1c;
      bVar5 = (byte)(uVar42 >> 0x18);
      fVar68 = 1.0;
      if (!(bool)(bVar5 >> 6 & 1) && bVar5 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        fVar68 = fVar67;
      }
      Player::setMaxHitpoints(pPVar55,(int)(fVar70 * fVar66 * fVar68));
    }
    puVar20 = *(undefined4 **)(DAT_000c2124 + 0xc2062);
    for (uVar42 = uVar58; uVar42 < **(uint **)(this + 0xf8); uVar42 = uVar42 + 1) {
      iVar18 = Globals::getRandomEnemyFighter((Globals *)*puVar20,iVar15);
      pRVar27 = *(Route **)(this + 0x108);
      iVar22 = *piVar56;
      Route::length();
      iVar22 = AbyssEngine::AERandom::nextInt(iVar22);
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar22);
      uVar16 = createShip(this,iVar15,0,iVar18,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4) = uVar16;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4) + 4),0));
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,7,uVar58,this);
    *(Objective **)(this + 0x28) = pOVar38;
    break;
  case 0xc:
    pLVar30 = (Level *)AbyssEngine::AERandom::nextInt(*piVar56);
    uVar16 = createRoute(pLVar30,(int)(pLVar30 + 3));
    *(undefined4 *)(this + 0x108) = uVar16;
    uVar16 = Mission::getDifficulty(pMVar14);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    iVar15 = (int)((fVar66 / 10.0) * 4.0);
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    uVar58 = iVar15 + 3;
    if ((uVar58 & 1) == 0) {
      uVar58 = iVar15 + 4;
    }
    uVar58 = uVar58 + 1;
    ArraySetLength<KIPlayer*>(uVar58,(Array *)pAVar19);
    pcVar21 = *(code **)(DAT_000c2460 + 0xc218e);
    pAVar40 = (Agent *)(*pcVar21)(pMVar14);
    iVar15 = Agent::getRace(pAVar40);
    puVar20 = *(undefined4 **)(DAT_000c2464 + 0xc21a0);
    this_03 = (Globals *)*puVar20;
    pAVar40 = (Agent *)(*pcVar21)(pMVar14);
    iVar18 = Agent::getRace(pAVar40);
    iVar18 = Globals::getRandomEnemyFighter(this_03,iVar18);
    uVar16 = createShip(this,iVar15,0,iVar18,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar16;
    piVar59 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    iVar15 = AbyssEngine::AERandom::nextInt(*piVar56);
    fVar67 = (float)VectorSignedToFloat(iVar15 + -700,(byte)(in_fpscr >> 0x16) & 3);
    iVar15 = AbyssEngine::AERandom::nextInt(*piVar56);
    fVar66 = (float)VectorSignedToFloat(iVar15 + -700,(byte)(in_fpscr >> 0x16) & 3);
    (**(code **)(*piVar59 + 0x48))
              (piVar59,local_ec + fVar67,local_e8 + fVar66,local_e4 + DAT_000c2468);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c246c);
    PlayerFighter::setRotate((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),3);
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    pKVar52 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar27 = (Route *)Route::clone(*(__fn **)(this + 0x108),
                                    *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_03,__arg_03)
    ;
    KIPlayer::setRoute(pKVar52,pRVar27);
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    (*pcVar21)(pMVar14);
    Agent::getName();
    AbyssEngine::String::operator=
              ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),(String *)&local_b8);
    AbyssEngine::String::~String((String *)&local_b8);
    puVar47 = *(uint **)(this + 0xf8);
    *(undefined4 *)(*(int *)puVar47[1] + 0x50) = 0;
    for (uVar42 = 1; uVar42 < *puVar47; uVar42 = uVar42 + 1) {
      iVar15 = Globals::getRandomEnemyFighter((Globals *)*puVar20,8);
      pRVar27 = *(Route **)(this + 0x108);
      iVar18 = *piVar56;
      Route::length();
      iVar18 = AbyssEngine::AERandom::nextInt(iVar18);
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar18);
      uVar16 = createShip(this,8,0,iVar15,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4) = uVar16;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar42 * 4));
      puVar47 = *(uint **)(this + 0xf8);
    }
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x14,1,uVar58,this);
    *(Objective **)(this + 0x28) = pOVar38;
    pOVar38 = operator_new(0x1c);
    Objective::Objective(pOVar38,0x15,1,uVar58,this);
LAB_000c374c:
    *(Objective **)(this + 0x2c) = pOVar38;
    break;
  case 0xf:
    pcVar21 = *(code **)(DAT_000c2474 + 0xc2370);
    iVar18 = (*pcVar21)(*piVar56,DAT_000c2478);
    iVar15 = DAT_000c247c;
    local_b8 = (float)(iVar18 + DAT_000c247c);
    local_b4 = (ulonglong)DAT_000c2480 << 0x20;
    local_ac = (*pcVar21)(*piVar56,uVar9);
    local_ac = local_ac + iVar15;
    local_a8 = 0;
    iStack_a4 = DAT_000c2484;
    iVar18 = (*pcVar21)(*piVar56,uVar9);
    local_a0 = (ulonglong)(uint)(iVar18 + iVar15);
    local_98 = uVar9;
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,(int *)&local_b8,3);
    *(Route **)(this + 0x110) = pRVar27;
    pMVar41 = (Mission *)Status::getMission();
    uVar16 = Mission::getDifficulty(pMVar41);
    fVar66 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
    fVar67 = *(float *)(*(int *)(DAT_000c2488 + 0xc23e0) + 0x2c);
    fVar66 = (float)VectorSignedToFloat((int)(fVar66 / 10.0 + fVar66 / 10.0) + 1,
                                        (byte)(in_fpscr >> 0x16) & 3);
    pSVar28 = (Standing *)Status::getStanding();
    iVar15 = Mission::getClientRace(pMVar14);
    iVar15 = Standing::getEnemyRace(pSVar28,iVar15);
    iVar24 = (int)(fVar66 + (fVar67 + -0.5) * fVar66);
    iVar18 = Mission::getClientRace(pMVar14);
    *(undefined4 *)(this + 0x28c) = 1;
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
    ArraySetLength<KIPlayer*>(*(int *)(this + 0x28c) + iVar24 + 2,(Array *)pAVar19);
    for (iVar22 = 0; iVar22 < iVar24; iVar22 = iVar22 + 1) {
      iVar26 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c26ec + 0xc2494),iVar15);
      iVar29 = *piVar56;
      pRVar27 = *(Route **)(this + 0x110);
      Route::length();
      iVar29 = AbyssEngine::AERandom::nextInt(iVar29);
      pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,iVar29);
      uVar16 = createShip(this,iVar15,0,iVar26,pWVar35,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) = uVar16;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) + 4),0));
    }
    *(int *)(this + 0xac) = iVar24;
    uVar16 = createStaticObject(this,*(Waypoint **)(this + 0xd8),0x4a88,true);
    iVar15 = *(int *)(this + 0xac);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar16;
    PlayerFixedObject::setDockingType
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4),1);
    PlayerFixedObject::setMoving
              (*(PlayerFixedObject **)
                (*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4),false);
    piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4);
    (**(code **)(*piVar56 + 0x44))(piVar56,this + 200);
    uStack_d4 = *(undefined4 *)(DAT_000c26f0 + 0xc2576);
    local_d0 = *(undefined8 *)(DAT_000c26f0 + 0xc257a);
    local_e0 = (int)(*(float *)(this + 200) + DAT_000c26f4);
    fStack_dc = (float)(int)(*(float *)(this + 0xcc) + DAT_000c26f4);
    local_d8 = (int)(*(float *)(this + 0xd0) + DAT_000c26f8);
    local_70 = 0x177000000000;
    pAVar19 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar19);
    ArraySetLength<KIPlayer*>(2,(Array *)pAVar19);
    *(undefined4 *)(*(int *)(pAVar19 + 4) + 4) =
         *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4);
    pRVar27 = operator_new(0x18);
    Route::Route(pRVar27,&local_e0,(Array *)pAVar19,(int *)&local_70,6);
    Route::setLoop(SUB41(pRVar27,0));
    pRVar39 = (Route *)Route::clone((__fn *)pRVar27,__child_stack_00,__flags_04,__arg_04);
    iVar15 = *(int *)(this + 0x28c);
    for (iVar22 = iVar15 + iVar24; iVar22 < iVar15 + iVar24 + 2; iVar22 = iVar22 + 1) {
      iVar26 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c26fc + 0xc261a),iVar18);
      if (iVar22 == iVar15 + iVar24) {
        pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,0);
        uVar16 = createShip(this,iVar18,0,iVar26,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) = uVar16;
        pRVar48 = pRVar27;
      }
      else {
        pWVar35 = (Waypoint *)Route::getWaypoint(pRVar39,0);
        uVar16 = createShip(this,iVar18,0,iVar26,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) = uVar16;
        pRVar48 = pRVar39;
      }
      KIPlayer::setRoute(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4),pRVar48);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) + 4),true);
      Player::setNeverAttack
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar22 * 4) + 4),true);
      iVar15 = *(int *)(this + 0x28c);
    }
    pOVar38 = operator_new(0x1c);
    iVar15 = Mission::getProductionGoodAmount();
    iVar18 = Mission::getProductionGoodIndex(pMVar14);
    Objective::Objective(pOVar38,0x1c,iVar15,iVar18,this);
    goto LAB_000c29da;
  default:
    if (iVar22 == 0xb8) {
      pcVar21 = *(code **)(DAT_000c2ac8 + 0xc2708);
      iVar18 = (*pcVar21)(*piVar56,DAT_000c2acc);
      iVar15 = DAT_000c2ad0;
      local_b8 = (float)(iVar18 + DAT_000c2ad0);
      local_b4 = (ulonglong)DAT_000c2ad4 << 0x20;
      local_ac = (*pcVar21)(*piVar56,uVar10);
      local_ac = local_ac + iVar15;
      local_a8 = 0;
      iStack_a4 = DAT_000c2ad8;
      iVar18 = (*pcVar21)(*piVar56,uVar10);
      local_a0 = (ulonglong)(uint)(iVar18 + iVar15);
      local_98 = uVar10;
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_b8,3);
      *(Route **)(this + 0x110) = pRVar27;
      pMVar41 = (Mission *)Status::getMission();
      Mission::getDifficulty(pMVar41);
      pSVar28 = (Standing *)Status::getStanding();
      iVar15 = Mission::getClientRace(pMVar14);
      Standing::getEnemyRace(pSVar28,iVar15);
      iVar15 = Mission::getClientRace(pMVar14);
      pAVar19 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar19);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
      ArraySetLength<KIPlayer*>(4,(Array *)pAVar19);
      pWVar35 = operator_new(0x138);
      Waypoint::Waypoint(pWVar35,DAT_000c2adc,0,40000,(Route *)0x0);
      uVar16 = createStaticObject(this,pWVar35,0x4a88,true);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar16;
      PlayerFixedObject::setMoving
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      PlayerFixedObject::setDockingType
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),2);
      piVar56 = *(int **)(DAT_000c2ae0 + 0xc27f8);
      uVar16 = **(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pSVar36 = (String *)GameText::getText(*piVar56);
      AbyssEngine::String::String(aSStack_164,pSVar36,false);
      PlayerFixedObject::setName(uVar16,aSStack_164);
      AbyssEngine::String::~String(aSStack_164);
      pWVar35 = operator_new(0x138);
      Waypoint::Waypoint(pWVar35,30000,0,40000,(Route *)0x0);
      uVar16 = createStaticObject(this,pWVar35,0x498e,true);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar16;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),false);
      PlayerFixedObject::setDockingType
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),1);
      uVar16 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      pSVar36 = (String *)GameText::getText(*piVar56);
      AbyssEngine::String::String(aSStack_170,pSVar36,false);
      PlayerFixedObject::setName(uVar16,aSStack_170);
      AbyssEngine::String::~String(aSStack_170);
      __aeabi_memcpy(&local_e0,DAT_000c2ae4 + 0xc28a4,0x24);
      local_70 = *(undefined8 *)(DAT_000c2ae8 + 0xc28ac);
      local_68 = *(int *)(DAT_000c2ae8 + 0xc28b4);
      pAVar19 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar19);
      ArraySetLength<KIPlayer*>(3,(Array *)pAVar19);
      **(undefined4 **)(pAVar19 + 4) = **(undefined4 **)(*(int *)(this + 0xf8) + 4);
      *(undefined4 *)(*(int *)(pAVar19 + 4) + 8) =
           *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,&local_e0,(Array *)pAVar19,(int *)&local_70,9);
      Route::setLoop(SUB41(pRVar27,0));
      pRVar39 = (Route *)Route::clone((__fn *)pRVar27,__child_stack_01,__flags_05,__arg_05);
      for (iVar18 = 8; iVar18 != 0x10; iVar18 = iVar18 + 4) {
        iVar22 = Globals::getRandomEnemyFighter
                           ((Globals *)**(undefined4 **)(DAT_000c2aec + 0xc291a),iVar15);
        if (iVar18 == 8) {
          pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,0);
          uVar16 = createShip(this,iVar15,0,iVar22,pWVar35,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 8) = uVar16;
          pKVar52 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
          pRVar48 = pRVar27;
        }
        else {
          pWVar35 = (Waypoint *)Route::getWaypoint(pRVar39,0);
          uVar16 = createShip(this,iVar15,0,iVar22,pWVar35,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18) = uVar16;
          pKVar52 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18);
          pRVar48 = pRVar39;
        }
        KIPlayer::setRoute(pKVar52,pRVar48);
        Player::setAlwaysFriend
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18) + 4),true);
        Player::setNeverAttack
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar18) + 4),true);
      }
      pOVar38 = operator_new(0x1c);
      iVar15 = Mission::getProductionGoodAmount();
      iVar18 = Mission::getProductionGoodIndex(pMVar14);
      Objective::Objective(pOVar38,0x1d,iVar15,iVar18,this);
    }
    else {
      if (iVar22 != 0xb7) break;
      local_e0 = *(int *)(DAT_000c1158 + 0xc10da);
      fStack_dc = *(float *)(DAT_000c1158 + 0xc10de);
      local_d8 = *(int *)(DAT_000c1158 + 0xc10e2);
      uStack_d4 = *(undefined4 *)(DAT_000c1158 + 0xc10e6);
      local_d0 = *(undefined8 *)(DAT_000c1158 + 0xc10ea);
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,&local_e0,6);
      *(Route **)(this + 0x110) = pRVar27;
      pAVar19 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar19);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar19;
      ArraySetLength<KIPlayer*>(0xe,(Array *)pAVar19);
      for (iVar15 = 0; iVar15 != 8; iVar15 = iVar15 + 1) {
        pWVar35 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
        uVar16 = createShip(this,10,0,0x2c,pWVar35,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) = uVar16;
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) + 4),0));
        Player::setMaxHitpoints
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) + 4),0xfa
                  );
      }
      uVar16 = createStaticObject(this,(Waypoint *)0x0,0x495d,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) = uVar16;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20),false);
      PlayerFixedObject::setDockingType
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20),2);
      uVar16 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20);
      pSVar36 = (String *)GameText::getText(**(int **)(DAT_000c159c + 0xc11ea));
      AbyssEngine::String::String(aSStack_158,pSVar36,false);
      PlayerFixedObject::setName(uVar16,aSStack_158);
      AbyssEngine::String::~String(aSStack_158);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) + 4),true);
      local_b8 = 0.0;
      local_b4 = (ulonglong)DAT_000c15a0;
      AEGeometry::rotate(*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) + 8));
      iVar15 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20);
      puVar20 = (undefined4 *)(iVar15 + 0x50);
      *(undefined4 *)(iVar15 + 0x28) = 10;
      *(undefined1 *)(iVar15 + 0x74) = 1;
      if ((Array<int> *)*puVar20 != (Array<int> *)0x0) {
        pvVar44 = (void *)Array<int>::~Array((Array<int> *)*puVar20);
        operator_delete(pvVar44);
        puVar20 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) + 0x50);
      }
      *puVar20 = 0;
      pWVar35 = operator_new(0x138);
      Waypoint::Waypoint(pWVar35,DAT_000c15a4,DAT_000c15a8,DAT_000c15ac,(Route *)0x0);
      uVar16 = createStaticObject(this,pWVar35,0x4974,true);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24) = uVar16;
      piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24);
      (**(code **)(*(int *)pWVar35 + 0x28))(&local_b8,pWVar35);
      (**(code **)(*piVar56 + 0x44))(piVar56,&local_b8);
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24),false);
      pPVar61 = *(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24);
      pPVar61[0x70] = (PlayerFixedObject)0x0;
      PlayerFixedObject::setDockingType(pPVar61,1);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24) + 4),true);
      Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24) + 4));
      iVar15 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24);
      puVar20 = (undefined4 *)(iVar15 + 0x50);
      *(undefined4 *)(iVar15 + 0x28) = 10;
      *(undefined1 *)(iVar15 + 0x74) = 1;
      if ((Array<int> *)*puVar20 != (Array<int> *)0x0) {
        pvVar44 = (void *)Array<int>::~Array((Array<int> *)*puVar20);
        operator_delete(pvVar44);
        puVar20 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24) + 0x50);
      }
      iVar15 = DAT_000c15b4;
      *puVar20 = 0;
      __aeabi_memcpy(&local_b8,iVar15 + 0xc1340,0x24);
      local_70 = *(undefined8 *)(DAT_000c15b8 + 0xc134a);
      local_68 = *(int *)(DAT_000c15b8 + 0xc1352);
      pAVar19 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar19);
      ArraySetLength<KIPlayer*>(3,(Array *)pAVar19);
      **(undefined4 **)(pAVar19 + 4) = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24);
      *(undefined4 *)(*(int *)(pAVar19 + 4) + 8) =
           *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20);
      pRVar27 = operator_new(0x18);
      Route::Route(pRVar27,(int *)&local_b8,(Array *)pAVar19,(int *)&local_70,9);
      Route::setLoop(SUB41(pRVar27,0));
      fVar66 = extraout_s0;
      fVar67 = extraout_s1;
      fVar68 = extraout_s2;
      for (iVar15 = 10; iVar15 != 0xe; iVar15 = iVar15 + 1) {
        pWVar35 = (Waypoint *)Route::getWaypoint(pRVar27,0);
        uVar16 = createShip(this,0,0,0x33,pWVar35,true,false);
        pvVar44 = *(void **)(*(int *)(this + 0xf8) + 4);
        *(undefined4 *)((int)pvVar44 + iVar15 * 4) = uVar16;
        pKVar52 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4);
        pRVar39 = (Route *)Route::clone((__fn *)pRVar27,pvVar44,__flags_01,__arg_01);
        KIPlayer::setRoute(pKVar52,pRVar39);
        Player::setAlwaysFriend
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4) + 4),true
                  );
        uVar58 = iVar15 - 10U & 0x7fffffff;
        piVar56 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar15 * 4);
        *(undefined2 *)((int)piVar56 + 0x13d) = 0;
        if (uVar58 == 2) {
          local_78 = 0.0;
          local_7c = DAT_000c15d4;
          local_74 = DAT_000c15d8;
        }
        else if (uVar58 == 1) {
          local_78 = DAT_000c15c8;
          local_7c = DAT_000c15cc;
          local_74 = DAT_000c15d0;
        }
        else if (uVar58 == 0) {
          local_78 = DAT_000c15bc;
          local_7c = DAT_000c15c0;
          local_74 = DAT_000c15c4;
        }
        else {
          local_78 = DAT_000c15dc;
          local_7c = DAT_000c15e0;
          local_74 = DAT_000c15e4;
        }
        (**(code **)(*piVar56 + 0x44))(piVar56,&local_7c);
        fVar66 = extraout_s0_00;
        fVar67 = extraout_s1_00;
        fVar68 = extraout_s2_00;
      }
      PlayerEgo::setPosition(fVar66,fVar67,fVar68);
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 200),(Vector *)&local_7c);
      iVar15 = DAT_000c15f0;
      *(int *)(this + 0x130) = DAT_000c15f0;
      pOVar38 = operator_new(0x1c);
      Objective::Objective(pOVar38,3,iVar15,this);
    }
LAB_000c29da:
    *(Objective **)(this + 0x28) = pOVar38;
  }
switchD_000c02a8_caseD_8:
LAB_000c0194:
  if (*piVar63 != local_64) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000bffd8: push {r4,r5,r6,r7,lr}
  000bffda: add r7,sp,#0xc
  000bffdc: push {r7,r8,r9,r10,r11}
  000bffe0: vpush {d8,d9,d10,d11,d12,d13,d14}
  000bffe4: sub sp,#0x190
  000bffe6: mov r10,r0
  000bffe8: ldr r0,[0x000c036c]
  000bffea: ldr r1,[0x000c0370]
  000bffec: add r0,pc
  000bffee: add r1,pc
  000bfff0: ldr.w r11,[r0,#0x0]
  000bfff4: ldr r5,[r1,#0x0]
  000bfff6: ldr.w r1,[r11,#0x0]
  000bfffa: ldr r0,[r5,#0x0]
  000bfffc: str r1,[sp,#0x18c]
  000bfffe: blx 0x00072850
  000c0002: mov r9,r0
  000c0004: cmp r0,#0x0
  000c0006: beq.w 0x000c0194
  000c000a: ldr.w r0,[r10,#0xf0]
  000c000e: ldr r1,[r0,#0x8]
  000c0010: add r0,sp,#0x104
  000c0012: blx 0x000720b8
  000c0016: ldr r0,[r5,#0x0]
  000c0018: blx 0x000723d0
  000c001c: cmp r0,#0x0
  000c001e: str.w r10,[sp,#0x7c]
  000c0022: str r5,[sp,#0x6c]
  000c0024: str.w r11,[sp,#0x58]
  000c0028: beq.w 0x000c01b2
  000c002c: ldr r0,[r5,#0x0]
  000c002e: blx 0x00071c5c
  000c0032: mov r5,r0
  000c0034: ldr r0,[0x000c0374]
  000c0036: movs r1,#0x2
  000c0038: movs r6,#0x2
  000c003a: add r0,pc
  000c003c: ldr r0,[r0,#0x0]
  000c003e: str r0,[sp,#0x78]
  000c0040: ldr r0,[r0,#0x0]
  000c0042: blx 0x00071848
  000c0046: vmov s2,r5
  000c004a: vmov.f32 s0,0x3f000000
  000c004e: vcvt.f32.s32 s2,s2
  000c0052: vmov.f32 s16,0xbf800000
  000c0056: vmov s4,r0
  000c005a: vcvt.f32.s32 s4,s4
  000c005e: vmla.f32 s16,s2,s0
  000c0062: vmov.f32 s0,0x40000000
  000c0066: vadd.f32 s2,s16,s4
  000c006a: vcmpe.f32 s2,s0
  000c006e: vmrs apsr,fpscr
  000c0072: bmi 0x000c0092
  000c0074: ldr r0,[sp,#0x78]
  000c0076: movs r1,#0x2
  000c0078: ldr r0,[r0,#0x0]
  000c007a: blx 0x00071848
  000c007e: vmov s0,r0
  000c0082: vcvt.f32.s32 s0,s0
  000c0086: vadd.f32 s0,s16,s0
  000c008a: vcvt.s32.f32 s0,s0
  000c008e: vmov r6,s0
  000c0092: mov r5,r6
  000c0094: ldr r6,[sp,#0x6c]
  000c0096: ldr r0,[r6,#0x0]
  000c0098: blx 0x00071770
  000c009c: mov r8,r0
  000c009e: ldr r0,[r6,#0x0]
  000c00a0: mov r6,r5
  000c00a2: blx 0x00071770
  000c00a6: cmp r0,#0x21
  000c00a8: it eq
  000c00aa: mov.eq r6,#0x2
  000c00ac: movs r0,#0xc
  000c00ae: cmp.w r8,#0x44
  000c00b2: it eq
  000c00b4: mov.eq r6,#0x2
  000c00b6: blx 0x0006eb24
  000c00ba: mov r5,r0
  000c00bc: blx 0x00073d08
  000c00c0: mov r0,r6
  000c00c2: mov r1,r5
  000c00c4: str.w r5,[r10,#0xf8]
  000c00c8: blx 0x00073d14
  000c00cc: ldr r0,[0x000c0378]
  000c00ce: mov.w r9,#0x0
  000c00d2: add r0,pc
  000c00d4: ldr r0,[r0,#0x0]
  000c00d6: str r0,[sp,#0x74]
  000c00d8: ldr r0,[0x000c037c]
  000c00da: str r6,[sp,#0x6c]
  000c00dc: add r0,pc
  000c00de: ldr r0,[r0,#0x0]
  000c00e0: str r0,[sp,#0x70]
  000c00e2: ldrd r11,r10,[sp,#0x78]
  000c00e6: ldr.w r8,[sp,#0x70]
  000c00ea: b 0x000c018e
  000c00ec: ldr r0,[sp,#0x74]
  000c00ee: movs r1,#0x9
  000c00f0: ldr r0,[r0,#0x0]
  000c00f2: blx 0x00071a34
  000c00f6: mov r3,r0
  000c00f8: movs r0,#0x0
  000c00fa: movs r1,#0x1
  000c00fc: str r0,[sp,#0x0]
  000c00fe: strd r1,r0,[sp,#0x4]
  000c0102: mov r0,r10
  000c0104: movs r1,#0x9
  000c0106: movs r2,#0x0
  000c0108: blx 0x00073e88
  000c010c: ldr.w r1,[r10,#0xf8]
  000c0110: ldr r4,[0x000c0380]
  000c0112: ldr r1,[r1,#0x4]
  000c0114: str.w r0,[r1,r9,lsl #0x2]
  000c0118: ldr.w r0,[r10,#0xf8]
  000c011c: ldr r1,[r0,#0x4]
  000c011e: ldr.w r0,[r11,#0x0]
  000c0122: ldr.w r6,[r1,r9,lsl #0x2]
  000c0126: mov r1,r4
  000c0128: blx r8
  000c012a: mov r5,r0
  000c012c: ldr.w r0,[r11,#0x0]
  000c0130: ldr r1,[0x000c0384]
  000c0132: blx r8
  000c0134: movw r1,#0x9c40
  000c0138: subs r0,r0,r1
  000c013a: movw r4,#0xea60
  000c013e: ldr r1,[0x000c0380]
  000c0140: vmov s0,r0
  000c0144: subs r0,r5,r4
  000c0146: vmov s2,r0
  000c014a: ldr.w r0,[r11,#0x0]
  000c014e: vcvt.f32.s32 s16,s2
  000c0152: vcvt.f32.s32 s18,s0
  000c0156: blx r8
  000c0158: subs r0,r0,r4
  000c015a: vmov r1,s16
  000c015e: vmov r2,s18
  000c0162: vmov s0,r0
  000c0166: vcvt.f32.s32 s0,s0
  000c016a: ldr r0,[r6,#0x0]
  000c016c: ldr r5,[r0,#0x48]
  000c016e: mov r0,r6
  000c0170: ldr r6,[sp,#0x6c]
  000c0172: vmov r3,s0
  000c0176: blx r5
  000c0178: ldr.w r0,[r10,#0xf8]
  000c017c: movs r1,#0x1
  000c017e: ldr r0,[r0,#0x4]
  000c0180: ldr.w r0,[r0,r9,lsl #0x2]
  000c0184: ldr r0,[r0,#0x4]
  000c0186: blx 0x00073e94
  000c018a: add.w r9,r9,#0x1
  000c018e: cmp r9,r6
  000c0190: blt 0x000c00ec
  000c0192: b 0x000c02ca
  000c0194: ldr r0,[sp,#0x18c]
  000c0196: ldr.w r1,[r11,#0x0]
  000c019a: subs r0,r1,r0
  000c019c: itttt eq
  000c019e: add.eq sp,#0x190
  000c01a0: vpop.eq {d8,d9,d10,d11,d12,d13,d14}
  000c01a4: add.eq sp,#0x4
  000c01a6: pop.eq.w {r8,r9,r10,r11}
  000c01aa: it eq
  000c01ac: pop.eq {r4,r5,r6,r7,pc}
  000c01ae: blx 0x0006e824
  000c01b2: mov r0,r9
  000c01b4: blx 0x00072868
  000c01b8: cmp r0,#0x0
  000c01ba: beq 0x000c0262
  000c01bc: ldr r0,[0x000c0388]
  000c01be: add r0,pc
  000c01c0: ldr r0,[r0,#0x0]
  000c01c2: str r0,[sp,#0x64]
  000c01c4: ldr r0,[r0,#0x0]
  000c01c6: blx 0x000718cc
  000c01ca: movs r6,#0x0
  000c01cc: mov r0,r10
  000c01ce: str.w r6,[r10,#0xc0]
  000c01d2: blx 0x00073c6c
  000c01d6: movs r0,#0x3
  000c01d8: str.w r0,[r10,#0xc0]
  000c01dc: ldr r0,[r5,#0x0]
  000c01de: blx 0x00071a10
  000c01e2: blx 0x00071a7c
  000c01e6: cmp r0,#0xf
  000c01e8: bne 0x000c01f6
  000c01ea: ldr r0,[r5,#0x0]
  000c01ec: blx 0x00071770
  000c01f0: cmp r0,#0x10
  000c01f2: it lt
  000c01f4: mov.lt r6,#0x1
  000c01f6: ldr r0,[r5,#0x0]
  000c01f8: blx 0x00071c14
  000c01fc: blx 0x00071824
  000c0200: str r0,[sp,#0x68]
  000c0202: movs r1,#0x64
  000c0204: ldr r0,[sp,#0x64]
  000c0206: ldr r0,[r0,#0x0]
  000c0208: blx 0x00071848
  000c020c: mov r10,r0
  000c020e: ldr r0,[r5,#0x0]
  000c0210: blx 0x00073444
  000c0214: mov r4,r0
  000c0216: cmp r0,#0x0
  000c0218: beq.w 0x000c0458
  000c021c: mov r0,r4
  000c021e: blx 0x00072874
  000c0222: cbz r0,0x000c023a
  000c0224: mov r0,r4
  000c0226: blx 0x00072874
  000c022a: cmp r0,#0xb
  000c022c: beq 0x000c023a
  000c022e: mov r0,r4
  000c0230: blx 0x00072874
  000c0234: cmp r0,#0xf
  000c0236: bne.w 0x000c0458
  000c023a: mov r0,r4
  000c023c: blx 0x00073ea0
  000c0240: vmov s2,r0
  000c0244: vmov.f32 s0,0x41200000
  000c0248: vcvt.f32.s32 s2,s2
  000c024c: vdiv.f32 s0,s2,s0
  000c0250: vmov.f32 s2,0x40a00000
  000c0254: vmul.f32 s0,s0,s2
  000c0258: vcvt.s32.f32 s0,s0
  000c025c: vmov r9,s0
  000c0260: b 0x000c045c
  000c0262: ldr r0,[r5,#0x0]
  000c0264: blx 0x00071a10
  000c0268: blx 0x00071950
  000c026c: mov r4,r0
  000c026e: ldr r0,[0x000c038c]
  000c0270: movs r1,#0x64
  000c0272: add r0,pc
  000c0274: ldr r0,[r0,#0x0]
  000c0276: str r0,[sp,#0x78]
  000c0278: ldr r0,[r0,#0x0]
  000c027a: blx 0x00071848
  000c027e: cmp r0,#0x4b
  000c0280: bge 0x000c0286
  000c0282: movs r0,#0x8
  000c0284: b 0x000c0292
  000c0286: ldr r0,[r5,#0x0]
  000c0288: blx 0x00071b84
  000c028c: mov r1,r4
  000c028e: blx 0x00073eac
  000c0292: str r0,[sp,#0x74]
  000c0294: mov r0,r9
  000c0296: blx 0x00072874
  000c029a: ldr.w r11,[sp,#0x78]
  000c029e: subs r1,r0,#0x1
  000c02a0: cmp r1,#0xe
  000c02a2: bhi.w 0x000c10c6
  000c02a6: str r4,[sp,#0x70]
  000c02a8: tbh [pc,r1]
  000c02ca: ldr.w r11,[sp,#0x58]
  000c02ce: b 0x000c0194
  000c02d0: ldr r2,[0x000c0390]
  000c02d2: ldr r1,[0x000c0384]
  000c02d4: add r2,pc
  000c02d6: ldr.w r0,[r11,#0x0]
  000c02da: ldr r5,[r2,#0x0]
  000c02dc: mov r10,r1
  000c02de: blx r5
  000c02e0: ldr.w r1,[r11,#0x0]
  000c02e4: movw r8,#0x9c40
  000c02e8: add.w r4,r0,r8
  000c02ec: mov r0,r1
  000c02ee: movs r1,#0x2
  000c02f0: blx r5
  000c02f2: cmp r0,#0x0
  000c02f4: mov.w r0,#0xffffffff
  000c02f8: it eq
  000c02fa: mov.eq r0,#0x1
  000c02fc: mov.w r6,#0xffffffff
  000c0300: mul r1,r0,r4
  000c0304: movs r0,#0x0
  000c0306: str r0,[sp,#0x13c]
  000c0308: ldr.w r0,[r11,#0x0]
  000c030c: str r1,[sp,#0x138]
  000c030e: mov r1,r10
  000c0310: blx r5
  000c0312: ldr.w r1,[r11,#0x0]
  000c0316: add.w r4,r0,r8
  000c031a: mov r0,r1
  000c031c: movs r1,#0x2
  000c031e: blx r5
  000c0320: cmp r0,#0x0
  000c0322: it eq
  000c0324: mov.eq r6,#0x1
  000c0326: mul r0,r6,r4
  000c032a: str r0,[sp,#0x140]
  000c032c: movs r0,#0x18
  000c032e: blx 0x0006eb24
  000c0332: mov r4,r0
  000c0334: add r1,sp,#0x138
  000c0336: movs r2,#0x3
  000c0338: blx 0x00073eb8
  000c033c: ldr r6,[sp,#0x7c]
  000c033e: movs r0,#0xc
  000c0340: str.w r4,[r6,#0x108]
  000c0344: blx 0x0006eb24
  000c0348: mov r4,r0
  000c034a: blx 0x00073d08
  000c034e: mov r0,r9
  000c0350: str.w r4,[r6,#0xf8]
  000c0354: blx 0x00072bb0
  000c0358: ldr.w r1,[r6,#0xf8]
  000c035c: blx 0x00073d14
  000c0360: mov.w r8,#0x0
  000c0364: mov.w r9,#0x1
  000c0368: movs r5,#0x0
  000c036a: b 0x000c03dc
  000c0394: ldr r0,[0x000c06d0]
  000c0396: movs r1,#0x8
  000c0398: add r0,pc
  000c039a: ldr r0,[r0,#0x0]
  000c039c: ldr r0,[r0,#0x0]
  000c039e: blx 0x00071a34
  000c03a2: mov r4,r0
  000c03a4: ldr.w r0,[r6,#0x108]
  000c03a8: movs r1,#0x0
  000c03aa: blx 0x00073ec4
  000c03ae: strd r0,r9,[sp,#0x0]
  000c03b2: mov r0,r6
  000c03b4: movs r1,#0x8
  000c03b6: movs r2,#0x0
  000c03b8: mov r3,r4
  000c03ba: str.w r8,[sp,#0x8]
  000c03be: blx 0x00073e88
  000c03c2: ldr.w r1,[r6,#0xf8]
  000c03c6: ldr r1,[r1,#0x4]
  000c03c8: str.w r0,[r1,r5,lsl #0x2]
  000c03cc: ldr.w r0,[r6,#0xf8]
  000c03d0: ldr r0,[r0,#0x4]
  000c03d2: ldr.w r0,[r0,r5,lsl #0x2]
  000c03d6: blx 0x000732dc
  000c03da: adds r5,#0x1
  000c03dc: ldr.w r0,[r6,#0xf8]
  000c03e0: ldr r1,[r0,#0x0]
  000c03e2: cmp r5,r1
  000c03e4: bcc 0x000c0394
  000c03e6: ldr r0,[r0,#0x4]
  000c03e8: add.w r0,r0,r1, lsl #0x2
  000c03ec: movs r1,#0x1
  000c03ee: ldr.w r0,[r0,#-0x4]
  000c03f2: blx 0x00073ed0
  000c03f6: ldr r0,[0x000c06d4]
  000c03f8: movw r1,#0x64b
  000c03fc: add r0,pc
  000c03fe: ldr r0,[r0,#0x0]
  000c0400: ldr r0,[r0,#0x0]
  000c0402: blx 0x00072f70
  000c0406: mov r1,r0
  000c0408: ldr.w r0,[r6,#0xf8]
  000c040c: ldrd r2,r0,[r0,#0x0]
  000c0410: add.w r0,r0,r2, lsl #0x2
  000c0414: ldr.w r0,[r0,#-0x4]
  000c0418: adds r0,#0x18
  000c041a: blx 0x0006f2b0
  000c041e: movs r0,#0x1c
  000c0420: blx 0x0006eb24
  000c0424: mov r4,r0
  000c0426: ldr.w r0,[r6,#0xf8]
  000c042a: ldr r0,[r0,#0x0]
  000c042c: subs r2,r0,#0x1
  000c042e: mov r0,r4
  000c0430: movs r1,#0xb
  000c0432: mov r3,r6
  000c0434: blx 0x00073edc
  000c0438: movs r0,#0x1c
  000c043a: str r4,[r6,#0x28]
  000c043c: blx 0x0006eb24
  000c0440: mov r4,r0
  000c0442: ldr.w r0,[r6,#0xf8]
  000c0446: ldr r0,[r0,#0x0]
  000c0448: subs r2,r0,#0x1
  000c044a: mov r0,r4
  000c044c: movs r1,#0xc
  000c044e: mov r3,r6
  000c0450: blx 0x00073edc
  000c0454: str r4,[r6,#0x2c]
  000c0456: b 0x000c02ca
  000c0458: mov.w r9,#0x0
  000c045c: ldr r0,[r5,#0x0]
  000c045e: blx 0x00071a10
  000c0462: cmp r0,#0x0
  000c0464: str r4,[sp,#0x78]
  000c0466: beq 0x000c0482
  000c0468: ldr r0,[r5,#0x0]
  000c046a: blx 0x00071a10
  000c046e: blx 0x00073ee8
  000c0472: cbz r0,0x000c0482
  000c0474: ldr r0,[sp,#0x64]
  000c0476: movs r1,#0x1
  000c0478: ldr r0,[r0,#0x0]
  000c047a: blx 0x00071848
  000c047e: add.w r9,r0,#0x2
  000c0482: ldr r0,[r5,#0x0]
  000c0484: blx 0x00071a10
  000c0488: blx 0x00073ef4
  000c048c: mov r4,r0
  000c048e: mov r0,r5
  000c0490: ldr r0,[r0,#0x0]
  000c0492: movs r5,#0x0
  000c0494: cmp r4,#0x0
  000c0496: it gt
  000c0498: mov.gt r5,#0x1
  000c049a: blx 0x0007192c
  000c049e: ands r0,r5
  000c04a0: cmp r6,#0x0
  000c04a2: sub.w r5,r4,r0
  000c04a6: str r6,[sp,#0x74]
  000c04a8: beq 0x000c04b4
  000c04aa: ldr r0,[sp,#0x6c]
  000c04ac: ldr r0,[r0,#0x0]
  000c04ae: blx 0x0007192c
  000c04b2: cbz r0,0x000c04be
  000c04b4: cbz r5,0x000c04c4
  000c04b6: cmp r5,#0x1
  000c04b8: bne 0x000c04c8
  000c04ba: movs r0,#0x41
  000c04bc: b 0x000c04d0
  000c04be: movs r0,#0x0
  000c04c0: str r0,[sp,#0x54]
  000c04c2: b 0x000c04da
  000c04c4: movs r0,#0x5a
  000c04c6: b 0x000c04d0
  000c04c8: movs r0,#0xa
  000c04ca: cmp r5,#0x2
  000c04cc: it eq
  000c04ce: mov.eq r0,#0x23
  000c04d0: movs r1,#0x0
  000c04d2: cmp r10,r0
  000c04d4: it lt
  000c04d6: mov.lt r1,#0x1
  000c04d8: str r1,[sp,#0x54]
  000c04da: ldr r6,[sp,#0x64]
  000c04dc: ldr r1,[0x000c06d8]
  000c04de: ldr r0,[r6,#0x0]
  000c04e0: blx 0x00071848
  000c04e4: movw r4,#0xc350
  000c04e8: subs r0,r0,r4
  000c04ea: str r0,[sp,#0x110]
  000c04ec: movs r1,#0x0
  000c04ee: ldr r0,[r6,#0x0]
  000c04f0: str r1,[sp,#0x114]
  000c04f2: movw r1,#0xc350
  000c04f6: blx 0x00071848
  000c04fa: add r0,r4
  000c04fc: str r0,[sp,#0x118]
  000c04fe: movs r0,#0x18
  000c0500: blx 0x0006eb24
  000c0504: mov r4,r0
  000c0506: add r1,sp,#0x110
  000c0508: movs r2,#0x3
  000c050a: blx 0x00073eb8
  000c050e: ldr.w r10,[sp,#0x7c]
  000c0512: str.w r4,[r10,#0x180]
  000c0516: ldr r4,[sp,#0x6c]
  000c0518: ldr r0,[r4,#0x0]
  000c051a: blx 0x00071a10
  000c051e: blx 0x00071950
  000c0522: mov r8,r0
  000c0524: ldr r0,[r6,#0x0]
  000c0526: movs r1,#0x64
  000c0528: blx 0x00071848
  000c052c: cmp r0,#0x4b
  000c052e: str.w r8,[sp,#0x70]
  000c0532: bge 0x000c0538
  000c0534: movs r0,#0x8
  000c0536: b 0x000c0544
  000c0538: ldr r0,[r4,#0x0]
  000c053a: blx 0x00071b84
  000c053e: mov r1,r8
  000c0540: blx 0x00073eac
  000c0544: str r0,[sp,#0x44]
  000c0546: ldr r0,[sp,#0x54]
  000c0548: ldr r4,[sp,#0x68]
  000c054a: cbz r0,0x000c0560
  000c054c: ldr r0,[sp,#0x64]
  000c054e: movs r1,#0x4
  000c0550: ldr r0,[r0,#0x0]
  000c0552: blx 0x00071848
  000c0556: vmov s0,r0
  000c055a: vcvt.f32.s32 s0,s0
  000c055e: b 0x000c0564
  000c0560: vldr.32 s0,[pc,#0x178]
  000c0564: vcvt.s32.f32 s2,s0
  000c0568: vcvt.s32.f32 s0,s0
  000c056c: vstr.32 s2,[r10,#0x16c]
  000c0570: vmov r0,s0
  000c0574: cmp r0,#0x1
  000c0576: blt 0x000c05d8
  000c0578: ldr r0,[sp,#0x6c]
  000c057a: ldr r0,[r0,#0x0]
  000c057c: blx 0x0007192c
  000c0580: cbz r0,0x000c0594
  000c0582: ldr r0,[sp,#0x64]
  000c0584: movs r1,#0x6
  000c0586: ldr r0,[r0,#0x0]
  000c0588: blx 0x00071848
  000c058c: adds r0,#0x2
  000c058e: str.w r0,[r10,#0x16c]
  000c0592: b 0x000c0598
  000c0594: ldr.w r0,[r10,#0x16c]
  000c0598: ldr r1,[0x000c06e0]
  000c059a: vmov.f32 s0,0xbf000000
  000c059e: vmov s4,r0
  000c05a2: add r1,pc
  000c05a4: ldr r1,[r1,#0x0]
  000c05a6: vldr.32 s2,[r1,#0x2c]
  000c05aa: vcvt.f32.s32 s4,s4
  000c05ae: vadd.f32 s0,s2,s0
  000c05b2: vmla.f32 s4,s0,s4
  000c05b6: vcvt.s32.f32 s0,s4
  000c05ba: ldr r0,[sp,#0x6c]
  000c05bc: vstr.32 s0,[r10,#0x16c]
  000c05c0: ldr r0,[r0,#0x0]
  000c05c2: blx 0x00071c5c
  000c05c6: asrs r2,r0,#0x1f
  000c05c8: ldr.w r1,[r10,#0x16c]
  000c05cc: add.w r0,r0,r2, lsr #0x1e
  000c05d0: add.w r0,r1,r0, asr #0x2
  000c05d4: str.w r0,[r10,#0x16c]
  000c05d8: ldr r0,[0x000c06e4]
  000c05da: ldr r1,[sp,#0x54]
  000c05dc: add r0,pc
  000c05de: ldr r0,[r0,#0x0]
  000c05e0: str r0,[sp,#0x50]
  000c05e2: subs r0,r5,#0x3
  000c05e4: clz r0,r0
  000c05e8: lsrs r0,r0,#0x5
  000c05ea: ands r0,r1
  000c05ec: cmp r0,#0x1
  000c05ee: bne 0x000c0614
  000c05f0: vmov.f32 s2,0x3f800000
  000c05f4: ldr r0,[sp,#0x50]
  000c05f6: vldr.32 s0,[r0,#0x2c]
  000c05fa: vcmpe.f32 s0,s2
  000c05fe: vmrs apsr,fpscr
  000c0602: bpl 0x000c0614
  000c0604: ldr r0,[sp,#0x64]
  000c0606: movs r1,#0x2
  000c0608: ldr r0,[r0,#0x0]
  000c060a: blx 0x00071848
  000c060e: adds r0,#0x1
  000c0610: str.w r0,[r10,#0x16c]
  000c0614: cmp r4,#0x4e
  000c0616: bne 0x000c0620
  000c0618: movs r4,#0x0
  000c061a: strd r4,r4,[r10,#0x164]
  000c061e: b 0x000c0644
  000c0620: ldr r0,[0x000c06e8]
  000c0622: movs r1,#0x2
  000c0624: ldr r6,[sp,#0x64]
  000c0626: add r0,pc
  000c0628: ldr r4,[r0,#0x0]
  000c062a: ldr r0,[r6,#0x0]
  000c062c: blx r4
  000c062e: str.w r0,[r10,#0x164]
  000c0632: movs r1,#0x5
  000c0634: ldr r0,[r6,#0x0]
  000c0636: blx r4
  000c0638: str.w r0,[r10,#0x168]
  000c063c: movs r1,#0x2
  000c063e: ldr r0,[r6,#0x0]
  000c0640: blx r4
  000c0642: mov r4,r0
  000c0644: add.w r0,r10,#0x168
  000c0648: str r0,[sp,#0x48]
  000c064a: add.w r0,r10,#0x164
  000c064e: str r0,[sp,#0x4c]
  000c0650: mov r6,r9
  000c0652: ldr r0,[sp,#0x74]
  000c0654: ldr.w r8,[sp,#0x64]
  000c0658: ldr.w r9,[sp,#0x70]
  000c065c: cbz r0,0x000c066a
  000c065e: ldr.w r0,[r8,#0x0]
  000c0662: movs r1,#0x2
  000c0664: blx 0x00071848
  000c0668: mov r5,r0
  000c066a: ldr.w r0,[r10,#0x168]
  000c066e: asrs r1,r0,#0x1f
  000c0670: add.w r0,r0,r1, lsr #0x1e
  000c0674: adds r1,r5,r4
  000c0676: ldr r5,[sp,#0x6c]
  000c0678: add.w r0,r1,r0, asr #0x2
  000c067c: str.w r0,[r10,#0x160]
  000c0680: ldr r0,[sp,#0x54]
  000c0682: cbz r0,0x000c06a6
  000c0684: ldr r0,[r5,#0x0]
  000c0686: blx 0x00071770
  000c068a: cmp r0,#0x20
  000c068c: blt 0x000c06a6
  000c068e: ldr.w r0,[r8,#0x0]
  000c0692: movs r1,#0x64
  000c0694: blx 0x00071848
  000c0698: cmp r0,#0x7
  000c069a: ittt le
  000c069c: mov.le r0,#0x9
  000c069e: str.w.le r0,[r10,#0x160]
  000c06a2: str.w.le r0,[r10,#0x16c]
  000c06a6: ldr r0,[r5,#0x0]
  000c06a8: blx 0x00071c14
  000c06ac: blx 0x00073f00
  000c06b0: cmp r0,#0x0
  000c06b2: mov r1,r0
  000c06b4: str r0,[sp,#0x30]
  000c06b6: beq 0x000c06ec
  000c06b8: vmov.f32 s2,0xbf000000
  000c06bc: ldr r0,[sp,#0x50]
  000c06be: vmov.f32 s0,0x40a00000
  000c06c2: vldr.32 s4,[r0,#0x2c]
  000c06c6: vadd.f32 s2,s4,s2
  000c06ca: vmla.f32 s0,s2,s0
  000c06ce: b 0x000c06f0
  000c06ec: vldr.32 s0,[pc,#-0x14]
  000c06f0: ldr r0,[r5,#0x0]
  000c06f2: vcvt.s32.f32 s16,s0
  000c06f6: blx 0x00071a10
  000c06fa: cbz r0,0x000c0736
  000c06fc: ldr r0,[r5,#0x0]
  000c06fe: blx 0x00071a10
  000c0702: blx 0x00073ee8
  000c0706: cbz r0,0x000c0736
  000c0708: ldr.w r0,[r8,#0x0]
  000c070c: movs r1,#0x1
  000c070e: blx 0x00071848
  000c0712: ldr r1,[r5,#0x0]
  000c0714: adds r6,r0,#0x2
  000c0716: mov r0,r1
  000c0718: blx 0x0007192c
  000c071c: cbz r0,0x000c072c
  000c071e: ldr.w r0,[r8,#0x0]
  000c0722: movs r1,#0x3
  000c0724: blx 0x00071848
  000c0728: add.w r6,r0,r6, lsl #0x1
  000c072c: movs r1,#0x0
  000c072e: movs r0,#0x0
  000c0730: str r0,[sp,#0x54]
  000c0732: str.w r1,[r10,#0x16c]
  000c0736: ldr r0,[r5,#0x0]
  000c0738: blx 0x00071c14
  000c073c: movs r1,#0x1
  000c073e: blx 0x00072718
  000c0742: str r0,[sp,#0x24]
  000c0744: ldr r0,[0x000c0a74]
  000c0746: add r0,pc
  000c0748: ldr r0,[r0,#0x0]
  000c074a: ldrb r0,[r0,#0x0]
  000c074c: cbnz r0,0x000c075a
  000c074e: ldr r0,[r5,#0x0]
  000c0750: blx 0x00071c14
  000c0754: blx 0x0007321c
  000c0758: cbz r0,0x000c07ac
  000c075a: ldr r0,[r5,#0x0]
  000c075c: blx 0x00071770
  000c0760: cmp r0,#0x2a
  000c0762: beq 0x000c07ac
  000c0764: ldr.w r0,[r10,#0x168]
  000c0768: movs r1,#0x4
  000c076a: cmp r0,#0x2
  000c076c: it le
  000c076e: mov.le r0,#0x2
  000c0770: str.w r0,[r10,#0x168]
  000c0774: ldr.w r0,[r8,#0x0]
  000c0778: blx 0x00071848
  000c077c: adds r0,#0x2
  000c077e: str.w r0,[r10,#0x16c]
  000c0782: ldr.w r0,[r10,#0x100]
  000c0786: cbz r0,0x000c07a4
  000c0788: ldr r0,[r0,#0x4]
  000c078a: ldr r1,[r0,#0xc]
  000c078c: cbz r1,0x000c07a4
  000c078e: ldr r0,[r1,#0x0]
  000c0790: ldr.w r4,[r10,#0x180]
  000c0794: ldr r2,[r0,#0x28]
  000c0796: add r0,sp,#0xf8
  000c0798: blx r2
  000c079a: add r3,sp,#0xf8
  000c079c: mov r0,r4
  000c079e: ldmia r3,{r1,r2,r3}
  000c07a0: blx 0x00073f0c
  000c07a4: movs r0,#0x1
  000c07a6: str r0,[sp,#0x54]
  000c07a8: movs r0,#0x9
  000c07aa: str r0,[sp,#0x44]
  000c07ac: ldr r0,[r5,#0x0]
  000c07ae: blx 0x00073f18
  000c07b2: mov r4,r0
  000c07b4: cbz r0,0x000c07c8
  000c07b6: ldr.w r0,[r10,#0x160]
  000c07ba: cmp r0,#0x3
  000c07bc: itt ge
  000c07be: mov.ge r0,#0x2
  000c07c0: str.w.ge r0,[r10,#0x160]
  000c07c4: movs r0,#0x1
  000c07c6: b 0x000c07ca
  000c07c8: movs r0,#0x0
  000c07ca: str r0,[sp,#0x5c]
  000c07cc: ldr r0,[r5,#0x0]
  000c07ce: blx 0x00071770
  000c07d2: vldr.32 s18,[pc,#-0xf8]
  000c07d6: cmp r0,#0x65
  000c07d8: str r4,[sp,#0x74]
  000c07da: blt 0x000c0808
  000c07dc: ldr r0,[r5,#0x0]
  000c07de: blx 0x00071770
  000c07e2: cmp r0,#0x90
  000c07e4: bgt 0x000c0808
  000c07e6: ldr r0,[r5,#0x0]
  000c07e8: blx 0x00071770
  000c07ec: vmov s0,r0
  000c07f0: vldr.32 s2,[pc,#0x284]
  000c07f4: vmov.f32 s18,0x40a00000
  000c07f8: vcvt.f32.s32 s0,s0
  000c07fc: vdiv.f32 s0,s0,s2
  000c0800: vmov.f32 s2,0x41700000
  000c0804: vmla.f32 s18,s0,s2
  000c0808: ldr r0,[r5,#0x0]
  000c080a: vcvt.s32.f32 s0,s18
  000c080e: vmov r4,s0
  000c0812: blx 0x0007192c
  000c0816: lsls r4,r0
  000c0818: cmp r4,#0x1
  000c081a: blt 0x000c0844
  000c081c: ldr.w r0,[r8,#0x0]
  000c0820: movs r1,#0x64
  000c0822: blx 0x00071848
  000c0826: cmp r0,r4
  000c0828: bge 0x000c0844
  000c082a: ldr.w r0,[r8,#0x0]
  000c082e: movs r1,#0x3
  000c0830: blx 0x00071848
  000c0834: ldr r1,[r5,#0x0]
  000c0836: adds r4,r0,#0x2
  000c0838: mov r0,r1
  000c083a: blx 0x0007192c
  000c083e: lsl.w r0,r4,r0
  000c0842: b 0x000c0846
  000c0844: movs r0,#0x0
  000c0846: ldr r4,[sp,#0x78]
  000c0848: str r0,[sp,#0x2c]
  000c084a: cbz r4,0x000c089c
  000c084c: mov r0,r4
  000c084e: blx 0x00072874
  000c0852: cmp r0,#0xd
  000c0854: bne 0x000c089c
  000c0856: ldr r0,[r5,#0x0]
  000c0858: ldrb.w r0,[r0,#0xf1]
  000c085c: cbnz r0,0x000c089c
  000c085e: mov r0,r4
  000c0860: blx 0x00073738
  000c0864: mov r4,r0
  000c0866: ldr r0,[r5,#0x0]
  000c0868: blx 0x00071c14
  000c086c: blx 0x00071824
  000c0870: cmp r4,r0
  000c0872: bne 0x000c089c
  000c0874: movs r1,#0x0
  000c0876: movs r0,#0x0
  000c0878: strd r1,r1,[r10,#0x164]
  000c087c: movs r6,#0x0
  000c087e: str.w r1,[r10,#0x16c]
  000c0882: str r0,[sp,#0x54]
  000c0884: ldr r0,[r5,#0x0]
  000c0886: ldrb.w r1,[r0,#0xf0]
  000c088a: movs r0,#0x6
  000c088c: cmp r1,#0x0
  000c088e: clz r1,r1
  000c0892: it eq
  000c0894: mov.eq r0,#0x7
  000c0896: lsrs r1,r1,#0x5
  000c0898: str r1,[sp,#0x1c]
  000c089a: b 0x000c08d4
  000c089c: ldr r0,[r5,#0x0]
  000c089e: blx 0x00071c14
  000c08a2: blx 0x00073f24
  000c08a6: ldr.w r1,[r10,#0x160]
  000c08aa: cbz r0,0x000c08be
  000c08ac: cmp r1,#0x7
  000c08ae: it le
  000c08b0: mov.le r1,#0x7
  000c08b2: movw r0,#0x101
  000c08b6: str.w r1,[r10,#0x160]
  000c08ba: strh.w r0,[r10,#0x189]
  000c08be: add.w r3,r10,#0x164
  000c08c2: ldmia r3,{r0,r2,r3}
  000c08c4: add r0,r1
  000c08c6: movs r1,#0x0
  000c08c8: add r0,r2
  000c08ca: str r1,[sp,#0x1c]
  000c08cc: add r0,r3
  000c08ce: cmn r0,r6
  000c08d0: bne 0x000c08d8
  000c08d2: movs r0,#0x4
  000c08d4: str.w r0,[r10,#0x160]
  000c08d8: ldr r0,[r5,#0x0]
  000c08da: blx 0x00071c14
  000c08de: blx 0x00071824
  000c08e2: sub.w r1,r0,#0x64
  000c08e6: cmp r1,#0x8
  000c08e8: bhi 0x000c08fe
  000c08ea: movs r2,#0x1
  000c08ec: lsl.w r1,r2,r1
  000c08f0: tst r1,#0x1c
  000c08f4: bne 0x000c090e
  000c08f6: movw r2,#0x103
  000c08fa: tst r1,r2
  000c08fc: bne 0x000c0902
  000c08fe: cmp r0,#0xa
  000c0900: bne 0x000c0932
  000c0902: movs r0,#0x0
  000c0904: movs r1,#0x0
  000c0906: str.w r0,[r10,#0x160]
  000c090a: str r1,[sp,#0x54]
  000c090c: b 0x000c0928
  000c090e: movs r0,#0x0
  000c0910: movs r1,#0x5
  000c0912: str.w r0,[r10,#0x160]
  000c0916: ldr.w r0,[r8,#0x0]
  000c091a: blx 0x00071848
  000c091e: adds r0,#0x3
  000c0920: movs r1,#0x1
  000c0922: str r1,[sp,#0x54]
  000c0924: movs r1,#0x8
  000c0926: str r1,[sp,#0x44]
  000c0928: movs r1,#0x0
  000c092a: strd r1,r1,[r10,#0x164]
  000c092e: str.w r0,[r10,#0x16c]
  000c0932: ldr r0,[r5,#0x0]
  000c0934: blx 0x00071770
  000c0938: cmp r0,#0x24
  000c093a: beq 0x000c0946
  000c093c: ldr r0,[r5,#0x0]
  000c093e: blx 0x00071770
  000c0942: cmp r0,#0x25
  000c0944: bne 0x000c096a
  000c0946: ldr r0,[r5,#0x0]
  000c0948: blx 0x00071a10
  000c094c: cbz r0,0x000c096a
  000c094e: ldr r0,[r5,#0x0]
  000c0950: blx 0x00071a10
  000c0954: blx 0x00071a7c
  000c0958: cmp r0,#0x5
  000c095a: bne 0x000c096a
  000c095c: movs r1,#0x0
  000c095e: movs r0,#0x0
  000c0960: str.w r1,[r10,#0x16c]
  000c0964: str r0,[sp,#0x54]
  000c0966: str.w r1,[r10,#0x160]
  000c096a: ldr r0,[r5,#0x0]
  000c096c: blx 0x00071770
  000c0970: cmp r0,#0x2a
  000c0972: blt 0x000c0992
  000c0974: ldr r0,[r5,#0x0]
  000c0976: blx 0x00071770
  000c097a: cmp r0,#0x2b
  000c097c: bgt 0x000c0992
  000c097e: ldr r0,[r5,#0x0]
  000c0980: blx 0x000723d0
  000c0984: cbnz r0,0x000c0992
  000c0986: movs r1,#0x0
  000c0988: movs r0,#0x0
  000c098a: movs r6,#0x0
  000c098c: str r0,[sp,#0x54]
  000c098e: str.w r1,[r10,#0x16c]
  000c0992: ldr r0,[r5,#0x0]
  000c0994: blx 0x00071998
  000c0998: cbz r0,0x000c09de
  000c099a: movs r6,#0x0
  000c099c: movs r1,#0x4
  000c099e: str.w r6,[r10,#0x160]
  000c09a2: ldr.w r0,[r8,#0x0]
  000c09a6: blx 0x00071848
  000c09aa: adds r0,#0x6
  000c09ac: str.w r0,[r10,#0x16c]
  000c09b0: ldr r0,[r5,#0x0]
  000c09b2: blx 0x0007192c
  000c09b6: cbz r0,0x000c09ce
  000c09b8: ldr.w r0,[r8,#0x0]
  000c09bc: movs r1,#0x3
  000c09be: ldr.w r4,[r10,#0x16c]
  000c09c2: blx 0x00071848
  000c09c6: add.w r0,r0,r4, lsl #0x1
  000c09ca: str.w r0,[r10,#0x16c]
  000c09ce: ldr r0,[sp,#0x48]
  000c09d0: str r6,[r0,#0x0]
  000c09d2: ldr r0,[sp,#0x4c]
  000c09d4: str r6,[r0,#0x0]
  000c09d6: movs r0,#0x1
  000c09d8: str r0,[sp,#0x54]
  000c09da: movs r0,#0x8
  000c09dc: str r0,[sp,#0x44]
  000c09de: ldr r0,[r5,#0x0]
  000c09e0: blx 0x00073f30
  000c09e4: cbz r0,0x000c0a2a
  000c09e6: movs r6,#0x0
  000c09e8: movs r1,#0x4
  000c09ea: str.w r6,[r10,#0x160]
  000c09ee: ldr.w r0,[r8,#0x0]
  000c09f2: blx 0x00071848
  000c09f6: adds r0,#0xa
  000c09f8: str.w r0,[r10,#0x16c]
  000c09fc: ldr r0,[r5,#0x0]
  000c09fe: blx 0x0007192c
  000c0a02: cbz r0,0x000c0a1a
  000c0a04: ldr.w r0,[r8,#0x0]
  000c0a08: movs r1,#0x3
  000c0a0a: ldr.w r4,[r10,#0x16c]
  000c0a0e: blx 0x00071848
  000c0a12: add.w r0,r0,r4, lsl #0x1
  000c0a16: str.w r0,[r10,#0x16c]
  000c0a1a: ldr r0,[sp,#0x48]
  000c0a1c: str r6,[r0,#0x0]
  000c0a1e: ldr r0,[sp,#0x4c]
  000c0a20: str r6,[r0,#0x0]
  000c0a22: movs r0,#0x1
  000c0a24: str r0,[sp,#0x54]
  000c0a26: movs r0,#0x8
  000c0a28: str r0,[sp,#0x44]
  000c0a2a: ldr r0,[r5,#0x0]
  000c0a2c: blx 0x00071c14
  000c0a30: blx 0x00071824
  000c0a34: mov r1,r0
  000c0a36: ldr r0,[r5,#0x0]
  000c0a38: cmp r1,#0x6c
  000c0a3a: bne 0x000c0a50
  000c0a3c: ldr.w r1,[r0,#0x114]
  000c0a40: cmp r1,#0x0
  000c0a42: beq.w 0x000c0f3e
  000c0a46: movs r2,#0x0
  000c0a48: movs r1,#0x0
  000c0a4a: str r1,[sp,#0x54]
  000c0a4c: str.w r2,[r10,#0x16c]
  000c0a50: blx 0x0007285c
  000c0a54: cmp r0,#0x0
  000c0a56: str r6,[sp,#0x20]
  000c0a58: beq 0x000c0aa0
  000c0a5a: ldr r0,[r5,#0x0]
  000c0a5c: blx 0x0007285c
  000c0a60: blx 0x00072874
  000c0a64: cmp r0,#0xa3
  000c0a66: bne 0x000c0aa0
  000c0a68: ldr r0,[r5,#0x0]
  000c0a6a: ldr.w r1,[r0,#0x90]
  000c0a6e: cbz r1,0x000c0aa0
  000c0a70: movs r4,#0x0
  000c0a72: b 0x000c0a9a
  000c0a7c: ldr r1,[r1,#0x4]
  000c0a7e: ldr.w r5,[r1,r4,lsl #0x2]
  000c0a82: blx 0x00071c14
  000c0a86: blx 0x00071824
  000c0a8a: cmp r5,r0
  000c0a8c: beq.w 0x000c14aa
  000c0a90: ldr r0,[sp,#0x6c]
  000c0a92: adds r4,#0x1
  000c0a94: ldr r0,[r0,#0x0]
  000c0a96: ldr.w r1,[r0,#0x90]
  000c0a9a: ldr r2,[r1,#0x0]
  000c0a9c: cmp r4,r2
  000c0a9e: bcc 0x000c0a7c
  000c0aa0: mov r11,r8
  000c0aa2: movs r4,#0x0
  000c0aa4: ldr r0,[0x000c0e68]
  000c0aa6: movw r1,#0x4e20
  000c0aaa: movw r5,#0x4e20
  000c0aae: add r0,pc
  000c0ab0: ldr.w r8,[r0,#0x0]
  000c0ab4: ldr.w r0,[r11,#0x0]
  000c0ab8: blx r8
  000c0aba: ldr.w r1,[r11,#0x0]
  000c0abe: movw r6,#0x2710
  000c0ac2: subs r0,r0,r6
  000c0ac4: str r0,[sp,#0x180]
  000c0ac6: mov r0,r1
  000c0ac8: movw r1,#0x4e20
  000c0acc: blx r8
  000c0ace: ldr.w r1,[r11,#0x0]
  000c0ad2: subs r0,r0,r6
  000c0ad4: str r0,[sp,#0x184]
  000c0ad6: str.w r8,[sp,#0x60]
  000c0ada: mov r0,r1
  000c0adc: movw r1,#0x7530
  000c0ae0: blx r8
  000c0ae2: add r0,r5
  000c0ae4: str r0,[sp,#0x188]
  000c0ae6: movs r0,#0x18
  000c0ae8: blx 0x0006eb24
  000c0aec: add r1,sp,#0x180
  000c0aee: movs r2,#0x3
  000c0af0: str r0,[sp,#0x78]
  000c0af2: blx 0x00073eb8
  000c0af6: ldr r5,[sp,#0x6c]
  000c0af8: cmp r4,#0x0
  000c0afa: beq.w 0x000c0e30
  000c0afe: movs r0,#0x0
  000c0b00: str.w r0,[r10,#0x16c]
  000c0b04: str.w r0,[r10,#0x164]
  000c0b08: movs r0,#0xc
  000c0b0a: blx 0x0006eb24
  000c0b0e: mov r4,r0
  000c0b10: blx 0x00073d08
  000c0b14: ldr.w r0,[r10,#0x160]
  000c0b18: mov r1,r4
  000c0b1a: str.w r4,[r10,#0xf8]
  000c0b1e: adds r0,#0x8
  000c0b20: blx 0x00073d14
  000c0b24: ldr r6,[sp,#0x64]
  000c0b26: movw r1,#0x7530
  000c0b2a: ldr r5,[sp,#0x60]
  000c0b2c: ldr r0,[r6,#0x0]
  000c0b2e: blx r5
  000c0b30: ldr r1,[r6,#0x0]
  000c0b32: ldr r2,[0x000c0e6c]
  000c0b34: adds r4,r0,r2
  000c0b36: mov r0,r1
  000c0b38: movs r1,#0x2
  000c0b3a: blx r5
  000c0b3c: mov.w r1,#0xffffffff
  000c0b40: cmp r0,#0x0
  000c0b42: it eq
  000c0b44: mov.eq r1,#0x1
  000c0b46: ldr r0,[r6,#0x0]
  000c0b48: muls r1,r4
  000c0b4a: str r1,[sp,#0x174]
  000c0b4c: movw r1,#0xbb8
  000c0b50: blx r5
  000c0b52: ldr r1,[r6,#0x0]
  000c0b54: movw r2,#0x1770
  000c0b58: subs r0,r0,r2
  000c0b5a: str r0,[sp,#0x178]
  000c0b5c: mov r0,r1
  000c0b5e: movw r1,#0xc350
  000c0b62: blx r5
  000c0b64: ldr r1,[0x000c0e70]
  000c0b66: add r0,r1
  000c0b68: str r0,[sp,#0x17c]
  000c0b6a: movs r0,#0x18
  000c0b6c: blx 0x0006eb24
  000c0b70: mov r4,r0
  000c0b72: add r1,sp,#0x174
  000c0b74: movs r2,#0x3
  000c0b76: blx 0x00073eb8
  000c0b7a: mov r0,r4
  000c0b7c: str.w r4,[r10,#0x108]
  000c0b80: blx 0x00073f3c
  000c0b84: movs r1,#0x1
  000c0b86: str.w r0,[r10,#0x110]
  000c0b8a: mov.w r8,#0x1
  000c0b8e: blx 0x00073f48
  000c0b92: ldr.w r0,[r10,#0x108]
  000c0b96: blx 0x000728bc
  000c0b9a: movs r6,#0x0
  000c0b9c: movs r3,#0xf
  000c0b9e: strd r0,r8,[sp,#0x0]
  000c0ba2: cmp.w r9,#0x1
  000c0ba6: str r6,[sp,#0x8]
  000c0ba8: it eq
  000c0baa: mov.eq r3,#0xd
  000c0bac: mov r0,r10
  000c0bae: mov r1,r9
  000c0bb0: movs r2,#0x1
  000c0bb2: blx 0x00073e88
  000c0bb6: ldr.w r1,[r10,#0xf8]
  000c0bba: ldr r1,[r1,#0x4]
  000c0bbc: str r0,[r1,#0x0]
  000c0bbe: movs r1,#0x0
  000c0bc0: ldr.w r0,[r10,#0xf8]
  000c0bc4: ldr r0,[r0,#0x4]
  000c0bc6: ldr r0,[r0,#0x0]
  000c0bc8: blx 0x00073f54
  000c0bcc: vldr.32 s0,[sp,#0x174]
  000c0bd0: vldr.32 s2,[sp,#0x178]
  000c0bd4: vldr.32 s4,[sp,#0x17c]
  000c0bd8: vcvt.f32.s32 s0,s0
  000c0bdc: vcvt.f32.s32 s2,s2
  000c0be0: vcvt.f32.s32 s4,s4
  000c0be4: ldr.w r0,[r10,#0xf8]
  000c0be8: ldr r0,[r0,#0x4]
  000c0bea: vmov r1,s0
  000c0bee: vmov r2,s2
  000c0bf2: ldr r0,[r0,#0x0]
  000c0bf4: vmov r3,s4
  000c0bf8: ldr r5,[r0,#0x0]
  000c0bfa: ldr r5,[r5,#0x48]
  000c0bfc: blx r5
  000c0bfe: ldr.w r0,[r10,#0xf8]
  000c0c02: ldr r0,[r0,#0x4]
  000c0c04: ldr r0,[r0,#0x0]
  000c0c06: ldr r0,[r0,#0x50]
  000c0c08: cbz r0,0x000c0c50
  000c0c0a: ldr r0,[sp,#0x64]
  000c0c0c: movs r1,#0x6
  000c0c0e: ldr r0,[r0,#0x0]
  000c0c10: blx 0x00071848
  000c0c14: subs r1,r0,#0x3
  000c0c16: cmp r1,#0x3
  000c0c18: bcs 0x000c0c22
  000c0c1a: ldr r0,[0x000c0e74]
  000c0c1c: add r0,pc
  000c0c1e: ldr.w r0,[r0,r1,lsl #0x2]
  000c0c22: ldr.w r1,[r10,#0xf8]
  000c0c26: ldr r1,[r1,#0x4]
  000c0c28: ldr r1,[r1,#0x0]
  000c0c2a: ldr r2,[0x000c0e78]
  000c0c2c: ldr r1,[r1,#0x50]
  000c0c2e: add r2,pc
  000c0c30: ldr r2,[r2,#0x0]
  000c0c32: ldr r1,[r1,#0x4]
  000c0c34: str r0,[r1,#0x0]
  000c0c36: mov.w r1,#0x680
  000c0c3a: ldr r0,[r2,#0x0]
  000c0c3c: blx 0x00072f70
  000c0c40: mov r1,r0
  000c0c42: ldr.w r0,[r10,#0xf8]
  000c0c46: ldr r0,[r0,#0x4]
  000c0c48: ldr r0,[r0,#0x0]
  000c0c4a: adds r0,#0x18
  000c0c4c: blx 0x0006f2b0
  000c0c50: ldr r0,[0x000c0e7c]
  000c0c52: movs r5,#0x1
  000c0c54: add r0,pc
  000c0c56: ldr.w r11,[r0,#0x0]
  000c0c5a: b 0x000c0ca8
  000c0c5c: ldr.w r0,[r11,#0x0]
  000c0c60: mov r1,r9
  000c0c62: blx 0x00071a34
  000c0c66: mov r4,r0
  000c0c68: ldr.w r0,[r10,#0x108]
  000c0c6c: blx 0x000728bc
  000c0c70: strd r0,r8,[sp,#0x0]
  000c0c74: mov r0,r10
  000c0c76: mov r1,r9
  000c0c78: movs r2,#0x0
  000c0c7a: mov r3,r4
  000c0c7c: str r6,[sp,#0x8]
  000c0c7e: blx 0x00073e88
  000c0c82: ldr.w r1,[r10,#0xf8]
  000c0c86: ldr r1,[r1,#0x4]
  000c0c88: str.w r0,[r1,r5,lsl #0x2]
  000c0c8c: ldr.w r1,[r10,#0xf8]
  000c0c90: ldr.w r0,[r10,#0x110]
  000c0c94: ldr r1,[r1,#0x4]
  000c0c96: ldr.w r4,[r1,r5,lsl #0x2]
  000c0c9a: blx 0x00073f3c
  000c0c9e: mov r1,r0
  000c0ca0: mov r0,r4
  000c0ca2: blx 0x00073f60
  000c0ca6: adds r5,#0x1
  000c0ca8: cmp r5,#0x6
  000c0caa: bne 0x000c0c5c
  000c0cac: adr r0,[0xc0e80]
  000c0cae: vmov.i32 q8,#0x0
  000c0cb2: add r1,sp,#0x138
  000c0cb4: vld1.64 {d18,d19},[r0]
  000c0cb8: orr r0,r1,#0x4
  000c0cbc: movs r5,#0x0
  000c0cbe: vst1.32 {d16,d17},[r0]
  000c0cc2: add.w r0,r1,#0x18
  000c0cc6: vst1.64 {d16,d17},[r0]
  000c0cca: add.w r0,r1,#0x28
  000c0cce: vst1.64 {d18,d19},[r0]
  000c0cd2: mov.w r0,#0x3f800000
  000c0cd6: str r0,[sp,#0x138]
  000c0cd8: str r0,[sp,#0x14c]
  000c0cda: str r0,[sp,#0x170]
  000c0cdc: strd r5,r5,[sp,#0xe0]
  000c0ce0: str r5,[sp,#0xe8]
  000c0ce2: ldr.w r0,[r10,#0xf8]
  000c0ce6: ldr r0,[r0,#0x4]
  000c0ce8: ldr r1,[r0,#0x0]
  000c0cea: ldr r0,[r1,#0x0]
  000c0cec: ldr r2,[r0,#0x28]
  000c0cee: add r0,sp,#0xd4
  000c0cf0: blx r2
  000c0cf2: ldr r0,[0x000c0e90]
  000c0cf4: rsb r1,r9,r9, lsl #0x3
  000c0cf8: movs r6,#0x18
  000c0cfa: add r0,pc
  000c0cfc: add.w r0,r0,r1, lsl #0x3
  000c0d00: str r0,[sp,#0x68]
  000c0d02: b 0x000c0dda
  000c0d04: ldr r4,[sp,#0x7c]
  000c0d06: movs r1,#0x0
  000c0d08: movw r2,#0x381b
  000c0d0c: movs r3,#0x1
  000c0d0e: mov r0,r4
  000c0d10: blx 0x00073f6c
  000c0d14: ldr.w r1,[r4,#0xf8]
  000c0d18: ldr r1,[r1,#0x4]
  000c0d1a: str r0,[r1,r6]
  000c0d1c: mov.w r1,#0x3e8
  000c0d20: ldr.w r0,[r4,#0xf8]
  000c0d24: ldr r0,[r0,#0x4]
  000c0d26: ldr r4,[r0,r6]
  000c0d28: ldr r0,[r4,#0x4]
  000c0d2a: blx 0x00073f78
  000c0d2e: ldr r0,[sp,#0x68]
  000c0d30: add.w r8,sp,#0xc8
  000c0d34: str r6,[sp,#0x74]
  000c0d36: add.w r10,sp,#0x138
  000c0d3a: add r0,r5
  000c0d3c: ldrd r6,r9,[r0,#0x4]
  000c0d40: str r5,[sp,#0x6c]
  000c0d42: mov r5,r11
  000c0d44: ldrd r11,r1,[r0,#0xc]
  000c0d48: ldrd r2,r0,[r0,#0x14]
  000c0d4c: strd r1,r2,[sp,#0xbc]
  000c0d50: add r2,sp,#0xbc
  000c0d52: str r0,[sp,#0xc4]
  000c0d54: mov r0,r8
  000c0d56: mov r1,r10
  000c0d58: blx 0x0006f694
  000c0d5c: add.w r10,sp,#0xe0
  000c0d60: mov r1,r8
  000c0d62: mov r0,r10
  000c0d64: blx 0x0006eb3c
  000c0d68: ldr r0,[r4,#0x8]
  000c0d6a: mov r1,r10
  000c0d6c: blx 0x00073dd4
  000c0d70: add r1,sp,#0x138
  000c0d72: add r2,sp,#0xbc
  000c0d74: strd r6,r9,[sp,#0xbc]
  000c0d78: mov r0,r8
  000c0d7a: str.w r11,[sp,#0xc4]
  000c0d7e: mov r11,r5
  000c0d80: ldr r6,[sp,#0x74]
  000c0d82: ldr r5,[sp,#0x6c]
  000c0d84: blx 0x0006f694
  000c0d88: mov r0,r10
  000c0d8a: mov r1,r8
  000c0d8c: blx 0x0006eb3c
  000c0d90: vldr.32 s0,[sp,#0xe0]
  000c0d94: mov r1,r8
  000c0d96: vldr.32 s6,[sp,#0xd4]
  000c0d9a: vldr.32 s2,[sp,#0xe4]
  000c0d9e: vldr.32 s8,[sp,#0xd8]
  000c0da2: vadd.f32 s0,s6,s0
  000c0da6: vldr.32 s4,[sp,#0xe8]
  000c0daa: vldr.32 s10,[sp,#0xdc]
  000c0dae: vadd.f32 s2,s8,s2
  000c0db2: vadd.f32 s4,s10,s4
  000c0db6: vstr.32 s0,[sp,#0xc8]
  000c0dba: vstr.32 s2,[sp,#0xcc]
  000c0dbe: vstr.32 s4,[sp,#0xd0]
  000c0dc2: ldr r0,[r4,#0x0]
  000c0dc4: ldr r2,[r0,#0x44]
  000c0dc6: mov r0,r4
  000c0dc8: blx r2
  000c0dca: ldr r1,[0x000c0e94]
  000c0dcc: mov r0,r4
  000c0dce: blx 0x00073f84
  000c0dd2: ldr r0,[sp,#0x70]
  000c0dd4: adds r6,#0x4
  000c0dd6: str r0,[r4,#0x28]
  000c0dd8: adds r5,#0x1c
  000c0dda: cmp r5,#0x38
  000c0ddc: bne 0x000c0d04
  000c0dde: ldr.w r10,[sp,#0x7c]
  000c0de2: movs r5,#0x8
  000c0de4: ldr r6,[sp,#0x70]
  000c0de6: mov.w r8,#0x0
  000c0dea: mov.w r9,#0x1
  000c0dee: b 0x000c0e22
  000c0df0: ldr.w r0,[r11,#0x0]
  000c0df4: mov r1,r6
  000c0df6: blx 0x00071a34
  000c0dfa: mov r4,r0
  000c0dfc: ldr r0,[sp,#0x78]
  000c0dfe: blx 0x000728bc
  000c0e02: strd r0,r9,[sp,#0x0]
  000c0e06: mov r0,r10
  000c0e08: mov r1,r6
  000c0e0a: movs r2,#0x0
  000c0e0c: mov r3,r4
  000c0e0e: str.w r8,[sp,#0x8]
  000c0e12: blx 0x00073e88
  000c0e16: ldr.w r1,[r10,#0xf8]
  000c0e1a: ldr r1,[r1,#0x4]
  000c0e1c: str.w r0,[r1,r5,lsl #0x2]
  000c0e20: adds r5,#0x1
  000c0e22: ldr.w r0,[r10,#0x160]
  000c0e26: adds r0,#0x8
  000c0e28: cmp r5,r0
  000c0e2a: blt 0x000c0df0
  000c0e2c: b.w 0x000c02ca
  000c0e30: ldr r0,[r5,#0x0]
  000c0e32: blx 0x0007285c
  000c0e36: cbz r0,0x000c0e98
  000c0e38: ldr r0,[r5,#0x0]
  000c0e3a: blx 0x0007285c
  000c0e3e: blx 0x00072874
  000c0e42: cmp r0,#0xa7
  000c0e44: bne 0x000c0e98
  000c0e46: ldr r0,[r5,#0x0]
  000c0e48: blx 0x00071c14
  000c0e4c: blx 0x00071824
  000c0e50: mov r4,r0
  000c0e52: ldr r0,[r5,#0x0]
  000c0e54: blx 0x0007285c
  000c0e58: blx 0x00073738
  000c0e5c: subs r0,r4,r0
  000c0e5e: clz r0,r0
  000c0e62: lsrs r0,r0,#0x5
  000c0e64: b 0x000c0e9a
  000c0e98: movs r0,#0x0
  000c0e9a: str r0,[sp,#0x18]
  000c0e9c: cmp.w r9,#0x0
  000c0ea0: ldr r0,[sp,#0x48]
  000c0ea2: ldr.w r8,[sp,#0x74]
  000c0ea6: ldr r4,[sp,#0x64]
  000c0ea8: ldr r0,[r0,#0x0]
  000c0eaa: bne 0x000c0eec
  000c0eac: cmp r0,#0x0
  000c0eae: ble 0x000c0eec
  000c0eb0: ldr r0,[r4,#0x0]
  000c0eb2: movs r1,#0x64
  000c0eb4: blx 0x00071848
  000c0eb8: ldr r6,[sp,#0x5c]
  000c0eba: cmp r0,#0x1d
  000c0ebc: bgt.w 0x000c14a0
  000c0ec0: ldr r0,[r4,#0x0]
  000c0ec2: movs r1,#0x64
  000c0ec4: blx 0x00071848
  000c0ec8: movs r1,#0x0
  000c0eca: cmp r0,#0x1d
  000c0ecc: str r1,[sp,#0x38]
  000c0ece: bgt.w 0x000c29e0
  000c0ed2: ldr r0,[r5,#0x0]
  000c0ed4: blx 0x00071770
  000c0ed8: cmp r0,#0x67
  000c0eda: mov.w r0,#0x0
  000c0ede: it gt
  000c0ee0: mov.gt r0,#0x1
  000c0ee2: str r0,[sp,#0x40]
  000c0ee4: movs r0,#0x1
  000c0ee6: str r0,[sp,#0x3c]
  000c0ee8: b.w 0x000c29ee
  000c0eec: movs r1,#0x0
  000c0eee: cmp.w r9,#0x1
  000c0ef2: str r1,[sp,#0x38]
  000c0ef4: bne.w 0x000c29e4
  000c0ef8: ldr r6,[sp,#0x5c]
  000c0efa: cmp r0,#0x1
  000c0efc: mov.w r0,#0x0
  000c0f00: str r0,[sp,#0x3c]
  000c0f02: mov.w r0,#0x0
  000c0f06: str r0,[sp,#0x40]
  000c0f08: blt.w 0x000c29ee
  000c0f0c: ldr r0,[r4,#0x0]
  000c0f0e: movs r1,#0x64
  000c0f10: blx 0x00071848
  000c0f14: cmp r0,#0x1d
  000c0f16: mov.w r0,#0x0
  000c0f1a: mov.w r1,#0x0
  000c0f1e: str r0,[sp,#0x3c]
  000c0f20: mov.w r0,#0x0
  000c0f24: str r1,[sp,#0x38]
  000c0f26: str r0,[sp,#0x40]
  000c0f28: bgt.w 0x000c29ee
  000c0f2c: ldr r0,[r5,#0x0]
  000c0f2e: blx 0x00071770
  000c0f32: cmp r0,#0x8c
  000c0f34: mov.w r0,#0x0
  000c0f38: it gt
  000c0f3a: mov.gt r0,#0x1
  000c0f3c: b 0x000c14a2
  000c0f3e: movs r4,#0x0
  000c0f40: movs r1,#0x1
  000c0f42: str.w r4,[r10,#0x160]
  000c0f46: strb.w r1,[r10,#0x288]
  000c0f4a: blx 0x0007192c
  000c0f4e: movs r1,#0x6
  000c0f50: cmp r0,#0x0
  000c0f52: it ne
  000c0f54: mov.ne r1,#0x8
  000c0f56: movs r0,#0xc
  000c0f58: strd r4,r4,[r10,#0x164]
  000c0f5c: str.w r1,[r10,#0x16c]
  000c0f60: blx 0x0006eb24
  000c0f64: mov r4,r0
  000c0f66: blx 0x00073d08
  000c0f6a: ldr.w r0,[r10,#0x16c]
  000c0f6e: mov r1,r4
  000c0f70: str.w r4,[r10,#0xf8]
  000c0f74: adds r0,#0x4
  000c0f76: blx 0x00073d14
  000c0f7a: ldr r0,[0x000c1124]
  000c0f7c: movs r4,#0x0
  000c0f7e: add r0,pc
  000c0f80: ldr.w r8,[r0,#0x0]
  000c0f84: b 0x000c0fbe
  000c0f86: mov r0,r10
  000c0f88: movs r1,#0x0
  000c0f8a: movw r2,#0x37a3
  000c0f8e: movs r3,#0x1
  000c0f90: blx 0x00073f6c
  000c0f94: ldr.w r1,[r10,#0xf8]
  000c0f98: ldr r1,[r1,#0x4]
  000c0f9a: str.w r0,[r1,r4,lsl #0x2]
  000c0f9e: movw r1,#0x1b9
  000c0fa2: ldr.w r0,[r8,#0x0]
  000c0fa6: blx 0x00072f70
  000c0faa: mov r1,r0
  000c0fac: ldr.w r0,[r10,#0xf8]
  000c0fb0: ldr r0,[r0,#0x4]
  000c0fb2: ldr.w r0,[r0,r4,lsl #0x2]
  000c0fb6: adds r0,#0x18
  000c0fb8: blx 0x0006f2b0
  000c0fbc: adds r4,#0x1
  000c0fbe: cmp r4,#0x4
  000c0fc0: bne 0x000c0f86
  000c0fc2: ldr.w r0,[r10,#0xf8]
  000c0fc6: ldr r5,[0x000c1128]
  000c0fc8: ldr r4,[0x000c112c]
  000c0fca: ldr r0,[r0,#0x4]
  000c0fcc: ldr r2,[0x000c1130]
  000c0fce: mov r3,r5
  000c0fd0: ldr r0,[r0,#0x0]
  000c0fd2: ldr r1,[r0,#0x0]
  000c0fd4: ldr r6,[r1,#0x48]
  000c0fd6: mov r1,r4
  000c0fd8: blx r6
  000c0fda: ldr.w r0,[r10,#0xf8]
  000c0fde: ldr r1,[0x000c1134]
  000c0fe0: ldr r2,[0x000c1138]
  000c0fe2: ldr r0,[r0,#0x4]
  000c0fe4: ldr r0,[r0,#0x4]
  000c0fe6: ldr r3,[r0,#0x0]
  000c0fe8: ldr r6,[r3,#0x48]
  000c0fea: ldr r3,[0x000c113c]
  000c0fec: blx r6
  000c0fee: ldr.w r0,[r10,#0xf8]
  000c0ff2: mov r2,r4
  000c0ff4: mov r3,r5
  000c0ff6: ldr r0,[r0,#0x4]
  000c0ff8: ldr r0,[r0,#0x8]
  000c0ffa: ldr r1,[r0,#0x0]
  000c0ffc: ldr r6,[r1,#0x48]
  000c0ffe: ldr r1,[0x000c1140]
  000c1000: blx r6
  000c1002: ldr.w r0,[r10,#0xf8]
  000c1006: ldr r1,[0x000c1144]
  000c1008: ldr r2,[0x000c1148]
  000c100a: ldr r0,[r0,#0x4]
  000c100c: ldr r0,[r0,#0xc]
  000c100e: ldr r3,[r0,#0x0]
  000c1010: ldr r6,[r3,#0x48]
  000c1012: ldr r3,[0x000c114c]
  000c1014: blx r6
  000c1016: ldr r0,[0x000c1150]
  000c1018: movs r4,#0x4
  000c101a: movs r6,#0x0
  000c101c: mov.w r9,#0x1
  000c1020: add r0,pc
  000c1022: ldr r5,[r0,#0x0]
  000c1024: b 0x000c104c
  000c1026: ldr r0,[r5,#0x0]
  000c1028: movs r1,#0x8
  000c102a: blx 0x00071a34
  000c102e: mov r3,r0
  000c1030: mov r0,r10
  000c1032: movs r1,#0x8
  000c1034: movs r2,#0x0
  000c1036: strd r6,r9,[sp,#0x0]
  000c103a: str r6,[sp,#0x8]
  000c103c: blx 0x00073e88
  000c1040: ldr.w r1,[r10,#0xf8]
  000c1044: ldr r1,[r1,#0x4]
  000c1046: str.w r0,[r1,r4,lsl #0x2]
  000c104a: adds r4,#0x1
  000c104c: ldr.w r0,[r10,#0xf8]
  000c1050: ldr r0,[r0,#0x0]
  000c1052: cmp r4,r0
  000c1054: bcc 0x000c1026
  000c1056: movs r0,#0x78
  000c1058: blx 0x0006eb24
  000c105c: mov r4,r0
  000c105e: ldr.w r0,[r8,#0x0]
  000c1062: movw r1,#0x641
  000c1066: blx 0x00072f70
  000c106a: mov r1,r0
  000c106c: add r0,sp,#0xec
  000c106e: movs r2,#0x0
  000c1070: movs r5,#0x0
  000c1072: blx 0x0006f028
  000c1076: ldr r3,[0x000c1154]
  000c1078: add r3,pc
  000c107a: movs r1,#0x4
  000c107c: movs r0,#0x6c
  000c107e: add r2,sp,#0xec
  000c1080: strd r1,r5,[sp,#0x0]
  000c1084: strd r0,r5,[sp,#0x8]
  000c1088: mov r0,r4
  000c108a: movs r1,#0x4
  000c108c: blx 0x00071c80
  000c1090: add r0,sp,#0xec
  000c1092: blx 0x0006ee30
  000c1096: ldr r5,[sp,#0x6c]
  000c1098: mov r1,r4
  000c109a: ldr r0,[r5,#0x0]
  000c109c: blx 0x00073f90
  000c10a0: ldr r0,[r5,#0x0]
  000c10a2: mov r1,r4
  000c10a4: blx 0x00073c3c
  000c10a8: movs r0,#0x1c
  000c10aa: blx 0x0006eb24
  000c10ae: mov r4,r0
  000c10b0: ldr.w r0,[r10,#0xf8]
  000c10b4: ldr r3,[r0,#0x0]
  000c10b6: mov r0,r4
  000c10b8: movs r1,#0x12
  000c10ba: movs r2,#0x0
  000c10bc: str.w r10,[sp,#0x0]
  000c10c0: blx 0x00073f9c
  000c10c4: b 0x000c16cc
  000c10c6: cmp r0,#0xb8
  000c10c8: beq.w 0x000c2700
  000c10cc: cmp r0,#0xb7
  000c10ce: bne.w 0x000c02ca
  000c10d2: ldr r0,[0x000c1158]
  000c10d4: add r5,sp,#0x110
  000c10d6: add r0,pc
  000c10d8: vld1.64 {d16,d17},[r0]!
  000c10dc: vldr.64 d18,[r0]
  000c10e0: mov r0,r5
  000c10e2: vst1.64 {d16,d17},[r0]!
  000c10e6: vstr.64 d18,[r0]
  000c10ea: movs r0,#0x18
  000c10ec: blx 0x0006eb24
  000c10f0: mov r6,r0
  000c10f2: mov r1,r5
  000c10f4: movs r2,#0x6
  000c10f6: blx 0x00073eb8
  000c10fa: ldr r4,[sp,#0x7c]
  000c10fc: movs r0,#0xc
  000c10fe: str.w r6,[r4,#0x110]
  000c1102: blx 0x0006eb24
  000c1106: mov r5,r0
  000c1108: blx 0x00073d08
  000c110c: movs r0,#0xe
  000c110e: mov r1,r5
  000c1110: str.w r5,[r4,#0xf8]
  000c1114: blx 0x00073d14
  000c1118: mov.w r8,#0x0
  000c111c: movs r5,#0x1
  000c111e: movs r6,#0x0
  000c1120: b 0x000c11a6
  000c115c: ldr.w r0,[r4,#0x110]
  000c1160: movs r1,#0x0
  000c1162: blx 0x00073ec4
  000c1166: stm sp,{r0,r5,r8}
  000c116a: mov r0,r4
  000c116c: movs r1,#0xa
  000c116e: movs r2,#0x0
  000c1170: movs r3,#0x2c
  000c1172: blx 0x00073e88
  000c1176: ldr.w r1,[r4,#0xf8]
  000c117a: ldr r1,[r1,#0x4]
  000c117c: str.w r0,[r1,r6,lsl #0x2]
  000c1180: movs r1,#0x1
  000c1182: ldr.w r0,[r4,#0xf8]
  000c1186: ldr r0,[r0,#0x4]
  000c1188: ldr.w r0,[r0,r6,lsl #0x2]
  000c118c: ldr r0,[r0,#0x4]
  000c118e: blx 0x00073e94
  000c1192: ldr.w r0,[r4,#0xf8]
  000c1196: movs r1,#0xfa
  000c1198: ldr r0,[r0,#0x4]
  000c119a: ldr.w r0,[r0,r6,lsl #0x2]
  000c119e: ldr r0,[r0,#0x4]
  000c11a0: blx 0x00073f78
  000c11a4: adds r6,#0x1
  000c11a6: cmp r6,#0x8
  000c11a8: bne 0x000c115c
  000c11aa: mov r0,r4
  000c11ac: movs r1,#0x0
  000c11ae: movw r2,#0x495d
  000c11b2: movs r3,#0x0
  000c11b4: mov.w r8,#0x0
  000c11b8: blx 0x00073f6c
  000c11bc: ldr.w r1,[r4,#0xf8]
  000c11c0: ldr r1,[r1,#0x4]
  000c11c2: str r0,[r1,#0x20]
  000c11c4: movs r1,#0x0
  000c11c6: ldr.w r0,[r4,#0xf8]
  000c11ca: ldr r0,[r0,#0x4]
  000c11cc: ldr r0,[r0,#0x20]
  000c11ce: blx 0x00073f54
  000c11d2: ldr.w r0,[r4,#0xf8]
  000c11d6: movs r1,#0x2
  000c11d8: ldr r0,[r0,#0x4]
  000c11da: ldr r0,[r0,#0x20]
  000c11dc: blx 0x00072748
  000c11e0: ldr r0,[0x000c159c]
  000c11e2: ldr.w r1,[r4,#0xf8]
  000c11e6: add r0,pc
  000c11e8: ldr r0,[r0,#0x0]
  000c11ea: ldr r1,[r1,#0x4]
  000c11ec: ldr r0,[r0,#0x0]
  000c11ee: ldr r6,[r1,#0x20]
  000c11f0: movw r1,#0xc88
  000c11f4: blx 0x00072f70
  000c11f8: add r5,sp,#0x98
  000c11fa: mov r1,r0
  000c11fc: movs r2,#0x0
  000c11fe: mov r0,r5
  000c1200: blx 0x0006f028
  000c1204: mov r0,r6
  000c1206: mov r1,r5
  000c1208: blx 0x00073fa8
  000c120c: add r0,sp,#0x98
  000c120e: blx 0x0006ee30
  000c1212: ldr.w r0,[r4,#0xf8]
  000c1216: movs r1,#0x1
  000c1218: movs r5,#0x1
  000c121a: ldr r0,[r0,#0x4]
  000c121c: ldr r0,[r0,#0x20]
  000c121e: ldr r0,[r0,#0x4]
  000c1220: blx 0x00073fb4
  000c1224: ldr.w r0,[r4,#0xf8]
  000c1228: ldr r1,[0x000c15a0]
  000c122a: ldr r0,[r0,#0x4]
  000c122c: ldr r0,[r0,#0x20]
  000c122e: ldr r0,[r0,#0x8]
  000c1230: strd r8,r1,[sp,#0x138]
  000c1234: add r1,sp,#0x138
  000c1236: str.w r8,[sp,#0x140]
  000c123a: blx 0x00072478
  000c123e: ldr.w r0,[r4,#0xf8]
  000c1242: movs r1,#0xa
  000c1244: ldr r0,[r0,#0x4]
  000c1246: ldr r2,[r0,#0x20]
  000c1248: ldr.w r0,[r2,#0x50]!
  000c124c: cmp r0,#0x0
  000c124e: str.w r1,[r2,#-0x28]
  000c1252: strb.w r5,[r2,#0x24]
  000c1256: beq 0x000c126c
  000c1258: blx 0x00070204
  000c125c: blx 0x0006eb48
  000c1260: ldr.w r0,[r4,#0xf8]
  000c1264: ldr r0,[r0,#0x4]
  000c1266: ldr r0,[r0,#0x20]
  000c1268: add.w r2,r0,#0x50
  000c126c: mov.w r9,#0x0
  000c1270: mov.w r0,#0x138
  000c1274: str.w r9,[r2,#0x0]
  000c1278: blx 0x0006eb24
  000c127c: ldr r1,[0x000c15a4]
  000c127e: mov r6,r0
  000c1280: ldr r2,[0x000c15a8]
  000c1282: ldr r3,[0x000c15ac]
  000c1284: str.w r9,[sp,#0x0]
  000c1288: blx 0x00073e4c
  000c128c: mov r0,r4
  000c128e: mov r1,r6
  000c1290: movw r2,#0x4974
  000c1294: movs r3,#0x1
  000c1296: mov.w r8,#0x1
  000c129a: blx 0x00073f6c
  000c129e: ldr.w r1,[r4,#0xf8]
  000c12a2: add.w r10,sp,#0x138
  000c12a6: ldr r1,[r1,#0x4]
  000c12a8: str r0,[r1,#0x24]
  000c12aa: mov r1,r6
  000c12ac: ldr.w r0,[r4,#0xf8]
  000c12b0: ldr r0,[r0,#0x4]
  000c12b2: ldr r5,[r0,#0x24]
  000c12b4: ldr r0,[r6,#0x0]
  000c12b6: ldr r2,[r0,#0x28]
  000c12b8: mov r0,r10
  000c12ba: blx r2
  000c12bc: ldr r0,[r5,#0x0]
  000c12be: mov r1,r10
  000c12c0: ldr r2,[r0,#0x44]
  000c12c2: mov r0,r5
  000c12c4: blx r2
  000c12c6: ldr.w r0,[r4,#0xf8]
  000c12ca: movs r1,#0x0
  000c12cc: ldr r0,[r0,#0x4]
  000c12ce: ldr r0,[r0,#0x24]
  000c12d0: blx 0x00073f54
  000c12d4: ldr.w r0,[r4,#0xf8]
  000c12d8: movs r1,#0x1
  000c12da: ldr r0,[r0,#0x4]
  000c12dc: ldr r0,[r0,#0x24]
  000c12de: strb.w r9,[r0,#0x70]
  000c12e2: blx 0x00072748
  000c12e6: ldr.w r0,[r4,#0xf8]
  000c12ea: movs r1,#0x1
  000c12ec: ldr r0,[r0,#0x4]
  000c12ee: ldr r0,[r0,#0x24]
  000c12f0: ldr r0,[r0,#0x4]
  000c12f2: blx 0x00073fb4
  000c12f6: ldr.w r0,[r4,#0xf8]
  000c12fa: ldr r1,[0x000c15b0]
  000c12fc: ldr r0,[r0,#0x4]
  000c12fe: ldr r0,[r0,#0x24]
  000c1300: ldr r0,[r0,#0x4]
  000c1302: blx 0x000727e4
  000c1306: ldr.w r0,[r4,#0xf8]
  000c130a: movs r1,#0xa
  000c130c: ldr r0,[r0,#0x4]
  000c130e: ldr r2,[r0,#0x24]
  000c1310: ldr.w r0,[r2,#0x50]!
  000c1314: cmp r0,#0x0
  000c1316: str.w r1,[r2,#-0x28]
  000c131a: strb.w r8,[r2,#0x24]
  000c131e: beq 0x000c1334
  000c1320: blx 0x00070204
  000c1324: blx 0x0006eb48
  000c1328: ldr.w r0,[r4,#0xf8]
  000c132c: ldr r0,[r0,#0x4]
  000c132e: ldr r0,[r0,#0x24]
  000c1330: add.w r2,r0,#0x50
  000c1334: ldr r1,[0x000c15b4]
  000c1336: movs r0,#0x0
  000c1338: str r0,[r2,#0x0]
  000c133a: add r0,sp,#0x138
  000c133c: add r1,pc
  000c133e: movs r2,#0x24
  000c1340: blx 0x0006f1e4
  000c1344: ldr r0,[0x000c15b8]
  000c1346: add r0,pc
  000c1348: vldr.64 d16,[r0]
  000c134c: ldr r0,[r0,#0x8]
  000c134e: str r0,[sp,#0x188]
  000c1350: movs r0,#0xc
  000c1352: vstr.64 d16,[sp,#0x180]
  000c1356: blx 0x0006eb24
  000c135a: mov r5,r0
  000c135c: blx 0x00073d08
  000c1360: movs r0,#0x3
  000c1362: mov r1,r5
  000c1364: blx 0x00073d14
  000c1368: ldr.w r0,[r4,#0xf8]
  000c136c: ldr r1,[r5,#0x4]
  000c136e: ldr r0,[r0,#0x4]
  000c1370: ldr r0,[r0,#0x24]
  000c1372: str r0,[r1,#0x0]
  000c1374: ldr.w r0,[r4,#0xf8]
  000c1378: ldr r1,[r5,#0x4]
  000c137a: ldr r0,[r0,#0x4]
  000c137c: ldr r0,[r0,#0x20]
  000c137e: str r0,[r1,#0x8]
  000c1380: movs r0,#0x18
  000c1382: blx 0x0006eb24
  000c1386: mov r11,r0
  000c1388: movs r0,#0x9
  000c138a: add r1,sp,#0x138
  000c138c: add r3,sp,#0x180
  000c138e: str r0,[sp,#0x0]
  000c1390: mov r0,r11
  000c1392: mov r2,r5
  000c1394: blx 0x00073fc0
  000c1398: mov r0,r11
  000c139a: movs r1,#0x1
  000c139c: blx 0x00073f48
  000c13a0: ldr.w r8,[sp,#0x7c]
  000c13a4: add.w r9,sp,#0x174
  000c13a8: movs r6,#0xa
  000c13aa: mov.w r10,#0x0
  000c13ae: b 0x000c145a
  000c13b0: mov r0,r11
  000c13b2: movs r1,#0x0
  000c13b4: sub.w r5,r6,#0xa
  000c13b8: blx 0x00073ec4
  000c13bc: str r0,[sp,#0x0]
  000c13be: movs r0,#0x1
  000c13c0: strd r0,r10,[sp,#0x4]
  000c13c4: mov r0,r8
  000c13c6: movs r1,#0x0
  000c13c8: movs r2,#0x0
  000c13ca: movs r3,#0x33
  000c13cc: blx 0x00073e88
  000c13d0: ldr.w r1,[r8,#0xf8]
  000c13d4: ldr r1,[r1,#0x4]
  000c13d6: str.w r0,[r1,r6,lsl #0x2]
  000c13da: ldr.w r0,[r8,#0xf8]
  000c13de: ldr r0,[r0,#0x4]
  000c13e0: ldr.w r4,[r0,r6,lsl #0x2]
  000c13e4: mov r0,r11
  000c13e6: blx 0x00073f3c
  000c13ea: mov r1,r0
  000c13ec: mov r0,r4
  000c13ee: blx 0x00073f60
  000c13f2: ldr.w r0,[r8,#0xf8]
  000c13f6: movs r1,#0x1
  000c13f8: ldr r0,[r0,#0x4]
  000c13fa: ldr.w r0,[r0,r6,lsl #0x2]
  000c13fe: ldr r0,[r0,#0x4]
  000c1400: blx 0x00073fb4
  000c1404: ldr.w r0,[r8,#0xf8]
  000c1408: bic r1,r5,#0x80000000
  000c140c: cmp r1,#0x2
  000c140e: ldr r0,[r0,#0x4]
  000c1410: ldr.w r0,[r0,r6,lsl #0x2]
  000c1414: strh.w r10,[r0,#0x13d]
  000c1418: beq 0x000c1438
  000c141a: cmp r1,#0x1
  000c141c: beq 0x000c142c
  000c141e: cbnz r1,0x000c1444
  000c1420: ldr r1,[0x000c15bc]
  000c1422: str r1,[sp,#0x178]
  000c1424: ldr r1,[0x000c15c0]
  000c1426: str r1,[sp,#0x174]
  000c1428: ldr r1,[0x000c15c4]
  000c142a: b 0x000c144e
  000c142c: ldr r1,[0x000c15c8]
  000c142e: str r1,[sp,#0x178]
  000c1430: ldr r1,[0x000c15cc]
  000c1432: str r1,[sp,#0x174]
  000c1434: ldr r1,[0x000c15d0]
  000c1436: b 0x000c144e
  000c1438: ldr r1,[0x000c15d4]
  000c143a: str.w r10,[sp,#0x178]
  000c143e: str r1,[sp,#0x174]
  000c1440: ldr r1,[0x000c15d8]
  000c1442: b 0x000c144e
  000c1444: ldr r1,[0x000c15dc]
  000c1446: str r1,[sp,#0x178]
  000c1448: ldr r1,[0x000c15e0]
  000c144a: str r1,[sp,#0x174]
  000c144c: ldr r1,[0x000c15e4]
  000c144e: str r1,[sp,#0x17c]
  000c1450: ldr r1,[r0,#0x0]
  000c1452: ldr r2,[r1,#0x44]
  000c1454: mov r1,r9
  000c1456: blx r2
  000c1458: adds r6,#0x1
  000c145a: cmp r6,#0xe
  000c145c: bne 0x000c13b0
  000c145e: ldr r6,[sp,#0x7c]
  000c1460: ldr r1,[0x000c15d0]
  000c1462: ldr r2,[0x000c15e8]
  000c1464: ldr.w r0,[r6,#0xf0]
  000c1468: ldr r3,[0x000c15ec]
  000c146a: blx 0x000725bc
  000c146e: add r4,sp,#0x174
  000c1470: ldr.w r1,[r6,#0xf0]
  000c1474: mov r0,r4
  000c1476: blx 0x0007264c
  000c147a: add.w r0,r6,#0xc8
  000c147e: mov r1,r4
  000c1480: blx 0x0006eb3c
  000c1484: ldr r5,[0x000c15f0]
  000c1486: movs r0,#0x1c
  000c1488: str.w r5,[r6,#0x130]
  000c148c: blx 0x0006eb24
  000c1490: mov r4,r0
  000c1492: movs r1,#0x3
  000c1494: mov r2,r5
  000c1496: mov r3,r6
  000c1498: blx 0x00073edc
  000c149c: b.w 0x000c29da
  000c14a0: movs r0,#0x0
  000c14a2: str r0,[sp,#0x38]
  000c14a4: movs r0,#0x0
  000c14a6: b.w 0x000c29e8
  000c14aa: mov r11,r8
  000c14ac: movs r4,#0x1
  000c14ae: b.w 0x000c0aa4
  000c14b2: ldr r2,[0x000c15f4]
  000c14b4: ldr r1,[0x000c15f8]
  000c14b6: add r2,pc
  000c14b8: ldr.w r0,[r11,#0x0]
  000c14bc: ldr r6,[r2,#0x0]
  000c14be: mov r9,r1
  000c14c0: blx r6
  000c14c2: movw r4,#0xc350
  000c14c6: subs r0,r0,r4
  000c14c8: str r0,[sp,#0x138]
  000c14ca: mov.w r8,#0x0
  000c14ce: ldr.w r0,[r11,#0x0]
  000c14d2: mov r1,r9
  000c14d4: strd r8,r4,[sp,#0x13c]
  000c14d8: blx r6
  000c14da: ldr.w r1,[r11,#0x0]
  000c14de: subs r0,r0,r4
  000c14e0: strd r0,r8,[sp,#0x144]
  000c14e4: ldr r2,[0x000c15fc]
  000c14e6: mov r0,r1
  000c14e8: mov r1,r9
  000c14ea: str r2,[sp,#0x14c]
  000c14ec: blx r6
  000c14ee: subs r0,r0,r4
  000c14f0: add r1,sp,#0x150
  000c14f2: stm r1,{r0,r8,r9}
  000c14f6: movs r0,#0x18
  000c14f8: blx 0x0006eb24
  000c14fc: mov r4,r0
  000c14fe: add r1,sp,#0x138
  000c1500: movs r2,#0x9
  000c1502: blx 0x00073eb8
  000c1506: ldr.w r10,[sp,#0x7c]
  000c150a: str.w r4,[r10,#0x110]
  000c150e: ldr r0,[r5,#0x0]
  000c1510: blx 0x00072850
  000c1514: blx 0x00073ea0
  000c1518: vmov s2,r0
  000c151c: movs r1,#0x5
  000c151e: vmov.f32 s0,0x41200000
  000c1522: vcvt.f32.s32 s2,s2
  000c1526: ldr.w r0,[r11,#0x0]
  000c152a: ldr r4,[0x000c1600]
  000c152c: add r4,pc
  000c152e: vdiv.f32 s0,s2,s0
  000c1532: vmov.f32 s2,0x40a00000
  000c1536: vmul.f32 s16,s0,s2
  000c153a: blx 0x00071848
  000c153e: mov r6,r0
  000c1540: ldr r0,[r4,#0x0]
  000c1542: vmov.f32 s0,0xbf000000
  000c1546: vldr.32 s2,[r0,#0x2c]
  000c154a: vcvt.s32.f32 s4,s16
  000c154e: vadd.f32 s0,s2,s0
  000c1552: vmov r0,s4
  000c1556: adds r0,#0x3
  000c1558: vmov s2,r0
  000c155c: movs r0,#0xc
  000c155e: vcvt.f32.s32 s2,s2
  000c1562: vmla.f32 s2,s0,s2
  000c1566: vcvt.s32.f32 s16,s2
  000c156a: blx 0x0006eb24
  000c156e: mov r4,r0
  000c1570: blx 0x00073d08
  000c1574: vmov r0,s16
  000c1578: mov r1,r4
  000c157a: str.w r4,[r10,#0xf8]
  000c157e: str r0,[sp,#0x6c]
  000c1580: add r0,r6
  000c1582: adds r0,#0x3
  000c1584: blx 0x00073d14
  000c1588: ldr r0,[0x000c1604]
  000c158a: movs r4,#0x0
  000c158c: mov.w r9,#0x1
  000c1590: movs r6,#0x0
  000c1592: add r0,pc
  000c1594: ldr.w r8,[r0,#0x0]
  000c1598: b 0x000c1644
  000c1608: ldr r5,[sp,#0x74]
  000c160a: ldr.w r0,[r8,#0x0]
  000c160e: mov r1,r5
  000c1610: blx 0x00071a34
  000c1614: mov r3,r0
  000c1616: mov r0,r10
  000c1618: mov r1,r5
  000c161a: movs r2,#0x0
  000c161c: strd r4,r9,[sp,#0x0]
  000c1620: str r4,[sp,#0x8]
  000c1622: blx 0x00073e88
  000c1626: ldr.w r1,[r10,#0xf8]
  000c162a: ldr r1,[r1,#0x4]
  000c162c: str.w r0,[r1,r6,lsl #0x2]
  000c1630: movs r1,#0x1
  000c1632: ldr.w r0,[r10,#0xf8]
  000c1636: ldr r0,[r0,#0x4]
  000c1638: ldr.w r0,[r0,r6,lsl #0x2]
  000c163c: ldr r0,[r0,#0x4]
  000c163e: blx 0x00073e94
  000c1642: adds r6,#0x1
  000c1644: ldr r0,[sp,#0x6c]
  000c1646: cmp r6,r0
  000c1648: blt 0x000c1608
  000c164a: ldrd r9,r11,[sp,#0x6c]
  000c164e: b 0x000c16b0
  000c1650: ldr.w r0,[r8,#0x0]
  000c1654: mov r1,r11
  000c1656: blx 0x00071a34
  000c165a: mov r4,r0
  000c165c: ldr r0,[sp,#0x78]
  000c165e: ldr.w r6,[r10,#0x110]
  000c1662: ldr r5,[r0,#0x0]
  000c1664: mov r0,r6
  000c1666: blx 0x00073fcc
  000c166a: mov r1,r0
  000c166c: mov r0,r5
  000c166e: blx 0x00071848
  000c1672: mov r1,r0
  000c1674: mov r0,r6
  000c1676: blx 0x00073ec4
  000c167a: str r0,[sp,#0x0]
  000c167c: movs r0,#0x1
  000c167e: str r0,[sp,#0x4]
  000c1680: movs r0,#0x0
  000c1682: str r0,[sp,#0x8]
  000c1684: mov r0,r10
  000c1686: mov r1,r11
  000c1688: movs r2,#0x0
  000c168a: mov r3,r4
  000c168c: blx 0x00073e88
  000c1690: ldr.w r1,[r10,#0xf8]
  000c1694: ldr r1,[r1,#0x4]
  000c1696: str.w r0,[r1,r9,lsl #0x2]
  000c169a: movs r1,#0x1
  000c169c: ldr.w r0,[r10,#0xf8]
  000c16a0: ldr r0,[r0,#0x4]
  000c16a2: ldr.w r0,[r0,r9,lsl #0x2]
  000c16a6: ldr r0,[r0,#0x4]
  000c16a8: blx 0x00073fb4
  000c16ac: add.w r9,r9,#0x1
  000c16b0: ldr.w r0,[r10,#0xf8]
  000c16b4: ldr r0,[r0,#0x0]
  000c16b6: cmp r9,r0
  000c16b8: bcc 0x000c1650
  000c16ba: movs r0,#0x1c
  000c16bc: blx 0x0006eb24
  000c16c0: mov r4,r0
  000c16c2: ldr r2,[sp,#0x6c]
  000c16c4: movs r1,#0x7
  000c16c6: mov r3,r10
  000c16c8: blx 0x00073edc
  000c16cc: str.w r4,[r10,#0x28]
  000c16d0: b.w 0x000c02ca
  000c16d4: movs r4,#0x0
  000c16d6: movw r1,#0x4e20
  000c16da: strd r4,r4,[sp,#0x110]
  000c16de: ldr.w r0,[r11,#0x0]
  000c16e2: str r4,[sp,#0x118]
  000c16e4: blx 0x00071848
  000c16e8: ldr r6,[0x000c19ec]
  000c16ea: movw r1,#0x4e20
  000c16ee: subs r0,r6,r0
  000c16f0: vmov s0,r0
  000c16f4: vcvt.f32.s32 s0,s0
  000c16f8: vcvt.s32.f32 s0,s0
  000c16fc: ldr.w r0,[r11,#0x0]
  000c1700: str r4,[sp,#0x13c]
  000c1702: vstr.32 s0,[sp,#0x138]
  000c1706: blx 0x00071848
  000c170a: subs r0,r6,r0
  000c170c: vmov s0,r0
  000c1710: movs r0,#0x18
  000c1712: vcvt.f32.s32 s0,s0
  000c1716: vcvt.s32.f32 s0,s0
  000c171a: strd r4,r4,[sp,#0x144]
  000c171e: str r4,[sp,#0x14c]
  000c1720: vstr.32 s0,[sp,#0x140]
  000c1724: blx 0x0006eb24
  000c1728: mov r4,r0
  000c172a: add r1,sp,#0x138
  000c172c: movs r2,#0x6
  000c172e: blx 0x00073eb8
  000c1732: ldr r5,[sp,#0x7c]
  000c1734: mov r0,r9
  000c1736: str.w r4,[r5,#0x110]
  000c173a: blx 0x00072bb0
  000c173e: mov r4,r0
  000c1740: mov r0,r9
  000c1742: blx 0x00073ea0
  000c1746: vmov s2,r0
  000c174a: movs r0,#0xc
  000c174c: vmov.f32 s0,0x41200000
  000c1750: vcvt.f32.s32 s2,s2
  000c1754: vdiv.f32 s0,s2,s0
  000c1758: vmov.f32 s2,0x40800000
  000c175c: vmul.f32 s0,s0,s2
  000c1760: vcvt.s32.f32 s16,s0
  000c1764: blx 0x0006eb24
  000c1768: mov r6,r0
  000c176a: blx 0x00073d08
  000c176e: vmov r0,s16
  000c1772: mov r1,r6
  000c1774: str.w r6,[r5,#0xf8]
  000c1778: str r0,[sp,#0x68]
  000c177a: add.w r11,r0,#0x2
  000c177e: add.w r0,r11,r4
  000c1782: str r0,[sp,#0x64]
  000c1784: blx 0x00073d14
  000c1788: movs r0,#0x0
  000c178a: mov.w r8,#0x0
  000c178e: str r0,[sp,#0x78]
  000c1790: b 0x000c1886
  000c1792: ldr r0,[0x000c19f0]
  000c1794: ldr r4,[sp,#0x74]
  000c1796: add r0,pc
  000c1798: ldr r0,[r0,#0x0]
  000c179a: mov r1,r4
  000c179c: ldr r0,[r0,#0x0]
  000c179e: blx 0x00071a34
  000c17a2: mov r3,r0
  000c17a4: movs r0,#0x0
  000c17a6: movs r1,#0x1
  000c17a8: str r0,[sp,#0x0]
  000c17aa: strd r1,r0,[sp,#0x4]
  000c17ae: mov r0,r5
  000c17b0: mov r1,r4
  000c17b2: movs r2,#0x0
  000c17b4: blx 0x00073e88
  000c17b8: ldr.w r1,[r5,#0xf8]
  000c17bc: ldr r1,[r1,#0x4]
  000c17be: str.w r0,[r1,r8,lsl #0x2]
  000c17c2: ldr.w r1,[r5,#0xf8]
  000c17c6: ldr.w r0,[r5,#0x110]
  000c17ca: ldr r2,[0x000c19f4]
  000c17cc: ldr r1,[r1,#0x4]
  000c17ce: add r2,pc
  000c17d0: ldr.w r1,[r1,r8,lsl #0x2]
  000c17d4: ldr r6,[r2,#0x0]
  000c17d6: str r1,[sp,#0x6c]
  000c17d8: movs r1,#0x0
  000c17da: blx r6
  000c17dc: mov r4,r11
  000c17de: ldr.w r11,[r0,#0x124]
  000c17e2: ldr.w r0,[r5,#0x110]
  000c17e6: movs r1,#0x0
  000c17e8: blx r6
  000c17ea: ldr.w r10,[r0,#0x128]
  000c17ee: movs r1,#0x0
  000c17f0: ldr r0,[sp,#0x7c]
  000c17f2: ldr.w r0,[r0,#0x110]
  000c17f6: blx r6
  000c17f8: ldr.w r9,[sp,#0x78]
  000c17fc: ldr.w r0,[r0,#0x12c]
  000c1800: add.w r1,r9,r11
  000c1804: add.w r2,r9,r10
  000c1808: add r0,r9
  000c180a: ldr r5,[sp,#0x7c]
  000c180c: vmov s0,r1
  000c1810: mov r11,r4
  000c1812: vmov s2,r2
  000c1816: vmov s4,r0
  000c181a: vcvt.f32.s32 s0,s0
  000c181e: vcvt.f32.s32 s2,s2
  000c1822: vcvt.f32.s32 s4,s4
  000c1826: ldr r0,[sp,#0x6c]
  000c1828: ldr r1,[r0,#0x0]
  000c182a: ldr r6,[r1,#0x48]
  000c182c: vmov r1,s0
  000c1830: vmov r2,s2
  000c1834: vmov r3,s4
  000c1838: blx r6
  000c183a: ldr.w r0,[r5,#0xf8]
  000c183e: movs r1,#0x1
  000c1840: ldr r0,[r0,#0x4]
  000c1842: ldr.w r0,[r0,r8,lsl #0x2]
  000c1846: ldr r0,[r0,#0x4]
  000c1848: blx 0x00073e94
  000c184c: ldr.w r1,[r5,#0xf8]
  000c1850: ldr.w r0,[r5,#0x110]
  000c1854: ldr r1,[r1,#0x4]
  000c1856: ldr.w r4,[r1,r8,lsl #0x2]
  000c185a: blx 0x00073f3c
  000c185e: mov r1,r0
  000c1860: mov r0,r4
  000c1862: blx 0x00073f60
  000c1866: ldr.w r0,[r5,#0xf8]
  000c186a: ldr r0,[r0,#0x4]
  000c186c: ldr.w r0,[r0,r8,lsl #0x2]
  000c1870: blx 0x00073fd8
  000c1874: movs r1,#0x0
  000c1876: blx 0x00073fe4
  000c187a: add.w r9,r9,#0x7d0
  000c187e: str.w r9,[sp,#0x78]
  000c1882: add.w r8,r8,#0x1
  000c1886: cmp r8,r11
  000c1888: blt 0x000c1792
  000c188a: ldr r1,[sp,#0x68]
  000c188c: movs r0,#0x8
  000c188e: mov.w r9,#0x0
  000c1892: add.w r10,sp,#0x180
  000c1896: vldr.32 s16,[pc,#0x160]
  000c189a: mov.w r8,#0x0
  000c189e: add.w r6,r0,r1, lsl #0x2
  000c18a2: ldr r0,[0x000c19fc]
  000c18a4: add r0,pc
  000c18a6: ldr r0,[r0,#0x0]
  000c18a8: str r0,[sp,#0x7c]
  000c18aa: b 0x000c1984
  000c18ac: ldr r0,[sp,#0x7c]
  000c18ae: ldr r4,[sp,#0x70]
  000c18b0: ldr r0,[r0,#0x0]
  000c18b2: mov r1,r4
  000c18b4: blx 0x00071a34
  000c18b8: mov r3,r0
  000c18ba: movs r0,#0x1
  000c18bc: str.w r9,[sp,#0x0]
  000c18c0: mov r1,r4
  000c18c2: strd r0,r9,[sp,#0x4]
  000c18c6: mov r0,r5
  000c18c8: movs r2,#0x0
  000c18ca: blx 0x00073e88
  000c18ce: ldr.w r1,[r5,#0xf8]
  000c18d2: ldr r1,[r1,#0x4]
  000c18d4: add r1,r6
  000c18d6: str.w r0,[r1,r8,lsl #0x2]
  000c18da: movs r1,#0x1
  000c18dc: ldr.w r0,[r5,#0xf8]
  000c18e0: ldr r0,[r0,#0x4]
  000c18e2: add r0,r6
  000c18e4: ldr.w r0,[r0,r8,lsl #0x2]
  000c18e8: ldr r0,[r0,#0x4]
  000c18ea: blx 0x00073fb4
  000c18ee: ldr.w r0,[r5,#0xfc]
  000c18f2: ldrd r1,r0,[r0,#0x0]
  000c18f6: add.w r1,r8,r1, lsr #0x1
  000c18fa: ldr.w r1,[r0,r1,lsl #0x2]
  000c18fe: ldr r0,[r1,#0x0]
  000c1900: ldr r2,[r0,#0x28]
  000c1902: mov r0,r10
  000c1904: blx r2
  000c1906: add r0,sp,#0x110
  000c1908: mov r1,r10
  000c190a: blx 0x0006eb3c
  000c190e: vldr.32 s0,[sp,#0x114]
  000c1912: ldr.w r0,[r5,#0xf8]
  000c1916: vadd.f32 s0,s0,s16
  000c191a: ldr r3,[sp,#0x118]
  000c191c: ldr r0,[r0,#0x4]
  000c191e: add r0,r6
  000c1920: ldr.w r0,[r0,r8,lsl #0x2]
  000c1924: vmov r2,s0
  000c1928: ldr r1,[r0,#0x0]
  000c192a: ldr r4,[r1,#0x48]
  000c192c: ldr r1,[sp,#0x110]
  000c192e: blx r4
  000c1930: ldr.w r0,[r5,#0xf8]
  000c1934: ldr r0,[r0,#0x4]
  000c1936: add r0,r6
  000c1938: ldr.w r1,[r0,r8,lsl #0x2]
  000c193c: str.w r9,[r1,#0x50]
  000c1940: strb.w r9,[r1,#0x4c]
  000c1944: ldr.w r0,[r0,r8,lsl #0x2]
  000c1948: ldr r1,[r0,#0x0]
  000c194a: ldr r2,[r1,#0x1c]
  000c194c: movs r1,#0x0
  000c194e: blx r2
  000c1950: ldr.w r0,[r5,#0xf8]
  000c1954: movs r1,#0x0
  000c1956: ldr r0,[r0,#0x4]
  000c1958: add r0,r6
  000c195a: ldr.w r0,[r0,r8,lsl #0x2]
  000c195e: blx 0x00073ff0
  000c1962: ldr.w r0,[r5,#0xf8]
  000c1966: ldr r0,[r0,#0x4]
  000c1968: add r0,r6
  000c196a: ldr.w r0,[r0,r8,lsl #0x2]
  000c196e: ldr r4,[r0,#0x4]
  000c1970: mov r0,r4
  000c1972: blx 0x00072910
  000c1976: add.w r1,r0,r0, lsl #0x1
  000c197a: mov r0,r4
  000c197c: blx 0x000727e4
  000c1980: add.w r8,r8,#0x1
  000c1984: ldr.w r0,[r5,#0xf8]
  000c1988: add.w r1,r11,r8
  000c198c: ldr r0,[r0,#0x0]
  000c198e: cmp r1,r0
  000c1990: bcc 0x000c18ac
  000c1992: movs r0,#0x1c
  000c1994: blx 0x0006eb24
  000c1998: mov r4,r0
  000c199a: movs r1,#0x12
  000c199c: movs r2,#0x0
  000c199e: mov r3,r11
  000c19a0: str r5,[sp,#0x0]
  000c19a2: blx 0x00073f9c
  000c19a6: movs r0,#0x1c
  000c19a8: str r4,[r5,#0x28]
  000c19aa: blx 0x0006eb24
  000c19ae: mov r4,r0
  000c19b0: ldr r3,[sp,#0x64]
  000c19b2: movs r1,#0x12
  000c19b4: mov r2,r11
  000c19b6: str r5,[sp,#0x0]
  000c19b8: blx 0x00073f9c
  000c19bc: b.w 0x000c374c
  000c19c0: ldr.w r0,[r11,#0x0]
  000c19c4: movs r1,#0x2
  000c19c6: blx 0x00071848
  000c19ca: cmp r0,#0x0
  000c19cc: beq.w 0x000c40b6
  000c19d0: ldr.w r0,[r11,#0x0]
  000c19d4: movs r1,#0x2
  000c19d6: blx 0x00071848
  000c19da: adds r1,r0,#0x2
  000c19dc: blx 0x00073ffc
  000c19e0: ldr.w r8,[sp,#0x7c]
  000c19e4: str.w r0,[r8,#0x108]
  000c19e8: b.w 0x000c40f2
  000c1a00: ldr r2,[0x000c1d8c]
  000c1a02: ldr r1,[0x000c1d90]
  000c1a04: add r2,pc
  000c1a06: ldr.w r0,[r11,#0x0]
  000c1a0a: ldr r5,[r2,#0x0]
  000c1a0c: mov r9,r1
  000c1a0e: blx r5
  000c1a10: ldr.w r1,[r11,#0x0]
  000c1a14: movw r8,#0xea60
  000c1a18: add.w r4,r0,r8
  000c1a1c: mov r0,r1
  000c1a1e: movs r1,#0x2
  000c1a20: blx r5
  000c1a22: cmp r0,#0x0
  000c1a24: mov.w r0,#0xffffffff
  000c1a28: it eq
  000c1a2a: mov.eq r0,#0x1
  000c1a2c: mov.w r6,#0xffffffff
  000c1a30: mul r1,r0,r4
  000c1a34: movs r0,#0x0
  000c1a36: str r0,[sp,#0x13c]
  000c1a38: ldr.w r0,[r11,#0x0]
  000c1a3c: str r1,[sp,#0x138]
  000c1a3e: mov r1,r9
  000c1a40: blx r5
  000c1a42: ldr.w r1,[r11,#0x0]
  000c1a46: add.w r4,r0,r8
  000c1a4a: mov r0,r1
  000c1a4c: movs r1,#0x2
  000c1a4e: blx r5
  000c1a50: cmp r0,#0x0
  000c1a52: it eq
  000c1a54: mov.eq r6,#0x1
  000c1a56: mul r0,r6,r4
  000c1a5a: str r0,[sp,#0x140]
  000c1a5c: movs r0,#0x18
  000c1a5e: blx 0x0006eb24
  000c1a62: mov r4,r0
  000c1a64: add r1,sp,#0x138
  000c1a66: movs r2,#0x3
  000c1a68: blx 0x00073eb8
  000c1a6c: ldr r5,[sp,#0x7c]
  000c1a6e: movs r0,#0xc
  000c1a70: str.w r4,[r5,#0x108]
  000c1a74: blx 0x0006eb24
  000c1a78: mov r4,r0
  000c1a7a: blx 0x00073d08
  000c1a7e: movs r0,#0x1
  000c1a80: mov r1,r4
  000c1a82: str.w r4,[r5,#0xf8]
  000c1a86: mov.w r8,#0x1
  000c1a8a: blx 0x00073d14
  000c1a8e: ldr r0,[0x000c1d94]
  000c1a90: movs r1,#0x8
  000c1a92: add r0,pc
  000c1a94: ldr r0,[r0,#0x0]
  000c1a96: ldr r0,[r0,#0x0]
  000c1a98: blx 0x00071a34
  000c1a9c: mov r4,r0
  000c1a9e: ldr.w r0,[r5,#0x108]
  000c1aa2: movs r1,#0x0
  000c1aa4: movs r6,#0x0
  000c1aa6: blx 0x00073ec4
  000c1aaa: strd r0,r8,[sp,#0x0]
  000c1aae: mov r0,r5
  000c1ab0: movs r1,#0x8
  000c1ab2: movs r2,#0x0
  000c1ab4: mov r3,r4
  000c1ab6: str r6,[sp,#0x8]
  000c1ab8: blx 0x00073e88
  000c1abc: ldr.w r1,[r5,#0xf8]
  000c1ac0: ldr r1,[r1,#0x4]
  000c1ac2: str r0,[r1,#0x0]
  000c1ac4: ldr.w r0,[r5,#0xf8]
  000c1ac8: ldr r0,[r0,#0x4]
  000c1aca: ldr r0,[r0,#0x0]
  000c1acc: blx 0x000732dc
  000c1ad0: ldr.w r0,[r5,#0xf8]
  000c1ad4: ldr r0,[r0,#0x4]
  000c1ad6: ldr r0,[r0,#0x0]
  000c1ad8: ldr r4,[r0,#0x4]
  000c1ada: mov r0,r4
  000c1adc: blx 0x00072910
  000c1ae0: add.w r1,r0,r0, lsl #0x1
  000c1ae4: mov r0,r4
  000c1ae6: blx 0x00073f78
  000c1aea: ldr.w r0,[r5,#0xf8]
  000c1aee: ldr r0,[r0,#0x4]
  000c1af0: ldr r0,[r0,#0x0]
  000c1af2: ldr r1,[r0,#0x0]
  000c1af4: ldr r2,[r1,#0x1c]
  000c1af6: ldr r1,[0x000c1d98]
  000c1af8: blx r2
  000c1afa: ldr.w r0,[r5,#0xf8]
  000c1afe: movs r1,#0x3
  000c1b00: ldr r0,[r0,#0x4]
  000c1b02: ldr r0,[r0,#0x0]
  000c1b04: blx 0x00074008
  000c1b08: movs r0,#0x1c
  000c1b0a: blx 0x0006eb24
  000c1b0e: mov r4,r0
  000c1b10: movs r1,#0x1
  000c1b12: movs r2,#0x0
  000c1b14: mov r3,r5
  000c1b16: blx 0x00073edc
  000c1b1a: str r4,[r5,#0x28]
  000c1b1c: b.w 0x000c02ca
  000c1b20: ldr r0,[0x000c1d9c]
  000c1b22: movw r1,#0x9c40
  000c1b26: movw r5,#0x9c40
  000c1b2a: add r0,pc
  000c1b2c: ldr r4,[r0,#0x0]
  000c1b2e: ldr.w r0,[r11,#0x0]
  000c1b32: blx r4
  000c1b34: ldr.w r1,[r11,#0x0]
  000c1b38: movw r2,#0x4e20
  000c1b3c: subs r0,r0,r2
  000c1b3e: str r0,[sp,#0x138]
  000c1b40: mov r0,r1
  000c1b42: movw r1,#0x4e20
  000c1b46: blx r4
  000c1b48: ldr.w r1,[r11,#0x0]
  000c1b4c: movw r2,#0x2710
  000c1b50: subs r0,r0,r2
  000c1b52: str r0,[sp,#0x13c]
  000c1b54: mov r0,r1
  000c1b56: movw r1,#0x7530
  000c1b5a: blx r4
  000c1b5c: add r0,r5
  000c1b5e: str r0,[sp,#0x140]
  000c1b60: movs r0,#0x18
  000c1b62: blx 0x0006eb24
  000c1b66: mov r10,r0
  000c1b68: add r1,sp,#0x138
  000c1b6a: movs r2,#0x3
  000c1b6c: blx 0x00073eb8
  000c1b70: ldr r4,[sp,#0x6c]
  000c1b72: ldr r0,[r4,#0x0]
  000c1b74: blx 0x00072850
  000c1b78: blx 0x00073ea0
  000c1b7c: vmov s0,r0
  000c1b80: vmov.f32 s16,0x41200000
  000c1b84: vcvt.f32.s32 s0,s0
  000c1b88: ldr r0,[r4,#0x0]
  000c1b8a: vdiv.f32 s0,s0,s16
  000c1b8e: vadd.f32 s0,s0,s0
  000c1b92: vcvt.s32.f32 s18,s0
  000c1b96: blx 0x00072850
  000c1b9a: blx 0x00073ea0
  000c1b9e: mov r8,r0
  000c1ba0: movs r0,#0xc
  000c1ba2: blx 0x0006eb24
  000c1ba6: mov r4,r0
  000c1ba8: blx 0x00073d08
  000c1bac: vmov s0,r8
  000c1bb0: mov r1,r4
  000c1bb2: vmov.f32 s2,0x41a00000
  000c1bb6: vcvt.f32.s32 s0,s0
  000c1bba: vmov r9,s18
  000c1bbe: vdiv.f32 s0,s0,s16
  000c1bc2: vmul.f32 s0,s0,s2
  000c1bc6: vcvt.s32.f32 s0,s0
  000c1bca: ldr.w r8,[sp,#0x7c]
  000c1bce: str.w r4,[r8,#0xf8]
  000c1bd2: vmov r0,s0
  000c1bd6: add r0,r9
  000c1bd8: adds r0,#0xf
  000c1bda: blx 0x00073d14
  000c1bde: movs r6,#0x0
  000c1be0: b 0x000c1c16
  000c1be2: mov r0,r10
  000c1be4: movs r1,#0x0
  000c1be6: blx 0x00073ec4
  000c1bea: mov r1,r0
  000c1bec: mov r0,r8
  000c1bee: movw r2,#0x4215
  000c1bf2: movs r3,#0x1
  000c1bf4: blx 0x00073f6c
  000c1bf8: ldr.w r1,[r8,#0xf8]
  000c1bfc: ldr r1,[r1,#0x4]
  000c1bfe: str.w r0,[r1,r6,lsl #0x2]
  000c1c02: movs r1,#0x1
  000c1c04: ldr.w r0,[r8,#0xf8]
  000c1c08: ldr r0,[r0,#0x4]
  000c1c0a: ldr.w r0,[r0,r6,lsl #0x2]
  000c1c0e: ldr r0,[r0,#0x4]
  000c1c10: blx 0x00073e94
  000c1c14: adds r6,#0x1
  000c1c16: ldr.w r5,[r8,#0xf8]
  000c1c1a: ldr r0,[r5,#0x0]
  000c1c1c: sub.w r4,r0,r9
  000c1c20: cmp r6,r4
  000c1c22: bcc 0x000c1be2
  000c1c24: ldr r1,[0x000c1da0]
  000c1c26: movs r6,#0x0
  000c1c28: mov.w r8,#0x1
  000c1c2c: add r1,pc
  000c1c2e: ldr.w r10,[r1,#0x0]
  000c1c32: b 0x000c1c64
  000c1c34: ldr.w r0,[r10,#0x0]
  000c1c38: movs r1,#0x8
  000c1c3a: blx 0x00071a34
  000c1c3e: strd r6,r8,[sp,#0x0]
  000c1c42: mov r3,r0
  000c1c44: ldr r5,[sp,#0x7c]
  000c1c46: movs r1,#0x8
  000c1c48: movs r2,#0x0
  000c1c4a: str r6,[sp,#0x8]
  000c1c4c: mov r0,r5
  000c1c4e: blx 0x00073e88
  000c1c52: ldr.w r1,[r5,#0xf8]
  000c1c56: ldr r1,[r1,#0x4]
  000c1c58: str.w r0,[r1,r4,lsl #0x2]
  000c1c5c: adds r4,#0x1
  000c1c5e: ldr.w r5,[r5,#0xf8]
  000c1c62: ldr r0,[r5,#0x0]
  000c1c64: cmp r4,r0
  000c1c66: bcc 0x000c1c34
  000c1c68: movs r0,#0x1c
  000c1c6a: blx 0x0006eb24
  000c1c6e: mov r4,r0
  000c1c70: ldr r0,[r5,#0x0]
  000c1c72: sub.w r2,r0,r9
  000c1c76: ldr r5,[sp,#0x7c]
  000c1c78: mov r0,r4
  000c1c7a: movs r1,#0x7
  000c1c7c: mov r3,r5
  000c1c7e: blx 0x00073edc
  000c1c82: ldr r0,[0x000c1da4]
  000c1c84: movs r1,#0x16
  000c1c86: str r4,[r5,#0x28]
  000c1c88: add.w r0,r0,#0x3e8
  000c1c8c: str.w r0,[r5,#0x130]
  000c1c90: ldr.w r0,[r10,#0x0]
  000c1c94: blx 0x00072298
  000c1c98: b.w 0x000c02ca
  000c1c9c: add r5,sp,#0x138
  000c1c9e: vmov.i32 q8,#0x0
  000c1ca2: adds r3,r5,#0x4
  000c1ca4: ldr r0,[0x000c1da8]
  000c1ca6: ldr r2,[0x000c1dac]
  000c1ca8: vst1.32 {d16,d17},[r3]
  000c1cac: add.w r3,r5,#0x10
  000c1cb0: vst1.32 {d16,d17},[r3]
  000c1cb4: movw r3,#0x2710
  000c1cb8: str r3,[sp,#0x138]
  000c1cba: str r2,[sp,#0x158]
  000c1cbc: strd r0,r3,[sp,#0x140]
  000c1cc0: movs r0,#0x18
  000c1cc2: ldr r1,[0x000c1db0]
  000c1cc4: strd r1,r3,[sp,#0x14c]
  000c1cc8: blx 0x0006eb24
  000c1ccc: mov r4,r0
  000c1cce: mov r1,r5
  000c1cd0: movs r2,#0x9
  000c1cd2: blx 0x00073eb8
  000c1cd6: ldr r5,[sp,#0x7c]
  000c1cd8: ldr r6,[sp,#0x6c]
  000c1cda: str.w r4,[r5,#0x110]
  000c1cde: ldr r0,[r6,#0x0]
  000c1ce0: blx 0x00072850
  000c1ce4: blx 0x00073ea0
  000c1ce8: vmov s2,r0
  000c1cec: vmov.f32 s0,0x41200000
  000c1cf0: vcvt.f32.s32 s2,s2
  000c1cf4: ldr r0,[0x000c1db4]
  000c1cf6: add r0,pc
  000c1cf8: ldr r0,[r0,#0x0]
  000c1cfa: str r0,[sp,#0x74]
  000c1cfc: vdiv.f32 s0,s2,s0
  000c1d00: vmov.f32 s2,0x40a00000
  000c1d04: vldr.32 s4,[r0,#0x2c]
  000c1d08: vmul.f32 s0,s0,s2
  000c1d0c: vmov.f32 s2,0xbf000000
  000c1d10: vcvt.s32.f32 s0,s0
  000c1d14: vadd.f32 s2,s4,s2
  000c1d18: vmov r0,s0
  000c1d1c: adds r0,#0x3
  000c1d1e: vmov s0,r0
  000c1d22: movs r0,#0xc
  000c1d24: vcvt.f32.s32 s0,s0
  000c1d28: vmla.f32 s0,s2,s0
  000c1d2c: vcvt.s32.f32 s16,s0
  000c1d30: blx 0x0006eb24
  000c1d34: mov r4,r0
  000c1d36: blx 0x00073d08
  000c1d3a: vmov r10,s16
  000c1d3e: mov r1,r4
  000c1d40: str.w r4,[r5,#0xf8]
  000c1d44: add.w r0,r10,#0x5
  000c1d48: str r0,[sp,#0x5c]
  000c1d4a: blx 0x00073d14
  000c1d4e: ldr r0,[r6,#0x0]
  000c1d50: blx 0x00071b84
  000c1d54: mov r4,r0
  000c1d56: mov r0,r9
  000c1d58: blx 0x00074014
  000c1d5c: mov r1,r0
  000c1d5e: mov r0,r4
  000c1d60: blx 0x00073eac
  000c1d64: str r0,[sp,#0x70]
  000c1d66: mov r0,r9
  000c1d68: blx 0x00074014
  000c1d6c: mov.w r8,#0xf
  000c1d70: cmp r0,#0x3
  000c1d72: bgt.w 0x000c34ea
  000c1d76: mov r0,r9
  000c1d78: blx 0x00074014
  000c1d7c: str r0,[sp,#0x64]
  000c1d7e: cmp r0,#0x1
  000c1d80: it eq
  000c1d82: mov.eq.w r8,#0xd
  000c1d86: mov r1,r0
  000c1d88: b.w 0x000c34ee
  000c1db8: ldr r0,[0x000c20fc]
  000c1dba: movw r1,#0x1388
  000c1dbe: add r0,pc
  000c1dc0: ldr r4,[r0,#0x0]
  000c1dc2: ldr.w r0,[r11,#0x0]
  000c1dc6: blx r4
  000c1dc8: ldr.w r1,[r11,#0x0]
  000c1dcc: subw r0,r0,#0x9c4
  000c1dd0: str r0,[sp,#0x138]
  000c1dd2: mov r0,r1
  000c1dd4: movw r1,#0x1388
  000c1dd8: blx r4
  000c1dda: ldr.w r1,[r11,#0x0]
  000c1dde: subw r0,r0,#0x9c4
  000c1de2: str r0,[sp,#0x13c]
  000c1de4: mov r0,r1
  000c1de6: movw r1,#0x7530
  000c1dea: blx r4
  000c1dec: ldr r2,[0x000c2100]
  000c1dee: ldr.w r1,[r11,#0x0]
  000c1df2: add r0,r2
  000c1df4: str r0,[sp,#0x140]
  000c1df6: mov r0,r1
  000c1df8: movw r1,#0x1388
  000c1dfc: blx r4
  000c1dfe: ldr.w r1,[r11,#0x0]
  000c1e02: subw r0,r0,#0x9c4
  000c1e06: str r0,[sp,#0x144]
  000c1e08: mov r0,r1
  000c1e0a: movw r1,#0x1388
  000c1e0e: blx r4
  000c1e10: ldr.w r1,[r11,#0x0]
  000c1e14: subw r0,r0,#0x9c4
  000c1e18: str r0,[sp,#0x148]
  000c1e1a: str r4,[sp,#0x64]
  000c1e1c: mov r0,r1
  000c1e1e: movw r1,#0x7530
  000c1e22: blx r4
  000c1e24: ldr r1,[0x000c2104]
  000c1e26: add r0,r1
  000c1e28: str r0,[sp,#0x14c]
  000c1e2a: movs r0,#0x18
  000c1e2c: blx 0x0006eb24
  000c1e30: mov r4,r0
  000c1e32: add r1,sp,#0x138
  000c1e34: movs r2,#0x6
  000c1e36: blx 0x00073eb8
  000c1e3a: ldr r5,[sp,#0x7c]
  000c1e3c: movs r1,#0x2
  000c1e3e: str.w r4,[r5,#0x108]
  000c1e42: ldr.w r0,[r11,#0x0]
  000c1e46: blx 0x00071848
  000c1e4a: ldr r6,[sp,#0x6c]
  000c1e4c: mov r8,r0
  000c1e4e: ldr r0,[r6,#0x0]
  000c1e50: blx 0x00072850
  000c1e54: blx 0x00073ea0
  000c1e58: vmov s2,r0
  000c1e5c: vmov.f32 s0,0x41200000
  000c1e60: vcvt.f32.s32 s2,s2
  000c1e64: ldr r0,[0x000c2108]
  000c1e66: add r0,pc
  000c1e68: ldr r0,[r0,#0x0]
  000c1e6a: str r0,[sp,#0x60]
  000c1e6c: vdiv.f32 s0,s2,s0
  000c1e70: vmov.f32 s2,0x40a00000
  000c1e74: vldr.32 s16,[r0,#0x2c]
  000c1e78: ldr r0,[r6,#0x0]
  000c1e7a: vmul.f32 s18,s0,s2
  000c1e7e: blx 0x00071b84
  000c1e82: mov r6,r0
  000c1e84: mov r0,r9
  000c1e86: blx 0x00074014
  000c1e8a: mov r1,r0
  000c1e8c: mov r0,r6
  000c1e8e: vcvt.s32.f32 s18,s18
  000c1e92: blx 0x00073eac
  000c1e96: vmov r1,s18
  000c1e9a: subs.w r4,r0,#0x8
  000c1e9e: add.w r1,r1,#0x3
  000c1ea2: vmov s0,r1
  000c1ea6: vcvt.f32.s32 s18,s0
  000c1eaa: it ne
  000c1eac: mov.ne r4,r0
  000c1eae: movs r0,#0xc
  000c1eb0: blx 0x0006eb24
  000c1eb4: mov r6,r0
  000c1eb6: blx 0x00073d08
  000c1eba: vmov.f32 s0,0xbf000000
  000c1ebe: add.w r2,r8,#0x2
  000c1ec2: mov r1,r6
  000c1ec4: vadd.f32 s0,s16,s0
  000c1ec8: vmla.f32 s18,s0,s18
  000c1ecc: vcvt.s32.f32 s0,s18
  000c1ed0: str.w r6,[r5,#0xf8]
  000c1ed4: str r2,[sp,#0x68]
  000c1ed6: vmov r0,s0
  000c1eda: add r0,r2
  000c1edc: blx 0x00073d14
  000c1ee0: vmov.f32 s16,0x3f800000
  000c1ee4: movs r0,#0xf
  000c1ee6: str r4,[sp,#0x74]
  000c1ee8: cmp r4,#0x1
  000c1eea: it eq
  000c1eec: mov.eq r0,#0xd
  000c1eee: ldr.w r10,[sp,#0x7c]
  000c1ef2: vldr.64 d9,[pc,#0x21c]
  000c1ef6: mov.w r9,#0x0
  000c1efa: vldr.32 s20,[pc,#0x21c]
  000c1efe: movw r8,#0x2710
  000c1f02: vldr.32 s22,[pc,#0x218]
  000c1f06: str r0,[sp,#0x5c]
  000c1f08: b 0x000c2050
  000c1f0a: ldr r1,[0x000c2120]
  000c1f0c: ldr.w r0,[r10,#0x108]
  000c1f10: add r1,pc
  000c1f12: ldr r5,[r1,#0x0]
  000c1f14: movs r1,#0x1
  000c1f16: blx r5
  000c1f18: str r0,[sp,#0x0]
  000c1f1a: movs r0,#0x1
  000c1f1c: ldr r1,[sp,#0x74]
  000c1f1e: movs r2,#0x1
  000c1f20: ldr r3,[sp,#0x5c]
  000c1f22: str r0,[sp,#0x4]
  000c1f24: movs r0,#0x0
  000c1f26: str r0,[sp,#0x8]
  000c1f28: mov r0,r10
  000c1f2a: blx 0x00073e88
  000c1f2e: ldr.w r1,[r10,#0xf8]
  000c1f32: ldr r1,[r1,#0x4]
  000c1f34: str.w r0,[r1,r9,lsl #0x2]
  000c1f38: ldr.w r0,[r10,#0xf8]
  000c1f3c: ldr r0,[r0,#0x4]
  000c1f3e: ldr.w r0,[r0,r9,lsl #0x2]
  000c1f42: blx 0x000732dc
  000c1f46: ldr.w r0,[r10,#0xf8]
  000c1f4a: movs r1,#0x1
  000c1f4c: ldr r0,[r0,#0x4]
  000c1f4e: ldr.w r0,[r0,r9,lsl #0x2]
  000c1f52: ldr r0,[r0,#0x4]
  000c1f54: blx 0x00073e94
  000c1f58: ldr.w r0,[r10,#0xf8]
  000c1f5c: movs r1,#0x0
  000c1f5e: ldr r0,[r0,#0x4]
  000c1f60: ldr.w r0,[r0,r9,lsl #0x2]
  000c1f64: blx 0x00073f54
  000c1f68: ldr.w r1,[r10,#0xf8]
  000c1f6c: ldr.w r0,[r10,#0x108]
  000c1f70: ldr r1,[r1,#0x4]
  000c1f72: ldr.w r1,[r1,r9,lsl #0x2]
  000c1f76: str r1,[sp,#0x70]
  000c1f78: movs r1,#0x1
  000c1f7a: blx r5
  000c1f7c: ldr.w r0,[r0,#0x124]
  000c1f80: movw r1,#0x4e20
  000c1f84: str r0,[sp,#0x6c]
  000c1f86: ldr.w r0,[r11,#0x0]
  000c1f8a: ldr r4,[sp,#0x64]
  000c1f8c: blx r4
  000c1f8e: mov r6,r0
  000c1f90: ldr.w r0,[r10,#0x108]
  000c1f94: movs r1,#0x1
  000c1f96: blx r5
  000c1f98: ldr.w r11,[r0,#0x128]
  000c1f9c: movw r1,#0x4e20
  000c1fa0: ldr r0,[sp,#0x78]
  000c1fa2: ldr r0,[r0,#0x0]
  000c1fa4: blx r4
  000c1fa6: add r0,r11
  000c1fa8: movs r1,#0x1
  000c1faa: sub.w r0,r0,r8
  000c1fae: ldr.w r11,[sp,#0x78]
  000c1fb2: vmov s24,r0
  000c1fb6: ldr r0,[sp,#0x6c]
  000c1fb8: add r0,r6
  000c1fba: sub.w r0,r0,r8
  000c1fbe: vmov s26,r0
  000c1fc2: ldr.w r0,[r10,#0x108]
  000c1fc6: blx r5
  000c1fc8: ldr.w r5,[r0,#0x12c]
  000c1fcc: movw r1,#0x4e20
  000c1fd0: ldr.w r0,[r11,#0x0]
  000c1fd4: vcvt.f32.s32 s26,s26
  000c1fd8: vcvt.f32.s32 s24,s24
  000c1fdc: blx r4
  000c1fde: add r0,r5
  000c1fe0: vmov r1,s26
  000c1fe4: sub.w r0,r0,r8
  000c1fe8: vmov r2,s24
  000c1fec: vmov s0,r0
  000c1ff0: vcvt.f32.s32 s0,s0
  000c1ff4: ldr r0,[sp,#0x70]
  000c1ff6: ldr r6,[r0,#0x0]
  000c1ff8: ldr r6,[r6,#0x48]
  000c1ffa: vmov r3,s0
  000c1ffe: blx r6
  000c2000: ldr.w r0,[r10,#0xf8]
  000c2004: ldr r0,[r0,#0x4]
  000c2006: ldr.w r0,[r0,r9,lsl #0x2]
  000c200a: ldr r4,[r0,#0x4]
  000c200c: mov r0,r4
  000c200e: blx 0x00072910
  000c2012: ldr r1,[sp,#0x60]
  000c2014: vmov s2,r0
  000c2018: mov r0,r4
  000c201a: vldr.32 s0,[r1,#0x2c]
  000c201e: vcvt.f32.s32 s2,s2
  000c2022: vcvt.f64.f32 d16,s0
  000c2026: vcmpe.f64 d16,d9
  000c202a: vmrs apsr,fpscr
  000c202e: vmul.f32 s2,s2,s20
  000c2032: vmov.f32 s0,s16
  000c2036: it gt
  000c2038: vmov.gt.f32 s0,s22
  000c203c: vmul.f32 s0,s2,s0
  000c2040: vcvt.s32.f32 s0,s0
  000c2044: vmov r1,s0
  000c2048: blx 0x00073f78
  000c204c: add.w r9,r9,#0x1
  000c2050: ldr r0,[sp,#0x68]
  000c2052: cmp r9,r0
  000c2054: blt.w 0x000c1f0a
  000c2058: ldr r0,[0x000c2124]
  000c205a: ldr.w r9,[sp,#0x68]
  000c205e: add r0,pc
  000c2060: ldr r4,[sp,#0x7c]
  000c2062: ldr.w r11,[sp,#0x74]
  000c2066: ldr.w r8,[r0,#0x0]
  000c206a: b 0x000c20da
  000c206c: ldr.w r0,[r8,#0x0]
  000c2070: mov r1,r11
  000c2072: blx 0x00071a34
  000c2076: mov r10,r0
  000c2078: ldr r0,[sp,#0x78]
  000c207a: ldr.w r5,[r4,#0x108]
  000c207e: ldr r6,[r0,#0x0]
  000c2080: mov r0,r5
  000c2082: blx 0x00073fcc
  000c2086: mov r1,r0
  000c2088: mov r0,r6
  000c208a: blx 0x00071848
  000c208e: mov r1,r0
  000c2090: mov r0,r5
  000c2092: blx 0x00073ec4
  000c2096: str r0,[sp,#0x0]
  000c2098: movs r0,#0x1
  000c209a: str r0,[sp,#0x4]
  000c209c: movs r0,#0x0
  000c209e: str r0,[sp,#0x8]
  000c20a0: mov r0,r4
  000c20a2: mov r1,r11
  000c20a4: movs r2,#0x0
  000c20a6: mov r3,r10
  000c20a8: blx 0x00073e88
  000c20ac: ldr.w r1,[r4,#0xf8]
  000c20b0: ldr r1,[r1,#0x4]
  000c20b2: str.w r0,[r1,r9,lsl #0x2]
  000c20b6: ldr.w r0,[r4,#0xf8]
  000c20ba: ldr r0,[r0,#0x4]
  000c20bc: ldr.w r0,[r0,r9,lsl #0x2]
  000c20c0: blx 0x000732dc
  000c20c4: ldr.w r0,[r4,#0xf8]
  000c20c8: movs r1,#0x1
  000c20ca: ldr r0,[r0,#0x4]
  000c20cc: ldr.w r0,[r0,r9,lsl #0x2]
  000c20d0: ldr r0,[r0,#0x4]
  000c20d2: blx 0x00073e94
  000c20d6: add.w r9,r9,#0x1
  000c20da: ldr.w r0,[r4,#0xf8]
  000c20de: ldr r0,[r0,#0x0]
  000c20e0: cmp r9,r0
  000c20e2: bcc 0x000c206c
  000c20e4: movs r0,#0x1c
  000c20e6: blx 0x0006eb24
  000c20ea: mov r5,r0
  000c20ec: ldr r2,[sp,#0x68]
  000c20ee: movs r1,#0x7
  000c20f0: mov r3,r4
  000c20f2: blx 0x00073edc
  000c20f6: str r5,[r4,#0x28]
  000c20f8: b.w 0x000c02ca
  000c2128: ldr.w r0,[r11,#0x0]
  000c212c: movs r1,#0x2
  000c212e: blx 0x00071848
  000c2132: adds r1,r0,#0x3
  000c2134: blx 0x00073ffc
  000c2138: ldr r5,[sp,#0x7c]
  000c213a: str.w r0,[r5,#0x108]
  000c213e: mov r0,r9
  000c2140: blx 0x00073ea0
  000c2144: vmov s2,r0
  000c2148: movs r0,#0xc
  000c214a: vmov.f32 s0,0x41200000
  000c214e: vcvt.f32.s32 s2,s2
  000c2152: vdiv.f32 s0,s2,s0
  000c2156: vmov.f32 s2,0x40800000
  000c215a: vmul.f32 s0,s0,s2
  000c215e: vcvt.s32.f32 s16,s0
  000c2162: blx 0x0006eb24
  000c2166: mov r4,r0
  000c2168: blx 0x00073d08
  000c216c: vmov r0,s16
  000c2170: str.w r4,[r5,#0xf8]
  000c2174: adds r1,r0,#0x3
  000c2176: tst r1,#0x1
  000c217a: it eq
  000c217c: add.eq r1,r0,#0x4
  000c217e: adds r0,r1,#0x1
  000c2180: mov r1,r4
  000c2182: str r0,[sp,#0x74]
  000c2184: blx 0x00073d14
  000c2188: ldr r0,[0x000c2460]
  000c218a: add r0,pc
  000c218c: ldr.w r8,[r0,#0x0]
  000c2190: mov r0,r9
  000c2192: blx r8
  000c2194: blx 0x00071bd8
  000c2198: mov r4,r0
  000c219a: ldr r0,[0x000c2464]
  000c219c: add r0,pc
  000c219e: ldr.w r10,[r0,#0x0]
  000c21a2: mov r0,r9
  000c21a4: ldr.w r6,[r10,#0x0]
  000c21a8: blx r8
  000c21aa: blx 0x00071bd8
  000c21ae: mov r1,r0
  000c21b0: mov r0,r6
  000c21b2: blx 0x00071a34
  000c21b6: mov r3,r0
  000c21b8: str.w r9,[sp,#0x70]
  000c21bc: mov.w r9,#0x0
  000c21c0: movs r0,#0x1
  000c21c2: strd r9,r0,[sp,#0x0]
  000c21c6: mov r0,r5
  000c21c8: mov r1,r4
  000c21ca: movs r2,#0x0
  000c21cc: str.w r9,[sp,#0x8]
  000c21d0: blx 0x00073e88
  000c21d4: ldr.w r1,[r5,#0xf8]
  000c21d8: ldr r1,[r1,#0x4]
  000c21da: str r0,[r1,#0x0]
  000c21dc: mov.w r1,#0x578
  000c21e0: ldr.w r0,[r5,#0xf8]
  000c21e4: vldr.32 s16,[sp,#0x104]
  000c21e8: ldr r0,[r0,#0x4]
  000c21ea: ldr r4,[r0,#0x0]
  000c21ec: ldr.w r0,[r11,#0x0]
  000c21f0: blx 0x00071848
  000c21f4: sub.w r0,r0,#0x2bc
  000c21f8: mov.w r1,#0x578
  000c21fc: vldr.32 s18,[sp,#0x108]
  000c2200: vmov s0,r0
  000c2204: ldr.w r0,[r11,#0x0]
  000c2208: vcvt.f32.s32 s20,s0
  000c220c: blx 0x00071848
  000c2210: sub.w r0,r0,#0x2bc
  000c2214: vldr.32 s2,[pc,#0x250]
  000c2218: vmov s0,r0
  000c221c: vcvt.f32.s32 s0,s0
  000c2220: vldr.32 s4,[sp,#0x10c]
  000c2224: ldr r0,[r4,#0x0]
  000c2226: vadd.f32 s2,s4,s2
  000c222a: vadd.f32 s4,s16,s20
  000c222e: ldr r6,[r0,#0x48]
  000c2230: mov r0,r4
  000c2232: vadd.f32 s0,s18,s0
  000c2236: vmov r3,s2
  000c223a: vmov r1,s4
  000c223e: vmov r2,s0
  000c2242: blx r6
  000c2244: ldr.w r0,[r5,#0xf8]
  000c2248: ldr r0,[r0,#0x4]
  000c224a: ldr r0,[r0,#0x0]
  000c224c: ldr r1,[r0,#0x0]
  000c224e: ldr r2,[r1,#0x1c]
  000c2250: ldr r1,[0x000c246c]
  000c2252: blx r2
  000c2254: ldr.w r0,[r5,#0xf8]
  000c2258: movs r1,#0x3
  000c225a: ldr r0,[r0,#0x4]
  000c225c: ldr r0,[r0,#0x0]
  000c225e: blx 0x00074008
  000c2262: ldr.w r0,[r5,#0xf8]
  000c2266: ldr r1,[0x000c2470]
  000c2268: ldr r0,[r0,#0x4]
  000c226a: ldr r0,[r0,#0x0]
  000c226c: ldr r0,[r0,#0x4]
  000c226e: blx 0x000727e4
  000c2272: ldr.w r1,[r5,#0xf8]
  000c2276: ldr.w r0,[r5,#0x108]
  000c227a: ldr r1,[r1,#0x4]
  000c227c: ldr r4,[r1,#0x0]
  000c227e: blx 0x00073f3c
  000c2282: mov r1,r0
  000c2284: mov r0,r4
  000c2286: blx 0x00073f60
  000c228a: ldr.w r0,[r5,#0xf8]
  000c228e: movs r1,#0x1
  000c2290: ldr r0,[r0,#0x4]
  000c2292: ldr r0,[r0,#0x0]
  000c2294: ldr r0,[r0,#0x4]
  000c2296: blx 0x00073fb4
  000c229a: ldr r0,[sp,#0x70]
  000c229c: blx r8
  000c229e: add r4,sp,#0x138
  000c22a0: mov r1,r0
  000c22a2: mov r0,r4
  000c22a4: blx 0x00071c2c
  000c22a8: ldr.w r0,[r5,#0xf8]
  000c22ac: ldr r0,[r0,#0x4]
  000c22ae: ldr r0,[r0,#0x0]
  000c22b0: adds r0,#0x18
  000c22b2: mov r1,r4
  000c22b4: blx 0x0006f2b0
  000c22b8: add r0,sp,#0x138
  000c22ba: blx 0x0006ee30
  000c22be: ldr.w r0,[r5,#0xf8]
  000c22c2: mov.w r11,#0x1
  000c22c6: ldr r4,[sp,#0x7c]
  000c22c8: ldr r1,[r0,#0x4]
  000c22ca: ldr r1,[r1,#0x0]
  000c22cc: str.w r9,[r1,#0x50]
  000c22d0: b 0x000c2330
  000c22d2: ldr.w r0,[r10,#0x0]
  000c22d6: movs r1,#0x8
  000c22d8: blx 0x00071a34
  000c22dc: mov r8,r0
  000c22de: ldr r0,[sp,#0x78]
  000c22e0: ldr.w r5,[r4,#0x108]
  000c22e4: ldr r6,[r0,#0x0]
  000c22e6: mov r0,r5
  000c22e8: blx 0x00073fcc
  000c22ec: mov r1,r0
  000c22ee: mov r0,r6
  000c22f0: blx 0x00071848
  000c22f4: mov r1,r0
  000c22f6: mov r0,r5
  000c22f8: blx 0x00073ec4
  000c22fc: str r0,[sp,#0x0]
  000c22fe: movs r0,#0x1
  000c2300: strd r0,r9,[sp,#0x4]
  000c2304: mov r0,r4
  000c2306: movs r1,#0x8
  000c2308: movs r2,#0x0
  000c230a: mov r3,r8
  000c230c: blx 0x00073e88
  000c2310: ldr.w r1,[r4,#0xf8]
  000c2314: ldr r1,[r1,#0x4]
  000c2316: str.w r0,[r1,r11,lsl #0x2]
  000c231a: ldr.w r0,[r4,#0xf8]
  000c231e: ldr r0,[r0,#0x4]
  000c2320: ldr.w r0,[r0,r11,lsl #0x2]
  000c2324: blx 0x000732dc
  000c2328: ldr.w r0,[r4,#0xf8]
  000c232c: add.w r11,r11,#0x1
  000c2330: ldr r0,[r0,#0x0]
  000c2332: cmp r11,r0
  000c2334: bcc 0x000c22d2
  000c2336: movs r0,#0x1c
  000c2338: blx 0x0006eb24
  000c233c: mov r4,r0
  000c233e: ldr r6,[sp,#0x74]
  000c2340: movs r1,#0x14
  000c2342: ldr r5,[sp,#0x7c]
  000c2344: movs r2,#0x1
  000c2346: str r5,[sp,#0x0]
  000c2348: mov r3,r6
  000c234a: blx 0x00073f9c
  000c234e: movs r0,#0x1c
  000c2350: str r4,[r5,#0x28]
  000c2352: blx 0x0006eb24
  000c2356: mov r4,r0
  000c2358: movs r1,#0x15
  000c235a: movs r2,#0x1
  000c235c: mov r3,r6
  000c235e: str r5,[sp,#0x0]
  000c2360: blx 0x00073f9c
  000c2364: b.w 0x000c374c
  000c2368: ldr r1,[0x000c2474]
  000c236a: ldr r4,[0x000c2478]
  000c236c: add r1,pc
  000c236e: ldr.w r0,[r11,#0x0]
  000c2372: ldr r6,[r1,#0x0]
  000c2374: mov r1,r4
  000c2376: blx r6
  000c2378: ldr r2,[0x000c247c]
  000c237a: movs r5,#0x0
  000c237c: ldr r1,[0x000c2480]
  000c237e: add r0,r2
  000c2380: str r0,[sp,#0x138]
  000c2382: ldr.w r0,[r11,#0x0]
  000c2386: mov r8,r2
  000c2388: strd r5,r1,[sp,#0x13c]
  000c238c: mov r1,r4
  000c238e: blx r6
  000c2390: ldr.w r1,[r11,#0x0]
  000c2394: add r0,r8
  000c2396: strd r0,r5,[sp,#0x144]
  000c239a: ldr r2,[0x000c2484]
  000c239c: mov r0,r1
  000c239e: mov r1,r4
  000c23a0: str r2,[sp,#0x14c]
  000c23a2: blx r6
  000c23a4: add r0,r8
  000c23a6: strd r0,r5,[sp,#0x150]
  000c23aa: movs r0,#0x18
  000c23ac: str r4,[sp,#0x158]
  000c23ae: blx 0x0006eb24
  000c23b2: mov r4,r0
  000c23b4: add r1,sp,#0x138
  000c23b6: movs r2,#0x3
  000c23b8: blx 0x00073eb8
  000c23bc: ldr r5,[sp,#0x7c]
  000c23be: str.w r4,[r5,#0x110]
  000c23c2: ldr r4,[sp,#0x6c]
  000c23c4: ldr r0,[r4,#0x0]
  000c23c6: blx 0x00072850
  000c23ca: blx 0x00073ea0
  000c23ce: vmov s2,r0
  000c23d2: vmov.f32 s0,0x41200000
  000c23d6: vcvt.f32.s32 s2,s2
  000c23da: ldr r0,[0x000c2488]
  000c23dc: add r0,pc
  000c23de: ldr r0,[r0,#0x0]
  000c23e0: vdiv.f32 s0,s2,s0
  000c23e4: vadd.f32 s0,s0,s0
  000c23e8: vldr.32 s4,[r0,#0x2c]
  000c23ec: vmov.f32 s2,0xbf000000
  000c23f0: vcvt.s32.f32 s0,s0
  000c23f4: vadd.f32 s2,s4,s2
  000c23f8: vmov r0,s0
  000c23fc: adds r0,#0x1
  000c23fe: vmov s0,r0
  000c2402: vcvt.f32.s32 s16,s0
  000c2406: ldr r0,[r4,#0x0]
  000c2408: vmla.f32 s16,s2,s16
  000c240c: blx 0x00071b84
  000c2410: mov r4,r0
  000c2412: mov r0,r9
  000c2414: blx 0x00074014
  000c2418: mov r1,r0
  000c241a: mov r0,r4
  000c241c: blx 0x00073eac
  000c2420: mov r10,r0
  000c2422: mov r0,r9
  000c2424: vcvt.s32.f32 s16,s16
  000c2428: movs r4,#0x1
  000c242a: str.w r9,[sp,#0x70]
  000c242e: blx 0x00074014
  000c2432: str r0,[sp,#0x74]
  000c2434: movs r0,#0xc
  000c2436: str.w r4,[r5,#0x28c]
  000c243a: blx 0x0006eb24
  000c243e: mov r4,r0
  000c2440: blx 0x00073d08
  000c2444: vmov r9,s16
  000c2448: ldr.w r0,[r5,#0x28c]
  000c244c: mov r1,r4
  000c244e: str.w r4,[r5,#0xf8]
  000c2452: add r0,r9
  000c2454: adds r0,#0x2
  000c2456: blx 0x00073d14
  000c245a: mov.w r8,#0x0
  000c245e: b 0x000c24f8
  000c248c: ldr r0,[0x000c26ec]
  000c248e: mov r1,r10
  000c2490: add r0,pc
  000c2492: ldr r0,[r0,#0x0]
  000c2494: ldr r0,[r0,#0x0]
  000c2496: blx 0x00071a34
  000c249a: ldr.w r6,[r11,#0x0]
  000c249e: mov r4,r0
  000c24a0: ldr.w r11,[sp,#0x7c]
  000c24a4: ldr.w r5,[r11,#0x110]
  000c24a8: mov r0,r5
  000c24aa: blx 0x00073fcc
  000c24ae: mov r1,r0
  000c24b0: mov r0,r6
  000c24b2: blx 0x00071848
  000c24b6: mov r1,r0
  000c24b8: mov r0,r5
  000c24ba: blx 0x00073ec4
  000c24be: str r0,[sp,#0x0]
  000c24c0: movs r0,#0x1
  000c24c2: str r0,[sp,#0x4]
  000c24c4: movs r0,#0x0
  000c24c6: str r0,[sp,#0x8]
  000c24c8: mov r0,r11
  000c24ca: mov r1,r10
  000c24cc: movs r2,#0x0
  000c24ce: mov r3,r4
  000c24d0: blx 0x00073e88
  000c24d4: ldr.w r1,[r11,#0xf8]
  000c24d8: ldr r1,[r1,#0x4]
  000c24da: str.w r0,[r1,r8,lsl #0x2]
  000c24de: movs r1,#0x1
  000c24e0: ldr.w r0,[r11,#0xf8]
  000c24e4: ldr.w r11,[sp,#0x78]
  000c24e8: ldr r0,[r0,#0x4]
  000c24ea: ldr.w r0,[r0,r8,lsl #0x2]
  000c24ee: ldr r0,[r0,#0x4]
  000c24f0: blx 0x00073e94
  000c24f4: add.w r8,r8,#0x1
  000c24f8: cmp r8,r9
  000c24fa: blt 0x000c248c
  000c24fc: ldr r5,[sp,#0x7c]
  000c24fe: movw r2,#0x4a88
  000c2502: movs r3,#0x1
  000c2504: ldr.w r1,[r5,#0xd8]
  000c2508: mov r0,r5
  000c250a: str.w r9,[r5,#0xac]
  000c250e: blx 0x00073f6c
  000c2512: ldr.w r2,[r5,#0xf8]
  000c2516: ldr.w r1,[r5,#0xac]
  000c251a: ldr r2,[r2,#0x4]
  000c251c: str.w r0,[r2,r1,lsl #0x2]
  000c2520: ldr.w r0,[r5,#0xf8]
  000c2524: ldr r0,[r0,#0x4]
  000c2526: ldr.w r0,[r0,r1,lsl #0x2]
  000c252a: movs r1,#0x1
  000c252c: blx 0x00072748
  000c2530: ldr.w r1,[r5,#0xf8]
  000c2534: movs r4,#0x0
  000c2536: ldr.w r0,[r5,#0xac]
  000c253a: ldr r1,[r1,#0x4]
  000c253c: ldr.w r0,[r1,r0,lsl #0x2]
  000c2540: movs r1,#0x0
  000c2542: blx 0x00073f54
  000c2546: ldr.w r1,[r5,#0xf8]
  000c254a: ldr.w r0,[r5,#0xac]
  000c254e: ldr r1,[r1,#0x4]
  000c2550: ldr.w r0,[r1,r0,lsl #0x2]
  000c2554: ldr r1,[r0,#0x0]
  000c2556: ldr r2,[r1,#0x44]
  000c2558: add.w r1,r5,#0xc8
  000c255c: blx r2
  000c255e: ldr r0,[0x000c26f0]
  000c2560: add r1,sp,#0x110
  000c2562: vldr.32 s0,[pc,#0x190]
  000c2566: add r0,pc
  000c2568: vld1.64 {d16,d17},[r0]!
  000c256c: vldr.64 d18,[r0]
  000c2570: movs r0,#0x8
  000c2572: vst1.64 {d16,d17},[r1],r0
  000c2576: movw r0,#0x1770
  000c257a: vstr.64 d18,[sp,#0x120]
  000c257e: vldr.32 s2,[r5,#0xc8]
  000c2582: vadd.f32 s2,s2,s0
  000c2586: vcvt.s32.f32 s2,s2
  000c258a: vstr.32 s2,[sp,#0x110]
  000c258e: vldr.32 s2,[r5,#0xcc]
  000c2592: vadd.f32 s0,s2,s0
  000c2596: vldr.32 s2,[pc,#0x160]
  000c259a: vcvt.s32.f32 s0,s0
  000c259e: vstr.32 s0,[sp,#0x114]
  000c25a2: vldr.32 s0,[r5,#0xd0]
  000c25a6: vadd.f32 s0,s0,s2
  000c25aa: vcvt.s32.f32 s0,s0
  000c25ae: vstr.32 s0,[r1]
  000c25b2: strd r4,r0,[sp,#0x180]
  000c25b6: movs r0,#0xc
  000c25b8: blx 0x0006eb24
  000c25bc: mov r4,r0
  000c25be: blx 0x00073d08
  000c25c2: movs r0,#0x2
  000c25c4: mov r1,r4
  000c25c6: blx 0x00073d14
  000c25ca: ldr.w r1,[r5,#0xf8]
  000c25ce: ldr.w r0,[r5,#0xac]
  000c25d2: ldr r2,[r4,#0x4]
  000c25d4: ldr r1,[r1,#0x4]
  000c25d6: ldr.w r0,[r1,r0,lsl #0x2]
  000c25da: str r0,[r2,#0x4]
  000c25dc: movs r0,#0x18
  000c25de: blx 0x0006eb24
  000c25e2: mov r10,r0
  000c25e4: movs r0,#0x6
  000c25e6: add r1,sp,#0x110
  000c25e8: add r3,sp,#0x180
  000c25ea: str r0,[sp,#0x0]
  000c25ec: mov r0,r10
  000c25ee: mov r2,r4
  000c25f0: blx 0x00073fc0
  000c25f4: mov r0,r10
  000c25f6: movs r1,#0x1
  000c25f8: blx 0x00073f48
  000c25fc: mov r0,r10
  000c25fe: blx 0x00073f3c
  000c2602: mov r11,r0
  000c2604: ldr.w r0,[r5,#0x28c]
  000c2608: ldr.w r8,[sp,#0x74]
  000c260c: add.w r6,r0,r9
  000c2610: b 0x000c26b8
  000c2612: ldr r0,[0x000c26fc]
  000c2614: mov r1,r8
  000c2616: add r0,pc
  000c2618: ldr r0,[r0,#0x0]
  000c261a: ldr r0,[r0,#0x0]
  000c261c: blx 0x00071a34
  000c2620: mov r4,r0
  000c2622: cmp r6,r5
  000c2624: bne 0x000c2654
  000c2626: mov r0,r10
  000c2628: movs r1,#0x0
  000c262a: blx 0x00073ec4
  000c262e: ldr r5,[sp,#0x7c]
  000c2630: mov r1,r8
  000c2632: str r0,[sp,#0x0]
  000c2634: movs r0,#0x1
  000c2636: str r0,[sp,#0x4]
  000c2638: movs r0,#0x0
  000c263a: str r0,[sp,#0x8]
  000c263c: mov r0,r5
  000c263e: movs r2,#0x0
  000c2640: mov r3,r4
  000c2642: blx 0x00073e88
  000c2646: ldr.w r1,[r5,#0xf8]
  000c264a: ldr r1,[r1,#0x4]
  000c264c: str.w r0,[r1,r6,lsl #0x2]
  000c2650: mov r1,r10
  000c2652: b 0x000c2680
  000c2654: mov r0,r11
  000c2656: movs r1,#0x0
  000c2658: blx 0x00073ec4
  000c265c: ldr r5,[sp,#0x7c]
  000c265e: mov r1,r8
  000c2660: str r0,[sp,#0x0]
  000c2662: movs r0,#0x1
  000c2664: str r0,[sp,#0x4]
  000c2666: movs r0,#0x0
  000c2668: str r0,[sp,#0x8]
  000c266a: mov r0,r5
  000c266c: movs r2,#0x0
  000c266e: mov r3,r4
  000c2670: blx 0x00073e88
  000c2674: ldr.w r1,[r5,#0xf8]
  000c2678: ldr r1,[r1,#0x4]
  000c267a: str.w r0,[r1,r6,lsl #0x2]
  000c267e: mov r1,r11
  000c2680: ldr.w r0,[r5,#0xf8]
  000c2684: ldr r0,[r0,#0x4]
  000c2686: ldr.w r0,[r0,r6,lsl #0x2]
  000c268a: blx 0x00073f60
  000c268e: ldr.w r0,[r5,#0xf8]
  000c2692: movs r1,#0x1
  000c2694: ldr r0,[r0,#0x4]
  000c2696: ldr.w r0,[r0,r6,lsl #0x2]
  000c269a: ldr r0,[r0,#0x4]
  000c269c: blx 0x00073fb4
  000c26a0: ldr.w r0,[r5,#0xf8]
  000c26a4: movs r1,#0x1
  000c26a6: ldr r0,[r0,#0x4]
  000c26a8: ldr.w r0,[r0,r6,lsl #0x2]
  000c26ac: ldr r0,[r0,#0x4]
  000c26ae: blx 0x00074020
  000c26b2: ldr.w r0,[r5,#0x28c]
  000c26b6: adds r6,#0x1
  000c26b8: add.w r5,r0,r9
  000c26bc: adds r0,r5,#0x2
  000c26be: cmp r6,r0
  000c26c0: blt 0x000c2612
  000c26c2: movs r0,#0x1c
  000c26c4: blx 0x0006eb24
  000c26c8: mov r4,r0
  000c26ca: ldr r5,[sp,#0x70]
  000c26cc: mov r0,r5
  000c26ce: blx 0x00072bb0
  000c26d2: ldr r6,[sp,#0x7c]
  000c26d4: mov r8,r0
  000c26d6: mov r0,r5
  000c26d8: blx 0x000728a4
  000c26dc: mov r3,r0
  000c26de: mov r0,r4
  000c26e0: movs r1,#0x1c
  000c26e2: mov r2,r8
  000c26e4: str r6,[sp,#0x0]
  000c26e6: blx 0x00073f9c
  000c26ea: b 0x000c29da
  000c2700: ldr r1,[0x000c2ac8]
  000c2702: ldr r6,[0x000c2acc]
  000c2704: add r1,pc
  000c2706: ldr.w r0,[r11,#0x0]
  000c270a: ldr r4,[r1,#0x0]
  000c270c: mov r1,r6
  000c270e: blx r4
  000c2710: ldr r2,[0x000c2ad0]
  000c2712: movs r5,#0x0
  000c2714: ldr r1,[0x000c2ad4]
  000c2716: add r0,r2
  000c2718: str r0,[sp,#0x138]
  000c271a: ldr.w r0,[r11,#0x0]
  000c271e: mov r8,r2
  000c2720: strd r5,r1,[sp,#0x13c]
  000c2724: mov r1,r6
  000c2726: blx r4
  000c2728: ldr.w r1,[r11,#0x0]
  000c272c: add r0,r8
  000c272e: strd r0,r5,[sp,#0x144]
  000c2732: ldr r2,[0x000c2ad8]
  000c2734: mov r0,r1
  000c2736: mov r1,r6
  000c2738: str r2,[sp,#0x14c]
  000c273a: blx r4
  000c273c: add r0,r8
  000c273e: add r1,sp,#0x150
  000c2740: stmia r1!,{r0,r5,r6}
  000c2742: movs r0,#0x18
  000c2744: blx 0x0006eb24
  000c2748: mov r4,r0
  000c274a: add r1,sp,#0x138
  000c274c: movs r2,#0x3
  000c274e: blx 0x00073eb8
  000c2752: str.w r4,[r10,#0x110]
  000c2756: ldr r4,[sp,#0x6c]
  000c2758: ldr r0,[r4,#0x0]
  000c275a: blx 0x00072850
  000c275e: blx 0x00073ea0
  000c2762: ldr r0,[r4,#0x0]
  000c2764: blx 0x00071b84
  000c2768: mov r4,r0
  000c276a: mov r0,r9
  000c276c: blx 0x00074014
  000c2770: mov r1,r0
  000c2772: mov r0,r4
  000c2774: blx 0x00073eac
  000c2778: mov r0,r9
  000c277a: str.w r9,[sp,#0x70]
  000c277e: blx 0x00074014
  000c2782: mov r9,r0
  000c2784: movs r0,#0xc
  000c2786: blx 0x0006eb24
  000c278a: mov r4,r0
  000c278c: blx 0x00073d08
  000c2790: movs r0,#0x4
  000c2792: mov r1,r4
  000c2794: str.w r4,[r10,#0xf8]
  000c2798: blx 0x00073d14
  000c279c: mov.w r0,#0x138
  000c27a0: blx 0x0006eb24
  000c27a4: ldr r1,[0x000c2adc]
  000c27a6: mov r4,r0
  000c27a8: movs r0,#0x0
  000c27aa: movs r2,#0x0
  000c27ac: str r0,[sp,#0x0]
  000c27ae: mov r0,r4
  000c27b0: movw r3,#0x9c40
  000c27b4: blx 0x00073e4c
  000c27b8: mov r0,r10
  000c27ba: mov r1,r4
  000c27bc: movw r2,#0x4a88
  000c27c0: movs r3,#0x1
  000c27c2: blx 0x00073f6c
  000c27c6: ldr.w r1,[r10,#0xf8]
  000c27ca: mov.w r8,#0x0
  000c27ce: ldr r1,[r1,#0x4]
  000c27d0: str r0,[r1,#0x0]
  000c27d2: movs r1,#0x0
  000c27d4: ldr.w r0,[r10,#0xf8]
  000c27d8: ldr r0,[r0,#0x4]
  000c27da: ldr r0,[r0,#0x0]
  000c27dc: blx 0x00073f54
  000c27e0: ldr.w r0,[r10,#0xf8]
  000c27e4: movs r1,#0x2
  000c27e6: ldr r0,[r0,#0x4]
  000c27e8: ldr r0,[r0,#0x0]
  000c27ea: blx 0x00072748
  000c27ee: ldr r0,[0x000c2ae0]
  000c27f0: ldr.w r1,[r10,#0xf8]
  000c27f4: add r0,pc
  000c27f6: ldr r6,[r0,#0x0]
  000c27f8: ldr r1,[r1,#0x4]
  000c27fa: ldr r0,[r6,#0x0]
  000c27fc: ldr r4,[r1,#0x0]
  000c27fe: movw r1,#0xc88
  000c2802: blx 0x00072f70
  000c2806: add r5,sp,#0x8c
  000c2808: mov r1,r0
  000c280a: movs r2,#0x0
  000c280c: mov r0,r5
  000c280e: blx 0x0006f028
  000c2812: mov r0,r4
  000c2814: mov r1,r5
  000c2816: blx 0x00073fa8
  000c281a: add r0,sp,#0x8c
  000c281c: blx 0x0006ee30
  000c2820: mov.w r0,#0x138
  000c2824: blx 0x0006eb24
  000c2828: mov r4,r0
  000c282a: movw r1,#0x7530
  000c282e: movs r2,#0x0
  000c2830: movw r3,#0x9c40
  000c2834: str.w r8,[sp,#0x0]
  000c2838: blx 0x00073e4c
  000c283c: mov r0,r10
  000c283e: mov r1,r4
  000c2840: movw r2,#0x498e
  000c2844: movs r3,#0x1
  000c2846: blx 0x00073f6c
  000c284a: ldr.w r1,[r10,#0xf8]
  000c284e: ldr r1,[r1,#0x4]
  000c2850: str r0,[r1,#0x4]
  000c2852: movs r1,#0x0
  000c2854: ldr.w r0,[r10,#0xf8]
  000c2858: ldr r0,[r0,#0x4]
  000c285a: ldr r0,[r0,#0x4]
  000c285c: blx 0x00073f54
  000c2860: ldr.w r0,[r10,#0xf8]
  000c2864: movs r1,#0x1
  000c2866: ldr r0,[r0,#0x4]
  000c2868: ldr r0,[r0,#0x4]
  000c286a: blx 0x00072748
  000c286e: ldr.w r0,[r10,#0xf8]
  000c2872: ldr r1,[r0,#0x4]
  000c2874: ldr r0,[r6,#0x0]
  000c2876: ldr r4,[r1,#0x4]
  000c2878: movw r1,#0xc89
  000c287c: blx 0x00072f70
  000c2880: add r5,sp,#0x80
  000c2882: mov r1,r0
  000c2884: movs r2,#0x0
  000c2886: mov r0,r5
  000c2888: blx 0x0006f028
  000c288c: mov r0,r4
  000c288e: mov r1,r5
  000c2890: blx 0x00073fa8
  000c2894: add r0,sp,#0x80
  000c2896: blx 0x0006ee30
  000c289a: ldr r1,[0x000c2ae4]
  000c289c: add r0,sp,#0x110
  000c289e: movs r2,#0x24
  000c28a0: add r1,pc
  000c28a2: blx 0x0006f1e4
  000c28a6: ldr r0,[0x000c2ae8]
  000c28a8: add r0,pc
  000c28aa: vldr.64 d16,[r0]
  000c28ae: ldr r0,[r0,#0x8]
  000c28b0: str r0,[sp,#0x188]
  000c28b2: movs r0,#0xc
  000c28b4: vstr.64 d16,[sp,#0x180]
  000c28b8: blx 0x0006eb24
  000c28bc: mov r4,r0
  000c28be: blx 0x00073d08
  000c28c2: movs r0,#0x3
  000c28c4: mov r1,r4
  000c28c6: blx 0x00073d14
  000c28ca: ldr.w r0,[r10,#0xf8]
  000c28ce: ldr r1,[r4,#0x4]
  000c28d0: ldr r0,[r0,#0x4]
  000c28d2: ldr r0,[r0,#0x0]
  000c28d4: str r0,[r1,#0x0]
  000c28d6: ldr.w r0,[r10,#0xf8]
  000c28da: ldr r1,[r4,#0x4]
  000c28dc: ldr r0,[r0,#0x4]
  000c28de: ldr r0,[r0,#0x4]
  000c28e0: str r0,[r1,#0x8]
  000c28e2: movs r0,#0x18
  000c28e4: blx 0x0006eb24
  000c28e8: mov r10,r0
  000c28ea: movs r0,#0x9
  000c28ec: add r1,sp,#0x110
  000c28ee: add r3,sp,#0x180
  000c28f0: str r0,[sp,#0x0]
  000c28f2: mov r0,r10
  000c28f4: mov r2,r4
  000c28f6: blx 0x00073fc0
  000c28fa: mov r0,r10
  000c28fc: movs r1,#0x1
  000c28fe: blx 0x00073f48
  000c2902: mov r0,r10
  000c2904: blx 0x00073f3c
  000c2908: mov r11,r0
  000c290a: movs r6,#0x8
  000c290c: mov.w r8,#0x0
  000c2910: b 0x000c29ae
  000c2912: ldr r0,[0x000c2aec]
  000c2914: mov r1,r9
  000c2916: add r0,pc
  000c2918: ldr r0,[r0,#0x0]
  000c291a: ldr r0,[r0,#0x0]
  000c291c: blx 0x00071a34
  000c2920: mov r4,r0
  000c2922: cmp r6,#0x8
  000c2924: bne 0x000c2958
  000c2926: mov r0,r10
  000c2928: movs r1,#0x0
  000c292a: blx 0x00073ec4
  000c292e: str r0,[sp,#0x0]
  000c2930: movs r0,#0x1
  000c2932: strd r0,r8,[sp,#0x4]
  000c2936: mov r1,r9
  000c2938: ldr r5,[sp,#0x7c]
  000c293a: movs r2,#0x0
  000c293c: mov r3,r4
  000c293e: mov r0,r5
  000c2940: blx 0x00073e88
  000c2944: ldr.w r1,[r5,#0xf8]
  000c2948: ldr r1,[r1,#0x4]
  000c294a: str r0,[r1,#0x8]
  000c294c: mov r1,r10
  000c294e: ldr.w r0,[r5,#0xf8]
  000c2952: ldr r0,[r0,#0x4]
  000c2954: ldr r0,[r0,#0x8]
  000c2956: b 0x000c2988
  000c2958: mov r0,r11
  000c295a: movs r1,#0x0
  000c295c: blx 0x00073ec4
  000c2960: str r0,[sp,#0x0]
  000c2962: movs r0,#0x1
  000c2964: strd r0,r8,[sp,#0x4]
  000c2968: mov r1,r9
  000c296a: ldr r5,[sp,#0x7c]
  000c296c: movs r2,#0x0
  000c296e: mov r3,r4
  000c2970: mov r0,r5
  000c2972: blx 0x00073e88
  000c2976: ldr.w r1,[r5,#0xf8]
  000c297a: ldr r1,[r1,#0x4]
  000c297c: str r0,[r1,r6]
  000c297e: mov r1,r11
  000c2980: ldr.w r0,[r5,#0xf8]
  000c2984: ldr r0,[r0,#0x4]
  000c2986: ldr r0,[r0,r6]
  000c2988: blx 0x00073f60
  000c298c: ldr.w r0,[r5,#0xf8]
  000c2990: movs r1,#0x1
  000c2992: ldr r0,[r0,#0x4]
  000c2994: ldr r0,[r0,r6]
  000c2996: ldr r0,[r0,#0x4]
  000c2998: blx 0x00073fb4
  000c299c: ldr.w r0,[r5,#0xf8]
  000c29a0: movs r1,#0x1
  000c29a2: ldr r0,[r0,#0x4]
  000c29a4: ldr r0,[r0,r6]
  000c29a6: ldr r0,[r0,#0x4]
  000c29a8: blx 0x00074020
  000c29ac: adds r6,#0x4
  000c29ae: cmp r6,#0x10
  000c29b0: bne 0x000c2912
  000c29b2: movs r0,#0x1c
  000c29b4: blx 0x0006eb24
  000c29b8: mov r4,r0
  000c29ba: ldr r5,[sp,#0x70]
  000c29bc: mov r0,r5
  000c29be: blx 0x00072bb0
  000c29c2: ldr r6,[sp,#0x7c]
  000c29c4: mov r8,r0
  000c29c6: mov r0,r5
  000c29c8: blx 0x000728a4
  000c29cc: mov r3,r0
  000c29ce: mov r0,r4
  000c29d0: movs r1,#0x1d
  000c29d2: mov r2,r8
  000c29d4: str r6,[sp,#0x0]
  000c29d6: blx 0x00073f9c
  000c29da: str r4,[r6,#0x28]
  000c29dc: b.w 0x000c02ca
  000c29e0: movs r0,#0x1
  000c29e2: b 0x000c29e8
  000c29e4: movs r0,#0x0
  000c29e6: ldr r6,[sp,#0x5c]
  000c29e8: str r0,[sp,#0x3c]
  000c29ea: movs r0,#0x0
  000c29ec: str r0,[sp,#0x40]
  000c29ee: ldr r0,[r5,#0x0]
  000c29f0: blx 0x00071878
  000c29f4: cbz r0,0x000c2a0c
  000c29f6: movs r1,#0x0
  000c29f8: movs r0,#0x0
  000c29fa: strd r1,r1,[r10,#0x160]
  000c29fe: str r0,[sp,#0x3c]
  000c2a00: movs r0,#0x0
  000c2a02: str.w r1,[r10,#0x168]
  000c2a06: str.w r1,[r10,#0x16c]
  000c2a0a: str r0,[sp,#0x54]
  000c2a0c: cbz r6,0x000c2a20
  000c2a0e: mov r0,r8
  000c2a10: blx 0x0007402c
  000c2a14: ldr.w r1,[r10,#0x160]
  000c2a18: add r0,r1
  000c2a1a: adds r0,#0x1
  000c2a1c: str.w r0,[r10,#0x160]
  000c2a20: ldr r0,[sp,#0x40]
  000c2a22: movs r4,#0x7
  000c2a24: cmp r0,#0x0
  000c2a26: it ne
  000c2a28: mov.ne r4,#0x8
  000c2a2a: ldr r0,[sp,#0x3c]
  000c2a2c: cmp r0,#0x0
  000c2a2e: it eq
  000c2a30: mov.eq r4,r0
  000c2a32: ldr.w r9,[sp,#0x38]
  000c2a36: movs r0,#0xc
  000c2a38: cmp.w r9,#0x0
  000c2a3c: it ne
  000c2a3e: mov.ne.w r9,#0x5
  000c2a42: blx 0x0006eb24
  000c2a46: mov r8,r0
  000c2a48: blx 0x00073d08
  000c2a4c: vmov r1,s16
  000c2a50: ldrd r12,lr,[r10,#0x160]
  000c2a54: ldrd r2,r3,[r10,#0x168]
  000c2a58: ldr r0,[sp,#0x30]
  000c2a5a: str.w r8,[r10,#0xf8]
  000c2a5e: str.w r9,[sp,#0x28]
  000c2a62: str r4,[sp,#0x34]
  000c2a64: str r1,[sp,#0x14]
  000c2a66: mov r6,r1
  000c2a68: adds r6,r1,r0
  000c2a6a: ldr r0,[sp,#0x24]
  000c2a6c: str r6,[sp,#0x10]
  000c2a6e: mov r1,r8
  000c2a70: add r6,r0
  000c2a72: ldr r0,[sp,#0x2c]
  000c2a74: add r6,r0
  000c2a76: ldr r0,[sp,#0x20]
  000c2a78: add r6,r0
  000c2a7a: ldr r0,[sp,#0x18]
  000c2a7c: add r6,r0
  000c2a7e: add r6,r9
  000c2a80: add r6,r4
  000c2a82: add.w r0,r6,r12
  000c2a86: add r0,lr
  000c2a88: add r0,r2
  000c2a8a: add r0,r3
  000c2a8c: blx 0x00073d14
  000c2a90: vmov.f32 s16,0xbf000000
  000c2a94: ldr r4,[sp,#0x74]
  000c2a96: mov.w r11,#0x0
  000c2a9a: mov.w r8,#0x1
  000c2a9e: movs r1,#0x0
  000c2aa0: b 0x000c2cfc
  000c2aa2: mov r0,r1
  000c2aa4: cmp r1,#0x0
  000c2aa6: str r1,[sp,#0x68]
  000c2aa8: it ne
  000c2aaa: mov.ne r0,#0x1
  000c2aac: eor r1,r2,#0x1
  000c2ab0: orrs r0,r1
  000c2ab2: bne 0x000c2af0
  000c2ab4: mov r0,r4
  000c2ab6: blx 0x00074038
  000c2aba: cmp r0,#0x3
  000c2abc: bhi 0x000c2b38
  000c2abe: mov r0,r4
  000c2ac0: blx 0x00074038
  000c2ac4: mov r9,r0
  000c2ac6: b 0x000c2b3c
  000c2af0: cbz r2,0x000c2b00
  000c2af2: mov r0,r4
  000c2af4: blx 0x0007402c
  000c2af8: ldr r1,[sp,#0x68]
  000c2afa: cmp r1,r0
  000c2afc: ble.w 0x000c2cf6
  000c2b00: ldr r0,[0x000c2de0]
  000c2b02: ldr r5,[sp,#0x70]
  000c2b04: add r0,pc
  000c2b06: ldr r0,[r0,#0x0]
  000c2b08: mov r1,r5
  000c2b0a: ldr r0,[r0,#0x0]
  000c2b0c: blx 0x00071a34
  000c2b10: mov r4,r0
  000c2b12: ldr r0,[sp,#0x78]
  000c2b14: blx 0x000728bc
  000c2b18: stm sp,{r0,r8,r11}
  000c2b1c: mov r3,r4
  000c2b1e: mov r0,r10
  000c2b20: mov r1,r5
  000c2b22: movs r2,#0x0
  000c2b24: ldr r4,[sp,#0x74]
  000c2b26: blx 0x00073e88
  000c2b2a: ldr.w r1,[r10,#0xf8]
  000c2b2e: ldr r2,[sp,#0x68]
  000c2b30: ldr r1,[r1,#0x4]
  000c2b32: str.w r0,[r1,r2,lsl #0x2]
  000c2b36: b 0x000c2cf6
  000c2b38: mov.w r9,#0x8
  000c2b3c: mov r0,r4
  000c2b3e: blx 0x00074044
  000c2b42: mov r6,r0
  000c2b44: ldr r0,[sp,#0x78]
  000c2b46: blx 0x000728bc
  000c2b4a: stm sp,{r0,r8,r11}
  000c2b4e: mov r0,r10
  000c2b50: mov r1,r9
  000c2b52: movs r2,#0x0
  000c2b54: mov r3,r6
  000c2b56: blx 0x00073e88
  000c2b5a: ldr.w r1,[r10,#0xf8]
  000c2b5e: ldr r1,[r1,#0x4]
  000c2b60: str r0,[r1,#0x0]
  000c2b62: mov r0,r4
  000c2b64: blx 0x00074050
  000c2b68: mov r6,r0
  000c2b6a: ldr r0,[r5,#0x0]
  000c2b6c: blx 0x00071c5c
  000c2b70: cmp r0,#0x14
  000c2b72: ble 0x000c2b78
  000c2b74: movs r0,#0x14
  000c2b76: b 0x000c2b7e
  000c2b78: ldr r0,[r5,#0x0]
  000c2b7a: blx 0x00071c5c
  000c2b7e: rsb r1,r0,r0, lsl #0x4
  000c2b82: ldr r0,[r5,#0x0]
  000c2b84: adds r4,r1,r6
  000c2b86: blx 0x00073c48
  000c2b8a: cbz r0,0x000c2b90
  000c2b8c: movs r0,#0x2d
  000c2b8e: b 0x000c2b96
  000c2b90: ldr r0,[r5,#0x0]
  000c2b92: blx 0x00071770
  000c2b96: ldr r1,[sp,#0x50]
  000c2b98: add.w r0,r4,r0, lsl #0x2
  000c2b9c: mov r4,r10
  000c2b9e: vmov s2,r0
  000c2ba2: vldr.32 s0,[r1,#0x2c]
  000c2ba6: vcvt.f32.s32 s2,s2
  000c2baa: ldr.w r0,[r10,#0xf8]
  000c2bae: vadd.f32 s0,s0,s16
  000c2bb2: ldr r0,[r0,#0x4]
  000c2bb4: ldr r0,[r0,#0x0]
  000c2bb6: vmla.f32 s2,s0,s2
  000c2bba: ldr r0,[r0,#0x4]
  000c2bbc: vcvt.s32.f32 s0,s2
  000c2bc0: vmov r10,s0
  000c2bc4: mov r1,r10
  000c2bc6: blx 0x00073f78
  000c2bca: ldr.w r0,[r4,#0xf8]
  000c2bce: mov r1,r10
  000c2bd0: ldr r0,[r0,#0x4]
  000c2bd2: ldr r0,[r0,#0x0]
  000c2bd4: ldr r0,[r0,#0x4]
  000c2bd6: blx 0x000727e4
  000c2bda: add r5,sp,#0x138
  000c2bdc: ldr r1,[sp,#0x74]
  000c2bde: mov r0,r5
  000c2be0: blx 0x0007405c
  000c2be4: ldr.w r0,[r4,#0xf8]
  000c2be8: ldr r0,[r0,#0x4]
  000c2bea: ldr r0,[r0,#0x0]
  000c2bec: adds r0,#0x18
  000c2bee: mov r1,r5
  000c2bf0: blx 0x0006f2b0
  000c2bf4: mov r0,r5
  000c2bf6: blx 0x0006ee30
  000c2bfa: ldr.w r0,[r4,#0xf8]
  000c2bfe: ldr r0,[r0,#0x4]
  000c2c00: ldr r2,[r0,#0x0]
  000c2c02: ldr.w r0,[r2,#0x50]!
  000c2c06: cbz r0,0x000c2c1c
  000c2c08: blx 0x00070204
  000c2c0c: blx 0x0006eb48
  000c2c10: ldr.w r0,[r4,#0xf8]
  000c2c14: ldr r0,[r0,#0x4]
  000c2c16: ldr r0,[r0,#0x0]
  000c2c18: add.w r2,r0,#0x50
  000c2c1c: movs r0,#0xc
  000c2c1e: str.w r11,[r2,#0x0]
  000c2c22: blx 0x0006eb24
  000c2c26: mov r6,r0
  000c2c28: blx 0x000701f8
  000c2c2c: ldr r4,[sp,#0x7c]
  000c2c2e: ldr.w r0,[r4,#0xf8]
  000c2c32: ldr r0,[r0,#0x4]
  000c2c34: ldr r0,[r0,#0x0]
  000c2c36: str r6,[r0,#0x50]
  000c2c38: ldr r6,[sp,#0x74]
  000c2c3a: mov r0,r6
  000c2c3c: blx 0x00074068
  000c2c40: ldr.w r1,[r4,#0xf8]
  000c2c44: ldr r1,[r1,#0x4]
  000c2c46: ldr r1,[r1,#0x0]
  000c2c48: ldr r1,[r1,#0x50]
  000c2c4a: blx 0x0007021c
  000c2c4e: mov r0,r6
  000c2c50: blx 0x00074074
  000c2c54: ldr.w r1,[r4,#0xf8]
  000c2c58: ldr r1,[r1,#0x4]
  000c2c5a: ldr r1,[r1,#0x0]
  000c2c5c: ldr r1,[r1,#0x50]
  000c2c5e: blx 0x0007021c
  000c2c62: ldr.w r0,[r4,#0xf8]
  000c2c66: movs r5,#0x1
  000c2c68: ldr r0,[r0,#0x4]
  000c2c6a: ldr r0,[r0,#0x0]
  000c2c6c: strb.w r5,[r0,#0x42]
  000c2c70: mov r0,r6
  000c2c72: blx 0x00074080
  000c2c76: ldr.w r1,[r4,#0xf8]
  000c2c7a: ldr r1,[r1,#0x4]
  000c2c7c: ldr r1,[r1,#0x0]
  000c2c7e: ldr r2,[r1,#0x0]
  000c2c80: str r0,[r1,#0x48]
  000c2c82: mov r0,r1
  000c2c84: ldr r1,[0x000c2de4]
  000c2c86: ldr r2,[r2,#0x1c]
  000c2c88: blx r2
  000c2c8a: add.w r0,r10,r10, lsr #0x1f
  000c2c8e: mov r10,r4
  000c2c90: asrs r6,r0,#0x1
  000c2c92: b 0x000c2cea
  000c2c94: ldr r0,[0x000c2de8]
  000c2c96: mov r1,r9
  000c2c98: add r0,pc
  000c2c9a: ldr r0,[r0,#0x0]
  000c2c9c: ldr r0,[r0,#0x0]
  000c2c9e: blx 0x00071a34
  000c2ca2: mov r4,r0
  000c2ca4: ldr r0,[sp,#0x78]
  000c2ca6: blx 0x000728bc
  000c2caa: stm sp,{r0,r8,r11}
  000c2cae: mov r0,r10
  000c2cb0: mov r1,r9
  000c2cb2: movs r2,#0x0
  000c2cb4: mov r3,r4
  000c2cb6: blx 0x00073e88
  000c2cba: ldr.w r1,[r10,#0xf8]
  000c2cbe: ldr r1,[r1,#0x4]
  000c2cc0: str.w r0,[r1,r5,lsl #0x2]
  000c2cc4: mov r1,r6
  000c2cc6: ldr.w r0,[r10,#0xf8]
  000c2cca: ldr r0,[r0,#0x4]
  000c2ccc: ldr.w r0,[r0,r5,lsl #0x2]
  000c2cd0: ldr r0,[r0,#0x4]
  000c2cd2: blx 0x00073f78
  000c2cd6: ldr.w r0,[r10,#0xf8]
  000c2cda: mov r1,r6
  000c2cdc: ldr r0,[r0,#0x4]
  000c2cde: ldr.w r0,[r0,r5,lsl #0x2]
  000c2ce2: ldr r0,[r0,#0x4]
  000c2ce4: blx 0x000727e4
  000c2ce8: adds r5,#0x1
  000c2cea: ldr r4,[sp,#0x74]
  000c2cec: mov r0,r4
  000c2cee: blx 0x0007402c
  000c2cf2: cmp r5,r0
  000c2cf4: ble 0x000c2c94
  000c2cf6: ldr r1,[sp,#0x68]
  000c2cf8: ldr r5,[sp,#0x6c]
  000c2cfa: adds r1,#0x1
  000c2cfc: ldr.w r0,[r10,#0x160]
  000c2d00: ldr r2,[sp,#0x5c]
  000c2d02: cmp r1,r0
  000c2d04: blt.w 0x000c2aa2
  000c2d08: ldr r1,[0x000c2dec]
  000c2d0a: add.w r8,sp,#0x138
  000c2d0e: ldr r6,[sp,#0x7c]
  000c2d10: mov.w r10,#0x0
  000c2d14: add r1,pc
  000c2d16: ldr r5,[sp,#0x60]
  000c2d18: mov r9,r0
  000c2d1a: ldr r1,[r1,#0x0]
  000c2d1c: str r1,[sp,#0x78]
  000c2d1e: b 0x000c2dbe
  000c2d20: ldr r0,[sp,#0x78]
  000c2d22: ldr r4,[sp,#0x70]
  000c2d24: ldr r0,[r0,#0x0]
  000c2d26: mov r1,r4
  000c2d28: blx 0x00071a34
  000c2d2c: mov r3,r0
  000c2d2e: movs r0,#0x1
  000c2d30: str.w r10,[sp,#0x0]
  000c2d34: mov r1,r4
  000c2d36: strd r0,r10,[sp,#0x4]
  000c2d3a: mov r0,r6
  000c2d3c: movs r2,#0x0
  000c2d3e: blx 0x00073e88
  000c2d42: ldr.w r1,[r6,#0xf8]
  000c2d46: ldr r1,[r1,#0x4]
  000c2d48: str.w r0,[r1,r9,lsl #0x2]
  000c2d4c: ldr.w r0,[r6,#0xf8]
  000c2d50: ldr r0,[r0,#0x4]
  000c2d52: ldr.w r0,[r0,r9,lsl #0x2]
  000c2d56: blx 0x0007408c
  000c2d5a: ldr r4,[sp,#0x64]
  000c2d5c: ldr r1,[0x000c2df0]
  000c2d5e: ldr r0,[r4,#0x0]
  000c2d60: blx r5
  000c2d62: ldr r2,[0x000c2df4]
  000c2d64: ldr r1,[r4,#0x0]
  000c2d66: add r0,r2
  000c2d68: str r0,[sp,#0x138]
  000c2d6a: mov r0,r1
  000c2d6c: ldr r1,[0x000c2df8]
  000c2d6e: blx r5
  000c2d70: ldr r2,[0x000c2dfc]
  000c2d72: ldr r1,[r4,#0x0]
  000c2d74: add r0,r2
  000c2d76: str r0,[sp,#0x13c]
  000c2d78: mov r0,r1
  000c2d7a: ldr r1,[0x000c2e00]
  000c2d7c: blx r5
  000c2d7e: movw r1,#0xc350
  000c2d82: add r0,r1
  000c2d84: str r0,[sp,#0x140]
  000c2d86: movs r0,#0x18
  000c2d88: blx 0x0006eb24
  000c2d8c: mov r4,r0
  000c2d8e: mov r1,r8
  000c2d90: movs r2,#0x3
  000c2d92: blx 0x00073eb8
  000c2d96: ldr.w r0,[r6,#0xf8]
  000c2d9a: mov r1,r4
  000c2d9c: ldr r0,[r0,#0x4]
  000c2d9e: ldr.w r0,[r0,r9,lsl #0x2]
  000c2da2: blx 0x00073f60
  000c2da6: ldr.w r0,[r6,#0xf8]
  000c2daa: movs r1,#0x1
  000c2dac: ldr r0,[r0,#0x4]
  000c2dae: ldr.w r0,[r0,r9,lsl #0x2]
  000c2db2: blx 0x00074098
  000c2db6: ldr.w r0,[r6,#0x160]
  000c2dba: add.w r9,r9,#0x1
  000c2dbe: ldr r1,[sp,#0x4c]
  000c2dc0: ldr r1,[r1,#0x0]
  000c2dc2: add.w r11,r1,r0
  000c2dc6: cmp r9,r11
  000c2dc8: blt 0x000c2d20
  000c2dca: ldr r3,[sp,#0x70]
  000c2dcc: lsl.w r8,r11,#0x2
  000c2dd0: mov.w r9,#0x0
  000c2dd4: subs r2,r3,#0x2
  000c2dd6: it ne
  000c2dd8: mov.ne r2,r3
  000c2dda: str r2,[sp,#0x74]
  000c2ddc: b 0x000c3220
  000c2e04: subs.w r0,r11,r3
  000c2e08: mov r1,r0
  000c2e0a: it ne
  000c2e0c: mov.ne r1,#0x1
  000c2e0e: ldr r2,[sp,#0x3c]
  000c2e10: eor r2,r2,#0x1
  000c2e14: orrs r1,r2
  000c2e16: bne.w 0x000c2f2a
  000c2e1a: ldr r0,[sp,#0x40]
  000c2e1c: cmp r0,#0x0
  000c2e1e: beq.w 0x000c307a
  000c2e22: ldr r4,[sp,#0x64]
  000c2e24: ldr r6,[0x000c3178]
  000c2e26: ldr r5,[sp,#0x60]
  000c2e28: ldr r0,[r4,#0x0]
  000c2e2a: mov r1,r6
  000c2e2c: blx r5
  000c2e2e: movw r1,#0x9c40
  000c2e32: subs r1,r0,r1
  000c2e34: ldr r0,[r4,#0x0]
  000c2e36: vmov s0,r1
  000c2e3a: movw r1,#0x9c40
  000c2e3e: vcvt.f32.s32 s16,s0
  000c2e42: blx r5
  000c2e44: movw r1,#0x4e20
  000c2e48: subs r1,r0,r1
  000c2e4a: ldr r0,[r4,#0x0]
  000c2e4c: vmov s0,r1
  000c2e50: mov r1,r6
  000c2e52: vcvt.f32.s32 s18,s0
  000c2e56: blx r5
  000c2e58: ldr r1,[0x000c317c]
  000c2e5a: add r0,r1
  000c2e5c: vmov s0,r0
  000c2e60: mov.w r0,#0x138
  000c2e64: vcvt.f32.s32 s0,s0
  000c2e68: vstr.32 s16,[sp,#0x138]
  000c2e6c: vcvt.s32.f32 s16,s16
  000c2e70: vstr.32 s18,[sp,#0x13c]
  000c2e74: vcvt.s32.f32 s18,s18
  000c2e78: vstr.32 s0,[sp,#0x140]
  000c2e7c: vcvt.s32.f32 s20,s0
  000c2e80: blx 0x0006eb24
  000c2e84: mov r4,r0
  000c2e86: vmov r1,s16
  000c2e8a: str.w r9,[sp,#0x0]
  000c2e8e: vmov r2,s18
  000c2e92: vmov r3,s20
  000c2e96: blx 0x00073e4c
  000c2e9a: ldr r5,[sp,#0x7c]
  000c2e9c: mov r1,r4
  000c2e9e: movw r2,#0x4974
  000c2ea2: movs r3,#0x1
  000c2ea4: mov r0,r5
  000c2ea6: blx 0x00073f6c
  000c2eaa: ldr.w r1,[r5,#0xf8]
  000c2eae: ldr r1,[r1,#0x4]
  000c2eb0: str.w r0,[r1,r8]
  000c2eb4: movs r1,#0x0
  000c2eb6: ldr.w r0,[r5,#0xf8]
  000c2eba: ldr r0,[r0,#0x4]
  000c2ebc: ldr.w r0,[r0,r8]
  000c2ec0: blx 0x000740a4
  000c2ec4: ldr.w r0,[r5,#0xf8]
  000c2ec8: ldr r0,[r0,#0x4]
  000c2eca: ldr.w r0,[r0,r8]
  000c2ece: ldr r1,[r0,#0x0]
  000c2ed0: ldr r2,[r1,#0x44]
  000c2ed2: add r1,sp,#0x138
  000c2ed4: blx r2
  000c2ed6: ldr.w r0,[r5,#0xf8]
  000c2eda: movs r1,#0x0
  000c2edc: ldr r0,[r0,#0x4]
  000c2ede: ldr.w r0,[r0,r8]
  000c2ee2: blx 0x00073f54
  000c2ee6: ldr.w r0,[r5,#0xf8]
  000c2eea: ldr r1,[0x000c3180]
  000c2eec: ldr r0,[r0,#0x4]
  000c2eee: ldr.w r0,[r0,r8]
  000c2ef2: ldr r0,[r0,#0x4]
  000c2ef4: blx 0x000727e4
  000c2ef8: ldr.w r2,[r5,#0xf8]
  000c2efc: ldr.w r1,[r2,#0x4]!
  000c2f00: ldr.w r3,[r1,r8]
  000c2f04: ldr.w r0,[r3,#0x50]!
  000c2f08: cmp r0,#0x0
  000c2f0a: beq.w 0x000c3190
  000c2f0e: blx 0x00070204
  000c2f12: blx 0x0006eb48
  000c2f16: ldr.w r2,[r5,#0xf8]
  000c2f1a: ldr.w r0,[r2,#0x4]!
  000c2f1e: ldr.w r1,[r0,r8]
  000c2f22: add r0,r8
  000c2f24: add.w r3,r1,#0x50
  000c2f28: b 0x000c3194
  000c2f2a: cmp r11,r3
  000c2f2c: it ne
  000c2f2e: mov.ne r0,#0x1
  000c2f30: ldr r1,[sp,#0x38]
  000c2f32: eor r1,r1,#0x1
  000c2f36: orrs r0,r1
  000c2f38: bne.w 0x000c30a4
  000c2f3c: ldr r4,[sp,#0x64]
  000c2f3e: ldr r6,[0x000c3178]
  000c2f40: ldr r5,[sp,#0x60]
  000c2f42: ldr r0,[r4,#0x0]
  000c2f44: mov r1,r6
  000c2f46: blx r5
  000c2f48: movw r1,#0x9c40
  000c2f4c: subs r1,r0,r1
  000c2f4e: ldr r0,[r4,#0x0]
  000c2f50: vmov s0,r1
  000c2f54: movw r1,#0x9c40
  000c2f58: vcvt.f32.s32 s16,s0
  000c2f5c: blx r5
  000c2f5e: movw r1,#0x4e20
  000c2f62: subs r1,r0,r1
  000c2f64: ldr r0,[r4,#0x0]
  000c2f66: vmov s0,r1
  000c2f6a: mov r1,r6
  000c2f6c: vcvt.f32.s32 s18,s0
  000c2f70: blx r5
  000c2f72: movw r1,#0xea60
  000c2f76: add r0,r1
  000c2f78: vmov s0,r0
  000c2f7c: mov.w r0,#0x138
  000c2f80: vcvt.f32.s32 s0,s0
  000c2f84: vstr.32 s16,[sp,#0x138]
  000c2f88: vcvt.s32.f32 s16,s16
  000c2f8c: vstr.32 s18,[sp,#0x13c]
  000c2f90: vcvt.s32.f32 s18,s18
  000c2f94: vstr.32 s0,[sp,#0x140]
  000c2f98: vcvt.s32.f32 s20,s0
  000c2f9c: blx 0x0006eb24
  000c2fa0: mov r4,r0
  000c2fa2: vmov r1,s16
  000c2fa6: str.w r9,[sp,#0x0]
  000c2faa: vmov r2,s18
  000c2fae: vmov r3,s20
  000c2fb2: blx 0x00073e4c
  000c2fb6: ldr r6,[sp,#0x7c]
  000c2fb8: mov r1,r4
  000c2fba: movw r2,#0x4a6b
  000c2fbe: movs r3,#0x1
  000c2fc0: mov r0,r6
  000c2fc2: blx 0x00073f6c
  000c2fc6: ldr.w r1,[r6,#0xf8]
  000c2fca: ldr r1,[r1,#0x4]
  000c2fcc: str.w r0,[r1,r8]
  000c2fd0: movs r1,#0x0
  000c2fd2: ldr.w r0,[r6,#0xf8]
  000c2fd6: ldr r0,[r0,#0x4]
  000c2fd8: ldr.w r0,[r0,r8]
  000c2fdc: blx 0x000740a4
  000c2fe0: ldr.w r0,[r6,#0xf8]
  000c2fe4: ldr r0,[r0,#0x4]
  000c2fe6: ldr.w r0,[r0,r8]
  000c2fea: ldr r1,[r0,#0x0]
  000c2fec: ldr r2,[r1,#0x44]
  000c2fee: add r1,sp,#0x138
  000c2ff0: blx r2
  000c2ff2: ldr.w r0,[r6,#0xf8]
  000c2ff6: movs r1,#0x0
  000c2ff8: ldr r0,[r0,#0x4]
  000c2ffa: ldr.w r0,[r0,r8]
  000c2ffe: blx 0x00073f54
  000c3002: ldr.w r0,[r6,#0xf8]
  000c3006: ldr r1,[0x000c3180]
  000c3008: ldr r0,[r0,#0x4]
  000c300a: ldr.w r0,[r0,r8]
  000c300e: ldr r0,[r0,#0x4]
  000c3010: blx 0x000727e4
  000c3014: ldr r0,[0x000c3184]
  000c3016: ldr.w r1,[r6,#0xf8]
  000c301a: add r0,pc
  000c301c: ldr r0,[r0,#0x0]
  000c301e: ldr r1,[r1,#0x4]
  000c3020: ldr r0,[r0,#0x0]
  000c3022: ldr.w r4,[r1,r8]
  000c3026: movw r1,#0x683
  000c302a: blx 0x00072f70
  000c302e: add r5,sp,#0xb0
  000c3030: mov r1,r0
  000c3032: movs r2,#0x0
  000c3034: mov r0,r5
  000c3036: blx 0x0006f028
  000c303a: mov r0,r4
  000c303c: mov r1,r5
  000c303e: blx 0x00073fa8
  000c3042: mov r0,r5
  000c3044: blx 0x0006ee30
  000c3048: ldr.w r0,[r6,#0xf8]
  000c304c: ldr r0,[r0,#0x4]
  000c304e: ldr.w r2,[r0,r8]
  000c3052: ldr.w r0,[r2,#0x50]!
  000c3056: cmp r0,#0x0
  000c3058: strb.w r9,[r2,#0x20]
  000c305c: beq 0x000c3074
  000c305e: blx 0x00070204
  000c3062: blx 0x0006eb48
  000c3066: ldr.w r0,[r6,#0xf8]
  000c306a: ldr r0,[r0,#0x4]
  000c306c: ldr.w r0,[r0,r8]
  000c3070: add.w r2,r0,#0x50
  000c3074: str.w r9,[r2,#0x0]
  000c3078: b 0x000c3214
  000c307a: movs r0,#0x1
  000c307c: str.w r9,[sp,#0x0]
  000c3080: strd r0,r9,[sp,#0x4]
  000c3084: movs r2,#0x1
  000c3086: ldr r6,[sp,#0x7c]
  000c3088: movs r3,#0xe
  000c308a: ldr r1,[sp,#0x70]
  000c308c: mov r0,r6
  000c308e: blx 0x00073e88
  000c3092: ldr.w r1,[r6,#0xf8]
  000c3096: ldr r1,[r1,#0x4]
  000c3098: str.w r0,[r1,r8]
  000c309c: ldr.w r0,[r6,#0xf8]
  000c30a0: adds r2,r0,#0x4
  000c30a2: b 0x000c31a0
  000c30a4: ldr r4,[sp,#0x64]
  000c30a6: movs r1,#0x64
  000c30a8: ldr.w r10,[sp,#0x60]
  000c30ac: ldr r0,[r4,#0x0]
  000c30ae: blx r10
  000c30b0: movs r1,#0x1
  000c30b2: str.w r9,[sp,#0x0]
  000c30b6: strd r1,r9,[sp,#0x4]
  000c30ba: cmp r0,#0x1d
  000c30bc: ldr r2,[sp,#0x70]
  000c30be: mov.w r0,#0x2
  000c30c2: mov.w r3,#0xf
  000c30c6: it gt
  000c30c8: mov.gt r0,r2
  000c30ca: ldr r1,[sp,#0x74]
  000c30cc: it gt
  000c30ce: mov.gt r1,r0
  000c30d0: cmp r2,#0x0
  000c30d2: it eq
  000c30d4: mov.eq r1,r0
  000c30d6: movs r2,#0x1
  000c30d8: cmp r1,#0x1
  000c30da: it eq
  000c30dc: mov.eq r3,#0xd
  000c30de: ldr r6,[sp,#0x7c]
  000c30e0: mov r0,r6
  000c30e2: blx 0x00073e88
  000c30e6: ldr.w r1,[r6,#0xf8]
  000c30ea: ldr r1,[r1,#0x4]
  000c30ec: str.w r0,[r1,r8]
  000c30f0: movs r1,#0x1
  000c30f2: ldr.w r0,[r6,#0xf8]
  000c30f6: ldr r0,[r0,#0x4]
  000c30f8: ldr.w r0,[r0,r8]
  000c30fc: blx 0x00073f54
  000c3100: ldr.w r0,[r6,#0xf8]
  000c3104: ldr r1,[r0,#0x4]
  000c3106: ldr r0,[r4,#0x0]
  000c3108: ldr.w r9,[r1,r8]
  000c310c: movw r1,#0xea60
  000c3110: blx r10
  000c3112: ldr r1,[r4,#0x0]
  000c3114: ldr r2,[0x000c3188]
  000c3116: adds r5,r0,r2
  000c3118: mov r0,r1
  000c311a: movs r1,#0x2
  000c311c: blx r10
  000c311e: ldr r1,[r4,#0x0]
  000c3120: cmp r0,#0x0
  000c3122: mov.w r0,#0xffffffff
  000c3126: it eq
  000c3128: mov.eq r0,#0x1
  000c312a: muls r5,r0
  000c312c: mov r0,r1
  000c312e: movw r1,#0x9c40
  000c3132: blx r10
  000c3134: movw r1,#0x4e20
  000c3138: vmov s0,r5
  000c313c: subs r1,r0,r1
  000c313e: ldr r0,[r4,#0x0]
  000c3140: vcvt.f32.s32 s16,s0
  000c3144: vmov s0,r1
  000c3148: ldr r1,[0x000c318c]
  000c314a: vcvt.f32.s32 s18,s0
  000c314e: blx r10
  000c3150: ldr r1,[0x000c3188]
  000c3152: vmov r2,s18
  000c3156: add r0,r1
  000c3158: vmov r1,s16
  000c315c: vmov s0,r0
  000c3160: vcvt.f32.s32 s0,s0
  000c3164: ldr.w r0,[r9,#0x0]
  000c3168: ldr r5,[r0,#0x48]
  000c316a: mov r0,r9
  000c316c: mov.w r9,#0x0
  000c3170: vmov r3,s0
  000c3174: blx r5
  000c3176: b 0x000c3214
  000c3190: add.w r0,r1,r11, lsl #0x2
  000c3194: str.w r9,[r3,#0x0]
  000c3198: ldr r0,[r0,#0x0]
  000c319a: ldr r6,[sp,#0x7c]
  000c319c: strb.w r9,[r0,#0x70]
  000c31a0: ldr r0,[r2,#0x0]
  000c31a2: movs r1,#0x0
  000c31a4: ldr.w r0,[r0,r8]
  000c31a8: blx 0x00073f54
  000c31ac: ldr.w r0,[r6,#0xf8]
  000c31b0: ldr r5,[sp,#0x64]
  000c31b2: ldr r4,[sp,#0x60]
  000c31b4: ldr r1,[r0,#0x4]
  000c31b6: ldr r0,[r5,#0x0]
  000c31b8: ldr.w r10,[r1,r8]
  000c31bc: ldr r1,[0x000c3590]
  000c31be: mov r9,r1
  000c31c0: blx r4
  000c31c2: movw r1,#0x9c40
  000c31c6: subs r1,r0,r1
  000c31c8: ldr r0,[r5,#0x0]
  000c31ca: vmov s0,r1
  000c31ce: movw r1,#0x2710
  000c31d2: vcvt.f32.s32 s16,s0
  000c31d6: blx r4
  000c31d8: movw r1,#0x1388
  000c31dc: subs r1,r0,r1
  000c31de: ldr r0,[r5,#0x0]
  000c31e0: vmov s0,r1
  000c31e4: mov r1,r9
  000c31e6: vcvt.f32.s32 s18,s0
  000c31ea: blx r4
  000c31ec: movw r1,#0x9c40
  000c31f0: add r0,r1
  000c31f2: vmov r1,s16
  000c31f6: vmov s0,r0
  000c31fa: vmov r2,s18
  000c31fe: vcvt.f32.s32 s0,s0
  000c3202: ldr.w r0,[r10,#0x0]
  000c3206: ldr r5,[r0,#0x48]
  000c3208: mov r0,r10
  000c320a: vmov r3,s0
  000c320e: blx r5
  000c3210: mov.w r9,#0x0
  000c3214: ldrd r0,r1,[r6,#0x160]
  000c3218: add.w r8,r8,#0x4
  000c321c: add.w r11,r11,#0x1
  000c3220: ldr r2,[sp,#0x48]
  000c3222: adds r3,r1,r0
  000c3224: ldr r2,[r2,#0x0]
  000c3226: adds r4,r3,r2
  000c3228: cmp r11,r4
  000c322a: blt.w 0x000c2e04
  000c322e: ldr.w r10,[sp,#0x34]
  000c3232: ldr r4,[sp,#0x7c]
  000c3234: cmp.w r10,#0x0
  000c3238: beq.w 0x000c3398
  000c323c: adr r0,[0xc3598]
  000c323e: vmov.i32 q8,#0x0
  000c3242: vld1.64 {d18,d19},[r0]
  000c3246: add r0,sp,#0x138
  000c3248: mov r1,r0
  000c324a: orr r0,r0,#0x4
  000c324e: vst1.32 {d16,d17},[r0]
  000c3252: add.w r0,r1,#0x18
  000c3256: vst1.64 {d16,d17},[r0]
  000c325a: add.w r0,r1,#0x28
  000c325e: vst1.64 {d18,d19},[r0]
  000c3262: mov.w r0,#0x3f800000
  000c3266: str r0,[sp,#0x138]
  000c3268: str r0,[sp,#0x14c]
  000c326a: str r0,[sp,#0x170]
  000c326c: movs r0,#0x0
  000c326e: strd r0,r0,[sp,#0x174]
  000c3272: str r0,[sp,#0x17c]
  000c3274: ldr.w r0,[r4,#0xf8]
  000c3278: ldr r0,[r0,#0x4]
  000c327a: ldr.w r1,[r0,r3,lsl #0x2]
  000c327e: ldr r0,[r1,#0x0]
  000c3280: ldr r2,[r0,#0x28]
  000c3282: add r0,sp,#0xe0
  000c3284: blx r2
  000c3286: add.w r2,r4,#0x160
  000c328a: add r5,sp,#0xd4
  000c328c: mov.w r9,#0x0
  000c3290: ldmia r2,{r0,r1,r2}
  000c3292: adds r3,r1,r0
  000c3294: add.w r11,r3,r2
  000c3298: b 0x000c338a
  000c329a: mov r0,r4
  000c329c: movs r1,#0x0
  000c329e: movw r2,#0x1a74
  000c32a2: movs r3,#0x1
  000c32a4: blx 0x00073f6c
  000c32a8: ldr.w r1,[r4,#0xf8]
  000c32ac: ldr r1,[r1,#0x4]
  000c32ae: str.w r0,[r1,r11,lsl #0x2]
  000c32b2: mov.w r1,#0x3e8
  000c32b6: ldr.w r0,[r4,#0xf8]
  000c32ba: ldr r0,[r0,#0x4]
  000c32bc: ldr.w r6,[r0,r11,lsl #0x2]
  000c32c0: ldr r0,[r6,#0x4]
  000c32c2: blx 0x00073f78
  000c32c6: ldr r0,[sp,#0x40]
  000c32c8: cbz r0,0x000c32d0
  000c32ca: ldr r1,[0x000c35a8]
  000c32cc: add r1,pc
  000c32ce: b 0x000c32d4
  000c32d0: ldr r1,[0x000c35ac]
  000c32d2: add r1,pc
  000c32d4: add.w r0,r1,r9
  000c32d8: mov r10,r1
  000c32da: add r4,sp,#0x138
  000c32dc: ldr r1,[r0,#0x8]
  000c32de: ldr r2,[r0,#0x10]
  000c32e0: str r1,[sp,#0x74]
  000c32e2: str r2,[sp,#0xcc]
  000c32e4: add r2,sp,#0xc8
  000c32e6: ldr r1,[r0,#0xc]
  000c32e8: ldr.w r8,[r0,#0x4]
  000c32ec: ldr r0,[r0,#0x14]
  000c32ee: str r1,[sp,#0xc8]
  000c32f0: mov r1,r4
  000c32f2: str r0,[sp,#0xd0]
  000c32f4: mov r0,r5
  000c32f6: blx 0x0006f694
  000c32fa: add r4,sp,#0x174
  000c32fc: mov r1,r5
  000c32fe: mov r0,r4
  000c3300: blx 0x0006eb3c
  000c3304: ldr r0,[r6,#0x8]
  000c3306: mov r1,r4
  000c3308: blx 0x00073dd4
  000c330c: ldr.w r0,[r10,r9]
  000c3310: add r1,sp,#0x138
  000c3312: str.w r8,[sp,#0xcc]
  000c3316: str r0,[sp,#0xc8]
  000c3318: add r2,sp,#0xc8
  000c331a: ldr r0,[sp,#0x74]
  000c331c: str r0,[sp,#0xd0]
  000c331e: mov r0,r5
  000c3320: blx 0x0006f694
  000c3324: mov r0,r4
  000c3326: mov r1,r5
  000c3328: blx 0x0006eb3c
  000c332c: vldr.32 s0,[sp,#0x174]
  000c3330: mov r1,r5
  000c3332: vldr.32 s6,[sp,#0xe0]
  000c3336: vldr.32 s2,[sp,#0x178]
  000c333a: vldr.32 s8,[sp,#0xe4]
  000c333e: vadd.f32 s0,s6,s0
  000c3342: vldr.32 s4,[sp,#0x17c]
  000c3346: vldr.32 s10,[sp,#0xe8]
  000c334a: vadd.f32 s2,s8,s2
  000c334e: vadd.f32 s4,s10,s4
  000c3352: vstr.32 s0,[sp,#0xd4]
  000c3356: vstr.32 s2,[sp,#0xd8]
  000c335a: vstr.32 s4,[sp,#0xdc]
  000c335e: ldr r0,[r6,#0x0]
  000c3360: ldr r2,[r0,#0x44]
  000c3362: mov r0,r6
  000c3364: blx r2
  000c3366: ldr r1,[0x000c35b0]
  000c3368: mov r0,r6
  000c336a: blx 0x00073f84
  000c336e: ldr r4,[sp,#0x7c]
  000c3370: movs r0,#0x0
  000c3372: str r0,[r6,#0x50]
  000c3374: add.w r9,r9,#0x18
  000c3378: ldr r0,[sp,#0x70]
  000c337a: add.w r2,r4,#0x160
  000c337e: str r0,[r6,#0x28]
  000c3380: add.w r11,r11,#0x1
  000c3384: ldmia r2,{r0,r1,r2}
  000c3386: ldr.w r10,[sp,#0x34]
  000c338a: add.w r3,r0,r10
  000c338e: add r3,r1
  000c3390: add r3,r2
  000c3392: cmp r11,r3
  000c3394: blt.w 0x000c329a
  000c3398: ldr r3,[sp,#0x38]
  000c339a: cmp r3,#0x0
  000c339c: beq.w 0x000c3752
  000c33a0: adr r2,[0xc3598]
  000c33a2: vmov.i32 q8,#0x0
  000c33a6: vld1.64 {d18,d19},[r2]
  000c33aa: add r2,sp,#0x138
  000c33ac: add r0,r1
  000c33ae: mov r3,r2
  000c33b0: orr r2,r2,#0x4
  000c33b4: vst1.32 {d16,d17},[r2]
  000c33b8: add.w r2,r3,#0x18
  000c33bc: vst1.64 {d16,d17},[r2]
  000c33c0: add.w r2,r3,#0x28
  000c33c4: vst1.64 {d18,d19},[r2]
  000c33c8: mov.w r2,#0x3f800000
  000c33cc: str r2,[sp,#0x138]
  000c33ce: str r2,[sp,#0x14c]
  000c33d0: str r2,[sp,#0x170]
  000c33d2: movs r2,#0x0
  000c33d4: strd r2,r2,[sp,#0x174]
  000c33d8: str r2,[sp,#0x17c]
  000c33da: ldr.w r2,[r4,#0xf8]
  000c33de: ldr r2,[r2,#0x4]
  000c33e0: ldr.w r1,[r2,r0,lsl #0x2]
  000c33e4: ldr r0,[r1,#0x0]
  000c33e6: ldr r2,[r0,#0x28]
  000c33e8: add r0,sp,#0xe0
  000c33ea: blx r2
  000c33ec: add.w r2,r4,#0x160
  000c33f0: ldmia r2,{r0,r1,r2}
  000c33f2: ldr r6,[0x000c35b4]
  000c33f4: adds r3,r1,r0
  000c33f6: ldr r5,[sp,#0x7c]
  000c33f8: add r6,pc
  000c33fa: adds r4,r3,r2
  000c33fc: b 0x000c34d2
  000c33fe: mov r0,r5
  000c3400: movs r1,#0x0
  000c3402: movw r2,#0x1a76
  000c3406: movs r3,#0x1
  000c3408: blx 0x00073f6c
  000c340c: ldr.w r1,[r5,#0xf8]
  000c3410: str r4,[sp,#0x74]
  000c3412: ldr r1,[r1,#0x4]
  000c3414: str.w r0,[r1,r4,lsl #0x2]
  000c3418: ldr.w r0,[r5,#0xf8]
  000c341c: ldr r1,[sp,#0x74]
  000c341e: ldr r0,[r0,#0x4]
  000c3420: ldr.w r4,[r0,r1,lsl #0x2]
  000c3424: mov.w r1,#0x3e8
  000c3428: ldr r0,[r4,#0x4]
  000c342a: blx 0x00073f78
  000c342e: add.w r2,r6,#0xc
  000c3432: add.w r9,sp,#0xd4
  000c3436: add.w r8,sp,#0x138
  000c343a: add.w r10,sp,#0xc8
  000c343e: ldmia r2,{r0,r1,r2}
  000c3440: add r3,sp,#0xc8
  000c3442: stmia r3!,{r0,r1,r2}
  000c3444: mov r0,r9
  000c3446: mov r1,r8
  000c3448: mov r2,r10
  000c344a: blx 0x0006f694
  000c344e: add.w r11,sp,#0x174
  000c3452: mov r1,r9
  000c3454: mov r0,r11
  000c3456: blx 0x0006eb3c
  000c345a: ldr r0,[r4,#0x8]
  000c345c: mov r1,r11
  000c345e: blx 0x00073dd4
  000c3462: ldm.w r6,{r0,r1,r2}
  000c3466: add r3,sp,#0xc8
  000c3468: stmia r3!,{r0,r1,r2}
  000c346a: mov r0,r9
  000c346c: mov r1,r8
  000c346e: mov r2,r10
  000c3470: blx 0x0006f694
  000c3474: mov r0,r11
  000c3476: mov r1,r9
  000c3478: blx 0x0006eb3c
  000c347c: vldr.32 s0,[sp,#0x174]
  000c3480: mov r1,r9
  000c3482: vldr.32 s6,[sp,#0xe0]
  000c3486: vldr.32 s2,[sp,#0x178]
  000c348a: vldr.32 s8,[sp,#0xe4]
  000c348e: vadd.f32 s0,s6,s0
  000c3492: vldr.32 s4,[sp,#0x17c]
  000c3496: vldr.32 s10,[sp,#0xe8]
  000c349a: vadd.f32 s2,s8,s2
  000c349e: vadd.f32 s4,s10,s4
  000c34a2: vstr.32 s0,[sp,#0xd4]
  000c34a6: vstr.32 s2,[sp,#0xd8]
  000c34aa: vstr.32 s4,[sp,#0xdc]
  000c34ae: ldr r0,[r4,#0x0]
  000c34b0: ldr r2,[r0,#0x44]
  000c34b2: mov r0,r4
  000c34b4: blx r2
  000c34b6: ldr r1,[0x000c35b0]
  000c34b8: mov r0,r4
  000c34ba: blx 0x00073f84
  000c34be: movs r0,#0x0
  000c34c0: add.w r2,r5,#0x160
  000c34c4: str r0,[r4,#0x50]
  000c34c6: movs r0,#0x1
  000c34c8: str r0,[r4,#0x28]
  000c34ca: adds r6,#0x18
  000c34cc: ldr r4,[sp,#0x74]
  000c34ce: ldmia r2,{r0,r1,r2}
  000c34d0: adds r4,#0x1
  000c34d2: ldr r3,[sp,#0x28]
  000c34d4: add r3,r0
  000c34d6: add r3,r1
  000c34d8: add r3,r2
  000c34da: cmp r4,r3
  000c34dc: blt 0x000c33fe
  000c34de: ldr r4,[sp,#0x7c]
  000c34e0: ldr.w r8,[sp,#0x6c]
  000c34e4: ldr.w r10,[sp,#0x34]
  000c34e8: b 0x000c3756
  000c34ea: movs r0,#0x0
  000c34ec: str r0,[sp,#0x64]
  000c34ee: vmov.f32 s18,0x3f800000
  000c34f2: ldr r0,[0x000c35b8]
  000c34f4: vldr.32 s16,[pc,#0xc4]
  000c34f8: movs r6,#0x0
  000c34fa: add r0,pc
  000c34fc: vldr.64 d10,[pc,#0xc0]
  000c3500: vldr.32 s22,[pc,#0xc4]
  000c3504: ldr r0,[r0,#0x0]
  000c3506: str r0,[sp,#0x60]
  000c3508: str.w r8,[sp,#0x68]
  000c350c: b 0x000c36a2
  000c350e: cmp r6,r10
  000c3510: bge 0x000c35cc
  000c3512: ldr r0,[sp,#0x60]
  000c3514: ldr.w r8,[sp,#0x70]
  000c3518: ldr r0,[r0,#0x0]
  000c351a: mov r1,r8
  000c351c: blx 0x00071a34
  000c3520: mov r4,r0
  000c3522: ldr r0,[sp,#0x7c]
  000c3524: mov r5,r10
  000c3526: ldr.w r9,[r11,#0x0]
  000c352a: ldr.w r10,[r0,#0x110]
  000c352e: mov r0,r10
  000c3530: blx 0x00073fcc
  000c3534: mov r1,r0
  000c3536: mov r0,r9
  000c3538: blx 0x00071848
  000c353c: mov r1,r0
  000c353e: mov r0,r10
  000c3540: mov r10,r5
  000c3542: ldr r5,[sp,#0x7c]
  000c3544: blx 0x00073ec4
  000c3548: str r0,[sp,#0x0]
  000c354a: movs r0,#0x1
  000c354c: str r0,[sp,#0x4]
  000c354e: movs r0,#0x0
  000c3550: str r0,[sp,#0x8]
  000c3552: mov r1,r8
  000c3554: mov r0,r5
  000c3556: movs r2,#0x0
  000c3558: mov r3,r4
  000c355a: ldr.w r8,[sp,#0x68]
  000c355e: blx 0x00073e88
  000c3562: ldr.w r1,[r5,#0xf8]
  000c3566: ldr r1,[r1,#0x4]
  000c3568: str.w r0,[r1,r6,lsl #0x2]
  000c356c: ldr.w r0,[r5,#0xf8]
  000c3570: ldr r0,[r0,#0x4]
  000c3572: ldr.w r0,[r0,r6,lsl #0x2]
  000c3576: blx 0x000732dc
  000c357a: ldr.w r0,[r5,#0xf8]
  000c357e: movs r1,#0x1
  000c3580: ldr r0,[r0,#0x4]
  000c3582: ldr.w r0,[r0,r6,lsl #0x2]
  000c3586: ldr r0,[r0,#0x4]
  000c3588: blx 0x00073e94
  000c358c: b 0x000c36a0
  000c35cc: movs r0,#0x0
  000c35ce: movs r1,#0x1
  000c35d0: str r0,[sp,#0x0]
  000c35d2: movs r2,#0x1
  000c35d4: strd r1,r0,[sp,#0x4]
  000c35d8: mov r0,r5
  000c35da: ldr r1,[sp,#0x64]
  000c35dc: mov r3,r8
  000c35de: blx 0x00073e88
  000c35e2: ldr.w r1,[r5,#0xf8]
  000c35e6: ldr r4,[sp,#0x6c]
  000c35e8: ldr r1,[r1,#0x4]
  000c35ea: str.w r0,[r1,r6,lsl #0x2]
  000c35ee: ldr.w r0,[r5,#0xf8]
  000c35f2: ldr r0,[r0,#0x4]
  000c35f4: ldr.w r1,[r0,r6,lsl #0x2]
  000c35f8: ldr r0,[r4,#0x0]
  000c35fa: ldr.w r9,[r1,#0x4]
  000c35fe: blx 0x00071c5c
  000c3602: cmp r0,#0x14
  000c3604: ble 0x000c360a
  000c3606: movs r0,#0x14
  000c3608: b 0x000c3610
  000c360a: ldr r0,[r4,#0x0]
  000c360c: blx 0x00071c5c
  000c3610: lsls r0,r0,#0x1
  000c3612: vmov s0,r0
  000c3616: vcvt.f32.s32 s0,s0
  000c361a: ldr r0,[r4,#0x0]
  000c361c: vadd.f32 s24,s0,s16
  000c3620: blx 0x00073c48
  000c3624: cbz r0,0x000c362a
  000c3626: movs r0,#0x2d
  000c3628: b 0x000c3630
  000c362a: ldr r0,[r4,#0x0]
  000c362c: blx 0x00071770
  000c3630: lsls r0,r0,#0x1
  000c3632: vmov s0,r0
  000c3636: ldr r0,[sp,#0x74]
  000c3638: vldr.32 s2,[r0,#0x2c]
  000c363c: vcvt.f32.s32 s0,s0
  000c3640: mov r0,r9
  000c3642: vcvt.f64.f32 d16,s2
  000c3646: vcmpe.f64 d16,d10
  000c364a: vmrs apsr,fpscr
  000c364e: vadd.f32 s0,s24,s0
  000c3652: vmov.f32 s2,s18
  000c3656: it gt
  000c3658: vmov.gt.f32 s2,s22
  000c365c: vmul.f32 s0,s0,s2
  000c3660: vcvt.s32.f32 s0,s0
  000c3664: vmov r1,s0
  000c3668: blx 0x00073f78
  000c366c: ldr.w r0,[r5,#0xf8]
  000c3670: movs r1,#0x1
  000c3672: ldr r0,[r0,#0x4]
  000c3674: ldr.w r0,[r0,r6,lsl #0x2]
  000c3678: blx 0x00073f54
  000c367c: ldr.w r0,[r5,#0xf8]
  000c3680: movs r1,#0x1
  000c3682: ldr r0,[r0,#0x4]
  000c3684: ldr.w r0,[r0,r6,lsl #0x2]
  000c3688: ldr r0,[r0,#0x4]
  000c368a: blx 0x00073fb4
  000c368e: ldr.w r0,[r5,#0xf8]
  000c3692: movs r1,#0x0
  000c3694: ldr r0,[r0,#0x4]
  000c3696: ldr.w r0,[r0,r6,lsl #0x2]
  000c369a: str r1,[r0,#0x50]
  000c369c: strb.w r1,[r0,#0x4c]
  000c36a0: adds r6,#0x1
  000c36a2: ldr.w r0,[r5,#0xf8]
  000c36a6: ldr r1,[r0,#0x0]
  000c36a8: cmp r6,r1
  000c36aa: bcc.w 0x000c350e
  000c36ae: ldr r0,[r0,#0x4]
  000c36b0: ldr r1,[0x000c3a98]
  000c36b2: ldr r2,[0x000c3a9c]
  000c36b4: ldr.w r0,[r0,r10,lsl #0x2]
  000c36b8: ldr r3,[r0,#0x0]
  000c36ba: ldr r6,[r3,#0x48]
  000c36bc: ldr r3,[0x000c3aa0]
  000c36be: blx r6
  000c36c0: ldr.w r0,[r5,#0xf8]
  000c36c4: ldr r4,[0x000c3aa4]
  000c36c6: ldr r1,[0x000c3aa8]
  000c36c8: ldr r0,[r0,#0x4]
  000c36ca: ldr r3,[0x000c3aac]
  000c36cc: add.w r0,r0,r10, lsl #0x2
  000c36d0: ldr r0,[r0,#0x4]
  000c36d2: ldr r2,[r0,#0x0]
  000c36d4: ldr r6,[r2,#0x48]
  000c36d6: mov r2,r4
  000c36d8: blx r6
  000c36da: ldr.w r0,[r5,#0xf8]
  000c36de: ldr r1,[0x000c3ab0]
  000c36e0: ldr r2,[0x000c3ab4]
  000c36e2: ldr r0,[r0,#0x4]
  000c36e4: add.w r0,r0,r10, lsl #0x2
  000c36e8: ldr r0,[r0,#0x8]
  000c36ea: ldr r3,[r0,#0x0]
  000c36ec: ldr r6,[r3,#0x48]
  000c36ee: ldr r3,[0x000c3ab8]
  000c36f0: blx r6
  000c36f2: ldr.w r0,[r5,#0xf8]
  000c36f6: ldr r1,[0x000c3abc]
  000c36f8: ldr r2,[0x000c3ac0]
  000c36fa: ldr r0,[r0,#0x4]
  000c36fc: add.w r0,r0,r10, lsl #0x2
  000c3700: ldr r0,[r0,#0xc]
  000c3702: ldr r3,[r0,#0x0]
  000c3704: ldr r6,[r3,#0x48]
  000c3706: ldr r3,[0x000c3ac4]
  000c3708: blx r6
  000c370a: ldr.w r0,[r5,#0xf8]
  000c370e: ldr r2,[0x000c3ac8]
  000c3710: ldr r3,[0x000c3acc]
  000c3712: ldr r0,[r0,#0x4]
  000c3714: add.w r0,r0,r10, lsl #0x2
  000c3718: ldr r0,[r0,#0x10]
  000c371a: ldr r1,[r0,#0x0]
  000c371c: ldr r6,[r1,#0x48]
  000c371e: mov r1,r4
  000c3720: blx r6
  000c3722: movs r0,#0x1c
  000c3724: blx 0x0006eb24
  000c3728: mov r4,r0
  000c372a: movs r1,#0x12
  000c372c: movs r2,#0x0
  000c372e: mov r3,r10
  000c3730: str r5,[sp,#0x0]
  000c3732: blx 0x00073f9c
  000c3736: movs r0,#0x1c
  000c3738: str r4,[r5,#0x28]
  000c373a: blx 0x0006eb24
  000c373e: mov r4,r0
  000c3740: ldr r3,[sp,#0x5c]
  000c3742: movs r1,#0x12
  000c3744: mov r2,r10
  000c3746: str r5,[sp,#0x0]
  000c3748: blx 0x00073f9c
  000c374c: str r4,[r5,#0x2c]
  000c374e: b.w 0x000c02ca
  000c3752: ldr.w r8,[sp,#0x6c]
  000c3756: ldr r3,[sp,#0x54]
  000c3758: ldr.w r11,[sp,#0x28]
  000c375c: cmp r3,#0x0
  000c375e: beq.w 0x000c396c
  000c3762: ldr r0,[sp,#0x78]
  000c3764: ldr r6,[sp,#0x44]
  000c3766: ldr r0,[r0,#0x0]
  000c3768: mov r1,r6
  000c376a: blx 0x00071a34
  000c376e: mov r9,r0
  000c3770: ldr.w r0,[r8,#0x0]
  000c3774: blx 0x0007192c
  000c3778: cmp r6,#0x8
  000c377a: bne 0x000c37e2
  000c377c: mov r8,r0
  000c377e: ldr r0,[0x000c3ad0]
  000c3780: add r0,pc
  000c3782: ldr r0,[r0,#0x0]
  000c3784: ldrb r0,[r0,#0x0]
  000c3786: cbz r0,0x000c37e2
  000c3788: ldr r0,[sp,#0x64]
  000c378a: movs r1,#0x64
  000c378c: ldr r0,[r0,#0x0]
  000c378e: blx 0x00071848
  000c3792: mov r6,r0
  000c3794: ldr r0,[sp,#0x6c]
  000c3796: movs r5,#0x14
  000c3798: cmp.w r8,#0x0
  000c379c: ldr r0,[r0,#0x0]
  000c379e: it ne
  000c37a0: mov.ne r5,#0x28
  000c37a2: blx 0x00071c5c
  000c37a6: ldr.w r10,[sp,#0x34]
  000c37aa: add r0,r5
  000c37ac: cmp r6,r0
  000c37ae: bge 0x000c37e2
  000c37b0: add.w r8,sp,#0x174
  000c37b4: ldr.w r1,[r4,#0xf0]
  000c37b8: mov r0,r8
  000c37ba: blx 0x0007264c
  000c37be: add r6,sp,#0x138
  000c37c0: ldr r2,[0x000c3ad4]
  000c37c2: mov r1,r8
  000c37c4: mov r0,r6
  000c37c6: blx 0x00073024
  000c37ca: add.w r0,r4,#0x18c
  000c37ce: mov r1,r6
  000c37d0: blx 0x0006eb3c
  000c37d4: add.w r3,r4,#0x18c
  000c37d8: ldr.w r0,[r4,#0x180]
  000c37dc: ldmia r3,{r1,r2,r3}
  000c37de: blx 0x00073f0c
  000c37e2: add.w r2,r4,#0x160
  000c37e6: add.w r5,r10,r11
  000c37ea: movs r6,#0x0
  000c37ec: ldmia r2,{r0,r1,r2}
  000c37ee: adds r3,r5,r0
  000c37f0: str.w r9,[sp,#0x70]
  000c37f4: add r3,r1
  000c37f6: add.w r10,r3,r2
  000c37fa: add.w r3,r4,#0x18c
  000c37fe: str r3,[sp,#0x74]
  000c3800: b 0x000c3954
  000c3802: ldr.w r0,[r4,#0x180]
  000c3806: blx 0x000728bc
  000c380a: str r0,[sp,#0x0]
  000c380c: movs r0,#0x1
  000c380e: strd r0,r6,[sp,#0x4]
  000c3812: mov r0,r4
  000c3814: ldr r1,[sp,#0x44]
  000c3816: movs r2,#0x0
  000c3818: mov r3,r9
  000c381a: blx 0x00073e88
  000c381e: ldr.w r1,[r4,#0xf8]
  000c3822: ldr r1,[r1,#0x4]
  000c3824: str.w r0,[r1,r10,lsl #0x2]
  000c3828: ldr r0,[sp,#0x6c]
  000c382a: ldr r0,[r0,#0x0]
  000c382c: blx 0x00073f30
  000c3830: cmp r0,#0x0
  000c3832: beq.w 0x000c3948
  000c3836: ldr.w r0,[r4,#0xf8]
  000c383a: ldr r0,[r0,#0x4]
  000c383c: ldr.w r0,[r0,r10,lsl #0x2]
  000c3840: ldr r5,[r0,#0x4]
  000c3842: mov r0,r5
  000c3844: blx 0x00072910
  000c3848: lsls r1,r0,#0x1
  000c384a: mov r0,r5
  000c384c: blx 0x000727e4
  000c3850: ldr.w r0,[r4,#0xf8]
  000c3854: ldr r0,[r0,#0x4]
  000c3856: ldr.w r0,[r0,r10,lsl #0x2]
  000c385a: ldr r1,[r0,#0x0]
  000c385c: ldr r2,[r1,#0x1c]
  000c385e: ldr r1,[0x000c3ad8]
  000c3860: blx r2
  000c3862: ldr.w r0,[r4,#0xf8]
  000c3866: movs r1,#0x3
  000c3868: ldr r0,[r0,#0x4]
  000c386a: ldr.w r0,[r0,r10,lsl #0x2]
  000c386e: blx 0x00074008
  000c3872: add r5,sp,#0x138
  000c3874: ldr.w r1,[r4,#0xf0]
  000c3878: mov r0,r5
  000c387a: blx 0x0007264c
  000c387e: ldr.w r9,[sp,#0x74]
  000c3882: mov r1,r5
  000c3884: mov r0,r9
  000c3886: blx 0x0006eb3c
  000c388a: ldr r6,[sp,#0x64]
  000c388c: movw r1,#0xc350
  000c3890: ldr r5,[sp,#0x60]
  000c3892: ldr r0,[r6,#0x0]
  000c3894: blx r5
  000c3896: mov r11,r0
  000c3898: ldr r0,[r6,#0x0]
  000c389a: movs r1,#0x2
  000c389c: blx r5
  000c389e: cmp r0,#0x0
  000c38a0: mov.w r0,#0xffffffff
  000c38a4: it eq
  000c38a6: mov.eq r0,#0x1
  000c38a8: movw r1,#0x4e20
  000c38ac: mla r0,r0,r11,r1
  000c38b0: movw r1,#0xc350
  000c38b4: vmov s0,r0
  000c38b8: vcvt.f32.s32 s0,s0
  000c38bc: vldr.32 s2,[r4,#0x18c]
  000c38c0: vadd.f32 s0,s2,s0
  000c38c4: vstr.32 s0,[r4,#0x18c]
  000c38c8: ldr r0,[r6,#0x0]
  000c38ca: blx r5
  000c38cc: mov r11,r0
  000c38ce: ldr r0,[r6,#0x0]
  000c38d0: movs r1,#0x2
  000c38d2: blx r5
  000c38d4: cmp r0,#0x0
  000c38d6: mov.w r0,#0xffffffff
  000c38da: it eq
  000c38dc: mov.eq r0,#0x1
  000c38de: movw r1,#0x2710
  000c38e2: mla r0,r0,r11,r1
  000c38e6: movw r1,#0xc350
  000c38ea: vmov s0,r0
  000c38ee: vcvt.f32.s32 s0,s0
  000c38f2: vldr.32 s2,[r4,#0x190]
  000c38f6: vadd.f32 s0,s2,s0
  000c38fa: vstr.32 s0,[r4,#0x190]
  000c38fe: ldr r0,[r6,#0x0]
  000c3900: blx r5
  000c3902: mov r11,r0
  000c3904: ldr r0,[r6,#0x0]
  000c3906: movs r1,#0x2
  000c3908: blx r5
  000c390a: cmp r0,#0x0
  000c390c: mov.w r0,#0xffffffff
  000c3910: it eq
  000c3912: mov.eq r0,#0x1
  000c3914: movw r1,#0x4e20
  000c3918: mla r0,r0,r11,r1
  000c391c: movs r6,#0x0
  000c391e: vmov s0,r0
  000c3922: vcvt.f32.s32 s0,s0
  000c3926: vldr.32 s2,[r4,#0x194]
  000c392a: ldr.w r0,[r4,#0xf8]
  000c392e: vadd.f32 s0,s2,s0
  000c3932: vstr.32 s0,[r4,#0x194]
  000c3936: ldr r0,[r0,#0x4]
  000c3938: ldr.w r0,[r0,r10,lsl #0x2]
  000c393c: ldr r1,[r0,#0x0]
  000c393e: ldr r2,[r1,#0x44]
  000c3940: mov r1,r9
  000c3942: ldr.w r9,[sp,#0x70]
  000c3946: blx r2
  000c3948: add.w r2,r4,#0x160
  000c394c: add.w r10,r10,#0x1
  000c3950: mov r5,r8
  000c3952: ldmia r2,{r0,r1,r2}
  000c3954: mov r8,r5
  000c3956: add r5,r0
  000c3958: add r5,r1
  000c395a: ldr.w r3,[r4,#0x16c]
  000c395e: add r5,r2
  000c3960: add.w r11,r5,r3
  000c3964: cmp r10,r11
  000c3966: blt.w 0x000c3802
  000c396a: b 0x000c3986
  000c396c: add.w r8,r10,r11
  000c3970: ldr.w r3,[r4,#0x16c]
  000c3974: add.w r6,r8,r0
  000c3978: add r6,r1
  000c397a: add r6,r2
  000c397c: add.w r11,r6,r3
  000c3980: add.w r6,r4,#0x18c
  000c3984: str r6,[sp,#0x74]
  000c3986: ldr r6,[sp,#0x20]
  000c3988: str.w r8,[sp,#0x5c]
  000c398c: add.w r9,r8,r6
  000c3990: vldr.32 s16,[pc,#0x148]
  000c3994: vldr.32 s18,[pc,#0x148]
  000c3998: ldr r5,[sp,#0x7c]
  000c399a: ldr.w r10,[sp,#0x60]
  000c399e: b 0x000c3a5c
  000c39a0: ldr r0,[sp,#0x78]
  000c39a2: movs r1,#0x8
  000c39a4: ldr r0,[r0,#0x0]
  000c39a6: blx 0x00071a34
  000c39aa: mov r3,r0
  000c39ac: movs r0,#0x0
  000c39ae: movs r1,#0x1
  000c39b0: str r0,[sp,#0x0]
  000c39b2: strd r1,r0,[sp,#0x4]
  000c39b6: mov r0,r5
  000c39b8: movs r1,#0x8
  000c39ba: movs r2,#0x0
  000c39bc: blx 0x00073e88
  000c39c0: ldr.w r1,[r5,#0xf8]
  000c39c4: add r4,sp,#0x138
  000c39c6: ldr r1,[r1,#0x4]
  000c39c8: str.w r0,[r1,r11,lsl #0x2]
  000c39cc: mov r0,r4
  000c39ce: ldr.w r1,[r5,#0xf0]
  000c39d2: blx 0x0007264c
  000c39d6: ldr r0,[sp,#0x74]
  000c39d8: mov r1,r4
  000c39da: blx 0x0006eb3c
  000c39de: ldr.w r0,[r5,#0xf8]
  000c39e2: ldr r6,[sp,#0x64]
  000c39e4: vldr.32 s20,[r5,#0x18c]
  000c39e8: ldr r1,[r0,#0x4]
  000c39ea: ldr r0,[r6,#0x0]
  000c39ec: ldr.w r4,[r1,r11,lsl #0x2]
  000c39f0: ldr r1,[0x000c3ae4]
  000c39f2: mov r8,r1
  000c39f4: blx r10
  000c39f6: ldr r1,[r6,#0x0]
  000c39f8: vmov s24,r0
  000c39fc: vldr.32 s22,[r5,#0x190]
  000c3a00: mov r0,r1
  000c3a02: ldr r1,[0x000c3ae8]
  000c3a04: blx r10
  000c3a06: vldr.32 s26,[r5,#0x194]
  000c3a0a: vcvt.f32.s32 s24,s24
  000c3a0e: ldr r1,[r6,#0x0]
  000c3a10: vmov s0,r0
  000c3a14: vcvt.f32.s32 s28,s0
  000c3a18: mov r0,r1
  000c3a1a: mov r1,r8
  000c3a1c: blx r10
  000c3a1e: vmov s6,r0
  000c3a22: vadd.f32 s0,s20,s16
  000c3a26: vadd.f32 s2,s22,s18
  000c3a2a: vcvt.f32.s32 s6,s6
  000c3a2e: ldr r0,[r4,#0x0]
  000c3a30: vadd.f32 s4,s26,s16
  000c3a34: ldr r6,[r0,#0x48]
  000c3a36: mov r0,r4
  000c3a38: vadd.f32 s0,s0,s24
  000c3a3c: vadd.f32 s2,s2,s28
  000c3a40: vadd.f32 s4,s4,s6
  000c3a44: vmov r1,s0
  000c3a48: vmov r2,s2
  000c3a4c: vmov r3,s4
  000c3a50: blx r6
  000c3a52: add.w r3,r5,#0x160
  000c3a56: add.w r11,r11,#0x1
  000c3a5a: ldmia r3,{r0,r1,r2,r3}
  000c3a5c: add.w r6,r9,r0
  000c3a60: add r6,r1
  000c3a62: add r6,r2
  000c3a64: add r6,r3
  000c3a66: cmp r11,r6
  000c3a68: blt 0x000c39a0
  000c3a6a: ldr r5,[sp,#0x30]
  000c3a6c: ldr.w r10,[sp,#0x7c]
  000c3a70: cmp r5,#0x0
  000c3a72: beq.w 0x000c3cdc
  000c3a76: movs r5,#0x0
  000c3a78: add.w r11,sp,#0xe0
  000c3a7c: strd r5,r5,[sp,#0x138]
  000c3a80: str r5,[sp,#0x140]
  000c3a82: ldr r5,[sp,#0x30]
  000c3a84: ldr r4,[sp,#0x5c]
  000c3a86: add r4,r5
  000c3a88: ldr r5,[sp,#0x20]
  000c3a8a: str r4,[sp,#0x70]
  000c3a8c: add.w r9,r4,r5
  000c3a90: ldr r5,[0x000c3aec]
  000c3a92: add r5,pc
  000c3a94: b 0x000c3b92
  000c3af0: ldr r0,[sp,#0x6c]
  000c3af2: ldr r0,[r0,#0x0]
  000c3af4: blx 0x00071c14
  000c3af8: blx 0x000740b0
  000c3afc: add.w r0,r0,r0, lsl #0x1
  000c3b00: ldr.w r1,[r5,r0,lsl #0x2]
  000c3b04: add.w r0,r5,r0, lsl #0x2
  000c3b08: ldrd r2,r0,[r0,#0x4]
  000c3b0c: strd r1,r2,[sp,#0x174]
  000c3b10: str r0,[sp,#0x17c]
  000c3b12: movs r0,#0x18
  000c3b14: blx 0x0006eb24
  000c3b18: mov r4,r0
  000c3b1a: add r1,sp,#0x174
  000c3b1c: movs r2,#0x3
  000c3b1e: blx 0x00073eb8
  000c3b22: mov r0,r4
  000c3b24: blx 0x000728bc
  000c3b28: mov r1,r0
  000c3b2a: mov r0,r10
  000c3b2c: movw r2,#0x37a3
  000c3b30: movs r3,#0x1
  000c3b32: blx 0x00073f6c
  000c3b36: ldr.w r1,[r10,#0xf8]
  000c3b3a: ldr r1,[r1,#0x4]
  000c3b3c: str.w r0,[r1,r6,lsl #0x2]
  000c3b40: mov r0,r4
  000c3b42: blx 0x000728bc
  000c3b46: mov r1,r0
  000c3b48: ldr r0,[r0,#0x0]
  000c3b4a: mov r4,r11
  000c3b4c: ldr r2,[r0,#0x28]
  000c3b4e: mov r0,r11
  000c3b50: blx r2
  000c3b52: add r0,sp,#0x138
  000c3b54: mov r1,r11
  000c3b56: blx 0x0006eb3c
  000c3b5a: ldr.w r0,[r10,#0xf8]
  000c3b5e: ldr r0,[r0,#0x4]
  000c3b60: ldr.w r0,[r0,r6,lsl #0x2]
  000c3b64: blx 0x000732dc
  000c3b68: ldr r0,[0x000c3dfc]
  000c3b6a: movw r1,#0x1b9
  000c3b6e: add r0,pc
  000c3b70: ldr r0,[r0,#0x0]
  000c3b72: ldr r0,[r0,#0x0]
  000c3b74: blx 0x00072f70
  000c3b78: mov r1,r0
  000c3b7a: ldr.w r0,[r10,#0xf8]
  000c3b7e: ldr r0,[r0,#0x4]
  000c3b80: ldr.w r0,[r0,r6,lsl #0x2]
  000c3b84: adds r0,#0x18
  000c3b86: blx 0x0006f2b0
  000c3b8a: add.w r3,r10,#0x160
  000c3b8e: adds r6,#0x1
  000c3b90: ldmia r3,{r0,r1,r2,r3}
  000c3b92: add.w r4,r9,r0
  000c3b96: add r4,r1
  000c3b98: add r4,r2
  000c3b9a: add.w r8,r4,r3
  000c3b9e: cmp r6,r8
  000c3ba0: blt 0x000c3af0
  000c3ba2: ldr r6,[sp,#0x14]
  000c3ba4: movw r11,#0x2710
  000c3ba8: ldr r5,[sp,#0x70]
  000c3baa: add r6,r5
  000c3bac: ldr r5,[sp,#0x20]
  000c3bae: add r6,r5
  000c3bb0: str r6,[sp,#0x68]
  000c3bb2: ldrd r9,r5,[sp,#0x60]
  000c3bb6: b 0x000c3ccc
  000c3bb8: ldr r0,[sp,#0x78]
  000c3bba: movs r1,#0x8
  000c3bbc: ldr r0,[r0,#0x0]
  000c3bbe: blx 0x00071a34
  000c3bc2: mov r3,r0
  000c3bc4: movs r0,#0x0
  000c3bc6: movs r1,#0x1
  000c3bc8: str r0,[sp,#0x0]
  000c3bca: strd r1,r0,[sp,#0x4]
  000c3bce: mov r0,r10
  000c3bd0: movs r1,#0x8
  000c3bd2: movs r2,#0x0
  000c3bd4: blx 0x00073e88
  000c3bd8: ldr.w r1,[r10,#0xf8]
  000c3bdc: ldr r1,[r1,#0x4]
  000c3bde: str.w r0,[r1,r8,lsl #0x2]
  000c3be2: movw r1,#0x4e20
  000c3be6: ldr r0,[r5,#0x0]
  000c3be8: vldr.32 s16,[sp,#0x138]
  000c3bec: blx r9
  000c3bee: str r0,[sp,#0x70]
  000c3bf0: movs r1,#0x2
  000c3bf2: ldr r0,[r5,#0x0]
  000c3bf4: blx r9
  000c3bf6: mov r4,r0
  000c3bf8: ldr r0,[r5,#0x0]
  000c3bfa: movw r1,#0x4e20
  000c3bfe: vldr.32 s18,[sp,#0x13c]
  000c3c02: blx r9
  000c3c04: mov r6,r0
  000c3c06: ldr r0,[r5,#0x0]
  000c3c08: movs r1,#0x2
  000c3c0a: blx r9
  000c3c0c: cmp r0,#0x0
  000c3c0e: mov.w r0,#0xffffffff
  000c3c12: it eq
  000c3c14: mov.eq r0,#0x1
  000c3c16: cmp r4,#0x0
  000c3c18: mla r0,r0,r6,r11
  000c3c1c: vmov s20,r0
  000c3c20: mov.w r0,#0xffffffff
  000c3c24: it eq
  000c3c26: mov.eq r0,#0x1
  000c3c28: ldr r1,[sp,#0x70]
  000c3c2a: vldr.32 s24,[sp,#0x140]
  000c3c2e: mla r0,r0,r1,r11
  000c3c32: movw r1,#0x4e20
  000c3c36: vmov s22,r0
  000c3c3a: ldr r0,[r5,#0x0]
  000c3c3c: blx r9
  000c3c3e: mov r4,r0
  000c3c40: ldr r0,[r5,#0x0]
  000c3c42: movs r1,#0x2
  000c3c44: vcvt.f32.s32 s22,s22
  000c3c48: vcvt.f32.s32 s20,s20
  000c3c4c: blx r9
  000c3c4e: cmp r0,#0x0
  000c3c50: mov.w r0,#0xffffffff
  000c3c54: it eq
  000c3c56: mov.eq r0,#0x1
  000c3c58: vadd.f32 s2,s16,s22
  000c3c5c: mla r0,r0,r4,r11
  000c3c60: vadd.f32 s4,s18,s20
  000c3c64: vmov s0,r0
  000c3c68: vcvt.f32.s32 s0,s0
  000c3c6c: ldr.w r0,[r10,#0xf8]
  000c3c70: vcvt.s32.f32 s2,s2
  000c3c74: vcvt.s32.f32 s4,s4
  000c3c78: ldr r0,[r0,#0x4]
  000c3c7a: vadd.f32 s0,s24,s0
  000c3c7e: vcvt.f32.s32 s2,s2
  000c3c82: vcvt.f32.s32 s4,s4
  000c3c86: vcvt.s32.f32 s0,s0
  000c3c8a: ldr.w r0,[r0,r8,lsl #0x2]
  000c3c8e: vmov r1,s2
  000c3c92: vmov r2,s4
  000c3c96: vcvt.f32.s32 s0,s0
  000c3c9a: ldr r3,[r0,#0x0]
  000c3c9c: ldr r6,[r3,#0x48]
  000c3c9e: vmov r3,s0
  000c3ca2: blx r6
  000c3ca4: ldr.w r0,[r10,#0xf8]
  000c3ca8: ldr r0,[r0,#0x4]
  000c3caa: ldr.w r0,[r0,r8,lsl #0x2]
  000c3cae: blx 0x000732dc
  000c3cb2: ldr.w r0,[r10,#0xf8]
  000c3cb6: movs r1,#0x1
  000c3cb8: add.w r3,r10,#0x160
  000c3cbc: ldr r0,[r0,#0x4]
  000c3cbe: ldr.w r0,[r0,r8,lsl #0x2]
  000c3cc2: add.w r8,r8,#0x1
  000c3cc6: strb.w r1,[r0,#0x12f]
  000c3cca: ldmia r3,{r0,r1,r2,r3}
  000c3ccc: ldr r6,[sp,#0x68]
  000c3cce: add r0,r6
  000c3cd0: add r0,r1
  000c3cd2: add r0,r2
  000c3cd4: add r0,r3
  000c3cd6: cmp r8,r0
  000c3cd8: blt.w 0x000c3bb8
  000c3cdc: ldr r0,[sp,#0x2c]
  000c3cde: ldr r6,[sp,#0x64]
  000c3ce0: cmp r0,#0x1
  000c3ce2: blt.w 0x000c3e54
  000c3ce6: add r4,sp,#0x138
  000c3ce8: ldr.w r1,[r10,#0xf0]
  000c3cec: mov r0,r4
  000c3cee: blx 0x0007264c
  000c3cf2: ldr r0,[sp,#0x74]
  000c3cf4: mov r1,r4
  000c3cf6: blx 0x0006eb3c
  000c3cfa: ldr r0,[r6,#0x0]
  000c3cfc: movw r1,#0xc350
  000c3d00: ldr r5,[sp,#0x60]
  000c3d02: blx r5
  000c3d04: mov r4,r0
  000c3d06: ldr r0,[r6,#0x0]
  000c3d08: movs r1,#0x2
  000c3d0a: blx r5
  000c3d0c: cmp r0,#0x0
  000c3d0e: mov.w r0,#0xffffffff
  000c3d12: it eq
  000c3d14: mov.eq r0,#0x1
  000c3d16: movw r8,#0x4e20
  000c3d1a: mla r0,r0,r4,r8
  000c3d1e: movw r1,#0xc350
  000c3d22: mov.w r9,#0xffffffff
  000c3d26: vmov s0,r0
  000c3d2a: vcvt.f32.s32 s0,s0
  000c3d2e: vldr.32 s2,[r10,#0x18c]
  000c3d32: vadd.f32 s0,s2,s0
  000c3d36: vstr.32 s0,[r10,#0x18c]
  000c3d3a: ldr r0,[r6,#0x0]
  000c3d3c: blx r5
  000c3d3e: mov r4,r0
  000c3d40: ldr r0,[r6,#0x0]
  000c3d42: movs r1,#0x2
  000c3d44: blx r5
  000c3d46: cmp r0,#0x0
  000c3d48: mov.w r0,#0xffffffff
  000c3d4c: it eq
  000c3d4e: mov.eq r0,#0x1
  000c3d50: movw r1,#0x2710
  000c3d54: mla r0,r0,r4,r1
  000c3d58: movw r1,#0xc350
  000c3d5c: vmov s0,r0
  000c3d60: vcvt.f32.s32 s0,s0
  000c3d64: vldr.32 s2,[r10,#0x190]
  000c3d68: vadd.f32 s0,s2,s0
  000c3d6c: vstr.32 s0,[r10,#0x190]
  000c3d70: ldr r0,[r6,#0x0]
  000c3d72: blx r5
  000c3d74: mov r4,r0
  000c3d76: ldr r0,[r6,#0x0]
  000c3d78: movs r1,#0x2
  000c3d7a: blx r5
  000c3d7c: cmp r0,#0x0
  000c3d7e: it eq
  000c3d80: mov.eq.w r9,#0x1
  000c3d84: mla r0,r9,r4,r8
  000c3d88: vmov s0,r0
  000c3d8c: movs r0,#0x18
  000c3d8e: vcvt.f32.s32 s0,s0
  000c3d92: vldr.32 s2,[r10,#0x194]
  000c3d96: vadd.f32 s0,s2,s0
  000c3d9a: vstr.32 s0,[r10,#0x194]
  000c3d9e: vldr.32 s2,[r10,#0x18c]
  000c3da2: vcvt.s32.f32 s2,s2
  000c3da6: vstr.32 s2,[sp,#0x138]
  000c3daa: vldr.32 s2,[r10,#0x190]
  000c3dae: vcvt.s32.f32 s0,s0
  000c3db2: vcvt.s32.f32 s2,s2
  000c3db6: vstr.32 s0,[sp,#0x140]
  000c3dba: vstr.32 s2,[sp,#0x13c]
  000c3dbe: blx 0x0006eb24
  000c3dc2: mov r5,r0
  000c3dc4: add r1,sp,#0x138
  000c3dc6: movs r2,#0x3
  000c3dc8: blx 0x00073eb8
  000c3dcc: add.w r3,r10,#0x160
  000c3dd0: mov.w r8,#0x0
  000c3dd4: mov.w r9,#0x1
  000c3dd8: ldmia r3,{r0,r1,r2,r3}
  000c3dda: ldr r6,[sp,#0x30]
  000c3ddc: ldr r4,[sp,#0x5c]
  000c3dde: add r6,r4
  000c3de0: ldr r4,[sp,#0x14]
  000c3de2: add.w r12,r6,r4
  000c3de6: ldr r6,[sp,#0x2c]
  000c3de8: add.w r4,r12,r6
  000c3dec: ldr r6,[sp,#0x20]
  000c3dee: add r4,r6
  000c3df0: add r6,r12
  000c3df2: add r6,r0
  000c3df4: add r6,r1
  000c3df6: add r6,r2
  000c3df8: add r6,r3
  000c3dfa: b 0x000c3e3e
  000c3e00: mov r0,r5
  000c3e02: blx 0x000728bc
  000c3e06: strd r0,r9,[sp,#0x0]
  000c3e0a: mov r0,r10
  000c3e0c: movs r1,#0xa
  000c3e0e: movs r2,#0x0
  000c3e10: movs r3,#0x2c
  000c3e12: str.w r8,[sp,#0x8]
  000c3e16: blx 0x00073e88
  000c3e1a: ldr.w r1,[r10,#0xf8]
  000c3e1e: ldr r1,[r1,#0x4]
  000c3e20: str.w r0,[r1,r6,lsl #0x2]
  000c3e24: movs r1,#0x1
  000c3e26: ldr.w r0,[r10,#0xf8]
  000c3e2a: ldr r0,[r0,#0x4]
  000c3e2c: ldr.w r0,[r0,r6,lsl #0x2]
  000c3e30: ldr r0,[r0,#0x4]
  000c3e32: blx 0x00073e94
  000c3e36: add.w r3,r10,#0x160
  000c3e3a: adds r6,#0x1
  000c3e3c: ldmia r3,{r0,r1,r2,r3}
  000c3e3e: add r0,r4
  000c3e40: add r0,r1
  000c3e42: add r0,r2
  000c3e44: add r0,r3
  000c3e46: cmp r6,r0
  000c3e48: blt 0x000c3e00
  000c3e4a: mov r0,r5
  000c3e4c: blx 0x00072310
  000c3e50: blx 0x0006eb48
  000c3e54: ldr r0,[sp,#0x18]
  000c3e56: cmp r0,#0x0
  000c3e58: beq 0x000c3f40
  000c3e5a: add.w r3,r10,#0x160
  000c3e5e: ldmia r3,{r0,r1,r2,r3}
  000c3e60: ldr r6,[sp,#0x30]
  000c3e62: ldr r5,[sp,#0x5c]
  000c3e64: add r6,r5
  000c3e66: ldr r5,[sp,#0x14]
  000c3e68: add r6,r5
  000c3e6a: ldr r5,[sp,#0x2c]
  000c3e6c: add r6,r5
  000c3e6e: ldr r5,[sp,#0x20]
  000c3e70: add r6,r5
  000c3e72: add r0,r6
  000c3e74: add r0,r1
  000c3e76: add r0,r2
  000c3e78: add r0,r3
  000c3e7a: str.w r0,[r10,#0xac]
  000c3e7e: mov.w r0,#0x138
  000c3e82: blx 0x0006eb24
  000c3e86: ldr r1,[0x000c423c]
  000c3e88: mov r4,r0
  000c3e8a: movs r0,#0x0
  000c3e8c: movs r2,#0x0
  000c3e8e: str r0,[sp,#0x0]
  000c3e90: mov r0,r4
  000c3e92: movw r3,#0x4e20
  000c3e96: blx 0x00073e4c
  000c3e9a: mov r0,r10
  000c3e9c: mov r1,r4
  000c3e9e: movw r2,#0x4a88
  000c3ea2: movs r3,#0x1
  000c3ea4: blx 0x00073f6c
  000c3ea8: ldr.w r2,[r10,#0xf8]
  000c3eac: ldr.w r1,[r10,#0xac]
  000c3eb0: ldr r2,[r2,#0x4]
  000c3eb2: str.w r0,[r2,r1,lsl #0x2]
  000c3eb6: ldr.w r0,[r10,#0xf8]
  000c3eba: ldr r0,[r0,#0x4]
  000c3ebc: ldr.w r0,[r0,r1,lsl #0x2]
  000c3ec0: movs r1,#0x1
  000c3ec2: blx 0x00072748
  000c3ec6: ldr.w r1,[r10,#0xf8]
  000c3eca: ldr.w r0,[r10,#0xac]
  000c3ece: ldr r1,[r1,#0x4]
  000c3ed0: ldr.w r0,[r1,r0,lsl #0x2]
  000c3ed4: movs r1,#0x0
  000c3ed6: blx 0x00073f54
  000c3eda: ldr.w r1,[r10,#0xf8]
  000c3ede: ldr.w r0,[r10,#0xac]
  000c3ee2: ldr r1,[r1,#0x4]
  000c3ee4: ldr.w r0,[r1,r0,lsl #0x2]
  000c3ee8: ldr r1,[r0,#0x0]
  000c3eea: ldr r2,[r1,#0x44]
  000c3eec: add.w r1,r10,#0xc8
  000c3ef0: blx r2
  000c3ef2: ldr.w r1,[r10,#0xf8]
  000c3ef6: ldr.w r0,[r10,#0xac]
  000c3efa: ldr r1,[r1,#0x4]
  000c3efc: ldr.w r0,[r1,r0,lsl #0x2]
  000c3f00: movs r1,#0x1
  000c3f02: ldr r0,[r0,#0x4]
  000c3f04: blx 0x00073fb4
  000c3f08: ldr r0,[0x000c4240]
  000c3f0a: ldr.w r2,[r10,#0xf8]
  000c3f0e: add r0,pc
  000c3f10: ldr.w r1,[r10,#0xac]
  000c3f14: ldr r0,[r0,#0x0]
  000c3f16: ldr r2,[r2,#0x4]
  000c3f18: ldr r0,[r0,#0x0]
  000c3f1a: ldr.w r4,[r2,r1,lsl #0x2]
  000c3f1e: movw r1,#0xc8a
  000c3f22: blx 0x00072f70
  000c3f26: add r5,sp,#0xa4
  000c3f28: mov r1,r0
  000c3f2a: movs r2,#0x0
  000c3f2c: mov r0,r5
  000c3f2e: blx 0x0006f028
  000c3f32: mov r0,r4
  000c3f34: mov r1,r5
  000c3f36: blx 0x00073fa8
  000c3f3a: add r0,sp,#0xa4
  000c3f3c: blx 0x0006ee30
  000c3f40: ldr r0,[sp,#0x24]
  000c3f42: cmp r0,#0x0
  000c3f44: beq.w 0x000c408a
  000c3f48: ldrd r6,r4,[r10,#0x160]
  000c3f4c: ldr r0,[sp,#0x7c]
  000c3f4e: ldr r5,[sp,#0x6c]
  000c3f50: ldr.w r11,[r10,#0x168]
  000c3f54: ldr.w r10,[r0,#0x16c]
  000c3f58: ldr r0,[r5,#0x0]
  000c3f5a: blx 0x00071c14
  000c3f5e: blx 0x00072724
  000c3f62: mov r9,r0
  000c3f64: mov.w r8,#0x0
  000c3f68: movs r0,#0x1
  000c3f6a: movs r3,#0xf
  000c3f6c: strd r8,r0,[sp,#0x0]
  000c3f70: cmp.w r9,#0x0
  000c3f74: str.w r8,[sp,#0x8]
  000c3f78: mov.w r2,#0x1
  000c3f7c: ldr r0,[0x000c42d4]
  000c3f7e: it eq
  000c3f80: mov.eq r3,#0xd
  000c3f82: add r0,pc
  000c3f84: ldr.w r1,[r0,r9,lsl #0x2]
  000c3f88: ldr r0,[sp,#0x7c]
  000c3f8a: blx 0x00073e88
  000c3f8e: ldr r1,[sp,#0x2c]
  000c3f90: ldr r2,[sp,#0x10]
  000c3f92: add r1,r2
  000c3f94: ldr r2,[sp,#0x20]
  000c3f96: add r1,r2
  000c3f98: ldr r2,[sp,#0x18]
  000c3f9a: add r1,r2
  000c3f9c: ldr r2,[sp,#0x28]
  000c3f9e: add r1,r2
  000c3fa0: ldr r2,[sp,#0x34]
  000c3fa2: add r1,r2
  000c3fa4: ldr r2,[sp,#0x7c]
  000c3fa6: add r1,r6
  000c3fa8: add r1,r4
  000c3faa: ldr.w r2,[r2,#0xf8]
  000c3fae: add r1,r11
  000c3fb0: add.w r4,r1,r10
  000c3fb4: ldr.w r10,[sp,#0x7c]
  000c3fb8: movs r1,#0x3
  000c3fba: ldr r2,[r2,#0x4]
  000c3fbc: str.w r0,[r2,r4,lsl #0x2]
  000c3fc0: ldr.w r0,[r10,#0xf8]
  000c3fc4: ldr r0,[r0,#0x4]
  000c3fc6: ldr.w r6,[r0,r4,lsl #0x2]
  000c3fca: mov r0,r6
  000c3fcc: blx 0x00072748
  000c3fd0: ldr r0,[r5,#0x0]
  000c3fd2: blx 0x00071c14
  000c3fd6: movs r1,#0x0
  000c3fd8: blx 0x00072718
  000c3fdc: ldr r1,[r6,#0x0]
  000c3fde: strb.w r0,[r6,#0x70]
  000c3fe2: mov r0,r6
  000c3fe4: ldr r2,[r1,#0x44]
  000c3fe6: add.w r1,r10,#0xc8
  000c3fea: blx r2
  000c3fec: mov r0,r6
  000c3fee: blx 0x000740bc
  000c3ff2: ldr.w r0,[r10,#0xf8]
  000c3ff6: add.w r5,r9,r9, lsl #0x1
  000c3ffa: ldr r0,[r0,#0x4]
  000c3ffc: ldr.w r0,[r0,r4,lsl #0x2]
  000c4000: ldr r2,[0x000c42d8]
  000c4002: add r2,pc
  000c4004: ldr r3,[r0,#0x0]
  000c4006: ldr.w r1,[r2,r5,lsl #0x2]
  000c400a: add.w r5,r2,r5, lsl #0x2
  000c400e: ldr r4,[r3,#0x48]
  000c4010: ldrd r2,r3,[r5,#0x4]
  000c4014: blx r4
  000c4016: ldr r0,[r6,#0x50]
  000c4018: cbz r0,0x000c4022
  000c401a: blx 0x00070204
  000c401e: blx 0x0006eb48
  000c4022: ldr r0,[sp,#0x6c]
  000c4024: str.w r8,[r6,#0x50]
  000c4028: ldr r0,[r0,#0x0]
  000c402a: blx 0x00071c14
  000c402e: movs r1,#0x0
  000c4030: blx 0x00072718
  000c4034: cbz r0,0x000c4058
  000c4036: movs r0,#0x1
  000c4038: blx 0x0006eb24
  000c403c: mov r4,r0
  000c403e: blx 0x000736cc
  000c4042: mov r0,r4
  000c4044: movs r1,#0x73
  000c4046: movs r2,#0x1
  000c4048: blx 0x000740c8
  000c404c: str r0,[r6,#0x50]
  000c404e: mov r0,r4
  000c4050: blx 0x000740d4
  000c4054: blx 0x0006eb48
  000c4058: movs r0,#0x1
  000c405a: blx 0x0006eb24
  000c405e: mov r4,r0
  000c4060: blx 0x00072124
  000c4064: ldr r0,[0x000c42dc]
  000c4066: mov.w r2,#0xffffffff
  000c406a: add r0,pc
  000c406c: ldr.w r1,[r0,r9,lsl #0x2]
  000c4070: mov r0,r4
  000c4072: blx 0x000740e0
  000c4076: mov r5,r0
  000c4078: mov r0,r4
  000c407a: blx 0x0007213c
  000c407e: blx 0x0006eb48
  000c4082: mov r0,r6
  000c4084: mov r1,r5
  000c4086: blx 0x000740a4
  000c408a: ldr r0,[sp,#0x1c]
  000c408c: cmp r0,#0x0
  000c408e: beq.w 0x000c02ca
  000c4092: ldr r0,[0x000c42e0]
  000c4094: movw r1,#0x67f
  000c4098: add r0,pc
  000c409a: ldr r0,[r0,#0x0]
  000c409c: ldr r0,[r0,#0x0]
  000c409e: blx 0x00072f70
  000c40a2: mov r1,r0
  000c40a4: ldr.w r0,[r10,#0xf8]
  000c40a8: ldr r0,[r0,#0x4]
  000c40aa: ldr r0,[r0,#0x0]
  000c40ac: adds r0,#0x18
  000c40ae: blx 0x0006f2b0
  000c40b2: b.w 0x000c02ca
  000c40b6: ldr.w r8,[sp,#0x7c]
  000c40ba: movs r0,#0x18
  000c40bc: vldr.32 s0,[r8,#0xc8]
  000c40c0: vcvt.s32.f32 s0,s0
  000c40c4: vstr.32 s0,[sp,#0x138]
  000c40c8: vldr.32 s0,[r8,#0xcc]
  000c40cc: vcvt.s32.f32 s0,s0
  000c40d0: vstr.32 s0,[sp,#0x13c]
  000c40d4: vldr.32 s0,[r8,#0xd0]
  000c40d8: vcvt.s32.f32 s0,s0
  000c40dc: vstr.32 s0,[sp,#0x140]
  000c40e0: blx 0x0006eb24
  000c40e4: mov r4,r0
  000c40e6: add r1,sp,#0x138
  000c40e8: movs r2,#0x3
  000c40ea: blx 0x00073eb8
  000c40ee: str.w r4,[r8,#0x108]
  000c40f2: ldr r0,[r5,#0x0]
  000c40f4: blx 0x00072850
  000c40f8: blx 0x00073ea0
  000c40fc: vmov s2,r0
  000c4100: vmov.f32 s0,0x41200000
  000c4104: vcvt.f32.s32 s2,s2
  000c4108: ldr r0,[0x000c42e4]
  000c410a: add r0,pc
  000c410c: ldr r0,[r0,#0x0]
  000c410e: vdiv.f32 s0,s2,s0
  000c4112: vmov.f32 s2,0x40a00000
  000c4116: vldr.32 s4,[r0,#0x2c]
  000c411a: vmul.f32 s0,s0,s2
  000c411e: vmov.f32 s2,0xbf000000
  000c4122: vcvt.s32.f32 s0,s0
  000c4126: vadd.f32 s2,s4,s2
  000c412a: vmov r0,s0
  000c412e: adds r0,#0x2
  000c4130: vmov s0,r0
  000c4134: movs r0,#0xc
  000c4136: vcvt.f32.s32 s0,s0
  000c413a: vmla.f32 s0,s2,s0
  000c413e: vcvt.s32.f32 s16,s0
  000c4142: blx 0x0006eb24
  000c4146: mov r5,r0
  000c4148: blx 0x00073d08
  000c414c: vmov r0,s16
  000c4150: mov r1,r5
  000c4152: str.w r5,[r8,#0xf8]
  000c4156: str r0,[sp,#0x7c]
  000c4158: blx 0x00073d14
  000c415c: ldr r0,[0x000c42e8]
  000c415e: mov.w r11,#0x1
  000c4162: mov.w r9,#0x0
  000c4166: add r0,pc
  000c4168: ldr.w r10,[r0,#0x0]
  000c416c: b 0x000c41c8
  000c416e: ldr.w r0,[r10,#0x0]
  000c4172: movs r1,#0x8
  000c4174: blx 0x00071a34
  000c4178: mov r5,r0
  000c417a: ldr r0,[sp,#0x78]
  000c417c: ldr.w r4,[r8,#0x108]
  000c4180: ldr r6,[r0,#0x0]
  000c4182: mov r0,r4
  000c4184: blx 0x00073fcc
  000c4188: mov r1,r0
  000c418a: mov r0,r6
  000c418c: blx 0x00071848
  000c4190: mov r1,r0
  000c4192: mov r0,r4
  000c4194: blx 0x00073ec4
  000c4198: strd r0,r11,[sp,#0x0]
  000c419c: movs r0,#0x0
  000c419e: str r0,[sp,#0x8]
  000c41a0: mov r0,r8
  000c41a2: movs r1,#0x8
  000c41a4: movs r2,#0x0
  000c41a6: mov r3,r5
  000c41a8: blx 0x00073e88
  000c41ac: ldr.w r1,[r8,#0xf8]
  000c41b0: ldr r1,[r1,#0x4]
  000c41b2: str.w r0,[r1,r9,lsl #0x2]
  000c41b6: ldr.w r0,[r8,#0xf8]
  000c41ba: ldr r0,[r0,#0x4]
  000c41bc: ldr.w r0,[r0,r9,lsl #0x2]
  000c41c0: blx 0x000732dc
  000c41c4: add.w r9,r9,#0x1
  000c41c8: ldr.w r0,[r8,#0xf8]
  000c41cc: ldr r0,[r0,#0x0]
  000c41ce: cmp r9,r0
  000c41d0: bcc 0x000c416e
  000c41d2: movs r0,#0x1c
  000c41d4: blx 0x0006eb24
  000c41d8: mov r5,r0
  000c41da: ldr r3,[sp,#0x7c]
  000c41dc: movs r1,#0x12
  000c41de: movs r2,#0x0
  000c41e0: str.w r8,[sp,#0x0]
  000c41e4: blx 0x00073f9c
  000c41e8: str.w r5,[r8,#0x28]
  000c41ec: b.w 0x000c02ca
```
