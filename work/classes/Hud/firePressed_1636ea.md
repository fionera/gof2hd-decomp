# Hud::firePressed
elf 0x1636ea body 10
Sig: undefined __thiscall firePressed(Hud * this)

## decompile
```c

/* Hud::firePressed() */

uint __thiscall Hud::firePressed(Hud *this)

{
  return ((byte)this[0x284] & 0x1f) >> 4;
}

```

## target disasm
```
  001736ea: ldrb.w r0,[r0,#0x284]
  001736ee: ubfx r0,r0,#0x4,#0x1
  001736f2: bx lr
```
