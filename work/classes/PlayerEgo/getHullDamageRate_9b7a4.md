# PlayerEgo::getHullDamageRate
elf 0x9b7a4 body 8
Sig: undefined __thiscall getHullDamageRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getHullDamageRate() */

void __thiscall PlayerEgo::getHullDamageRate(PlayerEgo *this)

{
  (*(code *)(DAT_001abab4 + 0x1abab8))(this->m_pPlayer);
  return;
}

```

## target disasm
```
  000ab7a4: ldr r0,[r0,#0x0]
  000ab7a6: b.w 0x001abaa8
  001abaa8: bx pc
```
