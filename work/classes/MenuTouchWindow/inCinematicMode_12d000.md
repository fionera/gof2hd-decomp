# MenuTouchWindow::inCinematicMode
elf 0x12d000 body 14
Sig: undefined __stdcall inCinematicMode(void)

## decompile
```c

/* MenuTouchWindow::inCinematicMode() */

bool MenuTouchWindow::inCinematicMode(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x16c) == 0xd;
}

```

## target disasm
```
  0013d000: ldr.w r0,[r0,#0x16c]
  0013d004: subs r0,#0xd
  0013d006: clz r0,r0
  0013d00a: lsrs r0,r0,#0x5
  0013d00c: bx lr
```
