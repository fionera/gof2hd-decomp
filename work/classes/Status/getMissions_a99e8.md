# Status::getMissions
elf 0xa99e8 body 6
Sig: undefined __thiscall getMissions(Status * this)

## decompile
```c

/* Status::getMissions() */

undefined4 __thiscall Status::getMissions(Status *this)

{
  return *(undefined4 *)(this + 0x198);
}

```
## target disasm
```
  000b99e8: ldr.w r0,[r0,#0x198]
  000b99ec: bx lr
```
