# Generator::createAgents
elf 0x96884 body 1356
Sig: undefined __thiscall createAgents(Generator * this, Station * param_1)

## decompile
```c

/* Generator::createAgents(Station*) */

void __thiscall Generator::createAgents(Generator *this,Station *param_1)

{
  bool bVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  Array<Agent*> *this_00;
  int *piVar5;
  Array<int> *this_01;
  void *pvVar6;
  Array *pAVar7;
  Agent *pAVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  int iVar13;
  undefined4 uVar14;
  Mission *pMVar15;
  int iVar16;
  int *piVar17;
  uint uVar18;
  uint uVar19;
  Generator *pGVar20;
  int iVar21;
  Generator *pGVar22;
  int *piVar23;
  int *piVar24;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar23 = *(int **)(DAT_000a6c18 + 0xa6898);
  piVar24 = *(int **)(DAT_000a6c1c + 0xa689a);
  local_28 = *piVar23;
  iVar2 = Status::inSupernovaSystem();
  if (iVar2 == 0) {
    puVar3 = (uint *)Status::getAgents((Status *)*piVar24);
    iVar2 = Status::getCurrentCampaignMission();
    bVar1 = 0x10 < iVar2;
    iVar4 = Status::dlc1Won();
    if (iVar4 == 0) {
      iVar4 = Station::getIndex(param_1);
      bVar1 = 0x10 < iVar2 && iVar4 != 0x6a;
    }
    uVar18 = 0;
    for (uVar19 = 0; uVar19 < *puVar3; uVar19 = uVar19 + 1) {
      iVar2 = Agent::getStation(*(Agent **)(puVar3[1] + uVar19 * 4));
      iVar4 = Station::getIndex(param_1);
      uVar18 = uVar18 + (byte)(iVar2 == iVar4 & bVar1);
    }
    iVar2 = Station::getIndex(param_1);
    if (iVar2 != 0x6c) {
      piVar17 = *(int **)(DAT_000a6c20 + 0xa693a);
      iVar2 = AbyssEngine::AERandom::nextInt(*piVar17);
      if ((int)(iVar2 + uVar18 + 3) < 5) {
        iVar2 = AbyssEngine::AERandom::nextInt(*piVar17);
        uVar18 = uVar18 + 3 + iVar2;
      }
      else {
        uVar18 = 5;
      }
    }
    this_00 = operator_new(0xc);
    Array<Agent*>::Array(this_00);
    ArraySetLength<Agent*>(uVar18,(Array *)this_00);
    pGVar20 = (Generator *)0x0;
    piVar17 = *(int **)(DAT_000a6c24 + 0xa697e);
    piVar5 = *(int **)(DAT_000a6c28 + 0xa6986);
    iVar2 = DAT_000a6c2c + 0xa6992;
    for (uVar18 = 0; uVar18 < *puVar3; uVar18 = uVar18 + 1) {
      iVar4 = Agent::getStation(*(Agent **)(puVar3[1] + uVar18 * 4));
      iVar13 = Station::getIndex(param_1);
      if (iVar4 == iVar13 && bVar1 == true) {
        *(undefined4 *)(*(int *)(this_00 + 4) + (int)pGVar20 * 4) =
             *(undefined4 *)(puVar3[1] + uVar18 * 4);
        iVar4 = Agent::getOffer(*(Agent **)(puVar3[1] + uVar18 * 4));
        pGVar20 = pGVar20 + 1;
        if (iVar4 == 9) {
          iVar4 = Status::getCurrentCampaignMission();
          if (iVar4 < 0x8e) {
            iVar4 = AbyssEngine::AERandom::nextInt(*piVar17);
            iVar4 = iVar4 + 2;
          }
          else {
            iVar4 = AbyssEngine::AERandom::nextInt(*piVar17);
          }
          iVar21 = *(int *)(iVar2 + iVar4 * 4);
          pAVar8 = *(Agent **)(puVar3[1] + uVar18 * 4);
          iVar13 = Item::getSinglePrice(*(Item **)(*(int *)(*piVar5 + 4) + iVar21 * 4));
          iVar16 = 1;
          if (iVar4 - 3U < 2) {
            iVar16 = 10;
          }
          Agent::setSellItemData(pAVar8,iVar21,iVar16,iVar13 * iVar16);
          Agent::setOfferAccepted(*(Agent **)(puVar3[1] + uVar18 * 4),false);
        }
        else {
          iVar4 = Agent::getOffer(*(Agent **)(puVar3[1] + uVar18 * 4));
          if (iVar4 == 10) {
            this_01 = operator_new(0xc);
            Array<int>::Array();
            for (iVar4 = 0; iVar4 != 6; iVar4 = iVar4 + 1) {
              iVar16 = *(int *)(DAT_000a6c30 + 0xa6a20 + iVar4 * 4);
              iVar13 = Station::hasShip(*(int *)(*piVar24 + 0x14c));
              if (iVar13 == 0) {
                Status::getShip();
                iVar13 = Ship::getIndex();
                if (iVar13 != iVar16) {
                  ArrayAdd<int>(iVar16,(Array *)this_01);
                }
              }
            }
            iVar4 = *(int *)this_01;
            if (iVar4 == 0) {
              uVar19 = puVar3[1];
            }
            else {
              iVar13 = AbyssEngine::AERandom::nextInt(*piVar17);
              iVar16 = *(int *)(*(int *)(this_01 + 4) + iVar13 * 4);
              pAVar8 = *(Agent **)(puVar3[1] + uVar18 * 4);
              iVar13 = Ship::getPrice(*(Ship **)(*(int *)(**(int **)(DAT_000a6c34 + 0xa6a66) + 4) +
                                                iVar16 * 4));
              Agent::setSellItemData(pAVar8,iVar16,1,iVar13);
              uVar19 = puVar3[1];
            }
            Agent::setOfferAccepted(*(Agent **)(uVar19 + uVar18 * 4),iVar4 == 0);
            pvVar6 = (void *)Array<int>::~Array(this_01);
            operator_delete(pvVar6);
          }
        }
      }
    }
    pAVar7 = (Array *)Galaxy::getSystems((Galaxy *)**(undefined4 **)(DAT_000a6e24 + 0xa6b0e));
    iVar2 = Status::getCurrentCampaignMission();
    if ((iVar2 == 0x17) && (iVar2 = Station::getIndex(param_1), iVar2 == 10)) {
      pAVar8 = operator_new(0x98);
      Globals::getRandomName((int)aSStack_34,SUB41(**(undefined4 **)(DAT_000a6e34 + 0xa6b3a),0));
      uVar9 = Station::getIndex(param_1);
      Status::getSystem();
      uVar10 = SolarSystem::getIndex();
      Agent::Agent(pAVar8,0xffffffff,aSStack_34,uVar9,uVar10,0,1,0xffffffff,0xffffffff,0xffffffff,
                   0xffffffff);
      AbyssEngine::String::~String(aSStack_34);
      Agent::setOffer(pAVar8,2);
      Agent::setSellItemData(pAVar8,0x44,1,DAT_000a6e20);
      piVar24 = (int *)ImageFactory::createChar
                                 (SUB41(**(undefined4 **)(DAT_000a6e38 + 0xa6ba2),0),1);
      Agent::setImageParts(pAVar8,piVar24);
      *(Agent **)(*(int *)(this_00 + 4) + (int)pGVar20 * 4) = pAVar8;
      pGVar20 = pGVar20 + 1;
    }
    bVar1 = false;
    for (pGVar22 = pGVar20; pGVar22 < *(Generator **)this_00; pGVar22 = pGVar22 + 1) {
      uVar9 = createAgent(*(Generator **)this_00,param_1);
      *(undefined4 *)(*(int *)(this_00 + 4) + (int)pGVar22 * 4) = uVar9;
      iVar2 = Agent::getOffer(*(Agent **)(*(int *)(this_00 + 4) + (int)pGVar22 * 4));
      if (iVar2 == 6) {
        if (bVar1) {
          bVar1 = true;
          Agent::setOffer(*(Agent **)(*(int *)(this_00 + 4) + (int)pGVar22 * 4),1);
        }
        else {
          bVar1 = true;
        }
      }
      else {
        iVar2 = Agent::getOffer(*(Agent **)(*(int *)(this_00 + 4) + (int)pGVar22 * 4));
        if (iVar2 == 0) {
          pAVar8 = *(Agent **)(*(Generator **)(this_00 + 4) + (int)pGVar22 * 4);
          pMVar15 = (Mission *)createMission(*(Generator **)(this_00 + 4),pAVar8,pAVar7);
          Agent::setMission(pAVar8,pMVar15);
        }
      }
    }
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar17);
    if (iVar2 < 0x23) {
      iVar2 = Status::getStanding();
      iVar4 = DAT_000a6e28 + 0xa6c68;
      puVar11 = *(undefined4 **)(DAT_000a6e2c + 0xa6c6e);
      puVar12 = *(undefined4 **)(DAT_000a6e30 + 0xa6c78);
      for (uVar18 = 0; uVar18 < 4; uVar18 = uVar18 + 1) {
        uVar9 = *(undefined4 *)(iVar4 + uVar18 * 4);
        iVar13 = Standing::isEnemy(iVar2);
        if (iVar13 != 0) {
          for (uVar19 = 0; uVar19 < *(uint *)this_00; uVar19 = uVar19 + 1) {
            iVar13 = Agent::isGenericAgent(*(Agent **)(*(int *)(this_00 + 4) + uVar19 * 4));
            if ((iVar13 != 0) &&
               (iVar13 = Agent::getOffer(*(Agent **)(*(int *)(this_00 + 4) + uVar19 * 4)),
               iVar13 != 7)) {
              *(undefined4 *)(*(int *)(this_00 + 4) + uVar19 * 4) = 0;
              pAVar8 = operator_new(0x98);
              Globals::getRandomName((int)aSStack_40,SUB41(*puVar11,0));
              uVar10 = Station::getIndex(param_1);
              Status::getSystem();
              uVar14 = SolarSystem::getIndex();
              Agent::Agent(pAVar8,0xffffffff,aSStack_40,uVar10,uVar14,uVar9,1,0xffffffff,0xffffffff,
                           0xffffffff,0xffffffff);
              *(Agent **)(*(int *)(this_00 + 4) + uVar19 * 4) = pAVar8;
              AbyssEngine::String::~String(aSStack_40);
              Agent::setOffer(*(Agent **)(*(int *)(this_00 + 4) + uVar19 * 4),7);
              pAVar8 = *(Agent **)(*(int *)(this_00 + 4) + uVar19 * 4);
              piVar24 = (int *)ImageFactory::createChar(SUB41(*puVar12,0),1);
              Agent::setImageParts(pAVar8,piVar24);
              break;
            }
          }
        }
      }
    }
    iVar2 = Status::getCurrentCampaignMission();
    if (((iVar2 == 0x17) && (iVar2 = Station::getIndex(param_1), iVar2 == 10)) ||
       (iVar2 = AbyssEngine::AERandom::nextInt(*piVar17), iVar2 == 1)) {
      for (; pGVar20 < *(Generator **)this_00; pGVar20 = pGVar20 + 1) {
        pAVar8 = *(Agent **)(*(int *)(this_00 + 4) + (int)pGVar20 * 4);
        iVar2 = Agent::isStoryAgent(pAVar8);
        if (((iVar2 == 0) && (iVar2 = Agent::getOffer(pAVar8), iVar2 == 0)) &&
           (iVar2 = Agent::getMission(pAVar8), iVar2 != 0)) {
          Agent::getMission(pAVar8);
          iVar2 = Mission::getReward();
          if (iVar2 < 50000) {
            Agent::getMission(pAVar8);
            iVar2 = Mission::getReward();
            iVar4 = 50000;
            if (iVar2 * 10 < 50000) {
              iVar4 = iVar2 * 10;
            }
            pMVar15 = (Mission *)Agent::getMission(pAVar8);
            Mission::setReward(pMVar15,iVar4);
            break;
          }
        }
      }
    }
  }
  iVar2 = *piVar23 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000a6884: push {r4,r5,r6,r7,lr}
  000a6886: add r7,sp,#0xc
  000a6888: push {r8,r9,r10,r11}
  000a688c: sub sp,#0x5c
  000a688e: str r1,[sp,#0x3c]
  000a6890: ldr r0,[0x000a6c18]
  000a6892: ldr r1,[0x000a6c1c]
  000a6894: add r0,pc
  000a6896: add r1,pc
  000a6898: ldr.w r10,[r0,#0x0]
  000a689c: ldr.w r11,[r1,#0x0]
  000a68a0: ldr.w r1,[r10,#0x0]
  000a68a4: ldr.w r0,[r11,#0x0]
  000a68a8: str r1,[sp,#0x58]
  000a68aa: blx 0x00071878
  000a68ae: movs r4,#0x0
  000a68b0: cmp r0,#0x0
  000a68b2: bne.w 0x000a6db0
  000a68b6: ldr.w r0,[r11,#0x0]
  000a68ba: blx 0x00071ab8
  000a68be: mov r9,r0
  000a68c0: ldr.w r0,[r11,#0x0]
  000a68c4: blx 0x00071770
  000a68c8: mov r5,r0
  000a68ca: ldr.w r0,[r11,#0x0]
  000a68ce: cmp r5,#0x10
  000a68d0: it gt
  000a68d2: mov.gt r4,#0x1
  000a68d4: blx 0x000719a4
  000a68d8: cbz r0,0x000a68de
  000a68da: mov r8,r4
  000a68dc: b 0x000a68f6
  000a68de: movs r4,#0x0
  000a68e0: cmp r5,#0x10
  000a68e2: it gt
  000a68e4: mov.gt r4,#0x1
  000a68e6: ldr r0,[sp,#0x3c]
  000a68e8: blx 0x00071824
  000a68ec: subs r0,#0x6a
  000a68ee: it ne
  000a68f0: mov.ne r0,#0x1
  000a68f2: and.w r8,r4,r0
  000a68f6: movs r6,#0x0
  000a68f8: movs r5,#0x0
  000a68fa: b 0x000a6920
  000a68fc: ldr.w r0,[r9,#0x4]
  000a6900: ldr.w r0,[r0,r6,lsl #0x2]
  000a6904: blx 0x00071ac4
  000a6908: mov r4,r0
  000a690a: ldr r0,[sp,#0x3c]
  000a690c: blx 0x00071824
  000a6910: subs r0,r4,r0
  000a6912: adds r6,#0x1
  000a6914: clz r0,r0
  000a6918: lsrs r0,r0,#0x5
  000a691a: and.w r0,r0,r8
  000a691e: add r5,r0
  000a6920: ldr.w r0,[r9,#0x0]
  000a6924: cmp r6,r0
  000a6926: bcc 0x000a68fc
  000a6928: ldr r0,[sp,#0x3c]
  000a692a: blx 0x00071824
  000a692e: cmp r0,#0x6c
  000a6930: beq 0x000a6956
  000a6932: ldr r0,[0x000a6c20]
  000a6934: movs r1,#0x2
  000a6936: add r0,pc
  000a6938: ldr r4,[r0,#0x0]
  000a693a: ldr r0,[r4,#0x0]
  000a693c: blx 0x00071848
  000a6940: adds r5,#0x3
  000a6942: add r0,r5
  000a6944: cmp r0,#0x4
  000a6946: bgt 0x000a6954
  000a6948: ldr r0,[r4,#0x0]
  000a694a: movs r1,#0x2
  000a694c: blx 0x00071848
  000a6950: add r5,r0
  000a6952: b 0x000a6956
  000a6954: movs r5,#0x5
  000a6956: movs r0,#0xc
  000a6958: str.w r10,[sp,#0x24]
  000a695c: blx 0x0006eb24
  000a6960: mov r4,r0
  000a6962: blx 0x00071ad0
  000a6966: mov r0,r5
  000a6968: mov r1,r4
  000a696a: blx 0x00071adc
  000a696e: ldr r0,[0x000a6c24]
  000a6970: eor r8,r8,#0x1
  000a6974: mov.w r10,#0x0
  000a6978: movs r6,#0x0
  000a697a: add r0,pc
  000a697c: ldr r0,[r0,#0x0]
  000a697e: str r0,[sp,#0x34]
  000a6980: ldr r0,[0x000a6c28]
  000a6982: add r0,pc
  000a6984: ldr r0,[r0,#0x0]
  000a6986: str r0,[sp,#0x2c]
  000a6988: ldr r0,[0x000a6c2c]
  000a698a: str.w r8,[sp,#0x30]
  000a698e: add r0,pc
  000a6990: str r0,[sp,#0x28]
  000a6992: b 0x000a6afe
  000a6994: ldr.w r0,[r9,#0x4]
  000a6998: ldr.w r0,[r0,r10,lsl #0x2]
  000a699c: blx 0x00071ac4
  000a69a0: mov r5,r0
  000a69a2: ldr r0,[sp,#0x3c]
  000a69a4: blx 0x00071824
  000a69a8: subs r0,r5,r0
  000a69aa: it ne
  000a69ac: mov.ne r0,#0x1
  000a69ae: orrs.w r0,r0,r8
  000a69b2: bne.w 0x000a6afa
  000a69b6: ldr.w r0,[r9,#0x4]
  000a69ba: ldr r1,[r4,#0x4]
  000a69bc: ldr.w r0,[r0,r10,lsl #0x2]
  000a69c0: str.w r0,[r1,r6,lsl #0x2]
  000a69c4: ldr.w r0,[r9,#0x4]
  000a69c8: ldr.w r0,[r0,r10,lsl #0x2]
  000a69cc: blx 0x00071ae8
  000a69d0: adds r6,#0x1
  000a69d2: cmp r0,#0x9
  000a69d4: bne 0x000a69f4
  000a69d6: ldr.w r0,[r11,#0x0]
  000a69da: blx 0x00071770
  000a69de: mov r1,r0
  000a69e0: ldr r0,[sp,#0x34]
  000a69e2: cmp r1,#0x8e
  000a69e4: str r6,[sp,#0x38]
  000a69e6: ldr r0,[r0,#0x0]
  000a69e8: blt 0x000a6a92
  000a69ea: movs r1,#0x9
  000a69ec: blx 0x00071848
  000a69f0: mov r5,r0
  000a69f2: b 0x000a6a9a
  000a69f4: ldr.w r0,[r9,#0x4]
  000a69f8: ldr.w r0,[r0,r10,lsl #0x2]
  000a69fc: blx 0x00071ae8
  000a6a00: cmp r0,#0xa
  000a6a02: bne 0x000a6afa
  000a6a04: movs r0,#0xc
  000a6a06: str r6,[sp,#0x38]
  000a6a08: blx 0x0006eb24
  000a6a0c: mov r8,r0
  000a6a0e: blx 0x000701f8
  000a6a12: movs r6,#0x0
  000a6a14: b 0x000a6a48
  000a6a16: ldr r0,[0x000a6c30]
  000a6a18: ldr.w r1,[r11,#0x0]
  000a6a1c: add r0,pc
  000a6a1e: ldr.w r5,[r0,r6,lsl #0x2]
  000a6a22: ldr.w r0,[r1,#0x14c]
  000a6a26: mov r1,r5
  000a6a28: blx 0x00071af4
  000a6a2c: cbnz r0,0x000a6a46
  000a6a2e: ldr.w r0,[r11,#0x0]
  000a6a32: blx 0x00071a58
  000a6a36: blx 0x000719c8
  000a6a3a: cmp r0,r5
  000a6a3c: beq 0x000a6a46
  000a6a3e: mov r0,r5
  000a6a40: mov r1,r8
  000a6a42: blx 0x0007021c
  000a6a46: adds r6,#0x1
  000a6a48: cmp r6,#0x6
  000a6a4a: bne 0x000a6a16
  000a6a4c: ldr.w r1,[r8,#0x0]
  000a6a50: cmp r1,#0x0
  000a6a52: beq 0x000a6adc
  000a6a54: ldr r0,[sp,#0x34]
  000a6a56: ldr r0,[r0,#0x0]
  000a6a58: blx 0x00071848
  000a6a5c: ldr r1,[0x000a6c34]
  000a6a5e: ldr.w r2,[r8,#0x4]
  000a6a62: add r1,pc
  000a6a64: ldr.w r3,[r9,#0x4]
  000a6a68: ldr r1,[r1,#0x0]
  000a6a6a: ldr.w r5,[r2,r0,lsl #0x2]
  000a6a6e: ldr.w r6,[r3,r10,lsl #0x2]
  000a6a72: ldr r1,[r1,#0x0]
  000a6a74: ldr r0,[r1,#0x4]
  000a6a76: ldr.w r0,[r0,r5,lsl #0x2]
  000a6a7a: blx 0x00071b00
  000a6a7e: mov r3,r0
  000a6a80: mov r0,r6
  000a6a82: mov r1,r5
  000a6a84: movs r2,#0x1
  000a6a86: blx 0x00071b0c
  000a6a8a: ldr.w r0,[r9,#0x4]
  000a6a8e: movs r1,#0x0
  000a6a90: b 0x000a6ae2
  000a6a92: movs r1,#0x7
  000a6a94: blx 0x00071848
  000a6a98: adds r5,r0,#0x2
  000a6a9a: ldr r0,[sp,#0x2c]
  000a6a9c: ldr r2,[sp,#0x28]
  000a6a9e: ldr.w r1,[r9,#0x4]
  000a6aa2: ldr r0,[r0,#0x0]
  000a6aa4: ldr.w r6,[r2,r5,lsl #0x2]
  000a6aa8: ldr.w r8,[r1,r10,lsl #0x2]
  000a6aac: ldr r0,[r0,#0x4]
  000a6aae: ldr.w r0,[r0,r6,lsl #0x2]
  000a6ab2: blx 0x00071944
  000a6ab6: subs r1,r5,#0x3
  000a6ab8: movs r2,#0x1
  000a6aba: cmp r1,#0x2
  000a6abc: it cc
  000a6abe: mov.cc r2,#0xa
  000a6ac0: mul r3,r0,r2
  000a6ac4: mov r0,r8
  000a6ac6: mov r1,r6
  000a6ac8: blx 0x00071b0c
  000a6acc: ldr.w r0,[r9,#0x4]
  000a6ad0: movs r1,#0x0
  000a6ad2: ldr.w r0,[r0,r10,lsl #0x2]
  000a6ad6: blx 0x00071b18
  000a6ada: b 0x000a6af4
  000a6adc: ldr.w r0,[r9,#0x4]
  000a6ae0: movs r1,#0x1
  000a6ae2: ldr.w r0,[r0,r10,lsl #0x2]
  000a6ae6: blx 0x00071b18
  000a6aea: mov r0,r8
  000a6aec: blx 0x00070204
  000a6af0: blx 0x0006eb48
  000a6af4: ldr r6,[sp,#0x38]
  000a6af6: ldr.w r8,[sp,#0x30]
  000a6afa: add.w r10,r10,#0x1
  000a6afe: ldr.w r0,[r9,#0x0]
  000a6b02: cmp r10,r0
  000a6b04: bcc.w 0x000a6994
  000a6b08: ldr r0,[0x000a6e24]
  000a6b0a: add r0,pc
  000a6b0c: ldr r0,[r0,#0x0]
  000a6b0e: ldr r0,[r0,#0x0]
  000a6b10: blx 0x00071884
  000a6b14: mov r9,r0
  000a6b16: ldr.w r0,[r11,#0x0]
  000a6b1a: blx 0x00071770
  000a6b1e: cmp r0,#0x17
  000a6b20: bne 0x000a6bb8
  000a6b22: ldr r0,[sp,#0x3c]
  000a6b24: blx 0x00071824
  000a6b28: cmp r0,#0xa
  000a6b2a: bne 0x000a6bb8
  000a6b2c: movs r0,#0x98
  000a6b2e: blx 0x0006eb24
  000a6b32: mov r8,r0
  000a6b34: ldr r0,[0x000a6e34]
  000a6b36: add r0,pc
  000a6b38: ldr r0,[r0,#0x0]
  000a6b3a: ldr r1,[r0,#0x0]
  000a6b3c: add r0,sp,#0x4c
  000a6b3e: movs r2,#0x0
  000a6b40: movs r3,#0x1
  000a6b42: blx 0x00071b24
  000a6b46: ldr r0,[sp,#0x3c]
  000a6b48: blx 0x00071824
  000a6b4c: mov r5,r0
  000a6b4e: ldr.w r0,[r11,#0x0]
  000a6b52: blx 0x00071a10
  000a6b56: blx 0x00071a7c
  000a6b5a: movs r3,#0x0
  000a6b5c: mov.w r1,#0xffffffff
  000a6b60: movs r2,#0x1
  000a6b62: strd r0,r3,[sp,#0x0]
  000a6b66: strd r2,r1,[sp,#0x8]
  000a6b6a: add r2,sp,#0x4c
  000a6b6c: strd r1,r1,[sp,#0x10]
  000a6b70: mov r0,r8
  000a6b72: str r1,[sp,#0x18]
  000a6b74: mov.w r1,#0xffffffff
  000a6b78: mov r3,r5
  000a6b7a: blx 0x00071b30
  000a6b7e: add r0,sp,#0x4c
  000a6b80: blx 0x0006ee30
  000a6b84: mov r0,r8
  000a6b86: movs r1,#0x2
  000a6b88: blx 0x00071b3c
  000a6b8c: ldr r3,[0x000a6e20]
  000a6b8e: mov r0,r8
  000a6b90: movs r1,#0x44
  000a6b92: movs r2,#0x1
  000a6b94: blx 0x00071b0c
  000a6b98: ldr r0,[0x000a6e38]
  000a6b9a: movs r1,#0x1
  000a6b9c: movs r2,#0x0
  000a6b9e: add r0,pc
  000a6ba0: ldr r0,[r0,#0x0]
  000a6ba2: ldr r0,[r0,#0x0]
  000a6ba4: blx 0x00071b48
  000a6ba8: mov r1,r0
  000a6baa: mov r0,r8
  000a6bac: blx 0x00071b54
  000a6bb0: ldr r0,[r4,#0x4]
  000a6bb2: str.w r8,[r0,r6,lsl #0x2]
  000a6bb6: adds r6,#0x1
  000a6bb8: str r6,[sp,#0x38]
  000a6bba: mov.w r8,#0x0
  000a6bbe: ldr r6,[sp,#0x38]
  000a6bc0: b 0x000a6c3e
  000a6bc2: ldr r1,[sp,#0x3c]
  000a6bc4: blx 0x00071b60
  000a6bc8: ldr r1,[r4,#0x4]
  000a6bca: str.w r0,[r1,r6,lsl #0x2]
  000a6bce: ldr r0,[r4,#0x4]
  000a6bd0: ldr.w r0,[r0,r6,lsl #0x2]
  000a6bd4: blx 0x00071ae8
  000a6bd8: cmp r0,#0x6
  000a6bda: bne 0x000a6bf4
  000a6bdc: lsls.w r0,r8,#0x1f
  000a6be0: beq 0x000a6c38
  000a6be2: ldr r0,[r4,#0x4]
  000a6be4: movs r1,#0x1
  000a6be6: mov.w r8,#0x1
  000a6bea: ldr.w r0,[r0,r6,lsl #0x2]
  000a6bee: blx 0x00071b3c
  000a6bf2: b 0x000a6c3c
  000a6bf4: ldr r0,[r4,#0x4]
  000a6bf6: ldr.w r0,[r0,r6,lsl #0x2]
  000a6bfa: blx 0x00071ae8
  000a6bfe: cbnz r0,0x000a6c3c
  000a6c00: ldr r0,[r4,#0x4]
  000a6c02: mov r2,r9
  000a6c04: ldr.w r5,[r0,r6,lsl #0x2]
  000a6c08: mov r1,r5
  000a6c0a: blx 0x00071b6c
  000a6c0e: mov r1,r0
  000a6c10: mov r0,r5
  000a6c12: blx 0x00071b78
  000a6c16: b 0x000a6c3c
  000a6c38: mov.w r8,#0x1
  000a6c3c: adds r6,#0x1
  000a6c3e: ldr r0,[r4,#0x0]
  000a6c40: cmp r6,r0
  000a6c42: bcc 0x000a6bc2
  000a6c44: ldr r0,[sp,#0x34]
  000a6c46: movs r1,#0x64
  000a6c48: ldr r0,[r0,#0x0]
  000a6c4a: blx 0x00071848
  000a6c4e: ldr r6,[sp,#0x38]
  000a6c50: cmp r0,#0x23
  000a6c52: bge 0x000a6d4e
  000a6c54: ldr.w r0,[r11,#0x0]
  000a6c58: blx 0x00071b84
  000a6c5c: mov r5,r0
  000a6c5e: ldr r0,[0x000a6e28]
  000a6c60: mov.w r10,#0x0
  000a6c64: add r0,pc
  000a6c66: str r0,[sp,#0x30]
  000a6c68: ldr r0,[0x000a6e2c]
  000a6c6a: add r0,pc
  000a6c6c: ldr r0,[r0,#0x0]
  000a6c6e: str r0,[sp,#0x2c]
  000a6c70: ldr r0,[0x000a6e30]
  000a6c72: str r5,[sp,#0x28]
  000a6c74: add r0,pc
  000a6c76: ldr r0,[r0,#0x0]
  000a6c78: str r0,[sp,#0x20]
  000a6c7a: b 0x000a6d48
  000a6c7c: ldr r0,[sp,#0x30]
  000a6c7e: ldr.w r8,[r0,r10,lsl #0x2]
  000a6c82: mov r0,r5
  000a6c84: mov r1,r8
  000a6c86: blx 0x00071b90
  000a6c8a: cmp r0,#0x0
  000a6c8c: beq 0x000a6d44
  000a6c8e: mov.w r9,#0x0
  000a6c92: b 0x000a6c98
  000a6c94: add.w r9,r9,#0x1
  000a6c98: ldr r0,[r4,#0x0]
  000a6c9a: cmp r9,r0
  000a6c9c: bcs 0x000a6d44
  000a6c9e: ldr r0,[r4,#0x4]
  000a6ca0: ldr.w r0,[r0,r9,lsl #0x2]
  000a6ca4: blx 0x00071b9c
  000a6ca8: cmp r0,#0x0
  000a6caa: beq 0x000a6c94
  000a6cac: ldr r0,[r4,#0x4]
  000a6cae: ldr.w r0,[r0,r9,lsl #0x2]
  000a6cb2: blx 0x00071ae8
  000a6cb6: cmp r0,#0x7
  000a6cb8: beq 0x000a6c94
  000a6cba: ldr r0,[r4,#0x4]
  000a6cbc: movs r1,#0x0
  000a6cbe: str.w r1,[r0,r9,lsl #0x2]
  000a6cc2: movs r0,#0x98
  000a6cc4: blx 0x0006eb24
  000a6cc8: mov r6,r0
  000a6cca: ldr r0,[sp,#0x2c]
  000a6ccc: ldr r1,[r0,#0x0]
  000a6cce: add r0,sp,#0x40
  000a6cd0: mov r2,r8
  000a6cd2: movs r3,#0x1
  000a6cd4: blx 0x00071b24
  000a6cd8: ldr r0,[sp,#0x3c]
  000a6cda: blx 0x00071824
  000a6cde: mov r5,r0
  000a6ce0: ldr.w r0,[r11,#0x0]
  000a6ce4: blx 0x00071a10
  000a6ce8: blx 0x00071a7c
  000a6cec: strd r0,r8,[sp,#0x0]
  000a6cf0: movs r0,#0x1
  000a6cf2: str r0,[sp,#0x8]
  000a6cf4: mov.w r0,#0xffffffff
  000a6cf8: add r2,sp,#0x40
  000a6cfa: strd r0,r0,[sp,#0xc]
  000a6cfe: strd r0,r0,[sp,#0x14]
  000a6d02: mov r0,r6
  000a6d04: mov.w r1,#0xffffffff
  000a6d08: mov r3,r5
  000a6d0a: blx 0x00071b30
  000a6d0e: ldr r0,[r4,#0x4]
  000a6d10: str.w r6,[r0,r9,lsl #0x2]
  000a6d14: add r0,sp,#0x40
  000a6d16: blx 0x0006ee30
  000a6d1a: ldr r0,[r4,#0x4]
  000a6d1c: movs r1,#0x7
  000a6d1e: ldr.w r0,[r0,r9,lsl #0x2]
  000a6d22: blx 0x00071b3c
  000a6d26: ldr r0,[sp,#0x20]
  000a6d28: mov r2,r8
  000a6d2a: ldr r1,[r4,#0x4]
  000a6d2c: ldr r0,[r0,#0x0]
  000a6d2e: ldr.w r5,[r1,r9,lsl #0x2]
  000a6d32: movs r1,#0x1
  000a6d34: blx 0x00071b48
  000a6d38: mov r1,r0
  000a6d3a: mov r0,r5
  000a6d3c: blx 0x00071b54
  000a6d40: ldr r6,[sp,#0x38]
  000a6d42: ldr r5,[sp,#0x28]
  000a6d44: add.w r10,r10,#0x1
  000a6d48: cmp.w r10,#0x3
  000a6d4c: bls 0x000a6c7c
  000a6d4e: ldr.w r0,[r11,#0x0]
  000a6d52: blx 0x00071770
  000a6d56: ldr.w r10,[sp,#0x24]
  000a6d5a: cmp r0,#0x17
  000a6d5c: bne 0x000a6d68
  000a6d5e: ldr r0,[sp,#0x3c]
  000a6d60: blx 0x00071824
  000a6d64: cmp r0,#0xa
  000a6d66: beq 0x000a6d76
  000a6d68: ldr r0,[sp,#0x34]
  000a6d6a: movs r1,#0x64
  000a6d6c: ldr r0,[r0,#0x0]
  000a6d6e: blx 0x00071848
  000a6d72: cmp r0,#0x1
  000a6d74: bne 0x000a6db0
  000a6d76: movw r8,#0xc34f
  000a6d7a: b 0x000a6daa
  000a6d7c: ldr r0,[r4,#0x4]
  000a6d7e: ldr.w r5,[r0,r6,lsl #0x2]
  000a6d82: mov r0,r5
  000a6d84: blx 0x00071ba8
  000a6d88: cbnz r0,0x000a6da8
  000a6d8a: mov r0,r5
  000a6d8c: blx 0x00071ae8
  000a6d90: cbnz r0,0x000a6da8
  000a6d92: mov r0,r5
  000a6d94: blx 0x00071bb4
  000a6d98: cbz r0,0x000a6da8
  000a6d9a: mov r0,r5
  000a6d9c: blx 0x00071bb4
  000a6da0: blx 0x00071bc0
  000a6da4: cmp r0,r8
  000a6da6: ble 0x000a6dc8
  000a6da8: adds r6,#0x1
  000a6daa: ldr r0,[r4,#0x0]
  000a6dac: cmp r6,r0
  000a6dae: bcc 0x000a6d7c
  000a6db0: ldr r0,[sp,#0x58]
  000a6db2: ldr.w r1,[r10,#0x0]
  000a6db6: subs r0,r1,r0
  000a6db8: itttt eq
  000a6dba: mov.eq r0,r4
  000a6dbc: add.eq sp,#0x5c
  000a6dbe: pop.eq.w {r8,r9,r10,r11}
  000a6dc2: pop.eq {r4,r5,r6,r7,pc}
  000a6dc4: blx 0x0006e824
  000a6dc8: mov r0,r5
  000a6dca: blx 0x00071bb4
  000a6dce: blx 0x00071bc0
  000a6dd2: add.w r0,r0,r0, lsl #0x2
  000a6dd6: movw r6,#0xc350
  000a6dda: lsls r1,r0,#0x1
  000a6ddc: cmp r1,r6
  000a6dde: it lt
  000a6de0: lsl.lt r6,r0,#0x1
  000a6de2: mov r0,r5
  000a6de4: blx 0x00071bb4
  000a6de8: mov r1,r6
  000a6dea: blx 0x00071bcc
  000a6dee: b 0x000a6db0
```
