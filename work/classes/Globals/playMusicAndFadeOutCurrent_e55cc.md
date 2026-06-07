# Globals::playMusicAndFadeOutCurrent
elf 0xe55cc body 554
Sig: undefined __stdcall playMusicAndFadeOutCurrent(int param_1)

## decompile
```c

/* Globals::playMusicAndFadeOutCurrent(int) */

int Globals::playMusicAndFadeOutCurrent(int param_1)

{
  int iVar1;
  Station *pSVar2;
  int iVar3;
  int in_r1;
  Vector *pVVar4;
  int *piVar5;
  float extraout_s0;
  float fVar6;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s0_09;
  float extraout_s0_10;
  float extraout_s0_11;
  float extraout_s0_12;
  float extraout_s0_13;
  float extraout_s0_14;
  float extraout_s0_15;
  float extraout_s0_16;
  
  if (in_r1 == 2) {
    piVar5 = *(int **)(DAT_000f5814 + 0xf5622);
    fVar6 = (float)FModSound::stop(*piVar5);
    iVar1 = *piVar5;
    pVVar4 = (Vector *)0x91;
    goto LAB_000f57e6;
  }
  if (in_r1 == 1) {
    iVar1 = Status::inAlienOrbit();
    if (iVar1 != 0) {
      piVar5 = *(int **)(DAT_000f5808 + 0xf5648);
      FModSound::stop(*piVar5);
      iVar1 = *piVar5;
      iVar3 = Status::getCurrentCampaignMission();
      pVVar4 = (Vector *)0x88;
      fVar6 = extraout_s0_00;
      if ((0x92 < iVar3) &&
         (iVar3 = Status::getCurrentCampaignMission(), fVar6 = extraout_s0_01, iVar3 < 0x9a)) {
        pVVar4 = (Vector *)0x91;
      }
      goto LAB_000f57e6;
    }
    Status::getSystem();
    iVar1 = SolarSystem::getRace();
    piVar5 = *(int **)(DAT_000f580c + 0xf569e);
    FModSound::stop(*piVar5);
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 == 0x6c) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x92;
      fVar6 = extraout_s0_03;
      goto LAB_000f57e6;
    }
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 == 0x65) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x93;
      fVar6 = extraout_s0_07;
      goto LAB_000f57e6;
    }
    iVar3 = Status::inSupernovaSystem();
    if (iVar3 != 0) {
      iVar1 = Status::getCurrentCampaignMission();
      if (iVar1 == 0x59) {
        iVar1 = *piVar5;
        pVVar4 = (Vector *)0x8be;
        fVar6 = extraout_s0_08;
      }
      else {
        iVar1 = Status::getMission();
        fVar6 = extraout_s0_10;
        if (iVar1 != 0) {
          Status::getMission();
          iVar1 = Mission::isEmpty();
          fVar6 = extraout_s0_11;
          if (iVar1 == 0) {
            Status::getMission();
            iVar1 = Mission::getTargetStation();
            pSVar2 = (Station *)Status::getStation();
            iVar3 = Station::getIndex(pSVar2);
            fVar6 = extraout_s0_12;
            if (iVar1 == iVar3) {
              iVar3 = Status::getCurrentCampaignMission();
              iVar1 = *piVar5;
              fVar6 = extraout_s0_13;
              if (iVar3 < 0x6a) {
                pVVar4 = (Vector *)0x8c1;
              }
              else {
                pVVar4 = (Vector *)0x8c2;
              }
              goto LAB_000f57e6;
            }
          }
        }
        iVar1 = *piVar5;
        pVVar4 = (Vector *)0x94;
      }
      goto LAB_000f57e6;
    }
    iVar3 = Status::inDeepScienceOrbit();
    if (iVar3 != 0) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x98;
      fVar6 = extraout_s0_09;
      goto LAB_000f57e6;
    }
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    fVar6 = extraout_s0_14;
    if ((iVar3 == 0x78) &&
       ((iVar3 = Status::getCurrentCampaignMission(), fVar6 = extraout_s0_15, iVar3 == 0x7e ||
        (iVar3 = Status::getCurrentCampaignMission(), fVar6 = extraout_s0_16, iVar3 == 0x85)))) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x8bf;
      goto LAB_000f57e6;
    }
    iVar3 = DAT_000f5810 + 0xf57de;
LAB_000f57dc:
    pVVar4 = *(Vector **)(iVar3 + iVar1 * 4);
    iVar1 = *piVar5;
  }
  else {
    if (in_r1 != 0) {
      return param_1;
    }
    Status::getSystem();
    iVar1 = SolarSystem::getRace();
    piVar5 = *(int **)(DAT_000f57fc + 0xf55fc);
    FModSound::stop(*piVar5);
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 == 0x6c) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x84;
      fVar6 = extraout_s0;
      goto LAB_000f57e6;
    }
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 == 0x65) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)&DAT_00000083;
      fVar6 = extraout_s0_02;
      goto LAB_000f57e6;
    }
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 != 10) {
      pSVar2 = (Station *)Status::getStation();
      iVar3 = Station::getIndex(pSVar2);
      if (iVar3 != 100) {
        iVar3 = DAT_000f5800 + 0xf5744;
        fVar6 = extraout_s0_04;
        goto LAB_000f57dc;
      }
    }
    pSVar2 = (Station *)Status::getStation();
    iVar1 = Station::getIndex(pSVar2);
    fVar6 = extraout_s0_05;
    if ((iVar1 == 10) &&
       (iVar1 = Status::getCurrentCampaignMission(), fVar6 = extraout_s0_06, iVar1 == 0x9f)) {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x90;
    }
    else {
      iVar1 = *piVar5;
      pVVar4 = (Vector *)0x85;
    }
  }
LAB_000f57e6:
  iVar1 = FModSound::play(iVar1,pVVar4,(Vector *)0x0,fVar6);
  return iVar1;
}

```

