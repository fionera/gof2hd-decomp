# LensFlare::~LensFlare
elf 0x11d06c body 22
Sig: undefined __thiscall ~LensFlare(LensFlare * this)

## decompile
```c

/* LensFlare::~LensFlare() */

LensFlare * __thiscall LensFlare::~LensFlare(LensFlare *this)

{
  if (*(void **)(this + 0x10) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x10));
  }
  *(undefined4 *)(this + 0x10) = 0;
  return this;
}

```

## target disasm
```
  0012d06c: push {r4,r6,r7,lr}
  0012d06e: add r7,sp,#0x8
  0012d070: mov r4,r0
  0012d072: ldr r0,[r0,#0x10]
  0012d074: cbz r0,0x0012d07a
  0012d076: blx 0x0006eb48
  0012d07a: movs r0,#0x0
  0012d07c: str r0,[r4,#0x10]
  0012d07e: mov r0,r4
  0012d080: pop {r4,r6,r7,pc}
```
