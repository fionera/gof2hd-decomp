# ParticleSystemManager::enableSystemUpdate
elf 0x183ab8 body 34
Sig: undefined __thiscall enableSystemUpdate(ParticleSystemManager * this, int param_1, bool param_2)

## decompile
```c

/* ParticleSystemManager::enableSystemUpdate(int, bool) */

void __thiscall
ParticleSystemManager::enableSystemUpdate(ParticleSystemManager *this,int param_1,bool param_2)

{
  int iVar1;
  
  if (param_1 != -1) {
    if (param_1 << 0x11 < 0) {
      iVar1 = *(int *)(this + 0x40);
      param_1 = param_1 & DAT_00193ad8;
    }
    else {
      iVar1 = *(int *)(this + 0x1c);
    }
    (*(code *)(DAT_001ac8f4 + 0x1ac8f8))(*(undefined4 *)(iVar1 + param_1 * 4),param_2);
    return;
  }
  return;
}

```

## target disasm
```
  00193ab8: adds r3,r1,#0x1
  00193aba: it eq
  00193abc: bx.eq lr
  00193abe: lsls r3,r1,#0x11
  00193ac0: bmi 0x00193ac6
  00193ac2: ldr r0,[r0,#0x1c]
  00193ac4: b 0x00193acc
  00193ac6: ldr r3,[0x00193ad8]
  00193ac8: ldr r0,[r0,#0x40]
  00193aca: ands r1,r3
  00193acc: add.w r0,r0,r1, lsl #0x2
  00193ad0: mov r1,r2
  00193ad2: ldr r0,[r0,#0x0]
  00193ad4: b.w 0x001ac8e8
  001ac8e8: bx pc
```
