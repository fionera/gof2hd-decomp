# PlayerFighter::update
elf 0xdcfd8 body 10012
Sig: undefined __thiscall update(PlayerFighter * this, int param_1)

## decompile
```c

/* PlayerFighter::update(int) */

void __thiscall PlayerFighter::update(PlayerFighter *this,int param_1)

{
  Player PVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  bool bVar5;
  PlayerFighter PVar6;
  int iVar7;
  Vector *pVVar8;
  Standing *this_00;
  undefined4 uVar9;
  PlayerFighter *pPVar10;
  int *piVar11;
  Matrix *pMVar12;
  Player *this_01;
  Vector *pVVar13;
  uint *puVar14;
  int *piVar15;
  Station *this_02;
  int iVar16;
  String *pSVar17;
  undefined4 *puVar18;
  int iVar19;
  TargetFollowCamera *pTVar20;
  EaseInOutMatrix *pEVar21;
  Array *pAVar22;
  undefined2 uVar23;
  void *pvVar24;
  float *pfVar25;
  int __flags;
  void *__arg;
  Vector *pVVar26;
  Route *pRVar27;
  Matrix *pMVar28;
  Explosion *this_03;
  uint uVar29;
  code *pcVar30;
  int iVar31;
  code *pcVar32;
  code *pcVar33;
  int *piVar34;
  code *pcVar35;
  byte bVar36;
  byte bVar37;
  bool bVar38;
  bool bVar39;
  uint in_fpscr;
  uint uVar40;
  uint uVar41;
  float fVar42;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar43;
  float fVar44;
  float fVar45;
  undefined4 uVar46;
  undefined8 uVar47;
  undefined8 uVar48;
  undefined8 uVar49;
  undefined6 uVar50;
  undefined8 local_138;
  float local_130;
  undefined1 auStack_f8 [12];
  Vector aVStack_ec [12];
  undefined8 local_e0;
  undefined4 local_d8;
  undefined4 local_d0;
  float local_cc;
  undefined4 local_c8;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined8 local_88;
  float local_80;
  int local_78;
  float local_74;
  float local_70;
  float local_6c;
  undefined4 local_68;
  undefined4 local_64;
  float local_60;
  float fStack_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined8 local_50;
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  
  piVar34 = *(int **)(DAT_000ed348 + 0xecff0);
  local_3c = *piVar34;
  if (((*(int *)(this + 0x88) == 4) && (iVar7 = Explosion::isPlaying(), iVar7 == 0)) &&
     ((this[0x4c] == (PlayerFighter)0x0 || (60000 < *(int *)(this + 0xd8))))) {
    iVar7 = *piVar34 - local_3c;
    if (iVar7 != 0) {
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(iVar7,*piVar34);
    }
    (*(code *)(DAT_001abe14 + 0x1abe18))();
    return;
  }
  *(int *)(this + 0x1c0) = *(int *)(this + 0x1c0) + param_1;
  *(int *)(this + 0x1b8) = *(int *)(this + 0x1b8) + param_1;
  if (*(int *)(this + 0x28) == 1) {
    *(undefined4 *)(this + 0x28) = 1;
  }
  *(int *)(this + 0x1d0) = param_1;
  *(int *)(this + 0x1d4) = param_1 >> 0x1f;
  AEGeometry::getPosition();
  pVVar8 = (Vector *)(this + 0x2c);
  AbyssEngine::AEMath::Vector::operator=(pVVar8,(Vector *)&local_78);
  if (this[0x43] == (PlayerFighter)0x0) {
    if ((*(uint *)(this + 0x28) & 0xfffffffe) == 8) {
      uVar4 = 1;
    }
    else {
      iVar7 = KIPlayer::isWingMan();
      if (iVar7 == 0) {
        iVar7 = Status::getStanding();
        uVar4 = Standing::isEnemy(iVar7);
      }
      else {
        uVar4 = 0;
      }
    }
    *(undefined1 *)(*(int *)(this + 4) + 0x5c) = uVar4;
    if ((*(uint *)(this + 0x28) & 0xfffffffe) == 8) {
      uVar4 = 0;
    }
    else {
      iVar7 = KIPlayer::isWingMan();
      if (iVar7 == 0) {
        this_00 = (Standing *)Status::getStanding();
        uVar4 = Standing::isFriend(this_00,*(int *)(this + 0x28));
      }
      else {
        uVar4 = 1;
      }
    }
    iVar7 = *(int *)(this + 4);
    *(undefined1 *)(iVar7 + 0x5d) = uVar4;
    if (this[0x43] != (PlayerFighter)0x0) goto LAB_000ed0fe;
  }
  else {
    iVar7 = *(int *)(this + 4);
LAB_000ed0fe:
    iVar31 = *(int *)(iVar7 + 0x6c);
    uVar9 = Player::getMaxHitpoints();
    iVar7 = __aeabi_idiv(uVar9,0x14);
    if (iVar7 < iVar31) {
      *(undefined1 *)(*(int *)(this + 4) + 0x5c) = 1;
    }
  }
  if (this[0x42] != (PlayerFighter)0x0) {
    iVar7 = *(int *)(this + 0x2e8);
    *(int *)(this + 0x2e8) = iVar7 + param_1;
    if (20000 < iVar7 + param_1) {
      *(undefined4 *)(this + 0x2e8) = 0;
      iVar7 = Player::gunAvailable(*(int *)(this + 4));
      if (iVar7 != 0) {
        *(uint *)(this + 0x140) = (uint)(*(int *)(this + 0x140) == 0);
      }
    }
    iVar7 = Player::isAlwaysEnemy();
    if ((iVar7 == 0) && (iVar7 = Level::getPlayer(), iVar7 != 0)) {
      iVar7 = Level::getPlayer();
      pPVar10 = *(PlayerFighter **)(*(int *)(iVar7 + 0x14) + 4);
      bVar38 = pPVar10 == this;
      if (bVar38) {
        pPVar10 = (PlayerFighter *)(uint)(byte)this[0x43];
      }
      if (bVar38 && pPVar10 == (PlayerFighter *)0x0) {
        Player::setAlwaysEnemy(SUB41(*(undefined4 *)(this + 4),0));
        Level::uncoverWanted(*(int *)(this + 0x54));
      }
    }
  }
  piVar11 = *(int **)(DAT_000ed354 + 0xed182);
  uVar50 = Status::inBlackMarketSystem();
  uVar23 = (undefined2)((uint6)uVar50 >> 0x20);
  if (((int)uVar50 != 0) && (*(char *)(*piVar11 + 0x110) != '\0')) {
    iVar7 = *(int *)(this + 0x28);
    bVar38 = iVar7 == 8;
    if (bVar38) {
      iVar7 = *(int *)(this + 4);
      uVar23 = 0;
    }
    if (bVar38) {
      *(undefined2 *)(iVar7 + 0x5c) = uVar23;
    }
  }
  iVar7 = Player::turnedEnemy();
  if (iVar7 != 0) {
    *(undefined2 *)(*(int *)(this + 4) + 0x5c) = 1;
  }
  uVar49 = Player::isAlwaysFriend();
  pvVar24 = (void *)((ulonglong)uVar49 >> 0x20);
  if ((int)uVar49 != 0) {
    pvVar24 = (void *)0x100;
    *(undefined2 *)(*(int *)(this + 4) + 0x5c) = 0x100;
  }
  if ((this[0xdc] == (PlayerFighter)0x0) && (*(int *)(this + 0x6c) == 0)) {
    iVar7 = Route::clone(*(__fn **)(this + 0x144),pvVar24,__flags,__arg);
    *(int *)(this + 0x6c) = iVar7;
  }
  iVar7 = *(int *)(this + 0x1c8);
  *(int *)(this + 0x1c8) = iVar7 + param_1;
  if (200 < iVar7 + param_1) {
    if (*(Vector **)(this + 0x154) != (Vector *)0x0) {
      Trail::update(*(Vector **)(this + 0x154),pVVar8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x158),pVVar8);
    }
    *(undefined4 *)(this + 0x1c8) = 0;
  }
  pMVar12 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar12);
  if (*(int *)(this + 0xc) != 0) {
    pMVar12 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(*(int *)(this + 4) + 4),pMVar12);
  }
  if (this[0xdc] != (PlayerFighter)0x0) {
    iVar7 = *(int *)(this + 0x88);
    bVar38 = iVar7 != 4;
    if (bVar38) {
      iVar7 = *(int *)(this + 0x6c);
    }
    if (bVar38 && iVar7 != 0) {
      Level::getPlayer();
      AEGeometry::getPosition();
      pVVar13 = (Vector *)(this + 0x90);
      AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)&local_78);
      if (*(int *)(this + 0xe4) == 1) {
        iVar7 = *(int *)(this + 0xe0);
        if (iVar7 == 0) {
          Level::getPlayer();
          AEGeometry::getRightVector();
          pVVar26 = (Vector *)(this + 0x9c);
          fVar45 = (float)AbyssEngine::AEMath::Vector::operator=(pVVar26,(Vector *)&local_78);
          AbyssEngine::AEMath::Vector::operator*=(pVVar26,fVar45);
          AbyssEngine::AEMath::Vector::operator-=(pVVar13,pVVar26);
LAB_000ed69a:
          Level::getPlayer();
          AEGeometry::getDirection();
          fVar45 = (float)AbyssEngine::AEMath::VectorNormalize
                                    ((AEMath *)&local_d0,(Vector *)&local_138);
LAB_000ed6bc:
          AbyssEngine::AEMath::operator*((Vector *)&local_78,fVar45);
          AbyssEngine::AEMath::Vector::operator-=(pVVar13,(Vector *)&local_78);
        }
        else {
          if (iVar7 == 1) {
            Level::getPlayer();
            AEGeometry::getRightVector();
            pVVar26 = (Vector *)(this + 0x9c);
            fVar45 = (float)AbyssEngine::AEMath::Vector::operator=(pVVar26,(Vector *)&local_78);
            AbyssEngine::AEMath::Vector::operator*=(pVVar26,fVar45);
            AbyssEngine::AEMath::Vector::operator+=(pVVar13,pVVar26);
            goto LAB_000ed69a;
          }
          if (iVar7 == 2) {
            Level::getPlayer();
            AEGeometry::getUpVector();
            pVVar26 = (Vector *)(this + 0x9c);
            fVar45 = (float)AbyssEngine::AEMath::Vector::operator=(pVVar26,(Vector *)&local_78);
            AbyssEngine::AEMath::Vector::operator*=(pVVar26,fVar45);
            AbyssEngine::AEMath::Vector::operator+=(pVVar13,pVVar26);
            Level::getPlayer();
            AEGeometry::getDirection();
            fVar45 = (float)AbyssEngine::AEMath::VectorNormalize
                                      ((AEMath *)&local_d0,(Vector *)&local_138);
            goto LAB_000ed6bc;
          }
        }
        iVar7 = Route::length();
        if (iVar7 < 2) {
          Route::setNewCoords(*(undefined4 *)(this + 0x6c),*(undefined4 *)(this + 0x90),
                              *(undefined4 *)(this + 0x94),*(undefined4 *)(this + 0x98));
          pRVar27 = *(Route **)(this + 0x6c);
        }
        else {
          local_78 = (int)*(float *)(this + 0x90);
          local_74 = (float)(int)*(float *)(this + 0x94);
          local_70 = (float)(int)*(float *)(this + 0x98);
          if (*(Route **)(this + 0x6c) != (Route *)0x0) {
            pvVar24 = (void *)Route::~Route(*(Route **)(this + 0x6c));
            operator_delete(pvVar24);
            *(undefined4 *)(this + 0x6c) = 0;
          }
          pRVar27 = operator_new(0x18);
          Route::Route(pRVar27,&local_78,3);
          *(Route **)(this + 0x6c) = pRVar27;
        }
        Route::setLoop(SUB41(pRVar27,0));
      }
    }
  }
  if (*(int *)(this + 0x88) - 3U < 2) {
    Player::StopEngineSound();
  }
  else {
    Player::update(*(int *)(this + 4),SUB41(param_1,0));
    puVar14 = (uint *)Player::getEnemies();
    if (this[0x2e4] == (PlayerFighter)0x0) {
      if (puVar14 == (uint *)0x0) {
        pRVar27 = *(Route **)(this + 0x6c);
        if (pRVar27 == (Route *)0x0) {
          *(undefined4 *)(this + 0x88) = 5;
        }
        else {
          AEGeometry::getPosition();
          Route::update(pRVar27,(Vector *)&local_78);
          iVar7 = Route::getWaypoint();
          if (iVar7 != 0) {
            pcVar32 = *(code **)(DAT_000ed720 + 0xed38a);
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x124),(byte)(in_fpscr >> 0x16) & 3)
            ;
            *(undefined4 *)(this + 0x164) = uVar9;
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x128),(byte)(in_fpscr >> 0x16) & 3)
            ;
            *(undefined4 *)(this + 0x168) = uVar9;
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 300),(byte)(in_fpscr >> 0x16) & 3);
            this[300] = (PlayerFighter)0x1;
            *(undefined4 *)(this + 0x16c) = uVar9;
          }
        }
        goto LAB_000eda88;
      }
      uVar40 = *(uint *)(this + 0x38);
      if (*puVar14 <= uVar40) {
        uVar40 = 0xffffffff;
        *(undefined4 *)(this + 0x38) = 0xffffffff;
      }
      if (this[0x12e] == (PlayerFighter)0x0) {
        *(undefined4 *)(this + 0x38) = 0xffffffff;
      }
      else if ((-1 < (int)uVar40) && (iVar7 = Player::isActive(), iVar7 == 0)) {
        this[0x12e] = (PlayerFighter)0x0;
      }
      *(undefined4 *)(this + 0x148) = 0;
      if (*(int *)(this + 0x1b8) < 0x1389) {
        if (this[0x12e] == (PlayerFighter)0x0) {
          for (uVar40 = 0; uVar40 < *puVar14; uVar40 = uVar40 + 1) {
            if (((*(int *)(puVar14[1] + uVar40 * 4) != 0) &&
                (iVar7 = Player::isActive(), iVar7 != 0)) && (iVar7 = Player::isDead(), iVar7 == 0))
            {
              Player::getPosition();
              AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
              if ((*(int *)(this + 0x28) != 8) && (iVar7 = Player::turnedEnemy(), iVar7 != 0)) {
LAB_000ef77a:
                this[0x12e] = (PlayerFighter)0x1;
                *(uint *)(this + 0x38) = uVar40;
                break;
              }
              fVar42 = *(float *)(this + 0x2c) - *(float *)(this + 0x90);
              fVar45 = (float)VectorSignedToFloat(*(int *)(this + 0x128),
                                                  (byte)(in_fpscr >> 0x16) & 3);
              uVar29 = in_fpscr & 0xfffffff;
              in_fpscr = uVar29;
              if (fVar42 < fVar45) {
                fVar43 = (float)VectorSignedToFloat(-*(int *)(this + 0x128),
                                                    (byte)(uVar29 >> 0x16) & 3);
                uVar41 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e
                ;
                in_fpscr = uVar41 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
                bVar37 = (byte)(uVar41 >> 0x18);
                if (((!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
                    (fVar42 = *(float *)(this + 0x30) - *(float *)(this + 0x94), in_fpscr = uVar29,
                    fVar42 < fVar45)) &&
                   ((uVar41 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f |
                              (uint)(fVar42 == fVar43) << 0x1e,
                    in_fpscr = uVar41 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                    bVar37 = (byte)(uVar41 >> 0x18),
                    !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1) &&
                    ((fVar42 = *(float *)(this + 0x34) - *(float *)(this + 0x98), in_fpscr = uVar29,
                     fVar42 < fVar45 &&
                     (uVar29 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f |
                               (uint)(fVar42 == fVar43) << 0x1e,
                     in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                     bVar37 = (byte)(uVar29 >> 0x18),
                     !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)))))))
                goto LAB_000ef77a;
              }
            }
          }
        }
      }
      else {
        PVar6 = (PlayerFighter)0x0;
        if (this[0x12d] == (PlayerFighter)0x0) {
          iVar7 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000ed724 + 0xed3f6));
          PVar6 = (PlayerFighter)0x0;
          if (iVar7 < 0x14) {
            PVar6 = (PlayerFighter)0x1;
          }
        }
        iVar7 = DAT_000ed728;
        *(undefined4 *)(this + 0x1b8) = 0;
        this[0x12d] = PVar6;
        piVar15 = *(int **)(iVar7 + 0xed410);
        iVar7 = AbyssEngine::AERandom::nextInt(*piVar15);
        if ((iVar7 < 0x1e) && (1 < *puVar14)) {
          PVar6 = (PlayerFighter)0x0;
          this[0x12e] = (PlayerFighter)0x0;
          for (uVar40 = 0; uVar40 < 5; uVar40 = uVar40 + 1) {
            if (PVar6 != (PlayerFighter)0x0) goto LAB_000ed50e;
            uVar9 = AbyssEngine::AERandom::nextInt(*piVar15);
            *(undefined4 *)(this + 0x38) = uVar9;
            iVar7 = Player::isActive();
            if (iVar7 != 0) {
              Player::getPosition();
              AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
              if ((*(int *)(this + 0x28) != 8) && (iVar7 = Player::turnedEnemy(), iVar7 != 0)) {
LAB_000ef770:
                this[0x12e] = (PlayerFighter)0x1;
                goto LAB_000ed50e;
              }
              fVar42 = *(float *)(this + 0x2c) - *(float *)(this + 0x90);
              fVar45 = (float)VectorSignedToFloat(*(int *)(this + 0x128),
                                                  (byte)(in_fpscr >> 0x16) & 3);
              uVar29 = in_fpscr & 0xfffffff;
              in_fpscr = uVar29;
              if (fVar42 < fVar45) {
                fVar43 = (float)VectorSignedToFloat(-*(int *)(this + 0x128),
                                                    (byte)(uVar29 >> 0x16) & 3);
                uVar41 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e
                ;
                in_fpscr = uVar41 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
                bVar37 = (byte)(uVar41 >> 0x18);
                if ((((!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
                     (fVar42 = *(float *)(this + 0x30) - *(float *)(this + 0x94), in_fpscr = uVar29,
                     fVar42 < fVar45)) &&
                    (uVar41 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f |
                              (uint)(fVar42 == fVar43) << 0x1e,
                    in_fpscr = uVar41 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                    bVar37 = (byte)(uVar41 >> 0x18),
                    !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) &&
                   ((fVar42 = *(float *)(this + 0x34) - *(float *)(this + 0x98), in_fpscr = uVar29,
                    fVar42 < fVar45 &&
                    (uVar29 = uVar29 | (uint)(fVar42 < fVar43) << 0x1f |
                              (uint)(fVar42 == fVar43) << 0x1e,
                    in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                    bVar37 = (byte)(uVar29 >> 0x18),
                    !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)))))
                goto LAB_000ef770;
              }
            }
            PVar6 = this[0x12e];
          }
          if (PVar6 == (PlayerFighter)0x0) goto LAB_000ed50a;
        }
        else {
LAB_000ed50a:
          *(undefined4 *)(this + 0x38) = 0;
        }
LAB_000ed50e:
        if (((this[0xdc] != (PlayerFighter)0x0) && (*(int *)(this + 0xe4) == 3)) &&
           (*(int *)(this + 0xe8) != 0)) {
          iVar7 = KIPlayer::isDead();
          if (iVar7 == 0) {
            for (uVar40 = 0; uVar40 < *puVar14; uVar40 = uVar40 + 1) {
              if (((*(int *)(puVar14[1] + uVar40 * 4) == *(int *)(*(int *)(this + 0xe8) + 4)) &&
                  (iVar7 = Player::isActive(), iVar7 != 0)) &&
                 (iVar7 = Player::isAlwaysFriend(), iVar7 == 0)) {
                *(uint *)(this + 0x38) = uVar40;
                break;
              }
            }
          }
          else {
            (**(code **)(*(int *)this + 0x10))(this,1,0);
          }
        }
        iVar7 = Player::isActive();
        if ((iVar7 == 0) || (iVar7 = Player::isDead(), iVar7 != 0)) {
          this[0x12e] = (PlayerFighter)0x0;
        }
        else {
          Player::getPosition();
          AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
          fVar42 = *(float *)(this + 0x2c) - *(float *)(this + 0x90);
          fVar45 = (float)VectorSignedToFloat(*(int *)(this + 0x128),(byte)(in_fpscr >> 0x16) & 3);
          uVar40 = in_fpscr & 0xfffffff;
          in_fpscr = uVar40;
          if (fVar42 < fVar45) {
            fVar43 = (float)VectorSignedToFloat(-*(int *)(this + 0x128),(byte)(uVar40 >> 0x16) & 3);
            uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e;
            in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
            bVar37 = (byte)(uVar29 >> 0x18);
            if (((!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
                (fVar42 = *(float *)(this + 0x30) - *(float *)(this + 0x94), in_fpscr = uVar40,
                fVar42 < fVar45)) &&
               ((uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f |
                          (uint)(fVar42 == fVar43) << 0x1e,
                in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                bVar37 = (byte)(uVar29 >> 0x18),
                !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1) &&
                ((fVar42 = *(float *)(this + 0x34) - *(float *)(this + 0x98), in_fpscr = uVar40,
                 fVar42 < fVar45 &&
                 (uVar40 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f |
                           (uint)(fVar42 == fVar43) << 0x1e,
                 in_fpscr = uVar40 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
                 bVar37 = (byte)(uVar40 >> 0x18),
                 !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)))))))
            goto LAB_000ed7ac;
          }
        }
        *(undefined4 *)(this + 0x38) = 0xffffffff;
      }
LAB_000ed7ac:
      if ((*(char *)(*(int *)(this + 4) + 0x5c) == '\0') && (*(int *)(this + 0x38) == 0)) {
        this[0x12e] = (PlayerFighter)0x0;
        *(undefined4 *)(this + 0x38) = 1;
      }
      PVar6 = this[0xdc];
      if ((PVar6 == (PlayerFighter)0x0) || (*(int *)(this + 0xe4) != 3)) {
        if (0 < *(int *)(this + 0x38)) {
          *(undefined4 *)(this + 0x38) = 0xffffffff;
          for (uVar40 = 1; uVar40 < *puVar14; uVar40 = uVar40 + 1) {
            iVar7 = Player::isActive();
            if ((iVar7 != 0) && (iVar7 = Player::isDead(), iVar7 == 0)) {
              iVar7 = Player::getKIPlayer(*(Player **)(puVar14[1] + uVar40 * 4));
              this_01 = *(Player **)(puVar14[1] + uVar40 * 4);
              PVar1 = this_01[0x69];
              if (iVar7 == 0) {
                if (PVar1 == (Player)0x0) goto LAB_000ed898;
LAB_000ed826:
                iVar7 = 0;
              }
              else {
                if (PVar1 != (Player)0x0) goto LAB_000ed826;
                iVar7 = Player::getKIPlayer(this_01);
                iVar7 = *(int *)(iVar7 + 0x28);
              }
              PVar6 = this[0xdc];
              if (PVar6 == (PlayerFighter)0x0) {
                iVar31 = *(int *)(this + 0x28);
                if (((((iVar7 == 8) && (iVar31 != 8)) || ((iVar7 != 8 && (iVar31 == 8)))) ||
                    ((iVar7 == 9 && (iVar31 != 9)))) ||
                   (((iVar7 != 9 && (iVar31 == 9)) ||
                    (((((iVar7 == 0 && (iVar31 == 1)) || (iVar7 == 1 && iVar31 == 0)) ||
                      (((iVar7 == 3 && iVar31 == 2 || (iVar7 == 2 && iVar31 == 3)) ||
                       ((iVar7 == 10 && (iVar31 != 10)))))) || ((iVar7 != 10 && (iVar31 == 10)))))))
                   ) goto LAB_000ed8a8;
              }
              else {
                iVar7 = *(int *)(puVar14[1] + uVar40 * 4);
                if ((*(char *)(iVar7 + 0x5c) != '\0') ||
                   ((*(int *)(this + 0xe8) != 0 && (iVar7 == *(int *)(*(int *)(this + 0xe8) + 4)))))
                {
LAB_000ed8a8:
                  this[0x12e] = (PlayerFighter)0x1;
                  *(uint *)(this + 0x38) = uVar40;
                  goto LAB_000ed8b0;
                }
              }
            }
LAB_000ed898:
          }
          PVar6 = this[0xdc];
        }
LAB_000ed8b0:
        if (PVar6 != (PlayerFighter)0x0) goto LAB_000ed8b2;
LAB_000ed8d2:
        this[300] = (PlayerFighter)0x0;
        if (*(int *)(this + 0x38) == -1) goto LAB_000ed8e2;
LAB_000ed8ea:
        iVar7 = Player::doesNeverAttack();
        if (iVar7 != 0) goto LAB_000ed8fc;
        uVar40 = *(uint *)(this + 0x38);
        if (((int)uVar40 < 0) || (puVar14 = (uint *)Player::getEnemies(), *puVar14 <= uVar40)) {
          iVar7 = 0;
          *(undefined4 *)(this + 0x38) = 0;
        }
        else {
          iVar7 = *(int *)(this + 0x38);
        }
        uVar9 = Player::getEnemy(*(Player **)(this + 4),iVar7);
        *(undefined4 *)(this + 0x148) = uVar9;
LAB_000eda5e:
        Player::getPosition();
        AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
        *(undefined4 *)(this + 0x164) = *(undefined4 *)(this + 0x90);
        *(undefined4 *)(this + 0x168) = *(undefined4 *)(this + 0x94);
        *(undefined4 *)(this + 0x16c) = *(undefined4 *)(this + 0x98);
      }
      else {
LAB_000ed8b2:
        if ((this[0x12e] != (PlayerFighter)0x0) || (*(int *)(this + 0xe4) != 1)) goto LAB_000ed8d2;
        this[300] = (PlayerFighter)0x0;
        *(undefined4 *)(this + 0x38) = 0xffffffff;
LAB_000ed8e2:
        if (*(int *)(this + 0x6c) == 0) goto LAB_000ed8ea;
LAB_000ed8fc:
        iVar7 = Route::getWaypoint();
        if ((iVar7 == 0) || (iVar7 = Player::isActive(), iVar7 == 0)) {
          *(undefined4 *)(this + 0x38) = 0;
          iVar7 = Player::getEnemy(*(Player **)(this + 4),0);
          *(int *)(this + 0x148) = iVar7;
          if (iVar7 != 0) goto LAB_000eda5e;
        }
        else {
          pRVar27 = *(Route **)(this + 0x6c);
          AEGeometry::getPosition();
          Route::update(pRVar27,(Vector *)&local_78);
          iVar7 = Route::getWaypoint();
          if (iVar7 != 0) {
            iVar7 = Route::getDockingTarget();
            if (iVar7 == 0) {
              pcVar32 = *(code **)(DAT_000ee278 + 0xedee0);
              iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
              uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x124),
                                          (byte)(in_fpscr >> 0x16) & 3);
              *(undefined4 *)(this + 0x164) = uVar9;
              iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
              uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x128),
                                          (byte)(in_fpscr >> 0x16) & 3);
              *(undefined4 *)(this + 0x168) = uVar9;
              iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x6c));
              uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 300),(byte)(in_fpscr >> 0x16) & 3)
              ;
              *(undefined4 *)(this + 0x16c) = uVar9;
            }
            else {
              if (*(int *)(this + 0x88) != 7) {
                if (*(int *)(this + 0x88) != 1) goto LAB_000ef766;
                *(undefined4 *)(this + 0x88) = 7;
              }
              pVVar13 = (Vector *)Route::getDockingTarget();
              (**(code **)(*(int *)this + 0x28))((SpacePoint *)&local_78,this);
              iVar7 = KIPlayer::getNearestNavigationPoint(pVVar13,(SpacePoint *)&local_78);
              if (iVar7 != 0) {
                if (*(int *)(this + 0x2c4) != iVar7) {
                  if (*(int *)(this + 0x2c4) != 0) {
                    SpacePoint::giveFree();
                  }
                  *(int *)(this + 0x2c4) = iVar7;
                  SpacePoint::take();
                }
                piVar15 = (int *)Route::getDockingTarget();
                (**(code **)(*piVar15 + 0x28))((Vector *)&local_d0,piVar15);
                Route::getDockingTarget();
                pVVar13 = (Vector *)AEGeometry::getMatrix();
                AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_138,pVVar13);
                AbyssEngine::AEMath::operator+((Vector *)&local_78,(Vector *)&local_d0);
                pVVar13 = (Vector *)(this + 0x9c);
                AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)&local_78);
                (**(code **)(*(int *)this + 0x28))(&local_d0,this);
                AbyssEngine::AEMath::operator-((Vector *)&local_78,pVVar13);
                fVar45 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_78);
                if ((int)fVar45 < 2000) {
                  *(undefined4 *)(this + 0x88) = 8;
                  if (*(EaseInOutMatrix **)(this + 0x2b8) != (EaseInOutMatrix *)0x0) {
                    pvVar24 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                                                (*(EaseInOutMatrix **)(this + 0x2b8));
                    operator_delete(pvVar24);
                  }
                  iVar31 = DAT_000ef850;
                  *(undefined4 *)(this + 0x2b8) = 0;
                  pcVar33 = *(code **)(iVar31 + 0xef580);
                  pVVar13 = (Vector *)(*pcVar33)(*(undefined4 *)(this + 0x6c));
                  piVar15 = (int *)(*pcVar33)(*(undefined4 *)(this + 0x6c));
                  (**(code **)(*piVar15 + 0x28))(&local_d0,piVar15);
                  iVar31 = (*pcVar33)(*(undefined4 *)(this + 0x6c));
                  pcVar30 = *(code **)(DAT_000ef854 + 0xef5a2);
                  uVar9 = (*pcVar30)(*(undefined4 *)(iVar31 + 8));
                  pcVar35 = *(code **)(DAT_000ef858 + 0xef5b0);
                  (*pcVar35)(&local_138,uVar9,*(undefined4 *)(this + 0x2c4));
                  AbyssEngine::AEMath::operator+((Vector *)&local_78,(Vector *)&local_d0);
                  iVar31 = KIPlayer::getNearestDockingPoint(pVVar13);
                  iVar16 = (*pcVar33)(*(undefined4 *)(this + 0x6c));
                  uVar9 = (*pcVar30)(*(undefined4 *)(iVar16 + 8));
                  (*pcVar35)(&local_78,uVar9,iVar31 + 0xc);
                  pcVar32 = *(code **)(DAT_000ef85c + 0xef5ea);
                  (*pcVar32)(&local_138,&local_78);
                  iVar16 = (*pcVar33)(*(undefined4 *)(this + 0x6c));
                  uVar9 = (*pcVar30)(*(undefined4 *)(iVar16 + 8));
                  (*pcVar35)(&local_d0,uVar9,iVar7);
                  iVar7 = (*pcVar33)(*(undefined4 *)(this + 0x6c));
                  uVar9 = (*pcVar30)(*(undefined4 *)(iVar7 + 8));
                  (*pcVar35)(&local_94,uVar9,iVar31);
                  AbyssEngine::AEMath::operator-((Vector *)&local_78,(Vector *)&local_d0);
                  (*pcVar32)((Vector *)&local_88,(Vector *)&local_78);
                  AbyssEngine::AEMath::VectorCross((Vector *)&local_78,(Vector *)&local_88);
                  (*pcVar32)((Vector *)&local_94,(Vector *)&local_78);
                  uVar3 = DAT_000ef838._4_4_;
                  uVar2 = (undefined4)DAT_000ef838;
                  uVar46 = DAT_000ef830._4_4_;
                  uVar9 = (undefined4)DAT_000ef830;
                  fStack_5c = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
                  local_58 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
                  local_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
                  puVar18 = (undefined4 *)((uint)&local_78 | 4);
                  *puVar18 = 0;
                  puVar18[1] = fStack_5c;
                  puVar18[2] = local_58;
                  puVar18[3] = local_54;
                  local_60 = 0.0;
                  local_50 = CONCAT44(uVar46,uVar9);
                  local_48 = CONCAT44(uVar3,uVar2);
                  local_78 = 0x3f800000;
                  local_64 = 0x3f800000;
                  local_40 = 0x3f800000;
                  AbyssEngine::AEMath::MatrixSetRotation
                            ((Matrix *)&local_d0,(Vector *)&local_78,(Vector *)&local_94,
                             (Vector *)&local_88);
                  pEVar21 = operator_new(0xf4);
                  iVar7 = *(int *)(this + 4);
                  AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
                            (pEVar21,*(undefined4 *)(iVar7 + 4),*(undefined4 *)(iVar7 + 8),
                             *(undefined4 *)(iVar7 + 0xc),*(undefined4 *)(iVar7 + 0x10),
                             *(undefined4 *)(iVar7 + 0x14),*(undefined4 *)(iVar7 + 0x18),
                             *(undefined4 *)(iVar7 + 0x1c),*(undefined4 *)(iVar7 + 0x20),
                             *(undefined4 *)(iVar7 + 0x24),*(undefined4 *)(iVar7 + 0x28),
                             *(undefined4 *)(iVar7 + 0x2c),*(undefined4 *)(iVar7 + 0x30),
                             *(undefined4 *)(iVar7 + 0x34),*(undefined4 *)(iVar7 + 0x38),
                             *(undefined4 *)(iVar7 + 0x3c),local_78,local_74,local_70,local_6c,
                             local_68,local_64,local_60,fStack_5c,local_58,local_54,(float)local_50,
                             local_50._4_4_,(undefined4)local_48,local_48._4_4_,local_40,3000);
                  *(EaseInOutMatrix **)(this + 0x2b8) = pEVar21;
                }
                else {
                  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x164),pVVar13);
                }
              }
            }
LAB_000ef766:
            this[300] = (PlayerFighter)0x1;
          }
        }
      }
LAB_000eda88:
      if ((this[0xec] == (PlayerFighter)0x0) || (this[300] == (PlayerFighter)0x0)) {
        *(undefined4 *)(this + 0xf0) = 0;
      }
      else {
        iVar7 = *(int *)(this + 0xf0);
        *(int *)(this + 0xf0) = iVar7 + param_1;
        if (19999 < iVar7 + param_1) {
          *(undefined4 *)(this + 0x88) = 6;
          *(undefined4 *)(this + 0xf0) = 0;
          this[300] = (PlayerFighter)0x0;
        }
      }
    }
    if ((this[0xdc] != (PlayerFighter)0x0) && (*(int *)(this + 0xe4) == 2)) {
      pRVar27 = *(Route **)(this + 0x14c);
      if (pRVar27 == (Route *)0x0) {
        iVar7 = *(int *)this;
      }
      else {
        AEGeometry::getPosition();
        Route::update(pRVar27,(Vector *)&local_78);
        iVar7 = Route::getCurrent();
        if (iVar7 <= *(int *)(this + 0x1e4)) {
          iVar7 = Route::getWaypoint();
          if (iVar7 != 0) {
            pcVar32 = *(code **)(DAT_000edec0 + 0xedb2e);
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x14c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x124),(byte)(in_fpscr >> 0x16) & 3)
            ;
            *(undefined4 *)(this + 0x164) = uVar9;
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x14c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 0x128),(byte)(in_fpscr >> 0x16) & 3)
            ;
            *(undefined4 *)(this + 0x168) = uVar9;
            iVar7 = (*pcVar32)(*(undefined4 *)(this + 0x14c));
            uVar9 = VectorSignedToFloat(*(undefined4 *)(iVar7 + 300),(byte)(in_fpscr >> 0x16) & 3);
            this[300] = (PlayerFighter)0x1;
            *(undefined4 *)(this + 0x16c) = uVar9;
          }
          goto LAB_000edb66;
        }
        if (*(Route **)(this + 0x14c) != (Route *)0x0) {
          pvVar24 = (void *)Route::~Route(*(Route **)(this + 0x14c));
          operator_delete(pvVar24);
        }
        iVar7 = *(int *)this;
        *(undefined4 *)(this + 0x14c) = 0;
      }
      (**(code **)(iVar7 + 0x10))(this,1,0);
    }
  }
LAB_000edb66:
  *(float *)(this + 0x170) = *(float *)(this + 0x164) - *(float *)(this + 0x2c);
  *(float *)(this + 0x174) = *(float *)(this + 0x168) - *(float *)(this + 0x30);
  *(float *)(this + 0x178) = *(float *)(this + 0x16c) - *(float *)(this + 0x34);
  if ((*(char *)(*(int *)(this + 4) + 0x5c) != '\0') &&
     (((*(int *)(this + 0x88) == 5 ||
       ((this[0xdc] != (PlayerFighter)0x0 && (*(int *)(this + 0xe4) == 1)))) &&
      (0 < *(int *)(this + 0x128))))) {
    Level::getPlayer();
    iVar7 = PlayerEgo::isInRocketControl();
    if (iVar7 == 0) {
      Level::getPlayer();
      Player::getPosition();
    }
    else {
      Level::getPlayer();
      PlayerEgo::getTargetFollowCamera();
      TargetFollowCamera::getTarget();
      AEGeometry::getPosition();
    }
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
    fVar45 = DAT_000edec4;
    fVar44 = *(float *)(this + 0x90) - *(float *)(this + 0x2c);
    fVar43 = *(float *)(this + 0x94) - *(float *)(this + 0x30);
    fVar42 = *(float *)(this + 0x98) - *(float *)(this + 0x34);
    bVar38 = fVar44 < DAT_000edec4;
    in_fpscr = in_fpscr & 0xfffffff;
    *(float *)(this + 0x170) = fVar44;
    *(float *)(this + 0x174) = fVar43;
    *(float *)(this + 0x178) = fVar42;
    if (bVar38) {
      uVar29 = in_fpscr | (uint)(fVar44 < DAT_000edec8) << 0x1f |
               (uint)(fVar44 == DAT_000edec8) << 0x1e;
      uVar40 = uVar29 | (uint)(NAN(fVar44) || NAN(DAT_000edec8)) << 0x1c;
      bVar37 = (byte)(uVar29 >> 0x18);
      bVar36 = bVar37 >> 7;
      bVar38 = (bool)(bVar37 >> 6 & 1);
      bVar37 = (byte)(uVar40 >> 0x1c) & 1;
      if (!bVar38 && bVar36 == bVar37) {
        uVar40 = in_fpscr | (uint)(fVar42 < DAT_000edec8) << 0x1f |
                 (uint)(fVar42 == DAT_000edec8) << 0x1e |
                 (uint)(NAN(fVar42) || NAN(DAT_000edec8)) << 0x1c;
        bVar37 = (byte)(uVar40 >> 0x18);
        bVar36 = bVar37 >> 7;
        bVar38 = (bool)(bVar37 >> 6 & 1);
        bVar37 = bVar37 >> 4 & 1;
      }
      in_fpscr = uVar40;
      if ((((!bVar38 && bVar36 == bVar37) &&
           (uVar40 = uVar40 & 0xfffffff, in_fpscr = uVar40, fVar43 < fVar45)) &&
          (uVar29 = uVar40 | (uint)(fVar43 < DAT_000edec8) << 0x1f |
                    (uint)(fVar43 == DAT_000edec8) << 0x1e,
          in_fpscr = uVar29 | (uint)(NAN(fVar43) || NAN(DAT_000edec8)) << 0x1c,
          bVar37 = (byte)(uVar29 >> 0x18),
          !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) &&
         (in_fpscr = uVar40, fVar42 < fVar45)) {
        iVar7 = *(int *)(this + 0xc);
        *(undefined4 *)(this + 0x88) = 1;
        if (iVar7 == 0) {
          iVar7 = *(int *)(this + 8);
        }
        AEGeometry::setVisible(SUB41(iVar7,0));
        Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
        in_fpscr = uVar40;
      }
    }
  }
  puVar14 = *(uint **)(this + 0x150);
  if (puVar14 != (uint *)0x0) {
    for (uVar40 = 0; uVar40 < *puVar14; uVar40 = uVar40 + 1) {
      piVar15 = *(int **)(puVar14[1] + uVar40 * 4);
      (**(code **)(*piVar15 + 4))
                (piVar15,*(undefined4 *)(this + 0x2c),*(undefined4 *)(this + 0x30),
                 *(undefined4 *)(this + 0x34));
      puVar14 = *(uint **)(this + 0x150);
    }
  }
  iVar7 = Player::getHitpoints();
  fVar45 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
  if (this[0x1fc] == (PlayerFighter)0x0) {
    uVar9 = Player::getMaxHitpoints();
    fVar42 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
    fVar42 = fVar42 * DAT_000edecc;
    uVar40 = in_fpscr & 0xfffffff;
    uVar29 = uVar40 | (uint)(fVar42 < fVar45) << 0x1f | (uint)(fVar42 == fVar45) << 0x1e;
    in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar45)) << 0x1c;
    bVar37 = (byte)(uVar29 >> 0x18);
    fVar42 = extraout_s1_01;
    fVar43 = DAT_000edecc;
    if ((bool)(bVar37 >> 6 & 1) || bVar37 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) {
      if (this[0x1fc] != (PlayerFighter)0x0) goto LAB_000edc94;
    }
    else {
      fVar45 = *(float *)(*(int *)(DAT_000eded0 + 0xedd00) + 0x28);
      uVar40 = uVar40 | (uint)(fVar45 < 0.0) << 0x1f | (uint)(fVar45 == 0.0) << 0x1e;
      in_fpscr = uVar40 | (uint)NAN(fVar45) << 0x1c;
      bVar37 = (byte)(uVar40 >> 0x18);
      if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        iVar31 = KIPlayer::isDocked();
        if (iVar31 == 0) {
          ParticleSystemManager::enableSystemEmit
                    (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
          iVar31 = *(int *)(this + 0x54);
        }
        else {
          ParticleSystemManager::enableSystemEmit
                    (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
          iVar31 = *(int *)(this + 0x54);
        }
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(iVar31 + 0x84),SUB41(*(undefined4 *)(this + 0x84),0));
        fVar42 = extraout_s1_02;
        fVar43 = extraout_s2_00;
      }
      this[0x1fc] = (PlayerFighter)0x1;
    }
  }
  else {
LAB_000edc94:
    uVar9 = Player::getMaxHitpoints();
    fVar42 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
    in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar42 * DAT_000edecc == fVar45) << 0x1e |
               (uint)(fVar45 <= fVar42 * DAT_000edecc) << 0x1d;
    bVar37 = (byte)(in_fpscr >> 0x18);
    fVar42 = extraout_s1;
    fVar43 = DAT_000edecc;
    if (!(bool)(bVar37 >> 5 & 1) || (bool)(bVar37 >> 6)) {
      this[0x1fc] = (PlayerFighter)0x0;
      ParticleSystemManager::enableSystemEmit
                (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
      ParticleSystemManager::enableSystemEmit
                (*(int *)(*(int *)(this + 0x54) + 0x84),SUB41(*(undefined4 *)(this + 0x84),0));
      fVar42 = extraout_s1_00;
      fVar43 = extraout_s2;
    }
  }
  bVar38 = SUB41(this,0);
  if ((iVar7 < 1) && (1 < *(int *)(this + 0x88) - 3U)) {
    iVar31 = Status::getFreelanceMission();
    if ((iVar31 != 0) && (iVar31 = Mission::getType(), iVar31 == 0xd)) {
      this_02 = (Station *)Status::getStation();
      iVar31 = Station::getIndex(this_02);
      iVar16 = Mission::getTargetStation();
      if ((iVar31 == iVar16) && (iVar31 = *piVar11, *(char *)(iVar31 + 0xf1) == '\0')) {
        if (*(int *)(this + 0x20) != 0) {
          pSVar17 = (String *)GameText::getText(**(int **)(DAT_000eded4 + 0xeddfe));
          iVar16 = AbyssEngine::String::Compare((String *)(this + 0x18),pSVar17);
          iVar31 = *piVar11;
          if (iVar16 == 0) {
            *(undefined1 *)(iVar31 + 0xf0) = 1;
            goto LAB_000edf18;
          }
        }
        *(undefined2 *)(iVar31 + 0xf0) = 0x100;
      }
    }
LAB_000edf18:
    if (this[0x42] != (PlayerFighter)0x0) {
      Wanted::setTerminated
                (SUB41(*(undefined4 *)(*(int *)(*(int *)*piVar11 + 4) + *(int *)(this + 0x48) * 4),0
                      ));
      Wanted::setActive(*(Wanted **)(*(int *)(*(int *)*piVar11 + 4) + *(int *)(this + 0x48) * 4),
                        false);
      iVar31 = **(int **)(DAT_000ee27c + 0xedf5e);
      bVar5 = (bool)Wanted::getReward();
      Layout::showMissionRewardMessage(iVar31,bVar5);
      iVar31 = *piVar11;
      Status::getCredits();
      Wanted::getReward();
      Status::setCredits(iVar31);
      iVar31 = *piVar11;
      Wanted::getBoard();
      Status::incCollectedBounties(iVar31);
    }
    if (*(char *)(*(int *)(this + 4) + 0x5c) == '\0') {
      Level::friendDied();
      if (this[0xdc] != (PlayerFighter)0x0) {
        Level::wingmanDied(*(String **)(this + 0x54));
      }
    }
    else {
      if (*(int *)(this + 0x28) == 9) {
        pAVar22 = operator_new(0xc);
        Array<int>::Array();
        *(Array **)(this + 0x50) = pAVar22;
        ArrayAdd<int>(0x83,pAVar22);
        iVar31 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000ee280 + 0xee01e));
        pAVar22 = *(Array **)(this + 0x50);
        iVar31 = iVar31 + 1;
LAB_000ee028:
        ArrayAdd<int>(iVar31,pAVar22);
      }
      else if (*(int *)(this + 0x28) == 8) {
        if (*(char *)(*(int *)(this + 4) + 0x44) == '\0') {
          Status::incPirateKills();
        }
      }
      else if (((this[0xd0] == (PlayerFighter)0x0) && (this[0x4c] != (PlayerFighter)0x0)) &&
              (*(char *)(*(int *)(DAT_000ee284 + 0xee040) + 0x35) != '\0')) {
        uVar40 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000ee288 + 0xee04e));
        bVar39 = SBORROW4(uVar40,-1);
        iVar31 = uVar40 + 1;
        bVar5 = uVar40 == 0xffffffff;
        if (0x7fffffff < uVar40) {
          iVar16 = *(int *)(this + 0x28);
          bVar39 = SBORROW4(iVar16,3);
          iVar31 = iVar16 + -3;
          bVar5 = iVar16 == 3;
        }
        if (bVar5 || iVar31 < 0 != bVar39) {
          if (*(Array<int> **)(this + 0x50) != (Array<int> *)0x0) {
            pvVar24 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x50));
            operator_delete(pvVar24);
          }
          *(undefined4 *)(this + 0x50) = 0;
          pAVar22 = operator_new(0xc);
          Array<int>::Array();
          *(Array **)(this + 0x50) = pAVar22;
          ArrayAdd<int>(*(int *)(this + 0x28) + 0xbd,pAVar22);
          pAVar22 = *(Array **)(this + 0x50);
          iVar31 = 1;
          goto LAB_000ee028;
        }
      }
      Level::enemyDied(*(int *)(this + 0x54),SUB41(*(undefined4 *)(this + 0xac),0));
    }
    iVar31 = DAT_000ee290;
    puVar18 = (undefined4 *)(DAT_000ee28c + 0xee082);
    *(undefined4 *)(this + 0x88) = 3;
    this[0x73] = (PlayerFighter)0x0;
    puVar18 = (undefined4 *)*puVar18;
    pcVar32 = *(code **)(iVar31 + 0xee088);
    iVar31 = (*pcVar32)(*puVar18,0x5dc);
    *(int *)(this + 0x1f8) = iVar31 + 0x5dc;
    AEGeometry::getDirection();
    fVar45 = (float)AbyssEngine::AEMath::Vector::operator=
                              ((Vector *)(this + 0x194),(Vector *)&local_78);
    local_78 = 0;
    local_74 = 0.0;
    local_70 = 0.0;
    fVar45 = (float)ParticleSystemManager::emitManual
                              (*(int *)(*(int *)(this + 0x54) + 0x74),
                               *(Vector **)(*(int *)(this + 0x54) + 0x3c),(int)pVVar8,(Vector *)0x0,
                               fVar45);
    pVVar13 = pVVar8;
    if (*(char *)(*(int *)(DAT_000ee29c + 0xee0de) + 0xf) == '\0') {
      pVVar13 = (Vector *)0x0;
    }
    FModSound::play(**(int **)(DAT_000ee298 + 0xee0dc),(Vector *)0x14,pVVar13,fVar45);
    iVar31 = (*pcVar32)(*puVar18,200);
    uVar9 = VectorSignedToFloat(iVar31 + -100,(byte)(in_fpscr >> 0x16) & 3);
    iVar31 = (*pcVar32)(*puVar18,200);
    fVar45 = (float)VectorSignedToFloat(iVar31 + -100,(byte)(in_fpscr >> 0x16) & 3);
    iVar31 = (*pcVar32)(*puVar18,200);
    local_c8 = VectorSignedToFloat(iVar31 + -100,(byte)(in_fpscr >> 0x16) & 3);
    local_d0 = uVar9;
    local_cc = fVar45;
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_78,(Vector *)&local_d0);
    fVar45 = (float)AbyssEngine::AEMath::Vector::operator=
                              ((Vector *)(this + 0x188),(Vector *)&local_78);
    AbyssEngine::AEMath::Vector::operator*=((Vector *)(this + 0x188),fVar45);
    pcVar32 = *(code **)(DAT_000ee2a4 + 0xee15c);
    (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x74),*(undefined4 *)(this + 0x1a0),1);
    (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x130),0);
    (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x134),0);
    (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x138),0);
    PVar6 = (PlayerFighter)KIPlayer::cargoAvailable();
    this[0x4c] = PVar6;
    setExhaustVisible(bVar38);
    fVar42 = extraout_s1_03;
    fVar43 = extraout_s2_01;
  }
  uVar47 = DAT_000ee2b8;
  uVar49 = DAT_000ee2b0;
  if (this[0x2e4] != (PlayerFighter)0x0) {
    AEGeometry::updateReferenceMatrix();
    handleCloaking();
    goto switchD_000ee1c4_caseD_2;
  }
  switch(*(undefined4 *)(this + 0x88)) {
  case 0:
    *(undefined4 *)(this + 0x88) = 1;
    break;
  case 1:
  case 7:
    pVVar13 = (Vector *)(this + 0x170);
    handleCloaking();
    if (this[0x13d] != (PlayerFighter)0x0) {
      if (iVar7 < *(int *)(this + 0x1d8)) {
        iVar31 = (*(int *)(this + 0x1d8) - iVar7) + *(int *)(this + 0x1dc);
        *(int *)(this + 0x1d8) = iVar7;
        *(int *)(this + 0x1dc) = iVar31;
        fVar42 = (float)VectorSignedToFloat(iVar31,(byte)(in_fpscr >> 0x16) & 3);
        uVar9 = Player::getMaxHitpoints();
        fVar45 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
        fVar45 = (fVar42 / fVar45) * DAT_000ee2a8;
        uVar40 = in_fpscr & 0xfffffff | (uint)(fVar45 < DAT_000ee2ac) << 0x1f |
                 (uint)(fVar45 == DAT_000ee2ac) << 0x1e;
        in_fpscr = uVar40 | (uint)(NAN(fVar45) || NAN(DAT_000ee2ac)) << 0x1c;
        bVar37 = (byte)(uVar40 >> 0x18);
        if ((bool)(bVar37 >> 6 & 1) || bVar37 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1))
        goto LAB_000ee840;
        *(undefined4 *)(this + 0x1c0) = 10000;
        *(undefined4 *)(this + 0x1dc) = 0;
        this[0x1e0] = (PlayerFighter)0x1;
LAB_000ee84c:
        if (this[500] == (PlayerFighter)0x0) {
          *(undefined4 *)(this + 0x1c0) = 0;
          if ((this[0x1e0] == (PlayerFighter)0x0) &&
             (iVar7 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000eec18 + 0xee86c)),
             *(int *)(this + 0x1b4) <= iVar7)) goto LAB_000ee938;
          iVar7 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000eec1c + 0xee884));
          uVar9 = DAT_000eec20;
          *(int *)(this + 0x1c4) = iVar7 + 5000;
          uVar46 = DAT_000eec24;
          this[500] = (PlayerFighter)0x1;
          *(undefined4 *)(this + 0x1ec) = uVar46;
          *(undefined4 *)(this + 0x1f0) = uVar9;
        }
      }
      else {
LAB_000ee840:
        if (5000 < *(int *)(this + 0x1c0)) goto LAB_000ee84c;
LAB_000ee938:
        if (this[500] == (PlayerFighter)0x0) goto LAB_000ee9c6;
      }
      if (*(int *)(this + 0x1c4) < *(int *)(this + 0x1c0)) {
        fVar45 = *(float *)(this + 0x1ac);
        this[0x1e0] = (PlayerFighter)0x0;
        *(undefined4 *)(this + 0x1c0) = 0;
        *(float *)(this + 0x1ec) = fVar45;
        *(undefined4 *)(this + 0x1f0) = *(undefined4 *)(this + 0x1a4);
      }
      else {
        fVar45 = *(float *)(this + 0x1ec);
      }
      uVar40 = in_fpscr & 0xfffffff;
      uVar29 = uVar40 | (uint)(fVar45 < 0.0) << 0x1f | (uint)(fVar45 == 0.0) << 0x1e;
      in_fpscr = uVar29 | (uint)NAN(fVar45) << 0x1c;
      bVar37 = (byte)(uVar29 >> 0x18);
      if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        pfVar25 = (float *)&DAT_000ef840;
        if (*(float *)(this + 0x1e8) < fVar45) {
          pfVar25 = (float *)&DAT_000ef844;
        }
        fVar42 = *(float *)(this + 0x1e8) * *pfVar25;
        *(float *)(this + 0x1e8) = fVar42;
        uVar29 = uVar40 | (uint)(fVar42 < 5.5) << 0x1f;
        uVar41 = uVar29 | (uint)NAN(fVar42) << 0x1c;
        if (((byte)(uVar29 >> 0x1f) == ((byte)(uVar41 >> 0x1c) & 1)) ||
           (in_fpscr = uVar40, uVar41 = uVar40, fVar42 < *(float *)(this + 0x1ac))) {
          in_fpscr = uVar41 & 0xfffffff | (uint)(fVar45 == *(float *)(this + 0x1ac)) << 0x1e;
          *(float *)(this + 0x1e8) = fVar45;
          if ((byte)(in_fpscr >> 0x1e) != 0) {
            this[500] = (PlayerFighter)0x0;
          }
          *(undefined4 *)(this + 0x1ec) = 0;
        }
      }
    }
LAB_000ee9c6:
    if ((*(int *)(this + 0x148) != 0) && (this[300] == (PlayerFighter)0x0)) {
      if (*(char *)(*(int *)(this + 0x148) + 0x69) == '\0') {
        iVar7 = 8000;
      }
      else {
        iVar31 = Level::getPlayer();
        iVar7 = 8000;
        if (iVar31 != 0) {
          Level::getPlayer();
          iVar31 = PlayerEgo::isDockedToDockingPoint();
          if (iVar31 != 0) {
            iVar7 = 12000;
          }
        }
      }
      fVar45 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
      fVar42 = *(float *)pVVar13;
      uVar40 = in_fpscr & 0xfffffff;
      in_fpscr = uVar40;
      if (fVar42 < fVar45) {
        fVar43 = (float)VectorSignedToFloat(-iVar7,(byte)(uVar40 >> 0x16) & 3);
        uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e;
        in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
        bVar37 = (byte)(uVar29 >> 0x18);
        if ((((!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
             (fVar42 = *(float *)(this + 0x174), in_fpscr = uVar40, fVar42 < fVar45)) &&
            (uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e,
            in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
            bVar37 = (byte)(uVar29 >> 0x18),
            !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) &&
           ((fVar42 = *(float *)(this + 0x178), in_fpscr = uVar40, fVar42 < fVar45 &&
            (uVar40 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e,
            in_fpscr = uVar40 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
            bVar37 = (byte)(uVar40 >> 0x18),
            !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))))) {
          AEGeometry::getRightVector();
          AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)&local_78);
        }
      }
    }
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_88,pVVar13);
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)&local_78);
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_d0,(Vector *)&local_78);
    AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)&local_d0);
    *(float *)(this + 0x174) = -*(float *)(this + 0x174);
    this[0x24] = *(PlayerFighter *)(*(int *)(this + 4) + 0x68);
    if (((this[0x12e] != (PlayerFighter)0x0) && (this[300] == (PlayerFighter)0x0)) &&
       ((*(int *)(this + 0x148) != 0 && (iVar7 = Player::doesNeverAttack(), iVar7 == 0)))) {
      if (*(char *)(*(int *)(this + 0x148) + 0x5e) == '\0') {
        fVar42 = *(float *)(this + 0x170);
        fVar45 = *(float *)(this + 0x1a8);
        uVar40 = in_fpscr & 0xfffffff;
        in_fpscr = uVar40;
        if (fVar42 < fVar45) {
          fVar43 = -fVar45;
          uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e;
          in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
          bVar37 = (byte)(uVar29 >> 0x18);
          if (((((!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) &&
                (fVar42 = *(float *)(this + 0x174), in_fpscr = uVar40, fVar42 < fVar45)) &&
               (uVar29 = uVar40 | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e
               , in_fpscr = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c,
               bVar37 = (byte)(uVar29 >> 0x18),
               !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) &&
              ((fVar45 = *(float *)(this + 0x164) - *(float *)(this + 0x2c), in_fpscr = uVar40,
               fVar45 < DAT_000ef848 &&
               (uVar29 = uVar40 | (uint)(fVar45 < DAT_000ef84c) << 0x1f |
                         (uint)(fVar45 == DAT_000ef84c) << 0x1e,
               in_fpscr = uVar29 | (uint)(NAN(fVar45) || NAN(DAT_000ef84c)) << 0x1c,
               bVar37 = (byte)(uVar29 >> 0x18),
               !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))))) &&
             (((fVar45 = *(float *)(this + 0x168) - *(float *)(this + 0x30), in_fpscr = uVar40,
               fVar45 < DAT_000ef848 &&
               ((uVar29 = uVar40 | (uint)(fVar45 < DAT_000ef84c) << 0x1f |
                          (uint)(fVar45 == DAT_000ef84c) << 0x1e,
                in_fpscr = uVar29 | (uint)(NAN(fVar45) || NAN(DAT_000ef84c)) << 0x1c,
                bVar37 = (byte)(uVar29 >> 0x18),
                !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1) &&
                (fVar45 = *(float *)(this + 0x16c) - *(float *)(this + 0x34), in_fpscr = uVar40,
                fVar45 < DAT_000ef848)))) &&
              (uVar40 = uVar40 | (uint)(fVar45 < DAT_000ef84c) << 0x1f |
                        (uint)(fVar45 == DAT_000ef84c) << 0x1e,
              in_fpscr = uVar40 | (uint)(NAN(fVar45) || NAN(DAT_000ef84c)) << 0x1c,
              bVar37 = (byte)(uVar40 >> 0x18),
              !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))))) {
            if (((this[0x25] == (PlayerFighter)0x0) || (iVar7 = Player::isActive(), iVar7 == 0)) ||
               (iVar7 = Player::isDead(), iVar7 != 0)) {
LAB_000ef550:
              this[0x12e] = (PlayerFighter)0x0;
            }
            else {
              if ((*(char *)(*(int *)(this + 0x148) + 0x69) != '\0') &&
                 (iVar7 = Level::getPlayer(), iVar7 != 0)) {
                Level::getPlayer();
                iVar7 = PlayerEgo::isDockedToDockingPoint();
                if (iVar7 != 0) {
                  Player::getPosition();
                  fVar45 = local_74;
                  Player::getPosition();
                  uVar40 = in_fpscr & 0xfffffff | (uint)(fVar45 < local_cc) << 0x1f |
                           (uint)(fVar45 == local_cc) << 0x1e;
                  in_fpscr = uVar40 | (uint)(NAN(fVar45) || NAN(local_cc)) << 0x1c;
                  bVar37 = (byte)(uVar40 >> 0x18);
                  if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))
                  goto LAB_000ef550;
                }
              }
              Player::shoot(*(int *)(this + 4),(longlong)param_1,false);
              this[0x2b5] = (PlayerFighter)0x1;
            }
          }
        }
      }
      else {
        this[0x12e] = (PlayerFighter)0x0;
        if (this[0x2b5] != (PlayerFighter)0x0) {
          Player::stopShooting(*(int *)(this + 4));
          this[0x2b5] = (PlayerFighter)0x0;
        }
      }
    }
    uVar9 = AEGeometry::getMatrix();
    __aeabi_memcpy(&local_78,uVar9,0x3c);
    if (this[0x12d] == (PlayerFighter)0x0) {
      local_130 = local_80;
      local_138 = local_88;
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_94);
      AbyssEngine::AEMath::Vector::operator-=((Vector *)&local_88,(Vector *)&local_94);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,(Vector *)&local_88);
      pcVar32 = *(code **)(DAT_000ef0f0 + 0xeeda4);
      (*pcVar32)((Vector *)&local_88,(AEMath *)&local_d0);
      fVar45 = (float)VectorSignedToFloat(param_1 * 0x30,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_88,fVar45 * DAT_000eec38);
      AbyssEngine::AEMath::operator+((Vector *)&local_d0,(Vector *)&local_88);
      (*pcVar32)(pVVar13,(Vector *)&local_d0);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,pVVar13);
      (*pcVar32)(pVVar13,(AEMath *)&local_d0);
      fVar42 = *(float *)(this + 0x178) - local_130;
      uVar47 = FloatVectorSub(*(undefined8 *)pVVar13,local_138,2,0x20);
      uVar49 = FloatVectorCompareGreaterThan(uVar47,0,2);
      uVar48 = FloatVectorNeg(uVar47,2,2);
      uVar49 = VectorBitwiseSelect(uVar49,uVar47,uVar48);
      in_fpscr = in_fpscr & 0xfffffff;
      fVar45 = -fVar42;
      if (0.0 < fVar42) {
        fVar45 = fVar42;
      }
      if ((float)uVar49 + (float)((ulonglong)uVar49 >> 0x20) + fVar45 < DAT_000ef0f4) {
        AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)&local_138);
      }
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,(Vector *)&local_94);
      fVar42 = (float)AbyssEngine::AEMath::VectorDot((Vector *)&local_d0,pVVar13);
      in_fpscr = in_fpscr & 0xfffffff;
      fVar45 = DAT_000eec34;
      if ((fVar42 < 1.0) &&
         (uVar40 = in_fpscr | (uint)(fVar42 < -1.0) << 0x1f | (uint)(fVar42 == -1.0) << 0x1e,
         in_fpscr = uVar40 | (uint)NAN(fVar42) << 0x1c, bVar37 = (byte)(uVar40 >> 0x18),
         !(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) {
        fVar45 = (float)AbyssEngine::AEMath::ACosf(DAT_000eec34);
      }
      fVar42 = -fVar45;
      if (0.0 < fVar45) {
        fVar42 = fVar45;
      }
      in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar42 == 0.0) << 0x1e;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        AbyssEngine::AEMath::MatrixGetRight((Matrix *)&local_d0);
        local_e0 = *(undefined8 *)pVVar13;
        local_d8 = *(undefined4 *)(this + 0x178);
        fVar45 = (float)AbyssEngine::AEMath::VectorDot((Vector *)&local_d0,(Vector *)&local_e0);
        fVar45 = (float)AbyssEngine::AEMath::ACosf(fVar45);
        in_fpscr = in_fpscr & 0xfffffff;
        if (fVar45 < DAT_000ef0f8) {
          fVar42 = -fVar42;
        }
      }
      iVar7 = *(int *)(this + 0x2b0);
      *(float *)(this + iVar7 * 4 + 0x29c) = fVar42;
      fVar45 = DAT_000eec34;
      if (this[0x2b4] == (PlayerFighter)0x0) {
        if (iVar7 != 0) {
          pPVar10 = this + 0x29c;
          for (iVar31 = 0; iVar31 < iVar7; iVar31 = iVar31 + 1) {
            fVar42 = *(float *)pPVar10;
            pPVar10 = pPVar10 + 4;
            fVar45 = fVar45 + fVar42;
          }
          fVar42 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
          goto LAB_000eef7a;
        }
        *(undefined4 *)(this + 0x2b0) = 1;
      }
      else {
        for (iVar31 = 0; iVar31 != 0x14; iVar31 = iVar31 + 4) {
          fVar45 = fVar45 + *(float *)(this + iVar31 + 0x29c);
        }
        fVar42 = 5.0;
LAB_000eef7a:
        fVar42 = fVar45 / fVar42;
        *(int *)(this + 0x2b0) = iVar7 + 1;
        if ((3 < iVar7) && (*(undefined4 *)(this + 0x2b0) = 0, this[0x2b4] == (PlayerFighter)0x0)) {
          this[0x2b4] = (PlayerFighter)0x1;
        }
      }
      fVar45 = *(float *)(this + 0x294);
      fVar42 = fVar42 * fVar45 * *(float *)(this + 0x298);
      in_fpscr = in_fpscr & 0xfffffff;
      uVar40 = in_fpscr | (uint)(fVar42 < fVar45) << 0x1f | (uint)(fVar42 == fVar45) << 0x1e;
      uVar29 = uVar40 | (uint)(NAN(fVar42) || NAN(fVar45)) << 0x1c;
      *(float *)(this + 0x20c) = fVar42;
      bVar37 = (byte)(uVar40 >> 0x18);
      if ((bool)(bVar37 >> 6 & 1) || bVar37 >> 7 != ((byte)(uVar29 >> 0x1c) & 1)) {
        if (fVar42 < -fVar45) {
          *(float *)(this + 0x20c) = -fVar45;
        }
      }
      else {
        *(float *)(this + 0x20c) = fVar45;
        in_fpscr = uVar29;
      }
      AbyssEngine::AEMath::MatrixGetUp((Matrix *)&local_d0);
      AbyssEngine::AEMath::VectorCross((Vector *)&local_e0,(Vector *)&local_d0);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,(Vector *)&local_e0);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)&local_d0);
      AbyssEngine::AEMath::VectorCross(aVStack_ec,pVVar13);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,aVStack_ec);
      AbyssEngine::AEMath::Vector::operator=(aVStack_ec,(Vector *)&local_d0);
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)&local_d0,(Vector *)&local_78,(Vector *)&local_e0,aVStack_ec);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_78,(Matrix *)&local_d0);
      fVar45 = extraout_s1_07;
    }
    else {
      *(undefined4 *)(this + 0x2b0) = 0;
      *(undefined4 *)(this + 0x20c) = 0;
      this[0x2b4] = (PlayerFighter)0x0;
      fVar45 = extraout_s1_06;
    }
    if (this[0x100] != (PlayerFighter)0x0) {
      roll((int)this);
      fVar45 = extraout_s1_08;
    }
    fVar43 = *(float *)(this + 0x20c);
    fVar42 = *(float *)(this + 0x210);
    in_fpscr = in_fpscr & 0xfffffff;
    uVar40 = in_fpscr | (uint)(fVar42 == fVar43) << 0x1e;
    if ((byte)(uVar40 >> 0x1e) == 0) {
      fVar44 = (float)VectorSignedToFloat(param_1,(byte)(uVar40 >> 0x16) & 3);
      uVar29 = in_fpscr | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e;
      uVar40 = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
      fVar44 = (fVar44 * 1.25) / DAT_000ef0fc;
      if ((int)uVar29 < 0) {
        fVar42 = fVar44 + fVar42;
        uVar29 = in_fpscr | (uint)(fVar42 < fVar43) << 0x1f | (uint)(fVar42 == fVar43) << 0x1e;
        uVar40 = uVar29 | (uint)(NAN(fVar42) || NAN(fVar43)) << 0x1c;
        *(float *)(this + 0x210) = fVar42;
        bVar37 = (byte)(uVar29 >> 0x18);
        in_fpscr = uVar40;
        if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(uVar40 >> 0x1c) & 1)) {
LAB_000ef0ac:
          *(float *)(this + 0x210) = fVar43;
          uVar40 = in_fpscr;
          fVar42 = fVar43;
        }
      }
      else if (!(bool)((byte)(uVar29 >> 0x1e) & 1) && ((byte)(uVar40 >> 0x1c) & 1) == 0) {
        fVar42 = fVar42 - fVar44;
        *(float *)(this + 0x210) = fVar42;
        uVar40 = in_fpscr;
        if (fVar42 < fVar43) goto LAB_000ef0ac;
      }
    }
    iVar7 = *(int *)(this + 0xfc);
    if (0 < iVar7) {
      iVar7 = iVar7 - param_1;
      *(int *)(this + 0xfc) = iVar7;
      if (iVar7 < 1) {
        this[0x100] = (PlayerFighter)0x1;
      }
    }
    uVar40 = uVar40 & 0xfffffff | (uint)(fVar43 == fVar42) << 0x1e;
    if ((byte)(uVar40 >> 0x1e) == 0) {
      *(undefined4 *)(this + 0xfc) = 0x2ee;
    }
    else if ((0 < iVar7) && (*(int *)(this + 0xfc) = iVar7 - param_1, iVar7 - param_1 < 1)) {
      this[0x100] = (PlayerFighter)0x1;
    }
    fVar43 = DAT_000ef104;
    uStack_b4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_b0 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_ac = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    fVar42 = fVar42 * DAT_000ef100;
    local_a8 = CONCAT44(DAT_000ef830._4_4_,(undefined4)DAT_000ef830);
    uStack_a0 = CONCAT44(DAT_000ef838._4_4_,(undefined4)DAT_000ef838);
    puVar18 = (undefined4 *)((uint)&local_d0 | 4);
    *puVar18 = 0;
    puVar18[1] = uStack_b4;
    puVar18[2] = uStack_b0;
    puVar18[3] = uStack_ac;
    local_b8 = 0;
    local_d0 = 0x3f800000;
    local_bc = 0x3f800000;
    local_98 = 0x3f800000;
    AbyssEngine::AEMath::MatrixSetRotation((Matrix *)&local_138,fVar42 * fVar43,fVar45,fVar43);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_d0,(Matrix *)&local_138);
    if (*(Matrix **)(this + 0xc) != (Matrix *)0x0) {
      AEGeometry::setMatrix(*(Matrix **)(this + 0xc));
    }
    if (*(char *)(*(int *)(this + 4) + 0x68) == '\0') {
      fVar45 = (float)VectorSignedToFloat(param_1,(byte)(uVar40 >> 0x16) & 3);
      fVar45 = (float)VectorSignedToFloat((int)(*(float *)(this + 0x1e8) * fVar45),
                                          (byte)(uVar40 >> 0x16) & 3);
      local_6c = local_6c + local_70 * fVar45;
      fStack_5c = fStack_5c + local_60 * fVar45;
      local_50 = CONCAT44(local_50._4_4_ + (float)local_50 * fVar45,(float)local_50);
      if (this[0x100] != (PlayerFighter)0x0) {
        AbyssEngine::AEMath::Matrix::operator*=((Matrix *)&local_78,(Matrix *)(this + 600));
      }
      AbyssEngine::PaintCanvas::TransformSetLocal
                (**(uint **)(DAT_000ef46c + 0xef204),*(Matrix **)(*(int *)(this + 8) + 0xc));
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),(Matrix *)&local_78)
      ;
      if (*(int *)(this + 0xc) != 0) {
        pMVar12 = (Matrix *)AEGeometry::getMatrix();
        AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(*(int *)(this + 4) + 4),pMVar12);
      }
      pcVar32 = *(code **)(DAT_000ef470 + 0xef232);
      (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x130),0);
      (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x134),0);
      iVar7 = *(int *)(this + 0x54);
      uVar9 = 0;
    }
    else {
      pcVar32 = *(code **)(DAT_000ef468 + 0xef182);
      (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x130),1);
      (*pcVar32)(*(undefined4 *)(*(int *)(this + 0x54) + 0x8c),*(undefined4 *)(this + 0x134),1);
      iVar7 = *(int *)(this + 0x54);
      uVar9 = 1;
    }
    (*pcVar32)(*(undefined4 *)(iVar7 + 0x8c),*(undefined4 *)(this + 0x138),uVar9);
    AEGeometry::updateReferenceMatrix();
    if (this[0x13e] != (PlayerFighter)0x0) {
      puVar14 = (uint *)Level::getLandmarks();
      if (puVar14 != (uint *)0x0) {
        for (uVar29 = 0; uVar29 < *puVar14; uVar29 = uVar29 + 1) {
          piVar11 = *(int **)(puVar14[1] + uVar29 * 4);
          if ((piVar11 != (int *)0x0) &&
             (iVar7 = (**(code **)(*piVar11 + 0x40))(piVar11,pVVar8), iVar7 != 0)) {
            piVar11 = *(int **)(puVar14[1] + uVar29 * 4);
            (**(code **)(*piVar11 + 0x50))((Vector *)&local_138,piVar11,pVVar8);
            local_94 = 0;
            local_90 = 0;
            local_8c = 0;
            iVar7 = AbyssEngine::AEMath::operator!=((Vector *)&local_138,(Vector *)&local_94);
            if (iVar7 != 0) {
              AEGeometry::getDirection();
              pVVar13 = (Vector *)(this + 0x90);
              pcVar32 = *(code **)(DAT_000ef474 + 0xef2da);
              (*pcVar32)(pVVar13,&local_94);
              AbyssEngine::AEMath::Vector::operator-=((Vector *)&local_138,pVVar13);
              AbyssEngine::AEMath::Vector::operator*=
                        ((Vector *)&local_138,*(float *)(this + 0x1e8) * DAT_000ef478);
              AbyssEngine::AEMath::operator+((Vector *)&local_94,pVVar13);
              pVVar13 = (Vector *)(this + 0x9c);
              (*pcVar32)(pVVar13,(Vector *)&local_94);
              AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_94,pVVar13);
              (*pcVar32)(pVVar13,(AEMath *)&local_94);
              local_90 = 0x3f800000;
              local_94 = 0;
              local_8c = 0;
              AEGeometry::setDirection(*(Vector **)(this + 8),pVVar13);
              fVar45 = (float)VectorSignedToFloat(param_1,(byte)(uVar40 >> 0x16) & 3);
              AEGeometry::moveForward(*(float *)(this + 0x1e8) * fVar45);
              break;
            }
          }
        }
      }
      puVar14 = (uint *)Level::getEnemies();
      if (puVar14 != (uint *)0x0) {
        for (uVar29 = 0; uVar29 < *puVar14; uVar29 = uVar29 + 1) {
          piVar11 = *(int **)(puVar14[1] + uVar29 * 4);
          if ((piVar11 != (int *)0x0) &&
             (iVar7 = (**(code **)(*piVar11 + 0x40))(piVar11,pVVar8), iVar7 != 0)) {
            piVar11 = *(int **)(puVar14[1] + uVar29 * 4);
            (**(code **)(*piVar11 + 0x50))((Vector *)&local_138,piVar11,pVVar8);
            local_94 = 0;
            local_90 = 0;
            local_8c = 0;
            iVar7 = AbyssEngine::AEMath::operator!=((Vector *)&local_138,(Vector *)&local_94);
            if (iVar7 != 0) {
              AEGeometry::getDirection();
              pVVar8 = (Vector *)(this + 0x90);
              pcVar32 = *(code **)(DAT_000ef47c + 0xef3c8);
              (*pcVar32)(pVVar8,&local_94);
              AbyssEngine::AEMath::Vector::operator-=((Vector *)&local_138,pVVar8);
              AbyssEngine::AEMath::Vector::operator*=
                        ((Vector *)&local_138,*(float *)(this + 0x1e8) * DAT_000ef478);
              AbyssEngine::AEMath::operator+((Vector *)&local_94,pVVar8);
              pVVar8 = (Vector *)(this + 0x9c);
              (*pcVar32)(pVVar8,(Vector *)&local_94);
              AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_94,pVVar8);
              (*pcVar32)(pVVar8,(AEMath *)&local_94);
              local_90 = 0x3f800000;
              local_94 = 0;
              local_8c = 0;
              AEGeometry::setDirection(*(Vector **)(this + 8),pVVar8);
              fVar45 = (float)VectorSignedToFloat(param_1,(byte)(uVar40 >> 0x16) & 3);
              AEGeometry::moveForward(*(float *)(this + 0x1e8) * fVar45);
              break;
            }
          }
        }
      }
    }
    (**(code **)(*(int *)this + 0x30))(this,param_1);
    break;
  case 3:
    fStack_5c = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    local_58 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    local_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    this[0x101] = (PlayerFighter)0x0;
    this[0x13e] = (PlayerFighter)0x0;
    puVar18 = (undefined4 *)((uint)&local_78 | 4);
    *puVar18 = 0;
    puVar18[1] = fStack_5c;
    puVar18[2] = local_58;
    puVar18[3] = local_54;
    local_60 = 0.0;
    local_50 = uVar49;
    local_48 = uVar47;
    local_78 = 0x3f800000;
    local_64 = 0x3f800000;
    local_40 = 0x3f800000;
    AbyssEngine::AEMath::MatrixSetRotation((Matrix *)&local_d0,*(float *)(this + 400),fVar42,fVar43)
    ;
    if (0 < param_1) {
      pMVar28 = *(Matrix **)(this + 8);
      pMVar12 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::operator*((Matrix *)&local_d0,pMVar12);
      AEGeometry::setMatrix(pMVar28);
    }
    fVar45 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    pVVar13 = *(Vector **)(this + 8);
    fVar45 = (float)AbyssEngine::AEMath::operator*((Vector *)&local_138,fVar45);
    AbyssEngine::AEMath::operator*((Vector *)&local_d0,fVar45);
    AEGeometry::translate(pVVar13);
    AEGeometry::updateReferenceMatrix();
    iVar7 = *(int *)(this + 0x1f8);
    *(int *)(this + 0x1f8) = iVar7 - param_1;
    if (0x7fffffff < (uint)(iVar7 - param_1)) {
      local_d0 = 0;
      local_cc = 0.0;
      local_c8 = 0;
      Explosion::start(*(Vector **)(this + 0x124),pVVar8);
      ParticleSystemManager::enableSystemEmit
                (*(int *)(*(int *)(this + 0x54) + 0x74),SUB41(*(undefined4 *)(this + 0x1a0),0));
      fVar45 = *(float *)(*(int *)(DAT_000ee6ec + 0xee3a6) + 0x28);
      uVar40 = in_fpscr & 0xfffffff | (uint)(fVar45 < 0.0) << 0x1f | (uint)(fVar45 == 0.0) << 0x1e;
      uVar29 = uVar40 | (uint)NAN(fVar45) << 0x1c;
      bVar37 = (byte)(uVar40 >> 0x18);
      if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(uVar29 >> 0x1c) & 1)) {
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x84),SUB41(*(undefined4 *)(this + 0x84),0));
      }
      puVar18 = *(undefined4 **)(DAT_000ee6f0 + 0xee3da);
      pcVar32 = *(code **)(DAT_000ee6f4 + 0xee3e0);
      uVar9 = (*pcVar32)(*puVar18,0x32);
      fVar45 = (float)VectorSignedToFloat(uVar9,(byte)(uVar29 >> 0x16) & 3);
      Player::setBombForce(fVar45);
      iVar7 = (*pcVar32)(*puVar18,200);
      uVar9 = VectorSignedToFloat(iVar7 + -100,(byte)(uVar29 >> 0x16) & 3);
      iVar7 = (*pcVar32)(*puVar18,200);
      uVar46 = VectorSignedToFloat(iVar7 + -100,(byte)(uVar29 >> 0x16) & 3);
      iVar7 = (*pcVar32)(*puVar18,200);
      local_130 = (float)VectorSignedToFloat(iVar7 + -100,(byte)(uVar29 >> 0x16) & 3);
      local_138 = CONCAT44(uVar46,uVar9);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,(Vector *)&local_138);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x17c),(Vector *)&local_d0);
      *(undefined4 *)(this + 0x1f8) = 0;
      *(undefined4 *)(this + 0x88) = 4;
      if ((*(int *)(this + 0x2c4) != 0) && (iVar7 = SpacePoint::isFree(), iVar7 == 0)) {
        SpacePoint::giveFree();
        *(undefined4 *)(this + 0x2c4) = 0;
      }
      if (this[0x4c] != (PlayerFighter)0x0) {
        if ((this[0xd0] == (PlayerFighter)0x0) || (this[0x69] != (PlayerFighter)0x0)) {
          KIPlayer::createCrate((int)this);
        }
        else {
          this[0x4c] = (PlayerFighter)0x0;
          this[0x68] = (PlayerFighter)0x1;
        }
      }
    }
    break;
  case 4:
    iVar7 = *(int *)(this + 0x1f8);
    *(int *)(this + 0x1f8) = iVar7 + param_1;
    if (iVar7 + param_1 < 1) {
      fStack_5c = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      local_58 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      local_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      local_50 = CONCAT44(DAT_000ef830._4_4_,(undefined4)DAT_000ef830);
      local_48 = CONCAT44(DAT_000ef838._4_4_,(undefined4)DAT_000ef838);
      puVar18 = (undefined4 *)((uint)&local_78 | 4);
      *puVar18 = 0;
      puVar18[1] = fStack_5c;
      puVar18[2] = local_58;
      puVar18[3] = local_54;
      local_60 = 0.0;
      local_78 = 0x3f800000;
      local_64 = 0x3f800000;
      local_40 = 0x3f800000;
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)&local_d0,*(float *)(this + 400),fVar42,fVar43);
      if (0 < param_1) {
        pMVar28 = *(Matrix **)(this + 8);
        pMVar12 = (Matrix *)AEGeometry::getMatrix();
        AbyssEngine::AEMath::operator*((Matrix *)&local_d0,pMVar12);
        AEGeometry::setMatrix(pMVar28);
      }
      fVar45 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      pVVar8 = *(Vector **)(this + 8);
      fVar45 = (float)AbyssEngine::AEMath::operator*((Vector *)&local_138,fVar45);
      AbyssEngine::AEMath::operator*((Vector *)&local_d0,fVar45);
      AEGeometry::translate(pVVar8);
      AEGeometry::updateReferenceMatrix();
    }
    else {
      this[0x73] = (PlayerFighter)0x0;
    }
    AEGeometry::updateReferenceMatrix();
    this_03 = *(Explosion **)(this + 0x124);
    iVar7 = Level::getPlayer();
    if (iVar7 == 0) {
      pTVar20 = (TargetFollowCamera *)0x0;
    }
    else {
      Level::getPlayer();
      pTVar20 = (TargetFollowCamera *)PlayerEgo::getTargetFollowCamera();
    }
    Explosion::update(this_03,param_1,pTVar20);
    *(int *)(this + 0xd8) = *(int *)(this + 0xd8) + param_1;
    if (((this[0x4c] == (PlayerFighter)0x0) || (iVar7 = Player::isActive(), iVar7 == 0)) ||
       ((param_1 < 1 || (*(int *)(this + 0x78) == 0)))) {
      iVar7 = Explosion::isPlaying();
      if (iVar7 != 0) break;
      if ((this[0xd0] != (PlayerFighter)0x0) && (this[0x69] == (PlayerFighter)0x0)) {
        this[0x68] = (PlayerFighter)0x1;
      }
      if (60000 < *(int *)(this + 0xd8)) goto LAB_000eec6c;
    }
    else {
      fVar42 = (float)Player::getBombForce();
      uVar29 = in_fpscr & 0xfffffff | (uint)(fVar42 < 0.0) << 0x1f | (uint)(fVar42 == 0.0) << 0x1e;
      uVar40 = uVar29 | (uint)NAN(fVar42) << 0x1c;
      bVar37 = (byte)(uVar29 >> 0x18);
      fVar45 = extraout_s1_04;
      if (!(bool)(bVar37 >> 6 & 1) && bVar37 >> 7 == ((byte)(uVar40 >> 0x1c) & 1)) {
        AbyssEngine::AEMath::operator*((Vector *)&local_78,extraout_s0);
        AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_78);
        AEGeometry::translate(*(Vector **)(this + 0x78));
        AEGeometry::translate(*(Vector **)(this + 8));
        uVar40 = uVar40 & 0xfffffff;
        fVar45 = fVar42 * DAT_000eec2c;
        if (fVar42 * DAT_000eec2c < DAT_000eec30) {
          fVar45 = DAT_000eec34;
        }
        Player::setBombForce(fVar45);
        fVar45 = extraout_s1_05;
      }
      fVar42 = (float)VectorSignedToFloat((uint)param_1 >> 1,(byte)(uVar40 >> 0x16) & 3);
      fVar42 = (float)VectorSignedToFloat((int)(fVar42 * DAT_000eec38 * DAT_000eec3c),
                                          (byte)(uVar40 >> 0x16) & 3);
      AEGeometry::rotate(fVar42,fVar45,DAT_000eec3c);
      pMVar12 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar12);
      if ((*(int *)(this + 0xd8) < 0xea61) || (iVar7 = Explosion::isPlaying(), iVar7 != 0)) break;
      iVar7 = Level::getPlayer();
      if ((iVar7 != 0) &&
         ((iVar7 = Level::getPlayer(), *(int *)(iVar7 + 0x14) != 0 &&
          (iVar7 = Level::getPlayer(), *(PlayerFighter **)(*(int *)(iVar7 + 0x14) + 0x1c) == this)))
         ) {
        iVar7 = Level::getPlayer();
        *(undefined4 *)(*(int *)(iVar7 + 0x14) + 0x1c) = 0;
      }
      if ((this[0xd0] != (PlayerFighter)0x0) && (this[0x69] == (PlayerFighter)0x0)) {
        this[0x68] = (PlayerFighter)0x1;
      }
      if (*(AEGeometry **)(this + 0x78) != (AEGeometry *)0x0) {
        pvVar24 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x78));
        operator_delete(pvVar24);
      }
      *(undefined4 *)(this + 0xd8) = 0;
      *(undefined4 *)(this + 0x78) = 0;
LAB_000eec6c:
      KIPlayer::setActive(bVar38);
    }
    this[0x101] = (PlayerFighter)0x1;
    break;
  case 5:
    if ((*(char *)(*(int *)(this + 4) + 0x5c) != '\0') &&
       (iVar7 = Status::getCurrentCampaignMission(), 1 < iVar7)) {
      iVar7 = *(int *)(this + 0xc);
      if (iVar7 == 0) {
        iVar7 = *(int *)(this + 8);
      }
      AEGeometry::setVisible(SUB41(iVar7,0));
    }
    if ((*(int *)(this + 0x148) != 0) && (*(char *)(*(int *)(this + 0x148) + 0x5e) == '\0')) {
      fVar45 = (float)VectorSignedToFloat(*(int *)(this + 0x128),(byte)(in_fpscr >> 0x16) & 3);
      if (((*(float *)(this + 0x170) < fVar45) &&
          (((fVar42 = (float)VectorSignedToFloat(-*(int *)(this + 0x128),
                                                 (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3),
            fVar42 < *(float *)(this + 0x170) &&
            ((int)((uint)(*(float *)(this + 0x174) < fVar45) << 0x1f) < 0)) &&
           (fVar42 < *(float *)(this + 0x174))))) &&
         (((int)((uint)(*(float *)(this + 0x178) < fVar45) << 0x1f) < 0 &&
          (fVar42 < *(float *)(this + 0x178))))) {
        iVar7 = *(int *)(this + 0xc);
        *(undefined4 *)(this + 0x88) = 1;
        if (iVar7 == 0) {
          iVar7 = *(int *)(this + 8);
        }
        AEGeometry::setVisible(SUB41(iVar7,0));
        Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
        if (this[0x12f] != (PlayerFighter)0x0) {
          Level::pirateStationAction(SUB41(*(undefined4 *)(this + 0x54),0));
        }
      }
    }
    break;
  case 6:
    fVar42 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    fVar45 = *(float *)(this + 0x1e8) * DAT_000ee700;
    VectorSignedToFloat((int)(fVar45 * fVar42),(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x1e8) = fVar45;
    AEGeometry::moveForward(fVar45);
    if (DAT_000ee2a8 < *(float *)(this + 0x1e8)) {
      KIPlayer::setDead();
    }
    break;
  case 8:
    pcVar32 = *(code **)(DAT_000ee704 + 0xee5e6);
    pVVar8 = (Vector *)(*pcVar32)(*(undefined4 *)(this + 0x6c));
    piVar11 = (int *)(*pcVar32)(*(undefined4 *)(this + 0x6c));
    (**(code **)(*piVar11 + 0x28))((Vector *)&local_d0,piVar11);
    (*pcVar32)(*(undefined4 *)(this + 0x6c));
    pVVar13 = (Vector *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_138,pVVar13);
    AbyssEngine::AEMath::operator+((Vector *)&local_78,(Vector *)&local_d0);
    KIPlayer::getNearestDockingPoint(pVVar8);
    piVar11 = (int *)(*pcVar32)(*(undefined4 *)(this + 0x6c));
    (**(code **)(*piVar11 + 0x28))((Vector *)&local_d0,piVar11);
    (*pcVar32)(*(undefined4 *)(this + 0x6c));
    pVVar8 = (Vector *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_138,pVVar8);
    AbyssEngine::AEMath::operator+((Vector *)&local_78,(Vector *)&local_d0);
    pVVar8 = (Vector *)(this + 0x9c);
    AbyssEngine::AEMath::Vector::operator=(pVVar8,(Vector *)&local_78);
    (**(code **)(*(int *)this + 0x28))(&local_d0,this);
    AbyssEngine::AEMath::operator-((Vector *)&local_78,pVVar8);
    fVar45 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_78);
    if ((int)fVar45 < *(int *)(DAT_000ee708 + 0xee69c + *(int *)(this + 0xac) * 4)) {
      setExhaustVisible(bVar38);
      *(undefined4 *)(this + 700) = 0;
      *(undefined4 *)(this + 0x2c0) = 0;
      *(undefined4 *)(this + 0x88) = 9;
      if (this[0x1fc] != (PlayerFighter)0x0) {
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x84),SUB41(*(undefined4 *)(this + 0x84),0));
      }
    }
    else {
      fVar45 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::EaseInOutMatrix::Increase(fVar45);
      AbyssEngine::EaseInOutMatrix::GetValue();
      (**(code **)(*(int *)this + 0x28))((Vector *)&local_88,this);
      (**(code **)(*(int *)this + 0x28))(auStack_f8,this);
      fVar45 = (float)AbyssEngine::AEMath::operator-(aVStack_ec,pVVar8);
      fVar45 = (float)AbyssEngine::AEMath::operator*((Vector *)&local_e0,fVar45);
      AbyssEngine::AEMath::operator*((Vector *)&local_94,fVar45);
      AbyssEngine::AEMath::operator+((Vector *)&local_138,(Vector *)&local_88);
      AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_d0,(Vector *)&local_78);
      AEGeometry::setMatrix(*(Matrix **)(this + 8));
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),(Matrix *)&local_78)
      ;
    }
    break;
  case 9:
    iVar7 = Status::getMission();
    *(int *)(this + 700) = *(int *)(this + 700) + param_1;
    iVar31 = Mission::isEmpty();
    if ((iVar31 == 0) && (iVar31 = Mission::getType(), iVar31 == 0xb8)) {
      Route::getDockingTarget();
      iVar31 = PlayerFixedObject::getDockingType();
      if (iVar31 == 1) {
        pfVar25 = (float *)&DAT_000ee70c;
        fVar45 = (float)VectorSignedToFloat(*(undefined4 *)(this + 700),(byte)(in_fpscr >> 0x16) & 3
                                           );
        if (*(int *)(this + 0x7c) == 0x33) {
          pfVar25 = (float *)&DAT_000ee710;
        }
        iVar16 = *(int *)(this + 0x2c0);
        *(int *)(this + 0x2c0) = (int)(fVar45 / *pfVar25);
        iVar31 = Status::getCurrentCampaignMission();
        if (iVar31 == 0x5e) {
          iVar31 = Mission::getStatusValue();
          Status::getShip();
          iVar19 = Ship::getMaxPassengers();
          if (iVar31 <= iVar19) goto LAB_000ee7d8;
        }
        if (iVar16 < *(int *)(this + 0x2c0)) {
          iVar31 = Level::getNumDeliveredPassengers();
          iVar16 = Mission::getProductionGoodAmount();
          if (iVar31 < iVar16) {
            Mission::getStatusValue();
            Mission::setStatusValue(iVar7);
            uVar40 = Mission::getStatusValue();
            if (0x7fffffff < uVar40) {
              Mission::setStatusValue(iVar7);
            }
          }
        }
      }
    }
LAB_000ee7d8:
    iVar31 = *(int *)(this + 700);
    iVar7 = Route::getDockingTime();
    if (iVar7 < iVar31) {
      setExhaustVisible(bVar38);
      iVar7 = *(int *)(this + 0x6c);
      Route::getCurrent();
      Route::reachWaypoint(iVar7);
      *(undefined4 *)(this + 0x88) = 1;
      if (this[0x1fc] != (PlayerFighter)0x0) {
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x78),SUB41(*(undefined4 *)(this + 0x80),0));
        ParticleSystemManager::enableSystemEmit
                  (*(int *)(*(int *)(this + 0x54) + 0x84),SUB41(*(undefined4 *)(this + 0x84),0));
      }
      if (*(int *)(this + 0x2c4) != 0) {
        SpacePoint::giveFree();
        *(undefined4 *)(this + 0x2c4) = 0;
      }
    }
  }
switchD_000ee1c4_caseD_2:
  if (*piVar34 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ecfd8: push {r4,r5,r6,r7,lr}
  000ecfda: add r7,sp,#0xc
  000ecfdc: push {r7,r8,r9,r10,r11}
  000ecfe0: vpush {d8,d9}
  000ecfe4: sub sp,#0x1d8
  000ecfe6: mov r4,r0
  000ecfe8: ldr r0,[0x000ed348]
  000ecfea: mov r8,r1
  000ecfec: add r0,pc
  000ecfee: ldr.w r10,[r0,#0x0]
  000ecff2: ldr.w r0,[r10,#0x0]
  000ecff6: str r0,[sp,#0x1d4]
  000ecff8: ldr.w r0,[r4,#0x88]
  000ecffc: cmp r0,#0x4
  000ecffe: bne 0x000ed044
  000ed000: ldr.w r0,[r4,#0x124]
  000ed004: blx 0x000757fc
  000ed008: cbnz r0,0x000ed044
  000ed00a: ldrb.w r0,[r4,#0x4c]
  000ed00e: cbz r0,0x000ed01c
  000ed010: ldr.w r0,[r4,#0xd8]
  000ed014: movw r1,#0xea61
  000ed018: cmp r0,r1
  000ed01a: blt 0x000ed044
  000ed01c: ldr r0,[sp,#0x1d4]
  000ed01e: ldr.w r1,[r10,#0x0]
  000ed022: subs r0,r1,r0
  000ed024: itttt eq
  000ed026: mov.eq r0,r4
  000ed028: mov.eq r1,#0x0
  000ed02a: add.eq sp,#0x1d8
  000ed02c: vpop.eq {d8,d9}
  000ed030: itttt eq
  000ed032: add.eq sp,#0x4
  000ed034: pop.eq.w {r8,r9,r10,r11}
  000ed038: pop.eq.w {r4,r5,r6,r7,lr}
  000ed03c: b.eq.w 0x001abe08
  000ed040: blx 0x0006e824
  000ed044: ldr.w r1,[r4,#0x1b8]
  000ed048: asr.w r9,r8, asr #0x1f
  000ed04c: ldr.w r2,[r4,#0x1c0]
  000ed050: add r5,sp,#0x198
  000ed052: ldr r0,[r4,#0x28]
  000ed054: add r1,r8
  000ed056: add r2,r8
  000ed058: str.w r2,[r4,#0x1c0]
  000ed05c: str.w r1,[r4,#0x1b8]
  000ed060: cmp r0,#0x1
  000ed062: itt eq
  000ed064: mov.eq r0,#0x1
  000ed066: str.eq r0,[r4,#0x28]
  000ed068: strd r8,r9,[r4,#0x1d0]
  000ed06c: ldr r1,[r4,#0x8]
  000ed06e: mov r0,r5
  000ed070: blx 0x000720b8
  000ed074: add.w r0,r4,#0x2c
  000ed078: mov r1,r5
  000ed07a: str r0,[sp,#0xd4]
  000ed07c: blx 0x0006eb3c
  000ed080: ldrb.w r0,[r4,#0x43]
  000ed084: cbz r0,0x000ed08e
  000ed086: mov r5,r4
  000ed088: ldr.w r0,[r5,#0x4]!
  000ed08c: b 0x000ed0fe
  000ed08e: ldr r0,[r4,#0x28]
  000ed090: bic r0,r0,#0x1
  000ed094: cmp r0,#0x8
  000ed096: bne 0x000ed09c
  000ed098: movs r0,#0x1
  000ed09a: b 0x000ed0ba
  000ed09c: mov r0,r4
  000ed09e: blx 0x00072ed4
  000ed0a2: cbz r0,0x000ed0a8
  000ed0a4: movs r0,#0x0
  000ed0a6: b 0x000ed0ba
  000ed0a8: ldr r0,[0x000ed34c]
  000ed0aa: add r0,pc
  000ed0ac: ldr r0,[r0,#0x0]
  000ed0ae: ldr r0,[r0,#0x0]
  000ed0b0: blx 0x00071b84
  000ed0b4: ldr r1,[r4,#0x28]
  000ed0b6: blx 0x00071b90
  000ed0ba: mov r5,r4
  000ed0bc: ldr.w r1,[r5,#0x4]!
  000ed0c0: strb.w r0,[r1,#0x5c]
  000ed0c4: ldr r0,[r5,#0x24]
  000ed0c6: bic r0,r0,#0x1
  000ed0ca: cmp r0,#0x8
  000ed0cc: bne 0x000ed0d2
  000ed0ce: movs r1,#0x0
  000ed0d0: b 0x000ed0f2
  000ed0d2: mov r0,r4
  000ed0d4: blx 0x00072ed4
  000ed0d8: cbz r0,0x000ed0de
  000ed0da: movs r1,#0x1
  000ed0dc: b 0x000ed0f2
  000ed0de: ldr r0,[0x000ed350]
  000ed0e0: add r0,pc
  000ed0e2: ldr r0,[r0,#0x0]
  000ed0e4: ldr r0,[r0,#0x0]
  000ed0e6: blx 0x00071b84
  000ed0ea: ldr r1,[r4,#0x28]
  000ed0ec: blx 0x00075808
  000ed0f0: mov r1,r0
  000ed0f2: ldr r0,[r4,#0x4]
  000ed0f4: strb.w r1,[r0,#0x5d]
  000ed0f8: ldrb.w r1,[r4,#0x43]
  000ed0fc: cbz r1,0x000ed116
  000ed0fe: ldr r6,[r0,#0x6c]
  000ed100: blx 0x00072910
  000ed104: movs r1,#0x14
  000ed106: blx 0x0007198c
  000ed10a: cmp r6,r0
  000ed10c: ittt gt
  000ed10e: ldr.gt r0,[r5,#0x0]
  000ed110: mov.gt r1,#0x1
  000ed112: strb.gt.w r1,[r0,#0x5c]
  000ed116: ldrb.w r0,[r4,#0x42]
  000ed11a: cbz r0,0x000ed17c
  000ed11c: ldr.w r0,[r4,#0x2e8]
  000ed120: movw r1,#0x4e20
  000ed124: add r0,r8
  000ed126: str.w r0,[r4,#0x2e8]
  000ed12a: cmp r0,r1
  000ed12c: ble 0x000ed152
  000ed12e: mov r5,r4
  000ed130: movs r1,#0x0
  000ed132: ldr.w r0,[r5,#0x4]!
  000ed136: str.w r1,[r5,#0x2e4]
  000ed13a: movs r1,#0x1
  000ed13c: blx 0x00072520
  000ed140: cbz r0,0x000ed154
  000ed142: ldr.w r0,[r4,#0x140]
  000ed146: clz r0,r0
  000ed14a: lsrs r0,r0,#0x5
  000ed14c: str.w r0,[r4,#0x140]
  000ed150: b 0x000ed154
  000ed152: adds r5,r4,#0x4
  000ed154: ldr r0,[r5,#0x0]
  000ed156: blx 0x00074254
  000ed15a: cbnz r0,0x000ed17c
  000ed15c: ldr r0,[r4,#0x54]
  000ed15e: blx 0x00072034
  000ed162: cbz r0,0x000ed17c
  000ed164: ldr r0,[r4,#0x54]
  000ed166: blx 0x00072034
  000ed16a: ldr r0,[r0,#0x14]
  000ed16c: ldr r0,[r0,#0x4]
  000ed16e: cmp r0,r4
  000ed170: itt eq
  000ed172: ldrb.eq.w r0,[r4,#0x43]
  000ed176: cmp.eq r0,#0x0
  000ed178: beq.w 0x000ed62a
  000ed17c: ldr r0,[0x000ed354]
  000ed17e: add r0,pc
  000ed180: ldr r0,[r0,#0x0]
  000ed182: str r0,[sp,#0xd0]
  000ed184: ldr r0,[r0,#0x0]
  000ed186: blx 0x00071998
  000ed18a: cbz r0,0x000ed1a4
  000ed18c: ldr r0,[sp,#0xd0]
  000ed18e: ldr r0,[r0,#0x0]
  000ed190: ldrb.w r0,[r0,#0x110]
  000ed194: cbz r0,0x000ed1a4
  000ed196: ldr r0,[r4,#0x28]
  000ed198: cmp r0,#0x8
  000ed19a: ittt eq
  000ed19c: ldr.eq r0,[r4,#0x4]
  000ed19e: mov.eq r1,#0x0
  000ed1a0: strh.eq.w r1,[r0,#0x5c]
  000ed1a4: ldr r0,[r4,#0x4]
  000ed1a6: blx 0x00075814
  000ed1aa: mov r1,r0
  000ed1ac: ldr r0,[r4,#0x4]
  000ed1ae: cbz r1,0x000ed1b6
  000ed1b0: movs r1,#0x1
  000ed1b2: strh.w r1,[r0,#0x5c]
  000ed1b6: blx 0x0007420c
  000ed1ba: cbz r0,0x000ed1c6
  000ed1bc: ldr r0,[r4,#0x4]
  000ed1be: mov.w r1,#0x100
  000ed1c2: strh.w r1,[r0,#0x5c]
  000ed1c6: ldrb.w r0,[r4,#0xdc]
  000ed1ca: cbnz r0,0x000ed1da
  000ed1cc: ldr r0,[r4,#0x6c]
  000ed1ce: cbnz r0,0x000ed1da
  000ed1d0: ldr.w r0,[r4,#0x144]
  000ed1d4: blx 0x00073f3c
  000ed1d8: str r0,[r4,#0x6c]
  000ed1da: ldr.w r0,[r4,#0x1c8]
  000ed1de: add r0,r8
  000ed1e0: str.w r0,[r4,#0x1c8]
  000ed1e4: cmp r0,#0xc9
  000ed1e6: blt 0x000ed20a
  000ed1e8: ldr.w r0,[r4,#0x154]
  000ed1ec: cbz r0,0x000ed204
  000ed1ee: ldr r6,[sp,#0xd4]
  000ed1f0: add.w r5,r4,#0x158
  000ed1f4: mov r2,r5
  000ed1f6: mov r1,r6
  000ed1f8: blx 0x00075820
  000ed1fc: mov r0,r5
  000ed1fe: mov r1,r6
  000ed200: blx 0x0006eb3c
  000ed204: movs r0,#0x0
  000ed206: str.w r0,[r4,#0x1c8]
  000ed20a: ldr r0,[r4,#0x8]
  000ed20c: blx 0x000721cc
  000ed210: mov r1,r0
  000ed212: ldr r0,[r4,#0x4]
  000ed214: adds r0,#0x4
  000ed216: blx 0x0006f148
  000ed21a: ldr r0,[r4,#0xc]
  000ed21c: cbz r0,0x000ed22c
  000ed21e: blx 0x000721cc
  000ed222: mov r1,r0
  000ed224: ldr r0,[r4,#0x4]
  000ed226: adds r0,#0x4
  000ed228: blx 0x0006f4a8
  000ed22c: ldrb.w r0,[r4,#0xdc]
  000ed230: cbz r0,0x000ed240
  000ed232: ldr.w r0,[r4,#0x88]
  000ed236: cmp r0,#0x4
  000ed238: itt ne
  000ed23a: ldr.ne r0,[r4,#0x6c]
  000ed23c: cmp.ne r0,#0x0
  000ed23e: bne 0x000ed2b8
  000ed240: ldr.w r1,[r4,#0x88]
  000ed244: ldr r0,[r4,#0x4]
  000ed246: subs r2,r1,#0x3
  000ed248: cmp r2,#0x2
  000ed24a: bcs 0x000ed254
  000ed24c: blx 0x0007240c
  000ed250: b.w 0x000edb66
  000ed254: ldr.w r2,[r4,#0xf8]
  000ed258: subs r1,#0x5
  000ed25a: it ne
  000ed25c: mov.ne r1,#0x1
  000ed25e: adds r2,#0x1
  000ed260: it ne
  000ed262: mov.ne r2,#0x1
  000ed264: ands r2,r1
  000ed266: mov r1,r8
  000ed268: blx 0x00072958
  000ed26c: ldr r0,[r4,#0x4]
  000ed26e: blx 0x0007582c
  000ed272: mov r6,r0
  000ed274: ldrb.w r0,[r4,#0x2e4]
  000ed278: cmp r0,#0x0
  000ed27a: bne.w 0x000edabe
  000ed27e: cmp r6,#0x0
  000ed280: beq 0x000ed35c
  000ed282: ldr r0,[r4,#0x38]
  000ed284: ldr r1,[r6,#0x0]
  000ed286: cmp r0,r1
  000ed288: itt cs
  000ed28a: mov.cs.w r0,#0xffffffff
  000ed28e: str.cs r0,[r4,#0x38]
  000ed290: ldrb.w r1,[r4,#0x12e]
  000ed294: cmp r1,#0x0
  000ed296: beq.w 0x000ed3c0
  000ed29a: cmp r0,#0x0
  000ed29c: blt.w 0x000ed3c6
  000ed2a0: ldr r1,[r6,#0x4]
  000ed2a2: ldr.w r0,[r1,r0,lsl #0x2]
  000ed2a6: blx 0x00071f14
  000ed2aa: cmp r0,#0x0
  000ed2ac: bne.w 0x000ed3c6
  000ed2b0: movs r0,#0x0
  000ed2b2: strb.w r0,[r4,#0x12e]
  000ed2b6: b 0x000ed3c6
  000ed2b8: ldr r0,[r4,#0x54]
  000ed2ba: blx 0x00072034
  000ed2be: ldr r1,[r0,#0x8]
  000ed2c0: add r5,sp,#0x198
  000ed2c2: mov r0,r5
  000ed2c4: blx 0x000720b8
  000ed2c8: add.w r6,r4,#0x90
  000ed2cc: mov r1,r5
  000ed2ce: mov r0,r6
  000ed2d0: blx 0x0006eb3c
  000ed2d4: ldr.w r0,[r4,#0xe4]
  000ed2d8: cmp r0,#0x1
  000ed2da: bne 0x000ed240
  000ed2dc: ldr.w r0,[r4,#0xe0]
  000ed2e0: mov r11,r9
  000ed2e2: cmp r0,#0x0
  000ed2e4: beq.w 0x000ed66c
  000ed2e8: cmp r0,#0x1
  000ed2ea: beq.w 0x000ed63c
  000ed2ee: cmp r0,#0x2
  000ed2f0: bne.w 0x000ed6ca
  000ed2f4: ldr r0,[r4,#0x54]
  000ed2f6: blx 0x00072034
  000ed2fa: ldr r1,[r0,#0x8]
  000ed2fc: add.w r9,sp,#0x198
  000ed300: mov r0,r9
  000ed302: blx 0x00072568
  000ed306: add.w r5,r4,#0x9c
  000ed30a: mov r1,r9
  000ed30c: mov r0,r5
  000ed30e: blx 0x0006eb3c
  000ed312: ldr r1,[0x000ed358]
  000ed314: mov r0,r5
  000ed316: blx 0x00072628
  000ed31a: mov r0,r6
  000ed31c: mov r1,r5
  000ed31e: blx 0x00073534
  000ed322: ldr r0,[r4,#0x54]
  000ed324: blx 0x00072034
  000ed328: ldr r1,[r0,#0x8]
  000ed32a: add r5,sp,#0xd8
  000ed32c: mov r0,r5
  000ed32e: blx 0x00072574
  000ed332: add.w r9,sp,#0x140
  000ed336: mov r1,r5
  000ed338: mov r0,r9
  000ed33a: blx 0x0006ec80
  000ed33e: add r5,sp,#0x198
  000ed340: mov r1,r9
  000ed342: ldr r2,[0x000ed358]
  000ed344: b 0x000ed6bc
  000ed35c: ldr r5,[r4,#0x6c]
  000ed35e: cmp r5,#0x0
  000ed360: beq.w 0x000ed622
  000ed364: add r6,sp,#0x198
  000ed366: ldr r1,[r4,#0x8]
  000ed368: mov r0,r6
  000ed36a: blx 0x000720b8
  000ed36e: mov r0,r5
  000ed370: mov r1,r6
  000ed372: blx 0x00072994
  000ed376: ldr r0,[r4,#0x6c]
  000ed378: blx 0x000728bc
  000ed37c: cmp r0,#0x0
  000ed37e: beq.w 0x000eda88
  000ed382: ldr r1,[0x000ed720]
  000ed384: ldr r0,[r4,#0x6c]
  000ed386: add r1,pc
  000ed388: ldr r5,[r1,#0x0]
  000ed38a: blx r5
  000ed38c: vldr.32 s0,[r0,#0x124]
  000ed390: vcvt.f32.s32 s0,s0
  000ed394: ldr r0,[r4,#0x6c]
  000ed396: vstr.32 s0,[r4,#0x164]
  000ed39a: blx r5
  000ed39c: vldr.32 s0,[r0,#0x128]
  000ed3a0: vcvt.f32.s32 s0,s0
  000ed3a4: ldr r0,[r4,#0x6c]
  000ed3a6: vstr.32 s0,[r4,#0x168]
  000ed3aa: blx r5
  000ed3ac: vldr.32 s0,[r0,#0x12c]
  000ed3b0: movs r0,#0x1
  000ed3b2: vcvt.f32.s32 s0,s0
  000ed3b6: strb.w r0,[r4,#0x12c]
  000ed3ba: vstr.32 s0,[r4,#0x16c]
  000ed3be: b 0x000eda88
  000ed3c0: mov.w r0,#0xffffffff
  000ed3c4: str r0,[r4,#0x38]
  000ed3c6: ldr.w r0,[r4,#0x1b8]
  000ed3ca: movs r1,#0x0
  000ed3cc: str.w r1,[r4,#0x148]
  000ed3d0: movw r1,#0x1389
  000ed3d4: cmp r0,r1
  000ed3d6: str.w r9,[sp,#0xcc]
  000ed3da: blt.w 0x000ed544
  000ed3de: ldrb.w r0,[r4,#0x12d]
  000ed3e2: mov.w r9,#0x0
  000ed3e6: cmp r0,#0x0
  000ed3e8: mov.w r0,#0x0
  000ed3ec: bne 0x000ed406
  000ed3ee: ldr r0,[0x000ed724]
  000ed3f0: movs r1,#0x64
  000ed3f2: add r0,pc
  000ed3f4: ldr r0,[r0,#0x0]
  000ed3f6: ldr r0,[r0,#0x0]
  000ed3f8: blx 0x00071848
  000ed3fc: cmp r0,#0x14
  000ed3fe: mov.w r0,#0x0
  000ed402: it lt
  000ed404: mov.lt r0,#0x1
  000ed406: ldr r1,[0x000ed728]
  000ed408: str.w r9,[r4,#0x1b8]
  000ed40c: add r1,pc
  000ed40e: strb.w r0,[r4,#0x12d]
  000ed412: ldr r5,[r1,#0x0]
  000ed414: movs r1,#0x64
  000ed416: ldr r0,[r5,#0x0]
  000ed418: blx 0x00071848
  000ed41c: cmp r0,#0x1d
  000ed41e: bgt 0x000ed50a
  000ed420: ldr r0,[r6,#0x0]
  000ed422: cmp r0,#0x2
  000ed424: bcc 0x000ed50a
  000ed426: movs r0,#0x0
  000ed428: mov.w r9,#0x0
  000ed42c: add.w r1,r4,#0x90
  000ed430: strb.w r0,[r4,#0x12e]
  000ed434: str r1,[sp,#0xc0]
  000ed436: b 0x000ed500
  000ed438: lsls r0,r0,#0x18
  000ed43a: bne 0x000ed50e
  000ed43c: ldr r1,[r6,#0x0]
  000ed43e: ldr r0,[r5,#0x0]
  000ed440: blx 0x00071848
  000ed444: str r0,[r4,#0x38]
  000ed446: ldr r1,[r6,#0x4]
  000ed448: ldr.w r0,[r1,r0,lsl #0x2]
  000ed44c: blx 0x00071f14
  000ed450: cmp r0,#0x0
  000ed452: beq 0x000ed4f8
  000ed454: ldr r0,[r4,#0x38]
  000ed456: add.w r11,sp,#0x198
  000ed45a: ldr r1,[r6,#0x4]
  000ed45c: ldr.w r1,[r1,r0,lsl #0x2]
  000ed460: mov r0,r11
  000ed462: blx 0x00072964
  000ed466: ldr r0,[sp,#0xc0]
  000ed468: mov r1,r11
  000ed46a: blx 0x0006eb3c
  000ed46e: ldr r0,[r4,#0x28]
  000ed470: cmp r0,#0x8
  000ed472: beq 0x000ed480
  000ed474: ldr r0,[r4,#0x4]
  000ed476: blx 0x00075814
  000ed47a: cmp r0,#0x0
  000ed47c: bne.w 0x000ef770
  000ed480: ldr.w r0,[r4,#0x128]
  000ed484: vldr.32 s2,[r4,#0x2c]
  000ed488: vldr.32 s4,[r4,#0x90]
  000ed48c: vmov s0,r0
  000ed490: vsub.f32 s4,s2,s4
  000ed494: vcvt.f32.s32 s0,s0
  000ed498: vcmpe.f32 s4,s0
  000ed49c: vmrs apsr,fpscr
  000ed4a0: bpl 0x000ed4f8
  000ed4a2: rsbs r0,r0
  000ed4a4: vmov s2,r0
  000ed4a8: vcvt.f32.s32 s2,s2
  000ed4ac: vcmpe.f32 s4,s2
  000ed4b0: vmrs apsr,fpscr
  000ed4b4: ble 0x000ed4f8
  000ed4b6: vldr.32 s4,[r4,#0x30]
  000ed4ba: vldr.32 s6,[r4,#0x94]
  000ed4be: vsub.f32 s4,s4,s6
  000ed4c2: vcmpe.f32 s4,s0
  000ed4c6: vmrs apsr,fpscr
  000ed4ca: bpl 0x000ed4f8
  000ed4cc: vcmpe.f32 s4,s2
  000ed4d0: vmrs apsr,fpscr
  000ed4d4: ble 0x000ed4f8
  000ed4d6: vldr.32 s4,[r4,#0x34]
  000ed4da: vldr.32 s6,[r4,#0x98]
  000ed4de: vsub.f32 s4,s4,s6
  000ed4e2: vcmpe.f32 s4,s0
  000ed4e6: vmrs apsr,fpscr
  000ed4ea: bpl 0x000ed4f8
  000ed4ec: vcmpe.f32 s4,s2
  000ed4f0: vmrs apsr,fpscr
  000ed4f4: bgt.w 0x000ef770
  000ed4f8: ldrb.w r0,[r4,#0x12e]
  000ed4fc: add.w r9,r9,#0x1
  000ed500: cmp.w r9,#0x4
  000ed504: bls 0x000ed438
  000ed506: lsls r0,r0,#0x18
  000ed508: bne 0x000ed50e
  000ed50a: movs r0,#0x0
  000ed50c: str r0,[r4,#0x38]
  000ed50e: ldrb.w r0,[r4,#0xdc]
  000ed512: cmp r0,#0x0
  000ed514: beq.w 0x000ed780
  000ed518: ldr.w r0,[r4,#0xe4]
  000ed51c: cmp r0,#0x3
  000ed51e: bne.w 0x000ed780
  000ed522: ldr.w r0,[r4,#0xe8]
  000ed526: cmp r0,#0x0
  000ed528: beq.w 0x000ed780
  000ed52c: blx 0x00071ec0
  000ed530: cmp r0,#0x0
  000ed532: beq.w 0x000ed74e
  000ed536: ldr r0,[r4,#0x0]
  000ed538: movs r1,#0x1
  000ed53a: movs r2,#0x0
  000ed53c: ldr r3,[r0,#0x10]
  000ed53e: mov r0,r4
  000ed540: blx r3
  000ed542: b 0x000ed780
  000ed544: ldrb.w r0,[r4,#0x12e]
  000ed548: cmp r0,#0x0
  000ed54a: bne.w 0x000ed7ac
  000ed54e: add.w r11,r4,#0x90
  000ed552: add.w r9,sp,#0x198
  000ed556: movs r5,#0x0
  000ed558: b 0x000ed61a
  000ed55a: ldr r0,[r6,#0x4]
  000ed55c: ldr.w r0,[r0,r5,lsl #0x2]
  000ed560: cmp r0,#0x0
  000ed562: beq 0x000ed618
  000ed564: blx 0x00071f14
  000ed568: cmp r0,#0x0
  000ed56a: beq 0x000ed618
  000ed56c: ldr r0,[r6,#0x4]
  000ed56e: ldr.w r0,[r0,r5,lsl #0x2]
  000ed572: blx 0x00075838
  000ed576: cmp r0,#0x0
  000ed578: bne 0x000ed618
  000ed57a: ldr r0,[r6,#0x4]
  000ed57c: ldr.w r1,[r0,r5,lsl #0x2]
  000ed580: mov r0,r9
  000ed582: blx 0x00072964
  000ed586: mov r0,r11
  000ed588: mov r1,r9
  000ed58a: blx 0x0006eb3c
  000ed58e: ldr r0,[r4,#0x28]
  000ed590: cmp r0,#0x8
  000ed592: beq 0x000ed5a0
  000ed594: ldr r0,[r4,#0x4]
  000ed596: blx 0x00075814
  000ed59a: cmp r0,#0x0
  000ed59c: bne.w 0x000ef77a
  000ed5a0: ldr.w r0,[r4,#0x128]
  000ed5a4: vldr.32 s2,[r4,#0x2c]
  000ed5a8: vldr.32 s4,[r4,#0x90]
  000ed5ac: vmov s0,r0
  000ed5b0: vsub.f32 s4,s2,s4
  000ed5b4: vcvt.f32.s32 s0,s0
  000ed5b8: vcmpe.f32 s4,s0
  000ed5bc: vmrs apsr,fpscr
  000ed5c0: bpl 0x000ed618
  000ed5c2: rsbs r0,r0
  000ed5c4: vmov s2,r0
  000ed5c8: vcvt.f32.s32 s2,s2
  000ed5cc: vcmpe.f32 s4,s2
  000ed5d0: vmrs apsr,fpscr
  000ed5d4: ble 0x000ed618
  000ed5d6: vldr.32 s4,[r4,#0x30]
  000ed5da: vldr.32 s6,[r4,#0x94]
  000ed5de: vsub.f32 s4,s4,s6
  000ed5e2: vcmpe.f32 s4,s0
  000ed5e6: vmrs apsr,fpscr
  000ed5ea: bpl 0x000ed618
  000ed5ec: vcmpe.f32 s4,s2
  000ed5f0: vmrs apsr,fpscr
  000ed5f4: ble 0x000ed618
  000ed5f6: vldr.32 s4,[r4,#0x34]
  000ed5fa: vldr.32 s6,[r4,#0x98]
  000ed5fe: vsub.f32 s4,s4,s6
  000ed602: vcmpe.f32 s4,s0
  000ed606: vmrs apsr,fpscr
  000ed60a: bpl 0x000ed618
  000ed60c: vcmpe.f32 s4,s2
  000ed610: vmrs apsr,fpscr
  000ed614: bgt.w 0x000ef77a
  000ed618: adds r5,#0x1
  000ed61a: ldr r0,[r6,#0x0]
  000ed61c: cmp r5,r0
  000ed61e: bcc 0x000ed55a
  000ed620: b 0x000ed7ac
  000ed622: movs r0,#0x5
  000ed624: str.w r0,[r4,#0x88]
  000ed628: b 0x000eda88
  000ed62a: ldr r0,[r4,#0x4]
  000ed62c: movs r1,#0x1
  000ed62e: blx 0x00073e94
  000ed632: ldr r1,[r4,#0x48]
  000ed634: ldr r0,[r4,#0x54]
  000ed636: blx 0x00075844
  000ed63a: b 0x000ed17c
  000ed63c: ldr r0,[r4,#0x54]
  000ed63e: blx 0x00072034
  000ed642: ldr r1,[r0,#0x8]
  000ed644: add.w r9,sp,#0x198
  000ed648: mov r0,r9
  000ed64a: blx 0x0007261c
  000ed64e: add.w r5,r4,#0x9c
  000ed652: mov r1,r9
  000ed654: mov r0,r5
  000ed656: blx 0x0006eb3c
  000ed65a: ldr r1,[0x000ed72c]
  000ed65c: mov r0,r5
  000ed65e: blx 0x00072628
  000ed662: mov r0,r6
  000ed664: mov r1,r5
  000ed666: blx 0x00073534
  000ed66a: b 0x000ed69a
  000ed66c: ldr r0,[r4,#0x54]
  000ed66e: blx 0x00072034
  000ed672: ldr r1,[r0,#0x8]
  000ed674: add.w r9,sp,#0x198
  000ed678: mov r0,r9
  000ed67a: blx 0x0007261c
  000ed67e: add.w r5,r4,#0x9c
  000ed682: mov r1,r9
  000ed684: mov r0,r5
  000ed686: blx 0x0006eb3c
  000ed68a: ldr r1,[0x000ed72c]
  000ed68c: mov r0,r5
  000ed68e: blx 0x00072628
  000ed692: mov r0,r6
  000ed694: mov r1,r5
  000ed696: blx 0x00072694
  000ed69a: ldr r0,[r4,#0x54]
  000ed69c: blx 0x00072034
  000ed6a0: ldr r1,[r0,#0x8]
  000ed6a2: add r5,sp,#0xd8
  000ed6a4: mov r0,r5
  000ed6a6: blx 0x00072574
  000ed6aa: add.w r9,sp,#0x140
  000ed6ae: mov r1,r5
  000ed6b0: mov r0,r9
  000ed6b2: blx 0x0006ec80
  000ed6b6: add r5,sp,#0x198
  000ed6b8: ldr r2,[0x000ed730]
  000ed6ba: mov r1,r9
  000ed6bc: mov r0,r5
  000ed6be: blx 0x0006ec74
  000ed6c2: mov r0,r6
  000ed6c4: mov r1,r5
  000ed6c6: blx 0x00072694
  000ed6ca: ldr r0,[r4,#0x6c]
  000ed6cc: blx 0x00073fcc
  000ed6d0: cmp r0,#0x2
  000ed6d2: blt 0x000ed734
  000ed6d4: vldr.32 s0,[r4,#0x90]
  000ed6d8: mov r9,r11
  000ed6da: vcvt.s32.f32 s0,s0
  000ed6de: vstr.32 s0,[sp,#0x198]
  000ed6e2: vldr.32 s0,[r4,#0x94]
  000ed6e6: vcvt.s32.f32 s0,s0
  000ed6ea: vstr.32 s0,[sp,#0x19c]
  000ed6ee: vldr.32 s0,[r4,#0x98]
  000ed6f2: vcvt.s32.f32 s0,s0
  000ed6f6: vstr.32 s0,[sp,#0x1a0]
  000ed6fa: ldr r0,[r4,#0x6c]
  000ed6fc: cbz r0,0x000ed70a
  000ed6fe: blx 0x00072310
  000ed702: blx 0x0006eb48
  000ed706: movs r0,#0x0
  000ed708: str r0,[r4,#0x6c]
  000ed70a: movs r0,#0x18
  000ed70c: blx 0x0006eb24
  000ed710: mov r5,r0
  000ed712: add r1,sp,#0x198
  000ed714: movs r2,#0x3
  000ed716: blx 0x00073eb8
  000ed71a: str r5,[r4,#0x6c]
  000ed71c: b 0x000ed744
  000ed734: add.w r3,r4,#0x90
  000ed738: ldr r0,[r4,#0x6c]
  000ed73a: ldmia r3,{r1,r2,r3}
  000ed73c: blx 0x00073f0c
  000ed740: ldr r5,[r4,#0x6c]
  000ed742: mov r9,r11
  000ed744: mov r0,r5
  000ed746: movs r1,#0x1
  000ed748: blx 0x00073f48
  000ed74c: b 0x000ed240
  000ed74e: movs r5,#0x0
  000ed750: b 0x000ed77a
  000ed752: ldr r0,[r6,#0x4]
  000ed754: ldr.w r1,[r4,#0xe8]
  000ed758: ldr.w r0,[r0,r5,lsl #0x2]
  000ed75c: ldr r1,[r1,#0x4]
  000ed75e: cmp r0,r1
  000ed760: bne 0x000ed778
  000ed762: blx 0x00071f14
  000ed766: cbz r0,0x000ed778
  000ed768: ldr r0,[r6,#0x4]
  000ed76a: ldr.w r0,[r0,r5,lsl #0x2]
  000ed76e: blx 0x0007420c
  000ed772: cmp r0,#0x0
  000ed774: beq.w 0x000ef7b6
  000ed778: adds r5,#0x1
  000ed77a: ldr r0,[r6,#0x0]
  000ed77c: cmp r5,r0
  000ed77e: bcc 0x000ed752
  000ed780: ldr r0,[r4,#0x38]
  000ed782: ldr r1,[r6,#0x4]
  000ed784: ldr.w r0,[r1,r0,lsl #0x2]
  000ed788: blx 0x00071f14
  000ed78c: cbz r0,0x000ed7a0
  000ed78e: ldr r0,[r4,#0x38]
  000ed790: ldr r1,[r6,#0x4]
  000ed792: ldr.w r0,[r1,r0,lsl #0x2]
  000ed796: blx 0x00075838
  000ed79a: cmp r0,#0x0
  000ed79c: beq.w 0x000ede20
  000ed7a0: movs r0,#0x0
  000ed7a2: strb.w r0,[r4,#0x12e]
  000ed7a6: mov.w r0,#0xffffffff
  000ed7aa: str r0,[r4,#0x38]
  000ed7ac: ldr r0,[r4,#0x4]
  000ed7ae: ldrb.w r0,[r0,#0x5c]
  000ed7b2: cbnz r0,0x000ed7c2
  000ed7b4: ldr r0,[r4,#0x38]
  000ed7b6: cbnz r0,0x000ed7c2
  000ed7b8: movs r0,#0x0
  000ed7ba: strb.w r0,[r4,#0x12e]
  000ed7be: movs r0,#0x1
  000ed7c0: str r0,[r4,#0x38]
  000ed7c2: ldrb.w r0,[r4,#0xdc]
  000ed7c6: ldr.w r9,[sp,#0xcc]
  000ed7ca: cbz r0,0x000ed7d4
  000ed7cc: ldr.w r1,[r4,#0xe4]
  000ed7d0: cmp r1,#0x3
  000ed7d2: beq 0x000ed8b2
  000ed7d4: ldr r1,[r4,#0x38]
  000ed7d6: cmp r1,#0x1
  000ed7d8: blt 0x000ed8b0
  000ed7da: mov.w r0,#0xffffffff
  000ed7de: movs r5,#0x1
  000ed7e0: str r0,[r4,#0x38]
  000ed7e2: b 0x000ed89a
  000ed7e4: ldr r0,[r6,#0x4]
  000ed7e6: ldr.w r0,[r0,r5,lsl #0x2]
  000ed7ea: blx 0x00071f14
  000ed7ee: cmp r0,#0x0
  000ed7f0: beq 0x000ed898
  000ed7f2: ldr r0,[r6,#0x4]
  000ed7f4: ldr.w r0,[r0,r5,lsl #0x2]
  000ed7f8: blx 0x00075838
  000ed7fc: cmp r0,#0x0
  000ed7fe: bne 0x000ed898
  000ed800: ldr r0,[r6,#0x4]
  000ed802: ldr.w r0,[r0,r5,lsl #0x2]
  000ed806: blx 0x00075850
  000ed80a: mov r1,r0
  000ed80c: ldr r0,[r6,#0x4]
  000ed80e: cmp r1,#0x0
  000ed810: ldr.w r0,[r0,r5,lsl #0x2]
  000ed814: ldrb.w r2,[r0,#0x69]
  000ed818: beq 0x000ed824
  000ed81a: cbnz r2,0x000ed826
  000ed81c: blx 0x00075850
  000ed820: ldr r1,[r0,#0x28]
  000ed822: b 0x000ed828
  000ed824: cbz r2,0x000ed898
  000ed826: movs r1,#0x0
  000ed828: ldrb.w r0,[r4,#0xdc]
  000ed82c: cbz r0,0x000ed848
  000ed82e: ldr r1,[r6,#0x4]
  000ed830: ldr.w r1,[r1,r5,lsl #0x2]
  000ed834: ldrb.w r2,[r1,#0x5c]
  000ed838: cbnz r2,0x000ed8a8
  000ed83a: ldr.w r2,[r4,#0xe8]
  000ed83e: cbz r2,0x000ed898
  000ed840: ldr r2,[r2,#0x4]
  000ed842: cmp r1,r2
  000ed844: bne 0x000ed898
  000ed846: b 0x000ed8a8
  000ed848: ldr r2,[r4,#0x28]
  000ed84a: cmp r1,#0x8
  000ed84c: bne 0x000ed852
  000ed84e: cmp r2,#0x8
  000ed850: bne 0x000ed8a8
  000ed852: cmp r1,#0x8
  000ed854: beq 0x000ed85a
  000ed856: cmp r2,#0x8
  000ed858: beq 0x000ed8a8
  000ed85a: cmp r1,#0x9
  000ed85c: bne 0x000ed862
  000ed85e: cmp r2,#0x9
  000ed860: bne 0x000ed8a8
  000ed862: cmp r1,#0x9
  000ed864: beq 0x000ed86a
  000ed866: cmp r2,#0x9
  000ed868: beq 0x000ed8a8
  000ed86a: cbnz r1,0x000ed870
  000ed86c: cmp r2,#0x1
  000ed86e: beq 0x000ed8a8
  000ed870: cmp r1,#0x1
  000ed872: it eq
  000ed874: cmp.eq r2,#0x0
  000ed876: beq 0x000ed8a8
  000ed878: cmp r1,#0x3
  000ed87a: it eq
  000ed87c: cmp.eq r2,#0x2
  000ed87e: beq 0x000ed8a8
  000ed880: cmp r1,#0x2
  000ed882: it eq
  000ed884: cmp.eq r2,#0x3
  000ed886: beq 0x000ed8a8
  000ed888: cmp r1,#0xa
  000ed88a: bne 0x000ed890
  000ed88c: cmp r2,#0xa
  000ed88e: bne 0x000ed8a8
  000ed890: cmp r1,#0xa
  000ed892: beq 0x000ed898
  000ed894: cmp r2,#0xa
  000ed896: beq 0x000ed8a8
  000ed898: adds r5,#0x1
  000ed89a: ldr r0,[r6,#0x0]
  000ed89c: cmp r5,r0
  000ed89e: bcc.w 0x000ed7e4
  000ed8a2: ldrb.w r0,[r4,#0xdc]
  000ed8a6: b 0x000ed8b0
  000ed8a8: movs r1,#0x1
  000ed8aa: strb.w r1,[r4,#0x12e]
  000ed8ae: str r5,[r4,#0x38]
  000ed8b0: cbz r0,0x000ed8d2
  000ed8b2: ldrb.w r0,[r4,#0x12e]
  000ed8b6: cbnz r0,0x000ed8d2
  000ed8b8: ldr.w r0,[r4,#0xe4]
  000ed8bc: cmp r0,#0x1
  000ed8be: bne 0x000ed8d2
  000ed8c0: movs r0,#0x0
  000ed8c2: add.w r11,r4,#0x12c
  000ed8c6: strb.w r0,[r4,#0x12c]
  000ed8ca: mov.w r0,#0xffffffff
  000ed8ce: str r0,[r4,#0x38]
  000ed8d0: b 0x000ed8e2
  000ed8d2: ldr r1,[r4,#0x38]
  000ed8d4: movs r0,#0x0
  000ed8d6: add.w r11,r4,#0x12c
  000ed8da: strb.w r0,[r4,#0x12c]
  000ed8de: adds r0,r1,#0x1
  000ed8e0: bne 0x000ed8ea
  000ed8e2: mov r6,r4
  000ed8e4: ldr.w r0,[r6,#0x6c]!
  000ed8e8: cbnz r0,0x000ed8fc
  000ed8ea: ldr r0,[r4,#0x4]
  000ed8ec: blx 0x0007585c
  000ed8f0: cmp r0,#0x0
  000ed8f2: beq.w 0x000eda38
  000ed8f6: mov r6,r4
  000ed8f8: ldr.w r0,[r6,#0x6c]!
  000ed8fc: blx 0x000728bc
  000ed900: cmp r0,#0x0
  000ed902: beq.w 0x000eda20
  000ed906: ldr r0,[r4,#0x4]
  000ed908: blx 0x00071f14
  000ed90c: cmp r0,#0x0
  000ed90e: beq.w 0x000eda20
  000ed912: add.w r9,sp,#0x198
  000ed916: ldr r1,[r4,#0x8]
  000ed918: ldr r5,[r4,#0x6c]
  000ed91a: mov r0,r9
  000ed91c: blx 0x000720b8
  000ed920: mov r1,r9
  000ed922: mov r0,r5
  000ed924: ldr.w r9,[sp,#0xcc]
  000ed928: blx 0x00072994
  000ed92c: ldr r0,[r4,#0x6c]
  000ed92e: blx 0x000728bc
  000ed932: cmp r0,#0x0
  000ed934: beq.w 0x000eda88
  000ed938: ldr r0,[r6,#0x0]
  000ed93a: blx 0x00075868
  000ed93e: cmp r0,#0x0
  000ed940: beq.w 0x000eded8
  000ed944: ldr.w r0,[r4,#0x88]
  000ed948: cmp r0,#0x7
  000ed94a: beq 0x000ed958
  000ed94c: cmp r0,#0x1
  000ed94e: bne.w 0x000ef766
  000ed952: movs r0,#0x7
  000ed954: str.w r0,[r4,#0x88]
  000ed958: ldr r0,[r4,#0x6c]
  000ed95a: blx 0x00075868
  000ed95e: mov r5,r0
  000ed960: ldr r0,[r4,#0x0]
  000ed962: add r6,sp,#0x198
  000ed964: mov r1,r4
  000ed966: ldr r2,[r0,#0x28]
  000ed968: mov r0,r6
  000ed96a: blx r2
  000ed96c: ldr.w r2,[r4,#0x2c4]
  000ed970: mov r0,r5
  000ed972: mov r1,r6
  000ed974: blx 0x00072b5c
  000ed978: mov r5,r0
  000ed97a: cmp r0,#0x0
  000ed97c: beq.w 0x000ef766
  000ed980: ldr.w r0,[r4,#0x2c4]
  000ed984: cmp r0,r5
  000ed986: beq 0x000ed998
  000ed988: cbz r0,0x000ed98e
  000ed98a: blx 0x00072b80
  000ed98e: mov r0,r5
  000ed990: str.w r5,[r4,#0x2c4]
  000ed994: blx 0x00072b8c
  000ed998: ldr r0,[r4,#0x6c]
  000ed99a: blx 0x00075868
  000ed99e: mov r1,r0
  000ed9a0: ldr r0,[r0,#0x0]
  000ed9a2: add.w r9,sp,#0x140
  000ed9a6: ldr r2,[r0,#0x28]
  000ed9a8: mov r0,r9
  000ed9aa: blx r2
  000ed9ac: ldr r0,[r4,#0x6c]
  000ed9ae: blx 0x00075868
  000ed9b2: ldr r0,[r0,#0x8]
  000ed9b4: blx 0x000721cc
  000ed9b8: add r6,sp,#0xd8
  000ed9ba: mov r1,r0
  000ed9bc: mov r2,r5
  000ed9be: str r5,[sp,#0xc0]
  000ed9c0: mov r0,r6
  000ed9c2: blx 0x0006f694
  000ed9c6: add r5,sp,#0x198
  000ed9c8: mov r1,r9
  000ed9ca: mov r2,r6
  000ed9cc: mov r0,r5
  000ed9ce: blx 0x0006f1cc
  000ed9d2: add.w r6,r4,#0x9c
  000ed9d6: mov r1,r5
  000ed9d8: mov r0,r6
  000ed9da: blx 0x0006eb3c
  000ed9de: ldr r0,[r4,#0x0]
  000ed9e0: add.w r9,sp,#0x140
  000ed9e4: mov r1,r4
  000ed9e6: ldr r2,[r0,#0x28]
  000ed9e8: mov r0,r9
  000ed9ea: blx r2
  000ed9ec: add r5,sp,#0x198
  000ed9ee: mov r1,r6
  000ed9f0: mov r2,r9
  000ed9f2: mov r0,r5
  000ed9f4: blx 0x0006ec38
  000ed9f8: mov r0,r5
  000ed9fa: blx 0x0006ec44
  000ed9fe: vmov s0,r0
  000eda02: vcvt.s32.f32 s0,s0
  000eda06: vmov r0,s0
  000eda0a: cmp.w r0,#0x7d0
  000eda0e: blt.w 0x000ef55a
  000eda12: add.w r0,r4,#0x164
  000eda16: mov r1,r6
  000eda18: blx 0x0006eb3c
  000eda1c: b.w 0x000ef762
  000eda20: ldr r0,[r4,#0x4]
  000eda22: movs r1,#0x0
  000eda24: str r1,[r4,#0x38]
  000eda26: movs r1,#0x0
  000eda28: blx 0x00075874
  000eda2c: cmp r0,#0x0
  000eda2e: str.w r0,[r4,#0x148]
  000eda32: beq 0x000eda88
  000eda34: mov r1,r0
  000eda36: b 0x000eda5e
  000eda38: ldr r5,[r4,#0x38]
  000eda3a: cmp r5,#0x0
  000eda3c: blt 0x000eda4e
  000eda3e: ldr r0,[r4,#0x4]
  000eda40: blx 0x0007582c
  000eda44: ldr r0,[r0,#0x0]
  000eda46: cmp r5,r0
  000eda48: bcs 0x000eda4e
  000eda4a: ldr r1,[r4,#0x38]
  000eda4c: b 0x000eda52
  000eda4e: movs r1,#0x0
  000eda50: str r1,[r4,#0x38]
  000eda52: ldr r0,[r4,#0x4]
  000eda54: blx 0x00075874
  000eda58: mov r1,r0
  000eda5a: str.w r0,[r4,#0x148]
  000eda5e: add r5,sp,#0x198
  000eda60: mov r0,r5
  000eda62: blx 0x00072964
  000eda66: add.w r0,r4,#0x90
  000eda6a: mov r1,r5
  000eda6c: blx 0x0006eb3c
  000eda70: ldr.w r0,[r4,#0x90]
  000eda74: ldr.w r1,[r4,#0x94]
  000eda78: ldr.w r2,[r4,#0x98]
  000eda7c: str.w r0,[r4,#0x164]
  000eda80: str.w r1,[r4,#0x168]
  000eda84: str.w r2,[r4,#0x16c]
  000eda88: ldrb.w r0,[r4,#0xec]
  000eda8c: cbz r0,0x000edab8
  000eda8e: ldrb.w r0,[r4,#0x12c]
  000eda92: cbz r0,0x000edab8
  000eda94: ldr.w r0,[r4,#0xf0]
  000eda98: movw r1,#0x4e20
  000eda9c: add r0,r8
  000eda9e: str.w r0,[r4,#0xf0]
  000edaa2: cmp r0,r1
  000edaa4: blt 0x000edabe
  000edaa6: movs r0,#0x6
  000edaa8: str.w r0,[r4,#0x88]
  000edaac: movs r0,#0x0
  000edaae: str.w r0,[r4,#0xf0]
  000edab2: strb.w r0,[r4,#0x12c]
  000edab6: b 0x000edabe
  000edab8: movs r0,#0x0
  000edaba: str.w r0,[r4,#0xf0]
  000edabe: ldrb.w r0,[r4,#0xdc]
  000edac2: cmp r0,#0x0
  000edac4: beq 0x000edb66
  000edac6: ldr.w r0,[r4,#0xe4]
  000edaca: cmp r0,#0x2
  000edacc: bne 0x000edb66
  000edace: ldr.w r5,[r4,#0x14c]
  000edad2: cbz r5,0x000edb10
  000edad4: add r6,sp,#0x198
  000edad6: ldr r1,[r4,#0x8]
  000edad8: mov r0,r6
  000edada: blx 0x000720b8
  000edade: mov r0,r5
  000edae0: mov r1,r6
  000edae2: blx 0x00072994
  000edae6: ldr.w r0,[r4,#0x14c]
  000edaea: blx 0x00072988
  000edaee: mov r1,r0
  000edaf0: ldr.w r2,[r4,#0x1e4]
  000edaf4: ldr.w r0,[r4,#0x14c]
  000edaf8: cmp r1,r2
  000edafa: ble 0x000edb1e
  000edafc: cbz r0,0x000edb06
  000edafe: blx 0x00072310
  000edb02: blx 0x0006eb48
  000edb06: movs r1,#0x0
  000edb08: ldr r0,[r4,#0x0]
  000edb0a: str.w r1,[r4,#0x14c]
  000edb0e: b 0x000edb12
  000edb10: ldr r0,[r4,#0x0]
  000edb12: ldr r3,[r0,#0x10]
  000edb14: mov r0,r4
  000edb16: movs r1,#0x1
  000edb18: movs r2,#0x0
  000edb1a: blx r3
  000edb1c: b 0x000edb66
  000edb1e: blx 0x000728bc
  000edb22: cbz r0,0x000edb66
  000edb24: ldr r1,[0x000edec0]
  000edb26: ldr.w r0,[r4,#0x14c]
  000edb2a: add r1,pc
  000edb2c: ldr r5,[r1,#0x0]
  000edb2e: blx r5
  000edb30: vldr.32 s0,[r0,#0x124]
  000edb34: vcvt.f32.s32 s0,s0
  000edb38: ldr.w r0,[r4,#0x14c]
  000edb3c: vstr.32 s0,[r4,#0x164]
  000edb40: blx r5
  000edb42: vldr.32 s0,[r0,#0x128]
  000edb46: vcvt.f32.s32 s0,s0
  000edb4a: ldr.w r0,[r4,#0x14c]
  000edb4e: vstr.32 s0,[r4,#0x168]
  000edb52: blx r5
  000edb54: vldr.32 s0,[r0,#0x12c]
  000edb58: movs r0,#0x1
  000edb5a: vcvt.f32.s32 s0,s0
  000edb5e: strb.w r0,[r4,#0x12c]
  000edb62: vstr.32 s0,[r4,#0x16c]
  000edb66: vldr.32 s0,[r4,#0x2c]
  000edb6a: vldr.32 s6,[r4,#0x164]
  000edb6e: vldr.32 s2,[r4,#0x30]
  000edb72: vldr.32 s8,[r4,#0x168]
  000edb76: vsub.f32 s0,s6,s0
  000edb7a: vldr.32 s4,[r4,#0x34]
  000edb7e: vldr.32 s10,[r4,#0x16c]
  000edb82: vsub.f32 s2,s8,s2
  000edb86: ldr r0,[r4,#0x4]
  000edb88: vsub.f32 s4,s10,s4
  000edb8c: vstr.32 s0,[r4,#0x170]
  000edb90: vstr.32 s2,[r4,#0x174]
  000edb94: vstr.32 s4,[r4,#0x178]
  000edb98: ldrb.w r0,[r0,#0x5c]
  000edb9c: cmp r0,#0x0
  000edb9e: beq 0x000edc56
  000edba0: ldr.w r0,[r4,#0x88]
  000edba4: cmp r0,#0x5
  000edba6: beq 0x000edbb8
  000edba8: ldrb.w r0,[r4,#0xdc]
  000edbac: cmp r0,#0x0
  000edbae: beq 0x000edc56
  000edbb0: ldr.w r0,[r4,#0xe4]
  000edbb4: cmp r0,#0x1
  000edbb6: bne 0x000edc56
  000edbb8: ldr.w r0,[r4,#0x128]
  000edbbc: cmp r0,#0x1
  000edbbe: blt 0x000edc56
  000edbc0: ldr r0,[r4,#0x54]
  000edbc2: blx 0x00072034
  000edbc6: blx 0x00075880
  000edbca: cbz r0,0x000edbe6
  000edbcc: ldr r0,[r4,#0x54]
  000edbce: blx 0x00072034
  000edbd2: blx 0x0007588c
  000edbd6: blx 0x00075898
  000edbda: add r5,sp,#0x198
  000edbdc: mov r1,r0
  000edbde: mov r0,r5
  000edbe0: blx 0x000720b8
  000edbe4: b 0x000edbf6
  000edbe6: ldr r0,[r4,#0x54]
  000edbe8: blx 0x00072034
  000edbec: ldr r1,[r0,#0x0]
  000edbee: add r5,sp,#0x198
  000edbf0: mov r0,r5
  000edbf2: blx 0x00072964
  000edbf6: add.w r0,r4,#0x90
  000edbfa: mov r1,r5
  000edbfc: blx 0x0006eb3c
  000edc00: vldr.32 s0,[r4,#0x2c]
  000edc04: vldr.32 s4,[r4,#0x90]
  000edc08: vldr.32 s2,[r4,#0x30]
  000edc0c: vldr.32 s6,[r4,#0x34]
  000edc10: vsub.f32 s8,s4,s0
  000edc14: vldr.32 s10,[r4,#0x94]
  000edc18: vldr.32 s12,[r4,#0x98]
  000edc1c: vsub.f32 s4,s10,s2
  000edc20: vldr.32 s2,[pc,#0x2a0]
  000edc24: vsub.f32 s0,s12,s6
  000edc28: vcmpe.f32 s8,s2
  000edc2c: vstr.32 s8,[r4,#0x170]
  000edc30: vmrs apsr,fpscr
  000edc34: vstr.32 s4,[r4,#0x174]
  000edc38: vstr.32 s0,[r4,#0x178]
  000edc3c: bpl 0x000edc56
  000edc3e: vldr.32 s6,[pc,#0x288]
  000edc42: vcmpe.f32 s8,s6
  000edc46: vmrs apsr,fpscr
  000edc4a: itt gt
  000edc4c: vcmpe.gt.f32 s0,s6
  000edc50: vmrs.gt apsr,fpscr
  000edc54: bgt 0x000edd38
  000edc56: ldr.w r0,[r4,#0x150]
  000edc5a: cbz r0,0x000edc7e
  000edc5c: movs r5,#0x0
  000edc5e: b 0x000edc78
  000edc60: ldr r0,[r0,#0x4]
  000edc62: add.w r3,r4,#0x2c
  000edc66: ldr.w r0,[r0,r5,lsl #0x2]
  000edc6a: ldr r6,[r0,#0x0]
  000edc6c: ldmia r3,{r1,r2,r3}
  000edc6e: ldr r6,[r6,#0x4]
  000edc70: blx r6
  000edc72: ldr.w r0,[r4,#0x150]
  000edc76: adds r5,#0x1
  000edc78: ldr r1,[r0,#0x0]
  000edc7a: cmp r5,r1
  000edc7c: bcc 0x000edc60
  000edc7e: ldr r0,[r4,#0x4]
  000edc80: blx 0x000724f0
  000edc84: mov r11,r0
  000edc86: vmov s0,r11
  000edc8a: vcvt.f32.s32 s16,s0
  000edc8e: ldrb.w r0,[r4,#0x1fc]
  000edc92: cbz r0,0x000edcda
  000edc94: ldr r0,[r4,#0x4]
  000edc96: blx 0x00072910
  000edc9a: vmov s0,r0
  000edc9e: vldr.32 s2,[pc,#0x22c]
  000edca2: vcvt.f32.s32 s0,s0
  000edca6: vmul.f32 s0,s0,s2
  000edcaa: vcmpe.f32 s0,s16
  000edcae: vmrs apsr,fpscr
  000edcb2: bhi 0x000edd94
  000edcb4: ldr r0,[r4,#0x54]
  000edcb6: movs r2,#0x0
  000edcb8: ldr.w r1,[r4,#0x80]
  000edcbc: strb.w r2,[r4,#0x1fc]
  000edcc0: movs r2,#0x0
  000edcc2: ldr r0,[r0,#0x78]
  000edcc4: blx 0x000723c4
  000edcc8: ldr r0,[r4,#0x54]
  000edcca: movs r2,#0x0
  000edccc: ldr.w r1,[r4,#0x84]
  000edcd0: ldr.w r0,[r0,#0x84]
  000edcd4: blx 0x000723c4
  000edcd8: b 0x000edd94
  000edcda: ldr r0,[r4,#0x4]
  000edcdc: blx 0x00072910
  000edce0: vmov s0,r0
  000edce4: vldr.32 s2,[pc,#0x1e4]
  000edce8: vcvt.f32.s32 s0,s0
  000edcec: vmul.f32 s0,s0,s2
  000edcf0: vcmpe.f32 s0,s16
  000edcf4: vmrs apsr,fpscr
  000edcf8: ble 0x000edd2e
  000edcfa: ldr r0,[0x000eded0]
  000edcfc: add r0,pc
  000edcfe: ldr r0,[r0,#0x0]
  000edd00: vldr.32 s0,[r0,#0x28]
  000edd04: vcmpe.f32 s0,#0
  000edd08: vmrs apsr,fpscr
  000edd0c: ble 0x000edd8e
  000edd0e: mov r0,r4
  000edd10: blx 0x000758a4
  000edd14: mov r2,r0
  000edd16: ldr r0,[r4,#0x54]
  000edd18: ldr.w r1,[r4,#0x80]
  000edd1c: cmp r2,#0x0
  000edd1e: ldr r0,[r0,#0x78]
  000edd20: beq 0x000edd78
  000edd22: movs r2,#0x0
  000edd24: blx 0x000723c4
  000edd28: ldr r0,[r4,#0x54]
  000edd2a: movs r2,#0x0
  000edd2c: b 0x000edd82
  000edd2e: ldrb.w r0,[r4,#0x1fc]
  000edd32: cmp r0,#0x0
  000edd34: bne 0x000edc94
  000edd36: b 0x000edd94
  000edd38: vcmpe.f32 s4,s2
  000edd3c: vmrs apsr,fpscr
  000edd40: bpl.w 0x000edc56
  000edd44: vcmpe.f32 s4,s6
  000edd48: vmrs apsr,fpscr
  000edd4c: ble.w 0x000edc56
  000edd50: vcmpe.f32 s0,s2
  000edd54: vmrs apsr,fpscr
  000edd58: bpl.w 0x000edc56
  000edd5c: ldr r0,[r4,#0xc]
  000edd5e: movs r1,#0x1
  000edd60: str.w r1,[r4,#0x88]
  000edd64: cbnz r0,0x000edd68
  000edd66: ldr r0,[r4,#0x8]
  000edd68: movs r1,#0x1
  000edd6a: blx 0x00072d24
  000edd6e: ldr r0,[r4,#0x4]
  000edd70: movs r1,#0x1
  000edd72: blx 0x00072580
  000edd76: b 0x000edc56
  000edd78: movs r2,#0x1
  000edd7a: blx 0x000723c4
  000edd7e: ldr r0,[r4,#0x54]
  000edd80: movs r2,#0x1
  000edd82: ldr.w r1,[r4,#0x84]
  000edd86: ldr.w r0,[r0,#0x84]
  000edd8a: blx 0x000723c4
  000edd8e: movs r0,#0x1
  000edd90: strb.w r0,[r4,#0x1fc]
  000edd94: cmp.w r11,#0x0
  000edd98: bgt.w 0x000ee1a8
  000edd9c: ldr.w r0,[r4,#0x88]
  000edda0: subs r0,#0x3
  000edda2: cmp r0,#0x2
  000edda4: bcc.w 0x000ee1a8
  000edda8: ldr r0,[sp,#0xd0]
  000eddaa: ldr r0,[r0,#0x0]
  000eddac: blx 0x00073444
  000eddb0: mov r5,r0
  000eddb2: cmp r0,#0x0
  000eddb4: beq.w 0x000edf18
  000eddb8: mov r0,r5
  000eddba: blx 0x00072874
  000eddbe: cmp r0,#0xd
  000eddc0: bne.w 0x000edf18
  000eddc4: ldr r0,[sp,#0xd0]
  000eddc6: ldr r0,[r0,#0x0]
  000eddc8: blx 0x00071c14
  000eddcc: blx 0x00071824
  000eddd0: mov r6,r9
  000eddd2: mov r9,r0
  000eddd4: mov r0,r5
  000eddd6: blx 0x00073738
  000eddda: cmp r9,r0
  000edddc: mov r9,r6
  000eddde: bne.w 0x000edf18
  000edde2: ldr r0,[sp,#0xd0]
  000edde4: ldr r1,[r0,#0x0]
  000edde6: ldrb.w r0,[r1,#0xf1]
  000eddea: cmp r0,#0x0
  000eddec: bne.w 0x000edf18
  000eddf0: ldr r0,[r4,#0x20]
  000eddf2: cbz r0,0x000ede16
  000eddf4: ldr r0,[0x000eded4]
  000eddf6: movw r1,#0x67f
  000eddfa: add r0,pc
  000eddfc: ldr r0,[r0,#0x0]
  000eddfe: ldr r0,[r0,#0x0]
  000ede00: blx 0x00072f70
  000ede04: mov r1,r0
  000ede06: add.w r0,r4,#0x18
  000ede0a: blx 0x0006f664
  000ede0e: ldr r1,[sp,#0xd0]
  000ede10: cmp r0,#0x0
  000ede12: ldr r1,[r1,#0x0]
  000ede14: beq 0x000edf12
  000ede16: mov.w r0,#0x100
  000ede1a: strh.w r0,[r1,#0xf0]
  000ede1e: b 0x000edf18
  000ede20: ldr r0,[r4,#0x38]
  000ede22: add r5,sp,#0x198
  000ede24: ldr r1,[r6,#0x4]
  000ede26: ldr.w r1,[r1,r0,lsl #0x2]
  000ede2a: mov r0,r5
  000ede2c: blx 0x00072964
  000ede30: add.w r0,r4,#0x90
  000ede34: mov r1,r5
  000ede36: blx 0x0006eb3c
  000ede3a: ldr.w r0,[r4,#0x128]
  000ede3e: vldr.32 s2,[r4,#0x2c]
  000ede42: vldr.32 s4,[r4,#0x90]
  000ede46: vmov s0,r0
  000ede4a: vsub.f32 s4,s2,s4
  000ede4e: vcvt.f32.s32 s0,s0
  000ede52: vcmpe.f32 s4,s0
  000ede56: vmrs apsr,fpscr
  000ede5a: bpl.w 0x000ed7a6
  000ede5e: rsbs r0,r0
  000ede60: vmov s2,r0
  000ede64: vcvt.f32.s32 s2,s2
  000ede68: vcmpe.f32 s4,s2
  000ede6c: vmrs apsr,fpscr
  000ede70: ble.w 0x000ed7a6
  000ede74: vldr.32 s4,[r4,#0x30]
  000ede78: vldr.32 s6,[r4,#0x94]
  000ede7c: vsub.f32 s4,s4,s6
  000ede80: vcmpe.f32 s4,s0
  000ede84: vmrs apsr,fpscr
  000ede88: bpl.w 0x000ed7a6
  000ede8c: vcmpe.f32 s4,s2
  000ede90: vmrs apsr,fpscr
  000ede94: ble.w 0x000ed7a6
  000ede98: vldr.32 s4,[r4,#0x34]
  000ede9c: vldr.32 s6,[r4,#0x98]
  000edea0: vsub.f32 s4,s4,s6
  000edea4: vcmpe.f32 s4,s0
  000edea8: vmrs apsr,fpscr
  000edeac: bpl.w 0x000ed7a6
  000edeb0: vcmpe.f32 s4,s2
  000edeb4: vmrs apsr,fpscr
  000edeb8: bgt.w 0x000ed7ac
  000edebc: b 0x000ed7a6
  000eded8: ldr r1,[0x000ee278]
  000ededa: ldr r0,[r4,#0x6c]
  000ededc: add r1,pc
  000edede: ldr r5,[r1,#0x0]
  000edee0: blx r5
  000edee2: vldr.32 s0,[r0,#0x124]
  000edee6: vcvt.f32.s32 s0,s0
  000edeea: ldr r0,[r4,#0x6c]
  000edeec: vstr.32 s0,[r4,#0x164]
  000edef0: blx r5
  000edef2: vldr.32 s0,[r0,#0x128]
  000edef6: vcvt.f32.s32 s0,s0
  000edefa: ldr r0,[r4,#0x6c]
  000edefc: vstr.32 s0,[r4,#0x168]
  000edf00: blx r5
  000edf02: vldr.32 s0,[r0,#0x12c]
  000edf06: vcvt.f32.s32 s0,s0
  000edf0a: vstr.32 s0,[r4,#0x16c]
  000edf0e: b.w 0x000ef766
  000edf12: movs r0,#0x1
  000edf14: strb.w r0,[r1,#0xf0]
  000edf18: ldrb.w r0,[r4,#0x42]
  000edf1c: str.w r8,[sp,#0xc8]
  000edf20: cmp r0,#0x0
  000edf22: beq 0x000edfbc
  000edf24: mov r8,r9
  000edf26: mov r9,r10
  000edf28: ldr.w r10,[sp,#0xd0]
  000edf2c: ldr r1,[r4,#0x48]
  000edf2e: ldr.w r0,[r10,#0x0]
  000edf32: ldr r0,[r0,#0x0]
  000edf34: ldr r0,[r0,#0x4]
  000edf36: ldr.w r0,[r0,r1,lsl #0x2]
  000edf3a: movs r1,#0x1
  000edf3c: blx 0x00074a10
  000edf40: ldr.w r0,[r10,#0x0]
  000edf44: ldr r1,[r4,#0x48]
  000edf46: ldr r0,[r0,#0x0]
  000edf48: ldr r0,[r0,#0x4]
  000edf4a: ldr.w r0,[r0,r1,lsl #0x2]
  000edf4e: movs r1,#0x0
  000edf50: blx 0x00073a20
  000edf54: ldr r0,[0x000ee27c]
  000edf56: ldr.w r1,[r10,#0x0]
  000edf5a: add r0,pc
  000edf5c: ldr r2,[r4,#0x48]
  000edf5e: ldr r1,[r1,#0x0]
  000edf60: ldr r0,[r0,#0x0]
  000edf62: ldr r1,[r1,#0x4]
  000edf64: ldr r5,[r0,#0x0]
  000edf66: ldr.w r0,[r1,r2,lsl #0x2]
  000edf6a: blx 0x00074c44
  000edf6e: mov r1,r0
  000edf70: mov r0,r5
  000edf72: movs r2,#0x1
  000edf74: blx 0x0007519c
  000edf78: ldr.w r5,[r10,#0x0]
  000edf7c: mov r0,r5
  000edf7e: blx 0x000733d8
  000edf82: mov r6,r0
  000edf84: ldr.w r0,[r10,#0x0]
  000edf88: ldr r1,[r4,#0x48]
  000edf8a: ldr r0,[r0,#0x0]
  000edf8c: ldr r0,[r0,#0x4]
  000edf8e: ldr.w r0,[r0,r1,lsl #0x2]
  000edf92: blx 0x00074c44
  000edf96: adds r1,r0,r6
  000edf98: mov r0,r5
  000edf9a: blx 0x000758b0
  000edf9e: ldr.w r5,[r10,#0x0]
  000edfa2: mov r10,r9
  000edfa4: ldr r1,[r4,#0x48]
  000edfa6: mov r9,r8
  000edfa8: ldr r0,[r5,#0x0]
  000edfaa: ldr r0,[r0,#0x4]
  000edfac: ldr.w r0,[r0,r1,lsl #0x2]
  000edfb0: blx 0x00073a08
  000edfb4: mov r1,r0
  000edfb6: mov r0,r5
  000edfb8: blx 0x000758bc
  000edfbc: ldr r0,[r4,#0x4]
  000edfbe: str.w r9,[sp,#0xcc]
  000edfc2: ldrb.w r1,[r0,#0x5c]
  000edfc6: cbz r1,0x000edfe4
  000edfc8: ldr r1,[r4,#0x28]
  000edfca: cmp r1,#0x9
  000edfcc: beq 0x000ee000
  000edfce: cmp r1,#0x8
  000edfd0: bne 0x000ee02e
  000edfd2: ldrb.w r0,[r0,#0x44]
  000edfd6: cmp r0,#0x0
  000edfd8: bne 0x000ee062
  000edfda: ldr r0,[sp,#0xd0]
  000edfdc: ldr r0,[r0,#0x0]
  000edfde: blx 0x000758c8
  000edfe2: b 0x000ee062
  000edfe4: ldr r0,[r4,#0x54]
  000edfe6: mov r8,r10
  000edfe8: blx 0x000758d4
  000edfec: ldrb.w r0,[r4,#0xdc]
  000edff0: cmp r0,#0x0
  000edff2: beq 0x000ee074
  000edff4: ldr r0,[r4,#0x54]
  000edff6: add.w r1,r4,#0x18
  000edffa: blx 0x000758e0
  000edffe: b 0x000ee074
  000ee000: movs r0,#0xc
  000ee002: blx 0x0006eb24
  000ee006: mov r5,r0
  000ee008: blx 0x000701f8
  000ee00c: movs r0,#0x83
  000ee00e: mov r1,r5
  000ee010: str r5,[r4,#0x50]
  000ee012: blx 0x0007021c
  000ee016: ldr r0,[0x000ee280]
  000ee018: movs r1,#0x3
  000ee01a: add r0,pc
  000ee01c: ldr r0,[r0,#0x0]
  000ee01e: ldr r0,[r0,#0x0]
  000ee020: blx 0x00071848
  000ee024: ldr r1,[r4,#0x50]
  000ee026: adds r0,#0x1
  000ee028: blx 0x0007021c
  000ee02c: b 0x000ee062
  000ee02e: ldrb.w r0,[r4,#0xd0]
  000ee032: cbnz r0,0x000ee062
  000ee034: ldrb.w r0,[r4,#0x4c]
  000ee038: cbz r0,0x000ee062
  000ee03a: ldr r0,[0x000ee284]
  000ee03c: add r0,pc
  000ee03e: ldr r0,[r0,#0x0]
  000ee040: ldrb.w r0,[r0,#0x35]
  000ee044: cbz r0,0x000ee062
  000ee046: ldr r0,[0x000ee288]
  000ee048: movs r1,#0x64
  000ee04a: add r0,pc
  000ee04c: ldr r0,[r0,#0x0]
  000ee04e: ldr r0,[r0,#0x0]
  000ee050: blx 0x00071848
  000ee054: cmp.w r0,#0xffffffff
  000ee058: itt le
  000ee05a: ldr.le r0,[r4,#0x28]
  000ee05c: cmp.le r0,#0x3
  000ee05e: ble.w 0x000ef786
  000ee062: ldr r2,[r4,#0x4]
  000ee064: mov r8,r10
  000ee066: ldr r0,[r4,#0x54]
  000ee068: ldr.w r1,[r4,#0xac]
  000ee06c: ldrb.w r2,[r2,#0x44]
  000ee070: blx 0x000758ec
  000ee074: ldr r0,[0x000ee28c]
  000ee076: movs r2,#0x3
  000ee078: ldr r1,[0x000ee290]
  000ee07a: mov.w r10,#0x0
  000ee07e: add r0,pc
  000ee080: str.w r2,[r4,#0x88]
  000ee084: add r1,pc
  000ee086: strb.w r10,[r4,#0x73]
  000ee08a: ldr r6,[r0,#0x0]
  000ee08c: ldr.w r9,[r1,#0x0]
  000ee090: movw r1,#0x5dc
  000ee094: ldr r0,[r6,#0x0]
  000ee096: blx r9
  000ee098: addw r0,r0,#0x5dc
  000ee09c: str.w r0,[r4,#0x1f8]
  000ee0a0: add r5,sp,#0x198
  000ee0a2: ldr r1,[r4,#0x8]
  000ee0a4: mov r0,r5
  000ee0a6: blx 0x00072574
  000ee0aa: add.w r0,r4,#0x194
  000ee0ae: mov r1,r5
  000ee0b0: blx 0x0006eb3c
  000ee0b4: ldr r0,[r4,#0x54]
  000ee0b6: movs r3,#0x0
  000ee0b8: ldr r2,[0x000ee294]
  000ee0ba: ldr r1,[r0,#0x3c]
  000ee0bc: ldr r0,[r0,#0x74]
  000ee0be: strd r10,r10,[sp,#0x198]
  000ee0c2: str.w r10,[sp,#0x1a0]
  000ee0c6: strd r5,r2,[sp,#0x0]
  000ee0ca: ldr r5,[sp,#0xd4]
  000ee0cc: mov r2,r5
  000ee0ce: blx 0x00072a30
  000ee0d2: ldr r0,[0x000ee298]
  000ee0d4: movs r3,#0x0
  000ee0d6: ldr r1,[0x000ee29c]
  000ee0d8: add r0,pc
  000ee0da: add r1,pc
  000ee0dc: ldr r0,[r0,#0x0]
  000ee0de: ldr r1,[r1,#0x0]
  000ee0e0: ldr r0,[r0,#0x0]
  000ee0e2: ldrb r1,[r1,#0xf]
  000ee0e4: str.w r10,[sp,#0x0]
  000ee0e8: cmp r1,#0x0
  000ee0ea: it eq
  000ee0ec: mov.eq r5,r1
  000ee0ee: movs r1,#0x14
  000ee0f0: mov r2,r5
  000ee0f2: blx 0x00071548
  000ee0f6: ldr r0,[r6,#0x0]
  000ee0f8: movs r1,#0xc8
  000ee0fa: blx r9
  000ee0fc: sub.w r1,r0,#0x64
  000ee100: ldr r0,[r6,#0x0]
  000ee102: vmov s0,r1
  000ee106: movs r1,#0xc8
  000ee108: vcvt.f32.s32 s16,s0
  000ee10c: blx r9
  000ee10e: sub.w r1,r0,#0x64
  000ee112: ldr r0,[r6,#0x0]
  000ee114: vmov s0,r1
  000ee118: movs r1,#0xc8
  000ee11a: vcvt.f32.s32 s18,s0
  000ee11e: blx r9
  000ee120: subs r0,#0x64
  000ee122: add r5,sp,#0x198
  000ee124: add r1,sp,#0x140
  000ee126: vmov s0,r0
  000ee12a: mov r0,r5
  000ee12c: vcvt.f32.s32 s0,s0
  000ee130: vstr.32 s16,[sp,#0x140]
  000ee134: vstr.32 s18,[sp,#0x144]
  000ee138: vstr.32 s0,[sp,#0x148]
  000ee13c: blx 0x0006ec80
  000ee140: add.w r6,r4,#0x188
  000ee144: mov r1,r5
  000ee146: mov r0,r6
  000ee148: blx 0x0006eb3c
  000ee14c: ldr r1,[0x000ee2a0]
  000ee14e: mov r0,r6
  000ee150: blx 0x00072628
  000ee154: ldr r2,[0x000ee2a4]
  000ee156: ldr r0,[r4,#0x54]
  000ee158: add r2,pc
  000ee15a: ldr.w r1,[r4,#0x1a0]
  000ee15e: ldr r5,[r2,#0x0]
  000ee160: movs r2,#0x1
  000ee162: ldr r0,[r0,#0x74]
  000ee164: blx r5
  000ee166: ldr r0,[r4,#0x54]
  000ee168: movs r2,#0x0
  000ee16a: ldr.w r1,[r4,#0x130]
  000ee16e: ldr.w r0,[r0,#0x8c]
  000ee172: blx r5
  000ee174: ldr r0,[r4,#0x54]
  000ee176: movs r2,#0x0
  000ee178: ldr.w r1,[r4,#0x134]
  000ee17c: ldr.w r0,[r0,#0x8c]
  000ee180: blx r5
  000ee182: ldr r0,[r4,#0x54]
  000ee184: movs r2,#0x0
  000ee186: ldr.w r1,[r4,#0x138]
  000ee18a: ldr.w r0,[r0,#0x8c]
  000ee18e: blx r5
  000ee190: mov r0,r4
  000ee192: blx 0x000758f8
  000ee196: strb.w r0,[r4,#0x4c]
  000ee19a: mov r0,r4
  000ee19c: movs r1,#0x0
  000ee19e: blx 0x000721f0
  000ee1a2: mov r10,r8
  000ee1a4: ldrd r8,r9,[sp,#0xc8]
  000ee1a8: ldrb.w r0,[r4,#0x2e4]
  000ee1ac: cbz r0,0x000ee1bc
  000ee1ae: ldr r0,[r4,#0x8]
  000ee1b0: blx 0x000742b4
  000ee1b4: mov r0,r4
  000ee1b6: blx 0x000757e4
  000ee1ba: b 0x000ee1dc
  000ee1bc: ldr.w r0,[r4,#0x88]
  000ee1c0: cmp r0,#0x9
  000ee1c2: bhi 0x000ee1dc
  000ee1c4: tbh [pc,r0]
  000ee1dc: ldr r0,[sp,#0x1d4]
  000ee1de: ldr.w r1,[r10,#0x0]
  000ee1e2: subs r0,r1,r0
  000ee1e4: itttt eq
  000ee1e6: add.eq sp,#0x1d8
  000ee1e8: vpop.eq {d8,d9}
  000ee1ec: add.eq sp,#0x4
  000ee1ee: pop.eq.w {r8,r9,r10,r11}
  000ee1f2: it eq
  000ee1f4: pop.eq {r4,r5,r6,r7,pc}
  000ee1f6: blx 0x0006e824
  000ee1fa: mov r0,r4
  000ee1fc: add.w r6,r4,#0x170
  000ee200: blx 0x000757e4
  000ee204: ldrb.w r0,[r4,#0x13d]
  000ee208: cmp r0,#0x0
  000ee20a: beq.w 0x000ee9c6
  000ee20e: ldr.w r1,[r4,#0x1d8]
  000ee212: cmp r1,r11
  000ee214: ble.w 0x000ee840
  000ee218: ldr.w r2,[r4,#0x1dc]
  000ee21c: sub.w r1,r1,r11
  000ee220: ldr r0,[r4,#0x4]
  000ee222: add r1,r2
  000ee224: vmov s0,r1
  000ee228: strd r11,r1,[r4,#0x1d8]
  000ee22c: vcvt.f32.s32 s16,s0
  000ee230: blx 0x00072910
  000ee234: vmov s0,r0
  000ee238: vldr.32 s2,[pc,#0x6c]
  000ee23c: vcvt.f32.s32 s0,s0
  000ee240: vdiv.f32 s0,s16,s0
  000ee244: vmul.f32 s0,s0,s2
  000ee248: vldr.32 s2,[pc,#0x60]
  000ee24c: vcmpe.f32 s0,s2
  000ee250: vmrs apsr,fpscr
  000ee254: ble.w 0x000ee840
  000ee258: movw r0,#0x2710
  000ee25c: str.w r0,[r4,#0x1c0]
  000ee260: movs r0,#0x0
  000ee262: str.w r0,[r4,#0x1dc]
  000ee266: movs r0,#0x1
  000ee268: strb.w r0,[r4,#0x1e0]
  000ee26c: b 0x000ee84c
  000ee26e: movs r0,#0x1
  000ee270: str.w r0,[r4,#0x88]
  000ee274: b 0x000ee1dc
  000ee2c0: adr.w r0,0xee2b0
  000ee2c4: vmov.i32 q9,#0x0
  000ee2c8: vld1.64 {d16,d17},[r0]
  000ee2cc: movs r0,#0x0
  000ee2ce: add r1,sp,#0x198
  000ee2d0: strb.w r0,[r4,#0x101]
  000ee2d4: strb.w r0,[r4,#0x13e]
  000ee2d8: orr r0,r1,#0x4
  000ee2dc: vst1.32 {d18,d19},[r0]
  000ee2e0: add.w r0,r1,#0x18
  000ee2e4: vst1.64 {d18,d19},[r0]
  000ee2e8: add.w r0,r1,#0x28
  000ee2ec: vst1.64 {d16,d17},[r0]
  000ee2f0: mov.w r0,#0x3f800000
  000ee2f4: str r0,[sp,#0x198]
  000ee2f6: str r0,[sp,#0x1ac]
  000ee2f8: str r0,[sp,#0x1d0]
  000ee2fa: add r0,sp,#0x140
  000ee2fc: ldrd r2,r3,[r4,#0x188]
  000ee300: vldr.32 s0,[r4,#0x190]
  000ee304: vstr.32 s0,[sp]
  000ee308: blx 0x00072094
  000ee30c: cmp.w r8,#0x1
  000ee310: blt 0x000ee32e
  000ee312: ldr r5,[r4,#0x8]
  000ee314: mov r0,r5
  000ee316: blx 0x000721cc
  000ee31a: add r6,sp,#0x140
  000ee31c: add r2,sp,#0x198
  000ee31e: mov r1,r0
  000ee320: mov r0,r6
  000ee322: blx 0x0006f82c
  000ee326: mov r0,r5
  000ee328: mov r1,r6
  000ee32a: blx 0x000721e4
  000ee32e: vmov s0,r8
  000ee332: add.w r9,sp,#0xd8
  000ee336: add.w r1,r4,#0x194
  000ee33a: vcvt.f32.s32 s0,s0
  000ee33e: mov r0,r9
  000ee340: ldr r5,[r4,#0x8]
  000ee342: vmov r2,s0
  000ee346: blx 0x0006ec74
  000ee34a: add r6,sp,#0x140
  000ee34c: ldr.w r2,[r4,#0x1e8]
  000ee350: mov r1,r9
  000ee352: mov r0,r6
  000ee354: blx 0x0006ec74
  000ee358: mov r0,r5
  000ee35a: mov r1,r6
  000ee35c: blx 0x00072460
  000ee360: ldr r0,[r4,#0x8]
  000ee362: blx 0x000742b4
  000ee366: ldr.w r0,[r4,#0x1f8]
  000ee36a: sub.w r0,r0,r8
  000ee36e: str.w r0,[r4,#0x1f8]
  000ee372: cmp.w r0,#0xffffffff
  000ee376: bgt.w 0x000ee1dc
  000ee37a: mov.w r8,#0x0
  000ee37e: ldr.w r0,[r4,#0x124]
  000ee382: strd r8,r8,[sp,#0x140]
  000ee386: add r2,sp,#0x140
  000ee388: ldr r1,[sp,#0xd4]
  000ee38a: str.w r8,[sp,#0x148]
  000ee38e: blx 0x00072808
  000ee392: ldr r0,[r4,#0x54]
  000ee394: movs r2,#0x0
  000ee396: ldr.w r1,[r4,#0x1a0]
  000ee39a: ldr r0,[r0,#0x74]
  000ee39c: blx 0x000723c4
  000ee3a0: ldr r0,[0x000ee6ec]
  000ee3a2: add r0,pc
  000ee3a4: ldr r0,[r0,#0x0]
  000ee3a6: vldr.32 s0,[r0,#0x28]
  000ee3aa: vcmpe.f32 s0,#0
  000ee3ae: vmrs apsr,fpscr
  000ee3b2: ble 0x000ee3d2
  000ee3b4: ldr r0,[r4,#0x54]
  000ee3b6: movs r2,#0x0
  000ee3b8: ldr.w r1,[r4,#0x80]
  000ee3bc: ldr r0,[r0,#0x78]
  000ee3be: blx 0x000723c4
  000ee3c2: ldr r0,[r4,#0x54]
  000ee3c4: movs r2,#0x0
  000ee3c6: ldr.w r1,[r4,#0x84]
  000ee3ca: ldr.w r0,[r0,#0x84]
  000ee3ce: blx 0x000723c4
  000ee3d2: ldr r0,[0x000ee6f0]
  000ee3d4: ldr r1,[0x000ee6f4]
  000ee3d6: add r0,pc
  000ee3d8: ldr.w r9,[r4,#0x4]
  000ee3dc: add r1,pc
  000ee3de: ldr r5,[r0,#0x0]
  000ee3e0: ldr r6,[r1,#0x0]
  000ee3e2: movs r1,#0x32
  000ee3e4: ldr r0,[r5,#0x0]
  000ee3e6: blx r6
  000ee3e8: vmov s0,r0
  000ee3ec: vldr.32 s2,[pc,#0x308]
  000ee3f0: vldr.32 s4,[pc,#0x308]
  000ee3f4: mov r0,r9
  000ee3f6: vcvt.f32.s32 s0,s0
  000ee3fa: vmla.f32 s4,s0,s2
  000ee3fe: vmov r1,s4
  000ee402: blx 0x00072aa8
  000ee406: ldr r0,[r5,#0x0]
  000ee408: movs r1,#0xc8
  000ee40a: blx r6
  000ee40c: sub.w r1,r0,#0x64
  000ee410: ldr r0,[r5,#0x0]
  000ee412: vmov s0,r1
  000ee416: movs r1,#0xc8
  000ee418: vcvt.f32.s32 s16,s0
  000ee41c: blx r6
  000ee41e: sub.w r1,r0,#0x64
  000ee422: ldr r0,[r5,#0x0]
  000ee424: vmov s0,r1
  000ee428: movs r1,#0xc8
  000ee42a: vcvt.f32.s32 s18,s0
  000ee42e: blx r6
  000ee430: subs r0,#0x64
  000ee432: add r5,sp,#0x140
  000ee434: add r1,sp,#0xd8
  000ee436: vmov s0,r0
  000ee43a: mov r0,r5
  000ee43c: vcvt.f32.s32 s0,s0
  000ee440: vstr.32 s16,[sp,#0xd8]
  000ee444: vstr.32 s18,[sp,#0xdc]
  000ee448: vstr.32 s0,[sp,#0xe0]
  000ee44c: blx 0x0006ec80
  000ee450: add.w r0,r4,#0x17c
  000ee454: mov r1,r5
  000ee456: blx 0x0006eb3c
  000ee45a: str.w r8,[r4,#0x1f8]
  000ee45e: movs r1,#0x4
  000ee460: ldr.w r0,[r4,#0x2c4]
  000ee464: str.w r1,[r4,#0x88]
  000ee468: cbz r0,0x000ee47e
  000ee46a: blx 0x000732c4
  000ee46e: cbnz r0,0x000ee47e
  000ee470: ldr.w r0,[r4,#0x2c4]
  000ee474: blx 0x00072b80
  000ee478: movs r0,#0x0
  000ee47a: str.w r0,[r4,#0x2c4]
  000ee47e: ldrb.w r0,[r4,#0x4c]
  000ee482: cmp r0,#0x0
  000ee484: beq.w 0x000ee1dc
  000ee488: ldrb.w r0,[r4,#0xd0]
  000ee48c: cbz r0,0x000ee498
  000ee48e: ldrb.w r0,[r4,#0x69]
  000ee492: cmp r0,#0x0
  000ee494: beq.w 0x000ef458
  000ee498: ldr r0,[r4,#0x28]
  000ee49a: subs r0,#0x9
  000ee49c: clz r0,r0
  000ee4a0: lsrs r0,r0,#0x5
  000ee4a2: lsls r1,r0,#0x2
  000ee4a4: mov r0,r4
  000ee4a6: blx 0x00075904
  000ee4aa: b 0x000ee1dc
  000ee4ac: ldr.w r0,[r4,#0x1f8]
  000ee4b0: add r0,r8
  000ee4b2: str.w r0,[r4,#0x1f8]
  000ee4b6: cmp r0,#0x1
  000ee4b8: blt.w 0x000eea08
  000ee4bc: movs r0,#0x0
  000ee4be: strb.w r0,[r4,#0x73]
  000ee4c2: b 0x000eeaa4
  000ee4c4: ldr r0,[r4,#0x4]
  000ee4c6: ldrb.w r0,[r0,#0x5c]
  000ee4ca: cbz r0,0x000ee4e4
  000ee4cc: ldr r0,[sp,#0xd0]
  000ee4ce: ldr r0,[r0,#0x0]
  000ee4d0: blx 0x00071770
  000ee4d4: cmp r0,#0x2
  000ee4d6: blt 0x000ee4e4
  000ee4d8: ldr r0,[r4,#0xc]
  000ee4da: cbnz r0,0x000ee4de
  000ee4dc: ldr r0,[r4,#0x8]
  000ee4de: movs r1,#0x0
  000ee4e0: blx 0x00072d24
  000ee4e4: ldr.w r0,[r4,#0x148]
  000ee4e8: cmp r0,#0x0
  000ee4ea: beq.w 0x000ee1dc
  000ee4ee: ldrb.w r0,[r0,#0x5e]
  000ee4f2: cmp r0,#0x0
  000ee4f4: bne.w 0x000ee1dc
  000ee4f8: ldr.w r0,[r4,#0x128]
  000ee4fc: vmov s0,r0
  000ee500: vcvt.f32.s32 s0,s0
  000ee504: vldr.32 s4,[r4,#0x170]
  000ee508: vcmpe.f32 s4,s0
  000ee50c: vmrs apsr,fpscr
  000ee510: bpl.w 0x000ee1dc
  000ee514: rsbs r0,r0
  000ee516: vmov s2,r0
  000ee51a: vcvt.f32.s32 s2,s2
  000ee51e: vcmpe.f32 s4,s2
  000ee522: vmrs apsr,fpscr
  000ee526: ble.w 0x000ee1dc
  000ee52a: vldr.32 s4,[r4,#0x174]
  000ee52e: vcmpe.f32 s4,s0
  000ee532: vmrs apsr,fpscr
  000ee536: bpl.w 0x000ee1dc
  000ee53a: vcmpe.f32 s4,s2
  000ee53e: vmrs apsr,fpscr
  000ee542: ble.w 0x000ee1dc
  000ee546: vldr.32 s4,[r4,#0x178]
  000ee54a: vcmpe.f32 s4,s0
  000ee54e: vmrs apsr,fpscr
  000ee552: bpl.w 0x000ee1dc
  000ee556: vcmpe.f32 s4,s2
  000ee55a: vmrs apsr,fpscr
  000ee55e: ble.w 0x000ee1dc
  000ee562: ldr r0,[r4,#0xc]
  000ee564: movs r1,#0x1
  000ee566: str.w r1,[r4,#0x88]
  000ee56a: cbnz r0,0x000ee56e
  000ee56c: ldr r0,[r4,#0x8]
  000ee56e: movs r1,#0x1
  000ee570: blx 0x00072d24
  000ee574: ldr r0,[r4,#0x4]
  000ee576: movs r1,#0x1
  000ee578: blx 0x00072580
  000ee57c: ldrb.w r0,[r4,#0x12f]
  000ee580: cmp r0,#0x0
  000ee582: beq.w 0x000ee1dc
  000ee586: ldr r0,[r4,#0x54]
  000ee588: movs r1,#0x1
  000ee58a: blx 0x00075910
  000ee58e: b 0x000ee1dc
  000ee590: vldr.32 s0,[pc,#0x16c]
  000ee594: vmov s4,r8
  000ee598: vldr.32 s2,[r4,#0x1e8]
  000ee59c: vcvt.f32.s32 s4,s4
  000ee5a0: vmul.f32 s0,s2,s0
  000ee5a4: vmul.f32 s2,s0,s4
  000ee5a8: vcvt.s32.f32 s2,s2
  000ee5ac: vcvt.f32.s32 s2,s2
  000ee5b0: ldr r0,[r4,#0x8]
  000ee5b2: vstr.32 s0,[r4,#0x1e8]
  000ee5b6: vmov r1,s2
  000ee5ba: blx 0x000725c8
  000ee5be: vldr.32 s0,[pc,#-0x318]
  000ee5c2: vldr.32 s2,[r4,#0x1e8]
  000ee5c6: vcmpe.f32 s2,s0
  000ee5ca: vmrs apsr,fpscr
  000ee5ce: ble.w 0x000ee1dc
  000ee5d2: mov r0,r4
  000ee5d4: blx 0x0007408c
  000ee5d8: b 0x000ee1dc
  000ee5da: str.w r10,[sp,#0xc4]
  000ee5de: ldr r1,[0x000ee704]
  000ee5e0: ldr r0,[r4,#0x6c]
  000ee5e2: add r1,pc
  000ee5e4: ldr r6,[r1,#0x0]
  000ee5e6: blx r6
  000ee5e8: mov r10,r0
  000ee5ea: ldr r0,[r4,#0x6c]
  000ee5ec: blx r6
  000ee5ee: mov r1,r0
  000ee5f0: ldr r0,[r0,#0x0]
  000ee5f2: add.w r9,sp,#0x140
  000ee5f6: ldr r2,[r0,#0x28]
  000ee5f8: mov r0,r9
  000ee5fa: blx r2
  000ee5fc: ldr r0,[r4,#0x6c]
  000ee5fe: blx r6
  000ee600: ldr r0,[r0,#0x8]
  000ee602: blx 0x000721cc
  000ee606: ldr.w r2,[r4,#0x2c4]
  000ee60a: add.w r11,sp,#0xd8
  000ee60e: mov r1,r0
  000ee610: mov r0,r11
  000ee612: blx 0x0006f694
  000ee616: add r5,sp,#0x198
  000ee618: mov r1,r9
  000ee61a: mov r2,r11
  000ee61c: mov r0,r5
  000ee61e: blx 0x0006f1cc
  000ee622: mov r0,r10
  000ee624: mov r1,r5
  000ee626: blx 0x00072b98
  000ee62a: mov r5,r0
  000ee62c: ldr r0,[r4,#0x6c]
  000ee62e: blx r6
  000ee630: mov r1,r0
  000ee632: ldr r0,[r0,#0x0]
  000ee634: add.w r9,sp,#0x140
  000ee638: ldr r2,[r0,#0x28]
  000ee63a: mov r0,r9
  000ee63c: blx r2
  000ee63e: ldr r0,[r4,#0x6c]
  000ee640: blx r6
  000ee642: ldr r0,[r0,#0x8]
  000ee644: blx 0x000721cc
  000ee648: add r6,sp,#0xd8
  000ee64a: mov r1,r0
  000ee64c: mov r2,r5
  000ee64e: mov r0,r6
  000ee650: blx 0x0006f694
  000ee654: add r5,sp,#0x198
  000ee656: mov r1,r9
  000ee658: mov r2,r6
  000ee65a: mov r0,r5
  000ee65c: blx 0x0006f1cc
  000ee660: add.w r10,r4,#0x9c
  000ee664: mov r1,r5
  000ee666: mov r0,r10
  000ee668: blx 0x0006eb3c
  000ee66c: ldr r0,[r4,#0x0]
  000ee66e: add r5,sp,#0x140
  000ee670: mov r1,r4
  000ee672: ldr r2,[r0,#0x28]
  000ee674: mov r0,r5
  000ee676: blx r2
  000ee678: add r6,sp,#0x198
  000ee67a: mov r1,r10
  000ee67c: mov r2,r5
  000ee67e: mov r0,r6
  000ee680: blx 0x0006ec38
  000ee684: mov r0,r6
  000ee686: blx 0x0006ec44
  000ee68a: vmov s0,r0
  000ee68e: ldr r1,[0x000ee708]
  000ee690: ldr.w r2,[r4,#0xac]
  000ee694: vcvt.s32.f32 s0,s0
  000ee698: add r1,pc
  000ee69a: ldr.w r0,[r1,r2,lsl #0x2]
  000ee69e: vmov r1,s0
  000ee6a2: cmp r0,r1
  000ee6a4: ble.w 0x000ee8a4
  000ee6a8: mov r0,r4
  000ee6aa: movs r1,#0x0
  000ee6ac: movs r5,#0x0
  000ee6ae: blx 0x000721f0
  000ee6b2: strd r5,r5,[r4,#0x2bc]
  000ee6b6: movs r1,#0x9
  000ee6b8: ldrb.w r0,[r4,#0x1fc]
  000ee6bc: ldr.w r10,[sp,#0xc4]
  000ee6c0: cmp r0,#0x0
  000ee6c2: str.w r1,[r4,#0x88]
  000ee6c6: beq.w 0x000ee1dc
  000ee6ca: ldr r0,[r4,#0x54]
  000ee6cc: movs r2,#0x0
  000ee6ce: ldr.w r1,[r4,#0x80]
  000ee6d2: ldr r0,[r0,#0x78]
  000ee6d4: blx 0x000723c4
  000ee6d8: ldr r0,[r4,#0x54]
  000ee6da: movs r2,#0x0
  000ee6dc: ldr.w r1,[r4,#0x84]
  000ee6e0: ldr.w r0,[r0,#0x84]
  000ee6e4: blx 0x000723c4
  000ee6e8: b 0x000ee1dc
  000ee714: ldr r0,[sp,#0xd0]
  000ee716: ldr r0,[r0,#0x0]
  000ee718: blx 0x00072850
  000ee71c: mov r6,r0
  000ee71e: ldr.w r0,[r4,#0x2bc]
  000ee722: add r0,r8
  000ee724: str.w r0,[r4,#0x2bc]
  000ee728: mov r0,r6
  000ee72a: blx 0x00072868
  000ee72e: cmp r0,#0x0
  000ee730: bne 0x000ee7d8
  000ee732: mov r0,r6
  000ee734: blx 0x00072874
  000ee738: cmp r0,#0xb8
  000ee73a: bne 0x000ee7d8
  000ee73c: ldr r0,[r4,#0x6c]
  000ee73e: blx 0x00075868
  000ee742: blx 0x00072880
  000ee746: cmp r0,#0x1
  000ee748: bne 0x000ee7d8
  000ee74a: vldr.32 s0,[r4,#0x2bc]
  000ee74e: adr.w r1,0xee70c
  000ee752: vcvt.f32.s32 s0,s0
  000ee756: ldr r0,[r4,#0x7c]
  000ee758: cmp r0,#0x33
  000ee75a: it eq
  000ee75c: add.eq r1,#0x4
  000ee75e: vldr.32 s2,[r1]
  000ee762: ldr.w r8,[r4,#0x2c0]
  000ee766: vdiv.f32 s0,s0,s2
  000ee76a: vcvt.s32.f32 s0,s0
  000ee76e: ldr r0,[sp,#0xd0]
  000ee770: vstr.32 s0,[r4,#0x2c0]
  000ee774: ldr r0,[r0,#0x0]
  000ee776: blx 0x00071770
  000ee77a: cmp r0,#0x5e
  000ee77c: bne 0x000ee796
  000ee77e: mov r0,r6
  000ee780: blx 0x0007288c
  000ee784: mov r5,r0
  000ee786: ldr r0,[sp,#0xd0]
  000ee788: ldr r0,[r0,#0x0]
  000ee78a: blx 0x00071a58
  000ee78e: blx 0x00072bd4
  000ee792: cmp r5,r0
  000ee794: ble 0x000ee7d8
  000ee796: ldr.w r0,[r4,#0x2c0]
  000ee79a: cmp r0,r8
  000ee79c: ble 0x000ee7d8
  000ee79e: ldr r0,[r4,#0x54]
  000ee7a0: blx 0x00071f44
  000ee7a4: mov r5,r0
  000ee7a6: mov r0,r6
  000ee7a8: blx 0x00072bb0
  000ee7ac: cmp r5,r0
  000ee7ae: bge 0x000ee7d8
  000ee7b0: mov r0,r6
  000ee7b2: blx 0x0007288c
  000ee7b6: ldr.w r1,[r4,#0x2c0]
  000ee7ba: add r0,r8
  000ee7bc: subs r1,r0,r1
  000ee7be: mov r0,r6
  000ee7c0: blx 0x00072898
  000ee7c4: mov r0,r6
  000ee7c6: blx 0x0007288c
  000ee7ca: cmp.w r0,#0xffffffff
  000ee7ce: bgt 0x000ee7d8
  000ee7d0: mov r0,r6
  000ee7d2: movs r1,#0x0
  000ee7d4: blx 0x00072898
  000ee7d8: ldr r0,[r4,#0x6c]
  000ee7da: ldr.w r5,[r4,#0x2bc]
  000ee7de: blx 0x0007591c
  000ee7e2: cmp r5,r0
  000ee7e4: ble.w 0x000ee1dc
  000ee7e8: mov r0,r4
  000ee7ea: movs r1,#0x1
  000ee7ec: movs r6,#0x1
  000ee7ee: blx 0x000721f0
  000ee7f2: ldr r5,[r4,#0x6c]
  000ee7f4: mov r0,r5
  000ee7f6: blx 0x00072988
  000ee7fa: mov r1,r0
  000ee7fc: mov r0,r5
  000ee7fe: blx 0x00073fe4
  000ee802: ldrb.w r0,[r4,#0x1fc]
  000ee806: str.w r6,[r4,#0x88]
  000ee80a: cbz r0,0x000ee82a
  000ee80c: ldr r0,[r4,#0x54]
  000ee80e: movs r2,#0x1
  000ee810: ldr.w r1,[r4,#0x80]
  000ee814: ldr r0,[r0,#0x78]
  000ee816: blx 0x000723c4
  000ee81a: ldr r0,[r4,#0x54]
  000ee81c: movs r2,#0x1
  000ee81e: ldr.w r1,[r4,#0x84]
  000ee822: ldr.w r0,[r0,#0x84]
  000ee826: blx 0x000723c4
  000ee82a: ldr.w r0,[r4,#0x2c4]
  000ee82e: cmp r0,#0x0
  000ee830: beq.w 0x000ee1dc
  000ee834: blx 0x00072b80
  000ee838: movs r0,#0x0
  000ee83a: str.w r0,[r4,#0x2c4]
  000ee83e: b 0x000ee1dc
  000ee840: ldr.w r0,[r4,#0x1c0]
  000ee844: movw r1,#0x1388
  000ee848: cmp r0,r1
  000ee84a: ble 0x000ee934
  000ee84c: ldrb.w r0,[r4,#0x1f4]
  000ee850: add.w r5,r4,#0x1f4
  000ee854: cmp r0,#0x0
  000ee856: bne 0x000ee93e
  000ee858: ldrb.w r0,[r4,#0x1e0]
  000ee85c: movs r1,#0x0
  000ee85e: str.w r1,[r4,#0x1c0]
  000ee862: cbnz r0,0x000ee87a
  000ee864: ldr r0,[0x000eec18]
  000ee866: movs r1,#0x64
  000ee868: add r0,pc
  000ee86a: ldr r0,[r0,#0x0]
  000ee86c: ldr r0,[r0,#0x0]
  000ee86e: blx 0x00071848
  000ee872: ldr.w r1,[r4,#0x1b4]
  000ee876: cmp r0,r1
  000ee878: bge 0x000ee938
  000ee87a: ldr r0,[0x000eec1c]
  000ee87c: movw r1,#0xbb8
  000ee880: add r0,pc
  000ee882: ldr r0,[r0,#0x0]
  000ee884: ldr r0,[r0,#0x0]
  000ee886: blx 0x00071848
  000ee88a: ldr r1,[0x000eec20]
  000ee88c: movw r3,#0x1388
  000ee890: add r0,r3
  000ee892: str.w r0,[r4,#0x1c4]
  000ee896: movs r0,#0x1
  000ee898: ldr r2,[0x000eec24]
  000ee89a: strb.w r0,[r4,#0x1f4]
  000ee89e: strd r2,r1,[r4,#0x1ec]
  000ee8a2: b 0x000ee93e
  000ee8a4: vmov s0,r8
  000ee8a8: vcvt.f32.s32 s0,s0
  000ee8ac: ldr.w r0,[r4,#0x2b8]
  000ee8b0: vmov r5,s0
  000ee8b4: mov r1,r5
  000ee8b6: blx 0x00072bbc
  000ee8ba: add.w r8,sp,#0x198
  000ee8be: ldr.w r1,[r4,#0x2b8]
  000ee8c2: mov r0,r8
  000ee8c4: blx 0x00072bc8
  000ee8c8: ldr r0,[r4,#0x0]
  000ee8ca: add.w r9,sp,#0x188
  000ee8ce: mov r1,r4
  000ee8d0: ldr r2,[r0,#0x28]
  000ee8d2: mov r0,r9
  000ee8d4: blx r2
  000ee8d6: ldr r0,[r4,#0x0]
  000ee8d8: add r6,sp,#0x118
  000ee8da: mov r1,r4
  000ee8dc: ldr r2,[r0,#0x28]
  000ee8de: mov r0,r6
  000ee8e0: blx r2
  000ee8e2: add.w r11,sp,#0x124
  000ee8e6: mov r1,r10
  000ee8e8: mov r2,r6
  000ee8ea: mov r0,r11
  000ee8ec: blx 0x0006ec38
  000ee8f0: add r6,sp,#0x130
  000ee8f2: mov r1,r11
  000ee8f4: mov r2,r5
  000ee8f6: mov r0,r6
  000ee8f8: blx 0x0006ec74
  000ee8fc: add r5,sp,#0x17c
  000ee8fe: ldr r2,[0x000eec28]
  000ee900: mov r1,r6
  000ee902: mov r0,r5
  000ee904: blx 0x0006ec74
  000ee908: add r6,sp,#0xd8
  000ee90a: mov r1,r9
  000ee90c: mov r2,r5
  000ee90e: mov r0,r6
  000ee910: blx 0x0006f1cc
  000ee914: add r0,sp,#0x140
  000ee916: mov r1,r8
  000ee918: mov r2,r6
  000ee91a: blx 0x0006f1d8
  000ee91e: ldr r0,[r4,#0x8]
  000ee920: mov r1,r8
  000ee922: blx 0x000721e4
  000ee926: ldr r0,[r4,#0x4]
  000ee928: mov r1,r8
  000ee92a: adds r0,#0x4
  000ee92c: blx 0x0006f148
  000ee930: b.w 0x000ef450
  000ee934: add.w r5,r4,#0x1f4
  000ee938: ldrb r0,[r5,#0x0]
  000ee93a: cmp r0,#0x0
  000ee93c: beq 0x000ee9c6
  000ee93e: ldrd r0,r1,[r4,#0x1c0]
  000ee942: cmp r0,r1
  000ee944: ble 0x000ee962
  000ee946: ldr.w r1,[r4,#0x1ac]
  000ee94a: movs r2,#0x0
  000ee94c: ldr.w r0,[r4,#0x1a4]
  000ee950: strb.w r2,[r4,#0x1e0]
  000ee954: vmov s0,r1
  000ee958: str.w r2,[r4,#0x1c0]
  000ee95c: strd r1,r0,[r4,#0x1ec]
  000ee960: b 0x000ee966
  000ee962: vldr.32 s0,[r4,#0x1ec]
  000ee966: vcmpe.f32 s0,#0
  000ee96a: add.w r0,r4,#0x1ec
  000ee96e: vmrs apsr,fpscr
  000ee972: ble 0x000ee9c6
  000ee974: vldr.32 s4,[r4,#0x1e8]
  000ee978: adr.w r1,0xef840
  000ee97c: vldr.32 s2,[r4,#0x1ac]
  000ee980: vcmpe.f32 s0,s4
  000ee984: vmrs apsr,fpscr
  000ee988: it gt
  000ee98a: add.gt r1,#0x4
  000ee98c: vldr.32 s6,[r1]
  000ee990: vmul.f32 s4,s4,s6
  000ee994: vmov.f32 s6,0x40b00000
  000ee998: vstr.32 s4,[r4,#0x1e8]
  000ee99c: vcmpe.f32 s4,s6
  000ee9a0: vmrs apsr,fpscr
  000ee9a4: bge 0x000ee9b0
  000ee9a6: vcmpe.f32 s4,s2
  000ee9aa: vmrs apsr,fpscr
  000ee9ae: bpl 0x000ee9c6
  000ee9b0: vcmpe.f32 s0,s2
  000ee9b4: vstr.32 s0,[r4,#0x1e8]
  000ee9b8: vmrs apsr,fpscr
  000ee9bc: itt eq
  000ee9be: mov.eq r1,#0x0
  000ee9c0: strb.eq r1,[r5,#0x0]
  000ee9c2: movs r1,#0x0
  000ee9c4: str r1,[r0,#0x0]
  000ee9c6: ldr.w r0,[r4,#0x148]
  000ee9ca: cmp r0,#0x0
  000ee9cc: beq.w 0x000eecee
  000ee9d0: ldrb.w r1,[r4,#0x12c]
  000ee9d4: cmp r1,#0x0
  000ee9d6: bne.w 0x000eecee
  000ee9da: ldrb.w r0,[r0,#0x69]
  000ee9de: cmp r0,#0x0
  000ee9e0: beq.w 0x000eec7e
  000ee9e4: ldr r0,[r4,#0x54]
  000ee9e6: blx 0x00072034
  000ee9ea: mov.w r5,#0x1f40
  000ee9ee: cmp r0,#0x0
  000ee9f0: beq.w 0x000eec82
  000ee9f4: ldr r0,[r4,#0x54]
  000ee9f6: blx 0x00072034
  000ee9fa: blx 0x00075928
  000ee9fe: cmp r0,#0x0
  000eea00: it ne
  000eea02: movw.ne r5,#0x2ee0
  000eea06: b 0x000eec82
  000eea08: adr.w r0,0xef830
  000eea0c: vmov.i32 q8,#0x0
  000eea10: add r1,sp,#0x198
  000eea12: vld1.64 {d18,d19},[r0]
  000eea16: orr r0,r1,#0x4
  000eea1a: vst1.32 {d16,d17},[r0]
  000eea1e: add.w r0,r1,#0x18
  000eea22: vst1.64 {d16,d17},[r0]
  000eea26: add.w r0,r1,#0x28
  000eea2a: vst1.64 {d18,d19},[r0]
  000eea2e: mov.w r0,#0x3f800000
  000eea32: str r0,[sp,#0x198]
  000eea34: str r0,[sp,#0x1ac]
  000eea36: str r0,[sp,#0x1d0]
  000eea38: add r0,sp,#0x140
  000eea3a: ldrd r2,r3,[r4,#0x188]
  000eea3e: vldr.32 s0,[r4,#0x190]
  000eea42: vstr.32 s0,[sp]
  000eea46: blx 0x00072094
  000eea4a: cmp.w r8,#0x1
  000eea4e: blt 0x000eea6c
  000eea50: ldr r5,[r4,#0x8]
  000eea52: mov r0,r5
  000eea54: blx 0x000721cc
  000eea58: add r6,sp,#0x140
  000eea5a: add r2,sp,#0x198
  000eea5c: mov r1,r0
  000eea5e: mov r0,r6
  000eea60: blx 0x0006f82c
  000eea64: mov r0,r5
  000eea66: mov r1,r6
  000eea68: blx 0x000721e4
  000eea6c: vmov s0,r8
  000eea70: add.w r9,sp,#0xd8
  000eea74: add.w r1,r4,#0x194
  000eea78: vcvt.f32.s32 s0,s0
  000eea7c: mov r0,r9
  000eea7e: ldr r5,[r4,#0x8]
  000eea80: vmov r2,s0
  000eea84: blx 0x0006ec74
  000eea88: add r6,sp,#0x140
  000eea8a: ldr.w r2,[r4,#0x1e8]
  000eea8e: mov r1,r9
  000eea90: mov r0,r6
  000eea92: blx 0x0006ec74
  000eea96: mov r0,r5
  000eea98: mov r1,r6
  000eea9a: blx 0x00072460
  000eea9e: ldr r0,[r4,#0x8]
  000eeaa0: blx 0x000742b4
  000eeaa4: ldr r0,[r4,#0x8]
  000eeaa6: blx 0x000742b4
  000eeaaa: ldr r0,[r4,#0x54]
  000eeaac: ldr.w r5,[r4,#0x124]
  000eeab0: blx 0x00072034
  000eeab4: cbz r0,0x000eeac4
  000eeab6: ldr r0,[r4,#0x54]
  000eeab8: blx 0x00072034
  000eeabc: blx 0x0007588c
  000eeac0: mov r2,r0
  000eeac2: b 0x000eeac6
  000eeac4: movs r2,#0x0
  000eeac6: mov r0,r5
  000eeac8: mov r1,r8
  000eeaca: blx 0x00075934
  000eeace: ldrb.w r1,[r4,#0x4c]
  000eead2: ldr.w r0,[r4,#0xd8]
  000eead6: cmp r1,#0x0
  000eead8: add r0,r8
  000eeada: str.w r0,[r4,#0xd8]
  000eeade: beq.w 0x000eec40
  000eeae2: ldr r0,[r4,#0x4]
  000eeae4: blx 0x00071f14
  000eeae8: cmp r0,#0x0
  000eeaea: beq.w 0x000eec40
  000eeaee: cmp.w r8,#0x1
  000eeaf2: blt.w 0x000eec40
  000eeaf6: ldr r0,[r4,#0x78]
  000eeaf8: cmp r0,#0x0
  000eeafa: beq.w 0x000eec40
  000eeafe: ldr r0,[r4,#0x4]
  000eeb00: blx 0x00075940
  000eeb04: vmov s16,r0
  000eeb08: vcmpe.f32 s16,#0
  000eeb0c: vmrs apsr,fpscr
  000eeb10: ble 0x000eeb64
  000eeb12: add r5,sp,#0x198
  000eeb14: add.w r1,r4,#0x17c
  000eeb18: mov r2,r0
  000eeb1a: mov r0,r5
  000eeb1c: blx 0x0006ec74
  000eeb20: add.w r6,r4,#0x90
  000eeb24: mov r1,r5
  000eeb26: mov r0,r6
  000eeb28: blx 0x0006eb3c
  000eeb2c: ldr r0,[r4,#0x78]
  000eeb2e: mov r1,r6
  000eeb30: blx 0x00072460
  000eeb34: ldr r0,[r4,#0x8]
  000eeb36: mov r1,r6
  000eeb38: blx 0x00072460
  000eeb3c: vldr.32 s0,[pc,#0xec]
  000eeb40: vldr.32 s2,[pc,#0xec]
  000eeb44: vmul.f32 s0,s16,s0
  000eeb48: vldr.32 s4,[pc,#0xe8]
  000eeb4c: vcmpe.f32 s0,s2
  000eeb50: vmrs apsr,fpscr
  000eeb54: it mi
  000eeb56: vmov.mi.f32 s0,s4
  000eeb5a: ldr r0,[r4,#0x4]
  000eeb5c: vmov r1,s0
  000eeb60: blx 0x00072aa8
  000eeb64: lsr.w r0,r8,#0x1
  000eeb68: vldr.32 s2,[pc,#0xcc]
  000eeb6c: vmov s0,r0
  000eeb70: vcvt.f32.s32 s0,s0
  000eeb74: vmul.f32 s0,s0,s2
  000eeb78: vldr.32 s2,[pc,#0xc0]
  000eeb7c: vmul.f32 s0,s0,s2
  000eeb80: vcvt.s32.f32 s0,s0
  000eeb84: vcvt.f32.s32 s0,s0
  000eeb88: ldr r0,[r4,#0x78]
  000eeb8a: vmov r1,s0
  000eeb8e: mov r2,r1
  000eeb90: mov r3,r1
  000eeb92: blx 0x00072160
  000eeb96: ldr r0,[r4,#0x78]
  000eeb98: blx 0x000721cc
  000eeb9c: mov r1,r0
  000eeb9e: ldr r0,[r4,#0x4]
  000eeba0: adds r0,#0x4
  000eeba2: blx 0x0006f148
  000eeba6: ldr.w r0,[r4,#0xd8]
  000eebaa: movw r1,#0xea61
  000eebae: cmp r0,r1
  000eebb0: blt.w 0x000ee1dc
  000eebb4: ldr.w r0,[r4,#0x124]
  000eebb8: blx 0x000757fc
  000eebbc: cmp r0,#0x0
  000eebbe: bne.w 0x000ee1dc
  000eebc2: ldr r0,[r4,#0x54]
  000eebc4: blx 0x00072034
  000eebc8: cbz r0,0x000eebee
  000eebca: ldr r0,[r4,#0x54]
  000eebcc: blx 0x00072034
  000eebd0: ldr r0,[r0,#0x14]
  000eebd2: cbz r0,0x000eebee
  000eebd4: ldr r0,[r4,#0x54]
  000eebd6: blx 0x00072034
  000eebda: ldr r0,[r0,#0x14]
  000eebdc: ldr r0,[r0,#0x1c]
  000eebde: cmp r0,r4
  000eebe0: bne 0x000eebee
  000eebe2: ldr r0,[r4,#0x54]
  000eebe4: blx 0x00072034
  000eebe8: ldr r0,[r0,#0x14]
  000eebea: movs r1,#0x0
  000eebec: str r1,[r0,#0x1c]
  000eebee: ldrb.w r0,[r4,#0xd0]
  000eebf2: cbz r0,0x000eec00
  000eebf4: ldrb.w r0,[r4,#0x69]
  000eebf8: cbnz r0,0x000eec00
  000eebfa: movs r0,#0x1
  000eebfc: strb.w r0,[r4,#0x68]
  000eec00: ldr r0,[r4,#0x78]
  000eec02: cbz r0,0x000eec0c
  000eec04: blx 0x00071fc8
  000eec08: blx 0x0006eb48
  000eec0c: movs r0,#0x0
  000eec0e: str.w r0,[r4,#0xd8]
  000eec12: str r0,[r4,#0x78]
  000eec14: b 0x000eec6c
  000eec40: ldr.w r0,[r4,#0x124]
  000eec44: blx 0x000757fc
  000eec48: cmp r0,#0x0
  000eec4a: bne.w 0x000ee1dc
  000eec4e: ldrb.w r0,[r4,#0xd0]
  000eec52: cbz r0,0x000eec60
  000eec54: ldrb.w r0,[r4,#0x69]
  000eec58: cbnz r0,0x000eec60
  000eec5a: movs r0,#0x1
  000eec5c: strb.w r0,[r4,#0x68]
  000eec60: ldr.w r0,[r4,#0xd8]
  000eec64: movw r1,#0xea61
  000eec68: cmp r0,r1
  000eec6a: blt 0x000eec74
  000eec6c: mov r0,r4
  000eec6e: movs r1,#0x0
  000eec70: blx 0x000732f4
  000eec74: movs r0,#0x1
  000eec76: strb.w r0,[r4,#0x101]
  000eec7a: b.w 0x000ee1dc
  000eec7e: mov.w r5,#0x1f40
  000eec82: vmov s0,r5
  000eec86: vcvt.f32.s32 s0,s0
  000eec8a: vldr.32 s4,[r6]
  000eec8e: vcmpe.f32 s4,s0
  000eec92: vmrs apsr,fpscr
  000eec96: bpl 0x000eecee
  000eec98: rsbs r0,r5
  000eec9a: vmov s2,r0
  000eec9e: vcvt.f32.s32 s2,s2
  000eeca2: vcmpe.f32 s4,s2
  000eeca6: vmrs apsr,fpscr
  000eecaa: ble 0x000eecee
  000eecac: vldr.32 s4,[r4,#0x174]
  000eecb0: vcmpe.f32 s4,s0
  000eecb4: vmrs apsr,fpscr
  000eecb8: bpl 0x000eecee
  000eecba: vcmpe.f32 s4,s2
  000eecbe: vmrs apsr,fpscr
  000eecc2: ble 0x000eecee
  000eecc4: vldr.32 s4,[r4,#0x178]
  000eecc8: vcmpe.f32 s4,s0
  000eeccc: vmrs apsr,fpscr
  000eecd0: bpl 0x000eecee
  000eecd2: vcmpe.f32 s4,s2
  000eecd6: vmrs apsr,fpscr
  000eecda: ble 0x000eecee
  000eecdc: add r5,sp,#0x198
  000eecde: ldr r1,[r4,#0x8]
  000eece0: mov r0,r5
  000eece2: blx 0x0007261c
  000eece6: mov r0,r6
  000eece8: mov r1,r5
  000eecea: blx 0x0006eb3c
  000eecee: add r5,sp,#0x188
  000eecf0: mov r1,r6
  000eecf2: mov r0,r5
  000eecf4: blx 0x0006ec80
  000eecf8: ldr r0,[r4,#0x4]
  000eecfa: str r6,[sp,#0xd0]
  000eecfc: add r6,sp,#0x198
  000eecfe: adds r1,r0,#0x4
  000eed00: mov r0,r6
  000eed02: blx 0x0006fb50
  000eed06: add.w r11,sp,#0x140
  000eed0a: mov r1,r6
  000eed0c: mov r2,r5
  000eed0e: mov r0,r11
  000eed10: blx 0x0006f694
  000eed14: ldr r0,[sp,#0xd0]
  000eed16: mov r1,r11
  000eed18: blx 0x0006eb3c
  000eed1c: vldr.32 s0,[r4,#0x174]
  000eed20: ldr r0,[r4,#0x4]
  000eed22: vneg.f32 s0,s0
  000eed26: ldrb.w r1,[r4,#0x12e]
  000eed2a: cmp r1,#0x0
  000eed2c: vstr.32 s0,[r4,#0x174]
  000eed30: ldrb.w r2,[r0,#0x68]
  000eed34: strb.w r2,[r4,#0x24]
  000eed38: beq 0x000eed44
  000eed3a: ldrb.w r1,[r4,#0x12c]
  000eed3e: cmp r1,#0x0
  000eed40: beq.w 0x000eef12
  000eed44: ldr r0,[r4,#0x8]
  000eed46: blx 0x000721cc
  000eed4a: mov r1,r0
  000eed4c: add r0,sp,#0x198
  000eed4e: movs r2,#0x3c
  000eed50: blx 0x0006f1e4
  000eed54: ldrb.w r0,[r4,#0x12d]
  000eed58: str.w r10,[sp,#0xc4]
  000eed5c: cbz r0,0x000eed6e
  000eed5e: movs r0,#0x0
  000eed60: str.w r0,[r4,#0x2b0]
  000eed64: str.w r0,[r4,#0x20c]
  000eed68: strb.w r0,[r4,#0x2b4]
  000eed6c: b 0x000ef042
  000eed6e: add.w r9,sp,#0x17c
  000eed72: vldr.64 d16,[sp,#0x188]
  000eed76: ldr r0,[sp,#0x190]
  000eed78: add r1,sp,#0x198
  000eed7a: str r0,[sp,#0xe0]
  000eed7c: mov r0,r9
  000eed7e: vstr.64 d16,[sp,#0xd8]
  000eed82: blx 0x0006f4c0
  000eed86: add r6,sp,#0x188
  000eed88: mov r1,r9
  000eed8a: mov r0,r6
  000eed8c: blx 0x00072694
  000eed90: add.w r11,sp,#0x140
  000eed94: mov r1,r6
  000eed96: mov r0,r11
  000eed98: blx 0x0006ec80
  000eed9c: ldr r0,[0x000ef0f0]
  000eed9e: mov r1,r11
  000eeda0: add r0,pc
  000eeda2: ldr r5,[r0,#0x0]
  000eeda4: mov r0,r6
  000eeda6: blx r5
  000eeda8: add.w r0,r8,r8, lsl #0x1
  000eedac: vldr.32 s2,[pc,#-0x178]
  000eedb0: mov r11,r8
  000eedb2: lsls r0,r0,#0x4
  000eedb4: vmov s0,r0
  000eedb8: mov r0,r6
  000eedba: vcvt.f32.s32 s0,s0
  000eedbe: vmul.f32 s0,s0,s2
  000eedc2: vmov r1,s0
  000eedc6: blx 0x00072628
  000eedca: add.w r10,sp,#0x140
  000eedce: mov r1,r6
  000eedd0: mov r2,r9
  000eedd2: mov r0,r10
  000eedd4: blx 0x0006f1cc
  000eedd8: ldr r0,[sp,#0xd0]
  000eedda: mov r1,r10
  000eeddc: blx r5
  000eedde: add r6,sp,#0x140
  000eede0: ldr r1,[sp,#0xd0]
  000eede2: mov r0,r6
  000eede4: blx 0x0006ec80
  000eede8: ldr r0,[sp,#0xd0]
  000eedea: mov r1,r6
  000eedec: ldr r6,[sp,#0xd0]
  000eedee: blx r5
  000eedf0: vldr.32 s0,[r4,#0x178]
  000eedf4: vldr.32 s2,[sp,#0xe0]
  000eedf8: vldr.64 d16,[r6]
  000eedfc: vsub.f32 s0,s0,s2
  000eee00: vldr.64 d17,[sp,#0xd8]
  000eee04: vsub.f32 d16,d16,d17
  000eee08: vcgt.f32 d2,d16,#0
  000eee0c: vneg.f32 d17,d16
  000eee10: vbsl d2,d16,d17
  000eee14: vcmpe.f32 s0,#0
  000eee18: vmrs apsr,fpscr
  000eee1c: vneg.f32 s2,s0
  000eee20: vadd.f32 s4,s4,s5
  000eee24: it gt
  000eee26: vmov.gt.f32 s2,s0
  000eee2a: vadd.f32 s0,s4,s2
  000eee2e: vldr.32 s2,[pc,#0x2c4]
  000eee32: vcmpe.f32 s0,s2
  000eee36: vmrs apsr,fpscr
  000eee3a: bpl 0x000eee44
  000eee3c: add r1,sp,#0xd8
  000eee3e: mov r0,r6
  000eee40: blx 0x0006eb3c
  000eee44: add r5,sp,#0x140
  000eee46: add r1,sp,#0x17c
  000eee48: mov r0,r5
  000eee4a: blx 0x0006ec80
  000eee4e: mov r0,r5
  000eee50: mov r1,r6
  000eee52: blx 0x0006ec68
  000eee56: vmov.f32 s4,0x3f800000
  000eee5a: vldr.32 s0,[pc,#-0x228]
  000eee5e: vmov s2,r0
  000eee62: vcmpe.f32 s2,s4
  000eee66: vmrs apsr,fpscr
  000eee6a: bpl 0x000eee82
  000eee6c: vmov.f32 s4,0xbf800000
  000eee70: vcmpe.f32 s2,s4
  000eee74: vmrs apsr,fpscr
  000eee78: ble 0x000eee82
  000eee7a: blx 0x000726a0
  000eee7e: vmov s0,r0
  000eee82: vcmpe.f32 s0,#0
  000eee86: mov r8,r11
  000eee88: vmrs apsr,fpscr
  000eee8c: vneg.f32 s16,s0
  000eee90: it gt
  000eee92: vmov.gt.f32 s16,s0
  000eee96: vcmpe.f32 s16,#0
  000eee9a: vmrs apsr,fpscr
  000eee9e: beq 0x000eeedc
  000eeea0: add r5,sp,#0x140
  000eeea2: add r1,sp,#0x198
  000eeea4: mov r0,r5
  000eeea6: blx 0x0006f4e4
  000eeeaa: vldr.64 d16,[r6]
  000eeeae: add r1,sp,#0x130
  000eeeb0: ldr r0,[r6,#0x8]
  000eeeb2: str r0,[sp,#0x138]
  000eeeb4: mov r0,r5
  000eeeb6: vstr.64 d16,[sp,#0x130]
  000eeeba: blx 0x0006ec68
  000eeebe: blx 0x000726a0
  000eeec2: vldr.32 s0,[pc,#0x234]
  000eeec6: vmov s2,r0
  000eeeca: vneg.f32 s4,s16
  000eeece: vcmpe.f32 s2,s0
  000eeed2: vmrs apsr,fpscr
  000eeed6: it mi
  000eeed8: vmov.mi.f32 s16,s4
  000eeedc: ldr.w r1,[r4,#0x2b0]
  000eeee0: add.w r0,r4,r1, lsl #0x2
  000eeee4: vstr.32 s16,[r0,#0x29c]
  000eeee8: ldrb.w r0,[r4,#0x2b4]
  000eeeec: cbz r0,0x000eef54
  000eeeee: vldr.32 s0,[pc,#-0x2bc]
  000eeef2: add.w r2,r4,#0x29c
  000eeef6: movs r3,#0x0
  000eeef8: b 0x000eef06
  000eeefa: adds r6,r2,r3
  000eeefc: adds r3,#0x4
  000eeefe: vldr.32 s2,[r6]
  000eef02: vadd.f32 s0,s0,s2
  000eef06: cmp r3,#0x14
  000eef08: bne 0x000eeefa
  000eef0a: vmov.f32 s2,0x40a00000
  000eef0e: ldr r6,[sp,#0xd0]
  000eef10: b 0x000eef7a
  000eef12: ldr.w r1,[r4,#0x148]
  000eef16: cmp r1,#0x0
  000eef18: beq.w 0x000eed44
  000eef1c: blx 0x0007585c
  000eef20: cmp r0,#0x0
  000eef22: bne.w 0x000eed44
  000eef26: ldr.w r0,[r4,#0x148]
  000eef2a: ldrb.w r1,[r0,#0x5e]
  000eef2e: cmp r1,#0x0
  000eef30: beq.w 0x000ef480
  000eef34: ldrb.w r0,[r4,#0x2b5]
  000eef38: movs r5,#0x0
  000eef3a: strb.w r5,[r4,#0x12e]
  000eef3e: cmp r0,#0x0
  000eef40: beq.w 0x000eed44
  000eef44: ldr r0,[r4,#0x4]
  000eef46: ldr.w r1,[r4,#0x140]
  000eef4a: blx 0x00072484
  000eef4e: strb.w r5,[r4,#0x2b5]
  000eef52: b 0x000eed44
  000eef54: cbz r1,0x000eef9c
  000eef56: vldr.32 s0,[pc,#-0x324]
  000eef5a: add.w r2,r4,#0x29c
  000eef5e: movs r3,#0x0
  000eef60: b 0x000eef6e
  000eef62: vldr.32 s2,[r2]
  000eef66: adds r2,#0x4
  000eef68: adds r3,#0x1
  000eef6a: vadd.f32 s0,s0,s2
  000eef6e: cmp r3,r1
  000eef70: blt 0x000eef62
  000eef72: vmov s2,r1
  000eef76: vcvt.f32.s32 s2,s2
  000eef7a: vdiv.f32 s16,s0,s2
  000eef7e: cmp r1,#0x4
  000eef80: add.w r2,r1,#0x1
  000eef84: str.w r2,[r4,#0x2b0]
  000eef88: blt 0x000eefa2
  000eef8a: movs r1,#0x0
  000eef8c: cmp r0,#0x0
  000eef8e: str.w r1,[r4,#0x2b0]
  000eef92: bne 0x000eefa2
  000eef94: movs r0,#0x1
  000eef96: strb.w r0,[r4,#0x2b4]
  000eef9a: b 0x000eefa2
  000eef9c: movs r0,#0x1
  000eef9e: str.w r0,[r4,#0x2b0]
  000eefa2: vldr.32 s0,[r4,#0x294]
  000eefa6: vldr.32 s2,[r4,#0x298]
  000eefaa: vmul.f32 s4,s16,s0
  000eefae: vmul.f32 s2,s4,s2
  000eefb2: vcmpe.f32 s2,s0
  000eefb6: vstr.32 s2,[r4,#0x20c]
  000eefba: vmrs apsr,fpscr
  000eefbe: ble 0x000eefc6
  000eefc0: vstr.32 s0,[r4,#0x20c]
  000eefc4: b 0x000eefd8
  000eefc6: vneg.f32 s0,s0
  000eefca: vcmpe.f32 s2,s0
  000eefce: vmrs apsr,fpscr
  000eefd2: it mi
  000eefd4: vstr.mi.32 s0,[r4,#0x20c]
  000eefd8: add r5,sp,#0x140
  000eefda: add r1,sp,#0x198
  000eefdc: mov r0,r5
  000eefde: blx 0x0006f4d8
  000eefe2: add.w r11,sp,#0x130
  000eefe6: mov r1,r5
  000eefe8: mov r2,r6
  000eefea: mov r0,r11
  000eefec: blx 0x0006ec8c
  000eeff0: add r5,sp,#0x140
  000eeff2: mov r1,r11
  000eeff4: mov r0,r5
  000eeff6: blx 0x0006ec80
  000eeffa: mov r0,r11
  000eeffc: mov r1,r5
  000eeffe: blx 0x0006eb3c
  000ef002: add r5,sp,#0x124
  000ef004: mov r1,r6
  000ef006: mov r2,r11
  000ef008: mov r0,r5
  000ef00a: blx 0x0006ec8c
  000ef00e: add.w r9,sp,#0x140
  000ef012: mov r1,r5
  000ef014: mov r0,r9
  000ef016: blx 0x0006ec80
  000ef01a: mov r0,r5
  000ef01c: mov r1,r9
  000ef01e: blx 0x0006eb3c
  000ef022: str r6,[sp,#0x0]
  000ef024: add.w r9,sp,#0x140
  000ef028: add r6,sp,#0x198
  000ef02a: mov r2,r11
  000ef02c: mov r0,r9
  000ef02e: mov r3,r5
  000ef030: mov r1,r6
  000ef032: blx 0x000701bc
  000ef036: mov r0,r6
  000ef038: mov r1,r9
  000ef03a: blx 0x0006f148
  000ef03e: add.w r11,sp,#0x140
  000ef042: ldrb.w r0,[r4,#0x100]
  000ef046: cbz r0,0x000ef050
  000ef048: mov r0,r4
  000ef04a: mov r1,r8
  000ef04c: blx 0x0007594c
  000ef050: vldr.32 s2,[r4,#0x20c]
  000ef054: vldr.32 s0,[r4,#0x210]
  000ef058: vcmpe.f32 s0,s2
  000ef05c: vmrs apsr,fpscr
  000ef060: beq 0x000ef0b4
  000ef062: vmov s6,r8
  000ef066: vmov.f32 s4,0x3fa00000
  000ef06a: vcvt.f32.s32 s6,s6
  000ef06e: vcmpe.f32 s0,s2
  000ef072: vmrs apsr,fpscr
  000ef076: vmul.f32 s4,s6,s4
  000ef07a: vldr.32 s6,[pc,#0x80]
  000ef07e: vdiv.f32 s4,s4,s6
  000ef082: bpl 0x000ef098
  000ef084: vadd.f32 s0,s4,s0
  000ef088: vcmpe.f32 s0,s2
  000ef08c: vstr.32 s0,[r4,#0x210]
  000ef090: vmrs apsr,fpscr
  000ef094: bgt 0x000ef0ac
  000ef096: b 0x000ef0b4
  000ef098: ble 0x000ef0b4
  000ef09a: vsub.f32 s0,s0,s4
  000ef09e: vcmpe.f32 s0,s2
  000ef0a2: vstr.32 s0,[r4,#0x210]
  000ef0a6: vmrs apsr,fpscr
  000ef0aa: bpl 0x000ef0b4
  000ef0ac: vmov.f32 s0,s2
  000ef0b0: vstr.32 s2,[r4,#0x210]
  000ef0b4: ldr.w r0,[r4,#0xfc]
  000ef0b8: cmp r0,#0x1
  000ef0ba: blt 0x000ef0ce
  000ef0bc: sub.w r0,r0,r8
  000ef0c0: str.w r0,[r4,#0xfc]
  000ef0c4: cmp r0,#0x0
  000ef0c6: bgt 0x000ef0ce
  000ef0c8: movs r1,#0x1
  000ef0ca: strb.w r1,[r4,#0x100]
  000ef0ce: vcmpe.f32 s2,s0
  000ef0d2: vmrs apsr,fpscr
  000ef0d6: bne 0x000ef108
  000ef0d8: cmp r0,#0x1
  000ef0da: blt 0x000ef110
  000ef0dc: sub.w r0,r0,r8
  000ef0e0: str.w r0,[r4,#0xfc]
  000ef0e4: cmp r0,#0x0
  000ef0e6: bgt 0x000ef110
  000ef0e8: movs r0,#0x1
  000ef0ea: strb.w r0,[r4,#0x100]
  000ef0ee: b 0x000ef110
  000ef108: movw r0,#0x2ee
  000ef10c: str.w r0,[r4,#0xfc]
  000ef110: vldr.32 s4,[pc,#-0x14]
  000ef114: adr.w r0,0xef830
  000ef118: vldr.32 s2,[pc,#-0x18]
  000ef11c: vmov.i32 q8,#0x0
  000ef120: vmul.f32 s0,s0,s4
  000ef124: vld1.64 {d18,d19},[r0]
  000ef128: orr r0,r11,#0x4
  000ef12c: add r5,sp,#0xd8
  000ef12e: mov r1,r11
  000ef130: movs r2,#0x0
  000ef132: vst1.32 {d16,d17},[r0]
  000ef136: add.w r0,r11,#0x18
  000ef13a: movs r3,#0x0
  000ef13c: vst1.64 {d16,d17},[r0]
  000ef140: add.w r0,r11,#0x28
  000ef144: vmul.f32 s0,s0,s2
  000ef148: vst1.64 {d18,d19},[r0]
  000ef14c: mov.w r0,#0x3f800000
  000ef150: str r0,[sp,#0x140]
  000ef152: str r0,[sp,#0x154]
  000ef154: str r0,[sp,#0x178]
  000ef156: mov r0,r5
  000ef158: vstr.32 s0,[sp]
  000ef15c: blx 0x00072094
  000ef160: mov r0,r11
  000ef162: mov r1,r5
  000ef164: blx 0x0006f148
  000ef168: ldr r0,[r4,#0xc]
  000ef16a: cbz r0,0x000ef172
  000ef16c: add r1,sp,#0x140
  000ef16e: blx 0x000721e4
  000ef172: ldr r0,[r4,#0x4]
  000ef174: ldrb.w r0,[r0,#0x68]
  000ef178: cbz r0,0x000ef1a2
  000ef17a: ldr r2,[0x000ef468]
  000ef17c: ldr r0,[r4,#0x54]
  000ef17e: add r2,pc
  000ef180: ldr.w r1,[r4,#0x130]
  000ef184: ldr r5,[r2,#0x0]
  000ef186: movs r2,#0x1
  000ef188: ldr.w r0,[r0,#0x8c]
  000ef18c: blx r5
  000ef18e: ldr r0,[r4,#0x54]
  000ef190: movs r2,#0x1
  000ef192: ldr.w r1,[r4,#0x134]
  000ef196: ldr.w r0,[r0,#0x8c]
  000ef19a: blx r5
  000ef19c: ldr r0,[r4,#0x54]
  000ef19e: movs r2,#0x1
  000ef1a0: b 0x000ef250
  000ef1a2: vmov s0,r8
  000ef1a6: vcvt.f32.s32 s0,s0
  000ef1aa: vldr.32 s2,[r4,#0x1e8]
  000ef1ae: vmul.f32 s0,s2,s0
  000ef1b2: vcvt.s32.f32 s0,s0
  000ef1b6: vcvt.f32.s32 s0,s0
  000ef1ba: vldr.32 s2,[sp,#0x1a0]
  000ef1be: vldr.32 s4,[sp,#0x1a4]
  000ef1c2: vldr.32 s6,[sp,#0x1b0]
  000ef1c6: vldr.32 s10,[sp,#0x1c0]
  000ef1ca: vldr.32 s8,[sp,#0x1b4]
  000ef1ce: vmla.f32 s4,s2,s0
  000ef1d2: vldr.32 s2,[sp,#0x1c4]
  000ef1d6: vmla.f32 s8,s6,s0
  000ef1da: vmla.f32 s2,s10,s0
  000ef1de: vstr.32 s4,[sp,#0x1a4]
  000ef1e2: vstr.32 s8,[sp,#0x1b4]
  000ef1e6: vstr.32 s2,[sp,#0x1c4]
  000ef1ea: ldrb.w r0,[r4,#0x100]
  000ef1ee: cbz r0,0x000ef1fa
  000ef1f0: add.w r1,r4,#0x258
  000ef1f4: add r0,sp,#0x198
  000ef1f6: blx 0x0006f4a8
  000ef1fa: ldr r0,[0x000ef46c]
  000ef1fc: add r5,sp,#0x198
  000ef1fe: ldr r1,[r4,#0x8]
  000ef200: add r0,pc
  000ef202: mov r2,r5
  000ef204: ldr r0,[r0,#0x0]
  000ef206: ldr r1,[r1,#0xc]
  000ef208: ldr r0,[r0,#0x0]
  000ef20a: blx 0x000721c0
  000ef20e: ldr r0,[r4,#0x4]
  000ef210: mov r1,r5
  000ef212: adds r0,#0x4
  000ef214: blx 0x0006f148
  000ef218: ldr r0,[r4,#0xc]
  000ef21a: cbz r0,0x000ef22a
  000ef21c: blx 0x000721cc
  000ef220: mov r1,r0
  000ef222: ldr r0,[r4,#0x4]
  000ef224: adds r0,#0x4
  000ef226: blx 0x0006f4a8
  000ef22a: ldr r2,[0x000ef470]
  000ef22c: ldr r0,[r4,#0x54]
  000ef22e: add r2,pc
  000ef230: ldr.w r1,[r4,#0x130]
  000ef234: ldr r5,[r2,#0x0]
  000ef236: movs r2,#0x0
  000ef238: ldr.w r0,[r0,#0x8c]
  000ef23c: blx r5
  000ef23e: ldr r0,[r4,#0x54]
  000ef240: movs r2,#0x0
  000ef242: ldr.w r1,[r4,#0x134]
  000ef246: ldr.w r0,[r0,#0x8c]
  000ef24a: blx r5
  000ef24c: ldr r0,[r4,#0x54]
  000ef24e: movs r2,#0x0
  000ef250: ldr.w r1,[r4,#0x138]
  000ef254: ldr.w r0,[r0,#0x8c]
  000ef258: blx r5
  000ef25a: ldr r0,[r4,#0x8]
  000ef25c: blx 0x000742b4
  000ef260: ldrb.w r0,[r4,#0x13e]
  000ef264: cmp r0,#0x0
  000ef266: beq.w 0x000ef446
  000ef26a: ldr r0,[r4,#0x54]
  000ef26c: blx 0x000725b0
  000ef270: mov r6,r0
  000ef272: cmp r0,#0x0
  000ef274: beq 0x000ef358
  000ef276: add.w r10,sp,#0xd8
  000ef27a: add.w r9,sp,#0x17c
  000ef27e: mov.w r11,#0x0
  000ef282: movs r5,#0x0
  000ef284: b 0x000ef2bc
  000ef286: ldr r0,[r6,#0x4]
  000ef288: ldr.w r0,[r0,r5,lsl #0x2]
  000ef28c: cbz r0,0x000ef2ba
  000ef28e: ldr r1,[r0,#0x0]
  000ef290: ldr r2,[r1,#0x40]
  000ef292: ldr r1,[sp,#0xd4]
  000ef294: blx r2
  000ef296: cbz r0,0x000ef2ba
  000ef298: ldr r0,[r6,#0x4]
  000ef29a: ldr r2,[sp,#0xd4]
  000ef29c: ldr.w r1,[r0,r5,lsl #0x2]
  000ef2a0: ldr r0,[r1,#0x0]
  000ef2a2: ldr r3,[r0,#0x50]
  000ef2a4: mov r0,r10
  000ef2a6: blx r3
  000ef2a8: mov r0,r10
  000ef2aa: mov r1,r9
  000ef2ac: strd r11,r11,[sp,#0x17c]
  000ef2b0: str.w r11,[sp,#0x184]
  000ef2b4: blx 0x00072a90
  000ef2b8: cbnz r0,0x000ef2c4
  000ef2ba: adds r5,#0x1
  000ef2bc: ldr r0,[r6,#0x0]
  000ef2be: cmp r5,r0
  000ef2c0: bcc 0x000ef286
  000ef2c2: b 0x000ef358
  000ef2c4: add r5,sp,#0x17c
  000ef2c6: ldr r1,[r4,#0x8]
  000ef2c8: mov r0,r5
  000ef2ca: blx 0x00072574
  000ef2ce: ldr r0,[0x000ef474]
  000ef2d0: add.w r6,r4,#0x90
  000ef2d4: mov r1,r5
  000ef2d6: add r0,pc
  000ef2d8: ldr.w r10,[r0,#0x0]
  000ef2dc: mov r0,r6
  000ef2de: blx r10
  000ef2e0: add r5,sp,#0xd8
  000ef2e2: mov r1,r6
  000ef2e4: mov r0,r5
  000ef2e6: blx 0x00072694
  000ef2ea: vldr.32 s0,[pc,#0x18c]
  000ef2ee: mov r0,r5
  000ef2f0: vldr.32 s2,[r4,#0x1e8]
  000ef2f4: vmul.f32 s0,s2,s0
  000ef2f8: vmov r1,s0
  000ef2fc: blx 0x00072628
  000ef300: add.w r9,sp,#0x17c
  000ef304: mov r1,r6
  000ef306: mov r2,r5
  000ef308: mov r0,r9
  000ef30a: blx 0x0006f1cc
  000ef30e: add.w r5,r4,#0x9c
  000ef312: mov r1,r9
  000ef314: mov r0,r5
  000ef316: blx r10
  000ef318: add r6,sp,#0x17c
  000ef31a: mov r1,r5
  000ef31c: mov r0,r6
  000ef31e: blx 0x0006ec80
  000ef322: mov r0,r5
  000ef324: mov r1,r6
  000ef326: blx r10
  000ef328: ldr r0,[r4,#0x8]
  000ef32a: mov.w r1,#0x3f800000
  000ef32e: str r1,[sp,#0x180]
  000ef330: movs r1,#0x0
  000ef332: add r2,sp,#0x17c
  000ef334: str r1,[sp,#0x17c]
  000ef336: str r1,[sp,#0x184]
  000ef338: mov r1,r5
  000ef33a: blx 0x000726ac
  000ef33e: vmov s0,r8
  000ef342: vcvt.f32.s32 s0,s0
  000ef346: vldr.32 s2,[r4,#0x1e8]
  000ef34a: ldr r0,[r4,#0x8]
  000ef34c: vmul.f32 s0,s2,s0
  000ef350: vmov r1,s0
  000ef354: blx 0x000725c8
  000ef358: ldr r0,[r4,#0x54]
  000ef35a: blx 0x00071eb4
  000ef35e: mov r6,r0
  000ef360: cmp r0,#0x0
  000ef362: beq 0x000ef446
  000ef364: add.w r10,sp,#0xd8
  000ef368: add.w r9,sp,#0x17c
  000ef36c: mov.w r11,#0x0
  000ef370: movs r5,#0x0
  000ef372: b 0x000ef3aa
  000ef374: ldr r0,[r6,#0x4]
  000ef376: ldr.w r0,[r0,r5,lsl #0x2]
  000ef37a: cbz r0,0x000ef3a8
  000ef37c: ldr r1,[r0,#0x0]
  000ef37e: ldr r2,[r1,#0x40]
  000ef380: ldr r1,[sp,#0xd4]
  000ef382: blx r2
  000ef384: cbz r0,0x000ef3a8
  000ef386: ldr r0,[r6,#0x4]
  000ef388: ldr r2,[sp,#0xd4]
  000ef38a: ldr.w r1,[r0,r5,lsl #0x2]
  000ef38e: ldr r0,[r1,#0x0]
  000ef390: ldr r3,[r0,#0x50]
  000ef392: mov r0,r10
  000ef394: blx r3
  000ef396: mov r0,r10
  000ef398: mov r1,r9
  000ef39a: strd r11,r11,[sp,#0x17c]
  000ef39e: str.w r11,[sp,#0x184]
  000ef3a2: blx 0x00072a90
  000ef3a6: cbnz r0,0x000ef3b2
  000ef3a8: adds r5,#0x1
  000ef3aa: ldr r0,[r6,#0x0]
  000ef3ac: cmp r5,r0
  000ef3ae: bcc 0x000ef374
  000ef3b0: b 0x000ef446
  000ef3b2: add r5,sp,#0x17c
  000ef3b4: ldr r1,[r4,#0x8]
  000ef3b6: mov r0,r5
  000ef3b8: blx 0x00072574
  000ef3bc: ldr r0,[0x000ef47c]
  000ef3be: add.w r6,r4,#0x90
  000ef3c2: mov r1,r5
  000ef3c4: add r0,pc
  000ef3c6: ldr.w r10,[r0,#0x0]
  000ef3ca: mov r0,r6
  000ef3cc: blx r10
  000ef3ce: add r5,sp,#0xd8
  000ef3d0: mov r1,r6
  000ef3d2: mov r0,r5
  000ef3d4: blx 0x00072694
  000ef3d8: vldr.32 s0,[pc,#0x9c]
  000ef3dc: mov r0,r5
  000ef3de: vldr.32 s2,[r4,#0x1e8]
  000ef3e2: vmul.f32 s0,s2,s0
  000ef3e6: vmov r1,s0
  000ef3ea: blx 0x00072628
  000ef3ee: add.w r9,sp,#0x17c
  000ef3f2: mov r1,r6
  000ef3f4: mov r2,r5
  000ef3f6: mov r0,r9
  000ef3f8: blx 0x0006f1cc
  000ef3fc: add.w r5,r4,#0x9c
  000ef400: mov r1,r9
  000ef402: mov r0,r5
  000ef404: blx r10
  000ef406: add r6,sp,#0x17c
  000ef408: mov r1,r5
  000ef40a: mov r0,r6
  000ef40c: blx 0x0006ec80
  000ef410: mov r0,r5
  000ef412: mov r1,r6
  000ef414: blx r10
  000ef416: ldr r0,[r4,#0x8]
  000ef418: mov.w r1,#0x3f800000
  000ef41c: str r1,[sp,#0x180]
  000ef41e: movs r1,#0x0
  000ef420: add r2,sp,#0x17c
  000ef422: str r1,[sp,#0x17c]
  000ef424: str r1,[sp,#0x184]
  000ef426: mov r1,r5
  000ef428: blx 0x000726ac
  000ef42c: vmov s0,r8
  000ef430: vcvt.f32.s32 s0,s0
  000ef434: vldr.32 s2,[r4,#0x1e8]
  000ef438: ldr r0,[r4,#0x8]
  000ef43a: vmul.f32 s0,s2,s0
  000ef43e: vmov r1,s0
  000ef442: blx 0x000725c8
  000ef446: ldr r0,[r4,#0x0]
  000ef448: mov r1,r8
  000ef44a: ldr r2,[r0,#0x30]
  000ef44c: mov r0,r4
  000ef44e: blx r2
  000ef450: ldr.w r10,[sp,#0xc4]
  000ef454: b.w 0x000ee1dc
  000ef458: movs r0,#0x0
  000ef45a: strb.w r0,[r4,#0x4c]
  000ef45e: movs r0,#0x1
  000ef460: strb.w r0,[r4,#0x68]
  000ef464: b.w 0x000ee1dc
  000ef480: vldr.32 s4,[r4,#0x170]
  000ef484: vldr.32 s0,[r4,#0x1a8]
  000ef488: vcmpe.f32 s4,s0
  000ef48c: vmrs apsr,fpscr
  000ef490: bpl.w 0x000eed44
  000ef494: vneg.f32 s2,s0
  000ef498: vcmpe.f32 s4,s2
  000ef49c: vmrs apsr,fpscr
  000ef4a0: ble.w 0x000eed44
  000ef4a4: vldr.32 s4,[r4,#0x174]
  000ef4a8: vcmpe.f32 s4,s0
  000ef4ac: vmrs apsr,fpscr
  000ef4b0: bpl.w 0x000eed44
  000ef4b4: vcmpe.f32 s4,s2
  000ef4b8: vmrs apsr,fpscr
  000ef4bc: ble.w 0x000eed44
  000ef4c0: vldr.32 s0,[r4,#0x2c]
  000ef4c4: vldr.32 s2,[r4,#0x164]
  000ef4c8: vsub.f32 s4,s2,s0
  000ef4cc: vldr.32 s0,[pc,#0x378]
  000ef4d0: vcmpe.f32 s4,s0
  000ef4d4: vmrs apsr,fpscr
  000ef4d8: bpl.w 0x000eed44
  000ef4dc: vldr.32 s2,[pc,#0x36c]
  000ef4e0: vcmpe.f32 s4,s2
  000ef4e4: vmrs apsr,fpscr
  000ef4e8: ble.w 0x000eed44
  000ef4ec: vldr.32 s4,[r4,#0x30]
  000ef4f0: vldr.32 s6,[r4,#0x168]
  000ef4f4: vsub.f32 s4,s6,s4
  000ef4f8: vcmpe.f32 s4,s0
  000ef4fc: vmrs apsr,fpscr
  000ef500: bpl.w 0x000eed44
  000ef504: vcmpe.f32 s4,s2
  000ef508: vmrs apsr,fpscr
  000ef50c: ble.w 0x000eed44
  000ef510: vldr.32 s4,[r4,#0x34]
  000ef514: vldr.32 s6,[r4,#0x16c]
  000ef518: vsub.f32 s4,s6,s4
  000ef51c: vcmpe.f32 s4,s0
  000ef520: vmrs apsr,fpscr
  000ef524: bpl.w 0x000eed44
  000ef528: vcmpe.f32 s4,s2
  000ef52c: vmrs apsr,fpscr
  000ef530: ble.w 0x000eed44
  000ef534: ldrb.w r1,[r4,#0x25]
  000ef538: cbz r1,0x000ef550
  000ef53a: mov r5,r9
  000ef53c: blx 0x00071f14
  000ef540: cbz r0,0x000ef550
  000ef542: ldr.w r0,[r4,#0x148]
  000ef546: blx 0x00075838
  000ef54a: cmp r0,#0x0
  000ef54c: beq.w 0x000ef7bc
  000ef550: movs r0,#0x0
  000ef552: strb.w r0,[r4,#0x12e]
  000ef556: b.w 0x000eed44
  000ef55a: ldr.w r0,[r4,#0x2b8]
  000ef55e: movs r1,#0x8
  000ef560: str.w r10,[sp,#0xc4]
  000ef564: cmp r0,#0x0
  000ef566: str.w r1,[r4,#0x88]
  000ef56a: beq 0x000ef574
  000ef56c: blx 0x00072340
  000ef570: blx 0x0006eb48
  000ef574: ldr r1,[0x000ef850]
  000ef576: movs r0,#0x0
  000ef578: str.w r0,[r4,#0x2b8]
  000ef57c: add r1,pc
  000ef57e: ldr r0,[r4,#0x6c]
  000ef580: ldr.w r9,[r1,#0x0]
  000ef584: blx r9
  000ef586: str r0,[sp,#0xbc]
  000ef588: ldr r0,[r4,#0x6c]
  000ef58a: blx r9
  000ef58c: mov r1,r0
  000ef58e: ldr r0,[r0,#0x0]
  000ef590: ldr r2,[r0,#0x28]
  000ef592: add r0,sp,#0x140
  000ef594: blx r2
  000ef596: ldr r0,[r4,#0x6c]
  000ef598: blx r9
  000ef59a: ldr r1,[0x000ef854]
  000ef59c: ldr r0,[r0,#0x8]
  000ef59e: add r1,pc
  000ef5a0: ldr r5,[r1,#0x0]
  000ef5a2: blx r5
  000ef5a4: mov r1,r0
  000ef5a6: ldr r0,[0x000ef858]
  000ef5a8: ldr.w r2,[r4,#0x2c4]
  000ef5ac: add r0,pc
  000ef5ae: ldr.w r10,[r0,#0x0]
  000ef5b2: add r0,sp,#0xd8
  000ef5b4: blx r10
  000ef5b6: add r6,sp,#0x198
  000ef5b8: add r1,sp,#0x140
  000ef5ba: add r2,sp,#0xd8
  000ef5bc: mov r0,r6
  000ef5be: blx 0x0006f1cc
  000ef5c2: ldr r0,[sp,#0xbc]
  000ef5c4: mov r1,r6
  000ef5c6: blx 0x00072b98
  000ef5ca: mov r6,r0
  000ef5cc: str r0,[sp,#0xb8]
  000ef5ce: ldr r0,[r4,#0x6c]
  000ef5d0: blx r9
  000ef5d2: ldr r0,[r0,#0x8]
  000ef5d4: blx r5
  000ef5d6: add.w r2,r6,#0xc
  000ef5da: add r6,sp,#0x198
  000ef5dc: mov r1,r0
  000ef5de: mov r0,r6
  000ef5e0: blx r10
  000ef5e2: ldr r0,[0x000ef85c]
  000ef5e4: mov r1,r6
  000ef5e6: add r0,pc
  000ef5e8: ldr r2,[r0,#0x0]
  000ef5ea: add r0,sp,#0xd8
  000ef5ec: str r2,[sp,#0xbc]
  000ef5ee: blx r2
  000ef5f0: ldr r0,[r4,#0x6c]
  000ef5f2: blx r9
  000ef5f4: ldr r0,[r0,#0x8]
  000ef5f6: blx r5
  000ef5f8: ldr r2,[sp,#0xc0]
  000ef5fa: mov r1,r0
  000ef5fc: add r0,sp,#0x140
  000ef5fe: blx r10
  000ef600: ldr r0,[r4,#0x6c]
  000ef602: blx r9
  000ef604: ldr r0,[r0,#0x8]
  000ef606: blx r5
  000ef608: add.w r9,sp,#0x17c
  000ef60c: ldr r2,[sp,#0xb8]
  000ef60e: mov r1,r0
  000ef610: mov r0,r9
  000ef612: blx r10
  000ef614: add r5,sp,#0x198
  000ef616: add r1,sp,#0x140
  000ef618: mov r2,r9
  000ef61a: mov r0,r5
  000ef61c: blx 0x0006ec38
  000ef620: add r6,sp,#0x188
  000ef622: ldr.w r9,[sp,#0xbc]
  000ef626: mov r1,r5
  000ef628: mov r0,r6
  000ef62a: blx r9
  000ef62c: add r5,sp,#0x198
  000ef62e: add.w r10,sp,#0xd8
  000ef632: mov r1,r6
  000ef634: mov r0,r5
  000ef636: mov r2,r10
  000ef638: blx 0x0006ec8c
  000ef63c: add.w r10,sp,#0x17c
  000ef640: mov r1,r5
  000ef642: mov r0,r10
  000ef644: blx r9
  000ef646: adr r0,[0xef830]
  000ef648: vmov.i32 q8,#0x0
  000ef64c: vld1.64 {d18,d19},[r0]
  000ef650: orr r0,r5,#0x4
  000ef654: mov r1,r5
  000ef656: mov r2,r10
  000ef658: vst1.32 {d16,d17},[r0]
  000ef65c: add.w r0,r5,#0x18
  000ef660: mov r3,r6
  000ef662: vst1.64 {d16,d17},[r0]
  000ef666: add.w r0,r5,#0x28
  000ef66a: vst1.64 {d18,d19},[r0]
  000ef66e: mov.w r0,#0x3f800000
  000ef672: str r0,[sp,#0x198]
  000ef674: str r0,[sp,#0x1ac]
  000ef676: str r0,[sp,#0x1d0]
  000ef678: add r0,sp,#0xd8
  000ef67a: str r0,[sp,#0x0]
  000ef67c: add r0,sp,#0x140
  000ef67e: blx 0x000701bc
  000ef682: movs r0,#0xf4
  000ef684: blx 0x0006eb24
  000ef688: ldr r3,[r4,#0x4]
  000ef68a: add.w lr,sp,#0x1bc
  000ef68e: str r0,[sp,#0x94]
  000ef690: add r2,sp,#0x1c8
  000ef692: ldr r0,[r3,#0x4]
  000ef694: str r0,[sp,#0xc0]
  000ef696: ldr r0,[r3,#0x8]
  000ef698: str r0,[sp,#0xbc]
  000ef69a: ldr r0,[r3,#0xc]
  000ef69c: str r0,[sp,#0xb8]
  000ef69e: ldr r0,[r3,#0x10]
  000ef6a0: str r0,[sp,#0xb4]
  000ef6a2: ldr r0,[r3,#0x14]
  000ef6a4: str r0,[sp,#0xb0]
  000ef6a6: ldr r0,[r3,#0x18]
  000ef6a8: str r0,[sp,#0xac]
  000ef6aa: ldr r0,[r3,#0x1c]
  000ef6ac: str r0,[sp,#0xa8]
  000ef6ae: ldr r0,[r3,#0x20]
  000ef6b0: str r0,[sp,#0xa4]
  000ef6b2: ldr r0,[r3,#0x24]
  000ef6b4: str r0,[sp,#0xa0]
  000ef6b6: ldr r0,[r3,#0x28]
  000ef6b8: str r0,[sp,#0x9c]
  000ef6ba: ldr r0,[r3,#0x2c]
  000ef6bc: str r0,[sp,#0x98]
  000ef6be: ldr r0,[r3,#0x30]
  000ef6c0: str r0,[sp,#0x90]
  000ef6c2: ldr r0,[r3,#0x34]
  000ef6c4: str r0,[sp,#0x8c]
  000ef6c6: ldr r0,[r3,#0x38]
  000ef6c8: str r0,[sp,#0x88]
  000ef6ca: ldr r0,[r3,#0x3c]
  000ef6cc: str r0,[sp,#0x84]
  000ef6ce: ldr r0,[sp,#0x198]
  000ef6d0: str r0,[sp,#0x80]
  000ef6d2: ldr r0,[sp,#0x19c]
  000ef6d4: str r0,[sp,#0x7c]
  000ef6d6: ldr r0,[sp,#0x1a0]
  000ef6d8: str r0,[sp,#0x78]
  000ef6da: ldr r0,[sp,#0x1a4]
  000ef6dc: str r0,[sp,#0x74]
  000ef6de: ldrd r9,r10,[sp,#0x1a8]
  000ef6e2: ldrd r5,r6,[sp,#0x1b0]
  000ef6e6: ldr r0,[sp,#0x1b8]
  000ef6e8: str r0,[sp,#0x70]
  000ef6ea: ldm.w lr,{r3,r12,lr}
  000ef6ee: ldmia r2,{r0,r1,r2}
  000ef6f0: str r0,[sp,#0x60]
  000ef6f2: movw r0,#0xbb8
  000ef6f6: str r1,[sp,#0x64]
  000ef6f8: add r1,sp,#0x50
  000ef6fa: str r2,[sp,#0x68]
  000ef6fc: str r0,[sp,#0x6c]
  000ef6fe: strd r9,r10,[sp,#0x40]
  000ef702: str r5,[sp,#0x48]
  000ef704: ldr r5,[sp,#0x94]
  000ef706: str r6,[sp,#0x4c]
  000ef708: ldr r0,[sp,#0x70]
  000ef70a: stm r1,{r0,r3,r12,lr}
  000ef70e: ldr r0,[sp,#0x90]
  000ef710: str r0,[sp,#0x20]
  000ef712: ldr r0,[sp,#0x8c]
  000ef714: str r0,[sp,#0x24]
  000ef716: ldr r0,[sp,#0x88]
  000ef718: str r0,[sp,#0x28]
  000ef71a: ldr r0,[sp,#0x84]
  000ef71c: str r0,[sp,#0x2c]
  000ef71e: ldr r0,[sp,#0x80]
  000ef720: str r0,[sp,#0x30]
  000ef722: ldr r0,[sp,#0x7c]
  000ef724: str r0,[sp,#0x34]
  000ef726: ldr r0,[sp,#0x78]
  000ef728: str r0,[sp,#0x38]
  000ef72a: ldr r0,[sp,#0x74]
  000ef72c: str r0,[sp,#0x3c]
  000ef72e: ldr r0,[sp,#0xb4]
  000ef730: str r0,[sp,#0x0]
  000ef732: ldr r0,[sp,#0xb0]
  000ef734: str r0,[sp,#0x4]
  000ef736: ldr r0,[sp,#0xac]
  000ef738: str r0,[sp,#0x8]
  000ef73a: ldr r0,[sp,#0xa8]
  000ef73c: str r0,[sp,#0xc]
  000ef73e: ldr r0,[sp,#0xa4]
  000ef740: str r0,[sp,#0x10]
  000ef742: ldr r0,[sp,#0xa0]
  000ef744: str r0,[sp,#0x14]
  000ef746: ldr r0,[sp,#0x9c]
  000ef748: str r0,[sp,#0x18]
  000ef74a: ldr r0,[sp,#0x98]
  000ef74c: str r0,[sp,#0x1c]
  000ef74e: mov r0,r5
  000ef750: ldr r1,[sp,#0xc0]
  000ef752: ldrd r3,r2,[sp,#0xb8]
  000ef756: blx 0x00072b74
  000ef75a: str.w r5,[r4,#0x2b8]
  000ef75e: ldr.w r10,[sp,#0xc4]
  000ef762: ldr.w r9,[sp,#0xcc]
  000ef766: movs r0,#0x1
  000ef768: strb.w r0,[r11,#0x0]
  000ef76c: b.w 0x000eda88
  000ef770: movs r0,#0x1
  000ef772: strb.w r0,[r4,#0x12e]
  000ef776: b.w 0x000ed50e
  000ef77a: movs r0,#0x1
  000ef77c: strb.w r0,[r4,#0x12e]
  000ef780: str r5,[r4,#0x38]
  000ef782: b.w 0x000ed7ac
  000ef786: ldr r0,[r4,#0x50]
  000ef788: cbz r0,0x000ef792
  000ef78a: blx 0x00070204
  000ef78e: blx 0x0006eb48
  000ef792: movs r0,#0x0
  000ef794: str r0,[r4,#0x50]
  000ef796: movs r0,#0xc
  000ef798: blx 0x0006eb24
  000ef79c: mov r5,r0
  000ef79e: blx 0x000701f8
  000ef7a2: ldr r0,[r4,#0x28]
  000ef7a4: mov r1,r5
  000ef7a6: str r5,[r4,#0x50]
  000ef7a8: adds r0,#0xbd
  000ef7aa: blx 0x0007021c
  000ef7ae: ldr r1,[r4,#0x50]
  000ef7b0: movs r0,#0x1
  000ef7b2: b.w 0x000ee028
  000ef7b6: str r5,[r4,#0x38]
  000ef7b8: b.w 0x000ed780
  000ef7bc: ldr.w r0,[r4,#0x148]
  000ef7c0: ldrb.w r0,[r0,#0x69]
  000ef7c4: cbz r0,0x000ef800
  000ef7c6: ldr r0,[r4,#0x54]
  000ef7c8: blx 0x00072034
  000ef7cc: cbz r0,0x000ef800
  000ef7ce: ldr r0,[r4,#0x54]
  000ef7d0: blx 0x00072034
  000ef7d4: blx 0x00075928
  000ef7d8: cbz r0,0x000ef800
  000ef7da: ldr.w r1,[r4,#0x148]
  000ef7de: add r0,sp,#0x198
  000ef7e0: blx 0x00072964
  000ef7e4: ldr r1,[r4,#0x4]
  000ef7e6: add r0,sp,#0x140
  000ef7e8: vldr.32 s16,[sp,#0x19c]
  000ef7ec: blx 0x00072964
  000ef7f0: vldr.32 s0,[sp,#0x144]
  000ef7f4: vcmpe.f32 s16,s0
  000ef7f8: vmrs apsr,fpscr
  000ef7fc: bgt.w 0x000ef550
  000ef800: ldr r0,[r4,#0x4]
  000ef802: movs r2,#0x0
  000ef804: ldr.w r1,[r4,#0x140]
  000ef808: mov r3,r5
  000ef80a: str r2,[sp,#0x0]
  000ef80c: mov r2,r8
  000ef80e: blx 0x00072514
  000ef812: movs r0,#0x1
  000ef814: strb.w r0,[r4,#0x2b5]
  000ef818: b.w 0x000eed44
```
