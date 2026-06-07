# TargetFollowCamera::TargetFollowCamera
elf 0x15a4c8 body 414
Sig: undefined __thiscall TargetFollowCamera(TargetFollowCamera * this, uint param_1, AEGeometry * param_2, Vector param_3, Vector param_4)

## decompile
```c

/* TargetFollowCamera::TargetFollowCamera(unsigned int, AEGeometry*, AbyssEngine::AEMath::Vector,
   AbyssEngine::AEMath::Vector) */

void __thiscall
TargetFollowCamera::TargetFollowCamera
          (TargetFollowCamera *this,int *param_1,void *param_2,undefined4 param_4,undefined4 param_5
          ,undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  float fVar1;
  undefined8 uVar2;
  double *pdVar3;
  undefined8 uVar4;
  int *piVar5;
  undefined4 uVar6;
  int iVar7;
  code *pcVar8;
  float *pfVar9;
  float extraout_s0;
  float fVar10;
  undefined4 local_8c;
  undefined4 uStack_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  int local_28;
  
  fVar10 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  fVar1 = *(float *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  iVar7 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  pfVar9 = &this->m_nPosX;
  piVar5 = *(int **)(DAT_0016a67c + 0x16a4e6);
  local_28 = *piVar5;
  local_30 = param_5;
  uStack_2c = param_6;
  local_40 = param_7;
  uStack_3c = param_8;
  local_38 = param_9;
  *pfVar9 = 0.0;
  this->m_nPosY = fVar10;
  this->m_nPosZ = fVar1;
  this->field_14 = iVar7;
  this->field_34 = 0;
  this->field_38 = (int)fVar10;
  this->field_3C = (int)fVar1;
  this->field_40 = iVar7;
  this->field_28 = 0;
  this->field_2C = (int)fVar10;
  this->field_30 = (int)fVar1;
  this->field_34 = iVar7;
  this->field_18 = 0;
  this->field_1C = (int)fVar10;
  this->field_20 = (int)fVar1;
  this->field_24 = iVar7;
  this->field_50 = 0;
  this->field_54 = 0;
  this->field_58 = 0;
  uStack_34 = param_4;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)&this->field_B4);
  this->field_F4 = 0;
  this->field_F8 = 0;
  this->field_FC = 0;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)&this[1].field_24);
  pcVar8 = *(code **)(DAT_0016a680 + 0x16a54e);
  this->field_0 = param_1;
  this->m_pTarget = param_2;
  (*pcVar8)(&this->field_2C,&uStack_34);
  (*pcVar8)(&this->field_38,&local_40);
  local_80 = 0;
  uStack_7c = 0;
  local_78 = 0;
  (*pcVar8)(pfVar9,&local_80);
  local_80 = 0;
  uStack_7c = 0;
  local_78 = 0;
  (*pcVar8)(&this->field_14,&local_80);
  uVar6 = AEGeometry::getMatrix();
  __aeabi_memcpy((Vector *)&local_80,uVar6,0x3c);
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_8c,(Vector *)&local_80);
  (*pcVar8)(&this->field_14,(Matrix *)&local_8c);
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_8c,(Vector *)&local_80);
  (*pcVar8)(pfVar9,(Matrix *)&local_8c);
  local_8c = 0;
  uStack_88 = 0x3f800000;
  local_84 = 0;
  (*pcVar8)(&this->field_20,&local_8c);
  this->field_44 = 0x10000;
  this->field_48 = 0;
  *(undefined1 *)&this->field_4C = 0;
  *(undefined1 *)&this->field_F0 = 0;
  *(undefined1 *)&this->field_100 = 0;
  *(undefined1 *)&this->field_10C = 1;
  this->m_nShakeAmount = 0.0;
  this->m_nShakeFrequency = 5;
  iVar7 = AbyssEngine::AEMath::VectorLength((Vector *)&local_40);
  pdVar3 = DAT_0016a678;
  uVar2 = DAT_0016a670;
  uVar6 = DAT_0016a668._4_4_;
  this[1].m_nPosZ = (float)(undefined4)DAT_0016a668;
  uVar4 = DAT_0016a670;
  this[1].field_14 = uVar6;
  DAT_0016a670._0_4_ = (undefined4)uVar2;
  DAT_0016a670._4_4_ = SUB84(uVar2,4);
  uVar6 = DAT_0016a670._4_4_;
  this[1].field_18 = (undefined4)DAT_0016a670;
  DAT_0016a670 = uVar4;
  this[1].field_1C = uVar6;
  this->field_B0 = iVar7;
  *(undefined1 *)&this[1].field_20 = 0;
  fVar10 = (float)aproximateCooefficientsForAproximationOfDampingFunktion
                            ((TargetFollowCamera *)&this->field_80,extraout_s0,pdVar3,
                             (double *)&this->field_60,(double *)&this->field_68,
                             (double *)&this->field_70,(double *)&this->field_78);
  aproximateCooefficientsForAproximationOfDampingFunktion
            ((TargetFollowCamera *)&this->field_A8,fVar10,(double *)this[1].field_14,
             (double *)&this->field_88,(double *)&this->field_90,(double *)&this->field_98,
             (double *)&this->field_A0);
  iVar7 = *piVar5 - local_28;
  if (iVar7 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar7);
  }
  return;
}

```

