# Explosion::reset
elf 0xa7fb0 body 286
Sig: undefined __thiscall reset(Explosion * this)

## decompile
```c

/* Explosion::reset() */

void __thiscall Explosion::reset(Explosion *this)

{
  undefined4 uVar1;
  uint *puVar2;
  uint *puVar3;
  uint uVar4;
  longlong lVar5;
  
  puVar3 = *(uint **)(DAT_000b80d4 + 0xb7fc6);
  uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
  AbyssEngine::Transform::SetAnimationState(uVar1,3,0);
  uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
  AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
  if (*(int *)(*(int *)(this + 4) + 0x14) != -1) {
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationState(uVar1,3,0);
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
  }
  if (*(int *)(this + 8) != 0) {
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationState(uVar1,3,0);
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
  }
  if (*(int *)this == 6) {
    lVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationRangeInTime(lVar5,0x8fc);
    lVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    AbyssEngine::Transform::SetAnimationRangeInTime(lVar5,0x8fc);
  }
  puVar2 = *(uint **)(this + 0xc);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
      AbyssEngine::Transform::SetAnimationState(uVar1,3,0);
      uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
      AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
      puVar2 = *(uint **)(this + 0xc);
    }
  }
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  this[0x20] = (Explosion)0x0;
  return;
}

```

## target disasm
```
  000b7fb0: push {r4,r5,r6,r7,lr}
  000b7fb2: add r7,sp,#0xc
  000b7fb4: push.w r8
  000b7fb8: sub sp,#0x8
  000b7fba: mov r8,r0
  000b7fbc: ldr r0,[0x000b80d4]
  000b7fbe: ldr.w r1,[r8,#0x4]
  000b7fc2: add r0,pc
  000b7fc4: ldr r5,[r0,#0x0]
  000b7fc6: ldr r1,[r1,#0xc]
  000b7fc8: ldr r0,[r5,#0x0]
  000b7fca: blx 0x00072088
  000b7fce: movs r1,#0x3
  000b7fd0: movs r2,#0x0
  000b7fd2: blx 0x0006fd18
  000b7fd6: ldr.w r1,[r8,#0x4]
  000b7fda: ldr r0,[r5,#0x0]
  000b7fdc: ldr r1,[r1,#0xc]
  000b7fde: blx 0x00072088
  000b7fe2: movs r1,#0x1
  000b7fe4: movs r2,#0x0
  000b7fe6: blx 0x0006fd18
  000b7fea: ldr.w r0,[r8,#0x4]
  000b7fee: ldr r1,[r0,#0x14]
  000b7ff0: adds r0,r1,#0x1
  000b7ff2: beq 0x000b8016
  000b7ff4: ldr r0,[r5,#0x0]
  000b7ff6: blx 0x00072088
  000b7ffa: movs r1,#0x3
  000b7ffc: movs r2,#0x0
  000b7ffe: blx 0x0006fd18
  000b8002: ldr.w r1,[r8,#0x4]
  000b8006: ldr r0,[r5,#0x0]
  000b8008: ldr r1,[r1,#0x14]
  000b800a: blx 0x00072088
  000b800e: movs r1,#0x1
  000b8010: movs r2,#0x0
  000b8012: blx 0x0006fd18
  000b8016: ldr.w r0,[r8,#0x8]
  000b801a: cbz r0,0x000b8040
  000b801c: ldr r1,[r0,#0xc]
  000b801e: ldr r0,[r5,#0x0]
  000b8020: blx 0x00072088
  000b8024: movs r1,#0x3
  000b8026: movs r2,#0x0
  000b8028: blx 0x0006fd18
  000b802c: ldr.w r1,[r8,#0x8]
  000b8030: ldr r0,[r5,#0x0]
  000b8032: ldr r1,[r1,#0xc]
  000b8034: blx 0x00072088
  000b8038: movs r1,#0x1
  000b803a: movs r2,#0x0
  000b803c: blx 0x0006fd18
  000b8040: ldr.w r0,[r8,#0x0]
  000b8044: cmp r0,#0x6
  000b8046: bne 0x000b8080
  000b8048: ldr.w r0,[r8,#0x4]
  000b804c: ldr r1,[r0,#0xc]
  000b804e: ldr r0,[r5,#0x0]
  000b8050: blx 0x00072088
  000b8054: ldr r4,[0x000b80d0]
  000b8056: movs r6,#0x0
  000b8058: movw r2,#0x8fc
  000b805c: movs r3,#0x0
  000b805e: strd r4,r6,[sp,#0x0]
  000b8062: blx 0x0006ebc0
  000b8066: ldr.w r1,[r8,#0x8]
  000b806a: ldr r0,[r5,#0x0]
  000b806c: ldr r1,[r1,#0xc]
  000b806e: blx 0x00072088
  000b8072: movw r2,#0x8fc
  000b8076: movs r3,#0x0
  000b8078: strd r4,r6,[sp,#0x0]
  000b807c: blx 0x0006ebc0
  000b8080: ldr.w r0,[r8,#0xc]
  000b8084: cbz r0,0x000b80bc
  000b8086: movs r6,#0x0
  000b8088: b 0x000b80b6
  000b808a: ldr r0,[r0,#0x4]
  000b808c: ldr.w r4,[r0,r6,lsl #0x2]
  000b8090: ldr r0,[r5,#0x0]
  000b8092: ldr r1,[r4,#0xc]
  000b8094: blx 0x00072088
  000b8098: movs r1,#0x3
  000b809a: movs r2,#0x0
  000b809c: blx 0x0006fd18
  000b80a0: ldr r1,[r4,#0xc]
  000b80a2: ldr r0,[r5,#0x0]
  000b80a4: blx 0x00072088
  000b80a8: movs r1,#0x1
  000b80aa: movs r2,#0x0
  000b80ac: blx 0x0006fd18
  000b80b0: ldr.w r0,[r8,#0xc]
  000b80b4: adds r6,#0x1
  000b80b6: ldr r1,[r0,#0x0]
  000b80b8: cmp r6,r1
  000b80ba: bcc 0x000b808a
  000b80bc: movs r0,#0x0
  000b80be: strd r0,r0,[r8,#0x18]
  000b80c2: strb.w r0,[r8,#0x20]
  000b80c6: add sp,#0x8
  000b80c8: pop.w r8
  000b80cc: pop {r4,r5,r6,r7,pc}
```
