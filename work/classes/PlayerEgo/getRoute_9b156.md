# PlayerEgo::getRoute
elf 0x9b156 body 6
Sig: undefined __thiscall getRoute(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getRoute() */

int __thiscall PlayerEgo::getRoute(PlayerEgo *this)

{
  return this[7].m_pPlayer;
}

```

## target disasm
```
  000ab156: ldr.w r0,[r0,#0xfc]
  000ab15a: bx lr
```
