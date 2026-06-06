# Item::setAmount
elf 0xe016c body 4
Sig: undefined __thiscall setAmount(Item * this, int param_1)

## decompile
```c

/* Item::setAmount(int) */

void __thiscall Item::setAmount(Item *this,int param_1)

{
  *(int *)(this + 0x34) = param_1;
  return;
}

```
## target disasm
```
  000f016c: str r1,[r0,#0x34]
  000f016e: bx lr
```
