# Level::getBoundingVolume
elf 0xc3e48 body 496
Sig: undefined __stdcall getBoundingVolume(int param_1, AEGeometry * param_2)

## decompile
```c

/* Level::getBoundingVolume(int, AEGeometry*) */

void Level::getBoundingVolume(int param_1,AEGeometry *param_2)

{
  FileRead *this;
  Array *this_00;
  void *pvVar1;
  Array<BoundingVolume*> *this_01;
  int iVar2;
  BoundingSphere *this_02;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  uint in_fpscr;
  float extraout_s0;
  float fVar9;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s3;
  float extraout_s3_00;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined8 extraout_d2;
  float extraout_s5;
  float extraout_s6;
  float extraout_s7;
  float extraout_s8;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined8 local_58;
  undefined8 local_50;
  
  piVar6 = *(int **)(DAT_000d405c + 0xd3e5c);
  local_50 = CONCAT44(*piVar6,in_s14);
  local_58 = in_d6;
  this = operator_new(1);
  FileRead::FileRead(this);
  if ((int)param_2 < 2000) {
    this_00 = (Array *)FileRead::loadStationCollision((int)this);
  }
  else {
    this_00 = (Array *)FileRead::loadStaticCollision((int)this);
  }
  pvVar1 = (void *)FileRead::~FileRead(this);
  operator_delete(pvVar1);
  if (this_00 != (Array *)0x0) {
    uVar8 = **(uint **)(this_00 + 4);
    local_58 = 0;
    local_50 = local_50 & 0xffffffff00000000;
    this_01 = operator_new(0xc);
    Array<BoundingVolume*>::Array(this_01);
    ArraySetLength<BoundingVolume*>(uVar8,(Array *)this_01);
    iVar3 = 1;
    for (iVar7 = 0; iVar7 < (int)uVar8; iVar7 = iVar7 + 1) {
      iVar2 = *(int *)(this_00 + 4);
      iVar4 = iVar3 + 1;
      iVar5 = *(int *)(iVar2 + iVar3 * 4);
      if (iVar5 == 1) {
        iVar5 = iVar2 + iVar3 * 4;
        uVar11 = VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x10),(byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(undefined4 *)(iVar5 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
        iVar2 = *(int *)(iVar2 + iVar4 * 4);
        VectorSignedToFloat(*(undefined4 *)(iVar5 + 8),(byte)(in_fpscr >> 0x16) & 3);
        uVar10 = VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x14),(byte)(in_fpscr >> 0x16) & 3);
        local_50 = CONCAT44(local_50._4_4_,uVar10);
        fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x18),
                                           (byte)(in_fpscr >> 0x16) & 3);
        local_58 = CONCAT44(fVar9,uVar11);
        AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_58,fVar9);
        fVar9 = (float)VectorSignedToFloat(-iVar2,(byte)(in_fpscr >> 0x16) & 3);
        this_02 = operator_new(0x2c);
        BoundingAAB::BoundingAAB
                  ((BoundingAAB *)this_02,(float)local_58 + (float)local_58,extraout_s1_00,
                   local_58._4_4_ + local_58._4_4_,extraout_s3_00,(float)local_50 + (float)local_50,
                   extraout_s5,-fVar9,extraout_s7,extraout_s8);
        iVar4 = iVar3 + 7;
LAB_000d3ff6:
        *(BoundingSphere **)(*(int *)(this_01 + 4) + iVar7 * 4) = this_02;
      }
      else if (iVar5 == 0) {
        iVar5 = iVar2 + iVar3 * 4;
        uVar10 = VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x10),(byte)(in_fpscr >> 0x16) & 3);
        iVar2 = *(int *)(iVar2 + iVar4 * 4);
        VectorSignedToFloat(*(undefined4 *)(iVar5 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(float *)(iVar5 + 8),(byte)(in_fpscr >> 0x16) & 3);
        local_58 = CONCAT44(local_58._4_4_,uVar10);
        AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_58,*(float *)(iVar5 + 8));
        fVar9 = (float)-iVar2;
        iVar4 = iVar3 + 5;
        VectorSignedToFloat(fVar9,(byte)(in_fpscr >> 0x16) & 3);
        in_fpscr = in_fpscr & 0xfffffff;
        if ((float)local_58 < 0.0) {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_58,fVar9);
        }
        this_02 = operator_new(0x48);
        BoundingSphere::BoundingSphere
                  (this_02,extraout_s0,extraout_s1,extraout_s2,extraout_s3,(float)extraout_d2,
                   (float)((ulonglong)extraout_d2 >> 0x20),extraout_s6);
        goto LAB_000d3ff6;
      }
      iVar3 = iVar4;
    }
    ArrayRelease<int>(this_00);
    pvVar1 = (void *)Array<int>::~Array((Array<int> *)this_00);
    operator_delete(pvVar1);
  }
  iVar3 = *piVar6 - local_50._4_4_;
  if (iVar3 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

```
## target disasm
```
  000d3e48: push {r4,r5,r6,r7,lr}
  000d3e4a: add r7,sp,#0xc
  000d3e4c: push {r7,r8,r9,r10,r11}
  000d3e50: vpush {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11}
  000d3e54: ldr r0,[0x000d405c]
  000d3e56: mov r4,r1
  000d3e58: add r0,pc
  000d3e5a: ldr r6,[r0,#0x0]
  000d3e5c: ldr r0,[r6,#0x0]
  000d3e5e: str r0,[sp,#0x2c]
  000d3e60: movs r0,#0x1
  000d3e62: blx 0x0006eb24
  000d3e66: mov r5,r0
  000d3e68: blx 0x00072124
  000d3e6c: mov r0,r5
  000d3e6e: mov r1,r4
  000d3e70: cmp.w r4,#0x7d0
  000d3e74: blt 0x000d3e7c
  000d3e76: blx 0x000743b0
  000d3e7a: b 0x000d3e80
  000d3e7c: blx 0x000743bc
  000d3e80: mov r4,r0
  000d3e82: mov r0,r5
  000d3e84: blx 0x0007213c
  000d3e88: blx 0x0006eb48
  000d3e8c: mov.w r9,#0x0
  000d3e90: cmp r4,#0x0
  000d3e92: beq.w 0x000d401c
  000d3e96: ldr r0,[r4,#0x4]
  000d3e98: str r6,[sp,#0x1c]
  000d3e9a: ldr.w r11,[r0,#0x0]
  000d3e9e: movs r0,#0xc
  000d3ea0: strd r9,r9,[sp,#0x20]
  000d3ea4: str.w r9,[sp,#0x28]
  000d3ea8: blx 0x0006eb24
  000d3eac: mov r9,r0
  000d3eae: blx 0x000730c0
  000d3eb2: mov r0,r11
  000d3eb4: mov r1,r9
  000d3eb6: blx 0x000730cc
  000d3eba: add.w r8,sp,#0x20
  000d3ebe: movs r6,#0x0
  000d3ec0: mov.w r10,#0x1
  000d3ec4: b 0x000d4004
  000d3ec6: ldr r0,[r4,#0x4]
  000d3ec8: add.w r1,r10,#0x1
  000d3ecc: ldr.w r2,[r0,r10,lsl #0x2]
  000d3ed0: cmp r2,#0x1
  000d3ed2: beq 0x000d3f5a
  000d3ed4: cmp r2,#0x0
  000d3ed6: bne.w 0x000d4000
  000d3eda: add.w r2,r0,r10, lsl #0x2
  000d3ede: vldr.32 s4,[r2,#0x10]
  000d3ee2: vldr.32 s0,[r2,#0x8]
  000d3ee6: vldr.32 s2,[r2,#0xc]
  000d3eea: vcvt.f32.s32 s4,s4
  000d3eee: ldr.w r5,[r0,r1,lsl #0x2]
  000d3ef2: mov r0,r8
  000d3ef4: ldr r1,[0x000d4054]
  000d3ef6: vcvt.f32.s32 s16,s2
  000d3efa: vcvt.f32.s32 s18,s0
  000d3efe: vstr.32 s4,[sp,#0x20]
  000d3f02: blx 0x00072628
  000d3f06: rsbs r0,r5
  000d3f08: add.w r10,r10,#0x5
  000d3f0c: vmov s0,r0
  000d3f10: vcvt.f32.s32 s22,s0
  000d3f14: vldr.32 s20,[sp,#0x20]
  000d3f18: vcmpe.f32 s20,#0
  000d3f1c: vmrs apsr,fpscr
  000d3f20: bpl 0x000d3f2e
  000d3f22: ldr r1,[0x000d4058]
  000d3f24: mov r0,r8
  000d3f26: blx 0x00072628
  000d3f2a: vldr.32 s20,[sp,#0x20]
  000d3f2e: movs r0,#0x48
  000d3f30: vneg.f32 s22,s22
  000d3f34: vneg.f32 s18,s18
  000d3f38: blx 0x0006eb24
  000d3f3c: mov r5,r0
  000d3f3e: movs r1,#0x0
  000d3f40: movs r2,#0x0
  000d3f42: movs r3,#0x0
  000d3f44: vstr.32 s22,[sp]
  000d3f48: vstr.32 s16,[sp,#0x4]
  000d3f4c: vstr.32 s18,[sp,#0x8]
  000d3f50: vstr.32 s20,[sp,#0xc]
  000d3f54: blx 0x000730e4
  000d3f58: b 0x000d3ff6
  000d3f5a: add.w r2,r0,r10, lsl #0x2
  000d3f5e: vldr.32 s4,[r2,#0x10]
  000d3f62: vldr.32 s0,[r2,#0x8]
  000d3f66: vldr.32 s2,[r2,#0xc]
  000d3f6a: vcvt.f32.s32 s4,s4
  000d3f6e: vcvt.f32.s32 s16,s2
  000d3f72: ldr.w r5,[r0,r1,lsl #0x2]
  000d3f76: vcvt.f32.s32 s18,s0
  000d3f7a: ldr r1,[0x000d4050]
  000d3f7c: mov r0,r8
  000d3f7e: vstr.32 s4,[sp,#0x20]
  000d3f82: vldr.32 s0,[r2,#0x14]
  000d3f86: vcvt.f32.s32 s0,s0
  000d3f8a: vstr.32 s0,[sp,#0x28]
  000d3f8e: vldr.32 s0,[r2,#0x18]
  000d3f92: vcvt.f32.s32 s0,s0
  000d3f96: vstr.32 s0,[sp,#0x24]
  000d3f9a: blx 0x00072628
  000d3f9e: rsbs r0,r5
  000d3fa0: vneg.f32 s18,s18
  000d3fa4: vmov s0,r0
  000d3fa8: movs r0,#0x2c
  000d3faa: vcvt.f32.s32 s20,s0
  000d3fae: blx 0x0006eb24
  000d3fb2: vldr.32 s4,[sp,#0x28]
  000d3fb6: mov r5,r0
  000d3fb8: vldr.32 s0,[sp,#0x20]
  000d3fbc: vneg.f32 s6,s20
  000d3fc0: vldr.32 s2,[sp,#0x24]
  000d3fc4: vadd.f32 s4,s4,s4
  000d3fc8: vadd.f32 s2,s2,s2
  000d3fcc: movs r1,#0x0
  000d3fce: vadd.f32 s0,s0,s0
  000d3fd2: movs r2,#0x0
  000d3fd4: movs r3,#0x0
  000d3fd6: vstr.32 s6,[sp]
  000d3fda: vstr.32 s16,[sp,#0x4]
  000d3fde: vstr.32 s18,[sp,#0x8]
  000d3fe2: vstr.32 s4,[sp,#0x14]
  000d3fe6: vstr.32 s2,[sp,#0x10]
  000d3fea: vstr.32 s0,[sp,#0xc]
  000d3fee: blx 0x00074350
  000d3ff2: add.w r10,r10,#0x7
  000d3ff6: ldr.w r0,[r9,#0x4]
  000d3ffa: str.w r5,[r0,r6,lsl #0x2]
  000d3ffe: b 0x000d4002
  000d4000: mov r10,r1
  000d4002: adds r6,#0x1
  000d4004: cmp r6,r11
  000d4006: blt.w 0x000d3ec6
  000d400a: mov r0,r4
  000d400c: blx 0x0007369c
  000d4010: mov r0,r4
  000d4012: blx 0x00070204
  000d4016: blx 0x0006eb48
  000d401a: ldr r6,[sp,#0x1c]
  000d401c: ldr r0,[sp,#0x2c]
  000d401e: ldr r1,[r6,#0x0]
  000d4020: subs r0,r1,r0
  000d4022: itttt eq
  000d4024: mov.eq r0,r9
  000d4026: vpop.eq {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11}
  000d402a: add.eq sp,#0x4
  000d402c: pop.eq.w {r8,r9,r10,r11}
  000d4030: it eq
  000d4032: pop.eq {r4,r5,r6,r7,pc}
  000d4034: blx 0x0006e824
```
