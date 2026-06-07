# PlayerGasCloud::getSparks
elf 0x176640 body 6
Sig: undefined __thiscall getSparks(PlayerGasCloud * this)

## decompile
```c

/* PlayerGasCloud::getSparks() */

undefined4 __thiscall PlayerGasCloud::getSparks(PlayerGasCloud *this)

{
  return *(undefined4 *)(this + 0x138);
}

```

## target disasm
```
  00186640: ldr.w r0,[r0,#0x138]
  00186644: bx lr
```
