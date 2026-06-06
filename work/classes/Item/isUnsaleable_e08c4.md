# Item::isUnsaleable
elf 0xe08c4 body 6
Sig: undefined __thiscall isUnsaleable(Item * this)

## decompile
```c

/* Item::isUnsaleable() */

Item __thiscall Item::isUnsaleable(Item *this)

{
  return this[0x44];
}

```
## target disasm
```
  000f08c4: ldrb.w r0,[r0,#0x44]
  000f08c8: bx lr
```
