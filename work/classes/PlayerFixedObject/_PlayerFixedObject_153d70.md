# PlayerFixedObject::~PlayerFixedObject
elf 0x153d70 body 138
Sig: undefined __thiscall ~PlayerFixedObject(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::~PlayerFixedObject() */

void __thiscall PlayerFixedObject::~PlayerFixedObject(PlayerFixedObject *this)

{
  AEGeometry *this_00;
  void *pvVar1;
  
  this_00 = *(AEGeometry **)(this + 0x124);
  *(int *)this = *(int *)(DAT_00163e10 + 0x163d7e) + 8;
  if (this_00 != *(AEGeometry **)(this + 8)) {
    if (this_00 != (AEGeometry *)0x0) {
      pvVar1 = (void *)AEGeometry::~AEGeometry(this_00);
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x124) = 0;
  }
  if (*(Array **)(this + 0x128) != (Array *)0x0) {
    ArrayReleaseClasses<BoundingVolume*>(*(Array **)(this + 0x128));
    if (*(Array<BoundingVolume*> **)(this + 0x128) != (Array<BoundingVolume*> *)0x0) {
      pvVar1 = (void *)Array<BoundingVolume*>::~Array(*(Array<BoundingVolume*> **)(this + 0x128));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x128) = 0;
  if (*(Array **)(this + 300) != (Array *)0x0) {
    ArrayReleaseClasses<BoundingVolume*>(*(Array **)(this + 300));
    if (*(Array<BoundingVolume*> **)(this + 300) != (Array<BoundingVolume*> *)0x0) {
      pvVar1 = (void *)Array<BoundingVolume*>::~Array(*(Array<BoundingVolume*> **)(this + 300));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 300) = 0;
  if (*(Explosion **)(this + 0x18c) != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(*(Explosion **)(this + 0x18c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18c) = 0;
  AbyssEngine::String::~String((String *)(this + 0x1ac));
  (*(code *)(DAT_001ac1a4 + 0x1ac1a8))(this);
  return;
}

```

## target disasm
```
  00163d70: push {r4,r5,r7,lr}
  00163d72: add r7,sp,#0x8
  00163d74: mov r4,r0
  00163d76: ldr r0,[0x00163e10]
  00163d78: ldr r2,[r4,#0x8]
  00163d7a: add r0,pc
  00163d7c: ldr r1,[r0,#0x0]
  00163d7e: ldr.w r0,[r4,#0x124]
  00163d82: adds r1,#0x8
  00163d84: str r1,[r4,#0x0]
  00163d86: cmp r0,r2
  00163d88: beq 0x00163d9a
  00163d8a: cbz r0,0x00163d94
  00163d8c: blx 0x00071fc8
  00163d90: blx 0x0006eb48
  00163d94: movs r0,#0x0
  00163d96: str.w r0,[r4,#0x124]
  00163d9a: ldr.w r0,[r4,#0x128]
  00163d9e: cbz r0,0x00163db2
  00163da0: blx 0x000757a8
  00163da4: ldr.w r0,[r4,#0x128]
  00163da8: cbz r0,0x00163db2
  00163daa: blx 0x000757b4
  00163dae: blx 0x0006eb48
  00163db2: ldr.w r0,[r4,#0x12c]
  00163db6: movs r5,#0x0
  00163db8: str.w r5,[r4,#0x128]
  00163dbc: cbz r0,0x00163dd0
  00163dbe: blx 0x000757a8
  00163dc2: ldr.w r0,[r4,#0x12c]
  00163dc6: cbz r0,0x00163dd0
  00163dc8: blx 0x000757b4
  00163dcc: blx 0x0006eb48
  00163dd0: ldr.w r0,[r4,#0x18c]
  00163dd4: str.w r5,[r4,#0x12c]
  00163dd8: cbz r0,0x00163de2
  00163dda: blx 0x00072334
  00163dde: blx 0x0006eb48
  00163de2: movs r0,#0x0
  00163de4: str.w r0,[r4,#0x18c]
  00163de8: add.w r0,r4,#0x1ac
  00163dec: blx 0x0006ee30
  00163df0: mov r0,r4
  00163df2: pop.w {r4,r5,r7,lr}
  00163df6: b.w 0x001ac198
```
