# IParticleSystem::IParticleSystem
elf 0x1819c0 body 248
Sig: undefined __thiscall IParticleSystem(IParticleSystem * this, PaintCanvas * param_1, Matrix * param_2, Array * param_3, bool param_4, bool param_5)

## decompile
```c

/* IParticleSystem::IParticleSystem(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*,
   Array<ParticleSettings::ParticleSet> const&, bool, bool) */

IParticleSystem * __thiscall
IParticleSystem::IParticleSystem
          (IParticleSystem *this,PaintCanvas *param_1,Matrix *param_2,Array *param_3,bool param_4,
          bool param_5)

{
  int iVar1;
  uint uVar2;
  void *pvVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint uVar7;
  
  iVar1 = DAT_00191ad0;
  *(PaintCanvas **)(this + 8) = param_1;
  *(int *)this = *(int *)(iVar1 + 0x1919d6) + 8;
  AbyssEngine::AERandom::AERandom((AERandom *)(this + 0x10));
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x28) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(Matrix **)(this + 0x18) = param_2;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  Array<ParticleSettings::ParticleSet>::Array((Array<ParticleSettings::ParticleSet> *)(this + 0x38))
  ;
  this[0x4c] = (IParticleSystem)param_4;
  this[0x45] = (IParticleSystem)param_5;
  ArraySet<ParticleSettings::ParticleSet>(param_3,(Array *)(this + 0x38));
  iVar1 = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0xffffffff;
  *(undefined4 *)(this + 0x58) = 0xffffffff;
  *(undefined2 *)(this + 0xc) = 0x101;
  this[0xe] = (IParticleSystem)0x1;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  piVar5 = *(int **)(param_3 + 4);
  uVar7 = 0;
  for (iVar4 = *(int *)param_3; iVar4 != 0; iVar4 = iVar4 + -1) {
    if (*piVar5 != -1) {
      iVar6 = *(int *)(DAT_00191ad4 + 0x191a48) + *piVar5 * 0xa0;
      uVar2 = *(uint *)(iVar6 + 0x10);
      if ((int)uVar7 <= (int)uVar2) {
        uVar7 = uVar2;
      }
      *(uint *)(this + 0x48) = uVar7;
      if (iVar1 == 0) {
        iVar1 = *(int *)(iVar6 + 0xc);
        *(int *)(this + 0x34) = iVar1;
      }
    }
    piVar5 = piVar5 + 1;
  }
  uVar2 = (uint)((ulonglong)uVar7 * 4);
  *(undefined4 *)(this + 0x60) = 0;
  this[0x44] = (IParticleSystem)0x0;
  if ((int)((ulonglong)uVar7 * 4 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  pvVar3 = operator_new__(uVar2);
  *(void **)(this + 0x68) = pvVar3;
  pvVar3 = operator_new__(uVar7 | (int)uVar7 >> 0x1f);
  *(void **)(this + 0x6c) = pvVar3;
  for (iVar1 = 0; iVar1 < (int)uVar7; iVar1 = iVar1 + 1) {
    *(undefined1 *)(*(int *)(this + 0x6c) + iVar1) = 200;
    uVar7 = *(uint *)(this + 0x48);
  }
  this[0x5c] = (IParticleSystem)0x0;
  *(undefined2 *)(this + 4) = 0x101;
  return this;
}

```

