# ParticleSystemManager::ParticleSystemManager
elf 0x183662 body 86
Sig: undefined __thiscall ParticleSystemManager(ParticleSystemManager * this, PaintCanvas * param_1, CameraSet param_2, ushort param_3, BlendMode param_4, bool param_5, ushort param_6, BlendMode param_7, bool param_8)

## decompile
```c

/* ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*,
   ParticleSettings::CameraSet, unsigned short, AbyssEngine::BlendMode, bool, unsigned short,
   AbyssEngine::BlendMode, bool) */

ParticleSystemManager * __thiscall
ParticleSystemManager::ParticleSystemManager
          (ParticleSystemManager *this,undefined4 param_1,undefined4 param_3,undefined2 param_4,
          undefined4 param_5,ParticleSystemManager param_6,undefined2 param_7,undefined4 param_8,
          ParticleSystemManager param_9)

{
  *(undefined4 *)(this + 0xc) = param_3;
  *(undefined4 *)(this + 4) = param_1;
  Array<ParticleSystemSprite*>::Array((Array<ParticleSystemSprite*> *)(this + 0x18));
  *(undefined2 *)(this + 0x26) = param_4;
  *(undefined2 *)(this + 0x24) = 0xffff;
  *(undefined4 *)(this + 0x28) = param_5;
  this[0x38] = param_6;
  Array<ParticleSystemMesh*>::Array((Array<ParticleSystemMesh*> *)(this + 0x3c));
  *(undefined2 *)(this + 0x4a) = param_7;
  *(undefined2 *)(this + 0x48) = 0xffff;
  *(undefined4 *)(this + 0x4c) = param_8;
  this[0x60] = param_9;
  construct(this);
  return this;
}

```

## target disasm
```
  00193662: push {r4,r5,r6,r7,lr}
  00193664: add r7,sp,#0xc
  00193666: push.w r11
  0019366a: add.w r5,r0,#0x18
  0019366e: str r2,[r0,#0xc]
  00193670: mov r4,r0
  00193672: str r1,[r0,#0x4]
  00193674: mov r0,r5
  00193676: mov r6,r3
  00193678: blx 0x00078964
  0019367c: ldrd r1,r0,[r7,#0x8]
  00193680: strh r6,[r4,#0x26]
  00193682: movw r6,#0xffff
  00193686: strh r6,[r4,#0x24]
  00193688: str r1,[r4,#0x28]
  0019368a: strb.w r0,[r4,#0x38]
  0019368e: add.w r0,r4,#0x3c
  00193692: blx 0x00078970
  00193696: ldrd r2,r1,[r7,#0x10]
  0019369a: ldr r0,[r7,#0x18]
  0019369c: strh.w r2,[r4,#0x4a]
  001936a0: strh.w r6,[r4,#0x48]
  001936a4: str r1,[r4,#0x4c]
  001936a6: strb.w r0,[r4,#0x60]
  001936aa: mov r0,r4
  001936ac: blx 0x0007897c
  001936b0: mov r0,r4
  001936b2: pop.w r11
  001936b6: pop {r4,r5,r6,r7,pc}
```
