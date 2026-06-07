# Explosion::setScaling
elf 0xa7e90 body 270
Sig: undefined __stdcall setScaling(float param_1)

## decompile
```c

/* Explosion::setScaling(float) */

void Explosion::setScaling(float param_1)

{
  int *in_r0;
  undefined4 uVar1;
  Transform *pTVar2;
  uint *puVar3;
  float fVar4;
  float in_r1;
  uint uVar5;
  uint *puVar6;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float in_s1;
  float extraout_s1;
  float in_s2;
  float extraout_s2;
  float fVar7;
  undefined8 uVar8;
  
  in_r0[9] = (int)in_r1;
  AEGeometry::setScaling(param_1,in_s1,in_s2);
  if (in_r0[2] != 0) {
    AEGeometry::setScaling(extraout_s0,extraout_s1,extraout_s2);
  }
  in_fpscr = in_fpscr & 0xfffffff;
  fVar7 = 1.0;
  if (in_r1 < 1.0) {
    fVar7 = (1.0 - in_r1) * 3.0 + 1.0;
  }
  if (*in_r0 == 0xb) {
    fVar7 = fVar7 * 0.5;
  }
  if (*in_r0 - 8U < 3) {
    uVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000b7fa8 + 0xb7f04));
    fVar7 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = DAT_000b7fa4 + fVar7 * DAT_000b7fa0;
  }
  puVar6 = *(uint **)(DAT_000b7fac + 0xb7f28);
  pTVar2 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
  AbyssEngine::Transform::SetAnimationSpeed(pTVar2,extraout_s0_00);
  if (in_r0[2] != 0) {
    pTVar2 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
    AbyssEngine::Transform::SetAnimationSpeed(pTVar2,extraout_s0_01);
  }
  puVar3 = (uint *)in_r0[3];
  if (puVar3 != (uint *)0x0) {
    for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
      pTVar2 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
      AbyssEngine::Transform::SetAnimationSpeed(pTVar2,extraout_s0_02);
      puVar3 = (uint *)in_r0[3];
    }
  }
  fVar4 = (float)__aeabi_l2f(in_r0[4],in_r0[5]);
  uVar8 = __aeabi_f2lz(fVar4 / fVar7);
  *(undefined8 *)(in_r0 + 4) = uVar8;
  return;
}

```

## target disasm
```
  000b7e90: push {r4,r5,r6,r7,lr}
  000b7e92: add r7,sp,#0xc
  000b7e94: push.w r8
  000b7e98: vpush {d8,d9}
  000b7e9c: mov r8,r0
  000b7e9e: ldr r0,[r0,#0x4]
  000b7ea0: mov r2,r1
  000b7ea2: mov r3,r1
  000b7ea4: mov r5,r1
  000b7ea6: str.w r1,[r8,#0x24]
  000b7eaa: blx 0x000727b4
  000b7eae: ldr.w r0,[r8,#0x8]
  000b7eb2: vmov s18,r5
  000b7eb6: cbz r0,0x000b7ec2
  000b7eb8: mov r1,r5
  000b7eba: mov r2,r5
  000b7ebc: mov r3,r5
  000b7ebe: blx 0x000727b4
  000b7ec2: vmov.f32 s16,0x3f800000
  000b7ec6: vcmpe.f32 s18,s16
  000b7eca: vmrs apsr,fpscr
  000b7ece: itttt mi
  000b7ed0: vmov.mi.f32 s16,0x3f800000
  000b7ed4: vmov.mi.f32 s0,0x40400000
  000b7ed8: vsub.mi.f32 s2,s16,s18
  000b7edc: vmla.mi.f32 s16,s2,s0
  000b7ee0: ldr.w r0,[r8,#0x0]
  000b7ee4: vmov.f32 s0,0x3f000000
  000b7ee8: cmp r0,#0xb
  000b7eea: sub.w r0,r0,#0x8
  000b7eee: vmul.f32 s0,s16,s0
  000b7ef2: it eq
  000b7ef4: vmov.eq.f32 s16,s0
  000b7ef8: cmp r0,#0x2
  000b7efa: bhi 0x000b7f1e
  000b7efc: ldr r0,[0x000b7fa8]
  000b7efe: movs r1,#0x3c
  000b7f00: add r0,pc
  000b7f02: ldr r0,[r0,#0x0]
  000b7f04: ldr r0,[r0,#0x0]
  000b7f06: blx 0x00071848
  000b7f0a: vmov s0,r0
  000b7f0e: vldr.32 s2,[pc,#0x90]
  000b7f12: vldr.32 s16,[pc,#0x90]
  000b7f16: vcvt.f32.s32 s0,s0
  000b7f1a: vmla.f32 s16,s0,s2
  000b7f1e: ldr r0,[0x000b7fac]
  000b7f20: ldr.w r1,[r8,#0x4]
  000b7f24: add r0,pc
  000b7f26: ldr r6,[r0,#0x0]
  000b7f28: ldr r1,[r1,#0xc]
  000b7f2a: ldr r0,[r6,#0x0]
  000b7f2c: blx 0x00072088
  000b7f30: vmov r5,s16
  000b7f34: mov r1,r5
  000b7f36: blx 0x00073570
  000b7f3a: ldr.w r0,[r8,#0x8]
  000b7f3e: cbz r0,0x000b7f4e
  000b7f40: ldr r1,[r0,#0xc]
  000b7f42: ldr r0,[r6,#0x0]
  000b7f44: blx 0x00072088
  000b7f48: mov r1,r5
  000b7f4a: blx 0x00073570
  000b7f4e: ldr.w r0,[r8,#0xc]
  000b7f52: cbz r0,0x000b7f78
  000b7f54: movs r4,#0x0
  000b7f56: b 0x000b7f72
  000b7f58: ldr r0,[r0,#0x4]
  000b7f5a: ldr.w r1,[r0,r4,lsl #0x2]
  000b7f5e: ldr r0,[r6,#0x0]
  000b7f60: ldr r1,[r1,#0xc]
  000b7f62: blx 0x00072088
  000b7f66: mov r1,r5
  000b7f68: blx 0x00073570
  000b7f6c: ldr.w r0,[r8,#0xc]
  000b7f70: adds r4,#0x1
  000b7f72: ldr r1,[r0,#0x0]
  000b7f74: cmp r4,r1
  000b7f76: bcc 0x000b7f58
  000b7f78: ldrd r0,r1,[r8,#0x10]
  000b7f7c: blx 0x0006f7d8
  000b7f80: vmov s0,r0
  000b7f84: vdiv.f32 s0,s0,s16
  000b7f88: vmov r0,s0
  000b7f8c: blx 0x0006ebb4
  000b7f90: strd r0,r1,[r8,#0x10]
  000b7f94: vpop {d8,d9}
  000b7f98: pop.w r8
  000b7f9c: pop {r4,r5,r6,r7,pc}
```
