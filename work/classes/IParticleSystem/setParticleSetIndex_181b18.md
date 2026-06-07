# IParticleSystem::setParticleSetIndex
elf 0x181b18 body 6
Sig: undefined __thiscall setParticleSetIndex(IParticleSystem * this, uchar param_1)

## decompile
```c

/* IParticleSystem::setParticleSetIndex(unsigned char) */

void __thiscall IParticleSystem::setParticleSetIndex(IParticleSystem *this,uchar param_1)

{
  this[0x44] = (IParticleSystem)param_1;
  return;
}

```

## target disasm
```
  00191b18: strb.w r1,[r0,#0x44]
  00191b1c: bx lr
```
