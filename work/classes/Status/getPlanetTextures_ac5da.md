# Status::getPlanetTextures
elf 0xac5da body 6
Sig: undefined __thiscall getPlanetTextures(Status * this)

## decompile
```c

/* Status::getPlanetTextures() */

undefined4 __thiscall Status::getPlanetTextures(Status *this)

{
  return *(undefined4 *)(this + 0x1ac);
}

```
## target disasm
```
  000bc5da: ldr.w r0,[r0,#0x1ac]
  000bc5de: bx lr
```
