# PlayerFighter::hasCrateCaptured
elf 0xdcb88 body 12
Sig: undefined __thiscall hasCrateCaptured(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::hasCrateCaptured() */

bool __thiscall PlayerFighter::hasCrateCaptured(PlayerFighter *this)

{
  return this[0x4c] == (PlayerFighter)0x0;
}

```

## target disasm
```
  000ecb88: ldrb.w r0,[r0,#0x4c]
  000ecb8c: clz r0,r0
  000ecb90: lsrs r0,r0,#0x5
  000ecb92: bx lr
```
