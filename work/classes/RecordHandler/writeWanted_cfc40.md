# RecordHandler::writeWanted
elf 0xcfc40 body 290
Sig: undefined __thiscall writeWanted(RecordHandler * this, Wanted * param_1, uint param_2)

## decompile
```c

/* RecordHandler::writeWanted(Wanted*, unsigned int) */

void __thiscall RecordHandler::writeWanted(RecordHandler *this,Wanted *param_1,uint param_2)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  code *pcVar5;
  int iVar6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_000dfd70 + 0xdfc52);
  iVar2 = *piVar7;
  bVar1 = (bool)Wanted::isActive(param_1);
  AEFile::Write(bVar1,param_2);
  bVar1 = (bool)Wanted::isTerminated(param_1);
  AEFile::Write(bVar1,param_2);
  uVar3 = Wanted::getCurrentLocation(param_1);
  pcVar5 = *(code **)(DAT_000dfd74 + 0xdfc7e);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getTravelsTo(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getLastSeen(param_1);
  (*pcVar5)(uVar3,param_2);
  Wanted::getName();
  AEFile::Write((String *)&stack0xffffffd0,param_2,true);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  uVar3 = Wanted::getIndex(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getBoard();
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getRace(param_1);
  (*pcVar5)(uVar3,param_2);
  bVar1 = (bool)Wanted::isMale(param_1);
  AEFile::Write(bVar1,param_2);
  uVar3 = Wanted::getShip(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getWeapon(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getHitpoints(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getLoot(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getLootAmount(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getReward();
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getRequiredBounties(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getRequiredMission(param_1);
  (*pcVar5)(uVar3,param_2);
  uVar3 = Wanted::getNumWingmen(param_1);
  (*pcVar5)(uVar3,param_2);
  for (iVar6 = 0; iVar6 != 5; iVar6 = iVar6 + 1) {
    iVar4 = Wanted::getImageParts(param_1);
    AEFile::Write(*(int *)(iVar4 + iVar6 * 4),param_2);
  }
  iVar6 = *piVar7;
  iVar2 = iVar6 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2,iVar6);
  }
  return;
}

```

## target disasm
```
  000dfc40: push {r4,r5,r6,r7,lr}
  000dfc42: add r7,sp,#0xc
  000dfc44: push {r4,r5,r6,r7,r8,r9,r11}
  000dfc48: ldr r0,[0x000dfd70]
  000dfc4a: mov r4,r2
  000dfc4c: mov r5,r1
  000dfc4e: add r0,pc
  000dfc50: ldr.w r8,[r0,#0x0]
  000dfc54: ldr.w r0,[r8,#0x0]
  000dfc58: str r0,[sp,#0xc]
  000dfc5a: mov r0,r1
  000dfc5c: blx 0x00073774
  000dfc60: mov r1,r4
  000dfc62: blx 0x00074a28
  000dfc66: mov r0,r5
  000dfc68: blx 0x00071a04
  000dfc6c: mov r1,r4
  000dfc6e: blx 0x00074a28
  000dfc72: mov r0,r5
  000dfc74: blx 0x00073780
  000dfc78: ldr r1,[0x000dfd74]
  000dfc7a: add r1,pc
  000dfc7c: ldr r6,[r1,#0x0]
  000dfc7e: mov r1,r4
  000dfc80: blx r6
  000dfc82: mov r0,r5
  000dfc84: blx 0x000737b0
  000dfc88: mov r1,r4
  000dfc8a: blx r6
  000dfc8c: mov r0,r5
  000dfc8e: blx 0x00074c2c
  000dfc92: mov r1,r4
  000dfc94: blx r6
  000dfc96: mov r9,sp
  000dfc98: mov r1,r5
  000dfc9a: mov r0,r9
  000dfc9c: blx 0x0007405c
  000dfca0: mov r0,r9
  000dfca2: mov r1,r4
  000dfca4: movs r2,#0x1
  000dfca6: blx 0x00074968
  000dfcaa: mov r0,sp
  000dfcac: blx 0x0006ee30
  000dfcb0: mov r0,r5
  000dfcb2: blx 0x00074080
  000dfcb6: mov r1,r4
  000dfcb8: blx r6
  000dfcba: mov r0,r5
  000dfcbc: blx 0x00073a08
  000dfcc0: mov r1,r4
  000dfcc2: blx r6
  000dfcc4: mov r0,r5
  000dfcc6: blx 0x00074038
  000dfcca: mov r1,r4
  000dfccc: blx r6
  000dfcce: mov r0,r5
  000dfcd0: blx 0x00074c38
  000dfcd4: mov r1,r4
  000dfcd6: blx 0x00074a28
  000dfcda: mov r0,r5
  000dfcdc: blx 0x00074044
  000dfce0: mov r1,r4
  000dfce2: blx r6
  000dfce4: mov r0,r5
  000dfce6: blx 0x00074290
  000dfcea: mov r1,r4
  000dfcec: blx r6
  000dfcee: mov r0,r5
  000dfcf0: blx 0x00074050
  000dfcf4: mov r1,r4
  000dfcf6: blx r6
  000dfcf8: mov r0,r5
  000dfcfa: blx 0x00074068
  000dfcfe: mov r1,r4
  000dfd00: blx r6
  000dfd02: mov r0,r5
  000dfd04: blx 0x00074074
  000dfd08: mov r1,r4
  000dfd0a: blx r6
  000dfd0c: mov r0,r5
  000dfd0e: blx 0x00074c44
  000dfd12: mov r1,r4
  000dfd14: blx r6
  000dfd16: mov r0,r5
  000dfd18: blx 0x00073918
  000dfd1c: mov r1,r4
  000dfd1e: blx r6
  000dfd20: mov r0,r5
  000dfd22: blx 0x00073a14
  000dfd26: mov r1,r4
  000dfd28: blx r6
  000dfd2a: mov r0,r5
  000dfd2c: blx 0x0007402c
  000dfd30: mov r1,r4
  000dfd32: blx r6
  000dfd34: movs r6,#0x0
  000dfd36: b 0x000dfd4a
  000dfd38: mov r0,r5
  000dfd3a: blx 0x00074c50
  000dfd3e: ldr.w r0,[r0,r6,lsl #0x2]
  000dfd42: mov r1,r4
  000dfd44: blx 0x00074a1c
  000dfd48: adds r6,#0x1
  000dfd4a: cmp r6,#0x5
  000dfd4c: bne 0x000dfd38
  000dfd4e: ldr r0,[sp,#0xc]
  000dfd50: ldr.w r1,[r8,#0x0]
  000dfd54: subs r0,r1,r0
  000dfd56: itt eq
  000dfd58: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000dfd5c: pop.eq {r4,r5,r6,r7,pc}
  000dfd5e: blx 0x0006e824
```
