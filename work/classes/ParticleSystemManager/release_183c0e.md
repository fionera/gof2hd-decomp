# ParticleSystemManager::release
elf 0x183c0e body 28
Sig: undefined __thiscall release(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::release() */

void __thiscall ParticleSystemManager::release(ParticleSystemManager *this)

{
  releaseSprites(this);
  *(undefined4 *)(this + 4) = 0;
  (*(code *)(DAT_001ac944 + 0x1ac948))(this + 0x3c);
  return;
}

```

## target disasm
```
  00193c0e: push {r4,r6,r7,lr}
  00193c10: add r7,sp,#0x8
  00193c12: mov r4,r0
  00193c14: blx 0x00078ab4
  00193c18: movs r0,#0x0
  00193c1a: str r0,[r4,#0x4]
  00193c1c: add.w r0,r4,#0x3c
  00193c20: pop.w {r4,r6,r7,lr}
  00193c24: b.w 0x001ac938
  001ac938: bx pc
```
