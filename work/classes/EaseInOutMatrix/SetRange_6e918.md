# EaseInOutMatrix::SetRange
elf 0x6e918 body 394
Sig: undefined __stdcall SetRange(Matrix param_1, Matrix param_2)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::SetRange(AbyssEngine::AEMath::Matrix, AbyssEngine::AEMath::Matrix)
    */

void AbyssEngine::EaseInOutMatrix::SetRange
               (Matrix *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
               undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
               undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16,
               undefined4 param_17,undefined4 param_18,undefined4 param_19,undefined4 param_20,
               undefined4 param_21,undefined4 param_22,undefined4 param_23,undefined4 param_24,
               undefined4 param_25,undefined4 param_26,undefined4 param_27,undefined4 param_28,
               undefined4 param_29,undefined4 param_30,undefined4 param_31)

{
  undefined4 *puVar1;
  Quaternion *this;
  Quaternion *this_00;
  int *piVar2;
  Quaternion aQStack_108 [16];
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  Quaternion aQStack_e8 [16];
  undefined4 local_d8 [5];
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  
  piVar2 = *(int **)(DAT_0007ead0 + 0x7e92e);
  local_24 = *piVar2;
  local_54 = param_5;
  local_50 = param_6;
  local_4c = param_7;
  local_48 = param_8;
  local_44 = param_9;
  local_40 = param_10;
  local_3c = param_11;
  local_38 = param_12;
  local_34 = param_13;
  local_30 = param_14;
  local_2c = param_15;
  local_9c = param_17;
  local_98 = param_18;
  local_28 = param_16;
  local_94 = param_19;
  local_8c = param_21;
  local_90 = param_20;
  local_88 = param_22;
  local_84 = param_23;
  local_80 = param_24;
  uStack_7c = param_25;
  local_78 = param_26;
  uStack_74 = param_27;
  local_70 = param_28;
  uStack_6c = param_29;
  local_68 = param_30;
  uStack_64 = param_31;
  local_60 = param_2;
  uStack_5c = param_3;
  local_58 = param_4;
  AEMath::Matrix::operator=(param_1,(Matrix *)&local_60);
  AEMath::Matrix::operator=(param_1 + 0xb4,(Matrix *)&local_9c);
  this_00 = (Quaternion *)(param_1 + 0x3c);
  Quaternion::Set(this_00,(Matrix *)&local_60);
  uStack_a8 = DAT_0007eac8;
  uStack_b0 = DAT_0007eac0;
  uStack_bc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_b8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_b4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar1 = (undefined4 *)((uint)local_d8 | 4);
  *puVar1 = 0;
  puVar1[1] = uStack_bc;
  puVar1[2] = uStack_b8;
  puVar1[3] = uStack_b4;
  local_c0 = 0;
  local_d8[0] = 0x3f800000;
  local_c4 = 0x3f800000;
  local_a0 = 0x3f800000;
  Quaternion::Convert(this_00,(Matrix *)local_d8);
  this = (Quaternion *)Quaternion::Quaternion(aQStack_e8);
  Quaternion::Set(this,(Matrix *)local_d8);
  AEMath::MatrixGetPosition((Matrix *)&local_f8);
  AEMath::Vector::operator=((Vector *)(param_1 + 0x4c),(Vector *)&local_f8);
  Quaternion::Quaternion(aQStack_108,(Matrix *)&local_9c);
  operator-((AbyssEngine *)&local_f8,aQStack_108,this_00);
  *(undefined4 *)(param_1 + 0x58) = local_f8;
  *(undefined4 *)(param_1 + 0x5c) = uStack_f4;
  *(undefined4 *)(param_1 + 0x60) = uStack_f0;
  *(undefined4 *)(param_1 + 100) = uStack_ec;
  Quaternion::~Quaternion((Quaternion *)&local_f8);
  Quaternion::~Quaternion(aQStack_108);
  AEMath::MatrixGetPosition((Matrix *)aQStack_108);
  AEMath::operator-((Vector *)&local_f8,(Vector *)aQStack_108);
  AEMath::Vector::operator=((Vector *)(param_1 + 0x68),(Vector *)&local_f8);
  *(undefined4 *)(param_1 + 0x74) = 0x3f400000;
  UpdateCurrentValue((EaseInOutMatrix *)param_1);
  Quaternion::~Quaternion(aQStack_e8);
  if (*piVar2 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0007e918: push {r4,r5,r6,r7,lr}
  0007e91a: add r7,sp,#0xc
  0007e91c: push {r8,r9,r10}
  0007e920: sub sp,#0xe8
  0007e922: mov r4,r0
  0007e924: ldr r0,[0x0007ead0]
  0007e926: ldrd r6,r5,[r7,#0x8]
  0007e92a: add r0,pc
  0007e92c: ldrd r12,lr,[r7,#0x10]
  0007e930: ldrd r8,r10,[r7,#0x18]
  0007e934: ldr.w r9,[r0,#0x0]
  0007e938: strd r1,r2,[sp,#0xa8]
  0007e93c: ldr r1,[r7,#0x20]
  0007e93e: ldr.w r0,[r9,#0x0]
  0007e942: str r3,[sp,#0xb0]
  0007e944: str r6,[sp,#0xb4]
  0007e946: ldr r2,[r7,#0x24]
  0007e948: ldr r3,[r7,#0x28]
  0007e94a: ldr r6,[r7,#0x2c]
  0007e94c: str r5,[sp,#0xb8]
  0007e94e: str.w r12,[sp,#0xbc]
  0007e952: str.w lr,[sp,#0xc0]
  0007e956: str.w r8,[sp,#0xc4]
  0007e95a: str.w r10,[sp,#0xc8]
  0007e95e: str r1,[sp,#0xcc]
  0007e960: str r2,[sp,#0xd0]
  0007e962: str r3,[sp,#0xd4]
  0007e964: ldr.w lr,[r7,#0x38]
  0007e968: str r6,[sp,#0xd8]
  0007e96a: ldr r5,[r7,#0x30]
  0007e96c: ldr.w r12,[r7,#0x34]
  0007e970: ldr r1,[r7,#0x3c]
  0007e972: ldr r2,[r7,#0x40]
  0007e974: ldr r3,[r7,#0x48]
  0007e976: ldr r6,[r7,#0x44]
  0007e978: str r5,[sp,#0xdc]
  0007e97a: str.w lr,[sp,#0x6c]
  0007e97e: str r1,[sp,#0x70]
  0007e980: str.w r12,[sp,#0xe0]
  0007e984: str r2,[sp,#0x74]
  0007e986: str r0,[sp,#0xe4]
  0007e988: str r3,[sp,#0x7c]
  0007e98a: str r6,[sp,#0x78]
  0007e98c: ldr.w r8,[r7,#0x58]
  0007e990: ldr r0,[r7,#0x60]
  0007e992: ldr r6,[r7,#0x68]
  0007e994: ldr r5,[r7,#0x4c]
  0007e996: ldr.w r12,[r7,#0x50]
  0007e99a: ldr.w lr,[r7,#0x54]
  0007e99e: ldr r2,[r7,#0x5c]
  0007e9a0: ldr r3,[r7,#0x64]
  0007e9a2: ldr r1,[r7,#0x70]
  0007e9a4: str r5,[sp,#0x80]
  0007e9a6: str.w r12,[sp,#0x84]
  0007e9aa: strd lr,r8,[sp,#0x88]
  0007e9ae: strd r2,r0,[sp,#0x90]
  0007e9b2: mov r0,r4
  0007e9b4: strd r3,r6,[sp,#0x98]
  0007e9b8: add r6,sp,#0xa8
  0007e9ba: ldr r5,[r7,#0x6c]
  0007e9bc: strd r5,r1,[sp,#0xa0]
  0007e9c0: mov r1,r6
  0007e9c2: blx 0x0006f148
  0007e9c6: add.w r0,r4,#0xb4
  0007e9ca: add r1,sp,#0x6c
  0007e9cc: blx 0x0006f148
  0007e9d0: add.w r5,r4,#0x3c
  0007e9d4: mov r1,r6
  0007e9d6: mov r0,r5
  0007e9d8: blx 0x0006f154
  0007e9dc: adr r0,[0x7eac0]
  0007e9de: add r6,sp,#0x30
  0007e9e0: vmov.i32 q8,#0x0
  0007e9e4: vld1.64 {d18,d19},[r0]
  0007e9e8: orr r0,r6,#0x4
  0007e9ec: mov r1,r6
  0007e9ee: vst1.32 {d16,d17},[r0]
  0007e9f2: add.w r0,r6,#0x18
  0007e9f6: vst1.64 {d16,d17},[r0]
  0007e9fa: add.w r0,r6,#0x28
  0007e9fe: vst1.64 {d18,d19},[r0]
  0007ea02: mov.w r0,#0x3f800000
  0007ea06: str r0,[sp,#0x30]
  0007ea08: str r0,[sp,#0x44]
  0007ea0a: str r0,[sp,#0x68]
  0007ea0c: mov r0,r5
  0007ea0e: blx 0x0006f160
  0007ea12: add r0,sp,#0x20
  0007ea14: blx 0x0006f124
  0007ea18: mov r1,r6
  0007ea1a: blx 0x0006f154
  0007ea1e: add r0,sp,#0x10
  0007ea20: add r1,sp,#0xa8
  0007ea22: blx 0x0006f16c
  0007ea26: add.w r8,r4,#0x4c
  0007ea2a: add r1,sp,#0x10
  0007ea2c: mov r0,r8
  0007ea2e: blx 0x0006eb3c
  0007ea32: add r1,sp,#0x6c
  0007ea34: mov r0,sp
  0007ea36: blx 0x0006f178
  0007ea3a: add r6,sp,#0x10
  0007ea3c: mov r1,sp
  0007ea3e: mov r2,r5
  0007ea40: mov r0,r6
  0007ea42: blx 0x0006f184
  0007ea46: vld1.64 {d16,d17},[r6]
  0007ea4a: add.w r0,r4,#0x58
  0007ea4e: vst1.32 {d16,d17},[r0]
  0007ea52: mov r0,r6
  0007ea54: blx 0x0006f13c
  0007ea58: mov r0,sp
  0007ea5a: blx 0x0006f13c
  0007ea5e: add r1,sp,#0x6c
  0007ea60: mov r0,sp
  0007ea62: blx 0x0006f16c
  0007ea66: add r0,sp,#0x10
  0007ea68: mov r1,sp
  0007ea6a: mov r2,r8
  0007ea6c: blx 0x0006ec38
  0007ea70: add.w r0,r4,#0x68
  0007ea74: add r1,sp,#0x10
  0007ea76: blx 0x0006eb3c
  0007ea7a: mov.w r0,#0x3f400000
  0007ea7e: str r0,[r4,#0x74]
  0007ea80: mov r0,r4
  0007ea82: blx 0x0006f190
  0007ea86: add r0,sp,#0x20
  0007ea88: blx 0x0006f13c
  0007ea8c: ldr r0,[sp,#0xe4]
  0007ea8e: ldr.w r1,[r9,#0x0]
  0007ea92: subs r0,r1,r0
  0007ea94: ittt eq
  0007ea96: add.eq sp,#0xe8
  0007ea98: pop.eq.w {r8,r9,r10}
  0007ea9c: pop.eq {r4,r5,r6,r7,pc}
  0007ea9e: blx 0x0006e824
```
