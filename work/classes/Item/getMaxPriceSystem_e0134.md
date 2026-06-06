# Item::getMaxPriceSystem
elf 0xe0134 body 4
Sig: undefined __thiscall getMaxPriceSystem(Item * this)

## decompile
```c

/* Item::getMaxPriceSystem() */

undefined4 __thiscall Item::getMaxPriceSystem(Item *this)

{
  return *(undefined4 *)(this + 0x14);
}

```
## target disasm
```
  000f0134: ldr r0,[r0,#0x14]
  000f0136: bx lr
```
