# Status::getStationStack
elf 0xa8fe2 body 6
Sig: undefined __thiscall getStationStack(Status * this)

## decompile
```c

/* Status::getStationStack() */

undefined4 __thiscall Status::getStationStack(Status *this)

{
  return *(undefined4 *)(this + 0x1a0);
}

```
## target disasm
```
  000b8fe2: ldr.w r0,[r0,#0x1a0]
  000b8fe6: bx lr
```
