# Standing::applyStealCargo
elf 0x11d98c body 6
Sig: undefined __thiscall applyStealCargo(Standing * this, int param_1)

## decompile
```c

/* Standing::applyStealCargo(int) */

void __thiscall Standing::applyStealCargo(Standing *this,int param_1)

{
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,2);
  return;
}

```

## target disasm
```
  0012d98c: movs r2,#0x2
  0012d98e: b.w 0x001ac3b8
```
