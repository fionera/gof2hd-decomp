# ParticleSystemMesh::incId
elf 0x1855e6 body 16
Sig: undefined __thiscall incId(ParticleSystemMesh * this)

## decompile
```c

/* ParticleSystemMesh::incId() */

void __thiscall ParticleSystemMesh::incId(ParticleSystemMesh *this)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x50) + 1;
  if (*(int *)(this + 0x48) <= iVar1) {
    iVar1 = 0;
  }
  *(int *)(this + 0x50) = iVar1;
  return;
}

```

## target disasm
```
  001955e6: ldr r2,[r0,#0x50]
  001955e8: ldr r1,[r0,#0x48]
  001955ea: adds r2,#0x1
  001955ec: cmp r2,r1
  001955ee: it ge
  001955f0: mov.ge r2,#0x0
  001955f2: str r2,[r0,#0x50]
  001955f4: bx lr
```
