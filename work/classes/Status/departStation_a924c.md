# Status::departStation
elf 0xa924c body 972
Sig: undefined __stdcall departStation(Station * param_1)

## decompile
```c

/* Status::departStation(Station*) */

void Status::departStation(Station *param_1)

{
  int iVar1;
  int iVar2;
  Station *pSVar3;
  Generator *this;
  Array *pAVar4;
  undefined4 uVar5;
  SolarSystem *this_00;
  uint *puVar6;
  Item *this_01;
  Station *in_r1;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int *piVar10;
  bool bVar11;
  bool bVar12;
  
  iVar1 = inAlienOrbit();
  if (iVar1 == 0) {
    iVar1 = Station::getIndex(*(Station **)(param_1 + 0x19c));
    bVar11 = iVar1 == 0x6c;
    if (bVar11) {
      iVar1 = *(int *)(param_1 + 0x114);
    }
    if (bVar11 && iVar1 == 3) {
      pSVar3 = *(Station **)(param_1 + 0x14c);
      pAVar4 = (Array *)Station::getItems();
      Station::setItems(pSVar3,pAVar4,true);
      pAVar4 = *(Array **)(param_1 + 0x14c);
      bVar11 = (bool)Station::getShips();
      Station::setShips(pAVar4,bVar11);
    }
  }
  iVar1 = inAlienOrbit();
  if ((iVar1 == 0) && (*(Station **)(param_1 + 0x78) != in_r1)) {
    iVar1 = Station::getIndex(in_r1);
    iVar2 = Station::getIndex(*(Station **)(param_1 + 0x19c));
    bVar11 = iVar1 != iVar2;
  }
  else {
    bVar11 = false;
  }
  if (*(Station **)(param_1 + 0x78) != in_r1) {
    pSVar3 = (Station *)isOnStack((Status *)param_1,in_r1);
    addStationToStack((Status *)param_1,in_r1);
    iVar1 = Station::getIndex(in_r1);
    bVar12 = iVar1 == 0x6c;
    if (bVar12) {
      iVar1 = *(int *)(param_1 + 0x114);
    }
    if (bVar12 && iVar1 == 3) {
      pAVar4 = (Array *)Station::getItems();
      Station::setItems(in_r1,pAVar4,true);
      bVar12 = (bool)Station::getShips();
      Station::setShips((Array *)in_r1,bVar12);
      if ((pSVar3 != (Station *)0x0) && (pSVar3 != in_r1)) {
        pAVar4 = (Array *)Station::getItems();
        Station::setItems(pSVar3,pAVar4,true);
        bVar12 = (bool)Station::getShips();
        Station::setShips((Array *)pSVar3,bVar12);
      }
      if (pSVar3 == (Station *)0x0) {
        this = operator_new(1);
        Generator::Generator(this);
        goto LAB_000b935c;
      }
    }
    else if (pSVar3 == (Station *)0x0) {
      this = operator_new(1);
      Generator::Generator(this);
      pAVar4 = (Array *)Generator::getItemBuyList(this,in_r1);
      Station::setItems(in_r1,pAVar4,false);
      bVar12 = (bool)Generator::getShipBuyList((Station *)this);
      Station::setShips((Array *)in_r1,bVar12);
LAB_000b935c:
      pAVar4 = (Array *)Generator::createAgents(this,in_r1);
      Station::setAgents(in_r1,pAVar4);
    }
    *(undefined4 *)(param_1 + 0x70) = *(undefined4 *)(param_1 + 0x1b8);
    *(undefined4 *)(param_1 + 0x74) = *(undefined4 *)(param_1 + 0x1bc);
  }
  *(undefined4 *)(param_1 + 0x194) = **(undefined4 **)(DAT_000b962c + 0xb937e);
  uVar8 = DAT_000b9628;
  piVar10 = *(int **)(DAT_000b9630 + 0xb938a);
  for (uVar7 = 0; uVar7 < **(uint **)(param_1 + 0x198); uVar7 = uVar7 + 1) {
    if (*(int *)((*(uint **)(param_1 + 0x198))[1] + uVar7 * 4) == 0) goto LAB_000b946e;
    iVar1 = Mission::getType();
    iVar2 = Mission::isCampaignMission
                      (*(Mission **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4));
    if ((iVar1 == 0xad) && (iVar2 != 0)) {
      iVar9 = *(int *)(*piVar10 + 400);
      iVar2 = Mission::getProductionGoodIndex
                        (*(Mission **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4));
      iVar2 = Ship::hasCargo(iVar9,iVar2);
      if (iVar2 == 0) goto LAB_000b93e4;
LAB_000b94c4:
      *(undefined4 *)(param_1 + 0x194) =
           *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4);
      break;
    }
LAB_000b93e4:
    if ((((*(int *)(*piVar10 + 0x1e8) < 0x2d) &&
         (iVar2 = Mission::isCampaignMission
                            (*(Mission **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4)),
         iVar1 == 0xa1)) && (iVar2 != 0)) &&
       ((iVar2 = Station::getIndex(in_r1), iVar2 == *(int *)(param_1 + 0x80) &&
        (iVar2 = inAlienOrbit(), iVar2 == 0)))) goto LAB_000b94c4;
    iVar2 = Mission::isEmpty();
    if (iVar2 == 0) {
      iVar2 = Mission::getTargetStation();
      iVar9 = Station::getIndex(in_r1);
      if (((iVar2 == iVar9) &&
          ((0x17 < iVar1 - 0x96U || ((1 << (iVar1 - 0x96U & 0xff) & uVar8) == 0)))) &&
         (iVar1 != 8 && iVar1 != 0xe)) {
        iVar2 = Mission::isCampaignMission
                          (*(Mission **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4));
        if ((iVar2 != 0) || (iVar1 != 0xb && iVar1 != 0xd)) goto LAB_000b94c4;
        break;
      }
    }
    iVar2 = Mission::isEmpty();
    if ((iVar2 == 0) &&
       (iVar2 = Mission::isCampaignMission
                          (*(Mission **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar7 * 4)),
       iVar2 != 0)) {
      iVar2 = Mission::getTargetStation();
      iVar9 = Station::getIndex(in_r1);
      if ((iVar1 == 0xa0) && (iVar2 != iVar9)) goto LAB_000b94c4;
    }
LAB_000b946e:
  }
  if (*(int *)(param_1 + 0x1e8) < 0x2d) {
    if (*(int *)(param_1 + 0x1e8) < 0x20) goto LAB_000b95d4;
    iVar1 = Station::getIndex(in_r1);
    iVar2 = Mission::getTargetStation();
    if ((iVar1 == iVar2) || (iVar1 = Station::getIndex(in_r1), iVar1 == *(int *)(param_1 + 0x80))) {
      if (*(int *)(param_1 + 0x1e8) < 0x2d) goto LAB_000b95d4;
      goto LAB_000b94dc;
    }
    iVar1 = *(int *)(param_1 + 0x88);
    *(int *)(param_1 + 0x88) = iVar1 + 1;
    if (iVar1 < 10) goto LAB_000b95d4;
    *(undefined4 *)(param_1 + 0x88) = 0;
    piVar10 = *(int **)(DAT_000b9634 + 0xb9568);
    do {
      do {
        iVar1 = AbyssEngine::AERandom::nextInt(*piVar10);
        *(int *)(param_1 + 0x7c) = iVar1;
      } while (*(char *)(*(int *)(*(int *)(param_1 + 0x38) + 4) + iVar1) == '\0');
    } while ((iVar1 == 10 || iVar1 == 0xf) ||
            ((iVar1 = Mission::isCampaignMission(*(Mission **)(param_1 + 0x194)), iVar1 != 0 &&
             (iVar1 = Mission::getTargetStation(), iVar1 == *(int *)(param_1 + 0x80)))));
    this_00 = (SolarSystem *)
              Galaxy::getSystem((Galaxy *)**(undefined4 **)(DAT_000b9638 + 0xb95ac),
                                *(int *)(param_1 + 0x7c));
    iVar1 = SolarSystem::getStations(this_00);
    iVar2 = *piVar10;
    SolarSystem::getStations(this_00);
    iVar2 = AbyssEngine::AERandom::nextInt(iVar2);
    uVar5 = *(undefined4 *)(*(int *)(iVar1 + 4) + iVar2 * 4);
  }
  else {
LAB_000b94dc:
    uVar5 = 0xfffffff6;
    *(undefined4 *)(param_1 + 0x7c) = 0xfffffff6;
  }
  *(undefined4 *)(param_1 + 0x80) = uVar5;
LAB_000b95d4:
  if (bVar11) {
    moveWanted((Status *)param_1);
  }
  puVar6 = (uint *)Ship::getCargo();
  if (puVar6 != (uint *)0x0) {
    for (uVar8 = 0; uVar8 < *puVar6; uVar8 = uVar8 + 1) {
      this_01 = *(Item **)(puVar6[1] + uVar8 * 4);
      if (this_01 != (Item *)0x0) {
        Item::setStationAmount(this_01,0);
      }
    }
  }
  param_1[0x108] = (Station)0x0;
  (*(code *)(DAT_001abf14 + 0x1abf18))(*(undefined4 *)(param_1 + 400));
  return;
}

```
## target disasm
```
  000b924c: push {r4,r5,r6,r7,lr}
  000b924e: add r7,sp,#0xc
  000b9250: push {r7,r8,r9,r10,r11}
  000b9254: mov r8,r1
  000b9256: mov r10,r0
  000b9258: blx 0x000723d0
  000b925c: cbnz r0,0x000b9274
  000b925e: ldr.w r0,[r10,#0x19c]
  000b9262: blx 0x00071824
  000b9266: cmp r0,#0x6c
  000b9268: itt eq
  000b926a: ldr.eq.w r0,[r10,#0x114]
  000b926e: cmp.eq r0,#0x3
  000b9270: beq.w 0x000b951e
  000b9274: mov r0,r10
  000b9276: blx 0x000723d0
  000b927a: cbnz r0,0x000b929c
  000b927c: ldr.w r0,[r10,#0x78]
  000b9280: cmp r0,r8
  000b9282: beq 0x000b929c
  000b9284: mov r0,r8
  000b9286: blx 0x00071824
  000b928a: mov r5,r0
  000b928c: ldr.w r0,[r10,#0x19c]
  000b9290: blx 0x00071824
  000b9294: subs r4,r5,r0
  000b9296: it ne
  000b9298: mov.ne r4,#0x1
  000b929a: b 0x000b929e
  000b929c: movs r4,#0x0
  000b929e: ldr.w r0,[r10,#0x78]
  000b92a2: cmp r0,r8
  000b92a4: beq 0x000b9374
  000b92a6: mov r0,r10
  000b92a8: mov r1,r8
  000b92aa: blx 0x00073660
  000b92ae: mov r6,r0
  000b92b0: mov r0,r10
  000b92b2: mov r1,r8
  000b92b4: blx 0x000736c0
  000b92b8: mov r0,r8
  000b92ba: blx 0x00071824
  000b92be: cmp r0,#0x6c
  000b92c0: itt eq
  000b92c2: ldr.eq.w r0,[r10,#0x114]
  000b92c6: cmp.eq r0,#0x3
  000b92c8: beq 0x000b9300
  000b92ca: cmp r6,#0x0
  000b92cc: bne 0x000b936c
  000b92ce: movs r0,#0x1
  000b92d0: blx 0x0006eb24
  000b92d4: mov r6,r0
  000b92d6: blx 0x000736cc
  000b92da: mov r0,r6
  000b92dc: mov r1,r8
  000b92de: blx 0x000736d8
  000b92e2: mov r1,r0
  000b92e4: mov r0,r8
  000b92e6: movs r2,#0x0
  000b92e8: blx 0x000736e4
  000b92ec: mov r0,r6
  000b92ee: mov r1,r8
  000b92f0: blx 0x000736f0
  000b92f4: mov r1,r0
  000b92f6: mov r0,r8
  000b92f8: movs r2,#0x0
  000b92fa: blx 0x000736fc
  000b92fe: b 0x000b935c
  000b9300: ldr.w r0,[r10,#0x14c]
  000b9304: blx 0x00071830
  000b9308: mov r1,r0
  000b930a: mov r0,r8
  000b930c: movs r2,#0x1
  000b930e: blx 0x000736e4
  000b9312: ldr.w r0,[r10,#0x14c]
  000b9316: blx 0x00073708
  000b931a: mov r1,r0
  000b931c: mov r0,r8
  000b931e: movs r2,#0x1
  000b9320: blx 0x000736fc
  000b9324: cbz r6,0x000b934e
  000b9326: cmp r6,r8
  000b9328: beq 0x000b934e
  000b932a: ldr.w r0,[r10,#0x14c]
  000b932e: blx 0x00071830
  000b9332: mov r1,r0
  000b9334: mov r0,r6
  000b9336: movs r2,#0x1
  000b9338: blx 0x000736e4
  000b933c: ldr.w r0,[r10,#0x14c]
  000b9340: blx 0x00073708
  000b9344: mov r1,r0
  000b9346: mov r0,r6
  000b9348: movs r2,#0x1
  000b934a: blx 0x000736fc
  000b934e: cbnz r6,0x000b936c
  000b9350: movs r0,#0x1
  000b9352: blx 0x0006eb24
  000b9356: mov r6,r0
  000b9358: blx 0x000736cc
  000b935c: mov r0,r6
  000b935e: mov r1,r8
  000b9360: blx 0x00073714
  000b9364: mov r1,r0
  000b9366: mov r0,r8
  000b9368: blx 0x00073720
  000b936c: ldrd r0,r1,[r10,#0x1b8]
  000b9370: strd r0,r1,[r10,#0x70]
  000b9374: str r4,[sp,#0x0]
  000b9376: movs r4,#0x0
  000b9378: ldr r0,[0x000b962c]
  000b937a: add r0,pc
  000b937c: ldr r0,[r0,#0x0]
  000b937e: ldr r0,[r0,#0x0]
  000b9380: str.w r0,[r10,#0x194]
  000b9384: ldr r0,[0x000b9630]
  000b9386: add r0,pc
  000b9388: ldr.w r11,[r0,#0x0]
  000b938c: ldr r0,[0x000b9628]
  000b938e: mov r9,r0
  000b9390: ldr.w r0,[r10,#0x198]
  000b9394: ldr r1,[r0,#0x0]
  000b9396: cmp r4,r1
  000b9398: bcs.w 0x000b94d2
  000b939c: ldr r0,[r0,#0x4]
  000b939e: ldr.w r0,[r0,r4,lsl #0x2]
  000b93a2: cmp r0,#0x0
  000b93a4: beq 0x000b946e
  000b93a6: blx 0x00072874
  000b93aa: mov r6,r0
  000b93ac: ldr.w r0,[r10,#0x198]
  000b93b0: ldr r0,[r0,#0x4]
  000b93b2: ldr.w r0,[r0,r4,lsl #0x2]
  000b93b6: blx 0x0007372c
  000b93ba: cmp r6,#0xad
  000b93bc: bne 0x000b93e4
  000b93be: cbz r0,0x000b93e4
  000b93c0: ldr.w r0,[r10,#0x198]
  000b93c4: ldr.w r1,[r11,#0x0]
  000b93c8: ldr r0,[r0,#0x4]
  000b93ca: ldr.w r5,[r1,#0x190]
  000b93ce: ldr.w r0,[r0,r4,lsl #0x2]
  000b93d2: blx 0x000728a4
  000b93d6: mov r1,r0
  000b93d8: mov r0,r5
  000b93da: movs r2,#0x1
  000b93dc: blx 0x00071a70
  000b93e0: cmp r0,#0x0
  000b93e2: bne 0x000b94c4
  000b93e4: ldr.w r0,[r11,#0x0]
  000b93e8: ldr.w r0,[r0,#0x1e8]
  000b93ec: cmp r0,#0x2c
  000b93ee: ble 0x000b9472
  000b93f0: ldr.w r0,[r10,#0x198]
  000b93f4: ldr r0,[r0,#0x4]
  000b93f6: ldr.w r0,[r0,r4,lsl #0x2]
  000b93fa: blx 0x00072868
  000b93fe: cbnz r0,0x000b9430
  000b9400: ldr.w r0,[r10,#0x198]
  000b9404: ldr r0,[r0,#0x4]
  000b9406: ldr.w r0,[r0,r4,lsl #0x2]
  000b940a: blx 0x00073738
  000b940e: mov r5,r0
  000b9410: mov r0,r8
  000b9412: blx 0x00071824
  000b9416: cmp r5,r0
  000b9418: bne 0x000b9430
  000b941a: sub.w r0,r6,#0x96
  000b941e: cmp r0,#0x17
  000b9420: bhi 0x000b94a4
  000b9422: movs r1,#0x1
  000b9424: lsl.w r0,r1,r0
  000b9428: tst.w r0,r9
  000b942c: mov r1,r9
  000b942e: beq 0x000b94a4
  000b9430: ldr.w r0,[r10,#0x198]
  000b9434: ldr r0,[r0,#0x4]
  000b9436: ldr.w r0,[r0,r4,lsl #0x2]
  000b943a: blx 0x00072868
  000b943e: cbnz r0,0x000b946e
  000b9440: ldr.w r0,[r10,#0x198]
  000b9444: ldr r0,[r0,#0x4]
  000b9446: ldr.w r0,[r0,r4,lsl #0x2]
  000b944a: blx 0x0007372c
  000b944e: cbz r0,0x000b946e
  000b9450: ldr.w r0,[r10,#0x198]
  000b9454: ldr r0,[r0,#0x4]
  000b9456: ldr.w r0,[r0,r4,lsl #0x2]
  000b945a: blx 0x00073738
  000b945e: mov r5,r0
  000b9460: mov r0,r8
  000b9462: blx 0x00071824
  000b9466: cmp r6,#0xa0
  000b9468: bne 0x000b946e
  000b946a: cmp r5,r0
  000b946c: bne 0x000b94c4
  000b946e: adds r4,#0x1
  000b9470: b 0x000b9390
  000b9472: ldr.w r0,[r10,#0x198]
  000b9476: ldr r0,[r0,#0x4]
  000b9478: ldr.w r0,[r0,r4,lsl #0x2]
  000b947c: blx 0x0007372c
  000b9480: cmp r6,#0xa1
  000b9482: bne 0x000b93f0
  000b9484: cmp r0,#0x0
  000b9486: beq 0x000b93f0
  000b9488: mov r0,r8
  000b948a: blx 0x00071824
  000b948e: ldr.w r1,[r10,#0x80]
  000b9492: cmp r0,r1
  000b9494: bne 0x000b93f0
  000b9496: ldr.w r0,[r11,#0x0]
  000b949a: blx 0x000723d0
  000b949e: cmp r0,#0x0
  000b94a0: bne 0x000b93f0
  000b94a2: b 0x000b94c4
  000b94a4: cmp r6,#0x8
  000b94a6: it ne
  000b94a8: cmp.ne r6,#0xe
  000b94aa: beq 0x000b9430
  000b94ac: ldr.w r0,[r10,#0x198]
  000b94b0: ldr r0,[r0,#0x4]
  000b94b2: ldr.w r0,[r0,r4,lsl #0x2]
  000b94b6: blx 0x0007372c
  000b94ba: cbnz r0,0x000b94c4
  000b94bc: cmp r6,#0xb
  000b94be: it ne
  000b94c0: cmp.ne r6,#0xd
  000b94c2: beq 0x000b94d2
  000b94c4: ldr.w r0,[r10,#0x198]
  000b94c8: ldr r0,[r0,#0x4]
  000b94ca: ldr.w r0,[r0,r4,lsl #0x2]
  000b94ce: str.w r0,[r10,#0x194]
  000b94d2: ldr.w r0,[r10,#0x1e8]
  000b94d6: ldr r6,[sp,#0x0]
  000b94d8: cmp r0,#0x2c
  000b94da: ble 0x000b94ea
  000b94dc: mvn r0,#0x9
  000b94e0: str.w r0,[r10,#0x7c]
  000b94e4: str.w r0,[r10,#0x80]
  000b94e8: b 0x000b95d4
  000b94ea: cmp r0,#0x20
  000b94ec: blt 0x000b95d4
  000b94ee: mov r0,r8
  000b94f0: blx 0x00071824
  000b94f4: mov r5,r0
  000b94f6: ldr.w r0,[r10,#0x198]
  000b94fa: ldr r0,[r0,#0x4]
  000b94fc: ldr r0,[r0,#0x0]
  000b94fe: blx 0x00073738
  000b9502: cmp r5,r0
  000b9504: beq 0x000b9514
  000b9506: mov r0,r8
  000b9508: blx 0x00071824
  000b950c: ldr.w r1,[r10,#0x80]
  000b9510: cmp r0,r1
  000b9512: bne 0x000b954c
  000b9514: ldr.w r0,[r10,#0x1e8]
  000b9518: cmp r0,#0x2d
  000b951a: bge 0x000b94dc
  000b951c: b 0x000b95d4
  000b951e: ldr.w r0,[r10,#0x19c]
  000b9522: ldr.w r5,[r10,#0x14c]
  000b9526: blx 0x00071830
  000b952a: mov r1,r0
  000b952c: mov r0,r5
  000b952e: movs r2,#0x1
  000b9530: blx 0x000736e4
  000b9534: ldr.w r0,[r10,#0x19c]
  000b9538: ldr.w r5,[r10,#0x14c]
  000b953c: blx 0x00073708
  000b9540: mov r1,r0
  000b9542: mov r0,r5
  000b9544: movs r2,#0x1
  000b9546: blx 0x000736fc
  000b954a: b 0x000b9274
  000b954c: ldr.w r0,[r10,#0x88]
  000b9550: cmp r0,#0xa
  000b9552: add.w r1,r0,#0x1
  000b9556: str.w r1,[r10,#0x88]
  000b955a: blt 0x000b95d4
  000b955c: movs r0,#0x0
  000b955e: str.w r0,[r10,#0x88]
  000b9562: ldr r0,[0x000b9634]
  000b9564: add r0,pc
  000b9566: ldr r4,[r0,#0x0]
  000b9568: ldr r0,[r4,#0x0]
  000b956a: movs r1,#0x16
  000b956c: blx 0x00071848
  000b9570: ldr.w r1,[r10,#0x38]
  000b9574: str.w r0,[r10,#0x7c]
  000b9578: ldr r1,[r1,#0x4]
  000b957a: ldrb r1,[r1,r0]
  000b957c: cmp r1,#0x0
  000b957e: beq 0x000b9568
  000b9580: cmp r0,#0xa
  000b9582: it ne
  000b9584: cmp.ne r0,#0xf
  000b9586: beq 0x000b9568
  000b9588: ldr.w r0,[r10,#0x194]
  000b958c: blx 0x0007372c
  000b9590: cbz r0,0x000b95a2
  000b9592: ldr.w r0,[r10,#0x194]
  000b9596: blx 0x00073738
  000b959a: ldr.w r1,[r10,#0x80]
  000b959e: cmp r0,r1
  000b95a0: beq 0x000b9568
  000b95a2: ldr r0,[0x000b9638]
  000b95a4: ldr.w r1,[r10,#0x7c]
  000b95a8: add r0,pc
  000b95aa: ldr r0,[r0,#0x0]
  000b95ac: ldr r0,[r0,#0x0]
  000b95ae: blx 0x00073684
  000b95b2: mov r5,r0
  000b95b4: blx 0x00071a88
  000b95b8: mov r8,r0
  000b95ba: mov r0,r5
  000b95bc: ldr r4,[r4,#0x0]
  000b95be: blx 0x00071a88
  000b95c2: ldr r1,[r0,#0x0]
  000b95c4: mov r0,r4
  000b95c6: blx 0x00071848
  000b95ca: ldr.w r1,[r8,#0x4]
  000b95ce: ldr.w r0,[r1,r0,lsl #0x2]
  000b95d2: b 0x000b94e4
  000b95d4: cbz r6,0x000b95dc
  000b95d6: mov r0,r10
  000b95d8: blx 0x00073744
  000b95dc: ldr.w r0,[r10,#0x190]
  000b95e0: blx 0x00073750
  000b95e4: mov r5,r0
  000b95e6: cbz r0,0x000b9602
  000b95e8: movs r4,#0x0
  000b95ea: b 0x000b95fc
  000b95ec: ldr r0,[r5,#0x4]
  000b95ee: ldr.w r0,[r0,r4,lsl #0x2]
  000b95f2: cbz r0,0x000b95fa
  000b95f4: movs r1,#0x0
  000b95f6: blx 0x0007375c
  000b95fa: adds r4,#0x1
  000b95fc: ldr r0,[r5,#0x0]
  000b95fe: cmp r4,r0
  000b9600: bcc 0x000b95ec
  000b9602: movs r1,#0x0
  000b9604: ldr.w r0,[r10,#0x190]
  000b9608: strb.w r1,[r10,#0x108]
  000b960c: pop.w {r3,r8,r9,r10,r11}
  000b9610: pop.w {r4,r5,r6,r7,lr}
  000b9614: b.w 0x001abf08
```
