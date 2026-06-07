# MenuTouchWindow::setSkipButtonVisible
elf 0x12d026 body 62
Sig: undefined __stdcall setSkipButtonVisible(bool param_1)

## decompile
```c

/* MenuTouchWindow::setSkipButtonVisible(bool) */

void MenuTouchWindow::setSkipButtonVisible(bool param_1)

{
  TouchButton *this;
  bool in_r1;
  uint *puVar1;
  uint uVar2;
  
  puVar1 = *(uint **)(param_1 + 4);
  if (puVar1 != (uint *)0x0) {
    for (uVar2 = 0; uVar2 < *puVar1; uVar2 = uVar2 + 1) {
      this = *(TouchButton **)(puVar1[1] + uVar2 * 4);
      if ((this != (TouchButton *)0x0) && (*(int *)this == 0x12 && *(int *)(this + 4) == 0)) {
        TouchButton::setVisible(this,in_r1);
        puVar1 = *(uint **)(param_1 + 4);
      }
    }
  }
  return;
}

```

## target disasm
```
  0013d026: push {r4,r5,r6,r7,lr}
  0013d028: add r7,sp,#0xc
  0013d02a: push.w r11
  0013d02e: mov r4,r1
  0013d030: ldr r1,[r0,#0x4]
  0013d032: mov r5,r0
  0013d034: cbz r1,0x0013d05e
  0013d036: movs r6,#0x0
  0013d038: b 0x0013d058
  0013d03a: ldr r0,[r1,#0x4]
  0013d03c: ldr.w r0,[r0,r6,lsl #0x2]
  0013d040: cbz r0,0x0013d056
  0013d042: ldrd r2,r3,[r0,#0x0]
  0013d046: eor r2,r2,#0x12
  0013d04a: orrs r2,r3
  0013d04c: bne 0x0013d056
  0013d04e: mov r1,r4
  0013d050: blx 0x00074e48
  0013d054: ldr r1,[r5,#0x4]
  0013d056: adds r6,#0x1
  0013d058: ldr r0,[r1,#0x0]
  0013d05a: cmp r6,r0
  0013d05c: bcc 0x0013d03a
  0013d05e: pop.w r11
  0013d062: pop {r4,r5,r6,r7,pc}
```
