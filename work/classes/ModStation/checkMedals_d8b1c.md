# ModStation::checkMedals
elf 0xd8b1c body 424
Sig: undefined __stdcall checkMedals(void)

## decompile
```c

/* ModStation::checkMedals() */

void ModStation::checkMedals(void)

{
  int in_r0;
  undefined4 *puVar1;
  int iVar2;
  Array<int*> *this;
  void *pvVar3;
  int iVar4;
  ChoiceWindow *this_00;
  int *piVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  
  Globals::reportLeaderboards();
  if ((*(char *)(in_r0 + 0x6a) == '\0') && (*(char *)(in_r0 + 99) == '\0')) {
    piVar7 = *(int **)(DAT_000e8cdc + 0xe8bb6);
    iVar6 = Status::getCurrentCampaignMission();
    if (iVar6 == 1) {
      addAchievement((ModStation *)0x1,0,1);
    }
    iVar6 = DAT_000e8ce0;
    iVar8 = 0;
    *(undefined4 *)(in_r0 + 0xbc) = 0;
    iVar6 = Achievements::getNewMedals((Achievements *)**(undefined4 **)(iVar6 + 0xe8bd8));
    iVar2 = 0;
    *(undefined4 *)(in_r0 + 0xc4) = 0;
    while (iVar2 != 0x2d) {
      iVar4 = iVar2 * 4;
      iVar2 = iVar2 + 1;
      if (0 < *(int *)(iVar6 + iVar4)) {
        iVar8 = iVar8 + 1;
        *(int *)(in_r0 + 0xc4) = iVar8;
      }
    }
    if (iVar8 < 1) {
      return;
    }
    this = operator_new(0xc);
    Array<int*>::Array(this);
    *(Array<int*> **)(in_r0 + 0xbc) = this;
    ArraySetLength<int*>(*(uint *)(in_r0 + 0xc4),(Array *)this);
    iVar2 = 0;
    *(undefined4 *)(in_r0 + 0xc4) = 0;
    for (iVar8 = 0; iVar8 != 0x2d; iVar8 = iVar8 + 1) {
      if (0 < *(int *)(iVar6 + iVar8 * 4)) {
        pvVar3 = operator_new__(8);
        *(void **)(*(int *)(*(int *)(in_r0 + 0xbc) + 4) + iVar2 * 4) = pvVar3;
        iVar4 = *(int *)(*(int *)(in_r0 + 0xbc) + 4);
        **(int **)(iVar4 + iVar2 * 4) = iVar8;
        *(undefined4 *)(*(int *)(iVar4 + *(int *)(in_r0 + 0xc4) * 4) + 4) =
             *(undefined4 *)(iVar6 + iVar8 * 4);
        iVar2 = *(int *)(in_r0 + 0xc4) + 1;
        *(int *)(in_r0 + 0xc4) = iVar2;
      }
    }
    *(undefined4 *)(in_r0 + 0xc0) = 0;
    *(undefined1 *)(in_r0 + 0x6a) = 1;
    this_00 = operator_new(0x5c);
    ChoiceWindow::ChoiceWindow(this_00);
    *(ChoiceWindow **)(in_r0 + 0x88) = this_00;
    piVar5 = (int *)**(int **)(*(int *)(in_r0 + 0xbc) + 4);
    ChoiceWindow::setMedal(this_00,*piVar5,piVar5[1]);
    iVar6 = Status::hardCoreMode();
    if (iVar6 == 0) {
      Status::changeCredits(*piVar7);
    }
    puVar1 = (undefined4 *)**(int **)(*(int *)(in_r0 + 0xbc) + 4);
  }
  else {
    iVar6 = *(int *)(in_r0 + 0xc0) + 1;
    *(int *)(in_r0 + 0xc0) = iVar6;
    if (*(int *)(in_r0 + 0xc4) <= iVar6) {
      *(undefined1 *)(in_r0 + 0x6a) = 0;
      return;
    }
    piVar7 = *(int **)(*(int *)(*(int *)(in_r0 + 0xbc) + 4) + iVar6 * 4);
    ChoiceWindow::setMedal(*(ChoiceWindow **)(in_r0 + 0x88),*piVar7,piVar7[1]);
    piVar7 = *(int **)(DAT_000e8ce8 + 0xe8b68);
    iVar6 = Status::hardCoreMode();
    if (iVar6 == 0) {
      Status::changeCredits(*piVar7);
    }
    puVar1 = *(undefined4 **)(*(int *)(*(int *)(in_r0 + 0xbc) + 4) + *(int *)(in_r0 + 0xc0) * 4);
  }
  (*(code *)(DAT_001ac164 + 0x1ac168))(puVar1,*puVar1,puVar1[1]);
  return;
}

```

