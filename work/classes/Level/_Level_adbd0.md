# Level::~Level
elf 0xadbd0 body 648
Sig: undefined __thiscall ~Level(Level * this)

## decompile
```c

/* Level::~Level() */

Level * __thiscall Level::~Level(Level *this)

{
  void *pvVar1;
  
  *(undefined4 *)(this + 4) = 0xffffffff;
  *(undefined4 *)(this + 8) = 0xffffffff;
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  if (*(Objective **)(this + 0x28) != (Objective *)0x0) {
    pvVar1 = (void *)Objective::~Objective(*(Objective **)(this + 0x28));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x28) = 0;
  if (*(Objective **)(this + 0x2c) != (Objective *)0x0) {
    pvVar1 = (void *)Objective::~Objective(*(Objective **)(this + 0x2c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x2c) = 0;
  if (*(BoundingVolume **)(this + 0xc4) != (BoundingVolume *)0x0) {
    pvVar1 = (void *)BoundingVolume::~BoundingVolume(*(BoundingVolume **)(this + 0xc4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xc4) = 0;
  if (*(int **)(this + 0xd8) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xd8) + 4))();
  }
  *(undefined4 *)(this + 0xd8) = 0;
  if (*(StarSystem **)(this + 0xec) != (StarSystem *)0x0) {
    pvVar1 = (void *)StarSystem::~StarSystem(*(StarSystem **)(this + 0xec));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xec) = 0;
  if (*(PlayerEgo **)(this + 0xf0) != (PlayerEgo *)0x0) {
    pvVar1 = (void *)PlayerEgo::~PlayerEgo(*(PlayerEgo **)(this + 0xf0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xf0) = 0;
  if (*(Route **)(this + 0x180) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(this + 0x180));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x180) = 0;
  if (*(ParticleSystemManager **)(this + 0x80) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x80));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x80) = 0;
  if (*(ParticleSystemManager **)(this + 0x88) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x88));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x88) = 0;
  if (*(ParticleSystemManager **)(this + 0x74) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x74));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x74) = 0;
  if (*(ParticleSystemManager **)(this + 0x78) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x78));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x78) = 0;
  if (*(ParticleSystemManager **)(this + 0x7c) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x7c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x7c) = 0;
  if (*(ParticleSystemManager **)(this + 0x90) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x90));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x90) = 0;
  if (*(ParticleSystemManager **)(this + 0x84) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x84));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x84) = 0;
  if (*(ParticleSystemManager **)(this + 0x98) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x98));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x98) = 0;
  if (*(ParticleSystemManager **)(this + 0x9c) != (ParticleSystemManager *)0x0) {
    pvVar1 = (void *)ParticleSystemManager::~ParticleSystemManager
                               (*(ParticleSystemManager **)(this + 0x9c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x9c) = 0;
  if (*(Array **)(this + 0xa4) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0xa4));
    if (*(Array<AEGeometry*> **)(this + 0xa4) != (Array<AEGeometry*> *)0x0) {
      pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0xa4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xa4) = 0;
  if (*(Array **)(this + 0xa8) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0xa8));
    if (*(Array<int> **)(this + 0xa8) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0xa8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xa8) = 0;
  if (*(Array **)(this + 0xe4) != (Array *)0x0) {
    ArrayReleaseClasses<AbstractGun*>(*(Array **)(this + 0xe4));
    if (*(Array<AbstractGun*> **)(this + 0xe4) != (Array<AbstractGun*> *)0x0) {
      pvVar1 = (void *)Array<AbstractGun*>::~Array(*(Array<AbstractGun*> **)(this + 0xe4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xe4) = 0;
  if (*(Array **)(this + 0xe8) != (Array *)0x0) {
    ArrayReleaseClasses<AbstractGun*>(*(Array **)(this + 0xe8));
    if (*(Array<AbstractGun*> **)(this + 0xe8) != (Array<AbstractGun*> *)0x0) {
      pvVar1 = (void *)Array<AbstractGun*>::~Array(*(Array<AbstractGun*> **)(this + 0xe8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xe8) = 0;
  if (*(Array **)(this + 0xf8) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0xf8));
    if (*(Array<KIPlayer*> **)(this + 0xf8) != (Array<KIPlayer*> *)0x0) {
      pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0xf8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xf8) = 0;
  if (*(Array **)(this + 0xfc) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0xfc));
    if (*(Array<KIPlayer*> **)(this + 0xfc) != (Array<KIPlayer*> *)0x0) {
      pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0xfc));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xfc) = 0;
  if (*(Array **)(this + 0xf4) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0xf4));
    if (*(Array<KIPlayer*> **)(this + 0xf4) != (Array<KIPlayer*> *)0x0) {
      pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0xf4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xf4) = 0;
  if (*(Array **)(this + 0x100) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0x100));
    if (*(Array<KIPlayer*> **)(this + 0x100) != (Array<KIPlayer*> *)0x0) {
      pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0x100));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x100) = 0;
  if (*(Array **)(this + 0x114) != (Array *)0x0) {
    ArrayReleaseClasses<RadioMessage*>(*(Array **)(this + 0x114));
    if (*(Array<RadioMessage*> **)(this + 0x114) != (Array<RadioMessage*> *)0x0) {
      pvVar1 = (void *)Array<RadioMessage*>::~Array(*(Array<RadioMessage*> **)(this + 0x114));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x114) = 0;
  if (*(Array **)(this + 0x104) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x104));
    if (*(Array<AEGeometry*> **)(this + 0x104) != (Array<AEGeometry*> *)0x0) {
      pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x104));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x104) = 0;
  if (*(LODManager **)this != (LODManager *)0x0) {
    pvVar1 = (void *)LODManager::~LODManager(*(LODManager **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  if (*(LodMeshMerger **)(this + 0xa0) != (LodMeshMerger *)0x0) {
    pvVar1 = (void *)LodMeshMerger::~LodMeshMerger(*(LodMeshMerger **)(this + 0xa0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xa0) = 0;
  if (*(Array<KIPlayer*> **)(this + 0xb0) != (Array<KIPlayer*> *)0x0) {
    pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0xb0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xb0) = 0;
  return this;
}

```
## target disasm
```
  000bdbd0: push {r4,r5,r7,lr}
  000bdbd2: add r7,sp,#0x8
  000bdbd4: mov r4,r0
  000bdbd6: ldr r0,[r0,#0x28]
  000bdbd8: mov.w r1,#0xffffffff
  000bdbdc: cmp r0,#0x0
  000bdbde: strd r1,r1,[r4,#0x4]
  000bdbe2: str r1,[r4,#0xc]
  000bdbe4: beq 0x000bdbee
  000bdbe6: blx 0x00073b34
  000bdbea: blx 0x0006eb48
  000bdbee: ldr r0,[r4,#0x2c]
  000bdbf0: movs r5,#0x0
  000bdbf2: str r5,[r4,#0x28]
  000bdbf4: cbz r0,0x000bdbfe
  000bdbf6: blx 0x00073b34
  000bdbfa: blx 0x0006eb48
  000bdbfe: ldr.w r0,[r4,#0xc4]
  000bdc02: str r5,[r4,#0x2c]
  000bdc04: cbz r0,0x000bdc0e
  000bdc06: blx 0x00073b40
  000bdc0a: blx 0x0006eb48
  000bdc0e: ldr.w r0,[r4,#0xd8]
  000bdc12: movs r5,#0x0
  000bdc14: str.w r5,[r4,#0xc4]
  000bdc18: cbz r0,0x000bdc20
  000bdc1a: ldr r1,[r0,#0x0]
  000bdc1c: ldr r1,[r1,#0x4]
  000bdc1e: blx r1
  000bdc20: ldr.w r0,[r4,#0xec]
  000bdc24: str.w r5,[r4,#0xd8]
  000bdc28: cbz r0,0x000bdc32
  000bdc2a: blx 0x00073b4c
  000bdc2e: blx 0x0006eb48
  000bdc32: ldr.w r0,[r4,#0xf0]
  000bdc36: movs r5,#0x0
  000bdc38: str.w r5,[r4,#0xec]
  000bdc3c: cbz r0,0x000bdc46
  000bdc3e: blx 0x00073b58
  000bdc42: blx 0x0006eb48
  000bdc46: ldr.w r0,[r4,#0x180]
  000bdc4a: str.w r5,[r4,#0xf0]
  000bdc4e: cbz r0,0x000bdc58
  000bdc50: blx 0x00072310
  000bdc54: blx 0x0006eb48
  000bdc58: ldr.w r0,[r4,#0x80]
  000bdc5c: movs r5,#0x0
  000bdc5e: str.w r5,[r4,#0x180]
  000bdc62: cbz r0,0x000bdc6c
  000bdc64: blx 0x00073b64
  000bdc68: blx 0x0006eb48
  000bdc6c: ldr.w r0,[r4,#0x88]
  000bdc70: str.w r5,[r4,#0x80]
  000bdc74: cbz r0,0x000bdc7e
  000bdc76: blx 0x00073b64
  000bdc7a: blx 0x0006eb48
  000bdc7e: ldr r0,[r4,#0x74]
  000bdc80: movs r5,#0x0
  000bdc82: str.w r5,[r4,#0x88]
  000bdc86: cbz r0,0x000bdc90
  000bdc88: blx 0x00073b64
  000bdc8c: blx 0x0006eb48
  000bdc90: ldr r0,[r4,#0x78]
  000bdc92: str r5,[r4,#0x74]
  000bdc94: cbz r0,0x000bdc9e
  000bdc96: blx 0x00073b64
  000bdc9a: blx 0x0006eb48
  000bdc9e: ldr r0,[r4,#0x7c]
  000bdca0: movs r5,#0x0
  000bdca2: str r5,[r4,#0x78]
  000bdca4: cbz r0,0x000bdcae
  000bdca6: blx 0x00073b64
  000bdcaa: blx 0x0006eb48
  000bdcae: ldr.w r0,[r4,#0x90]
  000bdcb2: str r5,[r4,#0x7c]
  000bdcb4: cbz r0,0x000bdcbe
  000bdcb6: blx 0x00073b64
  000bdcba: blx 0x0006eb48
  000bdcbe: ldr.w r0,[r4,#0x84]
  000bdcc2: movs r5,#0x0
  000bdcc4: str.w r5,[r4,#0x90]
  000bdcc8: cbz r0,0x000bdcd2
  000bdcca: blx 0x00073b64
  000bdcce: blx 0x0006eb48
  000bdcd2: ldr.w r0,[r4,#0x98]
  000bdcd6: str.w r5,[r4,#0x84]
  000bdcda: cbz r0,0x000bdce4
  000bdcdc: blx 0x00073b64
  000bdce0: blx 0x0006eb48
  000bdce4: ldr.w r0,[r4,#0x9c]
  000bdce8: movs r5,#0x0
  000bdcea: str.w r5,[r4,#0x98]
  000bdcee: cbz r0,0x000bdcf8
  000bdcf0: blx 0x00073b64
  000bdcf4: blx 0x0006eb48
  000bdcf8: ldr.w r0,[r4,#0xa4]
  000bdcfc: str.w r5,[r4,#0x9c]
  000bdd00: cbz r0,0x000bdd14
  000bdd02: blx 0x00071fec
  000bdd06: ldr.w r0,[r4,#0xa4]
  000bdd0a: cbz r0,0x000bdd14
  000bdd0c: blx 0x000717c4
  000bdd10: blx 0x0006eb48
  000bdd14: ldr.w r0,[r4,#0xa8]
  000bdd18: movs r5,#0x0
  000bdd1a: str.w r5,[r4,#0xa4]
  000bdd1e: cbz r0,0x000bdd32
  000bdd20: blx 0x0007369c
  000bdd24: ldr.w r0,[r4,#0xa8]
  000bdd28: cbz r0,0x000bdd32
  000bdd2a: blx 0x00070204
  000bdd2e: blx 0x0006eb48
  000bdd32: ldr.w r0,[r4,#0xe4]
  000bdd36: str.w r5,[r4,#0xa8]
  000bdd3a: cbz r0,0x000bdd4e
  000bdd3c: blx 0x00073b70
  000bdd40: ldr.w r0,[r4,#0xe4]
  000bdd44: cbz r0,0x000bdd4e
  000bdd46: blx 0x00073b7c
  000bdd4a: blx 0x0006eb48
  000bdd4e: ldr.w r0,[r4,#0xe8]
  000bdd52: movs r5,#0x0
  000bdd54: str.w r5,[r4,#0xe4]
  000bdd58: cbz r0,0x000bdd6c
  000bdd5a: blx 0x00073b70
  000bdd5e: ldr.w r0,[r4,#0xe8]
  000bdd62: cbz r0,0x000bdd6c
  000bdd64: blx 0x00073b7c
  000bdd68: blx 0x0006eb48
  000bdd6c: ldr.w r0,[r4,#0xf8]
  000bdd70: str.w r5,[r4,#0xe8]
  000bdd74: cbz r0,0x000bdd88
  000bdd76: blx 0x00073b88
  000bdd7a: ldr.w r0,[r4,#0xf8]
  000bdd7e: cbz r0,0x000bdd88
  000bdd80: blx 0x00073b94
  000bdd84: blx 0x0006eb48
  000bdd88: ldr.w r0,[r4,#0xfc]
  000bdd8c: movs r5,#0x0
  000bdd8e: str.w r5,[r4,#0xf8]
  000bdd92: cbz r0,0x000bdda6
  000bdd94: blx 0x00073b88
  000bdd98: ldr.w r0,[r4,#0xfc]
  000bdd9c: cbz r0,0x000bdda6
  000bdd9e: blx 0x00073b94
  000bdda2: blx 0x0006eb48
  000bdda6: ldr.w r0,[r4,#0xf4]
  000bddaa: str.w r5,[r4,#0xfc]
  000bddae: cbz r0,0x000bddc2
  000bddb0: blx 0x00073b88
  000bddb4: ldr.w r0,[r4,#0xf4]
  000bddb8: cbz r0,0x000bddc2
  000bddba: blx 0x00073b94
  000bddbe: blx 0x0006eb48
  000bddc2: ldr.w r0,[r4,#0x100]
  000bddc6: movs r5,#0x0
  000bddc8: str.w r5,[r4,#0xf4]
  000bddcc: cbz r0,0x000bdde0
  000bddce: blx 0x00073b88
  000bddd2: ldr.w r0,[r4,#0x100]
  000bddd6: cbz r0,0x000bdde0
  000bddd8: blx 0x00073b94
  000bdddc: blx 0x0006eb48
  000bdde0: ldr.w r0,[r4,#0x114]
  000bdde4: str.w r5,[r4,#0x100]
  000bdde8: cbz r0,0x000bddfc
  000bddea: blx 0x00073ba0
  000bddee: ldr.w r0,[r4,#0x114]
  000bddf2: cbz r0,0x000bddfc
  000bddf4: blx 0x00073bac
  000bddf8: blx 0x0006eb48
  000bddfc: ldr.w r0,[r4,#0x104]
  000bde00: movs r5,#0x0
  000bde02: str.w r5,[r4,#0x114]
  000bde06: cbz r0,0x000bde1a
  000bde08: blx 0x00071fec
  000bde0c: ldr.w r0,[r4,#0x104]
  000bde10: cbz r0,0x000bde1a
  000bde12: blx 0x000717c4
  000bde16: blx 0x0006eb48
  000bde1a: ldr r0,[r4,#0x0]
  000bde1c: str.w r5,[r4,#0x104]
  000bde20: cbz r0,0x000bde2a
  000bde22: blx 0x00073bb8
  000bde26: blx 0x0006eb48
  000bde2a: ldr.w r0,[r4,#0xa0]
  000bde2e: movs r5,#0x0
  000bde30: str r5,[r4,#0x0]
  000bde32: cbz r0,0x000bde3c
  000bde34: blx 0x00073bc4
  000bde38: blx 0x0006eb48
  000bde3c: ldr.w r0,[r4,#0xb0]
  000bde40: str.w r5,[r4,#0xa0]
  000bde44: cbz r0,0x000bde4e
  000bde46: blx 0x00073b94
  000bde4a: blx 0x0006eb48
  000bde4e: movs r0,#0x0
  000bde50: str.w r0,[r4,#0xb0]
  000bde54: mov r0,r4
  000bde56: pop {r4,r5,r7,pc}
```
