# PlayerEgo::getDockTotalAmount
elf 0xa2280 body 6
Sig: undefined __thiscall getDockTotalAmount(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getDockTotalAmount() */

int __thiscall PlayerEgo::getDockTotalAmount(PlayerEgo *this)

{
  return this[0x18].m_pPlayer;
}

```

## target disasm
```
  000b2280: ldr.w r0,[r0,#0x360]
  000b2284: bx lr
```
