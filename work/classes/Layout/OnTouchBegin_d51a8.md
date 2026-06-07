# Layout::OnTouchBegin
elf 0xd51a8 body 96
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* Layout::OnTouchBegin(int, int) */

undefined4 Layout::OnTouchBegin(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  if (*(char *)(param_1 + 0x3cc) != '\0') {
    TouchButton::OnTouchBegin(*(int *)(param_1 + 0x3bc),param_2);
  }
  if ((*(int *)(param_1 + 0x3c4) != 0) && (*(char *)param_1 != '\0')) {
    ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0x3c4),param_2);
    return 0;
  }
  iVar1 = TouchButton::isVisible(*(TouchButton **)(param_1 + 0x3b4));
  if (iVar1 == 0) {
    uVar2 = *(undefined4 *)(param_1 + 0x3b8);
  }
  else {
    uVar2 = *(undefined4 *)(param_1 + 0x3b4);
  }
  uVar2 = (*(code *)(DAT_001ac0d4 + 0x1ac0d8))(uVar2,param_2);
  return uVar2;
}

```

## target disasm
```
  000e51a8: push {r4,r5,r6,r7,lr}
  000e51aa: add r7,sp,#0xc
  000e51ac: push.w r11
  000e51b0: mov r6,r0
  000e51b2: ldrb.w r0,[r0,#0x3cc]
  000e51b6: mov r4,r2
  000e51b8: mov r5,r1
  000e51ba: cbz r0,0x000e51c8
  000e51bc: ldr.w r0,[r6,#0x3bc]
  000e51c0: mov r1,r5
  000e51c2: mov r2,r4
  000e51c4: blx 0x00074734
  000e51c8: ldr.w r0,[r6,#0x3c4]
  000e51cc: cbz r0,0x000e51e2
  000e51ce: ldrb r1,[r6,#0x0]
  000e51d0: cbz r1,0x000e51e2
  000e51d2: mov r1,r5
  000e51d4: mov r2,r4
  000e51d6: blx 0x0007471c
  000e51da: movs r0,#0x0
  000e51dc: pop.w r11
  000e51e0: pop {r4,r5,r6,r7,pc}
  000e51e2: ldr.w r0,[r6,#0x3b4]
  000e51e6: blx 0x00074ec0
  000e51ea: cmp r0,#0x0
  000e51ec: ite ne
  000e51ee: ldr.ne.w r0,[r6,#0x3b4]
  000e51f2: ldr.eq.w r0,[r6,#0x3b8]
  000e51f6: mov r1,r5
  000e51f8: mov r2,r4
  000e51fa: pop.w r11
  000e51fe: pop.w {r4,r5,r6,r7,lr}
  000e5202: b.w 0x001ac0c8
  001ac0c8: bx pc
```
