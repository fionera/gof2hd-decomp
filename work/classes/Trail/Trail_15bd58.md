# Trail::Trail
elf 0x15bd58 body 404
Sig: undefined __thiscall Trail(Trail * this, int param_1, int param_2)

## decompile
```c

/* Trail::Trail(int, int) */

Trail * __thiscall Trail::Trail(Trail *this,int param_1,int param_2)

{
  longlong lVar1;
  ushort uVar2;
  float fVar3;
  int iVar4;
  void *pvVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint *puVar10;
  uint in_fpscr;
  float fVar11;
  float extraout_s1;
  float extraout_s1_00;
  undefined8 uVar12;
  float extraout_s1_01;
  float fVar13;
  float fVar14;
  
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(int *)(this + 0x24) = param_2;
  iVar7 = 2;
  puVar10 = *(uint **)(DAT_0016bef0 + 0x16bda8);
  *(undefined4 *)(this + 0xc) = 0x3c;
  AbyssEngine::PaintCanvas::MeshCreate
            ((PaintCanvas *)*puVar10,param_2 * 2 + 2U & 0xffff,(param_2 & 0x7fffU) << 1,0x13,0x4e77)
  ;
  fVar13 = extraout_s1;
  while( true ) {
    fVar3 = DAT_0016beec;
    iVar4 = *(int *)(this + 0x24);
    if (iVar4 * 2 + -2 <= iVar7 + -2) break;
    uVar2 = (ushort)(iVar7 + -2);
    AbyssEngine::PaintCanvas::MeshSetTriangle
              (*puVar10,(ushort)*(undefined4 *)(this + 0x10),uVar2,uVar2,uVar2 | 1);
    AbyssEngine::PaintCanvas::MeshSetTriangle
              (*puVar10,(ushort)*(undefined4 *)(this + 0x10),uVar2 | 1,uVar2 + 3,(ushort)iVar7);
    iVar7 = iVar7 + 2;
    fVar13 = extraout_s1_00;
  }
  iVar7 = 0;
  for (iVar8 = 0; iVar6 = iVar4 * 2 + 2, iVar8 < iVar6; iVar8 = iVar8 + 2) {
    fVar11 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
    fVar14 = (float)VectorSignedToFloat(iVar4 + 1,(byte)(in_fpscr >> 0x16) & 3);
    fVar11 = (float)-(int)((fVar11 / fVar14) * fVar3);
    VectorSignedToFloat(fVar11,(byte)(in_fpscr >> 0x16) & 3);
    uVar12 = AbyssEngine::PaintCanvas::MeshSetUv
                       (*puVar10,(ushort)*(undefined4 *)(this + 0x10),fVar11,fVar13);
    AbyssEngine::PaintCanvas::MeshSetUv
              (*puVar10,(ushort)*(undefined4 *)(this + 0x10),(float)uVar12,
               (float)((ulonglong)uVar12 >> 0x20));
    iVar7 = iVar7 + 1;
    iVar4 = *(int *)(this + 0x24);
    fVar13 = extraout_s1_01;
  }
  uVar9 = iVar6 * 3;
  *(uint *)(this + 0x20) = uVar9;
  lVar1 = (ulonglong)uVar9 * 4;
  uVar9 = (uint)lVar1;
  if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
    uVar9 = 0xffffffff;
  }
  pvVar5 = operator_new__(uVar9);
  *(void **)(this + 0x18) = pvVar5;
  pvVar5 = operator_new__(uVar9);
  *(void **)(this + 0x1c) = pvVar5;
  AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar10,(uint *)(this + 0x14));
  AbyssEngine::PaintCanvas::TransformAddMeshId
            ((PaintCanvas *)*puVar10,*(uint *)(this + 0x14),*(uint *)(this + 0x10));
  changeType(this,param_1);
  return this;
}

```

