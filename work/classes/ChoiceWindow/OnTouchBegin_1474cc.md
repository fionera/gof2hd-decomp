# ChoiceWindow::OnTouchBegin
elf 0x1474cc body 76
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* ChoiceWindow::OnTouchBegin(int, int) */

undefined4 ChoiceWindow::OnTouchBegin(int param_1,int param_2)

{
  if (*(char *)(param_1 + 0x59) != '\0') {
    if (*(int *)(param_1 + 0x10) != 0) {
      TouchButton::OnTouchBegin(*(int *)(param_1 + 0x10),param_2);
    }
    if (*(int *)(param_1 + 0x14) != 0) {
      TouchButton::OnTouchBegin(*(int *)(param_1 + 0x14),param_2);
    }
    if (*(int *)(param_1 + 0x18) != 0) {
      TouchButton::OnTouchBegin(*(int *)(param_1 + 0x18),param_2);
    }
  }
  if (*(int *)(param_1 + 0x1c) != 0) {
    ScrollTouchWindow::OnTouchBegin(*(int *)(param_1 + 0x1c),param_2);
  }
  return 0;
}

```

## target disasm
```
  001574cc: push {r4,r5,r6,r7,lr}
  001574ce: add r7,sp,#0xc
  001574d0: push.w r11
  001574d4: mov r6,r0
  001574d6: ldrb.w r0,[r0,#0x59]
  001574da: mov r4,r2
  001574dc: mov r5,r1
  001574de: cbz r0,0x00157504
  001574e0: ldr r0,[r6,#0x10]
  001574e2: cbz r0,0x001574ec
  001574e4: mov r1,r5
  001574e6: mov r2,r4
  001574e8: blx 0x00074734
  001574ec: ldr r0,[r6,#0x14]
  001574ee: cbz r0,0x001574f8
  001574f0: mov r1,r5
  001574f2: mov r2,r4
  001574f4: blx 0x00074734
  001574f8: ldr r0,[r6,#0x18]
  001574fa: cbz r0,0x00157504
  001574fc: mov r1,r5
  001574fe: mov r2,r4
  00157500: blx 0x00074734
  00157504: ldr r0,[r6,#0x1c]
  00157506: cbz r0,0x00157510
  00157508: mov r1,r5
  0015750a: mov r2,r4
  0015750c: blx 0x00075ba4
  00157510: movs r0,#0x0
  00157512: pop.w r11
  00157516: pop {r4,r5,r6,r7,pc}
```
