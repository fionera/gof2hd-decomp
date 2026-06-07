# ParticleSystemManager::addSpriteSystem
elf 0x1839fc body 76
Sig: undefined __thiscall addSpriteSystem(ParticleSystemManager * this, Matrix * param_1, Array * param_2, bool param_3)

## decompile
```c

/* ParticleSystemManager::addSpriteSystem(AbyssEngine::AEMath::Matrix const*,
   Array<ParticleSettings::ParticleSet> const&, bool) */

int __thiscall
ParticleSystemManager::addSpriteSystem
          (ParticleSystemManager *this,Matrix *param_1,Array *param_2,bool param_3)

{
  ParticleSystemSprite *this_00;
  int iVar1;
  
  this_00 = operator_new(0x78);
  ParticleSystemSprite::ParticleSystemSprite
            (this_00,*(PaintCanvas **)(this + 4),param_1,param_2,param_3,(bool)this[0x38]);
  ArrayAdd<ParticleSystemSprite*>(this_00,(Array *)(this + 0x18));
  iVar1 = IParticleSystem::getParticleCount((IParticleSystem *)this_00);
  *(int *)(this + 0x34) = iVar1 + *(int *)(this + 0x34);
  return *(int *)(this + 0x18) + -1;
}

```

## target disasm
```
  001939fc: push {r4,r5,r6,r7,lr}
  001939fe: add r7,sp,#0xc
  00193a00: push {r6,r7,r8,r9,r11}
  00193a04: mov r4,r0
  00193a06: movs r0,#0x78
  00193a08: mov r6,r3
  00193a0a: mov r8,r2
  00193a0c: mov r9,r1
  00193a0e: blx 0x0006eb24
  00193a12: mov r5,r0
  00193a14: ldr r1,[r4,#0x4]
  00193a16: ldrb.w r0,[r4,#0x38]
  00193a1a: strd r6,r0,[sp,#0x0]
  00193a1e: mov r0,r5
  00193a20: mov r2,r9
  00193a22: mov r3,r8
  00193a24: blx 0x00078a30
  00193a28: add.w r1,r4,#0x18
  00193a2c: mov r0,r5
  00193a2e: blx 0x00078a3c
  00193a32: mov r0,r5
  00193a34: blx 0x000789c4
  00193a38: ldr r2,[r4,#0x34]
  00193a3a: ldr r1,[r4,#0x18]
  00193a3c: add r0,r2
  00193a3e: str r0,[r4,#0x34]
  00193a40: subs r0,r1,#0x1
  00193a42: pop.w {r2,r3,r8,r9,r11}
  00193a46: pop {r4,r5,r6,r7,pc}
```
