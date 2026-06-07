# Generator::getItemBuyList
elf 0x95428 body 1760
Sig: undefined __thiscall getItemBuyList(Generator * this, Station * param_1)

## decompile
```c

/* Generator::getItemBuyList(Station*) */

Array<Item*> * __thiscall Generator::getItemBuyList(Generator *this,Station *param_1)

{
  bool bVar1;
  bool bVar2;
  float fVar3;
  int iVar4;
  Array<Item*> *this_00;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  Item *pIVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  uint uVar15;
  undefined4 uVar16;
  int iVar17;
  int *piVar18;
  code *pcVar19;
  uint *puVar20;
  uint uVar21;
  uint uVar22;
  int iVar23;
  uint in_fpscr;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  
  iVar4 = Station::getIndex(param_1);
  if ((iVar4 == 0x4e) && (iVar4 = Status::getCurrentCampaignMission(), iVar4 < 7)) {
    this_00 = operator_new(0xc);
    Array<Item*>::Array(this_00);
    ArraySetLength<Item*>(3,(Array *)this_00);
    piVar18 = *(int **)(DAT_000a57c4 + 0xa546c);
    pcVar19 = *(code **)(DAT_000a57c8 + 0xa5476);
    uVar5 = (*pcVar19)(**(undefined4 **)(*piVar18 + 4),1,0);
    **(undefined4 **)(this_00 + 4) = uVar5;
    uVar5 = (*pcVar19)(*(undefined4 *)(*(int *)(*piVar18 + 4) + 0x58),1,0);
    *(undefined4 *)(*(int *)(this_00 + 4) + 4) = uVar5;
    uVar5 = (*pcVar19)(*(undefined4 *)(*(int *)(*piVar18 + 4) + 0xdc),1,0);
    *(undefined4 *)(*(int *)(this_00 + 4) + 8) = uVar5;
  }
  else {
    iVar4 = Station::getIndex(param_1);
    if ((iVar4 == 0x6c) || (iVar4 = Status::inSupernovaSystem(), iVar4 != 0)) {
      this_00 = (Array<Item*> *)0x0;
    }
    else {
      uVar6 = Station::getIndex(param_1);
      this_00 = operator_new(0xc);
      Array<Item*>::Array(this_00);
      piVar18 = *(int **)(DAT_000a57d4 + 0xa54e0);
      puVar20 = (uint *)**(undefined4 **)(DAT_000a57d0 + 0xa54de);
      iVar4 = Galaxy::getSystems((Galaxy *)*piVar18);
      iVar7 = Station::getTecLevel();
      iVar17 = iVar7 / 2;
      if (iVar7 < 4) {
        iVar17 = 1;
      }
      if ((uVar6 | 2) == 0x6b) {
        iVar17 = 0;
      }
      iVar8 = Status::getCurrentCampaignMission();
      if ((iVar8 == 0x8b) && (iVar8 = Station::getSystem(param_1), iVar8 == 0x19)) {
        pIVar9 = (Item *)Item::makeItem();
        ArrayAdd<Item*>(pIVar9,(Array *)this_00);
      }
      if (uVar6 == 0x7e) {
        iVar8 = Item::makeItem();
        iVar10 = Status::getCurrentCampaignMission();
        if (iVar10 != 0x75) {
          AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a57d8 + 0xa5564));
        }
        pIVar9 = (Item *)Item::makeItem(iVar8);
        ArrayAdd<Item*>(pIVar9,(Array *)this_00);
      }
      piVar11 = *(int **)(DAT_000a57dc + 0xa5580);
      AbyssEngine::AERandom::reset();
      iVar8 = Status::getCurrentCampaignMission();
      fVar3 = DAT_000a57e0;
      fVar25 = 1.5;
      fVar24 = (float)VectorSignedToFloat(iVar8 + 0x19,(byte)(in_fpscr >> 0x16) & 3);
      in_fpscr = in_fpscr & 0xfffffff;
      if (fVar24 / DAT_000a57e0 < 1.5) {
        iVar8 = Status::getCurrentCampaignMission();
        fVar25 = (float)VectorSignedToFloat(iVar8 + 0x19,(byte)(in_fpscr >> 0x16) & 3);
        fVar25 = fVar25 / fVar3;
      }
      fVar24 = DAT_000a57ec;
      fVar3 = DAT_000a57e8;
      iVar8 = DAT_000a57e4 + 0xa55dc;
      for (uVar22 = 0; uVar22 < *puVar20; uVar22 = uVar22 + 1) {
        pIVar9 = *(Item **)(puVar20[1] + uVar22 * 4);
        iVar10 = Item::getIndex(pIVar9);
        if (iVar10 < 0x84) {
          bVar1 = false;
        }
        else {
          iVar10 = Item::getIndex(pIVar9);
          bVar1 = iVar10 < 0x9a;
        }
        iVar10 = Station::getIndex(param_1);
        iVar12 = Item::getAttribute((int)pIVar9);
        if (((iVar10 == iVar12) &&
            (((iVar10 = Item::getIndex(pIVar9), iVar10 < 0xc4 ||
              (iVar10 = Item::getIndex(pIVar9), 0xc4 < iVar10)) ||
             (iVar10 = Status::getCurrentCampaignMission(), 0x8d < iVar10)))) &&
           (((iVar10 = Item::getIndex(pIVar9), iVar10 < 0xc6 ||
             (iVar10 = Item::getIndex(pIVar9), 200 < iVar10)) ||
            (iVar10 = Status::getCurrentCampaignMission(), 0x8d < iVar10)))) {
          bVar2 = true;
        }
        else {
          bVar2 = false;
        }
        if (uVar6 == 0x6a) {
          uVar21 = 0;
          do {
            if (9 < uVar21) goto LAB_000a5a52;
            iVar12 = *(int *)(iVar8 + uVar21 * 4);
            iVar10 = Item::getIndex(pIVar9);
            uVar21 = uVar21 + 1;
          } while ((bool)(~bVar1 & iVar12 != iVar10));
        }
        iVar10 = Item::getOccurence(pIVar9);
        iVar12 = Item::getIndex(pIVar9);
        if ((iVar12 == 0x7a) && (**(int **)(DAT_000a57f0 + 0xa56be) != 0)) {
          fVar26 = (float)VectorSignedToFloat(iVar10 * **(int **)(DAT_000a57f0 + 0xa56be),
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar27 = (float)VectorSignedToFloat(iVar10,(byte)(in_fpscr >> 0x16) & 3);
          iVar10 = (int)(fVar27 + fVar26 * fVar3);
        }
        iVar12 = Item::getType(pIVar9);
        if ((iVar12 == 1) && (**(int **)(DAT_000a57f4 + 0xa56f0) != 0)) {
          fVar26 = (float)VectorSignedToFloat(iVar10 * **(int **)(DAT_000a57f4 + 0xa56f0),
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar27 = (float)VectorSignedToFloat(iVar10,(byte)(in_fpscr >> 0x16) & 3);
          iVar10 = (int)(fVar27 + fVar26 * fVar3);
        }
        uVar21 = Item::getIndex(pIVar9);
        uVar13 = Item::getSort(pIVar9);
        iVar12 = Status::getCurrentCampaignMission();
        if ((iVar10 == 0) &&
           (((0xc3 < (int)uVar21 && (*(char *)(*(int *)(DAT_000a57f8 + 0xa5736) + 0x37) != '\0')) ||
            ((iVar10 = 0, (int)uVar21 < 0xc4 &&
             (*(char *)(*(int *)(DAT_000a57f8 + 0xa5736) + 0x35) != '\0')))))) {
          iVar14 = Item::getType(pIVar9);
          iVar10 = 0;
          if (uVar21 != 0x55 && iVar14 != 4) {
            iVar10 = Item::getIngredients();
            if ((iVar10 == 0) && ((uVar21 != 0xb5 || (0x3a < iVar12)))) {
              iVar10 = 0;
              if (((uVar13 != 0x22 && (uVar13 | 2) != 0x23) || (0x8d < iVar12)) &&
                 ((uVar13 != 0x24 && (0x8d < iVar12 || uVar13 != 0x2b)))) {
                iVar10 = 0;
                if ((1 < (uVar21 & 0xfffffff7) - 0xd1) && (0x5d < iVar12 || uVar21 != 0xcd)) {
                  uVar15 = 0;
                  do {
                    if (8 < uVar15) {
                      if ((uVar13 == 0x1d) && (iVar10 = Status::inBlackMarketSystem(), iVar10 == 0))
                      goto LAB_000a5772;
                      uVar5 = AbyssEngine::AERandom::nextInt(*piVar11);
                      uVar16 = Item::getTecLevel(pIVar9);
                      fVar26 = (float)VectorSignedToFloat(uVar16,(byte)(in_fpscr >> 0x16) & 3);
                      fVar27 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
                      iVar10 = (int)(fVar27 + (fVar26 / -10.0 + 1.0) * 30.0);
                      break;
                    }
                    iVar10 = 0;
                    iVar12 = uVar15 * 4;
                    uVar15 = uVar15 + 1;
                  } while (*(uint *)(DAT_000a5b54 + 0xa5ade + iVar12) != uVar21);
                }
              }
            }
            else {
LAB_000a5772:
              iVar10 = 0;
            }
          }
        }
        iVar12 = Item::getTecLevel(pIVar9);
        if (bVar2) {
LAB_000a5784:
          iVar14 = Status::hardCoreMode();
          if ((iVar14 == 0) ||
             ((iVar14 = Item::getSort(pIVar9), iVar14 != 0x17 &&
              (iVar14 = Item::getSort(pIVar9), iVar14 != 0x18)))) {
            if (uVar6 == 0x6b) {
              iVar14 = Item::getType(pIVar9);
              if (iVar14 == 3) goto LAB_000a58aa;
            }
            else if (uVar6 == 0x69) {
              iVar14 = Item::isWeapon(pIVar9);
              if ((iVar14 != 0) || (iVar14 = Item::getSort(pIVar9), iVar14 == 0x1c))
              goto LAB_000a58aa;
            }
            else if ((uVar6 != 0x65) || (iVar14 = Item::isWeapon(pIVar9), iVar14 != 0)) {
LAB_000a58aa:
              if ((uVar6 != 0x6a) || (iVar14 = Item::getType(pIVar9), iVar14 == 4)) {
                fVar26 = (float)VectorSignedToFloat(iVar10,(byte)(in_fpscr >> 0x16) & 3);
                if ((bVar2) ||
                   ((((iVar12 <= iVar7 || (bVar1)) &&
                     (iVar10 = AbyssEngine::AERandom::nextInt(*piVar11),
                     iVar10 < (int)(fVar25 * fVar26))) &&
                    (((iVar10 = Item::getIndex(pIVar9), iVar17 <= iVar12 || (iVar10 == 0x7a)) ||
                     (iVar10 = AbyssEngine::AERandom::nextInt(*piVar11), iVar10 < 0x3d)))))) {
                  iVar10 = Item::getMinPriceSystem(pIVar9);
                  iVar10 = SolarSystem::getX(*(SolarSystem **)(*(int *)(iVar4 + 4) + iVar10 * 4));
                  iVar12 = Item::getMinPriceSystem(pIVar9);
                  SolarSystem::getY(*(SolarSystem **)(*(int *)(iVar4 + 4) + iVar12 * 4));
                  iVar23 = *piVar18;
                  iVar12 = Station::getSystem(param_1);
                  iVar12 = SolarSystem::getX(*(SolarSystem **)(*(int *)(iVar4 + 4) + iVar12 * 4));
                  iVar14 = Station::getSystem(param_1);
                  iVar14 = SolarSystem::getY(*(SolarSystem **)(*(int *)(iVar4 + 4) + iVar14 * 4));
                  iVar10 = Galaxy::invDistancePercent(iVar23,iVar12,iVar14,iVar10);
                  iVar12 = AbyssEngine::AERandom::nextInt(*piVar11);
                  iVar12 = iVar12 + 5;
                  iVar14 = Item::getType(pIVar9);
                  if ((iVar14 == 4) || (iVar14 = Item::getType(pIVar9), iVar14 == 1)) {
                    iVar14 = Item::getIndex(pIVar9);
                    if ((iVar14 != 0x6d) && (iVar14 = Item::getType(pIVar9), iVar14 == 4)) {
                      if (0x32 < iVar10) {
                        fVar26 = (float)VectorSignedToFloat(iVar10 + -0x32,
                                                            (byte)(in_fpscr >> 0x16) & 3);
                        iVar10 = Status::hardCoreMode();
                        fVar27 = 20.0;
                        if (iVar10 != 0) {
                          fVar27 = 2.0;
                        }
                        iVar10 = (int)((fVar26 / fVar24) * fVar27);
                        if (iVar10 < 2) {
                          iVar10 = 1;
                        }
                        iVar12 = iVar10 * iVar12;
                      }
                      iVar10 = Item::getIndex(pIVar9);
                      if ((iVar10 == 0x6e) &&
                         (iVar10 = AbyssEngine::AERandom::nextInt(*piVar11), iVar10 + 10 < iVar12))
                      {
                        AbyssEngine::AERandom::nextInt(*piVar11);
                      }
                    }
                  }
                  else {
                    __aeabi_idiv(iVar12,5);
                  }
                  pIVar9 = (Item *)Item::makeItem((int)pIVar9);
                  ArrayAdd<Item*>(pIVar9,(Array *)this_00);
                }
              }
            }
          }
        }
        else {
          iVar14 = Item::getIngredients();
          if (((((iVar14 == 0) && (uVar21 = uVar22 & 0x7fffffff, 1 < uVar21 - 0xd9)) &&
               (uVar21 != 0xa4 && uVar21 != 0xaf)) &&
              ((iVar14 = Station::getTecLevel(), iVar10 != 0 && (iVar12 <= iVar14)))) &&
             (iVar14 = Item::getSinglePrice(pIVar9), iVar14 != 0)) {
            iVar14 = Item::getAttribute((int)pIVar9);
            if (iVar14 == 1) {
              Station::getSystem(param_1);
              iVar14 = SolarSystem::getRace();
              if (iVar14 != 1) goto LAB_000a5a52;
            }
            if ((uVar6 != 0x6a) && (bVar1)) {
              iVar14 = Item::getIndex(pIVar9);
              iVar23 = Station::getSystem(param_1);
              if (iVar14 != iVar23 + 0x84) goto LAB_000a5a52;
            }
            goto LAB_000a5784;
          }
        }
LAB_000a5a52:
      }
    }
  }
  return this_00;
}

```

