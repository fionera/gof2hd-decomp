# ChoiceWindow::OnTouchEnd
elf 0x147564 body 96
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* ChoiceWindow::OnTouchEnd(int, int) */

undefined4 ChoiceWindow::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  
  if (*(char *)(param_1 + 0x59) != '\0') {
    if ((*(int *)(param_1 + 0x10) != 0) &&
       (iVar1 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x10),param_2), iVar1 != 0)) {
      return 0;
    }
    if ((*(int *)(param_1 + 0x14) != 0) &&
       (iVar1 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x14),param_2), iVar1 != 0)) {
      return 1;
    }
    if ((*(int *)(param_1 + 0x18) != 0) &&
       (iVar1 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x18),param_2), iVar1 != 0)) {
      return 2;
    }
  }
  if (*(int *)(param_1 + 0x1c) != 0) {
    ScrollTouchWindow::OnTouchEnd(*(int *)(param_1 + 0x1c),param_2);
  }
  return 0xffffffff;
}

```

## target disasm
```
  00157564: push {r4,r5,r6,r7,lr}
  00157566: add r7,sp,#0xc
  00157568: push.w r11
  0015756c: mov r6,r0
  0015756e: ldrb.w r0,[r0,#0x59]
  00157572: mov r4,r2
  00157574: mov r5,r1
  00157576: cbz r0,0x001575ae
  00157578: ldr r0,[r6,#0x10]
  0015757a: cbz r0,0x0015758a
  0015757c: mov r1,r5
  0015757e: mov r2,r4
  00157580: blx 0x00074788
  00157584: cbz r0,0x0015758a
  00157586: movs r0,#0x0
  00157588: b 0x001575be
  0015758a: ldr r0,[r6,#0x14]
  0015758c: cbz r0,0x0015759c
  0015758e: mov r1,r5
  00157590: mov r2,r4
  00157592: blx 0x00074788
  00157596: cbz r0,0x0015759c
  00157598: movs r0,#0x1
  0015759a: b 0x001575be
  0015759c: ldr r0,[r6,#0x18]
  0015759e: cbz r0,0x001575ae
  001575a0: mov r1,r5
  001575a2: mov r2,r4
  001575a4: blx 0x00074788
  001575a8: cbz r0,0x001575ae
  001575aa: movs r0,#0x2
  001575ac: b 0x001575be
  001575ae: ldr r0,[r6,#0x1c]
  001575b0: cbz r0,0x001575ba
  001575b2: mov r1,r5
  001575b4: mov r2,r4
  001575b6: blx 0x00075bc8
  001575ba: mov.w r0,#0xffffffff
  001575be: pop.w r11
  001575c2: pop {r4,r5,r6,r7,pc}
```
