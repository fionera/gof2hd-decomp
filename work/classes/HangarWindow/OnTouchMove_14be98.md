# HangarWindow::OnTouchMove
elf 0x14be98 body 334
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* HangarWindow::OnTouchMove(int, int) */

undefined4 HangarWindow::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  int in_r2;
  int *piVar2;
  int iVar3;
  uint uVar4;
  
  piVar2 = *(int **)(DAT_0015bfe8 + 0x15beac);
  Layout::OnTouchMove(*piVar2,param_2);
  if (*(char *)(param_1 + 0x3c) == '\0') {
    if (*(int *)(param_1 + 0x58) == 1) {
      ListItemWindow::OnTouchMove(*(int *)(param_1 + 0x18),param_2);
    }
    else {
      iVar3 = *piVar2;
      if ((*(int *)(iVar3 + 0xc) < in_r2) &&
         (in_r2 < **(int **)(DAT_0015bfec + 0x15bf34) - *(int *)(iVar3 + 0x10))) {
        iVar3 = in_r2 - *(int *)(param_1 + 0xb8);
        *(int *)(param_1 + 0xc0) = iVar3;
        *(undefined4 *)(param_1 + 0xc4) = 0x3f800000;
        *(int *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + iVar3;
        *(int *)(param_1 + 0xb8) = in_r2;
        iVar3 = TouchButton::isTouched
                          (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x20));
        if (iVar3 == 0) {
          iVar3 = TouchButton::isTouched
                            (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x24));
        }
        else {
          iVar3 = 1;
        }
        iVar1 = in_r2 - *(int *)(param_1 + 0xcc);
        if (iVar1 < 0) {
          iVar1 = -iVar1;
        }
        if ((iVar3 == 0) && (5 < iVar1)) {
          *(undefined4 *)(param_1 + 0x6c) = 0;
          *(undefined4 *)(param_1 + 0x70) = 0;
          for (uVar4 = 0; uVar4 < **(uint **)(param_1 + 0x24); uVar4 = uVar4 + 1) {
            TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0x24))[1] + uVar4 * 4),param_2);
          }
          setSellMode(SUB41(param_1,0));
          *(undefined1 *)(param_1 + 0xd2) = 0;
          *(undefined4 *)(param_1 + 0x68) = 0;
          TouchButton::resetTouch(*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x20));
          TouchButton::resetTouch(*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x24));
        }
      }
      for (uVar4 = 0; uVar4 < **(uint **)(param_1 + 4); uVar4 = uVar4 + 1) {
        TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 4))[1] + uVar4 * 4),param_2);
      }
    }
  }
  else {
    if (*(char *)(param_1 + 0xae) == '\0') {
      if (*(char *)(param_1 + 0xb0) != '\0') {
        for (iVar3 = 0x12; iVar3 != 0x17; iVar3 = iVar3 + 1) {
          TouchButton::OnTouchMove
                    (*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + iVar3 * 4),param_2);
        }
      }
    }
    else {
      for (iVar3 = 0xc; iVar3 != 0x11; iVar3 = iVar3 + 1) {
        TouchButton::OnTouchMove
                  (*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + iVar3 * 4),param_2);
      }
      TouchButton::OnTouchMove(*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x44),param_2);
    }
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0x20),param_2);
  }
  return 0;
}

