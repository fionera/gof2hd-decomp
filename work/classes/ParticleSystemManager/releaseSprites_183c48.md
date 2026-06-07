# ParticleSystemManager::releaseSprites
elf 0x183c48 body 32
Sig: undefined __thiscall releaseSprites(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::releaseSprites() */

void __thiscall ParticleSystemManager::releaseSprites(ParticleSystemManager *this)

{
  ArrayReleaseClasses<ParticleSystemSprite*>((Array *)(this + 0x18));
  if (*(uint *)(this + 0x30) != 0xffffffff) {
    AbyssEngine::PaintCanvas::ReleaseSpriteSystemResource
              (*(PaintCanvas **)(this + 4),*(uint *)(this + 0x30));
    *(undefined4 *)(this + 0x30) = 0xffffffff;
  }
  return;
}

```

## target disasm
```
  00193c48: push {r4,r6,r7,lr}
  00193c4a: add r7,sp,#0x8
  00193c4c: mov r4,r0
  00193c4e: adds r0,#0x18
  00193c50: blx 0x00078acc
  00193c54: ldr r1,[r4,#0x30]
  00193c56: adds r0,r1,#0x1
  00193c58: beq 0x00193c66
  00193c5a: ldr r0,[r4,#0x4]
  00193c5c: blx 0x00078ad8
  00193c60: mov.w r0,#0xffffffff
  00193c64: str r0,[r4,#0x30]
  00193c66: pop {r4,r6,r7,pc}
```
