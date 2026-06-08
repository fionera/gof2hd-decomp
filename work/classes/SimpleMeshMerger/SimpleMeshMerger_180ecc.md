# SimpleMeshMerger::SimpleMeshMerger
elf 0x180ecc body 704
Sig: undefined __thiscall SimpleMeshMerger(SimpleMeshMerger * this, Array * param_1, Array param_2, PaintCanvas * param_3, ushort param_4)

## decompile
```c

/* SimpleMeshMerger::SimpleMeshMerger(Array<unsigned short> const&,
   Array<AbyssEngine::AEMath::Matrix>, AbyssEngine::PaintCanvas*, unsigned short) */

void __thiscall
SimpleMeshMerger::SimpleMeshMerger
          (SimpleMeshMerger *this,uint *param_1,undefined4 *param_3,PaintCanvas *param_4,
          float param_5)

{
  byte bVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  short sVar5;
  int *piVar6;
  undefined4 uVar7;
  int iVar8;
  SimpleMeshMerger *pSVar9;
  uint uVar10;
  SimpleMeshMerger *pSVar11;
  int iVar12;
  uint uVar13;
  short sVar14;
  int iVar15;
  uint uVar16;
  byte *pbVar17;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float fVar18;
  float extraout_s1_05;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s3_01;
  float extraout_s3_02;
  float extraout_s3_03;
  float fVar19;
  float extraout_s3_04;
  uint local_34;
  float local_30;
  SimpleMeshMerger *local_2c;
  int local_28;
  
  piVar6 = *(int **)(DAT_001911ac + 0x190ee4);
  local_28 = *piVar6;
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 8));
  *(short *)(this + 4) = SUB42(param_5,0);
  *(PaintCanvas **)(this + 0x14) = param_4;
  *(undefined4 *)this = *param_3;
  ArraySetLength<AbyssEngine::Mesh*>(*param_1,(Array *)(this + 8));
  sVar5 = 0;
  sVar14 = 0;
  for (uVar16 = 0; uVar16 < *param_1; uVar16 = uVar16 + 1) {
    AbyssEngine::PaintCanvas::MeshCreate
              (param_4,*(ushort *)(param_1[1] + uVar16 * 2),&local_34,false);
    uVar7 = AbyssEngine::PaintCanvas::MeshGetPointer(param_4,local_34);
    *(undefined4 *)(*(int *)(this + 0xc) + uVar16 * 4) = uVar7;
    iVar8 = *(int *)(*(int *)(this + 0xc) + uVar16 * 4);
    sVar4 = *(short *)(iVar8 + 2);
    sVar2 = __aeabi_uidiv(*(undefined2 *)(iVar8 + 0x28),3);
    sVar14 = sVar14 + sVar4;
    sVar5 = sVar5 + sVar2;
  }
  pSVar9 = this + 0x18;
  pSVar11 = pSVar9;
  AbyssEngine::PaintCanvas::MeshCreate
            (param_4,sVar14,sVar5,(int)*(char *)**(undefined4 **)(this + 0xc),param_5,pSVar9);
  sVar5 = 0;
  sVar14 = 0;
  fVar18 = extraout_s1;
  fVar19 = extraout_s3;
  for (uVar16 = 0; uVar16 < *param_1; uVar16 = uVar16 + 1) {
    iVar8 = 0;
    iVar12 = 0;
    iVar15 = 0;
    uVar10 = 0;
    while( true ) {
      pbVar17 = *(byte **)(*(int *)(this + 0xc) + uVar16 * 4);
      if (*(ushort *)(pbVar17 + 2) <= uVar10) break;
      bVar1 = *pbVar17;
      if ((bVar1 & 1) != 0) {
        AbyssEngine::AEMath::MatrixTransformVector
                  ((Matrix *)&local_34,(Vector *)(param_3[1] + uVar16 * 0x3c));
        param_5 = local_30;
        pSVar11 = local_2c;
        AbyssEngine::PaintCanvas::MeshSetPoint
                  ((uint)param_4,(ushort)*(undefined4 *)pSVar9,local_30,extraout_s1_00,
                   (float)local_2c);
        pbVar17 = *(byte **)(*(int *)(this + 0xc) + uVar16 * 4);
        bVar1 = *pbVar17;
        fVar18 = extraout_s1_01;
        fVar19 = extraout_s3_00;
      }
      uVar13 = (uint)bVar1;
      if ((int)(uVar13 << 0x1d) < 0) {
        AbyssEngine::AEMath::MatrixRotateVector
                  ((Matrix *)&local_34,(Vector *)(param_3[1] + uVar16 * 0x3c));
        AbyssEngine::PaintCanvas::MeshSetNormal
                  (param_4,*(uint *)pSVar9,sVar5 + (short)uVar10,(Vector *)&local_34);
        pbVar17 = *(byte **)(*(int *)(this + 0xc) + uVar16 * 4);
        uVar13 = (uint)*pbVar17;
        fVar18 = extraout_s1_02;
        fVar19 = extraout_s3_01;
      }
      if ((int)(uVar13 << 0x1e) < 0) {
        param_5 = *(float *)(*(int *)(pbVar17 + 8) + iVar12 + 4);
        AbyssEngine::PaintCanvas::MeshSetUv
                  ((uint)param_4,(ushort)*(undefined4 *)pSVar9,param_5,fVar18);
        pbVar17 = *(byte **)(*(int *)(this + 0xc) + uVar16 * 4);
        uVar13 = (uint)*pbVar17;
        fVar18 = extraout_s1_03;
        fVar19 = extraout_s3_02;
      }
      if ((int)(uVar13 << 0x1c) < 0) {
        param_5 = *(float *)(*(int *)(pbVar17 + 0xc) + iVar15 + 4);
        pSVar11 = *(SimpleMeshMerger **)(*(int *)(pbVar17 + 0xc) + iVar15 + 8);
        AbyssEngine::PaintCanvas::MeshSetColor
                  (param_4,*(uint *)pSVar9,sVar5 + (short)uVar10,param_5,fVar18,(float)pSVar11,
                   fVar19);
        fVar18 = extraout_s1_04;
        fVar19 = extraout_s3_03;
      }
      iVar8 = iVar8 + 0xc;
      iVar12 = iVar12 + 8;
      iVar15 = iVar15 + 0x10;
      uVar10 = uVar10 + 1;
    }
    uVar10 = __aeabi_uidiv(*(undefined2 *)(pbVar17 + 0x28),3,uVar10,iVar8,param_5,pSVar11);
    iVar8 = 0;
    for (uVar13 = 0; uVar13 < uVar10; uVar13 = uVar13 + 1) {
      if ((int)((uint)*pbVar17 << 0x1b) < 0) {
        iVar12 = *(int *)(pbVar17 + 0x2c) + iVar8;
        pSVar11 = (SimpleMeshMerger *)(uint)(ushort)(*(short *)(iVar12 + 4) + sVar5);
        uVar3 = *(short *)(iVar12 + 2) + sVar5;
        param_5 = (float)(uint)uVar3;
        AbyssEngine::PaintCanvas::MeshSetTriangle
                  ((uint)param_4,(ushort)*(undefined4 *)pSVar9,sVar14 + (short)uVar13,
                   *(short *)(*(int *)(pbVar17 + 0x2c) + iVar8) + sVar5,uVar3);
        pbVar17 = *(byte **)(*(int *)(this + 0xc) + uVar16 * 4);
      }
      iVar8 = iVar8 + 6;
    }
    sVar4 = __aeabi_uidiv(*(undefined2 *)(pbVar17 + 0x28),3);
    sVar14 = sVar14 + sVar4;
    sVar5 = sVar5 + *(short *)(pbVar17 + 2);
    fVar18 = extraout_s1_05;
    fVar19 = extraout_s3_04;
  }
  AbyssEngine::PaintCanvas::TransformCreate(param_4,(uint *)(this + 0x1c));
  AbyssEngine::PaintCanvas::TransformAddMeshId(param_4,*(uint *)(this + 0x1c),*(uint *)pSVar9);
  this[6] = (SimpleMeshMerger)0x1;
  iVar8 = *piVar6 - local_28;
  if (iVar8 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar8);
  }
  return;
}

```

