# ParticleSystemManager::systemSetMatrix
elf 0x183e44 body 34
Sig: undefined __thiscall systemSetMatrix(ParticleSystemManager * this, int param_1, Matrix * param_2)

## decompile
```c

/* ParticleSystemManager::systemSetMatrix(int, AbyssEngine::AEMath::Matrix const*) */

void __thiscall
ParticleSystemManager::systemSetMatrix(ParticleSystemManager *this,int param_1,Matrix *param_2)

{
  int iVar1;
  
  if (param_1 != -1) {
    if (param_1 << 0x11 < 0) {
      iVar1 = *(int *)(this + 0x40);
      param_1 = param_1 & DAT_00193e64;
    }
    else {
      iVar1 = *(int *)(this + 0x1c);
    }
    (*(code *)(DAT_001ac9b4 + 0x1ac9b8))(*(undefined4 *)(iVar1 + param_1 * 4),param_2);
    return;
  }
  return;
}

```

## target disasm
```
  00193e44: adds r3,r1,#0x1
  00193e46: it eq
  00193e48: bx.eq lr
  00193e4a: lsls r3,r1,#0x11
  00193e4c: bmi 0x00193e52
  00193e4e: ldr r0,[r0,#0x1c]
  00193e50: b 0x00193e58
  00193e52: ldr r3,[0x00193e64]
  00193e54: ldr r0,[r0,#0x40]
  00193e56: ands r1,r3
  00193e58: add.w r0,r0,r1, lsl #0x2
  00193e5c: mov r1,r2
  00193e5e: ldr r0,[r0,#0x0]
  00193e60: b.w 0x001ac9a8
  001ac9a8: bx pc
```
