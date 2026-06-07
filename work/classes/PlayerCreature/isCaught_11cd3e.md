# PlayerCreature::isCaught
elf 0x11cd3e body 6
Sig: undefined __thiscall isCaught(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::isCaught() */

PlayerCreature __thiscall PlayerCreature::isCaught(PlayerCreature *this)

{
  return this[0x12a];
}

```

## target disasm
```
  0012cd3e: ldrb.w r0,[r0,#0x12a]
  0012cd42: bx lr
```
