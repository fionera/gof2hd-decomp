# Status::getSystemVisibilities
elf 0xa8fd6 body 4
Sig: undefined __thiscall getSystemVisibilities(Status * this)

## decompile
```c

/* Status::getSystemVisibilities() */

undefined4 __thiscall Status::getSystemVisibilities(Status *this)

{
  return *(undefined4 *)(this + 0x38);
}

```
## target disasm
```
  000b8fd6: ldr r0,[r0,#0x38]
  000b8fd8: bx lr
```
