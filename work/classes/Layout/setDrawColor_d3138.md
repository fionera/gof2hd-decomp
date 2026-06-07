# Layout::setDrawColor
elf 0xd3138 body 6
Sig: undefined __thiscall setDrawColor(Layout * this, int param_1)

## decompile
```c

/* Layout::setDrawColor(int) */

void __thiscall Layout::setDrawColor(Layout *this,int param_1)

{
  *(int *)(this + 0x3b0) = param_1;
  return;
}

```

## target disasm
```
  000e3138: str.w r1,[r0,#0x3b0]
  000e313c: bx lr
```
