# BoundingSphere::~BoundingSphere
elf 0x151d64 body 14
Sig: undefined __thiscall ~BoundingSphere(BoundingSphere * this)

## decompile
```c

/* BoundingSphere::~BoundingSphere() */

void __thiscall BoundingSphere::~BoundingSphere(BoundingSphere *this)

{
  (*(code *)(DAT_001ab984 + 0x1ab988))(this);
  return;
}

```

## target disasm
```
  00161d64: b.w 0x001ab978
  001ab978: bx pc
  001ab97c: ldr r12,[0x1ab984]
  001ab980: add pc,r12,pc
```
