# Globals::getWreckCollision
elf 0xe4bb8 body 562
Sig: undefined __stdcall getWreckCollision(int param_1, AEGeometry * param_2)

## decompile
```c

/* Globals::getWreckCollision(int, AEGeometry*) */

void Globals::getWreckCollision(int param_1,AEGeometry *param_2)

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
  uint uVar7;
  int iVar8;
  uint in_fpscr;
  float fVar9;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s5;
  float extraout_s5_00;
  float extraout_s7;
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  float local_30;
  float local_2c;
  int local_28;
  
  piVar6 = *(int **)(DAT_000f4e10 + 0xf4bca);
  local_28 = *piVar6;
  this = operator_new(1);
  FileRead::FileRead(this);
  this_00 = (Array *)FileRead::loadWreckCollision((int)this);
  pvVar1 = (void *)FileRead::~FileRead(this);
  operator_delete(pvVar1);
  if (this_00 != (Array *)0x0) {
    uVar7 = **(uint **)(this_00 + 4);
    local_30 = 0.0;
    local_2c = 0.0;
    local_40 = 0.0;
    local_3c = 0.0;
    local_38 = 0.0;
    local_34 = 0.0;
    this_01 = operator_new(0xc);
    Array<BoundingVolume*>::Array(this_01);
    ArraySetLength<BoundingVolume*>(uVar7,(Array *)this_01);
    iVar3 = 1;
    for (iVar8 = 0; iVar8 < (int)uVar7; iVar8 = iVar8 + 1) {
      iVar2 = *(int *)(this_00 + 4);
      iVar4 = iVar3 + 1;
      iVar5 = *(int *)(iVar2 + iVar3 * 4);
      if (iVar5 == 1) {
        iVar5 = iVar2 + iVar3 * 4;
        local_34 = (float)VectorSignedToFloat(-*(int *)(iVar2 + iVar4 * 4),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_2c = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 8),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_30 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0xc),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_40 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x10),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_38 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x14),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_3c = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x18),
                                              (byte)(in_fpscr >> 0x16) & 3);
        fVar9 = (float)AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_40,local_3c);
        iVar4 = iVar3 + 7;
        if (param_2 == (AEGeometry *)0x0) {
          fVar9 = (float)AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_34,fVar9);
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_40,fVar9);
        }
        this_02 = operator_new(0x2c);
        BoundingAAB::BoundingAAB
                  ((BoundingAAB *)this_02,local_40 + local_40,extraout_s1_00,local_3c + local_3c,
                   extraout_s3_00,local_38 + local_38,extraout_s5_00,local_34,extraout_s7,local_30);
LAB_000f4dac:
        *(BoundingSphere **)(*(int *)(this_01 + 4) + iVar8 * 4) = this_02;
      }
      else if (iVar5 == 0) {
        iVar5 = iVar2 + iVar3 * 4;
        local_34 = (float)VectorSignedToFloat(-*(int *)(iVar2 + iVar4 * 4),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_2c = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 8),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_30 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0xc),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_40 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x10),
                                              (byte)(in_fpscr >> 0x16) & 3);
        AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_40,local_40);
        in_fpscr = in_fpscr & 0xfffffff;
        fVar9 = local_40;
        if (local_40 < 0.0) {
          fVar9 = (float)AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_40,local_40);
        }
        iVar4 = iVar3 + 5;
        if (param_2 == (AEGeometry *)0x0) {
          fVar9 = (float)AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_34,fVar9);
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_40,fVar9);
        }
        this_02 = operator_new(0x48);
        BoundingSphere::BoundingSphere
                  (this_02,local_34,extraout_s1,local_30,extraout_s3,local_2c,extraout_s5,local_40);
        goto LAB_000f4dac;
      }
      iVar3 = iVar4;
    }
    ArrayRelease<int>(this_00);
    pvVar1 = (void *)Array<int>::~Array((Array<int> *)this_00);
    operator_delete(pvVar1);
  }
  iVar3 = *piVar6 - local_28;
  if (iVar3 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

```

