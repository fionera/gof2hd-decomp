# MissionsWindow::OnTouchMove
elf 0x150ca8 body 198
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* MissionsWindow::OnTouchMove(int, int) */

undefined4 MissionsWindow::OnTouchMove(int param_1,int param_2)

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
            TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0x14))[1] + uVar3 * 4),param_2);
          }
        }
        Layout::OnTouchMove(**(int **)(DAT_00160d70 + 0x160d26),param_2);
        ScrollTouchWindow::OnTouchMove(*(int *)param_1,param_2);
        ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 4),param_2);
        if (*(int *)(param_1 + 0x24) != 0) {
          TouchButton::OnTouchMove(*(int *)(param_1 + 0x24),param_2);
        }
        if (*(int *)(param_1 + 0x2c) != 0) {
          TouchButton::OnTouchMove(*(int *)(param_1 + 0x2c),param_2);
        }
        if (*(int *)(param_1 + 0x28) != 0) {
          TouchButton::OnTouchMove(*(int *)(param_1 + 0x28),param_2);
        }
      }
      else {
        StarMap::OnTouchMove(*(int *)(param_1 + 8),param_2);
      }
    }
    else {
      ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0xc),param_2);
    }
    return 0;
  }
                    /* WARNING: Could not recover jumptable at 0x001ac530. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar2 = (*(code *)(&UNK_001ac538 + DAT_001ac534))(*(undefined4 *)(param_1 + 0x10),param_2);
  return uVar2;
}

```

## target disasm
```
  00160ca8: push {r4,r5,r6,r7,lr}
  00160caa: add r7,sp,#0xc
  00160cac: push.w r8
  00160cb0: mov r6,r0
  00160cb2: ldr r0,[r0,#0x40]
  00160cb4: mov r8,r2
  00160cb6: mov r5,r1
  00160cb8: cmp r0,#0x1
  00160cba: bne 0x00160cce
  00160cbc: ldr r0,[r6,#0x10]
  00160cbe: mov r1,r5
  00160cc0: mov r2,r8
  00160cc2: pop.w r8
  00160cc6: pop.w {r4,r5,r6,r7,lr}
  00160cca: b.w 0x001ac528
  00160cce: ldrb.w r0,[r6,#0x20]
  00160cd2: cbz r0,0x00160ce0
  00160cd4: ldr r0,[r6,#0xc]
  00160cd6: mov r1,r5
  00160cd8: mov r2,r8
  00160cda: blx 0x0007474c
  00160cde: b 0x00160d64
  00160ce0: ldrb.w r0,[r6,#0x22]
  00160ce4: cbz r0,0x00160cf2
  00160ce6: ldr r0,[r6,#0x8]
  00160ce8: mov r1,r5
  00160cea: mov r2,r8
  00160cec: blx 0x000750e8
  00160cf0: b 0x00160d64
  00160cf2: ldr r0,[0x00160d6c]
  00160cf4: add r0,pc
  00160cf6: ldr r0,[r0,#0x0]
  00160cf8: ldr r0,[r0,#0x0]
  00160cfa: blx 0x00077290
  00160cfe: cbz r0,0x00160d1c
  00160d00: movs r4,#0x0
  00160d02: b 0x00160d14
  00160d04: ldr r0,[r0,#0x4]
  00160d06: mov r1,r5
  00160d08: mov r2,r8
  00160d0a: ldr.w r0,[r0,r4,lsl #0x2]
  00160d0e: blx 0x00074764
  00160d12: adds r4,#0x1
  00160d14: ldr r0,[r6,#0x14]
  00160d16: ldr r1,[r0,#0x0]
  00160d18: cmp r4,r1
  00160d1a: bcc 0x00160d04
  00160d1c: ldr r0,[0x00160d70]
  00160d1e: mov r1,r5
  00160d20: mov r2,r8
  00160d22: add r0,pc
  00160d24: ldr r0,[r0,#0x0]
  00160d26: ldr r0,[r0,#0x0]
  00160d28: blx 0x00074758
  00160d2c: ldr r0,[r6,#0x0]
  00160d2e: mov r1,r5
  00160d30: mov r2,r8
  00160d32: blx 0x00075bbc
  00160d36: ldr r0,[r6,#0x4]
  00160d38: mov r1,r5
  00160d3a: mov r2,r8
  00160d3c: blx 0x00075bbc
  00160d40: ldr r0,[r6,#0x24]
  00160d42: cbz r0,0x00160d4c
  00160d44: mov r1,r5
  00160d46: mov r2,r8
  00160d48: blx 0x00074764
  00160d4c: ldr r0,[r6,#0x2c]
  00160d4e: cbz r0,0x00160d58
  00160d50: mov r1,r5
  00160d52: mov r2,r8
  00160d54: blx 0x00074764
  00160d58: ldr r0,[r6,#0x28]
  00160d5a: cbz r0,0x00160d64
  00160d5c: mov r1,r5
  00160d5e: mov r2,r8
  00160d60: blx 0x00074764
  00160d64: movs r0,#0x0
  00160d66: pop.w r8
  00160d6a: pop {r4,r5,r6,r7,pc}
  001ac528: bx pc
```
