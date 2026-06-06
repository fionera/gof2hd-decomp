# Status::getPendingProducts
elf 0xa8fd2 body 4
Sig: undefined __thiscall getPendingProducts(Status * this)

## decompile
```c

/* Status::getPendingProducts() */

undefined4 __thiscall Status::getPendingProducts(Status *this)

{
  return *(undefined4 *)(this + 0x1c);
}

```
## target disasm
```
  000b8fd2: ldr r0,[r0,#0x1c]
  000b8fd4: bx lr
```
