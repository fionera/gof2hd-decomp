# ChoiceWindow::OnTouchMove
elf 0x147518 body 76
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* ChoiceWindow::OnTouchMove(int, int) */

undefined4 ChoiceWindow::OnTouchMove(int param_1,int param_2)

{
  if (*(char *)(param_1 + 0x59) != '\0') {
    if (*(int *)(param_1 + 0x10) != 0) {
      TouchButton::OnTouchMove(*(int *)(param_1 + 0x10),param_2);
    }
    if (*(int *)(param_1 + 0x14) != 0) {
      TouchButton::OnTouchMove(*(int *)(param_1 + 0x14),param_2);
    }
    if (*(int *)(param_1 + 0x18) != 0) {
      TouchButton::OnTouchMove(*(int *)(param_1 + 0x18),param_2);
    }
  }
  if (*(int *)(param_1 + 0x1c) != 0) {
    ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0x1c),param_2);
  }
  return 0;
}

```

## target disasm
```
  00157518: push {r4,r5,r6,r7,lr}
  0015751a: add r7,sp,#0xc
  0015751c: push.w r11
  00157520: mov r6,r0
  00157522: ldrb.w r0,[r0,#0x59]
  00157526: mov r4,r2
  00157528: mov r5,r1
  0015752a: cbz r0,0x00157550
  0015752c: ldr r0,[r6,#0x10]
  0015752e: cbz r0,0x00157538
  00157530: mov r1,r5
  00157532: mov r2,r4
  00157534: blx 0x00074764
  00157538: ldr r0,[r6,#0x14]
  0015753a: cbz r0,0x00157544
  0015753c: mov r1,r5
  0015753e: mov r2,r4
  00157540: blx 0x00074764
  00157544: ldr r0,[r6,#0x18]
  00157546: cbz r0,0x00157550
  00157548: mov r1,r5
  0015754a: mov r2,r4
  0015754c: blx 0x00074764
  00157550: ldr r0,[r6,#0x1c]
  00157552: cbz r0,0x0015755c
  00157554: mov r1,r5
  00157556: mov r2,r4
  00157558: blx 0x00075bbc
  0015755c: movs r0,#0x0
  0015755e: pop.w r11
  00157562: pop {r4,r5,r6,r7,pc}
```
