# Status::setRating
elf 0xac5e6 body 6
Sig: undefined __thiscall setRating(Status * this, int param_1)

## decompile
```c

/* Status::setRating(int) */

void __thiscall Status::setRating(Status *this,int param_1)

{
  *(int *)(this + 0x1b4) = param_1;
  return;
}

```
## target disasm
```
  000bc5e6: str.w r1,[r0,#0x1b4]
  000bc5ea: bx lr
```
