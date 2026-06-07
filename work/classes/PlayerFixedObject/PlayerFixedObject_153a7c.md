# PlayerFixedObject::PlayerFixedObject
elf 0x153a7c body 666
Sig: undefined __thiscall PlayerFixedObject(PlayerFixedObject * this, int param_1, int param_2, Player * param_3, AEGeometry * param_4, float param_5, float param_6, float param_7)

## decompile
```c

/* PlayerFixedObject::PlayerFixedObject(int, int, Player*, AEGeometry*, float, float, float) */

void __thiscall
PlayerFixedObject::PlayerFixedObject
          (PlayerFixedObject *this,int param_1,int param_2,Player *param_3,AEGeometry *param_4,
          float param_5,float param_6,float param_7)

{
  int *piVar1;
  int iVar2;
  Mission *this_00;
  void *pvVar3;
  Generator *this_01;
  Station *this_02;
  undefined4 uVar4;
  uint *puVar5;
  char *pcVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  float in_stack_00000004;
  float in_stack_00000008;
  float in_stack_0000000c;
  Vector local_60 [8];
  float local_58;
  int local_54;
  
  piVar10 = *(int **)(DAT_00163d54 + 0x163a9a);
  local_54 = *piVar10;
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,param_3,param_4,param_5,param_6,param_7,
                     SUB41(in_stack_00000004,0));
  uVar4 = 0;
  uVar11 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar1 = (int *)(DAT_00163d58 + 0x163ade);
  *(undefined4 *)(this + 0x158) = 0;
  *(undefined4 *)(this + 0x15c) = uVar11;
  *(undefined4 *)(this + 0x160) = uVar12;
  *(undefined4 *)(this + 0x164) = uVar13;
  iVar2 = *piVar1;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = uVar11;
  *(undefined4 *)(this + 0x150) = uVar12;
  *(undefined4 *)(this + 0x154) = uVar13;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = uVar11;
  *(undefined4 *)(this + 0x140) = uVar12;
  *(undefined4 *)(this + 0x144) = uVar13;
  *(int *)this = iVar2 + 8;
  AbyssEngine::String::String((String *)(this + 0x1ac));
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined2 *)(this + 0x184) = 0;
  *(int *)(this + 0x28) = param_2;
  *(undefined4 *)(this + 0x124) = 0;
  *(undefined4 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 300) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  *(undefined4 *)(this + 0x16c) = 0;
  this[0x170] = (PlayerFixedObject)0x0;
  this[0x1b8] = (PlayerFixedObject)0x0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x174) = uVar4;
  *(undefined4 *)(this + 0x178) = uVar11;
  *(undefined4 *)(this + 0x17c) = uVar12;
  *(undefined4 *)(this + 0x180) = uVar13;
  _local_58 = CONCAT44(local_54,in_stack_0000000c);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),local_60);
  pcVar6 = (char *)(DAT_00163d5c + 0x163b54);
  this[0x134] = (PlayerFixedObject)0x0;
  *(undefined4 *)(this + 0x194) = 0xffffffff;
  *(undefined4 *)(this + 0x1a0) = 0xffffffff;
  *(undefined4 *)(this + 0x1a4) = 0;
  *(int *)(this + 0x178) = (int)in_stack_00000004;
  *(int *)(this + 0x17c) = (int)in_stack_00000008;
  *(int *)(this + 0x180) = (int)in_stack_0000000c;
  AbyssEngine::String::String((String *)local_60,pcVar6,false);
  AbyssEngine::String::operator=((String *)(this + 0x1ac),local_60);
  AbyssEngine::String::~String((String *)local_60);
  this_00 = (Mission *)Status::getMission();
  iVar2 = Mission::isCampaignMission(this_00);
  if ((iVar2 == 0) ||
     ((iVar2 = Status::getCurrentCampaignMission(), iVar2 != 0x28 &&
      (iVar2 = Status::getCurrentCampaignMission(), iVar2 != 0x29)))) {
    this_01 = operator_new(1);
    Generator::Generator(this_01);
    if (param_1 == 0x37a3) {
      this[0x41] = (PlayerFixedObject)0x1;
      this_02 = (Station *)Status::getStation();
      iVar2 = Station::getIndex(this_02);
      iVar7 = DAT_00163d64 + 0x163bf6;
      iVar8 = DAT_00163d68 + 0x163bf8;
      for (uVar9 = 0; uVar9 < 4; uVar9 = uVar9 + 1) {
        if (*(int *)(iVar7 + uVar9 * 4) == iVar2) {
          uVar4 = Generator::getLootList((int)this_01,*(int *)(iVar8 + uVar9 * 8));
          *(undefined4 *)(this + 0x50) = uVar4;
        }
      }
    }
    else {
      puVar5 = (uint *)Generator::getLootList((int)this_01,-1);
      *(uint **)(this + 0x50) = puVar5;
      if (puVar5 != (uint *)0x0) {
        iVar2 = 0x498e;
        if (param_1 != 0x498e) {
          iVar2 = 0x4a88;
        }
        if (param_1 != 0x498e && param_1 != iVar2) {
          piVar1 = *(int **)(DAT_00163d6c + 0x163ca6);
          for (iVar2 = 1; iVar2 - 1U < *puVar5; iVar2 = iVar2 + 2) {
            iVar7 = *piVar1;
            if (param_1 == 0xe) {
              iVar7 = AbyssEngine::AERandom::nextInt(iVar7);
              puVar5 = *(uint **)(this + 0x50);
              *(int *)(puVar5[1] + iVar2 * 4) = *(int *)(puVar5[1] + iVar2 * 4) * (iVar7 + 5);
            }
            else {
              iVar7 = AbyssEngine::AERandom::nextInt(iVar7);
              *(int *)(*(int *)(*(int *)(this + 0x50) + 4) + iVar2 * 4) =
                   *(int *)(*(int *)(*(int *)(this + 0x50) + 4) + iVar2 * 4) * (iVar7 + 2);
              iVar8 = AbyssEngine::AERandom::nextInt(*piVar1);
              puVar5 = *(uint **)(this + 0x50);
              iVar7 = *(int *)(puVar5[1] + iVar2 * 4);
              if (iVar7 < iVar8 + 8) {
                iVar7 = iVar8 + 8;
              }
              *(int *)(puVar5[1] + iVar2 * 4) = iVar7;
            }
          }
        }
      }
    }
    pvVar3 = (void *)Generator::~Generator(this_01);
    operator_delete(pvVar3);
  }
  else {
    if (*(Array<int> **)(this + 0x50) != (Array<int> *)0x0) {
      pvVar3 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x50));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(this + 0x50) = 0;
  }
  *(undefined1 *)(*(int *)(this + 4) + 0x45) = 1;
  if ((param_1 != 0x37a3) && (*(undefined4 *)(this + 0xf8) = 0x2f, param_1 == 0xe)) {
    *(undefined4 *)(this + 0xf8) = 0xffffffff;
    this[0x134] = (PlayerFixedObject)0x0;
  }
  iVar2 = *piVar10 - local_54;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  00163a7c: push {r4,r5,r6,r7,lr}
  00163a7e: add r7,sp,#0xc
  00163a80: push {r7,r8,r9,r10,r11}
  00163a84: vpush {d12,d13}
  00163a88: vpush {d2,d3,d4,d5,d6,d7,d8,d9,d10}
  00163a8c: mov r8,r0
  00163a8e: ldr r0,[0x00163d54]
  00163a90: mov r9,r1
  00163a92: vldr.32 s16,[r7,#0x14]
  00163a96: add r0,pc
  00163a98: vldr.32 s20,[r7,#0x10]
  00163a9c: vldr.32 s18,[r7,#0xc]
  00163aa0: mov r6,r2
  00163aa2: ldr.w r11,[r0,#0x0]
  00163aa6: movs r5,#0x0
  00163aa8: ldr r1,[r7,#0x8]
  00163aaa: mov.w r2,#0xffffffff
  00163aae: ldr.w r0,[r11,#0x0]
  00163ab2: str r0,[sp,#0x2c]
  00163ab4: mov r0,r8
  00163ab6: str r1,[sp,#0x0]
  00163ab8: mov r1,r9
  00163aba: str r5,[sp,#0x10]
  00163abc: vstr.32 s18,[sp,#0x4]
  00163ac0: vstr.32 s20,[sp,#0x8]
  00163ac4: vstr.32 s16,[sp,#0xc]
  00163ac8: blx 0x00075790
  00163acc: ldr r0,[0x00163d58]
  00163ace: vmov.i32 q6,#0x0
  00163ad2: add.w r1,r8,#0x158
  00163ad6: add.w r4,r8,#0x1ac
  00163ada: add r0,pc
  00163adc: vst1.32 {d12,d13},[r1]
  00163ae0: add.w r1,r8,#0x148
  00163ae4: ldr r0,[r0,#0x0]
  00163ae6: vst1.32 {d12,d13},[r1]
  00163aea: add.w r1,r8,#0x138
  00163aee: adds r0,#0x8
  00163af0: vst1.32 {d12,d13},[r1]
  00163af4: str.w r0,[r8,#0x0]
  00163af8: mov r0,r4
  00163afa: blx 0x0006efbc
  00163afe: add.w r0,r8,#0x174
  00163b02: str.w r5,[r8,#0x18c]
  00163b06: strh.w r5,[r8,#0x184]
  00163b0a: str.w r6,[r8,#0x28]
  00163b0e: strd r5,r5,[r8,#0x124]
  00163b12: strd r5,r5,[r8,#0x12c]
  00163b16: strd r5,r5,[r8,#0x168]
  00163b1a: strb.w r5,[r8,#0x170]
  00163b1e: strb.w r5,[r8,#0x1b8]
  00163b22: str.w r5,[r8,#0x190]
  00163b26: vst1.32 {d12,d13},[r0]
  00163b2a: vstr.32 s20,[sp,#0x24]
  00163b2e: vstr.32 s18,[sp,#0x20]
  00163b32: vstr.32 s16,[sp,#0x28]
  00163b36: add.w r0,r8,#0x2c
  00163b3a: add r1,sp,#0x20
  00163b3c: blx 0x0006eb3c
  00163b40: vcvt.s32.f32 s0,s20
  00163b44: ldr r1,[0x00163d5c]
  00163b46: vcvt.s32.f32 s2,s18
  00163b4a: movs r0,#0x0
  00163b4c: vcvt.s32.f32 s4,s16
  00163b50: add r1,pc
  00163b52: mov.w r2,#0xffffffff
  00163b56: strb.w r0,[r8,#0x134]
  00163b5a: str.w r2,[r8,#0x194]
  00163b5e: strd r2,r0,[r8,#0x1a0]
  00163b62: vstr.32 s2,[r8,#0x178]
  00163b66: vstr.32 s0,[r8,#0x17c]
  00163b6a: vstr.32 s4,[r8,#0x180]
  00163b6e: add r0,sp,#0x20
  00163b70: movs r2,#0x0
  00163b72: blx 0x0006ee18
  00163b76: add r1,sp,#0x20
  00163b78: mov r0,r4
  00163b7a: blx 0x0006f2b0
  00163b7e: add r0,sp,#0x20
  00163b80: blx 0x0006ee30
  00163b84: ldr r0,[0x00163d60]
  00163b86: add r0,pc
  00163b88: ldr r5,[r0,#0x0]
  00163b8a: ldr r0,[r5,#0x0]
  00163b8c: blx 0x00072850
  00163b90: blx 0x0007372c
  00163b94: cbz r0,0x00163bc0
  00163b96: ldr r0,[r5,#0x0]
  00163b98: blx 0x00071770
  00163b9c: cmp r0,#0x28
  00163b9e: beq 0x00163baa
  00163ba0: ldr r0,[r5,#0x0]
  00163ba2: blx 0x00071770
  00163ba6: cmp r0,#0x29
  00163ba8: bne 0x00163bc0
  00163baa: ldr.w r0,[r8,#0x50]
  00163bae: cbz r0,0x00163bb8
  00163bb0: blx 0x00070204
  00163bb4: blx 0x0006eb48
  00163bb8: movs r0,#0x0
  00163bba: str.w r0,[r8,#0x50]
  00163bbe: b 0x00163c50
  00163bc0: movs r0,#0x1
  00163bc2: str r4,[sp,#0x18]
  00163bc4: blx 0x0006eb24
  00163bc8: mov r10,r0
  00163bca: blx 0x000736cc
  00163bce: movw r0,#0x37a3
  00163bd2: cmp r9,r0
  00163bd4: str.w r11,[sp,#0x1c]
  00163bd8: bne 0x00163c1c
  00163bda: movs r0,#0x1
  00163bdc: strb.w r0,[r8,#0x41]
  00163be0: ldr r0,[r5,#0x0]
  00163be2: blx 0x00071c14
  00163be6: blx 0x00071824
  00163bea: ldr r4,[0x00163d64]
  00163bec: mov r11,r0
  00163bee: ldr r5,[0x00163d68]
  00163bf0: movs r6,#0x0
  00163bf2: add r4,pc
  00163bf4: add r5,pc
  00163bf6: b 0x00163c16
  00163bf8: ldr.w r0,[r4,r6,lsl #0x2]
  00163bfc: cmp r0,r11
  00163bfe: bne 0x00163c14
  00163c00: add.w r0,r5,r6, lsl #0x3
  00163c04: ldr.w r1,[r5,r6,lsl #0x3]
  00163c08: ldr r2,[r0,#0x4]
  00163c0a: mov r0,r10
  00163c0c: blx 0x000740c8
  00163c10: str.w r0,[r8,#0x50]
  00163c14: adds r6,#0x1
  00163c16: cmp r6,#0x4
  00163c18: bcc 0x00163bf8
  00163c1a: b 0x00163c42
  00163c1c: mov r0,r10
  00163c1e: mov.w r1,#0xffffffff
  00163c22: mov.w r2,#0xffffffff
  00163c26: blx 0x000740c8
  00163c2a: cmp r0,#0x0
  00163c2c: str.w r0,[r8,#0x50]
  00163c30: beq 0x00163c42
  00163c32: movw r1,#0x498e
  00163c36: cmp r9,r1
  00163c38: itt ne
  00163c3a: movw.ne r1,#0x4a88
  00163c3e: cmp.ne r9,r1
  00163c40: bne 0x00163c9e
  00163c42: mov r0,r10
  00163c44: blx 0x000740d4
  00163c48: blx 0x0006eb48
  00163c4c: ldr.w r11,[sp,#0x1c]
  00163c50: ldr.w r0,[r8,#0x4]
  00163c54: movs r1,#0x1
  00163c56: strb.w r1,[r0,#0x45]
  00163c5a: movw r0,#0x37a3
  00163c5e: cmp r9,r0
  00163c60: beq 0x00163c7c
  00163c62: movs r0,#0x2f
  00163c64: cmp.w r9,#0xe
  00163c68: str.w r0,[r8,#0xf8]
  00163c6c: itttt eq
  00163c6e: mov.eq.w r0,#0xffffffff
  00163c72: str.w.eq r0,[r8,#0xf8]
  00163c76: mov.eq r0,#0x0
  00163c78: strb.eq.w r0,[r8,#0x134]
  00163c7c: ldr r0,[sp,#0x2c]
  00163c7e: ldr.w r1,[r11,#0x0]
  00163c82: subs r0,r1,r0
  00163c84: itttt eq
  00163c86: mov.eq r0,r8
  00163c88: vpop.eq {d2,d3,d4,d5,d6,d7,d8,d9,d10}
  00163c8c: vpop.eq {d12,d13}
  00163c90: add.eq sp,#0x4
  00163c92: itt eq
  00163c94: pop.eq.w {r8,r9,r10,r11}
  00163c98: pop.eq {r4,r5,r6,r7,pc}
  00163c9a: blx 0x0006e824
  00163c9e: ldr r1,[0x00163d6c]
  00163ca0: movs r5,#0x1
  00163ca2: add r1,pc
  00163ca4: ldr.w r11,[r1,#0x0]
  00163ca8: b 0x00163d0c
  00163caa: ldr.w r0,[r11,#0x0]
  00163cae: cmp.w r9,#0xe
  00163cb2: bne 0x00163cd0
  00163cb4: movs r1,#0x8
  00163cb6: blx 0x00071848
  00163cba: mov r1,r0
  00163cbc: ldr.w r0,[r8,#0x50]
  00163cc0: adds r1,#0x5
  00163cc2: ldr r2,[r0,#0x4]
  00163cc4: ldr.w r3,[r2,r5,lsl #0x2]
  00163cc8: muls r1,r3
  00163cca: str.w r1,[r2,r5,lsl #0x2]
  00163cce: b 0x00163d0a
  00163cd0: movs r1,#0x4
  00163cd2: blx 0x00071848
  00163cd6: ldr.w r1,[r8,#0x50]
  00163cda: adds r0,#0x2
  00163cdc: ldr r1,[r1,#0x4]
  00163cde: ldr.w r2,[r1,r5,lsl #0x2]
  00163ce2: muls r0,r2
  00163ce4: str.w r0,[r1,r5,lsl #0x2]
  00163ce8: ldr.w r0,[r11,#0x0]
  00163cec: movs r1,#0x5
  00163cee: blx 0x00071848
  00163cf2: mov r1,r0
  00163cf4: ldr.w r0,[r8,#0x50]
  00163cf8: adds r1,#0x8
  00163cfa: ldr r2,[r0,#0x4]
  00163cfc: ldr.w r3,[r2,r5,lsl #0x2]
  00163d00: cmp r1,r3
  00163d02: it gt
  00163d04: mov.gt r3,r1
  00163d06: str.w r3,[r2,r5,lsl #0x2]
  00163d0a: adds r5,#0x2
  00163d0c: ldr r0,[r0,#0x0]
  00163d0e: subs r1,r5,#0x1
  00163d10: cmp r1,r0
  00163d12: bcc 0x00163caa
  00163d14: b 0x00163c42
```
