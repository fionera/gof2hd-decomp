# Item::getMissingIngredients
elf 0xe0180 body 4
Sig: undefined __thiscall getMissingIngredients(Item * this)

## decompile
```c

/* Item::getMissingIngredients() */

undefined4 __thiscall Item::getMissingIngredients(Item *this)

{
  return *(undefined4 *)(this + 0x40);
}

```
## target disasm
```
  000f0180: ldr r0,[r0,#0x40]
  000f0182: bx lr
```
