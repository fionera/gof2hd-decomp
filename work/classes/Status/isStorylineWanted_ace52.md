# Status::isStorylineWanted
elf 0xace52 body 10
Sig: undefined __thiscall isStorylineWanted(Status * this, int param_1)

## decompile
```c

/* Status::isStorylineWanted(int) */

bool __thiscall Status::isStorylineWanted(Status *this,int param_1)

{
  return (uint)param_1 < 2;
}

```
## target disasm
```
  000bce52: movs r0,#0x0
  000bce54: cmp r1,#0x2
  000bce56: it cc
  000bce58: mov.cc r0,#0x1
  000bce5a: bx lr
```
