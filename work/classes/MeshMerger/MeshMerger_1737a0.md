# MeshMerger::MeshMerger
elf 0x1737a0 body 696
Sig: undefined __thiscall MeshMerger(MeshMerger * this, Array * param_1, Array param_2, PaintCanvas * param_3, ushort param_4)

## decompile
```c

/* MeshMerger::MeshMerger(Array<unsigned short> const&, Array<AbyssEngine::AEMath::Matrix>,
   AbyssEngine::PaintCanvas*, unsigned short) */

void __thiscall
MeshMerger::MeshMerger
          (MeshMerger *this,uint *param_1,undefined4 *param_3,PaintCanvas *param_4,
          undefined4 param_5)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int *piVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  byte *pbVar12;
  int iVar13;
  short sVar14;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float fVar15;
  float extraout_s1_05;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s3_01;
  float extraout_s3_02;
  float extraout_s3_03;
  float fVar16;
  float extraout_s3_04;
  uint local_34;
  float local_30;
  float local_2c;
  int local_28;
  
  sVar4 = 0;
  piVar5 = *(int **)(DAT_00183a58 + 0x1837b8);
  local_28 = *piVar5;
  *(undefined4 *)(this + 0x1c) = 0;
  *(PaintCanvas **)(this + 0xc) = param_4;
  *(undefined4 *)(this + 0x30) = 1;
  *(short *)(this + 4) = (short)param_5;
  *(undefined4 *)this = *param_3;
  uVar11 = *param_1;
  uVar6 = (uint)((ulonglong)uVar11 * 4);
  if ((int)((ulonglong)uVar11 * 4 >> 0x20) != 0) {
    uVar6 = 0xffffffff;
  }
  puVar7 = operator_new__(uVar6);
  *(undefined4 **)(this + 8) = puVar7;
  sVar14 = 0;
  for (uVar6 = 0; uVar6 < uVar11; uVar6 = uVar6 + 1) {
    AbyssEngine::PaintCanvas::MeshCreate
              (param_4,*(ushort *)(param_1[1] + uVar6 * 2),&local_34,false);
    uVar8 = AbyssEngine::PaintCanvas::MeshGetPointer(param_4,local_34);
    *(undefined4 *)(*(int *)(this + 8) + uVar6 * 4) = uVar8;
    puVar7 = *(undefined4 **)(this + 8);
    sVar3 = *(short *)(puVar7[uVar6] + 2);
    sVar2 = __aeabi_uidiv(*(undefined2 *)(puVar7[uVar6] + 0x28),3);
    sVar14 = sVar14 + sVar3;
    sVar4 = sVar4 + sVar2;
    uVar11 = *param_1;
  }
  AbyssEngine::PaintCanvas::MeshCreate
            (param_4,sVar14,sVar4,(int)*(char *)*puVar7,param_5,this + 0x10);
  sVar14 = 0;
  sVar4 = 0;
  fVar15 = extraout_s1;
  fVar16 = extraout_s3;
  for (uVar6 = 0; uVar6 < *param_1; uVar6 = uVar6 + 1) {
    iVar9 = 0;
    iVar13 = 0;
    for (uVar11 = 0; pbVar12 = *(byte **)(*(int *)(this + 8) + uVar6 * 4),
        uVar11 < *(ushort *)(pbVar12 + 2); uVar11 = uVar11 + 1) {
      bVar1 = *pbVar12;
      if ((bVar1 & 1) != 0) {
        AbyssEngine::AEMath::MatrixTransformVector
                  ((Matrix *)&local_34,(Vector *)(param_3[1] + uVar6 * 0x3c));
        AbyssEngine::PaintCanvas::MeshSetPoint
                  ((uint)param_4,(ushort)*(undefined4 *)(this + 0x10),local_30,extraout_s1_00,
                   local_2c);
        pbVar12 = *(byte **)(*(int *)(this + 8) + uVar6 * 4);
        bVar1 = *pbVar12;
        fVar15 = extraout_s1_01;
        fVar16 = extraout_s3_00;
      }
      uVar10 = (uint)bVar1;
      if ((int)(uVar10 << 0x1d) < 0) {
        AbyssEngine::AEMath::MatrixRotateVector
                  ((Matrix *)&local_34,(Vector *)(param_3[1] + uVar6 * 0x3c));
        AbyssEngine::PaintCanvas::MeshSetNormal
                  (param_4,*(uint *)(this + 0x10),sVar4 + (short)uVar11,(Vector *)&local_34);
        pbVar12 = *(byte **)(*(int *)(this + 8) + uVar6 * 4);
        uVar10 = (uint)*pbVar12;
        fVar15 = extraout_s1_02;
        fVar16 = extraout_s3_01;
      }
      if ((int)(uVar10 << 0x1e) < 0) {
        AbyssEngine::PaintCanvas::MeshSetUv
                  ((uint)param_4,(ushort)*(undefined4 *)(this + 0x10),
                   *(float *)(*(int *)(pbVar12 + 8) + iVar9 + 4),fVar15);
        pbVar12 = *(byte **)(*(int *)(this + 8) + uVar6 * 4);
        uVar10 = (uint)*pbVar12;
        fVar15 = extraout_s1_03;
        fVar16 = extraout_s3_02;
      }
      if ((int)(uVar10 << 0x1c) < 0) {
        AbyssEngine::PaintCanvas::MeshSetColor
                  (param_4,*(uint *)(this + 0x10),sVar4 + (short)uVar11,
                   *(float *)(*(int *)(pbVar12 + 0xc) + iVar13 + 4),fVar15,
                   *(float *)(*(int *)(pbVar12 + 0xc) + iVar13 + 8),fVar16);
        fVar15 = extraout_s1_04;
        fVar16 = extraout_s3_03;
      }
      iVar9 = iVar9 + 8;
      iVar13 = iVar13 + 0x10;
    }
    uVar11 = __aeabi_uidiv(*(undefined2 *)(pbVar12 + 0x28),3);
    iVar9 = 0;
    for (uVar10 = 0; uVar10 < uVar11; uVar10 = uVar10 + 1) {
      if ((int)((uint)*pbVar12 << 0x1b) < 0) {
        AbyssEngine::PaintCanvas::MeshSetTriangle
                  ((uint)param_4,(ushort)*(undefined4 *)(this + 0x10),sVar14 + (short)uVar10,
                   *(short *)(*(int *)(pbVar12 + 0x2c) + iVar9) + sVar4,
                   *(short *)(*(int *)(pbVar12 + 0x2c) + iVar9 + 2) + sVar4);
        pbVar12 = *(byte **)(*(int *)(this + 8) + uVar6 * 4);
      }
      iVar9 = iVar9 + 6;
    }
    sVar3 = __aeabi_uidiv(*(undefined2 *)(pbVar12 + 0x28),3);
    sVar14 = sVar14 + sVar3;
    sVar4 = sVar4 + *(short *)(pbVar12 + 2);
    fVar15 = extraout_s1_05;
    fVar16 = extraout_s3_04;
  }
  AbyssEngine::PaintCanvas::TransformCreate(param_4,(uint *)(this + 0x14));
  AbyssEngine::PaintCanvas::TransformAddMeshId
            (param_4,*(uint *)(this + 0x14),*(uint *)(this + 0x10));
  *(undefined4 *)(this + 0x18) = 0;
  this[6] = (MeshMerger)0x1;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  this[0x34] = (MeshMerger)0x0;
  iVar9 = *piVar5 - local_28;
  if (iVar9 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar9);
}

```

