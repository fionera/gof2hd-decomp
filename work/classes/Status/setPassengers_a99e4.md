# Status::setPassengers
elf 0xa99e4 body 4
Sig: undefined __thiscall setPassengers(Status * this, int param_1)

## decompile
```c

/* Status::setPassengers(int) */

void __thiscall Status::setPassengers(Status *this,int param_1)

{
  *(int *)(this + 0x34) = param_1;
  return;
}

```
## target disasm
```
  000b99e4: str r1,[r0,#0x34]
  000b99e6: bx lr
```
