# StarMap::~StarMap
elf 0xc749c body 224
Sig: undefined __thiscall ~StarMap(StarMap * this)

## decompile
```c

/* StarMap::~StarMap() */

StarMap * __thiscall StarMap::~StarMap(StarMap *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x194) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(this + 0x194));
    if (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x194) !=
        (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array
                                 (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x194));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x194) = 0;
  if (*(Array **)(this + 0x198) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(this + 0x198));
    if (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x198) !=
        (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array
                                 (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x198));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x198) = 0;
  if (*(AEGeometry **)(this + 0x1bc) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1bc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1bc) = 0;
  if (*(AEGeometry **)(this + 0x1b0) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1b0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1b0) = 0;
  if (*(AEGeometry **)(this + 0x1b4) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1b4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1b4) = 0;
  if (*(AEGeometry **)(this + 0x1b8) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1b8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1b8) = 0;
  if (*(void **)(this + 0x17c) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x17c));
  }
  *(undefined4 *)(this + 0x17c) = 0;
  if (*(void **)(this + 0x180) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x180));
  }
  *(undefined4 *)(this + 0x180) = 0;
  if (*(void **)(this + 0x184) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x184));
  }
  *(undefined4 *)(this + 0x184) = 0;
  if (*(AEGeometry **)(this + 0xf8) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0xf8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xf8) = 0;
  if (*(SystemPathFinder **)(this + 0x50) != (SystemPathFinder *)0x0) {
    pvVar1 = (void *)SystemPathFinder::~SystemPathFinder(*(SystemPathFinder **)(this + 0x50));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x50) = 0;
  return this;
}

```

## target disasm
```
  000d749c: push {r4,r5,r7,lr}
  000d749e: add r7,sp,#0x8
  000d74a0: mov r4,r0
  000d74a2: ldr.w r0,[r0,#0x194]
  000d74a6: cbz r0,0x000d74ba
  000d74a8: blx 0x0007216c
  000d74ac: ldr.w r0,[r4,#0x194]
  000d74b0: cbz r0,0x000d74ba
  000d74b2: blx 0x00072178
  000d74b6: blx 0x0006eb48
  000d74ba: ldr.w r0,[r4,#0x198]
  000d74be: movs r5,#0x0
  000d74c0: str.w r5,[r4,#0x194]
  000d74c4: cbz r0,0x000d74d8
  000d74c6: blx 0x0007216c
  000d74ca: ldr.w r0,[r4,#0x198]
  000d74ce: cbz r0,0x000d74d8
  000d74d0: blx 0x00072178
  000d74d4: blx 0x0006eb48
  000d74d8: ldr.w r0,[r4,#0x1bc]
  000d74dc: str.w r5,[r4,#0x198]
  000d74e0: cbz r0,0x000d74ea
  000d74e2: blx 0x00071fc8
  000d74e6: blx 0x0006eb48
  000d74ea: ldr.w r0,[r4,#0x1b0]
  000d74ee: movs r5,#0x0
  000d74f0: str.w r5,[r4,#0x1bc]
  000d74f4: cbz r0,0x000d74fe
  000d74f6: blx 0x00071fc8
  000d74fa: blx 0x0006eb48
  000d74fe: ldr.w r0,[r4,#0x1b4]
  000d7502: str.w r5,[r4,#0x1b0]
  000d7506: cbz r0,0x000d7510
  000d7508: blx 0x00071fc8
  000d750c: blx 0x0006eb48
  000d7510: ldr.w r0,[r4,#0x1b8]
  000d7514: movs r5,#0x0
  000d7516: str.w r5,[r4,#0x1b4]
  000d751a: cbz r0,0x000d7524
  000d751c: blx 0x00071fc8
  000d7520: blx 0x0006eb48
  000d7524: ldr.w r0,[r4,#0x17c]
  000d7528: str.w r5,[r4,#0x1b8]
  000d752c: cbz r0,0x000d7532
  000d752e: blx 0x0006eb48
  000d7532: ldr.w r0,[r4,#0x180]
  000d7536: movs r5,#0x0
  000d7538: str.w r5,[r4,#0x17c]
  000d753c: cbz r0,0x000d7542
  000d753e: blx 0x0006eb48
  000d7542: ldr.w r0,[r4,#0x184]
  000d7546: str.w r5,[r4,#0x180]
  000d754a: cbz r0,0x000d7550
  000d754c: blx 0x0006eb48
  000d7550: ldr.w r0,[r4,#0xf8]
  000d7554: movs r5,#0x0
  000d7556: str.w r5,[r4,#0x184]
  000d755a: cbz r0,0x000d7564
  000d755c: blx 0x00071fc8
  000d7560: blx 0x0006eb48
  000d7564: ldr r0,[r4,#0x50]
  000d7566: str.w r5,[r4,#0xf8]
  000d756a: cbz r0,0x000d7574
  000d756c: blx 0x0007381c
  000d7570: blx 0x0006eb48
  000d7574: movs r0,#0x0
  000d7576: str r0,[r4,#0x50]
  000d7578: mov r0,r4
  000d757a: pop {r4,r5,r7,pc}
```
