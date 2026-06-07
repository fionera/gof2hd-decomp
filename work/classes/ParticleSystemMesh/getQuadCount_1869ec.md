# ParticleSystemMesh::getQuadCount
elf 0x1869ec body 6
Sig: undefined __thiscall getQuadCount(ParticleSystemMesh * this)

## decompile
```c

/* ParticleSystemMesh::getQuadCount() */

int __thiscall ParticleSystemMesh::getQuadCount(ParticleSystemMesh *this)

{
  return *(int *)(this + 0x70) >> 2;
}

```

## target disasm
```
  001969ec: ldr r0,[r0,#0x70]
  001969ee: asrs r0,r0,#0x2
  001969f0: bx lr
```
