# StatusWindow::OnTouchBegin
elf 0x15a010 body 158
Sig: undefined __thiscall OnTouchBegin(StatusWindow * this, int param_1, int param_2)

## decompile
```c

/* StatusWindow::OnTouchBegin(int, int) */

undefined4 __thiscall StatusWindow::OnTouchBegin(StatusWindow *this,int param_1,int param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  puVar1 = (undefined4 *)(DAT_0016a0b0 + 0x16a024);
  *(int *)(this + 0x50) = param_2;
  *(int *)(this + 0x3c) = param_2;
  piVar2 = (int *)*puVar1;
  *(undefined4 *)(this + 0x44) = 0;
  this[0x54] = (StatusWindow)0x1;
  Layout::OnTouchBegin(*piVar2,param_1);
  if (**(char **)(DAT_0016a0b4 + 0x16a040) == '\0') {
    for (uVar5 = 0; uVar5 < **(uint **)(this + 4); uVar5 = uVar5 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(this + 4))[1] + uVar5 * 4),param_1);
    }
  }
  if (*(int *)(this + 0x30) == 1) {
    puVar1 = *(undefined4 **)(DAT_0016a0b8 + 0x16a06c);
    iVar3 = Achievements::getMedals((Achievements *)*puVar1);
    for (iVar6 = 0; iVar6 < *(int *)this; iVar6 = iVar6 + 1) {
      if ((*(int *)(iVar3 + iVar6 * 4) != 0) ||
         (iVar4 = Achievements::isEliteMedal((Achievements *)*puVar1,iVar6), iVar4 != 0)) {
        TouchButton::OnTouchBegin(*(int *)(*(int *)(*(int *)(this + 8) + 4) + iVar6 * 4),param_1);
      }
    }
  }
  return 0;
}

```

## target disasm
```
  0016a010: push {r4,r5,r6,r7,lr}
  0016a012: add r7,sp,#0xc
  0016a014: push {r8,r9,r10}
  0016a018: mov r6,r0
  0016a01a: ldr r0,[0x0016a0b0]
  0016a01c: mov r8,r1
  0016a01e: movs r1,#0x0
  0016a020: add r0,pc
  0016a022: str r2,[r6,#0x50]
  0016a024: str r2,[r6,#0x3c]
  0016a026: mov r9,r2
  0016a028: ldr r0,[r0,#0x0]
  0016a02a: str r1,[r6,#0x44]
  0016a02c: movs r1,#0x1
  0016a02e: strb.w r1,[r6,#0x54]
  0016a032: mov r1,r8
  0016a034: ldr r0,[r0,#0x0]
  0016a036: blx 0x00074728
  0016a03a: ldr r0,[0x0016a0b4]
  0016a03c: add r0,pc
  0016a03e: ldr r0,[r0,#0x0]
  0016a040: ldrb r0,[r0,#0x0]
  0016a042: cbnz r0,0x0016a060
  0016a044: movs r4,#0x0
  0016a046: b 0x0016a058
  0016a048: ldr r0,[r0,#0x4]
  0016a04a: mov r1,r8
  0016a04c: mov r2,r9
  0016a04e: ldr.w r0,[r0,r4,lsl #0x2]
  0016a052: blx 0x00074734
  0016a056: adds r4,#0x1
  0016a058: ldr r0,[r6,#0x4]
  0016a05a: ldr r1,[r0,#0x0]
  0016a05c: cmp r4,r1
  0016a05e: bcc 0x0016a048
  0016a060: ldr r0,[r6,#0x30]
  0016a062: cmp r0,#0x1
  0016a064: bne 0x0016a0a6
  0016a066: ldr r0,[0x0016a0b8]
  0016a068: add r0,pc
  0016a06a: ldr.w r10,[r0,#0x0]
  0016a06e: ldr.w r0,[r10,#0x0]
  0016a072: blx 0x00074ac4
  0016a076: mov r5,r0
  0016a078: movs r4,#0x0
  0016a07a: b 0x0016a0a0
  0016a07c: ldr.w r0,[r5,r4,lsl #0x2]
  0016a080: cbnz r0,0x0016a08e
  0016a082: ldr.w r0,[r10,#0x0]
  0016a086: mov r1,r4
  0016a088: blx 0x000756ac
  0016a08c: cbz r0,0x0016a09e
  0016a08e: ldr r0,[r6,#0x8]
  0016a090: mov r1,r8
  0016a092: mov r2,r9
  0016a094: ldr r0,[r0,#0x4]
  0016a096: ldr.w r0,[r0,r4,lsl #0x2]
  0016a09a: blx 0x00074734
  0016a09e: adds r4,#0x1
  0016a0a0: ldr r0,[r6,#0x0]
  0016a0a2: cmp r4,r0
  0016a0a4: blt 0x0016a07c
  0016a0a6: movs r0,#0x0
  0016a0a8: pop.w {r8,r9,r10}
  0016a0ac: pop {r4,r5,r6,r7,pc}
```
