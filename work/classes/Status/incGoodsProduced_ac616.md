# Status::incGoodsProduced
elf 0xac616 body 12
Sig: undefined __thiscall incGoodsProduced(Status * this, int param_1)

## decompile
```c

/* Status::incGoodsProduced(int) */

void __thiscall Status::incGoodsProduced(Status *this,int param_1)

{
  *(int *)(this + 0x1d4) = param_1 + *(int *)(this + 0x1d4);
  return;
}

```
## target disasm
```
  000bc616: ldr.w r2,[r0,#0x1d4]
  000bc61a: add r1,r2
  000bc61c: str.w r1,[r0,#0x1d4]
  000bc620: bx lr
```
