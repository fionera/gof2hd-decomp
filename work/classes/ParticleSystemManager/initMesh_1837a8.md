# ParticleSystemManager::initMesh
elf 0x1837a8 body 190
Sig: undefined __thiscall initMesh(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::initMesh() */

void __thiscall ParticleSystemManager::initMesh(ParticleSystemManager *this)

{
  ParticleSystemManager *pPVar1;
  short sVar2;
  undefined4 *puVar3;
  short sVar4;
  uint uVar5;
  
  if (*(int *)(this + 0x3c) != 0) {
    pPVar1 = this + 0x54;
    *(undefined4 *)pPVar1 = 0xffffffff;
    *(uint *)(this + 0x58) = 0xffffffff;
    if (*(short *)(this + 0x48) == -1) {
      if (*(short *)(this + 0x4a) != -1) {
        AbyssEngine::PaintCanvas::MeshCreate
                  (*(undefined4 *)(this + 4),(*(ushort *)(this + 0x5c) & 0x3fff) << 2,
                   (*(ushort *)(this + 0x5c) & 0x7fff) << 1,0x1b,pPVar1);
        AbyssEngine::PaintCanvas::TextureCreate
                  ((ushort)*(undefined4 *)(this + 4),(uint *)(uint)*(ushort *)(this + 0x4a),
                   (bool)((char)this + 'P'));
      }
    }
    else {
      AbyssEngine::PaintCanvas::MeshCreate
                (*(PaintCanvas **)(this + 4),(*(ushort *)(this + 0x5c) & 0x3fff) << 2,
                 (*(ushort *)(this + 0x5c) & 0x7fff) << 1,0x1b,*(short *)(this + 0x48),pPVar1);
    }
    AbyssEngine::PaintCanvas::TransformCreate(*(PaintCanvas **)(this + 4),(uint *)(this + 0x58));
    AbyssEngine::PaintCanvas::TransformAddMeshId
              (*(PaintCanvas **)(this + 4),*(uint *)(this + 0x58),*(uint *)(this + 0x54));
    sVar4 = 0;
    for (uVar5 = 0; uVar5 < *(uint *)(this + 0x3c); uVar5 = uVar5 + 1) {
      puVar3 = *(undefined4 **)(*(int *)(this + 0x40) + uVar5 * 4);
      (**(code **)*puVar3)(puVar3,*(undefined4 *)(this + 0x54),sVar4);
      sVar2 = (**(code **)(**(int **)(*(int *)(this + 0x40) + uVar5 * 4) + 0x10))();
      sVar4 = sVar4 + sVar2 * 4;
    }
  }
  return;
}

```

## target disasm
```
  001937a8: push {r4,r5,r6,r7,lr}
  001937aa: add r7,sp,#0xc
  001937ac: push.w r11
  001937b0: sub sp,#0x8
  001937b2: mov r4,r0
  001937b4: ldr r0,[r0,#0x3c]
  001937b6: cmp r0,#0x0
  001937b8: beq 0x0019385e
  001937ba: mov.w r0,#0xffffffff
  001937be: mov r1,r4
  001937c0: str.w r0,[r1,#0x54]!
  001937c4: mov r5,r1
  001937c6: str.w r0,[r5,#0x4]!
  001937ca: movw r0,#0xffff
  001937ce: ldrh.w r2,[r1,#-0xc]
  001937d2: cmp r2,r0
  001937d4: bne 0x00193806
  001937d6: ldrh.w r2,[r4,#0x4a]
  001937da: cmp r2,r0
  001937dc: beq 0x0019381e
  001937de: ldrh.w r2,[r4,#0x5c]
  001937e2: movs r3,#0x1b
  001937e4: ldr r0,[r4,#0x4]
  001937e6: str r1,[sp,#0x0]
  001937e8: lsls r1,r2,#0x2
  001937ea: lsls r2,r2,#0x1
  001937ec: uxth r1,r1
  001937ee: uxth r2,r2
  001937f0: blx 0x000789d0
  001937f4: ldrh.w r1,[r4,#0x4a]
  001937f8: add.w r2,r4,#0x50
  001937fc: ldr r0,[r4,#0x4]
  001937fe: movs r3,#0x0
  00193800: blx 0x0006fbb0
  00193804: b 0x0019381e
  00193806: ldrh.w r3,[r4,#0x5c]
  0019380a: ldr r0,[r4,#0x4]
  0019380c: strd r2,r1,[sp,#0x0]
  00193810: lsls r1,r3,#0x2
  00193812: lsls r2,r3,#0x1
  00193814: uxth r1,r1
  00193816: uxth r2,r2
  00193818: movs r3,#0x1b
  0019381a: blx 0x00075da8
  0019381e: ldr r0,[r4,#0x4]
  00193820: mov r1,r5
  00193822: blx 0x000720ac
  00193826: ldr r0,[r4,#0x4]
  00193828: ldrd r2,r1,[r4,#0x54]
  0019382c: blx 0x00073030
  00193830: movs r5,#0x0
  00193832: movs r6,#0x0
  00193834: b 0x00193858
  00193836: ldr r0,[r4,#0x40]
  00193838: ldr r1,[r4,#0x54]
  0019383a: ldr.w r0,[r0,r6,lsl #0x2]
  0019383e: ldr r2,[r0,#0x0]
  00193840: ldr r3,[r2,#0x0]
  00193842: uxth r2,r5
  00193844: blx r3
  00193846: ldr r0,[r4,#0x40]
  00193848: ldr.w r0,[r0,r6,lsl #0x2]
  0019384c: ldr r1,[r0,#0x0]
  0019384e: ldr r1,[r1,#0x10]
  00193850: blx r1
  00193852: add.w r5,r5,r0, lsl #0x2
  00193856: adds r6,#0x1
  00193858: ldr r0,[r4,#0x3c]
  0019385a: cmp r6,r0
  0019385c: bcc 0x00193836
  0019385e: add sp,#0x8
  00193860: pop.w r11
  00193864: pop {r4,r5,r6,r7,pc}
```
