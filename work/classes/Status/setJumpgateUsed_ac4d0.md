# Status::setJumpgateUsed
elf 0xac4d0 body 6
Sig: undefined __thiscall setJumpgateUsed(Status * this, int param_1)

## decompile
```c

/* Status::setJumpgateUsed(int) */

void __thiscall Status::setJumpgateUsed(Status *this,int param_1)

{
  *(int *)(this + 0x1dc) = param_1;
  return;
}

```
## target disasm
```
  000bc4d0: str.w r1,[r0,#0x1dc]
  000bc4d4: bx lr
```
