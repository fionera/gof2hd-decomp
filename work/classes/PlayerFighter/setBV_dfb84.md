# PlayerFighter::setBV
elf 0xdfb84 body 6
Sig: undefined __thiscall setBV(PlayerFighter * this, Array * param_1)

## decompile
```c

/* PlayerFighter::setBV(Array<BoundingVolume*>*) */

void __thiscall PlayerFighter::setBV(PlayerFighter *this,Array *param_1)

{
  *(Array **)(this + 0x150) = param_1;
  return;
}

```

## target disasm
```
  000efb84: str.w r1,[r0,#0x150]
  000efb88: bx lr
```
