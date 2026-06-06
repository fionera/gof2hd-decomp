# Level::alarmAllFriends
elf 0xc55d0 body 152
Sig: undefined __thiscall alarmAllFriends(Level * this, int param_1, bool param_2)

## decompile
```c

/* Level::alarmAllFriends(int, bool) */

void __thiscall Level::alarmAllFriends(Level *this,int param_1,bool param_2)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  
  puVar1 = *(uint **)(this + 0xf8);
  if (puVar1 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      iVar2 = *(int *)(puVar1[1] + uVar4 * 4);
      if (*(int *)(iVar2 + 0x28) == param_1) {
        Player::setAlwaysEnemy(SUB41(*(undefined4 *)(iVar2 + 4),0));
        puVar1 = *(uint **)(this + 0xf8);
      }
    }
  }
  if ((this[0x189] == (Level)0x0) && (param_2)) {
    iVar5 = 1;
    this[0x189] = (Level)0x1;
    iVar2 = Status::inBlackMarketSystem();
    if (iVar2 != 0) {
      iVar5 = 0xc;
    }
    createRadioMessage((int)this,iVar5);
    Status::getSystem();
    iVar2 = SolarSystem::getRace();
    if (iVar2 == param_1) {
      uVar3 = Status::getStation();
      (*(code *)(DAT_001abff4 + 0x1abff8))(uVar3,1);
      return;
    }
  }
  return;
}

```
## target disasm
```
  000d55d0: push {r4,r5,r6,r7,lr}
  000d55d2: add r7,sp,#0xc
  000d55d4: push.w r8
  000d55d8: mov r5,r0
  000d55da: ldr.w r0,[r0,#0xf8]
  000d55de: mov r8,r2
  000d55e0: mov r4,r1
  000d55e2: cbz r0,0x000d5608
  000d55e4: movs r6,#0x0
  000d55e6: b 0x000d5602
  000d55e8: ldr r1,[r0,#0x4]
  000d55ea: ldr.w r1,[r1,r6,lsl #0x2]
  000d55ee: ldr r2,[r1,#0x28]
  000d55f0: cmp r2,r4
  000d55f2: bne 0x000d5600
  000d55f4: ldr r0,[r1,#0x4]
  000d55f6: movs r1,#0x1
  000d55f8: blx 0x00073e94
  000d55fc: ldr.w r0,[r5,#0xf8]
  000d5600: adds r6,#0x1
  000d5602: ldr r1,[r0,#0x0]
  000d5604: cmp r6,r1
  000d5606: bcc 0x000d55e8
  000d5608: ldrb.w r0,[r5,#0x189]
  000d560c: cbnz r0,0x000d5660
  000d560e: cmp.w r8,#0x0
  000d5612: beq 0x000d5660
  000d5614: ldr r0,[0x000d5668]
  000d5616: movs r6,#0x1
  000d5618: strb.w r6,[r5,#0x189]
  000d561c: add r0,pc
  000d561e: ldr.w r8,[r0,#0x0]
  000d5622: ldr.w r0,[r8,#0x0]
  000d5626: blx 0x00071998
  000d562a: cmp r0,#0x0
  000d562c: it ne
  000d562e: mov.ne r6,#0xc
  000d5630: mov r0,r5
  000d5632: mov r1,r6
  000d5634: mov r2,r4
  000d5636: blx 0x00072754
  000d563a: ldr.w r0,[r8,#0x0]
  000d563e: blx 0x00071a10
  000d5642: blx 0x00071950
  000d5646: cmp r0,r4
  000d5648: bne 0x000d5660
  000d564a: ldr.w r0,[r8,#0x0]
  000d564e: blx 0x00071c14
  000d5652: movs r1,#0x1
  000d5654: pop.w r8
  000d5658: pop.w {r4,r5,r6,r7,lr}
  000d565c: b.w 0x001abfe8
  000d5660: pop.w r8
  000d5664: pop {r4,r5,r6,r7,pc}
  001abfe8: bx pc
```
