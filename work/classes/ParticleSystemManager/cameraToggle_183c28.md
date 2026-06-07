# ParticleSystemManager::cameraToggle
elf 0x183c28 body 34
Sig: undefined __thiscall cameraToggle(ParticleSystemManager * this, CameraSet param_1)

## decompile
```c

/* ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet) */

void __thiscall ParticleSystemManager::cameraToggle(ParticleSystemManager *this,int param_2)

{
  if (*(int *)(this + 0xc) != param_2) {
    *(int *)(this + 0xc) = param_2;
    releaseSprites(this);
    (*(code *)(DAT_001ac954 + 0x1ac958))(this);
    return;
  }
  return;
}

```

## target disasm
```
  00193c28: push {r4,r6,r7,lr}
  00193c2a: add r7,sp,#0x8
  00193c2c: mov r4,r0
  00193c2e: ldr r0,[r0,#0xc]
  00193c30: cmp r0,r1
  00193c32: it eq
  00193c34: pop.eq {r4,r6,r7,pc}
  00193c36: mov r0,r4
  00193c38: str r1,[r4,#0xc]
  00193c3a: blx 0x00078ab4
  00193c3e: mov r0,r4
  00193c40: pop.w {r4,r6,r7,lr}
  00193c44: b.w 0x001ac948
  001ac948: bx pc
```
