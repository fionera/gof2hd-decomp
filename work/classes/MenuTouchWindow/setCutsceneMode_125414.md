# MenuTouchWindow::setCutsceneMode
elf 0x125414 body 62
Sig: undefined __stdcall setCutsceneMode(bool param_1)

## decompile
```c

/* MenuTouchWindow::setCutsceneMode(bool) */

void MenuTouchWindow::setCutsceneMode(bool param_1)

{
  TouchButton *this;
  uint *puVar1;
  byte in_r1;
  uint uVar2;
  
  *(byte *)(param_1 + 0x238) = in_r1;
  for (uVar2 = 0; puVar1 = *(uint **)(param_1 + 4), uVar2 < *puVar1; uVar2 = uVar2 + 1) {
    this = *(TouchButton **)(puVar1[1] + uVar2 * 4);
    if (*(int *)this == 0x13 && *(int *)(this + 4) == 0) {
      TouchButton::setVisible(this,(bool)(in_r1 ^ 1));
    }
  }
  return;
}

```

## target disasm
```
  00135414: push {r4,r5,r6,r7,lr}
  00135416: add r7,sp,#0xc
  00135418: push.w r11
  0013541c: eor r5,r1,#0x1
  00135420: mov r4,r0
  00135422: movs r6,#0x0
  00135424: strb.w r1,[r0,#0x238]
  00135428: b 0x00135444
  0013542a: ldr r0,[r0,#0x4]
  0013542c: ldr.w r0,[r0,r6,lsl #0x2]
  00135430: ldrd r1,r2,[r0,#0x0]
  00135434: eor r1,r1,#0x13
  00135438: orrs r1,r2
  0013543a: itt eq
  0013543c: mov.eq r1,r5
  0013543e: blx.eq 0x00074e48
  00135442: adds r6,#0x1
  00135444: ldr r0,[r4,#0x4]
  00135446: ldr r1,[r0,#0x0]
  00135448: cmp r6,r1
  0013544a: bcc 0x0013542a
  0013544c: pop.w r11
  00135450: pop {r4,r5,r6,r7,pc}
```
