# IParticleSystem::setParticleSet
elf 0x181b1e body 20
Sig: undefined __thiscall setParticleSet(IParticleSystem * this, ParticleSet param_1)

## decompile
```c

/* IParticleSystem::setParticleSet(ParticleSettings::ParticleSet) */

void __thiscall IParticleSystem::setParticleSet(IParticleSystem *this,int param_2)

{
  if ((*(int *)(this + 0x38) != 0) && (**(int **)(this + 0x3c) == param_2)) {
    this[0x44] = (IParticleSystem)0x0;
  }
  return;
}

```

## target disasm
```
  00191b1e: ldr r2,[r0,#0x38]
  00191b20: cbz r2,0x00191b30
  00191b22: ldr r2,[r0,#0x3c]
  00191b24: ldr r2,[r2,#0x0]
  00191b26: cmp r2,r1
  00191b28: itt eq
  00191b2a: mov.eq r1,#0x0
  00191b2c: strb.eq.w r1,[r0,#0x44]
  00191b30: bx lr
```
