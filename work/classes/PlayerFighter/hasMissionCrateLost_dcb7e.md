# PlayerFighter::hasMissionCrateLost
elf 0xdcb7e body 6
Sig: undefined __thiscall hasMissionCrateLost(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::hasMissionCrateLost() */

PlayerFighter __thiscall PlayerFighter::hasMissionCrateLost(PlayerFighter *this)

{
  return this[0x68];
}

```

## target disasm
```
  000ecb7e: ldrb.w r0,[r0,#0x68]
  000ecb82: bx lr
```
