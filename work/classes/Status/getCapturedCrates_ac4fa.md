# Status::getCapturedCrates
elf 0xac4fa body 6
Sig: undefined __thiscall getCapturedCrates(Status * this)

## decompile
```c

/* Status::getCapturedCrates() */

undefined4 __thiscall Status::getCapturedCrates(Status *this)

{
  return *(undefined4 *)(this + 0x1e0);
}

```
## target disasm
```
  000bc4fa: ldr.w r0,[r0,#0x1e0]
  000bc4fe: bx lr
```
