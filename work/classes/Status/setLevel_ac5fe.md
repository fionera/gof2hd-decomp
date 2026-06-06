# Status::setLevel
elf 0xac5fe body 6
Sig: undefined __thiscall setLevel(Status * this, int param_1)

## decompile
```c

/* Status::setLevel(int) */

void __thiscall Status::setLevel(Status *this,int param_1)

{
  *(int *)(this + 0x1c8) = param_1;
  return;
}

```
## target disasm
```
  000bc5fe: str.w r1,[r0,#0x1c8]
  000bc602: bx lr
```
