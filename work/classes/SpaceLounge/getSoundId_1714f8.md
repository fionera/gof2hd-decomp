# SpaceLounge::getSoundId
elf 0x1714f8 body 562
Sig: undefined __thiscall getSoundId(SpaceLounge * this, Agent * param_1)

## decompile
```c

/* SpaceLounge::getSoundId(Agent*) */

void __thiscall SpaceLounge::getSoundId(SpaceLounge *this,Agent *param_1)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  SpaceLounge *this_00;
  String *pSVar7;
  int iVar8;
  SpaceLounge *pSVar9;
  int *piVar10;
  int *piVar11;
  
  piVar11 = *(int **)(DAT_00181744 + 0x181508);
  iVar3 = *piVar11;
  iVar4 = Agent::getRace(param_1);
  bVar2 = (bool)Agent::isMale(param_1);
  uVar5 = Agent::getOffer(param_1);
  iVar6 = Agent::getMission(param_1);
  if (iVar6 == 0) {
LAB_0018153c:
    iVar6 = -1;
  }
  else {
    Agent::getMission(param_1);
    iVar6 = Mission::isEmpty();
    if (iVar6 != 0) goto LAB_0018153c;
    Agent::getMission(param_1);
    iVar6 = Mission::getType();
  }
  Globals::getAgentMissionText((Agent *)&stack0xffffffcc);
  bVar1 = true;
  switch(uVar5) {
  case 0:
    if (iVar6 == 0 || iVar6 == 0xb) {
      iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181764 + 0x181698));
      pSVar9 = (SpaceLounge *)(iVar6 + 0x301);
    }
    else if (iVar6 == 0xc) {
      iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181760 + 0x1816fe));
      pSVar9 = (SpaceLounge *)(iVar6 + 0x2fa);
    }
    else {
      piVar10 = *(int **)(DAT_00181760 + 0x1816fe);
      iVar6 = AbyssEngine::AERandom::nextInt(*piVar10);
      iVar8 = *piVar10;
      if (iVar6 == 0) {
        iVar6 = AbyssEngine::AERandom::nextInt(iVar8);
        pSVar9 = (SpaceLounge *)(iVar6 + 799);
      }
      else {
        iVar6 = AbyssEngine::AERandom::nextInt(iVar8);
        pSVar9 = (SpaceLounge *)(iVar6 + 0x309);
      }
    }
    break;
  case 1:
    goto switchD_00181566_caseD_1;
  case 2:
  case 3:
  case 8:
  case 9:
  case 10:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181750 + 0x18157c));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x2f7);
    break;
  case 4:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181758 + 0x1816ac));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x2fe);
    break;
  case 5:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018174c + 0x1816c0));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x31b);
    break;
  case 6:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018175c + 0x1816d4));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x323);
    break;
  case 7:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181754 + 0x1816e8));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x305);
    break;
  default:
    pSVar9 = (SpaceLounge *)0xffffffff;
  }
  iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181768 + 0x18158e));
  bVar1 = false;
  if (iVar6 < 0x1e) {
switchD_00181566_caseD_1:
    iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018176c + 0x1815a2));
    pSVar9 = (SpaceLounge *)(iVar6 + 0x30d);
  }
  iVar6 = Agent::hasAcceptedOffer(param_1);
  this_00 = (SpaceLounge *)0x0;
  if (iVar6 != 0) {
    this_00 = (SpaceLounge *)AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181770 + 0x1815bc));
    pSVar9 = this_00 + 0x30d;
  }
  if (bVar1) {
    piVar10 = *(int **)(DAT_00181774 + 0x1815d0);
    pSVar7 = (String *)GameText::getText(*piVar10);
    iVar6 = AbyssEngine::String::Compare((String *)&stack0xffffffcc,pSVar7);
    if (iVar6 != 0) {
      pSVar7 = (String *)GameText::getText(*piVar10);
      iVar6 = AbyssEngine::String::Compare((String *)&stack0xffffffcc,pSVar7);
      if (iVar6 != 0) {
        pSVar7 = (String *)GameText::getText(*piVar10);
        iVar6 = AbyssEngine::String::Compare((String *)&stack0xffffffcc,pSVar7);
        if (iVar6 != 0) {
          pSVar7 = (String *)GameText::getText(*piVar10);
          this_00 = (SpaceLounge *)AbyssEngine::String::Compare((String *)&stack0xffffffcc,pSVar7);
          if (this_00 != (SpaceLounge *)0x0) goto LAB_00181632;
        }
      }
    }
    this_00 = (SpaceLounge *)AbyssEngine::AERandom::nextInt(**(int **)(DAT_00181778 + 0x181626));
    pSVar9 = this_00 + 0x314;
  }
LAB_00181632:
  if (iVar4 == 3) {
    iVar4 = Agent::getImageParts(param_1);
    if (iVar4 == 0) {
      iVar4 = 3;
      this_00 = (SpaceLounge *)0x0;
    }
    else {
      piVar10 = (int *)Agent::getImageParts(param_1);
      this_00 = (SpaceLounge *)*piVar10;
      iVar4 = 0;
      if (this_00 == (SpaceLounge *)0x2) {
        iVar4 = 3;
      }
    }
  }
  getSpecificSoundForRace(this_00,(int)pSVar9,iVar4,bVar2);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  iVar3 = *piVar11 - iVar3;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  001814f8: push {r4,r5,r6,r7,lr}
  001814fa: add r7,sp,#0xc
  001814fc: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00181500: ldr r0,[0x00181744]
  00181502: mov r11,r1
  00181504: add r0,pc
  00181506: ldr.w r10,[r0,#0x0]
  0018150a: ldr.w r0,[r10,#0x0]
  0018150e: str r0,[sp,#0x10]
  00181510: mov r0,r1
  00181512: blx 0x00071bd8
  00181516: mov r9,r0
  00181518: mov r0,r11
  0018151a: blx 0x00074110
  0018151e: mov r8,r0
  00181520: mov r0,r11
  00181522: blx 0x00071ae8
  00181526: mov r4,r0
  00181528: mov r0,r11
  0018152a: blx 0x00071bb4
  0018152e: cbz r0,0x0018153c
  00181530: mov r0,r11
  00181532: blx 0x00071bb4
  00181536: blx 0x00072868
  0018153a: cbz r0,0x00181542
  0018153c: mov.w r6,#0xffffffff
  00181540: b 0x0018154e
  00181542: mov r0,r11
  00181544: blx 0x00071bb4
  00181548: blx 0x00072874
  0018154c: mov r6,r0
  0018154e: ldr r0,[0x00181748]
  00181550: mov r2,r11
  00181552: add r0,pc
  00181554: ldr r0,[r0,#0x0]
  00181556: ldr r1,[r0,#0x0]
  00181558: add r0,sp,#0x4
  0018155a: blx 0x00077278
  0018155e: cmp r4,#0xa
  00181560: bhi.w 0x00181684
  00181564: movs r5,#0x1
  00181566: tbb [pc,r4]
  00181576: ldr r0,[0x00181750]
  00181578: add r0,pc
  0018157a: ldr r0,[r0,#0x0]
  0018157c: ldr r0,[r0,#0x0]
  0018157e: movs r1,#0x2
  00181580: blx 0x00071848
  00181584: addw r4,r0,#0x2f7
  00181588: ldr r0,[0x00181768]
  0018158a: add r0,pc
  0018158c: ldr r0,[r0,#0x0]
  0018158e: ldr r0,[r0,#0x0]
  00181590: movs r1,#0x64
  00181592: blx 0x00071848
  00181596: movs r5,#0x0
  00181598: cmp r0,#0x1d
  0018159a: bgt 0x001815ae
  0018159c: ldr r0,[0x0018176c]
  0018159e: add r0,pc
  001815a0: ldr r0,[r0,#0x0]
  001815a2: ldr r0,[r0,#0x0]
  001815a4: movs r1,#0x2
  001815a6: blx 0x00071848
  001815aa: addw r4,r0,#0x30d
  001815ae: mov r0,r11
  001815b0: blx 0x00074bf0
  001815b4: cbz r0,0x001815c8
  001815b6: ldr r0,[0x00181770]
  001815b8: add r0,pc
  001815ba: ldr r0,[r0,#0x0]
  001815bc: ldr r0,[r0,#0x0]
  001815be: movs r1,#0x2
  001815c0: blx 0x00071848
  001815c4: addw r4,r0,#0x30d
  001815c8: cbz r5,0x00181632
  001815ca: ldr r0,[0x00181774]
  001815cc: add r0,pc
  001815ce: ldr r6,[r0,#0x0]
  001815d0: ldr r0,[r6,#0x0]
  001815d2: mov.w r1,#0x334
  001815d6: blx 0x00072f70
  001815da: mov r1,r0
  001815dc: add r0,sp,#0x4
  001815de: blx 0x0006f664
  001815e2: cbz r0,0x00181620
  001815e4: ldr r0,[r6,#0x0]
  001815e6: mov.w r1,#0x338
  001815ea: blx 0x00072f70
  001815ee: mov r1,r0
  001815f0: add r0,sp,#0x4
  001815f2: blx 0x0006f664
  001815f6: cbz r0,0x00181620
  001815f8: ldr r0,[r6,#0x0]
  001815fa: movw r1,#0x33b
  001815fe: blx 0x00072f70
  00181602: mov r1,r0
  00181604: add r0,sp,#0x4
  00181606: blx 0x0006f664
  0018160a: cbz r0,0x00181620
  0018160c: ldr r0,[r6,#0x0]
  0018160e: movw r1,#0x341
  00181612: blx 0x00072f70
  00181616: mov r1,r0
  00181618: add r0,sp,#0x4
  0018161a: blx 0x0006f664
  0018161e: cbnz r0,0x00181632
  00181620: ldr r0,[0x00181778]
  00181622: add r0,pc
  00181624: ldr r0,[r0,#0x0]
  00181626: ldr r0,[r0,#0x0]
  00181628: movs r1,#0x2
  0018162a: blx 0x00071848
  0018162e: add.w r4,r0,#0x314
  00181632: cmp.w r9,#0x3
  00181636: bne 0x0018165a
  00181638: mov r0,r11
  0018163a: blx 0x00071c74
  0018163e: cbz r0,0x00181656
  00181640: mov r0,r11
  00181642: blx 0x00071c74
  00181646: ldr r0,[r0,#0x0]
  00181648: mov.w r9,#0x0
  0018164c: cmp r0,#0x2
  0018164e: it eq
  00181650: mov.eq.w r9,#0x3
  00181654: b 0x0018165a
  00181656: mov.w r9,#0x3
  0018165a: mov r1,r4
  0018165c: mov r2,r9
  0018165e: mov r3,r8
  00181660: blx 0x00077d58
  00181664: mov r4,r0
  00181666: add r0,sp,#0x4
  00181668: blx 0x0006ee30
  0018166c: ldr r0,[sp,#0x10]
  0018166e: ldr.w r1,[r10,#0x0]
  00181672: subs r0,r1,r0
  00181674: itttt eq
  00181676: mov.eq r0,r4
  00181678: add.eq sp,#0x14
  0018167a: pop.eq.w {r8,r9,r10,r11}
  0018167e: pop.eq {r4,r5,r6,r7,pc}
  00181680: blx 0x0006e824
  00181684: mov.w r4,#0xffffffff
  00181688: b 0x00181588
  0018168a: cmp r6,#0x0
  0018168c: it ne
  0018168e: cmp.ne r6,#0xb
  00181690: bne 0x001816f6
  00181692: ldr r0,[0x00181764]
  00181694: add r0,pc
  00181696: ldr r0,[r0,#0x0]
  00181698: ldr r0,[r0,#0x0]
  0018169a: movs r1,#0x4
  0018169c: blx 0x00071848
  001816a0: addw r4,r0,#0x301
  001816a4: b 0x00181588
  001816a6: ldr r0,[0x00181758]
  001816a8: add r0,pc
  001816aa: ldr r0,[r0,#0x0]
  001816ac: ldr r0,[r0,#0x0]
  001816ae: movs r1,#0x2
  001816b0: blx 0x00071848
  001816b4: addw r4,r0,#0x2fe
  001816b8: b 0x00181588
  001816ba: ldr r0,[0x0018174c]
  001816bc: add r0,pc
  001816be: ldr r0,[r0,#0x0]
  001816c0: ldr r0,[r0,#0x0]
  001816c2: movs r1,#0x4
  001816c4: blx 0x00071848
  001816c8: addw r4,r0,#0x31b
  001816cc: b 0x00181588
  001816ce: ldr r0,[0x0018175c]
  001816d0: add r0,pc
  001816d2: ldr r0,[r0,#0x0]
  001816d4: ldr r0,[r0,#0x0]
  001816d6: movs r1,#0x4
  001816d8: blx 0x00071848
  001816dc: addw r4,r0,#0x323
  001816e0: b 0x00181588
  001816e2: ldr r0,[0x00181754]
  001816e4: add r0,pc
  001816e6: ldr r0,[r0,#0x0]
  001816e8: ldr r0,[r0,#0x0]
  001816ea: movs r1,#0x4
  001816ec: blx 0x00071848
  001816f0: addw r4,r0,#0x305
  001816f4: b 0x00181588
  001816f6: ldr r0,[0x00181760]
  001816f8: cmp r6,#0xc
  001816fa: add r0,pc
  001816fc: bne 0x0018170e
  001816fe: ldr r0,[r0,#0x0]
  00181700: ldr r0,[r0,#0x0]
  00181702: movs r1,#0x4
  00181704: blx 0x00071848
  00181708: addw r4,r0,#0x2fa
  0018170c: b 0x00181588
  0018170e: ldr r4,[r0,#0x0]
  00181710: ldr r0,[r4,#0x0]
  00181712: movs r1,#0x2
  00181714: blx 0x00071848
  00181718: mov r1,r0
  0018171a: ldr r0,[r4,#0x0]
  0018171c: cbz r1,0x0018172a
  0018171e: movs r1,#0x4
  00181720: blx 0x00071848
  00181724: addw r4,r0,#0x309
  00181728: b 0x00181588
  0018172a: movs r1,#0x4
  0018172c: blx 0x00071848
  00181730: addw r4,r0,#0x31f
  00181734: b 0x00181588
```
