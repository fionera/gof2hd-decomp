# PlayerEgo::isDockedToStream
elf 0xa167c body 6
Sig: undefined __thiscall isDockedToStream(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isDockedToStream() */

undefined1 __thiscall PlayerEgo::isDockedToStream(PlayerEgo *this)

{
  return *(undefined1 *)((int)&this[0xd].field_18 + 1);
}

```

## target disasm
```
  000b167c: ldrb.w r0,[r0,#0x1ed]
  000b1680: bx lr
```
