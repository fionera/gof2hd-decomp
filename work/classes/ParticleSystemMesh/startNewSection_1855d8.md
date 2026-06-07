# ParticleSystemMesh::startNewSection
elf 0x1855d8 body 8
Sig: undefined __thiscall startNewSection(ParticleSystemMesh * this)

## decompile
```c

/* ParticleSystemMesh::startNewSection() */

void __thiscall ParticleSystemMesh::startNewSection(ParticleSystemMesh *this)

{
  this[0x90] = (ParticleSystemMesh)0x1;
  return;
}

```

## target disasm
```
  001955d8: movs r1,#0x1
  001955da: strb.w r1,[r0,#0x90]
  001955de: bx lr
```
