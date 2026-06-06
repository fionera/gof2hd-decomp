# Status::getBluePrints
elf 0xac724 body 4
Sig: undefined __thiscall getBluePrints(Status * this)

## decompile
```c

/* Status::getBluePrints() */

undefined4 __thiscall Status::getBluePrints(Status *this)

{
  return *(undefined4 *)(this + 0x18);
}

```
## target disasm
```
  000bc724: ldr r0,[r0,#0x18]
  000bc726: bx lr
```
