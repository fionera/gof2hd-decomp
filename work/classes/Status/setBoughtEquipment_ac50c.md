# Status::setBoughtEquipment
elf 0xac50c body 6
Sig: undefined __thiscall setBoughtEquipment(Status * this, int param_1)

## decompile
```c

/* Status::setBoughtEquipment(int) */

void __thiscall Status::setBoughtEquipment(Status *this,int param_1)

{
  *(int *)(this + 0x1e4) = param_1;
  return;
}

```
## target disasm
```
  000bc50c: str.w r1,[r0,#0x1e4]
  000bc510: bx lr
```
