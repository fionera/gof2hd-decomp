# Level::assignGuns
elf 0xbc680 body 2814
Sig: undefined __thiscall assignGuns(Level * this)

## decompile
```c

/* Level::assignGuns() */

void __thiscall Level::assignGuns(Level *this)

{
  Level *pLVar1;
  byte bVar2;
  float fVar3;
  int iVar4;
  void *pvVar5;
  int iVar6;
  Wanted *this_00;
  Array<AbstractGun*> *this_01;
  int iVar7;
  int iVar8;
  Gun *pGVar9;
  PlayerTurret *this_02;
  int iVar10;
  KIPlayer *this_03;
  Mission *this_04;
  RocketGun *pRVar11;
  ObjectGun *pOVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  uint *puVar15;
  int iVar16;
  undefined4 *puVar17;
  uint uVar18;
  Player *this_05;
  int iVar19;
  uint uVar20;
  code *pcVar21;
  int *piVar22;
  int *piVar23;
  uint in_fpscr;
  uint uVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  int iVar29;
  double dVar30;
  int local_88;
  int local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  int local_6c;
  
  piVar23 = *(int **)(DAT_000cca28 + 0xcc696);
  local_6c = *piVar23;
  if (*(Array **)(this + 0xe8) != (Array *)0x0) {
    ArrayReleaseClasses<AbstractGun*>(*(Array **)(this + 0xe8));
    if (*(Array<AbstractGun*> **)(this + 0xe8) != (Array<AbstractGun*> *)0x0) {
      pvVar5 = (void *)Array<AbstractGun*>::~Array(*(Array<AbstractGun*> **)(this + 0xe8));
      operator_delete(pvVar5);
    }
    *(undefined4 *)(this + 0xe8) = 0;
  }
  puVar17 = *(undefined4 **)(DAT_000cca38 + 0xcc6c2);
  *(undefined4 *)(this + 0xe8) = 0;
  iVar6 = Status::getLevel();
  fVar28 = DAT_000cca3c;
  fVar25 = (float)VectorSignedToFloat(iVar6 + -2,(byte)(in_fpscr >> 0x16) & 3);
  fVar25 = fVar25 * DAT_000cca3c;
  uVar18 = in_fpscr & 0xfffffff | (uint)(fVar25 < 20.0) << 0x1f | (uint)(fVar25 == 20.0) << 0x1e;
  uVar24 = uVar18 | (uint)NAN(fVar25) << 0x1c;
  bVar2 = (byte)(uVar18 >> 0x18);
  if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar24 >> 0x1c) & 1)) {
    iVar6 = Status::getLevel();
    fVar26 = (float)VectorSignedToFloat(iVar6 + -2,(byte)(uVar24 >> 0x16) & 3);
    uVar24 = uVar24 & 0xfffffff;
    fVar25 = DAT_000cca40;
    if (fVar26 * fVar28 < 0.0) goto LAB_000cc74c;
  }
  iVar6 = Status::getLevel();
  fVar25 = (float)VectorSignedToFloat(iVar6 + -2,(byte)(uVar24 >> 0x16) & 3);
  fVar25 = fVar25 * fVar28;
  uVar18 = uVar24 & 0xfffffff | (uint)(fVar25 < 20.0) << 0x1f | (uint)(fVar25 == 20.0) << 0x1e;
  uVar24 = uVar18 | (uint)NAN(fVar25) << 0x1c;
  bVar2 = (byte)(uVar18 >> 0x18);
  fVar25 = 20.0;
  if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar24 >> 0x1c) & 1)) {
    iVar6 = Status::getLevel();
    fVar25 = (float)VectorSignedToFloat(iVar6 + -2,(byte)(uVar24 >> 0x16) & 3);
    fVar25 = (float)VectorSignedToFloat((int)(fVar25 * fVar28),(byte)(uVar24 >> 0x16) & 3);
  }
LAB_000cc74c:
  fVar28 = *(float *)(*(int *)(DAT_000cca44 + 0xcc756) + 0x2c);
  iVar6 = Status::getCurrentCampaignMission();
  iVar29 = (int)(fVar25 + fVar25 * (fVar28 + -0.5));
  this_00 = (Wanted *)Status::getWantedInCurrentOrbit((Status *)*puVar17);
  pLVar1 = this + 0xf8;
  puVar15 = *(uint **)pLVar1;
  if (0x15 < iVar29) {
    iVar29 = 0x16;
  }
  if (puVar15 != (uint *)0x0) {
    uVar20 = 0;
    for (uVar18 = 0; uVar18 < *puVar15; uVar18 = uVar18 + 1) {
      iVar16 = *(int *)(puVar15[1] + uVar18 * 4);
      if ((iVar16 != 0) && (*(char *)(iVar16 + 0x25) != '\0')) {
        iVar16 = KIPlayer::isWingMan();
        iVar7 = 1;
        if (iVar16 != 0) {
          iVar7 = 2;
        }
        puVar15 = *(uint **)pLVar1;
        uVar20 = uVar20 + iVar7;
      }
    }
    this_01 = operator_new(0xc);
    Array<AbstractGun*>::Array(this_01);
    *(Array<AbstractGun*> **)(this + 0xe8) = this_01;
    ArraySetLength<AbstractGun*>(uVar20,(Array *)this_01);
    fVar3 = DAT_000cca54;
    fVar26 = DAT_000cca50;
    fVar25 = DAT_000cca4c;
    fVar28 = DAT_000cca48;
    iVar16 = iVar29 + 2;
    if (iVar29 == 0) {
      iVar16 = 3;
    }
    iVar29 = iVar16;
    if (iVar6 == 4) {
      iVar29 = 1;
    }
    local_7c = 0;
    for (uVar18 = 0; uVar18 < **(uint **)pLVar1; uVar18 = uVar18 + 1) {
      iVar7 = *(int *)((*(uint **)pLVar1)[1] + uVar18 * 4);
      if (iVar7 != 0) {
        if (*(char *)(iVar7 + 0x25) != '\0') {
          if (*(int *)(this + 0xc0) == 2) {
            Player::setPlayShootSound(*(Player **)(iVar7 + 4),false,2);
          }
          Status::getMission();
          iVar7 = Mission::getType();
          if ((iVar7 == 6) && (iVar7 = Player::isAlwaysFriend(), iVar7 == 0)) {
LAB_000cc8ba:
            iVar7 = Status::getLevel();
            uVar13 = 0x41e00000;
            iVar7 = iVar7 + iVar16;
          }
          else {
            Status::getMission();
            iVar8 = Mission::getType();
            uVar13 = 0x41800000;
            iVar7 = iVar29;
            if (iVar8 == 0xc) {
              iVar8 = Player::isAlwaysFriend();
              uVar13 = 0x41800000;
              if (iVar8 != 0) goto LAB_000cc8ba;
            }
          }
          iVar8 = KIPlayer::isWingMan();
          if (((iVar8 == 0) && (iVar8 = Player::isAlwaysFriend(), iVar8 == 0)) &&
             (*(int *)(*(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4) + 0x28) == 9)) {
            fVar27 = (float)VectorSignedToFloat(iVar7,(byte)(uVar24 >> 0x16) & 3);
            if (iVar6 != 0x10) {
              iVar7 = (int)(fVar27 * fVar3);
              goto LAB_000cc922;
            }
            iVar8 = (int)(fVar27 + fVar27);
          }
          else {
LAB_000cc922:
            iVar8 = 5;
            if ((0x8fU >> (iVar6 - 0x31U & 0xff) & 1) == 0) {
              iVar8 = iVar7;
            }
            if (7 < iVar6 - 0x31U) {
              iVar8 = iVar7;
            }
          }
          if ((iVar6 == 0x50) &&
             (*(char *)(*(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4) + 0x3e) != '\0')) {
            fVar27 = (float)VectorSignedToFloat(iVar8,(byte)(uVar24 >> 0x16) & 3);
            iVar8 = (int)(fVar27 * fVar28);
          }
          iVar7 = Status::getCurrentCampaignMission();
          if ((iVar7 == 0x46) && (iVar7 = KIPlayer::isWingMan(), iVar7 == 0)) {
            fVar27 = (float)VectorSignedToFloat(iVar8,(byte)(uVar24 >> 0x16) & 3);
            iVar8 = (int)(fVar27 * 2.5);
          }
          iVar7 = Status::getMission();
          if (iVar7 != 0) {
            Status::getMission();
            iVar7 = Mission::getType();
            if (iVar7 == 0xb7) {
              iVar8 = 1;
            }
          }
          pGVar9 = operator_new(0x114);
          iVar7 = Status::gameWon();
          if (iVar7 == 0) {
            iVar7 = Status::getCurrentCampaignMission();
          }
          else {
            iVar7 = 0x2d;
          }
          Gun::Gun(pGVar9,0,iVar8,4,0xffffffff,3000,iVar7 * -2 + 600,uVar13,0,0,0,0,0,0);
          Gun::setFriendGun(pGVar9,true);
          Gun::setLevel(pGVar9,this);
          Gun::setIndex(pGVar9,0);
          *(undefined4 *)(pGVar9 + 0x5c) = 0;
          switch(*(undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4) + 0x28))
          {
          case 0:
            *(undefined4 *)(pGVar9 + 0x5c) = 0;
            Gun::setIndex(pGVar9,0);
            local_88 = 0x1a62;
            break;
          case 1:
            Gun::setIndex(pGVar9,3);
            local_88 = 0x1a68;
            break;
          case 2:
            *(undefined4 *)(pGVar9 + 0x5c) = 0;
            Gun::setIndex(pGVar9,7);
            local_88 = 0x1a6c;
            break;
          case 3:
            Gun::setIndex(pGVar9,0x19);
            local_88 = 0x1a92;
            break;
          default:
            *(undefined4 *)(pGVar9 + 0x5c) = 1;
            Gun::setIndex(pGVar9,0x13);
            local_88 = 0x1a8b;
            break;
          case 9:
            Gun::setIndex(pGVar9,5);
            local_88 = 0x1a6a;
            break;
          case 10:
            Gun::setIndex(pGVar9,0xe5);
            local_88 = 0x4a93;
            fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(pGVar9 + 0x60),
                                                (byte)(uVar24 >> 0x16) & 3);
            *(int *)(pGVar9 + 0x60) = (int)(fVar27 * fVar25);
          }
          iVar7 = Status::getCurrentCampaignMission();
          this_02 = *(PlayerTurret **)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4);
          if (this_02[0x3e] == (PlayerTurret)0x0) {
LAB_000cccdc:
            if (iVar7 == 7) {
              if (*(int *)(*(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4) + 0x28) == 8) {
                fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(pGVar9 + 0x60),
                                                    (byte)(uVar24 >> 0x16) & 3);
                *(int *)(pGVar9 + 0x60) = (int)(fVar27 * 0.5);
              }
            }
            else if ((iVar7 == 0x46) && (iVar10 = KIPlayer::isWingMan(), iVar10 == 0)) {
              Gun::setIndex(pGVar9,0xb7);
              local_88 = 0x37d9;
            }
          }
          else {
            iVar10 = PlayerTurret::getHost(this_02);
            if ((iVar10 != 0) &&
               (*(int *)(iVar10 + 0x7c) == 0x2d || *(int *)(iVar10 + 0x7c) == 0x33)) {
              *(undefined4 *)(pGVar9 + 0x5c) = 2;
              Gun::setIndex(pGVar9,0x16);
              local_88 = 0x1a8e;
              dVar30 = (double)VectorSignedToFloat(*(undefined4 *)(pGVar9 + 0x60),
                                                   (byte)(uVar24 >> 0x16) & 3);
              *(int *)(pGVar9 + 0x60) = (int)(longlong)(dVar30 * 0.5);
              goto LAB_000cccdc;
            }
            this_03 = *(KIPlayer **)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4);
            if (this_03[0x3f] == (KIPlayer)0x0) {
              iVar10 = *(int *)(this_03 + 0x28);
              *(undefined4 *)(pGVar9 + 0x5c) = 1;
              if (iVar10 == 1) {
                Gun::setIndex(pGVar9,0xf);
                local_88 = 0x1a87;
              }
              else {
                Gun::setIndex(pGVar9,0x14);
                local_88 = 0x1a8c;
              }
              goto LAB_000cccdc;
            }
            iVar10 = KIPlayer::getType(this_03);
            if (iVar10 == 0x49c1) {
              *(undefined4 *)(pGVar9 + 0x5c) = 1;
              Gun::setIndex(pGVar9,0x14);
              local_78 = 0;
              uStack_74 = 0;
              local_70 = DAT_000cce30;
              AbyssEngine::AEMath::Vector::operator=((Vector *)(pGVar9 + 0x7c),(Vector *)&local_78);
              iVar19 = 0xd4;
              local_88 = 0x1a8d;
            }
            else if (iVar10 == 0x49c0) {
              *(undefined4 *)(pGVar9 + 0x5c) = 0;
              Gun::setIndex(pGVar9,2);
              local_78 = 0;
              uStack_74 = 0;
              local_70 = DAT_000cce30;
              AbyssEngine::AEMath::Vector::operator=((Vector *)(pGVar9 + 0x7c),(Vector *)&local_78);
              iVar19 = 0xd3;
              local_88 = 0x1a64;
            }
            else {
              *(undefined4 *)(pGVar9 + 0x5c) = 1;
              Gun::setIndex(pGVar9,0xe);
              local_78 = 0;
              uStack_74 = 0;
              local_70 = DAT_000cce34;
              AbyssEngine::AEMath::Vector::operator=((Vector *)(pGVar9 + 0x7c),(Vector *)&local_78);
              iVar19 = 0xd5;
              local_88 = 0x1a86;
            }
            iVar4 = DAT_000cce38;
            pGVar9[0xa8] = (Gun)0x1;
            piVar22 = *(int **)(iVar4 + 0xccbea);
            pcVar21 = *(code **)(DAT_000cce3c + 0xccbf8);
            uVar14 = (*pcVar21)(*(undefined4 *)(*(int *)(*piVar22 + 4) + iVar19 * 4),9);
            *(undefined4 *)(pGVar9 + 0x60) = uVar14;
            uVar14 = (*pcVar21)(*(undefined4 *)(*(int *)(*piVar22 + 4) + iVar19 * 4),0xb);
            *(undefined4 *)(pGVar9 + 0x48) = uVar14;
            uVar14 = (*pcVar21)(*(undefined4 *)(*(int *)(*piVar22 + 4) + iVar19 * 4),0xc);
            *(undefined4 *)(pGVar9 + 0x44) = uVar14;
            uVar14 = (*pcVar21)(*(undefined4 *)(*(int *)(*piVar22 + 4) + iVar19 * 4),0xd);
            uVar14 = VectorSignedToFloat(uVar14,(byte)(uVar24 >> 0x16) & 3);
            *(undefined4 *)(pGVar9 + 0x50) = uVar14;
            if ((iVar7 != 0x9e) || (iVar10 != 0x49c2)) goto LAB_000cccdc;
            iVar10 = Player::isAlwaysEnemy();
            if (iVar10 != 0) {
              fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(pGVar9 + 0x60),
                                                  (byte)(uVar24 >> 0x16) & 3);
              *(float *)(pGVar9 + 0x50) = *(float *)(pGVar9 + 0x50) * fVar26;
              *(int *)(pGVar9 + 0x60) = (int)(fVar27 * 1.5);
              this_05 = *(Player **)(*(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4) + 4);
              uVar14 = Player::getMaxHitpoints();
              fVar27 = (float)VectorSignedToFloat(uVar14,(byte)(uVar24 >> 0x16) & 3);
              Player::setMaxHitpoints(this_05,(int)(fVar27 * 5.0));
            }
          }
          iVar10 = Status::getMission();
          if (iVar10 == 0) {
LAB_000ccd80:
            iVar10 = *(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4);
            iVar19 = *(int *)(iVar10 + 0x7c);
            if (iVar7 == 0x91 && iVar19 == 0x31) {
              *(undefined4 *)(pGVar9 + 0x5c) = 0x28;
              Gun::setIndex(pGVar9,0xd6);
              *(int *)(pGVar9 + 0x60) = *(int *)(pGVar9 + 0x60) << 1;
              local_88 = 0x37a0;
            }
            else if ((iVar7 - 0x9dU < 2) && (iVar19 == 0x31)) {
              *(undefined4 *)(pGVar9 + 0x5c) = 0;
              Gun::setIndex(pGVar9,7);
              *(int *)(pGVar9 + 0x60) = *(int *)(pGVar9 + 0x60) * 3;
              local_88 = 0x1a6c;
            }
            else if ((this_00 != (Wanted *)0x0) && (*(char *)(iVar10 + 0x42) != '\0')) {
              iVar10 = Wanted::getWeapon(this_00);
              Gun::setIndex(pGVar9,iVar10);
              uVar14 = Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_000cd1f0 + 0xcd18a) + 4)
                                                  + iVar10 * 4));
              local_88 = *(int *)(DAT_000cd1f4 + 0xcd19e + iVar10 * 4);
              *(undefined4 *)(pGVar9 + 0x5c) = uVar14;
              *(int *)(pGVar9 + 0x60) = *(int *)(pGVar9 + 0x60) << 2;
            }
          }
          else {
            this_04 = (Mission *)Status::getMission();
            iVar10 = Mission::isCampaignMission(this_04);
            if (iVar10 == 0) goto LAB_000ccd80;
            iVar19 = **(int **)(DAT_000cce40 + 0xccd42);
            iVar10 = Status::getCurrentCampaignMission();
            if (((iVar19 == iVar10) && (2 < **(int **)(DAT_000cce44 + 0xccd54))) &&
               (iVar10 = KIPlayer::isEnemy(*(KIPlayer **)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4)
                                          ), iVar10 != 0)) {
              fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(pGVar9 + 0x60),
                                                  (byte)(uVar24 >> 0x16) & 3);
              *(int *)(pGVar9 + 0x60) = (int)(fVar27 * fVar25);
            }
          }
          iVar10 = *(int *)(pGVar9 + 0x5c);
          if (iVar10 == 0x28 || iVar10 == 5) {
            pRVar11 = operator_new(0xe8);
            RocketGun::RocketGun
                      (pRVar11,*(int *)(pGVar9 + 0x58),pGVar9,local_88,0,0,iVar10,iVar10 == 5,this);
            *(RocketGun **)(*(int *)(*(int *)(this + 0xe8) + 4) + local_7c * 4) = pRVar11;
            *(undefined4 *)(pGVar9 + 0x50) = 0x41000000;
            *(undefined4 *)(pGVar9 + 0x44) = 10000;
            *(undefined4 *)(pGVar9 + 0x48) = 3000;
            *(int *)(pGVar9 + 0x60) = *(int *)(pGVar9 + 0x60) << 2;
          }
          else {
            pOVar12 = operator_new(0xb0);
            ObjectGun::ObjectGun(pOVar12,0,pGVar9,local_88,0x2711,this);
            *(ObjectGun **)(*(int *)(*(int *)(this + 0xe8) + 4) + local_7c * 4) = pOVar12;
          }
          KIPlayer::addGun(*(Gun **)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4),(int)pGVar9);
          piVar22 = *(int **)(DAT_000cd1f8 + 0xcce8e);
          Globals::addSoundResourceToList(*piVar22);
          iVar10 = *(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4);
          if (((this_00 != (Wanted *)0x0) && (*(char *)(iVar10 + 0x42) != '\0')) &&
             (*(int *)(iVar10 + 0x7c) - 0x2dU < 4)) {
            pGVar9 = operator_new(0x114);
            Gun::Gun(pGVar9,0,iVar8 << 2,4,0xffffffff,10000,3000,0x41000000,0,0,0,0,0,0);
            Gun::setFriendGun(pGVar9,true);
            Gun::setLevel(pGVar9,this);
            *(undefined4 *)(pGVar9 + 0x5c) = 4;
            Gun::setIndex(pGVar9,0x1f);
            pRVar11 = operator_new(0xe8);
            RocketGun::RocketGun
                      (pRVar11,*(int *)(pGVar9 + 0x58),pGVar9,0x37a0,0,0,*(int *)(pGVar9 + 0x5c),
                       false,this);
            ArrayAdd<AbstractGun*>((AbstractGun *)pRVar11,*(Array **)(this + 0xe8));
            KIPlayer::addGun(*(Gun **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4),(int)pGVar9
                            );
            Globals::addSoundResourceToList(*piVar22);
            iVar10 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4);
          }
          if ((iVar7 - 0x9dU < 2) && (*(int *)(iVar10 + 0x7c) == 0x31)) {
            pGVar9 = operator_new(0x114);
            iVar7 = Status::gameWon();
            if (iVar7 == 0) {
              iVar7 = Status::getCurrentCampaignMission();
            }
            else {
              iVar7 = 0x2d;
            }
            Gun::Gun(pGVar9,0,iVar8,4,0xffffffff,3000,iVar7 * -2 + 600,uVar13,0,0,0,0,0,0);
            Gun::setFriendGun(pGVar9,true);
            Gun::setLevel(pGVar9,this);
            *(undefined4 *)(pGVar9 + 0x5c) = 0x28;
            Gun::setIndex(pGVar9,0xd6);
            pRVar11 = operator_new(0xe8);
            RocketGun::RocketGun
                      (pRVar11,*(int *)(pGVar9 + 0x58),pGVar9,0x37a0,0,0,*(int *)(pGVar9 + 0x5c),
                       *(int *)(pGVar9 + 0x5c) == 5,this);
            ArrayAdd<AbstractGun*>((AbstractGun *)pRVar11,*(Array **)(this + 0xe8));
            *(undefined4 *)(pGVar9 + 0x50) = 0x41000000;
            *(undefined4 *)(pGVar9 + 0x44) = 10000;
            *(undefined4 *)(pGVar9 + 0x48) = 3000;
            *(int *)(pGVar9 + 0x60) = *(int *)(pGVar9 + 0x60) << 2;
            KIPlayer::addGun(*(Gun **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4),(int)pGVar9
                            );
            Globals::addSoundResourceToList(*piVar22);
            iVar10 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4);
          }
          local_7c = local_7c + 1;
          if (iVar10 == 0) goto LAB_000cd158;
        }
        iVar7 = KIPlayer::isWingMan();
        if ((iVar7 != 0) &&
           (*(char *)(*(int *)(*(int *)(*(int *)pLVar1 + 4) + uVar18 * 4) + 0x25) != '\0')) {
          pGVar9 = operator_new(0x114);
          Gun::Gun(pGVar9,0x12,0,4,0xffffffff,3000,400,0x41800000,0,0,0,0,0,0);
          Gun::setFriendGun(pGVar9,true);
          Gun::setLevel(pGVar9,this);
          *(undefined4 *)(pGVar9 + 0x58) = 0x12;
          *(undefined4 *)(pGVar9 + 0x5c) = 1;
          pOVar12 = operator_new(0xb0);
          ObjectGun::ObjectGun(pOVar12,0x12,pGVar9,0x1a8a,0x2711,this);
          *(ObjectGun **)(*(int *)(*(int *)(this + 0xe8) + 4) + local_7c * 4) = pOVar12;
          Gun::setIndex(pGVar9,0x12);
          uVar13 = Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_000cd1fc + 0xcd126) + 4) +
                                              0x48));
          *(undefined4 *)(pGVar9 + 100) = uVar13;
          KIPlayer::addGun(*(Gun **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar18 * 4),(int)pGVar9);
          Globals::addSoundResourceToList(**(int **)(DAT_000cd200 + 0xcd14c));
          local_7c = local_7c + 1;
        }
      }
LAB_000cd158:
    }
  }
  if (*piVar23 != local_6c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000cc680: push {r4,r5,r6,r7,lr}
  000cc682: add r7,sp,#0xc
  000cc684: push {r7,r8,r9,r10,r11}
  000cc688: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  000cc68c: sub sp,#0x70
  000cc68e: mov r8,r0
  000cc690: ldr r0,[0x000cca28]
  000cc692: add r0,pc
  000cc694: ldr.w r9,[r0,#0x0]
  000cc698: ldr.w r0,[r9,#0x0]
  000cc69c: str r0,[sp,#0x6c]
  000cc69e: ldr.w r0,[r8,#0xe8]
  000cc6a2: cbz r0,0x000cc6bc
  000cc6a4: blx 0x00073b70
  000cc6a8: ldr.w r0,[r8,#0xe8]
  000cc6ac: cbz r0,0x000cc6b6
  000cc6ae: blx 0x00073b7c
  000cc6b2: blx 0x0006eb48
  000cc6b6: movs r0,#0x0
  000cc6b8: str.w r0,[r8,#0xe8]
  000cc6bc: ldr r0,[0x000cca38]
  000cc6be: add r0,pc
  000cc6c0: ldr r4,[r0,#0x0]
  000cc6c2: movs r0,#0x0
  000cc6c4: str.w r0,[r8,#0xe8]
  000cc6c8: ldr r0,[r4,#0x0]
  000cc6ca: blx 0x00071c5c
  000cc6ce: subs r0,#0x2
  000cc6d0: vldr.32 s18,[pc,#0x368]
  000cc6d4: vmov.f32 s16,0x41a00000
  000cc6d8: vmov s0,r0
  000cc6dc: vcvt.f32.s32 s0,s0
  000cc6e0: vmul.f32 s0,s0,s18
  000cc6e4: vcmpe.f32 s0,s16
  000cc6e8: vmrs apsr,fpscr
  000cc6ec: bgt 0x000cc712
  000cc6ee: ldr r0,[r4,#0x0]
  000cc6f0: blx 0x00071c5c
  000cc6f4: subs r0,#0x2
  000cc6f6: vmov s0,r0
  000cc6fa: vcvt.f32.s32 s0,s0
  000cc6fe: vmul.f32 s0,s0,s18
  000cc702: vcmpe.f32 s0,#0
  000cc706: vmrs apsr,fpscr
  000cc70a: bpl 0x000cc712
  000cc70c: vldr.32 s16,[pc,#0x330]
  000cc710: b 0x000cc74c
  000cc712: ldr r0,[r4,#0x0]
  000cc714: blx 0x00071c5c
  000cc718: subs r0,#0x2
  000cc71a: vmov s0,r0
  000cc71e: vcvt.f32.s32 s0,s0
  000cc722: vmul.f32 s0,s0,s18
  000cc726: vcmpe.f32 s0,s16
  000cc72a: vmrs apsr,fpscr
  000cc72e: bgt 0x000cc74c
  000cc730: ldr r0,[r4,#0x0]
  000cc732: blx 0x00071c5c
  000cc736: subs r0,#0x2
  000cc738: vmov s0,r0
  000cc73c: vcvt.f32.s32 s0,s0
  000cc740: vmul.f32 s0,s0,s18
  000cc744: vcvt.s32.f32 s0,s0
  000cc748: vcvt.f32.s32 s16,s0
  000cc74c: ldr r0,[0x000cca44]
  000cc74e: vmov.f32 s0,0xbf000000
  000cc752: add r0,pc
  000cc754: ldr r0,[r0,#0x0]
  000cc756: vldr.32 s2,[r0,#0x2c]
  000cc75a: ldr r0,[r4,#0x0]
  000cc75c: vadd.f32 s0,s2,s0
  000cc760: vmla.f32 s16,s16,s0
  000cc764: blx 0x00071770
  000cc768: str r0,[sp,#0x44]
  000cc76a: ldr r0,[r4,#0x0]
  000cc76c: str r4,[sp,#0x58]
  000cc76e: vcvt.s32.f32 s16,s16
  000cc772: blx 0x00073f18
  000cc776: vmov r6,s16
  000cc77a: mov r10,r8
  000cc77c: ldr.w r1,[r10,#0xf8]!
  000cc780: str r0,[sp,#0x40]
  000cc782: cmp r6,#0x16
  000cc784: it ge
  000cc786: mov.ge r6,#0x16
  000cc788: cmp r1,#0x0
  000cc78a: beq.w 0x000cd1aa
  000cc78e: movs r4,#0x0
  000cc790: movs r5,#0x0
  000cc792: b 0x000cc7b8
  000cc794: ldr r0,[r1,#0x4]
  000cc796: ldr.w r0,[r0,r4,lsl #0x2]
  000cc79a: cbz r0,0x000cc7b6
  000cc79c: ldrb.w r2,[r0,#0x25]
  000cc7a0: cbz r2,0x000cc7b6
  000cc7a2: blx 0x00072ed4
  000cc7a6: cmp r0,#0x0
  000cc7a8: mov.w r0,#0x1
  000cc7ac: it ne
  000cc7ae: mov.ne r0,#0x2
  000cc7b0: ldr.w r1,[r10,#0x0]
  000cc7b4: add r5,r0
  000cc7b6: adds r4,#0x1
  000cc7b8: ldr r0,[r1,#0x0]
  000cc7ba: cmp r4,r0
  000cc7bc: bcc 0x000cc794
  000cc7be: movs r0,#0xc
  000cc7c0: blx 0x0006eb24
  000cc7c4: mov r4,r0
  000cc7c6: blx 0x000741f4
  000cc7ca: mov r0,r5
  000cc7cc: mov r1,r4
  000cc7ce: str.w r4,[r8,#0xe8]
  000cc7d2: blx 0x00074200
  000cc7d6: adds r1,r6,#0x2
  000cc7d8: cmp r6,#0x0
  000cc7da: it eq
  000cc7dc: mov.eq r1,#0x3
  000cc7de: ldr r0,[sp,#0x44]
  000cc7e0: vmov.f32 s16,0x41800000
  000cc7e4: str r1,[sp,#0x30]
  000cc7e6: vmov.f32 s18,0x41e00000
  000cc7ea: cmp r0,#0x4
  000cc7ec: vmov.f32 s22,0x40200000
  000cc7f0: sub.w r0,r0,#0x31
  000cc7f4: vmov.f32 s24,0x3f000000
  000cc7f8: it eq
  000cc7fa: mov.eq r1,#0x1
  000cc7fc: vmov.f32 s19,0x3fc00000
  000cc800: str r1,[sp,#0x3c]
  000cc802: vmov.f32 s17,0x40a00000
  000cc806: str r0,[sp,#0x38]
  000cc808: uxtb r0,r0
  000cc80a: movs r1,#0x8f
  000cc80c: vmov.f64 d14,0x3fe0000000000000
  000cc810: lsr.w r0,r1,r0
  000cc814: and r0,r0,#0x1
  000cc818: str r0,[sp,#0x34]
  000cc81a: movs r0,#0x0
  000cc81c: movs r4,#0x0
  000cc81e: mov.w r11,#0x0
  000cc822: str r0,[sp,#0x5c]
  000cc824: vldr.32 s20,[pc,#0x220]
  000cc828: vldr.32 s26,[pc,#0x220]
  000cc82c: vldr.32 s30,[pc,#0x220]
  000cc830: vldr.32 s21,[pc,#0x220]
  000cc834: str.w r8,[sp,#0x48]
  000cc838: ldr.w r0,[r10,#0x0]
  000cc83c: ldr r1,[r0,#0x0]
  000cc83e: cmp r11,r1
  000cc840: bcs.w 0x000cd1aa
  000cc844: ldr r0,[r0,#0x4]
  000cc846: ldr.w r0,[r0,r11,lsl #0x2]
  000cc84a: cmp r0,#0x0
  000cc84c: beq.w 0x000cd158
  000cc850: ldrb.w r1,[r0,#0x25]
  000cc854: ldr r6,[sp,#0x58]
  000cc856: cmp r1,#0x0
  000cc858: beq.w 0x000cd080
  000cc85c: ldr.w r1,[r8,#0xc0]
  000cc860: cmp r1,#0x2
  000cc862: bne 0x000cc86e
  000cc864: ldr r0,[r0,#0x4]
  000cc866: movs r1,#0x0
  000cc868: movs r2,#0x2
  000cc86a: blx 0x00072244
  000cc86e: ldr r0,[r6,#0x0]
  000cc870: blx 0x00072850
  000cc874: blx 0x00072874
  000cc878: cmp r0,#0x6
  000cc87a: bne 0x000cc88e
  000cc87c: ldr.w r0,[r10,#0x0]
  000cc880: ldr r0,[r0,#0x4]
  000cc882: ldr.w r0,[r0,r11,lsl #0x2]
  000cc886: ldr r0,[r0,#0x4]
  000cc888: blx 0x0007420c
  000cc88c: cbz r0,0x000cc8ba
  000cc88e: ldr r0,[r6,#0x0]
  000cc890: blx 0x00072850
  000cc894: blx 0x00072874
  000cc898: vmov.f32 s23,s16
  000cc89c: ldr r4,[sp,#0x3c]
  000cc89e: cmp r0,#0xc
  000cc8a0: bne 0x000cc8c8
  000cc8a2: ldr.w r0,[r10,#0x0]
  000cc8a6: ldr r0,[r0,#0x4]
  000cc8a8: ldr.w r0,[r0,r11,lsl #0x2]
  000cc8ac: ldr r0,[r0,#0x4]
  000cc8ae: blx 0x0007420c
  000cc8b2: vmov.f32 s23,s16
  000cc8b6: ldr r4,[sp,#0x3c]
  000cc8b8: cbz r0,0x000cc8c8
  000cc8ba: ldr r0,[r6,#0x0]
  000cc8bc: blx 0x00071c5c
  000cc8c0: vmov.f32 s23,s18
  000cc8c4: ldr r1,[sp,#0x30]
  000cc8c6: adds r4,r0,r1
  000cc8c8: ldr.w r0,[r10,#0x0]
  000cc8cc: ldr r0,[r0,#0x4]
  000cc8ce: ldr.w r0,[r0,r11,lsl #0x2]
  000cc8d2: blx 0x00072ed4
  000cc8d6: cbnz r0,0x000cc922
  000cc8d8: ldr.w r0,[r10,#0x0]
  000cc8dc: ldr r0,[r0,#0x4]
  000cc8de: ldr.w r0,[r0,r11,lsl #0x2]
  000cc8e2: ldr r0,[r0,#0x4]
  000cc8e4: blx 0x0007420c
  000cc8e8: cbnz r0,0x000cc922
  000cc8ea: ldr.w r0,[r10,#0x0]
  000cc8ee: ldr r0,[r0,#0x4]
  000cc8f0: ldr.w r0,[r0,r11,lsl #0x2]
  000cc8f4: ldr r0,[r0,#0x28]
  000cc8f6: cmp r0,#0x9
  000cc8f8: bne 0x000cc922
  000cc8fa: vmov s0,r4
  000cc8fe: vcvt.f32.s32 s0,s0
  000cc902: ldr r0,[sp,#0x44]
  000cc904: cmp r0,#0x10
  000cc906: bne 0x000cc916
  000cc908: vadd.f32 s0,s0,s0
  000cc90c: vcvt.s32.f32 s0,s0
  000cc910: vmov r5,s0
  000cc914: b 0x000cc934
  000cc916: vmul.f32 s0,s0,s21
  000cc91a: vcvt.s32.f32 s0,s0
  000cc91e: vmov r4,s0
  000cc922: ldr r0,[sp,#0x34]
  000cc924: movs r5,#0x5
  000cc926: cmp r0,#0x0
  000cc928: it eq
  000cc92a: mov.eq r5,r4
  000cc92c: ldr r0,[sp,#0x38]
  000cc92e: cmp r0,#0x8
  000cc930: it cs
  000cc932: mov.cs r5,r4
  000cc934: ldr r0,[sp,#0x44]
  000cc936: movs r4,#0x0
  000cc938: cmp r0,#0x50
  000cc93a: bne 0x000cc960
  000cc93c: ldr.w r0,[r10,#0x0]
  000cc940: ldr r0,[r0,#0x4]
  000cc942: ldr.w r0,[r0,r11,lsl #0x2]
  000cc946: ldrb.w r0,[r0,#0x3e]
  000cc94a: cbz r0,0x000cc960
  000cc94c: vmov s0,r5
  000cc950: vcvt.f32.s32 s0,s0
  000cc954: vmul.f32 s0,s0,s20
  000cc958: vcvt.s32.f32 s0,s0
  000cc95c: vmov r5,s0
  000cc960: ldr r0,[r6,#0x0]
  000cc962: blx 0x00071770
  000cc966: cmp r0,#0x46
  000cc968: bne 0x000cc98e
  000cc96a: ldr.w r0,[r10,#0x0]
  000cc96e: ldr r0,[r0,#0x4]
  000cc970: ldr.w r0,[r0,r11,lsl #0x2]
  000cc974: blx 0x00072ed4
  000cc978: cbnz r0,0x000cc98e
  000cc97a: vmov s0,r5
  000cc97e: vcvt.f32.s32 s0,s0
  000cc982: vmul.f32 s0,s0,s22
  000cc986: vcvt.s32.f32 s0,s0
  000cc98a: vmov r5,s0
  000cc98e: ldr r0,[r6,#0x0]
  000cc990: blx 0x00072850
  000cc994: cbz r0,0x000cc9a6
  000cc996: ldr r0,[r6,#0x0]
  000cc998: blx 0x00072850
  000cc99c: blx 0x00072874
  000cc9a0: cmp r0,#0xb7
  000cc9a2: it eq
  000cc9a4: mov.eq r5,#0x1
  000cc9a6: mov.w r0,#0x114
  000cc9aa: str r5,[sp,#0x4c]
  000cc9ac: blx 0x0006eb24
  000cc9b0: mov r5,r0
  000cc9b2: ldr r0,[r6,#0x0]
  000cc9b4: blx 0x00073c48
  000cc9b8: cbz r0,0x000cc9be
  000cc9ba: movs r0,#0x2d
  000cc9bc: b 0x000cc9c4
  000cc9be: ldr r0,[r6,#0x0]
  000cc9c0: blx 0x00071770
  000cc9c4: mov.w r1,#0x258
  000cc9c8: sub.w r0,r1,r0, lsl #0x1
  000cc9cc: mov.w r1,#0xffffffff
  000cc9d0: strd r4,r4,[sp,#0x10]
  000cc9d4: strd r4,r4,[sp,#0x18]
  000cc9d8: movs r3,#0x4
  000cc9da: strd r4,r4,[sp,#0x20]
  000cc9de: str r1,[sp,#0x0]
  000cc9e0: movw r1,#0xbb8
  000cc9e4: vstr.32 s23,[sp,#0xc]
  000cc9e8: strd r1,r0,[sp,#0x4]
  000cc9ec: mov r0,r5
  000cc9ee: ldr r2,[sp,#0x4c]
  000cc9f0: movs r1,#0x0
  000cc9f2: blx 0x00074218
  000cc9f6: mov r0,r5
  000cc9f8: movs r1,#0x1
  000cc9fa: blx 0x00074224
  000cc9fe: mov r0,r5
  000cca00: mov r1,r8
  000cca02: blx 0x00074230
  000cca06: mov r0,r5
  000cca08: movs r1,#0x0
  000cca0a: blx 0x0007423c
  000cca0e: str r4,[r5,#0x5c]
  000cca10: ldr.w r0,[r8,#0xf8]
  000cca14: ldr r0,[r0,#0x4]
  000cca16: ldr.w r0,[r0,r11,lsl #0x2]
  000cca1a: ldr r0,[r0,#0x28]
  000cca1c: cmp r0,#0xa
  000cca1e: bhi 0x000cca94
  000cca20: adr.w r1,0xcca2c
  000cca24: tbb [r1,r0]
  000cca58: mov r0,r5
  000cca5a: movs r1,#0x0
  000cca5c: str r4,[r5,#0x5c]
  000cca5e: blx 0x0007423c
  000cca62: movw r0,#0x1a62
  000cca66: b 0x000ccad4
  000cca68: mov r0,r5
  000cca6a: movs r1,#0x3
  000cca6c: blx 0x0007423c
  000cca70: movw r0,#0x1a68
  000cca74: b 0x000ccad4
  000cca76: mov r0,r5
  000cca78: movs r1,#0x7
  000cca7a: str r4,[r5,#0x5c]
  000cca7c: blx 0x0007423c
  000cca80: movw r0,#0x1a6c
  000cca84: b 0x000ccad4
  000cca86: mov r0,r5
  000cca88: movs r1,#0x19
  000cca8a: blx 0x0007423c
  000cca8e: movw r0,#0x1a92
  000cca92: b 0x000ccad4
  000cca94: movs r0,#0x1
  000cca96: movs r1,#0x13
  000cca98: str r0,[r5,#0x5c]
  000cca9a: mov r0,r5
  000cca9c: blx 0x0007423c
  000ccaa0: movw r0,#0x1a8b
  000ccaa4: b 0x000ccad4
  000ccaa6: mov r0,r5
  000ccaa8: movs r1,#0x5
  000ccaaa: blx 0x0007423c
  000ccaae: movw r0,#0x1a6a
  000ccab2: b 0x000ccad4
  000ccab4: mov r0,r5
  000ccab6: movs r1,#0xe5
  000ccab8: blx 0x0007423c
  000ccabc: vldr.32 s0,[r5,#0x60]
  000ccac0: movw r0,#0x4a93
  000ccac4: vcvt.f32.s32 s0,s0
  000ccac8: vmul.f32 s0,s0,s26
  000ccacc: vcvt.s32.f32 s0,s0
  000ccad0: vstr.32 s0,[r5,#0x60]
  000ccad4: str r0,[sp,#0x50]
  000ccad6: ldr r0,[r6,#0x0]
  000ccad8: blx 0x00071770
  000ccadc: mov r2,r0
  000ccade: ldr.w r0,[r10,#0x0]
  000ccae2: str r2,[sp,#0x54]
  000ccae4: ldr r0,[r0,#0x4]
  000ccae6: ldr.w r0,[r0,r11,lsl #0x2]
  000ccaea: ldrb.w r1,[r0,#0x3e]
  000ccaee: cmp r1,#0x0
  000ccaf0: beq.w 0x000cccdc
  000ccaf4: blx 0x00074248
  000ccaf8: cbz r0,0x000ccb2a
  000ccafa: ldr r0,[r0,#0x7c]
  000ccafc: cmp r0,#0x2d
  000ccafe: it ne
  000ccb00: cmp.ne r0,#0x33
  000ccb02: bne 0x000ccb2a
  000ccb04: movs r0,#0x2
  000ccb06: movs r1,#0x16
  000ccb08: str r0,[r5,#0x5c]
  000ccb0a: mov r0,r5
  000ccb0c: blx 0x0007423c
  000ccb10: vldr.32 s0,[r5,#0x60]
  000ccb14: movw r0,#0x1a8e
  000ccb18: vcvt.f64.s32 d16,s0
  000ccb1c: vmul.f64 d16,d16,d14
  000ccb20: vcvt.s32.f64 s0,d16
  000ccb24: vstr.32 s0,[r5,#0x60]
  000ccb28: b 0x000cccca
  000ccb2a: ldr.w r0,[r10,#0x0]
  000ccb2e: ldr r0,[r0,#0x4]
  000ccb30: ldr.w r0,[r0,r11,lsl #0x2]
  000ccb34: ldrb.w r1,[r0,#0x3f]
  000ccb38: cbz r1,0x000ccb7c
  000ccb3a: blx 0x00072010
  000ccb3e: mov r1,r0
  000ccb40: movw r0,#0x49c1
  000ccb44: cmp r1,r0
  000ccb46: strd r1,r9,[sp,#0x28]
  000ccb4a: beq 0x000ccb96
  000ccb4c: movw r0,#0x49c0
  000ccb50: cmp r1,r0
  000ccb52: mov.w r6,#0x1
  000ccb56: bne 0x000ccbbc
  000ccb58: mov r0,r5
  000ccb5a: movs r1,#0x2
  000ccb5c: str r4,[r5,#0x5c]
  000ccb5e: blx 0x0007423c
  000ccb62: ldr r0,[0x000cce30]
  000ccb64: add r1,sp,#0x60
  000ccb66: strd r4,r4,[sp,#0x60]
  000ccb6a: str r0,[sp,#0x68]
  000ccb6c: add.w r0,r5,#0x7c
  000ccb70: blx 0x0006eb3c
  000ccb74: movs r4,#0xd3
  000ccb76: movw r0,#0x1a64
  000ccb7a: b 0x000ccbde
  000ccb7c: ldr r0,[r0,#0x28]
  000ccb7e: movs r1,#0x1
  000ccb80: str r1,[r5,#0x5c]
  000ccb82: cmp r0,#0x1
  000ccb84: bne.w 0x000cccbe
  000ccb88: mov r0,r5
  000ccb8a: movs r1,#0xf
  000ccb8c: blx 0x0007423c
  000ccb90: movw r0,#0x1a87
  000ccb94: b 0x000cccca
  000ccb96: movs r6,#0x1
  000ccb98: mov r0,r5
  000ccb9a: movs r1,#0x14
  000ccb9c: str r6,[r5,#0x5c]
  000ccb9e: blx 0x0007423c
  000ccba2: ldr r0,[0x000cce30]
  000ccba4: add r1,sp,#0x60
  000ccba6: strd r4,r4,[sp,#0x60]
  000ccbaa: str r0,[sp,#0x68]
  000ccbac: add.w r0,r5,#0x7c
  000ccbb0: blx 0x0006eb3c
  000ccbb4: movs r4,#0xd4
  000ccbb6: movw r0,#0x1a8d
  000ccbba: b 0x000ccbde
  000ccbbc: mov r0,r5
  000ccbbe: movs r1,#0xe
  000ccbc0: str r6,[r5,#0x5c]
  000ccbc2: blx 0x0007423c
  000ccbc6: ldr r0,[0x000cce34]
  000ccbc8: add r1,sp,#0x60
  000ccbca: strd r4,r4,[sp,#0x60]
  000ccbce: str r0,[sp,#0x68]
  000ccbd0: add.w r0,r5,#0x7c
  000ccbd4: blx 0x0006eb3c
  000ccbd8: movs r4,#0xd5
  000ccbda: movw r0,#0x1a86
  000ccbde: str r0,[sp,#0x50]
  000ccbe0: ldr r0,[0x000cce38]
  000ccbe2: strb.w r6,[r5,#0xa8]
  000ccbe6: add r0,pc
  000ccbe8: ldr.w r9,[r0,#0x0]
  000ccbec: ldr.w r0,[r9,#0x0]
  000ccbf0: ldr r1,[0x000cce3c]
  000ccbf2: ldr r0,[r0,#0x4]
  000ccbf4: add r1,pc
  000ccbf6: ldr.w r8,[r1,#0x0]
  000ccbfa: movs r1,#0x9
  000ccbfc: ldr.w r0,[r0,r4,lsl #0x2]
  000ccc00: blx r8
  000ccc02: str r0,[r5,#0x60]
  000ccc04: movs r1,#0xb
  000ccc06: ldr.w r0,[r9,#0x0]
  000ccc0a: ldr r0,[r0,#0x4]
  000ccc0c: ldr.w r0,[r0,r4,lsl #0x2]
  000ccc10: blx r8
  000ccc12: str r0,[r5,#0x48]
  000ccc14: movs r1,#0xc
  000ccc16: ldr.w r0,[r9,#0x0]
  000ccc1a: ldr r0,[r0,#0x4]
  000ccc1c: ldr.w r0,[r0,r4,lsl #0x2]
  000ccc20: blx r8
  000ccc22: str r0,[r5,#0x44]
  000ccc24: movs r1,#0xd
  000ccc26: ldr.w r0,[r9,#0x0]
  000ccc2a: ldr r0,[r0,#0x4]
  000ccc2c: ldr.w r0,[r0,r4,lsl #0x2]
  000ccc30: blx r8
  000ccc32: vmov s0,r0
  000ccc36: vcvt.f32.s32 s0,s0
  000ccc3a: ldr r2,[sp,#0x54]
  000ccc3c: cmp r2,#0x9e
  000ccc3e: vstr.32 s0,[r5,#0x50]
  000ccc42: bne 0x000cccd0
  000ccc44: ldr r1,[sp,#0x28]
  000ccc46: movw r0,#0x49c2
  000ccc4a: ldr.w r9,[sp,#0x2c]
  000ccc4e: movs r4,#0x0
  000ccc50: ldr.w r8,[sp,#0x48]
  000ccc54: cmp r1,r0
  000ccc56: bne 0x000cccda
  000ccc58: ldr.w r0,[r10,#0x0]
  000ccc5c: ldr r0,[r0,#0x4]
  000ccc5e: ldr.w r0,[r0,r11,lsl #0x2]
  000ccc62: ldr r0,[r0,#0x4]
  000ccc64: blx 0x00074254
  000ccc68: ldr r6,[sp,#0x58]
  000ccc6a: cmp r0,#0x0
  000ccc6c: beq 0x000ccd28
  000ccc6e: vldr.32 s2,[r5,#0x60]
  000ccc72: vldr.32 s0,[r5,#0x50]
  000ccc76: vcvt.f32.s32 s2,s2
  000ccc7a: vmul.f32 s0,s0,s30
  000ccc7e: vmul.f32 s2,s2,s19
  000ccc82: vcvt.s32.f32 s2,s2
  000ccc86: vstr.32 s0,[r5,#0x50]
  000ccc8a: vstr.32 s2,[r5,#0x60]
  000ccc8e: ldr.w r0,[r10,#0x0]
  000ccc92: ldr r0,[r0,#0x4]
  000ccc94: ldr.w r0,[r0,r11,lsl #0x2]
  000ccc98: ldr r4,[r0,#0x4]
  000ccc9a: mov r0,r4
  000ccc9c: blx 0x00072910
  000ccca0: vmov s0,r0
  000ccca4: mov r0,r4
  000ccca6: movs r4,#0x0
  000ccca8: vcvt.f32.s32 s0,s0
  000cccac: vmul.f32 s0,s0,s17
  000cccb0: vcvt.s32.f32 s0,s0
  000cccb4: vmov r1,s0
  000cccb8: blx 0x00073f78
  000cccbc: b 0x000ccd28
  000cccbe: mov r0,r5
  000cccc0: movs r1,#0x14
  000cccc2: blx 0x0007423c
  000cccc6: movw r0,#0x1a8c
  000cccca: ldr r2,[sp,#0x54]
  000ccccc: str r0,[sp,#0x50]
  000cccce: b 0x000cccdc
  000cccd0: ldr.w r9,[sp,#0x2c]
  000cccd4: movs r4,#0x0
  000cccd6: ldr.w r8,[sp,#0x48]
  000cccda: ldr r6,[sp,#0x58]
  000cccdc: cmp r2,#0x7
  000cccde: beq 0x000ccd04
  000ccce0: cmp r2,#0x46
  000ccce2: bne 0x000ccd28
  000ccce4: ldr.w r0,[r10,#0x0]
  000ccce8: ldr r0,[r0,#0x4]
  000cccea: ldr.w r0,[r0,r11,lsl #0x2]
  000cccee: blx 0x00072ed4
  000cccf2: cbnz r0,0x000ccd28
  000cccf4: mov r0,r5
  000cccf6: movs r1,#0xb7
  000cccf8: blx 0x0007423c
  000cccfc: movw r0,#0x37d9
  000ccd00: str r0,[sp,#0x50]
  000ccd02: b 0x000ccd28
  000ccd04: ldr.w r0,[r10,#0x0]
  000ccd08: ldr r0,[r0,#0x4]
  000ccd0a: ldr.w r0,[r0,r11,lsl #0x2]
  000ccd0e: ldr r0,[r0,#0x28]
  000ccd10: cmp r0,#0x8
  000ccd12: bne 0x000ccd28
  000ccd14: vldr.32 s0,[r5,#0x60]
  000ccd18: vcvt.f32.s32 s0,s0
  000ccd1c: vmul.f32 s0,s0,s24
  000ccd20: vcvt.s32.f32 s0,s0
  000ccd24: vstr.32 s0,[r5,#0x60]
  000ccd28: ldr r0,[r6,#0x0]
  000ccd2a: blx 0x00072850
  000ccd2e: cbz r0,0x000ccd80
  000ccd30: ldr r0,[r6,#0x0]
  000ccd32: blx 0x00072850
  000ccd36: blx 0x0007372c
  000ccd3a: cbz r0,0x000ccd80
  000ccd3c: ldr r0,[0x000cce40]
  000ccd3e: add r0,pc
  000ccd40: ldr r0,[r0,#0x0]
  000ccd42: ldr r4,[r0,#0x0]
  000ccd44: ldr r0,[r6,#0x0]
  000ccd46: blx 0x00071770
  000ccd4a: cmp r4,r0
  000ccd4c: bne 0x000ccdd4
  000ccd4e: ldr r0,[0x000cce44]
  000ccd50: add r0,pc
  000ccd52: ldr r0,[r0,#0x0]
  000ccd54: ldr r0,[r0,#0x0]
  000ccd56: cmp r0,#0x3
  000ccd58: blt 0x000ccdd4
  000ccd5a: ldr.w r0,[r10,#0x0]
  000ccd5e: ldr r0,[r0,#0x4]
  000ccd60: ldr.w r0,[r0,r11,lsl #0x2]
  000ccd64: blx 0x000726b8
  000ccd68: cbz r0,0x000ccdd4
  000ccd6a: vldr.32 s0,[r5,#0x60]
  000ccd6e: vcvt.f32.s32 s0,s0
  000ccd72: vmul.f32 s0,s0,s26
  000ccd76: vcvt.s32.f32 s0,s0
  000ccd7a: vstr.32 s0,[r5,#0x60]
  000ccd7e: b 0x000ccdd4
  000ccd80: ldr.w r0,[r10,#0x0]
  000ccd84: ldr r2,[sp,#0x54]
  000ccd86: ldr r0,[r0,#0x4]
  000ccd88: cmp r2,#0x91
  000ccd8a: ldr.w r0,[r0,r11,lsl #0x2]
  000ccd8e: ldr r1,[r0,#0x7c]
  000ccd90: it eq
  000ccd92: cmp.eq r1,#0x31
  000ccd94: beq 0x000ccdbc
  000ccd96: subs r2,#0x9d
  000ccd98: cmp r2,#0x1
  000ccd9a: bhi.w 0x000cd160
  000ccd9e: cmp r1,#0x31
  000ccda0: bne.w 0x000cd160
  000ccda4: mov r0,r5
  000ccda6: movs r1,#0x7
  000ccda8: str r4,[r5,#0x5c]
  000ccdaa: blx 0x0007423c
  000ccdae: ldr r0,[r5,#0x60]
  000ccdb0: add.w r0,r0,r0, lsl #0x1
  000ccdb4: str r0,[r5,#0x60]
  000ccdb6: movw r0,#0x1a6c
  000ccdba: b 0x000ccdd2
  000ccdbc: movs r0,#0x28
  000ccdbe: movs r1,#0xd6
  000ccdc0: str r0,[r5,#0x5c]
  000ccdc2: mov r0,r5
  000ccdc4: blx 0x0007423c
  000ccdc8: ldr r0,[r5,#0x60]
  000ccdca: lsls r0,r0,#0x1
  000ccdcc: str r0,[r5,#0x60]
  000ccdce: movw r0,#0x37a0
  000ccdd2: str r0,[sp,#0x50]
  000ccdd4: ldr r6,[r5,#0x5c]
  000ccdd6: cmp r6,#0x28
  000ccdd8: it ne
  000ccdda: cmp.ne r6,#0x5
  000ccddc: bne 0x000cce48
  000ccdde: movs r0,#0xe8
  000ccde0: blx 0x0006eb24
  000ccde4: ldr r1,[r5,#0x58]
  000ccde6: mov r4,r0
  000ccde8: subs r0,r6,#0x5
  000ccdea: movs r2,#0x0
  000ccdec: clz r0,r0
  000ccdf0: strd r2,r2,[sp,#0x0]
  000ccdf4: mov r2,r5
  000ccdf6: lsrs r0,r0,#0x5
  000ccdf8: strd r6,r0,[sp,#0x8]
  000ccdfc: mov r0,r4
  000ccdfe: ldr r3,[sp,#0x50]
  000cce00: str.w r8,[sp,#0x10]
  000cce04: blx 0x00074260
  000cce08: ldr.w r0,[r8,#0xe8]
  000cce0c: ldr r1,[sp,#0x5c]
  000cce0e: ldr r0,[r0,#0x4]
  000cce10: str.w r4,[r0,r1,lsl #0x2]
  000cce14: mov.w r1,#0x41000000
  000cce18: ldr r0,[r5,#0x60]
  000cce1a: str r1,[r5,#0x50]
  000cce1c: movw r1,#0x2710
  000cce20: str r1,[r5,#0x44]
  000cce22: movw r1,#0xbb8
  000cce26: lsls r0,r0,#0x2
  000cce28: str r1,[r5,#0x48]
  000cce2a: str r0,[r5,#0x60]
  000cce2c: b 0x000cce70
  000cce48: movs r0,#0xb0
  000cce4a: blx 0x0006eb24
  000cce4e: mov r4,r0
  000cce50: movw r0,#0x2711
  000cce54: movs r1,#0x0
  000cce56: strd r0,r8,[sp,#0x0]
  000cce5a: mov r0,r4
  000cce5c: ldr r3,[sp,#0x50]
  000cce5e: mov r2,r5
  000cce60: blx 0x0007426c
  000cce64: ldr.w r0,[r8,#0xe8]
  000cce68: ldr r1,[sp,#0x5c]
  000cce6a: ldr r0,[r0,#0x4]
  000cce6c: str.w r4,[r0,r1,lsl #0x2]
  000cce70: ldr.w r0,[r10,#0x0]
  000cce74: mov r1,r5
  000cce76: movs r2,#0x0
  000cce78: ldr r0,[r0,#0x4]
  000cce7a: ldr.w r0,[r0,r11,lsl #0x2]
  000cce7e: blx 0x00074278
  000cce82: ldr.w r0,[r10,#0x0]
  000cce86: ldr r0,[r0,#0x4]
  000cce88: ldr r1,[0x000cd1f8]
  000cce8a: add r1,pc
  000cce8c: ldr.w r0,[r0,r11,lsl #0x2]
  000cce90: ldr.w r8,[r1,#0x0]
  000cce94: ldr r1,[r0,#0x28]
  000cce96: ldr.w r0,[r8,#0x0]
  000cce9a: cmp r1,#0x9
  000cce9c: mov.w r1,#0x3d
  000ccea0: it eq
  000ccea2: mov.eq r1,#0x3e
  000ccea4: blx 0x00072298
  000ccea8: ldr.w r0,[r10,#0x0]
  000cceac: ldr r1,[sp,#0x40]
  000cceae: ldr r4,[sp,#0x58]
  000cceb0: ldr r0,[r0,#0x4]
  000cceb2: cmp r1,#0x0
  000cceb4: ldr.w r0,[r0,r11,lsl #0x2]
  000cceb8: beq 0x000ccf7c
  000cceba: ldrb.w r1,[r0,#0x42]
  000ccebe: ldr r2,[sp,#0x54]
  000ccec0: cmp r1,#0x0
  000ccec2: beq 0x000ccf7e
  000ccec4: ldr r1,[r0,#0x7c]
  000ccec6: subs r1,#0x2d
  000ccec8: cmp r1,#0x3
  000cceca: bhi 0x000ccf7e
  000ccecc: mov.w r0,#0x114
  000cced0: blx 0x0006eb24
  000cced4: mov r5,r0
  000cced6: movs r0,#0x0
  000cced8: mov.w r1,#0xffffffff
  000ccedc: strd r0,r0,[sp,#0x20]
  000ccee0: movs r3,#0x4
  000ccee2: str r1,[sp,#0x0]
  000ccee4: movw r1,#0x2710
  000ccee8: str r1,[sp,#0x4]
  000cceea: movw r1,#0xbb8
  000cceee: str r1,[sp,#0x8]
  000ccef0: mov.w r1,#0x41000000
  000ccef4: strd r1,r0,[sp,#0xc]
  000ccef8: movs r1,#0x0
  000ccefa: strd r0,r0,[sp,#0x14]
  000ccefe: str r0,[sp,#0x1c]
  000ccf00: ldr r0,[sp,#0x4c]
  000ccf02: lsls r2,r0,#0x2
  000ccf04: mov r0,r5
  000ccf06: blx 0x00074218
  000ccf0a: mov r0,r5
  000ccf0c: movs r1,#0x1
  000ccf0e: blx 0x00074224
  000ccf12: ldr r6,[sp,#0x48]
  000ccf14: mov r0,r5
  000ccf16: mov r1,r6
  000ccf18: blx 0x00074230
  000ccf1c: movs r0,#0x4
  000ccf1e: movs r1,#0x1f
  000ccf20: str r0,[r5,#0x5c]
  000ccf22: mov r0,r5
  000ccf24: blx 0x0007423c
  000ccf28: movs r0,#0xe8
  000ccf2a: blx 0x0006eb24
  000ccf2e: mov r4,r0
  000ccf30: ldrd r1,r0,[r5,#0x58]
  000ccf34: movs r2,#0x0
  000ccf36: add r3,sp,#0x8
  000ccf38: strd r2,r2,[sp,#0x0]
  000ccf3c: stmia r3!,{r0,r2,r6}
  000ccf3e: mov r0,r4
  000ccf40: mov r2,r5
  000ccf42: movw r3,#0x37a0
  000ccf46: blx 0x00074260
  000ccf4a: ldr.w r1,[r6,#0xe8]
  000ccf4e: mov r0,r4
  000ccf50: blx 0x00074284
  000ccf54: ldr.w r0,[r6,#0xf8]
  000ccf58: mov r1,r5
  000ccf5a: movs r2,#0x1
  000ccf5c: ldr r0,[r0,#0x4]
  000ccf5e: ldr.w r0,[r0,r11,lsl #0x2]
  000ccf62: blx 0x00074278
  000ccf66: ldr.w r0,[r8,#0x0]
  000ccf6a: movs r1,#0x54
  000ccf6c: blx 0x00072298
  000ccf70: ldr.w r0,[r6,#0xf8]
  000ccf74: ldr r4,[sp,#0x58]
  000ccf76: ldr r0,[r0,#0x4]
  000ccf78: ldr.w r0,[r0,r11,lsl #0x2]
  000ccf7c: ldr r2,[sp,#0x54]
  000ccf7e: sub.w r1,r2,#0x9d
  000ccf82: cmp r1,#0x1
  000ccf84: bhi 0x000cd06c
  000ccf86: ldr r1,[r0,#0x7c]
  000ccf88: cmp r1,#0x31
  000ccf8a: bne 0x000cd06c
  000ccf8c: mov.w r0,#0x114
  000ccf90: blx 0x0006eb24
  000ccf94: mov r5,r0
  000ccf96: ldr r0,[r4,#0x0]
  000ccf98: blx 0x00073c48
  000ccf9c: cbz r0,0x000ccfa2
  000ccf9e: movs r0,#0x2d
  000ccfa0: b 0x000ccfa8
  000ccfa2: ldr r0,[r4,#0x0]
  000ccfa4: blx 0x00071770
  000ccfa8: movs r1,#0x0
  000ccfaa: movs r3,#0x4
  000ccfac: strd r1,r1,[sp,#0x10]
  000ccfb0: strd r1,r1,[sp,#0x18]
  000ccfb4: strd r1,r1,[sp,#0x20]
  000ccfb8: mov.w r1,#0x258
  000ccfbc: sub.w r0,r1,r0, lsl #0x1
  000ccfc0: mov.w r1,#0xffffffff
  000ccfc4: str r1,[sp,#0x0]
  000ccfc6: movw r1,#0xbb8
  000ccfca: vstr.32 s23,[sp,#0xc]
  000ccfce: strd r1,r0,[sp,#0x4]
  000ccfd2: mov r0,r5
  000ccfd4: ldr r2,[sp,#0x4c]
  000ccfd6: movs r1,#0x0
  000ccfd8: blx 0x00074218
  000ccfdc: mov r0,r5
  000ccfde: movs r1,#0x1
  000ccfe0: blx 0x00074224
  000ccfe4: ldr r4,[sp,#0x48]
  000ccfe6: mov r0,r5
  000ccfe8: mov r1,r4
  000ccfea: blx 0x00074230
  000ccfee: movs r0,#0x28
  000ccff0: movs r1,#0xd6
  000ccff2: str r0,[r5,#0x5c]
  000ccff4: mov r0,r5
  000ccff6: blx 0x0007423c
  000ccffa: movs r0,#0xe8
  000ccffc: blx 0x0006eb24
  000cd000: mov r6,r0
  000cd002: ldrd r1,r0,[r5,#0x58]
  000cd006: subs r2,r0,#0x5
  000cd008: movs r3,#0x0
  000cd00a: clz r2,r2
  000cd00e: strd r3,r3,[sp,#0x0]
  000cd012: add r3,sp,#0x8
  000cd014: lsrs r2,r2,#0x5
  000cd016: stmia r3!,{r0,r2,r4}
  000cd018: mov r0,r6
  000cd01a: mov r2,r5
  000cd01c: movw r3,#0x37a0
  000cd020: blx 0x00074260
  000cd024: ldr.w r1,[r4,#0xe8]
  000cd028: mov r0,r6
  000cd02a: blx 0x00074284
  000cd02e: ldr r0,[r5,#0x60]
  000cd030: mov.w r1,#0x41000000
  000cd034: str r1,[r5,#0x50]
  000cd036: movw r1,#0x2710
  000cd03a: str r1,[r5,#0x44]
  000cd03c: movw r1,#0xbb8
  000cd040: lsls r0,r0,#0x2
  000cd042: str r1,[r5,#0x48]
  000cd044: str r0,[r5,#0x60]
  000cd046: mov r1,r5
  000cd048: ldr.w r0,[r4,#0xf8]
  000cd04c: movs r2,#0x1
  000cd04e: ldr r0,[r0,#0x4]
  000cd050: ldr.w r0,[r0,r11,lsl #0x2]
  000cd054: blx 0x00074278
  000cd058: ldr.w r0,[r8,#0x0]
  000cd05c: movs r1,#0x54
  000cd05e: blx 0x00072298
  000cd062: ldr.w r0,[r4,#0xf8]
  000cd066: ldr r0,[r0,#0x4]
  000cd068: ldr.w r0,[r0,r11,lsl #0x2]
  000cd06c: ldr.w r8,[sp,#0x48]
  000cd070: cmp r0,#0x0
  000cd072: ldr r1,[sp,#0x5c]
  000cd074: mov.w r4,#0x0
  000cd078: add.w r1,r1,#0x1
  000cd07c: str r1,[sp,#0x5c]
  000cd07e: beq 0x000cd158
  000cd080: blx 0x00072ed4
  000cd084: cmp r0,#0x0
  000cd086: beq 0x000cd158
  000cd088: ldr.w r0,[r10,#0x0]
  000cd08c: ldr r0,[r0,#0x4]
  000cd08e: ldr.w r0,[r0,r11,lsl #0x2]
  000cd092: ldrb.w r0,[r0,#0x25]
  000cd096: cmp r0,#0x0
  000cd098: beq 0x000cd158
  000cd09a: mov.w r0,#0x114
  000cd09e: blx 0x0006eb24
  000cd0a2: mov r5,r0
  000cd0a4: mov.w r0,#0xffffffff
  000cd0a8: strd r4,r4,[sp,#0x20]
  000cd0ac: str r0,[sp,#0x0]
  000cd0ae: movw r0,#0xbb8
  000cd0b2: str r0,[sp,#0x4]
  000cd0b4: mov.w r0,#0x190
  000cd0b8: str r0,[sp,#0x8]
  000cd0ba: mov.w r0,#0x41800000
  000cd0be: strd r0,r4,[sp,#0xc]
  000cd0c2: mov r0,r5
  000cd0c4: movs r1,#0x12
  000cd0c6: movs r2,#0x0
  000cd0c8: movs r3,#0x4
  000cd0ca: strd r4,r4,[sp,#0x14]
  000cd0ce: str r4,[sp,#0x1c]
  000cd0d0: blx 0x00074218
  000cd0d4: mov r0,r5
  000cd0d6: movs r1,#0x1
  000cd0d8: blx 0x00074224
  000cd0dc: mov r0,r5
  000cd0de: mov r1,r8
  000cd0e0: blx 0x00074230
  000cd0e4: movs r0,#0x12
  000cd0e6: str r0,[r5,#0x58]
  000cd0e8: movs r0,#0x1
  000cd0ea: str r0,[r5,#0x5c]
  000cd0ec: movs r0,#0xb0
  000cd0ee: blx 0x0006eb24
  000cd0f2: mov r4,r0
  000cd0f4: movw r0,#0x2711
  000cd0f8: movs r1,#0x12
  000cd0fa: strd r0,r8,[sp,#0x0]
  000cd0fe: mov r0,r4
  000cd100: mov r2,r5
  000cd102: movw r3,#0x1a8a
  000cd106: blx 0x0007426c
  000cd10a: ldr.w r0,[r8,#0xe8]
  000cd10e: movs r1,#0x12
  000cd110: ldr r6,[sp,#0x5c]
  000cd112: ldr r0,[r0,#0x4]
  000cd114: str.w r4,[r0,r6,lsl #0x2]
  000cd118: mov r0,r5
  000cd11a: blx 0x0007423c
  000cd11e: ldr r0,[0x000cd1fc]
  000cd120: movs r1,#0xa
  000cd122: add r0,pc
  000cd124: ldr r0,[r0,#0x0]
  000cd126: ldr r0,[r0,#0x0]
  000cd128: ldr r0,[r0,#0x4]
  000cd12a: ldr r0,[r0,#0x48]
  000cd12c: blx 0x000718e4
  000cd130: str r0,[r5,#0x64]
  000cd132: mov r1,r5
  000cd134: ldr.w r0,[r8,#0xf8]
  000cd138: movs r2,#0x1
  000cd13a: ldr r0,[r0,#0x4]
  000cd13c: ldr.w r0,[r0,r11,lsl #0x2]
  000cd140: blx 0x00074278
  000cd144: ldr r0,[0x000cd200]
  000cd146: movs r1,#0x4a
  000cd148: add r0,pc
  000cd14a: ldr r0,[r0,#0x0]
  000cd14c: ldr r0,[r0,#0x0]
  000cd14e: blx 0x00072298
  000cd152: movs r4,#0x0
  000cd154: adds r6,#0x1
  000cd156: str r6,[sp,#0x5c]
  000cd158: add.w r11,r11,#0x1
  000cd15c: b.w 0x000cc838
  000cd160: ldr r1,[sp,#0x40]
  000cd162: cmp r1,#0x0
  000cd164: beq.w 0x000ccdd4
  000cd168: ldrb.w r0,[r0,#0x42]
  000cd16c: cmp r0,#0x0
  000cd16e: beq.w 0x000ccdd4
  000cd172: ldr r0,[sp,#0x40]
  000cd174: blx 0x00074290
  000cd178: mov r4,r0
  000cd17a: mov r0,r5
  000cd17c: mov r1,r4
  000cd17e: blx 0x0007423c
  000cd182: ldr r0,[0x000cd1f0]
  000cd184: movs r1,#0x2
  000cd186: add r0,pc
  000cd188: ldr r0,[r0,#0x0]
  000cd18a: ldr r0,[r0,#0x0]
  000cd18c: ldr r0,[r0,#0x4]
  000cd18e: ldr.w r0,[r0,r4,lsl #0x2]
  000cd192: blx 0x000718e4
  000cd196: ldr r1,[0x000cd1f4]
  000cd198: ldr r2,[r5,#0x60]
  000cd19a: add r1,pc
  000cd19c: ldr.w r1,[r1,r4,lsl #0x2]
  000cd1a0: str r1,[sp,#0x50]
  000cd1a2: lsls r1,r2,#0x2
  000cd1a4: strd r0,r1,[r5,#0x5c]
  000cd1a8: b 0x000ccdd4
  000cd1aa: ldr r0,[sp,#0x6c]
  000cd1ac: ldr.w r1,[r9,#0x0]
  000cd1b0: subs r0,r1,r0
  000cd1b2: itttt eq
  000cd1b4: add.eq sp,#0x70
  000cd1b6: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  000cd1ba: add.eq sp,#0x4
  000cd1bc: pop.eq.w {r8,r9,r10,r11}
  000cd1c0: it eq
  000cd1c2: pop.eq {r4,r5,r6,r7,pc}
  000cd1c4: blx 0x0006e824
```
