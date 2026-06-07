# AEGeometry::~AEGeometry
elf 0xa44f8 body 66
Sig: undefined __thiscall ~AEGeometry(AEGeometry * this)

## decompile
```c

/* AEGeometry::~AEGeometry() */

AEGeometry * __thiscall AEGeometry::~AEGeometry(AEGeometry *this)

{
  if (*(void **)(this + 0x54) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x54));
  }
  *(undefined4 *)(this + 0x54) = 0;
  if (*(void **)(this + 0x5c) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x5c));
  }
  *(undefined4 *)(this + 0x5c) = 0;
  if (*(void **)(this + 0x58) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x58));
  }
  *(undefined4 *)(this + 0x58) = 0;
  if (*(void **)(this + 0x60) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x60));
  }
  *(undefined4 *)(this + 0x60) = 0;
  if (*(void **)(this + 100) != (void *)0x0) {
    operator_delete__(*(void **)(this + 100));
  }
  *(undefined4 *)(this + 100) = 0;
  return this;
}

```

## target disasm
```
  000b44f8: push {r4,r5,r7,lr}
  000b44fa: add r7,sp,#0x8
  000b44fc: mov r4,r0
  000b44fe: ldr r0,[r0,#0x54]
  000b4500: cbz r0,0x000b4506
  000b4502: blx 0x0006ebfc
  000b4506: ldr r0,[r4,#0x5c]
  000b4508: movs r5,#0x0
  000b450a: str r5,[r4,#0x54]
  000b450c: cbz r0,0x000b4512
  000b450e: blx 0x0006ebfc
  000b4512: ldr r0,[r4,#0x58]
  000b4514: str r5,[r4,#0x5c]
  000b4516: cbz r0,0x000b451c
  000b4518: blx 0x0006ebfc
  000b451c: ldr r0,[r4,#0x60]
  000b451e: movs r5,#0x0
  000b4520: str r5,[r4,#0x58]
  000b4522: cbz r0,0x000b4528
  000b4524: blx 0x0006ebfc
  000b4528: ldr r0,[r4,#0x64]
  000b452a: str r5,[r4,#0x60]
  000b452c: cbz r0,0x000b4532
  000b452e: blx 0x0006ebfc
  000b4532: movs r0,#0x0
  000b4534: str r0,[r4,#0x64]
  000b4536: mov r0,r4
  000b4538: pop {r4,r5,r7,pc}
```
