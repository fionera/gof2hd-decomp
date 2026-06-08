# PendingProduct::~PendingProduct
elf 0x12d480 body 14
Sig: undefined __thiscall ~PendingProduct(PendingProduct * this)

## decompile
```c

/* PendingProduct::~PendingProduct() */

void __thiscall PendingProduct::~PendingProduct(PendingProduct *this)

{
  (*(code *)(DAT_001abe94 + 0x1abe98))(this);
  return;
}

```

## target disasm
```
  0013d480: b.w 0x001abe88
  001abe88: bx pc
  001abe8c: ldr r12,[0x1abe94]
  001abe90: add pc,r12,pc
```
