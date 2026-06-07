# Standing::applyDisable
elf 0x11d99a body 8
Sig: undefined __thiscall applyDisable(Standing * this, int param_1)

## decompile
```c

/* Standing::applyDisable(int) */

void __thiscall Standing::applyDisable(Standing *this,int param_1)

{
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,2);
  return;
}

```

## target disasm
```
  0012d99a: movs r2,#0x2
  0012d99c: b.w 0x001ac3b8
  001ac3b8: bx pc
```
