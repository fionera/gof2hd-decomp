# PlayerStation::setVisible
elf 0x122760 body 6
Sig: undefined __thiscall setVisible(PlayerStation * this, bool param_1)

## decompile
```c

/* PlayerStation::setVisible(bool) */

void __thiscall PlayerStation::setVisible(PlayerStation *this,bool param_1)

{
  this[0xf5] = (PlayerStation)param_1;
  return;
}

```

## target disasm
```
  00132760: strb.w r1,[r0,#0xf5]
  00132764: bx lr
```
