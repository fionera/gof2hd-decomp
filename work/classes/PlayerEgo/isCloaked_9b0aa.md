# PlayerEgo::isCloaked
elf 0x9b0aa body 6
Sig: undefined __thiscall isCloaked(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isCloaked() */

undefined1 __thiscall PlayerEgo::isCloaked(PlayerEgo *this)

{
  return (char)this[0xb].field_20;
}

```

## target disasm
```
  000ab0aa: ldrb.w r0,[r0,#0x1ac]
  000ab0ae: bx lr
```
