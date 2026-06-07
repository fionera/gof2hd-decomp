# ParticleSystemMesh::ParticleSystemMesh
elf 0x185490 body 286
Sig: undefined __thiscall ParticleSystemMesh(ParticleSystemMesh * this, PaintCanvas * param_1, Matrix * param_2, Array * param_3, bool param_4, bool param_5)

## decompile
```c

/* ParticleSystemMesh::ParticleSystemMesh(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix
   const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) */

ParticleSystemMesh * __thiscall
ParticleSystemMesh::ParticleSystemMesh
          (ParticleSystemMesh *this,PaintCanvas *param_1,Matrix *param_2,Array *param_3,bool param_4
          ,bool param_5)

{
  longlong lVar1;
  int *piVar2;
  int iVar3;
  void *pvVar4;
  int iVar5;
  int extraout_r1;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  
  IParticleSystem::IParticleSystem((IParticleSystem *)this,param_1,param_2,param_3,param_4,param_5);
  uVar7 = *(uint *)(this + 0x34);
  piVar2 = (int *)(DAT_001955bc + 0x1954b6);
  uVar9 = *(uint *)(this + 0x48);
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  iVar6 = -((int)(uVar7 << 0x13) >> 0x1f) - ((int)(uVar7 << 0x12) >> 0x1f);
  if ((uVar7 & 0x4000) != 0) {
    iVar6 = iVar6 + 1;
  }
  *(int *)this = *piVar2 + 8;
  iVar3 = iVar6 << ((uVar7 & 0x1ffff) >> 0x10);
  this[0x74] = SUB41((uVar7 << 0xf) >> 0x1f,0);
  *(int *)(this + 0x98) = iVar6;
  *(int *)(this + 0x9c) = iVar3;
  iVar5 = uVar9 * iVar3 * 4;
  *(int *)(this + 0x70) = iVar5;
  if ((uVar7 & 0x8000) == 0) {
    uVar7 = (uint)((ulonglong)uVar9 * 0xc);
    if ((int)((ulonglong)uVar9 * 0xc >> 0x20) != 0) {
      uVar7 = 0xffffffff;
    }
    pvVar4 = operator_new__(uVar7);
    if (uVar9 == 0) goto LAB_0019559e;
    iVar6 = uVar9 * 0xc;
  }
  else {
    if (((*(int *)param_3 != 0) && (**(int **)(param_3 + 4) != -1)) &&
       (0.0 < *(float *)(*(int *)(DAT_001955c0 + 0x195542) + **(int **)(param_3 + 4) * 0xa0 + 0x40))
       ) {
      *(int *)(this + 0x70) = iVar5 + iVar3 * 4;
    }
    uVar8 = uVar9 * iVar6 * 2;
    lVar1 = (ulonglong)uVar8 * 0xc;
    uVar7 = (uint)lVar1;
    if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
      uVar7 = 0xffffffff;
    }
    pvVar4 = operator_new__(uVar7);
    if (uVar8 == 0) goto LAB_0019559e;
    iVar6 = uVar9 * iVar6 * 0x18;
  }
  __aeabi_uidivmod(iVar6 + -0xc,0xc);
  __aeabi_memclr4(pvVar4,((iVar6 + -0xc) - extraout_r1) + 0xc);
LAB_0019559e:
  *(void **)(this + 100) = pvVar4;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  return this;
}

```

