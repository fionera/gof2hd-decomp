# Layout::drawFooterStation
elf 0xd4098 body 10
Sig: undefined __stdcall drawFooterStation(void)

## decompile
```c

/* Layout::drawFooterStation() */

void Layout::drawFooterStation(void)

{
  (*(code *)(DAT_001ac0a4 + 0x1ac0a8))();
  return;
}

```

## target disasm
```
  000e4098: movs r1,#0x1
  000e409a: movs r2,#0x0
  000e409c: b.w 0x001ac098
  001ac098: bx pc
```
