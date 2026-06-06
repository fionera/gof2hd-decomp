# Status::setMissionCount
elf 0xac5f8 body 6
Sig: undefined __thiscall setMissionCount(Status * this, int param_1)

## decompile
```c

/* Status::setMissionCount(int) */

void __thiscall Status::setMissionCount(Status *this,int param_1)

{
  *(int *)(this + 0x1c4) = param_1;
  return;
}

```
## target disasm
```
  000bc5f8: str.w r1,[r0,#0x1c4]
  000bc5fc: bx lr
```
