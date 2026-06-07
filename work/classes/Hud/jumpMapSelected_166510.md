# Hud::jumpMapSelected
elf 0x166510 body 6
Sig: undefined __thiscall jumpMapSelected(Hud * this)

## decompile
```c

/* Hud::jumpMapSelected() */

Hud __thiscall Hud::jumpMapSelected(Hud *this)

{
  return this[0x274];
}

```

## target disasm
```
  00176510: ldrb.w r0,[r0,#0x274]
  00176514: bx lr
```
