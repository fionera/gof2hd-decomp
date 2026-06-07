# PlayerFighter::hasMissionCrateCaptured
elf 0xdcb78 body 6
Sig: undefined __thiscall hasMissionCrateCaptured(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::hasMissionCrateCaptured() */

PlayerFighter __thiscall PlayerFighter::hasMissionCrateCaptured(PlayerFighter *this)

{
  return this[0x69];
}

```

## target disasm
```
  000ecb78: ldrb.w r0,[r0,#0x69]
  000ecb7c: bx lr
```
