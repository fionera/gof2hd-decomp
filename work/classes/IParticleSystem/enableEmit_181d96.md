# IParticleSystem::enableEmit
elf 0x181d96 body 14
Sig: undefined __thiscall enableEmit(IParticleSystem * this, bool param_1)

## decompile
```c

/* IParticleSystem::enableEmit(bool) */

void __thiscall IParticleSystem::enableEmit(IParticleSystem *this,bool param_1)

{
  if ((param_1) && (this[0xc] == (IParticleSystem)0x0)) {
    *(undefined4 *)(this + 0x60) = 0;
  }
  this[0xc] = (IParticleSystem)param_1;
  return;
}

```

## target disasm
```
  00191d96: cbz r1,0x00191da0
  00191d98: ldrb r2,[r0,#0xc]
  00191d9a: cbnz r2,0x00191da0
  00191d9c: movs r2,#0x0
  00191d9e: str r2,[r0,#0x60]
  00191da0: strb r1,[r0,#0xc]
  00191da2: bx lr
```
