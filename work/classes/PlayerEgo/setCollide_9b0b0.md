# PlayerEgo::setCollide
elf 0x9b0b0 body 6
Sig: undefined __thiscall setCollide(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setCollide(bool) */

void __thiscall PlayerEgo::setCollide(PlayerEgo *this,bool param_1)

{
  *(bool *)&this[9].m_pPlayer = param_1;
  return;
}

```

## target disasm
```
  000ab0b0: strb.w r1,[r0,#0x144]
  000ab0b4: bx lr
```
