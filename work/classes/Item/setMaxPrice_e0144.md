# Item::setMaxPrice
elf 0xe0144 body 4
Sig: undefined __thiscall setMaxPrice(Item * this, int param_1)

## decompile
```c

/* Item::setMaxPrice(int) */

void __thiscall Item::setMaxPrice(Item *this,int param_1)

{
  *(int *)(this + 0x24) = param_1;
  return;
}

```
## target disasm
```
  000f0144: str r1,[r0,#0x24]
  000f0146: bx lr
```
