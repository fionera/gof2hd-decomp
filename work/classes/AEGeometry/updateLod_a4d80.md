# AEGeometry::updateLod
elf 0xa4d80 body 518
Sig: undefined __thiscall updateLod(AEGeometry * this, Vector * param_1, float param_2)

## decompile
```c

/* AEGeometry::updateLod(AbyssEngine::AEMath::Vector const&, float) */

void __thiscall AEGeometry::updateLod(AEGeometry *this,Vector *param_1,float param_2)

{
  bool bVar1;
  undefined4 uVar2;
  float fVar3;
  LodMeshMerger *pLVar4;
  uint uVar5;
  Matrix *pMVar6;
  float in_r2;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  float fVar12;
  float fVar13;
  undefined8 uVar14;
  longlong lVar15;
  Matrix aMStack_90 [12];
  Vector aVStack_84 [12];
  Matrix aMStack_78 [60];
  int local_3c;
  
  piVar11 = *(int **)(DAT_000b4f90 + 0xb4d9a);
  local_3c = *piVar11;
  this[0x48] = this[0x49];
  uVar2 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(this + 0x2c));
  __aeabi_memcpy(aMStack_78,uVar2,0x3c);
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(this + 0x2c));
  AbyssEngine::AEMath::MatrixGetPosition(aMStack_90);
  AbyssEngine::AEMath::operator-(aVStack_84,(Vector *)aMStack_90);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x78),aVStack_84);
  uVar14 = __aeabi_f2ulz(*(float *)(this + 0x7c) * *(float *)(this + 0x7c) +
                         *(float *)(this + 0x78) * *(float *)(this + 0x78) +
                         *(float *)(this + 0x80) * *(float *)(this + 0x80));
  uVar5 = (uint)((ulonglong)uVar14 >> 0x20);
  uVar7 = *(uint *)(this + 0x74);
  *(undefined8 *)(this + 0x68) = uVar14;
  if ((*(uint *)(this + 0x70) == 0 && uVar7 == 0) ||
     (bVar1 = (uint)(*(uint *)(this + 0x70) <= (uint)uVar14) <= uVar5 - uVar7,
     this[0x48] = (AEGeometry)(uVar5 <= uVar7 && bVar1), uVar5 <= uVar7 && bVar1)) {
    AbyssEngine::PaintCanvas::TransformGetTransform(*(uint *)(this + 0x2c));
    fVar12 = 1.0;
    if (in_r2 <= DAT_000b4f88) {
      fVar12 = 0.75;
    }
    iVar9 = *(int *)(this + 0x50);
    iVar8 = iVar9 * 8;
    fVar13 = 0.5;
    if (DAT_000b4f8c < in_r2) {
      fVar13 = fVar12;
    }
    do {
      iVar10 = iVar9;
      iVar8 = iVar8 + -8;
      if (iVar10 < 1) {
        AbyssEngine::PaintCanvas::TransformSetLocal
                  (*(uint *)(this + 0x2c),*(Matrix **)(this + 0x18));
        pLVar4 = *(LodMeshMerger **)(this + 4);
        *(undefined4 *)(this + 0x28) = 0;
        *(undefined4 *)(this + 0xc) = *(undefined4 *)(this + 0x18);
        if (pLVar4 == (LodMeshMerger *)0x0) goto LAB_000b4f68;
        uVar2 = *(undefined4 *)this;
        iVar8 = 0;
        goto LAB_000b4f5a;
      }
      iVar9 = iVar10 + -1;
      fVar12 = (float)__aeabi_ul2f(*(undefined4 *)(*(int *)(this + 100) + iVar8),
                                   *(undefined4 *)(*(int *)(this + 100) + iVar8 + 4));
      fVar3 = (float)__aeabi_ul2f(*(undefined4 *)(this + 0x68),*(undefined4 *)(this + 0x6c));
    } while (-1 < (int)((uint)(fVar13 * fVar12 < fVar3) << 0x1f));
    pMVar6 = *(Matrix **)(*(int *)(this + 0x54) + iVar9 * 4);
    if (pMVar6 != *(Matrix **)(this + 0xc)) {
      AbyssEngine::PaintCanvas::TransformSetLocal(*(uint *)(this + 0x2c),pMVar6);
      *(undefined4 *)(this + 0xc) = *(undefined4 *)(*(int *)(this + 0x54) + iVar9 * 4);
      lVar15 = AbyssEngine::PaintCanvas::TransformGetTransform(*(uint *)(this + 0x2c));
      AbyssEngine::Transform::SetCurrentAnimationTime(lVar15);
      lVar15 = AbyssEngine::PaintCanvas::TransformGetTransform(*(uint *)(this + 0x2c));
      AbyssEngine::Transform::SetCurrentAnimationTime(lVar15);
      *(int *)(this + 0x28) = iVar10;
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x84),aMStack_78);
      pLVar4 = *(LodMeshMerger **)(this + 4);
      if (pLVar4 != (LodMeshMerger *)0x0) {
        uVar2 = *(undefined4 *)this;
        iVar8 = (int)(char)iVar10;
LAB_000b4f5a:
        LodMeshMerger::setLod(pLVar4,uVar2,iVar8);
      }
    }
  }
  else {
    *(undefined4 *)(this + 0x28) = 0xffffffff;
  }
LAB_000b4f68:
  if (*piVar11 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b4d80: push {r4,r5,r6,r7,lr}
  000b4d82: add r7,sp,#0xc
  000b4d84: push {r7,r8,r9,r10,r11}
  000b4d88: vpush {d8,d9}
  000b4d8c: sub sp,#0x58
  000b4d8e: mov r11,r0
  000b4d90: ldr r0,[0x000b4f90]
  000b4d92: mov r9,r1
  000b4d94: mov r8,r2
  000b4d96: add r0,pc
  000b4d98: ldr.w r10,[r0,#0x0]
  000b4d9c: ldr.w r0,[r10,#0x0]
  000b4da0: str r0,[sp,#0x54]
  000b4da2: ldrb.w r0,[r11,#0x49]
  000b4da6: strb.w r0,[r11,#0x48]
  000b4daa: ldr.w r1,[r11,#0xc]
  000b4dae: ldr.w r0,[r11,#0x2c]
  000b4db2: blx 0x000720c4
  000b4db6: mov r1,r0
  000b4db8: add r0,sp,#0x18
  000b4dba: movs r2,#0x3c
  000b4dbc: blx 0x0006f1e4
  000b4dc0: ldr.w r1,[r11,#0x24]
  000b4dc4: adds r0,r1,#0x1
  000b4dc6: it eq
  000b4dc8: ldr.eq.w r1,[r11,#0xc]
  000b4dcc: ldr.w r0,[r11,#0x2c]
  000b4dd0: blx 0x000720c4
  000b4dd4: mov r5,sp
  000b4dd6: mov r1,r0
  000b4dd8: mov r0,r5
  000b4dda: blx 0x0006f16c
  000b4dde: add r6,sp,#0xc
  000b4de0: mov r1,r5
  000b4de2: mov r2,r9
  000b4de4: mov r0,r6
  000b4de6: blx 0x0006ec38
  000b4dea: add.w r0,r11,#0x78
  000b4dee: mov r1,r6
  000b4df0: blx 0x0006eb3c
  000b4df4: vldr.32 s2,[r11,#0x7c]
  000b4df8: vldr.32 s0,[r11,#0x78]
  000b4dfc: vmul.f32 s2,s2,s2
  000b4e00: vldr.32 s4,[r11,#0x80]
  000b4e04: vmla.f32 s2,s0,s0
  000b4e08: vmla.f32 s2,s4,s4
  000b4e0c: vmov r0,s2
  000b4e10: blx 0x00073078
  000b4e14: ldrd r3,r2,[r11,#0x70]
  000b4e18: strd r0,r1,[r11,#0x68]
  000b4e1c: orrs.w r6,r3,r2
  000b4e20: beq 0x000b4e3a
  000b4e22: subs r0,r0,r3
  000b4e24: mov.w r6,#0x0
  000b4e28: sbcs.w r0,r1,r2
  000b4e2c: it cc
  000b4e2e: mov.cc r6,#0x1
  000b4e30: cmp r6,#0x0
  000b4e32: strb.w r6,[r11,#0x48]
  000b4e36: beq.w 0x000b4f60
  000b4e3a: ldr.w r1,[r11,#0xc]
  000b4e3e: vmov s18,r8
  000b4e42: ldr.w r0,[r11,#0x2c]
  000b4e46: add.w r6,r11,#0x68
  000b4e4a: blx 0x00072088
  000b4e4e: vldr.32 s2,[pc,#0x138]
  000b4e52: vmov.f32 s4,0x3f400000
  000b4e56: vmov.f32 s0,0x3f800000
  000b4e5a: mvn r1,#0x7
  000b4e5e: vcmpe.f32 s18,s2
  000b4e62: vldr.32 s2,[pc,#0x128]
  000b4e66: vmrs apsr,fpscr
  000b4e6a: vmov.f32 s16,0x3f000000
  000b4e6e: vcmpe.f32 s18,s2
  000b4e72: it ls
  000b4e74: vmov.ls.f32 s0,s4
  000b4e78: vmrs apsr,fpscr
  000b4e7c: ldr.w r5,[r11,#0x50]
  000b4e80: add.w r4,r1,r5, lsl #0x3
  000b4e84: it hi
  000b4e86: vmov.hi.f32 s16,s0
  000b4e8a: ldrd r8,r9,[r0,#0x110]
  000b4e8e: cmp r5,#0x1
  000b4e90: blt 0x000b4f30
  000b4e92: ldr.w r0,[r11,#0x64]
  000b4e96: subs r5,#0x1
  000b4e98: adds r1,r0,r4
  000b4e9a: ldr r0,[r0,r4]
  000b4e9c: ldr r1,[r1,#0x4]
  000b4e9e: blx 0x00073084
  000b4ea2: vmov s0,r0
  000b4ea6: ldrd r0,r1,[r6,#0x0]
  000b4eaa: vmul.f32 s18,s16,s0
  000b4eae: blx 0x00073084
  000b4eb2: vmov s0,r0
  000b4eb6: subs r4,#0x8
  000b4eb8: vcmpe.f32 s18,s0
  000b4ebc: vmrs apsr,fpscr
  000b4ec0: bpl 0x000b4e8e
  000b4ec2: ldr.w r1,[r11,#0x54]
  000b4ec6: ldr.w r0,[r11,#0xc]
  000b4eca: ldr.w r1,[r1,r5,lsl #0x2]
  000b4ece: cmp r1,r0
  000b4ed0: beq 0x000b4f68
  000b4ed2: add r4,sp,#0x18
  000b4ed4: ldr.w r0,[r11,#0x2c]
  000b4ed8: adds r6,r5,#0x1
  000b4eda: mov r2,r4
  000b4edc: blx 0x000721c0
  000b4ee0: ldr.w r1,[r11,#0x54]
  000b4ee4: ldr.w r0,[r11,#0x2c]
  000b4ee8: ldr.w r2,[r1,r5,lsl #0x2]
  000b4eec: str.w r2,[r11,#0xc]
  000b4ef0: ldr.w r1,[r1,r5,lsl #0x2]
  000b4ef4: blx 0x00072088
  000b4ef8: mov r2,r8
  000b4efa: mov r3,r9
  000b4efc: blx 0x00073090
  000b4f00: ldr.w r1,[r11,#0x18]
  000b4f04: ldr.w r0,[r11,#0x2c]
  000b4f08: blx 0x00072088
  000b4f0c: mov r2,r8
  000b4f0e: mov r3,r9
  000b4f10: blx 0x00073090
  000b4f14: add.w r0,r11,#0x84
  000b4f18: mov r1,r4
  000b4f1a: str.w r6,[r11,#0x28]
  000b4f1e: blx 0x0006f148
  000b4f22: ldr.w r0,[r11,#0x4]
  000b4f26: cbz r0,0x000b4f68
  000b4f28: ldr.w r1,[r11,#0x0]
  000b4f2c: sxtb r2,r6
  000b4f2e: b 0x000b4f5a
  000b4f30: ldr.w r1,[r11,#0x18]
  000b4f34: add r2,sp,#0x18
  000b4f36: ldr.w r0,[r11,#0x2c]
  000b4f3a: blx 0x000721c0
  000b4f3e: ldr.w r0,[r11,#0x4]
  000b4f42: movs r2,#0x0
  000b4f44: ldr.w r1,[r11,#0x18]
  000b4f48: cmp r0,#0x0
  000b4f4a: str.w r2,[r11,#0x28]
  000b4f4e: str.w r1,[r11,#0xc]
  000b4f52: beq 0x000b4f68
  000b4f54: ldr.w r1,[r11,#0x0]
  000b4f58: movs r2,#0x0
  000b4f5a: blx 0x0007309c
  000b4f5e: b 0x000b4f68
  000b4f60: mov.w r0,#0xffffffff
  000b4f64: str.w r0,[r11,#0x28]
  000b4f68: ldr r0,[sp,#0x54]
  000b4f6a: ldr.w r1,[r10,#0x0]
  000b4f6e: subs r0,r1,r0
  000b4f70: itttt eq
  000b4f72: add.eq sp,#0x58
  000b4f74: vpop.eq {d8,d9}
  000b4f78: add.eq sp,#0x4
  000b4f7a: pop.eq.w {r8,r9,r10,r11}
  000b4f7e: it eq
  000b4f80: pop.eq {r4,r5,r6,r7,pc}
  000b4f82: blx 0x0006e824
```
