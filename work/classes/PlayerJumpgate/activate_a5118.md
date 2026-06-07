# PlayerJumpgate::activate
elf 0xa5118 body 72
Sig: undefined __thiscall activate(PlayerJumpgate * this)

## decompile
```c

/* PlayerJumpgate::activate() */

void __thiscall PlayerJumpgate::activate(PlayerJumpgate *this)

{
  undefined4 uVar1;
  uint *puVar2;
  
  if (this[0x140] != (PlayerJumpgate)0x0) {
    return;
  }
  if (*(int *)(this + 0x144) != -1) {
    puVar2 = *(uint **)(DAT_000b5160 + 0xb5134);
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar2);
    AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
    AbyssEngine::PaintCanvas::TransformRemoveChild
              ((PaintCanvas *)*puVar2,*(uint *)(*(int *)(this + 8) + 0xc),
               *(uint *)(*(int *)(this + 8) + 0x14));
    AEGeometry::addChild(*(uint *)(this + 8));
  }
  this[0x140] = (PlayerJumpgate)0x1;
  return;
}

```

## target disasm
```
  000b5118: push {r4,r5,r7,lr}
  000b511a: add r7,sp,#0x8
  000b511c: mov r4,r0
  000b511e: ldrb.w r0,[r0,#0x140]
  000b5122: cbz r0,0x000b5126
  000b5124: pop {r4,r5,r7,pc}
  000b5126: ldr.w r1,[r4,#0x144]
  000b512a: adds r0,r1,#0x1
  000b512c: beq 0x000b5158
  000b512e: ldr r0,[0x000b5160]
  000b5130: add r0,pc
  000b5132: ldr r5,[r0,#0x0]
  000b5134: ldr r0,[r5,#0x0]
  000b5136: blx 0x00072088
  000b513a: movs r1,#0x1
  000b513c: movs r2,#0x0
  000b513e: blx 0x0006fd18
  000b5142: ldr r2,[r4,#0x8]
  000b5144: ldr r0,[r5,#0x0]
  000b5146: ldr r1,[r2,#0xc]
  000b5148: ldr r2,[r2,#0x14]
  000b514a: blx 0x0007210c
  000b514e: ldr r0,[r4,#0x8]
  000b5150: ldr.w r1,[r4,#0x144]
  000b5154: blx 0x000720e8
  000b5158: movs r0,#0x1
  000b515a: strb.w r0,[r4,#0x140]
  000b515e: pop {r4,r5,r7,pc}
```
