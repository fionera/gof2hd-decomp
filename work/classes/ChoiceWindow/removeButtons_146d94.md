# ChoiceWindow::removeButtons
elf 0x146d94 body 8
Sig: undefined __thiscall removeButtons(ChoiceWindow * this)

## decompile
```c

/* ChoiceWindow::removeButtons() */

void __thiscall ChoiceWindow::removeButtons(ChoiceWindow *this)

{
  this[0x59] = (ChoiceWindow)0x0;
  return;
}

```

## target disasm
```
  00156d94: movs r1,#0x0
  00156d96: strb.w r1,[r0,#0x59]
  00156d9a: bx lr
```
