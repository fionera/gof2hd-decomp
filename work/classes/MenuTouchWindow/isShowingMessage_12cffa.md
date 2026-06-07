# MenuTouchWindow::isShowingMessage
elf 0x12cffa body 6
Sig: undefined __stdcall isShowingMessage(void)

## decompile
```c

/* MenuTouchWindow::isShowingMessage() */

undefined1 MenuTouchWindow::isShowingMessage(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x170);
}

```

## target disasm
```
  0013cffa: ldrb.w r0,[r0,#0x170]
  0013cffe: bx lr
```
