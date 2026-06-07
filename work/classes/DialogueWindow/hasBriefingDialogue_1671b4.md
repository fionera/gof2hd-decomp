# DialogueWindow::hasBriefingDialogue
elf 0x1671b4 body 26
Sig: undefined __stdcall hasBriefingDialogue(int param_1)

## decompile
```c

/* DialogueWindow::hasBriefingDialogue(int) */

bool DialogueWindow::hasBriefingDialogue(int param_1)

{
  if (0xa1 < param_1) {
    return false;
  }
  return 0 < *(int *)(DAT_001771d0 + 0x1771c4 + param_1 * 4);
}

```

## target disasm
```
  001771b4: mov r1,r0
  001771b6: movs r0,#0x0
  001771b8: cmp r1,#0xa1
  001771ba: it gt
  001771bc: bx.gt lr
  001771be: ldr r2,[0x001771d0]
  001771c0: add r2,pc
  001771c2: ldr.w r1,[r2,r1,lsl #0x2]
  001771c6: cmp r1,#0x0
  001771c8: it gt
  001771ca: mov.gt r0,#0x1
  001771cc: bx lr
```
