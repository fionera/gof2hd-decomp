# ParticleSettings::ParticleSettings
elf 0x183fe8 body 34
Sig: undefined __thiscall ParticleSettings(ParticleSettings * this)

## decompile
```c

/* ParticleSettings::ParticleSettings() */

ParticleSettings * __thiscall ParticleSettings::ParticleSettings(ParticleSettings *this)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    AbyssEngine::String::String((String *)(this + iVar1));
    iVar1 = iVar1 + 0xa0;
  } while (iVar1 != 0x1e00);
  return this;
}

```

## target disasm
```
  00193fe8: push {r4,r5,r6,r7,lr}
  00193fea: add r7,sp,#0xc
  00193fec: push.w r11
  00193ff0: mov r4,r0
  00193ff2: movs r6,#0x0
  00193ff4: adds r0,r4,r6
  00193ff6: blx 0x0006efbc
  00193ffa: adds r6,#0xa0
  00193ffc: cmp.w r6,#0x1e00
  00194000: bne 0x00193ff4
  00194002: mov r0,r4
  00194004: pop.w r11
  00194008: pop {r4,r5,r6,r7,pc}
```
