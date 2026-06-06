# Item::getSinglePrice
elf 0xe0120 body 4
Sig: undefined __thiscall getSinglePrice(Item * this)

## decompile
```c

/* Item::getSinglePrice() */

undefined4 __thiscall Item::getSinglePrice(Item *this)

{
  return *(undefined4 *)(this + 0x18);
}

```
## target disasm
```
  000f0120: ldr r0,[r0,#0x18]
  000f0122: bx lr
```