## target disasm
```
  000a5428: push {r4,r5,r6,r7,lr}
  000a542a: add r7,sp,#0xc
  000a542c: push {r7,r8,r9,r10,r11}
  000a5430: vpush {d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  000a5434: mov r0,r1
  000a5436: mov r10,r1
  000a5438: blx 0x00071824
  000a543c: cmp r0,#0x4e
  000a543e: bne 0x000a54a4
  000a5440: ldr r0,[0x000a57c0]
  000a5442: add r0,pc
  000a5444: ldr r0,[r0,#0x0]
  000a5446: ldr r0,[r0,#0x0]
  000a5448: blx 0x00071770
  000a544c: cmp r0,#0x6
  000a544e: bgt 0x000a54a4
  000a5450: movs r0,#0xc
  000a5452: blx 0x0006eb24
  000a5456: mov r6,r0
  000a5458: blx 0x00071860
  000a545c: movs r0,#0x3
  000a545e: mov r1,r6
  000a5460: blx 0x0007186c
  000a5464: ldr r0,[0x000a57c4]
  000a5466: movs r2,#0x0
  000a5468: add r0,pc
  000a546a: ldr r4,[r0,#0x0]
  000a546c: ldr r0,[r4,#0x0]
  000a546e: ldr r1,[0x000a57c8]
  000a5470: ldr r0,[r0,#0x4]
  000a5472: add r1,pc
  000a5474: ldr r5,[r1,#0x0]
  000a5476: movs r1,#0x1
  000a5478: ldr r0,[r0,#0x0]
  000a547a: blx r5
  000a547c: ldr r1,[r6,#0x4]
  000a547e: movs r2,#0x0
  000a5480: str r0,[r1,#0x0]
  000a5482: movs r1,#0x1
  000a5484: ldr r0,[r4,#0x0]
  000a5486: ldr r0,[r0,#0x4]
  000a5488: ldr r0,[r0,#0x58]
  000a548a: blx r5
  000a548c: ldr r1,[r6,#0x4]
  000a548e: movs r2,#0x0
  000a5490: str r0,[r1,#0x4]
  000a5492: movs r1,#0x1
  000a5494: ldr r0,[r4,#0x0]
  000a5496: ldr r0,[r0,#0x4]
  000a5498: ldr.w r0,[r0,#0xdc]
  000a549c: blx r5
  000a549e: ldr r1,[r6,#0x4]
  000a54a0: str r0,[r1,#0x8]
  000a54a2: b 0x000a5b36
  000a54a4: mov r0,r10
  000a54a6: blx 0x00071824
  000a54aa: cmp r0,#0x6c
  000a54ac: beq 0x000a54be
  000a54ae: ldr r0,[0x000a57cc]
  000a54b0: add r0,pc
  000a54b2: ldr r0,[r0,#0x0]
  000a54b4: str r0,[sp,#0x2c]
  000a54b6: ldr r0,[r0,#0x0]
  000a54b8: blx 0x00071878
  000a54bc: cbz r0,0x000a54c2
  000a54be: movs r6,#0x0
  000a54c0: b 0x000a5b36
  000a54c2: mov r0,r10
  000a54c4: blx 0x00071824
  000a54c8: mov r4,r0
  000a54ca: movs r0,#0xc
  000a54cc: blx 0x0006eb24
  000a54d0: mov r6,r0
  000a54d2: blx 0x00071860
  000a54d6: ldr r0,[0x000a57d0]
  000a54d8: ldr r1,[0x000a57d4]
  000a54da: add r0,pc
  000a54dc: add r1,pc
  000a54de: ldr r0,[r0,#0x0]
  000a54e0: ldr r1,[r1,#0x0]
  000a54e2: str r1,[sp,#0x10]
  000a54e4: ldr r5,[r0,#0x0]
  000a54e6: ldr r0,[r1,#0x0]
  000a54e8: blx 0x00071884
  000a54ec: str r0,[sp,#0x14]
  000a54ee: mov r0,r10
  000a54f0: blx 0x00071890
  000a54f4: mov r3,r0
  000a54f6: ldr r0,[sp,#0x2c]
  000a54f8: add.w r2,r3,r3, lsr #0x1f
  000a54fc: str r4,[sp,#0x28]
  000a54fe: orr r1,r4,#0x2
  000a5502: str r3,[sp,#0xc]
  000a5504: ldr r0,[r0,#0x0]
  000a5506: asrs r2,r2,#0x1
  000a5508: cmp r3,#0x4
  000a550a: it lt
  000a550c: mov.lt r2,#0x1
  000a550e: cmp r1,#0x6b
  000a5510: it eq
  000a5512: sub.eq.w r2,r1,#0x6b
  000a5516: str r2,[sp,#0x8]
  000a5518: blx 0x00071770
  000a551c: cmp r0,#0x8b
  000a551e: bne 0x000a553a
  000a5520: mov r0,r10
  000a5522: blx 0x0007189c
  000a5526: cmp r0,#0x19
  000a5528: bne 0x000a553a
  000a552a: ldr r0,[r5,#0x4]
  000a552c: ldr.w r0,[r0,#0x2f8]
  000a5530: blx 0x000718a8
  000a5534: mov r1,r6
  000a5536: blx 0x000718b4
  000a553a: ldr r0,[sp,#0x28]
  000a553c: cmp r0,#0x7e
  000a553e: bne 0x000a5578
  000a5540: ldr r0,[r5,#0x4]
  000a5542: ldr.w r0,[r0,#0x344]
  000a5546: blx 0x000718a8
  000a554a: mov r4,r0
  000a554c: ldr r0,[sp,#0x2c]
  000a554e: ldr r0,[r0,#0x0]
  000a5550: blx 0x00071770
  000a5554: cmp r0,#0x75
  000a5556: bne 0x000a555c
  000a5558: movs r1,#0x1
  000a555a: b 0x000a556c
  000a555c: ldr r0,[0x000a57d8]
  000a555e: movs r1,#0xa
  000a5560: add r0,pc
  000a5562: ldr r0,[r0,#0x0]
  000a5564: ldr r0,[r0,#0x0]
  000a5566: blx 0x00071848
  000a556a: adds r1,r0,#0x1
  000a556c: mov r0,r4
  000a556e: blx 0x000718c0
  000a5572: mov r1,r6
  000a5574: blx 0x000718b4
  000a5578: str r6,[sp,#0x18]
  000a557a: ldr r0,[0x000a57dc]
  000a557c: add r0,pc
  000a557e: ldr r0,[r0,#0x0]
  000a5580: str r0,[sp,#0x1c]
  000a5582: ldr r0,[r0,#0x0]
  000a5584: blx 0x000718cc
  000a5588: ldr r0,[sp,#0x2c]
  000a558a: ldr r0,[r0,#0x0]
  000a558c: blx 0x00071770
  000a5590: adds r0,#0x19
  000a5592: vldr.32 s18,[pc,#0x24c]
  000a5596: vmov.f32 s16,0x3fc00000
  000a559a: vmov s0,r0
  000a559e: vcvt.f32.s32 s0,s0
  000a55a2: vdiv.f32 s0,s0,s18
  000a55a6: vcmpe.f32 s0,s16
  000a55aa: vmrs apsr,fpscr
  000a55ae: bpl 0x000a55c6
  000a55b0: ldr r0,[sp,#0x2c]
  000a55b2: ldr r0,[r0,#0x0]
  000a55b4: blx 0x00071770
  000a55b8: adds r0,#0x19
  000a55ba: vmov s0,r0
  000a55be: vcvt.f32.s32 s0,s0
  000a55c2: vdiv.f32 s16,s0,s18
  000a55c6: vmov.f32 s20,0xc1200000
  000a55ca: ldr r0,[0x000a57e4]
  000a55cc: vmov.f32 s22,0x3f800000
  000a55d0: vldr.32 s18,[pc,#0x214]
  000a55d4: vmov.f32 s24,0x41f00000
  000a55d8: add r0,pc
  000a55da: vmov.f32 s28,0x41a00000
  000a55de: vldr.32 s26,[pc,#0x20c]
  000a55e2: vmov.f32 s30,0x40000000
  000a55e6: movs r6,#0x0
  000a55e8: mov r8,r0
  000a55ea: strd r10,r5,[sp,#0x20]
  000a55ee: ldr r0,[r5,#0x0]
  000a55f0: cmp r6,r0
  000a55f2: bcs.w 0x000a5b34
  000a55f6: ldr r0,[r5,#0x4]
  000a55f8: ldr.w r4,[r0,r6,lsl #0x2]
  000a55fc: mov r0,r4
  000a55fe: blx 0x000718d8
  000a5602: cmp r0,#0x84
  000a5604: blt 0x000a561a
  000a5606: mov r0,r4
  000a5608: blx 0x000718d8
  000a560c: mov.w r11,#0x0
  000a5610: cmp r0,#0x9a
  000a5612: it lt
  000a5614: mov.lt.w r11,#0x1
  000a5618: b 0x000a561e
  000a561a: mov.w r11,#0x0
  000a561e: mov r0,r10
  000a5620: blx 0x00071824
  000a5624: mov r5,r0
  000a5626: mov r0,r4
  000a5628: movs r1,#0x3d
  000a562a: blx 0x000718e4
  000a562e: cmp r5,r0
  000a5630: str r6,[sp,#0x34]
  000a5632: bne 0x000a5674
  000a5634: mov r0,r4
  000a5636: blx 0x000718d8
  000a563a: cmp r0,#0xc4
  000a563c: blt 0x000a5654
  000a563e: mov r0,r4
  000a5640: blx 0x000718d8
  000a5644: cmp r0,#0xc4
  000a5646: bgt 0x000a5654
  000a5648: ldr r0,[sp,#0x2c]
  000a564a: ldr r0,[r0,#0x0]
  000a564c: blx 0x00071770
  000a5650: cmp r0,#0x8e
  000a5652: blt 0x000a5674
  000a5654: mov r0,r4
  000a5656: blx 0x000718d8
  000a565a: cmp r0,#0xc6
  000a565c: blt 0x000a5678
  000a565e: mov r0,r4
  000a5660: blx 0x000718d8
  000a5664: cmp r0,#0xc8
  000a5666: bgt 0x000a5678
  000a5668: ldr r0,[sp,#0x2c]
  000a566a: ldr r0,[r0,#0x0]
  000a566c: blx 0x00071770
  000a5670: cmp r0,#0x8e
  000a5672: bge 0x000a5678
  000a5674: movs r0,#0x0
  000a5676: b 0x000a567a
  000a5678: movs r0,#0x1
  000a567a: str r0,[sp,#0x30]
  000a567c: ldr r0,[sp,#0x28]
  000a567e: cmp r0,#0x6a
  000a5680: bne 0x000a56a6
  000a5682: movs r5,#0x0
  000a5684: cmp r5,#0x9
  000a5686: bhi.w 0x000a5a52
  000a568a: ldr.w r6,[r8,r5,lsl #0x2]
  000a568e: mov r0,r4
  000a5690: blx 0x000718d8
  000a5694: subs r0,r6,r0
  000a5696: add.w r5,r5,#0x1
  000a569a: it ne
  000a569c: mov.ne r0,#0x1
  000a569e: mvn.w r1,r11
  000a56a2: tst r1,r0
  000a56a4: bne 0x000a5684
  000a56a6: mov r0,r4
  000a56a8: blx 0x000718f0
  000a56ac: mov r5,r0
  000a56ae: mov r0,r4
  000a56b0: blx 0x000718d8
  000a56b4: cmp r0,#0x7a
  000a56b6: bne 0x000a56e0
  000a56b8: ldr r0,[0x000a57f0]
  000a56ba: add r0,pc
  000a56bc: ldr r0,[r0,#0x0]
  000a56be: ldr r0,[r0,#0x0]
  000a56c0: cbz r0,0x000a56e0
  000a56c2: muls r0,r5
  000a56c4: vmov s2,r5
  000a56c8: vmov s0,r0
  000a56cc: vcvt.f32.s32 s0,s0
  000a56d0: vcvt.f32.s32 s2,s2
  000a56d4: vmla.f32 s2,s0,s18
  000a56d8: vcvt.s32.f32 s0,s2
  000a56dc: vmov r5,s0
  000a56e0: mov r0,r4
  000a56e2: blx 0x000718fc
  000a56e6: cmp r0,#0x1
  000a56e8: bne 0x000a5712
  000a56ea: ldr r0,[0x000a57f4]
  000a56ec: add r0,pc
  000a56ee: ldr r0,[r0,#0x0]
  000a56f0: ldr r0,[r0,#0x0]
  000a56f2: cbz r0,0x000a5712
  000a56f4: muls r0,r5
  000a56f6: vmov s2,r5
  000a56fa: vmov s0,r0
  000a56fe: vcvt.f32.s32 s0,s0
  000a5702: vcvt.f32.s32 s2,s2
  000a5706: vmla.f32 s2,s0,s18
  000a570a: vcvt.s32.f32 s0,s2
  000a570e: vmov r5,s0
  000a5712: mov r0,r4
  000a5714: blx 0x000718d8
  000a5718: mov r6,r0
  000a571a: mov r0,r4
  000a571c: blx 0x00071908
  000a5720: mov r9,r0
  000a5722: ldr r0,[sp,#0x2c]
  000a5724: ldr r0,[r0,#0x0]
  000a5726: blx 0x00071770
  000a572a: mov r10,r0
  000a572c: cbnz r5,0x000a5774
  000a572e: ldr r0,[0x000a57f8]
  000a5730: cmp r6,#0xc4
  000a5732: add r0,pc
  000a5734: ldr r0,[r0,#0x0]
  000a5736: blt 0x000a573e
  000a5738: ldrb.w r1,[r0,#0x37]
  000a573c: cbnz r1,0x000a574a
  000a573e: movs r5,#0x0
  000a5740: cmp r6,#0xc3
  000a5742: bgt 0x000a5774
  000a5744: ldrb.w r0,[r0,#0x35]
  000a5748: cbz r0,0x000a5774
  000a574a: mov r0,r4
  000a574c: blx 0x000718fc
  000a5750: cmp r6,#0x55
  000a5752: mov.w r5,#0x0
  000a5756: it ne
  000a5758: cmp.ne r0,#0x4
  000a575a: beq 0x000a5774
  000a575c: mov r0,r4
  000a575e: blx 0x00071914
  000a5762: cbnz r0,0x000a5772
  000a5764: cmp r6,#0xb5
  000a5766: bne.w 0x000a5a5a
  000a576a: cmp.w r10,#0x3b
  000a576e: bge.w 0x000a5a5a
  000a5772: movs r5,#0x0
  000a5774: mov r0,r4
  000a5776: blx 0x00071920
  000a577a: mov r9,r0
  000a577c: ldr r0,[sp,#0x30]
  000a577e: ldr.w r10,[sp,#0x20]
  000a5782: cbz r0,0x000a57fc
  000a5784: ldr r0,[sp,#0x2c]
  000a5786: ldr r0,[r0,#0x0]
  000a5788: blx 0x0007192c
  000a578c: cbz r0,0x000a57a6
  000a578e: mov r0,r4
  000a5790: blx 0x00071908
  000a5794: cmp r0,#0x17
  000a5796: beq.w 0x000a5a52
  000a579a: mov r0,r4
  000a579c: blx 0x00071908
  000a57a0: cmp r0,#0x18
  000a57a2: beq.w 0x000a5a52
  000a57a6: ldr r0,[sp,#0x28]
  000a57a8: cmp r0,#0x6b
  000a57aa: beq 0x000a588a
  000a57ac: cmp r0,#0x69
  000a57ae: beq 0x000a5896
  000a57b0: cmp r0,#0x65
  000a57b2: bne 0x000a58aa
  000a57b4: mov r0,r4
  000a57b6: blx 0x00071938
  000a57ba: cmp r0,#0x0
  000a57bc: bne 0x000a58aa
  000a57be: b 0x000a5a52
  000a57fc: mov r0,r4
  000a57fe: blx 0x00071914
  000a5802: cmp r0,#0x0
  000a5804: bne.w 0x000a5a52
  000a5808: ldr r0,[sp,#0x34]
  000a580a: bic r0,r0,#0x80000000
  000a580e: sub.w r1,r0,#0xd9
  000a5812: cmp r1,#0x2
  000a5814: bcc.w 0x000a5a52
  000a5818: cmp r0,#0xa4
  000a581a: it ne
  000a581c: cmp.ne r0,#0xaf
  000a581e: beq.w 0x000a5a52
  000a5822: mov r0,r10
  000a5824: blx 0x00071890
  000a5828: cmp r5,#0x0
  000a582a: beq.w 0x000a5a52
  000a582e: cmp r9,r0
  000a5830: bgt.w 0x000a5a52
  000a5834: mov r0,r4
  000a5836: blx 0x00071944
  000a583a: cmp r0,#0x0
  000a583c: beq.w 0x000a5a52
  000a5840: mov r0,r4
  000a5842: movs r1,#0x3c
  000a5844: blx 0x000718e4
  000a5848: cmp r0,#0x1
  000a584a: bne 0x000a5864
  000a584c: mov r0,r10
  000a584e: blx 0x0007189c
  000a5852: ldr r1,[sp,#0x14]
  000a5854: ldr r1,[r1,#0x4]
  000a5856: ldr.w r0,[r1,r0,lsl #0x2]
  000a585a: blx 0x00071950
  000a585e: cmp r0,#0x1
  000a5860: bne.w 0x000a5a52
  000a5864: ldr r0,[sp,#0x28]
  000a5866: cmp r0,#0x6a
  000a5868: beq 0x000a5784
  000a586a: eors r0,r11,#0x1
  000a586e: bne.w 0x000a5784
  000a5872: mov r0,r4
  000a5874: blx 0x000718d8
  000a5878: mov r6,r0
  000a587a: mov r0,r10
  000a587c: blx 0x0007189c
  000a5880: adds r0,#0x84
  000a5882: cmp r6,r0
  000a5884: beq.w 0x000a5784
  000a5888: b 0x000a5a52
  000a588a: mov r0,r4
  000a588c: blx 0x000718fc
  000a5890: cmp r0,#0x3
  000a5892: beq 0x000a58aa
  000a5894: b 0x000a5a52
  000a5896: mov r0,r4
  000a5898: blx 0x00071938
  000a589c: cbnz r0,0x000a58aa
  000a589e: mov r0,r4
  000a58a0: blx 0x00071908
  000a58a4: cmp r0,#0x1c
  000a58a6: bne.w 0x000a5a52
  000a58aa: ldr r0,[sp,#0x28]
  000a58ac: cmp r0,#0x6a
  000a58ae: bne 0x000a58bc
  000a58b0: mov r0,r4
  000a58b2: blx 0x000718fc
  000a58b6: cmp r0,#0x4
  000a58b8: bne.w 0x000a5a52
  000a58bc: vmov s0,r5
  000a58c0: vcvt.f32.s32 s0,s0
  000a58c4: vmul.f32 s0,s16,s0
  000a58c8: vcvt.s32.f32 s0,s0
  000a58cc: ldr r0,[sp,#0x30]
  000a58ce: cmp r0,#0x0
  000a58d0: beq 0x000a5980
  000a58d2: mov r0,r4
  000a58d4: blx 0x0007195c
  000a58d8: ldr.w r9,[sp,#0x14]
  000a58dc: ldr.w r1,[r9,#0x4]
  000a58e0: ldr.w r0,[r1,r0,lsl #0x2]
  000a58e4: blx 0x00071968
  000a58e8: str r0,[sp,#0x30]
  000a58ea: mov r0,r4
  000a58ec: blx 0x0007195c
  000a58f0: ldr.w r1,[r9,#0x4]
  000a58f4: ldr.w r0,[r1,r0,lsl #0x2]
  000a58f8: blx 0x00071974
  000a58fc: mov r11,r0
  000a58fe: ldr r0,[sp,#0x10]
  000a5900: mov r6,r10
  000a5902: ldr.w r10,[r0,#0x0]
  000a5906: mov r0,r6
  000a5908: blx 0x0007189c
  000a590c: ldr.w r1,[r9,#0x4]
  000a5910: ldr.w r0,[r1,r0,lsl #0x2]
  000a5914: blx 0x00071968
  000a5918: mov r5,r0
  000a591a: mov r0,r6
  000a591c: blx 0x0007189c
  000a5920: ldr.w r1,[r9,#0x4]
  000a5924: ldr.w r0,[r1,r0,lsl #0x2]
  000a5928: blx 0x00071974
  000a592c: ldr r3,[sp,#0x30]
  000a592e: mov r2,r0
  000a5930: mov r0,r10
  000a5932: mov r1,r5
  000a5934: str.w r11,[sp,#0x0]
  000a5938: blx 0x00071980
  000a593c: mov r9,r0
  000a593e: ldr r0,[sp,#0x1c]
  000a5940: movs r1,#0xf
  000a5942: ldr r0,[r0,#0x0]
  000a5944: blx 0x00071848
  000a5948: mov r6,r0
  000a594a: adds r5,r0,#0x5
  000a594c: mov r0,r4
  000a594e: blx 0x000718fc
  000a5952: cmp r0,#0x4
  000a5954: beq 0x000a5960
  000a5956: mov r0,r4
  000a5958: blx 0x000718fc
  000a595c: cmp r0,#0x1
  000a595e: bne 0x000a5a30
  000a5960: mov r0,r4
  000a5962: blx 0x000718d8
  000a5966: ldr.w r10,[sp,#0x20]
  000a596a: cmp r0,#0x6d
  000a596c: bne 0x000a59be
  000a596e: add.w r0,r5,r5, lsr #0x1f
  000a5972: cmn r6,#0x3
  000a5976: asr.w r5,r0, asr #0x1
  000a597a: it lt
  000a597c: mov.lt r5,#0x1
  000a597e: b 0x000a5a44
  000a5980: ldr r0,[sp,#0xc]
  000a5982: cmp r9,r0
  000a5984: ble 0x000a598c
  000a5986: eors r0,r11,#0x1
  000a598a: bne 0x000a5a52
  000a598c: ldr r0,[sp,#0x1c]
  000a598e: movs r1,#0x64
  000a5990: vmov r5,s0
  000a5994: ldr r0,[r0,#0x0]
  000a5996: blx 0x00071848
  000a599a: cmp r0,r5
  000a599c: bge 0x000a5a52
  000a599e: mov r0,r4
  000a59a0: blx 0x000718d8
  000a59a4: ldr r1,[sp,#0x8]
  000a59a6: cmp r9,r1
  000a59a8: bge 0x000a58d2
  000a59aa: cmp r0,#0x7a
  000a59ac: beq 0x000a58d2
  000a59ae: ldr r0,[sp,#0x1c]
  000a59b0: movs r1,#0x64
  000a59b2: ldr r0,[r0,#0x0]
  000a59b4: blx 0x00071848
  000a59b8: cmp r0,#0x3c
  000a59ba: ble 0x000a58d2
  000a59bc: b 0x000a5a52
  000a59be: mov r0,r4
  000a59c0: blx 0x000718fc
  000a59c4: cmp r0,#0x4
  000a59c6: bne 0x000a5a44
  000a59c8: cmp.w r9,#0x33
  000a59cc: blt 0x000a5a06
  000a59ce: sub.w r0,r9,#0x32
  000a59d2: vmov s0,r0
  000a59d6: vcvt.f32.s32 s0,s0
  000a59da: ldr r0,[sp,#0x2c]
  000a59dc: ldr r0,[r0,#0x0]
  000a59de: vdiv.f32 s17,s0,s26
  000a59e2: blx 0x0007192c
  000a59e6: vmov.f32 s0,s28
  000a59ea: cmp r0,#0x0
  000a59ec: it ne
  000a59ee: vmov.ne.f32 s0,s30
  000a59f2: vmul.f32 s0,s17,s0
  000a59f6: vcvt.s32.f32 s0,s0
  000a59fa: vmov r0,s0
  000a59fe: cmp r0,#0x1
  000a5a00: it le
  000a5a02: mov.le r0,#0x1
  000a5a04: muls r5,r0
  000a5a06: mov r0,r4
  000a5a08: blx 0x000718d8
  000a5a0c: cmp r0,#0x6e
  000a5a0e: bne 0x000a5a44
  000a5a10: ldr r0,[sp,#0x1c]
  000a5a12: movs r1,#0xa
  000a5a14: ldr r0,[r0,#0x0]
  000a5a16: blx 0x00071848
  000a5a1a: adds r0,#0xa
  000a5a1c: cmp r5,r0
  000a5a1e: ble 0x000a5a44
  000a5a20: ldr r0,[sp,#0x1c]
  000a5a22: movs r1,#0xa
  000a5a24: ldr r0,[r0,#0x0]
  000a5a26: blx 0x00071848
  000a5a2a: add.w r5,r0,#0xa
  000a5a2e: b 0x000a5a44
  000a5a30: mov r0,r5
  000a5a32: movs r1,#0x5
  000a5a34: blx 0x0007198c
  000a5a38: mov r5,r0
  000a5a3a: cmp r6,#0x0
  000a5a3c: it lt
  000a5a3e: mov.lt r5,#0x1
  000a5a40: ldr.w r10,[sp,#0x20]
  000a5a44: mov r0,r4
  000a5a46: mov r1,r5
  000a5a48: blx 0x000718c0
  000a5a4c: ldr r1,[sp,#0x18]
  000a5a4e: blx 0x000718b4
  000a5a52: ldr r6,[sp,#0x34]
  000a5a54: ldr r5,[sp,#0x24]
  000a5a56: adds r6,#0x1
  000a5a58: b 0x000a55ee
  000a5a5a: sub.w r0,r9,#0x22
  000a5a5e: orr r1,r9,#0x2
  000a5a62: clz r0,r0
  000a5a66: subs r1,#0x23
  000a5a68: clz r1,r1
  000a5a6c: movs r5,#0x0
  000a5a6e: lsrs r0,r0,#0x5
  000a5a70: lsrs r1,r1,#0x5
  000a5a72: orrs r0,r1
  000a5a74: cmp r0,#0x1
  000a5a76: bne 0x000a5a80
  000a5a78: cmp.w r10,#0x8e
  000a5a7c: blt.w 0x000a5774
  000a5a80: cmp.w r10,#0x8e
  000a5a84: mov.w r0,#0x0
  000a5a88: it lt
  000a5a8a: mov.lt r0,#0x1
  000a5a8c: cmp.w r9,#0x24
  000a5a90: beq.w 0x000a5774
  000a5a94: sub.w r1,r9,#0x2b
  000a5a98: clz r1,r1
  000a5a9c: lsrs r1,r1,#0x5
  000a5a9e: ands r0,r1
  000a5aa0: bne.w 0x000a5774
  000a5aa4: bic r1,r6,#0x8
  000a5aa8: cmp.w r10,#0x5e
  000a5aac: mov.w r0,#0x0
  000a5ab0: sub.w r1,r1,#0xd1
  000a5ab4: it lt
  000a5ab6: mov.lt r0,#0x1
  000a5ab8: movs r5,#0x0
  000a5aba: cmp r1,#0x2
  000a5abc: bcc.w 0x000a5774
  000a5ac0: sub.w r1,r6,#0xcd
  000a5ac4: clz r1,r1
  000a5ac8: lsrs r1,r1,#0x5
  000a5aca: ands r0,r1
  000a5acc: bne.w 0x000a5774
  000a5ad0: movs r0,#0x0
  000a5ad2: cmp r0,#0x8
  000a5ad4: bhi 0x000a5ae8
  000a5ad6: ldr r1,[0x000a5b54]
  000a5ad8: movs r5,#0x0
  000a5ada: add r1,pc
  000a5adc: ldr.w r1,[r1,r0,lsl #0x2]
  000a5ae0: adds r0,#0x1
  000a5ae2: cmp r1,r6
  000a5ae4: bne 0x000a5ad2
  000a5ae6: b 0x000a5774
  000a5ae8: cmp.w r9,#0x1d
  000a5aec: bne 0x000a5afc
  000a5aee: ldr r0,[sp,#0x2c]
  000a5af0: ldr r0,[r0,#0x0]
  000a5af2: blx 0x00071998
  000a5af6: cmp r0,#0x0
  000a5af8: beq.w 0x000a5772
  000a5afc: ldr r0,[sp,#0x1c]
  000a5afe: movs r1,#0x1e
  000a5b00: ldr r0,[r0,#0x0]
  000a5b02: blx 0x00071848
  000a5b06: mov r5,r0
  000a5b08: mov r0,r4
  000a5b0a: blx 0x00071920
  000a5b0e: vmov s0,r0
  000a5b12: vmov s2,r5
  000a5b16: vcvt.f32.s32 s0,s0
  000a5b1a: vcvt.f32.s32 s2,s2
  000a5b1e: vdiv.f32 s0,s0,s20
  000a5b22: vadd.f32 s0,s0,s22
  000a5b26: vmla.f32 s2,s0,s24
  000a5b2a: vcvt.s32.f32 s0,s2
  000a5b2e: vmov r5,s0
  000a5b32: b 0x000a5774
  000a5b34: ldr r6,[sp,#0x18]
  000a5b36: mov r0,r6
  000a5b38: vpop {d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  000a5b3c: add sp,#0x4
  000a5b3e: pop.w {r8,r9,r10,r11}
  000a5b42: pop {r4,r5,r6,r7,pc}
```
