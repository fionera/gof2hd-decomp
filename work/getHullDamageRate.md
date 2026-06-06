# PlayerEgo::getHullDamageRate

ELF vaddr 0x9b7a4, body 8 bytes
Signature: undefined __thiscall getHullDamageRate(PlayerEgo * this)

## Ghidra decompile
```c

/* PlayerEgo::getHullDamageRate() */

void __thiscall PlayerEgo::getHullDamageRate(PlayerEgo *this)

{
  (*(code *)(DAT_001abab4 + 0x1abab8))(this->m_pPlayer);
  return;
}

```

## Target disassembly (must match)
```
  000ab7a4: ldr r0,[r0,#0x0]
  000ab7a6: b.w 0x001abaa8
  001abaa8: bx pc
```

Verify: tools/try.sh work/src/getHullDamageRate.cpp getHullDamageRate 9b7a4 32
