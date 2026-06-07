# PlayerFighter::hasCrateLost
elf 0xdcb94 body 6
Sig: undefined __thiscall hasCrateLost(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::hasCrateLost() */

PlayerFighter __thiscall PlayerFighter::hasCrateLost(PlayerFighter *this)

{
  return this[0x6a];
}

```

## target disasm
```
  000ecb94: ldrb.w r0,[r0,#0x6a]
  000ecb98: bx lr
```
