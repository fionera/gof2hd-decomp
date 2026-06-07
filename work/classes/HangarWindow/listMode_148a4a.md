# HangarWindow::listMode
elf 0x148a4a body 26
Sig: undefined __thiscall listMode(HangarWindow * this)

## decompile
```c

/* HangarWindow::listMode() */

bool __thiscall HangarWindow::listMode(HangarWindow *this)

{
  if ((*(int *)(this + 0x58) == 0) && (this[0x89] == (HangarWindow)0x0)) {
    return this[0x3c] == (HangarWindow)0x0;
  }
  return false;
}

```

## target disasm
```
  00158a4a: ldr r1,[r0,#0x58]
  00158a4c: cbnz r1,0x00158a54
  00158a4e: ldrb.w r1,[r0,#0x89]
  00158a52: cbz r1,0x00158a58
  00158a54: movs r0,#0x0
  00158a56: bx lr
  00158a58: ldrb.w r0,[r0,#0x3c]
  00158a5c: clz r0,r0
  00158a60: lsrs r0,r0,#0x5
  00158a62: bx lr
```
