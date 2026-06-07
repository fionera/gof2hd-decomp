# ParticleSystemSprite::~ParticleSystemSprite
elf 0x182c58 body 32
Sig: undefined __thiscall ~ParticleSystemSprite(ParticleSystemSprite * this)

## decompile
```c

/* ParticleSystemSprite::~ParticleSystemSprite() */

void __thiscall ParticleSystemSprite::~ParticleSystemSprite(ParticleSystemSprite *this)

{
  *(int *)this = *(int *)(DAT_00192c78 + 0x192c64) + 8;
  release(this);
  (*(code *)(DAT_001ac8b4 + 0x1ac8b8))(this);
  return;
}

```

## target disasm
```
  00192c58: push {r4,r6,r7,lr}
  00192c5a: add r7,sp,#0x8
  00192c5c: mov r4,r0
  00192c5e: ldr r0,[0x00192c78]
  00192c60: add r0,pc
  00192c62: ldr r0,[r0,#0x0]
  00192c64: adds r0,#0x8
  00192c66: str r0,[r4,#0x0]
  00192c68: mov r0,r4
  00192c6a: blx 0x000788bc
  00192c6e: mov r0,r4
  00192c70: pop.w {r4,r6,r7,lr}
  00192c74: b.w 0x001ac8a8
```
