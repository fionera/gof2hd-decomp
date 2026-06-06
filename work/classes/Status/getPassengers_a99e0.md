# Status::getPassengers
elf 0xa99e0 body 4
Sig: undefined __thiscall getPassengers(Status * this)

## decompile
```c

/* Status::getPassengers() */

undefined4 __thiscall Status::getPassengers(Status *this)

{
  return *(undefined4 *)(this + 0x34);
}

```
## target disasm
```
  000b99e0: ldr r0,[r0,#0x34]
  000b99e2: bx lr
```
