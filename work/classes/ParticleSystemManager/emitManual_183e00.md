# ParticleSystemManager::emitManual
elf 0x183e00 body 62
Sig: undefined __thiscall emitManual(ParticleSystemManager * this, int param_1, Vector * param_2, int param_3, float param_4)

## decompile
```c

/* ParticleSystemManager::emitManual(int, AbyssEngine::AEMath::Vector const&, int, float) */

ulonglong __thiscall
ParticleSystemManager::emitManual
          (ParticleSystemManager *this,int param_1,Vector *param_2,int param_3,float param_4)

{
  int iVar1;
  
  if (param_1 == -1) {
    return CONCAT44(0xffffffff,this);
  }
  if (param_1 << 0x11 < 0) {
    iVar1 = *(int *)(this + 0x40);
    param_1 = param_1 & DAT_00193e40;
  }
  else {
    iVar1 = *(int *)(this + 0x1c);
  }
  IParticleSystem::emitManual
            (*(undefined4 *)(iVar1 + param_1 * 4),*(undefined4 *)param_2,
             *(undefined4 *)(param_2 + 4),*(undefined4 *)(param_2 + 8));
  return (ulonglong)(uint)param_3;
}

```

## target disasm
```
  00193e00: adds.w r12,r1,#0x1
  00193e04: it eq
  00193e06: bx.eq lr
  00193e08: push {r0,r1,r2,r3,r4,r6,r7,lr}
  00193e0a: add r7,sp,#0x18
  00193e0c: vldr.32 s0,[r7,#0x8]
  00193e10: lsls.w r12,r1,#0x11
  00193e14: bmi 0x00193e1a
  00193e16: ldr r0,[r0,#0x1c]
  00193e18: b 0x00193e20
  00193e1a: ldr r4,[0x00193e40]
  00193e1c: ldr r0,[r0,#0x40]
  00193e1e: ands r1,r4
  00193e20: add.w r0,r0,r1, lsl #0x2
  00193e24: ldm.w r2,{r1,r12,lr}
  00193e28: movs r2,#0x0
  00193e2a: ldr r0,[r0,#0x0]
  00193e2c: vstr.32 s0,[sp,#0x8]
  00193e30: strd r3,r2,[sp,#0x0]
  00193e34: mov r2,r12
  00193e36: mov r3,lr
  00193e38: blx 0x00078b44
  00193e3c: pop {r0,r1,r2,r3,r4,r6,r7,pc}
```
