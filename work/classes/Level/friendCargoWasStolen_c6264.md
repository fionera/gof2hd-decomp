# Level::friendCargoWasStolen
elf 0xc6264 body 6
Sig: undefined __thiscall friendCargoWasStolen(Level * this)

## decompile
```c

/* Level::friendCargoWasStolen() */

Level __thiscall Level::friendCargoWasStolen(Level *this)

{
  return this[0x13c];
}

```
## target disasm
```
  000d6264: ldrb.w r0,[r0,#0x13c]
  000d6268: bx lr
```
