# ParticleSystemManager::ParticleSystemManager
elf 0x18357c body 86
Sig: undefined __thiscall ParticleSystemManager(ParticleSystemManager * this, PaintCanvas * param_1, CameraSet param_2, ushort param_3, bool param_4, ushort param_5, bool param_6)

## decompile
```c

/* ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*,
   ParticleSettings::CameraSet, unsigned short, bool, unsigned short, bool) */

ParticleSystemManager * __thiscall
ParticleSystemManager::ParticleSystemManager
          (ParticleSystemManager *this,undefined4 param_1,undefined4 param_3,undefined2 param_4,
          ParticleSystemManager param_5,undefined2 param_6,ParticleSystemManager param_7)

{
  *(undefined4 *)(this + 0xc) = param_3;
  *(undefined4 *)(this + 4) = param_1;
  Array<ParticleSystemSprite*>::Array((Array<ParticleSystemSprite*> *)(this + 0x18));
  *(undefined2 *)(this + 0x26) = 0xffff;
  *(undefined2 *)(this + 0x24) = param_4;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x38] = param_5;
  Array<ParticleSystemMesh*>::Array((Array<ParticleSystemMesh*> *)(this + 0x3c));
  *(undefined2 *)(this + 0x4a) = 0xffff;
  *(undefined2 *)(this + 0x48) = param_6;
  *(undefined4 *)(this + 0x4c) = 0;
  this[0x60] = param_7;
  construct(this);
  return this;
}

```

## target disasm
```
  0019357c: push {r4,r5,r6,r7,lr}
  0019357e: add r7,sp,#0xc
  00193580: push.w r8
  00193584: add.w r5,r0,#0x18
  00193588: str r2,[r0,#0xc]
  0019358a: mov r4,r0
  0019358c: str r1,[r0,#0x4]
  0019358e: mov r0,r5
  00193590: mov r6,r3
  00193592: blx 0x00078964
  00193596: ldr r0,[r7,#0x8]
  00193598: movw r8,#0xffff
  0019359c: strh.w r8,[r4,#0x26]
  001935a0: strh r6,[r4,#0x24]
  001935a2: movs r6,#0x0
  001935a4: str r6,[r4,#0x28]
  001935a6: strb.w r0,[r4,#0x38]
  001935aa: add.w r0,r4,#0x3c
  001935ae: blx 0x00078970
  001935b2: ldrd r1,r0,[r7,#0xc]
  001935b6: strh.w r8,[r4,#0x4a]
  001935ba: strh.w r1,[r4,#0x48]
  001935be: str r6,[r4,#0x4c]
  001935c0: strb.w r0,[r4,#0x60]
  001935c4: mov r0,r4
  001935c6: blx 0x0007897c
  001935ca: mov r0,r4
  001935cc: pop.w r8
  001935d0: pop {r4,r5,r6,r7,pc}
```
