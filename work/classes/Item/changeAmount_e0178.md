# Item::changeAmount
elf 0xe0178 body 8
Sig: undefined __thiscall changeAmount(Item * this, int param_1)

## decompile
```c

/* Item::changeAmount(int) */

void __thiscall Item::changeAmount(Item *this,int param_1)

{
  *(int *)(this + 0x34) = param_1 + *(int *)(this + 0x34);
  return;
}

```
## target disasm
```
  000f0178: ldr r2,[r0,#0x34]
  000f017a: add r1,r2
  000f017c: str r1,[r0,#0x34]
  000f017e: bx lr
```
