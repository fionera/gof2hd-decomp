# Status::getPlanetNames
elf 0xac5d4 body 6
Sig: undefined __thiscall getPlanetNames(Status * this)

## decompile
```c

/* Status::getPlanetNames() */

undefined4 __thiscall Status::getPlanetNames(Status *this)

{
  return *(undefined4 *)(this + 0x1a8);
}

```
## target disasm
```
  000bc5d4: ldr.w r0,[r0,#0x1a8]
  000bc5d8: bx lr
```
