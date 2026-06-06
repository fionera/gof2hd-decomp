# Layout::drawFooterNoBackButton

ELF vaddr 0xd4090, body 8 bytes
Signature: undefined __thiscall drawFooterNoBackButton(Layout * this)

## Ghidra decompile
```c

/* Layout::drawFooterNoBackButton() */

void __thiscall Layout::drawFooterNoBackButton(Layout *this)

{
  (*(code *)(DAT_001ac0a4 + 0x1ac0a8))(this,0,0);
  return;
}

```

## Target disassembly (must match)
```
  000e4090: movs r1,#0x0
  000e4092: movs r2,#0x0
  000e4094: b.w 0x001ac098
```

Verify: tools/try.sh work/src/drawFooterNoBackButton.cpp drawFooterNoBackButton d4090 32
