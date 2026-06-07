# HackingGame::getDockingIndex
elf 0x14f10a body 6
Sig: undefined __thiscall getDockingIndex(HackingGame * this)

## decompile
```c

/* HackingGame::getDockingIndex() */

undefined4 __thiscall HackingGame::getDockingIndex(HackingGame *this)

{
  return *(undefined4 *)(this + 0x13c);
}

```

## target disasm
```
  0015f10a: ldr.w r0,[r0,#0x13c]
  0015f10e: bx lr
```
