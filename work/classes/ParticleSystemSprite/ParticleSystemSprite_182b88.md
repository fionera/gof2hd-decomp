# ParticleSystemSprite::ParticleSystemSprite
elf 0x182b88 body 120
Sig: undefined __thiscall ParticleSystemSprite(ParticleSystemSprite * this, PaintCanvas * param_1, Matrix * param_2, Array * param_3, bool param_4, bool param_5)

## decompile
```c

/* ParticleSystemSprite::ParticleSystemSprite(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix
   const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) */

ParticleSystemSprite * __thiscall
ParticleSystemSprite::ParticleSystemSprite
          (ParticleSystemSprite *this,PaintCanvas *param_1,Matrix *param_2,Array *param_3,
          bool param_4,bool param_5)

{
  longlong lVar1;
  uint uVar2;
  void *pvVar3;
  undefined4 uVar4;
  int extraout_r1;
  uint uVar5;
  uint uVar6;
  int iVar7;
  undefined4 extraout_s0;
  undefined4 extraout_s1;
  undefined8 uVar8;
  
  IParticleSystem::IParticleSystem((IParticleSystem *)this,param_1,param_2,param_3,param_4,param_5);
  uVar5 = *(uint *)(this + 0x48);
  lVar1 = (ulonglong)uVar5 * 0xc;
  uVar6 = (uint)lVar1;
  *(int *)this = *(int *)(DAT_00192c18 + 0x192bb0) + 8;
  uVar2 = uVar6;
  if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  pvVar3 = operator_new__(uVar2);
  uVar8 = CONCAT44(extraout_s1,extraout_s0);
  if (uVar5 != 0) {
    iVar7 = uVar6 - 0xc;
    __aeabi_uidivmod(iVar7,0xc);
    uVar8 = __aeabi_memclr4(pvVar3,(iVar7 - extraout_r1) + 0xc);
  }
  *(void **)(this + 100) = pvVar3;
  uVar4 = AbyssEngine::AEMath::Pow((float)uVar8,(float)((ulonglong)uVar8 >> 0x20));
  *(undefined4 *)(this + 0x70) = uVar4;
  return this;
}

```

## target disasm
```
  00192b88: push {r4,r5,r6,r7,lr}
  00192b8a: add r7,sp,#0xc
  00192b8c: push.w r8
  00192b90: sub sp,#0x8
  00192b92: mov r4,r0
  00192b94: ldrd r0,r6,[r7,#0x8]
  00192b98: strd r0,r6,[sp,#0x0]
  00192b9c: mov r0,r4
  00192b9e: blx 0x00078898
  00192ba2: ldr r5,[r4,#0x48]
  00192ba4: movs r1,#0xc
  00192ba6: ldr r0,[0x00192c18]
  00192ba8: umull r6,r1,r5,r1
  00192bac: add r0,pc
  00192bae: ldr r0,[r0,#0x0]
  00192bb0: adds r0,#0x8
  00192bb2: str r0,[r4,#0x0]
  00192bb4: cmp r1,#0x0
  00192bb6: it ne
  00192bb8: mov.ne r1,#0x1
  00192bba: mov r0,r6
  00192bbc: cmp r1,#0x0
  00192bbe: it ne
  00192bc0: mov.ne.w r0,#0xffffffff
  00192bc4: blx 0x0006ec08
  00192bc8: mov r8,r0
  00192bca: cbz r5,0x00192be2
  00192bcc: subs r6,#0xc
  00192bce: movs r1,#0xc
  00192bd0: mov r0,r6
  00192bd2: blx 0x0006ec5c
  00192bd6: subs r0,r6,r1
  00192bd8: add.w r1,r0,#0xc
  00192bdc: mov r0,r8
  00192bde: blx 0x0006ec14
  00192be2: ldr r0,[0x00192c10]
  00192be4: ldr r1,[0x00192c14]
  00192be6: str.w r8,[r4,#0x64]
  00192bea: blx 0x000788a4
  00192bee: vmov s0,r0
  00192bf2: mov r0,r4
  00192bf4: vstr.32 s0,[r4,#0x70]
  00192bf8: add sp,#0x8
  00192bfa: pop.w r8
  00192bfe: pop {r4,r5,r6,r7,pc}
```
