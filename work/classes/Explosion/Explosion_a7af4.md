# Explosion::Explosion
elf 0xa7af4 body 796
Sig: undefined __thiscall Explosion(Explosion * this, int param_1)

## decompile
```c

/* Explosion::Explosion(int) */

Explosion * __thiscall Explosion::Explosion(Explosion *this,int param_1)

{
  void *pvVar1;
  AEGeometry *pAVar2;
  Transform *this_00;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  uint uVar9;
  uint *puVar10;
  bool bVar11;
  float fVar12;
  float extraout_s0;
  
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x2c));
  *(undefined4 *)(this + 0x24) = 0x3f800000;
  *(int *)this = param_1;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  switch(param_1) {
  case 0:
  case 6:
    pAVar2 = operator_new(0xc0);
    puVar8 = *(undefined4 **)(DAT_000b7e60 + 0xb7b4a);
    AEGeometry::AEGeometry(pAVar2,0x41b5,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x41b4,(PaintCanvas *)*puVar8,false);
    AEGeometry::addChild(*(uint *)(this + 4));
    pvVar1 = (void *)AEGeometry::~AEGeometry(pAVar2);
    operator_delete(pvVar1);
    goto LAB_000b7d6e;
  default:
    pAVar2 = operator_new(0xc0);
    puVar10 = *(uint **)(DAT_000b7e8c + 0xb7b96);
    if (param_1 == 0xd) {
      fVar12 = (float)AEGeometry::AEGeometry(pAVar2,0x41a9,(PaintCanvas *)*puVar10,false);
      *(AEGeometry **)(this + 4) = pAVar2;
      setScaling(fVar12);
      this_00 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
      AbyssEngine::Transform::SetAnimationSpeed(this_00,extraout_s0);
      goto LAB_000b7d6e;
    }
    AEGeometry::AEGeometry(pAVar2,0x4213,(PaintCanvas *)*puVar10,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x4211,(PaintCanvas *)*puVar10,false);
    break;
  case 3:
    pAVar2 = operator_new(0xc0);
    puVar8 = *(undefined4 **)(DAT_000b7e64 + 0xb7be0);
    AEGeometry::AEGeometry(pAVar2,0x4213,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x421d,(PaintCanvas *)*puVar8,false);
    break;
  case 4:
    pAVar2 = operator_new(0xc0);
    puVar8 = *(undefined4 **)(DAT_000b7e68 + 0xb7c16);
    AEGeometry::AEGeometry(pAVar2,0x420d,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x420c,(PaintCanvas *)*puVar8,false);
    break;
  case 5:
    pAVar2 = operator_new(0xc0);
    puVar8 = *(undefined4 **)(DAT_000b7e6c + 0xb7c4c);
    AEGeometry::AEGeometry(pAVar2,0x4999,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x4998,(PaintCanvas *)*puVar8,false);
    break;
  case 7:
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar2,0x41a5,(PaintCanvas *)**(undefined4 **)(DAT_000b7e70 + 0xb7c82),false);
    goto LAB_000b7d3e;
  case 8:
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar2,0x41a6,(PaintCanvas *)**(undefined4 **)(DAT_000b7e78 + 0xb7ca0),false);
    goto LAB_000b7d3e;
  case 9:
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar2,0x41a7,(PaintCanvas *)**(undefined4 **)(DAT_000b7e7c + 0xb7cbe),false);
    goto LAB_000b7d3e;
  case 10:
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar2,0x41a8,(PaintCanvas *)**(undefined4 **)(DAT_000b7e80 + 0xb7cdc),false);
    goto LAB_000b7d3e;
  case 0xb:
    pAVar2 = operator_new(0xc0);
    puVar8 = *(undefined4 **)(DAT_000b7e84 + 0xb7cfa);
    AEGeometry::AEGeometry(pAVar2,0x4a34,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 4) = pAVar2;
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,0x4a33,(PaintCanvas *)*puVar8,false);
    break;
  case 0xc:
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar2,0x4a7e,(PaintCanvas *)**(undefined4 **)(DAT_000b7e88 + 0xb7d30),false);
LAB_000b7d3e:
    *(AEGeometry **)(this + 4) = pAVar2;
    goto LAB_000b7d6e;
  }
  *(AEGeometry **)(this + 8) = pAVar2;
LAB_000b7d6e:
  puVar10 = *(uint **)(DAT_000b7e74 + 0xb7d78);
  uVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
  AbyssEngine::Transform::SetAnimationState(uVar3,1,0);
  if (*(int *)(this + 8) != 0) {
    uVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    AbyssEngine::Transform::SetAnimationState(uVar3,1,0);
    iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    *(undefined4 *)(iVar4 + 0xe0) = DAT_000b7e5c;
  }
  iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
  uVar9 = *(uint *)(iVar4 + 0xf8);
  iVar4 = *(int *)(iVar4 + 0xfc);
  if (*(int *)(this + 8) == 0) {
    uVar6 = 0;
    iVar5 = 0;
  }
  else {
    iVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    uVar6 = *(uint *)(iVar5 + 0xf8);
    iVar5 = *(int *)(iVar5 + 0xfc);
  }
  bVar11 = uVar6 < uVar9;
  if (((int)((iVar5 - iVar4) - (uint)bVar11) < 0 ==
       (SBORROW4(iVar5,iVar4) != SBORROW4(iVar5 - iVar4,(uint)bVar11))) && (*(int *)(this + 8) == 0)
     ) {
    uVar3 = 0;
    uVar7 = 0;
  }
  else {
    iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    uVar3 = *(undefined4 *)(iVar4 + 0xf8);
    uVar7 = *(undefined4 *)(iVar4 + 0xfc);
  }
  *(undefined4 *)(this + 0x10) = uVar3;
  *(undefined4 *)(this + 0x14) = uVar7;
  iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
  *(undefined4 *)(iVar4 + 0xe0) = DAT_000b7e5c;
  *(undefined4 *)(this + 0x28) = 0xffffffff;
  reset(this);
  return this;
}

```

