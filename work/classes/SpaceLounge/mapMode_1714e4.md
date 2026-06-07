# SpaceLounge::mapMode
elf 0x1714e4 body 6
Sig: undefined __thiscall mapMode(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::mapMode() */

SpaceLounge __thiscall SpaceLounge::mapMode(SpaceLounge *this)

{
  return this[0x34];
}

```

## target disasm
```
  001814e4: ldrb.w r0,[r0,#0x34]
  001814e8: bx lr
```
