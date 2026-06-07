# MenuTouchWindow::callDlcMenu
elf 0x12d064 body 98
Sig: undefined __thiscall callDlcMenu(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::callDlcMenu() */

void __thiscall MenuTouchWindow::callDlcMenu(MenuTouchWindow *this)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  *(undefined1 *)(iVar1 + 0x4c) = 0;
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  piVar5 = *(int **)(DAT_0013d0c8 + 0x13d08c);
  *(undefined1 *)(iVar1 + 0x3d) = 1;
  uVar4 = *(undefined4 *)(this + 0x104);
  this[0x170] = (MenuTouchWindow)0x1;
  this[0x17a] = (MenuTouchWindow)0x1;
  uVar2 = GameText::getText(*piVar5);
  uVar3 = GameText::getText(*piVar5);
  (*(code *)(DAT_001ac424 + 0x1ac428))(uVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  0013d064: push {r4,r5,r6,r7,lr}
  0013d066: add r7,sp,#0xc
  0013d068: push.w r11
  0013d06c: mov r4,r0
  0013d06e: ldr r0,[0x0013d0c4]
  0013d070: add r0,pc
  0013d072: ldr r5,[r0,#0x0]
  0013d074: ldr r0,[r5,#0x0]
  0013d076: blx 0x00071704
  0013d07a: movs r1,#0x0
  0013d07c: strb.w r1,[r0,#0x4c]
  0013d080: ldr r0,[r5,#0x0]
  0013d082: blx 0x00071704
  0013d086: ldr r1,[0x0013d0c8]
  0013d088: add r1,pc
  0013d08a: ldr r6,[r1,#0x0]
  0013d08c: movs r1,#0x1
  0013d08e: strb.w r1,[r0,#0x3d]
  0013d092: ldr.w r5,[r4,#0x104]
  0013d096: strb.w r1,[r4,#0x170]
  0013d09a: strb.w r1,[r4,#0x17a]
  0013d09e: movs r1,#0x47
  0013d0a0: ldr r0,[r6,#0x0]
  0013d0a2: blx 0x00072f70
  0013d0a6: mov r4,r0
  0013d0a8: ldr r0,[r6,#0x0]
  0013d0aa: movw r1,#0x1a9
  0013d0ae: blx 0x00072f70
  0013d0b2: mov r2,r0
  0013d0b4: mov r0,r5
  0013d0b6: mov r1,r4
  0013d0b8: pop.w r11
  0013d0bc: pop.w {r4,r5,r6,r7,lr}
  0013d0c0: b.w 0x001ac418
  001ac418: bx pc
```
