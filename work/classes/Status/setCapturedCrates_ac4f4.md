# Status::setCapturedCrates
elf 0xac4f4 body 6
Sig: undefined __thiscall setCapturedCrates(Status * this, int param_1)

## decompile
```c

/* Status::setCapturedCrates(int) */

void __thiscall Status::setCapturedCrates(Status *this,int param_1)

{
  *(int *)(this + 0x1e0) = param_1;
  return;
}

```
## target disasm
```
  000bc4f4: str.w r1,[r0,#0x1e0]
  000bc4f8: bx lr
```
