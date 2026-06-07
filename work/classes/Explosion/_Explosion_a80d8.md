# Explosion::~Explosion
elf 0xa80d8 body 64
Sig: undefined __thiscall ~Explosion(Explosion * this)

## decompile
```c

/* Explosion::~Explosion() */

Explosion * __thiscall Explosion::~Explosion(Explosion *this)

{
  void *pvVar1;
  
  if (*(AEGeometry **)(this + 4) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(AEGeometry **)(this + 8) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0xc));
    if (*(Array<AEGeometry*> **)(this + 0xc) != (Array<AEGeometry*> *)0x0) {
      pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0xc));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  return this;
}

```

## target disasm
```
  000b80d8: push {r4,r5,r7,lr}
  000b80da: add r7,sp,#0x8
  000b80dc: mov r4,r0
  000b80de: ldr r0,[r0,#0x4]
  000b80e0: cbz r0,0x000b80ea
  000b80e2: blx 0x00071fc8
  000b80e6: blx 0x0006eb48
  000b80ea: ldr r0,[r4,#0x8]
  000b80ec: movs r5,#0x0
  000b80ee: str r5,[r4,#0x4]
  000b80f0: cbz r0,0x000b80fa
  000b80f2: blx 0x00071fc8
  000b80f6: blx 0x0006eb48
  000b80fa: ldr r0,[r4,#0xc]
  000b80fc: str r5,[r4,#0x8]
  000b80fe: cbz r0,0x000b8110
  000b8100: blx 0x00071fec
  000b8104: ldr r0,[r4,#0xc]
  000b8106: cbz r0,0x000b8110
  000b8108: blx 0x000717c4
  000b810c: blx 0x0006eb48
  000b8110: movs r0,#0x0
  000b8112: str r0,[r4,#0xc]
  000b8114: mov r0,r4
  000b8116: pop {r4,r5,r7,pc}
```
