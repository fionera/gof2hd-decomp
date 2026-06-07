# Hud::drawChallengeModeScore
elf 0x1637e0 body 834
Sig: undefined __stdcall drawChallengeModeScore(int param_1)

## decompile
```c

/* Hud::drawChallengeModeScore(int) */

void Hud::drawChallengeModeScore(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int extraout_r1;
  uint *puVar6;
  int *piVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  Sprite *this;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  uint in_fpscr;
  float fVar16;
  undefined8 uVar17;
  float fVar18;
  String aSStack_58 [12];
  String aSStack_4c [8];
  uint local_44;
  String aSStack_40 [8];
  uint local_38;
  int local_34;
  
  piVar1 = *(int **)(DAT_00173b78 + 0x1737f8);
  puVar6 = *(uint **)(DAT_00173b7c + 0x1737fa);
  local_34 = *piVar1;
  AbyssEngine::PaintCanvas::SetColor(*puVar6);
  iVar2 = Sprite::getFrameWidth(*(Sprite **)(param_1 + 0x534));
  piVar10 = *(int **)(DAT_00173b80 + 0x17381e);
  iVar5 = *(int *)(*piVar10 + 0x2c);
  iVar3 = Sprite::getFrameHeight(*(Sprite **)(param_1 + 0x534));
  piVar7 = *(int **)(DAT_00173b84 + 0x173836);
  piVar9 = *(int **)(DAT_00173b88 + 0x17383a);
  iVar13 = *(int *)(*piVar10 + 0x2c);
  iVar8 = *piVar9;
  AbyssEngine::String::String(aSStack_40,*(int *)(*piVar7 + 0x184));
  if (local_38 < 7) {
    iVar12 = 7 - local_38;
    for (iVar14 = 0; iVar14 < iVar12; iVar14 = iVar14 + 1) {
      AbyssEngine::String::String(aSStack_58,(char *)(DAT_00173b8c + 0x173872),false);
      AbyssEngine::operator+(aSStack_4c,aSStack_58);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      AbyssEngine::String::~String(aSStack_4c);
      AbyssEngine::String::~String(aSStack_58);
    }
  }
  AbyssEngine::PaintCanvas::SetColor(*puVar6);
  iVar2 = iVar2 - iVar5;
  iVar8 = iVar8 / 2;
  iVar5 = (iVar2 * 7) / 2;
  iVar14 = iVar8 - iVar5;
  iVar12 = iVar14;
  for (iVar11 = 1; iVar11 - 1U < local_38; iVar11 = iVar11 + 1) {
    AbyssEngine::String::SubString((uint)aSStack_4c,(uint)aSStack_40);
    iVar15 = AbyssEngine::String::ValueOf();
    AbyssEngine::String::~String(aSStack_4c);
    Sprite::setFrame(*(Sprite **)(param_1 + 0x534),iVar15);
    uVar17 = Sprite::setPosition(*(Sprite **)(param_1 + 0x534),iVar12,iVar13);
    Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
    iVar12 = iVar12 + iVar2;
  }
  if ((0 < *(int *)(*piVar7 + 0x180)) && (1 < *(int *)(*piVar7 + 0x18c))) {
    AbyssEngine::PaintCanvas::SetColor(*puVar6);
    iVar11 = *piVar7;
    iVar15 = *(int *)(*piVar10 + 0x2c);
    iVar12 = *(int *)(iVar11 + 0x180);
    iVar13 = iVar13 + iVar3 + iVar15;
    if (iVar12 < 0xbb9) {
      __aeabi_idivmod(iVar12,100);
      if (extraout_r1 < 0x32) goto LAB_00173afe;
      iVar12 = *(int *)(iVar11 + 0x18c);
      fVar16 = (float)VectorSignedToFloat(iVar12,(byte)(in_fpscr >> 0x16) & 3);
      fVar18 = (float)VectorSignedToFloat(iVar12 * 1000,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::String::String(aSStack_4c,(int)((fVar16 * DAT_00173b70 + 1.0) * fVar18));
      iVar11 = 0;
      for (iVar12 = 1; iVar12 - 1U < local_44; iVar12 = iVar12 + 1) {
        AbyssEngine::String::SubString((uint)aSStack_58,(uint)aSStack_4c);
        iVar4 = AbyssEngine::String::ValueOf();
        AbyssEngine::String::~String(aSStack_58);
        Sprite::setFrame(*(Sprite **)(param_1 + 0x534),iVar4);
        uVar17 = Sprite::setPosition(*(Sprite **)(param_1 + 0x534),
                                     (*piVar9 / 2 - (local_44 * iVar2 >> 1)) + iVar11,
                                     iVar3 + iVar13 + *(int *)(*piVar10 + 0x2c));
        iVar11 = iVar11 + iVar2;
        Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
      }
      AbyssEngine::String::~String(aSStack_4c);
    }
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar6,*(int *)(param_1 + 0x538),iVar15 + iVar14);
    VectorSignedToFloat(*(int *)(*piVar7 + 0x180) + -7000,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::String::String(aSStack_4c,*(int *)(*piVar7 + 0x18c));
    iVar5 = (iVar8 + iVar15) - iVar5;
    for (iVar3 = 1; iVar3 - 1U < local_44; iVar3 = iVar3 + 1) {
      AbyssEngine::String::SubString((uint)aSStack_58,(uint)aSStack_4c);
      iVar8 = AbyssEngine::String::ValueOf();
      AbyssEngine::String::~String(aSStack_58);
      Sprite::setFrame(*(Sprite **)(param_1 + 0x534),iVar8);
      this = *(Sprite **)(param_1 + 0x534);
      iVar8 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar6);
      uVar17 = Sprite::setPosition(this,iVar5 + iVar8,iVar13);
      iVar5 = iVar5 + iVar2;
      Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
    }
    AbyssEngine::String::~String(aSStack_4c);
  }
  AbyssEngine::PaintCanvas::SetColor(*puVar6);
LAB_00173afe:
  AbyssEngine::String::~String(aSStack_40);
  if (*piVar1 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001737e0: push {r4,r5,r6,r7,lr}
  001737e2: add r7,sp,#0xc
  001737e4: push {r7,r8,r9,r10,r11}
  001737e8: vpush {d8}
  001737ec: sub sp,#0x50
  001737ee: mov r11,r0
  001737f0: ldr r0,[0x00173b78]
  001737f2: ldr r1,[0x00173b7c]
  001737f4: add r0,pc
  001737f6: add r1,pc
  001737f8: ldr r0,[r0,#0x0]
  001737fa: ldr r2,[r1,#0x0]
  001737fc: str r0,[sp,#0x10]
  001737fe: ldr r1,[r0,#0x0]
  00173800: ldr r0,[r2,#0x0]
  00173802: str r2,[sp,#0x20]
  00173804: str r1,[sp,#0x4c]
  00173806: mov.w r1,#0xffffffff
  0017380a: blx 0x0006fac0
  0017380e: ldr.w r0,[r11,#0x534]
  00173812: blx 0x000768ac
  00173816: str r0,[sp,#0x24]
  00173818: ldr r0,[0x00173b80]
  0017381a: add r0,pc
  0017381c: ldr r4,[r0,#0x0]
  0017381e: ldr.w r0,[r11,#0x534]
  00173822: ldr r1,[r4,#0x0]
  00173824: ldr r1,[r1,#0x2c]
  00173826: str r1,[sp,#0x1c]
  00173828: blx 0x00077ad0
  0017382c: str r0,[sp,#0x4]
  0017382e: ldr r0,[0x00173b84]
  00173830: ldr r1,[0x00173b88]
  00173832: add r0,pc
  00173834: str r4,[sp,#0xc]
  00173836: add r1,pc
  00173838: ldr r2,[r0,#0x0]
  0017383a: ldr r3,[r1,#0x0]
  0017383c: ldr r0,[r4,#0x0]
  0017383e: ldr r1,[r2,#0x0]
  00173840: str r2,[sp,#0x14]
  00173842: ldr.w r9,[r0,#0x2c]
  00173846: add r0,sp,#0x40
  00173848: ldr.w r1,[r1,#0x184]
  0017384c: ldr r2,[r3,#0x0]
  0017384e: str r3,[sp,#0x8]
  00173850: str r2,[sp,#0x18]
  00173852: blx 0x0006f658
  00173856: ldr r0,[sp,#0x48]
  00173858: cmp r0,#0x6
  0017385a: bhi 0x0017389e
  0017385c: rsb.w r8,r0,#0x7
  00173860: add r4,sp,#0x28
  00173862: add r5,sp,#0x34
  00173864: add r6,sp,#0x40
  00173866: mov.w r10,#0x0
  0017386a: b 0x0017389a
  0017386c: ldr r1,[0x00173b8c]
  0017386e: add r1,pc
  00173870: mov r0,r4
  00173872: movs r2,#0x0
  00173874: blx 0x0006ee18
  00173878: mov r0,r5
  0017387a: mov r1,r4
  0017387c: mov r2,r6
  0017387e: blx 0x0006ef98
  00173882: mov r0,r6
  00173884: mov r1,r5
  00173886: blx 0x0006f2b0
  0017388a: mov r0,r5
  0017388c: blx 0x0006ee30
  00173890: mov r0,r4
  00173892: blx 0x0006ee30
  00173896: add.w r10,r10,#0x1
  0017389a: cmp r10,r8
  0017389c: blt 0x0017386c
  0017389e: ldr r0,[sp,#0x20]
  001738a0: ldr r1,[0x00173b6c]
  001738a2: ldr r0,[r0,#0x0]
  001738a4: blx 0x0006fac0
  001738a8: ldr r0,[sp,#0x24]
  001738aa: add r5,sp,#0x34
  001738ac: ldr r1,[sp,#0x1c]
  001738ae: add.w r10,sp,#0x40
  001738b2: movs r6,#0x1
  001738b4: subs r0,r0,r1
  001738b6: ldr r1,[sp,#0x18]
  001738b8: str r0,[sp,#0x24]
  001738ba: rsb r0,r0,r0, lsl #0x3
  001738be: add.w r1,r1,r1, lsr #0x1f
  001738c2: add.w r0,r0,r0, lsr #0x1f
  001738c6: asrs r1,r1,#0x1
  001738c8: sub.w r4,r1,r0, asr #0x1
  001738cc: asrs r0,r0,#0x1
  001738ce: str r0,[sp,#0x0]
  001738d0: str r1,[sp,#0x18]
  001738d2: mov r0,r4
  001738d4: str r4,[sp,#0x1c]
  001738d6: b 0x0017391c
  001738d8: mov r0,r5
  001738da: mov r1,r10
  001738dc: mov r3,r6
  001738de: blx 0x0006f604
  001738e2: mov r0,r5
  001738e4: blx 0x000708b8
  001738e8: mov r8,r0
  001738ea: mov r0,r5
  001738ec: blx 0x0006ee30
  001738f0: ldr.w r0,[r11,#0x534]
  001738f4: mov r1,r8
  001738f6: blx 0x00074d70
  001738fa: ldr.w r0,[r11,#0x534]
  001738fe: mov r1,r4
  00173900: mov r2,r9
  00173902: blx 0x00076060
  00173906: ldr.w r0,[r11,#0x534]
  0017390a: mov.w r1,#0x3f800000
  0017390e: mov.w r2,#0x3f800000
  00173912: blx 0x0007606c
  00173916: ldr r0,[sp,#0x24]
  00173918: adds r6,#0x1
  0017391a: add r4,r0
  0017391c: ldr r0,[sp,#0x48]
  0017391e: subs r2,r6,#0x1
  00173920: cmp r2,r0
  00173922: bcc 0x001738d8
  00173924: ldr r0,[sp,#0x14]
  00173926: ldr r0,[r0,#0x0]
  00173928: ldr.w r1,[r0,#0x180]
  0017392c: cmp r1,#0x0
  0017392e: ble.w 0x00173af2
  00173932: ldr.w r0,[r0,#0x18c]
  00173936: cmp r0,#0x2
  00173938: blt.w 0x00173af2
  0017393c: ldr r0,[sp,#0x20]
  0017393e: ldr r0,[r0,#0x0]
  00173940: mov.w r1,#0xffffffff
  00173944: blx 0x0006fac0
  00173948: ldr r0,[sp,#0xc]
  0017394a: ldr r1,[sp,#0x14]
  0017394c: ldr r0,[r0,#0x0]
  0017394e: ldr r4,[r1,#0x0]
  00173950: ldr.w r10,[r0,#0x2c]
  00173954: ldr r0,[sp,#0x4]
  00173956: add.w r1,r9,r0
  0017395a: ldr.w r0,[r4,#0x180]
  0017395e: add.w r9,r1,r10
  00173962: movw r1,#0xbb8
  00173966: cmp r0,r1
  00173968: bgt 0x00173a2e
  0017396a: movs r1,#0x64
  0017396c: blx 0x0006f514
  00173970: cmp r1,#0x32
  00173972: blt.w 0x00173afe
  00173976: ldr.w r0,[r4,#0x18c]
  0017397a: mov.w r1,#0x3e8
  0017397e: vmov.f32 s2,0x3f800000
  00173982: vldr.32 s4,[pc,#0x1ec]
  00173986: vmov s0,r0
  0017398a: muls r0,r1
  0017398c: vcvt.f32.s32 s0,s0
  00173990: vmov s6,r0
  00173994: vcvt.f32.s32 s6,s6
  00173998: vmla.f32 s2,s0,s4
  0017399c: vmul.f32 s0,s2,s6
  001739a0: vcvt.s32.f32 s0,s0
  001739a4: vmov r1,s0
  001739a8: add r0,sp,#0x34
  001739aa: blx 0x0006f658
  001739ae: ldr r0,[sp,#0x4]
  001739b0: add r6,sp,#0x28
  001739b2: mov.w r8,#0x0
  001739b6: movs r5,#0x1
  001739b8: add r0,r9
  001739ba: str r0,[sp,#0x4]
  001739bc: ldr r0,[sp,#0x3c]
  001739be: subs r2,r5,#0x1
  001739c0: cmp r2,r0
  001739c2: bcs 0x00173a28
  001739c4: add r1,sp,#0x34
  001739c6: mov r0,r6
  001739c8: mov r3,r5
  001739ca: blx 0x0006f604
  001739ce: mov r0,r6
  001739d0: blx 0x000708b8
  001739d4: mov r4,r0
  001739d6: mov r0,r6
  001739d8: blx 0x0006ee30
  001739dc: ldr.w r0,[r11,#0x534]
  001739e0: mov r1,r4
  001739e2: blx 0x00074d70
  001739e6: ldr r0,[sp,#0x3c]
  001739e8: ldr r2,[sp,#0x24]
  001739ea: ldr r1,[sp,#0xc]
  001739ec: mul r3,r0,r2
  001739f0: ldr r0,[sp,#0x8]
  001739f2: ldr r1,[r1,#0x0]
  001739f4: ldr r2,[sp,#0x4]
  001739f6: ldr r4,[r0,#0x0]
  001739f8: ldr r1,[r1,#0x2c]
  001739fa: ldr.w r0,[r11,#0x534]
  001739fe: add r2,r1
  00173a00: add.w r1,r4,r4, lsr #0x1f
  00173a04: asrs r1,r1,#0x1
  00173a06: sub.w r1,r1,r3, lsr #0x1
  00173a0a: add r1,r8
  00173a0c: blx 0x00076060
  00173a10: ldr r0,[sp,#0x24]
  00173a12: adds r5,#0x1
  00173a14: add r8,r0
  00173a16: ldr.w r0,[r11,#0x534]
  00173a1a: mov.w r1,#0x3f800000
  00173a1e: mov.w r2,#0x3f800000
  00173a22: blx 0x0007606c
  00173a26: b 0x001739bc
  00173a28: add r0,sp,#0x34
  00173a2a: blx 0x0006ee30
  00173a2e: ldr r0,[sp,#0x1c]
  00173a30: ldr.w r1,[r11,#0x538]
  00173a34: add.w r2,r10,r0
  00173a38: ldr r0,[sp,#0x20]
  00173a3a: ldr r0,[r0,#0x0]
  00173a3c: mov r3,r9
  00173a3e: blx 0x00072dc0
  00173a42: ldr r0,[sp,#0x14]
  00173a44: movw r2,#0x1b58
  00173a48: vldr.32 s2,[pc,#0x128]
  00173a4c: vmov.f32 s4,0x3f800000
  00173a50: ldr r0,[r0,#0x0]
  00173a52: ldr.w r1,[r0,#0x180]
  00173a56: subs r1,r1,r2
  00173a58: vmov s0,r1
  00173a5c: vcvt.f32.s32 s0,s0
  00173a60: vmul.f32 s0,s0,s2
  00173a64: vcmpe.f32 s0,#0
  00173a68: vmrs apsr,fpscr
  00173a6c: vadd.f32 s16,s0,s4
  00173a70: it mi
  00173a72: vmov.mi.f32 s16,s4
  00173a76: ldr.w r1,[r0,#0x18c]
  00173a7a: add r0,sp,#0x34
  00173a7c: blx 0x0006f658
  00173a80: ldr r0,[sp,#0x18]
  00173a82: add.w r8,sp,#0x34
  00173a86: ldr r1,[sp,#0x0]
  00173a88: movs r6,#0x1
  00173a8a: add r0,r10
  00173a8c: add.w r10,sp,#0x28
  00173a90: subs r5,r0,r1
  00173a92: ldr r0,[sp,#0x3c]
  00173a94: subs r2,r6,#0x1
  00173a96: cmp r2,r0
  00173a98: bcs 0x00173aec
  00173a9a: mov r0,r10
  00173a9c: mov r1,r8
  00173a9e: mov r3,r6
  00173aa0: blx 0x0006f604
  00173aa4: mov r0,r10
  00173aa6: blx 0x000708b8
  00173aaa: mov r4,r0
  00173aac: mov r0,r10
  00173aae: blx 0x0006ee30
  00173ab2: ldr.w r0,[r11,#0x534]
  00173ab6: mov r1,r4
  00173ab8: blx 0x00074d70
  00173abc: ldr r0,[sp,#0x20]
  00173abe: ldr.w r4,[r11,#0x534]
  00173ac2: ldr.w r1,[r11,#0x538]
  00173ac6: ldr r0,[r0,#0x0]
  00173ac8: blx 0x00072d84
  00173acc: adds r1,r5,r0
  00173ace: mov r0,r4
  00173ad0: mov r2,r9
  00173ad2: blx 0x00076060
  00173ad6: vmov r1,s16
  00173ada: ldr r0,[sp,#0x24]
  00173adc: adds r6,#0x1
  00173ade: add r5,r0
  00173ae0: ldr.w r0,[r11,#0x534]
  00173ae4: mov r2,r1
  00173ae6: blx 0x0007606c
  00173aea: b 0x00173a92
  00173aec: add r0,sp,#0x34
  00173aee: blx 0x0006ee30
  00173af2: ldr r0,[sp,#0x20]
  00173af4: ldr r0,[r0,#0x0]
  00173af6: mov.w r1,#0xffffffff
  00173afa: blx 0x0006fac0
  00173afe: add r0,sp,#0x40
  00173b00: blx 0x0006ee30
  00173b04: ldr r0,[sp,#0x4c]
  00173b06: ldr r1,[sp,#0x10]
  00173b08: ldr r1,[r1,#0x0]
  00173b0a: subs r0,r1,r0
  00173b0c: itttt eq
  00173b0e: add.eq sp,#0x50
  00173b10: vpop.eq {d8}
  00173b14: add.eq sp,#0x4
  00173b16: pop.eq.w {r8,r9,r10,r11}
  00173b1a: it eq
  00173b1c: pop.eq {r4,r5,r6,r7,pc}
  00173b1e: blx 0x0006e824
```
