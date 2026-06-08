# RepairBeam::~RepairBeam
elf 0xa72a0 body 64
Sig: undefined __thiscall ~RepairBeam(RepairBeam * this)

## decompile
```c

/* RepairBeam::~RepairBeam() */

RepairBeam * __thiscall RepairBeam::~RepairBeam(RepairBeam *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x10) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x10));
    if (*(Array<AEGeometry*> **)(this + 0x10) != (Array<AEGeometry*> *)0x0) {
      pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x10));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x10) = 0;
  }
  if (*(Array<int> **)(this + 0x14) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x14));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14) = 0;
  if (*(Array<float> **)(this + 0x18) != (Array<float> *)0x0) {
    pvVar1 = (void *)Array<float>::~Array(*(Array<float> **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}

```

## target disasm
```
  000b72a0: push {r4,r5,r7,lr}
  000b72a2: add r7,sp,#0x8
  000b72a4: mov r4,r0
  000b72a6: ldr r0,[r0,#0x10]
  000b72a8: cbz r0,0x000b72be
  000b72aa: blx 0x00071fec
  000b72ae: ldr r0,[r4,#0x10]
  000b72b0: cbz r0,0x000b72ba
  000b72b2: blx 0x000717c4
  000b72b6: blx 0x0006eb48
  000b72ba: movs r0,#0x0
  000b72bc: str r0,[r4,#0x10]
  000b72be: ldr r0,[r4,#0x14]
  000b72c0: cbz r0,0x000b72ca
  000b72c2: blx 0x00070204
  000b72c6: blx 0x0006eb48
  000b72ca: ldr r0,[r4,#0x18]
  000b72cc: movs r5,#0x0
  000b72ce: str r5,[r4,#0x14]
  000b72d0: cbz r0,0x000b72da
  000b72d2: blx 0x00073528
  000b72d6: blx 0x0006eb48
  000b72da: mov r0,r4
  000b72dc: str r5,[r4,#0x18]
  000b72de: pop {r4,r5,r7,pc}
```
