# Layout::drawFooter
elf 0xd3fb4 body 8
Sig: undefined __thiscall drawFooter(Layout * this)

## decompile
```c

/* Layout::drawFooter() */

void __thiscall Layout::drawFooter(Layout *this)

{
  (*(code *)(DAT_001ac0a4 + 0x1ac0a8))(this,0,1);
  return;
}

```

## target disasm
```
  000e3fb4: movs r1,#0x0
  000e3fb6: movs r2,#0x1
  000e3fb8: b.w 0x001ac098
```