## target disasm
```
  00190ecc: push {r4,r5,r6,r7,lr}
  00190ece: add r7,sp,#0xc
  00190ed0: push {r8,r9,r10,r11}
  00190ed4: sub sp,#0x44
  00190ed6: mov r4,r0
  00190ed8: ldr r0,[0x001911ac]
  00190eda: add.w r6,r4,#0x8
  00190ede: mov r11,r3
  00190ee0: add r0,pc
  00190ee2: mov r5,r2
  00190ee4: mov r8,r1
  00190ee6: ldr r0,[r0,#0x0]
  00190ee8: str r0,[sp,#0x10]
  00190eea: ldr r0,[r0,#0x0]
  00190eec: str r0,[sp,#0x40]
  00190eee: mov r0,r6
  00190ef0: blx 0x0006f724
  00190ef4: ldr.w r10,[r7,#0x8]
  00190ef8: strh.w r10,[r4,#0x4]
  00190efc: str.w r11,[r4,#0x14]
  00190f00: ldr r0,[r5,#0x0]
  00190f02: str r0,[r4,#0x0]
  00190f04: ldr.w r0,[r8,#0x0]
  00190f08: str r5,[sp,#0x20]
  00190f0a: str.w r8,[sp,#0x30]
  00190f0e: mov r1,r6
  00190f10: blx 0x00078814
  00190f14: movs r5,#0x0
  00190f16: mov.w r8,#0x0
  00190f1a: movs r6,#0x0
  00190f1c: b 0x00190f5c
  00190f1e: ldr r0,[sp,#0x30]
  00190f20: ldr r0,[r0,#0x4]
  00190f22: ldrh.w r1,[r0,r8,lsl #0x1]
  00190f26: add r2,sp,#0x34
  00190f28: mov r0,r11
  00190f2a: movs r3,#0x0
  00190f2c: blx 0x0006fcf4
  00190f30: ldr r1,[sp,#0x34]
  00190f32: mov r0,r11
  00190f34: blx 0x00072370
  00190f38: ldr r1,[r4,#0xc]
  00190f3a: mov r9,r11
  00190f3c: str.w r0,[r1,r8,lsl #0x2]
  00190f40: ldr r0,[r4,#0xc]
  00190f42: ldr.w r1,[r0,r8,lsl #0x2]
  00190f46: ldrh r0,[r1,#0x28]
  00190f48: ldrh.w r11,[r1,#0x2]
  00190f4c: movs r1,#0x3
  00190f4e: blx 0x0006ec2c
  00190f52: add r6,r11
  00190f54: add r5,r0
  00190f56: mov r11,r9
  00190f58: add.w r8,r8,#0x1
  00190f5c: ldr r0,[sp,#0x30]
  00190f5e: ldr r0,[r0,#0x0]
  00190f60: cmp r8,r0
  00190f62: bcc 0x00190f1e
  00190f64: ldr r0,[r4,#0xc]
  00190f66: add.w r1,r4,#0x18
  00190f6a: ldr r0,[r0,#0x0]
  00190f6c: ldrsb.w r3,[r0,#0x0]
  00190f70: str r1,[sp,#0x24]
  00190f72: uxth r2,r5
  00190f74: strd r10,r1,[sp,#0x0]
  00190f78: uxth r1,r6
  00190f7a: mov r0,r11
  00190f7c: blx 0x00075da8
  00190f80: movs r0,#0x0
  00190f82: mov.w r10,#0x0
  00190f86: str r0,[sp,#0x28]
  00190f88: movs r0,#0x0
  00190f8a: str r0,[sp,#0x14]
  00190f8c: b 0x0019114c
  00190f8e: movs r3,#0x0
  00190f90: movs r5,#0x0
  00190f92: movs r6,#0x0
  00190f94: movs r2,#0x0
  00190f96: b 0x001910ba
  00190f98: ldrb.w r0,[r9,#0x0]
  00190f9c: str r2,[sp,#0x2c]
  00190f9e: tst r0,#0x1
  00190fa2: beq 0x00190ff0
  00190fa4: ldr r1,[sp,#0x20]
  00190fa6: rsb r2,r10,r10, lsl #0x4
  00190faa: ldr.w r0,[r9,#0x4]
  00190fae: mov r8,r3
  00190fb0: ldr r1,[r1,#0x4]
  00190fb2: add.w r1,r1,r2, lsl #0x2
  00190fb6: adds r2,r0,r3
  00190fb8: add r0,sp,#0x34
  00190fba: blx 0x0006f688
  00190fbe: ldr r0,[sp,#0x24]
  00190fc0: ldr r3,[sp,#0x34]
  00190fc2: vldr.32 s0,[sp,#0x38]
  00190fc6: ldr r1,[r0,#0x0]
  00190fc8: vldr.32 s2,[sp,#0x3c]
  00190fcc: vstr.32 s0,[sp]
  00190fd0: vstr.32 s2,[sp,#0x4]
  00190fd4: ldrd r0,r2,[sp,#0x28]
  00190fd8: add r0,r2
  00190fda: uxth r2,r0
  00190fdc: mov r0,r11
  00190fde: blx 0x000777f4
  00190fe2: ldr r0,[r4,#0xc]
  00190fe4: mov r3,r8
  00190fe6: ldr r2,[sp,#0x2c]
  00190fe8: ldr.w r9,[r0,r10,lsl #0x2]
  00190fec: ldrb.w r0,[r9,#0x0]
  00190ff0: lsls r1,r0,#0x1d
  00190ff2: bpl 0x00191044
  00190ff4: strd r6,r5,[sp,#0x18]
  00190ff8: rsb r2,r10,r10, lsl #0x4
  00190ffc: ldr r1,[sp,#0x20]
  00190ffe: mov r8,r4
  00191000: ldr.w r0,[r9,#0x10]
  00191004: mov r6,r10
  00191006: mov r4,r3
  00191008: ldr r1,[r1,#0x4]
  0019100a: add.w r1,r1,r2, lsl #0x2
  0019100e: adds r2,r0,r3
  00191010: add r5,sp,#0x34
  00191012: mov r0,r5
  00191014: blx 0x0006f694
  00191018: ldr r0,[sp,#0x24]
  0019101a: ldr r1,[r0,#0x0]
  0019101c: ldrd r0,r2,[sp,#0x28]
  00191020: add r0,r2
  00191022: uxth r2,r0
  00191024: mov r0,r11
  00191026: mov r3,r5
  00191028: blx 0x00077de8
  0019102c: ldr.w r0,[r8,#0xc]
  00191030: mov r10,r6
  00191032: mov r3,r4
  00191034: mov r4,r8
  00191036: ldr.w r9,[r0,r6,lsl #0x2]
  0019103a: ldrb.w r0,[r9,#0x0]
  0019103e: ldrd r6,r5,[sp,#0x18]
  00191042: ldr r2,[sp,#0x2c]
  00191044: lsls r1,r0,#0x1e
  00191046: bpl 0x00191078
  00191048: ldr r1,[sp,#0x24]
  0019104a: ldr.w r0,[r9,#0x8]
  0019104e: mov r9,r3
  00191050: ldr r1,[r1,#0x0]
  00191052: ldr r3,[r0,r5]
  00191054: add r0,r5
  00191056: vldr.32 s0,[r0,#0x4]
  0019105a: ldr r0,[sp,#0x28]
  0019105c: vstr.32 s0,[sp]
  00191060: add r0,r2
  00191062: uxth r2,r0
  00191064: mov r0,r11
  00191066: blx 0x000777dc
  0019106a: ldr r2,[sp,#0x2c]
  0019106c: ldr r0,[r4,#0xc]
  0019106e: mov r3,r9
  00191070: ldr.w r9,[r0,r10,lsl #0x2]
  00191074: ldrb.w r0,[r9,#0x0]
  00191078: lsls r0,r0,#0x1c
  0019107a: bpl 0x001910b2
  0019107c: ldr.w r0,[r9,#0xc]
  00191080: mov r9,r3
  00191082: ldr r1,[sp,#0x24]
  00191084: ldr r3,[r0,r6]
  00191086: add r0,r6
  00191088: ldr r1,[r1,#0x0]
  0019108a: vldr.32 s0,[r0,#0x4]
  0019108e: vldr.32 s2,[r0,#0x8]
  00191092: vldr.32 s4,[r0,#0xc]
  00191096: ldr r0,[sp,#0x28]
  00191098: vstr.32 s0,[sp]
  0019109c: add r0,r2
  0019109e: vstr.32 s2,[sp,#0x4]
  001910a2: vstr.32 s4,[sp,#0x8]
  001910a6: uxth r2,r0
  001910a8: mov r0,r11
  001910aa: blx 0x00077df4
  001910ae: ldr r2,[sp,#0x2c]
  001910b0: mov r3,r9
  001910b2: adds r3,#0xc
  001910b4: adds r5,#0x8
  001910b6: adds r6,#0x10
  001910b8: adds r2,#0x1
  001910ba: ldr r0,[r4,#0xc]
  001910bc: ldr.w r9,[r0,r10,lsl #0x2]
  001910c0: ldrh.w r0,[r9,#0x2]
  001910c4: cmp r2,r0
  001910c6: bcc.w 0x00190f98
  001910ca: ldrh.w r0,[r9,#0x28]
  001910ce: movs r1,#0x3
  001910d0: str.w r10,[sp,#0x2c]
  001910d4: blx 0x0006ec2c
  001910d8: mov r8,r0
  001910da: movs r6,#0x0
  001910dc: movs r5,#0x0
  001910de: b 0x00191126
  001910e0: ldrb.w r0,[r9,#0x0]
  001910e4: lsls r0,r0,#0x1b
  001910e6: bpl 0x00191122
  001910e8: ldr.w r0,[r9,#0x2c]
  001910ec: ldr r1,[sp,#0x24]
  001910ee: ldr r3,[sp,#0x28]
  001910f0: ldrh.w r12,[r0,r6]
  001910f4: add r0,r6
  001910f6: ldr r1,[r1,#0x0]
  001910f8: ldrh r2,[r0,#0x4]
  001910fa: ldrh r0,[r0,#0x2]
  001910fc: add r2,r3
  001910fe: uxth r2,r2
  00191100: add r0,r3
  00191102: uxth r0,r0
  00191104: strd r0,r2,[sp,#0x0]
  00191108: ldr r0,[sp,#0x14]
  0019110a: add r0,r5
  0019110c: uxth r2,r0
  0019110e: add.w r0,r12,r3
  00191112: uxth r3,r0
  00191114: mov r0,r11
  00191116: blx 0x000777d0
  0019111a: ldr r0,[r4,#0xc]
  0019111c: ldr r1,[sp,#0x2c]
  0019111e: ldr.w r9,[r0,r1,lsl #0x2]
  00191122: adds r6,#0x6
  00191124: adds r5,#0x1
  00191126: cmp r5,r8
  00191128: bcc 0x001910e0
  0019112a: ldrh.w r0,[r9,#0x28]
  0019112e: movs r1,#0x3
  00191130: blx 0x0006ec2c
  00191134: ldr r2,[sp,#0x14]
  00191136: ldr.w r10,[sp,#0x2c]
  0019113a: add r2,r0
  0019113c: ldrh.w r1,[r9,#0x2]
  00191140: ldr r0,[sp,#0x28]
  00191142: add.w r10,r10,#0x1
  00191146: str r2,[sp,#0x14]
  00191148: add r0,r1
  0019114a: str r0,[sp,#0x28]
  0019114c: ldr r0,[sp,#0x30]
  0019114e: ldr r0,[r0,#0x0]
  00191150: cmp r10,r0
  00191152: bcc.w 0x00190f8e
  00191156: add.w r8,r4,#0x1c
  0019115a: mov r0,r11
  0019115c: mov r1,r8
  0019115e: blx 0x000720ac
  00191162: ldr r0,[sp,#0x24]
  00191164: ldr.w r1,[r8,#0x0]
  00191168: ldr r2,[r0,#0x0]
  0019116a: mov r0,r11
  0019116c: blx 0x00073030
  00191170: movs r0,#0x1
  00191172: strb r0,[r4,#0x6]
  00191174: ldr r0,[sp,#0x40]
  00191176: ldr r1,[sp,#0x10]
  00191178: ldr r1,[r1,#0x0]
  0019117a: subs r0,r1,r0
  0019117c: itttt eq
  0019117e: mov.eq r0,r4
  00191180: add.eq sp,#0x44
  00191182: pop.eq.w {r8,r9,r10,r11}
  00191186: pop.eq {r4,r5,r6,r7,pc}
  00191188: blx 0x0006e824
```
