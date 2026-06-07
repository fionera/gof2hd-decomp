# RecordHandler::writeAgent
elf 0xcf9f0 body 548
Sig: undefined __thiscall writeAgent(RecordHandler * this, Agent * param_1, uint param_2)

## decompile
```c

/* RecordHandler::writeAgent(Agent*, unsigned int) */

void __thiscall RecordHandler::writeAgent(RecordHandler *this,Agent *param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  String *pSVar5;
  Mission *pMVar6;
  code *pcVar7;
  int *piVar8;
  
  piVar8 = *(int **)(DAT_000dfc2c + 0xdfa04);
  iVar1 = *piVar8;
  uVar2 = Agent::getCosts(param_1);
  pcVar7 = *(code **)(DAT_000dfc30 + 0xdfa18);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSellSystemIndex(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSellBlueprintIndex(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getEvent(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getIndex(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getOffer(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getRace(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSellItemIndex(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSellItemPrice(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSellItemQuantity(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getStation(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getSystem(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::getWingmanFriendsCount(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::isMale(param_1);
  pcVar7 = *(code **)(DAT_000dfc34 + 0xdfaa0);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::hasReward(param_1);
  (*pcVar7)(uVar2,param_2);
  uVar2 = Agent::hasAcceptedOffer(param_1);
  (*pcVar7)(uVar2,param_2);
  (*pcVar7)(param_1[0x24],param_2);
  (*pcVar7)(param_1[0x25],param_2);
  iVar3 = Agent::getImageParts(param_1);
  if (iVar3 == 0) {
    AEFile::Write(-1,param_2);
  }
  else {
    AEFile::Write(5,param_2);
    for (iVar3 = 0; iVar3 != 5; iVar3 = iVar3 + 1) {
      iVar4 = Agent::getImageParts(param_1);
      AEFile::Write(*(int *)(iVar4 + iVar3 * 4),param_2);
    }
  }
  iVar3 = Agent::getIndex(param_1);
  if (0x12 < iVar3) {
    iVar3 = Agent::getSellModIndex(param_1);
    AEFile::Write(iVar3,param_2);
  }
  Agent::getMissionString();
  AEFile::Write((String *)&stack0xffffffd0,param_2,true);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  Agent::getName();
  AEFile::Write((String *)&stack0xffffffd0,param_2,true);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  Agent::getStationName();
  AEFile::Write((String *)&stack0xffffffd0,param_2,true);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  Agent::getSystemName();
  AEFile::Write((String *)&stack0xffffffd0,param_2,true);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  if (*(String **)(param_1 + 0xc) == (String *)0x0) {
    pSVar5 = (String *)
             AbyssEngine::String::String
                       ((String *)&stack0xffffffd0,(char *)(DAT_000dfc38 + 0xdfb94),false);
    AEFile::Write(pSVar5,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
  }
  else {
    AEFile::Write(*(String **)(param_1 + 0xc),param_2,true);
  }
  if (*(String **)(param_1 + 0x10) == (String *)0x0) {
    pSVar5 = (String *)
             AbyssEngine::String::String
                       ((String *)&stack0xffffffd0,(char *)(DAT_000dfc3c + 0xdfbbc),false);
    AEFile::Write(pSVar5,param_2,true);
    AbyssEngine::String::~String((String *)&stack0xffffffd0);
  }
  else {
    AEFile::Write(*(String **)(param_1 + 0x10),param_2,true);
  }
  *(Agent **)(this + 4) = param_1;
  pMVar6 = (Mission *)Agent::getMission(param_1);
  if ((pMVar6 == (Mission *)0x0) || (*(Mission **)this == pMVar6)) {
    AEFile::Write(-1,param_2);
  }
  else {
    AEFile::Write(1,param_2);
    writeMission(this,pMVar6,param_2);
  }
  iVar3 = *piVar8;
  iVar1 = iVar3 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1,iVar3);
}

```

