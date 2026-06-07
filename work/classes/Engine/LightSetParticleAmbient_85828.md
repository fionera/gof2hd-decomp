# Engine::LightSetParticleAmbient
elf 0x85828 body 8
Sig: undefined __thiscall LightSetParticleAmbient(Engine * this, float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetParticleAmbient(float, float, float) */

Engine * __thiscall
AbyssEngine::Engine::LightSetParticleAmbient(Engine *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  
  *(undefined4 *)(this + 0x314) = in_r1;
  *(undefined4 *)(this + 0x318) = in_r2;
  *(undefined4 *)(this + 0x31c) = in_r3;
  return this + 800;
}

```

## target disasm
```
  00095828: add.w r0,r0,#0x314
  0009582c: stmia r0!,{r1,r2,r3}
  0009582e: bx lr
```
