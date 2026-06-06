# Item::changeStationAmount
elf 0xe0190 body 8
Sig: undefined __thiscall changeStationAmount(Item * this, int param_1)

## decompile
```c

/* Item::changeStationAmount(int) */

void __thiscall Item::changeStationAmount(Item *this,int param_1)

{
  *(int *)(this + 0x38) = param_1 + *(int *)(this + 0x38);
  return;
}

```
## target disasm
```
  000f0190: ldr r2,[r0,#0x38]
  000f0192: add r1,r2
  000f0194: str r1,[r0,#0x38]
  000f0196: bx lr
```
