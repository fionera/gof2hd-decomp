# ParticleSystemManager::render3d
elf 0x183c68 body 38
Sig: undefined __thiscall render3d(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::render3d() */

void __thiscall ParticleSystemManager::render3d(ParticleSystemManager *this)

{
  if (this[0x14] != (ParticleSystemManager)0x0) {
    if (this[1] != (ParticleSystemManager)0x0) {
      renderMeshes(this);
    }
    if (*this != (ParticleSystemManager)0x0) {
      (*(code *)(DAT_001ac964 + 0x1ac968))(this);
      return;
    }
  }
  return;
}

```

## target disasm
```
  00193c68: push {r4,r6,r7,lr}
  00193c6a: add r7,sp,#0x8
  00193c6c: mov r4,r0
  00193c6e: ldrb r0,[r0,#0x14]
  00193c70: cbz r0,0x00193c8a
  00193c72: ldrb r0,[r4,#0x1]
  00193c74: cbz r0,0x00193c7c
  00193c76: mov r0,r4
  00193c78: blx 0x00078ae4
  00193c7c: ldrb r0,[r4,#0x0]
  00193c7e: cbz r0,0x00193c8a
  00193c80: mov r0,r4
  00193c82: pop.w {r4,r6,r7,lr}
  00193c86: b.w 0x001ac958
  00193c8a: pop {r4,r6,r7,pc}
  001ac958: bx pc
```