## target disasm
```
  000f4bb8: push {r4,r5,r6,r7,lr}
  000f4bba: add r7,sp,#0xc
  000f4bbc: push {r8,r9,r10,r11}
  000f4bc0: sub sp,#0x3c
  000f4bc2: ldr r0,[0x000f4e10]
  000f4bc4: mov r8,r1
  000f4bc6: add r0,pc
  000f4bc8: ldr r4,[r0,#0x0]
  000f4bca: ldr r0,[r4,#0x0]
  000f4bcc: str r0,[sp,#0x38]
  000f4bce: movs r0,#0x1
  000f4bd0: blx 0x0006eb24
  000f4bd4: mov r5,r0
  000f4bd6: blx 0x00072124
  000f4bda: mov r0,r5
  000f4bdc: mov r1,r8
  000f4bde: blx 0x00075de4
  000f4be2: mov r6,r0
  000f4be4: mov r0,r5
  000f4be6: blx 0x0007213c
  000f4bea: blx 0x0006eb48
  000f4bee: mov.w r11,#0x0
  000f4bf2: cmp r6,#0x0
  000f4bf4: beq.w 0x000f4dd4
  000f4bf8: ldr r0,[r6,#0x4]
  000f4bfa: str r4,[sp,#0x1c]
  000f4bfc: ldr r4,[r0,#0x0]
  000f4bfe: movs r0,#0xc
  000f4c00: strd r11,r11,[sp,#0x30]
  000f4c04: strd r11,r11,[sp,#0x20]
  000f4c08: strd r11,r11,[sp,#0x28]
  000f4c0c: blx 0x0006eb24
  000f4c10: mov r11,r0
  000f4c12: blx 0x000730c0
  000f4c16: mov r0,r4
  000f4c18: mov r1,r11
  000f4c1a: blx 0x000730cc
  000f4c1e: mov.w r10,#0x0
  000f4c22: mov.w r9,#0x1
  000f4c26: b 0x000f4dbc
  000f4c28: ldr r0,[r6,#0x4]
  000f4c2a: add.w r1,r9,#0x1
  000f4c2e: ldr.w r2,[r0,r9,lsl #0x2]
  000f4c32: cmp r2,#0x1
  000f4c34: beq 0x000f4ce6
  000f4c36: cmp r2,#0x0
  000f4c38: bne.w 0x000f4db6
  000f4c3c: ldr.w r1,[r0,r1,lsl #0x2]
  000f4c40: add.w r0,r0,r9, lsl #0x2
  000f4c44: rsbs r1,r1
  000f4c46: vmov s0,r1
  000f4c4a: ldr r1,[0x000f4e08]
  000f4c4c: vcvt.f32.s32 s0,s0
  000f4c50: vstr.32 s0,[sp,#0x2c]
  000f4c54: vldr.32 s0,[r0,#0x8]
  000f4c58: vcvt.f32.s32 s0,s0
  000f4c5c: vstr.32 s0,[sp,#0x34]
  000f4c60: vldr.32 s0,[r0,#0xc]
  000f4c64: vcvt.f32.s32 s0,s0
  000f4c68: vstr.32 s0,[sp,#0x30]
  000f4c6c: vldr.32 s0,[r0,#0x10]
  000f4c70: add r0,sp,#0x20
  000f4c72: vcvt.f32.s32 s0,s0
  000f4c76: vstr.32 s0,[sp,#0x20]
  000f4c7a: blx 0x00072628
  000f4c7e: vldr.32 s0,[sp,#0x20]
  000f4c82: vcmpe.f32 s0,#0
  000f4c86: vmrs apsr,fpscr
  000f4c8a: bpl 0x000f4c94
  000f4c8c: ldr r1,[0x000f4e0c]
  000f4c8e: add r0,sp,#0x20
  000f4c90: blx 0x00072628
  000f4c94: add.w r9,r9,#0x5
  000f4c98: cmp.w r8,#0x0
  000f4c9c: bne 0x000f4cb2
  000f4c9e: add r0,sp,#0x2c
  000f4ca0: mov.w r1,#0x40000000
  000f4ca4: blx 0x00072628
  000f4ca8: add r0,sp,#0x20
  000f4caa: mov.w r1,#0x40000000
  000f4cae: blx 0x00072628
  000f4cb2: movs r0,#0x48
  000f4cb4: blx 0x0006eb24
  000f4cb8: mov r5,r0
  000f4cba: vldr.32 s0,[sp,#0x2c]
  000f4cbe: vldr.32 s2,[sp,#0x30]
  000f4cc2: vldr.32 s4,[sp,#0x34]
  000f4cc6: vldr.32 s6,[sp,#0x20]
  000f4cca: movs r1,#0x0
  000f4ccc: movs r2,#0x0
  000f4cce: movs r3,#0x0
  000f4cd0: vstr.32 s0,[sp]
  000f4cd4: vstr.32 s2,[sp,#0x4]
  000f4cd8: vstr.32 s4,[sp,#0x8]
  000f4cdc: vstr.32 s6,[sp,#0xc]
  000f4ce0: blx 0x000730e4
  000f4ce4: b 0x000f4dac
  000f4ce6: ldr.w r1,[r0,r1,lsl #0x2]
  000f4cea: add.w r0,r0,r9, lsl #0x2
  000f4cee: rsbs r1,r1
  000f4cf0: vmov s0,r1
  000f4cf4: ldr r1,[0x000f4e04]
  000f4cf6: vcvt.f32.s32 s0,s0
  000f4cfa: vstr.32 s0,[sp,#0x2c]
  000f4cfe: vldr.32 s0,[r0,#0x8]
  000f4d02: vcvt.f32.s32 s0,s0
  000f4d06: vstr.32 s0,[sp,#0x34]
  000f4d0a: vldr.32 s0,[r0,#0xc]
  000f4d0e: vcvt.f32.s32 s0,s0
  000f4d12: vstr.32 s0,[sp,#0x30]
  000f4d16: vldr.32 s0,[r0,#0x10]
  000f4d1a: vcvt.f32.s32 s0,s0
  000f4d1e: vstr.32 s0,[sp,#0x20]
  000f4d22: vldr.32 s0,[r0,#0x14]
  000f4d26: vcvt.f32.s32 s0,s0
  000f4d2a: vstr.32 s0,[sp,#0x28]
  000f4d2e: vldr.32 s0,[r0,#0x18]
  000f4d32: add r0,sp,#0x20
  000f4d34: vcvt.f32.s32 s0,s0
  000f4d38: vstr.32 s0,[sp,#0x24]
  000f4d3c: blx 0x00072628
  000f4d40: add.w r9,r9,#0x7
  000f4d44: cmp.w r8,#0x0
  000f4d48: bne 0x000f4d5e
  000f4d4a: add r0,sp,#0x2c
  000f4d4c: mov.w r1,#0x40000000
  000f4d50: blx 0x00072628
  000f4d54: add r0,sp,#0x20
  000f4d56: mov.w r1,#0x40000000
  000f4d5a: blx 0x00072628
  000f4d5e: movs r0,#0x2c
  000f4d60: blx 0x0006eb24
  000f4d64: vldr.32 s4,[sp,#0x28]
  000f4d68: mov r5,r0
  000f4d6a: vldr.32 s0,[sp,#0x20]
  000f4d6e: vldr.32 s2,[sp,#0x24]
  000f4d72: vadd.f32 s4,s4,s4
  000f4d76: vldr.32 s6,[sp,#0x2c]
  000f4d7a: vldr.32 s8,[sp,#0x30]
  000f4d7e: vldr.32 s10,[sp,#0x34]
  000f4d82: vadd.f32 s2,s2,s2
  000f4d86: movs r1,#0x0
  000f4d88: vadd.f32 s0,s0,s0
  000f4d8c: movs r2,#0x0
  000f4d8e: movs r3,#0x0
  000f4d90: vstr.32 s6,[sp]
  000f4d94: vstr.32 s8,[sp,#0x4]
  000f4d98: vstr.32 s10,[sp,#0x8]
  000f4d9c: vstr.32 s4,[sp,#0x14]
  000f4da0: vstr.32 s2,[sp,#0x10]
  000f4da4: vstr.32 s0,[sp,#0xc]
  000f4da8: blx 0x00074350
  000f4dac: ldr.w r0,[r11,#0x4]
  000f4db0: str.w r5,[r0,r10,lsl #0x2]
  000f4db4: b 0x000f4db8
  000f4db6: mov r9,r1
  000f4db8: add.w r10,r10,#0x1
  000f4dbc: cmp r10,r4
  000f4dbe: blt.w 0x000f4c28
  000f4dc2: mov r0,r6
  000f4dc4: blx 0x0007369c
  000f4dc8: mov r0,r6
  000f4dca: blx 0x00070204
  000f4dce: blx 0x0006eb48
  000f4dd2: ldr r4,[sp,#0x1c]
  000f4dd4: ldr r0,[sp,#0x38]
  000f4dd6: ldr r1,[r4,#0x0]
  000f4dd8: subs r0,r1,r0
  000f4dda: itttt eq
  000f4ddc: mov.eq r0,r11
  000f4dde: add.eq sp,#0x3c
  000f4de0: pop.eq.w {r8,r9,r10,r11}
  000f4de4: pop.eq {r4,r5,r6,r7,pc}
  000f4de6: blx 0x0006e824
```
