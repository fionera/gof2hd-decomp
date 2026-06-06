# Item::getMinPrice
elf 0xe0130 body 4
Sig: undefined __thiscall getMinPrice(Item * this)

## decompile
```c

/* Item::getMinPrice() */

undefined4 __thiscall Item::getMinPrice(Item *this)

{
  return *(undefined4 *)(this + 0x20);
}

```
## target disasm
```
  000f0130: ldr r0,[r0,#0x20]
  000f0132: bx lr
```