## target disasm
```
  000e8b1c: push {r4,r5,r6,r7,lr}
  000e8b1e: add r7,sp,#0xc
  000e8b20: push {r8,r9,r11}
  000e8b24: mov r9,r0
  000e8b26: ldr r0,[0x000e8cd8]
  000e8b28: add r0,pc
  000e8b2a: ldr r0,[r0,#0x0]
  000e8b2c: ldr r0,[r0,#0x0]
  000e8b2e: blx 0x0007528c
  000e8b32: ldrb.w r0,[r9,#0x6a]
  000e8b36: cbnz r0,0x000e8b3e
  000e8b38: ldrb.w r0,[r9,#0x63]
  000e8b3c: cbz r0,0x000e8bb0
  000e8b3e: ldrd r0,r2,[r9,#0xc0]
  000e8b42: adds r1,r0,#0x1
  000e8b44: str.w r1,[r9,#0xc0]
  000e8b48: cmp r1,r2
  000e8b4a: bge 0x000e8ba4
  000e8b4c: ldr.w r2,[r9,#0xbc]
  000e8b50: ldr.w r0,[r9,#0x88]
  000e8b54: ldr r2,[r2,#0x4]
  000e8b56: ldr.w r2,[r2,r1,lsl #0x2]
  000e8b5a: ldrd r1,r2,[r2,#0x0]
  000e8b5e: blx 0x00075298
  000e8b62: ldr r0,[0x000e8ce8]
  000e8b64: add r0,pc
  000e8b66: ldr r4,[r0,#0x0]
  000e8b68: ldr r0,[r4,#0x0]
  000e8b6a: blx 0x0007192c
  000e8b6e: cbnz r0,0x000e8b8a
  000e8b70: ldrd r0,r1,[r9,#0xbc]
  000e8b74: ldr r0,[r0,#0x4]
  000e8b76: ldr.w r0,[r0,r1,lsl #0x2]
  000e8b7a: ldr r1,[0x000e8cec]
  000e8b7c: add r1,pc
  000e8b7e: ldr r2,[r0,#0x4]
  000e8b80: ldr r0,[r4,#0x0]
  000e8b82: ldr.w r1,[r1,r2,lsl #0x2]
  000e8b86: blx 0x0007171c
  000e8b8a: ldrd r0,r1,[r9,#0xbc]
  000e8b8e: ldr r0,[r0,#0x4]
  000e8b90: ldr.w r0,[r0,r1,lsl #0x2]
  000e8b94: ldr r1,[r0,#0x0]
  000e8b96: ldr r2,[r0,#0x4]
  000e8b98: pop.w {r8,r9,r11}
  000e8b9c: pop.w {r4,r5,r6,r7,lr}
  000e8ba0: b.w 0x001ac158
  000e8ba4: movs r0,#0x0
  000e8ba6: strb.w r0,[r9,#0x6a]
  000e8baa: pop.w {r8,r9,r11}
  000e8bae: pop {r4,r5,r6,r7,pc}
  000e8bb0: ldr r0,[0x000e8cdc]
  000e8bb2: add r0,pc
  000e8bb4: ldr.w r8,[r0,#0x0]
  000e8bb8: ldr.w r0,[r8,#0x0]
  000e8bbc: blx 0x00071770
  000e8bc0: cmp r0,#0x1
  000e8bc2: bne 0x000e8bcc
  000e8bc4: movs r1,#0x0
  000e8bc6: movs r2,#0x1
  000e8bc8: blx 0x000752a4
  000e8bcc: ldr r0,[0x000e8ce0]
  000e8bce: movs r4,#0x0
  000e8bd0: str.w r4,[r9,#0xbc]
  000e8bd4: add r0,pc
  000e8bd6: ldr r0,[r0,#0x0]
  000e8bd8: ldr r0,[r0,#0x0]
  000e8bda: blx 0x000752b0
  000e8bde: mov r5,r0
  000e8be0: movs r0,#0x0
  000e8be2: str.w r4,[r9,#0xc4]
  000e8be6: b 0x000e8bf8
  000e8be8: ldr.w r1,[r5,r0,lsl #0x2]
  000e8bec: adds r0,#0x1
  000e8bee: cmp r1,#0x1
  000e8bf0: itt ge
  000e8bf2: add.ge r4,#0x1
  000e8bf4: str.w.ge r4,[r9,#0xc4]
  000e8bf8: cmp r0,#0x2d
  000e8bfa: bne 0x000e8be8
  000e8bfc: cmp r4,#0x0
  000e8bfe: ble 0x000e8baa
  000e8c00: movs r0,#0xc
  000e8c02: blx 0x0006eb24
  000e8c06: mov r6,r0
  000e8c08: blx 0x000752bc
  000e8c0c: ldr.w r0,[r9,#0xc4]
  000e8c10: mov r1,r6
  000e8c12: str.w r6,[r9,#0xbc]
  000e8c16: blx 0x000752c8
  000e8c1a: movs r4,#0x0
  000e8c1c: movs r6,#0x0
  000e8c1e: str.w r4,[r9,#0xc4]
  000e8c22: b 0x000e8c62
  000e8c24: ldr.w r0,[r5,r6,lsl #0x2]
  000e8c28: cmp r0,#0x1
  000e8c2a: blt 0x000e8c60
  000e8c2c: movs r0,#0x8
  000e8c2e: blx 0x0006ec08
  000e8c32: ldr.w r1,[r9,#0xbc]
  000e8c36: ldr r1,[r1,#0x4]
  000e8c38: str.w r0,[r1,r4,lsl #0x2]
  000e8c3c: ldr.w r0,[r9,#0xbc]
  000e8c40: ldr r0,[r0,#0x4]
  000e8c42: ldr.w r1,[r0,r4,lsl #0x2]
  000e8c46: str r6,[r1,#0x0]
  000e8c48: ldr.w r1,[r9,#0xc4]
  000e8c4c: ldr.w r0,[r0,r1,lsl #0x2]
  000e8c50: ldr.w r1,[r5,r6,lsl #0x2]
  000e8c54: str r1,[r0,#0x4]
  000e8c56: ldr.w r0,[r9,#0xc4]
  000e8c5a: adds r4,r0,#0x1
  000e8c5c: str.w r4,[r9,#0xc4]
  000e8c60: adds r6,#0x1
  000e8c62: cmp r6,#0x2d
  000e8c64: bne 0x000e8c24
  000e8c66: movs r0,#0x0
  000e8c68: str.w r0,[r9,#0xc0]
  000e8c6c: movs r0,#0x1
  000e8c6e: strb.w r0,[r9,#0x6a]
  000e8c72: movs r0,#0x5c
  000e8c74: blx 0x0006eb24
  000e8c78: mov r5,r0
  000e8c7a: blx 0x00074584
  000e8c7e: ldr.w r0,[r9,#0xbc]
  000e8c82: str.w r5,[r9,#0x88]
  000e8c86: ldr r0,[r0,#0x4]
  000e8c88: ldr r0,[r0,#0x0]
  000e8c8a: ldrd r1,r2,[r0,#0x0]
  000e8c8e: mov r0,r5
  000e8c90: blx 0x00075298
  000e8c94: ldr.w r0,[r8,#0x0]
  000e8c98: blx 0x0007192c
  000e8c9c: cbnz r0,0x000e8cb8
  000e8c9e: ldr.w r0,[r9,#0xbc]
  000e8ca2: ldr r0,[r0,#0x4]
  000e8ca4: ldr r0,[r0,#0x0]
  000e8ca6: ldr r1,[0x000e8ce4]
  000e8ca8: ldr r2,[r0,#0x4]
  000e8caa: add r1,pc
  000e8cac: ldr.w r0,[r8,#0x0]
  000e8cb0: ldr.w r1,[r1,r2,lsl #0x2]
  000e8cb4: blx 0x0007171c
  000e8cb8: ldr.w r0,[r9,#0xbc]
  000e8cbc: ldr r0,[r0,#0x4]
  000e8cbe: ldr r0,[r0,#0x0]
  000e8cc0: b 0x000e8b94
  001ac158: bx pc
```
