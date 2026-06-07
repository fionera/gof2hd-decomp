# Hud::setJumpMapSelected
elf 0x166516 body 6
Sig: undefined __thiscall setJumpMapSelected(Hud * this, bool param_1)

## decompile
```c

/* Hud::setJumpMapSelected(bool) */

void __thiscall Hud::setJumpMapSelected(Hud *this,bool param_1)

{
  this[0x274] = (Hud)param_1;
  return;
}

```

## target disasm
```
  00176516: strb.w r1,[r0,#0x274]
  0017651a: bx lr
```
