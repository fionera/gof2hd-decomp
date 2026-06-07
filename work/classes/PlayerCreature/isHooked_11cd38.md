# PlayerCreature::isHooked
elf 0x11cd38 body 6
Sig: undefined __thiscall isHooked(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::isHooked() */

PlayerCreature __thiscall PlayerCreature::isHooked(PlayerCreature *this)

{
  return this[0x129];
}

```

## target disasm
```
  0012cd38: ldrb.w r0,[r0,#0x129]
  0012cd3c: bx lr
```
