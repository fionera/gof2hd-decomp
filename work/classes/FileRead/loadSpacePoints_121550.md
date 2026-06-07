# FileRead::loadSpacePoints
elf 0x121550 body 572
Sig: undefined __thiscall loadSpacePoints(FileRead * this, int param_1, int param_2)

## decompile
```c

/* FileRead::loadSpacePoints(int, int) */

void __thiscall FileRead::loadSpacePoints(FileRead *this,int param_1,int param_2)

{
  uint uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  String *pSVar7;
  int iVar8;
  Array<SpacePoint*> *this_00;
  undefined4 *puVar9;
  uint uVar10;
  SpacePoint *this_01;
  uint extraout_r1;
  uint *extraout_r2;
  uint uVar11;
  int *piVar12;
  code *pcVar13;
  float fVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  Matrix aMStack_114 [60];
  undefined4 local_d8 [5];
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined4 local_a0;
  ushort uStack_96;
  undefined4 local_94;
  undefined4 uStack_90;
  undefined4 local_8c;
  undefined4 uStack_88;
  undefined4 local_84;
  undefined4 uStack_80;
  float local_7c;
  float local_78;
  float local_74;
  undefined4 uStack_70;
  float local_6c;
  float local_68;
  ushort local_64;
  ushort uStack_62;
  uint local_60;
  int local_5c;
  
  piVar12 = *(int **)(DAT_001317c8 + 0x13156a);
  local_5c = *piVar12;
  pSVar7 = (String *)
           AbyssEngine::String::String((String *)local_d8,(char *)(DAT_001317cc + 0x131576),false);
  iVar8 = AEFile::FileExist(pSVar7);
  AbyssEngine::String::~String((String *)local_d8);
  if (iVar8 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_001317d0 + 0x13159a),(char *)&local_60,extraout_r2);
    uStack_62 = 0;
    local_64 = 0;
    local_6c = 0.0;
    local_68 = 0.0;
    local_7c = 0.0;
    local_78 = 0.0;
    local_74 = 0.0;
    uStack_70 = 0;
    local_84 = 0;
    uStack_80 = 0;
    local_94 = 0;
    uStack_90 = 0;
    local_8c = 0;
    uStack_88 = 0;
    while( true ) {
      AEFile::Read(&uStack_62,local_60);
      AEFile::Read(&local_64,local_60);
      if ((uint)uStack_62 == param_1) break;
      AEFile::Skip((uint)local_64 * 0x26,local_60);
    }
    this_00 = operator_new(0xc);
    Array<SpacePoint*>::Array(this_00);
    fVar5 = DAT_001317c4;
    fVar4 = DAT_001317c0;
    uVar3 = DAT_001317b8;
    uVar2 = DAT_001317b0;
    puVar9 = (undefined4 *)((uint)local_d8 | 4);
    uVar15 = 0;
    uVar16 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar17 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar18 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    pcVar13 = *(code **)(DAT_001317d4 + 0x131644);
    for (uVar11 = 0; uVar11 < local_64; uVar11 = uVar11 + 1) {
      AEFile::Read(&uStack_96,local_60);
      (*pcVar13)(&uStack_70,local_60);
      (*pcVar13)(&local_6c,local_60);
      (*pcVar13)(&local_68,local_60);
      (*pcVar13)(&local_7c,local_60);
      (*pcVar13)(&local_78,local_60);
      (*pcVar13)(&local_74,local_60);
      (*pcVar13)(&uStack_88,local_60);
      (*pcVar13)(&local_84,local_60);
      (*pcVar13)(&uStack_80,local_60);
      fVar6 = local_6c;
      local_7c = local_7c * fVar4;
      fVar14 = local_74 * fVar4;
      local_6c = local_68;
      local_68 = -fVar6;
      local_d8[0] = 0x3f800000;
      *puVar9 = uVar15;
      puVar9[1] = uVar16;
      puVar9[2] = uVar17;
      puVar9[3] = uVar18;
      local_74 = local_78 * fVar5;
      local_c4 = 0x3f800000;
      uStack_b0 = uVar2;
      uStack_a8 = uVar3;
      local_a0 = 0x3f800000;
      local_78 = fVar14;
      local_c0 = uVar15;
      uStack_bc = uVar16;
      uStack_b8 = uVar17;
      uStack_b4 = uVar18;
      AbyssEngine::AEMath::MatrixSetRotation
                (aMStack_114,(String *)local_d8,local_7c,fVar14,local_74,1);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)local_d8,aMStack_114);
      AbyssEngine::AEMath::MatrixGetDir(aMStack_114);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_94,(Vector *)aMStack_114);
      uVar10 = 0;
      uVar1 = extraout_r1;
      if (param_2 != -1) {
        uVar10 = uVar11 >> 1;
        uVar1 = param_2;
      }
      if (param_2 == -1 || uVar10 == uVar1) {
        this_01 = operator_new(0x24);
        SpacePoint::SpacePoint
                  (this_01,(uint)uStack_96,(Vector *)&uStack_70,(Vector *)&local_94,uVar11);
        ArrayAdd<SpacePoint*>(this_01,(Array *)this_00);
      }
    }
    AEFile::Close(local_60);
  }
  iVar8 = *piVar12 - local_5c;
  if (iVar8 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar8);
  }
  return;
}

```

