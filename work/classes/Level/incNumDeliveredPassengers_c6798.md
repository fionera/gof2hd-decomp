# Level::incNumDeliveredPassengers
elf 0xc6798 body 12
Sig: undefined __thiscall incNumDeliveredPassengers(Level * this, int param_1)

## decompile
```c

/* Level::incNumDeliveredPassengers(int) */

void __thiscall Level::incNumDeliveredPassengers(Level *this,int param_1)

{
  *(int *)(this + 0x294) = param_1 + *(int *)(this + 0x294);
  return;
}

```
## target disasm
```
  000d6798: ldr.w r2,[r0,#0x294]
  000d679c: add r1,r2
  000d679e: str.w r1,[r0,#0x294]
  000d67a2: bx lr
```
