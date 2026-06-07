# PlayerEgo::isInFreeLookMode
elf 0xa21e6 body 6
Sig: undefined __thiscall isInFreeLookMode(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isInFreeLookMode() */

undefined1 __thiscall PlayerEgo::isInFreeLookMode(PlayerEgo *this)

{
  return *(undefined1 *)&this[5].field_10;
}

```

## target disasm
```
  000b21e6: ldrb.w r0,[r0,#0xc4]
  000b21ea: bx lr
```