## target disasm
```
  00131550: push {r4,r5,r6,r7,lr}
  00131552: add r7,sp,#0xc
  00131554: push {r7,r8,r9,r10,r11}
  00131558: vpush {d8,d9,d10,d11,d12,d13}
  0013155c: sub sp,#0xf8
  0013155e: str r2,[sp,#0x38]
  00131560: mov r8,r1
  00131562: ldr r0,[0x001317c8]
  00131564: movs r2,#0x0
  00131566: add r0,pc
  00131568: ldr r6,[r0,#0x0]
  0013156a: ldr r0,[r6,#0x0]
  0013156c: ldr r1,[0x001317cc]
  0013156e: str r0,[sp,#0xf4]
  00131570: add r0,sp,#0x78
  00131572: add r1,pc
  00131574: blx 0x0006ee18
  00131578: blx 0x0006fff4
  0013157c: add r5,sp,#0x78
  0013157e: mov r4,r0
  00131580: mov r0,r5
  00131582: blx 0x0006ee30
  00131586: movs r0,#0x0
  00131588: cmp r4,#0x0
  0013158a: str r0,[sp,#0x10]
  0013158c: beq.w 0x0013176e
  00131590: str r6,[sp,#0xc]
  00131592: add r1,sp,#0xf0
  00131594: ldr r0,[0x001317d0]
  00131596: add r0,pc
  00131598: blx 0x0006ecd4
  0013159c: movs r0,#0x0
  0013159e: sub.w r6,r7,#0x5a
  001315a2: strh r0,[r7,#-0x5a]
  001315a6: add r4,sp,#0xec
  001315a8: strh.w r0,[sp,#0xec]
  001315ac: mov.w r9,#0x26
  001315b0: strd r0,r0,[sp,#0xe4]
  001315b4: strd r0,r0,[sp,#0xd4]
  001315b8: strd r0,r0,[sp,#0xdc]
  001315bc: strd r0,r0,[sp,#0xcc]
  001315c0: strd r0,r0,[sp,#0xbc]
  001315c4: strd r0,r0,[sp,#0xc4]
  001315c8: add r0,sp,#0xc8
  001315ca: add.w r1,r0,#0x8
  001315ce: adds r0,#0x4
  001315d0: str r0,[sp,#0x30]
  001315d2: add r0,sp,#0xd4
  001315d4: str r1,[sp,#0x34]
  001315d6: add.w r1,r0,#0x8
  001315da: adds r0,#0x4
  001315dc: str r0,[sp,#0x28]
  001315de: add r0,sp,#0xe0
  001315e0: str r1,[sp,#0x2c]
  001315e2: add.w r1,r0,#0x8
  001315e6: adds r0,#0x4
  001315e8: str r1,[sp,#0x24]
  001315ea: str r0,[sp,#0x20]
  001315ec: b 0x001315fc
  001315ee: ldrh.w r0,[sp,#0xec]
  001315f2: ldr r1,[sp,#0xf0]
  001315f4: mul r0,r0,r9
  001315f8: blx 0x0006f1f0
  001315fc: ldr r1,[sp,#0xf0]
  001315fe: mov r0,r6
  00131600: blx 0x000763cc
  00131604: ldr r1,[sp,#0xf0]
  00131606: mov r0,r4
  00131608: blx 0x000763cc
  0013160c: ldrh.w r0,[r7,#-0x5a]
  00131610: cmp r0,r8
  00131612: bne 0x001315ee
  00131614: movs r0,#0xc
  00131616: blx 0x0006eb24
  0013161a: str r0,[sp,#0x10]
  0013161c: blx 0x000763d8
  00131620: add.w r0,r5,#0x28
  00131624: str r0,[sp,#0x14]
  00131626: add.w r0,r5,#0x18
  0013162a: str r0,[sp,#0x18]
  0013162c: orr r0,r5,#0x4
  00131630: str r0,[sp,#0x1c]
  00131632: ldr r0,[0x001317d4]
  00131634: vmov.i32 q6,#0x0
  00131638: sub.w r10,r7,#0x8e
  0013163c: add.w r8,sp,#0xd4
  00131640: add r0,pc
  00131642: add.w r9,sp,#0xc8
  00131646: vldr.32 s16,[pc,#0x178]
  0013164a: movs r4,#0x0
  0013164c: ldr r6,[r0,#0x0]
  0013164e: adr r0,[0x1317b0]
  00131650: vldr.32 s18,[pc,#0x170]
  00131654: vld1.64 {d10,d11},[r0]
  00131658: b 0x0013175a
  0013165a: mov r0,r10
  0013165c: blx 0x000763cc
  00131660: ldr r1,[sp,#0xf0]
  00131662: add r0,sp,#0xe0
  00131664: blx r6
  00131666: ldr r1,[sp,#0xf0]
  00131668: ldr r0,[sp,#0x20]
  0013166a: blx r6
  0013166c: ldr r1,[sp,#0xf0]
  0013166e: ldr r0,[sp,#0x24]
  00131670: blx r6
  00131672: ldr r1,[sp,#0xf0]
  00131674: mov r0,r8
  00131676: blx r6
  00131678: ldr r1,[sp,#0xf0]
  0013167a: ldr r0,[sp,#0x28]
  0013167c: blx r6
  0013167e: ldr r1,[sp,#0xf0]
  00131680: ldr r0,[sp,#0x2c]
  00131682: blx r6
  00131684: ldr r1,[sp,#0xf0]
  00131686: mov r0,r9
  00131688: blx r6
  0013168a: ldr r1,[sp,#0xf0]
  0013168c: ldr r0,[sp,#0x30]
  0013168e: blx r6
  00131690: ldr r1,[sp,#0xf0]
  00131692: ldr r0,[sp,#0x34]
  00131694: blx r6
  00131696: vldr.32 s2,[sp,#0xd4]
  0013169a: mov.w r1,#0x3f800000
  0013169e: vldr.32 s6,[sp,#0xdc]
  001316a2: add.w r11,sp,#0x3c
  001316a6: vmul.f32 s2,s2,s16
  001316aa: ldr r0,[sp,#0xe8]
  001316ac: vmul.f32 s6,s6,s16
  001316b0: vldr.32 s0,[sp,#0xe4]
  001316b4: str r0,[sp,#0xe4]
  001316b6: ldr r0,[sp,#0x1c]
  001316b8: vneg.f32 s0,s0
  001316bc: vldr.32 s4,[sp,#0xd8]
  001316c0: str r1,[sp,#0x78]
  001316c2: vst1.32 {d12,d13},[r0]
  001316c6: vmul.f32 s4,s4,s18
  001316ca: vmov r2,s2
  001316ce: vmov r3,s6
  001316d2: ldr r0,[sp,#0x18]
  001316d4: str r1,[sp,#0x8c]
  001316d6: vstr.32 s2,[sp,#0xd4]
  001316da: vst1.64 {d12,d13},[r0]
  001316de: ldr r0,[sp,#0x14]
  001316e0: vstr.32 s4,[sp,#0xdc]
  001316e4: vstr.32 s0,[sp,#0xe8]
  001316e8: vst1.32 {d10,d11},[r0]
  001316ec: movs r0,#0x1
  001316ee: str r1,[sp,#0xb0]
  001316f0: mov r1,r5
  001316f2: str r0,[sp,#0x4]
  001316f4: mov r0,r11
  001316f6: vstr.32 s6,[sp,#0xd8]
  001316fa: vstr.32 s4,[sp]
  001316fe: blx 0x00073060
  00131702: mov r0,r5
  00131704: mov r1,r11
  00131706: blx 0x0006f148
  0013170a: mov r0,r11
  0013170c: mov r1,r5
  0013170e: blx 0x0006f4c0
  00131712: add r0,sp,#0xbc
  00131714: mov r1,r11
  00131716: blx 0x0006eb3c
  0013171a: ldr r0,[sp,#0x38]
  0013171c: adds r0,#0x1
  0013171e: ittt ne
  00131720: lsr.ne r0,r4,#0x1
  00131722: ldr.ne r1,[sp,#0x38]
  00131724: cmp.ne r0,r1
  00131726: bne 0x00131758
  00131728: movs r0,#0x24
  0013172a: mov r9,r8
  0013172c: mov r8,r10
  0013172e: mov r10,r4
  00131730: mov r11,r5
  00131732: blx 0x0006eb24
  00131736: ldrh.w r1,[r7,#-0x8e]
  0013173a: mov r5,r0
  0013173c: add r2,sp,#0xe0
  0013173e: add r3,sp,#0xbc
  00131740: str r4,[sp,#0x0]
  00131742: blx 0x000763e4
  00131746: ldr r1,[sp,#0x10]
  00131748: mov r0,r5
  0013174a: blx 0x000763f0
  0013174e: mov r10,r8
  00131750: mov r8,r9
  00131752: add.w r9,sp,#0xc8
  00131756: mov r5,r11
  00131758: adds r4,#0x1
  0013175a: ldrh.w r0,[sp,#0xec]
  0013175e: ldr r1,[sp,#0xf0]
  00131760: cmp r4,r0
  00131762: bcc.w 0x0013165a
  00131766: mov r0,r1
  00131768: blx 0x0006ece0
  0013176c: ldr r6,[sp,#0xc]
  0013176e: ldr r0,[sp,#0xf4]
  00131770: ldr r1,[r6,#0x0]
  00131772: subs r0,r1,r0
  00131774: itttt eq
  00131776: ldr.eq r0,[sp,#0x10]
  00131778: add.eq sp,#0xf8
  0013177a: vpop.eq {d8,d9,d10,d11,d12,d13}
  0013177e: add.eq sp,#0x4
  00131780: itt eq
  00131782: pop.eq.w {r8,r9,r10,r11}
  00131786: pop.eq {r4,r5,r6,r7,pc}
  00131788: blx 0x0006e824
```
