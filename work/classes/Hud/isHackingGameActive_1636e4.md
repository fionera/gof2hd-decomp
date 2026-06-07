# Hud::isHackingGameActive
elf 0x1636e4 body 6
Sig: undefined __thiscall isHackingGameActive(Hud * this)

## decompile
```c

/* Hud::isHackingGameActive() */

Hud __thiscall Hud::isHackingGameActive(Hud *this)

{
  return this[0x528];
}

```

## target disasm
```
  001736e4: ldrb.w r0,[r0,#0x528]
  001736e8: bx lr
```
