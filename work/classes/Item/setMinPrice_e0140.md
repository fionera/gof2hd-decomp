# Item::setMinPrice
elf 0xe0140 body 4
Sig: undefined __thiscall setMinPrice(Item * this, int param_1)

## decompile
```c

/* Item::setMinPrice(int) */

void __thiscall Item::setMinPrice(Item *this,int param_1)

{
  *(int *)(this + 0x20) = param_1;
  return;
}

```
## target disasm
```
  000f0140: str r1,[r0,#0x20]
  000f0142: bx lr
```
