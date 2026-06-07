# ParticleSystemMesh::~ParticleSystemMesh
elf 0x1855c4 body 16
Sig: undefined __thiscall ~ParticleSystemMesh(ParticleSystemMesh * this)

## decompile
```c

/* ParticleSystemMesh::~ParticleSystemMesh() */

void __thiscall ParticleSystemMesh::~ParticleSystemMesh(ParticleSystemMesh *this)

{
  *(int *)this = *(int *)(DAT_001955d4 + 0x1955ca) + 8;
  (*(code *)(DAT_001ac8b4 + 0x1ac8b8))();
  return;
}

```

## target disasm
```
  001955c4: ldr r1,[0x001955d4]
  001955c6: add r1,pc
  001955c8: ldr r1,[r1,#0x0]
  001955ca: adds r1,#0x8
  001955cc: str r1,[r0,#0x0]
  001955ce: b.w 0x001ac8a8
  001ac8a8: bx pc
```
