# Status::setStationStack
elf 0xa8fe8 body 6
Sig: undefined __thiscall setStationStack(Status * this, Array * param_1)

## decompile
```c

/* Status::setStationStack(Array<Station*>*) */

void __thiscall Status::setStationStack(Status *this,Array *param_1)

{
  *(Array **)(this + 0x1a0) = param_1;
  return;
}

```
## target disasm
```
  000b8fe8: str.w r1,[r0,#0x1a0]
  000b8fec: bx lr
```
