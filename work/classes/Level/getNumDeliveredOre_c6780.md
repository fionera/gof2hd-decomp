# Level::getNumDeliveredOre
elf 0xc6780 body 6
Sig: undefined __thiscall getNumDeliveredOre(Level * this)

## decompile
```c

/* Level::getNumDeliveredOre() */

undefined4 __thiscall Level::getNumDeliveredOre(Level *this)

{
  return *(undefined4 *)(this + 0x290);
}

```
## target disasm
```
  000d6780: ldr.w r0,[r0,#0x290]
  000d6784: bx lr
```
