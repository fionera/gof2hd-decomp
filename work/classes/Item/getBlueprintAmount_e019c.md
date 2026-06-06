# Item::getBlueprintAmount
elf 0xe019c body 4
Sig: undefined __thiscall getBlueprintAmount(Item * this)

## decompile
```c

/* Item::getBlueprintAmount() */

undefined4 __thiscall Item::getBlueprintAmount(Item *this)

{
  return *(undefined4 *)(this + 0x3c);
}

```
## target disasm
```
  000f019c: ldr r0,[r0,#0x3c]
  000f019e: bx lr
```
