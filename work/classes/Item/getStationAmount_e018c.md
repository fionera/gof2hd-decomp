# Item::getStationAmount
elf 0xe018c body 4
Sig: undefined __thiscall getStationAmount(Item * this)

## decompile
```c

/* Item::getStationAmount() */

undefined4 __thiscall Item::getStationAmount(Item *this)

{
  return *(undefined4 *)(this + 0x38);
}

```
## target disasm
```
  000f018c: ldr r0,[r0,#0x38]
  000f018e: bx lr
```
