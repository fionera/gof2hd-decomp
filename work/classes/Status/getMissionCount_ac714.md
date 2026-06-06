# Status::getMissionCount
elf 0xac714 body 6
Sig: undefined __thiscall getMissionCount(Status * this)

## decompile
```c

/* Status::getMissionCount() */

undefined4 __thiscall Status::getMissionCount(Status *this)

{
  return *(undefined4 *)(this + 0x1c4);
}

```
## target disasm
```
  000bc714: ldr.w r0,[r0,#0x1c4]
  000bc718: bx lr
```
