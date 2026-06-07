# Layout::enableFillScreen
elf 0xd5032 body 6
Sig: undefined __thiscall enableFillScreen(Layout * this, bool param_1)

## decompile
```c

/* Layout::enableFillScreen(bool) */

void __thiscall Layout::enableFillScreen(Layout *this,bool param_1)

{
  this[0x410] = (Layout)param_1;
  return;
}

```

## target disasm
```
  000e5032: strb.w r1,[r0,#0x410]
  000e5036: bx lr
```
