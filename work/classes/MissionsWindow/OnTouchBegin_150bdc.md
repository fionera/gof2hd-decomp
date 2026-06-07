# MissionsWindow::OnTouchBegin
elf 0x150bdc body 198
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* MissionsWindow::OnTouchBegin(int, int) */

undefined4 MissionsWindow::OnTouchBegin(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  if (*(int *)(param_1 + 0x40) != 1) {
    if (*(char *)(param_1 + 0x20) == '\0') {
      if (*(char *)(param_1 + 0x22) == '\0') {
        iVar1 = Status::wantedBoardAccessible();
        if (iVar1 != 0) {
          for (uVar3 = 0; uVar3 < **(uint **)(param_1 + 0x14); uVar3 = uVar3 + 1) {
            TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0x14))[1] + uVar3 * 4),param_2)
            ;
          }
        }
        Layout::OnTouchBegin(**(int **)(DAT_00160ca4 + 0x160c5a),param_2);
        ScrollTouchWindow::OnTouchBegin(*(int *)param_1,param_2);
        ScrollTouchWindow::OnTouchBegin(*(int *)(param_1 + 4),param_2);
        if (*(int *)(param_1 + 0x24) != 0) {
          TouchButton::OnTouchBegin(*(int *)(param_1 + 0x24),param_2);
        }
        if (*(int *)(param_1 + 0x2c) != 0) {
          TouchButton::OnTouchBegin(*(int *)(param_1 + 0x2c),param_2);
        }
        if (*(int *)(param_1 + 0x28) != 0) {
          TouchButton::OnTouchBegin(*(int *)(param_1 + 0x28),param_2);
        }
      }
      else {
        StarMap::OnTouchBegin(*(int *)(param_1 + 8),param_2);
      }
    }
    else {
      ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0xc),param_2);
    }
    return 0;
  }
  uVar2 = (*(code *)(DAT_001ac524 + 0x1ac528))(*(undefined4 *)(param_1 + 0x10),param_2);
  return uVar2;
}

```

## target disasm
```
  00160bdc: push {r4,r5,r6,r7,lr}
  00160bde: add r7,sp,#0xc
  00160be0: push.w r8
  00160be4: mov r6,r0
  00160be6: ldr r0,[r0,#0x40]
  00160be8: mov r8,r2
  00160bea: mov r5,r1
  00160bec: cmp r0,#0x1
  00160bee: bne 0x00160c02
  00160bf0: ldr r0,[r6,#0x10]
  00160bf2: mov r1,r5
  00160bf4: mov r2,r8
  00160bf6: pop.w r8
  00160bfa: pop.w {r4,r5,r6,r7,lr}
  00160bfe: b.w 0x001ac518
  00160c02: ldrb.w r0,[r6,#0x20]
  00160c06: cbz r0,0x00160c14
  00160c08: ldr r0,[r6,#0xc]
  00160c0a: mov r1,r5
  00160c0c: mov r2,r8
  00160c0e: blx 0x0007471c
  00160c12: b 0x00160c98
  00160c14: ldrb.w r0,[r6,#0x22]
  00160c18: cbz r0,0x00160c26
  00160c1a: ldr r0,[r6,#0x8]
  00160c1c: mov r1,r5
  00160c1e: mov r2,r8
  00160c20: blx 0x00074704
  00160c24: b 0x00160c98
  00160c26: ldr r0,[0x00160ca0]
  00160c28: add r0,pc
  00160c2a: ldr r0,[r0,#0x0]
  00160c2c: ldr r0,[r0,#0x0]
  00160c2e: blx 0x00077290
  00160c32: cbz r0,0x00160c50
  00160c34: movs r4,#0x0
  00160c36: b 0x00160c48
  00160c38: ldr r0,[r0,#0x4]
  00160c3a: mov r1,r5
  00160c3c: mov r2,r8
  00160c3e: ldr.w r0,[r0,r4,lsl #0x2]
  00160c42: blx 0x00074734
  00160c46: adds r4,#0x1
  00160c48: ldr r0,[r6,#0x14]
  00160c4a: ldr r1,[r0,#0x0]
  00160c4c: cmp r4,r1
  00160c4e: bcc 0x00160c38
  00160c50: ldr r0,[0x00160ca4]
  00160c52: mov r1,r5
  00160c54: mov r2,r8
  00160c56: add r0,pc
  00160c58: ldr r0,[r0,#0x0]
  00160c5a: ldr r0,[r0,#0x0]
  00160c5c: blx 0x00074728
  00160c60: ldr r0,[r6,#0x0]
  00160c62: mov r1,r5
  00160c64: mov r2,r8
  00160c66: blx 0x00075ba4
  00160c6a: ldr r0,[r6,#0x4]
  00160c6c: mov r1,r5
  00160c6e: mov r2,r8
  00160c70: blx 0x00075ba4
  00160c74: ldr r0,[r6,#0x24]
  00160c76: cbz r0,0x00160c80
  00160c78: mov r1,r5
  00160c7a: mov r2,r8
  00160c7c: blx 0x00074734
  00160c80: ldr r0,[r6,#0x2c]
  00160c82: cbz r0,0x00160c8c
  00160c84: mov r1,r5
  00160c86: mov r2,r8
  00160c88: blx 0x00074734
  00160c8c: ldr r0,[r6,#0x28]
  00160c8e: cbz r0,0x00160c98
  00160c90: mov r1,r5
  00160c92: mov r2,r8
  00160c94: blx 0x00074734
  00160c98: movs r0,#0x0
  00160c9a: pop.w r8
  00160c9e: pop {r4,r5,r6,r7,pc}
  001ac518: bx pc
```
