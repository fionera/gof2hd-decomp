# Item::getMinPriceSystem
elf 0xe0138 body 4
Sig: undefined __thiscall getMinPriceSystem(Item * this)

## decompile
```c

/* Item::getMinPriceSystem() */

undefined4 __thiscall Item::getMinPriceSystem(Item *this)

{
  return *(undefined4 *)(this + 0x10);
}

```
## target disasm
```
  000f0138: ldr r0,[r0,#0x10]
  000f013a: bx lr
```
