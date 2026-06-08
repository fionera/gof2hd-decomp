# RepairBeam::update
elf 0xa72f8 body 1788
Sig: undefined __stdcall update(int param_1, Radar * param_2, Level * param_3, Hud * param_4)

## decompile
```c

/* RepairBeam::update(int, Radar*, Level*, Hud*) */

void RepairBeam::update(int param_1,Radar *param_2,Level *param_3,Hud *param_4)

{
  byte bVar1;
  int *piVar2;
  PlayerEgo *pPVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  float *pfVar8;
  uint *puVar9;
  uint uVar10;
  uint uVar11;
  Vector *pVVar12;
  AEGeometry *this;
  Player *pPVar13;
  int iVar14;
  int *piVar15;
  int iVar16;
  bool bVar17;
  uint in_fpscr;
  float fVar18;
  float fVar19;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s2;
  float fVar20;
  float fVar21;
  longlong lVar22;
  uint *local_b0;
  Vector *local_ac;
  Vector aVStack_a8 [12];
  AEMath aAStack_9c [12];
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  Vector aVStack_84 [12];
  AEMath aAStack_78 [12];
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  int local_54;
  
  piVar2 = *(int **)(DAT_000b7674 + 0xb7312);
  local_54 = *piVar2;
  local_b0 = (uint *)Level::getEnemies();
  *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) - (int)param_2;
  if (local_b0 != (uint *)0x0) {
    pPVar3 = (PlayerEgo *)Level::getPlayer();
    iVar4 = PlayerEgo::isDead(pPVar3);
    if (iVar4 == 0) {
      iVar4 = Status::getShip();
      iVar4 = Ship::getFirstEquipmentOfSort(iVar4);
      uVar5 = Item::getAttribute(iVar4);
      if (*(int *)(param_1 + 0x20) < 0) {
        puVar9 = *(uint **)(param_1 + 0x14);
        iVar4 = 0;
        for (uVar10 = 0; uVar10 < *puVar9; uVar10 = uVar10 + 1) {
          *(undefined4 *)(puVar9[1] + iVar4) = 0xffffffff;
          *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x18) + 4) + iVar4) = 0;
          iVar4 = iVar4 + 4;
        }
        uVar10 = 0;
        fVar20 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
        *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 0x9c4;
        pVVar12 = (Vector *)(param_1 + 4);
        for (; uVar10 < *local_b0; uVar10 = uVar10 + 1) {
          iVar16 = *(int *)(local_b0[1] + uVar10 * 4);
          iVar4 = KIPlayer::isDead();
          if ((iVar4 == 0) && (iVar4 = KIPlayer::isDying(), iVar4 == 0)) {
            uVar6 = *(uint *)(param_1 + 0x1c);
            if (uVar6 == 0x25) {
              if (*(char *)(*(int *)(iVar16 + 4) + 0x5d) != '\0') {
                iVar4 = Player::getHitpoints();
                iVar14 = Player::getMaxHitpoints();
                if (iVar14 <= iVar4) {
                  uVar6 = *(uint *)(param_1 + 0x1c);
                  goto LAB_000b74ec;
                }
LAB_000b741a:
                Player::getPosition();
                AbyssEngine::AEMath::Vector::operator=(pVVar12,(Vector *)&local_60);
                Level::getPlayer();
                PlayerEgo::getPosition();
                AbyssEngine::AEMath::Vector::operator-=(pVVar12,(Vector *)&local_60);
                fVar18 = (float)AbyssEngine::AEMath::VectorLength(pVVar12);
                in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar18 == fVar20) << 0x1e |
                           (uint)(fVar20 <= fVar18) << 0x1d;
                bVar1 = (byte)(in_fpscr >> 0x18);
                if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
                  puVar9 = *(uint **)(param_1 + 0x14);
                  bVar17 = false;
                  for (uVar6 = 0; (!bVar17 && (uVar6 < *puVar9)); uVar6 = uVar6 + 1) {
                    uVar11 = puVar9[1];
                    bVar17 = *(int *)(uVar11 + uVar6 * 4) == -1;
                    if (bVar17) {
                      *(uint *)(uVar11 + uVar6 * 4) = uVar10;
                    }
                  }
                  if (!bVar17) {
                    iVar16 = Player::getHitpoints();
                    uVar6 = 0xffffffff;
                    iVar4 = DAT_000b767c;
                    for (uVar11 = 0; puVar9 = *(uint **)(param_1 + 0x14), uVar11 < *puVar9;
                        uVar11 = uVar11 + 1) {
                      if ((*(int *)(puVar9[1] + uVar11 * 4) != -1) &&
                         (iVar14 = Player::getHitpoints(), iVar14 < iVar4 && iVar16 < iVar14)) {
                        iVar4 = iVar14;
                        uVar6 = uVar11;
                      }
                    }
                    if (uVar6 != 0xffffffff) {
                      *(uint *)(puVar9[1] + uVar6 * 4) = uVar10;
                    }
                  }
                }
              }
            }
            else {
LAB_000b74ec:
              bVar17 = uVar6 == 0x29;
              if (bVar17) {
                uVar6 = (uint)*(byte *)(iVar16 + 0x74);
              }
              if ((bVar17 && uVar6 == 0) && (*(char *)(*(int *)(iVar16 + 4) + 0x5c) != '\0')) {
                puVar7 = (undefined4 *)Level::getPlayer();
                iVar4 = Player::getShieldDamageRate((Player *)*puVar7);
                if (iVar4 < 100) {
                  iVar4 = Status::getShip();
                  iVar4 = Ship::getFirstEquipmentOfSort(iVar4);
                  if (iVar4 != 0) goto LAB_000b741a;
                }
              }
            }
          }
        }
      }
      fVar20 = DAT_000b7688;
      local_ac = (Vector *)(param_1 + 4);
      local_b0 = local_b0 + 1;
      bVar17 = true;
      iVar4 = 0;
      fVar18 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      puVar9 = *(uint **)(DAT_000b768c + 0xb7568);
      fVar21 = fVar18 * DAT_000b7680;
      fVar18 = fVar18 * DAT_000b7684;
      for (uVar10 = 0; uVar10 < **(uint **)(param_1 + 0x14); uVar10 = uVar10 + 1) {
        if (*(int *)((*(uint **)(param_1 + 0x14))[1] + iVar4) != -1) {
          lVar22 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar9);
          AbyssEngine::Transform::Update(lVar22,SUB41(param_2,0));
          Player::getPosition();
          AbyssEngine::AEMath::Vector::operator=(local_ac,(Vector *)&local_60);
          iVar16 = *(int *)(*(int *)(*local_b0 +
                                    *(int *)(*(int *)(*(int *)(param_1 + 0x14) + 4) + iVar4) * 4) +
                           0x7c);
          if (iVar16 == 0x2c) {
            AEGeometry::getDirection();
            fVar19 = (float)AbyssEngine::AEMath::VectorNormalize
                                      ((AEMath *)&local_6c,(Vector *)aAStack_78);
            AbyssEngine::AEMath::operator*((Vector *)&local_60,fVar19);
          }
          else if (iVar16 == 0x31) {
            AEGeometry::getDirection();
            fVar19 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_78,aVStack_84);
            AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar19);
            AEGeometry::getUpVector();
            fVar19 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_9c,aVStack_a8);
            AbyssEngine::AEMath::operator*((Vector *)&local_90,fVar19);
            AbyssEngine::AEMath::operator+((Vector *)&local_60,(Vector *)&local_6c);
          }
          else {
            local_60 = 0;
            uStack_5c = 0;
            local_58 = 0;
          }
          AbyssEngine::AEMath::Vector::operator+=(local_ac,(Vector *)&local_60);
          Level::getPlayer();
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::Vector::operator-=(local_ac,(Vector *)&local_6c);
          Status::getShip();
          iVar16 = Ship::getIndex();
          if (iVar16 == 0x2c) {
            Level::getPlayer();
            fVar19 = (float)PlayerEgo::GetDirVector();
            AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar19);
          }
          else {
            Status::getShip();
            iVar16 = Ship::getIndex();
            if (iVar16 == 0x31) {
              Level::getPlayer();
              fVar19 = (float)PlayerEgo::GetDirVector();
              AbyssEngine::AEMath::operator*((Vector *)aAStack_78,fVar19);
              Level::getPlayer();
              fVar19 = (float)PlayerEgo::GetUpVector();
              AbyssEngine::AEMath::operator*((Vector *)&local_90,fVar19);
              AbyssEngine::AEMath::operator+((Vector *)&local_6c,(Vector *)aAStack_78);
            }
            else {
              local_6c = 0;
              uStack_68 = 0;
              local_64 = 0;
            }
          }
          AbyssEngine::AEMath::VectorLength(local_ac);
          AEGeometry::setScaling(extraout_s0,extraout_s1,extraout_s2);
          pVVar12 = *(Vector **)(*(int *)(*(int *)(param_1 + 0x10) + 4) + iVar4);
          AbyssEngine::AEMath::operator-(aVStack_84,local_ac);
          AbyssEngine::AEMath::VectorNormalize(aAStack_78,aVStack_84);
          local_8c = 0x3f800000;
          local_90 = 0;
          local_88 = 0;
          AEGeometry::setDirection(pVVar12,(Vector *)aAStack_78);
          this = *(AEGeometry **)(*(int *)(*(int *)(param_1 + 0x10) + 4) + iVar4);
          Level::getPlayer();
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::operator+((Vector *)aAStack_78,aVStack_84);
          AEGeometry::setPosition(this,(Vector *)aAStack_78);
          uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar9);
          AbyssEngine::Transform::Update((ulonglong)uVar6,SUB41(param_2,0));
          if (*(int *)(param_1 + 0x1c) == 0x29) {
            puVar7 = (undefined4 *)Level::getPlayer();
            iVar16 = Player::getShieldDamageRate((Player *)*puVar7);
            if (iVar16 < 100) {
              iVar16 = Status::getShip();
              iVar16 = Ship::getFirstEquipmentOfSort(iVar16);
              uVar5 = Item::getAttribute(iVar16);
              fVar19 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
              pfVar8 = (float *)(*(int *)(*(int *)(param_1 + 0x18) + 4) + iVar4);
              fVar19 = *pfVar8 + (fVar21 * fVar19) / fVar20;
              uVar6 = in_fpscr & 0xfffffff | (uint)(fVar19 < 1.0) << 0x1f;
              in_fpscr = uVar6 | (uint)NAN(fVar19) << 0x1c;
              *pfVar8 = fVar19;
              if ((byte)(uVar6 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
                Player::damage(*(int *)(*(int *)(*local_b0 +
                                                *(int *)(*(int *)(*(int *)(param_1 + 0x14) + 4) +
                                                        iVar4) * 4) + 4),true,0);
                pfVar8 = (float *)(*(int *)(*(int *)(param_1 + 0x18) + 4) + iVar4);
                *pfVar8 = *pfVar8 + -1.0;
              }
              puVar7 = (undefined4 *)Level::getPlayer();
              pPVar13 = (Player *)*puVar7;
              iVar16 = Status::getShip();
              iVar16 = Ship::getFirstEquipmentOfSort(iVar16);
              uVar5 = Item::getAttribute(iVar16);
              fVar19 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
              Player::regenerateShield(pPVar13,(fVar21 * fVar19) / fVar20);
            }
          }
          else if (*(int *)(param_1 + 0x1c) == 0x25) {
            pPVar13 = *(Player **)
                       (*(int *)(*local_b0 +
                                *(int *)(*(int *)(*(int *)(param_1 + 0x14) + 4) + iVar4) * 4) + 4);
            iVar16 = Status::getShip();
            iVar16 = Ship::getFirstEquipmentOfSort(iVar16);
            uVar5 = Item::getAttribute(iVar16);
            fVar19 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
            Player::heal(pPVar13,(fVar18 * fVar19) / fVar20);
          }
          iVar14 = DAT_000b7a44 + 0xb7924;
          piVar15 = *(int **)(DAT_000b7a40 + 0xb791e);
          iVar16 = FModSound::isPlaying(*piVar15);
          if (iVar16 == 0) {
            FModSound::play(*piVar15,*(Vector **)(iVar14 + *(int *)param_1 * 4),(Vector *)0x0,
                            extraout_s0_00);
          }
          bVar17 = false;
        }
        iVar4 = iVar4 + 4;
      }
      if (bVar17) {
        FModSound::stop(**(int **)(DAT_000b7a24 + 0xb7966));
      }
    }
  }
  pPVar3 = (PlayerEgo *)Level::getPlayer();
  iVar4 = PlayerEgo::isDead(pPVar3);
  if (iVar4 != 0) {
    piVar15 = *(int **)(DAT_000b7a2c + 0xb798a);
    iVar4 = FModSound::isPlaying(*piVar15);
    if (iVar4 != 0) {
      FModSound::stop(*piVar15);
    }
  }
  if (*(char *)(*(int *)(DAT_000b7a34 + 0xb79b0) + 0xf) != '\0') {
    iVar16 = DAT_000b7a3c + 0xb79c2;
    piVar15 = *(int **)(DAT_000b7a38 + 0xb79bc);
    iVar4 = FModSound::isPlaying(*piVar15);
    if (iVar4 != 0) {
      iVar4 = *(int *)param_1;
      Level::getPlayer();
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 4),(Vector *)&local_60);
      FModSound::updateEvent3DAttributes
                ((FModSound *)*piVar15,*(int *)(iVar16 + iVar4 * 4),(Vector *)(param_1 + 4),
                 (Vector *)0x0,false);
    }
  }
  if (*piVar2 != local_54) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b72f8: push {r4,r5,r6,r7,lr}
  000b72fa: add r7,sp,#0xc
  000b72fc: push {r7,r8,r9,r10,r11}
  000b7300: vpush {d8,d9,d10,d11,d12}
  000b7304: sub sp,#0x88
  000b7306: mov r8,r0
  000b7308: ldr r0,[0x000b7674]
  000b730a: mov r9,r3
  000b730c: mov r5,r1
  000b730e: add r0,pc
  000b7310: ldr r0,[r0,#0x0]
  000b7312: str r0,[sp,#0x8]
  000b7314: ldr r0,[r0,#0x0]
  000b7316: str r0,[sp,#0x84]
  000b7318: mov r0,r3
  000b731a: blx 0x00071eb4
  000b731e: mov r10,r0
  000b7320: ldr.w r0,[r8,#0x20]
  000b7324: cmp.w r10,#0x0
  000b7328: str r5,[sp,#0x24]
  000b732a: sub.w r0,r0,r5
  000b732e: str.w r0,[r8,#0x20]
  000b7332: beq.w 0x000b7976
  000b7336: mov r0,r9
  000b7338: blx 0x00072034
  000b733c: blx 0x000724b4
  000b7340: cmp r0,#0x0
  000b7342: bne.w 0x000b7976
  000b7346: ldr r0,[0x000b7678]
  000b7348: add r0,pc
  000b734a: ldr r5,[r0,#0x0]
  000b734c: ldr r0,[r5,#0x0]
  000b734e: blx 0x00071a58
  000b7352: ldr.w r1,[r8,#0x1c]
  000b7356: blx 0x0007228c
  000b735a: movs r1,#0x35
  000b735c: blx 0x000718e4
  000b7360: ldr.w r1,[r8,#0x20]
  000b7364: strd r9,r5,[sp,#0x10]
  000b7368: cmp r1,#0x0
  000b736a: blt 0x000b737a
  000b736c: add.w r0,r8,#0x4
  000b7370: str r0,[sp,#0x2c]
  000b7372: add.w r0,r10,#0x4
  000b7376: str r0,[sp,#0x28]
  000b7378: b 0x000b7534
  000b737a: ldr.w r1,[r8,#0x14]
  000b737e: mov.w r12,#0x0
  000b7382: mov.w r3,#0xffffffff
  000b7386: movs r6,#0x0
  000b7388: movs r5,#0x0
  000b738a: b 0x000b739e
  000b738c: ldr r2,[r1,#0x4]
  000b738e: adds r5,#0x1
  000b7390: str r3,[r2,r6]
  000b7392: ldr.w r2,[r8,#0x18]
  000b7396: ldr r2,[r2,#0x4]
  000b7398: str.w r12,[r2,r6]
  000b739c: adds r6,#0x4
  000b739e: ldr r2,[r1,#0x0]
  000b73a0: cmp r5,r2
  000b73a2: bcc 0x000b738c
  000b73a4: vmov s0,r0
  000b73a8: ldr.w r1,[r8,#0x20]
  000b73ac: add.w r9,sp,#0x78
  000b73b0: movs r6,#0x0
  000b73b2: vcvt.f32.s32 s16,s0
  000b73b6: addw r0,r1,#0x9c4
  000b73ba: str.w r0,[r8,#0x20]
  000b73be: add.w r0,r8,#0x4
  000b73c2: str r0,[sp,#0x2c]
  000b73c4: add.w r0,r10,#0x4
  000b73c8: str r0,[sp,#0x28]
  000b73ca: ldr.w r0,[r10,#0x0]
  000b73ce: cmp r6,r0
  000b73d0: bcs.w 0x000b7530
  000b73d4: ldr r0,[sp,#0x28]
  000b73d6: ldr r0,[r0,#0x0]
  000b73d8: ldr.w r5,[r0,r6,lsl #0x2]
  000b73dc: mov r0,r5
  000b73de: blx 0x00071ec0
  000b73e2: cmp r0,#0x0
  000b73e4: bne.w 0x000b74f8
  000b73e8: mov r0,r5
  000b73ea: blx 0x00071f50
  000b73ee: cmp r0,#0x0
  000b73f0: bne.w 0x000b74f8
  000b73f4: ldr.w r0,[r8,#0x1c]
  000b73f8: cmp r0,#0x25
  000b73fa: bne 0x000b74ec
  000b73fc: mov r4,r5
  000b73fe: ldr.w r0,[r4,#0x4]!
  000b7402: ldrb.w r1,[r0,#0x5d]
  000b7406: cmp r1,#0x0
  000b7408: beq 0x000b74f8
  000b740a: blx 0x000724f0
  000b740e: mov r11,r0
  000b7410: ldr r0,[r4,#0x0]
  000b7412: blx 0x00072910
  000b7416: cmp r11,r0
  000b7418: bge 0x000b74e8
  000b741a: ldr r1,[r4,#0x0]
  000b741c: mov r0,r9
  000b741e: blx 0x00072964
  000b7422: ldr r5,[sp,#0x2c]
  000b7424: mov r1,r9
  000b7426: mov r0,r5
  000b7428: blx 0x0006eb3c
  000b742c: ldr r0,[sp,#0x10]
  000b742e: blx 0x00072034
  000b7432: mov r1,r0
  000b7434: mov r0,r9
  000b7436: blx 0x0007264c
  000b743a: mov r0,r5
  000b743c: mov r1,r9
  000b743e: blx 0x00072694
  000b7442: mov r0,r5
  000b7444: blx 0x0006ec44
  000b7448: vmov s0,r0
  000b744c: vcmpe.f32 s0,s16
  000b7450: vmrs apsr,fpscr
  000b7454: bhi 0x000b74f8
  000b7456: ldr.w r0,[r8,#0x14]
  000b745a: movs r1,#0x0
  000b745c: movs r2,#0x0
  000b745e: b 0x000b7476
  000b7460: ldr r2,[r0,#0x4]
  000b7462: ldr.w r3,[r2,r1,lsl #0x2]
  000b7466: adds r3,#0x1
  000b7468: beq 0x000b746e
  000b746a: movs r2,#0x0
  000b746c: b 0x000b7474
  000b746e: str.w r6,[r2,r1,lsl #0x2]
  000b7472: movs r2,#0x1
  000b7474: adds r1,#0x1
  000b7476: lsls r2,r2,#0x1f
  000b7478: bne 0x000b7480
  000b747a: ldr r3,[r0,#0x0]
  000b747c: cmp r1,r3
  000b747e: bcc 0x000b7460
  000b7480: cmp r2,#0x0
  000b7482: bne 0x000b74f8
  000b7484: ldr r0,[r4,#0x0]
  000b7486: blx 0x000724f0
  000b748a: ldr r4,[0x000b767c]
  000b748c: mov r9,r0
  000b748e: movs r5,#0x0
  000b7490: mov.w r11,#0xffffffff
  000b7494: b 0x000b74cc
  000b7496: ldr r0,[r0,#0x4]
  000b7498: ldr.w r0,[r0,r5,lsl #0x2]
  000b749c: adds r1,r0,#0x1
  000b749e: beq 0x000b74ca
  000b74a0: ldr r1,[sp,#0x28]
  000b74a2: ldr r1,[r1,#0x0]
  000b74a4: ldr.w r0,[r1,r0,lsl #0x2]
  000b74a8: ldr r0,[r0,#0x4]
  000b74aa: blx 0x000724f0
  000b74ae: cmp r0,r4
  000b74b0: mov.w r1,#0x0
  000b74b4: mov.w r2,#0x0
  000b74b8: it lt
  000b74ba: mov.lt r1,#0x1
  000b74bc: cmp r9,r0
  000b74be: it lt
  000b74c0: mov.lt r2,#0x1
  000b74c2: ands r1,r2
  000b74c4: itt ne
  000b74c6: mov.ne r4,r0
  000b74c8: mov.ne r11,r5
  000b74ca: adds r5,#0x1
  000b74cc: ldr.w r0,[r8,#0x14]
  000b74d0: ldr r1,[r0,#0x0]
  000b74d2: cmp r5,r1
  000b74d4: bcc 0x000b7496
  000b74d6: adds.w r1,r11,#0x1
  000b74da: add.w r9,sp,#0x78
  000b74de: itt ne
  000b74e0: ldr.ne r0,[r0,#0x4]
  000b74e2: str.ne.w r6,[r0,r11,lsl #0x2]
  000b74e6: b 0x000b74f8
  000b74e8: ldr.w r0,[r8,#0x1c]
  000b74ec: cmp r0,#0x29
  000b74ee: itt eq
  000b74f0: ldrb.eq.w r0,[r5,#0x74]
  000b74f4: cmp.eq r0,#0x0
  000b74f6: beq 0x000b74fc
  000b74f8: adds r6,#0x1
  000b74fa: b 0x000b73ca
  000b74fc: ldr.w r0,[r5,#0x4]!
  000b7500: ldrb.w r0,[r0,#0x5c]
  000b7504: cmp r0,#0x0
  000b7506: beq 0x000b74f8
  000b7508: ldr r0,[sp,#0x10]
  000b750a: blx 0x00072034
  000b750e: ldr r0,[r0,#0x0]
  000b7510: blx 0x0007255c
  000b7514: cmp r0,#0x63
  000b7516: bgt 0x000b74f8
  000b7518: ldr r0,[sp,#0x14]
  000b751a: ldr r0,[r0,#0x0]
  000b751c: blx 0x00071a58
  000b7520: movs r1,#0x9
  000b7522: blx 0x0007228c
  000b7526: cmp r0,#0x0
  000b7528: mov r4,r5
  000b752a: bne.w 0x000b741a
  000b752e: b 0x000b74f8
  000b7530: ldr.w r9,[sp,#0x10]
  000b7534: ldr r0,[sp,#0x24]
  000b7536: vmov.f32 s22,0x3f800000
  000b753a: vldr.32 s2,[pc,#0x144]
  000b753e: vmov.f32 s24,0xbf800000
  000b7542: vldr.32 s4,[pc,#0x140]
  000b7546: mov.w r10,#0x0
  000b754a: vmov s0,r0
  000b754e: vldr.32 s20,[pc,#0x138]
  000b7552: movs r0,#0x1
  000b7554: mov.w r11,#0x0
  000b7558: vcvt.f32.s32 s0,s0
  000b755c: ldr r1,[0x000b768c]
  000b755e: movs r4,#0x0
  000b7560: str.w r8,[sp,#0xc]
  000b7564: add r1,pc
  000b7566: ldr r1,[r1,#0x0]
  000b7568: str r1,[sp,#0x20]
  000b756a: vmul.f32 s16,s0,s2
  000b756e: vmul.f32 s18,s0,s4
  000b7572: b 0x000b794e
  000b7574: ldr r1,[r1,#0x4]
  000b7576: ldr.w r1,[r1,r11]
  000b757a: adds r1,#0x1
  000b757c: beq.w 0x000b7948
  000b7580: ldr.w r0,[r8,#0x10]
  000b7584: mov r5,r9
  000b7586: str r4,[sp,#0x1c]
  000b7588: ldr r0,[r0,#0x4]
  000b758a: ldr.w r1,[r0,r11]
  000b758e: ldr r0,[sp,#0x20]
  000b7590: ldr r0,[r0,#0x0]
  000b7592: ldr r1,[r1,#0xc]
  000b7594: blx 0x00072088
  000b7598: ldr r2,[sp,#0x24]
  000b759a: str.w r10,[sp,#0x0]
  000b759e: asrs r3,r2,#0x1f
  000b75a0: str r3,[sp,#0x18]
  000b75a2: blx 0x0006f7cc
  000b75a6: ldr.w r0,[r8,#0x14]
  000b75aa: add.w r9,sp,#0x78
  000b75ae: ldr.w r10,[sp,#0x28]
  000b75b2: ldr r0,[r0,#0x4]
  000b75b4: ldr.w r1,[r10,#0x0]
  000b75b8: ldr.w r0,[r0,r11]
  000b75bc: ldr.w r0,[r1,r0,lsl #0x2]
  000b75c0: ldr r1,[r0,#0x4]
  000b75c2: mov r0,r9
  000b75c4: blx 0x00072964
  000b75c8: ldr r0,[sp,#0x2c]
  000b75ca: mov r1,r9
  000b75cc: blx 0x0006eb3c
  000b75d0: ldr.w r0,[r8,#0x14]
  000b75d4: ldr.w r1,[r10,#0x0]
  000b75d8: ldr r0,[r0,#0x4]
  000b75da: ldr.w r0,[r0,r11]
  000b75de: ldr.w r0,[r1,r0,lsl #0x2]
  000b75e2: ldr r1,[r0,#0x7c]
  000b75e4: cmp r1,#0x2c
  000b75e6: beq 0x000b7650
  000b75e8: add r6,sp,#0x6c
  000b75ea: cmp r1,#0x31
  000b75ec: mov r4,r5
  000b75ee: bne 0x000b7698
  000b75f0: ldr r1,[r0,#0x8]
  000b75f2: add.w r10,sp,#0x54
  000b75f6: mov r0,r10
  000b75f8: blx 0x00072574
  000b75fc: add r5,sp,#0x60
  000b75fe: mov r1,r10
  000b7600: mov r0,r5
  000b7602: blx 0x0006ec80
  000b7606: ldr r2,[0x000b7690]
  000b7608: mov r0,r6
  000b760a: mov r1,r5
  000b760c: blx 0x0006ec74
  000b7610: ldr.w r0,[r8,#0x14]
  000b7614: add r5,sp,#0x30
  000b7616: ldr r1,[sp,#0x28]
  000b7618: ldr r0,[r0,#0x4]
  000b761a: ldr r1,[r1,#0x0]
  000b761c: ldr.w r0,[r0,r11]
  000b7620: ldr.w r0,[r1,r0,lsl #0x2]
  000b7624: ldr r1,[r0,#0x8]
  000b7626: mov r0,r5
  000b7628: blx 0x00072568
  000b762c: add.w r8,sp,#0x3c
  000b7630: mov r1,r5
  000b7632: mov r0,r8
  000b7634: blx 0x0006ec80
  000b7638: add r5,sp,#0x48
  000b763a: ldr r2,[0x000b7694]
  000b763c: mov r1,r8
  000b763e: mov r0,r5
  000b7640: blx 0x0006ec74
  000b7644: mov r0,r9
  000b7646: mov r1,r6
  000b7648: mov r2,r5
  000b764a: blx 0x0006f1cc
  000b764e: b 0x000b76a0
  000b7650: ldr r1,[r0,#0x8]
  000b7652: add r4,sp,#0x60
  000b7654: mov r0,r4
  000b7656: blx 0x00072574
  000b765a: add r6,sp,#0x6c
  000b765c: mov r1,r4
  000b765e: mov r0,r6
  000b7660: blx 0x0006ec80
  000b7664: ldr r2,[0x000b7a1c]
  000b7666: mov r0,r9
  000b7668: mov r1,r6
  000b766a: blx 0x0006ec74
  000b766e: mov r4,r5
  000b7670: b 0x000b76a0
  000b7698: movs r0,#0x0
  000b769a: strd r0,r0,[sp,#0x78]
  000b769e: str r0,[sp,#0x80]
  000b76a0: ldr r5,[sp,#0x2c]
  000b76a2: mov r1,r9
  000b76a4: mov r0,r5
  000b76a6: blx 0x00073534
  000b76aa: mov r0,r4
  000b76ac: blx 0x00072034
  000b76b0: mov r1,r0
  000b76b2: mov r0,r6
  000b76b4: blx 0x0007264c
  000b76b8: mov r0,r5
  000b76ba: mov r1,r6
  000b76bc: blx 0x00072694
  000b76c0: ldr r5,[sp,#0x14]
  000b76c2: ldr r0,[r5,#0x0]
  000b76c4: blx 0x00071a58
  000b76c8: blx 0x000719c8
  000b76cc: mov r10,r11
  000b76ce: cmp r0,#0x2c
  000b76d0: bne 0x000b76ee
  000b76d2: mov r0,r4
  000b76d4: blx 0x00072034
  000b76d8: add r5,sp,#0x60
  000b76da: mov r1,r0
  000b76dc: mov r0,r5
  000b76de: blx 0x00073540
  000b76e2: ldr r2,[0x000b7a1c]
  000b76e4: mov r0,r6
  000b76e6: mov r1,r5
  000b76e8: blx 0x0006ec74
  000b76ec: b 0x000b7750
  000b76ee: ldr r0,[r5,#0x0]
  000b76f0: blx 0x00071a58
  000b76f4: blx 0x000719c8
  000b76f8: cmp r0,#0x31
  000b76fa: bne 0x000b7746
  000b76fc: mov r0,r4
  000b76fe: blx 0x00072034
  000b7702: add.w r8,sp,#0x54
  000b7706: mov r1,r0
  000b7708: mov r0,r8
  000b770a: blx 0x00073540
  000b770e: add r5,sp,#0x60
  000b7710: ldr r2,[0x000b7a1c]
  000b7712: mov r1,r8
  000b7714: mov r0,r5
  000b7716: blx 0x0006ec74
  000b771a: mov r0,r4
  000b771c: blx 0x00072034
  000b7720: add.w r9,sp,#0x3c
  000b7724: mov r1,r0
  000b7726: mov r0,r9
  000b7728: blx 0x0007354c
  000b772c: add.w r8,sp,#0x48
  000b7730: ldr r2,[0x000b7a20]
  000b7732: mov r1,r9
  000b7734: mov r0,r8
  000b7736: blx 0x0006ec74
  000b773a: mov r0,r6
  000b773c: mov r1,r5
  000b773e: mov r2,r8
  000b7740: blx 0x0006f1cc
  000b7744: b 0x000b7750
  000b7746: movs r0,#0x0
  000b7748: add r5,sp,#0x60
  000b774a: strd r0,r0,[sp,#0x6c]
  000b774e: str r0,[sp,#0x74]
  000b7750: ldr r4,[sp,#0x2c]
  000b7752: mov r0,r4
  000b7754: blx 0x0006ec44
  000b7758: ldr.w r8,[sp,#0xc]
  000b775c: mov r3,r0
  000b775e: mov.w r1,#0x3f000000
  000b7762: mov.w r2,#0x3f000000
  000b7766: ldr.w r0,[r8,#0x10]
  000b776a: ldr r0,[r0,#0x4]
  000b776c: ldr.w r0,[r0,r10]
  000b7770: blx 0x000727b4
  000b7774: ldr.w r0,[r8,#0x10]
  000b7778: mov r1,r4
  000b777a: add r4,sp,#0x6c
  000b777c: add.w r11,sp,#0x54
  000b7780: ldr r0,[r0,#0x4]
  000b7782: mov r2,r4
  000b7784: ldr.w r6,[r0,r10]
  000b7788: mov r0,r11
  000b778a: blx 0x0006ec38
  000b778e: mov r0,r5
  000b7790: mov r1,r11
  000b7792: blx 0x0006ec80
  000b7796: mov.w r0,#0x3f800000
  000b779a: add r2,sp,#0x48
  000b779c: str r0,[sp,#0x4c]
  000b779e: movs r0,#0x0
  000b77a0: str r0,[sp,#0x48]
  000b77a2: mov r1,r5
  000b77a4: str r0,[sp,#0x50]
  000b77a6: mov r0,r6
  000b77a8: blx 0x000726ac
  000b77ac: ldr.w r0,[r8,#0x10]
  000b77b0: ldr.w r9,[sp,#0x10]
  000b77b4: ldr r0,[r0,#0x4]
  000b77b6: ldr.w r6,[r0,r10]
  000b77ba: mov r0,r9
  000b77bc: blx 0x00072034
  000b77c0: mov r1,r0
  000b77c2: mov r0,r11
  000b77c4: blx 0x0007264c
  000b77c8: mov r1,r11
  000b77ca: mov r0,r5
  000b77cc: mov r2,r4
  000b77ce: mov r11,r10
  000b77d0: blx 0x0006f1cc
  000b77d4: mov r0,r6
  000b77d6: mov r1,r5
  000b77d8: blx 0x00072148
  000b77dc: ldr.w r0,[r8,#0x10]
  000b77e0: ldr r0,[r0,#0x4]
  000b77e2: ldr.w r1,[r0,r10]
  000b77e6: ldr r0,[sp,#0x20]
  000b77e8: ldr r0,[r0,#0x0]
  000b77ea: ldr r1,[r1,#0xc]
  000b77ec: blx 0x00072088
  000b77f0: ldr r2,[sp,#0x24]
  000b77f2: movs r1,#0x0
  000b77f4: ldr r3,[sp,#0x18]
  000b77f6: mov.w r10,#0x0
  000b77fa: str r1,[sp,#0x0]
  000b77fc: blx 0x0006f7cc
  000b7800: ldr.w r0,[r8,#0x1c]
  000b7804: cmp r0,#0x29
  000b7806: beq 0x000b7856
  000b7808: ldr r4,[sp,#0x1c]
  000b780a: cmp r0,#0x25
  000b780c: bne.w 0x000b7916
  000b7810: ldr.w r0,[r8,#0x14]
  000b7814: ldr r1,[sp,#0x28]
  000b7816: ldr r0,[r0,#0x4]
  000b7818: ldr r1,[r1,#0x0]
  000b781a: ldr.w r0,[r0,r11]
  000b781e: ldr.w r1,[r1,r0,lsl #0x2]
  000b7822: ldr r0,[sp,#0x14]
  000b7824: ldr r0,[r0,#0x0]
  000b7826: ldr r6,[r1,#0x4]
  000b7828: blx 0x00071a58
  000b782c: ldr.w r1,[r8,#0x1c]
  000b7830: blx 0x0007228c
  000b7834: movs r1,#0x36
  000b7836: blx 0x000718e4
  000b783a: vmov s0,r0
  000b783e: mov r0,r6
  000b7840: vcvt.f32.s32 s0,s0
  000b7844: vmul.f32 s0,s18,s0
  000b7848: vdiv.f32 s0,s0,s20
  000b784c: vmov r1,s0
  000b7850: blx 0x00073558
  000b7854: b 0x000b7916
  000b7856: mov r0,r9
  000b7858: blx 0x00072034
  000b785c: ldr r0,[r0,#0x0]
  000b785e: blx 0x0007255c
  000b7862: ldr r4,[sp,#0x1c]
  000b7864: cmp r0,#0x63
  000b7866: bgt 0x000b7916
  000b7868: ldr r5,[sp,#0x14]
  000b786a: ldr r0,[r5,#0x0]
  000b786c: blx 0x00071a58
  000b7870: ldr.w r1,[r8,#0x1c]
  000b7874: blx 0x0007228c
  000b7878: movs r1,#0x36
  000b787a: blx 0x000718e4
  000b787e: vmov s0,r0
  000b7882: vcvt.f32.s32 s0,s0
  000b7886: ldr.w r0,[r8,#0x18]
  000b788a: ldr r0,[r0,#0x4]
  000b788c: add r0,r11
  000b788e: vmul.f32 s0,s16,s0
  000b7892: vldr.32 s2,[r0]
  000b7896: vdiv.f32 s0,s0,s20
  000b789a: vadd.f32 s0,s2,s0
  000b789e: vcmpe.f32 s0,s22
  000b78a2: vstr.32 s0,[r0]
  000b78a6: vmrs apsr,fpscr
  000b78aa: blt 0x000b78e0
  000b78ac: ldr.w r0,[r8,#0x14]
  000b78b0: movs r2,#0x0
  000b78b2: ldr r1,[sp,#0x28]
  000b78b4: mov.w r3,#0xffffffff
  000b78b8: ldr r0,[r0,#0x4]
  000b78ba: ldr r1,[r1,#0x0]
  000b78bc: ldr.w r0,[r0,r11]
  000b78c0: ldr.w r0,[r1,r0,lsl #0x2]
  000b78c4: movs r1,#0x1
  000b78c6: ldr r0,[r0,#0x4]
  000b78c8: blx 0x00072f94
  000b78cc: ldr.w r0,[r8,#0x18]
  000b78d0: ldr r0,[r0,#0x4]
  000b78d2: add r0,r11
  000b78d4: vldr.32 s0,[r0]
  000b78d8: vadd.f32 s0,s0,s24
  000b78dc: vstr.32 s0,[r0]
  000b78e0: mov r0,r9
  000b78e2: blx 0x00072034
  000b78e6: ldr r6,[r0,#0x0]
  000b78e8: ldr r0,[r5,#0x0]
  000b78ea: blx 0x00071a58
  000b78ee: ldr.w r1,[r8,#0x1c]
  000b78f2: blx 0x0007228c
  000b78f6: movs r1,#0x36
  000b78f8: blx 0x000718e4
  000b78fc: vmov s0,r0
  000b7900: mov r0,r6
  000b7902: vcvt.f32.s32 s0,s0
  000b7906: vmul.f32 s0,s16,s0
  000b790a: vdiv.f32 s0,s0,s20
  000b790e: vmov r1,s0
  000b7912: blx 0x000728f8
  000b7916: ldr r0,[0x000b7a40]
  000b7918: ldr r6,[0x000b7a44]
  000b791a: add r0,pc
  000b791c: ldr.w r1,[r8,#0x0]
  000b7920: add r6,pc
  000b7922: ldr r5,[r0,#0x0]
  000b7924: ldr.w r1,[r6,r1,lsl #0x2]
  000b7928: ldr r0,[r5,#0x0]
  000b792a: blx 0x0007153c
  000b792e: cbnz r0,0x000b7946
  000b7930: ldr.w r1,[r8,#0x0]
  000b7934: movs r2,#0x0
  000b7936: ldr r0,[r5,#0x0]
  000b7938: movs r3,#0x0
  000b793a: str.w r10,[sp,#0x0]
  000b793e: ldr.w r1,[r6,r1,lsl #0x2]
  000b7942: blx 0x00071548
  000b7946: movs r0,#0x0
  000b7948: add.w r11,r11,#0x4
  000b794c: adds r4,#0x1
  000b794e: ldr.w r1,[r8,#0x14]
  000b7952: ldr r2,[r1,#0x0]
  000b7954: cmp r4,r2
  000b7956: bcc.w 0x000b7574
  000b795a: lsls r0,r0,#0x1f
  000b795c: beq 0x000b7976
  000b795e: ldr r0,[0x000b7a24]
  000b7960: ldr r1,[0x000b7a28]
  000b7962: add r0,pc
  000b7964: ldr.w r2,[r8,#0x0]
  000b7968: add r1,pc
  000b796a: ldr r0,[r0,#0x0]
  000b796c: ldr.w r1,[r1,r2,lsl #0x2]
  000b7970: ldr r0,[r0,#0x0]
  000b7972: blx 0x000724a8
  000b7976: mov r0,r9
  000b7978: blx 0x00072034
  000b797c: blx 0x000724b4
  000b7980: cbz r0,0x000b79aa
  000b7982: ldr r0,[0x000b7a2c]
  000b7984: ldr r5,[0x000b7a30]
  000b7986: add r0,pc
  000b7988: ldr.w r1,[r8,#0x0]
  000b798c: add r5,pc
  000b798e: ldr r6,[r0,#0x0]
  000b7990: ldr.w r1,[r5,r1,lsl #0x2]
  000b7994: ldr r0,[r6,#0x0]
  000b7996: blx 0x0007153c
  000b799a: cbz r0,0x000b79aa
  000b799c: ldr.w r0,[r8,#0x0]
  000b79a0: ldr.w r1,[r5,r0,lsl #0x2]
  000b79a4: ldr r0,[r6,#0x0]
  000b79a6: blx 0x000724a8
  000b79aa: ldr r0,[0x000b7a34]
  000b79ac: add r0,pc
  000b79ae: ldr r0,[r0,#0x0]
  000b79b0: ldrb r0,[r0,#0xf]
  000b79b2: cbz r0,0x000b79fc
  000b79b4: ldr r0,[0x000b7a38]
  000b79b6: ldr r6,[0x000b7a3c]
  000b79b8: add r0,pc
  000b79ba: ldr.w r1,[r8,#0x0]
  000b79be: add r6,pc
  000b79c0: ldr r5,[r0,#0x0]
  000b79c2: ldr.w r1,[r6,r1,lsl #0x2]
  000b79c6: ldr r0,[r5,#0x0]
  000b79c8: blx 0x0007153c
  000b79cc: cbz r0,0x000b79fc
  000b79ce: mov r0,r9
  000b79d0: ldr.w r10,[r8],#0x4
  000b79d4: blx 0x00072034
  000b79d8: add r4,sp,#0x78
  000b79da: mov r1,r0
  000b79dc: mov r0,r4
  000b79de: blx 0x0007264c
  000b79e2: mov r0,r8
  000b79e4: mov r1,r4
  000b79e6: blx 0x0006eb3c
  000b79ea: ldr.w r1,[r6,r10,lsl #0x2]
  000b79ee: movs r2,#0x0
  000b79f0: ldr r0,[r5,#0x0]
  000b79f2: str r2,[sp,#0x0]
  000b79f4: mov r2,r8
  000b79f6: movs r3,#0x0
  000b79f8: blx 0x000726e8
  000b79fc: ldr r0,[sp,#0x84]
  000b79fe: ldr r1,[sp,#0x8]
  000b7a00: ldr r1,[r1,#0x0]
  000b7a02: subs r0,r1,r0
  000b7a04: itttt eq
  000b7a06: add.eq sp,#0x88
  000b7a08: vpop.eq {d8,d9,d10,d11,d12}
  000b7a0c: add.eq sp,#0x4
  000b7a0e: pop.eq.w {r8,r9,r10,r11}
  000b7a12: it eq
  000b7a14: pop.eq {r4,r5,r6,r7,pc}
  000b7a16: blx 0x0006e824
```
