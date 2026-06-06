# Status::visitStation
elf 0xac5b4 body 12
Sig: undefined __thiscall visitStation(Status * this)

## decompile
```c

/* Status::visitStation() */

void __thiscall Status::visitStation(Status *this)

{
  *(int *)(this + 0x1d0) = *(int *)(this + 0x1d0) + 1;
  return;
}

```
## target disasm
```
  000bc5b4: ldr.w r1,[r0,#0x1d0]
  000bc5b8: adds r1,#0x1
  000bc5ba: str.w r1,[r0,#0x1d0]
  000bc5be: bx lr
```
