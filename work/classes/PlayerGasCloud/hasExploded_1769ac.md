# PlayerGasCloud::hasExploded
elf 0x1769ac body 6
Sig: undefined __thiscall hasExploded(PlayerGasCloud * this)

## decompile
```c

/* PlayerGasCloud::hasExploded() */

PlayerGasCloud __thiscall PlayerGasCloud::hasExploded(PlayerGasCloud *this)

{
  return this[0x154];
}

```

## target disasm
```
  001869ac: ldrb.w r0,[r0,#0x154]
  001869b0: bx lr
```
