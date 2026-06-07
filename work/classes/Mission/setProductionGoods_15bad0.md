# Mission::setProductionGoods
elf 0x15bad0 body 6
Sig: undefined __thiscall setProductionGoods(Mission * this, int param_1, int param_2)

## decompile
```c

/* Mission::setProductionGoods(int, int) */

void __thiscall Mission::setProductionGoods(Mission *this,int param_1,int param_2)

{
  *(int *)(this + 0x68) = param_1;
  *(int *)(this + 0x6c) = param_2;
  return;
}

```

## target disasm
```
  0016bad0: strd r1,r2,[r0,#0x68]
  0016bad4: bx lr
```
