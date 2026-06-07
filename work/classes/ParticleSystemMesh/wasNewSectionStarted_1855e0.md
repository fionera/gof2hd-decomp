# ParticleSystemMesh::wasNewSectionStarted
elf 0x1855e0 body 6
Sig: undefined __thiscall wasNewSectionStarted(ParticleSystemMesh * this)

## decompile
```c

/* ParticleSystemMesh::wasNewSectionStarted() */

ParticleSystemMesh __thiscall ParticleSystemMesh::wasNewSectionStarted(ParticleSystemMesh *this)

{
  return this[0x90];
}

```

## target disasm
```
  001955e0: ldrb.w r0,[r0,#0x90]
  001955e4: bx lr
```
