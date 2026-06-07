# Layout::getHelpButtonOffset
elf 0xd4bd0 body 26
Sig: undefined __stdcall getHelpButtonOffset(void)

## decompile
```c

/* Layout::getHelpButtonOffset() */

int Layout::getHelpButtonOffset(void)

{
  int iVar1;
  
  iVar1 = TouchButton::getWidth();
  return iVar1 - *(int *)(**(int **)(DAT_000e4bec + 0xe4be2) + 0x38);
}

```

## target disasm
```
  000e4bd0: push {r7,lr}
  000e4bd2: mov r7,sp
  000e4bd4: ldr.w r0,[r0,#0x3bc]
  000e4bd8: blx 0x00074da0
  000e4bdc: ldr r1,[0x000e4bec]
  000e4bde: add r1,pc
  000e4be0: ldr r1,[r1,#0x0]
  000e4be2: ldr r1,[r1,#0x0]
  000e4be4: ldr r1,[r1,#0x38]
  000e4be6: subs r0,r0,r1
  000e4be8: pop {r7,pc}
```
