# Item::setBlueprintAmount
elf 0xe0198 body 4
Sig: undefined __thiscall setBlueprintAmount(Item * this, int param_1)

## decompile
```c

/* Item::setBlueprintAmount(int) */

void __thiscall Item::setBlueprintAmount(Item *this,int param_1)

{
  *(int *)(this + 0x3c) = param_1;
  return;
}

```
## target disasm
```
  000f0198: str r1,[r0,#0x3c]
  000f019a: bx lr
```
