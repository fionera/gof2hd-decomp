# Status::moveWanted
elf 0xa9684 body 828
Sig: undefined __thiscall moveWanted(Status * this)

## decompile
```c

/* Status::moveWanted() */

void __thiscall Status::moveWanted(Status *this)

{
  int iVar1;
  int iVar2;
  FileRead *this_00;
  Station *this_01;
  Station *this_02;
  int iVar3;
  int iVar4;
  uint uVar5;
  Array<int> *this_03;
  void *pvVar6;
  int extraout_r1;
  bool bVar7;
  Wanted *pWVar8;
  uint uVar9;
  undefined4 *puVar10;
  int *piVar11;
  Array *this_04;
  SystemPathFinder *local_30;
  uint local_2c;
  
  bVar7 = false;
  uVar9 = 0;
  this_04 = (Array *)0x0;
  piVar11 = *(int **)(DAT_000b99d4 + 0xb969a);
  puVar10 = *(undefined4 **)(DAT_000b99d8 + 0xb96a6);
  local_30 = (SystemPathFinder *)0x0;
LAB_000b9980:
  if (**(uint **)this <= uVar9) {
    if (this_04 != (Array *)0x0) {
      ArrayReleaseClasses<SolarSystem*>(this_04);
      pvVar6 = (void *)Array<SolarSystem*>::~Array((Array<SolarSystem*> *)this_04);
      operator_delete(pvVar6);
    }
    if (local_30 == (SystemPathFinder *)0x0) {
      return;
    }
    SystemPathFinder::~SystemPathFinder(local_30);
    (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
    return;
  }
  iVar1 = Wanted::isActive(*(Wanted **)((*(uint **)this)[1] + uVar9 * 4));
  if (((iVar1 != 0) &&
      (iVar1 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4)),
      iVar1 == 0)) && (iVar1 = inAlienOrbit(), iVar1 == 0)) {
    iVar1 = Wanted::getCurrentLocation(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4));
    iVar2 = Station::getIndex(*(Station **)(*piVar11 + 0x19c));
    if (iVar1 == iVar2) goto LAB_000b997e;
    if ((Station *)*puVar10 != (Station *)0x0) {
      iVar1 = Station::getIndex((Station *)*puVar10);
      iVar2 = Wanted::getCurrentLocation(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4));
      if (iVar1 == iVar2) goto LAB_000b997e;
    }
    if (!bVar7) {
      this_00 = operator_new(1);
      FileRead::FileRead(this_00);
      this_04 = (Array *)FileRead::loadSystemsBinary();
      pvVar6 = (void *)FileRead::~FileRead(this_00);
      operator_delete(pvVar6);
      local_30 = operator_new(1);
      SystemPathFinder::SystemPathFinder(local_30);
    }
    Wanted::getCurrentLocation(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4));
    this_01 = (Station *)FileRead::loadStation(0);
    iVar1 = Station::getSystem(this_01);
    Wanted::getTravelsTo(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4));
    this_02 = (Station *)FileRead::loadStation(0);
    iVar2 = Station::getSystem(this_02);
    iVar3 = Station::getIndex(this_01);
    iVar4 = Station::getIndex(this_02);
    if (iVar3 == iVar4) {
      if (uVar9 < 2) {
        local_2c = 4;
        uVar5 = 2;
      }
      else {
        __aeabi_idivmod(uVar9 - 1,6);
        iVar2 = __aeabi_idiv(extraout_r1,3);
        uVar5 = iVar2 + 2;
        local_2c = extraout_r1 / 2 + 4;
      }
      pWVar8 = *(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4);
      iVar2 = Wanted::getCurrentLocation(pWVar8);
      Wanted::setLastSeen(pWVar8,iVar2);
      if (this_02 != (Station *)0x0) {
        pvVar6 = (void *)Station::~Station(this_02);
        operator_delete(pvVar6);
      }
      this_02 = (Station *)Globals::getRandomStation();
      iVar2 = Station::getSystem(this_02);
      do {
        this_03 = (Array<int> *)SystemPathFinder::getSystemPath(local_30,this_04,iVar1,iVar2);
        Station::getSystem(this_02);
        iVar2 = SolarSystem::getRoutes();
        if ((((this_03 != (Array<int> *)0x0) && (iVar2 != 0)) &&
            ((uVar5 <= *(uint *)this_03 &&
             ((*(uint *)this_03 <= local_2c &&
              (iVar3 = *(int *)(*piVar11 + 0x38), iVar2 = Station::getSystem(this_02),
              *(char *)(*(int *)(iVar3 + 4) + iVar2) != '\0')))))) &&
           ((iVar2 = Station::getSystem(this_02), iVar2 != 0x1b &&
            ((((iVar2 = Station::getSystem(this_02), iVar2 != 0x1c &&
               (iVar2 = Station::getSystem(this_02), iVar2 != 0x19)) &&
              (iVar2 = Station::getSystem(this_02), iVar2 != 0x1a)) &&
             (iVar2 = Station::getSystem(this_02), iVar2 != 6)))))) {
          iVar2 = Station::getSystem(this_02);
          iVar3 = Station::getSystem(this_01);
          if (iVar2 != iVar3) goto code_r0x000b9904;
        }
        if (this_02 != (Station *)0x0) {
          pvVar6 = (void *)Station::~Station(this_02);
          operator_delete(pvVar6);
        }
        this_02 = (Station *)Globals::getRandomStation();
        iVar2 = Station::getSystem(this_02);
      } while( true );
    }
    if (iVar1 == iVar2) {
      pWVar8 = *(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4);
      iVar1 = Wanted::getTravelsTo(pWVar8);
      Wanted::setCurrentLocation(pWVar8,iVar1);
      this_03 = (Array<int> *)0x0;
    }
    else {
      this_03 = (Array<int> *)SystemPathFinder::getSystemPath(local_30,this_04,iVar1,iVar2);
      iVar1 = SolarSystem::getWarpGateIndex
                        (*(SolarSystem **)
                          (*(int *)(this_04 + 4) + *(int *)(*(int *)(this_03 + 4) + 4) * 4));
      Wanted::setCurrentLocation(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4),iVar1);
    }
    goto LAB_000b9954;
  }
  goto LAB_000b997e;
code_r0x000b9904:
  pWVar8 = *(Wanted **)(*(int *)(*(int *)this + 4) + uVar9 * 4);
  iVar1 = Station::getIndex(this_02);
  Wanted::setTravelsTo(pWVar8,iVar1);
LAB_000b9954:
  if (this_02 != (Station *)0x0) {
    pvVar6 = (void *)Station::~Station(this_02);
    operator_delete(pvVar6);
  }
  if (this_01 != (Station *)0x0) {
    pvVar6 = (void *)Station::~Station(this_01);
    operator_delete(pvVar6);
  }
  if (this_03 != (Array<int> *)0x0) {
    pvVar6 = (void *)Array<int>::~Array(this_03);
    operator_delete(pvVar6);
  }
  bVar7 = true;
LAB_000b997e:
  uVar9 = uVar9 + 1;
  goto LAB_000b9980;
}

```
## target disasm
```
  000b9684: push {r4,r5,r6,r7,lr}
  000b9686: add r7,sp,#0xc
  000b9688: push {r8,r9,r10,r11}
  000b968c: sub sp,#0x24
  000b968e: mov r8,r0
  000b9690: ldr r0,[0x000b99d4]
  000b9692: movs r5,#0x0
  000b9694: movs r6,#0x0
  000b9696: add r0,pc
  000b9698: mov.w r11,#0x0
  000b969c: ldr.w r10,[r0,#0x0]
  000b96a0: ldr r0,[0x000b99d8]
  000b96a2: add r0,pc
  000b96a4: ldr.w r9,[r0,#0x0]
  000b96a8: movs r0,#0x0
  000b96aa: str r0,[sp,#0x18]
  000b96ac: str.w r9,[sp,#0x8]
  000b96b0: b 0x000b9980
  000b96b2: ldr r0,[r0,#0x4]
  000b96b4: ldr.w r0,[r0,r6,lsl #0x2]
  000b96b8: blx 0x00073774
  000b96bc: cmp r0,#0x0
  000b96be: beq.w 0x000b997e
  000b96c2: ldr.w r0,[r8,#0x0]
  000b96c6: ldr r0,[r0,#0x4]
  000b96c8: ldr.w r0,[r0,r6,lsl #0x2]
  000b96cc: blx 0x00071a04
  000b96d0: cmp r0,#0x0
  000b96d2: bne.w 0x000b997e
  000b96d6: ldr.w r0,[r10,#0x0]
  000b96da: blx 0x000723d0
  000b96de: cmp r0,#0x0
  000b96e0: bne.w 0x000b997e
  000b96e4: ldr.w r0,[r8,#0x0]
  000b96e8: ldr r0,[r0,#0x4]
  000b96ea: ldr.w r0,[r0,r6,lsl #0x2]
  000b96ee: blx 0x00073780
  000b96f2: mov r4,r0
  000b96f4: ldr.w r0,[r10,#0x0]
  000b96f8: ldr.w r0,[r0,#0x19c]
  000b96fc: blx 0x00071824
  000b9700: cmp r4,r0
  000b9702: beq.w 0x000b997e
  000b9706: ldr.w r0,[r9,#0x0]
  000b970a: cbz r0,0x000b9726
  000b970c: blx 0x00071824
  000b9710: mov r4,r0
  000b9712: ldr.w r0,[r8,#0x0]
  000b9716: ldr r0,[r0,#0x4]
  000b9718: ldr.w r0,[r0,r6,lsl #0x2]
  000b971c: blx 0x00073780
  000b9720: cmp r4,r0
  000b9722: beq.w 0x000b997e
  000b9726: lsls r0,r5,#0x1f
  000b9728: bne 0x000b9754
  000b972a: movs r0,#0x1
  000b972c: blx 0x0006eb24
  000b9730: mov r4,r0
  000b9732: blx 0x00072124
  000b9736: mov r0,r4
  000b9738: blx 0x0007378c
  000b973c: mov r11,r0
  000b973e: mov r0,r4
  000b9740: blx 0x0007213c
  000b9744: blx 0x0006eb48
  000b9748: movs r0,#0x1
  000b974a: blx 0x0006eb24
  000b974e: str r0,[sp,#0x18]
  000b9750: blx 0x00073798
  000b9754: ldr.w r0,[r8,#0x0]
  000b9758: ldr r0,[r0,#0x4]
  000b975a: ldr.w r0,[r0,r6,lsl #0x2]
  000b975e: blx 0x00073780
  000b9762: mov r1,r0
  000b9764: movs r0,#0x0
  000b9766: blx 0x000737a4
  000b976a: mov r5,r0
  000b976c: blx 0x0007189c
  000b9770: str r0,[sp,#0x14]
  000b9772: mov r9,r6
  000b9774: ldr.w r0,[r8,#0x0]
  000b9778: ldr r0,[r0,#0x4]
  000b977a: ldr.w r0,[r0,r6,lsl #0x2]
  000b977e: blx 0x000737b0
  000b9782: mov r1,r0
  000b9784: movs r0,#0x0
  000b9786: blx 0x000737a4
  000b978a: mov r4,r0
  000b978c: blx 0x0007189c
  000b9790: mov r6,r0
  000b9792: mov r0,r5
  000b9794: str r5,[sp,#0xc]
  000b9796: blx 0x00071824
  000b979a: mov r5,r0
  000b979c: mov r0,r4
  000b979e: blx 0x00071824
  000b97a2: cmp r5,r0
  000b97a4: bne 0x000b97b4
  000b97a6: cmp.w r9,#0x2
  000b97aa: bcs 0x000b97da
  000b97ac: movs r0,#0x4
  000b97ae: str r0,[sp,#0x1c]
  000b97b0: movs r0,#0x2
  000b97b2: b 0x000b97fc
  000b97b4: ldr r2,[sp,#0x14]
  000b97b6: cmp r2,r6
  000b97b8: bne.w 0x000b9926
  000b97bc: ldr.w r0,[r8,#0x0]
  000b97c0: mov r6,r9
  000b97c2: ldr r0,[r0,#0x4]
  000b97c4: ldr.w r5,[r0,r9,lsl #0x2]
  000b97c8: mov r0,r5
  000b97ca: blx 0x000737b0
  000b97ce: mov r1,r0
  000b97d0: mov r0,r5
  000b97d2: blx 0x000737bc
  000b97d6: movs r5,#0x0
  000b97d8: b 0x000b9954
  000b97da: sub.w r0,r9,#0x1
  000b97de: movs r1,#0x6
  000b97e0: blx 0x0006f514
  000b97e4: mov r5,r1
  000b97e6: mov r0,r1
  000b97e8: movs r1,#0x3
  000b97ea: blx 0x0007198c
  000b97ee: adds r0,#0x2
  000b97f0: add.w r1,r5,r5, lsr #0x1f
  000b97f4: movs r2,#0x4
  000b97f6: add.w r1,r2,r1, asr #0x1
  000b97fa: str r1,[sp,#0x1c]
  000b97fc: str r0,[sp,#0x20]
  000b97fe: ldr.w r0,[r8,#0x0]
  000b9802: str.w r8,[sp,#0x4]
  000b9806: str.w r9,[sp,#0x0]
  000b980a: ldr r0,[r0,#0x4]
  000b980c: ldr.w r5,[r0,r9,lsl #0x2]
  000b9810: mov r0,r5
  000b9812: blx 0x00073780
  000b9816: mov r1,r0
  000b9818: mov r0,r5
  000b981a: blx 0x000737c8
  000b981e: cbz r4,0x000b982a
  000b9820: mov r0,r4
  000b9822: blx 0x0007360c
  000b9826: blx 0x0006eb48
  000b982a: ldr r0,[0x000b99dc]
  000b982c: add r0,pc
  000b982e: ldr.w r8,[r0,#0x0]
  000b9832: ldr.w r0,[r8,#0x0]
  000b9836: blx 0x000737d4
  000b983a: mov r4,r0
  000b983c: blx 0x0007189c
  000b9840: ldr r6,[sp,#0x18]
  000b9842: mov r3,r0
  000b9844: ldr.w r9,[sp,#0x14]
  000b9848: mov r1,r11
  000b984a: mov r0,r6
  000b984c: b 0x000b986e
  000b984e: cbz r4,0x000b985a
  000b9850: mov r0,r4
  000b9852: blx 0x0007360c
  000b9856: blx 0x0006eb48
  000b985a: ldr.w r0,[r8,#0x0]
  000b985e: blx 0x000737d4
  000b9862: mov r4,r0
  000b9864: blx 0x0007189c
  000b9868: mov r3,r0
  000b986a: mov r0,r6
  000b986c: mov r1,r11
  000b986e: mov r2,r9
  000b9870: blx 0x000737e0
  000b9874: mov r5,r0
  000b9876: mov r0,r4
  000b9878: blx 0x0007189c
  000b987c: ldr.w r1,[r11,#0x4]
  000b9880: ldr.w r0,[r1,r0,lsl #0x2]
  000b9884: blx 0x00071aac
  000b9888: cmp r5,#0x0
  000b988a: beq 0x000b984e
  000b988c: cmp r0,#0x0
  000b988e: beq 0x000b984e
  000b9890: ldr r0,[r5,#0x0]
  000b9892: ldr r1,[sp,#0x20]
  000b9894: cmp r0,r1
  000b9896: bcc 0x000b984e
  000b9898: ldr r1,[sp,#0x1c]
  000b989a: cmp r0,r1
  000b989c: bhi 0x000b984e
  000b989e: ldr.w r0,[r10,#0x0]
  000b98a2: mov r9,r10
  000b98a4: ldr.w r10,[r0,#0x38]
  000b98a8: mov r0,r4
  000b98aa: blx 0x0007189c
  000b98ae: ldr.w r1,[r10,#0x4]
  000b98b2: mov r10,r9
  000b98b4: ldrd r9,r6,[sp,#0x14]
  000b98b8: ldrb r0,[r1,r0]
  000b98ba: cmp r0,#0x0
  000b98bc: beq 0x000b984e
  000b98be: mov r0,r4
  000b98c0: blx 0x0007189c
  000b98c4: cmp r0,#0x1b
  000b98c6: beq 0x000b984e
  000b98c8: mov r0,r4
  000b98ca: blx 0x0007189c
  000b98ce: cmp r0,#0x1c
  000b98d0: beq 0x000b984e
  000b98d2: mov r0,r4
  000b98d4: blx 0x0007189c
  000b98d8: cmp r0,#0x19
  000b98da: beq 0x000b984e
  000b98dc: mov r0,r4
  000b98de: blx 0x0007189c
  000b98e2: cmp r0,#0x1a
  000b98e4: beq 0x000b984e
  000b98e6: mov r0,r4
  000b98e8: blx 0x0007189c
  000b98ec: cmp r0,#0x6
  000b98ee: beq 0x000b984e
  000b98f0: mov r0,r4
  000b98f2: blx 0x0007189c
  000b98f6: str r0,[sp,#0x10]
  000b98f8: ldr r0,[sp,#0xc]
  000b98fa: blx 0x0007189c
  000b98fe: ldr r1,[sp,#0x10]
  000b9900: cmp r1,r0
  000b9902: beq 0x000b984e
  000b9904: ldr.w r9,[sp,#0x4]
  000b9908: ldr r6,[sp,#0x0]
  000b990a: ldr.w r0,[r9,#0x0]
  000b990e: ldr r0,[r0,#0x4]
  000b9910: ldr.w r8,[r0,r6,lsl #0x2]
  000b9914: mov r0,r4
  000b9916: blx 0x00071824
  000b991a: mov r1,r0
  000b991c: mov r0,r8
  000b991e: mov r8,r9
  000b9920: blx 0x000737ec
  000b9924: b 0x000b9954
  000b9926: ldr r0,[sp,#0x18]
  000b9928: mov r1,r11
  000b992a: mov r3,r6
  000b992c: blx 0x000737e0
  000b9930: mov r5,r0
  000b9932: ldr r0,[r0,#0x4]
  000b9934: ldr.w r1,[r11,#0x4]
  000b9938: ldr r0,[r0,#0x4]
  000b993a: ldr.w r0,[r1,r0,lsl #0x2]
  000b993e: blx 0x000737f8
  000b9942: mov r1,r0
  000b9944: ldr.w r0,[r8,#0x0]
  000b9948: mov r6,r9
  000b994a: ldr r0,[r0,#0x4]
  000b994c: ldr.w r0,[r0,r9,lsl #0x2]
  000b9950: blx 0x000737bc
  000b9954: ldr.w r9,[sp,#0x8]
  000b9958: cbz r4,0x000b9964
  000b995a: mov r0,r4
  000b995c: blx 0x0007360c
  000b9960: blx 0x0006eb48
  000b9964: ldr r0,[sp,#0xc]
  000b9966: cbz r0,0x000b9970
  000b9968: blx 0x0007360c
  000b996c: blx 0x0006eb48
  000b9970: cbz r5,0x000b997c
  000b9972: mov r0,r5
  000b9974: blx 0x00070204
  000b9978: blx 0x0006eb48
  000b997c: movs r5,#0x1
  000b997e: adds r6,#0x1
  000b9980: ldr.w r0,[r8,#0x0]
  000b9984: ldr r1,[r0,#0x0]
  000b9986: cmp r6,r1
  000b9988: bcc.w 0x000b96b2
  000b998c: cmp.w r11,#0x0
  000b9990: beq 0x000b99a2
  000b9992: mov r0,r11
  000b9994: blx 0x00073804
  000b9998: mov r0,r11
  000b999a: blx 0x00073810
  000b999e: blx 0x0006eb48
  000b99a2: ldr r0,[sp,#0x18]
  000b99a4: cbz r0,0x000b99b8
  000b99a6: blx 0x0007381c
  000b99aa: add sp,#0x24
  000b99ac: pop.w {r8,r9,r10,r11}
  000b99b0: pop.w {r4,r5,r6,r7,lr}
  000b99b4: b.w 0x001ab098
  000b99b8: add sp,#0x24
  000b99ba: pop.w {r8,r9,r10,r11}
  000b99be: pop {r4,r5,r6,r7,pc}
```
