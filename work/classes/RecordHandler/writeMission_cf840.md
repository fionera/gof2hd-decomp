# RecordHandler::writeMission
elf 0xcf840 body 402
Sig: undefined __thiscall writeMission(RecordHandler * this, Mission * param_1, uint param_2)

## decompile
```c

/* RecordHandler::writeMission(Mission*, unsigned int) */

void __thiscall RecordHandler::writeMission(RecordHandler *this,Mission *param_1,uint param_2)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  Agent *pAVar6;
  code *pcVar7;
  int *piVar8;
  
  piVar8 = *(int **)(DAT_000df9e8 + 0xdf854);
  iVar2 = *piVar8;
  iVar3 = Mission::getType();
  AEFile::Write(iVar3,param_2);
  iVar3 = Mission::isEmpty();
  if (iVar3 == 0) {
    Mission::getClientName();
    AEFile::Write((String *)&stack0xffffffd0,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
    Mission::getTargetName();
    AEFile::Write((String *)&stack0xffffffd0,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
    Mission::getTargetStationName();
    AEFile::Write((String *)&stack0xffffffd0,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
    Mission::getTargetSystemName();
    AEFile::Write((String *)&stack0xffffffd0,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
    bVar1 = (bool)Mission::isCampaignMission(param_1);
    AEFile::Write(bVar1,param_2);
    iVar3 = Mission::getClientImage(param_1);
    if (iVar3 == 0) {
      AEFile::Write(-1,param_2);
    }
    else {
      AEFile::Write(5,param_2);
      for (iVar3 = 0; iVar3 != 5; iVar3 = iVar3 + 1) {
        iVar4 = Mission::getClientImage(param_1);
        AEFile::Write(*(int *)(iVar4 + iVar3 * 4),param_2);
      }
    }
    uVar5 = Mission::getClientRace(param_1);
    pcVar7 = *(code **)(DAT_000df9ec + 0xdf92a);
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getCosts(param_1);
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getBonus();
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getReward();
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getTargetStation();
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getDifficulty(param_1);
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getProductionGoodIndex(param_1);
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getProductionGoodAmount();
    (*pcVar7)(uVar5,param_2);
    uVar5 = Mission::getStatusValue();
    (*pcVar7)(uVar5,param_2);
    bVar1 = (bool)Mission::isVisible(param_1);
    AEFile::Write(bVar1,param_2);
    *(Mission **)this = param_1;
    pAVar6 = (Agent *)Mission::getAgent(param_1);
    if ((pAVar6 == (Agent *)0x0) || (*(Agent **)(this + 4) == pAVar6)) {
      AEFile::Write(-1,param_2);
    }
    else {
      AEFile::Write(1,param_2);
      writeAgent(this,pAVar6,param_2);
    }
  }
  iVar3 = *piVar8;
  iVar2 = iVar3 - iVar2;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2,iVar3);
}

```

