# Level::createSpace
elf 0xae5b8 body 2458
Sig: undefined __thiscall createSpace(Level * this)

## decompile
```c

/* Level::createSpace() */

void __thiscall Level::createSpace(Level *this)

{
  char cVar1;
  Level *pLVar2;
  char cVar3;
  undefined8 uVar4;
  byte bVar5;
  ushort uVar6;
  short sVar7;
  int iVar8;
  Engine *this_00;
  StarSystem *this_01;
  float *pfVar9;
  Station *pSVar10;
  Array<KIPlayer*> *this_02;
  PlayerStation *this_03;
  void *pvVar11;
  Agent *pAVar12;
  int iVar13;
  int *piVar14;
  AEGeometry *this_04;
  PlayerJumpgate *this_05;
  undefined4 uVar15;
  Vector *pVVar16;
  short extraout_r1;
  int extraout_r1_00;
  uint *puVar17;
  undefined4 extraout_r1_01;
  int *piVar18;
  undefined4 extraout_r1_02;
  int iVar19;
  int extraout_r1_03;
  float *pfVar20;
  int *piVar21;
  int *piVar22;
  code *pcVar23;
  undefined4 uVar24;
  float *pfVar25;
  Galaxy *this_06;
  ScrollTouchWindow *pSVar26;
  PaintCanvas *pPVar27;
  uint uVar28;
  undefined4 *puVar29;
  uint uVar30;
  uint *puVar31;
  Agent *pAVar32;
  int *piVar33;
  uint in_fpscr;
  float fVar34;
  float fVar35;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float fVar36;
  float fVar37;
  float fVar38;
  int unaff_s29;
  undefined4 uVar39;
  undefined4 uVar40;
  longlong lVar41;
  undefined8 uVar42;
  uint local_108 [3];
  undefined4 local_fc;
  uint local_f8;
  uint local_f4 [3];
  String aSStack_e8 [12];
  String aSStack_dc [36];
  uint local_b8;
  undefined4 local_b4;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined4 local_80;
  String aSStack_7c [4];
  undefined8 local_78;
  float local_70 [2];
  ushort local_68;
  undefined2 local_66;
  int local_64;
  
  piVar33 = *(int **)(DAT_000be974 + 0xbe5d4);
  local_64 = *piVar33;
  pLVar2 = this + 4;
  if (*(uint *)pLVar2 == 0xffffffff) {
    puVar29 = *(undefined4 **)(DAT_000be978 + 0xbe5ec);
    iVar8 = Status::inAlienOrbit();
    puVar31 = *(uint **)(DAT_000be97c + 0xbe5fe);
    pPVar27 = (PaintCanvas *)*puVar31;
    cVar3 = (char)this;
    if (iVar8 == 0) {
      Status::getSystem();
      uVar15 = SolarSystem::getIndex();
      __aeabi_idivmod(uVar15,3);
      AbyssEngine::PaintCanvas::MeshCreate(pPVar27,extraout_r1 + 0x45ba,(uint *)(this + 8),false);
      uVar30 = *puVar31;
      Status::getSystem();
      uVar15 = SolarSystem::getIndex();
      __aeabi_idivmod(uVar15,3);
      AbyssEngine::PaintCanvas::TextureCreate
                ((ushort)uVar30,(uint *)(extraout_r1_00 + 0x2766U & 0xffff),(bool)(cVar3 + -100));
      iVar8 = Status::getCurrentCampaignMission();
      if ((iVar8 == 0) && (*(int *)(this + 0xc0) == 3)) {
        AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar31,0x458b,(uint *)pLVar2,false);
        uVar6 = (ushort)*puVar31;
        cVar1 = -0x68;
        puVar17 = (uint *)0x2754;
LAB_000be904:
        AbyssEngine::PaintCanvas::TextureCreate(uVar6,puVar17,(bool)(cVar3 + cVar1));
      }
      else {
        iVar8 = Status::inSupernovaSystem();
        if (iVar8 == 0) {
          pPVar27 = (PaintCanvas *)*puVar31;
          Status::getSystem();
          sVar7 = SolarSystem::getTextureIndex();
          AbyssEngine::PaintCanvas::MeshCreate(pPVar27,sVar7 + 0x4588,(uint *)pLVar2,false);
          uVar6 = (ushort)*puVar31;
          Status::getSystem();
          iVar8 = SolarSystem::getTextureIndex();
          puVar17 = (uint *)(iVar8 + 0x2751U & 0xffff);
        }
        else {
          iVar8 = Status::getCurrentCampaignMission();
          if (iVar8 == 0x59) {
            AbyssEngine::PaintCanvas::MeshCreate
                      ((PaintCanvas *)*puVar31,0x458d,(uint *)pLVar2,false);
            uVar6 = (ushort)*puVar31;
            puVar17 = (uint *)0x2756;
          }
          else {
            iVar8 = Status::getCurrentCampaignMission();
            if (iVar8 < 0x9e) {
              AbyssEngine::PaintCanvas::MeshCreate
                        ((PaintCanvas *)*puVar31,0x45a0,(uint *)(this + 0xc),false);
              AbyssEngine::PaintCanvas::TransformCreate
                        ((PaintCanvas *)*puVar31,(uint *)(this + 0x10));
              AbyssEngine::PaintCanvas::TransformAddMeshId
                        ((PaintCanvas *)*puVar31,*(uint *)(this + 0x10),*(uint *)(this + 0xc));
              lVar41 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar31);
              AbyssEngine::Transform::Update(lVar41,true);
              AbyssEngine::PaintCanvas::MeshCreate
                        ((PaintCanvas *)*puVar31,0x45a1,(uint *)(this + 0x14),false);
              AbyssEngine::PaintCanvas::TransformCreate
                        ((PaintCanvas *)*puVar31,(uint *)(this + 0x18));
              AbyssEngine::PaintCanvas::TransformAddMeshId
                        ((PaintCanvas *)*puVar31,*(uint *)(this + 0x18),*(uint *)(this + 0x14));
              lVar41 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar31);
              AbyssEngine::Transform::Update(lVar41,true);
              iVar8 = Status::getCurrentCampaignMission();
              if (iVar8 < 0x6a) {
                puVar17 = (uint *)0x2764;
              }
              else {
                puVar17 = (uint *)0x2765;
              }
              AbyssEngine::PaintCanvas::TextureCreate
                        ((ushort)*puVar31,puVar17,(bool)(cVar3 + -0x60));
            }
            AbyssEngine::PaintCanvas::MeshCreate
                      ((PaintCanvas *)*puVar31,0x4597,(uint *)pLVar2,false);
            uVar6 = (ushort)*puVar31;
            puVar17 = (uint *)0x2760;
          }
        }
        AbyssEngine::PaintCanvas::TextureCreate(uVar6,puVar17,(bool)(cVar3 + -0x68));
        Status::getSystem();
        uVar30 = SolarSystem::getIndex();
        Status::getSystem();
        iVar8 = SolarSystem::getIndex();
        if ((uVar30 | 2) == 0x1a || iVar8 == 0x19) {
          this[0x289] = (Level)0x1;
          AbyssEngine::PaintCanvas::MeshCreate
                    ((PaintCanvas *)*puVar31,0x3827,(uint *)(this + 0x1cc),false);
          AbyssEngine::PaintCanvas::TextureCreate
                    ((ushort)*puVar31,(uint *)0x8282,(bool)(cVar3 + -0x3c));
          AbyssEngine::PaintCanvas::TextureCreate
                    ((ushort)*puVar31,(uint *)(DAT_000be980 + 1U & 0xffff),(bool)(cVar3 + -0x38));
        }
        iVar8 = Status::inPlanetRingOrbit();
        if (iVar8 != 0) {
          AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar31,0x499f,&local_b8,false);
          AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar31,(uint *)(this + 0x1b4));
          AbyssEngine::PaintCanvas::TransformAddMeshId
                    ((PaintCanvas *)*puVar31,*(uint *)(this + 0x1b4),local_b8);
          AbyssEngine::PaintCanvas::TextureCreate
                    ((ushort)*puVar31,(uint *)0x715a,(bool)(cVar3 + -0x48));
        }
        iVar8 = Status::inStormOrbit((Status *)*puVar29);
        if (iVar8 != 0) {
          AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar31,0x49a0,&local_b8,false);
          AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar31,(uint *)(this + 0x1bc));
          AbyssEngine::PaintCanvas::TransformAddMeshId
                    ((PaintCanvas *)*puVar31,*(uint *)(this + 0x1bc),local_b8);
          uVar6 = (ushort)*puVar31;
          cVar1 = -0x40;
          puVar17 = (uint *)0x715b;
          goto LAB_000be904;
        }
      }
      Status::getSystem();
      iVar8 = SolarSystem::getTextureIndex();
      if (0xf < iVar8) {
        this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        iVar8 = AbyssEngine::Engine::IsPostEffectActivated(this_00);
        if (iVar8 != 0) {
          iVar8 = AbyssEngine::PaintCanvas::MeshGetPointer((PaintCanvas *)*puVar31,*(uint *)pLVar2);
          *(undefined4 *)(iVar8 + 0x1c) = DAT_000be988;
        }
      }
    }
    else {
      AbyssEngine::PaintCanvas::MeshCreate(pPVar27,0x45bc,(uint *)(this + 8),false);
      AbyssEngine::PaintCanvas::TextureCreate((ushort)*puVar31,(uint *)0x2768,(bool)(cVar3 + -100));
      AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar31,0x4592,(uint *)pLVar2,false);
      AbyssEngine::PaintCanvas::TextureCreate((ushort)*puVar31,(uint *)0x275b,(bool)(cVar3 + -0x68))
      ;
    }
    iVar8 = Status::inFogSkyboxOrbit();
    if (iVar8 == 0) {
      puVar29 = *(undefined4 **)(DAT_000be98c + 0xbe956);
      uVar24 = *puVar29;
      uVar42 = Status::getStation();
      uVar15 = (undefined4)((ulonglong)uVar42 >> 0x20);
      if ((int)uVar42 != 0) {
        pSVar10 = (Station *)Status::getStation();
        Station::getIndex(pSVar10);
        uVar15 = extraout_r1_01;
      }
      AbyssEngine::AERandom::setSeed(CONCAT44(uVar15,uVar24));
      pcVar23 = *(code **)(DAT_000bece4 + 0xbe9a6);
      uVar15 = (*pcVar23)(*puVar29,0x10000);
      fVar38 = DAT_000becec;
      fVar34 = DAT_000bece8;
      fVar35 = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x1a4) = fVar35 * DAT_000bece8 * DAT_000becec;
      uVar15 = (*pcVar23)(*puVar29,0x10000);
      fVar35 = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x1a8) = fVar35 * fVar34 * fVar38;
      uVar15 = (*pcVar23)(*puVar29,0x10000);
      fVar35 = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x1ac) = fVar35 * fVar34 * fVar38;
      AbyssEngine::AERandom::reset();
    }
    else {
      *(undefined4 *)(this + 0x1a4) = 0;
      *(undefined4 *)(this + 0x1a8) = 0;
      *(undefined4 *)(this + 0x1ac) = 0;
    }
  }
  if (*(int *)(this + 0xc0) == 4 || *(int *)(this + 0xc0) == 0x17) {
    this_01 = operator_new(0x60);
    StarSystem::StarSystem(this_01,*(int *)(this + 0xc0));
    *(StarSystem **)(this + 0xec) = this_01;
    puVar29 = (undefined4 *)Status::getSystem();
    fVar38 = DAT_000bed08;
    fVar34 = DAT_000bed04;
    if (puVar29 == (undefined4 *)0x0) {
      fVar35 = 10.0;
      fVar37 = (float)((int)DAT_000bed04 + 0x1e80000);
      pfVar9 = *(float **)(DAT_000bed00 + 0xbea7a);
      **(float **)(DAT_000becfc + 0xbea76) = DAT_000bed04;
      fVar36 = 10.0;
      *pfVar9 = fVar38;
    }
    else {
      fVar34 = (float)VectorSignedToFloat(*puVar29,(byte)(in_fpscr >> 0x16) & 3);
      **(float **)(DAT_000becf4 + 0xbea42) = fVar34;
      fVar37 = (float)VectorSignedToFloat(puVar29[1],(byte)(in_fpscr >> 0x16) & 3);
      **(float **)(DAT_000becf8 + 0xbea58) = fVar37;
      fVar36 = (float)VectorSignedToFloat(puVar29[2],(byte)(in_fpscr >> 0x16) & 3);
      fVar35 = fVar34;
      fVar38 = fVar37;
    }
    pfVar9 = *(float **)(DAT_000bed0c + 0xbea9e);
    piVar21 = *(int **)(DAT_000bed10 + 0xbeaa2);
    piVar22 = *(int **)(DAT_000bed14 + 0xbeaa6);
    pfVar25 = *(float **)(DAT_000bed18 + 0xbeaaa);
    pfVar20 = *(float **)(DAT_000bed20 + 0xbeab4);
    piVar18 = *(int **)(DAT_000bed24 + 0xbeab6);
    **(float **)(DAT_000bed1c + 0xbeaae) = fVar36;
    *pfVar20 = fVar34;
    *pfVar25 = fVar37;
    *pfVar9 = fVar36;
    *piVar21 = (int)(fVar35 / 3.0);
    *piVar22 = (int)(fVar38 / 3.0);
    *piVar18 = (int)(fVar36 / 3.0);
    if (*piVar33 != local_64) {
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    return;
  }
  if (**(char **)(DAT_000bed28 + 0xbeb1a) == '\0') {
    pSVar10 = (Station *)Status::getStation();
    iVar8 = Station::isAttackedByAliens(pSVar10);
    if (iVar8 != 0) goto LAB_000beb34;
    bVar5 = Status::inAlienOrbit();
  }
  else {
LAB_000beb34:
    bVar5 = 1;
  }
  pAVar32 = *(Agent **)(DAT_000bed30 + 0xbeb3c);
  iVar8 = Status::getCurrentCampaignMission();
  if ((bVar5 & iVar8 < 0x2b) == 1) {
    Globals::addSoundResourceToList(**(int **)(DAT_000bed34 + 0xbeb60));
  }
  this_02 = operator_new(0xc);
  Array<KIPlayer*>::Array(this_02);
  *(Array<KIPlayer*> **)(this + 0x100) = this_02;
  ArraySetLength<KIPlayer*>(4,(Array *)this_02);
  iVar8 = Status::inEmptyOrbit(*(Status **)pAVar32);
  if (iVar8 != 0) {
    iVar8 = Status::inAlienOrbit();
    if (iVar8 != 0) {
LAB_000beb92:
      **(undefined4 **)(*(int *)(this + 0x100) + 4) = 0;
      goto LAB_000bec4a;
    }
    pSVar10 = (Station *)Status::getStation();
    iVar8 = Station::getIndex(pSVar10);
    if (iVar8 != 0x1b) {
      pSVar10 = (Station *)Status::getStation();
      iVar8 = Station::getIndex(pSVar10);
      if (iVar8 != 0x6e) {
        pSVar10 = (Station *)Status::getStation();
        iVar8 = Station::getIndex(pSVar10);
        if (iVar8 != 0x6f) goto LAB_000beb92;
      }
    }
  }
  this_03 = operator_new(0x174);
  pSVar10 = (Station *)Status::getStation();
  PlayerStation::PlayerStation(this_03,pSVar10);
  **(undefined4 **)(*(int *)(this + 0x100) + 4) = this_03;
  LODManager::addObject
            (*(LODManager **)this,*(AEGeometry **)(**(int **)(*(int *)(this + 0x100) + 4) + 0x140));
  iVar8 = Status::dlc1Won();
  if (iVar8 != 0) {
    pSVar10 = (Station *)Galaxy::getStation(**(int **)(DAT_000bed38 + 0xbec16));
    Station::getName();
    AbyssEngine::String::operator=
              ((String *)(**(int **)(*(int *)(this + 0x100) + 4) + 0x18),(String *)&local_b8);
    AbyssEngine::String::~String((String *)&local_b8);
    if (pSVar10 != (Station *)0x0) {
      pvVar11 = (void *)Station::~Station(pSVar10);
      operator_delete(pvVar11);
    }
  }
LAB_000bec4a:
  local_78 = *(ulonglong *)(this + 0x18c);
  local_70[0] = *(float *)(this + 0x194);
  piVar18 = *(int **)(DAT_000bed3c + 0xbec54);
  *(undefined4 *)(this + 0x138) = 0;
  iVar8 = *piVar18;
  uVar42 = Status::getStation();
  uVar15 = (undefined4)((ulonglong)uVar42 >> 0x20);
  if ((int)uVar42 != 0) {
    pSVar10 = (Station *)Status::getStation();
    Station::getIndex(pSVar10);
    uVar15 = extraout_r1_02;
  }
  AbyssEngine::AERandom::setSeed(CONCAT44(uVar15,iVar8));
  uVar4 = uRam000bf190;
  uVar42 = uRam000bf188;
  fVar38 = DAT_000becec;
  fVar34 = DAT_000bece8;
  pAVar12 = (Agent *)((uint)&local_b8 | 4);
  uVar15 = 0;
  uVar24 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar39 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar40 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  uVar30 = 1;
  piVar22 = (int *)(DAT_000bed44 + 0xbecce);
  puVar31 = *(uint **)(DAT_000bed40 + 0xbecc8);
  piVar21 = piVar33;
  do {
    if (2 < uVar30) {
      AbyssEngine::AERandom::reset();
      AbyssEngine::String::String((String *)local_70,(char *)(extraout_r1_03 + 0x17cea0),false);
      iVar8 = *piVar22;
      Agent::getIndex(pAVar32);
      GameText::getText(iVar8);
      AbyssEngine::operator+((String *)&local_64,(String *)local_70);
      AbyssEngine::String::operator+=((String *)&stack0xffffffc0,(String *)&local_64);
      AbyssEngine::String::~String((String *)&local_64);
      AbyssEngine::String::~String((String *)local_70);
      AbyssEngine::String::String((String *)local_70,(char *)(DAT_0017d0e8 + 0x17ceea),false);
      GameText::getText(*piVar22);
      AbyssEngine::operator+((String *)&local_64,(String *)local_70);
      AbyssEngine::String::operator+=((String *)&stack0xffffffc0,(String *)&local_64);
      AbyssEngine::String::~String((String *)&local_64);
      AbyssEngine::String::~String((String *)local_70);
      this_06 = (Galaxy *)**(undefined4 **)(DAT_0017d0ec + 0x17cf2c);
      iVar8 = Agent::getSellSystemIndex(pAVar32);
      Galaxy::getSystem(this_06,iVar8);
      iVar8 = *piVar21;
      AbyssEngine::String::String((String *)&stack0xffffffb4,&stack0xffffffc0,false);
      SolarSystem::getName();
      AbyssEngine::String::String((String *)&stack0xffffffa8,(String *)local_70,false);
      uVar15 = AbyssEngine::String::String
                         ((String *)&local_64,(char *)(DAT_0017d0f0 + 0x17cf6a),false);
      Status::replaceHash(&local_64,iVar8,&stack0xffffffb4,&stack0xffffffa8,uVar15);
      AbyssEngine::String::operator=((String *)&stack0xffffffc0,(String *)&local_64);
      pcVar23 = *(code **)(DAT_0017d0f4 + 0x17cf92);
      (*pcVar23)(&local_64);
      (*pcVar23)(&local_64);
      (*pcVar23)(&stack0xffffffa8);
      (*pcVar23)(local_70);
      (*pcVar23)(&stack0xffffffb4);
      iVar8 = *piVar21;
      AbyssEngine::String::String((String *)local_70,&stack0xffffffc0,false);
      Agent::getSellItemPrice(pAVar32);
      Layout::formatCredits((int)local_70);
      AbyssEngine::String::String(aSStack_7c,(String *)local_70,false);
      uVar15 = AbyssEngine::String::String
                         ((String *)&uStack_88,(char *)(DAT_0017d0fc + 0x17cfe6),false);
      Status::replaceHash(&local_64,iVar8,local_70,aSStack_7c,uVar15);
      AbyssEngine::String::operator=((String *)&stack0xffffffc0,(String *)&local_64);
      (*pcVar23)(&local_64);
      (*pcVar23)(&uStack_88);
      (*pcVar23)(aSStack_7c);
      (*pcVar23)(local_70);
      (*pcVar23)(local_70);
      AbyssEngine::String::String((String *)local_70,(char *)(DAT_0017d7ac + 0x17d542),false);
      iVar8 = **(int **)(DAT_0017d7b0 + 0x17d552);
      AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d7b4 + 0x17d554));
      GameText::getText(iVar8);
      AbyssEngine::operator+((String *)&local_64,(String *)local_70);
      AbyssEngine::String::operator+=((String *)&stack0xffffffc0,(String *)&local_64);
      AbyssEngine::String::~String((String *)&local_64);
      AbyssEngine::String::~String((String *)local_70);
      pSVar26 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_e8,&stack0xffffffc0,false);
      ScrollTouchWindow::setText(pSVar26,aSStack_dc,aSStack_e8);
      AbyssEngine::String::~String(aSStack_e8);
      AbyssEngine::String::~String(aSStack_dc);
      pAVar32 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
      AbyssEngine::String::String((String *)local_f4,&stack0xffffffc0,false);
      Agent::setMissionString(pAVar32,local_f4);
      AbyssEngine::String::~String((String *)local_f4);
      *(undefined4 *)(this + 0x14) = 2;
      *(undefined4 *)(this + 0x2c) = 0;
      pVVar16 = (Vector *)SpaceLounge::getSoundId((SpaceLounge *)this,pAVar12);
      if (-1 < (int)pVVar16) {
        FModSound::play(**(int **)(DAT_0017d7b8 + 0x17d60a),pVVar16,(Vector *)0x0,extraout_s0);
      }
      AbyssEngine::String::~String((String *)&stack0xffffffc0);
      if (*piVar33 != unaff_s29) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
    if (uVar30 == 1) {
      iVar8 = Status::getSystem();
      if (iVar8 != 0) {
        Status::getSystem();
        iVar8 = SolarSystem::currentOrbitHasWarpGate();
        if (iVar8 != 0) goto LAB_000bed6a;
      }
      piVar14 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 4);
      if (piVar14 != (int *)0x0) {
        (**(code **)(*piVar14 + 4))();
        *(undefined4 *)(*(int *)(*(int *)(this + 0x100) + 4) + 4) = 0;
      }
    }
    else {
LAB_000bed6a:
      iVar8 = Status::getStation();
      if (iVar8 != 0) {
        iVar8 = AbyssEngine::AERandom::nextInt(*piVar18);
        iVar13 = AbyssEngine::AERandom::nextInt(*piVar18);
        iVar19 = iVar13 + 0xfa;
        if (iVar8 == 0) {
          iVar19 = DAT_000bf168 - iVar13;
        }
        iVar8 = *(int *)(this + 0x138) + iVar19 * 0x10;
        pfVar9 = (float *)&DAT_000bf1a4;
        fVar35 = (float)VectorSignedToFloat(iVar8,(byte)(in_fpscr >> 0x16) & 3);
        if (uVar30 == 1) {
          pfVar9 = (float *)&DAT_000bf1a8;
        }
        fVar37 = *pfVar9;
        *(int *)(this + 0x138) = iVar8;
        *(float *)(this + 0x194) = fVar37 + fVar35 * 3.0;
        local_b8 = 0x3f800000;
        *(undefined4 *)pAVar12 = uVar15;
        *(undefined4 *)(pAVar12 + 4) = uVar24;
        *(undefined4 *)(pAVar12 + 8) = uVar39;
        *(undefined4 *)(pAVar12 + 0xc) = uVar40;
        local_a4 = 0x3f800000;
        uStack_90 = uVar42;
        uStack_88 = uVar4;
        local_80 = 0x3f800000;
        local_a0 = uVar15;
        uStack_9c = uVar24;
        uStack_98 = uVar39;
        uStack_94 = uVar40;
        AbyssEngine::AEMath::MatrixSetRotation
                  ((Matrix *)local_f4,fVar35,extraout_s1,fVar35 * fVar34 * fVar38);
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)local_f4,(Vector *)&local_b8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_78,(Vector *)local_f4);
        local_78 = local_78 & 0xffffffff;
      }
      iVar8 = Status::inAlienOrbit();
      if (iVar8 == 0) {
        Status::getSystem();
        piVar21 = (int *)SolarSystem::getRace();
      }
      else {
        piVar21 = (int *)0x0;
      }
      this_04 = operator_new(0xc0);
      AEGeometry::AEGeometry
                (this_04,*(ushort *)(piVar22 + (int)piVar21 * 2),(PaintCanvas *)*puVar31,false);
      uVar28 = (int)piVar21 << 2;
      local_f4[0] = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar31,local_f4);
      AbyssEngine::PaintCanvas::TransformAddMesh
                (*puVar31,(ushort)local_f4[0],
                 SUB21(*(undefined2 *)((int)piVar22 + (uVar28 | 1) * 2),0));
      local_108[0] = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar31,local_108);
      AbyssEngine::PaintCanvas::TransformAddMesh
                (*puVar31,(ushort)local_108[0],
                 SUB21(*(undefined2 *)((int)piVar22 + (uVar28 | 2) * 2),0));
      AEGeometry::addChild((uint)this_04);
      AEGeometry::addChild((uint)this_04);
      this_05 = operator_new(0x148);
      PlayerJumpgate::PlayerJumpgate
                (this_05,0xf,this_04,local_78._4_4_,extraout_s1_00,local_70[0],SUB81(local_78,0));
      *(PlayerJumpgate **)(*(int *)(*(int *)(this + 0x100) + 4) + uVar30 * 4) = this_05;
      if (uVar30 == 2) {
        *(undefined4 *)(*(int *)(*(int *)(*(int *)(*(int *)(this + 0x100) + 4) + 8) + 4) + 0x40) = 0
        ;
      }
      local_f8 = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar31,&local_f8);
      AbyssEngine::PaintCanvas::TransformAddMesh
                (*puVar31,(ushort)local_f8,SUB21(*(undefined2 *)((int)piVar22 + (uVar28 | 3) * 2),0)
                );
      PlayerJumpgate::addJumpAnimationHandle
                (*(PlayerJumpgate **)(*(int *)(*(int *)(this + 0x100) + 4) + uVar30 * 4),local_f8);
      local_66 = *(undefined2 *)(iRam000bf1b8 + 0xbef52 + ((int)piVar21 << 2 | 2U));
      local_68 = *(ushort *)(iRam000bf1b8 + 0xbef52 + (int)piVar21 * 4);
      local_b4 = uRam000bf1ac;
      local_b8 = 40000;
      AEGeometry::setLodMeshes
                (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0x100) + 4) + uVar30 * 4) + 8),
                 &local_68,(int *)&local_b8,2);
      LODManager::addObject
                (*(LODManager **)this,
                 *(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0x100) + 4) + uVar30 * 4) + 8));
      if (piVar21 == (int *)0x1) {
        local_fc = uRam000bf1b0;
        AEGeometry::setLodChildMeshes
                  (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0x100) + 4) + uVar30 * 4) + 8)
                   ,(ushort *)&local_fc);
      }
    }
    uVar30 = uVar30 + 1;
  } while( true );
}

```
## target disasm
```
  000be5b8: push {r4,r5,r6,r7,lr}
  000be5ba: add r7,sp,#0xc
  000be5bc: push {r7,r8,r9,r10,r11}
  000be5c0: vpush {d12,d13,d14,d15}
  000be5c4: vpush {d8,d9,d10}
  000be5c8: sub sp,#0xe0
  000be5ca: mov r11,r0
  000be5cc: ldr r0,[0x000be974]
  000be5ce: mov r5,r11
  000be5d0: add r0,pc
  000be5d2: ldr.w r10,[r0,#0x0]
  000be5d6: ldr.w r0,[r10,#0x0]
  000be5da: str r0,[sp,#0xdc]
  000be5dc: ldr.w r0,[r5,#0x4]!
  000be5e0: adds r0,#0x1
  000be5e2: bne.w 0x000bea04
  000be5e6: ldr r0,[0x000be978]
  000be5e8: add r0,pc
  000be5ea: ldr.w r8,[r0,#0x0]
  000be5ee: ldr.w r0,[r8,#0x0]
  000be5f2: blx 0x000723d0
  000be5f6: ldr r1,[0x000be97c]
  000be5f8: cmp r0,#0x0
  000be5fa: add r1,pc
  000be5fc: ldr.w r9,[r1,#0x0]
  000be600: ldr.w r6,[r9,#0x0]
  000be604: beq 0x000be64c
  000be606: add.w r2,r11,#0x8
  000be60a: mov r0,r6
  000be60c: movw r1,#0x45bc
  000be610: movs r3,#0x0
  000be612: blx 0x0006fcf4
  000be616: ldr.w r0,[r9,#0x0]
  000be61a: add.w r2,r11,#0x19c
  000be61e: movw r1,#0x2768
  000be622: movs r3,#0x0
  000be624: blx 0x0006fbb0
  000be628: ldr.w r0,[r9,#0x0]
  000be62c: movw r1,#0x4592
  000be630: mov r2,r5
  000be632: movs r3,#0x0
  000be634: blx 0x0006fcf4
  000be638: ldr.w r0,[r9,#0x0]
  000be63c: add.w r2,r11,#0x198
  000be640: movw r1,#0x275b
  000be644: movs r3,#0x0
  000be646: blx 0x0006fbb0
  000be64a: b 0x000be93a
  000be64c: ldr.w r0,[r8,#0x0]
  000be650: blx 0x00071a10
  000be654: blx 0x00071a7c
  000be658: movs r1,#0x3
  000be65a: blx 0x0006f514
  000be65e: movw r0,#0x45ba
  000be662: add.w r2,r11,#0x8
  000be666: add r0,r1
  000be668: movs r3,#0x0
  000be66a: uxth r1,r0
  000be66c: mov r0,r6
  000be66e: blx 0x0006fcf4
  000be672: ldr.w r0,[r8,#0x0]
  000be676: ldr.w r6,[r9,#0x0]
  000be67a: blx 0x00071a10
  000be67e: blx 0x00071a7c
  000be682: movs r1,#0x3
  000be684: blx 0x0006f514
  000be688: movw r0,#0x2766
  000be68c: add.w r2,r11,#0x19c
  000be690: add r0,r1
  000be692: movs r3,#0x0
  000be694: uxth r1,r0
  000be696: mov r0,r6
  000be698: blx 0x0006fbb0
  000be69c: ldr.w r0,[r8,#0x0]
  000be6a0: blx 0x00071770
  000be6a4: cbnz r0,0x000be6cc
  000be6a6: ldr.w r0,[r11,#0xc0]
  000be6aa: cmp r0,#0x3
  000be6ac: bne 0x000be6cc
  000be6ae: ldr.w r0,[r9,#0x0]
  000be6b2: movw r1,#0x458b
  000be6b6: mov r2,r5
  000be6b8: movs r3,#0x0
  000be6ba: blx 0x0006fcf4
  000be6be: ldr.w r0,[r9,#0x0]
  000be6c2: add.w r2,r11,#0x198
  000be6c6: movw r1,#0x2754
  000be6ca: b 0x000be904
  000be6cc: ldr.w r0,[r8,#0x0]
  000be6d0: blx 0x00071878
  000be6d4: cbz r0,0x000be700
  000be6d6: ldr.w r0,[r8,#0x0]
  000be6da: blx 0x00071770
  000be6de: cmp r0,#0x59
  000be6e0: bne 0x000be742
  000be6e2: ldr.w r0,[r9,#0x0]
  000be6e6: movw r1,#0x458d
  000be6ea: mov r2,r5
  000be6ec: movs r3,#0x0
  000be6ee: blx 0x0006fcf4
  000be6f2: ldr.w r0,[r9,#0x0]
  000be6f6: add.w r2,r11,#0x198
  000be6fa: movw r1,#0x2756
  000be6fe: b 0x000be814
  000be700: ldr.w r0,[r8,#0x0]
  000be704: ldr.w r6,[r9,#0x0]
  000be708: blx 0x00071a10
  000be70c: blx 0x0007390c
  000be710: movw r1,#0x4588
  000be714: add r0,r1
  000be716: mov r2,r5
  000be718: movs r3,#0x0
  000be71a: uxth r1,r0
  000be71c: mov r0,r6
  000be71e: blx 0x0006fcf4
  000be722: ldr.w r0,[r8,#0x0]
  000be726: ldr.w r6,[r9,#0x0]
  000be72a: blx 0x00071a10
  000be72e: blx 0x0007390c
  000be732: movw r1,#0x2751
  000be736: add r0,r1
  000be738: add.w r2,r11,#0x198
  000be73c: uxth r1,r0
  000be73e: mov r0,r6
  000be740: b 0x000be814
  000be742: ldr.w r0,[r8,#0x0]
  000be746: blx 0x00071770
  000be74a: cmp r0,#0x9d
  000be74c: bgt 0x000be7f8
  000be74e: ldr.w r0,[r9,#0x0]
  000be752: add.w r2,r11,#0xc
  000be756: movw r1,#0x45a0
  000be75a: movs r3,#0x0
  000be75c: movs r4,#0x0
  000be75e: blx 0x0006fcf4
  000be762: ldr.w r0,[r9,#0x0]
  000be766: add.w r1,r11,#0x10
  000be76a: blx 0x000720ac
  000be76e: ldrd r2,r1,[r11,#0xc]
  000be772: ldr.w r0,[r9,#0x0]
  000be776: blx 0x00073030
  000be77a: ldr.w r1,[r11,#0x10]
  000be77e: ldr.w r0,[r9,#0x0]
  000be782: blx 0x00072088
  000be786: mov.w r2,#0x3e8
  000be78a: movs r3,#0x0
  000be78c: str r4,[sp,#0x0]
  000be78e: blx 0x0006f7cc
  000be792: ldr.w r0,[r9,#0x0]
  000be796: add.w r2,r11,#0x14
  000be79a: movw r1,#0x45a1
  000be79e: movs r3,#0x0
  000be7a0: blx 0x0006fcf4
  000be7a4: ldr.w r0,[r9,#0x0]
  000be7a8: add.w r1,r11,#0x18
  000be7ac: blx 0x000720ac
  000be7b0: ldrd r2,r1,[r11,#0x14]
  000be7b4: ldr.w r0,[r9,#0x0]
  000be7b8: blx 0x00073030
  000be7bc: ldr.w r1,[r11,#0x18]
  000be7c0: ldr.w r0,[r9,#0x0]
  000be7c4: blx 0x00072088
  000be7c8: mov.w r2,#0x3e8
  000be7cc: movs r3,#0x0
  000be7ce: str r4,[sp,#0x0]
  000be7d0: blx 0x0006f7cc
  000be7d4: ldr.w r0,[r8,#0x0]
  000be7d8: blx 0x00071770
  000be7dc: mov r1,r0
  000be7de: ldr.w r0,[r9,#0x0]
  000be7e2: cmp r1,#0x6a
  000be7e4: add.w r2,r11,#0x1a0
  000be7e8: ite lt
  000be7ea: movw.lt r1,#0x2764
  000be7ee: movw.ge r1,#0x2765
  000be7f2: movs r3,#0x0
  000be7f4: blx 0x0006fbb0
  000be7f8: ldr.w r0,[r9,#0x0]
  000be7fc: movw r1,#0x4597
  000be800: mov r2,r5
  000be802: movs r3,#0x0
  000be804: blx 0x0006fcf4
  000be808: ldr.w r0,[r9,#0x0]
  000be80c: add.w r2,r11,#0x198
  000be810: movw r1,#0x2760
  000be814: movs r3,#0x0
  000be816: blx 0x0006fbb0
  000be81a: ldr.w r0,[r8,#0x0]
  000be81e: blx 0x00071a10
  000be822: blx 0x00071a7c
  000be826: mov r6,r0
  000be828: ldr.w r0,[r8,#0x0]
  000be82c: blx 0x00071a10
  000be830: blx 0x00071a7c
  000be834: orr r1,r6,#0x2
  000be838: cmp r1,#0x1a
  000be83a: it ne
  000be83c: cmp.ne r0,#0x19
  000be83e: bne 0x000be87e
  000be840: movs r0,#0x1
  000be842: add.w r2,r11,#0x1cc
  000be846: strb.w r0,[r11,#0x289]
  000be84a: movw r1,#0x3827
  000be84e: ldr.w r0,[r9,#0x0]
  000be852: movs r3,#0x0
  000be854: blx 0x0006fcf4
  000be858: ldr.w r0,[r9,#0x0]
  000be85c: add.w r2,r11,#0x1c4
  000be860: movw r1,#0x8282
  000be864: movs r3,#0x0
  000be866: blx 0x0006fbb0
  000be86a: ldr r0,[0x000be980]
  000be86c: add.w r2,r11,#0x1c8
  000be870: movs r3,#0x0
  000be872: adds r1,r0,#0x1
  000be874: ldr.w r0,[r9,#0x0]
  000be878: uxth r1,r1
  000be87a: blx 0x0006fbb0
  000be87e: ldr.w r0,[r8,#0x0]
  000be882: blx 0x00073cd8
  000be886: cbz r0,0x000be8c4
  000be888: ldr.w r0,[r9,#0x0]
  000be88c: add r2,sp,#0x88
  000be88e: movw r1,#0x499f
  000be892: movs r3,#0x0
  000be894: blx 0x0006fcf4
  000be898: ldr.w r0,[r9,#0x0]
  000be89c: add.w r1,r11,#0x1b4
  000be8a0: blx 0x000720ac
  000be8a4: ldr.w r1,[r11,#0x1b4]
  000be8a8: ldr.w r0,[r9,#0x0]
  000be8ac: ldr r2,[sp,#0x88]
  000be8ae: blx 0x00073030
  000be8b2: ldr.w r0,[r9,#0x0]
  000be8b6: add.w r2,r11,#0x1b8
  000be8ba: movw r1,#0x715a
  000be8be: movs r3,#0x0
  000be8c0: blx 0x0006fbb0
  000be8c4: ldr.w r0,[r8,#0x0]
  000be8c8: blx 0x00073ce4
  000be8cc: cbz r0,0x000be90a
  000be8ce: ldr.w r0,[r9,#0x0]
  000be8d2: add r2,sp,#0x88
  000be8d4: movw r1,#0x49a0
  000be8d8: movs r3,#0x0
  000be8da: blx 0x0006fcf4
  000be8de: ldr.w r0,[r9,#0x0]
  000be8e2: add.w r1,r11,#0x1bc
  000be8e6: blx 0x000720ac
  000be8ea: ldr.w r1,[r11,#0x1bc]
  000be8ee: ldr.w r0,[r9,#0x0]
  000be8f2: ldr r2,[sp,#0x88]
  000be8f4: blx 0x00073030
  000be8f8: ldr.w r0,[r9,#0x0]
  000be8fc: add.w r2,r11,#0x1c0
  000be900: movw r1,#0x715b
  000be904: movs r3,#0x0
  000be906: blx 0x0006fbb0
  000be90a: ldr.w r0,[r8,#0x0]
  000be90e: blx 0x00071a10
  000be912: blx 0x0007390c
  000be916: cmp r0,#0x10
  000be918: blt 0x000be93a
  000be91a: ldr r0,[0x000be984]
  000be91c: add r0,pc
  000be91e: ldr r0,[r0,#0x0]
  000be920: ldr r0,[r0,#0x0]
  000be922: blx 0x0007258c
  000be926: blx 0x0006ee78
  000be92a: cbz r0,0x000be93a
  000be92c: ldr r1,[r5,#0x0]
  000be92e: ldr.w r0,[r9,#0x0]
  000be932: blx 0x00072370
  000be936: ldr r1,[0x000be988]
  000be938: str r1,[r0,#0x1c]
  000be93a: ldr.w r0,[r8,#0x0]
  000be93e: blx 0x00073cf0
  000be942: cbz r0,0x000be950
  000be944: movs r0,#0x0
  000be946: strd r0,r0,[r11,#0x1a4]
  000be94a: str.w r0,[r11,#0x1ac]
  000be94e: b 0x000bea04
  000be950: ldr r0,[0x000be98c]
  000be952: add r0,pc
  000be954: ldr r4,[r0,#0x0]
  000be956: ldr.w r0,[r8,#0x0]
  000be95a: ldr r5,[r4,#0x0]
  000be95c: blx 0x00071c14
  000be960: cbz r0,0x000be990
  000be962: ldr.w r0,[r8,#0x0]
  000be966: blx 0x00071c14
  000be96a: blx 0x00071824
  000be96e: lsls r2,r0,#0x1
  000be970: b 0x000be994
  000be990: mov.w r2,#0xffffffff
  000be994: asrs r3,r2,#0x1f
  000be996: mov r0,r5
  000be998: blx 0x000739c0
  000be99c: ldr r0,[0x000bece4]
  000be99e: mov.w r1,#0x10000
  000be9a2: add r0,pc
  000be9a4: ldr r5,[r0,#0x0]
  000be9a6: ldr r0,[r4,#0x0]
  000be9a8: blx r5
  000be9aa: vmov s0,r0
  000be9ae: vldr.32 s16,[pc,#0x338]
  000be9b2: vldr.32 s18,[pc,#0x338]
  000be9b6: mov.w r1,#0x10000
  000be9ba: vcvt.f32.s32 s0,s0
  000be9be: vmul.f32 s0,s0,s16
  000be9c2: vmul.f32 s0,s0,s18
  000be9c6: vstr.32 s0,[r11,#0x1a4]
  000be9ca: ldr r0,[r4,#0x0]
  000be9cc: blx r5
  000be9ce: vmov s0,r0
  000be9d2: mov.w r1,#0x10000
  000be9d6: vcvt.f32.s32 s0,s0
  000be9da: vmul.f32 s0,s0,s16
  000be9de: vmul.f32 s0,s0,s18
  000be9e2: vstr.32 s0,[r11,#0x1a8]
  000be9e6: ldr r0,[r4,#0x0]
  000be9e8: blx r5
  000be9ea: vmov s0,r0
  000be9ee: vcvt.f32.s32 s0,s0
  000be9f2: vmul.f32 s0,s0,s16
  000be9f6: vmul.f32 s0,s0,s18
  000be9fa: vstr.32 s0,[r11,#0x1ac]
  000be9fe: ldr r0,[r4,#0x0]
  000bea00: blx 0x000718cc
  000bea04: ldr.w r0,[r11,#0xc0]
  000bea08: cmp r0,#0x4
  000bea0a: it ne
  000bea0c: cmp.ne r0,#0x17
  000bea0e: bne.w 0x000beb14
  000bea12: movs r0,#0x60
  000bea14: blx 0x0006eb24
  000bea18: ldr.w r1,[r11,#0xc0]
  000bea1c: mov r5,r0
  000bea1e: blx 0x00073cfc
  000bea22: ldr r0,[0x000becf0]
  000bea24: str.w r5,[r11,#0xec]
  000bea28: add r0,pc
  000bea2a: ldr r0,[r0,#0x0]
  000bea2c: ldr r0,[r0,#0x0]
  000bea2e: blx 0x00071a10
  000bea32: cbz r0,0x000bea6a
  000bea34: vldr.32 s0,[r0]
  000bea38: ldr r1,[0x000becf4]
  000bea3a: vcvt.f32.s32 s0,s0
  000bea3e: add r1,pc
  000bea40: ldr r1,[r1,#0x0]
  000bea42: vstr.32 s0,[r1]
  000bea46: vmov lr,s0
  000bea4a: vldr.32 s2,[r0,#0x4]
  000bea4e: vcvt.f32.s32 s4,s2
  000bea52: ldr r3,[0x000becf8]
  000bea54: add r3,pc
  000bea56: ldr r3,[r3,#0x0]
  000bea58: vmov r12,s4
  000bea5c: vstr.32 s4,[r3]
  000bea60: vldr.32 s2,[r0,#0x8]
  000bea64: vcvt.f32.s32 s2,s2
  000bea68: b 0x000bea90
  000bea6a: ldr r0,[0x000becfc]
  000bea6c: vmov.f32 s0,0x41200000
  000bea70: ldr r2,[0x000bed00]
  000bea72: add r0,pc
  000bea74: ldr r1,[0x000bed04]
  000bea76: add r2,pc
  000bea78: ldr r3,[0x000bed08]
  000bea7a: ldr r0,[r0,#0x0]
  000bea7c: add.w r12,r1,#0x1e80000
  000bea80: ldr r2,[r2,#0x0]
  000bea82: mov lr,r1
  000bea84: vldr.32 s4,[pc,#0x280]
  000bea88: str r1,[r0,#0x0]
  000bea8a: vmov.f32 s2,s0
  000bea8e: str r3,[r2,#0x0]
  000bea90: vmov.f32 s6,0x40400000
  000bea94: ldr r0,[0x000bed0c]
  000bea96: ldr r3,[0x000bed10]
  000bea98: ldr r4,[0x000bed14]
  000bea9a: add r0,pc
  000bea9c: ldr r5,[0x000bed18]
  000bea9e: add r3,pc
  000beaa0: ldr r6,[0x000bed1c]
  000beaa2: add r4,pc
  000beaa4: ldr r2,[0x000bed20]
  000beaa6: add r5,pc
  000beaa8: ldr r1,[0x000bed24]
  000beaaa: add r6,pc
  000beaac: vdiv.f32 s0,s0,s6
  000beab0: add r2,pc
  000beab2: add r1,pc
  000beab4: ldr r0,[r0,#0x0]
  000beab6: ldr r3,[r3,#0x0]
  000beab8: ldr r4,[r4,#0x0]
  000beaba: ldr r5,[r5,#0x0]
  000beabc: ldr r6,[r6,#0x0]
  000beabe: ldr r2,[r2,#0x0]
  000beac0: ldr r1,[r1,#0x0]
  000beac2: vdiv.f32 s4,s4,s6
  000beac6: vdiv.f32 s6,s2,s6
  000beaca: vcvt.s32.f32 s0,s0
  000beace: vcvt.s32.f32 s4,s4
  000bead2: vcvt.s32.f32 s6,s6
  000bead6: vstr.32 s2,[r6]
  000beada: str.w lr,[r2,#0x0]
  000beade: str.w r12,[r5,#0x0]
  000beae2: vstr.32 s2,[r0]
  000beae6: vstr.32 s0,[r3]
  000beaea: vstr.32 s4,[r4]
  000beaee: vstr.32 s6,[r1]
  000beaf2: ldr r0,[sp,#0xdc]
  000beaf4: ldr.w r1,[r10,#0x0]
  000beaf8: subs r0,r1,r0
  000beafa: itttt eq
  000beafc: add.eq sp,#0xe0
  000beafe: vpop.eq {d8,d9,d10}
  000beb02: vpop.eq {d12,d13,d14,d15}
  000beb06: add.eq sp,#0x4
  000beb08: itt eq
  000beb0a: pop.eq.w {r8,r9,r10,r11}
  000beb0e: pop.eq {r4,r5,r6,r7,pc}
  000beb10: blx 0x0006e824
  000beb14: ldr r0,[0x000bed28]
  000beb16: add r0,pc
  000beb18: ldr r0,[r0,#0x0]
  000beb1a: ldrb r0,[r0,#0x0]
  000beb1c: cbnz r0,0x000beb34
  000beb1e: ldr r0,[0x000bed2c]
  000beb20: add r0,pc
  000beb22: ldr r4,[r0,#0x0]
  000beb24: ldr r0,[r4,#0x0]
  000beb26: blx 0x00071c14
  000beb2a: blx 0x0007321c
  000beb2e: cmp r0,#0x0
  000beb30: beq.w 0x000bf13e
  000beb34: movs r5,#0x1
  000beb36: ldr r0,[0x000bed30]
  000beb38: add r0,pc
  000beb3a: ldr.w r9,[r0,#0x0]
  000beb3e: ldr.w r0,[r9,#0x0]
  000beb42: blx 0x00071770
  000beb46: movs r1,#0x0
  000beb48: cmp r0,#0x2b
  000beb4a: it lt
  000beb4c: mov.lt r1,#0x1
  000beb4e: and.w r8,r5,r1
  000beb52: cmp.w r8,#0x1
  000beb56: bne 0x000beb66
  000beb58: ldr r0,[0x000bed34]
  000beb5a: movs r1,#0x22
  000beb5c: add r0,pc
  000beb5e: ldr r0,[r0,#0x0]
  000beb60: ldr r0,[r0,#0x0]
  000beb62: blx 0x00072298
  000beb66: movs r0,#0xc
  000beb68: blx 0x0006eb24
  000beb6c: mov r5,r0
  000beb6e: blx 0x00073d08
  000beb72: movs r0,#0x4
  000beb74: mov r1,r5
  000beb76: str.w r5,[r11,#0x100]
  000beb7a: blx 0x00073d14
  000beb7e: ldr.w r0,[r9,#0x0]
  000beb82: blx 0x00073d20
  000beb86: cbz r0,0x000bebce
  000beb88: ldr.w r0,[r9,#0x0]
  000beb8c: blx 0x000723d0
  000beb90: cbz r0,0x000beb9e
  000beb92: ldr.w r0,[r11,#0x100]
  000beb96: movs r1,#0x0
  000beb98: ldr r0,[r0,#0x4]
  000beb9a: str r1,[r0,#0x0]
  000beb9c: b 0x000bec4a
  000beb9e: ldr.w r0,[r9,#0x0]
  000beba2: blx 0x00071c14
  000beba6: blx 0x00071824
  000bebaa: cmp r0,#0x1b
  000bebac: beq 0x000bebce
  000bebae: ldr.w r0,[r9,#0x0]
  000bebb2: blx 0x00071c14
  000bebb6: blx 0x00071824
  000bebba: cmp r0,#0x6e
  000bebbc: beq 0x000bebce
  000bebbe: ldr.w r0,[r9,#0x0]
  000bebc2: blx 0x00071c14
  000bebc6: blx 0x00071824
  000bebca: cmp r0,#0x6f
  000bebcc: bne 0x000beb92
  000bebce: mov.w r0,#0x174
  000bebd2: blx 0x0006eb24
  000bebd6: mov r5,r0
  000bebd8: ldr.w r0,[r9,#0x0]
  000bebdc: blx 0x00071c14
  000bebe0: mov r1,r0
  000bebe2: mov r0,r5
  000bebe4: blx 0x00073d2c
  000bebe8: ldr.w r0,[r11,#0x100]
  000bebec: ldr r0,[r0,#0x4]
  000bebee: str r5,[r0,#0x0]
  000bebf0: ldr.w r1,[r11,#0x100]
  000bebf4: ldr.w r0,[r11,#0x0]
  000bebf8: ldr r1,[r1,#0x4]
  000bebfa: ldr r1,[r1,#0x0]
  000bebfc: ldr.w r1,[r1,#0x140]
  000bec00: blx 0x00073d38
  000bec04: ldr.w r0,[r9,#0x0]
  000bec08: blx 0x000719a4
  000bec0c: cbz r0,0x000bec4a
  000bec0e: ldr r0,[0x000bed38]
  000bec10: movs r1,#0x65
  000bec12: add r0,pc
  000bec14: ldr r0,[r0,#0x0]
  000bec16: ldr r0,[r0,#0x0]
  000bec18: blx 0x00071c44
  000bec1c: add r6,sp,#0x88
  000bec1e: mov r5,r0
  000bec20: mov r1,r5
  000bec22: mov r0,r6
  000bec24: blx 0x000736a8
  000bec28: ldr.w r0,[r11,#0x100]
  000bec2c: ldr r0,[r0,#0x4]
  000bec2e: ldr r0,[r0,#0x0]
  000bec30: adds r0,#0x18
  000bec32: mov r1,r6
  000bec34: blx 0x0006f2b0
  000bec38: add r0,sp,#0x88
  000bec3a: blx 0x0006ee30
  000bec3e: cbz r5,0x000bec4a
  000bec40: mov r0,r5
  000bec42: blx 0x0007360c
  000bec46: blx 0x0006eb48
  000bec4a: ldr r0,[0x000bed3c]
  000bec4c: vldr.64 d16,[r11,#0x18c]
  000bec50: add r0,pc
  000bec52: ldr.w r1,[r11,#0x194]
  000bec56: str r1,[sp,#0xd0]
  000bec58: movs r1,#0x0
  000bec5a: ldr r0,[r0,#0x0]
  000bec5c: vstr.64 d16,[sp,#0xc8]
  000bec60: str.w r1,[r11,#0x138]
  000bec64: add.w r1,r11,#0x18c
  000bec68: str r0,[sp,#0x28]
  000bec6a: ldr r5,[r0,#0x0]
  000bec6c: ldr.w r0,[r9,#0x0]
  000bec70: str r1,[sp,#0x24]
  000bec72: blx 0x00071c14
  000bec76: cmp r0,#0x0
  000bec78: strd r8,r10,[sp,#0x10]
  000bec7c: beq 0x000bec92
  000bec7e: ldr.w r0,[r9,#0x0]
  000bec82: blx 0x00071c14
  000bec86: blx 0x00071824
  000bec8a: sbfx r3,r0,#0x1e,#0x1
  000bec8e: lsls r2,r0,#0x1
  000bec90: b 0x000bec9a
  000bec92: mov.w r2,#0xffffffff
  000bec96: mov.w r3,#0xffffffff
  000bec9a: mov r0,r5
  000bec9c: blx 0x000739c0
  000beca0: add r0,sp,#0x88
  000beca2: vmov.f32 s18,0x40400000
  000beca6: add.w r1,r0,#0x28
  000becaa: str r1,[sp,#0x20]
  000becac: add.w r1,r0,#0x18
  000becb0: orr r0,r0,#0x4
  000becb4: str r1,[sp,#0x1c]
  000becb6: vmov.i32 q7,#0x0
  000becba: str r0,[sp,#0x18]
  000becbc: mov.w r8,#0x1
  000becc0: ldr r0,[0x000bed40]
  000becc2: ldr r4,[0x000bed44]
  000becc4: add r0,pc
  000becc6: vldr.32 s16,[pc,#0x20]
  000becca: add r4,pc
  000beccc: vldr.32 s20,[pc,#0x1c]
  000becd0: ldr r5,[r0,#0x0]
  000becd2: adr.w r0,0xbf188
  000becd6: vld1.64 {d12,d13},[r0]
  000becda: strd r11,r9,[sp,#0x2c]
  000becde: str r4,[sp,#0x34]
  000bece0: b 0x000befb4
  000bed48: cmp.w r8,#0x1
  000bed4c: bne 0x000bed6a
  000bed4e: ldr.w r0,[r9,#0x0]
  000bed52: blx 0x00071a10
  000bed56: cmp r0,#0x0
  000bed58: beq 0x000bee2c
  000bed5a: ldr.w r0,[r9,#0x0]
  000bed5e: blx 0x00071a10
  000bed62: blx 0x00073c18
  000bed66: cmp r0,#0x0
  000bed68: beq 0x000bee2c
  000bed6a: ldr.w r0,[r9,#0x0]
  000bed6e: blx 0x00071c14
  000bed72: cmp r0,#0x0
  000bed74: beq 0x000bee1c
  000bed76: ldr r4,[sp,#0x28]
  000bed78: movs r1,#0x2
  000bed7a: ldr r0,[r4,#0x0]
  000bed7c: blx 0x00071848
  000bed80: mov r6,r0
  000bed82: ldr r0,[r4,#0x0]
  000bed84: mov.w r1,#0x1f4
  000bed88: mov.w r10,#0x0
  000bed8c: blx 0x00071848
  000bed90: ldr r3,[0x000bf168]
  000bed92: add.w r1,r0,#0xfa
  000bed96: ldr.w r2,[r11,#0x138]
  000bed9a: cmp r6,#0x0
  000bed9c: it eq
  000bed9e: sub.eq r1,r3,r0
  000beda0: cmp.w r8,#0x1
  000beda4: add.w r0,r2,r1, lsl #0x4
  000beda8: adr.w r1,0xbf1a4
  000bedac: add r4,sp,#0x4c
  000bedae: add r6,sp,#0x88
  000bedb0: vmov s0,r0
  000bedb4: mov.w r2,#0x0
  000bedb8: vcvt.f32.s32 s0,s0
  000bedbc: it eq
  000bedbe: add.eq r1,#0x4
  000bedc0: vldr.32 s4,[r1]
  000bedc4: mov.w r1,#0x3f800000
  000bedc8: str.w r0,[r11,#0x138]
  000bedcc: ldr r0,[sp,#0x18]
  000bedce: vmul.f32 s2,s0,s16
  000bedd2: vmla.f32 s4,s0,s18
  000bedd6: vstr.32 s4,[r11,#0x194]
  000bedda: vmul.f32 s2,s2,s20
  000bedde: str r1,[sp,#0x88]
  000bede0: vst1.32 {d14,d15},[r0]
  000bede4: ldr r0,[sp,#0x1c]
  000bede6: str r1,[sp,#0x9c]
  000bede8: str.w r10,[sp,#0x0]
  000bedec: vmov r3,s2
  000bedf0: vst1.64 {d14,d15},[r0]
  000bedf4: ldr r0,[sp,#0x20]
  000bedf6: vst1.32 {d12,d13},[r0]
  000bedfa: mov r0,r4
  000bedfc: str r1,[sp,#0xc0]
  000bedfe: mov r1,r6
  000bee00: blx 0x00072094
  000bee04: ldr r2,[sp,#0x24]
  000bee06: mov r0,r4
  000bee08: mov r1,r6
  000bee0a: blx 0x0006f694
  000bee0e: add r0,sp,#0xc8
  000bee10: mov r1,r4
  000bee12: blx 0x0006eb3c
  000bee16: ldr r4,[sp,#0x34]
  000bee18: str.w r10,[sp,#0xcc]
  000bee1c: ldr.w r0,[r9,#0x0]
  000bee20: blx 0x000723d0
  000bee24: cbz r0,0x000bee4c
  000bee26: mov.w r10,#0x0
  000bee2a: b 0x000bee5a
  000bee2c: ldr.w r0,[r11,#0x100]
  000bee30: ldr r0,[r0,#0x4]
  000bee32: ldr r0,[r0,#0x4]
  000bee34: cmp r0,#0x0
  000bee36: beq.w 0x000befb0
  000bee3a: ldr r1,[r0,#0x0]
  000bee3c: ldr r1,[r1,#0x4]
  000bee3e: blx r1
  000bee40: ldr.w r0,[r11,#0x100]
  000bee44: movs r1,#0x0
  000bee46: ldr r0,[r0,#0x4]
  000bee48: str r1,[r0,#0x4]
  000bee4a: b 0x000befb0
  000bee4c: ldr.w r0,[r9,#0x0]
  000bee50: blx 0x00071a10
  000bee54: blx 0x00071950
  000bee58: mov r10,r0
  000bee5a: movs r0,#0xc0
  000bee5c: blx 0x0006eb24
  000bee60: ldrh.w r1,[r4,r10,lsl #0x3]
  000bee64: mov r11,r0
  000bee66: ldr r2,[r5,#0x0]
  000bee68: movs r3,#0x0
  000bee6a: blx 0x0007207c
  000bee6e: ldr r0,[r5,#0x0]
  000bee70: add r1,sp,#0x4c
  000bee72: mov.w r4,#0xffffffff
  000bee76: lsl.w r6,r10,#0x2
  000bee7a: str r4,[sp,#0x4c]
  000bee7c: blx 0x000720ac
  000bee80: ldr r3,[sp,#0x34]
  000bee82: orr r2,r6,#0x1
  000bee86: ldr r0,[r5,#0x0]
  000bee88: ldr r1,[sp,#0x4c]
  000bee8a: ldrh.w r2,[r3,r2,lsl #0x1]
  000bee8e: movs r3,#0x0
  000bee90: blx 0x0007303c
  000bee94: ldr r0,[r5,#0x0]
  000bee96: add r1,sp,#0x38
  000bee98: str r4,[sp,#0x38]
  000bee9a: ldr r4,[sp,#0x34]
  000bee9c: blx 0x000720ac
  000beea0: orr r0,r6,#0x2
  000beea4: ldr r1,[sp,#0x38]
  000beea6: movs r3,#0x0
  000beea8: ldrh.w r2,[r4,r0,lsl #0x1]
  000beeac: ldr r0,[r5,#0x0]
  000beeae: blx 0x0007303c
  000beeb2: ldr r1,[sp,#0x4c]
  000beeb4: mov r0,r11
  000beeb6: blx 0x000720e8
  000beeba: ldr r1,[sp,#0x38]
  000beebc: mov r0,r11
  000beebe: blx 0x000720e8
  000beec2: mov.w r0,#0x148
  000beec6: blx 0x0006eb24
  000beeca: mov r9,r0
  000beecc: subs.w r0,r8,#0x2
  000beed0: ldr r3,[sp,#0xc8]
  000beed2: vldr.32 s0,[sp,#0xcc]
  000beed6: it ne
  000beed8: mov.ne r0,#0x1
  000beeda: vldr.32 s2,[sp,#0xd0]
  000beede: str r0,[sp,#0x8]
  000beee0: mov r0,r9
  000beee2: movs r1,#0xf
  000beee4: mov r2,r11
  000beee6: vstr.32 s0,[sp]
  000beeea: vstr.32 s2,[sp,#0x4]
  000beeee: blx 0x00073d44
  000beef2: ldr.w r11,[sp,#0x2c]
  000beef6: cmp.w r8,#0x2
  000beefa: ldr.w r0,[r11,#0x100]
  000beefe: ldr r0,[r0,#0x4]
  000bef00: str.w r9,[r0,r8,lsl #0x2]
  000bef04: bne 0x000bef14
  000bef06: ldr.w r0,[r11,#0x100]
  000bef0a: movs r1,#0x0
  000bef0c: ldr r0,[r0,#0x4]
  000bef0e: ldr r0,[r0,#0x8]
  000bef10: ldr r0,[r0,#0x4]
  000bef12: str r1,[r0,#0x40]
  000bef14: mov.w r1,#0xffffffff
  000bef18: ldr r0,[r5,#0x0]
  000bef1a: str r1,[sp,#0x48]
  000bef1c: add r1,sp,#0x48
  000bef1e: blx 0x000720ac
  000bef22: orr r2,r6,#0x3
  000bef26: ldr r0,[r5,#0x0]
  000bef28: ldr r1,[sp,#0x48]
  000bef2a: movs r3,#0x0
  000bef2c: ldrh.w r2,[r4,r2,lsl #0x1]
  000bef30: blx 0x0007303c
  000bef34: ldr.w r0,[r11,#0x100]
  000bef38: ldr r1,[sp,#0x48]
  000bef3a: ldr r0,[r0,#0x4]
  000bef3c: ldr.w r0,[r0,r8,lsl #0x2]
  000bef40: blx 0x00073d50
  000bef44: ldr r0,[0x000bf1b8]
  000bef46: movs r1,#0x2
  000bef48: orr.w r1,r1,r10, lsl #0x2
  000bef4c: add r2,sp,#0x88
  000bef4e: add r0,pc
  000bef50: movs r3,#0x2
  000bef52: ldrh r1,[r0,r1]
  000bef54: ldrh.w r0,[r0,r10,lsl #0x2]
  000bef58: strh.w r1,[sp,#0xda]
  000bef5c: strh.w r0,[sp,#0xd8]
  000bef60: add r1,sp,#0xd8
  000bef62: ldr r0,[0x000bf1ac]
  000bef64: str r0,[sp,#0x8c]
  000bef66: movw r0,#0x9c40
  000bef6a: str r0,[sp,#0x88]
  000bef6c: ldr.w r0,[r11,#0x100]
  000bef70: ldr r0,[r0,#0x4]
  000bef72: ldr.w r0,[r0,r8,lsl #0x2]
  000bef76: ldr r0,[r0,#0x8]
  000bef78: blx 0x00073d5c
  000bef7c: ldr.w r1,[r11,#0x100]
  000bef80: ldr.w r0,[r11,#0x0]
  000bef84: ldr r1,[r1,#0x4]
  000bef86: ldr.w r1,[r1,r8,lsl #0x2]
  000bef8a: ldr r1,[r1,#0x8]
  000bef8c: blx 0x00073d38
  000bef90: ldr.w r9,[sp,#0x30]
  000bef94: cmp.w r10,#0x1
  000bef98: bne 0x000befb0
  000bef9a: ldr r0,[0x000bf1b0]
  000bef9c: add r1,sp,#0x44
  000bef9e: str r0,[sp,#0x44]
  000befa0: ldr.w r0,[r11,#0x100]
  000befa4: ldr r0,[r0,#0x4]
  000befa6: ldr.w r0,[r0,r8,lsl #0x2]
  000befaa: ldr r0,[r0,#0x8]
  000befac: blx 0x00073d68
  000befb0: add.w r8,r8,#0x1
  000befb4: cmp.w r8,#0x3
  000befb8: bcc.w 0x000bed48
  000befbc: ldr r0,[sp,#0x28]
  000befbe: ldr r0,[r0,#0x0]
  000befc0: blx 0x000718cc
  000befc4: ldr.w r0,[r9,#0x0]
  000befc8: b 0x0017ce9c
  000bf13e: ldr r0,[r4,#0x0]
  000bf140: blx 0x000723d0
  000bf144: mov r5,r0
  000bf146: b 0x000beb36
```
