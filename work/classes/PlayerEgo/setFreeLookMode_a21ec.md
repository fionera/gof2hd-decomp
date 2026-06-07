# PlayerEgo::setFreeLookMode
elf 0xa21ec body 6
Sig: undefined __thiscall setFreeLookMode(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setFreeLookMode(bool) */

void __thiscall PlayerEgo::setFreeLookMode(PlayerEgo *this,bool param_1)

{
  *(bool *)&this[5].field_10 = param_1;
  return;
}

```

## target disasm
```
  000b21ec: strb.w r1,[r0,#0xc4]
  000b21f0: bx lr
```
