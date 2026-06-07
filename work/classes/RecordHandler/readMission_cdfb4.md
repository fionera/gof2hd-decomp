# RecordHandler::readMission
elf 0xcdfb4 body 558
Sig: undefined __thiscall readMission(RecordHandler * this, uint param_1)

## decompile
```c

/* RecordHandler::readMission(unsigned int) */

void __thiscall RecordHandler::readMission(RecordHandler *this,uint param_1)

{
  String *pSVar1;
  uint uVar2;
  int *piVar3;
  Agent *this_00;
  Mission *this_01;
  uint uVar4;
  code *pcVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  String aSStack_a8 [12];
  String aSStack_9c [12];
  int local_90;
  bool bStack_89;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  uint local_64;
  bool bStack_5d;
  String aSStack_5c [12];
  String aSStack_50 [12];
  String aSStack_44 [12];
  String aSStack_38 [12];
  int local_2c;
  int local_28;
  
  piVar6 = *(int **)(DAT_000de244 + 0xddfc8);
  local_28 = *piVar6;
  local_2c = 0;
  AEFile::Read(&local_2c,param_1);
  if (local_2c != -1) {
    pSVar1 = (String *)AbyssEngine::String::String(aSStack_38);
    AEFile::Read(pSVar1,param_1,true);
    AbyssEngine::String::String(aSStack_44);
    AEFile::Read(aSStack_44,param_1,true);
    AbyssEngine::String::String(aSStack_50);
    AEFile::Read(aSStack_50,param_1,true);
    AbyssEngine::String::String(aSStack_5c);
    AEFile::Read(aSStack_5c,param_1,true);
    bStack_5d = false;
    AEFile::Read(&bStack_5d,param_1);
    local_64 = 0;
    AEFile::Read((int *)&local_64,param_1);
    uVar4 = local_64;
    if ((int)local_64 < 1) {
      piVar3 = (int *)0x0;
    }
    else {
      uVar2 = (uint)((ulonglong)local_64 * 4);
      if ((int)((ulonglong)local_64 * 4 >> 0x20) != 0) {
        uVar2 = 0xffffffff;
      }
      piVar3 = operator_new__(uVar2);
      piVar7 = piVar3;
      for (iVar8 = 0; iVar8 < (int)uVar4; iVar8 = iVar8 + 1) {
        AEFile::Read(piVar7,param_1);
        piVar7 = piVar7 + 1;
        uVar4 = local_64;
      }
    }
    local_68 = 0;
    local_70 = 0;
    local_6c = 0;
    local_78 = 0;
    local_74 = 0;
    local_80 = 0;
    local_7c = 0;
    local_88 = 0;
    local_84 = 0;
    bStack_89 = false;
    AEFile::Read(&local_68,param_1);
    AEFile::Read(&local_6c,param_1);
    AEFile::Read(&local_70,param_1);
    AEFile::Read(&local_74,param_1);
    AEFile::Read(&local_78,param_1);
    AEFile::Read(&local_7c,param_1);
    AEFile::Read(&local_80,param_1);
    AEFile::Read(&local_84,param_1);
    AEFile::Read(&local_88,param_1);
    AEFile::Read(&bStack_89,param_1);
    local_90 = 0;
    AEFile::Read(&local_90,param_1);
    if (local_90 < 1) {
      this_00 = (Agent *)0x0;
    }
    else {
      this_00 = (Agent *)readAgent(this,param_1);
    }
    if (bStack_5d == false) {
      this_01 = operator_new(0x78);
      iVar8 = local_2c;
      AbyssEngine::String::String(aSStack_9c,aSStack_38,false);
      Mission::Mission(this_01,iVar8,aSStack_9c,piVar3,local_68,local_74,local_78,local_7c);
      AbyssEngine::String::~String(aSStack_9c);
    }
    else {
      this_01 = operator_new(0x78);
      Mission::Mission(this_01,local_2c,local_74,local_78);
    }
    Mission::setCosts(this_01,local_6c);
    Mission::setBonus(this_01,local_70);
    Mission::setProductionGoods(this_01,local_80,local_84);
    Mission::setStatusValue((int)this_01);
    Mission::setVisible(this_01,bStack_89);
    Mission::setAgent(this_01,this_00);
    AbyssEngine::String::String(aSStack_a8,aSStack_44,false);
    Mission::setTargetName(this_01,aSStack_a8);
    AbyssEngine::String::~String(aSStack_a8);
    if (this_00 != (Agent *)0x0) {
      Agent::setMission(this_00,this_01);
    }
    pcVar5 = *(code **)(DAT_000de24c + 0xde1bc);
    (*pcVar5)(aSStack_5c);
    (*pcVar5)(aSStack_50);
    (*pcVar5)(aSStack_44);
    (*pcVar5)(aSStack_38);
  }
  iVar8 = *piVar6 - local_28;
  if (iVar8 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar8);
  }
  return;
}

```

