# PlayerEgo::approachDockingPoint
elf 0x9fcb8 body 2446
Sig: undefined __thiscall approachDockingPoint(PlayerEgo * this, Hud * param_1, int param_2, Radar * param_3)

## decompile
```c

/* PlayerEgo::approachDockingPoint(Hud*, int, Radar*) */

void __thiscall
PlayerEgo::approachDockingPoint(PlayerEgo *this,Hud *param_1,int param_2,Radar *param_3)

{
  char cVar1;
  float fVar2;
  int iVar3;
  undefined4 uVar4;
  EaseInOutMatrix *pEVar5;
  Mission *this_00;
  Mission *this_01;
  Vector *pVVar6;
  float fVar7;
  undefined4 *puVar8;
  Matrix *pMVar9;
  void *pvVar10;
  Ship *pSVar11;
  float fVar12;
  PlayerEgo *pPVar13;
  int *piVar14;
  SingleLevel *pSVar15;
  int iVar16;
  code *pcVar17;
  code *pcVar18;
  code *pcVar19;
  int **ppiVar20;
  int *piVar21;
  int *piVar22;
  uint in_fpscr;
  float fVar23;
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 local_f0;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined4 local_c0;
  Vector aVStack_b0 [12];
  Vector aVStack_a4 [12];
  Vector aVStack_98 [12];
  Vector aVStack_8c [12];
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined4 local_48;
  Matrix aMStack_40 [12];
  int local_34;
  
  piVar22 = *(int **)(DAT_000b0020 + 0xafcd2);
  local_34 = *piVar22;
  iVar3 = KIPlayer::isDying();
  if (iVar3 != 0) goto switchD_000afcf2_caseD_1;
  switch(this[0xc].field_14) {
  case (int *)0x0:
    pSVar15 = this[0xc].m_pLevel;
    getPosition();
    pSVar15 = (SingleLevel *)
              KIPlayer::getNearestNavigationPoint((Vector *)pSVar15,(SpacePoint *)&local_80);
    if (pSVar15 != (SingleLevel *)0x0) {
      if (this[0x18].m_pLevel != pSVar15) {
        if (this[0x18].m_pLevel != (SingleLevel *)0x0) {
          SpacePoint::giveFree();
        }
        this[0x18].m_pLevel = pSVar15;
        SpacePoint::take();
      }
      (*(code *)(this[0xc].m_pLevel)->field_0[10])((Vector *)&local_f8);
      pVVar6 = (Vector *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::MatrixRotateVector(aMStack_40,pVVar6);
      AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)&local_f8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&this[6].field_14,(Vector *)&local_80);
      getPosition();
      AbyssEngine::AEMath::operator-((Vector *)&local_80,(Vector *)&this[6].field_14);
      fVar7 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_80);
      if ((int)fVar7 < (int)this[0xd].field_4) {
        if (*(char *)&(this[0xc].m_pLevel)->field_70 != '\0') {
          *(undefined1 *)&(this[0xc].m_pLevel)->field_8C = 0;
        }
        iVar3 = this[3].field_1C;
        this[0xc].field_14 = (int *)0x2;
        TargetFollowCamera::setLookAtCam(SUB41(iVar3,0));
        TargetFollowCamera::useTargetsUpVector(SUB41(this[3].field_1C,0));
        if ((EaseInOutMatrix *)this[0x17].field_1C != (EaseInOutMatrix *)0x0) {
          pvVar10 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                                      ((EaseInOutMatrix *)this[0x17].field_1C);
          operator_delete(pvVar10);
        }
        pSVar15 = this[0xc].m_pLevel;
        this[0x17].field_1C = 0;
        (*(code *)pSVar15->field_0[10])(&local_f8,pSVar15);
        pcVar19 = *(code **)(DAT_000b0024 + 0xafe1e);
        uVar4 = (*pcVar19)((this[0xc].m_pLevel)->field_08);
        pcVar17 = *(code **)(DAT_000b0028 + 0xafe34);
        (*pcVar17)(aMStack_40,uVar4,this[0x18].m_pLevel);
        AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)&local_f8);
        iVar3 = KIPlayer::getNearestDockingPoint((Vector *)pSVar15);
        uVar4 = (*pcVar19)((this[0xc].m_pLevel)->field_08);
        (*pcVar17)(&local_80,uVar4,iVar3 + 0xc);
        pcVar18 = *(code **)(DAT_000b002c + 0xafe6c);
        (*pcVar18)(aMStack_40,&local_80);
        uVar4 = (*pcVar19)((this[0xc].m_pLevel)->field_08);
        (*pcVar17)(&local_f8,uVar4,this[0x18].m_pLevel);
        uVar4 = (*pcVar19)((this[0xc].m_pLevel)->field_08);
        (*pcVar17)(aVStack_98,uVar4,iVar3);
        AbyssEngine::AEMath::operator-((Vector *)&local_80,(Vector *)&local_f8);
        (*pcVar18)(aVStack_8c,(Vector *)&local_80);
        AbyssEngine::AEMath::VectorCross((Vector *)&local_80,aVStack_8c);
        (*pcVar18)(aVStack_98,(Vector *)&local_80);
        local_50 = DAT_000b0688;
        uStack_58 = DAT_000b0680;
        uStack_64 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_60 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        local_5c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar8 = (undefined4 *)((uint)&local_80 | 4);
        *puVar8 = 0;
        puVar8[1] = uStack_64;
        puVar8[2] = uStack_60;
        puVar8[3] = local_5c;
        local_68 = 0;
        local_80 = 0x3f800000;
        local_6c = 0x3f800000;
        local_48 = 0x3f800000;
        AbyssEngine::AEMath::MatrixSetRotation
                  ((Matrix *)&local_f8,(Vector *)&local_80,aVStack_98,aVStack_8c);
        pEVar5 = operator_new(0xf4);
        puVar8 = (undefined4 *)(*pcVar19)(this->field_8);
        fVar7 = (float)AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
                                 (pEVar5,*puVar8,puVar8[1],puVar8[2],puVar8[3],puVar8[4],puVar8[5],
                                  puVar8[6],puVar8[7],puVar8[8],puVar8[9],puVar8[10],puVar8[0xb],
                                  puVar8[0xc],puVar8[0xd],puVar8[0xe],local_80,local_7c,local_78,
                                  local_74,local_70,local_6c,local_68,uStack_64,uStack_60,local_5c,
                                  (undefined4)uStack_58,uStack_58._4_4_,(undefined4)local_50,
                                  local_50._4_4_,local_48,2000);
        iVar3 = DAT_000b0030;
        this[0x17].field_1C = (int)pEVar5;
        piVar14 = *(int **)(iVar3 + 0xaffea);
        FModSound::play(*piVar14,(Vector *)0x8de,(Vector *)0x0,fVar7);
        local_f8 = 0;
        uStack_f4 = 0;
        local_f0 = 0;
        Player::getPosition();
        FModSound::updateEvent3DAttributes
                  ((FModSound *)*piVar14,0x8de,aVStack_a4,(Vector *)&local_f8,false);
        StopEngineSound();
      }
      else {
        piVar14 = this[6].field_14;
        piVar21 = this[6].field_18;
        iVar3 = this[6].field_1C;
        pSVar11 = (Ship *)Status::getShip();
        fVar12 = (float)Ship::getHandling(pSVar11);
        fVar2 = DAT_000b0694;
        fVar23 = 4.0;
        if ((int)((uint)(fVar12 + DAT_000b0694 < 4.0) << 0x1f) < 0) {
          pSVar11 = (Ship *)Status::getShip();
          fVar23 = (float)Ship::getHandling(pSVar11);
          fVar23 = fVar23 + fVar2;
        }
        moveToPosition(this,piVar14,piVar21,iVar3,1,fVar23);
        if ((int)fVar7 < 20000) {
          *(undefined1 *)&this[0x16].field_18 = 1;
        }
      }
    }
  default:
    goto switchD_000afcf2_caseD_1;
  case (int *)0x2:
    cVar1 = *(char *)&this[0xb].field_14;
    *(undefined1 *)&this[0x16].field_18 = 1;
    if (cVar1 != '\0') {
      setTurretMode(SUB41(this,0));
    }
    pSVar15 = this[0xc].m_pLevel;
    (*(code *)pSVar15->field_0[10])((Vector *)&local_f8,pSVar15);
    pVVar6 = (Vector *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::MatrixRotateVector(aMStack_40,pVVar6);
    AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)&local_f8);
    KIPlayer::getNearestDockingPoint((Vector *)pSVar15);
    (*(code *)(this[0xc].m_pLevel)->field_0[10])((Vector *)&local_f8);
    pVVar6 = (Vector *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::MatrixRotateVector(aMStack_40,pVVar6);
    AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)&local_f8);
    ppiVar20 = &this[6].field_14;
    AbyssEngine::AEMath::Vector::operator=((Vector *)ppiVar20,(Vector *)&local_80);
    getPosition();
    AbyssEngine::AEMath::operator-((Vector *)&local_80,(Vector *)ppiVar20);
    fVar7 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_80);
    iVar16 = DAT_000b049c + 0xb00f2;
    piVar14 = *(int **)(DAT_000b0498 + 0xb00f0);
    Status::getShip();
    iVar3 = Ship::getIndex();
    if ((int)fVar7 < *(int *)(iVar16 + iVar3 * 4)) {
      this_00 = (Mission *)Status::getMission();
      this_01 = (Mission *)Status::getCampaignMission();
      iVar3 = Mission::isEmpty();
      if (((iVar3 == 0) && (iVar3 = Mission::getType(), iVar3 == 0xf)) &&
         (iVar3 = PlayerFixedObject::getDockingType(), iVar3 == 1)) {
        iVar3 = Status::getShip();
        iVar16 = Mission::getProductionGoodIndex(this_00);
        iVar3 = Ship::hasCargo(iVar3,iVar16);
        if (iVar3 != 0) {
          iVar3 = Status::getShip();
          Mission::getProductionGoodIndex(this_00);
          Ship::getCargo(iVar3);
          iVar3 = Item::getAmount();
          this[0x18].m_pPlayer = iVar3;
          iVar16 = Mission::getProductionGoodAmount();
          iVar3 = Level::getNumDeliveredOre((Level *)this->m_pLevel);
LAB_000b05c2:
          iVar16 = iVar16 - iVar3;
          iVar3 = this[0x18].m_pPlayer;
          if (iVar16 < this[0x18].m_pPlayer) {
            this[0x18].m_pPlayer = iVar16;
            iVar3 = iVar16;
          }
          this[0x18].field_4 = (int *)0x0;
          this[0x17].field_20 = 0;
          if (0 < iVar3) {
            pPVar13 = (PlayerEgo *)0x29;
            goto LAB_000b064c;
          }
        }
      }
      else {
        iVar3 = Mission::isEmpty();
        if (((iVar3 != 0) ||
            ((iVar3 = Mission::getType(), iVar3 != 0xb8 &&
             (iVar3 = Mission::getType(), iVar3 != 0xa8)))) ||
           (iVar3 = PlayerFixedObject::getDockingType(), iVar3 != 2)) {
          iVar3 = Mission::isEmpty();
          if (((iVar3 == 0) && (iVar3 = Mission::getType(), iVar3 == 0xb8)) &&
             (iVar3 = PlayerFixedObject::getDockingType(), iVar3 == 1)) {
            iVar3 = *(int *)(*piVar14 + 0x178);
            if (0 < iVar3) {
              this[0x17].field_20 = 0;
              this[0x18].m_pPlayer = iVar3;
              this[0x18].field_4 = (int *)0x0;
              pPVar13 = (PlayerEgo *)0x25;
              goto LAB_000b064c;
            }
          }
          else if ((((this_01 != (Mission *)0x0) && (iVar3 = Mission::isEmpty(), iVar3 == 0)) &&
                   ((iVar3 = Mission::getType(), iVar3 == 0xa7 ||
                    (iVar3 = Mission::getType(), iVar3 == 0xae)))) &&
                  (iVar3 = PlayerFixedObject::getDockingType(), iVar3 == 1)) {
            iVar3 = Status::getShip();
            iVar16 = Mission::getProductionGoodIndex(this_01);
            iVar3 = Ship::hasCargo(iVar3,iVar16);
            if (iVar3 != 0) {
              iVar3 = Status::getShip();
              Mission::getProductionGoodIndex(this_01);
              Ship::getCargo(iVar3);
              iVar3 = Item::getAmount();
              this[0x18].m_pPlayer = iVar3;
              iVar16 = Mission::getProductionGoodAmount();
              iVar3 = Mission::getStatusValue();
              goto LAB_000b05c2;
            }
          }
          goto LAB_000b0654;
        }
        Status::getShip();
        iVar3 = Ship::getMaxPassengers();
        if (iVar3 < 1) {
LAB_000b0476:
          iVar3 = Mission::getType();
          if (iVar3 != 0xa8) {
            Status::getShip();
            iVar3 = Ship::getMaxPassengers();
            if (iVar3 == 0) {
              pPVar13 = (PlayerEgo *)0x2b;
              goto LAB_000b064c;
            }
            goto LAB_000b0654;
          }
        }
        else {
          iVar16 = *(int *)(*piVar14 + 0x178);
          Status::getShip();
          iVar3 = Ship::getMaxPassengers();
          if (iVar3 <= iVar16) goto LAB_000b0476;
        }
        iVar3 = Mission::getType();
        if (iVar3 == 0xa8) {
          iVar3 = Mission::getStatusValue();
        }
        else {
          Status::getShip();
          iVar3 = Ship::getMaxPassengers();
        }
        iVar3 = iVar3 - *(int *)(*piVar14 + 0x178);
        this[0x18].m_pPlayer = iVar3;
        iVar16 = Mission::getStatusValue();
        if (iVar16 - *(int *)(*piVar14 + 0x178) < iVar3) {
          iVar3 = Mission::getStatusValue();
          iVar3 = iVar3 - *(int *)(*piVar14 + 0x178);
          this[0x18].m_pPlayer = iVar3;
        }
        else {
          iVar3 = this[0x18].m_pPlayer;
        }
        this[0x18].field_4 = (int *)0x0;
        this[0x17].field_20 = 0;
        if (0 < iVar3) {
          pPVar13 = (PlayerEgo *)0x23;
LAB_000b064c:
          Hud::hudEvent((int)param_1,pPVar13,(int)this);
        }
      }
LAB_000b0654:
      cVar1 = *(char *)((int)&this[4].field_20 + 2);
      this[0xc].field_14 = (int *)0x1;
      if (cVar1 == '\0') {
        *(undefined1 *)((int)&this[4].field_20 + 1) = 1;
      }
      *(undefined1 *)((int)&this[4].field_20 + 2) = 0;
      goto switchD_000afcf2_caseD_1;
    }
    fVar7 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::EaseInOutMatrix::Increase(fVar7);
    AbyssEngine::EaseInOutMatrix::GetValue();
    getPosition();
    getPosition();
    AbyssEngine::AEMath::operator-(aVStack_b0,(Vector *)ppiVar20);
    fVar7 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)AbyssEngine::AEMath::operator*(aVStack_a4,fVar7);
    AbyssEngine::AEMath::operator*(aVStack_98,fVar7);
    AbyssEngine::AEMath::operator+((Vector *)aMStack_40,aVStack_8c);
    AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_f8,(Vector *)&local_80);
    uStack_c8 = DAT_000b0688;
    uStack_d0 = DAT_000b0680;
    uStack_dc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_d8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_d4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar8 = (undefined4 *)((uint)&local_f8 | 4);
    *puVar8 = 0;
    puVar8[1] = uStack_dc;
    puVar8[2] = uStack_d8;
    puVar8[3] = uStack_d4;
    break;
  case (int *)0x3:
    pSVar15 = this[0xc].m_pLevel;
    getPosition();
    KIPlayer::getNearestNavigationPoint((Vector *)pSVar15,(SpacePoint *)&local_80);
    (*(code *)(this[0xc].m_pLevel)->field_0[10])((Vector *)&local_f8);
    pVVar6 = (Vector *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::MatrixRotateVector(aMStack_40,pVVar6);
    AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)&local_f8);
    ppiVar20 = &this[6].field_14;
    AbyssEngine::AEMath::Vector::operator=((Vector *)ppiVar20,(Vector *)&local_80);
    getPosition();
    AbyssEngine::AEMath::operator-((Vector *)&local_80,(Vector *)ppiVar20);
    fVar7 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_80);
    if ((int)fVar7 < 200) {
      *(undefined1 *)((int)&this[9].m_pPlayer + 1) = 0;
      *(undefined1 *)((int)&this[0x17].field_18 + 2) = 0;
      this[0xc].m_pLevel = (SingleLevel *)0x0;
      *(undefined4 *)(param_3 + 4) = 0;
      *(undefined4 *)(param_3 + 8) = 0;
      local_80 = 0;
      local_7c = 0;
      local_78 = 0;
      AbyssEngine::AEMath::Vector::operator=((Vector *)&this[0xc].field_18,(Vector *)&local_80);
      TargetFollowCamera::setActive((TargetFollowCamera *)this[3].field_1C,true);
      TargetFollowCamera::setLookAtCam(SUB41(this[3].field_1C,0));
      TargetFollowCamera::useTargetsUpVector(SUB41(this[3].field_1C,0));
      LevelScript::resetCamera((Level *)this->field_10);
      Player::resetGunDelay((Player *)this->m_pPlayer,0);
      pSVar15 = this[0x18].m_pLevel;
      this[0xc].field_14 = (int *)0x0;
      *(undefined1 *)&this[4].field_20 = 1;
      if (pSVar15 != (SingleLevel *)0x0) {
        SpacePoint::giveFree();
        this[0x18].m_pLevel = (SingleLevel *)0x0;
      }
      if ((HackingGame *)this[0xd].field_14 != (HackingGame *)0x0) {
        pvVar10 = (void *)HackingGame::~HackingGame((HackingGame *)this[0xd].field_14);
        operator_delete(pvVar10);
        this[0xd].field_14 = (int *)0x0;
        Hud::setHackingGameActive(param_1,false);
      }
      goto switchD_000afcf2_caseD_1;
    }
    fVar7 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::EaseInOutMatrix::Increase(fVar7);
    AbyssEngine::EaseInOutMatrix::GetValue();
    getPosition();
    getPosition();
    AbyssEngine::AEMath::operator-(aVStack_b0,(Vector *)ppiVar20);
    fVar7 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)AbyssEngine::AEMath::operator*(aVStack_a4,fVar7);
    AbyssEngine::AEMath::operator*(aVStack_98,fVar7);
    AbyssEngine::AEMath::operator+((Vector *)aMStack_40,aVStack_8c);
    AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_f8,(Vector *)&local_80);
    uStack_c8 = DAT_000b0688;
    uStack_d0 = DAT_000b0680;
    uStack_dc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_d8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_d4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar8 = (undefined4 *)((uint)&local_f8 | 4);
    *puVar8 = 0;
    puVar8[1] = uStack_dc;
    puVar8[2] = uStack_d8;
    puVar8[3] = uStack_d4;
  }
  local_e0 = 0;
  local_f8 = 0x3f800000;
  local_e4 = 0x3f800000;
  local_c0 = 0x3f800000;
  AEGeometry::setMatrix((Matrix *)this->field_4);
  AEGeometry::setMatrix((Matrix *)this->field_8);
  pMVar9 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this->m_pPlayer + 4),pMVar9);
  AEGeometry::getPosition();
  AbyssEngine::AEMath::Vector::operator=((Vector *)&this[9].field_4,(Vector *)aMStack_40);
switchD_000afcf2_caseD_1:
  iVar3 = *piVar22 - local_34;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  000afcb8: push {r4,r5,r6,r7,lr}
  000afcba: add r7,sp,#0xc
  000afcbc: push {r7,r8,r9,r10,r11}
  000afcc0: vpush {d8}
  000afcc4: sub sp,#0x188
  000afcc6: mov r4,r0
  000afcc8: ldr r0,[0x000b0020]
  000afcca: mov r6,r3
  000afccc: mov r9,r1
  000afcce: add r0,pc
  000afcd0: ldr.w r11,[r0,#0x0]
  000afcd4: ldr.w r0,[r11,#0x0]
  000afcd8: str r0,[sp,#0x184]
  000afcda: ldr.w r0,[r4,#0x1bc]
  000afcde: blx 0x00071f50
  000afce2: cbz r0,0x000afce8
  000afce4: movs r5,#0x0
  000afce6: b 0x000afcfe
  000afce8: ldr.w r0,[r4,#0x1c4]
  000afcec: cmp r0,#0x3
  000afcee: bhi 0x000afce4
  000afcf0: movs r5,#0x1
  000afcf2: tbh [pc,r0]
  000afcfe: ldr r0,[sp,#0x184]
  000afd00: ldr.w r1,[r11,#0x0]
  000afd04: subs r0,r1,r0
  000afd06: itttt eq
  000afd08: mov.eq r0,r5
  000afd0a: add.eq sp,#0x188
  000afd0c: vpop.eq {d8}
  000afd10: add.eq sp,#0x4
  000afd12: itt eq
  000afd14: pop.eq.w {r8,r9,r10,r11}
  000afd18: pop.eq {r4,r5,r6,r7,pc}
  000afd1a: blx 0x0006e824
  000afd1e: add r6,sp,#0x138
  000afd20: mov r1,r4
  000afd22: ldr.w r5,[r4,#0x1bc]
  000afd26: mov r0,r6
  000afd28: blx 0x0007264c
  000afd2c: ldr.w r2,[r4,#0x36c]
  000afd30: mov r0,r5
  000afd32: mov r1,r6
  000afd34: blx 0x00072b5c
  000afd38: mov r5,r0
  000afd3a: cmp r0,#0x0
  000afd3c: beq 0x000afce4
  000afd3e: ldr.w r0,[r4,#0x36c]
  000afd42: cmp r0,r5
  000afd44: beq 0x000afd56
  000afd46: cbz r0,0x000afd4c
  000afd48: blx 0x00072b80
  000afd4c: mov r0,r5
  000afd4e: str.w r5,[r4,#0x36c]
  000afd52: blx 0x00072b8c
  000afd56: ldr.w r1,[r4,#0x1bc]
  000afd5a: add.w r8,sp,#0xc0
  000afd5e: ldr r0,[r1,#0x0]
  000afd60: ldr r2,[r0,#0x28]
  000afd62: mov r0,r8
  000afd64: blx r2
  000afd66: ldr.w r0,[r4,#0x1bc]
  000afd6a: ldr r0,[r0,#0x8]
  000afd6c: blx 0x000721cc
  000afd70: add r6,sp,#0x178
  000afd72: mov r1,r0
  000afd74: mov r2,r5
  000afd76: mov r0,r6
  000afd78: blx 0x0006f694
  000afd7c: add r5,sp,#0x138
  000afd7e: mov r1,r8
  000afd80: mov r2,r6
  000afd82: mov r0,r5
  000afd84: blx 0x0006f1cc
  000afd88: add.w r6,r4,#0xec
  000afd8c: mov r1,r5
  000afd8e: mov r0,r6
  000afd90: blx 0x0006eb3c
  000afd94: add.w r8,sp,#0xc0
  000afd98: mov r1,r4
  000afd9a: mov r0,r8
  000afd9c: blx 0x0007264c
  000afda0: add r5,sp,#0x138
  000afda2: mov r1,r6
  000afda4: mov r2,r8
  000afda6: mov r0,r5
  000afda8: blx 0x0006ec38
  000afdac: mov r0,r5
  000afdae: blx 0x0006ec44
  000afdb2: vmov s0,r0
  000afdb6: ldr.w r0,[r4,#0x1d8]
  000afdba: vcvt.s32.f32 s0,s0
  000afdbe: vmov r5,s0
  000afdc2: cmp r0,r5
  000afdc4: ble.w 0x000b04e0
  000afdc8: ldr.w r0,[r4,#0x1bc]
  000afdcc: ldrb.w r1,[r0,#0x70]
  000afdd0: cbz r1,0x000afdd8
  000afdd2: movs r1,#0x0
  000afdd4: strb.w r1,[r0,#0x8c]
  000afdd8: ldr.w r0,[r4,#0x88]
  000afddc: movs r1,#0x2
  000afdde: str.w r1,[r4,#0x1c4]
  000afde2: movs r1,#0x1
  000afde4: blx 0x00072670
  000afde8: ldr.w r0,[r4,#0x88]
  000afdec: movs r1,#0x0
  000afdee: movs r5,#0x0
  000afdf0: blx 0x0007267c
  000afdf4: ldr.w r0,[r4,#0x358]
  000afdf8: cbz r0,0x000afe02
  000afdfa: blx 0x00072340
  000afdfe: blx 0x0006eb48
  000afe02: ldr.w r6,[r4,#0x1bc]
  000afe06: str.w r5,[r4,#0x358]
  000afe0a: ldr r0,[r6,#0x0]
  000afe0c: mov r1,r6
  000afe0e: ldr r2,[r0,#0x28]
  000afe10: add r0,sp,#0xc0
  000afe12: blx r2
  000afe14: ldr r1,[0x000b0024]
  000afe16: ldr.w r0,[r4,#0x1bc]
  000afe1a: add r1,pc
  000afe1c: ldr r0,[r0,#0x8]
  000afe1e: ldr.w r10,[r1,#0x0]
  000afe22: blx r10
  000afe24: mov r1,r0
  000afe26: ldr r0,[0x000b0028]
  000afe28: ldr.w r2,[r4,#0x36c]
  000afe2c: add.w r9,sp,#0x178
  000afe30: add r0,pc
  000afe32: ldr.w r8,[r0,#0x0]
  000afe36: mov r0,r9
  000afe38: blx r8
  000afe3a: add r5,sp,#0x138
  000afe3c: add r1,sp,#0xc0
  000afe3e: mov r2,r9
  000afe40: mov r0,r5
  000afe42: blx 0x0006f1cc
  000afe46: mov r0,r6
  000afe48: mov r1,r5
  000afe4a: blx 0x00072b98
  000afe4e: mov r6,r0
  000afe50: ldr.w r0,[r4,#0x1bc]
  000afe54: ldr r0,[r0,#0x8]
  000afe56: blx r10
  000afe58: add r5,sp,#0x138
  000afe5a: add.w r2,r6,#0xc
  000afe5e: mov r1,r0
  000afe60: mov r0,r5
  000afe62: blx r8
  000afe64: ldr r0,[0x000b002c]
  000afe66: mov r1,r5
  000afe68: add r0,pc
  000afe6a: ldr.w r9,[r0,#0x0]
  000afe6e: add r0,sp,#0x178
  000afe70: blx r9
  000afe72: ldr.w r0,[r4,#0x1bc]
  000afe76: ldr r0,[r0,#0x8]
  000afe78: blx r10
  000afe7a: ldr.w r2,[r4,#0x36c]
  000afe7e: mov r1,r0
  000afe80: add r0,sp,#0xc0
  000afe82: blx r8
  000afe84: ldr.w r0,[r4,#0x1bc]
  000afe88: ldr r0,[r0,#0x8]
  000afe8a: blx r10
  000afe8c: add r5,sp,#0x120
  000afe8e: mov r1,r0
  000afe90: mov r2,r6
  000afe92: mov r0,r5
  000afe94: blx r8
  000afe96: add r6,sp,#0x138
  000afe98: add r1,sp,#0xc0
  000afe9a: mov r2,r5
  000afe9c: mov r0,r6
  000afe9e: blx 0x0006ec38
  000afea2: add.w r8,sp,#0x12c
  000afea6: mov r1,r6
  000afea8: mov r0,r8
  000afeaa: blx r9
  000afeac: add r5,sp,#0x138
  000afeae: add r2,sp,#0x178
  000afeb0: mov r1,r8
  000afeb2: mov r0,r5
  000afeb4: blx 0x0006ec8c
  000afeb8: add r6,sp,#0x120
  000afeba: mov r1,r5
  000afebc: mov r0,r6
  000afebe: blx r9
  000afec0: adr.w r0,0xb0680
  000afec4: vmov.i32 q8,#0x0
  000afec8: vld1.64 {d18,d19},[r0]
  000afecc: orr r0,r5,#0x4
  000afed0: mov r1,r5
  000afed2: mov r2,r6
  000afed4: vst1.32 {d16,d17},[r0]
  000afed8: add.w r0,r5,#0x18
  000afedc: mov r3,r8
  000afede: vst1.64 {d16,d17},[r0]
  000afee2: add.w r0,r5,#0x28
  000afee6: vst1.64 {d18,d19},[r0]
  000afeea: mov.w r0,#0x3f800000
  000afeee: str r0,[sp,#0x138]
  000afef0: str r0,[sp,#0x14c]
  000afef2: str r0,[sp,#0x170]
  000afef4: add r0,sp,#0x178
  000afef6: str r0,[sp,#0x0]
  000afef8: add r0,sp,#0xc0
  000afefa: blx 0x000701bc
  000afefe: movs r0,#0xf4
  000aff00: blx 0x0006eb24
  000aff04: mov r5,r0
  000aff06: ldr r0,[r4,#0x8]
  000aff08: blx r10
  000aff0a: ldr r1,[r0,#0x0]
  000aff0c: add.w r10,sp,#0x150
  000aff10: str r1,[sp,#0xb8]
  000aff12: add.w lr,sp,#0x15c
  000aff16: ldr r1,[r0,#0x4]
  000aff18: add r2,sp,#0x168
  000aff1a: str r1,[sp,#0xb4]
  000aff1c: ldr r1,[r0,#0x8]
  000aff1e: str r1,[sp,#0xb0]
  000aff20: ldr r1,[r0,#0xc]
  000aff22: str r1,[sp,#0xac]
  000aff24: ldr r1,[r0,#0x10]
  000aff26: str r1,[sp,#0xa8]
  000aff28: ldr r1,[r0,#0x14]
  000aff2a: str r1,[sp,#0xa4]
  000aff2c: ldr r1,[r0,#0x18]
  000aff2e: str r1,[sp,#0xa0]
  000aff30: ldr r1,[r0,#0x1c]
  000aff32: str r1,[sp,#0x9c]
  000aff34: ldr r1,[r0,#0x20]
  000aff36: str r1,[sp,#0x98]
  000aff38: ldr r1,[r0,#0x24]
  000aff3a: str r1,[sp,#0x94]
  000aff3c: ldr r1,[r0,#0x28]
  000aff3e: str r1,[sp,#0x90]
  000aff40: ldr r1,[r0,#0x2c]
  000aff42: str r1,[sp,#0x8c]
  000aff44: ldr r1,[r0,#0x30]
  000aff46: str r1,[sp,#0x88]
  000aff48: ldr r1,[r0,#0x34]
  000aff4a: ldr r0,[r0,#0x38]
  000aff4c: str r0,[sp,#0x80]
  000aff4e: ldr r0,[sp,#0x138]
  000aff50: str r0,[sp,#0x7c]
  000aff52: ldr r0,[sp,#0x13c]
  000aff54: str r0,[sp,#0x78]
  000aff56: ldr r0,[sp,#0x140]
  000aff58: str r0,[sp,#0x74]
  000aff5a: ldr r0,[sp,#0x144]
  000aff5c: str r1,[sp,#0x84]
  000aff5e: str r0,[sp,#0x70]
  000aff60: ldrd r8,r9,[sp,#0x148]
  000aff64: str r5,[sp,#0xbc]
  000aff66: ldm.w r10,{r5,r6,r10}
  000aff6a: ldm.w lr,{r3,r12,lr}
  000aff6e: ldmia r2,{r0,r1,r2}
  000aff70: str r0,[sp,#0x60]
  000aff72: mov.w r0,#0x7d0
  000aff76: str r0,[sp,#0x6c]
  000aff78: add r0,sp,#0x54
  000aff7a: str r1,[sp,#0x64]
  000aff7c: str r2,[sp,#0x68]
  000aff7e: strd r8,r9,[sp,#0x40]
  000aff82: str r5,[sp,#0x48]
  000aff84: ldr r5,[sp,#0xbc]
  000aff86: strd r6,r10,[sp,#0x4c]
  000aff8a: stm r0,{r3,r12,lr}
  000aff8e: ldr r0,[sp,#0x8c]
  000aff90: str r0,[sp,#0x20]
  000aff92: ldr r0,[sp,#0x88]
  000aff94: str r0,[sp,#0x24]
  000aff96: ldr r0,[sp,#0x84]
  000aff98: str r0,[sp,#0x28]
  000aff9a: ldr r0,[sp,#0x80]
  000aff9c: str r0,[sp,#0x2c]
  000aff9e: ldr r0,[sp,#0x7c]
  000affa0: str r0,[sp,#0x30]
  000affa2: ldr r0,[sp,#0x78]
  000affa4: str r0,[sp,#0x34]
  000affa6: ldr r0,[sp,#0x74]
  000affa8: str r0,[sp,#0x38]
  000affaa: ldr r0,[sp,#0x70]
  000affac: str r0,[sp,#0x3c]
  000affae: ldr r0,[sp,#0xac]
  000affb0: str r0,[sp,#0x0]
  000affb2: ldr r0,[sp,#0xa8]
  000affb4: str r0,[sp,#0x4]
  000affb6: ldr r0,[sp,#0xa4]
  000affb8: str r0,[sp,#0x8]
  000affba: ldr r0,[sp,#0xa0]
  000affbc: str r0,[sp,#0xc]
  000affbe: ldr r0,[sp,#0x9c]
  000affc0: str r0,[sp,#0x10]
  000affc2: ldr r0,[sp,#0x98]
  000affc4: str r0,[sp,#0x14]
  000affc6: ldr r0,[sp,#0x94]
  000affc8: str r0,[sp,#0x18]
  000affca: ldr r0,[sp,#0x90]
  000affcc: str r0,[sp,#0x1c]
  000affce: mov r0,r5
  000affd0: ldr r1,[sp,#0xb8]
  000affd2: ldrd r3,r2,[sp,#0xb0]
  000affd6: blx 0x00072b74
  000affda: ldr r0,[0x000b0030]
  000affdc: movw r1,#0x8de
  000affe0: str.w r5,[r4,#0x358]
  000affe4: movs r5,#0x0
  000affe6: add r0,pc
  000affe8: movs r2,#0x0
  000affea: movs r3,#0x0
  000affec: str r5,[sp,#0x0]
  000affee: ldr r6,[r0,#0x0]
  000afff0: ldr r0,[r6,#0x0]
  000afff2: blx 0x00071548
  000afff6: strd r5,r5,[sp,#0xc0]
  000afffa: add.w r8,sp,#0x114
  000afffe: str r5,[sp,#0xc8]
  000b0000: ldr r1,[r4,#0x0]
  000b0002: mov r0,r8
  000b0004: blx 0x00072964
  000b0008: ldr r0,[r6,#0x0]
  000b000a: add r3,sp,#0xc0
  000b000c: movw r1,#0x8de
  000b0010: mov r2,r8
  000b0012: str r5,[sp,#0x0]
  000b0014: blx 0x000726e8
  000b0018: mov r0,r4
  000b001a: blx 0x00072ba4
  000b001e: b 0x000afce4
  000b0034: ldrb.w r0,[r4,#0x1a0]
  000b0038: movs r1,#0x1
  000b003a: strb.w r1,[r4,#0x330]
  000b003e: cbz r0,0x000b0048
  000b0040: mov r0,r4
  000b0042: movs r1,#0x0
  000b0044: blx 0x00072b08
  000b0048: ldr.w r5,[r4,#0x1bc]
  000b004c: add.w r8,sp,#0xc0
  000b0050: ldr r0,[r5,#0x0]
  000b0052: mov r1,r5
  000b0054: ldr r2,[r0,#0x28]
  000b0056: mov r0,r8
  000b0058: blx r2
  000b005a: ldr.w r0,[r4,#0x1bc]
  000b005e: ldr r0,[r0,#0x8]
  000b0060: blx 0x000721cc
  000b0064: ldr.w r2,[r4,#0x36c]
  000b0068: add.w r10,sp,#0x178
  000b006c: mov r1,r0
  000b006e: mov r0,r10
  000b0070: blx 0x0006f694
  000b0074: add r6,sp,#0x138
  000b0076: mov r1,r8
  000b0078: mov r2,r10
  000b007a: mov r0,r6
  000b007c: blx 0x0006f1cc
  000b0080: mov r0,r5
  000b0082: mov r1,r6
  000b0084: blx 0x00072b98
  000b0088: ldr.w r1,[r4,#0x1bc]
  000b008c: mov r5,r0
  000b008e: add.w r8,sp,#0xc0
  000b0092: ldr r0,[r1,#0x0]
  000b0094: ldr r2,[r0,#0x28]
  000b0096: mov r0,r8
  000b0098: blx r2
  000b009a: ldr.w r0,[r4,#0x1bc]
  000b009e: ldr r0,[r0,#0x8]
  000b00a0: blx 0x000721cc
  000b00a4: add r6,sp,#0x178
  000b00a6: mov r1,r0
  000b00a8: mov r2,r5
  000b00aa: mov r0,r6
  000b00ac: blx 0x0006f694
  000b00b0: add r5,sp,#0x138
  000b00b2: mov r1,r8
  000b00b4: mov r2,r6
  000b00b6: mov r0,r5
  000b00b8: blx 0x0006f1cc
  000b00bc: add.w r10,r4,#0xec
  000b00c0: mov r1,r5
  000b00c2: mov r0,r10
  000b00c4: blx 0x0006eb3c
  000b00c8: add r5,sp,#0xc0
  000b00ca: mov r1,r4
  000b00cc: mov r0,r5
  000b00ce: blx 0x0007264c
  000b00d2: add r6,sp,#0x138
  000b00d4: mov r1,r10
  000b00d6: mov r2,r5
  000b00d8: mov r0,r6
  000b00da: blx 0x0006ec38
  000b00de: mov r0,r6
  000b00e0: blx 0x0006ec44
  000b00e4: ldr r1,[0x000b0498]
  000b00e6: vmov s16,r0
  000b00ea: ldr r6,[0x000b049c]
  000b00ec: add r1,pc
  000b00ee: add r6,pc
  000b00f0: ldr r5,[r1,#0x0]
  000b00f2: ldr r1,[r5,#0x0]
  000b00f4: mov r0,r1
  000b00f6: blx 0x00071a58
  000b00fa: blx 0x000719c8
  000b00fe: vcvt.s32.f32 s0,s16
  000b0102: ldr.w r0,[r6,r0,lsl #0x2]
  000b0106: vmov r1,s0
  000b010a: cmp r0,r1
  000b010c: ble.w 0x000b02c2
  000b0110: ldr r0,[r5,#0x0]
  000b0112: blx 0x00072850
  000b0116: mov r6,r0
  000b0118: ldr r0,[r5,#0x0]
  000b011a: blx 0x0007285c
  000b011e: mov r8,r0
  000b0120: mov r0,r6
  000b0122: blx 0x00072868
  000b0126: cmp r0,#0x0
  000b0128: bne.w 0x000b042e
  000b012c: mov r0,r6
  000b012e: blx 0x00072874
  000b0132: cmp r0,#0xf
  000b0134: bne.w 0x000b042e
  000b0138: ldr.w r0,[r4,#0x1bc]
  000b013c: blx 0x00072880
  000b0140: cmp r0,#0x1
  000b0142: bne.w 0x000b042e
  000b0146: ldr r0,[r5,#0x0]
  000b0148: blx 0x00071a58
  000b014c: mov r8,r0
  000b014e: mov r0,r6
  000b0150: blx 0x000728a4
  000b0154: mov r1,r0
  000b0156: mov r0,r8
  000b0158: movs r2,#0x1
  000b015a: blx 0x00071a70
  000b015e: cmp r0,#0x0
  000b0160: beq.w 0x000b0654
  000b0164: ldr r0,[r5,#0x0]
  000b0166: blx 0x00071a58
  000b016a: mov r5,r0
  000b016c: mov r0,r6
  000b016e: blx 0x000728a4
  000b0172: mov r1,r0
  000b0174: mov r0,r5
  000b0176: blx 0x00072424
  000b017a: blx 0x0007183c
  000b017e: str.w r0,[r4,#0x360]
  000b0182: mov r0,r6
  000b0184: blx 0x00072bb0
  000b0188: mov r5,r0
  000b018a: ldr r0,[r4,#0xc]
  000b018c: blx 0x00071f38
  000b0190: b 0x000b05c2
  000b0192: add.w r8,sp,#0x138
  000b0196: mov r1,r4
  000b0198: ldr.w r5,[r4,#0x1bc]
  000b019c: mov r0,r8
  000b019e: blx 0x0007264c
  000b01a2: ldr.w r2,[r4,#0x36c]
  000b01a6: mov r0,r5
  000b01a8: mov r1,r8
  000b01aa: blx 0x00072b5c
  000b01ae: ldr.w r1,[r4,#0x1bc]
  000b01b2: mov r5,r0
  000b01b4: add.w r8,sp,#0xc0
  000b01b8: ldr r0,[r1,#0x0]
  000b01ba: ldr r2,[r0,#0x28]
  000b01bc: mov r0,r8
  000b01be: blx r2
  000b01c0: ldr.w r0,[r4,#0x1bc]
  000b01c4: ldr r0,[r0,#0x8]
  000b01c6: blx 0x000721cc
  000b01ca: add.w r10,sp,#0x178
  000b01ce: mov r1,r0
  000b01d0: mov r2,r5
  000b01d2: mov r0,r10
  000b01d4: blx 0x0006f694
  000b01d8: add r5,sp,#0x138
  000b01da: mov r1,r8
  000b01dc: mov r2,r10
  000b01de: mov r0,r5
  000b01e0: blx 0x0006f1cc
  000b01e4: add.w r10,r4,#0xec
  000b01e8: mov r1,r5
  000b01ea: mov r0,r10
  000b01ec: blx 0x0006eb3c
  000b01f0: add.w r8,sp,#0xc0
  000b01f4: mov r1,r4
  000b01f6: mov r0,r8
  000b01f8: blx 0x0007264c
  000b01fc: add r5,sp,#0x138
  000b01fe: mov r1,r10
  000b0200: mov r2,r8
  000b0202: mov r0,r5
  000b0204: blx 0x0006ec38
  000b0208: mov r0,r5
  000b020a: blx 0x0006ec44
  000b020e: vmov s0,r0
  000b0212: vcvt.s32.f32 s0,s0
  000b0216: vmov r0,s0
  000b021a: cmp r0,#0xc8
  000b021c: blt.w 0x000b03a2
  000b0220: vldr.32 s0,[r4,#0x134]
  000b0224: vcvt.f32.s32 s0,s0
  000b0228: ldr.w r0,[r4,#0x358]
  000b022c: vmov r1,s0
  000b0230: blx 0x00072bbc
  000b0234: add.w r8,sp,#0x138
  000b0238: ldr.w r1,[r4,#0x358]
  000b023c: mov r0,r8
  000b023e: blx 0x00072bc8
  000b0242: add.w r9,sp,#0x12c
  000b0246: mov r1,r4
  000b0248: mov r0,r9
  000b024a: blx 0x0007264c
  000b024e: add r5,sp,#0xfc
  000b0250: mov r1,r4
  000b0252: mov r0,r5
  000b0254: blx 0x0007264c
  000b0258: add r6,sp,#0x108
  000b025a: mov r1,r10
  000b025c: mov r2,r5
  000b025e: mov r0,r6
  000b0260: blx 0x0006ec38
  000b0264: vldr.32 s0,[r4,#0x134]
  000b0268: add r5,sp,#0x114
  000b026a: mov r1,r6
  000b026c: vcvt.f32.s32 s0,s0
  000b0270: mov r0,r5
  000b0272: vmov r2,s0
  000b0276: blx 0x0006ec74
  000b027a: add r6,sp,#0x120
  000b027c: ldr r2,[0x000b04a0]
  000b027e: mov r1,r5
  000b0280: mov r0,r6
  000b0282: blx 0x0006ec74
  000b0286: add r5,sp,#0x178
  000b0288: mov r1,r9
  000b028a: mov r2,r6
  000b028c: mov r0,r5
  000b028e: blx 0x0006f1cc
  000b0292: add r6,sp,#0xc0
  000b0294: mov r1,r8
  000b0296: mov r2,r5
  000b0298: mov r0,r6
  000b029a: blx 0x0006f1d8
  000b029e: adr.w r0,0xb0680
  000b02a2: vmov.i32 q8,#0x0
  000b02a6: vld1.64 {d18,d19},[r0]
  000b02aa: orr r0,r6,#0x4
  000b02ae: mov r1,r6
  000b02b0: vst1.32 {d16,d17},[r0]
  000b02b4: add.w r0,r6,#0x18
  000b02b8: vst1.64 {d16,d17},[r0]
  000b02bc: add.w r0,r6,#0x28
  000b02c0: b 0x000b0360
  000b02c2: vldr.32 s0,[r4,#0x134]
  000b02c6: vcvt.f32.s32 s0,s0
  000b02ca: ldr.w r0,[r4,#0x358]
  000b02ce: vmov r1,s0
  000b02d2: blx 0x00072bbc
  000b02d6: add.w r8,sp,#0x138
  000b02da: ldr.w r1,[r4,#0x358]
  000b02de: mov r0,r8
  000b02e0: blx 0x00072bc8
  000b02e4: add.w r9,sp,#0x12c
  000b02e8: mov r1,r4
  000b02ea: mov r0,r9
  000b02ec: blx 0x0007264c
  000b02f0: add r6,sp,#0xfc
  000b02f2: mov r1,r4
  000b02f4: mov r0,r6
  000b02f6: blx 0x0007264c
  000b02fa: add r5,sp,#0x108
  000b02fc: mov r1,r10
  000b02fe: mov r2,r6
  000b0300: mov r0,r5
  000b0302: blx 0x0006ec38
  000b0306: vldr.32 s0,[r4,#0x134]
  000b030a: add r6,sp,#0x114
  000b030c: mov r1,r5
  000b030e: vcvt.f32.s32 s0,s0
  000b0312: mov r0,r6
  000b0314: vmov r2,s0
  000b0318: blx 0x0006ec74
  000b031c: add r5,sp,#0x120
  000b031e: ldr r2,[0x000b0690]
  000b0320: mov r1,r6
  000b0322: mov r0,r5
  000b0324: blx 0x0006ec74
  000b0328: add r6,sp,#0x178
  000b032a: mov r1,r9
  000b032c: mov r2,r5
  000b032e: mov r0,r6
  000b0330: blx 0x0006f1cc
  000b0334: add r5,sp,#0xc0
  000b0336: mov r1,r8
  000b0338: mov r2,r6
  000b033a: mov r0,r5
  000b033c: blx 0x0006f1d8
  000b0340: adr r0,[0xb0680]
  000b0342: vmov.i32 q8,#0x0
  000b0346: vld1.64 {d18,d19},[r0]
  000b034a: orr r0,r5,#0x4
  000b034e: mov r1,r5
  000b0350: vst1.32 {d16,d17},[r0]
  000b0354: add.w r0,r5,#0x18
  000b0358: vst1.64 {d16,d17},[r0]
  000b035c: add.w r0,r5,#0x28
  000b0360: vst1.64 {d18,d19},[r0]
  000b0364: mov.w r0,#0x3f800000
  000b0368: str r0,[sp,#0xc0]
  000b036a: str r0,[sp,#0xd4]
  000b036c: str r0,[sp,#0xf8]
  000b036e: ldr r0,[r4,#0x4]
  000b0370: blx 0x000721e4
  000b0374: ldr r0,[r4,#0x8]
  000b0376: mov r1,r8
  000b0378: blx 0x000721e4
  000b037c: ldr r0,[r4,#0x8]
  000b037e: blx 0x000721cc
  000b0382: mov r1,r0
  000b0384: ldr r0,[r4,#0x0]
  000b0386: adds r0,#0x4
  000b0388: blx 0x0006f148
  000b038c: add r5,sp,#0x178
  000b038e: ldr r1,[r4,#0x8]
  000b0390: mov r0,r5
  000b0392: blx 0x000720b8
  000b0396: add.w r0,r4,#0x148
  000b039a: mov r1,r5
  000b039c: blx 0x0006eb3c
  000b03a0: b 0x000afce4
  000b03a2: add.w r0,r4,#0x1c8
  000b03a6: add r1,sp,#0x138
  000b03a8: movs r5,#0x0
  000b03aa: strb.w r5,[r4,#0x145]
  000b03ae: strb.w r5,[r4,#0x356]
  000b03b2: str.w r5,[r4,#0x1bc]
  000b03b6: strd r5,r5,[r6,#0x4]
  000b03ba: strd r5,r5,[sp,#0x138]
  000b03be: str r5,[sp,#0x140]
  000b03c0: blx 0x0006eb3c
  000b03c4: ldr.w r0,[r4,#0x88]
  000b03c8: movs r1,#0x1
  000b03ca: movs r6,#0x1
  000b03cc: blx 0x00072acc
  000b03d0: ldr.w r0,[r4,#0x88]
  000b03d4: movs r1,#0x0
  000b03d6: blx 0x00072670
  000b03da: ldr.w r0,[r4,#0x88]
  000b03de: movs r1,#0x0
  000b03e0: blx 0x0007267c
  000b03e4: ldrd r1,r0,[r4,#0xc]
  000b03e8: blx 0x0007246c
  000b03ec: ldr r0,[r4,#0x0]
  000b03ee: movs r1,#0x0
  000b03f0: blx 0x00072ad8
  000b03f4: ldr.w r0,[r4,#0x36c]
  000b03f8: str.w r5,[r4,#0x1c4]
  000b03fc: cmp r0,#0x0
  000b03fe: strb.w r6,[r4,#0xb0]
  000b0402: beq 0x000b040c
  000b0404: blx 0x00072b80
  000b0408: str.w r5,[r4,#0x36c]
  000b040c: ldr.w r0,[r4,#0x1e8]
  000b0410: movs r5,#0x0
  000b0412: cmp r0,#0x0
  000b0414: beq.w 0x000afcfe
  000b0418: blx 0x0007273c
  000b041c: blx 0x0006eb48
  000b0420: mov r0,r9
  000b0422: movs r1,#0x0
  000b0424: str.w r5,[r4,#0x1e8]
  000b0428: blx 0x0007270c
  000b042c: b 0x000afcfe
  000b042e: mov r0,r6
  000b0430: blx 0x00072868
  000b0434: cbnz r0,0x000b04a4
  000b0436: mov r0,r6
  000b0438: blx 0x00072874
  000b043c: cmp r0,#0xb8
  000b043e: beq 0x000b044a
  000b0440: mov r0,r6
  000b0442: blx 0x00072874
  000b0446: cmp r0,#0xa8
  000b0448: bne 0x000b04a4
  000b044a: ldr.w r0,[r4,#0x1bc]
  000b044e: blx 0x00072880
  000b0452: cmp r0,#0x2
  000b0454: bne 0x000b04a4
  000b0456: ldr r0,[r5,#0x0]
  000b0458: blx 0x00071a58
  000b045c: blx 0x00072bd4
  000b0460: cmp r0,#0x1
  000b0462: blt 0x000b0476
  000b0464: ldr r0,[r5,#0x0]
  000b0466: ldr.w r8,[r0,#0x178]
  000b046a: blx 0x00071a58
  000b046e: blx 0x00072bd4
  000b0472: cmp r8,r0
  000b0474: blt 0x000b0482
  000b0476: mov r0,r6
  000b0478: blx 0x00072874
  000b047c: cmp r0,#0xa8
  000b047e: bne.w 0x000b05e6
  000b0482: mov r0,r6
  000b0484: blx 0x00072874
  000b0488: cmp r0,#0xa8
  000b048a: bne.w 0x000b05f8
  000b048e: mov r0,r6
  000b0490: blx 0x0007288c
  000b0494: b 0x000b0602
  000b04a4: mov r0,r6
  000b04a6: blx 0x00072868
  000b04aa: cmp r0,#0x0
  000b04ac: bne 0x000b0548
  000b04ae: mov r0,r6
  000b04b0: blx 0x00072874
  000b04b4: cmp r0,#0xb8
  000b04b6: bne 0x000b0548
  000b04b8: ldr.w r0,[r4,#0x1bc]
  000b04bc: blx 0x00072880
  000b04c0: cmp r0,#0x1
  000b04c2: bne 0x000b0548
  000b04c4: ldr r0,[r5,#0x0]
  000b04c6: ldr.w r0,[r0,#0x178]
  000b04ca: cmp r0,#0x1
  000b04cc: blt.w 0x000b0654
  000b04d0: movs r1,#0x0
  000b04d2: strd r1,r0,[r4,#0x35c]
  000b04d6: mov r0,r9
  000b04d8: str.w r1,[r4,#0x364]
  000b04dc: movs r1,#0x25
  000b04de: b 0x000b064c
  000b04e0: ldr r0,[0x000b0698]
  000b04e2: ldrd r9,r10,[r4,#0xec]
  000b04e6: add r0,pc
  000b04e8: ldr.w r8,[r4,#0xf4]
  000b04ec: ldr r6,[r0,#0x0]
  000b04ee: ldr r0,[r6,#0x0]
  000b04f0: blx 0x00071a58
  000b04f4: blx 0x000722a4
  000b04f8: vldr.32 s16,[pc,#0x198]
  000b04fc: vmov s2,r0
  000b0500: vmov.f32 s0,0x40800000
  000b0504: vadd.f32 s2,s2,s16
  000b0508: vcmpe.f32 s2,s0
  000b050c: vmrs apsr,fpscr
  000b0510: bpl 0x000b0524
  000b0512: ldr r0,[r6,#0x0]
  000b0514: blx 0x00071a58
  000b0518: blx 0x000722a4
  000b051c: vmov s0,r0
  000b0520: vadd.f32 s0,s0,s16
  000b0524: movs r6,#0x1
  000b0526: mov r0,r4
  000b0528: mov r1,r9
  000b052a: mov r2,r10
  000b052c: mov r3,r8
  000b052e: vstr.32 s0,[sp,#0x4]
  000b0532: str r6,[sp,#0x0]
  000b0534: blx 0x00072664
  000b0538: movw r0,#0x4e1f
  000b053c: cmp r5,r0
  000b053e: it le
  000b0540: strb.le.w r6,[r4,#0x330]
  000b0544: b.w 0x000afce4
  000b0548: cmp.w r8,#0x0
  000b054c: beq.w 0x000b0654
  000b0550: mov r0,r8
  000b0552: blx 0x00072868
  000b0556: cmp r0,#0x0
  000b0558: bne 0x000b0654
  000b055a: mov r0,r8
  000b055c: blx 0x00072874
  000b0560: cmp r0,#0xa7
  000b0562: beq 0x000b056e
  000b0564: mov r0,r8
  000b0566: blx 0x00072874
  000b056a: cmp r0,#0xae
  000b056c: bne 0x000b0654
  000b056e: ldr.w r0,[r4,#0x1bc]
  000b0572: blx 0x00072880
  000b0576: cmp r0,#0x1
  000b0578: bne 0x000b0654
  000b057a: ldr r0,[r5,#0x0]
  000b057c: blx 0x00071a58
  000b0580: mov r6,r0
  000b0582: mov r0,r8
  000b0584: blx 0x000728a4
  000b0588: mov r1,r0
  000b058a: mov r0,r6
  000b058c: movs r2,#0x1
  000b058e: blx 0x00071a70
  000b0592: cmp r0,#0x0
  000b0594: beq 0x000b0654
  000b0596: ldr r0,[r5,#0x0]
  000b0598: blx 0x00071a58
  000b059c: mov r5,r0
  000b059e: mov r0,r8
  000b05a0: blx 0x000728a4
  000b05a4: mov r1,r0
  000b05a6: mov r0,r5
  000b05a8: blx 0x00072424
  000b05ac: blx 0x0007183c
  000b05b0: str.w r0,[r4,#0x360]
  000b05b4: mov r0,r8
  000b05b6: blx 0x00072bb0
  000b05ba: mov r5,r0
  000b05bc: mov r0,r8
  000b05be: blx 0x0007288c
  000b05c2: ldr.w r1,[r4,#0x360]
  000b05c6: subs r0,r5,r0
  000b05c8: cmp r1,r0
  000b05ca: itt gt
  000b05cc: str.w.gt r0,[r4,#0x360]
  000b05d0: mov.gt r1,r0
  000b05d2: movs r0,#0x0
  000b05d4: cmp r1,#0x1
  000b05d6: str.w r0,[r4,#0x364]
  000b05da: str.w r0,[r4,#0x35c]
  000b05de: blt 0x000b0654
  000b05e0: mov r0,r9
  000b05e2: movs r1,#0x29
  000b05e4: b 0x000b064c
  000b05e6: ldr r0,[r5,#0x0]
  000b05e8: blx 0x00071a58
  000b05ec: blx 0x00072bd4
  000b05f0: cbnz r0,0x000b0654
  000b05f2: mov r0,r9
  000b05f4: movs r1,#0x2b
  000b05f6: b 0x000b064c
  000b05f8: ldr r0,[r5,#0x0]
  000b05fa: blx 0x00071a58
  000b05fe: blx 0x00072bd4
  000b0602: ldr r1,[r5,#0x0]
  000b0604: ldr.w r1,[r1,#0x178]
  000b0608: sub.w r8,r0,r1
  000b060c: mov r0,r6
  000b060e: str.w r8,[r4,#0x360]
  000b0612: blx 0x0007288c
  000b0616: ldr r1,[r5,#0x0]
  000b0618: ldr.w r1,[r1,#0x178]
  000b061c: subs r0,r0,r1
  000b061e: cmp r8,r0
  000b0620: ble 0x000b0636
  000b0622: mov r0,r6
  000b0624: blx 0x0007288c
  000b0628: ldr r1,[r5,#0x0]
  000b062a: ldr.w r1,[r1,#0x178]
  000b062e: subs r0,r0,r1
  000b0630: str.w r0,[r4,#0x360]
  000b0634: b 0x000b063a
  000b0636: ldr.w r0,[r4,#0x360]
  000b063a: movs r1,#0x0
  000b063c: cmp r0,#0x1
  000b063e: str.w r1,[r4,#0x364]
  000b0642: str.w r1,[r4,#0x35c]
  000b0646: blt 0x000b0654
  000b0648: mov r0,r9
  000b064a: movs r1,#0x23
  000b064c: mov r2,r4
  000b064e: movs r3,#0x0
  000b0650: blx 0x00072418
  000b0654: ldrb.w r1,[r4,#0xb2]
  000b0658: movs r0,#0x1
  000b065a: str.w r0,[r4,#0x1c4]
  000b065e: cbnz r1,0x000b0664
  000b0660: strb.w r0,[r4,#0xb1]
  000b0664: movs r0,#0x0
  000b0666: movs r5,#0x1
  000b0668: strb.w r0,[r4,#0xb2]
  000b066c: b.w 0x000afcfe
```