## target disasm
```
  000df9f0: push {r4,r5,r6,r7,lr}
  000df9f2: add r7,sp,#0xc
  000df9f4: push {r4,r5,r6,r7,r8,r9,r11}
  000df9f8: mov r8,r0
  000df9fa: ldr r0,[0x000dfc2c]
  000df9fc: mov r4,r2
  000df9fe: mov r6,r1
  000dfa00: add r0,pc
  000dfa02: ldr.w r9,[r0,#0x0]
  000dfa06: ldr.w r0,[r9,#0x0]
  000dfa0a: str r0,[sp,#0xc]
  000dfa0c: mov r0,r1
  000dfa0e: blx 0x00071bfc
  000dfa12: ldr r1,[0x000dfc30]
  000dfa14: add r1,pc
  000dfa16: ldr r5,[r1,#0x0]
  000dfa18: mov r1,r4
  000dfa1a: blx r5
  000dfa1c: mov r0,r6
  000dfa1e: blx 0x00074b84
  000dfa22: mov r1,r4
  000dfa24: blx r5
  000dfa26: mov r0,r6
  000dfa28: blx 0x00074b90
  000dfa2c: mov r1,r4
  000dfa2e: blx r5
  000dfa30: mov r0,r6
  000dfa32: blx 0x00074b9c
  000dfa36: mov r1,r4
  000dfa38: blx r5
  000dfa3a: mov r0,r6
  000dfa3c: blx 0x00074ba8
  000dfa40: mov r1,r4
  000dfa42: blx r5
  000dfa44: mov r0,r6
  000dfa46: blx 0x00071ae8
  000dfa4a: mov r1,r4
  000dfa4c: blx r5
  000dfa4e: mov r0,r6
  000dfa50: blx 0x00071bd8
  000dfa54: mov r1,r4
  000dfa56: blx r5
  000dfa58: mov r0,r6
  000dfa5a: blx 0x00074bb4
  000dfa5e: mov r1,r4
  000dfa60: blx r5
  000dfa62: mov r0,r6
  000dfa64: blx 0x00074bc0
  000dfa68: mov r1,r4
  000dfa6a: blx r5
  000dfa6c: mov r0,r6
  000dfa6e: blx 0x00074bcc
  000dfa72: mov r1,r4
  000dfa74: blx r5
  000dfa76: mov r0,r6
  000dfa78: blx 0x00071ac4
  000dfa7c: mov r1,r4
  000dfa7e: blx r5
  000dfa80: mov r0,r6
  000dfa82: blx 0x00071c20
  000dfa86: mov r1,r4
  000dfa88: blx r5
  000dfa8a: mov r0,r6
  000dfa8c: blx 0x00074bd8
  000dfa90: mov r1,r4
  000dfa92: blx r5
  000dfa94: mov r0,r6
  000dfa96: blx 0x00074110
  000dfa9a: ldr r1,[0x000dfc34]
  000dfa9c: add r1,pc
  000dfa9e: ldr r5,[r1,#0x0]
  000dfaa0: mov r1,r4
  000dfaa2: blx r5
  000dfaa4: mov r0,r6
  000dfaa6: blx 0x00074be4
  000dfaaa: mov r1,r4
  000dfaac: blx r5
  000dfaae: mov r0,r6
  000dfab0: blx 0x00074bf0
  000dfab4: mov r1,r4
  000dfab6: blx r5
  000dfab8: ldrb.w r0,[r6,#0x24]
  000dfabc: mov r1,r4
  000dfabe: blx r5
  000dfac0: ldrb.w r0,[r6,#0x25]
  000dfac4: mov r1,r4
  000dfac6: blx r5
  000dfac8: mov r0,r6
  000dfaca: blx 0x00071c74
  000dface: cbz r0,0x000dfaf4
  000dfad0: movs r0,#0x5
  000dfad2: mov r1,r4
  000dfad4: blx 0x00074a1c
  000dfad8: movs r5,#0x0
  000dfada: b 0x000dfaee
  000dfadc: mov r0,r6
  000dfade: blx 0x00071c74
  000dfae2: ldr.w r0,[r0,r5,lsl #0x2]
  000dfae6: mov r1,r4
  000dfae8: blx 0x00074a1c
  000dfaec: adds r5,#0x1
  000dfaee: cmp r5,#0x5
  000dfaf0: bne 0x000dfadc
  000dfaf2: b 0x000dfafe
  000dfaf4: mov.w r0,#0xffffffff
  000dfaf8: mov r1,r4
  000dfafa: blx 0x00074a1c
  000dfafe: mov r0,r6
  000dfb00: blx 0x00074ba8
  000dfb04: cmp r0,#0x13
  000dfb06: blt 0x000dfb14
  000dfb08: mov r0,r6
  000dfb0a: blx 0x00074bfc
  000dfb0e: mov r1,r4
  000dfb10: blx 0x00074a1c
  000dfb14: mov r5,sp
  000dfb16: mov r1,r6
  000dfb18: mov r0,r5
  000dfb1a: blx 0x00074c08
  000dfb1e: mov r0,r5
  000dfb20: mov r1,r4
  000dfb22: movs r2,#0x1
  000dfb24: blx 0x00074968
  000dfb28: mov r0,sp
  000dfb2a: blx 0x0006ee30
  000dfb2e: mov r5,sp
  000dfb30: mov r1,r6
  000dfb32: mov r0,r5
  000dfb34: blx 0x00071c2c
  000dfb38: mov r0,r5
  000dfb3a: mov r1,r4
  000dfb3c: movs r2,#0x1
  000dfb3e: blx 0x00074968
  000dfb42: mov r0,sp
  000dfb44: blx 0x0006ee30
  000dfb48: mov r5,sp
  000dfb4a: mov r1,r6
  000dfb4c: mov r0,r5
  000dfb4e: blx 0x00074c14
  000dfb52: mov r0,r5
  000dfb54: mov r1,r4
  000dfb56: movs r2,#0x1
  000dfb58: blx 0x00074968
  000dfb5c: mov r0,sp
  000dfb5e: blx 0x0006ee30
  000dfb62: mov r5,sp
  000dfb64: mov r1,r6
  000dfb66: mov r0,r5
  000dfb68: blx 0x00074c20
  000dfb6c: mov r0,r5
  000dfb6e: mov r1,r4
  000dfb70: movs r2,#0x1
  000dfb72: blx 0x00074968
  000dfb76: mov r0,sp
  000dfb78: blx 0x0006ee30
  000dfb7c: ldr r0,[r6,#0xc]
  000dfb7e: cbz r0,0x000dfb8a
  000dfb80: mov r1,r4
  000dfb82: movs r2,#0x1
  000dfb84: blx 0x00074968
  000dfb88: b 0x000dfba4
  000dfb8a: ldr r1,[0x000dfc38]
  000dfb8c: mov r0,sp
  000dfb8e: movs r2,#0x0
  000dfb90: add r1,pc
  000dfb92: blx 0x0006ee18
  000dfb96: mov r1,r4
  000dfb98: movs r2,#0x1
  000dfb9a: blx 0x00074968
  000dfb9e: mov r0,sp
  000dfba0: blx 0x0006ee30
  000dfba4: ldr r0,[r6,#0x10]
  000dfba6: cbz r0,0x000dfbb2
  000dfba8: mov r1,r4
  000dfbaa: movs r2,#0x1
  000dfbac: blx 0x00074968
  000dfbb0: b 0x000dfbcc
  000dfbb2: ldr r1,[0x000dfc3c]
  000dfbb4: mov r0,sp
  000dfbb6: movs r2,#0x0
  000dfbb8: add r1,pc
  000dfbba: blx 0x0006ee18
  000dfbbe: mov r1,r4
  000dfbc0: movs r2,#0x1
  000dfbc2: blx 0x00074968
  000dfbc6: mov r0,sp
  000dfbc8: blx 0x0006ee30
  000dfbcc: mov r0,r6
  000dfbce: str.w r6,[r8,#0x4]
  000dfbd2: blx 0x00071bb4
  000dfbd6: mov r6,r0
  000dfbd8: cbz r0,0x000dfbf6
  000dfbda: ldr.w r0,[r8,#0x0]
  000dfbde: cmp r0,r6
  000dfbe0: beq 0x000dfbf6
  000dfbe2: movs r0,#0x1
  000dfbe4: mov r1,r4
  000dfbe6: blx 0x00074a1c
  000dfbea: mov r0,r8
  000dfbec: mov r1,r6
  000dfbee: mov r2,r4
  000dfbf0: blx 0x00074a88
  000dfbf4: b 0x000dfc00
  000dfbf6: mov.w r0,#0xffffffff
  000dfbfa: mov r1,r4
  000dfbfc: blx 0x00074a1c
  000dfc00: ldr r0,[sp,#0xc]
  000dfc02: ldr.w r1,[r9,#0x0]
  000dfc06: subs r0,r1,r0
  000dfc08: itt eq
  000dfc0a: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000dfc0e: pop.eq {r4,r5,r6,r7,pc}
  000dfc10: blx 0x0006e824
```
