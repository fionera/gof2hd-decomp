# Globals::sqrt

ELF vaddr 0xe8c80, body 8 bytes
Signature: double __thiscall sqrt(Globals * this, double __x)

## Ghidra decompile
```c

/* Globals::sqrt(float) */

double __thiscall Globals::sqrt(Globals *this,double __x)

{
  undefined4 uVar1;
  undefined4 extraout_s1;
  
  uVar1 = (*(code *)(DAT_001ac2f4 + 0x1ac2f8))(SUB84(__x,0));
  return (double)CONCAT44(extraout_s1,uVar1);
}

```

## Target disassembly (must match)
```
  000f8c80: mov r0,r1
  000f8c82: b.w 0x001ac2e8
  001ac2e8: bx pc
```

Verify: tools/try.sh work/src/sqrt.cpp sqrt e8c80 32
