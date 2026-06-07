# DialogueWindow::hasSuccessDialogue
elf 0x1671d4 body 26
Sig: undefined __stdcall hasSuccessDialogue(int param_1)

## decompile
```c

/* DialogueWindow::hasSuccessDialogue(int) */

bool DialogueWindow::hasSuccessDialogue(int param_1)

{
  if (0xa1 < param_1) {
    return false;
  }
  return 0 < *(int *)(DAT_001771f0 + 0x1771e4 + param_1 * 4);
}

```

## target disasm
```
  001771d4: mov r1,r0
  001771d6: movs r0,#0x0
  001771d8: cmp r1,#0xa1
  001771da: it gt
  001771dc: bx.gt lr
  001771de: ldr r2,[0x001771f0]
  001771e0: add r2,pc
  001771e2: ldr.w r1,[r2,r1,lsl #0x2]
  001771e6: cmp r1,#0x0
  001771e8: it gt
  001771ea: mov.gt r0,#0x1
  001771ec: bx lr
```
