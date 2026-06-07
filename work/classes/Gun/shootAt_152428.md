# Gun::shootAt
elf 0x152428 body 1714
Sig: undefined __stdcall shootAt(Matrix param_1, int param_2, Player * param_3, bool param_4)

## decompile
```c

/* Gun::shootAt(AbyssEngine::AEMath::Matrix, int, Player*, bool) */

void Gun::shootAt(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
                 undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                 undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16,
                 undefined4 param_17,undefined4 param_18)

{
  uint uVar1;
  byte bVar2;
  float fVar3;
  float fVar4;
  bool bVar5;
  bool bVar6;
  int *piVar7;
  Vector *pVVar8;
  undefined4 uVar9;
  TargetFollowCamera *this;
  int iVar10;
  Ship *pSVar11;
  int iVar12;
  int iVar13;
  Item *this_00;
  undefined4 *puVar14;
  uint uVar15;
  Vector *this_01;
  Player *this_02;
  code *pcVar16;
  code *pcVar17;
  code *pcVar18;
  uint in_fpscr;
  float fVar19;
  float fVar20;
  float fVar21;
  Matrix aMStack_b8 [12];
  Matrix aMStack_ac [12];
  Vector aVStack_a0 [12];
  undefined4 local_94;
  undefined4 uStack_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 local_54;
  undefined4 uStack_50;
  int local_4c;
  
  piVar7 = *(int **)(DAT_001627e8 + 0x162442);
  local_4c = *piVar7;
  local_7c = param_5;
  local_78 = param_6;
  local_74 = param_7;
  local_70 = param_8;
  local_6c = param_9;
  local_68 = param_10;
  local_64 = param_11;
  uStack_60 = param_12;
  local_5c = param_13;
  uStack_58 = param_14;
  local_54 = param_15;
  uStack_50 = param_16;
  *(undefined4 *)(param_1 + 4) = param_18;
  local_88 = param_2;
  uStack_84 = param_3;
  local_80 = param_4;
  if (*(char *)(param_1 + 0xf9) == '\0') {
    pSVar11 = (Ship *)Status::getShip();
    iVar10 = Ship::getEquipment(pSVar11);
    this_00 = *(Item **)(*(int *)(iVar10 + 4) + *(int *)(param_1 + 0xf4) * 4);
    if ((this_00 == (Item *)0x0) || (iVar10 = Item::getAmount(), iVar10 == 0)) {
      bVar6 = false;
      goto LAB_00162ae0;
    }
  }
  else {
    this_00 = (Item *)0x0;
  }
  fVar4 = DAT_001627f8;
  fVar3 = DAT_001627f4;
  fVar20 = DAT_001627f0;
  if (*(char *)(param_1 + 0x108) == '\0') {
    this_01 = (Vector *)(param_1 + 0xd8);
    pVVar8 = (Vector *)(param_1 + 0x7c);
    iVar10 = 8;
    bVar5 = false;
    for (uVar15 = 0; uVar15 < *(uint *)(param_1 + 8); uVar15 = uVar15 + 1) {
      iVar12 = *(int *)(param_1 + 0x5c);
      iVar13 = DAT_001627fc;
      if ((1 < iVar12 - 4U) && (iVar12 != 0x28)) {
        iVar13 = 0;
      }
      bVar6 = bVar5;
      if ((*(int *)(*(int *)(param_1 + 0x3c) + uVar15 * 4) <= iVar13) &&
         ((iVar12 != 0x27 || (*(int *)(*(int *)(param_1 + 0x38) + 0x6c) < 3)))) {
        *(undefined4 *)(param_1 + 0x6c) = 0;
        *(undefined4 *)(param_1 + 0x70) = 0;
        *(undefined2 *)(param_1 + 0x4c) = 0x101;
        *(uint *)(param_1 + 0xa0) = uVar15;
        AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_94);
        AbyssEngine::AEMath::Vector::operator=
                  ((Vector *)(*(int *)(param_1 + 0xc) + iVar10 + -8),(Vector *)&local_94);
        local_94 = 0;
        uStack_90 = 0;
        local_8c = 0;
        iVar13 = AbyssEngine::AEMath::operator!=(pVVar8,(Vector *)&local_94);
        if (iVar13 == 0) {
LAB_00162616:
          local_94 = 0;
          uStack_90 = 0;
          local_8c = 0;
          if (*(int *)(param_1 + 0x5c) != 0xb) goto LAB_001626f2;
          AbyssEngine::AEMath::MatrixGetDir(aMStack_ac);
          AbyssEngine::AEMath::MatrixGetUp(aMStack_b8);
          AbyssEngine::AEMath::operator+(aVStack_a0,(Vector *)aMStack_ac);
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_94,aVStack_a0);
          iVar13 = *(int *)(param_1 + 0xac);
          if (*(int *)(*(int *)(iVar13 + 4) + uVar15 * 4) == 0) {
            puVar14 = operator_new(0xc);
            *puVar14 = 0;
            puVar14[1] = 0;
            puVar14[2] = 0;
            *(undefined4 **)(*(int *)(iVar13 + 4) + uVar15 * 4) = puVar14;
          }
          puVar14 = *(undefined4 **)(DAT_00162800 + 0x162680);
          pcVar16 = *(code **)(DAT_00162804 + 0x162682);
          iVar13 = (*pcVar16)(*puVar14,200);
          fVar21 = (float)VectorSignedToFloat(iVar13 + -100,(byte)(in_fpscr >> 0x16) & 3);
          **(float **)(*(int *)(*(int *)(param_1 + 0xac) + 4) + uVar15 * 4) = fVar21 / fVar4;
          iVar13 = (*pcVar16)(*puVar14,200);
          fVar21 = (float)VectorSignedToFloat(iVar13 + -100,(byte)(in_fpscr >> 0x16) & 3);
          *(float *)(*(int *)(*(int *)(*(int *)(param_1 + 0xac) + 4) + uVar15 * 4) + 4) =
               fVar21 / fVar4;
          iVar13 = (*pcVar16)(*puVar14,200);
          fVar21 = (float)VectorSignedToFloat(iVar13 + -100,(byte)(in_fpscr >> 0x16) & 3);
          *(float *)(*(int *)(*(int *)(*(int *)(param_1 + 0xac) + 4) + uVar15 * 4) + 8) =
               fVar21 / fVar4;
        }
        else {
          if (*(int *)(param_1 + 0x5c) != 0x2a) {
            if (*(char *)(param_1 + 0xa5) == '\0') {
              if (*(char *)(param_1 + 0xa4) != '\0') goto LAB_0016259a;
LAB_001625c2:
              AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_94,(Vector *)&local_88);
              AbyssEngine::AEMath::Vector::operator+=
                        ((Vector *)(*(int *)(param_1 + 0xc) + iVar10 + -8),(Vector *)&local_94);
              if (*(char *)(param_1 + 0xa5) == '\0') goto LAB_00162616;
            }
            else if (*(char *)(param_1 + 0xa4) != '\0') {
LAB_0016259a:
              AbyssEngine::AEMath::Vector::operator=(this_01,pVVar8);
              if (*(byte *)(param_1 + 0xa6) != 0) {
                *(float *)this_01 = -*(float *)this_01;
              }
              *(byte *)(param_1 + 0xa6) = *(byte *)(param_1 + 0xa6) ^ 1;
              goto LAB_001625c2;
            }
            if (*(char *)(param_1 + 0xa6) != '\0') {
              AbyssEngine::AEMath::Vector::operator=(this_01,pVVar8);
              if (*(byte *)(param_1 + 0xa7) != 0) {
                *(float *)(param_1 + 0xdc) = -*(float *)(param_1 + 0xdc);
              }
              *(byte *)(param_1 + 0xa7) = *(byte *)(param_1 + 0xa7) ^ 1;
              AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_94,(Vector *)&local_88);
              AbyssEngine::AEMath::Vector::operator+=
                        ((Vector *)(*(int *)(param_1 + 0xc) + iVar10 + -8),(Vector *)&local_94);
            }
            goto LAB_00162616;
          }
LAB_001626f2:
          local_8c = 0;
          uStack_90 = 0;
          local_94 = 0;
          AbyssEngine::AEMath::MatrixGetDir((Matrix *)aVStack_a0);
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_94,aVStack_a0);
        }
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)aVStack_a0,(Vector *)&local_88);
        AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_94,aVStack_a0);
        AbyssEngine::AEMath::Vector::operator=
                  ((Vector *)(*(int *)(param_1 + 0x18) + iVar10 + -8),(Vector *)&local_94);
        AbyssEngine::AEMath::MatrixGetUp((Matrix *)aVStack_a0);
        AbyssEngine::AEMath::Vector::operator=
                  ((Vector *)(*(int *)(param_1 + 0x24) + iVar10 + -8),aVStack_a0);
        fVar21 = *(float *)(param_1 + 0xfc);
        uVar1 = in_fpscr & 0xfffffff | (uint)(fVar21 < 0.0) << 0x1f | (uint)(fVar21 == 0.0) << 0x1e;
        in_fpscr = uVar1 | (uint)NAN(fVar21) << 0x1c;
        bVar2 = (byte)(uVar1 >> 0x18);
        if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) {
          iVar13 = *(int *)(param_1 + 0x18);
        }
        else {
          puVar14 = *(undefined4 **)(DAT_00162808 + 0x162758);
          pcVar16 = *(code **)(DAT_0016280c + 0x16275a);
          uVar9 = (*pcVar16)(*puVar14,(int)fVar21);
          fVar21 = fVar21 * fVar20;
          fVar19 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
          iVar13 = *(int *)(param_1 + 0x18) + iVar10;
          *(float *)(iVar13 + -8) = *(float *)(iVar13 + -8) + (fVar19 * fVar3 - fVar21);
          uVar9 = (*pcVar16)(*puVar14,(int)*(float *)(param_1 + 0xfc));
          fVar19 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
          iVar13 = *(int *)(param_1 + 0x18) + iVar10;
          *(float *)(iVar13 + -4) = *(float *)(iVar13 + -4) + (fVar19 * fVar3 - fVar21);
          uVar9 = (*pcVar16)(*puVar14,(int)*(float *)(param_1 + 0xfc));
          fVar19 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
          iVar13 = *(int *)(param_1 + 0x18);
          *(float *)(iVar13 + iVar10) = *(float *)(iVar13 + iVar10) + (fVar19 * fVar3 - fVar21);
        }
        AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_a0,(Vector *)(iVar13 + iVar10 + -8));
        fVar21 = (float)AbyssEngine::AEMath::Vector::operator=
                                  ((Vector *)(*(int *)(param_1 + 0x18) + iVar10 + -8),aVStack_a0);
        AbyssEngine::AEMath::Vector::operator*=
                  ((Vector *)(*(int *)(param_1 + 0x18) + iVar10 + -8),fVar21);
        if (*(int *)(param_1 + 0x5c) == 0x27) {
          uVar9 = 0;
        }
        else {
          uVar9 = *(undefined4 *)(param_1 + 0x44);
        }
        *(undefined4 *)(*(int *)(param_1 + 0x3c) + uVar15 * 4) = uVar9;
        if (*(char *)(param_1 + 0xf9) == '\0') {
          bVar6 = true;
          if ((this_00 != (Item *)0x0) && (0 < *(int *)(param_1 + 0x74))) {
            iVar13 = Item::getType(this_00);
            if (iVar13 != 1) goto LAB_0016284e;
            bVar6 = false;
            if (*(int *)(param_1 + 0x58) == 0xb3) {
              *(undefined4 *)(**(int **)(DAT_00162b1c + 0x1628c0) + 0x144) = 0;
            }
          }
        }
        else {
LAB_0016284e:
          bVar6 = true;
        }
        *(undefined1 *)(param_1 + 0xa9) = 1;
        if ((*(char *)(param_1 + 0xf0) != '\0') && (*(char *)(param_1 + 0x4d) != '\0')) {
          Level::getPlayer();
          this = (TargetFollowCamera *)PlayerEgo::getTargetFollowCamera();
          TargetFollowCamera::hitSmall(this);
        }
        if (*(int *)(param_1 + 0x5c) != 0x28 && !bVar6) {
          *(int *)(param_1 + 0x74) = *(int *)(param_1 + 0x74) + -1;
          goto LAB_00162abe;
        }
        bVar6 = true;
        if (*(int *)(param_1 + 0x5c) != 0x28) goto LAB_00162ae0;
      }
      iVar10 = iVar10 + 0xc;
      bVar5 = bVar6;
    }
    bVar6 = *(int *)(param_1 + 0x5c) == 0x28;
    if ((*(int *)(param_1 + 0x5c) != 0x28) || (bVar5 != true)) goto LAB_00162ae0;
    *(int *)(param_1 + 0x74) = *(int *)(param_1 + 0x74) + -1;
    if (this_00 != (Item *)0x0) {
LAB_00162abe:
      Item::changeAmount(this_00,-1);
      if (*(int *)(param_1 + 0x74) == 0) {
        pSVar11 = (Ship *)Status::getShip();
        Ship::freeSlot(pSVar11,this_00);
      }
    }
  }
  else {
    if (0 < **(int **)(param_1 + 0x3c)) {
      bVar6 = *(int *)(param_1 + 0x5c) == 0x28;
      goto LAB_00162ae0;
    }
    *(undefined4 *)(param_1 + 0x6c) = 0;
    *(undefined4 *)(param_1 + 0x70) = 0;
    *(undefined2 *)(param_1 + 0x4c) = 0x101;
    if (*(char *)(param_1 + 0xf0) != '\0') {
      *(undefined1 *)(param_1 + 0xa9) = 1;
    }
    iVar10 = DAT_00162b04;
    *(undefined4 *)(param_1 + 0x9c) = 0xffffffff;
    for (uVar15 = 0; uVar15 < **(uint **)(param_1 + 0xb4); uVar15 = uVar15 + 1) {
      this_02 = *(Player **)((*(uint **)(param_1 + 0xb4))[1] + uVar15 * 4);
      iVar13 = Player::getKIPlayer(this_02);
      if ((((iVar13 != 0) &&
           (iVar13 = Player::getKIPlayer(this_02), *(char *)(iVar13 + 0x73) != '\0')) &&
          (iVar13 = Player::isActive(), iVar13 != 0)) && (iVar13 = Player::isDead(), iVar13 == 0)) {
        Player::getPosition();
        Level::getPlayer();
        PlayerEgo::getPosition();
        AbyssEngine::AEMath::operator-((Vector *)&local_94,aVStack_a0);
        fVar20 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_94);
        if ((int)fVar20 < iVar10) {
          *(uint *)(param_1 + 0x9c) = uVar15;
          iVar10 = (int)fVar20;
        }
      }
    }
    iVar13 = *(int *)(param_1 + 0x9c);
    **(undefined4 **)(param_1 + 0x3c) = *(undefined4 *)(param_1 + 0x44);
    if (iVar13 < 0) {
      pcVar16 = *(code **)(DAT_00162b10 + 0x162a3c);
      iVar10 = (*pcVar16)(*(undefined4 *)(param_1 + 0x38));
      pcVar17 = *(code **)(DAT_00162b14 + 0x162a48);
      (*pcVar17)(&local_94,*(undefined4 *)(iVar10 + 8));
      pcVar18 = *(code **)(DAT_00162b18 + 0x162a56);
      (*pcVar18)(param_1 + 0x90,&local_94);
      (*pcVar16)(*(undefined4 *)(param_1 + 0x38));
      PlayerEgo::getPosition();
      iVar10 = (*pcVar16)(*(undefined4 *)(param_1 + 0x38));
      fVar20 = (float)(*pcVar17)(aMStack_b8,*(undefined4 *)(iVar10 + 8));
      AbyssEngine::AEMath::operator*(fVar20,(Vector *)aMStack_ac);
      AbyssEngine::AEMath::operator+((Vector *)&local_94,aVStack_a0);
      (*pcVar18)(*(undefined4 *)(param_1 + 0xc),(Vector *)&local_94);
      iVar10 = (*pcVar16)(*(undefined4 *)(param_1 + 0x38));
      (*pcVar17)(&local_94,*(undefined4 *)(iVar10 + 8));
      (*pcVar18)(*(undefined4 *)(param_1 + 0x18),&local_94);
      *(undefined4 *)(param_1 + 0x8c) = 30000;
    }
    else {
      Player::getPosition();
      Level::getPlayer();
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::operator-(aVStack_a0,(Vector *)aMStack_ac);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_94,aVStack_a0);
      pcVar16 = *(code **)(DAT_00162b0c + 0x1629ce);
      (*pcVar16)(param_1 + 0x90,(AEMath *)&local_94);
      Player::getPosition();
      (*pcVar16)(*(undefined4 *)(param_1 + 0xc),&local_94);
      Level::getPlayer();
      AEGeometry::getDirection();
      (*pcVar16)(*(undefined4 *)(param_1 + 0x18),&local_94);
      *(int *)(param_1 + 0x8c) = iVar10;
    }
  }
  bVar6 = true;
LAB_00162ae0:
  if (*piVar7 == local_4c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar6);
}

```

