# IParticleSystem::enableRender
elf 0x181da4 body 26
Sig: undefined __thiscall enableRender(IParticleSystem * this, bool param_1)

## decompile
```c

/* IParticleSystem::enableRender(bool) */

void __thiscall IParticleSystem::enableRender(IParticleSystem *this,bool param_1)

{
  if ((!param_1) && (this[0xd] != (IParticleSystem)0x0)) {
    (**(code **)(*(int *)this + 8))(this);
  }
  this[0xd] = (IParticleSystem)param_1;
  return;
}

```

## target disasm
```
  00191da4: push {r4,r5,r7,lr}
  00191da6: add r7,sp,#0x8
  00191da8: mov r4,r1
  00191daa: mov r5,r0
  00191dac: cbnz r1,0x00191dba
  00191dae: ldrb r0,[r5,#0xd]
  00191db0: cbz r0,0x00191dba
  00191db2: ldr r0,[r5,#0x0]
  00191db4: ldr r1,[r0,#0x8]
  00191db6: mov r0,r5
  00191db8: blx r1
  00191dba: strb r4,[r5,#0xd]
  00191dbc: pop {r4,r5,r7,pc}
```