## target disasm
```
  001919c0: push {r4,r5,r6,r7,lr}
  001919c2: add r7,sp,#0xc
  001919c4: push {r8,r9,r11}
  001919c8: mov r4,r0
  001919ca: ldr r0,[0x00191ad0]
  001919cc: add.w r8,r4,#0x10
  001919d0: str r1,[r4,#0x8]
  001919d2: add r0,pc
  001919d4: mov r5,r3
  001919d6: mov r6,r2
  001919d8: ldr r0,[r0,#0x0]
  001919da: adds r0,#0x8
  001919dc: str r0,[r4,#0x0]
  001919de: mov r0,r8
  001919e0: blx 0x00075d54
  001919e4: vmov.i32 q8,#0x0
  001919e8: add.w r0,r4,#0x1c
  001919ec: add.w r9,r4,#0x38
  001919f0: vst1.32 {d16,d17},[r0]
  001919f4: movs r0,#0x0
  001919f6: str r6,[r4,#0x18]
  001919f8: strd r0,r0,[r4,#0x2c]
  001919fc: mov r0,r9
  001919fe: blx 0x00078844
  00191a02: ldrd r1,r0,[r7,#0x8]
  00191a06: strb.w r1,[r4,#0x4c]
  00191a0a: strb.w r0,[r4,#0x45]
  00191a0e: mov r0,r5
  00191a10: mov r1,r9
  00191a12: blx 0x00078850
  00191a16: mov.w r1,#0xffffffff
  00191a1a: movs r0,#0x0
  00191a1c: strd r0,r1,[r4,#0x50]
  00191a20: str r1,[r4,#0x58]
  00191a22: movw r1,#0x101
  00191a26: strh r1,[r4,#0xc]
  00191a28: movs r1,#0x1
  00191a2a: strb r1,[r4,#0xe]
  00191a2c: str r0,[r4,#0x48]
  00191a2e: str r0,[r4,#0x34]
  00191a30: ldrd r1,r2,[r5,#0x0]
  00191a34: movs r5,#0x0
  00191a36: b 0x00191a62
  00191a38: ldr r3,[r2,#0x0]
  00191a3a: adds r6,r3,#0x1
  00191a3c: beq 0x00191a5e
  00191a3e: ldr r6,[0x00191ad4]
  00191a40: add.w r3,r3,r3, lsl #0x2
  00191a44: add r6,pc
  00191a46: ldr r6,[r6,#0x0]
  00191a48: add.w r3,r6,r3, lsl #0x5
  00191a4c: ldr r6,[r3,#0x10]
  00191a4e: cmp r5,r6
  00191a50: it le
  00191a52: mov.le r5,r6
  00191a54: cmp r0,#0x0
  00191a56: str r5,[r4,#0x48]
  00191a58: bne 0x00191a5e
  00191a5a: ldr r0,[r3,#0xc]
  00191a5c: str r0,[r4,#0x34]
  00191a5e: adds r2,#0x4
  00191a60: subs r1,#0x1
  00191a62: cmp r1,#0x0
  00191a64: bne 0x00191a38
  00191a66: movs r0,#0x4
  00191a68: movs r2,#0x0
  00191a6a: umull r0,r1,r5,r0
  00191a6e: str r2,[r4,#0x60]
  00191a70: strb.w r2,[r4,#0x44]
  00191a74: cmp r1,#0x0
  00191a76: it ne
  00191a78: mov.ne r1,#0x1
  00191a7a: cmp r1,#0x0
  00191a7c: it ne
  00191a7e: mov.ne.w r0,#0xffffffff
  00191a82: blx 0x0006ec08
  00191a86: str r0,[r4,#0x68]
  00191a88: orr.w r0,r5,r5, asr #0x1f
  00191a8c: blx 0x0006ec08
  00191a90: str r0,[r4,#0x6c]
  00191a92: movs r0,#0x0
  00191a94: movs r1,#0xc8
  00191a96: b 0x00191aa0
  00191a98: ldr r2,[r4,#0x6c]
  00191a9a: strb r1,[r2,r0]
  00191a9c: adds r0,#0x1
  00191a9e: ldr r5,[r4,#0x48]
  00191aa0: cmp r0,r5
  00191aa2: blt 0x00191a98
  00191aa4: movs r0,#0x0
  00191aa6: strb.w r0,[r4,#0x5c]
  00191aaa: movw r0,#0x101
  00191aae: strh r0,[r4,#0x4]
  00191ab0: mov r0,r4
  00191ab2: pop.w {r8,r9,r11}
  00191ab6: pop {r4,r5,r6,r7,pc}
```
