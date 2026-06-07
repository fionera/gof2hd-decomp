# ParticleSystemManager::addMeshSystem
elf 0x183a56 body 82
Sig: undefined __thiscall addMeshSystem(ParticleSystemManager * this, Matrix * param_1, Array * param_2, bool param_3)

## decompile
```c

/* ParticleSystemManager::addMeshSystem(AbyssEngine::AEMath::Matrix const*,
   Array<ParticleSettings::ParticleSet> const&, bool) */

uint __thiscall
ParticleSystemManager::addMeshSystem
          (ParticleSystemManager *this,Matrix *param_1,Array *param_2,bool param_3)

{
  ParticleSystemMesh *this_00;
  int iVar1;
  
  this_00 = operator_new(0xa0);
  ParticleSystemMesh::ParticleSystemMesh
            (this_00,*(PaintCanvas **)(this + 4),param_1,param_2,param_3,(bool)this[0x60]);
  ArrayAdd<ParticleSystemMesh*>(this_00,(Array *)(this + 0x3c));
  iVar1 = (**(code **)(*(int *)this_00 + 0x10))(this_00);
  *(int *)(this + 0x5c) = iVar1 + *(int *)(this + 0x5c);
  return *(int *)(this + 0x3c) - 1U | 0x4000;
}

```

## target disasm
```
  00193a56: push {r4,r5,r6,r7,lr}
  00193a58: add r7,sp,#0xc
  00193a5a: push {r6,r7,r8,r9,r11}
  00193a5e: mov r4,r0
  00193a60: movs r0,#0xa0
  00193a62: mov r6,r3
  00193a64: mov r8,r2
  00193a66: mov r9,r1
  00193a68: blx 0x0006eb24
  00193a6c: mov r5,r0
  00193a6e: ldr r1,[r4,#0x4]
  00193a70: ldrb.w r0,[r4,#0x60]
  00193a74: strd r6,r0,[sp,#0x0]
  00193a78: mov r0,r5
  00193a7a: mov r2,r9
  00193a7c: mov r3,r8
  00193a7e: blx 0x00078a48
  00193a82: add.w r1,r4,#0x3c
  00193a86: mov r0,r5
  00193a88: blx 0x00078a54
  00193a8c: ldr r0,[r5,#0x0]
  00193a8e: ldr r1,[r0,#0x10]
  00193a90: mov r0,r5
  00193a92: blx r1
  00193a94: ldr r2,[r4,#0x5c]
  00193a96: ldr r1,[r4,#0x3c]
  00193a98: add r0,r2
  00193a9a: str r0,[r4,#0x5c]
  00193a9c: subs r0,r1,#0x1
  00193a9e: orr r0,r0,#0x4000
  00193aa2: pop.w {r2,r3,r8,r9,r11}
  00193aa6: pop {r4,r5,r6,r7,pc}
```
