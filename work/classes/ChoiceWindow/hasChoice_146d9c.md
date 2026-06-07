# ChoiceWindow::hasChoice
elf 0x146d9c body 6
Sig: undefined __thiscall hasChoice(ChoiceWindow * this)

## decompile
```c

/* ChoiceWindow::hasChoice() */

ChoiceWindow __thiscall ChoiceWindow::hasChoice(ChoiceWindow *this)

{
  return this[0x58];
}

```

## target disasm
```
  00156d9c: ldrb.w r0,[r0,#0x58]
  00156da0: bx lr
```
