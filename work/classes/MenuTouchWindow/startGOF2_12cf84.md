# MenuTouchWindow::startGOF2
elf 0x12cf84 body 78
Sig: undefined __thiscall startGOF2(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::startGOF2() */

void __thiscall MenuTouchWindow::startGOF2(MenuTouchWindow *this)

{
  int iVar1;
  undefined4 in_r3;
  int *piVar2;
  float fVar3;
  
  Status::resetGame();
  piVar2 = *(int **)(DAT_0013cfd8 + 0x13cf9e);
  iVar1 = *piVar2;
  *(undefined4 *)(*(int *)(DAT_0013cfdc + 0x13cfa4) + 0x2c) = *(undefined4 *)(this + 0x1a4);
  fVar3 = (float)FModSound::stop(iVar1);
  FModSound::play(*piVar2,(Vector *)0x8f,(Vector *)0x0,fVar3);
  (*(code *)(DAT_001ab904 + 0x1ab908))(**(undefined4 **)(DAT_0013cfe0 + 0x13cfc8),2,0,in_r3);
  return;
}

```

## target disasm
```
  0013cf84: push {r2,r3,r4,r5,r7,lr}
  0013cf86: add r7,sp,#0x10
  0013cf88: mov r4,r0
  0013cf8a: ldr r0,[0x0013cfd4]
  0013cf8c: add r0,pc
  0013cf8e: ldr r0,[r0,#0x0]
  0013cf90: ldr r0,[r0,#0x0]
  0013cf92: blx 0x00074d58
  0013cf96: ldr r0,[0x0013cfd8]
  0013cf98: ldr r1,[0x0013cfdc]
  0013cf9a: add r0,pc
  0013cf9c: ldr.w r2,[r4,#0x1a4]
  0013cfa0: add r1,pc
  0013cfa2: ldr r5,[r0,#0x0]
  0013cfa4: ldr r1,[r1,#0x0]
  0013cfa6: ldr r0,[r5,#0x0]
  0013cfa8: str r2,[r1,#0x2c]
  0013cfaa: ldr r1,[r0,#0x0]
  0013cfac: blx 0x000724a8
  0013cfb0: ldr r0,[r5,#0x0]
  0013cfb2: movs r1,#0x0
  0013cfb4: str r1,[sp,#0x0]
  0013cfb6: movs r1,#0x8f
  0013cfb8: movs r2,#0x0
  0013cfba: movs r3,#0x0
  0013cfbc: blx 0x00071548
  0013cfc0: ldr r0,[0x0013cfe0]
  0013cfc2: movs r1,#0x2
  0013cfc4: add r0,pc
  0013cfc6: ldr r0,[r0,#0x0]
  0013cfc8: ldr r0,[r0,#0x0]
  0013cfca: pop.w {r2,r3,r4,r5,r7,lr}
  0013cfce: b.w 0x001ab8f8
```
