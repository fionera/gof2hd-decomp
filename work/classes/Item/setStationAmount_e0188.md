# Item::setStationAmount
elf 0xe0188 body 4
Sig: undefined __thiscall setStationAmount(Item * this, int param_1)

## decompile
```c

/* Item::setStationAmount(int) */

void __thiscall Item::setStationAmount(Item *this,int param_1)

{
  *(int *)(this + 0x38) = param_1;
  return;
}

```
## target disasm
```
  000f0188: str r1,[r0,#0x38]
  000f018a: bx lr
```