## target disasm
```
  000b7af4: push {r4,r5,r6,r7,lr}
  000b7af6: add r7,sp,#0xc
  000b7af8: push {r8,r9,r11}
  000b7afc: mov r9,r0
  000b7afe: adds r0,#0x2c
  000b7b00: mov r6,r1
  000b7b02: blx 0x0006f118
  000b7b06: mov.w r1,#0x3f800000
  000b7b0a: cmp r6,#0xc
  000b7b0c: mov.w r0,#0x0
  000b7b10: str.w r1,[r9,#0x24]
  000b7b14: strd r6,r0,[r9,#0x0]
  000b7b18: strd r0,r0,[r9,#0x8]
  000b7b1c: bhi 0x000b7b86
  000b7b1e: tbh [pc,r6]
  000b7b3c: movs r0,#0xc0
  000b7b3e: blx 0x0006eb24
  000b7b42: mov r5,r0
  000b7b44: ldr r0,[0x000b7e60]
  000b7b46: add r0,pc
  000b7b48: ldr r4,[r0,#0x0]
  000b7b4a: ldr r2,[r4,#0x0]
  000b7b4c: mov r0,r5
  000b7b4e: movw r1,#0x41b5
  000b7b52: movs r3,#0x0
  000b7b54: blx 0x0007207c
  000b7b58: movs r0,#0xc0
  000b7b5a: str.w r5,[r9,#0x4]
  000b7b5e: blx 0x0006eb24
  000b7b62: mov r5,r0
  000b7b64: ldr r2,[r4,#0x0]
  000b7b66: movw r1,#0x41b4
  000b7b6a: movs r3,#0x0
  000b7b6c: blx 0x0007207c
  000b7b70: ldr r1,[r5,#0xc]
  000b7b72: ldr.w r0,[r9,#0x4]
  000b7b76: blx 0x000720e8
  000b7b7a: mov r0,r5
  000b7b7c: blx 0x00071fc8
  000b7b80: blx 0x0006eb48
  000b7b84: b 0x000b7d6e
  000b7b86: movs r0,#0xc0
  000b7b88: blx 0x0006eb24
  000b7b8c: mov r5,r0
  000b7b8e: ldr r0,[0x000b7e8c]
  000b7b90: cmp r6,#0xd
  000b7b92: add r0,pc
  000b7b94: ldr.w r8,[r0,#0x0]
  000b7b98: ldr.w r2,[r8,#0x0]
  000b7b9c: bne.w 0x000b7d44
  000b7ba0: mov r0,r5
  000b7ba2: movw r1,#0x41a9
  000b7ba6: movs r3,#0x0
  000b7ba8: blx 0x0007207c
  000b7bac: mov r0,r9
  000b7bae: mov.w r1,#0x3e800000
  000b7bb2: str.w r5,[r9,#0x4]
  000b7bb6: blx 0x000727fc
  000b7bba: ldr.w r0,[r9,#0x4]
  000b7bbe: ldr r1,[r0,#0xc]
  000b7bc0: ldr.w r0,[r8,#0x0]
  000b7bc4: blx 0x00072088
  000b7bc8: mov.w r1,#0x3f800000
  000b7bcc: blx 0x00073570
  000b7bd0: b 0x000b7d6e
  000b7bd2: movs r0,#0xc0
  000b7bd4: blx 0x0006eb24
  000b7bd8: mov r5,r0
  000b7bda: ldr r0,[0x000b7e64]
  000b7bdc: add r0,pc
  000b7bde: ldr r4,[r0,#0x0]
  000b7be0: ldr r2,[r4,#0x0]
  000b7be2: mov r0,r5
  000b7be4: movw r1,#0x4213
  000b7be8: movs r3,#0x0
  000b7bea: blx 0x0007207c
  000b7bee: movs r0,#0xc0
  000b7bf0: str.w r5,[r9,#0x4]
  000b7bf4: blx 0x0006eb24
  000b7bf8: mov r5,r0
  000b7bfa: ldr r2,[r4,#0x0]
  000b7bfc: movw r1,#0x421d
  000b7c00: movs r3,#0x0
  000b7c02: blx 0x0007207c
  000b7c06: b 0x000b7d6a
  000b7c08: movs r0,#0xc0
  000b7c0a: blx 0x0006eb24
  000b7c0e: mov r5,r0
  000b7c10: ldr r0,[0x000b7e68]
  000b7c12: add r0,pc
  000b7c14: ldr r4,[r0,#0x0]
  000b7c16: ldr r2,[r4,#0x0]
  000b7c18: mov r0,r5
  000b7c1a: movw r1,#0x420d
  000b7c1e: movs r3,#0x0
  000b7c20: blx 0x0007207c
  000b7c24: movs r0,#0xc0
  000b7c26: str.w r5,[r9,#0x4]
  000b7c2a: blx 0x0006eb24
  000b7c2e: mov r5,r0
  000b7c30: ldr r2,[r4,#0x0]
  000b7c32: movw r1,#0x420c
  000b7c36: movs r3,#0x0
  000b7c38: blx 0x0007207c
  000b7c3c: b 0x000b7d6a
  000b7c3e: movs r0,#0xc0
  000b7c40: blx 0x0006eb24
  000b7c44: mov r5,r0
  000b7c46: ldr r0,[0x000b7e6c]
  000b7c48: add r0,pc
  000b7c4a: ldr r4,[r0,#0x0]
  000b7c4c: ldr r2,[r4,#0x0]
  000b7c4e: mov r0,r5
  000b7c50: movw r1,#0x4999
  000b7c54: movs r3,#0x0
  000b7c56: blx 0x0007207c
  000b7c5a: movs r0,#0xc0
  000b7c5c: str.w r5,[r9,#0x4]
  000b7c60: blx 0x0006eb24
  000b7c64: mov r5,r0
  000b7c66: ldr r2,[r4,#0x0]
  000b7c68: movw r1,#0x4998
  000b7c6c: movs r3,#0x0
  000b7c6e: blx 0x0007207c
  000b7c72: b 0x000b7d6a
  000b7c74: movs r0,#0xc0
  000b7c76: blx 0x0006eb24
  000b7c7a: mov r5,r0
  000b7c7c: ldr r0,[0x000b7e70]
  000b7c7e: add r0,pc
  000b7c80: ldr r0,[r0,#0x0]
  000b7c82: ldr r2,[r0,#0x0]
  000b7c84: mov r0,r5
  000b7c86: movw r1,#0x41a5
  000b7c8a: movs r3,#0x0
  000b7c8c: blx 0x0007207c
  000b7c90: b 0x000b7d3e
  000b7c92: movs r0,#0xc0
  000b7c94: blx 0x0006eb24
  000b7c98: mov r5,r0
  000b7c9a: ldr r0,[0x000b7e78]
  000b7c9c: add r0,pc
  000b7c9e: ldr r0,[r0,#0x0]
  000b7ca0: ldr r2,[r0,#0x0]
  000b7ca2: mov r0,r5
  000b7ca4: movw r1,#0x41a6
  000b7ca8: movs r3,#0x0
  000b7caa: blx 0x0007207c
  000b7cae: b 0x000b7d3e
  000b7cb0: movs r0,#0xc0
  000b7cb2: blx 0x0006eb24
  000b7cb6: mov r5,r0
  000b7cb8: ldr r0,[0x000b7e7c]
  000b7cba: add r0,pc
  000b7cbc: ldr r0,[r0,#0x0]
  000b7cbe: ldr r2,[r0,#0x0]
  000b7cc0: mov r0,r5
  000b7cc2: movw r1,#0x41a7
  000b7cc6: movs r3,#0x0
  000b7cc8: blx 0x0007207c
  000b7ccc: b 0x000b7d3e
  000b7cce: movs r0,#0xc0
  000b7cd0: blx 0x0006eb24
  000b7cd4: mov r5,r0
  000b7cd6: ldr r0,[0x000b7e80]
  000b7cd8: add r0,pc
  000b7cda: ldr r0,[r0,#0x0]
  000b7cdc: ldr r2,[r0,#0x0]
  000b7cde: mov r0,r5
  000b7ce0: movw r1,#0x41a8
  000b7ce4: movs r3,#0x0
  000b7ce6: blx 0x0007207c
  000b7cea: b 0x000b7d3e
  000b7cec: movs r0,#0xc0
  000b7cee: blx 0x0006eb24
  000b7cf2: mov r5,r0
  000b7cf4: ldr r0,[0x000b7e84]
  000b7cf6: add r0,pc
  000b7cf8: ldr r4,[r0,#0x0]
  000b7cfa: ldr r2,[r4,#0x0]
  000b7cfc: mov r0,r5
  000b7cfe: movw r1,#0x4a34
  000b7d02: movs r3,#0x0
  000b7d04: blx 0x0007207c
  000b7d08: movs r0,#0xc0
  000b7d0a: str.w r5,[r9,#0x4]
  000b7d0e: blx 0x0006eb24
  000b7d12: mov r5,r0
  000b7d14: ldr r2,[r4,#0x0]
  000b7d16: movw r1,#0x4a33
  000b7d1a: movs r3,#0x0
  000b7d1c: blx 0x0007207c
  000b7d20: b 0x000b7d6a
  000b7d22: movs r0,#0xc0
  000b7d24: blx 0x0006eb24
  000b7d28: mov r5,r0
  000b7d2a: ldr r0,[0x000b7e88]
  000b7d2c: add r0,pc
  000b7d2e: ldr r0,[r0,#0x0]
  000b7d30: ldr r2,[r0,#0x0]
  000b7d32: mov r0,r5
  000b7d34: movw r1,#0x4a7e
  000b7d38: movs r3,#0x0
  000b7d3a: blx 0x0007207c
  000b7d3e: str.w r5,[r9,#0x4]
  000b7d42: b 0x000b7d6e
  000b7d44: mov r0,r5
  000b7d46: movw r1,#0x4213
  000b7d4a: movs r3,#0x0
  000b7d4c: blx 0x0007207c
  000b7d50: movs r0,#0xc0
  000b7d52: str.w r5,[r9,#0x4]
  000b7d56: blx 0x0006eb24
  000b7d5a: mov r5,r0
  000b7d5c: ldr.w r2,[r8,#0x0]
  000b7d60: movw r1,#0x4211
  000b7d64: movs r3,#0x0
  000b7d66: blx 0x0007207c
  000b7d6a: str.w r5,[r9,#0x8]
  000b7d6e: ldr r0,[0x000b7e74]
  000b7d70: ldr.w r1,[r9,#0x4]
  000b7d74: add r0,pc
  000b7d76: ldr r5,[r0,#0x0]
  000b7d78: ldr r1,[r1,#0xc]
  000b7d7a: ldr r0,[r5,#0x0]
  000b7d7c: blx 0x00072088
  000b7d80: movs r1,#0x1
  000b7d82: movs r2,#0x0
  000b7d84: blx 0x0006fd18
  000b7d88: ldr.w r0,[r9,#0x8]
  000b7d8c: cbz r0,0x000b7db0
  000b7d8e: ldr r1,[r0,#0xc]
  000b7d90: ldr r0,[r5,#0x0]
  000b7d92: blx 0x00072088
  000b7d96: movs r1,#0x1
  000b7d98: movs r2,#0x0
  000b7d9a: blx 0x0006fd18
  000b7d9e: ldr.w r1,[r9,#0x8]
  000b7da2: ldr r0,[r5,#0x0]
  000b7da4: ldr r1,[r1,#0xc]
  000b7da6: blx 0x00072088
  000b7daa: ldr r1,[0x000b7e5c]
  000b7dac: str.w r1,[r0,#0xe0]
  000b7db0: ldr.w r0,[r9,#0x4]
  000b7db4: ldr r1,[r0,#0xc]
  000b7db6: ldr r0,[r5,#0x0]
  000b7db8: blx 0x00072088
  000b7dbc: ldrd r4,r6,[r0,#0xf8]
  000b7dc0: ldr.w r0,[r9,#0x8]
  000b7dc4: cbz r0,0x000b7dd4
  000b7dc6: ldr r1,[r0,#0xc]
  000b7dc8: ldr r0,[r5,#0x0]
  000b7dca: blx 0x00072088
  000b7dce: ldrd r0,r1,[r0,#0xf8]
  000b7dd2: b 0x000b7dd8
  000b7dd4: movs r0,#0x0
  000b7dd6: movs r1,#0x0
  000b7dd8: subs r0,r0,r4
  000b7dda: sbcs.w r0,r1,r6
  000b7dde: bge 0x000b7de6
  000b7de0: ldr.w r0,[r9,#0x4]
  000b7de4: b 0x000b7dec
  000b7de6: ldr.w r0,[r9,#0x8]
  000b7dea: cbz r0,0x000b7e24
  000b7dec: ldr r1,[r0,#0xc]
  000b7dee: ldr r0,[r5,#0x0]
  000b7df0: blx 0x00072088
  000b7df4: ldrd r0,r1,[r0,#0xf8]
  000b7df8: ldr.w r2,[r9,#0x4]
  000b7dfc: strd r0,r1,[r9,#0x10]
  000b7e00: ldr r0,[r5,#0x0]
  000b7e02: ldr r1,[r2,#0xc]
  000b7e04: blx 0x00072088
  000b7e08: ldr r1,[0x000b7e5c]
  000b7e0a: str.w r1,[r0,#0xe0]
  000b7e0e: mov.w r0,#0xffffffff
  000b7e12: str.w r0,[r9,#0x28]
  000b7e16: mov r0,r9
  000b7e18: blx 0x00072c88
  000b7e1c: mov r0,r9
  000b7e1e: pop.w {r8,r9,r11}
  000b7e22: pop {r4,r5,r6,r7,pc}
  000b7e24: movs r0,#0x0
  000b7e26: movs r1,#0x0
  000b7e28: b 0x000b7df8
```