## target disasm
```
  001837a0: push {r4,r5,r6,r7,lr}
  001837a2: add r7,sp,#0xc
  001837a4: push {r8,r9,r10,r11}
  001837a8: sub sp,#0x44
  001837aa: mov r4,r0
  001837ac: ldr r0,[0x00183a58]
  001837ae: ldr r6,[r7,#0x8]
  001837b0: mov.w r9,#0x0
  001837b4: add r0,pc
  001837b6: ldr r0,[r0,#0x0]
  001837b8: str r0,[sp,#0x10]
  001837ba: ldr r0,[r0,#0x0]
  001837bc: str r0,[sp,#0x40]
  001837be: movs r0,#0x1
  001837c0: str.w r9,[r4,#0x1c]
  001837c4: str r3,[r4,#0xc]
  001837c6: str r0,[r4,#0x30]
  001837c8: strh r6,[r4,#0x4]
  001837ca: ldr r0,[r2,#0x0]
  001837cc: str r0,[r4,#0x0]
  001837ce: movs r0,#0x4
  001837d0: ldr r5,[r1,#0x0]
  001837d2: str r1,[sp,#0x18]
  001837d4: str r3,[sp,#0x2c]
  001837d6: umull r0,r1,r5,r0
  001837da: str r2,[sp,#0x24]
  001837dc: cmp r1,#0x0
  001837de: it ne
  001837e0: mov.ne r1,#0x1
  001837e2: cmp r1,#0x0
  001837e4: it ne
  001837e6: mov.ne.w r0,#0xffffffff
  001837ea: blx 0x0006ec08
  001837ee: ldr.w r8,[sp,#0x18]
  001837f2: add.w r10,sp,#0x34
  001837f6: mov r6,r0
  001837f8: str r4,[sp,#0x20]
  001837fa: str r0,[r4,#0x8]
  001837fc: movs r4,#0x0
  001837fe: mov.w r11,#0x0
  00183802: b 0x00183842
  00183804: ldr.w r0,[r8,#0x4]
  00183808: mov r2,r10
  0018380a: ldr r5,[sp,#0x2c]
  0018380c: movs r3,#0x0
  0018380e: ldrh.w r1,[r0,r4,lsl #0x1]
  00183812: mov r0,r5
  00183814: blx 0x0006fcf4
  00183818: ldr r1,[sp,#0x34]
  0018381a: mov r0,r5
  0018381c: blx 0x00072370
  00183820: ldr r2,[sp,#0x20]
  00183822: ldr r1,[r2,#0x8]
  00183824: str.w r0,[r1,r4,lsl #0x2]
  00183828: ldr r6,[r2,#0x8]
  0018382a: ldr.w r1,[r6,r4,lsl #0x2]
  0018382e: ldrh r0,[r1,#0x28]
  00183830: ldrh r5,[r1,#0x2]
  00183832: movs r1,#0x3
  00183834: blx 0x0006ec2c
  00183838: add r11,r5
  0018383a: add r9,r0
  0018383c: ldr.w r5,[r8,#0x0]
  00183840: adds r4,#0x1
  00183842: cmp r4,r5
  00183844: bcc 0x00183804
  00183846: ldr r0,[r6,#0x0]
  00183848: uxth.w r2,r9
  0018384c: ldr r4,[sp,#0x20]
  0018384e: ldr r1,[r7,#0x8]
  00183850: ldrsb.w r3,[r0,#0x0]
  00183854: add.w r0,r4,#0x10
  00183858: str r0,[sp,#0x1c]
  0018385a: strd r1,r0,[sp,#0x0]
  0018385e: uxth.w r1,r11
  00183862: ldr r0,[sp,#0x2c]
  00183864: blx 0x00075da8
  00183868: movs r0,#0x0
  0018386a: mov.w r10,#0x0
  0018386e: movs r6,#0x0
  00183870: str r0,[sp,#0x30]
  00183872: b 0x00183a10
  00183874: str.w r10,[sp,#0x14]
  00183878: mov.w r11,#0x0
  0018387c: mov.w r9,#0x0
  00183880: mov.w r10,#0x0
  00183884: movs r5,#0x0
  00183886: str r6,[sp,#0x28]
  00183888: b 0x00183986
  0018388a: ldrb.w r0,[r8,#0x0]
  0018388e: tst r0,#0x1
  00183892: beq 0x001838d8
  00183894: ldr r1,[sp,#0x24]
  00183896: rsb r2,r6,r6, lsl #0x4
  0018389a: ldr.w r0,[r8,#0x4]
  0018389e: ldr r1,[r1,#0x4]
  001838a0: add.w r1,r1,r2, lsl #0x2
  001838a4: add.w r2,r0,r11
  001838a8: add r0,sp,#0x34
  001838aa: blx 0x0006f688
  001838ae: ldr r0,[sp,#0x30]
  001838b0: ldr r1,[r4,#0x10]
  001838b2: add r0,r5
  001838b4: ldr r3,[sp,#0x34]
  001838b6: vldr.32 s0,[sp,#0x38]
  001838ba: uxth r2,r0
  001838bc: ldr r0,[sp,#0x2c]
  001838be: vldr.32 s2,[sp,#0x3c]
  001838c2: vstr.32 s0,[sp]
  001838c6: vstr.32 s2,[sp,#0x4]
  001838ca: blx 0x000777f4
  001838ce: ldr r0,[r4,#0x8]
  001838d0: ldr.w r8,[r0,r6,lsl #0x2]
  001838d4: ldrb.w r0,[r8,#0x0]
  001838d8: lsls r1,r0,#0x1d
  001838da: bpl 0x00183914
  001838dc: ldr r1,[sp,#0x24]
  001838de: rsb r2,r6,r6, lsl #0x4
  001838e2: ldr.w r0,[r8,#0x10]
  001838e6: add r6,sp,#0x34
  001838e8: ldr r1,[r1,#0x4]
  001838ea: add.w r1,r1,r2, lsl #0x2
  001838ee: add.w r2,r0,r11
  001838f2: mov r0,r6
  001838f4: blx 0x0006f694
  001838f8: ldr r0,[sp,#0x30]
  001838fa: mov r3,r6
  001838fc: ldr r1,[r4,#0x10]
  001838fe: add r0,r5
  00183900: ldr r6,[sp,#0x28]
  00183902: uxth r2,r0
  00183904: ldr r0,[sp,#0x2c]
  00183906: blx 0x00077de8
  0018390a: ldr r0,[r4,#0x8]
  0018390c: ldr.w r8,[r0,r6,lsl #0x2]
  00183910: ldrb.w r0,[r8,#0x0]
  00183914: lsls r1,r0,#0x1e
  00183916: bpl 0x00183942
  00183918: ldr.w r0,[r8,#0x8]
  0018391c: ldr r1,[r4,#0x10]
  0018391e: ldr.w r3,[r0,r9]
  00183922: add r0,r9
  00183924: vldr.32 s0,[r0,#0x4]
  00183928: ldr r0,[sp,#0x30]
  0018392a: vstr.32 s0,[sp]
  0018392e: add r0,r5
  00183930: uxth r2,r0
  00183932: ldr r0,[sp,#0x2c]
  00183934: blx 0x000777dc
  00183938: ldr r0,[r4,#0x8]
  0018393a: ldr.w r8,[r0,r6,lsl #0x2]
  0018393e: ldrb.w r0,[r8,#0x0]
  00183942: lsls r0,r0,#0x1c
  00183944: bpl 0x00183978
  00183946: ldr.w r0,[r8,#0xc]
  0018394a: ldr r1,[sp,#0x1c]
  0018394c: ldr.w r3,[r0,r10]
  00183950: add r0,r10
  00183952: ldr r1,[r1,#0x0]
  00183954: vldr.32 s0,[r0,#0x4]
  00183958: vldr.32 s2,[r0,#0x8]
  0018395c: vldr.32 s4,[r0,#0xc]
  00183960: ldr r0,[sp,#0x30]
  00183962: vstr.32 s0,[sp]
  00183966: add r0,r5
  00183968: vstr.32 s2,[sp,#0x4]
  0018396c: vstr.32 s4,[sp,#0x8]
  00183970: uxth r2,r0
  00183972: ldr r0,[sp,#0x2c]
  00183974: blx 0x00077df4
  00183978: add.w r11,r11,#0xc
  0018397c: add.w r9,r9,#0x8
  00183980: add.w r10,r10,#0x10
  00183984: adds r5,#0x1
  00183986: ldr r0,[r4,#0x8]
  00183988: ldr.w r8,[r0,r6,lsl #0x2]
  0018398c: ldrh.w r0,[r8,#0x2]
  00183990: cmp r5,r0
  00183992: bcc.w 0x0018388a
  00183996: ldrh.w r0,[r8,#0x28]
  0018399a: movs r1,#0x3
  0018399c: blx 0x0006ec2c
  001839a0: ldr.w r10,[sp,#0x14]
  001839a4: mov r9,r0
  001839a6: movs r5,#0x0
  001839a8: movs r4,#0x0
  001839aa: b 0x001839f2
  001839ac: ldrb.w r0,[r8,#0x0]
  001839b0: lsls r0,r0,#0x1b
  001839b2: bpl 0x001839ee
  001839b4: ldr.w r0,[r8,#0x2c]
  001839b8: ldr r3,[sp,#0x30]
  001839ba: ldr r6,[sp,#0x20]
  001839bc: ldrh.w r12,[r0,r5]
  001839c0: add r0,r5
  001839c2: ldrh r2,[r0,#0x4]
  001839c4: ldrh r0,[r0,#0x2]
  001839c6: add r2,r3
  001839c8: ldr r1,[r6,#0x10]
  001839ca: add r0,r3
  001839cc: uxth r2,r2
  001839ce: uxth r0,r0
  001839d0: strd r0,r2,[sp,#0x0]
  001839d4: add.w r0,r10,r4
  001839d8: uxth r2,r0
  001839da: add.w r0,r12,r3
  001839de: uxth r3,r0
  001839e0: ldr r0,[sp,#0x2c]
  001839e2: blx 0x000777d0
  001839e6: ldr r0,[r6,#0x8]
  001839e8: ldr r6,[sp,#0x28]
  001839ea: ldr.w r8,[r0,r6,lsl #0x2]
  001839ee: adds r5,#0x6
  001839f0: adds r4,#0x1
  001839f2: cmp r4,r9
  001839f4: bcc 0x001839ac
  001839f6: ldrh.w r0,[r8,#0x28]
  001839fa: movs r1,#0x3
  001839fc: blx 0x0006ec2c
  00183a00: add r10,r0
  00183a02: ldrh.w r1,[r8,#0x2]
  00183a06: ldr r0,[sp,#0x30]
  00183a08: adds r6,#0x1
  00183a0a: ldr r4,[sp,#0x20]
  00183a0c: add r0,r1
  00183a0e: str r0,[sp,#0x30]
  00183a10: ldr r0,[sp,#0x18]
  00183a12: ldr r0,[r0,#0x0]
  00183a14: cmp r6,r0
  00183a16: bcc.w 0x00183874
  00183a1a: ldr r5,[sp,#0x2c]
  00183a1c: add.w r1,r4,#0x14
  00183a20: mov r0,r5
  00183a22: blx 0x000720ac
  00183a26: ldrd r2,r1,[r4,#0x10]
  00183a2a: mov r0,r5
  00183a2c: blx 0x00073030
  00183a30: movs r0,#0x0
  00183a32: movs r1,#0x1
  00183a34: str r0,[r4,#0x18]
  00183a36: strb r1,[r4,#0x6]
  00183a38: strd r0,r0,[r4,#0x20]
  00183a3c: strb.w r0,[r4,#0x34]
  00183a40: ldr r0,[sp,#0x40]
  00183a42: ldr r1,[sp,#0x10]
  00183a44: ldr r1,[r1,#0x0]
  00183a46: subs r0,r1,r0
  00183a48: itttt eq
  00183a4a: mov.eq r0,r4
  00183a4c: add.eq sp,#0x44
  00183a4e: pop.eq.w {r8,r9,r10,r11}
  00183a52: pop.eq {r4,r5,r6,r7,pc}
  00183a54: blx 0x0006e824
```
