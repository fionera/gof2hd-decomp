# MenuTouchWindow::isMakingScreenshot
elf 0x12d00e body 16
Sig: undefined __stdcall isMakingScreenshot(void)

## decompile
```c

/* MenuTouchWindow::isMakingScreenshot() */

bool MenuTouchWindow::isMakingScreenshot(void)

{
  int in_r0;
  
  return *(uint *)(in_r0 + 0x184) < 0x80000000;
}

```

## target disasm
```
  0013d00e: ldr.w r1,[r0,#0x184]
  0013d012: movs r0,#0x0
  0013d014: cmp.w r1,#0xffffffff
  0013d018: it gt
  0013d01a: mov.gt r0,#0x1
  0013d01c: bx lr
```
