# PlayerEgo::getSpeed
elf 0xa227a body 6
Sig: undefined __thiscall getSpeed(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getSpeed() */

int * __thiscall PlayerEgo::getSpeed(PlayerEgo *this)

{
  return this[5].field_4;
}

```

## target disasm
```
  000b227a: ldr.w r0,[r0,#0xb8]
  000b227e: bx lr
```
