# Layout::OnTouchMove
elf 0xd5206 body 96
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* Layout::OnTouchMove(int, int) */

undefined4 Layout::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  if (*(char *)(param_1 + 0x3cc) != '\0') {
    TouchButton::OnTouchMove(*(int *)(param_1 + 0x3bc),param_2);
  }
  if ((*(int *)(param_1 + 0x3c4) != 0) && (*(char *)param_1 != '\0')) {
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0x3c4),param_2);
    return 0;
  }
  iVar1 = TouchButton::isVisible(*(TouchButton **)(param_1 + 0x3b4));
  if (iVar1 == 0) {
    uVar2 = *(undefined4 *)(param_1 + 0x3b8);
  }
  else {
    uVar2 = *(undefined4 *)(param_1 + 0x3b4);
  }
  uVar2 = (*(code *)(DAT_001ac0e4 + 0x1ac0e8))(uVar2,param_2);
  return uVar2;
}

```

## target disasm
```
  000e5206: push {r4,r5,r6,r7,lr}
  000e5208: add r7,sp,#0xc
  000e520a: push.w r11
  000e520e: mov r6,r0
  000e5210: ldrb.w r0,[r0,#0x3cc]
  000e5214: mov r4,r2
  000e5216: mov r5,r1
  000e5218: cbz r0,0x000e5226
  000e521a: ldr.w r0,[r6,#0x3bc]
  000e521e: mov r1,r5
  000e5220: mov r2,r4
  000e5222: blx 0x00074764
  000e5226: ldr.w r0,[r6,#0x3c4]
  000e522a: cbz r0,0x000e5240
  000e522c: ldrb r1,[r6,#0x0]
  000e522e: cbz r1,0x000e5240
  000e5230: mov r1,r5
  000e5232: mov r2,r4
  000e5234: blx 0x0007474c
  000e5238: movs r0,#0x0
  000e523a: pop.w r11
  000e523e: pop {r4,r5,r6,r7,pc}
  000e5240: ldr.w r0,[r6,#0x3b4]
  000e5244: blx 0x00074ec0
  000e5248: cmp r0,#0x0
  000e524a: ite ne
  000e524c: ldr.ne.w r0,[r6,#0x3b4]
  000e5250: ldr.eq.w r0,[r6,#0x3b8]
  000e5254: mov r1,r5
  000e5256: mov r2,r4
  000e5258: pop.w r11
  000e525c: pop.w {r4,r5,r6,r7,lr}
  000e5260: b.w 0x001ac0d8
  001ac0d8: bx pc
```
