# ModMainMenu::OnTouchEnd
elf 0x175ab4 body 50
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2, void * param_3)

## decompile
```c

/* ModMainMenu::OnTouchEnd(int, int, void*) */

void ModMainMenu::OnTouchEnd(int param_1,int param_2,void *param_3)

{
  int iVar1;
  void *in_r3;
  
  iVar1 = DAT_00185ae8;
  if (*(char *)(param_1 + 0x28) == '\0') {
    MenuTouchWindow::OnTouchEnd(*(MenuTouchWindow **)(param_1 + 0x18),param_2,(int)param_3,in_r3);
    Level::getStarSystem();
    (*(code *)(DAT_001ac764 + 0x1ac768))();
    return;
  }
  *(undefined1 *)(param_1 + 0x28) = 0;
  **(undefined4 **)(iVar1 + 0x185acc) = 0;
  return;
}

```

## target disasm
```
  00185ab4: push {r4,r6,r7,lr}
  00185ab6: add r7,sp,#0x8
  00185ab8: mov r4,r0
  00185aba: ldrb.w r0,[r0,#0x28]
  00185abe: cbz r0,0x00185ad0
  00185ac0: ldr r0,[0x00185ae8]
  00185ac2: movs r1,#0x0
  00185ac4: strb.w r1,[r4,#0x28]
  00185ac8: add r0,pc
  00185aca: ldr r0,[r0,#0x0]
  00185acc: str r1,[r0,#0x0]
  00185ace: pop {r4,r6,r7,pc}
  00185ad0: ldr r0,[r4,#0x18]
  00185ad2: blx 0x00075238
  00185ad6: ldr r0,[r4,#0x1c]
  00185ad8: ldr r0,[r0,#0x0]
  00185ada: blx 0x000727c0
  00185ade: pop.w {r4,r6,r7,lr}
  00185ae2: b.w 0x001ac758
```
