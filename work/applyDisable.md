# Standing::applyDisable

ELF vaddr 0x11d99a, body 8 bytes
Signature: undefined __thiscall applyDisable(Standing * this, int param_1)

## Ghidra decompile
```c

/* Standing::applyDisable(int) */

void __thiscall Standing::applyDisable(Standing *this,int param_1)

{
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,2);
  return;
}

```

## Target disassembly (must match)
```
  0012d99a: movs r2,#0x2
  0012d99c: b.w 0x001ac3b8
  001ac3b8: bx pc
```

Verify: tools/try.sh work/src/applyDisable.cpp applyDisable 11d99a 32
