# PlayerEgo::collidesWithStation
elf 0xa1106 body 6
Sig: undefined __thiscall collidesWithStation(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::collidesWithStation() */

undefined1 __thiscall PlayerEgo::collidesWithStation(PlayerEgo *this)

{
  return *(undefined1 *)&this[0xf].field_18;
}

```

## target disasm
```
  000b1106: ldrb.w r0,[r0,#0x234]
  000b110a: bx lr
```
