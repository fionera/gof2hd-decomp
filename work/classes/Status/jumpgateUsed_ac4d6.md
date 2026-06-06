# Status::jumpgateUsed
elf 0xac4d6 body 12
Sig: undefined __thiscall jumpgateUsed(Status * this)

## decompile
```c

/* Status::jumpgateUsed() */

void __thiscall Status::jumpgateUsed(Status *this)

{
  *(int *)(this + 0x1dc) = *(int *)(this + 0x1dc) + 1;
  return;
}

```
## target disasm
```
  000bc4d6: ldr.w r1,[r0,#0x1dc]
  000bc4da: adds r1,#0x1
  000bc4dc: str.w r1,[r0,#0x1dc]
  000bc4e0: bx lr
```
