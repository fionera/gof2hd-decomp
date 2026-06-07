# ModMainMenu::OnSuspend
elf 0x17582c body 28
Sig: undefined __stdcall OnSuspend(void)

## decompile
```c

/* ModMainMenu::OnSuspend() */

void ModMainMenu::OnSuspend(void)

{
  if (**(int **)(DAT_00185840 + 0x185832) == 0) {
    return;
  }
  (*(code *)(DAT_001ac194 + 0x1ac198))();
  return;
}

```

## target disasm
```
  0018582c: ldr r0,[0x00185840]
  0018582e: add r0,pc
  00185830: ldr r0,[r0,#0x0]
  00185832: ldr r0,[r0,#0x0]
  00185834: cmp r0,#0x0
  00185836: it ne
  00185838: b.ne.w 0x001ac188
  0018583c: bx lr
  001ac188: bx pc
  001ac18c: ldr r12,[0x1ac194]
  001ac190: add pc,r12,pc
```