## target disasm
```
  00162428: push {r4,r5,r6,r7,lr}
  0016242a: add r7,sp,#0xc
  0016242c: push {r7,r8,r9,r10,r11}
  00162430: vpush {d8,d9,d10,d11}
  00162434: sub sp,#0x88
  00162436: mov r4,r0
  00162438: ldr r0,[0x001627e8]
  0016243a: ldrd r6,r5,[r7,#0x8]
  0016243e: add r0,pc
  00162440: ldrd r12,lr,[r7,#0x10]
  00162444: ldrd r8,r9,[r7,#0x18]
  00162448: ldr r0,[r0,#0x0]
  0016244a: strd r1,r2,[sp,#0x48]
  0016244e: str r0,[sp,#0x4]
  00162450: ldr r0,[r0,#0x0]
  00162452: str r3,[sp,#0x50]
  00162454: str r6,[sp,#0x54]
  00162456: ldr.w r11,[r7,#0x24]
  0016245a: ldr r6,[r7,#0x2c]
  0016245c: ldr r1,[r7,#0x34]
  0016245e: str r5,[sp,#0x58]
  00162460: ldr.w r10,[r7,#0x20]
  00162464: ldr r3,[r7,#0x28]
  00162466: ldr r5,[r7,#0x30]
  00162468: ldr r2,[r7,#0x3c]
  0016246a: str.w r12,[sp,#0x5c]
  0016246e: str.w lr,[sp,#0x60]
  00162472: str.w r8,[sp,#0x64]
  00162476: str.w r9,[sp,#0x68]
  0016247a: strd r10,r11,[sp,#0x6c]
  0016247e: strd r3,r6,[sp,#0x74]
  00162482: strd r5,r1,[sp,#0x7c]
  00162486: str r0,[sp,#0x84]
  00162488: str r2,[r4,#0x4]
  0016248a: ldrb.w r0,[r4,#0xf9]
  0016248e: cbz r0,0x001624ae
  00162490: movs r5,#0x0
  00162492: ldrb.w r0,[r4,#0x108]
  00162496: cbz r0,0x001624d8
  00162498: ldr r0,[r4,#0x3c]
  0016249a: ldr r0,[r0,#0x0]
  0016249c: cmp r0,#0x1
  0016249e: blt.w 0x001628d0
  001624a2: ldr r0,[r4,#0x5c]
  001624a4: subs r0,#0x28
  001624a6: clz r0,r0
  001624aa: lsrs r0,r0,#0x5
  001624ac: b 0x00162ae0
  001624ae: ldr r0,[0x001627ec]
  001624b0: add r0,pc
  001624b2: ldr r0,[r0,#0x0]
  001624b4: ldr r0,[r0,#0x0]
  001624b6: blx 0x00071a58
  001624ba: blx 0x0007333c
  001624be: ldr.w r1,[r4,#0xf4]
  001624c2: ldr r0,[r0,#0x4]
  001624c4: ldr.w r5,[r0,r1,lsl #0x2]
  001624c8: cbz r5,0x001624d4
  001624ca: mov r0,r5
  001624cc: blx 0x0007183c
  001624d0: cmp r0,#0x0
  001624d2: bne 0x00162492
  001624d4: movs r0,#0x0
  001624d6: b 0x00162ae0
  001624d8: add.w r0,r4,#0xe4
  001624dc: add.w r6,r4,#0xd8
  001624e0: str r0,[sp,#0xc]
  001624e2: add.w r0,r4,#0x7c
  001624e6: add.w r10,sp,#0x3c
  001624ea: vldr.32 s16,[pc,#0x304]
  001624ee: vldr.32 s18,[pc,#0x304]
  001624f2: mov.w r9,#0x0
  001624f6: vldr.32 s20,[pc,#0x300]
  001624fa: mov.w r11,#0x0
  001624fe: str r5,[sp,#0x0]
  00162500: movs r5,#0x8
  00162502: str r0,[sp,#0x10]
  00162504: movs r0,#0x0
  00162506: str r6,[sp,#0x8]
  00162508: ldr r1,[r4,#0x8]
  0016250a: cmp r11,r1
  0016250c: bcs.w 0x00162a0c
  00162510: ldr r2,[r4,#0x3c]
  00162512: ldr r1,[r4,#0x5c]
  00162514: ldr.w r2,[r2,r11,lsl #0x2]
  00162518: subs r3,r1,#0x4
  0016251a: cmp r3,#0x2
  0016251c: bcc 0x00162522
  0016251e: cmp r1,#0x28
  00162520: bne 0x00162526
  00162522: ldr r3,[0x001627fc]
  00162524: b 0x00162528
  00162526: movs r3,#0x0
  00162528: cmp r2,r3
  0016252a: bgt.w 0x00162892
  0016252e: cmp r1,#0x27
  00162530: bne 0x0016253c
  00162532: ldr r1,[r4,#0x38]
  00162534: ldr r1,[r1,#0x6c]
  00162536: cmp r1,#0x2
  00162538: bgt.w 0x00162892
  0016253c: movw r0,#0x101
  00162540: add r1,sp,#0x48
  00162542: strd r9,r9,[r4,#0x6c]
  00162546: strh.w r0,[r4,#0x4c]
  0016254a: mov r0,r10
  0016254c: str.w r11,[r4,#0xa0]
  00162550: blx 0x0006f16c
  00162554: ldr r0,[r4,#0xc]
  00162556: mov r1,r10
  00162558: add r0,r5
  0016255a: subs r0,#0x8
  0016255c: blx 0x0006eb3c
  00162560: strd r9,r9,[sp,#0x3c]
  00162564: mov r1,r10
  00162566: ldr r0,[sp,#0x10]
  00162568: str.w r9,[sp,#0x44]
  0016256c: blx 0x00072a90
  00162570: cmp r0,#0x0
  00162572: str r5,[sp,#0x14]
  00162574: beq 0x00162616
  00162576: ldr r0,[r4,#0x5c]
  00162578: cmp r0,#0x2a
  0016257a: bne 0x00162586
  0016257c: strd r9,r9,[sp,#0x3c]
  00162580: str.w r9,[sp,#0x44]
  00162584: b 0x001626f2
  00162586: ldrb.w r1,[r4,#0xa5]
  0016258a: ldrb.w r0,[r4,#0xa4]
  0016258e: cbz r1,0x00162594
  00162590: cbnz r0,0x0016259a
  00162592: b 0x001625d8
  00162594: cmp r0,#0x0
  00162596: beq.w 0x001628c8
  0016259a: ldr r1,[sp,#0x10]
  0016259c: mov r0,r6
  0016259e: blx 0x0006eb3c
  001625a2: ldrb.w r0,[r4,#0xa6]
  001625a6: cbz r0,0x001625b4
  001625a8: vldr.32 s0,[r6]
  001625ac: vneg.f32 s0,s0
  001625b0: vstr.32 s0,[r6]
  001625b4: eor r0,r0,#0x1
  001625b8: add r1,sp,#0x48
  001625ba: strb.w r0,[r4,#0xa6]
  001625be: mov r0,r10
  001625c0: mov r2,r6
  001625c2: blx 0x0006f694
  001625c6: ldr r0,[r4,#0xc]
  001625c8: mov r1,r10
  001625ca: add r0,r5
  001625cc: subs r0,#0x8
  001625ce: blx 0x00073534
  001625d2: ldrb.w r0,[r4,#0xa5]
  001625d6: cbz r0,0x00162616
  001625d8: ldrb.w r0,[r4,#0xa6]
  001625dc: cbz r0,0x00162616
  001625de: ldr r1,[sp,#0x10]
  001625e0: mov r0,r6
  001625e2: blx 0x0006eb3c
  001625e6: ldrb.w r0,[r4,#0xa7]
  001625ea: cbz r0,0x001625f8
  001625ec: vldr.32 s0,[r4,#0xdc]
  001625f0: vneg.f32 s0,s0
  001625f4: vstr.32 s0,[r4,#0xdc]
  001625f8: eor r0,r0,#0x1
  001625fc: add r1,sp,#0x48
  001625fe: strb.w r0,[r4,#0xa7]
  00162602: mov r0,r10
  00162604: mov r2,r6
  00162606: blx 0x0006f694
  0016260a: ldr r0,[r4,#0xc]
  0016260c: mov r1,r10
  0016260e: add r0,r5
  00162610: subs r0,#0x8
  00162612: blx 0x00073534
  00162616: ldr r0,[r4,#0x5c]
  00162618: strd r9,r9,[sp,#0x3c]
  0016261c: cmp r0,#0xb
  0016261e: str.w r9,[sp,#0x44]
  00162622: bne 0x001626f2
  00162624: add r6,sp,#0x24
  00162626: add r5,sp,#0x48
  00162628: add.w r9,sp,#0x30
  0016262c: mov r0,r6
  0016262e: mov r1,r5
  00162630: blx 0x0006f4c0
  00162634: add.w r8,sp,#0x18
  00162638: mov r1,r5
  0016263a: mov r0,r8
  0016263c: blx 0x0006f4d8
  00162640: mov r0,r9
  00162642: mov r1,r6
  00162644: mov r2,r8
  00162646: mov r5,r9
  00162648: mov.w r9,#0x0
  0016264c: blx 0x0006f1cc
  00162650: mov r0,r10
  00162652: mov r1,r5
  00162654: blx 0x0006eb3c
  00162658: ldr.w r5,[r4,#0xac]
  0016265c: ldr r0,[r5,#0x4]
  0016265e: ldr.w r0,[r0,r11,lsl #0x2]
  00162662: cbnz r0,0x00162678
  00162664: movs r0,#0xc
  00162666: blx 0x0006eb24
  0016266a: strd r9,r9,[r0,#0x0]
  0016266e: str.w r9,[r0,#0x8]
  00162672: ldr r1,[r5,#0x4]
  00162674: str.w r0,[r1,r11,lsl #0x2]
  00162678: ldr r0,[0x00162800]
  0016267a: ldr r1,[0x00162804]
  0016267c: add r0,pc
  0016267e: add r1,pc
  00162680: ldr r6,[r0,#0x0]
  00162682: ldr r5,[r1,#0x0]
  00162684: movs r1,#0xc8
  00162686: ldr r0,[r6,#0x0]
  00162688: blx r5
  0016268a: subs r0,#0x64
  0016268c: movs r1,#0xc8
  0016268e: vmov s0,r0
  00162692: vcvt.f32.s32 s0,s0
  00162696: ldr.w r0,[r4,#0xac]
  0016269a: ldr r0,[r0,#0x4]
  0016269c: ldr.w r0,[r0,r11,lsl #0x2]
  001626a0: vdiv.f32 s0,s0,s20
  001626a4: vstr.32 s0,[r0]
  001626a8: ldr r0,[r6,#0x0]
  001626aa: blx r5
  001626ac: subs r0,#0x64
  001626ae: movs r1,#0xc8
  001626b0: vmov s0,r0
  001626b4: vcvt.f32.s32 s0,s0
  001626b8: ldr.w r0,[r4,#0xac]
  001626bc: ldr r0,[r0,#0x4]
  001626be: ldr.w r0,[r0,r11,lsl #0x2]
  001626c2: vdiv.f32 s0,s0,s20
  001626c6: vstr.32 s0,[r0,#0x4]
  001626ca: ldr r0,[r6,#0x0]
  001626cc: blx r5
  001626ce: subs r0,#0x64
  001626d0: add.w r10,sp,#0x3c
  001626d4: add r5,sp,#0x48
  001626d6: vmov s0,r0
  001626da: vcvt.f32.s32 s0,s0
  001626de: ldr.w r0,[r4,#0xac]
  001626e2: ldr r0,[r0,#0x4]
  001626e4: ldr.w r0,[r0,r11,lsl #0x2]
  001626e8: vdiv.f32 s0,s0,s20
  001626ec: vstr.32 s0,[r0,#0x8]
  001626f0: b 0x00162706
  001626f2: add r6,sp,#0x30
  001626f4: add r5,sp,#0x48
  001626f6: mov r0,r6
  001626f8: mov r1,r5
  001626fa: blx 0x0006f4c0
  001626fe: mov r0,r10
  00162700: mov r1,r6
  00162702: blx 0x0006eb3c
  00162706: add r6,sp,#0x30
  00162708: ldr r2,[sp,#0xc]
  0016270a: mov r1,r5
  0016270c: mov r0,r6
  0016270e: blx 0x0006f694
  00162712: mov r0,r10
  00162714: mov r1,r6
  00162716: blx 0x00073534
  0016271a: ldr r0,[r4,#0x18]
  0016271c: mov r1,r10
  0016271e: ldr.w r8,[sp,#0x14]
  00162722: add r0,r8
  00162724: subs r0,#0x8
  00162726: blx 0x0006eb3c
  0016272a: mov r0,r6
  0016272c: mov r1,r5
  0016272e: blx 0x0006f4d8
  00162732: ldr r0,[r4,#0x24]
  00162734: mov r1,r6
  00162736: add r0,r8
  00162738: subs r0,#0x8
  0016273a: blx 0x0006eb3c
  0016273e: vldr.32 s22,[r4,#0xfc]
  00162742: vcmpe.f32 s22,#0
  00162746: vmrs apsr,fpscr
  0016274a: ble 0x00162810
  0016274c: ldr r0,[0x00162808]
  0016274e: ldr r1,[0x0016280c]
  00162750: vcvt.s32.f32 s0,s22
  00162754: add r0,pc
  00162756: add r1,pc
  00162758: ldr r5,[r0,#0x0]
  0016275a: ldr r6,[r1,#0x0]
  0016275c: ldr r0,[r5,#0x0]
  0016275e: vmov r1,s0
  00162762: blx r6
  00162764: vmul.f32 s22,s22,s16
  00162768: vmov s0,r0
  0016276c: vcvt.f32.s32 s0,s0
  00162770: ldr r0,[r4,#0x18]
  00162772: add r0,r8
  00162774: vmov.f32 s2,s22
  00162778: vnmls.f32 s2,s0,s18
  0016277c: vldr.32 s0,[r0,#-0x8]
  00162780: vadd.f32 s0,s0,s2
  00162784: vstr.32 s0,[r0,#-0x8]
  00162788: vldr.32 s0,[r4,#0xfc]
  0016278c: ldr r0,[r5,#0x0]
  0016278e: vcvt.s32.f32 s0,s0
  00162792: vmov r1,s0
  00162796: blx r6
  00162798: vmov s0,r0
  0016279c: vmov.f32 s2,s22
  001627a0: vcvt.f32.s32 s0,s0
  001627a4: ldr r0,[r4,#0x18]
  001627a6: add r0,r8
  001627a8: vnmls.f32 s2,s0,s18
  001627ac: vldr.32 s0,[r0,#-0x4]
  001627b0: vadd.f32 s0,s0,s2
  001627b4: vstr.32 s0,[r0,#-0x4]
  001627b8: vldr.32 s0,[r4,#0xfc]
  001627bc: ldr r0,[r5,#0x0]
  001627be: vcvt.s32.f32 s0,s0
  001627c2: vmov r1,s0
  001627c6: blx r6
  001627c8: vmov s0,r0
  001627cc: vcvt.f32.s32 s0,s0
  001627d0: ldr r0,[r4,#0x18]
  001627d2: add.w r1,r0,r8
  001627d6: vnmls.f32 s22,s0,s18
  001627da: vldr.32 s0,[r1]
  001627de: vadd.f32 s0,s0,s22
  001627e2: vstr.32 s0,[r1]
  001627e6: b 0x00162812
  00162810: ldr r0,[r4,#0x18]
  00162812: add r0,r8
  00162814: add r5,sp,#0x30
  00162816: sub.w r1,r0,#0x8
  0016281a: mov r0,r5
  0016281c: blx 0x0006ec80
  00162820: ldr r0,[r4,#0x18]
  00162822: mov r1,r5
  00162824: add r0,r8
  00162826: subs r0,#0x8
  00162828: blx 0x0006eb3c
  0016282c: ldr r0,[r4,#0x18]
  0016282e: ldr r1,[r4,#0x50]
  00162830: add r0,r8
  00162832: subs r0,#0x8
  00162834: blx 0x00072628
  00162838: ldr r0,[r4,#0x5c]
  0016283a: cmp r0,#0x27
  0016283c: ite ne
  0016283e: ldr.ne r0,[r4,#0x44]
  00162840: mov.eq r0,#0x0
  00162842: ldr r1,[r4,#0x3c]
  00162844: str.w r0,[r1,r11,lsl #0x2]
  00162848: ldrb.w r0,[r4,#0xf9]
  0016284c: cbz r0,0x0016289a
  0016284e: movs r6,#0x1
  00162850: ldrb.w r0,[r4,#0xf0]
  00162854: movs r1,#0x1
  00162856: strb.w r1,[r4,#0xa9]
  0016285a: cbz r0,0x00162870
  0016285c: ldrb.w r0,[r4,#0x4d]
  00162860: cbz r0,0x00162870
  00162862: ldr r0,[r4,#0x38]
  00162864: blx 0x00072034
  00162868: blx 0x0007588c
  0016286c: blx 0x00077380
  00162870: ldr r1,[r4,#0x5c]
  00162872: sub.w r0,r1,#0x28
  00162876: clz r0,r0
  0016287a: lsrs r0,r0,#0x5
  0016287c: orrs r0,r6
  0016287e: beq.w 0x00162ab6
  00162882: ldr r5,[sp,#0x14]
  00162884: add.w r10,sp,#0x3c
  00162888: ldr r6,[sp,#0x8]
  0016288a: movs r0,#0x1
  0016288c: cmp r1,#0x28
  0016288e: bne.w 0x00162ae0
  00162892: adds r5,#0xc
  00162894: add.w r11,r11,#0x1
  00162898: b 0x00162508
  0016289a: ldr r0,[sp,#0x0]
  0016289c: movs r6,#0x1
  0016289e: cmp r0,#0x0
  001628a0: beq 0x00162850
  001628a2: ldr r0,[r4,#0x74]
  001628a4: cmp r0,#0x1
  001628a6: blt 0x00162850
  001628a8: ldr r0,[sp,#0x0]
  001628aa: blx 0x000718fc
  001628ae: cmp r0,#0x1
  001628b0: bne 0x0016284e
  001628b2: ldr r0,[r4,#0x58]
  001628b4: movs r6,#0x0
  001628b6: cmp r0,#0xb3
  001628b8: bne 0x00162850
  001628ba: ldr r0,[0x00162b1c]
  001628bc: add r0,pc
  001628be: ldr r0,[r0,#0x0]
  001628c0: ldr r0,[r0,#0x0]
  001628c2: str.w r6,[r0,#0x144]
  001628c6: b 0x00162850
  001628c8: mov r0,r10
  001628ca: add r1,sp,#0x48
  001628cc: ldr r2,[sp,#0x10]
  001628ce: b 0x001625c2
  001628d0: movs r0,#0x0
  001628d2: movw r1,#0x101
  001628d6: strd r0,r0,[r4,#0x6c]
  001628da: ldrb.w r0,[r4,#0xf0]
  001628de: strh.w r1,[r4,#0x4c]
  001628e2: cbz r0,0x001628ea
  001628e4: movs r0,#0x1
  001628e6: strb.w r0,[r4,#0xa9]
  001628ea: ldr r0,[0x00162b04]
  001628ec: add.w r8,sp,#0x30
  001628f0: add.w r9,sp,#0x24
  001628f4: add.w r10,sp,#0x3c
  001628f8: movs r5,#0x0
  001628fa: mov r11,r0
  001628fc: mov.w r0,#0xffffffff
  00162900: str.w r0,[r4,#0x9c]
  00162904: b 0x00162970
  00162906: ldr r0,[r0,#0x4]
  00162908: ldr.w r6,[r0,r5,lsl #0x2]
  0016290c: mov r0,r6
  0016290e: blx 0x00075850
  00162912: cbz r0,0x0016296e
  00162914: mov r0,r6
  00162916: blx 0x00075850
  0016291a: ldrb.w r0,[r0,#0x73]
  0016291e: cbz r0,0x0016296e
  00162920: mov r0,r6
  00162922: blx 0x00071f14
  00162926: cbz r0,0x0016296e
  00162928: mov r0,r6
  0016292a: blx 0x00075838
  0016292e: cbnz r0,0x0016296e
  00162930: mov r0,r8
  00162932: mov r1,r6
  00162934: blx 0x00072964
  00162938: ldr r0,[r4,#0x38]
  0016293a: blx 0x00072034
  0016293e: mov r1,r0
  00162940: mov r0,r9
  00162942: blx 0x0007264c
  00162946: mov r0,r10
  00162948: mov r1,r8
  0016294a: mov r2,r9
  0016294c: blx 0x0006ec38
  00162950: mov r0,r10
  00162952: blx 0x0006ec44
  00162956: vmov s0,r0
  0016295a: mov r1,r11
  0016295c: vcvt.s32.f32 s0,s0
  00162960: vmov r0,s0
  00162964: cmp r11,r0
  00162966: itt gt
  00162968: str.w.gt r5,[r4,#0x9c]
  0016296c: mov.gt r11,r0
  0016296e: adds r5,#0x1
  00162970: ldr.w r0,[r4,#0xb4]
  00162974: ldr r1,[r0,#0x0]
  00162976: cmp r5,r1
  00162978: bcc 0x00162906
  0016297a: ldr.w r3,[r4,#0x9c]
  0016297e: ldr r1,[r4,#0x3c]
  00162980: ldr r2,[r4,#0x44]
  00162982: cmp.w r3,#0xffffffff
  00162986: str r2,[r1,#0x0]
  00162988: ble 0x00162a34
  0016298a: ldr.w r1,[r4,#0x9c]
  0016298e: add.w r8,sp,#0x24
  00162992: ldr r0,[r0,#0x4]
  00162994: ldr.w r1,[r0,r1,lsl #0x2]
  00162998: mov r0,r8
  0016299a: blx 0x00072964
  0016299e: ldr r0,[r4,#0x38]
  001629a0: blx 0x00072034
  001629a4: add r6,sp,#0x18
  001629a6: mov r1,r0
  001629a8: mov r0,r6
  001629aa: blx 0x0007264c
  001629ae: add r5,sp,#0x30
  001629b0: mov r1,r8
  001629b2: mov r2,r6
  001629b4: mov r0,r5
  001629b6: blx 0x0006ec38
  001629ba: add.w r8,sp,#0x3c
  001629be: mov r1,r5
  001629c0: mov r0,r8
  001629c2: blx 0x0006ec80
  001629c6: ldr r0,[0x00162b0c]
  001629c8: mov r1,r8
  001629ca: add r0,pc
  001629cc: ldr r6,[r0,#0x0]
  001629ce: add.w r0,r4,#0x90
  001629d2: blx r6
  001629d4: ldr.w r1,[r4,#0xb4]
  001629d8: add r5,sp,#0x3c
  001629da: ldr.w r0,[r4,#0x9c]
  001629de: ldr r1,[r1,#0x4]
  001629e0: ldr.w r1,[r1,r0,lsl #0x2]
  001629e4: mov r0,r5
  001629e6: blx 0x00072964
  001629ea: ldr r0,[r4,#0xc]
  001629ec: mov r1,r5
  001629ee: blx r6
  001629f0: ldr r0,[r4,#0x38]
  001629f2: blx 0x00072034
  001629f6: ldr r1,[r0,#0x8]
  001629f8: add r5,sp,#0x3c
  001629fa: mov r0,r5
  001629fc: blx 0x00072574
  00162a00: ldr r0,[r4,#0x18]
  00162a02: mov r1,r5
  00162a04: blx r6
  00162a06: str.w r11,[r4,#0x8c]
  00162a0a: b 0x00162ade
  00162a0c: ldr r1,[r4,#0x5c]
  00162a0e: subs r1,#0x28
  00162a10: clz r1,r1
  00162a14: lsr.w r1,r1,#0x5
  00162a18: bne 0x00162afe
  00162a1a: eor r0,r0,#0x1
  00162a1e: lsls r0,r0,#0x1f
  00162a20: mov r0,r1
  00162a22: bne 0x00162ae0
  00162a24: ldr r5,[sp,#0x0]
  00162a26: ldr r0,[r4,#0x74]
  00162a28: cmp r5,#0x0
  00162a2a: sub.w r0,r0,#0x1
  00162a2e: str r0,[r4,#0x74]
  00162a30: bne 0x00162abe
  00162a32: b 0x00162ade
  00162a34: ldr r1,[0x00162b10]
  00162a36: ldr r0,[r4,#0x38]
  00162a38: add r1,pc
  00162a3a: ldr r5,[r1,#0x0]
  00162a3c: blx r5
  00162a3e: ldr r2,[0x00162b14]
  00162a40: add r6,sp,#0x3c
  00162a42: ldr r1,[r0,#0x8]
  00162a44: add r2,pc
  00162a46: mov r0,r6
  00162a48: ldr.w r10,[r2,#0x0]
  00162a4c: blx r10
  00162a4e: ldr r0,[0x00162b18]
  00162a50: mov r1,r6
  00162a52: add r0,pc
  00162a54: ldr.w r11,[r0,#0x0]
  00162a58: add.w r0,r4,#0x90
  00162a5c: blx r11
  00162a5e: ldr r0,[r4,#0x38]
  00162a60: blx r5
  00162a62: add.w r8,sp,#0x30
  00162a66: mov r1,r0
  00162a68: mov r0,r8
  00162a6a: blx 0x0007264c
  00162a6e: ldr r0,[r4,#0x38]
  00162a70: blx r5
  00162a72: ldr r1,[r0,#0x8]
  00162a74: add r6,sp,#0x18
  00162a76: mov r0,r6
  00162a78: blx r10
  00162a7a: add.w r9,sp,#0x24
  00162a7e: ldr r1,[0x00162b08]
  00162a80: mov r2,r6
  00162a82: mov r0,r9
  00162a84: blx 0x00072658
  00162a88: add r6,sp,#0x3c
  00162a8a: mov r1,r8
  00162a8c: mov r2,r9
  00162a8e: mov r0,r6
  00162a90: blx 0x0006f1cc
  00162a94: ldr r0,[r4,#0xc]
  00162a96: mov r1,r6
  00162a98: blx r11
  00162a9a: ldr r0,[r4,#0x38]
  00162a9c: blx r5
  00162a9e: ldr r1,[r0,#0x8]
  00162aa0: add r5,sp,#0x3c
  00162aa2: mov r0,r5
  00162aa4: blx r10
  00162aa6: ldr r0,[r4,#0x18]
  00162aa8: mov r1,r5
  00162aaa: blx r11
  00162aac: movw r0,#0x7530
  00162ab0: str.w r0,[r4,#0x8c]
  00162ab4: b 0x00162ade
  00162ab6: ldr r0,[r4,#0x74]
  00162ab8: ldr r5,[sp,#0x0]
  00162aba: subs r0,#0x1
  00162abc: str r0,[r4,#0x74]
  00162abe: mov r0,r5
  00162ac0: mov.w r1,#0xffffffff
  00162ac4: blx 0x00071854
  00162ac8: ldr r0,[r4,#0x74]
  00162aca: cbnz r0,0x00162ade
  00162acc: ldr r0,[0x00162b20]
  00162ace: add r0,pc
  00162ad0: ldr r0,[r0,#0x0]
  00162ad2: ldr r0,[r0,#0x0]
  00162ad4: blx 0x00071a58
  00162ad8: mov r1,r5
  00162ada: blx 0x00077230
  00162ade: movs r0,#0x1
  00162ae0: ldr r1,[sp,#0x84]
  00162ae2: ldr r2,[sp,#0x4]
  00162ae4: ldr r2,[r2,#0x0]
  00162ae6: subs r1,r2,r1
  00162ae8: itttt eq
  00162aea: add.eq sp,#0x88
  00162aec: vpop.eq {d8,d9,d10,d11}
  00162af0: add.eq sp,#0x4
  00162af2: pop.eq.w {r8,r9,r10,r11}
  00162af6: it eq
  00162af8: pop.eq {r4,r5,r6,r7,pc}
  00162afa: blx 0x0006e824
  00162afe: mov r0,r1
  00162b00: b 0x00162ae0
```
