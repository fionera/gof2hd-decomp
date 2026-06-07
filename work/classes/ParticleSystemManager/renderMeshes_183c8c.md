# ParticleSystemManager::renderMeshes
elf 0x183c8c body 50
Sig: undefined __thiscall renderMeshes(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::renderMeshes() */

void __thiscall ParticleSystemManager::renderMeshes(ParticleSystemManager *this)

{
  if (*(short *)(this + 0x48) != -1) {
    (*(code *)(DAT_001ac984 + 0x1ac988))(*(undefined4 *)(this + 4),*(undefined4 *)(this + 0x58));
    return;
  }
  if (*(short *)(this + 0x26) != -1) {
    (*(code *)(DAT_001ac974 + 0x1ac978))
              (*(undefined4 *)(this + 4),*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x50),
               *(undefined4 *)(this + 0x4c));
    return;
  }
  return;
}

```

## target disasm
```
  00193c8c: ldrh.w r2,[r0,#0x48]
  00193c90: movw r1,#0xffff
  00193c94: cmp r2,r1
  00193c96: bne 0x00193cb0
  00193c98: ldrh r2,[r0,#0x26]
  00193c9a: cmp r2,r1
  00193c9c: it eq
  00193c9e: bx.eq lr
  00193ca0: ldr.w r12,[r0,#0x4]
  00193ca4: ldrd r3,r2,[r0,#0x4c]
  00193ca8: ldr r1,[r0,#0x58]
  00193caa: mov r0,r12
  00193cac: b.w 0x001ac968
  00193cb0: ldr r2,[r0,#0x4]
  00193cb2: ldr r1,[r0,#0x58]
  00193cb4: mov r0,r2
  00193cb6: b.w 0x001ac978
  001ac968: bx pc
  001ac978: bx pc
```
