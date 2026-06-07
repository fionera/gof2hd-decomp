# Layout::startFade
elf 0xd5014 body 30
Sig: undefined __thiscall startFade(Layout * this, bool param_1, int param_2, int param_3)

## decompile
```c

/* Layout::startFade(bool, int, int) */

void __thiscall Layout::startFade(Layout *this,bool param_1,int param_2,int param_3)

{
  this[0x401] = (Layout)param_1;
  this[0x400] = (Layout)0x1;
  *(uint *)(this + 0x404) = param_2 & 0xffffff00;
  *(undefined4 *)(this + 0x408) = 0;
  *(int *)(this + 0x40c) = param_3;
  return;
}

```

## target disasm
```
  000e5014: strb.w r1,[r0,#0x401]
  000e5018: movs r1,#0x1
  000e501a: strb.w r1,[r0,#0x400]
  000e501e: bic r1,r2,#0xff
  000e5022: movs r2,#0x0
  000e5024: str.w r1,[r0,#0x404]
  000e5028: str.w r2,[r0,#0x408]
  000e502c: str.w r3,[r0,#0x40c]
  000e5030: bx lr
```
