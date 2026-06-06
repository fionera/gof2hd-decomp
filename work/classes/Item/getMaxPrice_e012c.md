# Item::getMaxPrice
elf 0xe012c body 4
Sig: undefined __thiscall getMaxPrice(Item * this)

## decompile
```c

/* Item::getMaxPrice() */

undefined4 __thiscall Item::getMaxPrice(Item *this)

{
  return *(undefined4 *)(this + 0x24);
}

```
## target disasm
```
  000f012c: ldr r0,[r0,#0x24]
  000f012e: bx lr
```
