# ModMainMenu::OnTouchBegin
elf 0x175a7c body 28
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2, void * param_3)

## decompile
```c

/* ModMainMenu::OnTouchBegin(int, int, void*) */

void ModMainMenu::OnTouchBegin(int param_1,int param_2,void *param_3)

{
  if (*(char *)(param_1 + 0x28) != '\0') {
    return;
  }
  MenuTouchWindow::OnTouchBegin(*(int *)(param_1 + 0x18),param_2,param_3);
  return;
}

```

## target disasm
```
  00185a7c: ldrb.w r12,[r0,#0x28]
  00185a80: cmp.w r12,#0x0
  00185a84: it ne
  00185a86: bx.ne lr
  00185a88: push {r7,lr}
  00185a8a: mov r7,sp
  00185a8c: ldr r0,[r0,#0x18]
  00185a8e: blx 0x000750b8
  00185a92: pop.w {r7,lr}
  00185a96: bx lr
```
