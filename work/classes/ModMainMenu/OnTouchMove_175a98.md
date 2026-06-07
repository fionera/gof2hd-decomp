# ModMainMenu::OnTouchMove
elf 0x175a98 body 28
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2, void * param_3)

## decompile
```c

/* ModMainMenu::OnTouchMove(int, int, void*) */

void ModMainMenu::OnTouchMove(int param_1,int param_2,void *param_3)

{
  if (*(char *)(param_1 + 0x28) != '\0') {
    return;
  }
  MenuTouchWindow::OnTouchMove(*(int *)(param_1 + 0x18),param_2,param_3);
  return;
}

```

## target disasm
```
  00185a98: ldrb.w r12,[r0,#0x28]
  00185a9c: cmp.w r12,#0x0
  00185aa0: it ne
  00185aa2: bx.ne lr
  00185aa4: push {r7,lr}
  00185aa6: mov r7,sp
  00185aa8: ldr r0,[r0,#0x18]
  00185aaa: blx 0x00075118
  00185aae: pop.w {r7,lr}
  00185ab2: bx lr
```
