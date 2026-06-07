# Layout::drawFooterNoBackButton
elf 0xd4090 body 8
Sig: undefined __thiscall drawFooterNoBackButton(Layout * this)

## decompile
```c

/* Layout::drawFooterNoBackButton() */

void __thiscall Layout::drawFooterNoBackButton(Layout *this)

{
  (*(code *)(DAT_001ac0a4 + 0x1ac0a8))(this,0,0);
  return;
}

```

## target disasm
```
  000e4090: movs r1,#0x0
  000e4092: movs r2,#0x0
  000e4094: b.w 0x001ac098
```
