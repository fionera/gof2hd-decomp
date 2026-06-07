# MenuTouchWindow::hideMessage
elf 0x12d01e body 8
Sig: undefined __thiscall hideMessage(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::hideMessage() */

void __thiscall MenuTouchWindow::hideMessage(MenuTouchWindow *this)

{
  this[0x170] = (MenuTouchWindow)0x0;
  return;
}

```

## target disasm
```
  0013d01e: movs r1,#0x0
  0013d020: strb.w r1,[r0,#0x170]
  0013d024: bx lr
```
