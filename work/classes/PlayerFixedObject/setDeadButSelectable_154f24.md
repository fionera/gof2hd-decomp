# PlayerFixedObject::setDeadButSelectable
elf 0x154f24 body 84
Sig: undefined __thiscall setDeadButSelectable(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::setDeadButSelectable() */

void __thiscall PlayerFixedObject::setDeadButSelectable(PlayerFixedObject *this)

{
  void *pvVar1;
  undefined4 *puVar2;
  longlong lVar3;
  
  this[0x134] = (PlayerFixedObject)0x0;
  Player::setHitpoints(*(int *)(this + 4));
  Player::setVulnerable(SUB41(*(undefined4 *)(this + 4),0));
  LODManager::removeObject((LODManager *)**(undefined4 **)(this + 0x54),*(AEGeometry **)(this + 8));
  if (*(AEGeometry **)(this + 8) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 8));
    operator_delete(pvVar1);
  }
  puVar2 = (undefined4 *)(DAT_00164f78 + 0x164f60);
  *(undefined4 *)(this + 8) = *(undefined4 *)(this + 0x124);
  lVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(*(uint *)*puVar2);
  AbyssEngine::Transform::SetAnimationRangeInTime(lVar3,*(longlong *)((int)lVar3 + 0xf8));
  return;
}

```

## target disasm
```
  00164f24: push {r2,r3,r4,r6,r7,lr}
  00164f26: add r7,sp,#0x10
  00164f28: mov r4,r0
  00164f2a: ldr r0,[r0,#0x4]
  00164f2c: movs r1,#0x0
  00164f2e: strb.w r1,[r4,#0x134]
  00164f32: movs r1,#0x1
  00164f34: blx 0x000727e4
  00164f38: ldr r0,[r4,#0x4]
  00164f3a: movs r1,#0x0
  00164f3c: blx 0x0007294c
  00164f40: ldr r0,[r4,#0x54]
  00164f42: ldr r1,[r4,#0x8]
  00164f44: ldr r0,[r0,#0x0]
  00164f46: blx 0x000721fc
  00164f4a: ldr r0,[r4,#0x8]
  00164f4c: cbz r0,0x00164f56
  00164f4e: blx 0x00071fc8
  00164f52: blx 0x0006eb48
  00164f56: ldr r0,[0x00164f78]
  00164f58: ldr.w r1,[r4,#0x124]
  00164f5c: add r0,pc
  00164f5e: str r1,[r4,#0x8]
  00164f60: ldr r0,[r0,#0x0]
  00164f62: ldr r1,[r1,#0xc]
  00164f64: ldr r0,[r0,#0x0]
  00164f66: blx 0x00072088
  00164f6a: ldrd r2,r3,[r0,#0xf8]
  00164f6e: strd r2,r3,[sp,#0x0]
  00164f72: blx 0x0006ebc0
  00164f76: pop {r2,r3,r4,r6,r7,pc}
```
