# PlayerEgo::getHandling
elf 0xa212e body 6
Sig: undefined __thiscall getHandling(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getHandling() */

int * __thiscall PlayerEgo::getHandling(PlayerEgo *this)

{
  return this[9].field_10;
}

```

## target disasm
```
  000b212e: ldr.w r0,[r0,#0x154]
  000b2132: bx lr
```
