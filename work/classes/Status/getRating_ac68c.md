# Status::getRating
elf 0xac68c body 6
Sig: undefined __thiscall getRating(Status * this)

## decompile
```c

/* Status::getRating() */

undefined4 __thiscall Status::getRating(Status *this)

{
  return *(undefined4 *)(this + 0x1b4);
}

```
## target disasm
```
  000bc68c: ldr.w r0,[r0,#0x1b4]
  000bc690: bx lr
```
