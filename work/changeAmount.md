# Item::changeAmount

ELF vaddr 0xe0178, body 8 bytes
Signature: undefined __thiscall changeAmount(Item * this, int param_1)

## Ghidra decompile
```c

/* Item::changeAmount(int) */

void __thiscall Item::changeAmount(Item *this,int param_1)

{
  *(int *)(this + 0x34) = param_1 + *(int *)(this + 0x34);
  return;
}

```

## Target disassembly (must match)
```
  000f0178: ldr r2,[r0,#0x34]
  000f017a: add r1,r2
  000f017c: str r1,[r0,#0x34]
  000f017e: bx lr
```

Verify: tools/try.sh work/src/changeAmount.cpp changeAmount e0178 32
