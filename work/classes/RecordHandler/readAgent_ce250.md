# RecordHandler::readAgent
elf 0xce250 body 866
Sig: undefined __thiscall readAgent(RecordHandler * this, uint param_1)

## decompile
```c

/* RecordHandler::readAgent(unsigned int) */

void __thiscall RecordHandler::readAgent(RecordHandler *this,uint param_1)

{
  uint uVar1;
  Mission *pMVar2;
  Agent *this_00;
  String *pSVar3;
  Array *pAVar4;
  String *this_01;
  undefined4 *puVar5;
  code *pcVar6;
  uint uVar7;
  int iVar8;
  code *pcVar9;
  int *piVar10;
  int *piVar11;
  int *piVar12;
  String aSStack_e8 [12];
  String aSStack_dc [12];
  String aSStack_d0 [12];
  String aSStack_c4 [12];
  int local_b8;
  String aSStack_b4 [8];
  int local_ac;
  String aSStack_a8 [8];
  int local_a0;
  String aSStack_9c [12];
  String aSStack_90 [12];
  String aSStack_84 [12];
  String aSStack_78 [12];
  int local_6c;
  uint local_68;
  Agent AStack_61;
  Agent local_60;
  bool bStack_5f;
  bool bStack_5e;
  undefined1 uStack_5d;
  uint local_5c;
  undefined4 local_58;
  undefined4 local_54;
  int local_50;
  int local_4c;
  int local_48;
  undefined4 local_44;
  int local_40;
  int local_3c;
  int local_38;
  undefined4 local_34;
  undefined4 local_30;
  int local_2c;
  int local_28;
  
  piVar11 = (int *)0x0;
  piVar12 = *(int **)(DAT_000de608 + 0xde266);
  pcVar9 = *(code **)(DAT_000de60c + 0xde26c);
  local_28 = *piVar12;
  local_2c = 0;
  (*pcVar9)(&local_2c,param_1);
  local_30 = 0;
  (*pcVar9)(&local_30,param_1);
  local_34 = 0;
  (*pcVar9)(&local_34,param_1);
  local_38 = 0;
  (*pcVar9)(&local_38,param_1);
  local_3c = 0;
  (*pcVar9)(&local_3c,param_1);
  local_40 = 0;
  (*pcVar9)(&local_40,param_1);
  local_44 = 0;
  (*pcVar9)(&local_44,param_1);
  local_48 = 0;
  (*pcVar9)(&local_48,param_1);
  local_4c = 0;
  (*pcVar9)(&local_4c,param_1);
  local_50 = 0;
  (*pcVar9)(&local_50,param_1);
  local_54 = 0;
  (*pcVar9)(&local_54,param_1);
  local_58 = 0;
  (*pcVar9)(&local_58,param_1);
  local_5c = 0;
  (*pcVar9)(&local_5c,param_1);
  uStack_5d = 0;
  pcVar6 = *(code **)(DAT_000de63c + 0xde304);
  (*pcVar6)(&uStack_5d,param_1);
  bStack_5e = false;
  (*pcVar6)(&bStack_5e,param_1);
  bStack_5f = false;
  (*pcVar6)(&bStack_5f,param_1);
  local_60 = (Agent)0x0;
  (*pcVar6)(&local_60,param_1);
  AStack_61 = (Agent)0x0;
  (*pcVar6)(&AStack_61,param_1);
  local_68 = 0;
  (*pcVar9)(&local_68,param_1);
  uVar7 = local_68;
  if (0 < (int)local_68) {
    uVar1 = (uint)((ulonglong)local_68 * 4);
    if ((int)((ulonglong)local_68 * 4 >> 0x20) != 0) {
      uVar1 = 0xffffffff;
    }
    piVar11 = operator_new__(uVar1);
    piVar10 = piVar11;
    for (iVar8 = 0; iVar8 < (int)uVar7; iVar8 = iVar8 + 1) {
      AEFile::Read(piVar10,param_1);
      piVar10 = piVar10 + 1;
      uVar7 = local_68;
    }
  }
  local_6c = -1;
  if (0x12 < local_3c) {
    AEFile::Read(&local_6c,param_1);
  }
  AbyssEngine::String::String(aSStack_78);
  AbyssEngine::String::String(aSStack_84);
  AbyssEngine::String::String(aSStack_90);
  AbyssEngine::String::String(aSStack_9c);
  AbyssEngine::String::String(aSStack_a8);
  AbyssEngine::String::String(aSStack_b4);
  AEFile::Read(aSStack_78,param_1,true);
  AEFile::Read(aSStack_84,param_1,true);
  AEFile::Read(aSStack_90,param_1,true);
  AEFile::Read(aSStack_9c,param_1,true);
  AEFile::Read(aSStack_a8,param_1,true);
  AEFile::Read(aSStack_b4,param_1,true);
  local_b8 = 0;
  AEFile::Read(&local_b8,param_1);
  if (local_b8 < 1) {
    pMVar2 = (Mission *)0x0;
  }
  else {
    pMVar2 = (Mission *)readMission(this,param_1);
  }
  this_00 = operator_new(0x98);
  iVar8 = local_3c;
  AbyssEngine::String::String(aSStack_c4,aSStack_84,false);
  Agent::Agent(this_00,iVar8,aSStack_c4,local_54,local_58,local_44,uStack_5d,local_30,local_34,
               local_6c,local_4c);
  AbyssEngine::String::~String(aSStack_c4);
  Agent::setCosts(this_00,local_2c);
  Agent::setEvent(this_00,local_38);
  Agent::setOffer(this_00,local_40);
  Agent::setSellItemData(this_00,local_48,local_50,local_4c);
  if (local_a0 != 0) {
    pSVar3 = operator_new(0xc);
    AbyssEngine::String::String(pSVar3,aSStack_a8,false);
    *(String **)(this_00 + 0xc) = pSVar3;
  }
  if (local_ac != 0) {
    pSVar3 = operator_new(0xc);
    AbyssEngine::String::String(pSVar3,aSStack_b4,false);
    *(String **)(this_00 + 0x10) = pSVar3;
  }
  *(uint *)(this_00 + 0x14) = local_5c;
  pAVar4 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  ArraySetLength<AbyssEngine::String*>(local_5c,pAVar4);
  for (iVar8 = 0; iVar8 < (int)local_5c; iVar8 = iVar8 + 1) {
    this_01 = operator_new(0xc);
    pSVar3 = aSStack_b4;
    if (iVar8 == 0) {
      pSVar3 = aSStack_a8;
    }
    AbyssEngine::String::String(this_01,pSVar3,false);
    *(String **)(*(int *)(pAVar4 + 4) + iVar8 * 4) = this_01;
  }
  Agent::setWingmanFriendNames(this_00,pAVar4);
  Agent::giveRewardAtNextChat(this_00,bStack_5e);
  Agent::setOfferAccepted(this_00,bStack_5f);
  Agent::setImageParts(this_00,piVar11);
  AbyssEngine::String::String(aSStack_d0,aSStack_78,false);
  Agent::setMissionString(this_00,aSStack_d0);
  AbyssEngine::String::~String(aSStack_d0);
  AbyssEngine::String::String(aSStack_dc,aSStack_90,false);
  Agent::setStationName(this_00,aSStack_dc);
  AbyssEngine::String::~String(aSStack_dc);
  AbyssEngine::String::String(aSStack_e8,aSStack_9c,false);
  Agent::setSystemName(this_00,aSStack_e8);
  AbyssEngine::String::~String(aSStack_e8);
  Agent::setMission(this_00,pMVar2);
  puVar5 = (undefined4 *)(DAT_000de640 + 0xde576);
  this_00[0x24] = local_60;
  pcVar6 = (code *)*puVar5;
  this_00[0x25] = AStack_61;
  (*pcVar6)(aSStack_b4);
  (*pcVar6)(aSStack_a8);
  (*pcVar6)(aSStack_9c);
  (*pcVar6)(aSStack_90);
  (*pcVar6)(aSStack_84);
  (*pcVar6)(aSStack_78);
  iVar8 = *piVar12 - local_28;
  if (iVar8 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar8);
}

```

