# MissionsWindow::hangarNeedsUpdate
elf 0x1509a8 body 6
Sig: undefined __stdcall hangarNeedsUpdate(void)

## decompile
```c

/* MissionsWindow::hangarNeedsUpdate() */

undefined1 MissionsWindow::hangarNeedsUpdate(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x23);
}

```

## target disasm
```
  001609a8: ldrb.w r0,[r0,#0x23]
  001609ac: bx lr
```
