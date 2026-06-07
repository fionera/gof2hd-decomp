# Hud::playerHit
elf 0x16323e body 14
Sig: undefined __thiscall playerHit(Hud * this)

## decompile
```c

/* Hud::playerHit() */

void __thiscall Hud::playerHit(Hud *this)

{
  *(undefined4 *)(this + 0x46c) = 0;
  this[0x244] = (Hud)0x1;
  return;
}

```

## target disasm
```
  0017323e: movs r1,#0x0
  00173240: str.w r1,[r0,#0x46c]
  00173244: movs r1,#0x1
  00173246: strb.w r1,[r0,#0x244]
  0017324a: bx lr
```
