# ParticleSystemMesh::render
elf 0x1861b2 body 12
Sig: undefined __stdcall render(PaintCanvas * param_1, uint param_2)

## decompile
```c

/* ParticleSystemMesh::render(AbyssEngine::PaintCanvas*, unsigned int) */

void ParticleSystemMesh::render(PaintCanvas *param_1,uint param_2)

{
  if (param_2 != 0xffffffff) {
    (*(code *)(DAT_001abda4 + 0x1abda8))(param_1,param_2,0);
    return;
  }
  return;
}

```

## target disasm
```
  001961b2: adds r2,r1,#0x1
  001961b4: itt ne
  001961b6: mov.ne r2,#0x0
  001961b8: b.ne.w 0x001abd98
  001961bc: bx lr
```