## target disasm
```
  0016a4c8: push {r4,r5,r6,r7,lr}
  0016a4ca: add r7,sp,#0xc
  0016a4cc: push {r8,r9,r10,r11}
  0016a4d0: sub sp,#0x7c
  0016a4d2: str r1,[sp,#0xc]
  0016a4d4: mov r4,r0
  0016a4d6: ldr r0,[0x0016a67c]
  0016a4d8: add.w lr,r7,#0x8
  0016a4dc: mov r10,r2
  0016a4de: vmov.i32 q8,#0x0
  0016a4e2: add r0,pc
  0016a4e4: add.w r11,r4,#0x8
  0016a4e8: mov.w r9,#0x0
  0016a4ec: ldr r0,[r0,#0x0]
  0016a4ee: str r0,[sp,#0x10]
  0016a4f0: ldrd r2,r6,[r7,#0x14]
  0016a4f4: ldm.w lr,{r5,r12,lr}
  0016a4f8: ldr r0,[r0,#0x0]
  0016a4fa: strd r5,r12,[sp,#0x70]
  0016a4fe: strd lr,r2,[sp,#0x60]
  0016a502: strd r6,r3,[sp,#0x68]
  0016a506: str r0,[sp,#0x78]
  0016a508: add.w r0,r4,#0x34
  0016a50c: vst1.32 {d16,d17},[r11]
  0016a510: vst1.32 {d16,d17},[r0]
  0016a514: add.w r0,r4,#0x28
  0016a518: vst1.32 {d16,d17},[r0]
  0016a51c: add.w r0,r4,#0x18
  0016a520: vst1.32 {d16,d17},[r0]
  0016a524: add.w r0,r4,#0xb4
  0016a528: strd r9,r9,[r4,#0x50]
  0016a52c: str.w r9,[r4,#0x58]
  0016a530: blx 0x0006f118
  0016a534: add.w r0,r4,#0x13c
  0016a538: strd r9,r9,[r4,#0xf4]
  0016a53c: str.w r9,[r4,#0xfc]
  0016a540: blx 0x0006f118
  0016a544: ldr r1,[0x0016a680]
  0016a546: add.w r0,r4,#0x2c
  0016a54a: add r1,pc
  0016a54c: ldr.w r8,[r1,#0x0]
  0016a550: ldr r1,[sp,#0xc]
  0016a552: strd r1,r10,[r4,#0x0]
  0016a556: add r1,sp,#0x6c
  0016a558: blx r8
  0016a55a: add.w r0,r4,#0x38
  0016a55e: add r1,sp,#0x60
  0016a560: blx r8
  0016a562: add r1,sp,#0x20
  0016a564: mov r0,r11
  0016a566: strd r9,r9,[sp,#0x20]
  0016a56a: str.w r9,[sp,#0x28]
  0016a56e: blx r8
  0016a570: add.w r6,r4,#0x14
  0016a574: add r1,sp,#0x20
  0016a576: strd r9,r9,[sp,#0x20]
  0016a57a: mov r0,r6
  0016a57c: str.w r9,[sp,#0x28]
  0016a580: blx r8
  0016a582: mov r0,r10
  0016a584: blx 0x000721cc
  0016a588: add.w r10,sp,#0x20
  0016a58c: mov r1,r0
  0016a58e: movs r2,#0x3c
  0016a590: mov r0,r10
  0016a592: blx 0x0006f1e4
  0016a596: add r5,sp,#0x14
  0016a598: add r2,sp,#0x6c
  0016a59a: mov r1,r10
  0016a59c: mov r0,r5
  0016a59e: blx 0x0006f688
  0016a5a2: mov r0,r6
  0016a5a4: mov r1,r5
  0016a5a6: blx r8
  0016a5a8: add r5,sp,#0x14
  0016a5aa: add r6,sp,#0x60
  0016a5ac: mov r1,r10
  0016a5ae: mov r0,r5
  0016a5b0: mov r2,r6
  0016a5b2: blx 0x0006f688
  0016a5b6: mov r0,r11
  0016a5b8: mov r1,r5
  0016a5ba: blx r8
  0016a5bc: mov.w r0,#0x3f800000
  0016a5c0: add r1,sp,#0x14
  0016a5c2: strd r9,r0,[sp,#0x14]
  0016a5c6: add.w r0,r4,#0x20
  0016a5ca: str.w r9,[sp,#0x1c]
  0016a5ce: blx r8
  0016a5d0: mov.w r0,#0x10000
  0016a5d4: strd r0,r9,[r4,#0x44]
  0016a5d8: movs r0,#0x1
  0016a5da: strb.w r9,[r4,#0x4c]
  0016a5de: strb.w r9,[r4,#0xf0]
  0016a5e2: strb.w r9,[r4,#0x100]
  0016a5e6: strb.w r0,[r4,#0x10c]
  0016a5ea: movs r0,#0x5
  0016a5ec: strd r9,r0,[r4,#0x110]
  0016a5f0: mov r0,r6
  0016a5f2: blx 0x0006ec44
  0016a5f6: adr r2,[0x16a668]
  0016a5f8: ldr r1,[0x0016a678]
  0016a5fa: vld1.64 {d16,d17},[r2]
  0016a5fe: add.w r2,r4,#0x128
  0016a602: add.w r3,r4,#0x70
  0016a606: vst1.64 {d16,d17},[r2]
  0016a60a: add.w r2,r4,#0x78
  0016a60e: str.w r0,[r4,#0xb0]
  0016a612: add.w r0,r4,#0x80
  0016a616: strb.w r9,[r4,#0x138]
  0016a61a: strd r3,r2,[sp,#0x0]
  0016a61e: add.w r2,r4,#0x60
  0016a622: add.w r3,r4,#0x68
  0016a626: str r0,[sp,#0x8]
  0016a628: blx 0x00077788
  0016a62c: add.w r2,r4,#0xa0
  0016a630: ldr.w r1,[r4,#0x12c]
  0016a634: add.w r3,r4,#0x98
  0016a638: add.w r0,r4,#0xa8
  0016a63c: strd r3,r2,[sp,#0x0]
  0016a640: add.w r2,r4,#0x88
  0016a644: add.w r3,r4,#0x90
  0016a648: str r0,[sp,#0x8]
  0016a64a: blx 0x00077788
  0016a64e: ldr r0,[sp,#0x78]
  0016a650: ldr r1,[sp,#0x10]
  0016a652: ldr r1,[r1,#0x0]
  0016a654: subs r0,r1,r0
  0016a656: itttt eq
  0016a658: mov.eq r0,r4
  0016a65a: add.eq sp,#0x7c
  0016a65c: pop.eq.w {r8,r9,r10,r11}
  0016a660: pop.eq {r4,r5,r6,r7,pc}
  0016a662: blx 0x0006e824
```
