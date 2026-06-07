# StatusWindow::OnTouchMove
elf 0x15a0bc body 242
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* StatusWindow::OnTouchMove(int, int) */

undefined4 StatusWindow::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int in_r2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  undefined4 *puVar6;
  
  piVar4 = *(int **)(DAT_0016a1b0 + 0x16a0d0);
  iVar1 = *piVar4;
  if (((*(int *)(iVar1 + 0xc) < in_r2) &&
      (in_r2 < **(int **)(DAT_0016a1b4 + 0x16a0e0) - *(int *)(iVar1 + 0x10))) ||
     (**(int **)(DAT_0016a1b8 + 0x16a0ee) != 0)) {
    iVar3 = in_r2 - *(int *)(param_1 + 0x3c);
    *(int *)(param_1 + 0x44) = iVar3;
    *(undefined4 *)(param_1 + 0x48) = 0x3f800000;
    *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + iVar3;
    *(int *)(param_1 + 0x3c) = in_r2;
    if (-1 < *(int *)(param_1 + 0x34)) {
      iVar3 = *(int *)(param_1 + 0x50) - in_r2;
      if (iVar3 < 0) {
        iVar3 = -iVar3;
      }
      if (3 < iVar3) {
        TouchButton::setAlwaysPressed
                  (*(TouchButton **)
                    (*(int *)(*(int *)(param_1 + 8) + 4) + *(int *)(param_1 + 0x34) * 4),false);
        *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
        iVar1 = *piVar4;
      }
    }
  }
  Layout::OnTouchMove(iVar1,param_2);
  if (**(char **)(DAT_0016a1bc + 0x16a140) == '\0') {
    for (uVar5 = 0; uVar5 < **(uint **)(param_1 + 4); uVar5 = uVar5 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 4))[1] + uVar5 * 4),param_2);
    }
  }
  if (*(int *)(param_1 + 0x30) == 1) {
    puVar6 = *(undefined4 **)(DAT_0016a1c0 + 0x16a16c);
    iVar1 = Achievements::getMedals((Achievements *)*puVar6);
    for (iVar3 = 0; iVar3 < *(int *)param_1; iVar3 = iVar3 + 1) {
      if ((*(int *)(iVar1 + iVar3 * 4) != 0) ||
         (iVar2 = Achievements::isEliteMedal((Achievements *)*puVar6,iVar3), iVar2 != 0)) {
        TouchButton::OnTouchMove(*(int *)(*(int *)(*(int *)(param_1 + 8) + 4) + iVar3 * 4),param_2);
      }
    }
  }
  return 0;
}

```

## target disasm
```
  0016a0bc: push {r4,r5,r6,r7,lr}
  0016a0be: add r7,sp,#0xc
  0016a0c0: push {r8,r9,r10}
  0016a0c4: mov r6,r0
  0016a0c6: ldr r0,[0x0016a1b0]
  0016a0c8: mov r8,r1
  0016a0ca: mov r9,r2
  0016a0cc: add r0,pc
  0016a0ce: ldr r4,[r0,#0x0]
  0016a0d0: ldr r0,[r4,#0x0]
  0016a0d2: ldr r1,[r0,#0xc]
  0016a0d4: cmp r1,r2
  0016a0d6: bge 0x0016a0e8
  0016a0d8: ldr r1,[0x0016a1b4]
  0016a0da: ldr r2,[r0,#0x10]
  0016a0dc: add r1,pc
  0016a0de: ldr r1,[r1,#0x0]
  0016a0e0: ldr r1,[r1,#0x0]
  0016a0e2: subs r1,r1,r2
  0016a0e4: cmp r1,r9
  0016a0e6: bgt 0x0016a0f2
  0016a0e8: ldr r1,[0x0016a1b8]
  0016a0ea: add r1,pc
  0016a0ec: ldr r1,[r1,#0x0]
  0016a0ee: ldr r1,[r1,#0x0]
  0016a0f0: cbz r1,0x0016a132
  0016a0f2: add.w r3,r6,#0x34
  0016a0f6: mov.w r5,#0x3f800000
  0016a0fa: ldmia r3,{r1,r2,r3}
  0016a0fc: cmp r1,#0x0
  0016a0fe: sub.w r3,r9,r3
  0016a102: strd r3,r5,[r6,#0x44]
  0016a106: add r2,r3
  0016a108: strd r2,r9,[r6,#0x38]
  0016a10c: blt 0x0016a132
  0016a10e: ldr r2,[r6,#0x50]
  0016a110: subs.w r2,r2,r9
  0016a114: it mi
  0016a116: rsb.mi r2,r2
  0016a118: cmp r2,#0x4
  0016a11a: blt 0x0016a132
  0016a11c: ldr r0,[r6,#0x8]
  0016a11e: ldr r0,[r0,#0x4]
  0016a120: ldr.w r0,[r0,r1,lsl #0x2]
  0016a124: movs r1,#0x0
  0016a126: blx 0x00075acc
  0016a12a: mov.w r0,#0xffffffff
  0016a12e: str r0,[r6,#0x34]
  0016a130: ldr r0,[r4,#0x0]
  0016a132: mov r1,r8
  0016a134: mov r2,r9
  0016a136: blx 0x00074758
  0016a13a: ldr r0,[0x0016a1bc]
  0016a13c: add r0,pc
  0016a13e: ldr r0,[r0,#0x0]
  0016a140: ldrb r0,[r0,#0x0]
  0016a142: cbnz r0,0x0016a160
  0016a144: movs r4,#0x0
  0016a146: b 0x0016a158
  0016a148: ldr r0,[r0,#0x4]
  0016a14a: mov r1,r8
  0016a14c: mov r2,r9
  0016a14e: ldr.w r0,[r0,r4,lsl #0x2]
  0016a152: blx 0x00074764
  0016a156: adds r4,#0x1
  0016a158: ldr r0,[r6,#0x4]
  0016a15a: ldr r1,[r0,#0x0]
  0016a15c: cmp r4,r1
  0016a15e: bcc 0x0016a148
  0016a160: ldr r0,[r6,#0x30]
  0016a162: cmp r0,#0x1
  0016a164: bne 0x0016a1a6
  0016a166: ldr r0,[0x0016a1c0]
  0016a168: add r0,pc
  0016a16a: ldr.w r10,[r0,#0x0]
  0016a16e: ldr.w r0,[r10,#0x0]
  0016a172: blx 0x00074ac4
  0016a176: mov r5,r0
  0016a178: movs r4,#0x0
  0016a17a: b 0x0016a1a0
  0016a17c: ldr.w r0,[r5,r4,lsl #0x2]
  0016a180: cbnz r0,0x0016a18e
  0016a182: ldr.w r0,[r10,#0x0]
  0016a186: mov r1,r4
  0016a188: blx 0x000756ac
  0016a18c: cbz r0,0x0016a19e
  0016a18e: ldr r0,[r6,#0x8]
  0016a190: mov r1,r8
  0016a192: mov r2,r9
  0016a194: ldr r0,[r0,#0x4]
  0016a196: ldr.w r0,[r0,r4,lsl #0x2]
  0016a19a: blx 0x00074764
  0016a19e: adds r4,#0x1
  0016a1a0: ldr r0,[r6,#0x0]
  0016a1a2: cmp r4,r0
  0016a1a4: blt 0x0016a17c
  0016a1a6: movs r0,#0x0
  0016a1a8: pop.w {r8,r9,r10}
  0016a1ac: pop {r4,r5,r6,r7,pc}
```