## target disasm
```
  0016bd58: push {r4,r5,r6,r7,lr}
  0016bd5a: add r7,sp,#0xc
  0016bd5c: push {r7,r8,r9,r10,r11}
  0016bd60: vpush {d6,d7,d8,d9}
  0016bd64: mov r9,r0
  0016bd66: movs r0,#0x0
  0016bd68: strd r0,r0,[r9,#0x0]
  0016bd6c: str.w r0,[r9,#0x8]
  0016bd70: bic r0,r1,#0x3
  0016bd74: cmp r0,#0x4
  0016bd76: str.w r2,[r9,#0x24]
  0016bd7a: str r1,[sp,#0xc]
  0016bd7c: bne 0x0016bd84
  0016bd7e: vldr.32 s16,[pc,#0x16c]
  0016bd82: b 0x0016bd98
  0016bd84: vmov.f32 s0,0x3f800000
  0016bd88: vldr.32 s16,[pc,#0x160]
  0016bd8c: sub.w r0,r1,#0x9
  0016bd90: cmp r0,#0x3
  0016bd92: it cc
  0016bd94: vmov.cc.f32 s16,s0
  0016bd98: ldr r0,[0x0016bef0]
  0016bd9a: add.w r4,r9,#0x10
  0016bd9e: movw r1,#0x4e77
  0016bda2: movs r5,#0x2
  0016bda4: add r0,pc
  0016bda6: movs r3,#0x13
  0016bda8: ldr.w r11,[r0,#0x0]
  0016bdac: movs r0,#0x3c
  0016bdae: str.w r0,[r9,#0xc]
  0016bdb2: ldr.w r0,[r11,#0x0]
  0016bdb6: strd r1,r4,[sp,#0x0]
  0016bdba: add.w r1,r5,r2, lsl #0x1
  0016bdbe: lsls r2,r2,#0x1
  0016bdc0: uxth r1,r1
  0016bdc2: uxth r2,r2
  0016bdc4: blx 0x00075da8
  0016bdc8: b 0x0016be00
  0016bdca: orr r2,r8,#0x1
  0016bdce: ldr r1,[r4,#0x0]
  0016bdd0: ldr.w r0,[r11,#0x0]
  0016bdd4: uxth.w r10,r5
  0016bdd8: uxth r6,r2
  0016bdda: uxth.w r2,r8
  0016bdde: mov r3,r2
  0016bde0: strd r6,r10,[sp,#0x0]
  0016bde4: blx 0x000777d0
  0016bde8: add.w r2,r8,#0x3
  0016bdec: ldr r1,[r4,#0x0]
  0016bdee: ldr.w r0,[r11,#0x0]
  0016bdf2: uxth r3,r2
  0016bdf4: mov r2,r6
  0016bdf6: strd r10,r6,[sp,#0x0]
  0016bdfa: blx 0x000777d0
  0016bdfe: adds r5,#0x2
  0016be00: ldr.w r0,[r9,#0x24]
  0016be04: mvn r1,#0x1
  0016be08: sub.w r8,r5,#0x2
  0016be0c: add.w r1,r1,r0, lsl #0x1
  0016be10: cmp r8,r1
  0016be12: blt 0x0016bdca
  0016be14: vmov r6,s16
  0016be18: vldr.32 s18,[pc,#0xd0]
  0016be1c: movs r4,#0x0
  0016be1e: mov.w r10,#0x2
  0016be22: movs r5,#0x0
  0016be24: b 0x0016be86
  0016be26: adds r0,#0x1
  0016be28: vmov s0,r4
  0016be2c: uxth r2,r5
  0016be2e: mov r3,r6
  0016be30: vmov s2,r0
  0016be34: vcvt.f32.s32 s0,s0
  0016be38: vcvt.f32.s32 s2,s2
  0016be3c: vdiv.f32 s0,s0,s2
  0016be40: vmul.f32 s0,s0,s18
  0016be44: vcvt.s32.f32 s0,s0
  0016be48: vmov r0,s0
  0016be4c: rsbs r0,r0
  0016be4e: vmov s0,r0
  0016be52: vcvt.f32.s32 s16,s0
  0016be56: ldr.w r1,[r9,#0x10]
  0016be5a: ldr.w r0,[r11,#0x0]
  0016be5e: vstr.32 s16,[sp]
  0016be62: blx 0x000777dc
  0016be66: orr r2,r5,#0x1
  0016be6a: ldr.w r1,[r9,#0x10]
  0016be6e: ldr.w r0,[r11,#0x0]
  0016be72: mov r3,r6
  0016be74: uxth r2,r2
  0016be76: vstr.32 s16,[sp]
  0016be7a: blx 0x000777dc
  0016be7e: adds r4,#0x1
  0016be80: ldr.w r0,[r9,#0x24]
  0016be84: adds r5,#0x2
  0016be86: add.w r1,r10,r0, lsl #0x1
  0016be8a: cmp r5,r1
  0016be8c: blt 0x0016be26
  0016be8e: add.w r0,r1,r1, lsl #0x1
  0016be92: movs r1,#0x4
  0016be94: str.w r0,[r9,#0x20]
  0016be98: umull r6,r1,r0,r1
  0016be9c: cmp r1,#0x0
  0016be9e: it ne
  0016bea0: mov.ne r1,#0x1
  0016bea2: cmp r1,#0x0
  0016bea4: it ne
  0016bea6: mov.ne.w r6,#0xffffffff
  0016beaa: mov r0,r6
  0016beac: blx 0x0006ec08
  0016beb0: str.w r0,[r9,#0x18]
  0016beb4: mov r0,r6
  0016beb6: blx 0x0006ec08
  0016beba: str.w r0,[r9,#0x1c]
  0016bebe: add.w r1,r9,#0x14
  0016bec2: ldr.w r0,[r11,#0x0]
  0016bec6: blx 0x000720ac
  0016beca: ldrd r2,r1,[r9,#0x10]
  0016bece: ldr.w r0,[r11,#0x0]
  0016bed2: blx 0x00073030
  0016bed6: ldr r1,[sp,#0xc]
  0016bed8: mov r0,r9
  0016beda: blx 0x000777e8
  0016bede: mov r0,r9
  0016bee0: vpop {d6,d7,d8,d9}
  0016bee4: add sp,#0x4
  0016bee6: pop.w {r8,r9,r10,r11}
  0016beea: pop {r4,r5,r6,r7,pc}
```