## target disasm
```
  000ddfb4: push {r4,r5,r6,r7,lr}
  000ddfb6: add r7,sp,#0xc
  000ddfb8: push {r8,r9,r10,r11}
  000ddfbc: sub sp,#0x9c
  000ddfbe: mov r9,r0
  000ddfc0: ldr r0,[0x000de244]
  000ddfc2: mov r4,r1
  000ddfc4: add r0,pc
  000ddfc6: ldr r6,[r0,#0x0]
  000ddfc8: ldr r0,[r6,#0x0]
  000ddfca: str r0,[sp,#0x98]
  000ddfcc: movs r0,#0x0
  000ddfce: str r0,[sp,#0x94]
  000ddfd0: add r0,sp,#0x94
  000ddfd2: blx 0x00074818
  000ddfd6: ldr r0,[sp,#0x94]
  000ddfd8: adds r0,#0x1
  000ddfda: beq 0x000de07a
  000ddfdc: add r0,sp,#0x88
  000ddfde: blx 0x0006efbc
  000ddfe2: mov r1,r4
  000ddfe4: movs r2,#0x1
  000ddfe6: blx 0x00074824
  000ddfea: add r0,sp,#0x7c
  000ddfec: blx 0x0006efbc
  000ddff0: add r0,sp,#0x7c
  000ddff2: mov r1,r4
  000ddff4: movs r2,#0x1
  000ddff6: blx 0x00074824
  000ddffa: add r0,sp,#0x70
  000ddffc: blx 0x0006efbc
  000de000: add r0,sp,#0x70
  000de002: mov r1,r4
  000de004: movs r2,#0x1
  000de006: blx 0x00074824
  000de00a: add r0,sp,#0x64
  000de00c: blx 0x0006efbc
  000de010: add r0,sp,#0x64
  000de012: mov r1,r4
  000de014: movs r2,#0x1
  000de016: blx 0x00074824
  000de01a: movs r5,#0x0
  000de01c: strb.w r5,[r7,#-0x55]
  000de020: sub.w r0,r7,#0x55
  000de024: mov r1,r4
  000de026: blx 0x00074854
  000de02a: str r5,[sp,#0x5c]
  000de02c: add r0,sp,#0x5c
  000de02e: mov r1,r4
  000de030: blx 0x00074818
  000de034: ldr r5,[sp,#0x5c]
  000de036: mov.w r11,#0x0
  000de03a: cmp r5,#0x1
  000de03c: blt 0x000de084
  000de03e: movs r0,#0x4
  000de040: umull r0,r1,r5,r0
  000de044: cmp r1,#0x0
  000de046: it ne
  000de048: mov.ne r1,#0x1
  000de04a: cmp r1,#0x0
  000de04c: it ne
  000de04e: mov.ne.w r0,#0xffffffff
  000de052: blx 0x0006ec08
  000de056: str r6,[sp,#0x14]
  000de058: mov.w r10,#0x0
  000de05c: mov r8,r0
  000de05e: mov r6,r0
  000de060: b 0x000de072
  000de062: mov r0,r6
  000de064: mov r1,r4
  000de066: blx 0x00074818
  000de06a: ldr r5,[sp,#0x5c]
  000de06c: adds r6,#0x4
  000de06e: add.w r10,r10,#0x1
  000de072: cmp r10,r5
  000de074: blt 0x000de062
  000de076: ldr r6,[sp,#0x14]
  000de078: b 0x000de088
  000de07a: ldr r0,[0x000de248]
  000de07c: add r0,pc
  000de07e: ldr r0,[r0,#0x0]
  000de080: ldr r4,[r0,#0x0]
  000de082: b 0x000de1cc
  000de084: mov.w r8,#0x0
  000de088: str.w r11,[sp,#0x58]
  000de08c: strd r11,r11,[sp,#0x50]
  000de090: strd r11,r11,[sp,#0x48]
  000de094: strd r11,r11,[sp,#0x40]
  000de098: strd r11,r11,[sp,#0x38]
  000de09c: strb.w r11,[r7,#-0x81]
  000de0a0: add r0,sp,#0x58
  000de0a2: mov r1,r4
  000de0a4: blx 0x00074818
  000de0a8: add r0,sp,#0x54
  000de0aa: mov r1,r4
  000de0ac: blx 0x00074818
  000de0b0: add r0,sp,#0x50
  000de0b2: mov r1,r4
  000de0b4: blx 0x00074818
  000de0b8: add r0,sp,#0x4c
  000de0ba: mov r1,r4
  000de0bc: blx 0x00074818
  000de0c0: add r0,sp,#0x48
  000de0c2: mov r1,r4
  000de0c4: blx 0x00074818
  000de0c8: add r0,sp,#0x44
  000de0ca: mov r1,r4
  000de0cc: blx 0x00074818
  000de0d0: add r0,sp,#0x40
  000de0d2: mov r1,r4
  000de0d4: blx 0x00074818
  000de0d8: add r0,sp,#0x3c
  000de0da: mov r1,r4
  000de0dc: blx 0x00074818
  000de0e0: add r0,sp,#0x38
  000de0e2: mov r1,r4
  000de0e4: blx 0x00074818
  000de0e8: sub.w r0,r7,#0x81
  000de0ec: mov r1,r4
  000de0ee: blx 0x00074854
  000de0f2: movs r0,#0x0
  000de0f4: str r0,[sp,#0x30]
  000de0f6: add r0,sp,#0x30
  000de0f8: mov r1,r4
  000de0fa: blx 0x00074818
  000de0fe: ldr r0,[sp,#0x30]
  000de100: cmp r0,#0x1
  000de102: blt 0x000de110
  000de104: mov r0,r9
  000de106: mov r1,r4
  000de108: blx 0x00074890
  000de10c: mov r5,r0
  000de10e: b 0x000de112
  000de110: movs r5,#0x0
  000de112: ldrb.w r0,[r7,#-0x55]
  000de116: cbz r0,0x000de12c
  000de118: movs r0,#0x78
  000de11a: blx 0x0006eb24
  000de11e: ldrd r3,r2,[sp,#0x48]
  000de122: mov r4,r0
  000de124: ldr r1,[sp,#0x94]
  000de126: blx 0x00073834
  000de12a: b 0x000de160
  000de12c: movs r0,#0x78
  000de12e: blx 0x0006eb24
  000de132: ldr.w r9,[sp,#0x94]
  000de136: mov r4,r0
  000de138: add r0,sp,#0x24
  000de13a: add r1,sp,#0x88
  000de13c: movs r2,#0x0
  000de13e: blx 0x0006f028
  000de142: ldr r0,[sp,#0x58]
  000de144: ldr r1,[sp,#0x4c]
  000de146: ldrd r3,r2,[sp,#0x44]
  000de14a: stm sp,{r0,r1,r2,r3}
  000de14e: add r2,sp,#0x24
  000de150: mov r0,r4
  000de152: mov r1,r9
  000de154: mov r3,r8
  000de156: blx 0x00071c80
  000de15a: add r0,sp,#0x24
  000de15c: blx 0x0006ee30
  000de160: ldr r1,[sp,#0x54]
  000de162: mov r0,r4
  000de164: blx 0x00071c98
  000de168: ldr r1,[sp,#0x50]
  000de16a: mov r0,r4
  000de16c: blx 0x00071cb0
  000de170: ldrd r2,r1,[sp,#0x3c]
  000de174: mov r0,r4
  000de176: blx 0x00071ca4
  000de17a: ldr r1,[sp,#0x38]
  000de17c: mov r0,r4
  000de17e: blx 0x00072898
  000de182: ldrb.w r1,[r7,#-0x81]
  000de186: mov r0,r4
  000de188: blx 0x00073858
  000de18c: mov r0,r4
  000de18e: mov r1,r5
  000de190: blx 0x00074140
  000de194: add r0,sp,#0x18
  000de196: add r1,sp,#0x7c
  000de198: movs r2,#0x0
  000de19a: blx 0x0006f028
  000de19e: add r1,sp,#0x18
  000de1a0: mov r0,r4
  000de1a2: blx 0x00071c8c
  000de1a6: add r0,sp,#0x18
  000de1a8: blx 0x0006ee30
  000de1ac: cbz r5,0x000de1b6
  000de1ae: mov r0,r5
  000de1b0: mov r1,r4
  000de1b2: blx 0x00071b78
  000de1b6: ldr r0,[0x000de24c]
  000de1b8: add r0,pc
  000de1ba: ldr r5,[r0,#0x0]
  000de1bc: add r0,sp,#0x64
  000de1be: blx r5
  000de1c0: add r0,sp,#0x70
  000de1c2: blx r5
  000de1c4: add r0,sp,#0x7c
  000de1c6: blx r5
  000de1c8: add r0,sp,#0x88
  000de1ca: blx r5
  000de1cc: ldr r0,[sp,#0x98]
  000de1ce: ldr r1,[r6,#0x0]
  000de1d0: subs r0,r1,r0
  000de1d2: itttt eq
  000de1d4: mov.eq r0,r4
  000de1d6: add.eq sp,#0x9c
  000de1d8: pop.eq.w {r8,r9,r10,r11}
  000de1dc: pop.eq {r4,r5,r6,r7,pc}
  000de1de: blx 0x0006e824
```
