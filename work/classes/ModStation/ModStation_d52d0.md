# ModStation::ModStation
elf 0xd52d0 body 898
Sig: undefined __thiscall ModStation(ModStation * this)

## decompile
```c

/* ModStation::ModStation() */

void __thiscall ModStation::ModStation(ModStation *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  Station *pSVar3;
  int iVar4;
  int iVar5;
  EaseInOutMatrix *pEVar6;
  Matrix *pMVar7;
  EaseInOut *pEVar8;
  int *piVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  uint uVar12;
  int iVar13;
  char *pcVar14;
  int iVar15;
  uint in_fpscr;
  float fVar16;
  float fVar17;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  int *piVar18;
  int *piVar19;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  EaseInOutMatrix *local_f8;
  Matrix aMStack_f4 [60];
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined4 local_80;
  undefined4 local_78;
  undefined4 local_74;
  int local_70;
  int local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  int *piStack_5c;
  int *local_58;
  int *local_54;
  undefined8 local_50;
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  
  piVar9 = *(int **)(DAT_000e5698 + 0xe52ea);
  local_3c = *piVar9;
  this->field_0 = *(int *)(DAT_000e569c + 0xe52ec) + 8;
  AbyssEngine::String::String((String *)&this->field_38);
  uVar2 = DAT_000e5690;
  uVar1 = DAT_000e5688;
  piStack_5c = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  local_58 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  local_54 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this[1].field_74 = 0;
  this[1].field_78 = 0;
  this[1].field_7C = 0;
  this[1].field_8C = (int *)0x0;
  this[2].field_0 = 0;
  this[2].field_4 = (int *)0x0;
  this->field_28 = 0;
  *(undefined1 *)((int)&this[1].field_20 + 3) = 0;
  *(undefined2 *)&this[1].field_20 = 0;
  *(undefined1 *)&this->field_24 = 0;
  *(undefined1 *)((int)&this->field_68 + 1) = 0;
  this->field_C = 100;
  this->field_10 = 0;
  this->field_14 = (int *)0x0;
  this->field_1C = (int *)0x0;
  this->field_20 = 0;
  this->field_50 = 0;
  this->field_54 = 0;
  this->field_58 = 0;
  *(undefined2 *)&this[1].field_4C = 0;
  *(undefined1 *)((int)&this->field_6C + 2) = 0;
  *(undefined1 *)&this->field_18 = 0;
  *(undefined1 *)&this[2].field_28 = 0;
  this[1].field_0 = 0;
  this[1].field_4 = piStack_5c;
  this[1].field_8 = local_58;
  this[1].field_C = (int)local_54;
  this->m_pDialogueWindow = (DialogueWindow *)0x0;
  this->field_84 = (int)piStack_5c;
  this->field_88 = local_58;
  this->field_8C = local_54;
  this->field_70 = 0;
  this->field_74 = (int)piStack_5c;
  this->field_78 = (int)local_58;
  this->field_7C = (int)local_54;
  puVar10 = (undefined4 *)((uint)&local_78 | 4);
  *puVar10 = 0;
  puVar10[1] = piStack_5c;
  puVar10[2] = local_58;
  puVar10[3] = local_54;
  local_60 = 0;
  local_50 = uVar1;
  local_48 = uVar2;
  local_78 = 0x3f800000;
  local_64 = 0x3f800000;
  local_40 = 0x3f800000;
  pSVar3 = (Station *)Status::getStation();
  iVar4 = Station::getIndex(pSVar3);
  if (iVar4 == 0x65) {
    iVar4 = 8;
    fVar17 = extraout_s1;
  }
  else {
    pSVar3 = (Station *)Status::getStation();
    iVar4 = Station::getIndex(pSVar3);
    if (iVar4 == 100) {
      iVar4 = 7;
      fVar17 = extraout_s1_00;
    }
    else {
      Status::getSystem();
      iVar4 = SolarSystem::getRace();
      fVar17 = extraout_s1_01;
    }
  }
  iVar15 = iVar4 * 3 + 1;
  iVar13 = DAT_000e56a8 + 0xe53da;
  pcVar14 = *(char **)(DAT_000e56a4 + 0xe53d6);
  local_fc = DAT_000e56ac + 0xe53e0;
  iVar5 = iVar13;
  if (*pcVar14 == '\0') {
    iVar5 = local_fc;
  }
  local_f8 = (EaseInOutMatrix *)(iVar4 * 3 + 2);
  fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + iVar4 * 0xc),
                                      (byte)(in_fpscr >> 0x16) & 3);
  piVar18 = (int *)VectorSignedToFloat(*(undefined4 *)(iVar5 + iVar15 * 4),
                                       (byte)(in_fpscr >> 0x16) & 3);
  piVar19 = (int *)VectorSignedToFloat(*(undefined4 *)(iVar5 + (int)local_f8 * 4),
                                       (byte)(in_fpscr >> 0x16) & 3);
  this[2].field_10 = (int)fVar16;
  this[2].field_14 = piVar18;
  this[2].field_18 = piVar19;
  AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_b8,fVar16,fVar17,(float)piVar18);
  local_104 = DAT_000e56b4 + 0xe5442;
  local_108 = DAT_000e56b8 + 0xe5448;
  uVar11 = *(undefined4 *)(DAT_000e56b0 + 0xe5440 + iVar4 * 4);
  iVar5 = local_104;
  if (*pcVar14 == '\0') {
    iVar5 = local_108;
  }
  local_100 = iVar4;
  AbyssEngine::AEMath::MatrixSetRotation
            (&local_b8,&local_78,uVar11,*(undefined4 *)(iVar5 + iVar4 * 4),0,2);
  uStack_9c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  local_98 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  if (*pcVar14 == '\0') {
    iVar13 = local_fc;
  }
  puVar10 = (undefined4 *)((uint)&local_b8 | 4);
  VectorSignedToFloat(*(undefined4 *)(iVar13 + iVar4 * 0xc),(byte)(in_fpscr >> 0x16) & 3);
  fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(iVar13 + iVar15 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  *puVar10 = 0;
  puVar10[1] = uStack_9c;
  puVar10[2] = local_98;
  puVar10[3] = uStack_94;
  local_a0 = 0;
  uStack_90 = uVar1;
  uStack_88 = uVar2;
  fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(iVar13 + (int)local_f8 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  local_b8 = 0x3f800000;
  local_a4 = 0x3f800000;
  local_80 = 0x3f800000;
  AbyssEngine::AEMath::MatrixSetTranslation(aMStack_f4,fVar17,extraout_s1_02,fVar16);
  piVar18 = &local_104;
  if (*pcVar14 == '\0') {
    piVar18 = &local_108;
  }
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_f4,&local_b8,uVar11,*(undefined4 *)(*piVar18 + local_100 * 4),0,2);
  pEVar6 = operator_new(0xf4);
  local_fc = local_78;
  local_100 = local_74;
  local_104 = local_70;
  local_108 = local_6c;
  local_f8 = pEVar6;
  AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
            (pEVar6,local_78,local_74,local_70,local_6c,local_68,local_64,local_60,piStack_5c,
             local_58,local_54,(undefined4)local_50,local_50._4_4_,(undefined4)local_48,
             local_48._4_4_,local_40,local_b8,local_b4,local_b0,local_ac,local_a8,local_a4,local_a0,
             uStack_9c,local_98,uStack_94,(undefined4)uStack_90,uStack_90._4_4_,
             (undefined4)uStack_88,uStack_88._4_4_,local_80,3000);
  iVar4 = DAT_000e56bc;
  this->field_20 = (int)pEVar6;
  uVar12 = **(uint **)(iVar4 + 0xe55de);
  pMVar7 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraSetLocal(uVar12,pMVar7);
  pEVar8 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar8,extraout_s0,extraout_s1_03);
  this[2].field_1C = (int *)pEVar8;
  pEVar8 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar8,extraout_s0_00,extraout_s1_04);
  this[2].field_20 = (int)pEVar8;
  pEVar8 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar8,extraout_s0_01,extraout_s1_05);
  this[2].field_24 = (int)pEVar8;
  iVar4 = *piVar9 - local_3c;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  000e52d0: push {r4,r5,r6,r7,lr}
  000e52d2: add r7,sp,#0xc
  000e52d4: push {r7,r8,r9,r10,r11}
  000e52d8: vpush {d8,d9}
  000e52dc: sub sp,#0x180
  000e52de: mov r5,r0
  000e52e0: ldr r0,[0x000e5698]
  000e52e2: ldr r1,[0x000e569c]
  000e52e4: mov r6,r5
  000e52e6: add r0,pc
  000e52e8: add r1,pc
  000e52ea: ldr r2,[r0,#0x0]
  000e52ec: ldr r0,[r1,#0x0]
  000e52ee: str r2,[sp,#0xac]
  000e52f0: ldr r1,[r2,#0x0]
  000e52f2: adds r0,#0x8
  000e52f4: str r1,[sp,#0x17c]
  000e52f6: str.w r0,[r6],#0x38
  000e52fa: mov r0,r6
  000e52fc: blx 0x0006efbc
  000e5300: adr r1,[0xe5688]
  000e5302: vmov.i32 q8,#0x0
  000e5306: vld1.64 {d8,d9},[r1]
  000e530a: movs r1,#0x0
  000e530c: movs r2,#0x64
  000e530e: ldr r0,[0x000e56a0]
  000e5310: strd r1,r1,[r5,#0x104]
  000e5314: str.w r1,[r5,#0x10c]
  000e5318: add r0,pc
  000e531a: strd r1,r1,[r5,#0x11c]
  000e531e: str.w r1,[r5,#0x124]
  000e5322: str r1,[r5,#0x28]
  000e5324: strb.w r1,[r5,#0xb3]
  000e5328: strh.w r1,[r5,#0xb0]
  000e532c: strb.w r1,[r5,#0x24]
  000e5330: strb.w r1,[r5,#0x69]
  000e5334: strd r2,r1,[r5,#0xc]
  000e5338: str r1,[r5,#0x14]
  000e533a: strd r1,r1,[r5,#0x1c]
  000e533e: strd r1,r1,[r5,#0x50]
  000e5342: str r1,[r5,#0x58]
  000e5344: strh.w r1,[r5,#0xdc]
  000e5348: strb.w r1,[r5,#0x6e]
  000e534c: strb r1,[r5,#0x18]
  000e534e: strb.w r1,[r5,#0x148]
  000e5352: add.w r1,r5,#0x90
  000e5356: ldr r4,[r0,#0x0]
  000e5358: vst1.64 {d16,d17},[r1]
  000e535c: add.w r1,r5,#0x80
  000e5360: vst1.64 {d16,d17},[r1]
  000e5364: add.w r1,r5,#0x70
  000e5368: vst1.64 {d16,d17},[r1]
  000e536c: add r1,sp,#0x140
  000e536e: orr r2,r1,#0x4
  000e5372: add.w r0,r1,#0x18
  000e5376: vst1.32 {d16,d17},[r2]
  000e537a: vst1.64 {d16,d17},[r0]
  000e537e: add.w r0,r1,#0x28
  000e5382: mov.w r1,#0x3f800000
  000e5386: vst1.64 {d8,d9},[r0]
  000e538a: ldr r0,[r4,#0x0]
  000e538c: str r1,[sp,#0x140]
  000e538e: str r1,[sp,#0x154]
  000e5390: str r1,[sp,#0x178]
  000e5392: str r6,[sp,#0x70]
  000e5394: blx 0x00071c14
  000e5398: blx 0x00071824
  000e539c: cmp r0,#0x65
  000e539e: bne 0x000e53a6
  000e53a0: mov.w r8,#0x8
  000e53a4: b 0x000e53c6
  000e53a6: ldr r0,[r4,#0x0]
  000e53a8: blx 0x00071c14
  000e53ac: blx 0x00071824
  000e53b0: cmp r0,#0x64
  000e53b2: bne 0x000e53ba
  000e53b4: mov.w r8,#0x7
  000e53b8: b 0x000e53c6
  000e53ba: ldr r0,[r4,#0x0]
  000e53bc: blx 0x00071a10
  000e53c0: blx 0x00071950
  000e53c4: mov r8,r0
  000e53c6: ldr r0,[0x000e56a4]
  000e53c8: add.w r9,r8,r8, lsl #0x1
  000e53cc: ldr r6,[0x000e56a8]
  000e53ce: add.w r11,r9,#0x1
  000e53d2: add r0,pc
  000e53d4: ldr r1,[0x000e56ac]
  000e53d6: add r6,pc
  000e53d8: ldr.w r10,[r0,#0x0]
  000e53dc: add r1,pc
  000e53de: str r1,[sp,#0xbc]
  000e53e0: ldrb.w r0,[r10,#0x0]
  000e53e4: cmp r0,#0x0
  000e53e6: mov r0,r6
  000e53e8: it eq
  000e53ea: mov.eq r0,r1
  000e53ec: add.w r1,r0,r9, lsl #0x2
  000e53f0: vldr.32 s0,[r1]
  000e53f4: add.w r1,r0,r11, lsl #0x2
  000e53f8: vldr.32 s2,[r1]
  000e53fc: add.w r1,r9,#0x2
  000e5400: vcvt.f32.s32 s0,s0
  000e5404: add.w r0,r0,r1, lsl #0x2
  000e5408: vcvt.f32.s32 s2,s2
  000e540c: str r1,[sp,#0xc0]
  000e540e: vldr.32 s4,[r0]
  000e5412: vcvt.f32.s32 s4,s4
  000e5416: vstr.32 s0,[r5,#0x130]
  000e541a: vmov r2,s0
  000e541e: vstr.32 s2,[r5,#0x134]
  000e5422: vmov r3,s2
  000e5426: vstr.32 s4,[r5,#0x138]
  000e542a: add r0,sp,#0x100
  000e542c: add r1,sp,#0x140
  000e542e: vstr.32 s4,[sp]
  000e5432: blx 0x0006f820
  000e5436: ldr r0,[0x000e56b0]
  000e5438: ldr r3,[0x000e56b4]
  000e543a: ldr r4,[0x000e56b8]
  000e543c: add r0,pc
  000e543e: add r3,pc
  000e5440: ldrb.w r1,[r10,#0x0]
  000e5444: add r4,pc
  000e5446: ldr.w r2,[r0,r8,lsl #0x2]
  000e544a: mov r0,r3
  000e544c: cmp r1,#0x0
  000e544e: str r3,[sp,#0xb4]
  000e5450: str r4,[sp,#0xb0]
  000e5452: it eq
  000e5454: mov.eq r0,r4
  000e5456: ldr.w r3,[r0,r8,lsl #0x2]
  000e545a: movs r0,#0x2
  000e545c: str.w r8,[sp,#0xb8]
  000e5460: movs r1,#0x0
  000e5462: add.w r8,sp,#0x100
  000e5466: strd r1,r0,[sp,#0x0]
  000e546a: add r1,sp,#0x140
  000e546c: mov r0,r8
  000e546e: mov r4,r2
  000e5470: blx 0x00073060
  000e5474: ldrb.w r0,[r10,#0x0]
  000e5478: vmov.i32 q8,#0x0
  000e547c: cmp r0,#0x0
  000e547e: ldr r0,[sp,#0xbc]
  000e5480: it eq
  000e5482: mov.eq r6,r0
  000e5484: add.w r0,r6,r9, lsl #0x2
  000e5488: vldr.32 s0,[r0]
  000e548c: add.w r0,r6,r11, lsl #0x2
  000e5490: vldr.32 s2,[r0]
  000e5494: orr r0,r8,#0x4
  000e5498: vcvt.f32.s32 s0,s0
  000e549c: vcvt.f32.s32 s2,s2
  000e54a0: vst1.32 {d16,d17},[r0]
  000e54a4: add.w r0,r8,#0x18
  000e54a8: vst1.64 {d16,d17},[r0]
  000e54ac: ldr r0,[sp,#0xc0]
  000e54ae: vmov r2,s0
  000e54b2: vmov r3,s2
  000e54b6: add.w r0,r6,r0, lsl #0x2
  000e54ba: vldr.32 s0,[r0]
  000e54be: add.w r0,r8,#0x28
  000e54c2: vst1.64 {d8,d9},[r0]
  000e54c6: mov.w r0,#0x3f800000
  000e54ca: vcvt.f32.s32 s0,s0
  000e54ce: str r0,[sp,#0x100]
  000e54d0: str r0,[sp,#0x114]
  000e54d2: str r0,[sp,#0x138]
  000e54d4: add r0,sp,#0xc4
  000e54d6: mov r1,r8
  000e54d8: vstr.32 s0,[sp]
  000e54dc: blx 0x0006f820
  000e54e0: ldrb.w r0,[r10,#0x0]
  000e54e4: cmp r0,#0x0
  000e54e6: ldrd r0,r1,[sp,#0xb0]
  000e54ea: it eq
  000e54ec: mov.eq r1,r0
  000e54ee: ldr r0,[sp,#0xb8]
  000e54f0: mov r2,r4
  000e54f2: ldr.w r3,[r1,r0,lsl #0x2]
  000e54f6: movs r0,#0x2
  000e54f8: movs r1,#0x0
  000e54fa: strd r1,r0,[sp,#0x0]
  000e54fe: add r0,sp,#0xc4
  000e5500: add r1,sp,#0x100
  000e5502: blx 0x00073060
  000e5506: movs r0,#0xf4
  000e5508: blx 0x0006eb24
  000e550c: str r0,[sp,#0xc0]
  000e550e: ldr r0,[sp,#0x140]
  000e5510: add.w r11,sp,#0x110
  000e5514: str r0,[sp,#0xbc]
  000e5516: add.w r12,sp,#0x120
  000e551a: ldr r0,[sp,#0x144]
  000e551c: str r0,[sp,#0xb8]
  000e551e: ldr r0,[sp,#0x148]
  000e5520: str r0,[sp,#0xb4]
  000e5522: ldr r0,[sp,#0x14c]
  000e5524: str r0,[sp,#0xb0]
  000e5526: ldr r0,[sp,#0x100]
  000e5528: str r0,[sp,#0x8c]
  000e552a: ldr r0,[sp,#0x104]
  000e552c: str r0,[sp,#0x88]
  000e552e: ldr r0,[sp,#0x108]
  000e5530: str r0,[sp,#0x84]
  000e5532: ldr r0,[sp,#0x10c]
  000e5534: str r0,[sp,#0x80]
  000e5536: ldr r0,[sp,#0x150]
  000e5538: str r0,[sp,#0xa8]
  000e553a: ldr r0,[sp,#0x154]
  000e553c: str r0,[sp,#0xa4]
  000e553e: ldr r0,[sp,#0x158]
  000e5540: str r0,[sp,#0xa0]
  000e5542: ldr r0,[sp,#0x15c]
  000e5544: str r0,[sp,#0x9c]
  000e5546: ldr r0,[sp,#0x160]
  000e5548: str r0,[sp,#0x98]
  000e554a: ldr r0,[sp,#0x164]
  000e554c: str r0,[sp,#0x94]
  000e554e: ldr r0,[sp,#0x168]
  000e5550: str r0,[sp,#0x90]
  000e5552: ldr r0,[sp,#0x16c]
  000e5554: str r0,[sp,#0x7c]
  000e5556: ldrd lr,r0,[sp,#0x170]
  000e555a: str r0,[sp,#0x78]
  000e555c: ldr r0,[sp,#0x178]
  000e555e: str r0,[sp,#0x74]
  000e5560: ldm.w r11,{r6,r9,r10,r11}
  000e5564: ldm.w r12,{r4,r8,r12}
  000e5568: ldrd r3,r0,[sp,#0x12c]
  000e556c: ldrd r1,r2,[sp,#0x134]
  000e5570: str r0,[sp,#0x60]
  000e5572: movw r0,#0xbb8
  000e5576: str r0,[sp,#0x6c]
  000e5578: add r0,sp,#0x40
  000e557a: str r1,[sp,#0x64]
  000e557c: str r2,[sp,#0x68]
  000e557e: stm r0,{r6,r9,r10,r11}
  000e5582: add r0,sp,#0x50
  000e5584: stm r0,{r4,r8,r12}
  000e5588: ldr r0,[sp,#0x7c]
  000e558a: str r3,[sp,#0x5c]
  000e558c: strd r0,lr,[sp,#0x20]
  000e5590: ldr r0,[sp,#0x78]
  000e5592: str r0,[sp,#0x28]
  000e5594: ldr r0,[sp,#0x74]
  000e5596: str r0,[sp,#0x2c]
  000e5598: ldr r0,[sp,#0x8c]
  000e559a: str r0,[sp,#0x30]
  000e559c: ldr r0,[sp,#0x88]
  000e559e: str r0,[sp,#0x34]
  000e55a0: ldr r0,[sp,#0x84]
  000e55a2: str r0,[sp,#0x38]
  000e55a4: ldr r0,[sp,#0x80]
  000e55a6: str r0,[sp,#0x3c]
  000e55a8: ldr r0,[sp,#0xb0]
  000e55aa: str r0,[sp,#0x0]
  000e55ac: ldr r0,[sp,#0xa8]
  000e55ae: str r0,[sp,#0x4]
  000e55b0: ldr r0,[sp,#0xa4]
  000e55b2: str r0,[sp,#0x8]
  000e55b4: ldr r0,[sp,#0xa0]
  000e55b6: str r0,[sp,#0xc]
  000e55b8: ldr r0,[sp,#0x9c]
  000e55ba: str r0,[sp,#0x10]
  000e55bc: ldr r0,[sp,#0x98]
  000e55be: str r0,[sp,#0x14]
  000e55c0: ldr r0,[sp,#0x94]
  000e55c2: str r0,[sp,#0x18]
  000e55c4: ldr r0,[sp,#0x90]
  000e55c6: str r0,[sp,#0x1c]
  000e55c8: ldr r4,[sp,#0xc0]
  000e55ca: ldr r1,[sp,#0xbc]
  000e55cc: ldrd r3,r2,[sp,#0xb4]
  000e55d0: mov r0,r4
  000e55d2: blx 0x00072b74
  000e55d6: ldr r0,[0x000e56bc]
  000e55d8: str r4,[r5,#0x20]
  000e55da: add r0,pc
  000e55dc: ldr r0,[r0,#0x0]
  000e55de: ldr r4,[r0,#0x0]
  000e55e0: mov r0,r4
  000e55e2: blx 0x000717f4
  000e55e6: mov r1,r0
  000e55e8: add r2,sp,#0x140
  000e55ea: mov r0,r4
  000e55ec: blx 0x000720a0
  000e55f0: movs r0,#0x10
  000e55f2: blx 0x0006eb24
  000e55f6: ldr.w r1,[r5,#0x130]
  000e55fa: mov r6,r0
  000e55fc: mov r2,r1
  000e55fe: blx 0x00074ecc
  000e5602: str.w r6,[r5,#0x13c]
  000e5606: movs r0,#0x10
  000e5608: blx 0x0006eb24
  000e560c: ldr.w r1,[r5,#0x134]
  000e5610: mov r6,r0
  000e5612: mov r2,r1
  000e5614: blx 0x00074ecc
  000e5618: str.w r6,[r5,#0x140]
  000e561c: movs r0,#0x10
  000e561e: blx 0x0006eb24
  000e5622: ldr.w r1,[r5,#0x138]
  000e5626: mov r6,r0
  000e5628: mov r2,r1
  000e562a: blx 0x00074ecc
  000e562e: str.w r6,[r5,#0x144]
  000e5632: ldr r0,[sp,#0x17c]
  000e5634: ldr r1,[sp,#0xac]
  000e5636: ldr r1,[r1,#0x0]
  000e5638: subs r0,r1,r0
  000e563a: itttt eq
  000e563c: mov.eq r0,r5
  000e563e: add.eq sp,#0x180
  000e5640: vpop.eq {d8,d9}
  000e5644: add.eq sp,#0x4
  000e5646: itt eq
  000e5648: pop.eq.w {r8,r9,r10,r11}
  000e564c: pop.eq {r4,r5,r6,r7,pc}
  000e564e: blx 0x0006e824
```
