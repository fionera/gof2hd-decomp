# ParticleSettings::~ParticleSettings
elf 0x185474 body 28
Sig: undefined __thiscall ~ParticleSettings(ParticleSettings * this)

## decompile
```c

/* ParticleSettings::~ParticleSettings() */

ParticleSettings * __thiscall ParticleSettings::~ParticleSettings(ParticleSettings *this)

{
  int iVar1;
  
  iVar1 = 0x1d60;
  do {
    AbyssEngine::String::~String((String *)(this + iVar1));
    iVar1 = iVar1 + -0xa0;
  } while (iVar1 != -0xa0);
  return this;
}

```

## target disasm
```
  00195474: push {r4,r5,r7,lr}
  00195476: add r7,sp,#0x8
  00195478: mov r4,r0
  0019547a: mov.w r5,#0x1d60
  0019547e: adds r0,r4,r5
  00195480: blx 0x0006ee30
  00195484: subs r5,#0xa0
  00195486: adds.w r0,r5,#0xa0
  0019548a: bne 0x0019547e
  0019548c: mov r0,r4
  0019548e: pop {r4,r5,r7,pc}
```
