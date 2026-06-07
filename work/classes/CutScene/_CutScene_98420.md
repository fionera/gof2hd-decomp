# CutScene::~CutScene
elf 0x98420 body 156
Sig: undefined __thiscall ~CutScene(CutScene * this)

## decompile
```c

/* CutScene::~CutScene() */

CutScene * __thiscall CutScene::~CutScene(CutScene *this)

{
  int iVar1;
  void *pvVar2;
  
  if (*(AEGeometry **)(this + 0x20) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x20));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x20) = 0;
  if (*(AEGeometry **)(this + 100) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 100));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 100) = 0;
  if (*(Level **)this != (Level *)0x0) {
    pvVar2 = (void *)Level::~Level(*(Level **)this);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000a84c0;
  *(undefined4 *)this = 0;
  AbyssEngine::PaintCanvas::FogEnable(**(undefined4 **)(iVar1 + 0xa845a),0,1);
  if (*(AEGeometry **)(this + 0x28) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x28));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x28) = 0;
  if (*(AEGeometry **)(this + 0x2c) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x2c));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x2c) = 0;
  if (*(AEGeometry **)(this + 0x30) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x30));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x30) = 0;
  if (*(AEGeometry **)(this + 0x34) != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x34));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x34) = 0;
  if (*(Array **)(this + 0x38) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x38));
    if (*(Array<AEGeometry*> **)(this + 0x38) != (Array<AEGeometry*> *)0x0) {
      pvVar2 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x38));
      operator_delete(pvVar2);
    }
  }
  *(undefined4 *)(this + 0x38) = 0;
  return this;
}

```

## target disasm
```
  000a8420: push {r4,r5,r7,lr}
  000a8422: add r7,sp,#0x8
  000a8424: mov r4,r0
  000a8426: ldr r0,[r0,#0x20]
  000a8428: cbz r0,0x000a8432
  000a842a: blx 0x00071fc8
  000a842e: blx 0x0006eb48
  000a8432: ldr r0,[r4,#0x64]
  000a8434: movs r5,#0x0
  000a8436: str r5,[r4,#0x20]
  000a8438: cbz r0,0x000a8442
  000a843a: blx 0x00071fc8
  000a843e: blx 0x0006eb48
  000a8442: ldr r0,[r4,#0x0]
  000a8444: str r5,[r4,#0x64]
  000a8446: cbz r0,0x000a8450
  000a8448: blx 0x00071fd4
  000a844c: blx 0x0006eb48
  000a8450: ldr r0,[0x000a84c0]
  000a8452: movs r1,#0x0
  000a8454: str r1,[r4,#0x0]
  000a8456: add r0,pc
  000a8458: ldr r0,[r0,#0x0]
  000a845a: ldr r0,[r0,#0x0]
  000a845c: movs r1,#0x0
  000a845e: movs r2,#0x1
  000a8460: blx 0x00071fe0
  000a8464: ldr r0,[r4,#0x28]
  000a8466: cbz r0,0x000a8470
  000a8468: blx 0x00071fc8
  000a846c: blx 0x0006eb48
  000a8470: ldr r0,[r4,#0x2c]
  000a8472: movs r5,#0x0
  000a8474: str r5,[r4,#0x28]
  000a8476: cbz r0,0x000a8480
  000a8478: blx 0x00071fc8
  000a847c: blx 0x0006eb48
  000a8480: ldr r0,[r4,#0x30]
  000a8482: str r5,[r4,#0x2c]
  000a8484: cbz r0,0x000a848e
  000a8486: blx 0x00071fc8
  000a848a: blx 0x0006eb48
  000a848e: ldr r0,[r4,#0x34]
  000a8490: movs r5,#0x0
  000a8492: str r5,[r4,#0x30]
  000a8494: cbz r0,0x000a849e
  000a8496: blx 0x00071fc8
  000a849a: blx 0x0006eb48
  000a849e: ldr r0,[r4,#0x38]
  000a84a0: str r5,[r4,#0x34]
  000a84a2: cbz r0,0x000a84b4
  000a84a4: blx 0x00071fec
  000a84a8: ldr r0,[r4,#0x38]
  000a84aa: cbz r0,0x000a84b4
  000a84ac: blx 0x000717c4
  000a84b0: blx 0x0006eb48
  000a84b4: movs r0,#0x0
  000a84b6: str r0,[r4,#0x38]
  000a84b8: mov r0,r4
  000a84ba: pop {r4,r5,r7,pc}
```
