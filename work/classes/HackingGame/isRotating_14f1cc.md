# HackingGame::isRotating
elf 0x14f1cc body 22
Sig: undefined __thiscall isRotating(HackingGame * this)

## decompile
```c

/* HackingGame::isRotating() */

bool __thiscall HackingGame::isRotating(HackingGame *this)

{
  if (this[0x128] != (HackingGame)0x0) {
    return true;
  }
  return this[0x129] != (HackingGame)0x0;
}

```

## target disasm
```
  0015f1cc: ldrb.w r1,[r0,#0x128]
  0015f1d0: cbz r1,0x0015f1d6
  0015f1d2: movs r0,#0x1
  0015f1d4: bx lr
  0015f1d6: ldrb.w r0,[r0,#0x129]
  0015f1da: cmp r0,#0x0
  0015f1dc: it ne
  0015f1de: mov.ne r0,#0x1
  0015f1e0: bx lr
```