## target disasm
```
  000f55cc: push {r4,r5,r6,r7,lr}
  000f55ce: add r7,sp,#0xc
  000f55d0: push.w r11
  000f55d4: sub sp,#0x8
  000f55d6: cmp r1,#0x2
  000f55d8: beq 0x000f561c
  000f55da: cmp r1,#0x1
  000f55dc: beq 0x000f5634
  000f55de: cmp r1,#0x0
  000f55e0: bne.w 0x000f57ee
  000f55e4: ldr r0,[0x000f57f8]
  000f55e6: add r0,pc
  000f55e8: ldr r6,[r0,#0x0]
  000f55ea: ldr r0,[r6,#0x0]
  000f55ec: blx 0x00071a10
  000f55f0: blx 0x00071950
  000f55f4: mov r4,r0
  000f55f6: ldr r0,[0x000f57fc]
  000f55f8: add r0,pc
  000f55fa: ldr r5,[r0,#0x0]
  000f55fc: ldr r0,[r5,#0x0]
  000f55fe: ldr r1,[r0,#0x0]
  000f5600: blx 0x000724a8
  000f5604: ldr r0,[r6,#0x0]
  000f5606: blx 0x00071c14
  000f560a: blx 0x00071824
  000f560e: cmp r0,#0x6c
  000f5610: bne 0x000f5674
  000f5612: movs r1,#0x0
  000f5614: ldr r0,[r5,#0x0]
  000f5616: str r1,[sp,#0x0]
  000f5618: movs r1,#0x84
  000f561a: b 0x000f57e6
  000f561c: ldr r0,[0x000f5814]
  000f561e: add r0,pc
  000f5620: ldr r4,[r0,#0x0]
  000f5622: ldr r0,[r4,#0x0]
  000f5624: ldr r1,[r0,#0x0]
  000f5626: blx 0x000724a8
  000f562a: movs r1,#0x0
  000f562c: ldr r0,[r4,#0x0]
  000f562e: str r1,[sp,#0x0]
  000f5630: movs r1,#0x91
  000f5632: b 0x000f57e6
  000f5634: ldr r0,[0x000f5804]
  000f5636: add r0,pc
  000f5638: ldr r6,[r0,#0x0]
  000f563a: ldr r0,[r6,#0x0]
  000f563c: blx 0x000723d0
  000f5640: cbz r0,0x000f568c
  000f5642: ldr r0,[0x000f5808]
  000f5644: add r0,pc
  000f5646: ldr r4,[r0,#0x0]
  000f5648: ldr r0,[r4,#0x0]
  000f564a: ldr r1,[r0,#0x0]
  000f564c: blx 0x000724a8
  000f5650: ldr r0,[r6,#0x0]
  000f5652: ldr r4,[r4,#0x0]
  000f5654: blx 0x00071770
  000f5658: movs r5,#0x88
  000f565a: cmp r0,#0x93
  000f565c: blt 0x000f566a
  000f565e: ldr r0,[r6,#0x0]
  000f5660: blx 0x00071770
  000f5664: cmp r0,#0x9a
  000f5666: it lt
  000f5668: mov.lt r5,#0x91
  000f566a: movs r0,#0x0
  000f566c: mov r1,r5
  000f566e: str r0,[sp,#0x0]
  000f5670: mov r0,r4
  000f5672: b 0x000f57e6
  000f5674: ldr r0,[r6,#0x0]
  000f5676: blx 0x00071c14
  000f567a: blx 0x00071824
  000f567e: cmp r0,#0x65
  000f5680: bne 0x000f56be
  000f5682: movs r1,#0x0
  000f5684: ldr r0,[r5,#0x0]
  000f5686: str r1,[sp,#0x0]
  000f5688: movs r1,#0x83
  000f568a: b 0x000f57e6
  000f568c: ldr r0,[r6,#0x0]
  000f568e: blx 0x00071a10
  000f5692: blx 0x00071950
  000f5696: mov r4,r0
  000f5698: ldr r0,[0x000f580c]
  000f569a: add r0,pc
  000f569c: ldr r5,[r0,#0x0]
  000f569e: ldr r0,[r5,#0x0]
  000f56a0: ldr r1,[r0,#0x0]
  000f56a2: blx 0x000724a8
  000f56a6: ldr r0,[r6,#0x0]
  000f56a8: blx 0x00071c14
  000f56ac: blx 0x00071824
  000f56b0: cmp r0,#0x6c
  000f56b2: bne 0x000f56fc
  000f56b4: movs r1,#0x0
  000f56b6: ldr r0,[r5,#0x0]
  000f56b8: str r1,[sp,#0x0]
  000f56ba: movs r1,#0x92
  000f56bc: b 0x000f57e6
  000f56be: ldr r0,[r6,#0x0]
  000f56c0: blx 0x00071c14
  000f56c4: blx 0x00071824
  000f56c8: cmp r0,#0xa
  000f56ca: beq 0x000f56da
  000f56cc: ldr r0,[r6,#0x0]
  000f56ce: blx 0x00071c14
  000f56d2: blx 0x00071824
  000f56d6: cmp r0,#0x64
  000f56d8: bne 0x000f573e
  000f56da: ldr r0,[r6,#0x0]
  000f56dc: blx 0x00071c14
  000f56e0: blx 0x00071824
  000f56e4: cmp r0,#0xa
  000f56e6: bne 0x000f5714
  000f56e8: ldr r0,[r6,#0x0]
  000f56ea: blx 0x00071770
  000f56ee: cmp r0,#0x9f
  000f56f0: bne 0x000f5714
  000f56f2: movs r1,#0x0
  000f56f4: ldr r0,[r5,#0x0]
  000f56f6: str r1,[sp,#0x0]
  000f56f8: movs r1,#0x90
  000f56fa: b 0x000f57e6
  000f56fc: ldr r0,[r6,#0x0]
  000f56fe: blx 0x00071c14
  000f5702: blx 0x00071824
  000f5706: cmp r0,#0x65
  000f5708: bne 0x000f571e
  000f570a: movs r1,#0x0
  000f570c: ldr r0,[r5,#0x0]
  000f570e: str r1,[sp,#0x0]
  000f5710: movs r1,#0x93
  000f5712: b 0x000f57e6
  000f5714: movs r1,#0x0
  000f5716: ldr r0,[r5,#0x0]
  000f5718: str r1,[sp,#0x0]
  000f571a: movs r1,#0x85
  000f571c: b 0x000f57e6
  000f571e: ldr r0,[r6,#0x0]
  000f5720: blx 0x00071878
  000f5724: mov r1,r0
  000f5726: ldr r0,[r6,#0x0]
  000f5728: cbz r1,0x000f5744
  000f572a: blx 0x00071770
  000f572e: cmp r0,#0x59
  000f5730: bne 0x000f5754
  000f5732: movs r1,#0x0
  000f5734: ldr r0,[r5,#0x0]
  000f5736: str r1,[sp,#0x0]
  000f5738: movw r1,#0x8be
  000f573c: b 0x000f57e6
  000f573e: ldr r1,[0x000f5800]
  000f5740: add r1,pc
  000f5742: b 0x000f57dc
  000f5744: blx 0x00075e08
  000f5748: cbz r0,0x000f57a0
  000f574a: movs r1,#0x0
  000f574c: ldr r0,[r5,#0x0]
  000f574e: str r1,[sp,#0x0]
  000f5750: movs r1,#0x98
  000f5752: b 0x000f57e6
  000f5754: ldr r0,[r6,#0x0]
  000f5756: blx 0x00072850
  000f575a: cbz r0,0x000f57ce
  000f575c: ldr r0,[r6,#0x0]
  000f575e: blx 0x00072850
  000f5762: blx 0x00072868
  000f5766: cbnz r0,0x000f57ce
  000f5768: ldr r0,[r6,#0x0]
  000f576a: blx 0x00072850
  000f576e: blx 0x00073738
  000f5772: mov r4,r0
  000f5774: ldr r0,[r6,#0x0]
  000f5776: blx 0x00071c14
  000f577a: blx 0x00071824
  000f577e: cmp r4,r0
  000f5780: bne 0x000f57ce
  000f5782: ldr r0,[r6,#0x0]
  000f5784: blx 0x00071770
  000f5788: mov r1,r0
  000f578a: cmp r1,#0x6a
  000f578c: mov.w r1,#0x0
  000f5790: ldr r0,[r5,#0x0]
  000f5792: str r1,[sp,#0x0]
  000f5794: ite ge
  000f5796: movw.ge r1,#0x8c2
  000f579a: movw.lt r1,#0x8c1
  000f579e: b 0x000f57e6
  000f57a0: ldr r0,[r6,#0x0]
  000f57a2: blx 0x00071c14
  000f57a6: blx 0x00071824
  000f57aa: cmp r0,#0x78
  000f57ac: bne 0x000f57d8
  000f57ae: ldr r0,[r6,#0x0]
  000f57b0: blx 0x00071770
  000f57b4: cmp r0,#0x7e
  000f57b6: beq 0x000f57c2
  000f57b8: ldr r0,[r6,#0x0]
  000f57ba: blx 0x00071770
  000f57be: cmp r0,#0x85
  000f57c0: bne 0x000f57d8
  000f57c2: movs r1,#0x0
  000f57c4: ldr r0,[r5,#0x0]
  000f57c6: str r1,[sp,#0x0]
  000f57c8: movw r1,#0x8bf
  000f57cc: b 0x000f57e6
  000f57ce: movs r1,#0x0
  000f57d0: ldr r0,[r5,#0x0]
  000f57d2: str r1,[sp,#0x0]
  000f57d4: movs r1,#0x94
  000f57d6: b 0x000f57e6
  000f57d8: ldr r1,[0x000f5810]
  000f57da: add r1,pc
  000f57dc: movs r2,#0x0
  000f57de: ldr.w r1,[r1,r4,lsl #0x2]
  000f57e2: ldr r0,[r5,#0x0]
  000f57e4: str r2,[sp,#0x0]
  000f57e6: movs r2,#0x0
  000f57e8: movs r3,#0x0
  000f57ea: blx 0x00071548
  000f57ee: add sp,#0x8
  000f57f0: pop.w r11
  000f57f4: pop {r4,r5,r6,r7,pc}
```
