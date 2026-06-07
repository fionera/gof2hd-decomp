# ParticleSystemManager::setParticleSetBySet
elf 0x183b48 body 34
Sig: undefined __thiscall setParticleSetBySet(ParticleSystemManager * this, int param_1, ParticleSet param_2)

## decompile
```c

/* ParticleSystemManager::setParticleSetBySet(int, ParticleSettings::ParticleSet) */

void __thiscall
ParticleSystemManager::setParticleSetBySet
          (ParticleSystemManager *this,uint param_1,undefined4 param_3)

{
  int iVar1;
  
  if (param_1 != 0xffffffff) {
    if ((int)(param_1 << 0x11) < 0) {
      iVar1 = *(int *)(this + 0x40);
      param_1 = param_1 & DAT_00193b68;
    }
    else {
      iVar1 = *(int *)(this + 0x1c);
    }
    (*(code *)(&UNK_001ac918 + DAT_001ac914))(*(undefined4 *)(iVar1 + param_1 * 4),param_3);
    return;
  }
  return;
}

```

## target disasm
```
  00193b48: adds r3,r1,#0x1
  00193b4a: it eq
  00193b4c: bx.eq lr
  00193b4e: lsls r3,r1,#0x11
  00193b50: bmi 0x00193b56
  00193b52: ldr r0,[r0,#0x1c]
  00193b54: b 0x00193b5c
  00193b56: ldr r3,[0x00193b68]
  00193b58: ldr r0,[r0,#0x40]
  00193b5a: ands r1,r3
  00193b5c: add.w r0,r0,r1, lsl #0x2
  00193b60: mov r1,r2
  00193b62: ldr r0,[r0,#0x0]
  00193b64: b.w 0x001ac908
  001ac908: bx pc
```
