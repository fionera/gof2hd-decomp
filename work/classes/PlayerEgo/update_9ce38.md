# PlayerEgo::update
elf 0x9ce38 body 8416
Sig: undefined __stdcall update(int param_1, Radar * param_2, Hud * param_3, Radio * param_4, LevelScript * param_5, int param_6, bool param_7, int param_8)

## decompile
```c

/* PlayerEgo::update(int, Radar*, Hud*, Radio*, LevelScript*, int, bool, int) */

void PlayerEgo::update(int param_1,Radar *param_2,Hud *param_3,Radio *param_4,LevelScript *param_5,
                      int param_6,bool param_7,int param_8)

{
  ulonglong uVar1;
  byte bVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined1 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  Station *pSVar10;
  void *pvVar11;
  uint *puVar12;
  PlayerFixedObject *this;
  Ship *pSVar13;
  Item *pIVar14;
  Engine *this_00;
  Mesh *pMVar15;
  Transform *pTVar16;
  int *piVar17;
  Matrix *pMVar18;
  Explosion *pEVar19;
  Mission *this_01;
  Mission *this_02;
  int iVar20;
  uint uVar21;
  HackingGame *this_03;
  int *piVar22;
  float *pfVar23;
  PlayerEgo *pPVar24;
  uint extraout_r1;
  uint extraout_r1_00;
  Radar *extraout_r2;
  int extraout_r2_00;
  uint uVar25;
  undefined4 *puVar26;
  code *pcVar27;
  undefined1 (*pauVar28) [16];
  int iVar29;
  int *piVar30;
  AEGeometry *this_04;
  undefined4 uVar31;
  Matrix *pMVar32;
  ushort uVar33;
  Player *this_05;
  FModSound *this_06;
  PaintCanvas *pPVar34;
  Level *this_07;
  undefined4 uVar35;
  Vector *this_08;
  uint *puVar36;
  Vector *pVVar37;
  bool bVar38;
  bool bVar39;
  uint in_fpscr;
  float extraout_s0;
  float fVar40;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float fVar41;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  undefined4 extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s1_09;
  float extraout_s1_10;
  float extraout_s1_11;
  float extraout_s1_12;
  float extraout_s1_13;
  float extraout_s1_14;
  float extraout_s1_15;
  float extraout_s1_16;
  float extraout_s1_17;
  float extraout_s1_18;
  float extraout_s1_19;
  float extraout_s1_20;
  float extraout_s1_21;
  float extraout_s1_22;
  float extraout_s1_23;
  float extraout_s1_24;
  float extraout_s1_25;
  float extraout_s1_26;
  float extraout_s1_27;
  float extraout_s1_28;
  float extraout_s1_29;
  float extraout_s1_30;
  float extraout_s1_31;
  float extraout_s1_32;
  float extraout_s1_33;
  float extraout_s1_34;
  float extraout_s1_35;
  float extraout_s1_36;
  float extraout_s1_37;
  float extraout_s1_38;
  float extraout_s1_39;
  float extraout_s1_40;
  float fVar42;
  float extraout_s1_41;
  float extraout_s1_42;
  float extraout_s1_43;
  float extraout_s1_44;
  float extraout_s1_45;
  float extraout_s1_46;
  float extraout_s1_47;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float extraout_s2_07;
  float extraout_s2_08;
  float extraout_s2_09;
  float fVar43;
  float extraout_s2_10;
  float extraout_s2_11;
  float extraout_s2_12;
  float fVar44;
  float fVar45;
  undefined4 extraout_s7;
  float fVar46;
  float fVar47;
  float fVar48;
  float fVar49;
  undefined1 in_q8 [16];
  undefined1 auVar50 [16];
  longlong lVar51;
  uint in_stack_00000010;
  Vector aVStack_f8 [12];
  AEMath aAStack_ec [12];
  undefined8 local_e0;
  undefined4 local_d8;
  float local_d4 [15];
  float local_98 [5];
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined4 local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  int local_4c;
  
  piVar22 = *(int **)(DAT_000ad208 + 0xace52);
  local_4c = *piVar22;
  *(int *)(param_1 + 0x10) = param_6;
  if (*(int *)(param_1 + 0x220) == 0) {
    *(Radio **)(param_1 + 0x220) = param_4;
    *(Hud **)(param_1 + 0x14) = param_3;
    *(LevelScript **)(param_1 + 0x18) = param_5;
  }
  if (*(char *)(param_1 + 0x24) != '\0') goto LAB_000aef94;
  AEGeometry::getPosition();
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x148),(Vector *)local_98);
  uVar6 = *(uint *)(param_1 + 0x138);
  *(Radar **)(param_1 + 0x134) = param_2;
  if ((0x7fffffff < uVar6) && (0 < (int)((int)param_2 * 3 + uVar6))) {
    uVar6 = 0;
    *(undefined4 *)(param_1 + 0x138) = 0;
  }
  *(Radar **)(param_1 + 0x138) = param_2 + uVar6;
  if (*(char *)(param_1 + 0x39a) == '\0') {
LAB_000acedc:
    if (*(char *)(param_1 + 0x399) != '\0') goto LAB_000acee8;
  }
  else {
    if (*(char *)(param_1 + 0x399) == '\0') {
      iVar7 = Player::getShieldHP(*(Player **)param_1);
      if (iVar7 < 1) {
        iVar7 = Status::getShip();
        iVar7 = Ship::getFirstEquipmentOfSort(iVar7);
        if (iVar7 != 0) {
          iVar7 = Status::getShip();
          iVar7 = Ship::getFirstEquipmentOfSort(iVar7);
          Item::getAttribute(iVar7);
          iVar7 = Status::getShip();
          iVar7 = Ship::hasCargo(iVar7,0xca);
          if (iVar7 != 0) {
            piVar30 = *(int **)(DAT_000ad224 + 0xad0ea);
            *(undefined1 *)(param_1 + 0x399) = 1;
            fVar40 = (float)FModSound::play(*piVar30,(Vector *)0x8d2,(Vector *)0x0,extraout_s0_00);
            FModSound::play(*piVar30,(Vector *)0x8d1,(Vector *)0x0,fVar40);
            local_98[0] = 0.0;
            local_98[1] = 0.0;
            local_98[2] = 0.0;
            getPosition();
            FModSound::updateEvent3DAttributes
                      ((FModSound *)*piVar30,0x8d2,(Vector *)local_d4,(Vector *)local_98,false);
            iVar7 = Status::getShip();
            Ship::removeCargo(iVar7,0xca);
            Hud::hudEvent((int)param_4,(PlayerEgo *)0x2f,param_1);
          }
        }
      }
      goto LAB_000acedc;
    }
LAB_000acee8:
    iVar7 = Player::getShieldHP(*(Player **)param_1);
    iVar8 = Player::getMaxShieldHP(*(Player **)param_1);
    if (iVar7 < iVar8) {
      this_05 = *(Player **)param_1;
      uVar9 = Player::getShieldHP(this_05);
      fVar40 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
      FloatVectorMax(CONCAT44(extraout_s1,fVar40 * DAT_000ad20c),CONCAT44(extraout_s7,0x3f800000),2,
                     0x20);
      Player::setShieldHP((int)this_05);
      puVar26 = *(undefined4 **)(DAT_000ad210 + 0xacf40);
      this_06 = (FModSound *)*puVar26;
      uVar9 = Player::getShieldHP(*(Player **)param_1);
      fVar47 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
      uVar9 = Player::getMaxShieldHP(*(Player **)param_1);
      fVar40 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
      FModSound::setParamValue(this_06,0,0x8d1,fVar47 / fVar40);
      local_98[0] = 0.0;
      local_98[1] = 0.0;
      local_98[2] = 0.0;
      getPosition();
      FModSound::updateEvent3DAttributes
                ((FModSound *)*puVar26,0x8d1,(Vector *)local_d4,(Vector *)local_98,false);
    }
    else {
      piVar30 = *(int **)(DAT_000ad214 + 0xacfaa);
      FModSound::play(*piVar30,(Vector *)0x8d3,(Vector *)0x0,extraout_s0);
      FModSound::stop(*piVar30);
      local_98[0] = 0.0;
      local_98[1] = 0.0;
      local_98[2] = 0.0;
      getPosition();
      FModSound::updateEvent3DAttributes
                ((FModSound *)*piVar30,0x8d3,(Vector *)local_d4,(Vector *)local_98,false);
      *(undefined1 *)(param_1 + 0x399) = 0;
    }
  }
  if ((0 < *(int *)(param_1 + 0x20c)) &&
     (iVar7 = *(int *)(param_1 + 0x20c) - (int)param_2, *(int *)(param_1 + 0x20c) = iVar7, iVar7 < 1
     )) {
    Hud::hudEvent((int)param_4,(PlayerEgo *)0x2e,param_1);
  }
  if (*(int *)(param_1 + 0x1e8) != 0) {
    HackingGame::update(*(int *)(param_1 + 0x1e8));
    iVar7 = HackingGame::gameWon(*(HackingGame **)(param_1 + 0x1e8));
    if (iVar7 != 0) {
      Hud::setHackingGameActive((Hud *)param_4,false);
      piVar30 = *(int **)(DAT_000ad218 + 0xad030);
      pSVar10 = (Station *)Status::getStation();
      iVar7 = Station::stationHasHiddenBlueprint(pSVar10,false);
      if (iVar7 != 0) {
        pSVar10 = (Station *)Status::getStation();
        iVar7 = Station::getHiddenBlueprintIndex(pSVar10);
        Status::unlockBluePrint((Status *)*piVar30,*(int *)(DAT_000ad21c + 0xad054 + iVar7 * 4));
        if (*(HackingGame **)(param_1 + 0x1e8) != (HackingGame *)0x0) {
          pvVar11 = (void *)HackingGame::~HackingGame(*(HackingGame **)(param_1 + 0x1e8));
          operator_delete(pvVar11);
        }
        *(undefined4 *)(param_1 + 0x1e8) = 0;
        puVar12 = (uint *)Level::getEnemies();
        if (puVar12 != (uint *)0x0) {
          for (uVar6 = 0; uVar6 < *puVar12; uVar6 = uVar6 + 1) {
            this = *(PlayerFixedObject **)(puVar12[1] + uVar6 * 4);
            if (this[0x70] != (PlayerFixedObject)0x0) {
              this[0x70] = (PlayerFixedObject)0x0;
              PlayerFixedObject::setDockingType(this,0);
              puVar26 = (undefined4 *)(*(int *)(puVar12[1] + uVar6 * 4) + 0x50);
              if ((Array<int> *)*puVar26 != (Array<int> *)0x0) {
                pvVar11 = (void *)Array<int>::~Array((Array<int> *)*puVar26);
                operator_delete(pvVar11);
                puVar26 = (undefined4 *)(*(int *)(puVar12[1] + uVar6 * 4) + 0x50);
              }
              *puVar26 = 0;
              break;
            }
          }
        }
        Level::createRadioMessage(*(int *)(param_1 + 0xc),iVar7 + 0x15);
        *(undefined1 *)(*(int *)(*(int *)(*piVar30 + 0x58) + 4) + iVar7) = 1;
      }
    }
  }
  piVar30 = *(int **)(DAT_000ad228 + 0xad1a4);
  Status::getShip();
  iVar7 = Ship::getFreeSpace();
  fVar40 = extraout_s2;
  fVar41 = extraout_s0_01;
  fVar47 = extraout_s1_00;
  if (iVar7 < *(int *)(param_1 + 0x250)) {
    Status::getShip();
    uVar9 = Ship::getFreeSpace();
    *(undefined4 *)(param_1 + 0x250) = uVar9;
    pSVar13 = (Ship *)Status::getShip();
    uVar5 = Ship::hasVolatileGoods(pSVar13);
    *(undefined1 *)(param_1 + 0x398) = uVar5;
    fVar40 = extraout_s2_00;
    fVar41 = extraout_s0_02;
    fVar47 = extraout_s1_01;
  }
  if (*(char *)(param_1 + 0x13c) != '\0') {
    if (*(char *)(param_1 + 0x398) != '\0') {
      iVar7 = Status::getShip();
      pIVar14 = (Item *)Ship::getFirstEquipmentOfSort(iVar7);
      fVar41 = DAT_000ad234;
      fVar47 = extraout_s1_02;
      if (pIVar14 != (Item *)0x0) {
        iVar7 = Item::getIndex(pIVar14);
        pfVar23 = (float *)&DAT_000ad22c;
        if (iVar7 == 0xc3) {
          pfVar23 = (float *)&DAT_000ad230;
        }
        fVar41 = *pfVar23;
        fVar47 = extraout_s1_03;
      }
      fVar40 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      fVar40 = fVar40 * DAT_000ad238;
      *(float *)(*(int *)param_1 + 0x60) = *(float *)(*(int *)param_1 + 0x60) + fVar40 * fVar41;
    }
    fVar44 = DAT_000ad4e0;
    puVar12 = *(uint **)(*(int *)(param_1 + 0xc) + 0xa4);
    if (puVar12 != (uint *)0x0) {
      pfVar23 = (float *)(*(int *)(DAT_000ad4dc + 0xad27c) + 0x1234);
      for (uVar6 = 0; uVar6 < *puVar12; uVar6 = uVar6 + 1) {
        AEGeometry::getScaling();
        fVar40 = local_98[0];
        fVar47 = (float)getBoostPercentage();
        fVar40 = fVar40 * fVar44;
        fVar41 = fVar40 * (fVar47 * 0.5 + 1.0);
        *pfVar23 = fVar41;
        pfVar23 = pfVar23 + 0x28;
        puVar12 = *(uint **)(*(int *)(param_1 + 0xc) + 0xa4);
        fVar47 = extraout_s1_04;
      }
    }
    if (*(int *)(param_1 + 0xcc) < *(int *)(param_1 + 0x138)) {
      this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
      fVar41 = (float)AbyssEngine::Engine::SetPostEffect(this_00,DAT_000ad4e8,false);
      *(undefined1 *)(param_1 + 0x13c) = 0;
      *(undefined4 *)(param_1 + 0xb8) = 0x40000000;
      *(int *)(param_1 + 0x138) = -*(int *)(param_1 + 0xd0);
      fVar40 = extraout_s2_01;
      fVar47 = extraout_s1_05;
      if (*(char *)(param_1 + 0x146) == '\0') {
        *(undefined4 *)(param_1 + 200) = 2;
        *(undefined4 *)(param_1 + 0xcc) = 0;
        *(undefined4 *)(param_1 + 0xd0) = 0;
      }
    }
  }
  if (*(char *)(param_1 + 0x1ac) == '\0') {
    if ((*(char *)(param_1 + 0x1ad) != '\0') &&
       (iVar7 = *(int *)(param_1 + 0x134) + *(int *)(param_1 + 0x208),
       *(int *)(param_1 + 0x208) = iVar7, *(int *)(param_1 + 0x214) < iVar7)) {
      Hud::hudEvent((int)param_4,(PlayerEgo *)0x1d,param_1);
      fVar41 = (float)toggleCloaking();
      *(undefined1 *)(param_1 + 0x1ad) = 0;
      fVar40 = extraout_s2_02;
      fVar47 = extraout_s1_06;
    }
  }
  else {
    uVar6 = *(uint *)(param_1 + 0x134);
    iVar7 = *(int *)(param_1 + 0x208) + uVar6;
    *(int *)(param_1 + 0x208) = iVar7;
    iVar8 = *piVar30;
    uVar25 = *(uint *)(iVar8 + 0xc0);
    *(uint *)(iVar8 + 0xc0) = uVar25 + uVar6;
    *(uint *)(iVar8 + 0xc4) =
         *(int *)(iVar8 + 0xc4) + ((int)uVar6 >> 0x1f) + (uint)CARRY4(uVar25,uVar6);
    fVar40 = DAT_000ad4ec;
    if (iVar7 < 2000) {
      fVar47 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
      uVar6 = 0xff - (int)((fVar47 / DAT_000ad4ec) * DAT_000ad4f0);
      if ((int)uVar6 < 0x33) {
        uVar6 = 0x32;
      }
      if (iVar7 == 0) {
        *(undefined4 *)(param_1 + 0x208) = 1;
      }
      uVar6 = uVar6 | 0xffffff00;
      piVar17 = (int *)(param_1 + 4);
      puVar12 = *(uint **)(DAT_000ad4f4 + 0xad384);
      pPVar34 = (PaintCanvas *)*puVar12;
      pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer(pPVar34,*(uint *)(*piVar17 + 0x1c))
      ;
      fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                          (byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
                (pPVar34,pMVar15,fVar47 / fVar40,(uint)(fVar47 / fVar40));
      if (*(char *)(param_1 + 0x170) != '\0') {
        pPVar34 = (PaintCanvas *)*puVar12;
        pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                    (pPVar34,*(uint *)(*(int *)(param_1 + 0xdc) + 0x1c));
        fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                            (byte)(in_fpscr >> 0x16) & 3);
        AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
                  (pPVar34,pMVar15,fVar47 / fVar40,(uint)(fVar47 / fVar40));
        pPVar34 = (PaintCanvas *)*puVar12;
        pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                    (pPVar34,*(uint *)(*(int *)(param_1 + 0x28) + 0x1c));
        fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                            (byte)(in_fpscr >> 0x16) & 3);
        AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
                  (pPVar34,pMVar15,fVar47 / fVar40,(uint)(fVar47 / fVar40));
      }
      iVar7 = *piVar17;
      if (*(uint *)(iVar7 + 0x10) != 0xffffffff) {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(iVar7 + 0x10),uVar6);
        iVar7 = *piVar17;
      }
      if (*(uint *)(iVar7 + 0x14) != 0xffffffff) {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(iVar7 + 0x14),uVar6);
      }
      if (*(char *)(param_1 + 0x170) != '\0') {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0xdc) + 0xc),uVar6);
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0x28) + 0xc),uVar6);
      }
      fVar40 = (float)getCloakingPercentage((PlayerEgo *)param_1);
      uVar6 = in_fpscr & 0xfffffff | (uint)(fVar40 < 25.0) << 0x1f | (uint)(fVar40 == 25.0) << 0x1e;
      in_fpscr = uVar6 | (uint)NAN(fVar40) << 0x1c;
      bVar2 = (byte)(uVar6 >> 0x18);
      if ((!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
         (*(char *)(param_1 + 0x325) == '\0')) {
        *(undefined1 *)(param_1 + 0x325) = 1;
        if ((*(int *)(param_1 + 4) != 0) && (*(int *)(*(int *)(param_1 + 4) + 0x14) != -1)) {
          pTVar16 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
          AbyssEngine::Transform::SetVisible(pTVar16,*(char *)(param_1 + 0x325) == '\0');
        }
      }
    }
    puVar12 = *(uint **)(DAT_000ad53c + 0xad508);
    pPVar34 = (PaintCanvas *)*puVar12;
    pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                (pPVar34,*(uint *)(*(int *)(param_1 + 4) + 0x1c));
    fVar40 = DAT_000ad238;
    fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar47 = (float)AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
                              (pPVar34,pMVar15,fVar47 * DAT_000ad238,(uint)(fVar47 * DAT_000ad238));
    if (*(char *)(param_1 + 0x170) != '\0') {
      pPVar34 = (PaintCanvas *)*puVar12;
      pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                  (pPVar34,*(uint *)(*(int *)(param_1 + 0xdc) + 0x1c));
      fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar47 = fVar47 * fVar40;
      AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar34,pMVar15,fVar47,(uint)fVar47);
      pPVar34 = (PaintCanvas *)*puVar12;
      pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                  (pPVar34,*(uint *)(*(int *)(param_1 + 0x28) + 0x1c));
      fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar47 = fVar47 * fVar40;
      fVar47 = (float)AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
                                (pPVar34,pMVar15,fVar47,(uint)fVar47);
    }
    iVar7 = *(int *)(param_1 + 0x208);
    iVar8 = *(int *)(param_1 + 0x210) + -2000;
    if (iVar8 < iVar7) {
      pPVar34 = (PaintCanvas *)*puVar12;
      pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                  (pPVar34,*(uint *)(*(int *)(param_1 + 4) + 0x1c));
      fVar40 = DAT_000ad4ec;
      fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar41 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x210) + -2000,
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar47 = 1.0 - (fVar47 - fVar41) / DAT_000ad4ec;
      AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar34,pMVar15,fVar47,(uint)fVar47);
      fVar47 = (float)VectorSignedToFloat(iVar8,(byte)(in_fpscr >> 0x16) & 3);
      fVar41 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
      uVar6 = (uint)(((fVar41 - fVar47) / fVar40) * DAT_000ad4f0);
      if ((int)uVar6 < 0x33) {
        uVar6 = 0x32;
      }
      uVar6 = uVar6 | 0xffffff00;
      if (*(char *)(param_1 + 0x170) != '\0') {
        pPVar34 = (PaintCanvas *)*puVar12;
        pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                    (pPVar34,*(uint *)(*(int *)(param_1 + 0xdc) + 0x1c));
        fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar41 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x210) + -2000,
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar47 = 1.0 - (fVar47 - fVar41) / fVar40;
        AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar34,pMVar15,fVar47,(uint)fVar47);
        pPVar34 = (PaintCanvas *)*puVar12;
        pMVar15 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer
                                    (pPVar34,*(uint *)(*(int *)(param_1 + 0x28) + 0x1c));
        fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x208),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar41 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x210) + -2000,
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar40 = 1.0 - (fVar47 - fVar41) / fVar40;
        AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar34,pMVar15,fVar40,(uint)fVar40);
      }
      iVar7 = *(int *)(param_1 + 4);
      if (*(uint *)(iVar7 + 0x10) != 0xffffffff) {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(iVar7 + 0x10),uVar6);
        iVar7 = *(int *)(param_1 + 4);
      }
      if (*(uint *)(iVar7 + 0x14) != 0xffffffff) {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(iVar7 + 0x14),uVar6);
      }
      if (*(char *)(param_1 + 0x170) != '\0') {
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0xdc) + 0xc),uVar6);
        AbyssEngine::PaintCanvas::TransformSetColor
                  ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0x28) + 0xc),uVar6);
      }
      fVar40 = (float)getCloakingPercentage((PlayerEgo *)param_1);
      fVar47 = 25.0;
      in_fpscr = in_fpscr & 0xfffffff;
      if ((fVar40 < 25.0) && (*(char *)(param_1 + 0x325) != '\0')) {
        *(undefined1 *)(param_1 + 0x325) = 0;
        if ((*(int *)(param_1 + 4) != 0) && (*(int *)(*(int *)(param_1 + 4) + 0x14) != -1)) {
          pTVar16 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
          fVar47 = (float)AbyssEngine::Transform::SetVisible
                                    (pTVar16,*(char *)(param_1 + 0x325) == '\0');
        }
      }
    }
    if (*(int *)(param_1 + 0x210) < *(int *)(param_1 + 0x208)) {
      puVar26 = (undefined4 *)(DAT_000ada78 + 0xad760);
      *(undefined1 *)(param_1 + 0x1ac) = 0;
      piVar17 = (int *)*puVar26;
      *(undefined4 *)(param_1 + 0x208) = 0;
      *(undefined4 *)(param_1 + 0x20c) = *(undefined4 *)(param_1 + 0x368);
      *(undefined1 *)(*(int *)param_1 + 0x5e) = 0;
      FModSound::play(*piVar17,(Vector *)0x1e,(Vector *)0x0,fVar47);
      if (*(char *)(param_1 + 0x325) != '\0') {
        *(undefined1 *)(param_1 + 0x325) = 0;
        if ((*(int *)(param_1 + 4) != 0) && (*(int *)(*(int *)(param_1 + 4) + 0x14) != -1)) {
          pTVar16 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
          AbyssEngine::Transform::SetVisible(pTVar16,*(char *)(param_1 + 0x325) == '\0');
        }
      }
      uVar9 = *(undefined4 *)(param_1 + 0x394);
      iVar7 = AbyssEngine::PaintCanvas::MaterialGetMaterial
                        ((PaintCanvas *)*puVar12,*(uint *)(param_1 + 0x388));
      *(undefined4 *)(iVar7 + 0x20) = uVar9;
      if (*(char *)(param_1 + 0x170) != '\0') {
        uVar9 = *(undefined4 *)(param_1 + 0x394);
        iVar7 = AbyssEngine::PaintCanvas::MaterialGetMaterial
                          ((PaintCanvas *)*puVar12,*(uint *)(param_1 + 0x38c));
        *(undefined4 *)(iVar7 + 0x20) = uVar9;
        uVar9 = *(undefined4 *)(param_1 + 0x394);
        iVar7 = AbyssEngine::PaintCanvas::MaterialGetMaterial
                          ((PaintCanvas *)*puVar12,*(uint *)(param_1 + 0x390));
        *(undefined4 *)(iVar7 + 0x20) = uVar9;
        if (*(char *)(param_1 + 0x170) != '\0') {
          iVar7 = Status::getShip();
          pIVar14 = (Item *)Ship::getFirstEquipmentOfSort(iVar7);
          uVar33 = 0x4e86;
          if ((pIVar14 != (Item *)0x0) && (iVar7 = Item::getIndex(pIVar14), iVar7 == 0xe0)) {
            uVar33 = 0x5e15;
          }
          iVar7 = Status::getShip();
          pIVar14 = (Item *)Ship::getFirstEquipmentOfSort(iVar7);
          if (pIVar14 != (Item *)0x0) {
            iVar7 = Item::getIndex(pIVar14);
            uVar33 = 0x716b;
            if (iVar7 == 199) {
              uVar33 = 0x7165;
            }
            if (iVar7 == 0xc6) {
              uVar33 = 0x715f;
            }
          }
          AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
                    ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0xdc) + 0x1c),uVar33);
          AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
                    ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0x28) + 0x1c),uVar33);
        }
      }
    }
    this_07 = *(Level **)(param_1 + 0xc);
    fVar40 = (float)getCloakingPercentage((PlayerEgo *)param_1);
    fVar47 = DAT_000ada84 + fVar40 * DAT_000ada7c * DAT_000ada80;
    in_fpscr = in_fpscr & 0xfffffff;
    fVar40 = DAT_000ada84;
    if ((fVar47 < DAT_000ada84) && (fVar40 = fVar47, fVar47 < 0.0)) {
      fVar40 = DAT_000ada88;
    }
    fVar41 = (float)Level::setPlayerEngineColor(this_07,(short)(int)fVar40);
    fVar40 = extraout_s2_03;
    fVar47 = extraout_s1_07;
  }
  if (*(char *)(param_1 + 0x204) != '\0') {
    iVar7 = *(int *)(param_1 + 0x134) + *(int *)(param_1 + 0x1fc);
    *(int *)(param_1 + 0x1fc) = iVar7;
    if (*(int *)(param_1 + 0x200) <= iVar7) {
      *(undefined1 *)(param_1 + 0x38) = 0;
      fVar41 = (float)Hud::hudEvent((int)param_4,(PlayerEgo *)0x1a,param_1);
      fVar40 = extraout_s2_04;
      fVar47 = extraout_s1_08;
    }
    if (*(char *)(param_1 + 0x398) != '\0') {
      fVar41 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      fVar41 = fVar41 * DAT_000ada8c;
      fVar40 = *(float *)(*(int *)param_1 + 0x60) + fVar41 * 0.5;
      *(float *)(*(int *)param_1 + 0x60) = fVar40;
    }
  }
  if (*(TractorBeam **)(param_1 + 0x1b4) != (TractorBeam *)0x0) {
    fVar41 = (float)TractorBeam::update(*(TractorBeam **)(param_1 + 0x1b4),*(int *)(param_1 + 0x134)
                                        ,(Radar *)param_3,*(Level **)(param_1 + 0xc),(Hud *)param_4)
    ;
    fVar40 = extraout_s2_05;
    fVar47 = extraout_s1_09;
  }
  puVar12 = *(uint **)(param_1 + 0x1b8);
  if (puVar12 != (uint *)0x0) {
    for (uVar6 = 0; uVar6 < *puVar12; uVar6 = uVar6 + 1) {
      fVar41 = (float)RepairBeam::update(*(int *)(puVar12[1] + uVar6 * 4),
                                         *(Radar **)(param_1 + 0x134),(Level *)param_3,
                                         *(Hud **)(param_1 + 0xc));
      puVar12 = *(uint **)(param_1 + 0x1b8);
      fVar40 = extraout_s2_06;
      fVar47 = extraout_s1_10;
    }
  }
  if ((*(char *)(param_1 + 0x370) != '\0') &&
     (iVar7 = *(int *)(param_1 + 0x374), *(Radar **)(param_1 + 0x374) = param_2 + iVar7,
     1999 < (int)(param_2 + iVar7))) {
    *(undefined1 *)(param_1 + 0x370) = 0;
  }
  bVar39 = SUB41(param_2,0);
  if (((*(char *)(param_1 + 0x170) != '\0') && (*(char *)(param_1 + 0x180) == '\0')) &&
     (*(char *)(param_1 + 0x1a0) != '\0')) {
    iVar7 = Status::getShip();
    iVar7 = Ship::getFirstEquipmentOfSort(iVar7);
    fVar40 = extraout_s2_07;
    fVar41 = extraout_s0_05;
    fVar47 = extraout_s1_11;
    if (iVar7 != 0) {
      puVar12 = *(uint **)(DAT_000aded0 + 0xadcb4);
      uVar9 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
      fVar41 = (float)AbyssEngine::Transform::Update(CONCAT44(1,uVar9),bVar39);
      fVar40 = extraout_s2_08;
      fVar47 = extraout_s1_12;
      if (*(int *)(param_1 + 0x30) != 0) {
        uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
        fVar41 = (float)AbyssEngine::Transform::Update((ulonglong)uVar6,bVar39);
        fVar40 = extraout_s2_09;
        fVar47 = extraout_s1_13;
      }
    }
  }
  if ((*(int *)(param_1 + 0x34) != 0) && (*(char *)(param_1 + 0x38) != '\0')) {
    AEGeometry::setScaling(fVar41,fVar47,fVar40);
    pVVar37 = *(Vector **)(param_1 + 0x34);
    Level::getStarSystem();
    StarSystem::getLightDirection();
    AbyssEngine::AEMath::VectorNormalize((AEMath *)local_98,(Vector *)local_d4);
    local_54 = 0x3f800000;
    local_58 = 0;
    local_50 = 0;
    AEGeometry::setDirection(pVVar37,(Vector *)local_98);
    this_04 = *(AEGeometry **)(param_1 + 0x34);
    AEGeometry::getPosition();
    AEGeometry::setPosition(this_04,(Vector *)local_98);
    Status::getShip();
    iVar7 = Ship::getIndex();
    if (iVar7 == 8) {
      pVVar37 = *(Vector **)(param_1 + 0x34);
      AEGeometry::getDirection();
      fVar40 = (float)AbyssEngine::AEMath::VectorNormalize((AEMath *)local_d4,(Vector *)&local_58);
      AbyssEngine::AEMath::operator*((Vector *)local_98,fVar40);
      AEGeometry::translate(pVVar37);
    }
    uVar9 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000ada94 + 0xada1e));
    AbyssEngine::Transform::Update(CONCAT44(1,uVar9),SUB41(*(undefined4 *)(param_1 + 0x134),0));
  }
  puVar12 = *(uint **)(DAT_000ada98 + 0xada3e);
  lVar51 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
  AbyssEngine::Transform::Update(lVar51,bVar39);
  *(undefined1 *)(param_1 + 0x330) = 0;
  *(undefined1 *)(param_1 + 0x145) = 0;
  if (*(char *)(param_1 + 500) == '\0') {
    if (*(char *)(param_1 + 0x1ee) != '\0') {
      *(undefined1 *)(param_1 + 0x145) = 1;
      goto LAB_000adaa8;
    }
    if (*(char *)(param_1 + 0x1ed) != '\0') {
      *(undefined1 *)(param_1 + 0x145) = 1;
      goto LAB_000adab6;
    }
  }
  else {
    *(undefined1 *)(param_1 + 0x145) = 1;
    if (*(char *)(param_1 + 0x1ee) != '\0') {
LAB_000adaa8:
      *(int *)(param_1 + 0x1f0) = *(int *)(param_1 + 0x134) + *(int *)(param_1 + 0x1f0);
    }
LAB_000adab6:
    fVar40 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::moveForward(*(float *)(param_1 + 0xb8) * fVar40);
    pMVar18 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
    AbyssEngine::AEMath::operator*((Matrix *)local_98,pMVar18);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)param_1 + 4),(Matrix *)local_98);
  }
  if ((*(char *)(param_1 + 0x144) != '\0') && (iVar7 = Player::isActive(), iVar7 != 0)) {
    *(undefined1 *)(param_1 + 0x234) = 0;
    uVar9 = Level::getLandmarks();
    pcVar27 = *(code **)(DAT_000adec4 + 0xadb2c);
    (*pcVar27)(param_1,uVar9);
    uVar9 = Level::getEnemies();
    (*pcVar27)(param_1,uVar9);
    uVar9 = Level::getAsteroids(*(Level **)(param_1 + 0xc));
    (*pcVar27)(param_1,uVar9);
    if ((*(char *)(param_1 + 0x234) != '\0') && (*(char *)(param_1 + 0x1a0) != '\0')) {
      handleTurretView((PlayerEgo *)param_1,(int)param_2);
    }
  }
  if (*(char *)(param_1 + 0x398) == '\0') {
    *(undefined4 *)(*(int *)param_1 + 0x60) = 0;
  }
  else {
    piVar17 = *(int **)(DAT_000adec8 + 0xadb72);
    iVar7 = FModSound::isPlaying(*piVar17);
    if (iVar7 == 0) {
      FModSound::play(*piVar17,(Vector *)0x23,(Vector *)0x0,extraout_s0_03);
    }
    fVar44 = *(float *)(param_1 + 0x270);
    fVar47 = *(float *)(param_1 + 0x268);
    fVar45 = fVar44 - *(float *)(param_1 + 0x274);
    fVar46 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    fVar41 = fVar47 - *(float *)(param_1 + 0x26c);
    fVar40 = -fVar45;
    if (0.0 < fVar45) {
      fVar40 = fVar45;
    }
    fVar45 = -fVar41;
    if (0.0 < fVar41) {
      fVar45 = fVar41;
    }
    if (fVar45 < fVar40) {
      fVar45 = fVar40;
    }
    iVar7 = *(int *)param_1;
    fVar40 = *(float *)(iVar7 + 0x60) + fVar46 * DAT_000ada8c * 0.5 * fVar45;
    *(float *)(iVar7 + 0x60) = fVar40;
    uVar6 = in_fpscr & 0xfffffff | (uint)(fVar40 < 1.0) << 0x1f;
    in_fpscr = uVar6 | (uint)NAN(fVar40) << 0x1c;
    *(float *)(param_1 + 0x26c) = fVar47;
    *(float *)(param_1 + 0x274) = fVar44;
    if (((((byte)(uVar6 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) &&
         (*(char *)(param_1 + 0x1ee) == '\0')) && (*(char *)(param_1 + 0x1ed) == '\0')) &&
       (Player::setHitpoints(iVar7), *(int *)(param_1 + 0x90) == 0)) {
      pEVar19 = operator_new(0x68);
      fVar40 = (float)Explosion::Explosion(pEVar19,0xc);
      *(Explosion **)(param_1 + 0x90) = pEVar19;
      Explosion::setScaling(fVar40);
      pVVar37 = *(Vector **)(param_1 + 0x90);
      AEGeometry::getPosition();
      local_d4[0] = 0.0;
      local_d4[1] = 0.0;
      local_d4[2] = 0.0;
      Explosion::start(pVVar37,(Vector *)local_98);
    }
    Player::getHitpoints();
    FModSound::setParamValue((FModSound *)*piVar17,0,0x23,extraout_s0_04);
  }
  iVar7 = Player::getGammaHP();
  fVar40 = extraout_s1_14;
  if (((iVar7 < 1) && (*(char *)(param_1 + 0x1ee) == '\0')) && (*(char *)(param_1 + 0x1ec) == '\0'))
  {
    Player::setHitpoints(*(int *)param_1);
    fVar40 = extraout_s1_15;
  }
  if (*(char *)(param_1 + 0x1c0) != '\0') {
    *(undefined1 *)(param_1 + 0x145) = 1;
    if (((*(int *)(param_1 + 0x1bc) == 0) || (iVar7 = KIPlayer::isDying(), iVar7 != 0)) ||
       (iVar7 = KIPlayer::isDead(), iVar7 != 0)) {
      dockToAsteroid((KIPlayer *)param_1,(Radar *)0x0);
      goto LAB_000aef94;
    }
    approachAsteroid((Hud *)param_1,(int)param_4,extraout_r2);
    fVar40 = extraout_s1_16;
  }
  if ((*(char *)(param_1 + 0x356) != '\0') &&
     (iVar7 = Player::getHitpoints(), fVar40 = extraout_s1_17, 0 < iVar7)) {
    *(undefined1 *)(param_1 + 0x145) = 1;
    if ((*(int *)(param_1 + 0x1bc) == 0) ||
       ((iVar7 = KIPlayer::isDying(), iVar7 != 0 || (iVar7 = KIPlayer::isDead(), iVar7 != 0)))) {
      dockToDockingPoint((KIPlayer *)param_1,(Radar *)0x0);
      goto LAB_000aef94;
    }
    iVar7 = approachDockingPoint
                      ((PlayerEgo *)param_1,(Hud *)param_4,extraout_r2_00,(Radar *)param_3);
    fVar40 = extraout_s1_18;
    if (iVar7 != 0) {
      this_01 = (Mission *)Status::getMission();
      this_02 = (Mission *)Status::getCampaignMission();
      iVar7 = Mission::isEmpty();
      if ((iVar7 == 0) &&
         (((iVar7 = Mission::getType(), iVar7 == 0xf || (iVar7 = Mission::getType(), iVar7 == 0xb8))
          && (iVar7 = PlayerFixedObject::getDockingType(), iVar7 == 1)))) {
        iVar7 = *(int *)(param_1 + 0x364);
        fVar40 = extraout_s1_19;
        if (iVar7 != *(int *)(param_1 + 0x360)) {
          iVar8 = Mission::getType();
          pfVar23 = (float *)&DAT_000aded4;
          fVar40 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x35c),
                                              (byte)(in_fpscr >> 0x16) & 3);
          if (iVar8 == 0xb8) {
            pfVar23 = (float *)&DAT_000aded8;
          }
          iVar8 = (int)(fVar40 / *pfVar23);
          *(int *)(param_1 + 0x364) = (int)(fVar40 / *pfVar23);
          fVar40 = extraout_s1_20;
          if (*(int *)(param_1 + 0x360) <= iVar8) {
            if (iVar7 < iVar8) {
              iVar8 = Mission::getType();
              if (iVar8 == 0xb8) {
                pPVar24 = (PlayerEgo *)0x26;
              }
              else {
                pPVar24 = (PlayerEgo *)0x2a;
              }
              Hud::hudEvent((int)param_4,pPVar24,param_1);
              fVar40 = extraout_s1_21;
            }
            iVar8 = *(int *)(param_1 + 0x360);
            *(int *)(param_1 + 0x364) = iVar8;
          }
          if (iVar7 < iVar8) {
            iVar8 = Mission::getType();
            if (iVar8 == 0xb8) {
              iVar8 = *(int *)(*piVar30 + 0x178);
              fVar40 = extraout_s1_22;
              if (0 < iVar8) {
                *(int *)(*piVar30 + 0x178) = (iVar8 + iVar7) - *(int *)(param_1 + 0x364);
                Mission::getStatusValue();
                Mission::setStatusValue((int)this_01);
                uVar6 = Mission::getStatusValue();
                fVar40 = extraout_s1_23;
                if (0x7fffffff < uVar6) {
                  Mission::setStatusValue((int)this_01);
                  fVar40 = extraout_s1_24;
                }
                if (0x7fffffff < *(uint *)(*piVar30 + 0x178)) {
                  *(undefined4 *)(*piVar30 + 0x178) = 0;
                }
              }
            }
            else {
              iVar8 = Mission::getType();
              fVar40 = extraout_s1_45;
              if (iVar8 == 0xf) {
                iVar8 = Status::getShip();
                iVar20 = Mission::getProductionGoodIndex(this_01);
                Ship::removeCargo(iVar8,iVar20);
                Level::incNumDeliveredOre
                          (*(Level **)(param_1 + 0xc),*(int *)(param_1 + 0x364) - iVar7);
                fVar40 = extraout_s1_46;
              }
            }
          }
        }
      }
      else {
        iVar7 = Mission::isEmpty();
        if ((iVar7 == 0) &&
           (((iVar7 = Mission::getType(), iVar7 == 0xb8 ||
             (iVar7 = Mission::getType(), iVar7 == 0xa8)) &&
            (iVar7 = PlayerFixedObject::getDockingType(), iVar7 == 2)))) {
          iVar7 = *(int *)(param_1 + 0x364);
          fVar40 = extraout_s1_25;
          if (iVar7 != *(int *)(param_1 + 0x360)) {
            fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x35c),
                                                (byte)(in_fpscr >> 0x16) & 3);
            iVar8 = (int)(fVar47 / DAT_000adedc);
            *(int *)(param_1 + 0x364) = (int)(fVar47 / DAT_000adedc);
            if (*(int *)(param_1 + 0x360) <= iVar8) {
              if (iVar7 < iVar8) {
                iVar8 = Mission::getType();
                if ((iVar8 == 0xb8) || (iVar8 = Mission::getType(), iVar8 == 0xa8)) {
                  pPVar24 = (PlayerEgo *)0x24;
                }
                else {
                  pPVar24 = (PlayerEgo *)0x28;
                }
                Hud::hudEvent((int)param_4,pPVar24,param_1);
                fVar40 = extraout_s1_47;
              }
              iVar8 = *(int *)(param_1 + 0x360);
              *(int *)(param_1 + 0x364) = iVar8;
            }
            if (iVar7 < iVar8) {
              *(int *)(*piVar30 + 0x178) = (iVar8 - iVar7) + *(int *)(*piVar30 + 0x178);
            }
          }
        }
        else if (((this_02 == (Mission *)0x0) || (iVar7 = Mission::isEmpty(), iVar7 != 0)) ||
                (((iVar7 = Mission::getType(), iVar7 != 0xa7 &&
                  (iVar7 = Mission::getType(), iVar7 != 0xae)) ||
                 (iVar7 = PlayerFixedObject::getDockingType(), iVar7 != 1)))) {
          iVar7 = PlayerFixedObject::getDockingType();
          bVar38 = iVar7 == 3;
          if (bVar38) {
            iVar7 = *(int *)(param_1 + 0x1e8);
          }
          fVar40 = extraout_s1_29;
          if (bVar38 && iVar7 == 0) {
            iVar7 = 0;
            for (iVar8 = 0; iVar20 = Level::getNumDockingTargets(), iVar8 < iVar20;
                iVar8 = iVar8 + 1) {
              iVar29 = *(int *)(param_1 + 0x1bc);
              iVar20 = Level::getDockingTarget(*(int *)(param_1 + 0xc));
              if (iVar29 == iVar20) {
                iVar7 = iVar8;
              }
            }
            iVar8 = Status::getCurrentCampaignMission();
            this_03 = operator_new(0x140);
            iVar20 = 4;
            if (iVar8 == 0x5b) {
              iVar20 = 1;
            }
            HackingGame::HackingGame(this_03,0,iVar20,-1,-1,iVar7);
            *(HackingGame **)(param_1 + 0x1e8) = this_03;
            Hud::setHackingGameActive((Hud *)param_4,true);
            fVar40 = extraout_s1_44;
          }
        }
        else {
          iVar7 = *(int *)(param_1 + 0x360);
          iVar8 = *(int *)(param_1 + 0x364);
          fVar40 = extraout_s1_26;
          if (iVar8 != iVar7) {
            fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x35c),
                                                (byte)(in_fpscr >> 0x16) & 3);
            iVar20 = (int)(fVar47 / DAT_000adee0);
            *(int *)(param_1 + 0x364) = (int)(fVar47 / DAT_000adee0);
            if (iVar7 <= iVar20) {
              if (iVar8 < iVar20) {
                Hud::hudEvent((int)param_4,(PlayerEgo *)0x2a,param_1);
                iVar7 = *(int *)(param_1 + 0x360);
                fVar40 = extraout_s1_27;
              }
              *(int *)(param_1 + 0x364) = iVar7;
              iVar20 = iVar7;
            }
            if (iVar8 < iVar20) {
              iVar7 = Status::getShip();
              iVar8 = Mission::getProductionGoodIndex(this_02);
              Ship::removeCargo(iVar7,iVar8);
              Mission::getStatusValue();
              Mission::setStatusValue((int)this_02);
              fVar40 = extraout_s1_28;
            }
          }
        }
      }
      *(int *)(param_1 + 0x35c) = *(int *)(param_1 + 0x134) + *(int *)(param_1 + 0x35c);
    }
    if ((*(char *)(param_1 + 0x1a0) != '\0') || (*(char *)(param_1 + 0x1a1) != '\0')) {
      handleTurretView((PlayerEgo *)param_1,(int)param_2);
      fVar40 = extraout_s1_30;
    }
  }
  fVar47 = DAT_000adee4;
  if ((*(char *)(param_1 + 0x158) == '\0') || (*(int *)(param_1 + 0x15c) == 0)) {
    if (*(char *)(param_1 + 0x145) == '\0') {
      if ((*(char *)(param_1 + 0x1a0) == '\0') && (*(char *)(param_1 + 0x1a1) == '\0')) {
        roll((PlayerEgo *)param_1,*(int *)(param_1 + 0x134));
        iVar7 = updateManeuver((PlayerEgo *)param_1);
        fVar40 = extraout_s1_33;
        if (iVar7 == 0) {
          handleShip((PlayerEgo *)param_1,(int)param_2);
          fVar40 = extraout_s1_34;
        }
      }
      else {
LAB_000ae0b2:
        handleTurretView((PlayerEgo *)param_1,(int)param_2);
        fVar40 = extraout_s1_32;
      }
    }
  }
  else if ((*(char *)(param_1 + 0x145) == '\0') &&
          (iVar7 = updateManeuver((PlayerEgo *)param_1), fVar40 = extraout_s1_31, iVar7 == 0)) {
    if (*(char *)(param_1 + 0x160) == '\0') {
      piVar17 = *(int **)(param_1 + 0x15c);
    }
    else {
      piVar17 = *(int **)(param_1 + 0x15c);
      if (piVar17[0x4d] != 0) {
        piVar17 = (int *)Route::getWaypoint();
        *(int **)(param_1 + 0x15c) = piVar17;
      }
    }
    if ((piVar17 == (int *)0x0) || (*(char *)(param_1 + 0x1ed) != '\0')) {
      setAutoPilot((KIPlayer *)param_1);
      fVar40 = extraout_s1_35;
    }
    else {
      (**(code **)(*piVar17 + 0x28))((Vector *)local_98);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0xec),(Vector *)local_98);
      getPosition();
      AbyssEngine::AEMath::operator-((Vector *)local_98,(Vector *)(param_1 + 0xec));
      fVar40 = (float)AbyssEngine::AEMath::VectorLength((Vector *)local_98);
      if ((int)fVar40 < 20000) {
        *(undefined1 *)(param_1 + 0x330) = 1;
      }
      uVar31 = *(undefined4 *)(param_1 + 0xec);
      uVar35 = *(undefined4 *)(param_1 + 0xf0);
      uVar9 = *(undefined4 *)(param_1 + 0xf4);
      pSVar13 = (Ship *)Status::getShip();
      fVar47 = (float)Ship::getHandling(pSVar13);
      fVar40 = DAT_000ae484;
      fVar41 = 4.0;
      in_fpscr = in_fpscr & 0xfffffff;
      if (fVar47 + DAT_000ae484 < 4.0) {
        pSVar13 = (Ship *)Status::getShip();
        fVar41 = (float)Ship::getHandling(pSVar13);
        fVar41 = fVar41 + fVar40;
      }
      fVar47 = (float)moveToPosition(param_1,uVar31,uVar35,uVar9,1,fVar41);
      fVar40 = extraout_s1_39;
      if (*(char *)(param_1 + 0x1a0) != '\0') goto LAB_000ae0b2;
    }
  }
  if ((((*(char *)(param_1 + 0x356) != '\0') && (*(uint *)(param_1 + 0x1c4) != 1)) &&
      (*(char *)(param_1 + 0x1a0) == '\0')) && ((*(uint *)(param_1 + 0x1c4) | 1) != 3)) {
    updateManeuver((PlayerEgo *)param_1);
    fVar40 = extraout_s1_36;
  }
  if (((*(char *)(param_1 + 0x180) != '\0') && (*(char *)(param_1 + 0x355) != '\0')) &&
     ((*(char *)(param_1 + 0x354) == '\0' ||
      ((int)(uint)(*(uint *)(param_6 + 8) < 10000) <= *(int *)(param_6 + 0xc))))) {
    iVar7 = isDead((PlayerEgo *)param_1);
    if (iVar7 == 0) {
      handleAutoTurret((PlayerEgo *)param_1,(int)param_2);
      fVar40 = extraout_s1_38;
    }
    else {
      *(undefined1 *)(param_1 + 0x355) = 0;
      Player::stopShooting(*(int *)param_1);
      fVar40 = extraout_s1_37;
    }
  }
  uVar4 = DAT_000aeef8;
  uVar3 = DAT_000aeef0;
  if ((*(char *)(param_1 + 0x1c0) == '\0') && (1 < *(int *)(param_1 + 0x1c4) - 1U)) {
    uStack_7c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_78 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_74 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar26 = (undefined4 *)((uint)local_98 | 4);
    *puVar26 = 0;
    puVar26[1] = uStack_7c;
    puVar26[2] = uStack_78;
    puVar26[3] = uStack_74;
    local_80 = 0;
    uStack_70 = uVar3;
    uStack_68 = uVar4;
    local_98[0] = 1.0;
    local_84 = 0x3f800000;
    local_60 = 0x3f800000;
    if ((*(int *)(param_1 + 0x15c) == 0) || (*(char *)(param_1 + 0x1ed) != '\0')) {
      bVar38 = false;
    }
    else {
      fVar44 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      fVar41 = *(float *)(param_1 + 0x280);
      uVar6 = in_fpscr & 0xfffffff;
      uVar25 = uVar6 | (uint)(fVar41 < fVar47) << 0x1f | (uint)(fVar41 == fVar47) << 0x1e;
      in_fpscr = uVar25 | (uint)(NAN(fVar41) || NAN(fVar47)) << 0x1c;
      fVar44 = *(float *)(param_1 + 0x154) * fVar44 * DAT_000adee8;
      if ((int)uVar25 < 0) {
        fVar41 = fVar41 + fVar44;
        uVar25 = uVar6 | (uint)(fVar41 < fVar47) << 0x1f | (uint)(fVar41 == fVar47) << 0x1e;
        uVar6 = uVar25 | (uint)(NAN(fVar41) || NAN(fVar47)) << 0x1c;
        *(float *)(param_1 + 0x280) = fVar41;
        bVar2 = (byte)(uVar25 >> 0x18);
        in_fpscr = uVar6;
        if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar6 >> 0x1c) & 1)) {
LAB_000ae200:
          *(float *)(param_1 + 0x280) = fVar47;
          in_fpscr = uVar6;
          fVar41 = fVar47;
        }
      }
      else if (!(bool)((byte)(uVar25 >> 0x1e) & 1) && ((byte)(in_fpscr >> 0x1c) & 1) == 0) {
        fVar41 = fVar41 - fVar44;
        *(float *)(param_1 + 0x280) = fVar41;
        in_fpscr = uVar6;
        if (fVar41 < fVar47) goto LAB_000ae200;
      }
      bVar38 = true;
      *(float *)(param_1 + 0x27c) = fVar41;
    }
    in_q8._0_8_ = (double)*(float *)(param_1 + 0x278) * 0.25 * DAT_000adef0 * DAT_000adef8;
    in_q8._8_8_ = DAT_000adef8;
    AbyssEngine::AEMath::MatrixSetRotation
              ((Matrix *)local_d4,*(float *)(param_1 + 0x27c) * DAT_000adf00 * DAT_000adf04,fVar40,
               DAT_000adf04);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)local_98,(Matrix *)local_d4);
    AEGeometry::setMatrix(*(Matrix **)(param_1 + 4));
    if (bVar38) {
      pMVar18 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
      AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
      AbyssEngine::AEMath::operator*((Matrix *)local_d4,pMVar18);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)param_1 + 4),(Matrix *)local_d4);
    }
    fVar40 = *(float *)(param_1 + 0x27c);
    in_fpscr = in_fpscr & 0xfffffff;
    uVar6 = in_fpscr | (uint)(fVar40 < 0.0) << 0x1f | (uint)(fVar40 == 0.0) << 0x1e;
    bVar2 = (byte)(uVar6 >> 0x18);
    if (**(int **)(DAT_000ae478 + 0xae2b4) == 0) {
      if (((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != NAN(fVar40)) ||
         (*(int *)(param_1 + 0x104) != 0)) {
        if ((fVar40 < 0.0) && (*(int *)(param_1 + 0x104) == 0)) {
          fVar47 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
          fVar40 = fVar40 + (*(float *)(param_1 + 0x154) * fVar47) / DAT_000ae480;
          goto LAB_000ae32a;
        }
        goto LAB_000ae428;
      }
      fVar47 = (float)VectorSignedToFloat(param_2,(byte)(uVar6 >> 0x16) & 3);
      fVar40 = fVar40 + (*(float *)(param_1 + 0x154) * fVar47) / DAT_000ae488;
LAB_000ae414:
      *(float *)(param_1 + 0x27c) = fVar40;
      if (fVar40 < 0.0) {
LAB_000ae422:
        *(undefined4 *)(param_1 + 0x27c) = 0;
      }
    }
    else {
      if ((!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == NAN(fVar40)) &&
         (*(int *)(param_1 + 0x104) == 0)) {
        fVar40 = fVar40 * DAT_000ae47c;
        goto LAB_000ae414;
      }
      if ((fVar40 < 0.0) && (*(int *)(param_1 + 0x104) == 0)) {
        fVar40 = fVar40 * DAT_000ae47c;
LAB_000ae32a:
        in_fpscr = in_fpscr | (uint)(fVar40 < 0.0) << 0x1f | (uint)(fVar40 == 0.0) << 0x1e;
        *(float *)(param_1 + 0x27c) = fVar40;
        bVar2 = (byte)(in_fpscr >> 0x18);
        if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != NAN(fVar40)) goto LAB_000ae428;
        goto LAB_000ae422;
      }
    }
LAB_000ae428:
    fVar40 = *(float *)(param_1 + 0x278);
    in_fpscr = in_fpscr & 0xfffffff;
    uVar6 = in_fpscr | (uint)(fVar40 < 0.0) << 0x1f | (uint)(fVar40 == 0.0) << 0x1e;
    uVar25 = uVar6 | (uint)NAN(fVar40) << 0x1c;
    bVar2 = (byte)(uVar6 >> 0x18);
    if (((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar25 >> 0x1c) & 1)) ||
       (*(int *)(param_1 + 0x100) != 0)) {
      if ((fVar40 < 0.0) && (*(int *)(param_1 + 0x100) == 0)) {
        fVar47 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
        fVar40 = fVar40 + (*(float *)(param_1 + 0x154) * fVar47) / DAT_000ae480;
        uVar6 = in_fpscr | (uint)(fVar40 < 0.0) << 0x1f | (uint)(fVar40 == 0.0) << 0x1e;
        in_fpscr = uVar6 | (uint)NAN(fVar40) << 0x1c;
        *(float *)(param_1 + 0x278) = fVar40;
        bVar2 = (byte)(uVar6 >> 0x18);
        if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))
        goto LAB_000ae4b6;
      }
    }
    else {
      fVar47 = (float)VectorSignedToFloat(param_2,(byte)(uVar25 >> 0x16) & 3);
      fVar40 = fVar40 + (*(float *)(param_1 + 0x154) * fVar47) / DAT_000ae488;
      *(float *)(param_1 + 0x278) = fVar40;
      if (fVar40 < 0.0) {
LAB_000ae4b6:
        *(undefined4 *)(param_1 + 0x278) = 0;
      }
    }
    if (*(int *)(param_1 + 0x194) != 0) {
      TargetFollowCamera::setRumblePercentage(fVar40,*(int *)(param_1 + 0x88));
    }
    *(undefined4 *)(param_1 + 600) = 0;
    *(undefined4 *)(param_1 + 0x25c) = 0;
    *(undefined4 *)(param_1 + 0x100) = 0;
    *(undefined4 *)(param_1 + 0x104) = 0;
    *(undefined4 *)(param_1 + 0x270) = 0;
    *(undefined4 *)(param_1 + 0x268) = 0;
  }
  pSVar13 = (Ship *)Status::getShip();
  iVar7 = Ship::getShieldRegen(pSVar13);
  if ((0 < iVar7) && (iVar7 = isDead((PlayerEgo *)param_1), iVar7 == 0)) {
    uVar6 = *(uint *)(param_1 + 0x118);
    iVar7 = *(int *)(param_1 + 0x11c) + ((int)param_2 >> 0x1f) + (uint)CARRY4(uVar6,(uint)param_2);
    *(Radar **)(param_1 + 0x118) = param_2 + uVar6;
    *(int *)(param_1 + 0x11c) = iVar7;
    if ((int)(uint)(param_2 + uVar6 < (Radar *)0x65) <= iVar7) {
      *(undefined4 *)(param_1 + 0x118) = 0;
      *(undefined4 *)(param_1 + 0x11c) = 0;
      Player::regenerateShield(*(Player **)param_1,extraout_s0_06);
    }
  }
  pSVar13 = (Ship *)Status::getShip();
  iVar7 = Ship::getRepairType(pSVar13);
  if ((-1 < iVar7) && (iVar8 = isDead((PlayerEgo *)param_1), iVar8 == 0)) {
    pauVar28 = (undefined1 (*) [16])(param_1 + 0x120);
    pfVar23 = (float *)&DAT_000af09c;
    if (iVar7 == 0) {
      pfVar23 = (float *)&DAT_000af0a0;
    }
    uVar1 = in_q8._0_8_ & 0xffff0000ffff0000 | ZEXT48(param_2) & 0xffff;
    auVar50._8_8_ = uVar1;
    auVar50._0_8_ = uVar1;
    fVar47 = *pfVar23;
    auVar50 = VectorAdd(*pauVar28,auVar50,8);
    *(longlong *)*pauVar28 = auVar50._0_8_;
    *(longlong *)(param_1 + 0x128) = auVar50._8_8_;
    fVar40 = (float)__aeabi_l2f(auVar50._0_4_,auVar50._4_4_);
    in_fpscr = in_fpscr & 0xfffffff;
    if (fVar47 < fVar40) {
      *(undefined4 *)*pauVar28 = 0;
      *(undefined4 *)(param_1 + 0x124) = 0;
      iVar8 = Player::getHitpoints();
      iVar20 = Player::getMaxHitpoints();
      if (iVar8 < iVar20) {
        Player::regenerateHull(*(Player **)param_1);
      }
    }
    pfVar23 = (float *)&DAT_000af0a4;
    if (iVar7 == 0) {
      pfVar23 = (float *)&DAT_000af0a8;
    }
    fVar40 = (float)__aeabi_l2f(*(undefined4 *)(param_1 + 0x128),*(undefined4 *)(param_1 + 300));
    in_fpscr = in_fpscr & 0xfffffff;
    if (*pfVar23 < fVar40) {
      *(undefined4 *)(param_1 + 0x128) = 0;
      *(undefined4 *)(param_1 + 300) = 0;
      iVar7 = Player::getHitpoints();
      iVar8 = Player::getMaxHitpoints();
      if (iVar8 <= iVar7) {
        iVar7 = Player::getArmorHP();
        iVar8 = Player::getMaxArmorHP(*(Player **)param_1);
        if (iVar7 < iVar8) {
          Player::regenerateArmor(*(Player **)param_1);
        }
      }
    }
  }
  if ((*(int *)(param_1 + 0xac) != 0) && (0 < *(int *)(param_1 + 0x30c))) {
    Player::setVulnerable(SUB41(*(undefined4 *)param_1,0));
    *(int *)(param_1 + 0x30c) = *(int *)(param_1 + 0x30c) - (int)param_2;
    AEGeometry::setMatrix(*(Matrix **)(param_1 + 0xac));
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)local_98,(Vector *)(*(int *)param_1 + 4));
    AEGeometry::translate(*(Vector **)(param_1 + 0xac));
    iVar7 = AbyssEngine::PaintCanvas::MeshGetPointer
                      ((PaintCanvas *)*puVar12,*(uint *)(*(int *)(param_1 + 0xac) + 0x1c));
    fVar40 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(iVar7 + 0x24) = *(float *)(iVar7 + 0x24) + fVar40 * DAT_000ae7f0;
    fVar40 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x310),(byte)(in_fpscr >> 0x16) & 3);
    fVar47 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x30c),(byte)(in_fpscr >> 0x16) & 3);
    in_fpscr = in_fpscr & 0xfffffff;
    if (fVar47 <= fVar40 * DAT_000ae7f4) {
      fVar41 = fVar40 * DAT_000ae7f8;
      uVar6 = in_fpscr | (uint)(fVar41 < fVar47) << 0x1f | (uint)(fVar41 == fVar47) << 0x1e;
      in_fpscr = uVar6 | (uint)(NAN(fVar41) || NAN(fVar47)) << 0x1c;
      bVar2 = (byte)(uVar6 >> 0x18);
      if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))
      goto LAB_000ae6d2;
      fVar47 = 1.0;
    }
    else {
      fVar47 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x310) - *(int *)(param_1 + 0x30c),
                                          (byte)(in_fpscr >> 0x16) & 3);
LAB_000ae6d2:
      fVar47 = fVar47 / (fVar40 * DAT_000ae7f8);
    }
    AEGeometry::setScaling
              (fVar47 * *(float *)(param_1 + 800),extraout_s1_40,*(float *)(param_1 + 800));
    uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar12);
    AbyssEngine::Transform::Update((ulonglong)uVar6,bVar39);
    if (*(int *)(param_1 + 0x30c) < 1) {
      if (*(AEGeometry **)(param_1 + 0xac) != (AEGeometry *)0x0) {
        pvVar11 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(param_1 + 0xac));
        operator_delete(pvVar11);
      }
      *(undefined4 *)(param_1 + 0xac) = 0;
      Player::setVulnerable(SUB41(*(undefined4 *)param_1,0));
      FModSound::stop(**(int **)(DAT_000ae7fc + 0xae73e));
      *(undefined4 *)(*piVar30 + 0x13c) = 0;
    }
  }
  iVar7 = *(int *)param_1;
  isDead((PlayerEgo *)param_1);
  Player::update(iVar7,bVar39);
  if (((*(char *)(param_1 + 0x38) != '\0') &&
      (*(char *)(*(int *)(DAT_000ae800 + 0xae77c) + 0xf) != '\0')) &&
     (*(int *)(param_1 + 0xb4) != -1)) {
    Player::getPosition();
    FModSound::updateEvent3DAttributes
              ((FModSound *)**(undefined4 **)(DAT_000ae804 + 0xae7a2),*(int *)(param_1 + 0xb4),
               (Vector *)local_98,(Vector *)0x0,false);
  }
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
  AbyssEngine::AEMath::MatrixGetUp((Matrix *)local_98);
  AbyssEngine::AEMath::Vector::operator=(*(Vector **)(DAT_000ae808 + 0xae7cc),(Vector *)local_98);
  if (*(char *)(param_1 + 0x13c) == '\0') {
    iVar7 = *(int *)(DAT_000aeb44 + 0xae814);
    uVar9 = *(undefined4 *)(iVar7 + 0x2b4);
    ParticleSettings::{unnamed_type#1}::operator=
              ((_unnamed_type_1_ *)(iVar7 + 0x280),
               (_unnamed *)(*(int *)(DAT_000aeb48 + 0xae816) + 0x280));
    *(undefined4 *)(iVar7 + 0x2b4) = uVar9;
  }
  else {
    uVar9 = getBoostPercentage();
    ParticleSettings::Interpolate(4,6,uVar9,4);
  }
  if (*(int *)(param_1 + 0xfc) != 0) {
    iVar7 = Route::getCurrent();
    Route::update(*(Route **)(param_1 + 0xfc),(Vector *)(param_1 + 0x148));
    iVar8 = Route::getCurrent();
    if (iVar8 != iVar7) {
      if ((*(char *)(*(int *)(param_1 + 0xfc) + 4) == '\0') &&
         (iVar7 = Route::getCurrent(), iVar7 == 0)) {
        pPVar24 = (PlayerEgo *)0x18;
      }
      else {
        pPVar24 = (PlayerEgo *)0x17;
      }
      Hud::hudEvent((int)param_4,pPVar24,param_1);
    }
  }
  puVar36 = *(uint **)(DAT_000aeb4c + 0xae874);
  *(undefined4 *)(param_1 + 0x20) = 0;
  *(undefined1 *)(param_1 + 0x30a) = 0;
  if (*puVar36 != (uint)*(byte *)(param_1 + 0x378)) {
    *(undefined1 *)(param_1 + 0x84) = 1;
  }
  iVar8 = *(int *)(param_1 + 0x130);
  iVar7 = Player::getCombinedHP(*(Player **)param_1);
  if (((iVar7 < iVar8) && (*(char *)(param_1 + 500) == '\0')) && (*(char *)(param_1 + 0x24) == '\0')
     ) {
    Hud::playerHit((Hud *)param_4);
    uVar9 = Player::getCombinedHP(*(Player **)param_1);
    *(undefined4 *)(param_1 + 0x130) = uVar9;
    fVar40 = (float)hitCamera((PlayerEgo *)param_1);
    iVar7 = DAT_000aeb50;
    iVar8 = *(int *)param_1;
    if (*(char *)(iVar8 + 100) == '\0') {
      if (*(char *)(iVar8 + 0x65) == '\0') {
        if (*(char *)(iVar8 + 0x66) != '\0') {
          FModSound::play(**(int **)(DAT_000aeb58 + 0xae93a),(Vector *)0x18,(Vector *)0x0,fVar40);
          *(undefined1 *)(*(int *)param_1 + 0x66) = 0;
        }
      }
      else {
        FModSound::play(**(int **)(DAT_000aeb54 + 0xae914),(Vector *)0x17,(Vector *)0x0,fVar40);
        *(undefined1 *)(*(int *)param_1 + 0x65) = 0;
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x30a) = 1;
      FModSound::play(**(int **)(iVar7 + 0xae8e4),(Vector *)0x19,(Vector *)0x0,fVar40);
      *(undefined1 *)(*(int *)param_1 + 100) = 0;
    }
    Player::getHitVector();
    fVar40 = (float)AbyssEngine::AEMath::VectorNormalize((AEMath *)local_98,(Vector *)local_d4);
    pVVar37 = (Vector *)*puVar12;
    local_d4[0] = 0.0;
    local_d4[1] = 0.0;
    local_d4[2] = 0.0;
    AbyssEngine::AEMath::operator*((Vector *)&local_e0,fVar40);
    AbyssEngine::AEMath::operator+((Vector *)&local_58,(Vector *)&local_e0);
    AbyssEngine::PaintCanvas::GetScreenPosition(pVVar37,(Vector *)&local_58);
    uVar6 = in_fpscr & 0xfffffff | (uint)(local_d4[0] == 0.0) << 0x1e |
            (uint)(0.0 <= local_d4[0]) << 0x1d;
    bVar2 = (byte)(uVar6 >> 0x18);
    if (!(bool)(bVar2 >> 5 & 1) || (bool)(bVar2 >> 6)) {
      uVar25 = 1;
    }
    else {
      uVar25 = 2;
      fVar40 = (float)VectorSignedToFloat(**(undefined4 **)(DAT_000aeb60 + 0xae9a6),
                                          (byte)(uVar6 >> 0x16) & 3);
      uVar6 = in_fpscr & 0xfffffff | (uint)(local_d4[0] < fVar40) << 0x1f;
      if (SUB41(uVar6 >> 0x1f,0) != (NAN(local_d4[0]) || NAN(fVar40))) {
        uVar25 = 0;
      }
    }
    *(uint *)(param_1 + 0x20) = uVar25;
    fVar40 = DAT_000aeb64;
    uVar6 = uVar6 & 0xfffffff;
    if (0.0 <= local_d4[2]) {
      uVar21 = uVar25 | 0x20;
      *(uint *)(param_1 + 0x20) = uVar21;
      if (fVar40 <= local_d4[0]) {
        if (local_d4[0] < DAT_000aeb68) {
          uVar21 = uVar25 | 0x21;
          goto LAB_000aea06;
        }
      }
      else {
        uVar21 = 1;
LAB_000aea06:
        *(uint *)(param_1 + 0x20) = uVar21;
      }
      uVar25 = uVar6 | (uint)(local_d4[0] < DAT_000aeb6c) << 0x1f |
               (uint)(local_d4[0] == DAT_000aeb6c) << 0x1e;
      in_fpscr = uVar25 | (uint)(NAN(local_d4[0]) || NAN(DAT_000aeb6c)) << 0x1c;
      bVar2 = (byte)(uVar25 >> 0x18);
      if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) {
        uVar6 = uVar6 | (uint)(local_d4[0] < DAT_000aeb70) << 0x1f |
                (uint)(local_d4[0] == DAT_000aeb70) << 0x1e;
        in_fpscr = uVar6 | (uint)(NAN(local_d4[0]) || NAN(DAT_000aeb70)) << 0x1c;
        bVar2 = (byte)(uVar6 >> 0x18);
        if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1))
        goto LAB_000aea32;
        uVar25 = uVar21 | 2;
      }
      else {
        uVar25 = 2;
      }
    }
    else {
      uVar25 = uVar25 | 0x10;
      in_fpscr = uVar6;
    }
    *(uint *)(param_1 + 0x20) = uVar25;
  }
LAB_000aea32:
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)local_d4);
  AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_58);
  uVar6 = extraout_r1;
  if (*(char *)(param_1 + 0x1a0) != '\0') {
    pMVar18 = (Matrix *)AEGeometry::getMatrix();
    AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*((Matrix *)local_98,pMVar18);
    AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_e0);
    AbyssEngine::AEMath::Vector::operator=((Vector *)local_d4,(Vector *)&local_e0);
    AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_e0);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_58,(Vector *)&local_e0);
    uVar6 = extraout_r1_00;
  }
  if ((*puVar36 == 0) || (param_8 != 0)) {
LAB_000aeac0:
    fVar40 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_ec,(Vector *)&local_58);
    AbyssEngine::AEMath::operator*(fVar40,(Vector *)&local_e0);
    AbyssEngine::AEMath::operator+((Vector *)local_98,(Vector *)local_d4);
    pVVar37 = *(Vector **)(DAT_000aeb78 + 0xaeaea);
    AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar12,(Vector *)local_98);
    if (*(float *)(pVVar37 + 8) <= 0.0) {
      fVar40 = (float)AbyssEngine::AEMath::operator*((Vector *)aAStack_ec,*(float *)(pVVar37 + 8));
      AbyssEngine::AEMath::operator*(aVStack_f8,fVar40);
      this_08 = (Vector *)&local_e0;
      AbyssEngine::AEMath::operator+(this_08,(Vector *)aAStack_ec);
    }
    else {
      local_e0 = *(undefined8 *)pVVar37;
      this_08 = (Vector *)&local_e0;
      local_d8 = *(undefined4 *)(pVVar37 + 8);
    }
  }
  else {
    bVar39 = in_stack_00000010 != 3;
    if (bVar39) {
      in_stack_00000010 = (uint)(in_stack_00000010 == 1);
      uVar6 = (uint)(*(char *)(param_1 + 0x158) == '\0');
    }
    if (!bVar39 || in_stack_00000010 == 0 && uVar6 == 0) goto LAB_000aeac0;
    fVar44 = (float)VectorSignedToFloat(**(int **)(DAT_000aeecc + 0xaeb8c) >> 1,
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar45 = (float)VectorSignedToFloat(**(int **)(DAT_000aeed0 + 0xaeb92) >> 1,
                                        (byte)(in_fpscr >> 0x16) & 3);
    piVar30 = *(int **)(DAT_000aeee0 + 0xaebb8);
    piVar17 = *(int **)(DAT_000aeee4 + 0xaebba);
    fVar46 = (float)VectorSignedToFloat(*piVar30,(byte)(in_fpscr >> 0x16) & 3);
    fVar47 = (float)((double)fVar44 * DAT_000aeed8);
    in_fpscr = in_fpscr & 0xfffffff;
    uVar6 = in_fpscr | (uint)(fVar47 < fVar46) << 0x1f | (uint)(fVar47 == fVar46) << 0x1e;
    bVar2 = (byte)(uVar6 >> 0x18);
    fVar41 = (float)VectorSignedToFloat(*piVar17,(byte)(uVar6 >> 0x16) & 3);
    fVar48 = (float)((double)fVar45 * DAT_000aeed8);
    fVar40 = fVar47;
    if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(fVar47) || NAN(fVar46))) {
      fVar40 = fVar46;
    }
    fVar46 = fVar48;
    if (fVar41 < fVar48) {
      fVar46 = fVar41;
    }
    if (fVar40 < -fVar47) {
      fVar40 = -fVar47;
    }
    if (fVar46 < -fVar48) {
      fVar46 = -fVar48;
    }
    uVar6 = (uint)fVar40;
    fVar43 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
    uVar25 = (uint)fVar46;
    fVar49 = (float)VectorSignedToFloat(uVar25,(byte)(in_fpscr >> 0x16) & 3);
    fVar41 = fVar44 + fVar47;
    if (fVar44 + fVar43 < fVar44 + fVar47) {
      fVar41 = fVar44 + fVar43;
    }
    fVar42 = fVar45 + fVar48;
    if (fVar45 + fVar49 < fVar45 + fVar48) {
      fVar42 = fVar45 + fVar49;
    }
    pfVar23 = *(float **)(DAT_000aeee8 + 0xaec8e);
    if (fVar41 < fVar44 - fVar47) {
      fVar41 = fVar44 - fVar47;
    }
    if (fVar42 < fVar45 - fVar48) {
      fVar42 = fVar45 - fVar48;
    }
    *pfVar23 = fVar41;
    *piVar30 = (int)fVar40;
    *piVar17 = (int)fVar46;
    pfVar23[1] = fVar42;
    if ((int)uVar6 < 1) {
      if (0x7fffffff < uVar6) {
        left(param_1,-fVar43 / fVar47);
      }
    }
    else {
      right(param_1,fVar43 / fVar47);
    }
    if ((int)uVar25 < 1) {
      if (0x7fffffff < uVar25) {
        up(param_1,-fVar49 / fVar48);
      }
    }
    else {
      down(param_1,fVar49 / fVar48);
    }
    fVar40 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_ec,(Vector *)&local_58);
    AbyssEngine::AEMath::operator*(fVar40,(Vector *)&local_e0);
    AbyssEngine::AEMath::operator+((Vector *)local_98,(Vector *)local_d4);
    this_08 = *(Vector **)(DAT_000af0bc + 0xaed38);
    AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar12,(Vector *)local_98);
    if (*(float *)(this_08 + 8) <= 0.0) {
      fVar40 = (float)AbyssEngine::AEMath::operator*((Vector *)aAStack_ec,*(float *)(this_08 + 8));
      AbyssEngine::AEMath::operator*(aVStack_f8,fVar40);
      AbyssEngine::AEMath::operator+((Vector *)&local_e0,(Vector *)aAStack_ec);
      AbyssEngine::AEMath::Vector::operator=(this_08,(Vector *)&local_e0);
    }
    else {
      local_e0 = *(undefined8 *)this_08;
      local_d8 = *(undefined4 *)(this_08 + 8);
      AbyssEngine::AEMath::Vector::operator=(this_08,(Vector *)&local_e0);
    }
    pVVar37 = (Vector *)(param_1 + 0xa0);
  }
  AbyssEngine::AEMath::Vector::operator=(pVVar37,this_08);
  if ((*(char *)(param_1 + 0x84) != '\0') &&
     (iVar7 = TargetFollowCamera::isInFastForwardMode(*(TargetFollowCamera **)(param_1 + 0x88)),
     iVar7 == 0)) {
    if (*puVar36 == 0) {
      TargetFollowCamera::resetShipHandling(*(TargetFollowCamera **)(param_1 + 0x88));
    }
    else {
      TargetFollowCamera::setShipHandling(*(TargetFollowCamera **)(param_1 + 0x88),extraout_s0_07);
    }
    *(undefined1 *)(param_1 + 0x84) = 0;
  }
  pVVar37 = *(Vector **)(DAT_000af0c0 + 0xaede2);
  *(bool *)(param_1 + 0x378) = *puVar36 != 0;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x94),pVVar37);
  iVar7 = isDead((PlayerEgo *)param_1);
  if ((iVar7 != 0) && (*(int *)(param_1 + 0x8c) != 0)) {
    fVar41 = (float)Player::stopShooting(*(int *)param_1);
    fVar40 = extraout_s2_10;
    fVar47 = extraout_s1_41;
    if (*(int *)(param_1 + 0x1b4) != 0) {
      fVar41 = (float)FModSound::stop(**(int **)(DAT_000af0c4 + 0xaee22));
      fVar40 = extraout_s2_11;
      fVar47 = extraout_s1_42;
    }
    if (*(int *)(param_1 + 0x1b8) != 0) {
      fVar41 = (float)FModSound::stop(**(int **)(DAT_000af0c8 + 0xaee36));
      fVar40 = extraout_s2_12;
      fVar47 = extraout_s1_43;
    }
    if ((*(char *)(param_1 + 0x356) == '\0') || (*(int *)(param_1 + 0x1c4) != 1)) {
      AEGeometry::rotate(fVar41,fVar47,fVar40);
    }
    iVar7 = *(int *)(param_1 + 0x2f8);
    if (iVar7 < 3000) {
      if (2999 < iVar7 + *(int *)(param_1 + 0x134)) {
        pVVar37 = *(Vector **)(param_1 + 0x8c);
        AEGeometry::getPosition();
        local_e0 = 0;
        local_d8 = 0;
        Explosion::start(pVVar37,(Vector *)local_98);
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(param_1 + 0xc) + 0x74),
                   SUB41(*(undefined4 *)(param_1 + 0x2fc),0));
        fVar40 = (float)ParticleSystemManager::enableSystemRender
                                  (*(int *)(*(int *)(param_1 + 0xc) + 0x74),
                                   SUB41(*(undefined4 *)(param_1 + 0x2fc),0));
        ParticleSystemManager::emitManual
                  (*(int *)(*(int *)(param_1 + 0xc) + 0x74),
                   *(Vector **)(*(int *)(param_1 + 0xc) + 0x3c),(int)local_d4,(Vector *)0x0,fVar40);
      }
    }
    else if (iVar7 != 3000) {
      pEVar19 = *(Explosion **)(param_1 + 0x8c);
      iVar7 = *(int *)(param_1 + 0x134);
      AEGeometry::getPosition();
      Explosion::update(pEVar19,iVar7,(Vector *)local_98);
    }
    pEVar19 = *(Explosion **)(param_1 + 0x90);
    if (pEVar19 != (Explosion *)0x0) {
      iVar7 = *(int *)(param_1 + 0x134);
      AEGeometry::getPosition();
      Explosion::update(pEVar19,iVar7,(Vector *)local_98);
    }
    *(int *)(param_1 + 0x2f8) = *(int *)(param_1 + 0x134) + *(int *)(param_1 + 0x2f8);
  }
  if (*(char *)(param_1 + 0x170) != '\0') {
    pMVar32 = *(Matrix **)(param_1 + 0x2c);
    pMVar18 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar12);
    AbyssEngine::AEMath::operator*((Matrix *)local_98,pMVar18);
    AEGeometry::setMatrix(pMVar32);
  }
LAB_000aef94:
  if (*piVar22 == local_4c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000ace38: push {r4,r5,r6,r7,lr}
  000ace3a: add r7,sp,#0xc
  000ace3c: push {r7,r8,r9,r10,r11}
  000ace40: vpush {d8,d9,d10,d11}
  000ace44: sub sp,#0xd8
  000ace46: mov r10,r0
  000ace48: ldr r0,[0x000ad208]
  000ace4a: mov r4,r1
  000ace4c: mov r8,r2
  000ace4e: add r0,pc
  000ace50: mov r11,r3
  000ace52: ldr r2,[r7,#0xc]
  000ace54: ldr r1,[r0,#0x0]
  000ace56: ldr r0,[r1,#0x0]
  000ace58: str r0,[sp,#0xd4]
  000ace5a: ldr.w r0,[r10,#0x220]
  000ace5e: str.w r2,[r10,#0x10]
  000ace62: cbnz r0,0x000ace6e
  000ace64: ldr r0,[r7,#0x8]
  000ace66: str.w r11,[r10,#0x220]
  000ace6a: strd r8,r0,[r10,#0x14]
  000ace6e: ldrb.w r0,[r10,#0x24]
  000ace72: cmp r0,#0x0
  000ace74: bne.w 0x000aef94
  000ace78: str r1,[sp,#0x14]
  000ace7a: add r5,sp,#0x88
  000ace7c: ldr.w r1,[r10,#0x8]
  000ace80: mov r0,r5
  000ace82: blx 0x000720b8
  000ace86: add.w r0,r10,#0x148
  000ace8a: mov r1,r5
  000ace8c: str r0,[sp,#0x10]
  000ace8e: blx 0x0006eb3c
  000ace92: ldr.w r0,[r10,#0x138]
  000ace96: str.w r4,[r10,#0x134]
  000ace9a: cmp.w r0,#0xffffffff
  000ace9e: bgt 0x000aceb0
  000acea0: add.w r1,r4,r4, lsl #0x1
  000acea4: add r1,r0
  000acea6: cmp r1,#0x1
  000acea8: itt ge
  000aceaa: mov.ge r0,#0x0
  000aceac: str.w.ge r0,[r10,#0x138]
  000aceb0: ldrb.w r1,[r10,#0x39a]
  000aceb4: add r0,r4
  000aceb6: str r4,[sp,#0x1c]
  000aceb8: cmp r1,#0x0
  000aceba: str.w r0,[r10,#0x138]
  000acebe: str.w r11,[sp,#0x18]
  000acec2: beq 0x000acedc
  000acec4: ldrb.w r0,[r10,#0x399]
  000acec8: ldr.w r9,[sp,#0x1c]
  000acecc: cbnz r0,0x000acee8
  000acece: ldr.w r0,[r10,#0x0]
  000aced2: blx 0x000726c4
  000aced6: cmp r0,#0x0
  000aced8: ble.w 0x000ad09c
  000acedc: ldrb.w r0,[r10,#0x399]
  000acee0: ldr.w r9,[sp,#0x1c]
  000acee4: cmp r0,#0x0
  000acee6: beq 0x000acfe2
  000acee8: ldr.w r0,[r10,#0x0]
  000aceec: blx 0x000726c4
  000acef0: mov r6,r0
  000acef2: ldr.w r0,[r10,#0x0]
  000acef6: blx 0x000726d0
  000acefa: cmp r6,r0
  000acefc: bge 0x000acf9c
  000acefe: ldr.w r6,[r10,#0x0]
  000acf02: mov r0,r6
  000acf04: blx 0x000726c4
  000acf08: vmov s0,r9
  000acf0c: vldr.32 s4,[pc,#0x2fc]
  000acf10: vmov.f32 s6,0x3f800000
  000acf14: vcvt.f32.s32 s0,s0
  000acf18: vmov s2,r0
  000acf1c: mov r0,r6
  000acf1e: vcvt.f32.s32 s2,s2
  000acf22: vmul.f32 s0,s0,s4
  000acf26: vmax.f32 d0,d0,d3
  000acf2a: vadd.f32 s0,s0,s2
  000acf2e: vcvt.s32.f32 s0,s0
  000acf32: vmov r1,s0
  000acf36: blx 0x000726dc
  000acf3a: ldr r0,[0x000ad210]
  000acf3c: add r0,pc
  000acf3e: ldr r4,[r0,#0x0]
  000acf40: ldr.w r0,[r10,#0x0]
  000acf44: ldr r6,[r4,#0x0]
  000acf46: blx 0x000726c4
  000acf4a: ldr.w r1,[r10,#0x0]
  000acf4e: vmov s0,r0
  000acf52: vcvt.f32.s32 s16,s0
  000acf56: mov r0,r1
  000acf58: blx 0x000726d0
  000acf5c: vmov s0,r0
  000acf60: mov r0,r6
  000acf62: movs r1,#0x0
  000acf64: movw r2,#0x8d1
  000acf68: vcvt.f32.s32 s0,s0
  000acf6c: movs r5,#0x0
  000acf6e: vdiv.f32 s0,s16,s0
  000acf72: vmov r3,s0
  000acf76: blx 0x0007159c
  000acf7a: add r6,sp,#0x4c
  000acf7c: mov r1,r10
  000acf7e: strd r5,r5,[sp,#0x88]
  000acf82: mov r0,r6
  000acf84: str r5,[sp,#0x90]
  000acf86: blx 0x0007264c
  000acf8a: ldr r0,[r4,#0x0]
  000acf8c: add r3,sp,#0x88
  000acf8e: movw r1,#0x8d1
  000acf92: mov r2,r6
  000acf94: str r5,[sp,#0x0]
  000acf96: blx 0x000726e8
  000acf9a: b 0x000acfe2
  000acf9c: ldr r0,[0x000ad214]
  000acf9e: movs r4,#0x0
  000acfa0: movw r1,#0x8d3
  000acfa4: movs r2,#0x0
  000acfa6: add r0,pc
  000acfa8: movs r3,#0x0
  000acfaa: str r4,[sp,#0x0]
  000acfac: ldr r5,[r0,#0x0]
  000acfae: ldr r0,[r5,#0x0]
  000acfb0: blx 0x00071548
  000acfb4: ldr r0,[r5,#0x0]
  000acfb6: movw r1,#0x8d1
  000acfba: blx 0x000724a8
  000acfbe: add r6,sp,#0x4c
  000acfc0: mov r1,r10
  000acfc2: strd r4,r4,[sp,#0x88]
  000acfc6: mov r0,r6
  000acfc8: str r4,[sp,#0x90]
  000acfca: blx 0x0007264c
  000acfce: ldr r0,[r5,#0x0]
  000acfd0: add r3,sp,#0x88
  000acfd2: movw r1,#0x8d3
  000acfd6: mov r2,r6
  000acfd8: str r4,[sp,#0x0]
  000acfda: blx 0x000726e8
  000acfde: strb.w r4,[r10,#0x399]
  000acfe2: ldr.w r0,[r10,#0x20c]
  000acfe6: cmp r0,#0x1
  000acfe8: blt 0x000ad002
  000acfea: sub.w r0,r0,r9
  000acfee: str.w r0,[r10,#0x20c]
  000acff2: cmp r0,#0x0
  000acff4: bgt 0x000ad002
  000acff6: mov r0,r11
  000acff8: movs r1,#0x2e
  000acffa: mov r2,r10
  000acffc: movs r3,#0x0
  000acffe: blx 0x00072418
  000ad002: ldr.w r0,[r10,#0x1e8]
  000ad006: cmp r0,#0x0
  000ad008: beq.w 0x000ad19e
  000ad00c: ldr.w r1,[r10,#0x134]
  000ad010: blx 0x000726f4
  000ad014: ldr.w r0,[r10,#0x1e8]
  000ad018: blx 0x00072700
  000ad01c: cmp r0,#0x0
  000ad01e: beq.w 0x000ad19e
  000ad022: mov r0,r11
  000ad024: movs r1,#0x0
  000ad026: blx 0x0007270c
  000ad02a: ldr r0,[0x000ad218]
  000ad02c: add r0,pc
  000ad02e: ldr r5,[r0,#0x0]
  000ad030: ldr r0,[r5,#0x0]
  000ad032: blx 0x00071c14
  000ad036: movs r1,#0x0
  000ad038: blx 0x00072718
  000ad03c: cmp r0,#0x0
  000ad03e: beq.w 0x000ad19e
  000ad042: ldr r0,[r5,#0x0]
  000ad044: blx 0x00071c14
  000ad048: blx 0x00072724
  000ad04c: mov r11,r0
  000ad04e: ldr r0,[0x000ad21c]
  000ad050: add r0,pc
  000ad052: ldr.w r1,[r0,r11,lsl #0x2]
  000ad056: ldr r0,[r5,#0x0]
  000ad058: blx 0x00072730
  000ad05c: ldr.w r0,[r10,#0x1e8]
  000ad060: cbz r0,0x000ad06a
  000ad062: blx 0x0007273c
  000ad066: blx 0x0006eb48
  000ad06a: ldr.w r0,[r10,#0xc]
  000ad06e: movs r1,#0x0
  000ad070: str.w r1,[r10,#0x1e8]
  000ad074: blx 0x00071eb4
  000ad078: mov r6,r0
  000ad07a: cmp r0,#0x0
  000ad07c: beq.w 0x000ad180
  000ad080: ldr r1,[r6,#0x0]
  000ad082: movs r4,#0x0
  000ad084: b 0x000ad096
  000ad086: ldr r0,[r6,#0x4]
  000ad088: ldr.w r0,[r0,r4,lsl #0x2]
  000ad08c: ldrb.w r2,[r0,#0x70]
  000ad090: cmp r2,#0x0
  000ad092: bne 0x000ad14c
  000ad094: adds r4,#0x1
  000ad096: cmp r4,r1
  000ad098: bcc 0x000ad086
  000ad09a: b 0x000ad180
  000ad09c: ldr r0,[0x000ad220]
  000ad09e: add r0,pc
  000ad0a0: ldr r5,[r0,#0x0]
  000ad0a2: ldr r0,[r5,#0x0]
  000ad0a4: blx 0x00071a58
  000ad0a8: movs r1,#0x2b
  000ad0aa: blx 0x0007228c
  000ad0ae: cmp r0,#0x0
  000ad0b0: beq.w 0x000acedc
  000ad0b4: ldr r0,[r5,#0x0]
  000ad0b6: blx 0x00071a58
  000ad0ba: movs r1,#0x2b
  000ad0bc: blx 0x0007228c
  000ad0c0: movs r1,#0x3b
  000ad0c2: blx 0x000718e4
  000ad0c6: mov r9,r0
  000ad0c8: ldr r0,[r5,#0x0]
  000ad0ca: blx 0x00071a58
  000ad0ce: movs r1,#0xca
  000ad0d0: mov r2,r9
  000ad0d2: blx 0x00071a70
  000ad0d6: cmp r0,#0x0
  000ad0d8: beq.w 0x000acedc
  000ad0dc: ldr r0,[0x000ad224]
  000ad0de: movs r6,#0x0
  000ad0e0: movw r1,#0x8d2
  000ad0e4: movs r2,#0x0
  000ad0e6: add r0,pc
  000ad0e8: movs r3,#0x0
  000ad0ea: ldr r4,[r0,#0x0]
  000ad0ec: movs r0,#0x1
  000ad0ee: strb.w r0,[r10,#0x399]
  000ad0f2: str r6,[sp,#0x0]
  000ad0f4: ldr r0,[r4,#0x0]
  000ad0f6: blx 0x00071548
  000ad0fa: ldr r0,[r4,#0x0]
  000ad0fc: movw r1,#0x8d1
  000ad100: movs r2,#0x0
  000ad102: movs r3,#0x0
  000ad104: str r6,[sp,#0x0]
  000ad106: blx 0x00071548
  000ad10a: add.w r11,sp,#0x4c
  000ad10e: mov r1,r10
  000ad110: strd r6,r6,[sp,#0x88]
  000ad114: mov r0,r11
  000ad116: str r6,[sp,#0x90]
  000ad118: blx 0x0007264c
  000ad11c: ldr r0,[r4,#0x0]
  000ad11e: add r3,sp,#0x88
  000ad120: mov r2,r11
  000ad122: movw r1,#0x8d2
  000ad126: str r6,[sp,#0x0]
  000ad128: ldr.w r11,[sp,#0x18]
  000ad12c: blx 0x000726e8
  000ad130: ldr r0,[r5,#0x0]
  000ad132: blx 0x00071a58
  000ad136: movs r1,#0xca
  000ad138: mov r2,r9
  000ad13a: blx 0x00072454
  000ad13e: mov r0,r11
  000ad140: movs r1,#0x2f
  000ad142: mov r2,r10
  000ad144: mov r3,r9
  000ad146: blx 0x00072418
  000ad14a: b 0x000acedc
  000ad14c: mov.w r9,#0x0
  000ad150: movs r1,#0x0
  000ad152: strb.w r9,[r0,#0x70]
  000ad156: blx 0x00072748
  000ad15a: ldr r0,[r6,#0x4]
  000ad15c: ldr.w r1,[r0,r4,lsl #0x2]
  000ad160: ldr.w r0,[r1,#0x50]!
  000ad164: cbz r0,0x000ad178
  000ad166: blx 0x00070204
  000ad16a: blx 0x0006eb48
  000ad16e: ldr r0,[r6,#0x4]
  000ad170: ldr.w r0,[r0,r4,lsl #0x2]
  000ad174: add.w r1,r0,#0x50
  000ad178: str.w r9,[r1,#0x0]
  000ad17c: ldr.w r9,[sp,#0x1c]
  000ad180: ldr.w r0,[r10,#0xc]
  000ad184: add.w r1,r11,#0x15
  000ad188: movs r2,#0x0
  000ad18a: blx 0x00072754
  000ad18e: ldr r0,[r5,#0x0]
  000ad190: movs r1,#0x1
  000ad192: ldr r0,[r0,#0x58]
  000ad194: ldr r0,[r0,#0x4]
  000ad196: strb.w r1,[r0,r11]
  000ad19a: ldr.w r11,[sp,#0x18]
  000ad19e: ldr r0,[0x000ad228]
  000ad1a0: add r0,pc
  000ad1a2: ldr r0,[r0,#0x0]
  000ad1a4: str r0,[sp,#0x24]
  000ad1a6: ldr r0,[r0,#0x0]
  000ad1a8: blx 0x00071a58
  000ad1ac: blx 0x000722ec
  000ad1b0: ldr.w r1,[r10,#0x250]
  000ad1b4: cmp r0,r1
  000ad1b6: bge 0x000ad1d6
  000ad1b8: ldr r4,[sp,#0x24]
  000ad1ba: ldr r0,[r4,#0x0]
  000ad1bc: blx 0x00071a58
  000ad1c0: blx 0x000722ec
  000ad1c4: str.w r0,[r10,#0x250]
  000ad1c8: ldr r0,[r4,#0x0]
  000ad1ca: blx 0x00071a58
  000ad1ce: blx 0x000722e0
  000ad1d2: strb.w r0,[r10,#0x398]
  000ad1d6: ldrb.w r0,[r10,#0x13c]
  000ad1da: cmp r0,#0x0
  000ad1dc: beq.w 0x000ad310
  000ad1e0: ldrb.w r0,[r10,#0x398]
  000ad1e4: cbz r0,0x000ad260
  000ad1e6: ldr r0,[sp,#0x24]
  000ad1e8: ldr r0,[r0,#0x0]
  000ad1ea: blx 0x00071a58
  000ad1ee: movs r1,#0xe
  000ad1f0: blx 0x0007228c
  000ad1f4: cbz r0,0x000ad23c
  000ad1f6: blx 0x000718d8
  000ad1fa: adr r1,[0xad22c]
  000ad1fc: cmp r0,#0xc3
  000ad1fe: it eq
  000ad200: add.eq r1,#0x4
  000ad202: vldr.32 s0,[r1]
  000ad206: b 0x000ad240
  000ad23c: vldr.32 s0,[pc,#-0xc]
  000ad240: vmov s2,r9
  000ad244: vldr.32 s4,[pc,#-0x10]
  000ad248: vcvt.f32.s32 s2,s2
  000ad24c: ldr.w r0,[r10,#0x0]
  000ad250: vmul.f32 s2,s2,s4
  000ad254: vldr.32 s4,[r0,#0x60]
  000ad258: vmla.f32 s4,s2,s0
  000ad25c: vstr.32 s4,[r0,#0x60]
  000ad260: ldr.w r0,[r10,#0xc]
  000ad264: ldr.w r0,[r0,#0xa4]
  000ad268: cbz r0,0x000ad2c8
  000ad26a: ldr r1,[0x000ad4dc]
  000ad26c: vmov.f32 s16,0x3f000000
  000ad270: vmov.f32 s18,0x3f800000
  000ad274: movw r2,#0x1234
  000ad278: add r1,pc
  000ad27a: add r6,sp,#0x88
  000ad27c: vldr.32 s20,[pc,#0x260]
  000ad280: movs r4,#0x0
  000ad282: ldr r1,[r1,#0x0]
  000ad284: adds r5,r1,r2
  000ad286: b 0x000ad2c2
  000ad288: ldr r0,[r0,#0x4]
  000ad28a: ldr.w r1,[r0,r4,lsl #0x2]
  000ad28e: mov r0,r6
  000ad290: blx 0x00072760
  000ad294: mov r0,r10
  000ad296: vldr.32 s22,[sp,#0x88]
  000ad29a: blx 0x000725d4
  000ad29e: vmov.f32 s4,s18
  000ad2a2: adds r4,#0x1
  000ad2a4: vmov s0,r0
  000ad2a8: vmul.f32 s2,s22,s20
  000ad2ac: vmla.f32 s4,s0,s16
  000ad2b0: vmul.f32 s0,s2,s4
  000ad2b4: vstr.32 s0,[r5]
  000ad2b8: adds r5,#0xa0
  000ad2ba: ldr.w r0,[r10,#0xc]
  000ad2be: ldr.w r0,[r0,#0xa4]
  000ad2c2: ldr r1,[r0,#0x0]
  000ad2c4: cmp r4,r1
  000ad2c6: bcc 0x000ad288
  000ad2c8: ldr.w r0,[r10,#0xcc]
  000ad2cc: ldr.w r1,[r10,#0x138]
  000ad2d0: cmp r1,r0
  000ad2d2: ble 0x000ad310
  000ad2d4: ldr r0,[0x000ad4e4]
  000ad2d6: add r0,pc
  000ad2d8: ldr r0,[r0,#0x0]
  000ad2da: ldr r0,[r0,#0x0]
  000ad2dc: blx 0x0007258c
  000ad2e0: ldr r1,[0x000ad4e8]
  000ad2e2: movs r2,#0x0
  000ad2e4: movs r4,#0x0
  000ad2e6: blx 0x000707f8
  000ad2ea: strb.w r4,[r10,#0x13c]
  000ad2ee: mov.w r1,#0x40000000
  000ad2f2: ldr.w r0,[r10,#0xd0]
  000ad2f6: str.w r1,[r10,#0xb8]
  000ad2fa: ldrb.w r1,[r10,#0x146]
  000ad2fe: rsbs r0,r0
  000ad300: str.w r0,[r10,#0x138]
  000ad304: cbnz r1,0x000ad310
  000ad306: movs r0,#0x2
  000ad308: strd r0,r4,[r10,#0xc8]
  000ad30c: str.w r4,[r10,#0xd0]
  000ad310: ldrb.w r0,[r10,#0x1ac]
  000ad314: cmp r0,#0x0
  000ad316: beq.w 0x000ad492
  000ad31a: ldr.w r1,[r10,#0x134]
  000ad31e: ldr.w r0,[r10,#0x208]
  000ad322: ldr r2,[sp,#0x24]
  000ad324: add r0,r1
  000ad326: str.w r0,[r10,#0x208]
  000ad32a: ldr r2,[r2,#0x0]
  000ad32c: ldrd r3,r6,[r2,#0xc0]
  000ad330: adds r3,r3,r1
  000ad332: adc.w r1,r6,r1, asr #0x1f
  000ad336: cmp.w r0,#0x7d0
  000ad33a: strd r3,r1,[r2,#0xc0]
  000ad33e: bge.w 0x000ad4ce
  000ad342: vmov s0,r0
  000ad346: vldr.32 s16,[pc,#0x1a4]
  000ad34a: vldr.32 s2,[pc,#0x1a4]
  000ad34e: vcvt.f32.s32 s0,s0
  000ad352: vdiv.f32 s0,s0,s16
  000ad356: vmul.f32 s0,s0,s2
  000ad35a: vcvt.s32.f32 s0,s0
  000ad35e: vmov r1,s0
  000ad362: rsb.w r1,r1,#0xff
  000ad366: cmp r1,#0x32
  000ad368: it le
  000ad36a: mov.le r1,#0x32
  000ad36c: cbnz r0,0x000ad374
  000ad36e: movs r0,#0x1
  000ad370: str.w r0,[r10,#0x208]
  000ad374: ldr r0,[0x000ad4f4]
  000ad376: mov r4,r10
  000ad378: orn r11,r1,#0xff
  000ad37c: ldr.w r1,[r4,#0x4]!
  000ad380: add r0,pc
  000ad382: ldr r5,[r0,#0x0]
  000ad384: ldr r1,[r1,#0x1c]
  000ad386: ldr r6,[r5,#0x0]
  000ad388: mov r0,r6
  000ad38a: blx 0x00072370
  000ad38e: vldr.32 s0,[r4,#0x204]
  000ad392: mov r1,r0
  000ad394: mov r0,r6
  000ad396: movs r3,#0x1
  000ad398: vcvt.f32.s32 s0,s0
  000ad39c: vdiv.f32 s0,s0,s16
  000ad3a0: vmov r2,s0
  000ad3a4: blx 0x0006fc58
  000ad3a8: ldrb.w r0,[r4,#0x16c]
  000ad3ac: cbz r0,0x000ad3fe
  000ad3ae: ldr.w r0,[r10,#0xdc]
  000ad3b2: ldr r6,[r5,#0x0]
  000ad3b4: ldr r1,[r0,#0x1c]
  000ad3b6: mov r0,r6
  000ad3b8: blx 0x00072370
  000ad3bc: vldr.32 s0,[r10,#0x208]
  000ad3c0: mov r1,r0
  000ad3c2: mov r0,r6
  000ad3c4: movs r3,#0x1
  000ad3c6: vcvt.f32.s32 s0,s0
  000ad3ca: vdiv.f32 s0,s0,s16
  000ad3ce: vmov r2,s0
  000ad3d2: blx 0x0006fc58
  000ad3d6: ldr.w r0,[r10,#0x28]
  000ad3da: ldr r6,[r5,#0x0]
  000ad3dc: ldr r1,[r0,#0x1c]
  000ad3de: mov r0,r6
  000ad3e0: blx 0x00072370
  000ad3e4: vldr.32 s0,[r10,#0x208]
  000ad3e8: mov r1,r0
  000ad3ea: mov r0,r6
  000ad3ec: movs r3,#0x1
  000ad3ee: vcvt.f32.s32 s0,s0
  000ad3f2: vdiv.f32 s0,s0,s16
  000ad3f6: vmov r2,s0
  000ad3fa: blx 0x0006fc58
  000ad3fe: ldr r0,[r4,#0x0]
  000ad400: add.w r6,r4,#0x16c
  000ad404: ldr r1,[r0,#0x10]
  000ad406: adds r2,r1,#0x1
  000ad408: beq 0x000ad414
  000ad40a: ldr r0,[r5,#0x0]
  000ad40c: mov r2,r11
  000ad40e: blx 0x0007276c
  000ad412: ldr r0,[r4,#0x0]
  000ad414: ldr r1,[r0,#0x14]
  000ad416: adds r0,r1,#0x1
  000ad418: ittt ne
  000ad41a: ldr.ne r0,[r5,#0x0]
  000ad41c: mov.ne r2,r11
  000ad41e: blx.ne 0x0007276c
  000ad422: ldrb r0,[r6,#0x0]
  000ad424: cbz r0,0x000ad442
  000ad426: ldr.w r0,[r10,#0xdc]
  000ad42a: mov r2,r11
  000ad42c: ldr r1,[r0,#0xc]
  000ad42e: ldr r0,[r5,#0x0]
  000ad430: blx 0x0007276c
  000ad434: ldr.w r1,[r10,#0x28]
  000ad438: mov r2,r11
  000ad43a: ldr r0,[r5,#0x0]
  000ad43c: ldr r1,[r1,#0xc]
  000ad43e: blx 0x0007276c
  000ad442: mov r0,r10
  000ad444: blx 0x00072778
  000ad448: vmov.f32 s0,0x41c80000
  000ad44c: str r6,[sp,#0xc]
  000ad44e: vmov s2,r0
  000ad452: vcmpe.f32 s2,s0
  000ad456: vmrs apsr,fpscr
  000ad45a: ble 0x000ad4f8
  000ad45c: ldrb.w r0,[r10,#0x325]
  000ad460: ldr.w r11,[sp,#0x18]
  000ad464: cmp r0,#0x0
  000ad466: bne 0x000ad4fc
  000ad468: ldr.w r0,[r10,#0x4]
  000ad46c: movs r1,#0x1
  000ad46e: strb.w r1,[r10,#0x325]
  000ad472: cmp r0,#0x0
  000ad474: beq 0x000ad4fc
  000ad476: ldr r1,[r0,#0x14]
  000ad478: adds r0,r1,#0x1
  000ad47a: beq 0x000ad4fc
  000ad47c: ldr r0,[r5,#0x0]
  000ad47e: blx 0x00072088
  000ad482: ldrb.w r1,[r10,#0x325]
  000ad486: clz r1,r1
  000ad48a: lsrs r1,r1,#0x5
  000ad48c: blx 0x0007249c
  000ad490: b 0x000ad4fc
  000ad492: ldrb.w r0,[r10,#0x1ad]
  000ad496: cmp r0,#0x0
  000ad498: beq.w 0x000ad8a6
  000ad49c: ldr.w r0,[r10,#0x134]
  000ad4a0: ldr.w r1,[r10,#0x208]
  000ad4a4: ldr.w r2,[r10,#0x214]
  000ad4a8: add r0,r1
  000ad4aa: str.w r0,[r10,#0x208]
  000ad4ae: cmp r0,r2
  000ad4b0: ble.w 0x000ad8a6
  000ad4b4: mov r0,r11
  000ad4b6: movs r1,#0x1d
  000ad4b8: mov r2,r10
  000ad4ba: movs r3,#0x0
  000ad4bc: movs r4,#0x0
  000ad4be: blx 0x00072418
  000ad4c2: mov r0,r10
  000ad4c4: blx 0x00072784
  000ad4c8: strb.w r4,[r10,#0x1ad]
  000ad4cc: b 0x000ad8a6
  000ad4ce: add.w r0,r10,#0x170
  000ad4d2: str r0,[sp,#0xc]
  000ad4d4: add.w r4,r10,#0x4
  000ad4d8: b 0x000ad4fc
  000ad4f8: ldr.w r11,[sp,#0x18]
  000ad4fc: str r4,[sp,#0x20]
  000ad4fe: ldr r0,[0x000ad53c]
  000ad500: ldr.w r1,[r10,#0x4]
  000ad504: add r0,pc
  000ad506: ldr.w r9,[r0,#0x0]
  000ad50a: ldr r1,[r1,#0x1c]
  000ad50c: ldr.w r6,[r9,#0x0]
  000ad510: mov r0,r6
  000ad512: blx 0x00072370
  000ad516: vldr.32 s0,[r10,#0x208]
  000ad51a: mov r1,r0
  000ad51c: vldr.32 s16,[pc,#-0x2e8]
  000ad520: mov r0,r6
  000ad522: vcvt.f32.s32 s0,s0
  000ad526: movs r3,#0x2
  000ad528: vmul.f32 s0,s0,s16
  000ad52c: vmov r2,s0
  000ad530: blx 0x0006fc58
  000ad534: ldrb.w r0,[r10,#0x170]
  000ad538: cbz r0,0x000ad594
  000ad53a: b 0x000ad540
  000ad540: ldr.w r0,[r10,#0xdc]
  000ad544: ldr.w r6,[r9,#0x0]
  000ad548: ldr r1,[r0,#0x1c]
  000ad54a: mov r0,r6
  000ad54c: blx 0x00072370
  000ad550: vldr.32 s0,[r10,#0x208]
  000ad554: mov r1,r0
  000ad556: mov r0,r6
  000ad558: movs r3,#0x2
  000ad55a: vcvt.f32.s32 s0,s0
  000ad55e: vmul.f32 s0,s0,s16
  000ad562: vmov r2,s0
  000ad566: blx 0x0006fc58
  000ad56a: ldr.w r0,[r10,#0x28]
  000ad56e: ldr.w r6,[r9,#0x0]
  000ad572: ldr r1,[r0,#0x1c]
  000ad574: mov r0,r6
  000ad576: blx 0x00072370
  000ad57a: vldr.32 s0,[r10,#0x208]
  000ad57e: mov r1,r0
  000ad580: mov r0,r6
  000ad582: movs r3,#0x2
  000ad584: vcvt.f32.s32 s0,s0
  000ad588: vmul.f32 s0,s0,s16
  000ad58c: vmov r2,s0
  000ad590: blx 0x0006fc58
  000ad594: ldr.w r0,[r10,#0x210]
  000ad598: ldr.w r4,[r10,#0x208]
  000ad59c: sub.w r5,r0,#0x7d0
  000ad5a0: cmp r4,r5
  000ad5a2: ble.w 0x000ad744
  000ad5a6: ldr.w r0,[r10,#0x4]
  000ad5aa: ldr.w r6,[r9,#0x0]
  000ad5ae: ldr r1,[r0,#0x1c]
  000ad5b0: mov r0,r6
  000ad5b2: blx 0x00072370
  000ad5b6: mov r1,r0
  000ad5b8: ldr.w r0,[r10,#0x210]
  000ad5bc: vldr.32 s0,[r10,#0x208]
  000ad5c0: vmov.f32 s18,0x3f800000
  000ad5c4: sub.w r0,r0,#0x7d0
  000ad5c8: vldr.32 s16,[pc,#-0xe0]
  000ad5cc: vcvt.f32.s32 s0,s0
  000ad5d0: movs r3,#0x1
  000ad5d2: vmov s2,r0
  000ad5d6: mov r0,r6
  000ad5d8: vcvt.f32.s32 s2,s2
  000ad5dc: vsub.f32 s0,s0,s2
  000ad5e0: vdiv.f32 s0,s0,s16
  000ad5e4: vsub.f32 s0,s18,s0
  000ad5e8: vmov r2,s0
  000ad5ec: blx 0x0006fc58
  000ad5f0: vmov s0,r5
  000ad5f4: vmov s2,r4
  000ad5f8: vcvt.f32.s32 s0,s0
  000ad5fc: vcvt.f32.s32 s2,s2
  000ad600: vsub.f32 s0,s2,s0
  000ad604: vldr.32 s2,[pc,#-0x118]
  000ad608: vdiv.f32 s0,s0,s16
  000ad60c: vmul.f32 s0,s0,s2
  000ad610: vcvt.s32.f32 s0,s0
  000ad614: vmov r0,s0
  000ad618: cmp r0,#0x32
  000ad61a: it le
  000ad61c: mov.le r0,#0x32
  000ad61e: ldrb.w r1,[r10,#0x170]
  000ad622: orn r11,r0,#0xff
  000ad626: cmp r1,#0x0
  000ad628: beq 0x000ad6ae
  000ad62a: ldr.w r0,[r10,#0xdc]
  000ad62e: ldr.w r6,[r9,#0x0]
  000ad632: ldr r1,[r0,#0x1c]
  000ad634: mov r0,r6
  000ad636: blx 0x00072370
  000ad63a: mov r1,r0
  000ad63c: ldr.w r0,[r10,#0x210]
  000ad640: vldr.32 s0,[r10,#0x208]
  000ad644: movs r3,#0x1
  000ad646: sub.w r0,r0,#0x7d0
  000ad64a: vcvt.f32.s32 s0,s0
  000ad64e: vmov s2,r0
  000ad652: mov r0,r6
  000ad654: vcvt.f32.s32 s2,s2
  000ad658: vsub.f32 s0,s0,s2
  000ad65c: vdiv.f32 s0,s0,s16
  000ad660: vsub.f32 s0,s18,s0
  000ad664: vmov r2,s0
  000ad668: blx 0x0006fc58
  000ad66c: ldr.w r0,[r10,#0x28]
  000ad670: ldr.w r6,[r9,#0x0]
  000ad674: ldr r1,[r0,#0x1c]
  000ad676: mov r0,r6
  000ad678: blx 0x00072370
  000ad67c: mov r1,r0
  000ad67e: ldr.w r0,[r10,#0x210]
  000ad682: vldr.32 s0,[r10,#0x208]
  000ad686: movs r3,#0x1
  000ad688: sub.w r0,r0,#0x7d0
  000ad68c: vcvt.f32.s32 s0,s0
  000ad690: vmov s2,r0
  000ad694: mov r0,r6
  000ad696: vcvt.f32.s32 s2,s2
  000ad69a: vsub.f32 s0,s0,s2
  000ad69e: vdiv.f32 s0,s0,s16
  000ad6a2: vsub.f32 s0,s18,s0
  000ad6a6: vmov r2,s0
  000ad6aa: blx 0x0006fc58
  000ad6ae: ldr r4,[sp,#0x20]
  000ad6b0: ldr r0,[r4,#0x0]
  000ad6b2: ldr r1,[r0,#0x10]
  000ad6b4: adds r2,r1,#0x1
  000ad6b6: beq 0x000ad6c4
  000ad6b8: ldr.w r0,[r9,#0x0]
  000ad6bc: mov r2,r11
  000ad6be: blx 0x0007276c
  000ad6c2: ldr r0,[r4,#0x0]
  000ad6c4: ldr r1,[r0,#0x14]
  000ad6c6: ldr r4,[sp,#0xc]
  000ad6c8: adds r0,r1,#0x1
  000ad6ca: ittt ne
  000ad6cc: ldr.ne.w r0,[r9,#0x0]
  000ad6d0: mov.ne r2,r11
  000ad6d2: blx.ne 0x0007276c
  000ad6d6: ldrb r0,[r4,#0x0]
  000ad6d8: cbz r0,0x000ad6fa
  000ad6da: ldr.w r0,[r10,#0xdc]
  000ad6de: mov r2,r11
  000ad6e0: ldr r1,[r0,#0xc]
  000ad6e2: ldr.w r0,[r9,#0x0]
  000ad6e6: blx 0x0007276c
  000ad6ea: ldr.w r1,[r10,#0x28]
  000ad6ee: mov r2,r11
  000ad6f0: ldr.w r0,[r9,#0x0]
  000ad6f4: ldr r1,[r1,#0xc]
  000ad6f6: blx 0x0007276c
  000ad6fa: mov r0,r10
  000ad6fc: blx 0x00072778
  000ad700: vmov.f32 s0,0x41c80000
  000ad704: ldr.w r11,[sp,#0x18]
  000ad708: vmov s2,r0
  000ad70c: vcmpe.f32 s2,s0
  000ad710: vmrs apsr,fpscr
  000ad714: bpl 0x000ad744
  000ad716: ldrb.w r0,[r10,#0x325]
  000ad71a: cbz r0,0x000ad744
  000ad71c: ldr.w r0,[r10,#0x4]
  000ad720: movs r1,#0x0
  000ad722: strb.w r1,[r10,#0x325]
  000ad726: cbz r0,0x000ad744
  000ad728: ldr r1,[r0,#0x14]
  000ad72a: adds r0,r1,#0x1
  000ad72c: beq 0x000ad744
  000ad72e: ldr.w r0,[r9,#0x0]
  000ad732: blx 0x00072088
  000ad736: ldrb.w r1,[r10,#0x325]
  000ad73a: clz r1,r1
  000ad73e: lsrs r1,r1,#0x5
  000ad740: blx 0x0007249c
  000ad744: ldr.w r0,[r10,#0x208]
  000ad748: ldr.w r1,[r10,#0x210]
  000ad74c: cmp r0,r1
  000ad74e: ble.w 0x000ad852
  000ad752: ldr r0,[0x000ada78]
  000ad754: movs r4,#0x0
  000ad756: ldr.w r1,[r10,#0x0]
  000ad75a: movs r3,#0x0
  000ad75c: add r0,pc
  000ad75e: ldr.w r2,[r10,#0x368]
  000ad762: strb.w r4,[r10,#0x1ac]
  000ad766: ldr r0,[r0,#0x0]
  000ad768: strd r4,r2,[r10,#0x208]
  000ad76c: movs r2,#0x0
  000ad76e: strb.w r4,[r1,#0x5e]
  000ad772: movs r1,#0x1e
  000ad774: ldr r0,[r0,#0x0]
  000ad776: str r4,[sp,#0x0]
  000ad778: blx 0x00071548
  000ad77c: ldrb.w r0,[r10,#0x325]
  000ad780: cbz r0,0x000ad7a8
  000ad782: ldr.w r0,[r10,#0x4]
  000ad786: strb.w r4,[r10,#0x325]
  000ad78a: cbz r0,0x000ad7a8
  000ad78c: ldr r1,[r0,#0x14]
  000ad78e: adds r0,r1,#0x1
  000ad790: beq 0x000ad7a8
  000ad792: ldr.w r0,[r9,#0x0]
  000ad796: blx 0x00072088
  000ad79a: ldrb.w r1,[r10,#0x325]
  000ad79e: clz r1,r1
  000ad7a2: lsrs r1,r1,#0x5
  000ad7a4: blx 0x0007249c
  000ad7a8: ldr.w r1,[r10,#0x388]
  000ad7ac: ldr.w r0,[r9,#0x0]
  000ad7b0: ldr.w r4,[r10,#0x394]
  000ad7b4: blx 0x00072430
  000ad7b8: str r4,[r0,#0x20]
  000ad7ba: ldrb.w r0,[r10,#0x170]
  000ad7be: cmp r0,#0x0
  000ad7c0: beq 0x000ad852
  000ad7c2: ldr.w r1,[r10,#0x38c]
  000ad7c6: ldr.w r0,[r9,#0x0]
  000ad7ca: ldr.w r4,[r10,#0x394]
  000ad7ce: blx 0x00072430
  000ad7d2: str r4,[r0,#0x20]
  000ad7d4: ldrd r1,r4,[r10,#0x390]
  000ad7d8: ldr.w r0,[r9,#0x0]
  000ad7dc: blx 0x00072430
  000ad7e0: str r4,[r0,#0x20]
  000ad7e2: ldrb.w r0,[r10,#0x170]
  000ad7e6: cbz r0,0x000ad852
  000ad7e8: ldr r0,[sp,#0x24]
  000ad7ea: ldr r0,[r0,#0x0]
  000ad7ec: blx 0x00071a58
  000ad7f0: movs r1,#0x8
  000ad7f2: blx 0x0007228c
  000ad7f6: movw r6,#0x4e86
  000ad7fa: cbz r0,0x000ad808
  000ad7fc: blx 0x000718d8
  000ad800: cmp r0,#0xe0
  000ad802: it eq
  000ad804: movw.eq r6,#0x5e15
  000ad808: ldr r0,[sp,#0x24]
  000ad80a: ldr r0,[r0,#0x0]
  000ad80c: blx 0x00071a58
  000ad810: movs r1,#0x23
  000ad812: blx 0x0007228c
  000ad816: cbz r0,0x000ad830
  000ad818: blx 0x000718d8
  000ad81c: movw r6,#0x716b
  000ad820: cmp r0,#0xc7
  000ad822: it eq
  000ad824: movw.eq r6,#0x7165
  000ad828: cmp r0,#0xc6
  000ad82a: it eq
  000ad82c: movw.eq r6,#0x715f
  000ad830: ldr.w r0,[r10,#0xdc]
  000ad834: uxth r6,r6
  000ad836: mov r2,r6
  000ad838: ldr r1,[r0,#0x1c]
  000ad83a: ldr.w r0,[r9,#0x0]
  000ad83e: blx 0x00072448
  000ad842: ldr.w r1,[r10,#0x28]
  000ad846: mov r2,r6
  000ad848: ldr.w r0,[r9,#0x0]
  000ad84c: ldr r1,[r1,#0x1c]
  000ad84e: blx 0x00072448
  000ad852: mov r0,r10
  000ad854: ldr.w r6,[r10,#0xc]
  000ad858: blx 0x00072778
  000ad85c: vldr.32 s0,[pc,#0x21c]
  000ad860: vmov s2,r0
  000ad864: vldr.32 s6,[pc,#0x218]
  000ad868: vmul.f32 s4,s2,s0
  000ad86c: vldr.32 s0,[pc,#0x214]
  000ad870: ldr.w r9,[sp,#0x1c]
  000ad874: vmov.f32 s2,s0
  000ad878: vmla.f32 s2,s4,s6
  000ad87c: vcmpe.f32 s2,s0
  000ad880: vmrs apsr,fpscr
  000ad884: bpl 0x000ad898
  000ad886: vcmpe.f32 s2,#0
  000ad88a: vmrs apsr,fpscr
  000ad88e: ite pl
  000ad890: vmov.pl.f32 s0,s2
  000ad894: vldr.mi.32 s0,[pc,#0x1f0]
  000ad898: vcvt.s32.f32 s0,s0
  000ad89c: mov r0,r6
  000ad89e: vmov r1,s0
  000ad8a2: blx 0x00072790
  000ad8a6: ldrb.w r0,[r10,#0x204]
  000ad8aa: cbz r0,0x000ad8fa
  000ad8ac: ldr.w r0,[r10,#0x134]
  000ad8b0: ldrd r1,r2,[r10,#0x1fc]
  000ad8b4: add r0,r1
  000ad8b6: str.w r0,[r10,#0x1fc]
  000ad8ba: cmp r0,r2
  000ad8bc: blt 0x000ad8d0
  000ad8be: movs r0,#0x0
  000ad8c0: movs r1,#0x1a
  000ad8c2: strb.w r0,[r10,#0x38]
  000ad8c6: mov r0,r11
  000ad8c8: mov r2,r10
  000ad8ca: movs r3,#0x0
  000ad8cc: blx 0x00072418
  000ad8d0: ldrb.w r0,[r10,#0x398]
  000ad8d4: cbz r0,0x000ad8fa
  000ad8d6: vmov s0,r9
  000ad8da: vldr.32 s2,[pc,#0x1b0]
  000ad8de: vmov.f32 s4,0x3f000000
  000ad8e2: vcvt.f32.s32 s0,s0
  000ad8e6: ldr.w r0,[r10,#0x0]
  000ad8ea: vmul.f32 s0,s0,s2
  000ad8ee: vldr.32 s2,[r0,#0x60]
  000ad8f2: vmla.f32 s2,s0,s4
  000ad8f6: vstr.32 s2,[r0,#0x60]
  000ad8fa: ldr.w r0,[r10,#0x1b4]
  000ad8fe: cbz r0,0x000ad912
  000ad900: ldr.w r3,[r10,#0xc]
  000ad904: mov r2,r8
  000ad906: ldr.w r1,[r10,#0x134]
  000ad90a: str.w r11,[sp,#0x0]
  000ad90e: blx 0x0007279c
  000ad912: ldr.w r0,[r10,#0x1b8]
  000ad916: cbz r0,0x000ad940
  000ad918: movs r4,#0x0
  000ad91a: b 0x000ad93a
  000ad91c: ldr r0,[r0,#0x4]
  000ad91e: mov r2,r8
  000ad920: ldr.w r3,[r10,#0xc]
  000ad924: ldr.w r1,[r10,#0x134]
  000ad928: ldr.w r0,[r0,r4,lsl #0x2]
  000ad92c: str.w r11,[sp,#0x0]
  000ad930: blx 0x000727a8
  000ad934: ldr.w r0,[r10,#0x1b8]
  000ad938: adds r4,#0x1
  000ad93a: ldr r1,[r0,#0x0]
  000ad93c: cmp r4,r1
  000ad93e: bcc 0x000ad91c
  000ad940: ldrb.w r0,[r10,#0x370]
  000ad944: cbz r0,0x000ad95c
  000ad946: ldr.w r0,[r10,#0x374]
  000ad94a: add r0,r9
  000ad94c: str.w r0,[r10,#0x374]
  000ad950: cmp.w r0,#0x7d0
  000ad954: itt ge
  000ad956: mov.ge r0,#0x0
  000ad958: strb.ge.w r0,[r10,#0x370]
  000ad95c: ldrb.w r0,[r10,#0x170]
  000ad960: cbz r0,0x000ad96c
  000ad962: ldrb.w r0,[r10,#0x180]
  000ad966: cmp r0,#0x0
  000ad968: beq.w 0x000adc8c
  000ad96c: ldr.w r0,[r10,#0x34]
  000ad970: cmp r0,#0x0
  000ad972: beq 0x000ada34
  000ad974: ldrb.w r1,[r10,#0x38]
  000ad978: cmp r1,#0x0
  000ad97a: beq 0x000ada34
  000ad97c: ldr.w r1,[r10,#0x3c]
  000ad980: mov r2,r1
  000ad982: mov r3,r1
  000ad984: blx 0x000727b4
  000ad988: ldr.w r0,[r10,#0xc]
  000ad98c: ldr.w r11,[r10,#0x34]
  000ad990: blx 0x000727c0
  000ad994: add r5,sp,#0x4c
  000ad996: mov r1,r0
  000ad998: mov r0,r5
  000ad99a: blx 0x000727cc
  000ad99e: add r6,sp,#0x88
  000ad9a0: mov r1,r5
  000ad9a2: mov r0,r6
  000ad9a4: blx 0x0006ec80
  000ad9a8: mov.w r0,#0x3f800000
  000ad9ac: add r2,sp,#0xc8
  000ad9ae: str r0,[sp,#0xcc]
  000ad9b0: movs r0,#0x0
  000ad9b2: str r0,[sp,#0xc8]
  000ad9b4: mov r1,r6
  000ad9b6: str r0,[sp,#0xd0]
  000ad9b8: mov r0,r11
  000ad9ba: blx 0x000726ac
  000ad9be: add r6,sp,#0x88
  000ad9c0: ldr.w r1,[r10,#0x8]
  000ad9c4: ldr.w r5,[r10,#0x34]
  000ad9c8: mov r0,r6
  000ad9ca: blx 0x000720b8
  000ad9ce: mov r0,r5
  000ad9d0: mov r1,r6
  000ad9d2: blx 0x00072148
  000ad9d6: ldr r0,[sp,#0x24]
  000ad9d8: ldr r0,[r0,#0x0]
  000ad9da: blx 0x00071a58
  000ad9de: blx 0x000719c8
  000ad9e2: cmp r0,#0x8
  000ad9e4: bne 0x000ada14
  000ad9e6: add r6,sp,#0xc8
  000ad9e8: ldr.w r1,[r10,#0x8]
  000ad9ec: ldr.w r11,[r10,#0x34]
  000ad9f0: mov r0,r6
  000ad9f2: blx 0x00072574
  000ad9f6: add r5,sp,#0x4c
  000ad9f8: mov r1,r6
  000ad9fa: mov r0,r5
  000ad9fc: blx 0x0006ec80
  000ada00: add r6,sp,#0x88
  000ada02: ldr r2,[0x000ada90]
  000ada04: mov r1,r5
  000ada06: mov r0,r6
  000ada08: blx 0x0006ec74
  000ada0c: mov r0,r11
  000ada0e: mov r1,r6
  000ada10: blx 0x00072460
  000ada14: ldr r0,[0x000ada94]
  000ada16: ldr.w r1,[r10,#0x34]
  000ada1a: add r0,pc
  000ada1c: ldr r0,[r0,#0x0]
  000ada1e: ldr r1,[r1,#0xc]
  000ada20: ldr r0,[r0,#0x0]
  000ada22: blx 0x00072088
  000ada26: ldr.w r2,[r10,#0x134]
  000ada2a: movs r1,#0x1
  000ada2c: str r1,[sp,#0x0]
  000ada2e: asrs r3,r2,#0x1f
  000ada30: blx 0x0006f7cc
  000ada34: ldr r0,[0x000ada98]
  000ada36: ldr.w r1,[r10,#0x4]
  000ada3a: add r0,pc
  000ada3c: ldr r0,[r0,#0x0]
  000ada3e: str r0,[sp,#0x20]
  000ada40: ldr r1,[r1,#0xc]
  000ada42: ldr r0,[r0,#0x0]
  000ada44: blx 0x00072088
  000ada48: asr.w r11,r9, asr #0x1f
  000ada4c: movs r4,#0x1
  000ada4e: mov r2,r9
  000ada50: mov r3,r11
  000ada52: str r4,[sp,#0x0]
  000ada54: blx 0x0006f7cc
  000ada58: ldrb.w r1,[r10,#0x1f4]
  000ada5c: movs r2,#0x0
  000ada5e: ldrb.w r0,[r10,#0x1ee]
  000ada62: cmp r1,#0x0
  000ada64: strb.w r2,[r10,#0x330]
  000ada68: strb.w r2,[r10,#0x145]
  000ada6c: beq 0x000ada9c
  000ada6e: cmp r0,#0x0
  000ada70: strb.w r4,[r10,#0x145]
  000ada74: bne 0x000adaa8
  000ada76: b 0x000adab6
  000ada9c: cmp r0,#0x0
  000ada9e: beq.w 0x000add48
  000adaa2: movs r0,#0x1
  000adaa4: strb.w r0,[r10,#0x145]
  000adaa8: ldr.w r0,[r10,#0x134]
  000adaac: ldr.w r1,[r10,#0x1f0]
  000adab0: add r0,r1
  000adab2: str.w r0,[r10,#0x1f0]
  000adab6: vmov s0,r9
  000adaba: vcvt.f32.s32 s0,s0
  000adabe: vldr.32 s2,[r10,#0xb8]
  000adac2: ldr.w r0,[r10,#0x8]
  000adac6: vmul.f32 s0,s2,s0
  000adaca: vmov r1,s0
  000adace: blx 0x000725c8
  000adad2: ldr.w r1,[r10,#0x8]
  000adad6: ldr r4,[sp,#0x20]
  000adad8: ldr r1,[r1,#0xc]
  000adada: ldr r0,[r4,#0x0]
  000adadc: blx 0x000720c4
  000adae0: ldr.w r1,[r10,#0x4]
  000adae4: mov r5,r0
  000adae6: ldr r0,[r4,#0x0]
  000adae8: ldr r1,[r1,#0xc]
  000adaea: blx 0x000720c4
  000adaee: add r6,sp,#0x88
  000adaf0: mov r2,r0
  000adaf2: mov r1,r5
  000adaf4: mov r0,r6
  000adaf6: blx 0x0006f82c
  000adafa: ldr.w r0,[r10,#0x0]
  000adafe: mov r1,r6
  000adb00: adds r0,#0x4
  000adb02: blx 0x0006f148
  000adb06: ldrb.w r0,[r10,#0x144]
  000adb0a: cbz r0,0x000adb60
  000adb0c: ldr.w r0,[r10,#0x0]
  000adb10: blx 0x00071f14
  000adb14: cbz r0,0x000adb60
  000adb16: ldr.w r0,[r10,#0xc]
  000adb1a: movs r1,#0x0
  000adb1c: strb.w r1,[r10,#0x234]
  000adb20: blx 0x000725b0
  000adb24: mov r1,r0
  000adb26: ldr r0,[0x000adec4]
  000adb28: add r0,pc
  000adb2a: ldr r4,[r0,#0x0]
  000adb2c: mov r0,r10
  000adb2e: blx r4
  000adb30: ldr.w r0,[r10,#0xc]
  000adb34: blx 0x00071eb4
  000adb38: mov r1,r0
  000adb3a: mov r0,r10
  000adb3c: blx r4
  000adb3e: ldr.w r0,[r10,#0xc]
  000adb42: blx 0x00071f08
  000adb46: mov r1,r0
  000adb48: mov r0,r10
  000adb4a: blx r4
  000adb4c: ldrb.w r0,[r10,#0x234]
  000adb50: cbz r0,0x000adb60
  000adb52: ldrb.w r0,[r10,#0x1a0]
  000adb56: cbz r0,0x000adb60
  000adb58: mov r0,r10
  000adb5a: mov r1,r9
  000adb5c: blx 0x000727d8
  000adb60: ldrb.w r0,[r10,#0x398]
  000adb64: cmp r0,#0x0
  000adb66: beq.w 0x000adc82
  000adb6a: ldr r0,[0x000adec8]
  000adb6c: movs r1,#0x23
  000adb6e: add r0,pc
  000adb70: ldr r5,[r0,#0x0]
  000adb72: ldr r0,[r5,#0x0]
  000adb74: blx 0x0007153c
  000adb78: cbnz r0,0x000adb8a
  000adb7a: ldr r0,[r5,#0x0]
  000adb7c: movs r1,#0x0
  000adb7e: str r1,[sp,#0x0]
  000adb80: movs r1,#0x23
  000adb82: movs r2,#0x0
  000adb84: movs r3,#0x0
  000adb86: blx 0x00071548
  000adb8a: vldr.32 s4,[r10,#0x270]
  000adb8e: vmov s8,r9
  000adb92: vldr.32 s6,[r10,#0x274]
  000adb96: vmov.f32 s1,0x3f000000
  000adb9a: vldr.32 s0,[r10,#0x268]
  000adb9e: vsub.f32 s6,s4,s6
  000adba2: vldr.32 s2,[r10,#0x26c]
  000adba6: vcvt.f32.s32 s8,s8
  000adbaa: vldr.32 s14,[pc,#-0x120]
  000adbae: vsub.f32 s2,s0,s2
  000adbb2: vcmpe.f32 s6,#0
  000adbb6: vmrs apsr,fpscr
  000adbba: vneg.f32 s10,s6
  000adbbe: vcmpe.f32 s2,#0
  000adbc2: vneg.f32 s12,s2
  000adbc6: vmul.f32 s8,s8,s14
  000adbca: it gt
  000adbcc: vmov.gt.f32 s10,s6
  000adbd0: vmrs apsr,fpscr
  000adbd4: it gt
  000adbd6: vmov.gt.f32 s12,s2
  000adbda: vmul.f32 s2,s8,s1
  000adbde: vcmpe.f32 s10,s12
  000adbe2: vmrs apsr,fpscr
  000adbe6: it gt
  000adbe8: vmov.gt.f32 s12,s10
  000adbec: ldr.w r0,[r10,#0x0]
  000adbf0: vldr.32 s6,[r0,#0x60]
  000adbf4: vmla.f32 s6,s2,s12
  000adbf8: vmov.f32 s2,0x3f800000
  000adbfc: vstr.32 s6,[r0,#0x60]
  000adc00: vcmpe.f32 s6,s2
  000adc04: vstr.32 s0,[r10,#0x26c]
  000adc08: vmrs apsr,fpscr
  000adc0c: vstr.32 s4,[r10,#0x274]
  000adc10: blt 0x000adc66
  000adc12: ldrb.w r1,[r10,#0x1ee]
  000adc16: cbnz r1,0x000adc66
  000adc18: ldrb.w r1,[r10,#0x1ed]
  000adc1c: cbnz r1,0x000adc66
  000adc1e: movs r1,#0x0
  000adc20: blx 0x000727e4
  000adc24: ldr.w r0,[r10,#0x90]
  000adc28: cbnz r0,0x000adc66
  000adc2a: movs r0,#0x68
  000adc2c: blx 0x0006eb24
  000adc30: mov r6,r0
  000adc32: movs r1,#0xc
  000adc34: blx 0x000727f0
  000adc38: ldr r1,[0x000adecc]
  000adc3a: mov r0,r6
  000adc3c: str.w r6,[r10,#0x90]
  000adc40: blx 0x000727fc
  000adc44: add r4,sp,#0x88
  000adc46: ldr.w r1,[r10,#0x8]
  000adc4a: ldr.w r6,[r10,#0x90]
  000adc4e: mov r0,r4
  000adc50: blx 0x000720b8
  000adc54: movs r0,#0x0
  000adc56: add r2,sp,#0x4c
  000adc58: strd r0,r0,[sp,#0x4c]
  000adc5c: mov r1,r4
  000adc5e: str r0,[sp,#0x54]
  000adc60: mov r0,r6
  000adc62: blx 0x00072808
  000adc66: ldr.w r0,[r10,#0x0]
  000adc6a: blx 0x000724f0
  000adc6e: mov r1,r0
  000adc70: ldr r0,[r5,#0x0]
  000adc72: cmp r1,#0x1
  000adc74: blt 0x000adcec
  000adc76: ldr.w r1,[r10,#0x0]
  000adc7a: movs r2,#0x23
  000adc7c: ldr r3,[r1,#0x60]
  000adc7e: movs r1,#0x0
  000adc80: b 0x000adcf2
  000adc82: ldr.w r0,[r10,#0x0]
  000adc86: movs r1,#0x0
  000adc88: str r1,[r0,#0x60]
  000adc8a: b 0x000adcf6
  000adc8c: ldrb.w r0,[r10,#0x1a0]
  000adc90: cmp r0,#0x0
  000adc92: beq.w 0x000ad96c
  000adc96: ldr r0,[sp,#0x24]
  000adc98: ldr r0,[r0,#0x0]
  000adc9a: blx 0x00071a58
  000adc9e: movs r1,#0x23
  000adca0: blx 0x0007228c
  000adca4: cmp r0,#0x0
  000adca6: beq.w 0x000ad96c
  000adcaa: ldr r0,[0x000aded0]
  000adcac: ldr.w r1,[r10,#0x28]
  000adcb0: add r0,pc
  000adcb2: ldr r4,[r0,#0x0]
  000adcb4: ldr r1,[r1,#0xc]
  000adcb6: ldr r0,[r4,#0x0]
  000adcb8: blx 0x00072088
  000adcbc: asr.w r6,r9, asr #0x1f
  000adcc0: movs r1,#0x1
  000adcc2: mov r2,r9
  000adcc4: mov r3,r6
  000adcc6: str r1,[sp,#0x0]
  000adcc8: blx 0x0006f7cc
  000adccc: ldr.w r0,[r10,#0x30]
  000adcd0: cmp r0,#0x0
  000adcd2: beq.w 0x000ad96c
  000adcd6: ldr r1,[r0,#0xc]
  000adcd8: ldr r0,[r4,#0x0]
  000adcda: blx 0x00072088
  000adcde: movs r1,#0x0
  000adce0: mov r2,r9
  000adce2: mov r3,r6
  000adce4: str r1,[sp,#0x0]
  000adce6: blx 0x0006f7cc
  000adcea: b 0x000ad96c
  000adcec: movs r1,#0x0
  000adcee: movs r2,#0x23
  000adcf0: movs r3,#0x0
  000adcf2: blx 0x0007159c
  000adcf6: ldr.w r0,[r10,#0x0]
  000adcfa: blx 0x00072814
  000adcfe: cmp r0,#0x0
  000add00: bgt 0x000add18
  000add02: ldrb.w r0,[r10,#0x1ee]
  000add06: cbnz r0,0x000add18
  000add08: ldrb.w r0,[r10,#0x1ec]
  000add0c: cbnz r0,0x000add18
  000add0e: ldr.w r0,[r10,#0x0]
  000add12: movs r1,#0x0
  000add14: blx 0x000727e4
  000add18: ldrb.w r0,[r10,#0x1c0]
  000add1c: cbz r0,0x000add62
  000add1e: ldr.w r0,[r10,#0x1bc]
  000add22: movs r1,#0x1
  000add24: strb.w r1,[r10,#0x145]
  000add28: cbz r0,0x000add3a
  000add2a: blx 0x00071f50
  000add2e: cbnz r0,0x000add3a
  000add30: ldr.w r0,[r10,#0x1bc]
  000add34: blx 0x00071ec0
  000add38: cbz r0,0x000add5a
  000add3a: mov r0,r10
  000add3c: movs r1,#0x0
  000add3e: mov r2,r8
  000add40: blx 0x00072820
  000add44: b.w 0x000aef92
  000add48: ldrb.w r0,[r10,#0x1ed]
  000add4c: cmp r0,#0x0
  000add4e: beq.w 0x000adb06
  000add52: movs r0,#0x1
  000add54: strb.w r0,[r10,#0x145]
  000add58: b 0x000adab6
  000add5a: ldr r1,[sp,#0x18]
  000add5c: mov r0,r10
  000add5e: blx 0x0007282c
  000add62: ldrb.w r0,[r10,#0x356]
  000add66: cmp r0,#0x0
  000add68: beq.w 0x000ae062
  000add6c: ldr.w r0,[r10,#0x0]
  000add70: blx 0x000724f0
  000add74: cmp r0,#0x1
  000add76: blt.w 0x000ae062
  000add7a: ldr.w r0,[r10,#0x1bc]
  000add7e: movs r1,#0x1
  000add80: strb.w r1,[r10,#0x145]
  000add84: cbz r0,0x000add96
  000add86: blx 0x00071f50
  000add8a: cbnz r0,0x000add96
  000add8c: ldr.w r0,[r10,#0x1bc]
  000add90: blx 0x00071ec0
  000add94: cbz r0,0x000adda2
  000add96: mov r0,r10
  000add98: movs r1,#0x0
  000add9a: blx 0x00072838
  000add9e: b.w 0x000aef92
  000adda2: ldr r1,[sp,#0x18]
  000adda4: mov r0,r10
  000adda6: mov r3,r8
  000adda8: blx 0x00072844
  000addac: cmp r0,#0x0
  000addae: beq.w 0x000ae04e
  000addb2: ldr r4,[sp,#0x24]
  000addb4: ldr r0,[r4,#0x0]
  000addb6: blx 0x00072850
  000addba: mov r5,r0
  000addbc: ldr r0,[r4,#0x0]
  000addbe: blx 0x0007285c
  000addc2: mov r4,r0
  000addc4: mov r0,r5
  000addc6: blx 0x00072868
  000addca: cmp r0,#0x0
  000addcc: bne.w 0x000adf08
  000addd0: mov r0,r5
  000addd2: blx 0x00072874
  000addd6: cmp r0,#0xf
  000addd8: beq 0x000adde6
  000addda: mov r0,r5
  000adddc: blx 0x00072874
  000adde0: cmp r0,#0xb8
  000adde2: bne.w 0x000adf08
  000adde6: ldr.w r0,[r10,#0x1bc]
  000addea: blx 0x00072880
  000addee: cmp r0,#0x1
  000addf0: bne.w 0x000adf08
  000addf4: ldrd r0,r6,[r10,#0x360]
  000addf8: cmp r6,r0
  000addfa: beq.w 0x000ae040
  000addfe: mov r0,r5
  000ade00: blx 0x00072874
  000ade04: vldr.32 s0,[r10,#0x35c]
  000ade08: adr r1,[0xaded4]
  000ade0a: cmp r0,#0xb8
  000ade0c: vcvt.f32.s32 s0,s0
  000ade10: it eq
  000ade12: add.eq r1,#0x4
  000ade14: vldr.32 s2,[r1]
  000ade18: vdiv.f32 s0,s0,s2
  000ade1c: vcvt.s32.f32 s2,s0
  000ade20: ldr.w r1,[r10,#0x360]
  000ade24: vcvt.s32.f32 s0,s0
  000ade28: vstr.32 s2,[r10,#0x364]
  000ade2c: vmov r0,s0
  000ade30: cmp r1,r0
  000ade32: bgt 0x000ade58
  000ade34: cmp r6,r0
  000ade36: bge 0x000ade50
  000ade38: mov r0,r5
  000ade3a: blx 0x00072874
  000ade3e: cmp r0,#0xb8
  000ade40: ldr r0,[sp,#0x18]
  000ade42: ite ne
  000ade44: mov.ne r1,#0x2a
  000ade46: mov.eq r1,#0x26
  000ade48: mov r2,r10
  000ade4a: movs r3,#0x0
  000ade4c: blx 0x00072418
  000ade50: ldr.w r0,[r10,#0x360]
  000ade54: str.w r0,[r10,#0x364]
  000ade58: cmp r0,r6
  000ade5a: ble.w 0x000ae040
  000ade5e: mov r0,r5
  000ade60: blx 0x00072874
  000ade64: cmp r0,#0xb8
  000ade66: bne.w 0x000af01a
  000ade6a: ldr r0,[sp,#0x24]
  000ade6c: ldr r0,[r0,#0x0]
  000ade6e: ldr.w r1,[r0,#0x178]
  000ade72: cmp r1,#0x1
  000ade74: blt.w 0x000ae040
  000ade78: ldr.w r2,[r10,#0x364]
  000ade7c: add r1,r6
  000ade7e: subs r1,r1,r2
  000ade80: str.w r1,[r0,#0x178]
  000ade84: mov r0,r5
  000ade86: blx 0x0007288c
  000ade8a: ldr.w r1,[r10,#0x364]
  000ade8e: add r0,r6
  000ade90: subs r1,r0,r1
  000ade92: mov r0,r5
  000ade94: blx 0x00072898
  000ade98: mov r0,r5
  000ade9a: blx 0x0007288c
  000ade9e: cmp.w r0,#0xffffffff
  000adea2: bgt 0x000adeac
  000adea4: mov r0,r5
  000adea6: movs r1,#0x0
  000adea8: blx 0x00072898
  000adeac: ldr r0,[sp,#0x24]
  000adeae: ldr r0,[r0,#0x0]
  000adeb0: ldr.w r1,[r0,#0x178]
  000adeb4: cmp.w r1,#0xffffffff
  000adeb8: itt le
  000adeba: mov.le r1,#0x0
  000adebc: str.w.le r1,[r0,#0x178]
  000adec0: b 0x000ae040
  000adf08: mov r0,r5
  000adf0a: blx 0x00072868
  000adf0e: cbnz r0,0x000adf84
  000adf10: mov r0,r5
  000adf12: blx 0x00072874
  000adf16: cmp r0,#0xb8
  000adf18: beq 0x000adf24
  000adf1a: mov r0,r5
  000adf1c: blx 0x00072874
  000adf20: cmp r0,#0xa8
  000adf22: bne 0x000adf84
  000adf24: ldr.w r0,[r10,#0x1bc]
  000adf28: blx 0x00072880
  000adf2c: cmp r0,#0x2
  000adf2e: bne 0x000adf84
  000adf30: ldrd r1,r6,[r10,#0x360]
  000adf34: cmp r6,r1
  000adf36: beq.w 0x000ae040
  000adf3a: vldr.32 s0,[r10,#0x35c]
  000adf3e: vldr.32 s2,[pc,#-0x64]
  000adf42: vcvt.f32.s32 s0,s0
  000adf46: vdiv.f32 s0,s0,s2
  000adf4a: vcvt.s32.f32 s2,s0
  000adf4e: vcvt.s32.f32 s0,s0
  000adf52: vstr.32 s2,[r10,#0x364]
  000adf56: vmov r0,s0
  000adf5a: cmp r1,r0
  000adf5c: bgt.w 0x000af06a
  000adf60: cmp r6,r0
  000adf62: bge.w 0x000af062
  000adf66: mov r0,r5
  000adf68: blx 0x00072874
  000adf6c: cmp r0,#0xb8
  000adf6e: beq 0x000adf7c
  000adf70: mov r0,r5
  000adf72: blx 0x00072874
  000adf76: cmp r0,#0xa8
  000adf78: bne.w 0x000af056
  000adf7c: ldr r0,[sp,#0x18]
  000adf7e: movs r1,#0x24
  000adf80: b.w 0x000af05a
  000adf84: cmp r4,#0x0
  000adf86: beq 0x000ae02a
  000adf88: mov r0,r4
  000adf8a: blx 0x00072868
  000adf8e: cmp r0,#0x0
  000adf90: bne 0x000ae02a
  000adf92: mov r0,r4
  000adf94: blx 0x00072874
  000adf98: cmp r0,#0xa7
  000adf9a: beq 0x000adfa6
  000adf9c: mov r0,r4
  000adf9e: blx 0x00072874
  000adfa2: cmp r0,#0xae
  000adfa4: bne 0x000ae02a
  000adfa6: ldr.w r0,[r10,#0x1bc]
  000adfaa: blx 0x00072880
  000adfae: cmp r0,#0x1
  000adfb0: bne 0x000ae02a
  000adfb2: ldrd r0,r6,[r10,#0x360]
  000adfb6: cmp r6,r0
  000adfb8: beq 0x000ae040
  000adfba: vldr.32 s0,[r10,#0x35c]
  000adfbe: vldr.32 s2,[pc,#-0xe0]
  000adfc2: vcvt.f32.s32 s0,s0
  000adfc6: vdiv.f32 s0,s0,s2
  000adfca: vcvt.s32.f32 s2,s0
  000adfce: vcvt.s32.f32 s0,s0
  000adfd2: vstr.32 s2,[r10,#0x364]
  000adfd6: vmov r1,s0
  000adfda: cmp r0,r1
  000adfdc: bgt 0x000adff8
  000adfde: cmp r6,r1
  000adfe0: bge 0x000adff2
  000adfe2: ldr r0,[sp,#0x18]
  000adfe4: movs r1,#0x2a
  000adfe6: mov r2,r10
  000adfe8: movs r3,#0x0
  000adfea: blx 0x00072418
  000adfee: ldr.w r0,[r10,#0x360]
  000adff2: mov r1,r0
  000adff4: str.w r0,[r10,#0x364]
  000adff8: cmp r1,r6
  000adffa: ble 0x000ae040
  000adffc: ldr r0,[sp,#0x24]
  000adffe: ldr r0,[r0,#0x0]
  000ae000: blx 0x00071a58
  000ae004: mov r5,r0
  000ae006: mov r0,r4
  000ae008: blx 0x000728a4
  000ae00c: mov r1,r0
  000ae00e: ldr.w r0,[r10,#0x364]
  000ae012: subs r2,r0,r6
  000ae014: mov r0,r5
  000ae016: blx 0x00072454
  000ae01a: mov r0,r4
  000ae01c: blx 0x0007288c
  000ae020: adds r1,r0,#0x1
  000ae022: mov r0,r4
  000ae024: blx 0x00072898
  000ae028: b 0x000ae040
  000ae02a: ldr.w r0,[r10,#0x1bc]
  000ae02e: blx 0x00072880
  000ae032: cmp r0,#0x3
  000ae034: itt eq
  000ae036: ldr.eq.w r0,[r10,#0x1e8]
  000ae03a: cmp.eq r0,#0x0
  000ae03c: beq.w 0x000aefb0
  000ae040: ldr.w r0,[r10,#0x134]
  000ae044: ldr.w r1,[r10,#0x35c]
  000ae048: add r0,r1
  000ae04a: str.w r0,[r10,#0x35c]
  000ae04e: ldrb.w r0,[r10,#0x1a0]
  000ae052: cbnz r0,0x000ae05a
  000ae054: ldrb.w r0,[r10,#0x1a1]
  000ae058: cbz r0,0x000ae062
  000ae05a: mov r0,r10
  000ae05c: mov r1,r9
  000ae05e: blx 0x000727d8
  000ae062: ldrb.w r0,[r10,#0x158]
  000ae066: cbz r0,0x000ae09c
  000ae068: ldr.w r0,[r10,#0x15c]
  000ae06c: cbz r0,0x000ae09c
  000ae06e: ldrb.w r0,[r10,#0x145]
  000ae072: vldr.32 s16,[pc,#-0x190]
  000ae076: cbnz r0,0x000ae0f0
  000ae078: mov r0,r10
  000ae07a: blx 0x000728b0
  000ae07e: cbnz r0,0x000ae0f0
  000ae080: ldrb.w r0,[r10,#0x160]
  000ae084: cbz r0,0x000ae0d8
  000ae086: ldr.w r1,[r10,#0x15c]
  000ae08a: ldr.w r0,[r1,#0x134]
  000ae08e: cbz r0,0x000ae0dc
  000ae090: blx 0x000728bc
  000ae094: mov r1,r0
  000ae096: str.w r0,[r10,#0x15c]
  000ae09a: b 0x000ae0dc
  000ae09c: ldrb.w r0,[r10,#0x145]
  000ae0a0: vldr.32 s16,[pc,#-0x1c0]
  000ae0a4: cbnz r0,0x000ae0f0
  000ae0a6: ldrb.w r0,[r10,#0x1a0]
  000ae0aa: cbnz r0,0x000ae0b2
  000ae0ac: ldrb.w r0,[r10,#0x1a1]
  000ae0b0: cbz r0,0x000ae0bc
  000ae0b2: mov r0,r10
  000ae0b4: mov r1,r9
  000ae0b6: blx 0x000727d8
  000ae0ba: b 0x000ae0f0
  000ae0bc: ldr.w r1,[r10,#0x134]
  000ae0c0: mov r0,r10
  000ae0c2: blx 0x000725ec
  000ae0c6: mov r0,r10
  000ae0c8: blx 0x000728b0
  000ae0cc: cbnz r0,0x000ae0f0
  000ae0ce: mov r0,r10
  000ae0d0: mov r1,r9
  000ae0d2: blx 0x000728c8
  000ae0d6: b 0x000ae0f0
  000ae0d8: ldr.w r1,[r10,#0x15c]
  000ae0dc: cbz r1,0x000ae0e8
  000ae0de: ldrb.w r0,[r10,#0x1ed]
  000ae0e2: cmp r0,#0x0
  000ae0e4: beq.w 0x000ae344
  000ae0e8: mov r0,r10
  000ae0ea: movs r1,#0x0
  000ae0ec: blx 0x000728d4
  000ae0f0: ldrb.w r0,[r10,#0x356]
  000ae0f4: cbz r0,0x000ae112
  000ae0f6: ldr.w r0,[r10,#0x1c4]
  000ae0fa: cmp r0,#0x1
  000ae0fc: beq 0x000ae112
  000ae0fe: ldrb.w r1,[r10,#0x1a0]
  000ae102: cbnz r1,0x000ae112
  000ae104: orr r0,r0,#0x1
  000ae108: cmp r0,#0x3
  000ae10a: itt ne
  000ae10c: mov.ne r0,r10
  000ae10e: blx.ne 0x000728b0
  000ae112: ldrb.w r0,[r10,#0x180]
  000ae116: cbz r0,0x000ae158
  000ae118: ldrb.w r0,[r10,#0x355]
  000ae11c: cbz r0,0x000ae158
  000ae11e: ldrb.w r0,[r10,#0x354]
  000ae122: cbz r0,0x000ae136
  000ae124: ldr r0,[r7,#0xc]
  000ae126: movw r2,#0x2710
  000ae12a: ldrd r0,r1,[r0,#0x8]
  000ae12e: subs r0,r0,r2
  000ae130: sbcs r0,r1,#0x0
  000ae134: blt 0x000ae158
  000ae136: mov r0,r10
  000ae138: blx 0x000724b4
  000ae13c: cbz r0,0x000ae150
  000ae13e: ldr.w r0,[r10,#0x0]
  000ae142: movs r1,#0x0
  000ae144: strb.w r1,[r10,#0x355]
  000ae148: movs r1,#0x2
  000ae14a: blx 0x00072484
  000ae14e: b 0x000ae158
  000ae150: mov r0,r10
  000ae152: mov r1,r9
  000ae154: blx 0x000728e0
  000ae158: ldrb.w r0,[r10,#0x1c0]
  000ae15c: cmp r0,#0x0
  000ae15e: bne.w 0x000ae4e0
  000ae162: ldr.w r0,[r10,#0x1c4]
  000ae166: subs r0,#0x1
  000ae168: cmp r0,#0x2
  000ae16a: bcc.w 0x000ae4e0
  000ae16e: adr.w r0,0xaeef0
  000ae172: vmov.i32 q8,#0x0
  000ae176: vld1.64 {d18,d19},[r0]
  000ae17a: add r0,sp,#0x88
  000ae17c: mov r1,r0
  000ae17e: orr r0,r0,#0x4
  000ae182: vst1.32 {d16,d17},[r0]
  000ae186: add.w r0,r1,#0x18
  000ae18a: vst1.64 {d16,d17},[r0]
  000ae18e: add.w r0,r1,#0x28
  000ae192: vst1.64 {d18,d19},[r0]
  000ae196: mov.w r0,#0x3f800000
  000ae19a: str r0,[sp,#0x88]
  000ae19c: str r0,[sp,#0x9c]
  000ae19e: str r0,[sp,#0xc0]
  000ae1a0: ldr.w r0,[r10,#0x15c]
  000ae1a4: cbz r0,0x000ae1ac
  000ae1a6: ldrb.w r0,[r10,#0x1ed]
  000ae1aa: cbz r0,0x000ae1b2
  000ae1ac: mov.w r8,#0x0
  000ae1b0: b 0x000ae210
  000ae1b2: vmov s0,r9
  000ae1b6: vcvt.f32.s32 s2,s0
  000ae1ba: vldr.32 s4,[r10,#0x154]
  000ae1be: vldr.32 s0,[r10,#0x280]
  000ae1c2: vcmpe.f32 s0,s16
  000ae1c6: vmrs apsr,fpscr
  000ae1ca: vmul.f32 s2,s4,s2
  000ae1ce: vldr.32 s4,[pc,#-0x2e8]
  000ae1d2: vmul.f32 s2,s2,s4
  000ae1d6: bpl 0x000ae1ec
  000ae1d8: vadd.f32 s0,s0,s2
  000ae1dc: vcmpe.f32 s0,s16
  000ae1e0: vstr.32 s0,[r10,#0x280]
  000ae1e4: vmrs apsr,fpscr
  000ae1e8: bgt 0x000ae200
  000ae1ea: b 0x000ae208
  000ae1ec: ble 0x000ae208
  000ae1ee: vsub.f32 s0,s0,s2
  000ae1f2: vcmpe.f32 s0,s16
  000ae1f6: vstr.32 s0,[r10,#0x280]
  000ae1fa: vmrs apsr,fpscr
  000ae1fe: bpl 0x000ae208
  000ae200: vmov.f32 s0,s16
  000ae204: vstr.32 s16,[r10,#0x280]
  000ae208: mov.w r8,#0x1
  000ae20c: vstr.32 s0,[r10,#0x27c]
  000ae210: vldr.32 s0,[r10,#0x278]
  000ae214: add r5,sp,#0x4c
  000ae216: vmov.f64 d17,0x3fd0000000000000
  000ae21a: add r6,sp,#0x88
  000ae21c: mov r0,r5
  000ae21e: movs r3,#0x0
  000ae220: mov r1,r6
  000ae222: vcvt.f64.f32 d16,s0
  000ae226: vmul.f64 d16,d16,d17
  000ae22a: vldr.64 d17,[pc,#-0x33c]
  000ae22e: vldr.32 s2,[r10,#0x27c]
  000ae232: vmul.f64 d16,d16,d17
  000ae236: vldr.64 d17,[pc,#-0x340]
  000ae23a: vldr.32 s4,[pc,#-0x33c]
  000ae23e: vmul.f64 d16,d16,d17
  000ae242: vcvt.f32.f64 s0,d16
  000ae246: vmov r2,s0
  000ae24a: vmul.f32 s0,s2,s4
  000ae24e: vldr.32 s2,[pc,#-0x34c]
  000ae252: vmul.f32 s0,s0,s2
  000ae256: vstr.32 s0,[sp]
  000ae25a: blx 0x00072094
  000ae25e: mov r0,r6
  000ae260: mov r1,r5
  000ae262: blx 0x0006f148
  000ae266: ldr.w r0,[r10,#0x4]
  000ae26a: mov r1,r6
  000ae26c: blx 0x000721e4
  000ae270: cmp.w r8,#0x0
  000ae274: beq 0x000ae2aa
  000ae276: ldr.w r0,[r10,#0x8]
  000ae27a: ldr r4,[sp,#0x20]
  000ae27c: ldr r1,[r0,#0xc]
  000ae27e: ldr r0,[r4,#0x0]
  000ae280: blx 0x000720c4
  000ae284: ldr.w r1,[r10,#0x4]
  000ae288: mov r5,r0
  000ae28a: ldr r0,[r4,#0x0]
  000ae28c: ldr r1,[r1,#0xc]
  000ae28e: blx 0x000720c4
  000ae292: add r6,sp,#0x4c
  000ae294: mov r2,r0
  000ae296: mov r1,r5
  000ae298: mov r0,r6
  000ae29a: blx 0x0006f82c
  000ae29e: ldr.w r0,[r10,#0x0]
  000ae2a2: mov r1,r6
  000ae2a4: adds r0,#0x4
  000ae2a6: blx 0x0006f148
  000ae2aa: ldr r0,[0x000ae478]
  000ae2ac: vldr.32 s0,[r10,#0x27c]
  000ae2b0: add r0,pc
  000ae2b2: vcmpe.f32 s0,#0
  000ae2b6: ldr r0,[r0,#0x0]
  000ae2b8: ldr r0,[r0,#0x0]
  000ae2ba: cbz r0,0x000ae2e8
  000ae2bc: vmrs apsr,fpscr
  000ae2c0: ble 0x000ae2c8
  000ae2c2: ldr.w r0,[r10,#0x104]
  000ae2c6: cbz r0,0x000ae33a
  000ae2c8: vcmpe.f32 s0,#0
  000ae2cc: vmrs apsr,fpscr
  000ae2d0: bpl.w 0x000ae428
  000ae2d4: ldr.w r0,[r10,#0x104]
  000ae2d8: cmp r0,#0x0
  000ae2da: bne.w 0x000ae428
  000ae2de: vldr.32 s2,[pc,#0x19c]
  000ae2e2: vmul.f32 s0,s0,s2
  000ae2e6: b 0x000ae32a
  000ae2e8: vmrs apsr,fpscr
  000ae2ec: ble 0x000ae2f8
  000ae2ee: ldr.w r0,[r10,#0x104]
  000ae2f2: cmp r0,#0x0
  000ae2f4: beq.w 0x000ae3f8
  000ae2f8: vcmpe.f32 s0,#0
  000ae2fc: vmrs apsr,fpscr
  000ae300: bpl.w 0x000ae428
  000ae304: ldr.w r0,[r10,#0x104]
  000ae308: cmp r0,#0x0
  000ae30a: bne.w 0x000ae428
  000ae30e: vmov s2,r9
  000ae312: vcvt.f32.s32 s2,s2
  000ae316: vldr.32 s4,[r10,#0x154]
  000ae31a: vmul.f32 s2,s4,s2
  000ae31e: vldr.32 s4,[pc,#0x160]
  000ae322: vdiv.f32 s2,s2,s4
  000ae326: vadd.f32 s0,s0,s2
  000ae32a: vcmpe.f32 s0,#0
  000ae32e: vstr.32 s0,[r10,#0x27c]
  000ae332: vmrs apsr,fpscr
  000ae336: bgt 0x000ae422
  000ae338: b 0x000ae428
  000ae33a: vldr.32 s2,[pc,#0x140]
  000ae33e: vmul.f32 s0,s0,s2
  000ae342: b 0x000ae414
  000ae344: ldr r0,[r1,#0x0]
  000ae346: add r5,sp,#0x88
  000ae348: ldr r2,[r0,#0x28]
  000ae34a: mov r0,r5
  000ae34c: blx r2
  000ae34e: add.w r6,r10,#0xec
  000ae352: mov r1,r5
  000ae354: mov r0,r6
  000ae356: blx 0x0006eb3c
  000ae35a: add.w r8,sp,#0x4c
  000ae35e: mov r1,r10
  000ae360: mov r0,r8
  000ae362: blx 0x0007264c
  000ae366: add r5,sp,#0x88
  000ae368: mov r1,r6
  000ae36a: mov r2,r8
  000ae36c: mov r0,r5
  000ae36e: blx 0x0006ec38
  000ae372: mov r0,r5
  000ae374: blx 0x0006ec44
  000ae378: vmov s0,r0
  000ae37c: movw r1,#0x4e1f
  000ae380: vcvt.s32.f32 s0,s0
  000ae384: vmov r0,s0
  000ae388: cmp r0,r1
  000ae38a: itt le
  000ae38c: mov.le r0,#0x1
  000ae38e: strb.le.w r0,[r10,#0x330]
  000ae392: ldrd r5,r6,[r10,#0xec]
  000ae396: ldr r0,[sp,#0x24]
  000ae398: ldr.w r4,[r10,#0xf4]
  000ae39c: ldr r0,[r0,#0x0]
  000ae39e: blx 0x00071a58
  000ae3a2: blx 0x000722a4
  000ae3a6: vldr.32 s16,[pc,#0xdc]
  000ae3aa: vmov s2,r0
  000ae3ae: vmov.f32 s0,0x40800000
  000ae3b2: vadd.f32 s2,s2,s16
  000ae3b6: vcmpe.f32 s2,s0
  000ae3ba: vmrs apsr,fpscr
  000ae3be: bpl 0x000ae3d4
  000ae3c0: ldr r0,[sp,#0x24]
  000ae3c2: ldr r0,[r0,#0x0]
  000ae3c4: blx 0x00071a58
  000ae3c8: blx 0x000722a4
  000ae3cc: vmov s0,r0
  000ae3d0: vadd.f32 s0,s0,s16
  000ae3d4: movs r0,#0x1
  000ae3d6: mov r1,r5
  000ae3d8: str r0,[sp,#0x0]
  000ae3da: mov r0,r10
  000ae3dc: mov r2,r6
  000ae3de: mov r3,r4
  000ae3e0: vstr.32 s0,[sp,#0x4]
  000ae3e4: blx 0x00072664
  000ae3e8: vmov s16,r0
  000ae3ec: ldrb.w r0,[r10,#0x1a0]
  000ae3f0: cmp r0,#0x0
  000ae3f2: bne.w 0x000ae0b2
  000ae3f6: b 0x000ae0f0
  000ae3f8: vmov s2,r9
  000ae3fc: vcvt.f32.s32 s2,s2
  000ae400: vldr.32 s4,[r10,#0x154]
  000ae404: vmul.f32 s2,s4,s2
  000ae408: vldr.32 s4,[pc,#0x7c]
  000ae40c: vdiv.f32 s2,s2,s4
  000ae410: vadd.f32 s0,s0,s2
  000ae414: vcmpe.f32 s0,#0
  000ae418: vstr.32 s0,[r10,#0x27c]
  000ae41c: vmrs apsr,fpscr
  000ae420: bpl 0x000ae428
  000ae422: movs r0,#0x0
  000ae424: str.w r0,[r10,#0x27c]
  000ae428: vldr.32 s0,[r10,#0x278]
  000ae42c: vcmpe.f32 s0,#0
  000ae430: vmrs apsr,fpscr
  000ae434: ble 0x000ae43c
  000ae436: ldr.w r0,[r10,#0x100]
  000ae43a: cbz r0,0x000ae48c
  000ae43c: vcmpe.f32 s0,#0
  000ae440: vmrs apsr,fpscr
  000ae444: bpl 0x000ae4bc
  000ae446: ldr.w r0,[r10,#0x100]
  000ae44a: cbnz r0,0x000ae4bc
  000ae44c: vmov s2,r9
  000ae450: vcvt.f32.s32 s2,s2
  000ae454: vldr.32 s4,[r10,#0x154]
  000ae458: vmul.f32 s2,s4,s2
  000ae45c: vldr.32 s4,[pc,#0x20]
  000ae460: vdiv.f32 s2,s2,s4
  000ae464: vadd.f32 s0,s0,s2
  000ae468: vcmpe.f32 s0,#0
  000ae46c: vstr.32 s0,[r10,#0x278]
  000ae470: vmrs apsr,fpscr
  000ae474: bgt 0x000ae4b6
  000ae476: b 0x000ae4bc
  000ae48c: vmov s2,r9
  000ae490: vcvt.f32.s32 s2,s2
  000ae494: vldr.32 s4,[r10,#0x154]
  000ae498: vmul.f32 s2,s4,s2
  000ae49c: vldr.32 s4,[pc,#-0x18]
  000ae4a0: vdiv.f32 s2,s2,s4
  000ae4a4: vadd.f32 s0,s0,s2
  000ae4a8: vcmpe.f32 s0,#0
  000ae4ac: vstr.32 s0,[r10,#0x278]
  000ae4b0: vmrs apsr,fpscr
  000ae4b4: bpl 0x000ae4bc
  000ae4b6: movs r0,#0x0
  000ae4b8: str.w r0,[r10,#0x278]
  000ae4bc: ldr.w r0,[r10,#0x194]
  000ae4c0: cbz r0,0x000ae4ce
  000ae4c2: ldr r1,[0x000ae7ec]
  000ae4c4: movs r2,#0x46
  000ae4c6: ldr.w r0,[r10,#0x88]
  000ae4ca: blx 0x000728ec
  000ae4ce: movs r0,#0x0
  000ae4d0: strd r0,r0,[r10,#0x258]
  000ae4d4: strd r0,r0,[r10,#0x100]
  000ae4d8: str.w r0,[r10,#0x270]
  000ae4dc: str.w r0,[r10,#0x268]
  000ae4e0: ldr r0,[sp,#0x24]
  000ae4e2: ldr r0,[r0,#0x0]
  000ae4e4: blx 0x00071a58
  000ae4e8: blx 0x000722c8
  000ae4ec: cmp r0,#0x1
  000ae4ee: blt 0x000ae526
  000ae4f0: mov r0,r10
  000ae4f2: blx 0x000724b4
  000ae4f6: cbnz r0,0x000ae526
  000ae4f8: ldrd r0,r1,[r10,#0x118]
  000ae4fc: adds.w r0,r0,r9
  000ae500: adc.w r1,r1,r9, asr #0x1f
  000ae504: strd r0,r1,[r10,#0x118]
  000ae508: subs r0,#0x65
  000ae50a: sbcs r0,r1,#0x0
  000ae50e: blt 0x000ae526
  000ae510: add.w r0,r10,#0x118
  000ae514: movs r1,#0x0
  000ae516: strd r1,r1,[r0,#0x0]
  000ae51a: ldr.w r1,[r10,#0x218]
  000ae51e: ldr.w r0,[r10,#0x0]
  000ae522: blx 0x000728f8
  000ae526: ldr r0,[sp,#0x24]
  000ae528: ldr r0,[r0,#0x0]
  000ae52a: blx 0x00071a58
  000ae52e: blx 0x00072904
  000ae532: mov r5,r0
  000ae534: cmp r0,#0x0
  000ae536: blt 0x000ae60c
  000ae538: mov r0,r10
  000ae53a: blx 0x000724b4
  000ae53e: cmp r0,#0x0
  000ae540: bne 0x000ae60c
  000ae542: vmov.32 d16[0x0],r9
  000ae546: add.w r4,r10,#0x120
  000ae54a: adr.w r2,0xaf09c
  000ae54e: cmp r5,#0x0
  000ae550: vld1.64 {d18,d19},[r4]
  000ae554: it eq
  000ae556: add.eq r2,#0x4
  000ae558: vmov.32 d16[0x1],r11
  000ae55c: vmov d17,d16
  000ae560: vldr.32 s16,[r2]
  000ae564: vadd.i64 q8,q9,q8
  000ae568: vmov.s32 r0,d16[0x0]
  000ae56c: vmov.s32 r1,d16[0x1]
  000ae570: vst1.64 {d16,d17},[r4]
  000ae574: blx 0x0006f7d8
  000ae578: vmov s0,r0
  000ae57c: add.w r6,r10,#0x128
  000ae580: vcmpe.f32 s16,s0
  000ae584: vmrs apsr,fpscr
  000ae588: bpl 0x000ae5ae
  000ae58a: movs r0,#0x0
  000ae58c: strd r0,r0,[r4,#0x0]
  000ae590: ldr.w r0,[r10,#0x0]
  000ae594: blx 0x000724f0
  000ae598: mov r8,r0
  000ae59a: ldr.w r0,[r10,#0x0]
  000ae59e: blx 0x00072910
  000ae5a2: cmp r8,r0
  000ae5a4: itt lt
  000ae5a6: ldr.lt.w r0,[r10,#0x0]
  000ae5aa: blx.lt 0x0007291c
  000ae5ae: ldrd r0,r1,[r6,#0x0]
  000ae5b2: adr.w r4,0xaf0a4
  000ae5b6: cmp r5,#0x0
  000ae5b8: it eq
  000ae5ba: add.eq r4,#0x4
  000ae5bc: blx 0x0006f7d8
  000ae5c0: vmov s0,r0
  000ae5c4: vldr.32 s2,[r4]
  000ae5c8: vcmpe.f32 s2,s0
  000ae5cc: vmrs apsr,fpscr
  000ae5d0: bpl 0x000ae60c
  000ae5d2: movs r0,#0x0
  000ae5d4: strd r0,r0,[r6,#0x0]
  000ae5d8: ldr.w r0,[r10,#0x0]
  000ae5dc: blx 0x000724f0
  000ae5e0: mov r5,r0
  000ae5e2: ldr.w r0,[r10,#0x0]
  000ae5e6: blx 0x00072910
  000ae5ea: cmp r5,r0
  000ae5ec: blt 0x000ae60c
  000ae5ee: ldr.w r0,[r10,#0x0]
  000ae5f2: blx 0x00072928
  000ae5f6: mov r5,r0
  000ae5f8: ldr.w r0,[r10,#0x0]
  000ae5fc: blx 0x00072934
  000ae600: cmp r5,r0
  000ae602: itt lt
  000ae604: ldr.lt.w r0,[r10,#0x0]
  000ae608: blx.lt 0x00072940
  000ae60c: ldr.w r0,[r10,#0xac]
  000ae610: cmp r0,#0x0
  000ae612: beq.w 0x000ae74c
  000ae616: ldr.w r0,[r10,#0x30c]
  000ae61a: cmp r0,#0x1
  000ae61c: blt.w 0x000ae74c
  000ae620: ldr.w r0,[r10,#0x0]
  000ae624: movs r1,#0x0
  000ae626: blx 0x0007294c
  000ae62a: ldr.w r1,[r10,#0x30c]
  000ae62e: ldr.w r2,[r10,#0x0]
  000ae632: ldr.w r0,[r10,#0xac]
  000ae636: sub.w r1,r1,r9
  000ae63a: str.w r1,[r10,#0x30c]
  000ae63e: adds r1,r2,#0x4
  000ae640: blx 0x000721e4
  000ae644: ldr.w r0,[r10,#0x0]
  000ae648: add r5,sp,#0x88
  000ae64a: add.w r2,r10,#0x314
  000ae64e: adds r1,r0,#0x4
  000ae650: mov r0,r5
  000ae652: blx 0x0006f694
  000ae656: ldr.w r0,[r10,#0xac]
  000ae65a: mov r1,r5
  000ae65c: blx 0x00072460
  000ae660: ldr.w r1,[r10,#0xac]
  000ae664: ldr r0,[sp,#0x20]
  000ae666: ldr r1,[r1,#0x1c]
  000ae668: ldr r0,[r0,#0x0]
  000ae66a: blx 0x00072370
  000ae66e: vmov s0,r9
  000ae672: vldr.32 s2,[pc,#0x17c]
  000ae676: vcvt.f32.s32 s0,s0
  000ae67a: vldr.32 s4,[r0,#0x24]
  000ae67e: vmla.f32 s4,s0,s2
  000ae682: vstr.32 s4,[r0,#0x24]
  000ae686: ldrd r0,r1,[r10,#0x30c]
  000ae68a: vmov s0,r1
  000ae68e: vldr.32 s4,[pc,#0x164]
  000ae692: vcvt.f32.s32 s0,s0
  000ae696: vmov s2,r0
  000ae69a: vcvt.f32.s32 s2,s2
  000ae69e: vmul.f32 s4,s0,s4
  000ae6a2: vcmpe.f32 s4,s2
  000ae6a6: vmrs apsr,fpscr
  000ae6aa: bpl 0x000ae6c0
  000ae6ac: vldr.32 s2,[pc,#0x148]
  000ae6b0: subs r0,r1,r0
  000ae6b2: vmul.f32 s0,s0,s2
  000ae6b6: vmov s2,r0
  000ae6ba: vcvt.f32.s32 s2,s2
  000ae6be: b 0x000ae6d2
  000ae6c0: vldr.32 s4,[pc,#0x134]
  000ae6c4: vmul.f32 s0,s0,s4
  000ae6c8: vcmpe.f32 s0,s2
  000ae6cc: vmrs apsr,fpscr
  000ae6d0: ble 0x000ae6d8
  000ae6d2: vdiv.f32 s0,s2,s0
  000ae6d6: b 0x000ae6dc
  000ae6d8: vmov.f32 s0,0x3f800000
  000ae6dc: vldr.32 s2,[r10,#0x320]
  000ae6e0: ldr.w r0,[r10,#0xac]
  000ae6e4: vmul.f32 s0,s0,s2
  000ae6e8: vmov r1,s0
  000ae6ec: mov r2,r1
  000ae6ee: mov r3,r1
  000ae6f0: blx 0x000727b4
  000ae6f4: ldr.w r1,[r10,#0xac]
  000ae6f8: ldr r0,[sp,#0x20]
  000ae6fa: ldr r1,[r1,#0xc]
  000ae6fc: ldr r0,[r0,#0x0]
  000ae6fe: blx 0x00072088
  000ae702: movs r1,#0x0
  000ae704: mov r2,r9
  000ae706: mov r3,r11
  000ae708: str r1,[sp,#0x0]
  000ae70a: blx 0x0006f7cc
  000ae70e: ldr.w r0,[r10,#0x30c]
  000ae712: cmp r0,#0x0
  000ae714: bgt 0x000ae74c
  000ae716: ldr.w r0,[r10,#0xac]
  000ae71a: cbz r0,0x000ae724
  000ae71c: blx 0x00071fc8
  000ae720: blx 0x0006eb48
  000ae724: ldr.w r0,[r10,#0x0]
  000ae728: movs r4,#0x0
  000ae72a: movs r1,#0x1
  000ae72c: str.w r4,[r10,#0xac]
  000ae730: blx 0x0007294c
  000ae734: ldr r0,[0x000ae7fc]
  000ae736: movw r1,#0x45b
  000ae73a: add r0,pc
  000ae73c: ldr r0,[r0,#0x0]
  000ae73e: ldr r0,[r0,#0x0]
  000ae740: blx 0x000724a8
  000ae744: ldr r0,[sp,#0x24]
  000ae746: ldr r0,[r0,#0x0]
  000ae748: str.w r4,[r0,#0x13c]
  000ae74c: mov r0,r10
  000ae74e: ldr.w r5,[r10,#0x0]
  000ae752: blx 0x000724b4
  000ae756: cbz r0,0x000ae75c
  000ae758: movs r2,#0x0
  000ae75a: b 0x000ae766
  000ae75c: ldr.w r0,[r10,#0x1c]
  000ae760: adds r2,r0,#0x1
  000ae762: it ne
  000ae764: mov.ne r2,#0x1
  000ae766: mov r0,r5
  000ae768: mov r1,r9
  000ae76a: ldr r6,[sp,#0x18]
  000ae76c: blx 0x00072958
  000ae770: ldrb.w r0,[r10,#0x38]
  000ae774: cbz r0,0x000ae7ac
  000ae776: ldr r0,[0x000ae800]
  000ae778: add r0,pc
  000ae77a: ldr r0,[r0,#0x0]
  000ae77c: ldrb r0,[r0,#0xf]
  000ae77e: cbz r0,0x000ae7ac
  000ae780: ldr.w r0,[r10,#0xb4]
  000ae784: adds r0,#0x1
  000ae786: beq 0x000ae7ac
  000ae788: add r5,sp,#0x88
  000ae78a: ldr.w r1,[r10,#0x0]
  000ae78e: mov r0,r5
  000ae790: blx 0x00072964
  000ae794: ldr r0,[0x000ae804]
  000ae796: movs r2,#0x0
  000ae798: ldr.w r1,[r10,#0xb4]
  000ae79c: movs r3,#0x0
  000ae79e: add r0,pc
  000ae7a0: str r2,[sp,#0x0]
  000ae7a2: mov r2,r5
  000ae7a4: ldr r0,[r0,#0x0]
  000ae7a6: ldr r0,[r0,#0x0]
  000ae7a8: blx 0x000726e8
  000ae7ac: ldr.w r0,[r10,#0x8]
  000ae7b0: ldr r1,[r0,#0xc]
  000ae7b2: ldr r0,[sp,#0x20]
  000ae7b4: ldr r0,[r0,#0x0]
  000ae7b6: blx 0x000720c4
  000ae7ba: add r5,sp,#0x88
  000ae7bc: mov r1,r0
  000ae7be: mov r0,r5
  000ae7c0: blx 0x0006f4d8
  000ae7c4: ldr r0,[0x000ae808]
  000ae7c6: mov r1,r5
  000ae7c8: add r0,pc
  000ae7ca: ldr r0,[r0,#0x0]
  000ae7cc: blx 0x0006eb3c
  000ae7d0: ldrb.w r0,[r10,#0x13c]
  000ae7d4: cbz r0,0x000ae80c
  000ae7d6: mov r0,r10
  000ae7d8: blx 0x000725d4
  000ae7dc: mov r2,r0
  000ae7de: movs r0,#0x4
  000ae7e0: movs r1,#0x6
  000ae7e2: movs r3,#0x4
  000ae7e4: blx 0x00072970
  000ae7e8: b 0x000ae82c
  000ae80c: ldr r0,[0x000aeb44]
  000ae80e: ldr r1,[0x000aeb48]
  000ae810: add r0,pc
  000ae812: add r1,pc
  000ae814: ldr r4,[r0,#0x0]
  000ae816: ldr r1,[r1,#0x0]
  000ae818: add.w r0,r4,#0x280
  000ae81c: add.w r1,r1,#0x280
  000ae820: ldr.w r5,[r4,#0x2b4]
  000ae824: blx 0x0007297c
  000ae828: str.w r5,[r4,#0x2b4]
  000ae82c: ldr.w r0,[r10,#0xfc]
  000ae830: cbz r0,0x000ae86a
  000ae832: blx 0x00072988
  000ae836: mov r5,r0
  000ae838: ldr.w r0,[r10,#0xfc]
  000ae83c: ldr r1,[sp,#0x10]
  000ae83e: blx 0x00072994
  000ae842: ldr.w r0,[r10,#0xfc]
  000ae846: blx 0x00072988
  000ae84a: cmp r0,r5
  000ae84c: beq 0x000ae86a
  000ae84e: ldr.w r0,[r10,#0xfc]
  000ae852: ldrb r1,[r0,#0x4]
  000ae854: cbnz r1,0x000ae85e
  000ae856: blx 0x00072988
  000ae85a: cmp r0,#0x0
  000ae85c: beq 0x000ae8fc
  000ae85e: mov r0,r6
  000ae860: movs r1,#0x17
  000ae862: mov r2,r10
  000ae864: movs r3,#0x0
  000ae866: blx 0x00072418
  000ae86a: ldr r0,[0x000aeb4c]
  000ae86c: ldrb.w r1,[r10,#0x378]
  000ae870: add r0,pc
  000ae872: ldr.w r8,[r0,#0x0]
  000ae876: movs r0,#0x0
  000ae878: str.w r0,[r10,#0x20]
  000ae87c: strb.w r0,[r10,#0x30a]
  000ae880: ldr.w r0,[r8,#0x0]
  000ae884: cmp r0,r1
  000ae886: itt ne
  000ae888: mov.ne r0,#0x1
  000ae88a: strb.ne.w r0,[r10,#0x84]
  000ae88e: ldr.w r0,[r10,#0x0]
  000ae892: ldr.w r4,[r10,#0x130]
  000ae896: blx 0x0007225c
  000ae89a: cmp r4,r0
  000ae89c: ble.w 0x000aea32
  000ae8a0: ldrb.w r0,[r10,#0x1f4]
  000ae8a4: cmp r0,#0x0
  000ae8a6: bne.w 0x000aea32
  000ae8aa: ldrb.w r0,[r10,#0x24]
  000ae8ae: cmp r0,#0x0
  000ae8b0: bne.w 0x000aea32
  000ae8b4: mov r0,r6
  000ae8b6: blx 0x000729a0
  000ae8ba: ldr.w r0,[r10,#0x0]
  000ae8be: blx 0x0007225c
  000ae8c2: str.w r0,[r10,#0x130]
  000ae8c6: mov r0,r10
  000ae8c8: blx 0x000729ac
  000ae8cc: ldr.w r1,[r10,#0x0]
  000ae8d0: ldrb.w r0,[r1,#0x64]
  000ae8d4: cbz r0,0x000ae902
  000ae8d6: ldr r0,[0x000aeb50]
  000ae8d8: movs r1,#0x1
  000ae8da: strb.w r1,[r10,#0x30a]
  000ae8de: movs r4,#0x0
  000ae8e0: add r0,pc
  000ae8e2: movs r1,#0x19
  000ae8e4: movs r2,#0x0
  000ae8e6: movs r3,#0x0
  000ae8e8: ldr r0,[r0,#0x0]
  000ae8ea: str r4,[sp,#0x0]
  000ae8ec: ldr r0,[r0,#0x0]
  000ae8ee: blx 0x00071548
  000ae8f2: ldr.w r1,[r10,#0x0]
  000ae8f6: strb.w r4,[r1,#0x64]
  000ae8fa: b 0x000ae94c
  000ae8fc: mov r0,r6
  000ae8fe: movs r1,#0x18
  000ae900: b 0x000ae862
  000ae902: ldrb.w r0,[r1,#0x65]
  000ae906: cbz r0,0x000ae928
  000ae908: ldr r0,[0x000aeb54]
  000ae90a: movs r4,#0x0
  000ae90c: movs r1,#0x17
  000ae90e: movs r2,#0x0
  000ae910: add r0,pc
  000ae912: movs r3,#0x0
  000ae914: str r4,[sp,#0x0]
  000ae916: ldr r0,[r0,#0x0]
  000ae918: ldr r0,[r0,#0x0]
  000ae91a: blx 0x00071548
  000ae91e: ldr.w r1,[r10,#0x0]
  000ae922: strb.w r4,[r1,#0x65]
  000ae926: b 0x000ae94c
  000ae928: ldrb.w r0,[r1,#0x66]
  000ae92c: cbz r0,0x000ae94c
  000ae92e: ldr r0,[0x000aeb58]
  000ae930: movs r4,#0x0
  000ae932: movs r1,#0x18
  000ae934: movs r2,#0x0
  000ae936: add r0,pc
  000ae938: movs r3,#0x0
  000ae93a: str r4,[sp,#0x0]
  000ae93c: ldr r0,[r0,#0x0]
  000ae93e: ldr r0,[r0,#0x0]
  000ae940: blx 0x00071548
  000ae944: ldr.w r1,[r10,#0x0]
  000ae948: strb.w r4,[r1,#0x66]
  000ae94c: add r5,sp,#0x4c
  000ae94e: mov r0,r5
  000ae950: blx 0x000729b8
  000ae954: add.w r11,sp,#0x88
  000ae958: mov r1,r5
  000ae95a: mov r0,r11
  000ae95c: blx 0x0006ec80
  000ae960: ldr r0,[sp,#0x20]
  000ae962: add r6,sp,#0x40
  000ae964: ldr r2,[0x000aeb5c]
  000ae966: mov r1,r11
  000ae968: ldr.w r9,[r0,#0x0]
  000ae96c: movs r0,#0x0
  000ae96e: strd r0,r0,[sp,#0x4c]
  000ae972: str r0,[sp,#0x54]
  000ae974: mov r0,r6
  000ae976: blx 0x0006ec74
  000ae97a: add r5,sp,#0xc8
  000ae97c: ldr r2,[sp,#0x10]
  000ae97e: mov r1,r6
  000ae980: mov r0,r5
  000ae982: blx 0x0006f1cc
  000ae986: add r2,sp,#0x4c
  000ae988: mov r0,r9
  000ae98a: mov r1,r5
  000ae98c: blx 0x000729c4
  000ae990: vldr.32 s0,[sp,#0x4c]
  000ae994: vcmpe.f32 s0,#0
  000ae998: vmrs apsr,fpscr
  000ae99c: bls 0x000ae9bc
  000ae99e: ldr r0,[0x000aeb60]
  000ae9a0: movs r1,#0x2
  000ae9a2: add r0,pc
  000ae9a4: ldr r0,[r0,#0x0]
  000ae9a6: vldr.32 s2,[r0]
  000ae9aa: vcvt.f32.s32 s2,s2
  000ae9ae: vcmpe.f32 s0,s2
  000ae9b2: vmrs apsr,fpscr
  000ae9b6: it lt
  000ae9b8: mov.lt r1,#0x0
  000ae9ba: b 0x000ae9be
  000ae9bc: movs r1,#0x1
  000ae9be: str.w r1,[r10,#0x20]
  000ae9c2: vldr.32 s2,[sp,#0x54]
  000ae9c6: ldr.w r9,[sp,#0x1c]
  000ae9ca: vcmpe.f32 s2,#0
  000ae9ce: vmrs apsr,fpscr
  000ae9d2: bpl 0x000ae9da
  000ae9d4: orr r0,r1,#0x10
  000ae9d8: b 0x000aea2e
  000ae9da: vldr.32 s2,[pc,#0x188]
  000ae9de: orr r0,r1,#0x20
  000ae9e2: str.w r0,[r10,#0x20]
  000ae9e6: vcmpe.f32 s0,s2
  000ae9ea: vmrs apsr,fpscr
  000ae9ee: bpl 0x000ae9f4
  000ae9f0: movs r0,#0x1
  000ae9f2: b 0x000aea06
  000ae9f4: vldr.32 s2,[pc,#0x170]
  000ae9f8: vcmpe.f32 s0,s2
  000ae9fc: vmrs apsr,fpscr
  000aea00: bpl 0x000aea0a
  000aea02: orr r0,r1,#0x21
  000aea06: str.w r0,[r10,#0x20]
  000aea0a: vldr.32 s2,[pc,#0x160]
  000aea0e: vcmpe.f32 s0,s2
  000aea12: vmrs apsr,fpscr
  000aea16: ble 0x000aea1c
  000aea18: movs r0,#0x2
  000aea1a: b 0x000aea2e
  000aea1c: vldr.32 s2,[pc,#0x150]
  000aea20: vcmpe.f32 s0,s2
  000aea24: vmrs apsr,fpscr
  000aea28: ble 0x000aea32
  000aea2a: orr r0,r0,#0x2
  000aea2e: str.w r0,[r10,#0x20]
  000aea32: ldr.w r0,[r10,#0x0]
  000aea36: adds r1,r0,#0x4
  000aea38: add r0,sp,#0x4c
  000aea3a: blx 0x0006f16c
  000aea3e: ldr.w r0,[r10,#0x0]
  000aea42: add.w r11,sp,#0xc8
  000aea46: adds r1,r0,#0x4
  000aea48: mov r0,r11
  000aea4a: blx 0x0006f4c0
  000aea4e: ldrb.w r0,[r10,#0x1a0]
  000aea52: cbz r0,0x000aea96
  000aea54: ldr.w r0,[r10,#0x2c]
  000aea58: blx 0x000721cc
  000aea5c: mov r5,r0
  000aea5e: ldr.w r0,[r10,#0x28]
  000aea62: blx 0x000721cc
  000aea66: add r6,sp,#0x88
  000aea68: mov r2,r0
  000aea6a: mov r1,r5
  000aea6c: mov r0,r6
  000aea6e: blx 0x0006f82c
  000aea72: add r5,sp,#0x40
  000aea74: mov r1,r6
  000aea76: mov r0,r5
  000aea78: blx 0x0006f16c
  000aea7c: add r0,sp,#0x4c
  000aea7e: mov r1,r5
  000aea80: blx 0x0006eb3c
  000aea84: add r5,sp,#0x40
  000aea86: mov r1,r6
  000aea88: mov r0,r5
  000aea8a: blx 0x0006f4c0
  000aea8e: add r0,sp,#0xc8
  000aea90: mov r1,r5
  000aea92: blx 0x0006eb3c
  000aea96: ldr.w r0,[r8,#0x0]
  000aea9a: cbz r0,0x000aeac0
  000aea9c: ldr r0,[r7,#0x14]
  000aea9e: cbnz r0,0x000aeac0
  000aeaa0: ldr r0,[r7,#0x18]
  000aeaa2: cmp r0,#0x3
  000aeaa4: itttt ne
  000aeaa6: ldrb.ne.w r1,[r10,#0x158]
  000aeaaa: clz.ne r1,r1
  000aeaae: sub.ne r0,#0x1
  000aeab0: clz.ne r0,r0
  000aeab4: ittt ne
  000aeab6: lsr.ne r0,r0,#0x5
  000aeab8: lsr.ne r1,r1,#0x5
  000aeaba: orrs.ne.w r0,r0,r1
  000aeabe: bne 0x000aeb84
  000aeac0: add r5,sp,#0x34
  000aeac2: add r1,sp,#0xc8
  000aeac4: mov r0,r5
  000aeac6: blx 0x0006ec80
  000aeaca: add r6,sp,#0x40
  000aeacc: ldr r1,[0x000aeb74]
  000aeace: mov r2,r5
  000aead0: mov r0,r6
  000aead2: blx 0x00072658
  000aead6: add r5,sp,#0x88
  000aead8: add r1,sp,#0x4c
  000aeada: mov r2,r6
  000aeadc: mov r0,r5
  000aeade: blx 0x0006f1cc
  000aeae2: ldr r1,[0x000aeb78]
  000aeae4: ldr r0,[sp,#0x20]
  000aeae6: add r1,pc
  000aeae8: ldr r6,[r1,#0x0]
  000aeaea: mov r1,r5
  000aeaec: ldr r0,[r0,#0x0]
  000aeaee: mov r2,r6
  000aeaf0: blx 0x000729c4
  000aeaf4: vldr.32 s0,[r6,#0x8]
  000aeaf8: vcmpe.f32 s0,#0
  000aeafc: vmrs apsr,fpscr
  000aeb00: ble 0x000aeb14
  000aeb02: vldr.64 d16,[r6]
  000aeb06: add r1,sp,#0x40
  000aeb08: ldr r0,[r6,#0x8]
  000aeb0a: str r0,[sp,#0x48]
  000aeb0c: mov r0,r6
  000aeb0e: vstr.64 d16,[sp,#0x40]
  000aeb12: b 0x000aeda6
  000aeb14: add.w r9,sp,#0x34
  000aeb18: ldr r2,[0x000aeb7c]
  000aeb1a: mov r1,r6
  000aeb1c: mov r0,r9
  000aeb1e: blx 0x0006ec74
  000aeb22: add r4,sp,#0x28
  000aeb24: ldr r2,[0x000aeb80]
  000aeb26: add.w r1,r10,#0x94
  000aeb2a: mov r0,r4
  000aeb2c: blx 0x0006ec74
  000aeb30: add r5,sp,#0x40
  000aeb32: mov r1,r9
  000aeb34: mov r2,r4
  000aeb36: mov r0,r5
  000aeb38: blx 0x0006f1cc
  000aeb3c: mov r0,r6
  000aeb3e: mov r1,r5
  000aeb40: b 0x000aeda6
  000aeb84: ldr r0,[0x000aeecc]
  000aeb86: ldr r1,[0x000aeed0]
  000aeb88: add r0,pc
  000aeb8a: vldr.64 d17,[pc,#0x34c]
  000aeb8e: add r1,pc
  000aeb90: ldr r0,[r0,#0x0]
  000aeb92: ldr r1,[r1,#0x0]
  000aeb94: ldr r0,[r0,#0x0]
  000aeb96: asrs r0,r0,#0x1
  000aeb98: vmov s0,r0
  000aeb9c: vcvt.f32.s32 s4,s0
  000aeba0: ldr r0,[r1,#0x0]
  000aeba2: asrs r0,r0,#0x1
  000aeba4: vmov s0,r0
  000aeba8: vcvt.f32.s32 s6,s0
  000aebac: ldr r0,[0x000aeee0]
  000aebae: vcvt.f64.f32 d16,s4
  000aebb2: ldr r1,[0x000aeee4]
  000aebb4: add r0,pc
  000aebb6: add r1,pc
  000aebb8: ldr r0,[r0,#0x0]
  000aebba: ldr r1,[r1,#0x0]
  000aebbc: vmul.f64 d16,d16,d17
  000aebc0: vldr.32 s0,[r0]
  000aebc4: vldr.32 s2,[r1]
  000aebc8: vcvt.f32.s32 s8,s0
  000aebcc: vcvt.f32.f64 s0,d16
  000aebd0: vcvt.f64.f32 d18,s6
  000aebd4: vcmpe.f32 s0,s8
  000aebd8: vmrs apsr,fpscr
  000aebdc: vcvt.f32.s32 s2,s2
  000aebe0: vmul.f64 d17,d18,d17
  000aebe4: vcvt.f32.f64 s16,d17
  000aebe8: vmov.f32 s10,s0
  000aebec: vcmpe.f32 s16,s2
  000aebf0: it gt
  000aebf2: vmov.gt.f32 s10,s8
  000aebf6: vmrs apsr,fpscr
  000aebfa: vneg.f32 s8,s0
  000aebfe: vmov.f32 s12,s16
  000aec02: vadd.f32 s1,s6,s16
  000aec06: vcmpe.f32 s10,s8
  000aec0a: it gt
  000aec0c: vmov.gt.f32 s12,s2
  000aec10: vmrs apsr,fpscr
  000aec14: vneg.f32 s2,s16
  000aec18: vcmpe.f32 s12,s2
  000aec1c: it mi
  000aec1e: vmov.mi.f32 s10,s8
  000aec22: vmrs apsr,fpscr
  000aec26: it mi
  000aec28: vmov.mi.f32 s12,s2
  000aec2c: vcvt.s32.f32 s2,s10
  000aec30: vmov r2,s2
  000aec34: vcvt.f32.s32 s2,s2
  000aec38: vcvt.s32.f32 s8,s12
  000aec3c: vadd.f32 s14,s4,s2
  000aec40: vcvt.f32.s32 s18,s8
  000aec44: vmov r5,s8
  000aec48: vadd.f32 s8,s4,s0
  000aec4c: vsub.f32 s4,s4,s0
  000aec50: vadd.f32 s3,s6,s18
  000aec54: vsub.f32 s6,s6,s16
  000aec58: vcmpe.f32 s8,s14
  000aec5c: vmrs apsr,fpscr
  000aec60: vcmpe.f32 s1,s3
  000aec64: it gt
  000aec66: vmov.gt.f32 s8,s14
  000aec6a: vmrs apsr,fpscr
  000aec6e: vcmpe.f32 s4,s8
  000aec72: it gt
  000aec74: vmov.gt.f32 s1,s3
  000aec78: vmrs apsr,fpscr
  000aec7c: vcmpe.f32 s6,s1
  000aec80: vcvt.s32.f32 s10,s10
  000aec84: vcvt.s32.f32 s12,s12
  000aec88: ldr r3,[0x000aeee8]
  000aec8a: add r3,pc
  000aec8c: ldr r3,[r3,#0x0]
  000aec8e: it gt
  000aec90: vmov.gt.f32 s8,s4
  000aec94: vmrs apsr,fpscr
  000aec98: it gt
  000aec9a: vmov.gt.f32 s1,s6
  000aec9e: cmp r2,#0x1
  000aeca0: vstr.32 s8,[r3]
  000aeca4: vstr.32 s10,[r0]
  000aeca8: vstr.32 s12,[r1]
  000aecac: vstr.32 s1,[r3,#0x4]
  000aecb0: blt 0x000aecc4
  000aecb2: vdiv.f32 s0,s2,s0
  000aecb6: mov r0,r10
  000aecb8: mov r1,r9
  000aecba: vmov r2,s0
  000aecbe: blx 0x000729d0
  000aecc2: b 0x000aecde
  000aecc4: cmp.w r2,#0xffffffff
  000aecc8: bgt 0x000aecde
  000aecca: vneg.f32 s2,s2
  000aecce: mov r0,r10
  000aecd0: mov r1,r9
  000aecd2: vdiv.f32 s0,s2,s0
  000aecd6: vmov r2,s0
  000aecda: blx 0x000729dc
  000aecde: cmp r5,#0x1
  000aece0: blt 0x000aecf4
  000aece2: vdiv.f32 s0,s18,s16
  000aece6: mov r0,r10
  000aece8: mov r1,r9
  000aecea: vmov r2,s0
  000aecee: blx 0x000729e8
  000aecf2: b 0x000aed0e
  000aecf4: cmp.w r5,#0xffffffff
  000aecf8: bgt 0x000aed0e
  000aecfa: vneg.f32 s0,s18
  000aecfe: mov r0,r10
  000aed00: mov r1,r9
  000aed02: vdiv.f32 s0,s0,s16
  000aed06: vmov r2,s0
  000aed0a: blx 0x000729f4
  000aed0e: add r5,sp,#0x34
  000aed10: add r1,sp,#0xc8
  000aed12: mov r0,r5
  000aed14: blx 0x0006ec80
  000aed18: add r6,sp,#0x40
  000aed1a: ldr r1,[0x000af0ac]
  000aed1c: mov r2,r5
  000aed1e: mov r0,r6
  000aed20: blx 0x00072658
  000aed24: add r5,sp,#0x88
  000aed26: add r1,sp,#0x4c
  000aed28: mov r2,r6
  000aed2a: mov r0,r5
  000aed2c: blx 0x0006f1cc
  000aed30: ldr r1,[0x000af0bc]
  000aed32: ldr r0,[sp,#0x20]
  000aed34: add r1,pc
  000aed36: ldr r6,[r1,#0x0]
  000aed38: mov r1,r5
  000aed3a: ldr r0,[r0,#0x0]
  000aed3c: mov r2,r6
  000aed3e: blx 0x000729c4
  000aed42: vldr.32 s0,[r6,#0x8]
  000aed46: vcmpe.f32 s0,#0
  000aed4a: vmrs apsr,fpscr
  000aed4e: ble 0x000aed6a
  000aed50: vldr.64 d16,[r6]
  000aed54: add r1,sp,#0x40
  000aed56: ldr r0,[r6,#0x8]
  000aed58: str r0,[sp,#0x48]
  000aed5a: mov r0,r6
  000aed5c: vstr.64 d16,[sp,#0x40]
  000aed60: blx 0x0006eb3c
  000aed64: add.w r5,r10,#0xa0
  000aed68: b 0x000aeda2
  000aed6a: add.w r9,sp,#0x34
  000aed6e: ldr r2,[0x000af098]
  000aed70: mov r1,r6
  000aed72: mov r0,r9
  000aed74: blx 0x0006ec74
  000aed78: add.w r5,r10,#0xa0
  000aed7c: add r4,sp,#0x28
  000aed7e: ldr r2,[0x000af0b0]
  000aed80: mov r0,r4
  000aed82: mov r1,r5
  000aed84: blx 0x0006ec74
  000aed88: add.w r11,sp,#0x40
  000aed8c: mov r1,r9
  000aed8e: mov r2,r4
  000aed90: mov r0,r11
  000aed92: blx 0x0006f1cc
  000aed96: mov r1,r11
  000aed98: mov r0,r6
  000aed9a: add.w r11,sp,#0xc8
  000aed9e: blx 0x0006eb3c
  000aeda2: mov r0,r5
  000aeda4: mov r1,r6
  000aeda6: blx 0x0006eb3c
  000aedaa: ldrb.w r0,[r10,#0x84]
  000aedae: cbz r0,0x000aedd8
  000aedb0: ldr.w r0,[r10,#0x88]
  000aedb4: blx 0x00072a00
  000aedb8: cbnz r0,0x000aedd8
  000aedba: ldr.w r1,[r8,#0x0]
  000aedbe: ldr.w r0,[r10,#0x88]
  000aedc2: cbz r1,0x000aedce
  000aedc4: ldr.w r1,[r10,#0x154]
  000aedc8: blx 0x00072a0c
  000aedcc: b 0x000aedd2
  000aedce: blx 0x00072a18
  000aedd2: movs r0,#0x0
  000aedd4: strb.w r0,[r10,#0x84]
  000aedd8: ldr r0,[0x000af0c0]
  000aedda: ldr.w r2,[r8,#0x0]
  000aedde: add r0,pc
  000aede0: cmp r2,#0x0
  000aede2: it ne
  000aede4: mov.ne r2,#0x1
  000aede6: ldr r1,[r0,#0x0]
  000aede8: add.w r0,r10,#0x94
  000aedec: strb.w r2,[r10,#0x378]
  000aedf0: blx 0x0006eb3c
  000aedf4: mov r0,r10
  000aedf6: blx 0x000724b4
  000aedfa: cmp r0,#0x0
  000aedfc: beq.w 0x000aef58
  000aee00: ldr.w r0,[r10,#0x8c]
  000aee04: cmp r0,#0x0
  000aee06: beq.w 0x000aef58
  000aee0a: ldr.w r0,[r10,#0x0]
  000aee0e: movs r1,#0x0
  000aee10: blx 0x00072484
  000aee14: ldr.w r0,[r10,#0x1b4]
  000aee18: cbz r0,0x000aee28
  000aee1a: ldr r0,[0x000af0c4]
  000aee1c: movs r1,#0x0
  000aee1e: add r0,pc
  000aee20: ldr r0,[r0,#0x0]
  000aee22: ldr r0,[r0,#0x0]
  000aee24: blx 0x000724a8
  000aee28: ldr.w r0,[r10,#0x1b8]
  000aee2c: cbz r0,0x000aee3c
  000aee2e: ldr r0,[0x000af0c8]
  000aee30: movs r1,#0x0
  000aee32: add r0,pc
  000aee34: ldr r0,[r0,#0x0]
  000aee36: ldr r0,[r0,#0x0]
  000aee38: blx 0x000724a8
  000aee3c: ldrb.w r0,[r10,#0x356]
  000aee40: cbz r0,0x000aee4a
  000aee42: ldr.w r0,[r10,#0x1c4]
  000aee46: cmp r0,#0x1
  000aee48: beq 0x000aee58
  000aee4a: ldr r1,[0x000af0b4]
  000aee4c: ldr.w r0,[r10,#0x4]
  000aee50: mov r2,r1
  000aee52: mov r3,r1
  000aee54: blx 0x00072160
  000aee58: ldr.w r0,[r10,#0x2f8]
  000aee5c: movw r1,#0xbb7
  000aee60: cmp r0,r1
  000aee62: bgt 0x000aef00
  000aee64: ldr.w r1,[r10,#0x134]
  000aee68: add r0,r1
  000aee6a: movw r1,#0xbb8
  000aee6e: cmp r0,r1
  000aee70: blt 0x000aef28
  000aee72: add r6,sp,#0x88
  000aee74: ldr.w r1,[r10,#0x8]
  000aee78: ldr.w r5,[r10,#0x8c]
  000aee7c: mov r0,r6
  000aee7e: blx 0x000720b8
  000aee82: movs r0,#0x0
  000aee84: add r2,sp,#0x40
  000aee86: strd r0,r0,[sp,#0x40]
  000aee8a: mov r1,r6
  000aee8c: str r0,[sp,#0x48]
  000aee8e: mov r0,r5
  000aee90: blx 0x00072808
  000aee94: ldr.w r0,[r10,#0xc]
  000aee98: movs r2,#0x0
  000aee9a: ldr.w r1,[r10,#0x2fc]
  000aee9e: ldr r0,[r0,#0x74]
  000aeea0: blx 0x000723c4
  000aeea4: ldr.w r0,[r10,#0xc]
  000aeea8: movs r2,#0x0
  000aeeaa: ldr.w r1,[r10,#0x2fc]
  000aeeae: ldr r0,[r0,#0x74]
  000aeeb0: blx 0x00072a24
  000aeeb4: ldr.w r0,[r10,#0xc]
  000aeeb8: movs r3,#0x0
  000aeeba: ldr r2,[0x000af0b8]
  000aeebc: ldr r1,[r0,#0x3c]
  000aeebe: ldr r0,[r0,#0x74]
  000aeec0: strd r11,r2,[sp,#0x0]
  000aeec4: add r2,sp,#0x4c
  000aeec6: blx 0x00072a30
  000aeeca: b 0x000aef28
  000aef00: movw r1,#0xbb8
  000aef04: cmp r0,r1
  000aef06: beq 0x000aef28
  000aef08: add.w r8,sp,#0x88
  000aef0c: ldr.w r1,[r10,#0x8]
  000aef10: ldr.w r5,[r10,#0x8c]
  000aef14: mov r0,r8
  000aef16: ldr.w r6,[r10,#0x134]
  000aef1a: blx 0x000720b8
  000aef1e: mov r0,r5
  000aef20: mov r1,r6
  000aef22: mov r2,r8
  000aef24: blx 0x00072a3c
  000aef28: ldr.w r5,[r10,#0x90]
  000aef2c: cbz r5,0x000aef4a
  000aef2e: add.w r8,sp,#0x88
  000aef32: ldr.w r1,[r10,#0x8]
  000aef36: ldr.w r6,[r10,#0x134]
  000aef3a: mov r0,r8
  000aef3c: blx 0x000720b8
  000aef40: mov r0,r5
  000aef42: mov r1,r6
  000aef44: mov r2,r8
  000aef46: blx 0x00072a3c
  000aef4a: ldr.w r0,[r10,#0x134]
  000aef4e: ldr.w r1,[r10,#0x2f8]
  000aef52: add r0,r1
  000aef54: str.w r0,[r10,#0x2f8]
  000aef58: ldrb.w r0,[r10,#0x170]
  000aef5c: cbz r0,0x000aef92
  000aef5e: ldr.w r0,[r10,#0x8]
  000aef62: ldr r4,[sp,#0x20]
  000aef64: ldr.w r5,[r10,#0x2c]
  000aef68: ldr r1,[r0,#0xc]
  000aef6a: ldr r0,[r4,#0x0]
  000aef6c: blx 0x000720c4
  000aef70: ldr.w r1,[r10,#0x4]
  000aef74: mov r6,r0
  000aef76: ldr r0,[r4,#0x0]
  000aef78: ldr r1,[r1,#0xc]
  000aef7a: blx 0x000720c4
  000aef7e: add r4,sp,#0x88
  000aef80: mov r2,r0
  000aef82: mov r1,r6
  000aef84: mov r0,r4
  000aef86: blx 0x0006f82c
  000aef8a: mov r0,r5
  000aef8c: mov r1,r4
  000aef8e: blx 0x000721e4
  000aef92: ldr r1,[sp,#0x14]
  000aef94: ldr r0,[sp,#0xd4]
  000aef96: ldr r1,[r1,#0x0]
  000aef98: subs r0,r1,r0
  000aef9a: itttt eq
  000aef9c: add.eq sp,#0xd8
  000aef9e: vpop.eq {d8,d9,d10,d11}
  000aefa2: add.eq sp,#0x4
  000aefa4: pop.eq.w {r8,r9,r10,r11}
  000aefa8: it eq
  000aefaa: pop.eq {r4,r5,r6,r7,pc}
  000aefac: blx 0x0006e824
  000aefb0: movs r5,#0x0
  000aefb2: movs r6,#0x0
  000aefb4: b 0x000aefcc
  000aefb6: ldr.w r0,[r10,#0xc]
  000aefba: mov r1,r5
  000aefbc: ldr.w r4,[r10,#0x1bc]
  000aefc0: blx 0x00072a48
  000aefc4: cmp r4,r0
  000aefc6: it eq
  000aefc8: mov.eq r6,r5
  000aefca: adds r5,#0x1
  000aefcc: ldr.w r0,[r10,#0xc]
  000aefd0: blx 0x00072a54
  000aefd4: cmp r5,r0
  000aefd6: blt 0x000aefb6
  000aefd8: ldr r0,[sp,#0x24]
  000aefda: ldr r0,[r0,#0x0]
  000aefdc: blx 0x00071770
  000aefe0: mov r8,r0
  000aefe2: mov.w r0,#0x140
  000aefe6: blx 0x0006eb24
  000aefea: mov r5,r0
  000aefec: mov.w r0,#0xffffffff
  000aeff0: movs r2,#0x4
  000aeff2: strd r0,r6,[sp,#0x0]
  000aeff6: cmp.w r8,#0x5b
  000aeffa: it eq
  000aeffc: mov.eq r2,#0x1
  000aeffe: mov r0,r5
  000af000: movs r1,#0x0
  000af002: mov.w r3,#0xffffffff
  000af006: blx 0x00072a60
  000af00a: ldr r0,[sp,#0x18]
  000af00c: movs r1,#0x1
  000af00e: str.w r5,[r10,#0x1e8]
  000af012: blx 0x0007270c
  000af016: b.w 0x000ae040
  000af01a: mov r0,r5
  000af01c: blx 0x00072874
  000af020: cmp r0,#0xf
  000af022: bne.w 0x000ae040
  000af026: ldr r0,[sp,#0x24]
  000af028: ldr r0,[r0,#0x0]
  000af02a: blx 0x00071a58
  000af02e: mov r8,r0
  000af030: mov r0,r5
  000af032: blx 0x000728a4
  000af036: mov r1,r0
  000af038: ldr.w r0,[r10,#0x364]
  000af03c: subs r2,r0,r6
  000af03e: mov r0,r8
  000af040: blx 0x00072454
  000af044: ldr.w r1,[r10,#0x364]
  000af048: ldr.w r0,[r10,#0xc]
  000af04c: subs r1,r1,r6
  000af04e: blx 0x00072a6c
  000af052: b.w 0x000ae040
  000af056: ldr r0,[sp,#0x18]
  000af058: movs r1,#0x28
  000af05a: mov r2,r10
  000af05c: movs r3,#0x0
  000af05e: blx 0x00072418
  000af062: ldr.w r0,[r10,#0x360]
  000af066: str.w r0,[r10,#0x364]
  000af06a: cmp r0,r6
  000af06c: ble.w 0x000ae040
  000af070: ldr r1,[sp,#0x24]
  000af072: subs r0,r0,r6
  000af074: ldr r1,[r1,#0x0]
  000af076: ldr.w r2,[r1,#0x178]
  000af07a: add r0,r2
  000af07c: str.w r0,[r1,#0x178]
  000af080: b.w 0x000ae040
```
