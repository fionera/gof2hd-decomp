# Status::setSystemVisibility
elf 0xa8fda body 8
Sig: undefined __thiscall setSystemVisibility(Status * this, int param_1, bool param_2)

## decompile
```c

/* Status::setSystemVisibility(int, bool) */

void __thiscall Status::setSystemVisibility(Status *this,int param_1,bool param_2)

{
  *(bool *)(*(int *)(*(int *)(this + 0x38) + 4) + param_1) = param_2;
  return;
}

```
## target disasm
```
  000b8fda: ldr r0,[r0,#0x38]
  000b8fdc: ldr r0,[r0,#0x4]
  000b8fde: strb r2,[r0,r1]
  000b8fe0: bx lr
```