## target disasm
```
  000de250: push {r4,r5,r6,r7,lr}
  000de252: add r7,sp,#0xc
  000de254: push {r8,r9,r10,r11}
  000de258: sub sp,#0xec
  000de25a: mov r8,r0
  000de25c: ldr r0,[0x000de608]
  000de25e: mov r11,r1
  000de260: ldr r1,[0x000de60c]
  000de262: add r0,pc
  000de264: mov.w r9,#0x0
  000de268: add r1,pc
  000de26a: ldr.w r10,[r0,#0x0]
  000de26e: add r0,sp,#0xe4
  000de270: ldr r6,[r1,#0x0]
  000de272: ldr.w r1,[r10,#0x0]
  000de276: str r1,[sp,#0xe8]
  000de278: mov r1,r11
  000de27a: str.w r9,[sp,#0xe4]
  000de27e: blx r6
  000de280: add r0,sp,#0xe0
  000de282: mov r1,r11
  000de284: str.w r9,[sp,#0xe0]
  000de288: blx r6
  000de28a: add r0,sp,#0xdc
  000de28c: mov r1,r11
  000de28e: str.w r9,[sp,#0xdc]
  000de292: blx r6
  000de294: add r0,sp,#0xd8
  000de296: mov r1,r11
  000de298: str.w r9,[sp,#0xd8]
  000de29c: blx r6
  000de29e: add r0,sp,#0xd4
  000de2a0: mov r1,r11
  000de2a2: str.w r9,[sp,#0xd4]
  000de2a6: blx r6
  000de2a8: add r0,sp,#0xd0
  000de2aa: mov r1,r11
  000de2ac: str.w r9,[sp,#0xd0]
  000de2b0: blx r6
  000de2b2: add r0,sp,#0xcc
  000de2b4: mov r1,r11
  000de2b6: str.w r9,[sp,#0xcc]
  000de2ba: blx r6
  000de2bc: add r0,sp,#0xc8
  000de2be: mov r1,r11
  000de2c0: str.w r9,[sp,#0xc8]
  000de2c4: blx r6
  000de2c6: add r0,sp,#0xc4
  000de2c8: mov r1,r11
  000de2ca: str.w r9,[sp,#0xc4]
  000de2ce: blx r6
  000de2d0: add r0,sp,#0xc0
  000de2d2: mov r1,r11
  000de2d4: str.w r9,[sp,#0xc0]
  000de2d8: blx r6
  000de2da: add r0,sp,#0xbc
  000de2dc: mov r1,r11
  000de2de: str.w r9,[sp,#0xbc]
  000de2e2: blx r6
  000de2e4: add r0,sp,#0xb8
  000de2e6: mov r1,r11
  000de2e8: str.w r9,[sp,#0xb8]
  000de2ec: blx r6
  000de2ee: add r0,sp,#0xb4
  000de2f0: mov r1,r11
  000de2f2: str.w r9,[sp,#0xb4]
  000de2f6: blx r6
  000de2f8: ldr r0,[0x000de63c]
  000de2fa: mov r1,r11
  000de2fc: strb.w r9,[r7,#-0x55]
  000de300: add r0,pc
  000de302: ldr r4,[r0,#0x0]
  000de304: sub.w r0,r7,#0x55
  000de308: blx r4
  000de30a: sub.w r0,r7,#0x56
  000de30e: mov r1,r11
  000de310: strb.w r9,[r7,#-0x56]
  000de314: blx r4
  000de316: sub.w r0,r7,#0x57
  000de31a: mov r1,r11
  000de31c: strb.w r9,[r7,#-0x57]
  000de320: blx r4
  000de322: add r0,sp,#0xb0
  000de324: mov r1,r11
  000de326: strb.w r9,[sp,#0xb0]
  000de32a: blx r4
  000de32c: sub.w r0,r7,#0x59
  000de330: mov r1,r11
  000de332: strb.w r9,[r7,#-0x59]
  000de336: blx r4
  000de338: add r0,sp,#0xa8
  000de33a: mov r1,r11
  000de33c: str.w r9,[sp,#0xa8]
  000de340: blx r6
  000de342: ldr r4,[sp,#0xa8]
  000de344: cmp r4,#0x1
  000de346: blt 0x000de37a
  000de348: movs r0,#0x4
  000de34a: umull r0,r1,r4,r0
  000de34e: cmp r1,#0x0
  000de350: it ne
  000de352: mov.ne r1,#0x1
  000de354: cmp r1,#0x0
  000de356: it ne
  000de358: mov.ne.w r0,#0xffffffff
  000de35c: blx 0x0006ec08
  000de360: mov r9,r0
  000de362: movs r5,#0x0
  000de364: mov r6,r0
  000de366: b 0x000de376
  000de368: mov r0,r6
  000de36a: mov r1,r11
  000de36c: blx 0x00074818
  000de370: adds r6,#0x4
  000de372: ldr r4,[sp,#0xa8]
  000de374: adds r5,#0x1
  000de376: cmp r5,r4
  000de378: blt 0x000de368
  000de37a: ldr r0,[sp,#0xd4]
  000de37c: mov.w r1,#0xffffffff
  000de380: str r1,[sp,#0xa4]
  000de382: cmp r0,#0x13
  000de384: blt 0x000de38e
  000de386: add r0,sp,#0xa4
  000de388: mov r1,r11
  000de38a: blx 0x00074818
  000de38e: add r0,sp,#0x98
  000de390: blx 0x0006efbc
  000de394: add r0,sp,#0x8c
  000de396: blx 0x0006efbc
  000de39a: add r0,sp,#0x80
  000de39c: blx 0x0006efbc
  000de3a0: add r0,sp,#0x74
  000de3a2: blx 0x0006efbc
  000de3a6: add r0,sp,#0x68
  000de3a8: blx 0x0006efbc
  000de3ac: add r0,sp,#0x5c
  000de3ae: str.w r10,[sp,#0x24]
  000de3b2: blx 0x0006efbc
  000de3b6: add r0,sp,#0x98
  000de3b8: mov r1,r11
  000de3ba: movs r2,#0x1
  000de3bc: blx 0x00074824
  000de3c0: add r0,sp,#0x8c
  000de3c2: mov r1,r11
  000de3c4: movs r2,#0x1
  000de3c6: blx 0x00074824
  000de3ca: add r0,sp,#0x80
  000de3cc: mov r1,r11
  000de3ce: movs r2,#0x1
  000de3d0: blx 0x00074824
  000de3d4: add r0,sp,#0x74
  000de3d6: mov r1,r11
  000de3d8: movs r2,#0x1
  000de3da: blx 0x00074824
  000de3de: add r0,sp,#0x68
  000de3e0: mov r1,r11
  000de3e2: movs r2,#0x1
  000de3e4: blx 0x00074824
  000de3e8: add.w r10,sp,#0x5c
  000de3ec: mov r1,r11
  000de3ee: movs r2,#0x1
  000de3f0: mov r0,r10
  000de3f2: blx 0x00074824
  000de3f6: movs r0,#0x0
  000de3f8: str r0,[sp,#0x58]
  000de3fa: add r0,sp,#0x58
  000de3fc: mov r1,r11
  000de3fe: blx 0x00074818
  000de402: ldr r0,[sp,#0x58]
  000de404: cmp r0,#0x1
  000de406: blt 0x000de412
  000de408: mov r0,r8
  000de40a: mov r1,r11
  000de40c: blx 0x00074860
  000de410: b 0x000de414
  000de412: movs r0,#0x0
  000de414: str r0,[sp,#0x20]
  000de416: movs r0,#0x98
  000de418: blx 0x0006eb24
  000de41c: ldr.w r8,[sp,#0xd4]
  000de420: mov r11,r0
  000de422: add r0,sp,#0x4c
  000de424: add r1,sp,#0x8c
  000de426: movs r2,#0x0
  000de428: blx 0x0006f028
  000de42c: ldrd r0,r3,[sp,#0xb8]
  000de430: ldr r1,[sp,#0xcc]
  000de432: ldrb.w r2,[r7,#-0x55]
  000de436: ldrd r4,r5,[sp,#0xdc]
  000de43a: ldr r6,[sp,#0xa4]
  000de43c: ldr.w r12,[sp,#0xc4]
  000de440: stm sp,{r0,r1,r2,r5}
  000de444: add r0,sp,#0x10
  000de446: add r2,sp,#0x4c
  000de448: stm r0,{r4,r6,r12}
  000de44c: mov r0,r11
  000de44e: mov r1,r8
  000de450: blx 0x00071b30
  000de454: add r0,sp,#0x4c
  000de456: blx 0x0006ee30
  000de45a: ldr r1,[sp,#0xe4]
  000de45c: mov r0,r11
  000de45e: blx 0x00071bf0
  000de462: add r4,sp,#0x68
  000de464: ldr r1,[sp,#0xd8]
  000de466: mov r0,r11
  000de468: blx 0x000749bc
  000de46c: ldr r1,[sp,#0xd0]
  000de46e: mov r0,r11
  000de470: blx 0x00071b3c
  000de474: ldrd r2,r3,[sp,#0xc0]
  000de478: ldr r1,[sp,#0xc8]
  000de47a: mov r0,r11
  000de47c: blx 0x00071b0c
  000de480: ldr r0,[sp,#0x70]
  000de482: cbz r0,0x000de498
  000de484: movs r0,#0xc
  000de486: blx 0x0006eb24
  000de48a: mov r5,r0
  000de48c: add r1,sp,#0x68
  000de48e: movs r2,#0x0
  000de490: blx 0x0006f028
  000de494: str.w r5,[r11,#0xc]
  000de498: ldr r0,[sp,#0x64]
  000de49a: cbz r0,0x000de4b0
  000de49c: movs r0,#0xc
  000de49e: blx 0x0006eb24
  000de4a2: mov r5,r0
  000de4a4: add r1,sp,#0x5c
  000de4a6: movs r2,#0x0
  000de4a8: blx 0x0006f028
  000de4ac: str.w r5,[r11,#0x10]
  000de4b0: ldr r0,[sp,#0xb4]
  000de4b2: str.w r0,[r11,#0x14]
  000de4b6: movs r0,#0xc
  000de4b8: blx 0x0006eb24
  000de4bc: mov r6,r0
  000de4be: blx 0x0006f628
  000de4c2: ldr r0,[sp,#0xb4]
  000de4c4: mov r1,r6
  000de4c6: blx 0x0006fe08
  000de4ca: mov.w r8,#0x0
  000de4ce: b 0x000de4f2
  000de4d0: movs r0,#0xc
  000de4d2: blx 0x0006eb24
  000de4d6: mov r1,r10
  000de4d8: cmp.w r8,#0x0
  000de4dc: mov r5,r0
  000de4de: it eq
  000de4e0: mov.eq r1,r4
  000de4e2: movs r2,#0x0
  000de4e4: blx 0x0006f028
  000de4e8: ldr r0,[r6,#0x4]
  000de4ea: str.w r5,[r0,r8,lsl #0x2]
  000de4ee: add.w r8,r8,#0x1
  000de4f2: ldr r0,[sp,#0xb4]
  000de4f4: cmp r8,r0
  000de4f6: blt 0x000de4d0
  000de4f8: mov r0,r11
  000de4fa: mov r1,r6
  000de4fc: blx 0x00071be4
  000de500: ldrb.w r1,[r7,#-0x56]
  000de504: mov r0,r11
  000de506: blx 0x000749c8
  000de50a: ldrb.w r1,[r7,#-0x57]
  000de50e: mov r0,r11
  000de510: blx 0x00071b18
  000de514: mov r0,r11
  000de516: mov r1,r9
  000de518: blx 0x00071b54
  000de51c: add r0,sp,#0x40
  000de51e: add r1,sp,#0x98
  000de520: movs r2,#0x0
  000de522: blx 0x0006f028
  000de526: add r1,sp,#0x40
  000de528: mov r0,r11
  000de52a: blx 0x000749d4
  000de52e: add r0,sp,#0x40
  000de530: blx 0x0006ee30
  000de534: add r0,sp,#0x34
  000de536: add r1,sp,#0x80
  000de538: movs r2,#0x0
  000de53a: blx 0x0006f028
  000de53e: add r1,sp,#0x34
  000de540: mov r0,r11
  000de542: blx 0x000749e0
  000de546: add r0,sp,#0x34
  000de548: blx 0x0006ee30
  000de54c: add r0,sp,#0x28
  000de54e: add r1,sp,#0x74
  000de550: movs r2,#0x0
  000de552: blx 0x0006f028
  000de556: add r1,sp,#0x28
  000de558: mov r0,r11
  000de55a: blx 0x000749ec
  000de55e: add r0,sp,#0x28
  000de560: blx 0x0006ee30
  000de564: ldr r1,[sp,#0x20]
  000de566: mov r0,r11
  000de568: blx 0x00071b78
  000de56c: ldr r0,[0x000de640]
  000de56e: ldrb.w r1,[sp,#0xb0]
  000de572: add r0,pc
  000de574: strb.w r1,[r11,#0x24]
  000de578: ldrb.w r1,[r7,#-0x59]
  000de57c: ldr r4,[r0,#0x0]
  000de57e: add r0,sp,#0x5c
  000de580: strb.w r1,[r11,#0x25]
  000de584: blx r4
  000de586: add r0,sp,#0x68
  000de588: blx r4
  000de58a: add r0,sp,#0x74
  000de58c: blx r4
  000de58e: add r0,sp,#0x80
  000de590: blx r4
  000de592: add r0,sp,#0x8c
  000de594: blx r4
  000de596: add r0,sp,#0x98
  000de598: blx r4
  000de59a: ldr r0,[sp,#0xe8]
  000de59c: ldr r1,[sp,#0x24]
  000de59e: ldr r1,[r1,#0x0]
  000de5a0: subs r0,r1,r0
  000de5a2: itttt eq
  000de5a4: mov.eq r0,r11
  000de5a6: add.eq sp,#0xec
  000de5a8: pop.eq.w {r8,r9,r10,r11}
  000de5ac: pop.eq {r4,r5,r6,r7,pc}
  000de5ae: blx 0x0006e824
```