## target disasm
```
  00195490: push {r4,r5,r6,r7,lr}
  00195492: add r7,sp,#0xc
  00195494: push {r6,r7,r8,r9,r11}
  00195498: mov r8,r3
  0019549a: mov r4,r0
  0019549c: ldrd r0,r3,[r7,#0x8]
  001954a0: strd r0,r3,[sp,#0x0]
  001954a4: mov r0,r4
  001954a6: mov r3,r8
  001954a8: blx 0x00078898
  001954ac: ldr r0,[0x001955bc]
  001954ae: movs r1,#0x0
  001954b0: ldr r3,[r4,#0x34]
  001954b2: add r0,pc
  001954b4: ldr.w r9,[r4,#0x48]
  001954b8: strd r1,r1,[r4,#0x80]
  001954bc: tst r3,#0x4000
  001954c0: str.w r1,[r4,#0x88]
  001954c4: ldr r1,[r0,#0x0]
  001954c6: ubfx r0,r3,#0xd,#0x1
  001954ca: ubfx r2,r3,#0xc,#0x1
  001954ce: add r2,r0
  001954d0: add.w r1,r1,#0x8
  001954d4: it ne
  001954d6: add.ne r2,#0x1
  001954d8: ubfx r5,r3,#0x10,#0x1
  001954dc: str r1,[r4,#0x0]
  001954de: tst r3,#0x8000
  001954e2: lsl.w r0,r2,r5
  001954e6: strb.w r5,[r4,#0x74]
  001954ea: mul r6,r9,r0
  001954ee: strd r2,r0,[r4,#0x98]
  001954f2: lsl.w r1,r6,#0x2
  001954f6: str r1,[r4,#0x70]
  001954f8: bne 0x00195528
  001954fa: movs r0,#0xc
  001954fc: umull r0,r1,r9,r0
  00195500: cmp r1,#0x0
  00195502: it ne
  00195504: mov.ne r1,#0x1
  00195506: cmp r1,#0x0
  00195508: it ne
  0019550a: mov.ne.w r0,#0xffffffff
  0019550e: blx 0x0006ec08
  00195512: mov r5,r0
  00195514: cmp.w r9,#0x0
  00195518: beq 0x0019559e
  0019551a: add.w r0,r9,r9, lsl #0x1
  0019551e: mvn r1,#0xb
  00195522: add.w r6,r1,r0, lsl #0x2
  00195526: b 0x0019558a
  00195528: ldr.w r3,[r8,#0x0]
  0019552c: cbz r3,0x0019555a
  0019552e: ldr.w r3,[r8,#0x4]
  00195532: ldr r3,[r3,#0x0]
  00195534: adds r6,r3,#0x1
  00195536: beq 0x0019555a
  00195538: ldr r5,[0x001955c0]
  0019553a: add.w r3,r3,r3, lsl #0x2
  0019553e: add r5,pc
  00195540: ldr r6,[r5,#0x0]
  00195542: add.w r3,r6,r3, lsl #0x5
  00195546: vldr.32 s0,[r3,#0x40]
  0019554a: vcmpe.f32 s0,#0
  0019554e: vmrs apsr,fpscr
  00195552: itt gt
  00195554: add.gt.w r0,r1,r0, lsl #0x2
  00195558: str.gt r0,[r4,#0x70]
  0019555a: mul r8,r9,r2
  0019555e: movs r0,#0xc
  00195560: lsl.w r6,r8,#0x1
  00195564: umull r0,r1,r6,r0
  00195568: cmp r1,#0x0
  0019556a: it ne
  0019556c: mov.ne r1,#0x1
  0019556e: cmp r1,#0x0
  00195570: it ne
  00195572: mov.ne.w r0,#0xffffffff
  00195576: blx 0x0006ec08
  0019557a: mov r5,r0
  0019557c: cbz r6,0x0019559e
  0019557e: add.w r0,r8,r8, lsl #0x1
  00195582: mvn r1,#0xb
  00195586: add.w r6,r1,r0, lsl #0x3
  0019558a: mov r0,r6
  0019558c: movs r1,#0xc
  0019558e: blx 0x0006ec5c
  00195592: subs r0,r6,r1
  00195594: add.w r1,r0,#0xc
  00195598: mov r0,r5
  0019559a: blx 0x0006ec14
  0019559e: movs r0,#0x0
  001955a0: str r5,[r4,#0x64]
  001955a2: strd r0,r0,[r4,#0x78]
  001955a6: mov r0,r4
  001955a8: pop.w {r2,r3,r8,r9,r11}
  001955ac: pop {r4,r5,r6,r7,pc}
```
