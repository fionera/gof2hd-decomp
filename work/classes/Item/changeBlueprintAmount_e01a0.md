# Item::changeBlueprintAmount
elf 0xe01a0 body 8
Sig: undefined __thiscall changeBlueprintAmount(Item * this, int param_1)

## decompile
```c

/* Item::changeBlueprintAmount(int) */

void __thiscall Item::changeBlueprintAmount(Item *this,int param_1)

{
  *(int *)(this + 0x3c) = param_1 + *(int *)(this + 0x3c);
  return;
}

```
## target disasm
```
  000f01a0: ldr r2,[r0,#0x3c]
  000f01a2: add r1,r2
  000f01a4: str r1,[r0,#0x3c]
  000f01a6: bx lr
```
