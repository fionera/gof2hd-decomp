# ParticleSystemManager::initSprites
elf 0x1836e8 body 188
Sig: undefined __thiscall initSprites(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::initSprites() */

void __thiscall ParticleSystemManager::initSprites(ParticleSystemManager *this)

{
  ParticleSystemManager *pPVar1;
  short sVar2;
  undefined4 *puVar3;
  code *pcVar4;
  short sVar5;
  uint uVar6;
  float fVar7;
  float extraout_s1;
  float fVar8;
  float extraout_s3;
  
  if (*(int *)(this + 0x18) != 0) {
    pPVar1 = this + 0x30;
    *(uint *)pPVar1 = 0xffffffff;
    if (*(int *)(this + 0xc) != 0) {
      if (*(ushort *)(this + 0x24) == 0xffff) {
        if (*(short *)(this + 0x26) != -1) {
          AbyssEngine::PaintCanvas::SpriteSystemCreate
                    (*(PaintCanvas **)(this + 4),*(ushort *)(this + 0x34),false,(uint *)pPVar1);
          AbyssEngine::PaintCanvas::TextureCreate
                    ((ushort)*(undefined4 *)(this + 4),(uint *)(uint)*(ushort *)(this + 0x26),
                     (bool)((char)this + ','));
        }
      }
      else {
        AbyssEngine::PaintCanvas::SpriteSystemCreate
                  (*(PaintCanvas **)(this + 4),*(ushort *)(this + 0x34),false,
                   *(ushort *)(this + 0x24),(uint *)pPVar1);
      }
      sVar5 = 0;
      AbyssEngine::PaintCanvas::SpriteSystemSetAllSize
                (*(uint *)(this + 4),(short)*(undefined4 *)(this + 0x30));
      fVar7 = *(float *)(*(int *)(DAT_001937a4 + 0x19375a) + 0x90);
      fVar8 = *(float *)(*(int *)(DAT_001937a4 + 0x19375a) + 0x94);
      AbyssEngine::PaintCanvas::SpriteSystemSetAllUv
                (*(PaintCanvas **)(this + 4),*(uint *)(this + 0x30),fVar7,extraout_s1,fVar8,
                 extraout_s3);
      for (uVar6 = 0; uVar6 < *(uint *)(this + 0x18); uVar6 = uVar6 + 1) {
        puVar3 = *(undefined4 **)(*(int *)(this + 0x1c) + uVar6 * 4);
        pcVar4 = *(code **)*puVar3;
        (*pcVar4)(puVar3,*(undefined4 *)(this + 0x30),sVar5,pcVar4,fVar7,fVar8);
        sVar2 = IParticleSystem::getParticleCount
                          (*(IParticleSystem **)(*(int *)(this + 0x1c) + uVar6 * 4));
        sVar5 = sVar5 + sVar2;
      }
    }
  }
  return;
}

```

## target disasm
```
  001936e8: push {r4,r5,r6,r7,lr}
  001936ea: add r7,sp,#0xc
  001936ec: push.w r11
  001936f0: sub sp,#0x8
  001936f2: mov r4,r0
  001936f4: ldr r0,[r0,#0x18]
  001936f6: cmp r0,#0x0
  001936f8: beq 0x0019379c
  001936fa: mov.w r0,#0xffffffff
  001936fe: mov r3,r4
  00193700: str.w r0,[r3,#0x30]!
  00193704: ldr.w r0,[r3,#-0x24]
  00193708: cmp r0,#0x0
  0019370a: beq 0x0019379c
  0019370c: ldrh.w r12,[r4,#0x24]
  00193710: movw r0,#0xffff
  00193714: cmp r12,r0
  00193716: bne 0x00193738
  00193718: ldrh r1,[r4,#0x26]
  0019371a: cmp r1,r0
  0019371c: beq 0x00193746
  0019371e: ldrh r1,[r4,#0x34]
  00193720: movs r2,#0x0
  00193722: ldr r0,[r4,#0x4]
  00193724: blx 0x000778b4
  00193728: ldrh r1,[r4,#0x26]
  0019372a: add.w r2,r4,#0x2c
  0019372e: ldr r0,[r4,#0x4]
  00193730: movs r3,#0x0
  00193732: blx 0x0006fbb0
  00193736: b 0x00193746
  00193738: ldrh r1,[r4,#0x34]
  0019373a: movs r2,#0x0
  0019373c: ldr r0,[r4,#0x4]
  0019373e: str r3,[sp,#0x0]
  00193740: mov r3,r12
  00193742: blx 0x000789ac
  00193746: ldr r0,[r4,#0x4]
  00193748: movs r2,#0x0
  0019374a: ldr r1,[r4,#0x30]
  0019374c: movs r5,#0x0
  0019374e: blx 0x000789b8
  00193752: ldr r0,[0x001937a4]
  00193754: ldr r1,[r4,#0x30]
  00193756: add r0,pc
  00193758: ldr r3,[r0,#0x0]
  0019375a: ldr r0,[r4,#0x4]
  0019375c: vldr.32 s0,[r3,#0x90]
  00193760: vldr.32 s2,[r3,#0x94]
  00193764: ldrd r2,r3,[r3,#0x88]
  00193768: vstr.32 s0,[sp]
  0019376c: vstr.32 s2,[sp,#0x4]
  00193770: blx 0x000778c0
  00193774: movs r6,#0x0
  00193776: b 0x00193796
  00193778: ldr r0,[r4,#0x1c]
  0019377a: ldr r1,[r4,#0x30]
  0019377c: ldr.w r0,[r0,r6,lsl #0x2]
  00193780: ldr r2,[r0,#0x0]
  00193782: ldr r3,[r2,#0x0]
  00193784: uxth r2,r5
  00193786: blx r3
  00193788: ldr r0,[r4,#0x1c]
  0019378a: ldr.w r0,[r0,r6,lsl #0x2]
  0019378e: blx 0x000789c4
  00193792: add r5,r0
  00193794: adds r6,#0x1
  00193796: ldr r0,[r4,#0x18]
  00193798: cmp r6,r0
  0019379a: bcc 0x00193778
  0019379c: add sp,#0x8
  0019379e: pop.w r11
  001937a2: pop {r4,r5,r6,r7,pc}
```
