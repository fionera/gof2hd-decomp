# MiningGame::gotCore
elf 0x11ebac body 6
Sig: undefined __thiscall gotCore(MiningGame * this)

## decompile
```c

/* MiningGame::gotCore() */

MiningGame __thiscall MiningGame::gotCore(MiningGame *this)

{
  return this[0x83];
}

```

## target disasm
```
  0012ebac: ldrb.w r0,[r0,#0x83]
  0012ebb0: bx lr
```
