# ListItemWindow::shows3DShip
elf 0x1330fc body 6
Sig: undefined __thiscall shows3DShip(ListItemWindow * this)

## decompile
```c

/* ListItemWindow::shows3DShip() */

ListItemWindow __thiscall ListItemWindow::shows3DShip(ListItemWindow *this)

{
  return this[0x54];
}

```

## target disasm
```
  001430fc: ldrb.w r0,[r0,#0x54]
  00143100: bx lr
```