## target disasm
```
  000df840: push {r4,r5,r6,r7,lr}
  000df842: add r7,sp,#0xc
  000df844: push {r4,r5,r6,r7,r8,r9,r11}
  000df848: mov r8,r0
  000df84a: ldr r0,[0x000df9e8]
  000df84c: mov r4,r2
  000df84e: mov r6,r1
  000df850: add r0,pc
  000df852: ldr.w r9,[r0,#0x0]
  000df856: ldr.w r0,[r9,#0x0]
  000df85a: str r0,[sp,#0xc]
  000df85c: mov r0,r1
  000df85e: blx 0x00072874
  000df862: mov r1,r4
  000df864: blx 0x00074a1c
  000df868: mov r0,r6
  000df86a: blx 0x00072868
  000df86e: cmp r0,#0x0
  000df870: bne.w 0x000df9be
  000df874: mov r5,sp
  000df876: mov r1,r6
  000df878: mov r0,r5
  000df87a: blx 0x00074b3c
  000df87e: mov r0,r5
  000df880: mov r1,r4
  000df882: movs r2,#0x1
  000df884: blx 0x00074968
  000df888: mov r0,sp
  000df88a: blx 0x0006ee30
  000df88e: mov r5,sp
  000df890: mov r1,r6
  000df892: mov r0,r5
  000df894: blx 0x00074b48
  000df898: mov r0,r5
  000df89a: mov r1,r4
  000df89c: movs r2,#0x1
  000df89e: blx 0x00074968
  000df8a2: mov r0,sp
  000df8a4: blx 0x0006ee30
  000df8a8: mov r5,sp
  000df8aa: mov r1,r6
  000df8ac: mov r0,r5
  000df8ae: blx 0x00074b54
  000df8b2: mov r0,r5
  000df8b4: mov r1,r4
  000df8b6: movs r2,#0x1
  000df8b8: blx 0x00074968
  000df8bc: mov r0,sp
  000df8be: blx 0x0006ee30
  000df8c2: mov r5,sp
  000df8c4: mov r1,r6
  000df8c6: mov r0,r5
  000df8c8: blx 0x00074b60
  000df8cc: mov r0,r5
  000df8ce: mov r1,r4
  000df8d0: movs r2,#0x1
  000df8d2: blx 0x00074968
  000df8d6: mov r0,sp
  000df8d8: blx 0x0006ee30
  000df8dc: mov r0,r6
  000df8de: blx 0x0007372c
  000df8e2: mov r1,r4
  000df8e4: blx 0x00074a28
  000df8e8: mov r0,r6
  000df8ea: blx 0x0007393c
  000df8ee: cbz r0,0x000df914
  000df8f0: movs r0,#0x5
  000df8f2: mov r1,r4
  000df8f4: blx 0x00074a1c
  000df8f8: movs r5,#0x0
  000df8fa: b 0x000df90e
  000df8fc: mov r0,r6
  000df8fe: blx 0x0007393c
  000df902: ldr.w r0,[r0,r5,lsl #0x2]
  000df906: mov r1,r4
  000df908: blx 0x00074a1c
  000df90c: adds r5,#0x1
  000df90e: cmp r5,#0x5
  000df910: bne 0x000df8fc
  000df912: b 0x000df91e
  000df914: mov.w r0,#0xffffffff
  000df918: mov r1,r4
  000df91a: blx 0x00074a1c
  000df91e: mov r0,r6
  000df920: blx 0x00074014
  000df924: ldr r1,[0x000df9ec]
  000df926: add r1,pc
  000df928: ldr r5,[r1,#0x0]
  000df92a: mov r1,r4
  000df92c: blx r5
  000df92e: mov r0,r6
  000df930: blx 0x00074b6c
  000df934: mov r1,r4
  000df936: blx r5
  000df938: mov r0,r6
  000df93a: blx 0x00074b78
  000df93e: mov r1,r4
  000df940: blx r5
  000df942: mov r0,r6
  000df944: blx 0x00071bc0
  000df948: mov r1,r4
  000df94a: blx r5
  000df94c: mov r0,r6
  000df94e: blx 0x00073738
  000df952: mov r1,r4
  000df954: blx r5
  000df956: mov r0,r6
  000df958: blx 0x00073ea0
  000df95c: mov r1,r4
  000df95e: blx r5
  000df960: mov r0,r6
  000df962: blx 0x000728a4
  000df966: mov r1,r4
  000df968: blx r5
  000df96a: mov r0,r6
  000df96c: blx 0x00072bb0
  000df970: mov r1,r4
  000df972: blx r5
  000df974: mov r0,r6
  000df976: blx 0x0007288c
  000df97a: mov r1,r4
  000df97c: blx r5
  000df97e: mov r0,r6
  000df980: blx 0x00074590
  000df984: mov r1,r4
  000df986: blx 0x00074a28
  000df98a: mov r0,r6
  000df98c: str.w r6,[r8,#0x0]
  000df990: blx 0x00073438
  000df994: mov r6,r0
  000df996: cbz r0,0x000df9b4
  000df998: ldr.w r0,[r8,#0x4]
  000df99c: cmp r0,r6
  000df99e: beq 0x000df9b4
  000df9a0: movs r0,#0x1
  000df9a2: mov r1,r4
  000df9a4: blx 0x00074a1c
  000df9a8: mov r0,r8
  000df9aa: mov r1,r6
  000df9ac: mov r2,r4
  000df9ae: blx 0x00074adc
  000df9b2: b 0x000df9be
  000df9b4: mov.w r0,#0xffffffff
  000df9b8: mov r1,r4
  000df9ba: blx 0x00074a1c
  000df9be: ldr r0,[sp,#0xc]
  000df9c0: ldr.w r1,[r9,#0x0]
  000df9c4: subs r0,r1,r0
  000df9c6: itt eq
  000df9c8: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000df9cc: pop.eq {r4,r5,r6,r7,pc}
  000df9ce: blx 0x0006e824
```
