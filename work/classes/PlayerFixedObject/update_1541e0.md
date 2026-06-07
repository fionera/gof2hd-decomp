# PlayerFixedObject::update
elf 0x1541e0 body 2640
Sig: undefined __thiscall update(PlayerFixedObject * this, int param_1)

## decompile
```c

/* PlayerFixedObject::update(int) */

void __thiscall PlayerFixedObject::update(PlayerFixedObject *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  undefined1 uVar3;
  int iVar4;
  Standing *this_00;
  float fVar5;
  float fVar6;
  undefined4 uVar7;
  uint *puVar8;
  Matrix *pMVar9;
  int *piVar10;
  Explosion *pEVar11;
  TargetFollowCamera *this_01;
  PlayerEgo *this_02;
  Hud *this_03;
  bool bVar12;
  ushort uVar13;
  int iVar14;
  Vector *pVVar15;
  undefined4 *puVar16;
  ParticleSystemManager *pPVar17;
  PlayerFixedObject *pPVar18;
  int *piVar19;
  bool bVar20;
  bool bVar21;
  uint in_fpscr;
  uint uVar22;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float fVar23;
  float fVar24;
  undefined8 in_d6;
  undefined4 in_s14;
  longlong lVar25;
  undefined8 local_48;
  undefined8 local_40;
  
  piVar19 = *(int **)(DAT_00164530 + 0x1641f6);
  local_40 = CONCAT44(*piVar19,in_s14);
  *(int *)(this + 0x130) = param_1;
  bVar12 = SUB41(param_1,0);
  local_48 = in_d6;
  Player::update(*(int *)(this + 4),bVar12);
  if ((*(uint *)(this + 0x28) & 0xfffffffe) == 8) {
    iVar4 = *(int *)(this + 4);
    *(undefined1 *)(iVar4 + 0x5c) = 1;
LAB_00164258:
    uVar3 = 0;
  }
  else {
    iVar4 = Status::getStanding();
    uVar3 = Standing::isEnemy(iVar4);
    iVar4 = *(int *)(this + 4);
    uVar22 = *(uint *)(this + 0x28);
    *(undefined1 *)(iVar4 + 0x5c) = uVar3;
    if ((uVar22 & 0xfffffffe) == 8) goto LAB_00164258;
    this_00 = (Standing *)Status::getStanding();
    uVar3 = Standing::isFriend(this_00,*(int *)(this + 0x28));
    iVar4 = *(int *)(this + 4);
  }
  *(undefined1 *)(iVar4 + 0x5d) = uVar3;
  iVar4 = Player::turnedEnemy();
  if (iVar4 != 0) {
    *(undefined2 *)(*(int *)(this + 4) + 0x5c) = 1;
  }
  iVar4 = Player::isAlwaysFriend();
  if (iVar4 != 0) {
    *(undefined2 *)(*(int *)(this + 4) + 0x5c) = 0x100;
  }
  if (*(int *)(this + 0x88) != 6) {
    fVar5 = (float)Player::getBombForce();
    fVar6 = (float)Player::getEmpForce(*(Player **)(this + 4));
    in_fpscr = in_fpscr & 0xfffffff;
    uVar1 = in_fpscr | (uint)(fVar5 < 0.0) << 0x1f | (uint)(fVar5 == 0.0) << 0x1e;
    uVar22 = uVar1 | (uint)NAN(fVar5) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar22 >> 0x1c) & 1)) {
      fVar23 = fVar5 * DAT_00164538;
      if (fVar5 * DAT_00164538 < DAT_0016453c) {
        fVar23 = DAT_00164540;
      }
      Player::setBombForce(fVar23);
      uVar22 = in_fpscr;
    }
    uVar22 = uVar22 & 0xfffffff;
    uVar1 = uVar22 | (uint)(fVar6 < 0.0) << 0x1f | (uint)(fVar6 == 0.0) << 0x1e;
    in_fpscr = uVar1 | (uint)NAN(fVar6) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar5 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      fVar6 = fVar6 - fVar5;
      fVar5 = fVar6;
      if (fVar6 < DAT_0016453c) {
        fVar5 = DAT_00164540;
      }
      this[0x24] = (PlayerFixedObject)(DAT_0016453c <= fVar6);
      Player::setEmpForce(*(Player **)(this + 4),fVar5);
      in_fpscr = uVar22;
    }
  }
  if ((this[0x24] == (PlayerFixedObject)0x0) && (1 < *(int *)(this + 0x88) - 3U)) {
    uVar22 = *(uint *)(this + 0xac);
    bVar21 = uVar22 != 0x37a3;
    if (bVar21) {
      uVar22 = (uint)(byte)this[0x134];
    }
    if (bVar21 && uVar22 != 0) {
      moveForward(this,param_1);
    }
    iVar4 = Status::getCurrentCampaignMission();
    iVar14 = *(int *)(this + 0xac);
    bVar21 = iVar4 == 0x5b;
    if (bVar21) {
      iVar4 = 0x494e;
    }
    if (!bVar21 || iVar14 != iVar4) {
      if (iVar14 == 0x494a) {
        iVar4 = Status::getCurrentCampaignMission();
        if (iVar4 == 0x91) goto LAB_001643a6;
        iVar14 = *(int *)(this + 0xac);
      }
      if (iVar14 != 0x4220) {
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform
                          (**(uint **)(DAT_00164548 + 0x164390));
        AbyssEngine::Transform::Update(CONCAT44(1,uVar7),bVar12);
      }
    }
  }
LAB_001643a6:
  iVar4 = Player::getHitpoints();
  if (iVar4 < 1) {
    iVar4 = *(int *)(this + 0x88);
    if (1 < iVar4 - 3U) {
      if (*(char *)(*(int *)(this + 4) + 0x5c) == '\0') {
        Level::friendDied();
      }
      else {
        Level::enemyDied(*(int *)(this + 0x54),SUB41(*(undefined4 *)(this + 0xac),0));
      }
      pPVar18 = this + 0xac;
      if (*(int *)pPVar18 == 0x37a3) {
        Level::pirateStationAction(SUB41(*(undefined4 *)(this + 0x54),0));
      }
      this[0x134] = (PlayerFixedObject)0x0;
      *(undefined4 *)(this + 0x88) = 3;
      iVar4 = KIPlayer::cargoAvailable();
      this[0x4c] = SUB41(iVar4,0);
      if (iVar4 != 0) {
        KIPlayer::createCrate((int)this);
      }
      setExhaustVisible(this,false);
      pMVar9 = *(Matrix **)(this + 0x124);
      AEGeometry::getMatrix();
      AEGeometry::setMatrix(pMVar9);
      pMVar9 = *(Matrix **)(this + 0x124);
      if (pMVar9 == (Matrix *)0x0) {
        iVar4 = *(int *)(this + 0x54);
        pPVar17 = *(ParticleSystemManager **)(iVar4 + 0x74);
        if (*(int *)(this + 0xac) == 0x37a3 || *(int *)(this + 0xac) == 0xe) {
          piVar10 = (int *)(iVar4 + 0x54);
        }
        else {
          piVar10 = (int *)(iVar4 + 0x50);
        }
        iVar4 = *piVar10;
        pMVar9 = (Matrix *)AEGeometry::getMatrix();
        fVar5 = (float)ParticleSystemManager::systemSetMatrix(pPVar17,iVar4,pMVar9);
        pVVar15 = (Vector *)0x0;
        if (*(char *)(*(int *)(DAT_00164ca0 + 0x1649e8) + 0xf) != '\0') {
          pVVar15 = (Vector *)(this + 0x2c);
        }
        FModSound::play(**(int **)(DAT_00164c9c + 0x1649e6),(Vector *)0x14,pVVar15,fVar5);
        iVar4 = *(int *)(this + 0x54);
        if (*(int *)(this + 0xac) == 0x37a3 || *(int *)(this + 0xac) == 0xe) {
          puVar16 = (undefined4 *)(iVar4 + 0x54);
        }
        else {
          puVar16 = (undefined4 *)(iVar4 + 0x50);
        }
        ParticleSystemManager::enableSystemEmit(*(int *)(iVar4 + 0x74),SUB41(*puVar16,0));
        pEVar11 = operator_new(0x68);
        Explosion::Explosion(pEVar11,0);
        *(Explosion **)(this + 0x18c) = pEVar11;
        Explosion::addFireStreaks(pEVar11);
        pVVar15 = *(Vector **)(this + 0x18c);
      }
      else {
        AEGeometry::getMatrix();
        AEGeometry::setMatrix(pMVar9);
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform
                          (**(uint **)(DAT_001648dc + 0x1647e2));
        AbyssEngine::Transform::SetAnimationState(uVar7,1,0);
        if (((*(int *)(this + 0x28) == 3) && (this[0x134] != (PlayerFixedObject)0x0)) &&
           (*(int *)(*(int *)(this + 8) + 0x10) != -1)) {
          AEGeometry::addChild(*(uint *)(this + 0x124));
        }
        iVar4 = *(int *)(this + 0x54);
        pPVar17 = *(ParticleSystemManager **)(iVar4 + 0x74);
        if (*(int *)(this + 0xac) == 0x37a3 || *(int *)(this + 0xac) == 0xe) {
          piVar10 = (int *)(iVar4 + 0x54);
        }
        else {
          piVar10 = (int *)(iVar4 + 0x50);
        }
        iVar4 = *piVar10;
        pMVar9 = (Matrix *)AEGeometry::getMatrix();
        fVar5 = (float)ParticleSystemManager::systemSetMatrix(pPVar17,iVar4,pMVar9);
        pVVar15 = (Vector *)0x0;
        if (*(char *)(*(int *)(DAT_001648e4 + 0x16486c) + 0xf) != '\0') {
          pVVar15 = (Vector *)(this + 0x2c);
        }
        FModSound::play(**(int **)(DAT_001648e0 + 0x16486a),(Vector *)0x14,pVVar15,fVar5);
        iVar4 = *(int *)(this + 0x54);
        if (*(int *)(this + 0xac) == 0x37a3 || *(int *)(this + 0xac) == 0xe) {
          puVar16 = (undefined4 *)(iVar4 + 0x54);
        }
        else {
          puVar16 = (undefined4 *)(iVar4 + 0x50);
        }
        ParticleSystemManager::enableSystemEmit(*(int *)(iVar4 + 0x74),SUB41(*puVar16,0));
        pEVar11 = operator_new(0x68);
        Explosion::Explosion(pEVar11,0);
        *(Explosion **)(this + 0x18c) = pEVar11;
        Explosion::addFireStreaks(pEVar11);
        pVVar15 = *(Vector **)(this + 0x18c);
      }
      AEGeometry::getPosition();
      Explosion::start(pVVar15,(Vector *)&local_48);
      iVar4 = DAT_00164c90;
      if (*(int *)pPVar18 == 0xe) {
        for (uVar22 = 0; puVar8 = (uint *)Level::getEnemies(), uVar22 < *puVar8; uVar22 = uVar22 + 1
            ) {
          iVar14 = Level::getEnemies();
          if (*(char *)(*(int *)(*(int *)(iVar14 + 4) + uVar22 * 4) + 0x3e) != '\0') {
            iVar14 = Level::getEnemies();
            Player::damage(*(Player **)(*(int *)(*(int *)(iVar14 + 4) + uVar22 * 4) + 4),iVar4);
          }
        }
        if ((*(int *)pPVar18 == 0xe) && (*(char *)(*(int *)(this + 4) + 0x44) == '\0')) {
          piVar10 = *(int **)(DAT_00164ca4 + 0x164ac4);
          puVar16 = *(undefined4 **)(DAT_00164ca8 + 0x164acc);
          *(int *)(*piVar10 + 0x118) = *(int *)(*piVar10 + 0x118) + 1;
          iVar4 = Achievements::hasMedal((Achievements *)*puVar16,0x27,1);
          if (iVar4 == 0) {
            fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar10 + 0x118),
                                               (byte)(in_fpscr >> 0x16) & 3);
            uVar7 = Achievements::getValue((Achievements *)*puVar16,0x27,1);
            fVar5 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
            if ((int)(fVar6 / fVar5) < 2) {
              this_02 = (PlayerEgo *)Level::getPlayer();
              this_03 = (Hud *)PlayerEgo::getHUD(this_02);
              fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar10 + 0x118),
                                                 (byte)(in_fpscr >> 0x16) & 3);
              uVar7 = Achievements::getValue((Achievements *)*puVar16,0x27,1);
              fVar5 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
              Hud::hudEventMedal(this_03,0x27,(int)((fVar6 / fVar5) * DAT_00164c8c));
            }
          }
        }
      }
      goto LAB_001643b2;
    }
  }
  else {
LAB_001643b2:
    iVar4 = *(int *)(this + 0x88);
  }
  if (iVar4 == 3) {
    if (*(Explosion **)(this + 0x18c) != (Explosion *)0x0) {
      Explosion::update(*(Explosion **)(this + 0x18c),param_1,(TargetFollowCamera *)0x0);
    }
    if (*(int *)(this + 0xac) != 0x37a3) {
      if (this[0x134] != (PlayerFixedObject)0x0) {
        fVar5 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
        *(int *)(this + 0x180) = *(int *)(this + 0x180) + param_1;
        fVar5 = (float)AEGeometry::moveForward(fVar5);
        if (*(int *)(this + 0x78) != 0) {
          AEGeometry::moveForward(fVar5);
        }
      }
      pMVar9 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar9);
      AEGeometry::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),(Vector *)&local_48);
      puVar8 = *(uint **)(this + 0x128);
      if (puVar8 != (uint *)0x0) {
        for (uVar22 = 0; uVar22 < *puVar8; uVar22 = uVar22 + 1) {
          piVar10 = *(int **)(puVar8[1] + uVar22 * 4);
          (**(code **)(*piVar10 + 4))
                    (piVar10,*(undefined4 *)(this + 0x2c),*(undefined4 *)(this + 0x30),
                     *(undefined4 *)(this + 0x34));
          puVar8 = *(uint **)(this + 0x128);
        }
      }
    }
    puVar16 = (undefined4 *)(DAT_001648cc + 0x1645fa);
    this[0x101] = (PlayerFixedObject)0x0;
    puVar8 = (uint *)*puVar16;
    lVar25 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
    AbyssEngine::Transform::Update(lVar25,bVar12);
    iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
    if (*(char *)(iVar4 + 0xed) == '\0') {
      iVar4 = *(int *)(this + 0x54);
      *(undefined4 *)(this + 0x88) = 4;
      if (*(int *)(this + 0xac) == 0x37a3 || *(int *)(this + 0xac) == 0xe) {
        puVar16 = (undefined4 *)(iVar4 + 0x54);
      }
      else {
        puVar16 = (undefined4 *)(iVar4 + 0x50);
      }
      ParticleSystemManager::enableSystemEmit(*(int *)(iVar4 + 0x74),SUB41(*puVar16,0));
      Explosion::reset(*(Explosion **)(this + 0x18c));
      fVar5 = 6.0;
      if (*(int *)(this + 0xac) == 0x37e7) {
        fVar5 = 8.0;
      }
      if (*(int *)(this + 0xac) == 0x37a3) {
        fVar5 = 8.0;
      }
      Explosion::setScaling(fVar5);
      local_48 = 0;
      local_40 = local_40 & 0xffffffff00000000;
      Explosion::start(*(Vector **)(this + 0x18c),(Vector *)(this + 0x2c));
      *(undefined4 *)(this + 0x198) = 1;
      *(undefined4 *)(this + 400) = 0;
      iVar4 = Level::getPlayer();
      if (iVar4 != 0) {
        Level::getPlayer();
        iVar4 = PlayerEgo::getTargetFollowCamera();
        if (iVar4 != 0) {
          Level::getPlayer();
          this_01 = (TargetFollowCamera *)PlayerEgo::getTargetFollowCamera();
          TargetFollowCamera::getPosition(this_01);
          AbyssEngine::AEMath::operator-((Vector *)&local_48,(Vector *)(this + 0x2c));
          fVar6 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_48);
          fVar5 = DAT_00164c98;
          if ((int)((uint)(fVar6 < DAT_00164c98) << 0x1f) < 0) {
            fVar5 = fVar6;
          }
          *(float *)(this + 0x19c) = 1.0 - fVar5 / DAT_00164c98;
          Level::getPlayer();
          iVar4 = PlayerEgo::getTargetFollowCamera();
          TargetFollowCamera::setRumblePercentage(extraout_s0,iVar4);
        }
      }
    }
    goto LAB_00164c4e;
  }
  if (iVar4 != 4) {
    if (iVar4 == 5) {
      puVar8 = (uint *)Player::getEnemies();
      fVar6 = DAT_00164550;
      fVar5 = DAT_0016454c;
      if (puVar8 != (uint *)0x0) {
        *(undefined4 *)(this + 0x168) = 0;
        for (uVar22 = 0; uVar22 < *puVar8; uVar22 = uVar22 + 1) {
          iVar4 = Player::isActive();
          if (iVar4 != 0) {
            Player::getPosition();
            AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_48);
            if ((((int)((uint)(*(float *)(this + 0x2c) - *(float *)(this + 0x90) < fVar5) << 0x1f) <
                  0) && (fVar6 < *(float *)(this + 0x2c) - *(float *)(this + 0x90))) &&
               (((int)((uint)(*(float *)(this + 0x30) - *(float *)(this + 0x94) < fVar5) << 0x1f) <
                 0 && (((fVar6 < *(float *)(this + 0x30) - *(float *)(this + 0x94) &&
                        ((int)((uint)(*(float *)(this + 0x34) - *(float *)(this + 0x98) < fVar5) <<
                              0x1f) < 0)) &&
                       (fVar6 < *(float *)(this + 0x34) - *(float *)(this + 0x98))))))) {
              uVar7 = Player::getEnemy(*(Player **)(this + 4),uVar22);
              *(undefined4 *)(this + 0x168) = uVar7;
              Player::getPosition();
              AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x90),(Vector *)&local_48);
              *(undefined4 *)(this + 0x144) = *(undefined4 *)(this + 0x90);
              *(undefined4 *)(this + 0x148) = *(undefined4 *)(this + 0x94);
              *(undefined4 *)(this + 0x14c) = *(undefined4 *)(this + 0x98);
              break;
            }
          }
        }
      }
      fVar5 = DAT_0016454c;
      fVar24 = *(float *)(this + 0x144) - *(float *)(this + 0x2c);
      fVar23 = *(float *)(this + 0x148) - *(float *)(this + 0x30);
      fVar6 = *(float *)(this + 0x14c) - *(float *)(this + 0x34);
      bVar12 = fVar24 < DAT_0016454c;
      *(float *)(this + 0x150) = fVar24;
      *(float *)(this + 0x154) = fVar23;
      *(float *)(this + 0x158) = fVar6;
      if ((int)((uint)bVar12 << 0x1f) < 0) {
        bVar12 = fVar24 < DAT_00164550;
        bVar21 = fVar24 == DAT_00164550;
        bVar20 = NAN(fVar24) || NAN(DAT_00164550);
        if (!bVar21 && !bVar12) {
          bVar12 = fVar6 < DAT_00164550;
          bVar21 = fVar6 == DAT_00164550;
          bVar20 = NAN(fVar6) || NAN(DAT_00164550);
        }
        if (((!bVar21 && bVar12 == bVar20) && ((int)((uint)(fVar23 < fVar5) << 0x1f) < 0)) &&
           ((DAT_00164550 < fVar23 && ((int)((uint)(fVar6 < fVar5) << 0x1f) < 0)))) {
          *(undefined4 *)(this + 0x88) = 1;
          Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
        }
      }
    }
    goto LAB_00164c4e;
  }
  *(int *)(this + 400) = *(int *)(this + 400) + param_1;
  if (*(Explosion **)(this + 0x18c) != (Explosion *)0x0) {
    Explosion::update(*(Explosion **)(this + 0x18c),param_1,(TargetFollowCamera *)0x0);
  }
  *(int *)(this + 0xd8) = *(int *)(this + 0xd8) + param_1;
  if (((this[0x4c] == (PlayerFixedObject)0x0) || (iVar4 = Player::isActive(), iVar4 == 0)) ||
     (*(int *)(this + 0x78) == 0)) {
    if ((*(int *)(this + 0x18c) != 0) && (iVar4 = Explosion::isPlaying(), iVar4 == 0)) {
      if (60000 < *(int *)(this + 0xd8)) goto LAB_001646d0;
      goto LAB_001646d8;
    }
  }
  else {
    fVar5 = (float)VectorSignedToFloat(param_1 >> 1,(byte)(in_fpscr >> 0x16) & 3);
    fVar5 = (float)VectorSignedToFloat((int)(fVar5 * DAT_001648d0 * DAT_001648d4),
                                       (byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::rotate(fVar5,extraout_s1,DAT_001648d4);
    if (60000 < *(int *)(this + 0xd8)) {
LAB_001646d0:
      KIPlayer::setActive(SUB41(this,0));
LAB_001646d8:
      this[0x101] = (PlayerFixedObject)0x1;
    }
  }
  if (-1 < *(int *)(this + 0x194)) {
    if (((*(int *)(this + 300) != 0) && (0x8c < *(int *)(this + 400))) &&
       (0x7fffffff < *(uint *)(this + 0x1a0))) {
      AEGeometry::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),(Vector *)&local_48);
      for (uVar22 = 0; iVar4 = DAT_00164cac, uVar22 < **(uint **)(this + 300); uVar22 = uVar22 + 1)
      {
        piVar10 = *(int **)((*(uint **)(this + 300))[1] + uVar22 * 4);
        (**(code **)(*piVar10 + 4))
                  (piVar10,*(undefined4 *)(this + 0x2c),*(undefined4 *)(this + 0x30),
                   *(undefined4 *)(this + 0x34));
      }
      if (*(uint *)(this + 0x194) < 5) {
        uVar13 = (short)DAT_001648d8 + 4;
        uVar7 = 0x824c;
        switch(*(uint *)(this + 0x194)) {
        case 1:
          uVar7 = 0x8248;
          uVar13 = 0x8248;
          break;
        case 2:
          uVar13 = 0x8249;
          uVar7 = 0x8249;
          break;
        case 3:
          uVar13 = 0x824a;
          uVar7 = 0x824a;
          break;
        case 4:
          uVar13 = 0x824b;
          uVar7 = 0x824b;
        }
      }
      else {
        uVar13 = 0x824d;
        uVar7 = 0x824d;
      }
      *(undefined4 *)(this + 0x1a0) = uVar7;
      puVar16 = *(undefined4 **)(iVar4 + 0x164bb0);
      AbyssEngine::PaintCanvas::MaterialCreate((PaintCanvas *)*puVar16,uVar13,(uint *)&local_48);
      AbyssEngine::PaintCanvas::MeshChangeMaterial
                ((PaintCanvas *)*puVar16,*(uint *)(*(int *)(this + 0x124) + 0x1c),(ushort)local_48);
    }
    iVar4 = Level::getPlayer();
    if (iVar4 != 0) {
      Level::getPlayer();
      iVar4 = PlayerEgo::getTargetFollowCamera();
      if ((iVar4 != 0) && (0 < *(int *)(this + 0x198))) {
        iVar4 = *(int *)(this + 0x198) + param_1;
        if (1999 < iVar4) {
          iVar4 = 2000;
        }
        *(int *)(this + 0x198) = iVar4;
        Level::getPlayer();
        iVar4 = PlayerEgo::getTargetFollowCamera();
        fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x198),
                                           (byte)(in_fpscr >> 0x16) & 3);
        TargetFollowCamera::setRumblePercentage
                  (*(float *)(this + 0x19c) * (fVar5 / DAT_00164c94 + 1.0),iVar4);
        if ((*(int *)(this + 0x18c) != 0) && (iVar4 = Explosion::isPlaying(), iVar4 == 0)) {
          Level::getPlayer();
          iVar4 = PlayerEgo::getTargetFollowCamera();
          TargetFollowCamera::setRumblePercentage(extraout_s0_00,iVar4);
          *(undefined4 *)(this + 0x198) = 0;
        }
      }
    }
  }
LAB_00164c4e:
  iVar4 = *(int *)(this + 4);
  *(undefined4 *)(iVar4 + 0x48) = *(undefined4 *)(this + 0x178);
  *(undefined4 *)(iVar4 + 0x4c) = *(undefined4 *)(this + 0x17c);
  *(undefined4 *)(iVar4 + 0x50) = *(undefined4 *)(this + 0x180);
  if (*piVar19 != local_40._4_4_) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001641e0: push {r4,r5,r6,r7,lr}
  001641e2: add r7,sp,#0xc
  001641e4: push {r7,r8,r9,r10,r11}
  001641e8: vpush {d4,d5,d6,d7,d8,d9}
  001641ec: mov r4,r0
  001641ee: ldr r0,[0x00164530]
  001641f0: mov r8,r1
  001641f2: add r0,pc
  001641f4: ldr.w r10,[r0,#0x0]
  001641f8: ldr.w r0,[r10,#0x0]
  001641fc: str r0,[sp,#0x1c]
  001641fe: ldr.w r1,[r4,#0xf8]
  00164202: ldr r0,[r4,#0x4]
  00164204: adds r1,#0x1
  00164206: str.w r8,[r4,#0x130]
  0016420a: beq 0x00164218
  0016420c: ldrb.w r2,[r4,#0x134]
  00164210: cmp r2,#0x0
  00164212: it ne
  00164214: mov.ne r2,#0x1
  00164216: b 0x0016421a
  00164218: movs r2,#0x0
  0016421a: mov r1,r8
  0016421c: blx 0x00072958
  00164220: ldr r0,[r4,#0x28]
  00164222: bic r0,r0,#0x1
  00164226: cmp r0,#0x8
  00164228: bne 0x00164234
  0016422a: ldr r0,[r4,#0x4]
  0016422c: movs r1,#0x1
  0016422e: strb.w r1,[r0,#0x5c]
  00164232: b 0x00164258
  00164234: ldr r0,[0x00164534]
  00164236: add r0,pc
  00164238: ldr r5,[r0,#0x0]
  0016423a: ldr r0,[r5,#0x0]
  0016423c: blx 0x00071b84
  00164240: ldr r1,[r4,#0x28]
  00164242: blx 0x00071b90
  00164246: mov r1,r0
  00164248: ldr r0,[r4,#0x4]
  0016424a: ldr r2,[r4,#0x28]
  0016424c: strb.w r1,[r0,#0x5c]
  00164250: bic r1,r2,#0x1
  00164254: cmp r1,#0x8
  00164256: bne 0x0016425c
  00164258: movs r1,#0x0
  0016425a: b 0x0016426c
  0016425c: ldr r0,[r5,#0x0]
  0016425e: blx 0x00071b84
  00164262: ldr r1,[r4,#0x28]
  00164264: blx 0x00075808
  00164268: mov r1,r0
  0016426a: ldr r0,[r4,#0x4]
  0016426c: strb.w r1,[r0,#0x5d]
  00164270: blx 0x00075814
  00164274: mov r1,r0
  00164276: ldr r0,[r4,#0x4]
  00164278: cbz r1,0x00164280
  0016427a: movs r1,#0x1
  0016427c: strh.w r1,[r0,#0x5c]
  00164280: blx 0x0007420c
  00164284: cbz r0,0x00164290
  00164286: ldr r0,[r4,#0x4]
  00164288: mov.w r1,#0x100
  0016428c: strh.w r1,[r0,#0x5c]
  00164290: ldr.w r0,[r4,#0x88]
  00164294: cmp r0,#0x6
  00164296: beq 0x00164328
  00164298: ldr r0,[r4,#0x4]
  0016429a: blx 0x00075940
  0016429e: ldr r1,[r4,#0x4]
  001642a0: vmov s18,r0
  001642a4: mov r0,r1
  001642a6: blx 0x00077410
  001642aa: vcmpe.f32 s18,#0
  001642ae: vmov s16,r0
  001642b2: vmrs apsr,fpscr
  001642b6: ble 0x001642e0
  001642b8: vldr.32 s0,[pc,#0x27c]
  001642bc: vldr.32 s2,[pc,#0x27c]
  001642c0: vmul.f32 s0,s18,s0
  001642c4: vldr.32 s4,[pc,#0x278]
  001642c8: vcmpe.f32 s0,s2
  001642cc: vmrs apsr,fpscr
  001642d0: it mi
  001642d2: vmov.mi.f32 s0,s4
  001642d6: ldr r0,[r4,#0x4]
  001642d8: vmov r1,s0
  001642dc: blx 0x00072aa8
  001642e0: vcmpe.f32 s16,#0
  001642e4: vmrs apsr,fpscr
  001642e8: ble 0x00164328
  001642ea: vmov s0,r8
  001642ee: vldr.32 s2,[pc,#0x24c]
  001642f2: vldr.32 s4,[pc,#0x24c]
  001642f6: movs r2,#0x0
  001642f8: vcvt.f32.s32 s0,s0
  001642fc: vsub.f32 s0,s16,s0
  00164300: vcmpe.f32 s0,s2
  00164304: vmrs apsr,fpscr
  00164308: vcmpe.f32 s0,s2
  0016430c: it mi
  0016430e: vmov.mi.f32 s0,s4
  00164312: vmrs apsr,fpscr
  00164316: vmov r1,s0
  0016431a: ldr r0,[r4,#0x4]
  0016431c: it pl
  0016431e: mov.pl r2,#0x1
  00164320: strb.w r2,[r4,#0x24]
  00164324: blx 0x0007738c
  00164328: ldrb.w r0,[r4,#0x24]
  0016432c: cbnz r0,0x001643a6
  0016432e: ldr.w r0,[r4,#0x88]
  00164332: subs r0,#0x3
  00164334: cmp r0,#0x2
  00164336: bcc 0x001643a6
  00164338: ldr.w r0,[r4,#0xac]
  0016433c: movw r1,#0x37a3
  00164340: cmp r0,r1
  00164342: itt ne
  00164344: ldrb.ne.w r0,[r4,#0x134]
  00164348: cmp.ne r0,#0x0
  0016434a: bne.w 0x00164840
  0016434e: ldr r0,[0x00164544]
  00164350: add r0,pc
  00164352: ldr r5,[r0,#0x0]
  00164354: ldr r0,[r5,#0x0]
  00164356: blx 0x00071770
  0016435a: ldr.w r1,[r4,#0xac]
  0016435e: cmp r0,#0x5b
  00164360: itt eq
  00164362: movw.eq r0,#0x494e
  00164366: cmp.eq r1,r0
  00164368: beq 0x001643a6
  0016436a: movw r0,#0x494a
  0016436e: cmp r1,r0
  00164370: bne 0x00164380
  00164372: ldr r0,[r5,#0x0]
  00164374: blx 0x00071770
  00164378: cmp r0,#0x91
  0016437a: beq 0x001643a6
  0016437c: ldr.w r1,[r4,#0xac]
  00164380: movw r0,#0x4220
  00164384: cmp r1,r0
  00164386: beq 0x001643a6
  00164388: ldr r0,[0x00164548]
  0016438a: ldr r1,[r4,#0x8]
  0016438c: add r0,pc
  0016438e: ldr r0,[r0,#0x0]
  00164390: ldr r1,[r1,#0xc]
  00164392: ldr r0,[r0,#0x0]
  00164394: blx 0x00072088
  00164398: movs r1,#0x1
  0016439a: asr.w r3,r8, asr #0x1f
  0016439e: mov r2,r8
  001643a0: str r1,[sp,#0x0]
  001643a2: blx 0x0006f7cc
  001643a6: ldr r0,[r4,#0x4]
  001643a8: blx 0x000724f0
  001643ac: cmp r0,#0x0
  001643ae: ble.w 0x00164504
  001643b2: ldr.w r0,[r4,#0x88]
  001643b6: cmp r0,#0x3
  001643b8: beq.w 0x00164554
  001643bc: cmp r0,#0x4
  001643be: beq.w 0x00164648
  001643c2: cmp r0,#0x5
  001643c4: bne.w 0x00164c4e
  001643c8: ldr r0,[r4,#0x4]
  001643ca: blx 0x0007582c
  001643ce: mov r6,r0
  001643d0: cmp r0,#0x0
  001643d2: beq 0x00164476
  001643d4: add.w r8,r4,#0x90
  001643d8: add.w r9,sp,#0x10
  001643dc: vldr.32 s16,[pc,#0x16c]
  001643e0: movs r5,#0x0
  001643e2: vldr.32 s18,[pc,#0x16c]
  001643e6: str.w r5,[r4,#0x168]
  001643ea: b 0x00164470
  001643ec: ldr r0,[r6,#0x4]
  001643ee: ldr.w r0,[r0,r5,lsl #0x2]
  001643f2: blx 0x00071f14
  001643f6: cbz r0,0x0016446e
  001643f8: ldr r0,[r6,#0x4]
  001643fa: ldr.w r1,[r0,r5,lsl #0x2]
  001643fe: mov r0,r9
  00164400: blx 0x00072964
  00164404: mov r0,r8
  00164406: mov r1,r9
  00164408: blx 0x0006eb3c
  0016440c: vldr.32 s0,[r4,#0x2c]
  00164410: vldr.32 s2,[r4,#0x90]
  00164414: vsub.f32 s0,s0,s2
  00164418: vcmpe.f32 s0,s16
  0016441c: vmrs apsr,fpscr
  00164420: bpl 0x0016446e
  00164422: vcmpe.f32 s0,s18
  00164426: vmrs apsr,fpscr
  0016442a: ble 0x0016446e
  0016442c: vldr.32 s0,[r4,#0x30]
  00164430: vldr.32 s2,[r4,#0x94]
  00164434: vsub.f32 s0,s0,s2
  00164438: vcmpe.f32 s0,s16
  0016443c: vmrs apsr,fpscr
  00164440: bpl 0x0016446e
  00164442: vcmpe.f32 s0,s18
  00164446: vmrs apsr,fpscr
  0016444a: ble 0x0016446e
  0016444c: vldr.32 s0,[r4,#0x34]
  00164450: vldr.32 s2,[r4,#0x98]
  00164454: vsub.f32 s0,s0,s2
  00164458: vcmpe.f32 s0,s16
  0016445c: vmrs apsr,fpscr
  00164460: bpl 0x0016446e
  00164462: vcmpe.f32 s0,s18
  00164466: vmrs apsr,fpscr
  0016446a: bgt.w 0x00164b5a
  0016446e: adds r5,#0x1
  00164470: ldr r0,[r6,#0x0]
  00164472: cmp r5,r0
  00164474: bcc 0x001643ec
  00164476: vldr.32 s0,[r4,#0x2c]
  0016447a: vldr.32 s4,[r4,#0x144]
  0016447e: vldr.32 s2,[r4,#0x30]
  00164482: vldr.32 s6,[r4,#0x34]
  00164486: vsub.f32 s8,s4,s0
  0016448a: vldr.32 s10,[r4,#0x148]
  0016448e: vldr.32 s12,[r4,#0x14c]
  00164492: vsub.f32 s4,s10,s2
  00164496: vldr.32 s2,[pc,#0xb4]
  0016449a: vsub.f32 s0,s12,s6
  0016449e: vcmpe.f32 s8,s2
  001644a2: vstr.32 s8,[r4,#0x150]
  001644a6: vmrs apsr,fpscr
  001644aa: vstr.32 s4,[r4,#0x154]
  001644ae: vstr.32 s0,[r4,#0x158]
  001644b2: bpl.w 0x00164c4e
  001644b6: vldr.32 s6,[pc,#0x98]
  001644ba: vcmpe.f32 s8,s6
  001644be: vmrs apsr,fpscr
  001644c2: itt gt
  001644c4: vcmpe.gt.f32 s0,s6
  001644c8: vmrs.gt apsr,fpscr
  001644cc: ble.w 0x00164c4e
  001644d0: vcmpe.f32 s4,s2
  001644d4: vmrs apsr,fpscr
  001644d8: bpl.w 0x00164c4e
  001644dc: vcmpe.f32 s4,s6
  001644e0: vmrs apsr,fpscr
  001644e4: ble.w 0x00164c4e
  001644e8: vcmpe.f32 s0,s2
  001644ec: vmrs apsr,fpscr
  001644f0: bpl.w 0x00164c4e
  001644f4: ldr r0,[r4,#0x4]
  001644f6: movs r1,#0x1
  001644f8: str.w r1,[r4,#0x88]
  001644fc: movs r1,#0x1
  001644fe: blx 0x00072580
  00164502: b 0x00164c4e
  00164504: ldr.w r0,[r4,#0x88]
  00164508: subs r1,r0,#0x3
  0016450a: cmp r1,#0x2
  0016450c: bcc.w 0x001643b6
  00164510: ldr r1,[r4,#0x4]
  00164512: ldr r0,[r4,#0x54]
  00164514: ldrb.w r2,[r1,#0x5c]
  00164518: cmp r2,#0x0
  0016451a: beq.w 0x0016476c
  0016451e: mov r9,r4
  00164520: ldrb.w r2,[r1,#0x44]
  00164524: ldr.w r1,[r9,#0xac]!
  00164528: blx 0x000758ec
  0016452c: b 0x00164774
  00164554: ldr.w r0,[r4,#0x18c]
  00164558: cbz r0,0x00164562
  0016455a: mov r1,r8
  0016455c: movs r2,#0x0
  0016455e: blx 0x00075934
  00164562: ldr.w r0,[r4,#0xac]
  00164566: movw r1,#0x37a3
  0016456a: cmp r0,r1
  0016456c: beq 0x001645ee
  0016456e: ldrb.w r0,[r4,#0x134]
  00164572: cbz r0,0x0016459e
  00164574: vmov s0,r8
  00164578: vcvt.f32.s32 s0,s0
  0016457c: ldr.w r1,[r4,#0x180]
  00164580: ldr.w r0,[r4,#0x124]
  00164584: add r1,r8
  00164586: str.w r1,[r4,#0x180]
  0016458a: vmov r6,s0
  0016458e: mov r1,r6
  00164590: blx 0x000725c8
  00164594: ldr r0,[r4,#0x78]
  00164596: cbz r0,0x0016459e
  00164598: mov r1,r6
  0016459a: blx 0x000725c8
  0016459e: ldr.w r0,[r4,#0x124]
  001645a2: blx 0x000721cc
  001645a6: mov r1,r0
  001645a8: ldr r0,[r4,#0x4]
  001645aa: adds r0,#0x4
  001645ac: blx 0x0006f148
  001645b0: add r5,sp,#0x10
  001645b2: ldr.w r1,[r4,#0x124]
  001645b6: mov r0,r5
  001645b8: blx 0x000720b8
  001645bc: add.w r0,r4,#0x2c
  001645c0: mov r1,r5
  001645c2: blx 0x0006eb3c
  001645c6: ldr.w r0,[r4,#0x128]
  001645ca: cbz r0,0x001645ee
  001645cc: movs r5,#0x0
  001645ce: b 0x001645e8
  001645d0: ldr r0,[r0,#0x4]
  001645d2: add.w r3,r4,#0x2c
  001645d6: ldr.w r0,[r0,r5,lsl #0x2]
  001645da: ldr r6,[r0,#0x0]
  001645dc: ldmia r3,{r1,r2,r3}
  001645de: ldr r6,[r6,#0x4]
  001645e0: blx r6
  001645e2: ldr.w r0,[r4,#0x128]
  001645e6: adds r5,#0x1
  001645e8: ldr r1,[r0,#0x0]
  001645ea: cmp r5,r1
  001645ec: bcc 0x001645d0
  001645ee: ldr r0,[0x001648cc]
  001645f0: movs r6,#0x0
  001645f2: ldr.w r1,[r4,#0x124]
  001645f6: add r0,pc
  001645f8: strb.w r6,[r4,#0x101]
  001645fc: ldr r5,[r0,#0x0]
  001645fe: ldr r1,[r1,#0xc]
  00164600: ldr r0,[r5,#0x0]
  00164602: blx 0x00072088
  00164606: asr.w r3,r8, asr #0x1f
  0016460a: mov r2,r8
  0016460c: str r6,[sp,#0x0]
  0016460e: blx 0x0006f7cc
  00164612: ldr.w r1,[r4,#0x124]
  00164616: ldr r0,[r5,#0x0]
  00164618: ldr r1,[r1,#0xc]
  0016461a: blx 0x00072088
  0016461e: ldrb.w r0,[r0,#0xed]
  00164622: cmp r0,#0x0
  00164624: bne.w 0x00164c4e
  00164628: ldr r1,[r4,#0x54]
  0016462a: movs r0,#0x4
  0016462c: ldr.w r2,[r4,#0xac]
  00164630: movw r3,#0x37a3
  00164634: str.w r0,[r4,#0x88]
  00164638: ldr r0,[r1,#0x74]
  0016463a: cmp r2,r3
  0016463c: it ne
  0016463e: cmp.ne r2,#0xe
  00164640: bne.w 0x001648e8
  00164644: adds r1,#0x54
  00164646: b 0x001648ea
  00164648: ldrd r0,r1,[r4,#0x18c]
  0016464c: add r1,r8
  0016464e: str.w r1,[r4,#0x190]
  00164652: cbz r0,0x0016465c
  00164654: mov r1,r8
  00164656: movs r2,#0x0
  00164658: blx 0x00075934
  0016465c: ldrb.w r1,[r4,#0x4c]
  00164660: ldr.w r0,[r4,#0xd8]
  00164664: cmp r1,#0x0
  00164666: add r0,r8
  00164668: str.w r0,[r4,#0xd8]
  0016466c: beq 0x001646b8
  0016466e: ldr r0,[r4,#0x4]
  00164670: blx 0x00071f14
  00164674: cbz r0,0x001646b8
  00164676: ldr r0,[r4,#0x78]
  00164678: cbz r0,0x001646b8
  0016467a: asr.w r1,r8, asr #0x1
  0016467e: vldr.32 s2,[pc,#0x250]
  00164682: vmov s0,r1
  00164686: vcvt.f32.s32 s0,s0
  0016468a: vmul.f32 s0,s0,s2
  0016468e: vldr.32 s2,[pc,#0x244]
  00164692: vmul.f32 s0,s0,s2
  00164696: vcvt.s32.f32 s0,s0
  0016469a: vcvt.f32.s32 s0,s0
  0016469e: vmov r1,s0
  001646a2: mov r2,r1
  001646a4: mov r3,r1
  001646a6: blx 0x00072160
  001646aa: ldr.w r0,[r4,#0xd8]
  001646ae: movw r1,#0xea61
  001646b2: cmp r0,r1
  001646b4: bge 0x001646d0
  001646b6: b 0x001646de
  001646b8: ldr.w r0,[r4,#0x18c]
  001646bc: cbz r0,0x001646de
  001646be: blx 0x000757fc
  001646c2: cbnz r0,0x001646de
  001646c4: ldr.w r0,[r4,#0xd8]
  001646c8: movw r1,#0xea61
  001646cc: cmp r0,r1
  001646ce: blt 0x001646d8
  001646d0: mov r0,r4
  001646d2: movs r1,#0x0
  001646d4: blx 0x000732f4
  001646d8: movs r0,#0x1
  001646da: strb.w r0,[r4,#0x101]
  001646de: ldr.w r0,[r4,#0x194]
  001646e2: cmp r0,#0x0
  001646e4: blt.w 0x00164c4e
  001646e8: ldr.w r0,[r4,#0x12c]
  001646ec: cmp r0,#0x0
  001646ee: beq.w 0x00164bc8
  001646f2: ldr.w r0,[r4,#0x190]
  001646f6: cmp r0,#0x8d
  001646f8: blt.w 0x00164bc8
  001646fc: ldr.w r0,[r4,#0x1a0]
  00164700: cmp.w r0,#0xffffffff
  00164704: bgt.w 0x00164bc8
  00164708: add r5,sp,#0x10
  0016470a: ldr r1,[r4,#0x8]
  0016470c: mov r0,r5
  0016470e: blx 0x000720b8
  00164712: add.w r0,r4,#0x2c
  00164716: mov r1,r5
  00164718: blx 0x0006eb3c
  0016471c: movs r5,#0x0
  0016471e: b 0x00164734
  00164720: ldr r0,[r0,#0x4]
  00164722: add.w r3,r4,#0x2c
  00164726: ldr.w r0,[r0,r5,lsl #0x2]
  0016472a: ldr r6,[r0,#0x0]
  0016472c: ldmia r3,{r1,r2,r3}
  0016472e: ldr r6,[r6,#0x4]
  00164730: blx r6
  00164732: adds r5,#0x1
  00164734: ldr.w r0,[r4,#0x12c]
  00164738: ldr r1,[r0,#0x0]
  0016473a: cmp r5,r1
  0016473c: bcc 0x00164720
  0016473e: ldr.w r3,[r4,#0x194]
  00164742: movw r2,#0x8248
  00164746: cmp r3,#0x4
  00164748: bhi.w 0x00164b86
  0016474c: ldr r0,[0x001648d8]
  0016474e: adds r1,r0,#0x4
  00164750: movw r0,#0x824c
  00164754: tbh [pc,r3]
  00164762: movw r0,#0x8248
  00164766: movw r1,#0x8248
  0016476a: b 0x00164ba4
  0016476c: blx 0x000758d4
  00164770: add.w r9,r4,#0xac
  00164774: ldr.w r0,[r9,#0x0]
  00164778: movw r1,#0x37a3
  0016477c: cmp r0,r1
  0016477e: bne 0x00164788
  00164780: ldr r0,[r4,#0x54]
  00164782: movs r1,#0x0
  00164784: blx 0x00075910
  00164788: movs r0,#0x0
  0016478a: strb.w r0,[r4,#0x134]
  0016478e: movs r0,#0x3
  00164790: str.w r0,[r4,#0x88]
  00164794: mov r0,r4
  00164796: blx 0x000758f8
  0016479a: cmp r0,#0x0
  0016479c: strb.w r0,[r4,#0x4c]
  001647a0: beq 0x001647aa
  001647a2: mov r0,r4
  001647a4: movs r1,#0x0
  001647a6: blx 0x00075904
  001647aa: mov r0,r4
  001647ac: movs r1,#0x0
  001647ae: blx 0x00076d08
  001647b2: ldr r0,[r4,#0x8]
  001647b4: ldr.w r6,[r4,#0x124]
  001647b8: blx 0x000721cc
  001647bc: mov r1,r0
  001647be: mov r0,r6
  001647c0: blx 0x000721e4
  001647c4: ldr.w r6,[r4,#0x124]
  001647c8: cbz r6,0x00164826
  001647ca: ldr r0,[r4,#0x8]
  001647cc: blx 0x000721cc
  001647d0: mov r1,r0
  001647d2: mov r0,r6
  001647d4: blx 0x000721e4
  001647d8: ldr r0,[0x001648dc]
  001647da: ldr.w r1,[r4,#0x124]
  001647de: add r0,pc
  001647e0: ldr r0,[r0,#0x0]
  001647e2: ldr r1,[r1,#0xc]
  001647e4: ldr r0,[r0,#0x0]
  001647e6: blx 0x00072088
  001647ea: movs r1,#0x1
  001647ec: movs r2,#0x0
  001647ee: blx 0x0006fd18
  001647f2: ldr r0,[r4,#0x28]
  001647f4: cmp r0,#0x3
  001647f6: bne 0x0016480e
  001647f8: ldrb.w r0,[r4,#0x134]
  001647fc: cbz r0,0x0016480e
  001647fe: ldr r0,[r4,#0x8]
  00164800: ldr r1,[r0,#0x10]
  00164802: adds r0,r1,#0x1
  00164804: itt ne
  00164806: ldr.ne.w r0,[r4,#0x124]
  0016480a: blx.ne 0x000720e8
  0016480e: ldr r0,[r4,#0x54]
  00164810: movw r2,#0x37a3
  00164814: ldr.w r1,[r4,#0xac]
  00164818: ldr r6,[r0,#0x74]
  0016481a: cmp r1,r2
  0016481c: it ne
  0016481e: cmp.ne r1,#0xe
  00164820: bne 0x0016484a
  00164822: adds r0,#0x54
  00164824: b 0x0016484c
  00164826: ldr r0,[r4,#0x54]
  00164828: movw r2,#0x37a3
  0016482c: ldr.w r1,[r4,#0xac]
  00164830: ldr r6,[r0,#0x74]
  00164832: cmp r1,r2
  00164834: it ne
  00164836: cmp.ne r1,#0xe
  00164838: bne.w 0x001649c8
  0016483c: adds r0,#0x54
  0016483e: b 0x001649ca
  00164840: mov r0,r4
  00164842: mov r1,r8
  00164844: blx 0x00076b88
  00164848: b 0x0016434e
  0016484a: adds r0,#0x50
  0016484c: ldr r5,[r0,#0x0]
  0016484e: ldr.w r0,[r4,#0x124]
  00164852: blx 0x000721cc
  00164856: mov r2,r0
  00164858: mov r0,r6
  0016485a: mov r1,r5
  0016485c: blx 0x00072d0c
  00164860: ldr r0,[0x001648e0]
  00164862: movs r3,#0x0
  00164864: ldr r1,[0x001648e4]
  00164866: add r0,pc
  00164868: add r1,pc
  0016486a: ldr r0,[r0,#0x0]
  0016486c: ldr r1,[r1,#0x0]
  0016486e: ldr r0,[r0,#0x0]
  00164870: ldrb r2,[r1,#0xf]
  00164872: movs r1,#0x0
  00164874: str r1,[sp,#0x0]
  00164876: movs r1,#0x14
  00164878: cmp r2,#0x0
  0016487a: it ne
  0016487c: add.ne.w r2,r4,#0x2c
  00164880: blx 0x00071548
  00164884: ldr r1,[r4,#0x54]
  00164886: movw r3,#0x37a3
  0016488a: ldr.w r2,[r4,#0xac]
  0016488e: ldr r0,[r1,#0x74]
  00164890: cmp r2,r3
  00164892: it ne
  00164894: cmp.ne r2,#0xe
  00164896: bne 0x0016489c
  00164898: adds r1,#0x54
  0016489a: b 0x0016489e
  0016489c: adds r1,#0x50
  0016489e: ldr r1,[r1,#0x0]
  001648a0: movs r2,#0x1
  001648a2: blx 0x000723c4
  001648a6: movs r0,#0x68
  001648a8: blx 0x0006eb24
  001648ac: mov r6,r0
  001648ae: movs r1,#0x0
  001648b0: mov.w r11,#0x0
  001648b4: blx 0x000727f0
  001648b8: mov r0,r6
  001648ba: str.w r6,[r4,#0x18c]
  001648be: blx 0x0007579c
  001648c2: ldr.w r5,[r4,#0x18c]
  001648c6: ldr.w r1,[r4,#0x124]
  001648ca: b 0x00164a44
  001648e8: adds r1,#0x50
  001648ea: ldr r1,[r1,#0x0]
  001648ec: movs r2,#0x0
  001648ee: movs r6,#0x0
  001648f0: blx 0x000723c4
  001648f4: ldr.w r0,[r4,#0x18c]
  001648f8: blx 0x00072c88
  001648fc: vmov.f32 s2,0x41000000
  00164900: ldr.w r1,[r4,#0xac]
  00164904: vmov.f32 s0,0x40c00000
  00164908: movw r2,#0x37e7
  0016490c: cmp r1,r2
  0016490e: ldr.w r0,[r4,#0x18c]
  00164912: movw r2,#0x37a3
  00164916: it eq
  00164918: vmov.eq.f32 s0,s2
  0016491c: cmp r1,r2
  0016491e: it eq
  00164920: vmov.eq.f32 s0,s2
  00164924: vmov r1,s0
  00164928: blx 0x000727fc
  0016492c: ldr.w r0,[r4,#0x18c]
  00164930: add.w r5,r4,#0x2c
  00164934: add r2,sp,#0x10
  00164936: strd r6,r6,[sp,#0x10]
  0016493a: mov r1,r5
  0016493c: str r6,[sp,#0x18]
  0016493e: blx 0x00072808
  00164942: ldr r0,[r4,#0x54]
  00164944: movs r1,#0x1
  00164946: str.w r1,[r4,#0x198]
  0016494a: str.w r6,[r4,#0x190]
  0016494e: blx 0x00072034
  00164952: cmp r0,#0x0
  00164954: beq.w 0x00164c4e
  00164958: ldr r0,[r4,#0x54]
  0016495a: blx 0x00072034
  0016495e: blx 0x0007588c
  00164962: cmp r0,#0x0
  00164964: beq.w 0x00164c4e
  00164968: ldr r0,[r4,#0x54]
  0016496a: blx 0x00072034
  0016496e: blx 0x0007588c
  00164972: blx 0x00076b28
  00164976: add r6,sp,#0x10
  00164978: mov r2,r0
  0016497a: mov r1,r5
  0016497c: mov r0,r6
  0016497e: blx 0x0006ec38
  00164982: mov r0,r6
  00164984: blx 0x0006ec44
  00164988: vldr.32 s0,[pc,#0x30c]
  0016498c: vmov s2,r0
  00164990: vcmpe.f32 s2,s0
  00164994: vmrs apsr,fpscr
  00164998: vmov.f32 s4,s0
  0016499c: it mi
  0016499e: vmov.mi.f32 s4,s2
  001649a2: vmov.f32 s2,0x3f800000
  001649a6: vdiv.f32 s0,s4,s0
  001649aa: ldr r0,[r4,#0x54]
  001649ac: vsub.f32 s0,s2,s0
  001649b0: vstr.32 s0,[r4,#0x19c]
  001649b4: blx 0x00072034
  001649b8: blx 0x0007588c
  001649bc: ldr.w r1,[r4,#0x19c]
  001649c0: movs r2,#0x32
  001649c2: blx 0x000728ec
  001649c6: b 0x00164c4e
  001649c8: adds r0,#0x50
  001649ca: ldr r5,[r0,#0x0]
  001649cc: ldr r0,[r4,#0x8]
  001649ce: blx 0x000721cc
  001649d2: mov r2,r0
  001649d4: mov r0,r6
  001649d6: mov r1,r5
  001649d8: blx 0x00072d0c
  001649dc: ldr r0,[0x00164c9c]
  001649de: movs r3,#0x0
  001649e0: ldr r1,[0x00164ca0]
  001649e2: add r0,pc
  001649e4: add r1,pc
  001649e6: ldr r0,[r0,#0x0]
  001649e8: ldr r1,[r1,#0x0]
  001649ea: ldr r0,[r0,#0x0]
  001649ec: ldrb r2,[r1,#0xf]
  001649ee: movs r1,#0x0
  001649f0: str r1,[sp,#0x0]
  001649f2: movs r1,#0x14
  001649f4: cmp r2,#0x0
  001649f6: it ne
  001649f8: add.ne.w r2,r4,#0x2c
  001649fc: blx 0x00071548
  00164a00: ldr r1,[r4,#0x54]
  00164a02: movw r3,#0x37a3
  00164a06: ldr.w r2,[r4,#0xac]
  00164a0a: ldr r0,[r1,#0x74]
  00164a0c: cmp r2,r3
  00164a0e: it ne
  00164a10: cmp.ne r2,#0xe
  00164a12: bne 0x00164a18
  00164a14: adds r1,#0x54
  00164a16: b 0x00164a1a
  00164a18: adds r1,#0x50
  00164a1a: ldr r1,[r1,#0x0]
  00164a1c: movs r2,#0x1
  00164a1e: blx 0x000723c4
  00164a22: movs r0,#0x68
  00164a24: blx 0x0006eb24
  00164a28: mov r6,r0
  00164a2a: movs r1,#0x0
  00164a2c: mov.w r11,#0x0
  00164a30: blx 0x000727f0
  00164a34: mov r0,r6
  00164a36: str.w r6,[r4,#0x18c]
  00164a3a: blx 0x0007579c
  00164a3e: ldr.w r5,[r4,#0x18c]
  00164a42: ldr r1,[r4,#0x8]
  00164a44: add r6,sp,#0x10
  00164a46: mov r0,r6
  00164a48: blx 0x000720b8
  00164a4c: add r2,sp,#0x4
  00164a4e: mov r0,r5
  00164a50: mov r1,r6
  00164a52: str.w r11,[sp,#0x8]
  00164a56: str.w r11,[sp,#0x4]
  00164a5a: str.w r11,[sp,#0xc]
  00164a5e: blx 0x00072808
  00164a62: ldr.w r0,[r9,#0x0]
  00164a66: cmp r0,#0xe
  00164a68: bne.w 0x001643b2
  00164a6c: ldr r6,[0x00164c90]
  00164a6e: movs r5,#0x0
  00164a70: b 0x00164a9a
  00164a72: ldr r0,[r4,#0x54]
  00164a74: blx 0x00071eb4
  00164a78: ldr r0,[r0,#0x4]
  00164a7a: ldr.w r0,[r0,r5,lsl #0x2]
  00164a7e: ldrb.w r0,[r0,#0x3e]
  00164a82: cbz r0,0x00164a98
  00164a84: ldr r0,[r4,#0x54]
  00164a86: blx 0x00071eb4
  00164a8a: ldr r0,[r0,#0x4]
  00164a8c: mov r1,r6
  00164a8e: ldr.w r0,[r0,r5,lsl #0x2]
  00164a92: ldr r0,[r0,#0x4]
  00164a94: blx 0x00072ab4
  00164a98: adds r5,#0x1
  00164a9a: ldr r0,[r4,#0x54]
  00164a9c: blx 0x00071eb4
  00164aa0: ldr r0,[r0,#0x0]
  00164aa2: cmp r5,r0
  00164aa4: bcc 0x00164a72
  00164aa6: ldr.w r0,[r9,#0x0]
  00164aaa: cmp r0,#0xe
  00164aac: bne.w 0x001643b2
  00164ab0: ldr r0,[r4,#0x4]
  00164ab2: ldrb.w r0,[r0,#0x44]
  00164ab6: cmp r0,#0x0
  00164ab8: bne.w 0x001643b2
  00164abc: ldr r0,[0x00164ca4]
  00164abe: movs r2,#0x1
  00164ac0: add r0,pc
  00164ac2: ldr r6,[r0,#0x0]
  00164ac4: ldr r0,[0x00164ca8]
  00164ac6: ldr r1,[r6,#0x0]
  00164ac8: add r0,pc
  00164aca: ldr r5,[r0,#0x0]
  00164acc: ldr.w r0,[r1,#0x118]
  00164ad0: adds r0,#0x1
  00164ad2: str.w r0,[r1,#0x118]
  00164ad6: ldr r0,[r5,#0x0]
  00164ad8: movs r1,#0x27
  00164ada: blx 0x00072ef8
  00164ade: cmp r0,#0x0
  00164ae0: bne.w 0x001643b2
  00164ae4: ldr r0,[r6,#0x0]
  00164ae6: movs r1,#0x27
  00164ae8: movs r2,#0x1
  00164aea: vldr.32 s0,[r0,#0x118]
  00164aee: ldr r0,[r5,#0x0]
  00164af0: vcvt.f32.s32 s16,s0
  00164af4: blx 0x00072f04
  00164af8: vmov s0,r0
  00164afc: vcvt.f32.s32 s0,s0
  00164b00: vdiv.f32 s0,s16,s0
  00164b04: vcvt.s32.f32 s0,s0
  00164b08: vmov r0,s0
  00164b0c: cmp r0,#0x1
  00164b0e: bgt.w 0x001643b2
  00164b12: ldr r0,[r4,#0x54]
  00164b14: blx 0x00072034
  00164b18: blx 0x00072f10
  00164b1c: mov r9,r0
  00164b1e: ldr r0,[r6,#0x0]
  00164b20: movs r1,#0x27
  00164b22: movs r2,#0x1
  00164b24: vldr.32 s0,[r0,#0x118]
  00164b28: ldr r0,[r5,#0x0]
  00164b2a: vcvt.f32.s32 s16,s0
  00164b2e: blx 0x00072f04
  00164b32: vmov s0,r0
  00164b36: vldr.32 s2,[pc,#0x154]
  00164b3a: mov r0,r9
  00164b3c: movs r1,#0x27
  00164b3e: vcvt.f32.s32 s0,s0
  00164b42: vdiv.f32 s0,s16,s0
  00164b46: vmul.f32 s0,s0,s2
  00164b4a: vcvt.s32.f32 s0,s0
  00164b4e: vmov r2,s0
  00164b52: blx 0x00072f1c
  00164b56: b.w 0x001643b2
  00164b5a: ldr r0,[r4,#0x4]
  00164b5c: mov r1,r5
  00164b5e: blx 0x00075874
  00164b62: add r5,sp,#0x10
  00164b64: mov r1,r0
  00164b66: str.w r0,[r4,#0x168]
  00164b6a: mov r0,r5
  00164b6c: blx 0x00072964
  00164b70: mov r0,r8
  00164b72: mov r1,r5
  00164b74: blx 0x0006eb3c
  00164b78: add.w r2,r4,#0x90
  00164b7c: add.w r3,r4,#0x144
  00164b80: ldmia r2,{r0,r1,r2}
  00164b82: stmia r3!,{r0,r1,r2}
  00164b84: b 0x00164476
  00164b86: adds r1,r2,#0x5
  00164b88: movw r0,#0x824d
  00164b8c: b 0x00164ba4
  00164b8e: adds r1,r2,#0x1
  00164b90: movw r0,#0x8249
  00164b94: b 0x00164ba4
  00164b96: adds r1,r2,#0x2
  00164b98: movw r0,#0x824a
  00164b9c: b 0x00164ba4
  00164b9e: adds r1,r2,#0x3
  00164ba0: movw r0,#0x824b
  00164ba4: ldr r2,[0x00164cac]
  00164ba6: uxth r1,r1
  00164ba8: str.w r0,[r4,#0x1a0]
  00164bac: add r2,pc
  00164bae: ldr r5,[r2,#0x0]
  00164bb0: add r2,sp,#0x10
  00164bb2: ldr r0,[r5,#0x0]
  00164bb4: blx 0x0006fc1c
  00164bb8: ldr.w r1,[r4,#0x124]
  00164bbc: ldr r0,[r5,#0x0]
  00164bbe: ldrh.w r2,[sp,#0x10]
  00164bc2: ldr r1,[r1,#0x1c]
  00164bc4: blx 0x0007243c
  00164bc8: ldr r0,[r4,#0x54]
  00164bca: blx 0x00072034
  00164bce: cbz r0,0x00164c4e
  00164bd0: ldr r0,[r4,#0x54]
  00164bd2: blx 0x00072034
  00164bd6: blx 0x0007588c
  00164bda: cbz r0,0x00164c4e
  00164bdc: ldr.w r0,[r4,#0x198]
  00164be0: cmp r0,#0x1
  00164be2: blt 0x00164c4e
  00164be4: add.w r1,r0,r8
  00164be8: ldr r0,[r4,#0x54]
  00164bea: cmp.w r1,#0x7d0
  00164bee: it ge
  00164bf0: mov.ge.w r1,#0x7d0
  00164bf4: str.w r1,[r4,#0x198]
  00164bf8: blx 0x00072034
  00164bfc: blx 0x0007588c
  00164c00: vldr.32 s0,[r4,#0x198]
  00164c04: movs r2,#0x32
  00164c06: vldr.32 s2,[r4,#0x19c]
  00164c0a: vcvt.f32.s32 s0,s0
  00164c0e: vldr.32 s4,[pc,#0x84]
  00164c12: vdiv.f32 s0,s0,s4
  00164c16: vmov.f32 s4,0x3f800000
  00164c1a: vadd.f32 s0,s0,s4
  00164c1e: vmul.f32 s0,s2,s0
  00164c22: vmov r1,s0
  00164c26: blx 0x000728ec
  00164c2a: ldr.w r0,[r4,#0x18c]
  00164c2e: cbz r0,0x00164c4e
  00164c30: blx 0x000757fc
  00164c34: cbnz r0,0x00164c4e
  00164c36: ldr r0,[r4,#0x54]
  00164c38: blx 0x00072034
  00164c3c: blx 0x0007588c
  00164c40: movs r1,#0x0
  00164c42: movs r2,#0x0
  00164c44: movs r5,#0x0
  00164c46: blx 0x000728ec
  00164c4a: str.w r5,[r4,#0x198]
  00164c4e: ldr r0,[r4,#0x4]
  00164c50: ldr.w r1,[r4,#0x178]
  00164c54: str r1,[r0,#0x48]
  00164c56: ldr.w r1,[r4,#0x17c]
  00164c5a: str r1,[r0,#0x4c]
  00164c5c: ldr.w r1,[r4,#0x180]
  00164c60: str r1,[r0,#0x50]
  00164c62: ldr r0,[sp,#0x1c]
  00164c64: ldr.w r1,[r10,#0x0]
  00164c68: subs r0,r1,r0
  00164c6a: itttt eq
  00164c6c: vpop.eq {d4,d5,d6,d7,d8,d9}
  00164c70: add.eq sp,#0x4
  00164c72: pop.eq.w {r8,r9,r10,r11}
  00164c76: pop.eq {r4,r5,r6,r7,pc}
  00164c78: blx 0x0006e824
```
