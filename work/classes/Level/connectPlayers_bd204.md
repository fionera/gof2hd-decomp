# Level::connectPlayers
elf 0xbd204 body 1124
Sig: undefined __stdcall connectPlayers(void)

## decompile
```c

/* Level::connectPlayers() */

void Level::connectPlayers(void)

{
  bool bVar1;
  bool bVar2;
  byte bVar3;
  int in_r0;
  int iVar4;
  Array<Player*> *pAVar5;
  void *pvVar6;
  Mission *this;
  int iVar7;
  int iVar8;
  undefined4 *puVar9;
  Station *pSVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  bool bVar16;
  Array<Player*> *local_3c;
  uint local_38;
  
  iVar4 = AbyssEngine::ApplicationManager::GetCurrentApplicationModule
                    ((ApplicationManager *)**(undefined4 **)(DAT_000cd530 + 0xcd214));
  if (iVar4 != 5) {
    if ((*(int *)(in_r0 + 0xf8) != 0) && (*(int *)(in_r0 + 0xf0) != 0)) {
      pAVar5 = operator_new(0xc);
      Array<Player*>::Array(pAVar5);
      ArraySetLength<Player*>(**(uint **)(in_r0 + 0xf8),(Array *)pAVar5);
      iVar4 = *(int *)pAVar5;
      for (iVar11 = 0; iVar4 != iVar11; iVar11 = iVar11 + 1) {
        *(undefined4 *)(*(int *)(pAVar5 + 4) + iVar11 * 4) =
             *(undefined4 *)(*(int *)(*(int *)(*(int *)(in_r0 + 0xf8) + 4) + iVar11 * 4) + 4);
      }
      Player::setEnemies((Player *)**(undefined4 **)(in_r0 + 0xf0),(Array *)pAVar5);
      pvVar6 = (void *)Array<Player*>::~Array(pAVar5);
      operator_delete(pvVar6);
    }
    if ((*(int *)(in_r0 + 0xfc) != 0) && (*(int *)(in_r0 + 0xf0) != 0)) {
      pAVar5 = operator_new(0xc);
      Array<Player*>::Array(pAVar5);
      ArraySetLength<Player*>(**(uint **)(in_r0 + 0xfc),(Array *)pAVar5);
      iVar4 = *(int *)pAVar5;
      for (iVar11 = 0; iVar4 != iVar11; iVar11 = iVar11 + 1) {
        *(undefined4 *)(*(int *)(pAVar5 + 4) + iVar11 * 4) =
             *(undefined4 *)(*(int *)(*(int *)(*(int *)(in_r0 + 0xfc) + 4) + iVar11 * 4) + 4);
      }
      Player::addEnemies((Player *)**(undefined4 **)(in_r0 + 0xf0),(Array *)pAVar5);
      pvVar6 = (void *)Array<Player*>::~Array(pAVar5);
      operator_delete(pvVar6);
    }
    if ((*(int *)(in_r0 + 0xf4) != 0) && (*(int *)(in_r0 + 0xf0) != 0)) {
      pAVar5 = operator_new(0xc);
      Array<Player*>::Array(pAVar5);
      ArraySetLength<Player*>(**(uint **)(in_r0 + 0xf4),(Array *)pAVar5);
      iVar4 = *(int *)pAVar5;
      for (iVar11 = 0; iVar4 != iVar11; iVar11 = iVar11 + 1) {
        *(undefined4 *)(*(int *)(pAVar5 + 4) + iVar11 * 4) =
             *(undefined4 *)(*(int *)(*(int *)(*(int *)(in_r0 + 0xf4) + 4) + iVar11 * 4) + 4);
      }
      Player::addEnemies((Player *)**(undefined4 **)(in_r0 + 0xf0),(Array *)pAVar5);
      pvVar6 = (void *)Array<Player*>::~Array(pAVar5);
      operator_delete(pvVar6);
    }
    if (*(int *)(in_r0 + 0xf8) != 0) {
      iVar4 = Status::getCurrentCampaignMission();
      for (uVar12 = 0; uVar12 < **(uint **)(in_r0 + 0xf8); uVar12 = uVar12 + 1) {
        iVar11 = *(int *)((*(uint **)(in_r0 + 0xf8))[1] + uVar12 * 4);
        iVar15 = *(int *)(iVar11 + 0x28);
        bVar3 = KIPlayer::isWingMan();
        uVar14 = 0;
        bVar1 = iVar4 != 0x24;
        bVar2 = uVar12 != 0;
        for (uVar13 = 0; uVar13 < **(uint **)(in_r0 + 0xf8); uVar13 = uVar13 + 1) {
          iVar7 = *(int *)((*(uint **)(in_r0 + 0xf8))[1] + uVar13 * 4);
          if ((iVar7 != iVar11) && ((~bVar3 & *(int *)(iVar7 + 0x28) == iVar15) == 0)) {
            if (bVar1 || bVar2) {
              if (iVar4 == 0x9a) {
                iVar7 = Status::inAlienOrbit();
                if (((uVar13 != 8) || (iVar7 == 0)) || (*(int *)(iVar11 + 0x28) != 8))
                goto LAB_000cd3d2;
              }
              else if ((iVar4 != 0x40) || (((uVar12 != 0 && (iVar11 != 0)) && (uVar13 != 0)))) {
LAB_000cd3d2:
                if ((*(char *)(iVar11 + 0x40) != '\0') &&
                   (iVar7 = PlayerFixedObject::getDockingType(), iVar7 == 3)) {
                  pSVar10 = (Station *)Status::getStation();
                  iVar7 = Station::stationHasHiddenBlueprint(pSVar10,true);
                  if (iVar7 != 0) goto LAB_000cd3f4;
                }
                uVar14 = uVar14 + 1;
              }
            }
            else {
              iVar7 = KIPlayer::isWingMan();
              if (iVar7 == 0) goto LAB_000cd3d2;
            }
          }
LAB_000cd3f4:
        }
        pAVar5 = operator_new(0xc);
        Array<Player*>::Array(pAVar5);
        if (*(int *)(in_r0 + 0xf0) != 0) {
          uVar14 = uVar14 + 1;
        }
        ArraySetLength<Player*>(uVar14,(Array *)pAVar5);
        this = (Mission *)Status::getMission();
        iVar7 = Mission::getType();
        if (((((uVar12 & 1) == 0) || (iVar7 != 0xc)) && (iVar7 = Mission::getType(), iVar7 != 2)) &&
           (iVar7 = Mission::getType(), iVar7 != 9)) {
          iVar7 = Mission::isCampaignMission(this);
          if (iVar7 != 0) {
            iVar7 = Status::getCurrentCampaignMission();
            bVar16 = iVar7 == 0x10;
            if (bVar16) {
              iVar7 = *(int *)(iVar11 + 0x28);
            }
            if (bVar16 && iVar7 == 9) goto LAB_000cd44a;
          }
          iVar7 = Mission::isCampaignMission(this);
          if (iVar7 != 0) {
            iVar7 = Status::getCurrentCampaignMission();
            bVar16 = iVar7 == 0x18;
            if (bVar16) {
              iVar7 = *(int *)(iVar11 + 0x28);
            }
            if (bVar16 && iVar7 == 9) goto LAB_000cd44a;
          }
          iVar7 = Mission::isCampaignMission(this);
          if (iVar7 != 0) {
            iVar7 = Status::getCurrentCampaignMission();
            bVar16 = iVar7 == 0x1c;
            if (bVar16) {
              iVar7 = *(int *)(iVar11 + 0x28);
            }
            if (bVar16 && iVar7 == 9) goto LAB_000cd44a;
          }
          iVar7 = Mission::isCampaignMission(this);
          if (iVar7 != 0) {
            Status::getCurrentCampaignMission();
          }
          iVar7 = Mission::isCampaignMission(this);
          if (iVar7 != 0) {
            Status::getCurrentCampaignMission();
          }
          puVar9 = *(undefined4 **)(in_r0 + 0xf0);
          if (puVar9 != (undefined4 *)0x0) {
            **(undefined4 **)(pAVar5 + 4) = *puVar9;
          }
          local_3c = pAVar5 + 4;
          local_38 = (uint)(puVar9 != (undefined4 *)0x0);
          for (uVar13 = 0; uVar13 < **(uint **)(in_r0 + 0xf8); uVar13 = uVar13 + 1) {
            iVar7 = *(int *)((*(uint **)(in_r0 + 0xf8))[1] + uVar13 * 4);
            if ((iVar7 != iVar11) && ((~bVar3 & *(int *)(iVar7 + 0x28) == iVar15) == 0)) {
              if (bVar1 || bVar2) {
                if (iVar4 == 0x9a) {
                  iVar7 = Status::inAlienOrbit();
                  if (((uVar13 != 8) || (iVar7 == 0)) || (*(int *)(iVar11 + 0x28) != 8))
                  goto LAB_000cd620;
                }
                else if ((iVar4 != 0x40) || (((uVar12 != 0 && (iVar11 != 0)) && (uVar13 != 0)))) {
LAB_000cd620:
                  if ((*(char *)(iVar11 + 0x40) != '\0') &&
                     (iVar7 = PlayerFixedObject::getDockingType(), iVar7 == 3)) {
                    pSVar10 = (Station *)Status::getStation();
                    iVar7 = Station::stationHasHiddenBlueprint(pSVar10,true);
                    if (iVar7 != 0) goto LAB_000cd65a;
                  }
                  *(undefined4 *)(*(int *)local_3c + local_38 * 4) =
                       *(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(in_r0 + 0xf8) + 4) + uVar13 * 4) + 4);
                  local_38 = local_38 + 1;
                }
              }
              else {
                iVar7 = KIPlayer::isWingMan();
                if (iVar7 == 0) goto LAB_000cd620;
              }
            }
LAB_000cd65a:
          }
        }
        else {
LAB_000cd44a:
          iVar7 = Player::isAlwaysFriend();
          if (iVar7 == 0) {
            iVar7 = 0;
            for (uVar13 = 0; uVar13 < **(uint **)(in_r0 + 0xf8); uVar13 = uVar13 + 1) {
              iVar8 = *(int *)((*(uint **)(in_r0 + 0xf8))[1] + uVar13 * 4);
              if (((iVar8 != iVar11) && (*(char *)(iVar8 + 0x3c) == '\0')) &&
                 ((~bVar3 & *(int *)(iVar8 + 0x28) == iVar15) == 0)) {
                if (bVar1 || bVar2) {
                  if ((iVar4 != 0x40) || (((uVar12 != 0 && (iVar11 != 0)) && (uVar13 != 0))))
                  goto LAB_000cd4c6;
                }
                else {
                  iVar8 = KIPlayer::isWingMan();
                  if (iVar8 == 0) {
                    iVar8 = *(int *)(*(int *)(*(int *)(in_r0 + 0xf8) + 4) + uVar13 * 4);
LAB_000cd4c6:
                    *(undefined4 *)(*(int *)(pAVar5 + 4) + iVar7 * 4) = *(undefined4 *)(iVar8 + 4);
                    iVar7 = iVar7 + 1;
                  }
                }
              }
            }
          }
          else {
            pvVar6 = (void *)Array<Player*>::~Array(pAVar5);
            operator_delete(pvVar6);
            pAVar5 = operator_new(0xc);
            Array<Player*>::Array(pAVar5);
            ArraySetLength<Player*>(1,(Array *)pAVar5);
          }
          *(undefined4 *)(*(int *)(pAVar5 + 4) + *(int *)pAVar5 * 4 + -4) =
               **(undefined4 **)(in_r0 + 0xf0);
        }
        Player::addEnemies(*(Player **)(iVar11 + 4),(Array *)pAVar5);
        pvVar6 = (void *)Array<Player*>::~Array(pAVar5);
        operator_delete(pvVar6);
        Status::getMission();
        iVar7 = Mission::isEmpty();
        if ((iVar15 == 10) && (iVar7 != 0)) {
          Player::setEnemy(*(Player **)(iVar11 + 4),(Player *)**(undefined4 **)(in_r0 + 0xf0));
        }
      }
    }
  }
  return;
}

```
## target disasm
```
  000cd204: push {r4,r5,r6,r7,lr}
  000cd206: add r7,sp,#0xc
  000cd208: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000cd20c: mov r8,r0
  000cd20e: ldr r0,[0x000cd530]
  000cd210: add r0,pc
  000cd212: ldr r0,[r0,#0x0]
  000cd214: ldr r0,[r0,#0x0]
  000cd216: blx 0x0006e848
  000cd21a: cmp r0,#0x5
  000cd21c: beq.w 0x000cd66a
  000cd220: ldr.w r0,[r8,#0xf8]
  000cd224: cbz r0,0x000cd278
  000cd226: ldr.w r0,[r8,#0xf0]
  000cd22a: cbz r0,0x000cd278
  000cd22c: movs r0,#0xc
  000cd22e: blx 0x0006eb24
  000cd232: mov r5,r0
  000cd234: blx 0x00072e74
  000cd238: ldr.w r0,[r8,#0xf8]
  000cd23c: mov r1,r5
  000cd23e: ldr r0,[r0,#0x0]
  000cd240: blx 0x0007429c
  000cd244: ldr r0,[r5,#0x0]
  000cd246: movs r1,#0x0
  000cd248: b 0x000cd25e
  000cd24a: ldr.w r2,[r8,#0xf8]
  000cd24e: ldr r3,[r5,#0x4]
  000cd250: ldr r2,[r2,#0x4]
  000cd252: ldr.w r2,[r2,r1,lsl #0x2]
  000cd256: ldr r2,[r2,#0x4]
  000cd258: str.w r2,[r3,r1,lsl #0x2]
  000cd25c: adds r1,#0x1
  000cd25e: cmp r0,r1
  000cd260: bne 0x000cd24a
  000cd262: ldr.w r0,[r8,#0xf0]
  000cd266: mov r1,r5
  000cd268: ldr r0,[r0,#0x0]
  000cd26a: blx 0x00072eb0
  000cd26e: mov r0,r5
  000cd270: blx 0x00072e5c
  000cd274: blx 0x0006eb48
  000cd278: ldr.w r0,[r8,#0xfc]
  000cd27c: cbz r0,0x000cd2d0
  000cd27e: ldr.w r0,[r8,#0xf0]
  000cd282: cbz r0,0x000cd2d0
  000cd284: movs r0,#0xc
  000cd286: blx 0x0006eb24
  000cd28a: mov r5,r0
  000cd28c: blx 0x00072e74
  000cd290: ldr.w r0,[r8,#0xfc]
  000cd294: mov r1,r5
  000cd296: ldr r0,[r0,#0x0]
  000cd298: blx 0x0007429c
  000cd29c: ldr r0,[r5,#0x0]
  000cd29e: movs r1,#0x0
  000cd2a0: b 0x000cd2b6
  000cd2a2: ldr.w r2,[r8,#0xfc]
  000cd2a6: ldr r3,[r5,#0x4]
  000cd2a8: ldr r2,[r2,#0x4]
  000cd2aa: ldr.w r2,[r2,r1,lsl #0x2]
  000cd2ae: ldr r2,[r2,#0x4]
  000cd2b0: str.w r2,[r3,r1,lsl #0x2]
  000cd2b4: adds r1,#0x1
  000cd2b6: cmp r0,r1
  000cd2b8: bne 0x000cd2a2
  000cd2ba: ldr.w r0,[r8,#0xf0]
  000cd2be: mov r1,r5
  000cd2c0: ldr r0,[r0,#0x0]
  000cd2c2: blx 0x000742a8
  000cd2c6: mov r0,r5
  000cd2c8: blx 0x00072e5c
  000cd2cc: blx 0x0006eb48
  000cd2d0: ldr.w r0,[r8,#0xf4]
  000cd2d4: cbz r0,0x000cd328
  000cd2d6: ldr.w r0,[r8,#0xf0]
  000cd2da: cbz r0,0x000cd328
  000cd2dc: movs r0,#0xc
  000cd2de: blx 0x0006eb24
  000cd2e2: mov r5,r0
  000cd2e4: blx 0x00072e74
  000cd2e8: ldr.w r0,[r8,#0xf4]
  000cd2ec: mov r1,r5
  000cd2ee: ldr r0,[r0,#0x0]
  000cd2f0: blx 0x0007429c
  000cd2f4: ldr r0,[r5,#0x0]
  000cd2f6: movs r1,#0x0
  000cd2f8: b 0x000cd30e
  000cd2fa: ldr.w r2,[r8,#0xf4]
  000cd2fe: ldr r3,[r5,#0x4]
  000cd300: ldr r2,[r2,#0x4]
  000cd302: ldr.w r2,[r2,r1,lsl #0x2]
  000cd306: ldr r2,[r2,#0x4]
  000cd308: str.w r2,[r3,r1,lsl #0x2]
  000cd30c: adds r1,#0x1
  000cd30e: cmp r0,r1
  000cd310: bne 0x000cd2fa
  000cd312: ldr.w r0,[r8,#0xf0]
  000cd316: mov r1,r5
  000cd318: ldr r0,[r0,#0x0]
  000cd31a: blx 0x000742a8
  000cd31e: mov r0,r5
  000cd320: blx 0x00072e5c
  000cd324: blx 0x0006eb48
  000cd328: ldr.w r0,[r8,#0xf8]
  000cd32c: cmp r0,#0x0
  000cd32e: beq.w 0x000cd66a
  000cd332: ldr r0,[0x000cd534]
  000cd334: add r0,pc
  000cd336: ldr.w r9,[r0,#0x0]
  000cd33a: ldr.w r0,[r9,#0x0]
  000cd33e: blx 0x00071770
  000cd342: movs r4,#0x0
  000cd344: str r0,[sp,#0x14]
  000cd346: str.w r9,[sp,#0xc]
  000cd34a: ldr.w r0,[r8,#0xf8]
  000cd34e: ldr r1,[r0,#0x0]
  000cd350: cmp r4,r1
  000cd352: bcs.w 0x000cd66a
  000cd356: ldr r0,[r0,#0x4]
  000cd358: ldr.w r6,[r0,r4,lsl #0x2]
  000cd35c: mov r0,r6
  000cd35e: ldr.w r11,[r6,#0x28]
  000cd362: blx 0x00072ed4
  000cd366: mov r10,r0
  000cd368: ldr r0,[sp,#0x14]
  000cd36a: str r4,[sp,#0x10]
  000cd36c: movs r5,#0x0
  000cd36e: eor r0,r0,#0x24
  000cd372: orrs r0,r4
  000cd374: movs r4,#0x0
  000cd376: clz r0,r0
  000cd37a: lsrs r0,r0,#0x5
  000cd37c: str r0,[sp,#0x18]
  000cd37e: b 0x000cd3f6
  000cd380: ldr r0,[r0,#0x4]
  000cd382: ldr.w r0,[r0,r4,lsl #0x2]
  000cd386: cmp r0,r6
  000cd388: beq 0x000cd3f4
  000cd38a: ldr r1,[r0,#0x28]
  000cd38c: mvn.w r2,r10
  000cd390: sub.w r1,r1,r11
  000cd394: clz r1,r1
  000cd398: lsrs r1,r1,#0x5
  000cd39a: tst r2,r1
  000cd39c: bne 0x000cd3f4
  000cd39e: ldr r1,[sp,#0x18]
  000cd3a0: cbz r1,0x000cd3aa
  000cd3a2: blx 0x00072ed4
  000cd3a6: cbz r0,0x000cd3d2
  000cd3a8: b 0x000cd3f4
  000cd3aa: ldr r0,[sp,#0x14]
  000cd3ac: cmp r0,#0x9a
  000cd3ae: beq 0x000cd3be
  000cd3b0: cmp r0,#0x40
  000cd3b2: bne 0x000cd3d2
  000cd3b4: ldr r0,[sp,#0x10]
  000cd3b6: cbz r0,0x000cd3f4
  000cd3b8: cbz r6,0x000cd3f4
  000cd3ba: cbnz r4,0x000cd3d2
  000cd3bc: b 0x000cd3f4
  000cd3be: ldr.w r0,[r9,#0x0]
  000cd3c2: blx 0x000723d0
  000cd3c6: cmp r4,#0x8
  000cd3c8: bne 0x000cd3d2
  000cd3ca: cbz r0,0x000cd3d2
  000cd3cc: ldr r0,[r6,#0x28]
  000cd3ce: cmp r0,#0x8
  000cd3d0: beq 0x000cd3f4
  000cd3d2: ldrb.w r0,[r6,#0x40]
  000cd3d6: cbz r0,0x000cd3f2
  000cd3d8: mov r0,r6
  000cd3da: blx 0x00072880
  000cd3de: cmp r0,#0x3
  000cd3e0: bne 0x000cd3f2
  000cd3e2: ldr.w r0,[r9,#0x0]
  000cd3e6: blx 0x00071c14
  000cd3ea: movs r1,#0x1
  000cd3ec: blx 0x00072718
  000cd3f0: cbnz r0,0x000cd3f4
  000cd3f2: adds r5,#0x1
  000cd3f4: adds r4,#0x1
  000cd3f6: ldr.w r0,[r8,#0xf8]
  000cd3fa: ldr r1,[r0,#0x0]
  000cd3fc: cmp r4,r1
  000cd3fe: bcc 0x000cd380
  000cd400: movs r0,#0xc
  000cd402: blx 0x0006eb24
  000cd406: mov r9,r0
  000cd408: blx 0x00072e74
  000cd40c: ldr.w r0,[r8,#0xf0]
  000cd410: mov r1,r9
  000cd412: cmp r0,#0x0
  000cd414: it ne
  000cd416: add.ne r5,#0x1
  000cd418: mov r0,r5
  000cd41a: blx 0x0007429c
  000cd41e: ldr r0,[sp,#0xc]
  000cd420: ldr r0,[r0,#0x0]
  000cd422: blx 0x00072850
  000cd426: mov r5,r0
  000cd428: blx 0x00072874
  000cd42c: ldr r4,[sp,#0x10]
  000cd42e: lsls r1,r4,#0x1f
  000cd430: beq 0x000cd436
  000cd432: cmp r0,#0xc
  000cd434: beq 0x000cd44a
  000cd436: mov r0,r5
  000cd438: blx 0x00072874
  000cd43c: cmp r0,#0x2
  000cd43e: beq 0x000cd44a
  000cd440: mov r0,r5
  000cd442: blx 0x00072874
  000cd446: cmp r0,#0x9
  000cd448: bne 0x000cd538
  000cd44a: mov r5,r6
  000cd44c: ldr.w r0,[r5,#0x4]!
  000cd450: blx 0x0007420c
  000cd454: cbz r0,0x000cd476
  000cd456: mov r0,r9
  000cd458: blx 0x00072e5c
  000cd45c: blx 0x0006eb48
  000cd460: movs r0,#0xc
  000cd462: blx 0x0006eb24
  000cd466: mov r9,r0
  000cd468: blx 0x00072e74
  000cd46c: movs r0,#0x1
  000cd46e: mov r1,r9
  000cd470: blx 0x0007429c
  000cd474: b 0x000cd4e2
  000cd476: str r5,[sp,#0x8]
  000cd478: movs r4,#0x0
  000cd47a: movs r5,#0x0
  000cd47c: b 0x000cd4d4
  000cd47e: ldr r0,[r0,#0x4]
  000cd480: ldr.w r0,[r0,r5,lsl #0x2]
  000cd484: cmp r0,r6
  000cd486: beq 0x000cd4d2
  000cd488: ldrb.w r1,[r0,#0x3c]
  000cd48c: cbnz r1,0x000cd4d2
  000cd48e: ldr r1,[r0,#0x28]
  000cd490: mvn.w r2,r10
  000cd494: sub.w r1,r1,r11
  000cd498: clz r1,r1
  000cd49c: lsrs r1,r1,#0x5
  000cd49e: tst r2,r1
  000cd4a0: bne 0x000cd4d2
  000cd4a2: ldr r1,[sp,#0x18]
  000cd4a4: cbz r1,0x000cd4b8
  000cd4a6: blx 0x00072ed4
  000cd4aa: cbnz r0,0x000cd4d2
  000cd4ac: ldr.w r0,[r8,#0xf8]
  000cd4b0: ldr r0,[r0,#0x4]
  000cd4b2: ldr.w r0,[r0,r5,lsl #0x2]
  000cd4b6: b 0x000cd4c6
  000cd4b8: ldr r1,[sp,#0x14]
  000cd4ba: cmp r1,#0x40
  000cd4bc: bne 0x000cd4c6
  000cd4be: ldr r1,[sp,#0x10]
  000cd4c0: cbz r1,0x000cd4d2
  000cd4c2: cbz r6,0x000cd4d2
  000cd4c4: cbz r5,0x000cd4d2
  000cd4c6: ldr.w r1,[r9,#0x4]
  000cd4ca: ldr r0,[r0,#0x4]
  000cd4cc: str.w r0,[r1,r4,lsl #0x2]
  000cd4d0: adds r4,#0x1
  000cd4d2: adds r5,#0x1
  000cd4d4: ldr.w r0,[r8,#0xf8]
  000cd4d8: ldr r1,[r0,#0x0]
  000cd4da: cmp r5,r1
  000cd4dc: bcc 0x000cd47e
  000cd4de: ldr r4,[sp,#0x10]
  000cd4e0: ldr r5,[sp,#0x8]
  000cd4e2: ldrd r0,r1,[r9,#0x0]
  000cd4e6: ldr.w r2,[r8,#0xf0]
  000cd4ea: add.w r0,r1,r0, lsl #0x2
  000cd4ee: ldr r2,[r2,#0x0]
  000cd4f0: str.w r2,[r0,#-0x4]
  000cd4f4: ldr r0,[r5,#0x0]
  000cd4f6: mov r1,r9
  000cd4f8: blx 0x000742a8
  000cd4fc: mov r0,r9
  000cd4fe: blx 0x00072e5c
  000cd502: blx 0x0006eb48
  000cd506: ldr.w r9,[sp,#0xc]
  000cd50a: ldr.w r0,[r9,#0x0]
  000cd50e: blx 0x00072850
  000cd512: blx 0x00072868
  000cd516: cmp.w r11,#0xa
  000cd51a: bne 0x000cd52a
  000cd51c: cbz r0,0x000cd52a
  000cd51e: ldr.w r1,[r8,#0xf0]
  000cd522: ldr r0,[r5,#0x0]
  000cd524: ldr r1,[r1,#0x0]
  000cd526: blx 0x00072ebc
  000cd52a: adds r4,#0x1
  000cd52c: b 0x000cd34a
  000cd538: mov r0,r5
  000cd53a: blx 0x0007372c
  000cd53e: cbz r0,0x000cd554
  000cd540: ldr r0,[sp,#0xc]
  000cd542: ldr r0,[r0,#0x0]
  000cd544: blx 0x00071770
  000cd548: cmp r0,#0x10
  000cd54a: itt eq
  000cd54c: ldr.eq r0,[r6,#0x28]
  000cd54e: cmp.eq r0,#0x9
  000cd550: beq.w 0x000cd44a
  000cd554: mov r0,r5
  000cd556: blx 0x0007372c
  000cd55a: cbz r0,0x000cd570
  000cd55c: ldr r0,[sp,#0xc]
  000cd55e: ldr r0,[r0,#0x0]
  000cd560: blx 0x00071770
  000cd564: cmp r0,#0x18
  000cd566: itt eq
  000cd568: ldr.eq r0,[r6,#0x28]
  000cd56a: cmp.eq r0,#0x9
  000cd56c: beq.w 0x000cd44a
  000cd570: mov r0,r5
  000cd572: blx 0x0007372c
  000cd576: cbz r0,0x000cd58c
  000cd578: ldr r0,[sp,#0xc]
  000cd57a: ldr r0,[r0,#0x0]
  000cd57c: blx 0x00071770
  000cd580: cmp r0,#0x1c
  000cd582: itt eq
  000cd584: ldr.eq r0,[r6,#0x28]
  000cd586: cmp.eq r0,#0x9
  000cd588: beq.w 0x000cd44a
  000cd58c: mov r0,r5
  000cd58e: blx 0x0007372c
  000cd592: cbz r0,0x000cd59c
  000cd594: ldr r0,[sp,#0xc]
  000cd596: ldr r0,[r0,#0x0]
  000cd598: blx 0x00071770
  000cd59c: mov r0,r5
  000cd59e: blx 0x0007372c
  000cd5a2: cbz r0,0x000cd5ac
  000cd5a4: ldr r0,[sp,#0xc]
  000cd5a6: ldr r0,[r0,#0x0]
  000cd5a8: blx 0x00071770
  000cd5ac: ldr.w r0,[r8,#0xf0]
  000cd5b0: cbz r0,0x000cd5c2
  000cd5b2: mov r2,r9
  000cd5b4: ldr r0,[r0,#0x0]
  000cd5b6: ldr.w r1,[r2,#0x4]!
  000cd5ba: str r2,[sp,#0x4]
  000cd5bc: str r0,[r1,#0x0]
  000cd5be: movs r0,#0x1
  000cd5c0: b 0x000cd5ca
  000cd5c2: add.w r0,r9,#0x4
  000cd5c6: str r0,[sp,#0x4]
  000cd5c8: movs r0,#0x0
  000cd5ca: movs r5,#0x0
  000cd5cc: str r0,[sp,#0x8]
  000cd5ce: b 0x000cd65c
  000cd5d0: ldr r0,[r0,#0x4]
  000cd5d2: ldr.w r0,[r0,r5,lsl #0x2]
  000cd5d6: cmp r0,r6
  000cd5d8: beq 0x000cd65a
  000cd5da: ldr r1,[r0,#0x28]
  000cd5dc: mvn.w r2,r10
  000cd5e0: sub.w r1,r1,r11
  000cd5e4: clz r1,r1
  000cd5e8: lsrs r1,r1,#0x5
  000cd5ea: tst r2,r1
  000cd5ec: bne 0x000cd65a
  000cd5ee: ldr r1,[sp,#0x18]
  000cd5f0: cbz r1,0x000cd5fa
  000cd5f2: blx 0x00072ed4
  000cd5f6: cbz r0,0x000cd620
  000cd5f8: b 0x000cd65a
  000cd5fa: ldr r0,[sp,#0x14]
  000cd5fc: cmp r0,#0x9a
  000cd5fe: beq 0x000cd60c
  000cd600: cmp r0,#0x40
  000cd602: bne 0x000cd620
  000cd604: cbz r4,0x000cd65a
  000cd606: cbz r6,0x000cd65a
  000cd608: cbnz r5,0x000cd620
  000cd60a: b 0x000cd65a
  000cd60c: ldr r0,[sp,#0xc]
  000cd60e: ldr r0,[r0,#0x0]
  000cd610: blx 0x000723d0
  000cd614: cmp r5,#0x8
  000cd616: bne 0x000cd620
  000cd618: cbz r0,0x000cd620
  000cd61a: ldr r0,[r6,#0x28]
  000cd61c: cmp r0,#0x8
  000cd61e: beq 0x000cd65a
  000cd620: ldrb.w r0,[r6,#0x40]
  000cd624: cbz r0,0x000cd640
  000cd626: mov r0,r6
  000cd628: blx 0x00072880
  000cd62c: cmp r0,#0x3
  000cd62e: bne 0x000cd640
  000cd630: ldr r0,[sp,#0xc]
  000cd632: ldr r0,[r0,#0x0]
  000cd634: blx 0x00071c14
  000cd638: movs r1,#0x1
  000cd63a: blx 0x00072718
  000cd63e: cbnz r0,0x000cd65a
  000cd640: ldr.w r0,[r8,#0xf8]
  000cd644: ldr r1,[sp,#0x4]
  000cd646: ldr r2,[sp,#0x8]
  000cd648: ldr r0,[r0,#0x4]
  000cd64a: ldr r1,[r1,#0x0]
  000cd64c: ldr.w r0,[r0,r5,lsl #0x2]
  000cd650: ldr r0,[r0,#0x4]
  000cd652: str.w r0,[r1,r2,lsl #0x2]
  000cd656: adds r2,#0x1
  000cd658: str r2,[sp,#0x8]
  000cd65a: adds r5,#0x1
  000cd65c: ldr.w r0,[r8,#0xf8]
  000cd660: ldr r1,[r0,#0x0]
  000cd662: cmp r5,r1
  000cd664: bcc 0x000cd5d0
  000cd666: adds r5,r6,#0x4
  000cd668: b 0x000cd4f4
  000cd66a: add sp,#0x1c
  000cd66c: pop.w {r8,r9,r10,r11}
  000cd670: pop {r4,r5,r6,r7,pc}
```
