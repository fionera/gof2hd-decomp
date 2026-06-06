# Status::crateCaptured
elf 0xac4e8 body 12
Sig: undefined __thiscall crateCaptured(Status * this, int param_1)

## decompile
```c

/* Status::crateCaptured(int) */

void __thiscall Status::crateCaptured(Status *this,int param_1)

{
  *(int *)(this + 0x1e0) = param_1 + *(int *)(this + 0x1e0);
  return;
}

```
## target disasm
```
  000bc4e8: ldr.w r2,[r0,#0x1e0]
  000bc4ec: add r1,r2
  000bc4ee: str.w r1,[r0,#0x1e0]
  000bc4f2: bx lr
```
