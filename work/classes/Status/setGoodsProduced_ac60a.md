# Status::setGoodsProduced
elf 0xac60a body 6
Sig: undefined __thiscall setGoodsProduced(Status * this, int param_1)

## decompile
```c

/* Status::setGoodsProduced(int) */

void __thiscall Status::setGoodsProduced(Status *this,int param_1)

{
  *(int *)(this + 0x1d4) = param_1;
  return;
}

```
## target disasm
```
  000bc60a: str.w r1,[r0,#0x1d4]
  000bc60e: bx lr
```
