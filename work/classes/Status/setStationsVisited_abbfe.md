# Status::setStationsVisited
elf 0xabbfe body 6
Sig: undefined __thiscall setStationsVisited(Status * this, int param_1)

## decompile
```c

/* Status::setStationsVisited(int) */

void __thiscall Status::setStationsVisited(Status *this,int param_1)

{
  *(int *)(this + 0x1d0) = param_1;
  return;
}

```
## target disasm
```
  000bbbfe: str.w r1,[r0,#0x1d0]
  000bbc02: bx lr
```
