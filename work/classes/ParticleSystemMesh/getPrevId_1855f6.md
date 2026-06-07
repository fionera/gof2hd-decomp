# ParticleSystemMesh::getPrevId
elf 0x1855f6 body 8
Sig: undefined __thiscall getPrevId(ParticleSystemMesh * this, int param_1)

## decompile
```c

/* ParticleSystemMesh::getPrevId(int) */

int __thiscall ParticleSystemMesh::getPrevId(ParticleSystemMesh *this,int param_1)

{
  if (param_1 == 0) {
    param_1 = *(int *)(this + 0x48);
  }
  return param_1 + -1;
}

```

## target disasm
```
  001955f6: cbnz r1,0x001955fa
  001955f8: ldr r1,[r0,#0x48]
  001955fa: subs r0,r1,#0x1
  001955fc: bx lr
```
