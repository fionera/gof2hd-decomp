# PlayerEgo::getShieldDamageRate
elf 0x9b7aa body 8
Sig: undefined __thiscall getShieldDamageRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getShieldDamageRate() */

void __thiscall PlayerEgo::getShieldDamageRate(PlayerEgo *this)

{
  (*(code *)(DAT_001abac4 + 0x1abac8))(this->m_pPlayer);
  return;
}

```

## target disasm
```
  000ab7aa: ldr r0,[r0,#0x0]
  000ab7ac: b.w 0x001abab8
  001abab8: bx pc
```
