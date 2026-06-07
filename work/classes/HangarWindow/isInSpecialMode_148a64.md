# HangarWindow::isInSpecialMode
elf 0x148a64 body 22
Sig: undefined __thiscall isInSpecialMode(HangarWindow * this)

## decompile
```c

/* HangarWindow::isInSpecialMode() */

bool __thiscall HangarWindow::isInSpecialMode(HangarWindow *this)

{
  if (this[0x89] != (HangarWindow)0x0) {
    return true;
  }
  return this[0x3c] != (HangarWindow)0x0;
}

```

## target disasm
```
  00158a64: ldrb.w r1,[r0,#0x89]
  00158a68: cbz r1,0x00158a6e
  00158a6a: movs r0,#0x1
  00158a6c: bx lr
  00158a6e: ldrb.w r0,[r0,#0x3c]
  00158a72: cmp r0,#0x0
  00158a74: it ne
  00158a76: mov.ne r0,#0x1
  00158a78: bx lr
```