```

## target disasm
```
  0015be98: push {r4,r5,r6,r7,lr}
  0015be9a: add r7,sp,#0xc
  0015be9c: push {r8,r9,r11}
  0015bea0: mov r6,r0
  0015bea2: ldr r0,[0x0015bfe8]
  0015bea4: mov r4,r2
  0015bea6: mov r9,r1
  0015bea8: add r0,pc
  0015beaa: ldr r5,[r0,#0x0]
  0015beac: ldr r0,[r5,#0x0]
  0015beae: blx 0x00074758
  0015beb2: ldrb.w r0,[r6,#0x3c]
  0015beb6: cbz r0,0x0015bee6
  0015beb8: ldrb.w r0,[r6,#0xae]
  0015bebc: cbz r0,0x0015bef8
  0015bebe: movs r5,#0xc
  0015bec0: b 0x0015bed2
  0015bec2: ldr r0,[r0,#0x4]
  0015bec4: mov r1,r9
  0015bec6: mov r2,r4
  0015bec8: ldr.w r0,[r0,r5,lsl #0x2]
  0015becc: blx 0x00074764
  0015bed0: adds r5,#0x1
  0015bed2: ldr r0,[r6,#0x24]
  0015bed4: cmp r5,#0x11
  0015bed6: bne 0x0015bec2
  0015bed8: ldr r0,[r0,#0x4]
  0015beda: mov r1,r9
  0015bedc: mov r2,r4
  0015bede: ldr r0,[r0,#0x44]
  0015bee0: blx 0x00074764
  0015bee4: b 0x0015bf18
  0015bee6: ldr r0,[r6,#0x58]
  0015bee8: cmp r0,#0x1
  0015beea: bne 0x0015bf24
  0015beec: ldr r0,[r6,#0x18]
  0015beee: mov r1,r9
  0015bef0: mov r2,r4
  0015bef2: blx 0x000770f8
  0015bef6: b 0x0015bfde
  0015bef8: ldrb.w r0,[r6,#0xb0]
  0015befc: cbz r0,0x0015bf18
  0015befe: movs r5,#0x12
  0015bf00: b 0x0015bf14
  0015bf02: ldr r0,[r6,#0x24]
  0015bf04: mov r1,r9
  0015bf06: mov r2,r4
  0015bf08: ldr r0,[r0,#0x4]
  0015bf0a: ldr.w r0,[r0,r5,lsl #0x2]
  0015bf0e: blx 0x00074764
  0015bf12: adds r5,#0x1
  0015bf14: cmp r5,#0x17
  0015bf16: bne 0x0015bf02
  0015bf18: ldr r0,[r6,#0x20]
  0015bf1a: mov r1,r9
  0015bf1c: mov r2,r4
  0015bf1e: blx 0x0007474c
  0015bf22: b 0x0015bfde
  0015bf24: ldr r0,[r5,#0x0]
  0015bf26: ldr r1,[r0,#0xc]
  0015bf28: cmp r1,r4
  0015bf2a: bge 0x0015bfc2
  0015bf2c: ldr r1,[0x0015bfec]
  0015bf2e: ldr r0,[r0,#0x10]
  0015bf30: add r1,pc
  0015bf32: ldr r1,[r1,#0x0]
  0015bf34: ldr r1,[r1,#0x0]
  0015bf36: subs r0,r1,r0
  0015bf38: cmp r0,r4
  0015bf3a: ble 0x0015bfc2
  0015bf3c: ldr r0,[r6,#0x24]
  0015bf3e: mov.w r3,#0x3f800000
  0015bf42: ldrd r1,r2,[r6,#0xb4]
  0015bf46: subs r2,r4,r2
  0015bf48: strd r2,r3,[r6,#0xc0]
  0015bf4c: add r1,r2
  0015bf4e: strd r1,r4,[r6,#0xb4]
  0015bf52: ldr r0,[r0,#0x4]
  0015bf54: ldr r0,[r0,#0x20]
  0015bf56: blx 0x00076f60
  0015bf5a: cbz r0,0x0015bf60
  0015bf5c: movs r0,#0x1
  0015bf5e: b 0x0015bf6a
  0015bf60: ldr r0,[r6,#0x24]
  0015bf62: ldr r0,[r0,#0x4]
  0015bf64: ldr r0,[r0,#0x24]
  0015bf66: blx 0x00076f60
  0015bf6a: ldr.w r1,[r6,#0xcc]
  0015bf6e: subs r1,r4,r1
  0015bf70: it mi
  0015bf72: rsb.mi r1,r1
  0015bf74: cbnz r0,0x0015bfc2
  0015bf76: cmp r1,#0x6
  0015bf78: blt 0x0015bfc2
  0015bf7a: mov.w r8,#0x0
  0015bf7e: strd r8,r8,[r6,#0x6c]
  0015bf82: b 0x0015bf96
  0015bf84: ldr r0,[r0,#0x4]
  0015bf86: mov r1,r9
  0015bf88: mov r2,r4
  0015bf8a: ldr.w r0,[r0,r8,lsl #0x2]
  0015bf8e: blx 0x00074764
  0015bf92: add.w r8,r8,#0x1
  0015bf96: ldr r0,[r6,#0x24]
  0015bf98: ldr r1,[r0,#0x0]
  0015bf9a: cmp r8,r1
  0015bf9c: bcc 0x0015bf84
  0015bf9e: mov r0,r6
  0015bfa0: movs r1,#0x0
  0015bfa2: movs r5,#0x0
  0015bfa4: blx 0x000751cc
  0015bfa8: ldr r0,[r6,#0x24]
  0015bfaa: strb.w r5,[r6,#0xd2]
  0015bfae: str r5,[r6,#0x68]
  0015bfb0: ldr r0,[r0,#0x4]
  0015bfb2: ldr r0,[r0,#0x20]
  0015bfb4: blx 0x00076fb4
  0015bfb8: ldr r0,[r6,#0x24]
  0015bfba: ldr r0,[r0,#0x4]
  0015bfbc: ldr r0,[r0,#0x24]
  0015bfbe: blx 0x00076fb4
  0015bfc2: movs r5,#0x0
  0015bfc4: b 0x0015bfd6
  0015bfc6: ldr r0,[r0,#0x4]
  0015bfc8: mov r1,r9
  0015bfca: mov r2,r4
  0015bfcc: ldr.w r0,[r0,r5,lsl #0x2]
  0015bfd0: blx 0x00074764
  0015bfd4: adds r5,#0x1
  0015bfd6: ldr r0,[r6,#0x4]
  0015bfd8: ldr r1,[r0,#0x0]
  0015bfda: cmp r5,r1
  0015bfdc: bcc 0x0015bfc6
  0015bfde: movs r0,#0x0
  0015bfe0: pop.w {r8,r9,r11}
  0015bfe4: pop {r4,r5,r6,r7,pc}
```
