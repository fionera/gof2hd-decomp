# Status::getLastXP
elf 0xac692 body 6
Sig: undefined __thiscall getLastXP(Status * this)

## decompile
```c

/* Status::getLastXP() */

undefined4 __thiscall Status::getLastXP(Status *this)

{
  return *(undefined4 *)(this + 0x1cc);
}

```
## target disasm
```
  000bc692: ldr.w r0,[r0,#0x1cc]
  000bc696: bx lr
```
