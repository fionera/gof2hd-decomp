# MissionsWindow::update
elf 0x1509b0 body 448
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* MissionsWindow::update(int) */

void MissionsWindow::update(int param_1)

{
  int iVar1;
  String *pSVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  code *pcVar6;
  ScrollTouchWindow *pSVar7;
  uint uVar8;
  int *piVar9;
  undefined4 uVar10;
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  undefined1 auStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar9 = *(int **)(DAT_00160bb8 + 0x1609c4);
  local_24 = *piVar9;
  if (*(int *)(param_1 + 0x40) == 1) {
    if (*piVar9 == local_24) {
      (*(code *)(DAT_001ac514 + 0x1ac518))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(param_1 + 0x10));
  }
  if (*(char *)(param_1 + 0x22) != '\0') {
    if (*piVar9 == local_24) {
      (*(code *)(DAT_001ac284 + 0x1ac288))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(param_1 + 8));
  }
  ScrollTouchWindow::update(*(int *)param_1);
  ScrollTouchWindow::update(*(int *)(param_1 + 4));
  puVar5 = *(undefined4 **)(DAT_00160bbc + 0x160a2a);
  Status::getCampaignMission();
  iVar1 = Mission::getType();
  if (iVar1 != 0xa7) {
    Status::getCampaignMission();
    iVar1 = Mission::getType();
    if (iVar1 != 0xae) goto LAB_00160b38;
  }
  iVar1 = Status::gameWon();
  if ((iVar1 == 0) ||
     (*(char *)(*(int *)(DAT_00160bc0 + 0x160a54) + 0x37) != '\0' ||
      *(char *)(*(int *)(DAT_00160bc0 + 0x160a54) + 0x35) != '\0')) {
    AbyssEngine::String::String(aSStack_30,(char *)(DAT_00160bc4 + 0x160a6c),false);
    iVar1 = Status::getCurrentCampaignMission();
    if (iVar1 < 0xa4) {
      iVar1 = **(int **)(DAT_00160bc8 + 0x160a7e);
      Status::getCurrentCampaignMission();
      pSVar2 = (String *)GameText::getText(iVar1);
      AbyssEngine::String::operator=(aSStack_30,pSVar2);
      uVar10 = *puVar5;
      AbyssEngine::String::String(aSStack_48,aSStack_30,false);
      Status::getCampaignMission();
      iVar1 = Mission::getProductionGoodAmount();
      Status::getCampaignMission();
      iVar3 = Mission::getStatusValue();
      AbyssEngine::String::String(aSStack_54,iVar1 - iVar3);
      uVar4 = AbyssEngine::String::String(aSStack_60,(char *)(DAT_00160bd0 + 0x160ad2),false);
      Status::replaceHash(auStack_3c,uVar10,aSStack_48,aSStack_54,uVar4);
      AbyssEngine::String::operator=(aSStack_30,auStack_3c);
      pcVar6 = *(code **)(DAT_00160bd4 + 0x160af4);
      (*pcVar6)(auStack_3c);
      (*pcVar6)(aSStack_60);
      (*pcVar6)(aSStack_54);
      (*pcVar6)(aSStack_48);
      pSVar7 = *(ScrollTouchWindow **)param_1;
      AbyssEngine::String::String(aSStack_6c,(char *)(DAT_00160bd8 + 0x160b0c),false);
      AbyssEngine::String::String(aSStack_78,aSStack_30,false);
      ScrollTouchWindow::setText(pSVar7,aSStack_6c,aSStack_78);
      AbyssEngine::String::~String(aSStack_78);
      AbyssEngine::String::~String(aSStack_6c);
    }
    AbyssEngine::String::~String(aSStack_30);
  }
LAB_00160b38:
  for (uVar8 = 0; uVar8 < **(uint **)(param_1 + 0x14); uVar8 = uVar8 + 1) {
    TouchButton::setAlwaysPressed
              (*(TouchButton **)((*(uint **)(param_1 + 0x14))[1] + uVar8 * 4),
               uVar8 == *(uint *)(param_1 + 0x40));
  }
  if (*piVar9 == local_24) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001609b0: push {r4,r5,r6,r7,lr}
  001609b2: add r7,sp,#0xc
  001609b4: push {r8,r9,r11}
  001609b8: sub sp,#0x60
  001609ba: mov r4,r0
  001609bc: ldr r0,[0x00160bb8]
  001609be: mov r5,r1
  001609c0: add r0,pc
  001609c2: ldr r6,[r0,#0x0]
  001609c4: ldr r0,[r6,#0x0]
  001609c6: str r0,[sp,#0x5c]
  001609c8: ldr r0,[r4,#0x40]
  001609ca: cmp r0,#0x1
  001609cc: bne 0x001609ee
  001609ce: ldr r0,[r4,#0x10]
  001609d0: ldr r1,[sp,#0x5c]
  001609d2: ldr r2,[r6,#0x0]
  001609d4: subs r1,r2,r1
  001609d6: itttt eq
  001609d8: mov.eq r1,r5
  001609da: add.eq sp,#0x60
  001609dc: pop.eq.w {r8,r9,r11}
  001609e0: pop.eq.w {r4,r5,r6,r7,lr}
  001609e4: it eq
  001609e6: b.eq.w 0x001ac508
  001609ea: blx 0x0006e824
  001609ee: ldrb.w r0,[r4,#0x22]
  001609f2: cbz r0,0x00160a14
  001609f4: ldr r0,[r4,#0x8]
  001609f6: ldr r1,[sp,#0x5c]
  001609f8: ldr r2,[r6,#0x0]
  001609fa: subs r1,r2,r1
  001609fc: itttt eq
  001609fe: mov.eq r1,r5
  00160a00: add.eq sp,#0x60
  00160a02: pop.eq.w {r8,r9,r11}
  00160a06: pop.eq.w {r4,r5,r6,r7,lr}
  00160a0a: it eq
  00160a0c: b.eq.w 0x001ac278
  00160a10: blx 0x0006e824
  00160a14: ldr r0,[r4,#0x0]
  00160a16: mov r1,r5
  00160a18: blx 0x00075b98
  00160a1c: ldr r0,[r4,#0x4]
  00160a1e: mov r1,r5
  00160a20: blx 0x00075b98
  00160a24: ldr r0,[0x00160bbc]
  00160a26: add r0,pc
  00160a28: ldr r5,[r0,#0x0]
  00160a2a: ldr r0,[r5,#0x0]
  00160a2c: blx 0x0007285c
  00160a30: blx 0x00072874
  00160a34: cmp r0,#0xa7
  00160a36: beq 0x00160a46
  00160a38: ldr r0,[r5,#0x0]
  00160a3a: blx 0x0007285c
  00160a3e: blx 0x00072874
  00160a42: cmp r0,#0xae
  00160a44: bne 0x00160b38
  00160a46: ldr r0,[r5,#0x0]
  00160a48: blx 0x00073c48
  00160a4c: cbz r0,0x00160a62
  00160a4e: ldr r0,[0x00160bc0]
  00160a50: add r0,pc
  00160a52: ldr r0,[r0,#0x0]
  00160a54: ldrb.w r1,[r0,#0x35]
  00160a58: ldrb.w r0,[r0,#0x37]
  00160a5c: orrs r0,r1
  00160a5e: lsls r0,r0,#0x18
  00160a60: beq 0x00160b38
  00160a62: ldr r1,[0x00160bc4]
  00160a64: add r0,sp,#0x50
  00160a66: movs r2,#0x0
  00160a68: add r1,pc
  00160a6a: blx 0x0006ee18
  00160a6e: ldr r0,[r5,#0x0]
  00160a70: blx 0x00071770
  00160a74: cmp r0,#0xa3
  00160a76: bgt 0x00160b32
  00160a78: ldr r0,[0x00160bc8]
  00160a7a: add r0,pc
  00160a7c: ldr r0,[r0,#0x0]
  00160a7e: ldr.w r8,[r0,#0x0]
  00160a82: ldr r0,[r5,#0x0]
  00160a84: blx 0x00071770
  00160a88: ldr r1,[0x00160bcc]
  00160a8a: add r1,pc
  00160a8c: ldr.w r1,[r1,r0,lsl #0x2]
  00160a90: mov r0,r8
  00160a92: blx 0x00072f70
  00160a96: mov r1,r0
  00160a98: add r0,sp,#0x50
  00160a9a: blx 0x0006f2b0
  00160a9e: ldr.w r8,[r5,#0x0]
  00160aa2: add r0,sp,#0x38
  00160aa4: add r1,sp,#0x50
  00160aa6: movs r2,#0x0
  00160aa8: blx 0x0006f028
  00160aac: ldr r0,[r5,#0x0]
  00160aae: blx 0x0007285c
  00160ab2: blx 0x00072bb0
  00160ab6: mov r9,r0
  00160ab8: ldr r0,[r5,#0x0]
  00160aba: blx 0x0007285c
  00160abe: blx 0x0007288c
  00160ac2: sub.w r1,r9,r0
  00160ac6: add r0,sp,#0x2c
  00160ac8: blx 0x0006f658
  00160acc: ldr r1,[0x00160bd0]
  00160ace: add r1,pc
  00160ad0: add r0,sp,#0x20
  00160ad2: movs r2,#0x0
  00160ad4: blx 0x0006ee18
  00160ad8: str r0,[sp,#0x0]
  00160ada: add r0,sp,#0x44
  00160adc: add r2,sp,#0x38
  00160ade: add r3,sp,#0x2c
  00160ae0: mov r1,r8
  00160ae2: blx 0x000739b4
  00160ae6: add r0,sp,#0x50
  00160ae8: add r1,sp,#0x44
  00160aea: blx 0x0006f2b0
  00160aee: ldr r0,[0x00160bd4]
  00160af0: add r0,pc
  00160af2: ldr r5,[r0,#0x0]
  00160af4: add r0,sp,#0x44
  00160af6: blx r5
  00160af8: add r0,sp,#0x20
  00160afa: blx r5
  00160afc: add r0,sp,#0x2c
  00160afe: blx r5
  00160b00: add r0,sp,#0x38
  00160b02: blx r5
  00160b04: ldr r5,[r4,#0x0]
  00160b06: ldr r1,[0x00160bd8]
  00160b08: add r1,pc
  00160b0a: add r0,sp,#0x14
  00160b0c: movs r2,#0x0
  00160b0e: blx 0x0006ee18
  00160b12: add r0,sp,#0x8
  00160b14: add r1,sp,#0x50
  00160b16: movs r2,#0x0
  00160b18: blx 0x0006f028
  00160b1c: add r1,sp,#0x14
  00160b1e: add r2,sp,#0x8
  00160b20: mov r0,r5
  00160b22: blx 0x00075b2c
  00160b26: add r0,sp,#0x8
  00160b28: blx 0x0006ee30
  00160b2c: add r0,sp,#0x14
  00160b2e: blx 0x0006ee30
  00160b32: add r0,sp,#0x50
  00160b34: blx 0x0006ee30
  00160b38: movs r5,#0x0
  00160b3a: b 0x00160b52
  00160b3c: ldr r0,[r0,#0x4]
  00160b3e: ldr r1,[r4,#0x40]
  00160b40: subs r1,r5,r1
  00160b42: ldr.w r0,[r0,r5,lsl #0x2]
  00160b46: clz r1,r1
  00160b4a: lsrs r1,r1,#0x5
  00160b4c: blx 0x00075acc
  00160b50: adds r5,#0x1
  00160b52: ldr r0,[r4,#0x14]
  00160b54: ldr r1,[r0,#0x0]
  00160b56: cmp r5,r1
  00160b58: bcc 0x00160b3c
  00160b5a: ldr r0,[sp,#0x5c]
  00160b5c: ldr r1,[r6,#0x0]
  00160b5e: subs r0,r1,r0
  00160b60: ittt eq
  00160b62: add.eq sp,#0x60
  00160b64: pop.eq.w {r8,r9,r11}
  00160b68: pop.eq {r4,r5,r6,r7,pc}
  00160b6a: blx 0x0006e824
  001ac508: bx pc
```
