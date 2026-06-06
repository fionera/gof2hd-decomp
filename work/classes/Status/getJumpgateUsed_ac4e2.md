# Status::getJumpgateUsed
elf 0xac4e2 body 6
Sig: undefined __thiscall getJumpgateUsed(Status * this)

## decompile
```c

/* Status::getJumpgateUsed() */

undefined4 __thiscall Status::getJumpgateUsed(Status *this)

{
  return *(undefined4 *)(this + 0x1dc);
}

```
## target disasm
```
  000bc4e2: ldr.w r0,[r0,#0x1dc]
  000bc4e6: bx lr
```
