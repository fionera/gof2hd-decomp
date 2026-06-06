# Item::setMissingIngredients
elf 0xe0184 body 4
Sig: undefined __thiscall setMissingIngredients(Item * this, int param_1)

## decompile
```c

/* Item::setMissingIngredients(int) */

void __thiscall Item::setMissingIngredients(Item *this,int param_1)

{
  *(int *)(this + 0x40) = param_1;
  return;
}

```
## target disasm
```
  000f0184: str r1,[r0,#0x40]
  000f0186: bx lr
```
