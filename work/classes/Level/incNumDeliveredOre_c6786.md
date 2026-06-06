# Level::incNumDeliveredOre
elf 0xc6786 body 12
Sig: undefined __thiscall incNumDeliveredOre(Level * this, int param_1)

## decompile
```c

/* Level::incNumDeliveredOre(int) */

void __thiscall Level::incNumDeliveredOre(Level *this,int param_1)

{
  *(int *)(this + 0x290) = param_1 + *(int *)(this + 0x290);
  return;
}

```
## target disasm
```
  000d6786: ldr.w r2,[r0,#0x290]
  000d678a: add r1,r2
  000d678c: str.w r1,[r0,#0x290]
  000d6790: bx lr
```
