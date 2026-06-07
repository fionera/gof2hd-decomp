# Hud::setHackingGameActive
elf 0x1636de body 6
Sig: undefined __thiscall setHackingGameActive(Hud * this, bool param_1)

## decompile
```c

/* Hud::setHackingGameActive(bool) */

void __thiscall Hud::setHackingGameActive(Hud *this,bool param_1)

{
  this[0x528] = (Hud)param_1;
  return;
}

```

## target disasm
```
  001736de: strb.w r1,[r0,#0x528]
  001736e2: bx lr
```
