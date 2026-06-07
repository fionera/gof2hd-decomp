# ParticleSystemManager::setParticleSetByIndex
elf 0x183b24 body 34
Sig: undefined __thiscall setParticleSetByIndex(ParticleSystemManager * this, int param_1, uchar param_2)

## decompile
```c

/* ParticleSystemManager::setParticleSetByIndex(int, unsigned char) */

void __thiscall
ParticleSystemManager::setParticleSetByIndex(ParticleSystemManager *this,int param_1,uchar param_2)

{
  int iVar1;
  
  if (param_1 != -1) {
    if (param_1 << 0x11 < 0) {
      iVar1 = *(int *)(this + 0x40);
      param_1 = param_1 & DAT_00193b44;
    }
    else {
      iVar1 = *(int *)(this + 0x1c);
    }
    (*(code *)(DAT_001ac904 + 0x1ac908))(*(undefined4 *)(iVar1 + param_1 * 4),param_2);
    return;
  }
  return;
}

```

## target disasm
```
  00193b24: adds r3,r1,#0x1
  00193b26: it eq
  00193b28: bx.eq lr
  00193b2a: lsls r3,r1,#0x11
  00193b2c: bmi 0x00193b32
  00193b2e: ldr r0,[r0,#0x1c]
  00193b30: b 0x00193b38
  00193b32: ldr r3,[0x00193b44]
  00193b34: ldr r0,[r0,#0x40]
  00193b36: ands r1,r3
  00193b38: add.w r0,r0,r1, lsl #0x2
  00193b3c: mov r1,r2
  00193b3e: ldr r0,[r0,#0x0]
  00193b40: b.w 0x001ac8f8
  001ac8f8: bx pc
```
