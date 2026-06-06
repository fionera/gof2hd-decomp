# Status::getGoodsProduced
elf 0xac610 body 6
Sig: undefined __thiscall getGoodsProduced(Status * this)

## decompile
```c

/* Status::getGoodsProduced() */

undefined4 __thiscall Status::getGoodsProduced(Status *this)

{
  return *(undefined4 *)(this + 0x1d4);
}

```
## target disasm
```
  000bc610: ldr.w r0,[r0,#0x1d4]
  000bc614: bx lr
```
