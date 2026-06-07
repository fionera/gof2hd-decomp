# Mission::isVisible
elf 0x15baba body 6
Sig: undefined __thiscall isVisible(Mission * this)

## decompile
```c

/* Mission::isVisible() */

Mission __thiscall Mission::isVisible(Mission *this)

{
  return this[0x74];
}

```

## target disasm
```
  0016baba: ldrb.w r0,[r0,#0x74]
  0016babe: bx lr
```
