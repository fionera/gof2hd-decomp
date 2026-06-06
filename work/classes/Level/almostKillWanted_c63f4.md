# Level::almostKillWanted
elf 0xc63f4 body 232
Sig: undefined __stdcall almostKillWanted(int param_1)

## decompile
```c

/* Level::almostKillWanted(int) */

void Level::almostKillWanted(int param_1)

{
  int iVar1;
  Mission *this;
  Station *this_00;
  void *pvVar2;
  Objective *this_01;
  int iVar3;
  int in_r1;
  undefined4 *puVar4;
  
  if (*(char *)(param_1 + 0x29e) == '\0') {
    puVar4 = *(undefined4 **)(DAT_000d64ec + 0xd6410);
    *(undefined1 *)(param_1 + 0x29e) = 1;
    iVar1 = Status::isStorylineWanted((Status *)*puVar4,in_r1);
    if (iVar1 != 0) {
      this = operator_new(0x78);
      this_00 = (Station *)Status::getStation();
      iVar1 = Station::getIndex(this_00);
      Mission::Mission(this,4,0,iVar1);
      Mission::setCampaignMission(SUB41(this,0));
      Mission::setWon(this,true);
      Status::setMission((Mission *)*puVar4);
      Status::setCampaignMission((Status *)*puVar4,this);
      if (*(Objective **)(param_1 + 0x28) != (Objective *)0x0) {
        pvVar2 = (void *)Objective::~Objective(*(Objective **)(param_1 + 0x28));
        operator_delete(pvVar2);
      }
      *(undefined4 *)(param_1 + 0x28) = 0;
      this_01 = operator_new(0x1c);
      iVar1 = param_1;
      Objective::Objective(this_01,3,0,0,(Level *)param_1);
      *(Objective **)(param_1 + 0x28) = this_01;
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(param_1 + 0xf8) + 4) + 4),0))
      ;
      Player::resetDamageDoneByPlayer(*(Player **)(**(int **)(*(int *)(param_1 + 0xf8) + 4) + 4));
      iVar3 = **(int **)(*(int *)(param_1 + 0xf8) + 4);
      *(undefined1 *)(*(int *)(iVar3 + 4) + 0x5c) = 0;
      *(undefined1 *)(iVar3 + 0x43) = 1;
      iVar3 = Status::getWanted((Status *)*puVar4);
      (*(code *)(DAT_001ac034 + 0x1ac038))
                (*(undefined4 *)(*(int *)(iVar3 + 4) + in_r1 * 4),0,iVar1,&stack0xfffffff8);
      return;
    }
  }
  return;
}

```
## target disasm
```
  000d63f4: push {r4,r5,r6,r7,lr}
  000d63f6: add r7,sp,#0xc
  000d63f8: push {r6,r7,r8,r9,r11}
  000d63fc: mov r5,r0
  000d63fe: ldrb.w r0,[r0,#0x29e]
  000d6402: mov r8,r1
  000d6404: cmp r0,#0x0
  000d6406: bne 0x000d64d4
  000d6408: ldr r0,[0x000d64ec]
  000d640a: mov r1,r8
  000d640c: add r0,pc
  000d640e: ldr r4,[r0,#0x0]
  000d6410: movs r0,#0x1
  000d6412: strb.w r0,[r5,#0x29e]
  000d6416: ldr r0,[r4,#0x0]
  000d6418: blx 0x000744f4
  000d641c: cmp r0,#0x0
  000d641e: beq 0x000d64d4
  000d6420: movs r0,#0x78
  000d6422: blx 0x0006eb24
  000d6426: mov r6,r0
  000d6428: ldr r0,[r4,#0x0]
  000d642a: blx 0x00071c14
  000d642e: blx 0x00071824
  000d6432: mov r3,r0
  000d6434: mov r0,r6
  000d6436: movs r1,#0x4
  000d6438: movs r2,#0x0
  000d643a: blx 0x00073834
  000d643e: mov r0,r6
  000d6440: movs r1,#0x1
  000d6442: blx 0x00073828
  000d6446: mov r0,r6
  000d6448: movs r1,#0x1
  000d644a: blx 0x00073984
  000d644e: ldr r0,[r4,#0x0]
  000d6450: mov r1,r6
  000d6452: blx 0x00073c3c
  000d6456: ldr r0,[r4,#0x0]
  000d6458: mov r1,r6
  000d645a: blx 0x00073840
  000d645e: ldr r0,[r5,#0x28]
  000d6460: cbz r0,0x000d646a
  000d6462: blx 0x00073b34
  000d6466: blx 0x0006eb48
  000d646a: mov.w r9,#0x0
  000d646e: movs r0,#0x1c
  000d6470: str.w r9,[r5,#0x28]
  000d6474: blx 0x0006eb24
  000d6478: mov r6,r0
  000d647a: movs r1,#0x3
  000d647c: movs r2,#0x0
  000d647e: movs r3,#0x0
  000d6480: str r5,[sp,#0x0]
  000d6482: blx 0x00073f9c
  000d6486: ldr.w r0,[r5,#0xf8]
  000d648a: movs r1,#0x0
  000d648c: str r6,[r5,#0x28]
  000d648e: ldr r0,[r0,#0x4]
  000d6490: ldr r0,[r0,#0x0]
  000d6492: ldr r0,[r0,#0x4]
  000d6494: blx 0x00073e94
  000d6498: ldr.w r0,[r5,#0xf8]
  000d649c: ldr r0,[r0,#0x4]
  000d649e: ldr r0,[r0,#0x0]
  000d64a0: ldr r0,[r0,#0x4]
  000d64a2: blx 0x00074500
  000d64a6: ldr.w r0,[r5,#0xf8]
  000d64aa: ldr r0,[r0,#0x4]
  000d64ac: ldr r0,[r0,#0x0]
  000d64ae: ldr r1,[r0,#0x4]
  000d64b0: strb.w r9,[r1,#0x5c]
  000d64b4: movs r1,#0x1
  000d64b6: strb.w r1,[r0,#0x43]
  000d64ba: ldr r0,[r4,#0x0]
  000d64bc: blx 0x0007450c
  000d64c0: ldr r0,[r0,#0x4]
  000d64c2: movs r1,#0x0
  000d64c4: ldr.w r0,[r0,r8,lsl #0x2]
  000d64c8: pop.w {r2,r3,r8,r9,r11}
  000d64cc: pop.w {r4,r5,r6,r7,lr}
  000d64d0: b.w 0x001ac028
  000d64d4: pop.w {r2,r3,r8,r9,r11}
  000d64d8: pop {r4,r5,r6,r7,pc}
  001ac028: bx pc
```
