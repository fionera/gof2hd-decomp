# PlayerEgo::getDockTransferedAmount
elf 0xa2286 body 6
Sig: undefined __thiscall getDockTransferedAmount(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getDockTransferedAmount() */

int * __thiscall PlayerEgo::getDockTransferedAmount(PlayerEgo *this)

{
  return this[0x18].field_4;
}

```

## target disasm
```
  000b2286: ldr.w r0,[r0,#0x364]
  000b228a: bx lr
```
