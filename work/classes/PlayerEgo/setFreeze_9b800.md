# PlayerEgo::setFreeze
elf 0x9b800 body 6
Sig: undefined __thiscall setFreeze(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setFreeze(bool) */

void __thiscall PlayerEgo::setFreeze(PlayerEgo *this,bool param_1)

{
  *(bool *)&this[1].m_pPlayer = param_1;
  return;
}

```

## target disasm
```
  000ab800: strb.w r1,[r0,#0x24]
  000ab804: bx lr
```
