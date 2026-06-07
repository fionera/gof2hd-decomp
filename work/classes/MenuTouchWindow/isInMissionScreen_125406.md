# MenuTouchWindow::isInMissionScreen
elf 0x125406 body 14
Sig: undefined __stdcall isInMissionScreen(void)

## decompile
```c

/* MenuTouchWindow::isInMissionScreen() */

bool MenuTouchWindow::isInMissionScreen(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x16c) == 9;
}

```

## target disasm
```
  00135406: ldr.w r0,[r0,#0x16c]
  0013540a: subs r0,#0x9
  0013540c: clz r0,r0
  00135410: lsrs r0,r0,#0x5
  00135412: bx lr
```
