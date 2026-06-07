# MineGun::update
elf 0x1566ec body 454
Sig: undefined __thiscall update(MineGun * this, int param_1)

## decompile
```c

/* MineGun::update(int) */

void __thiscall MineGun::update(MineGun *this,int param_1)

{
  float fVar1;
  float fVar2;
  int *piVar3;
  uint uVar4;
  float fVar5;
  int iVar6;
  int iVar7;
  uint in_fpscr;
  float extraout_s0;
  float fVar8;
  float extraout_s0_00;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined8 local_50;
  undefined8 local_48;
  
  piVar3 = *(int **)(DAT_001668bc + 0x166702);
  local_48 = CONCAT44(*piVar3,in_s14);
  local_50 = in_d6;
  ObjectGun::update((ObjectGun *)this,param_1);
  if (*(char *)(*(int *)(this + 8) + 0x4c) != '\0') {
    uVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_001668c0 + 0x166720));
    AbyssEngine::Transform::Update((ulonglong)uVar4,SUB41(param_1,0));
  }
  fVar2 = DAT_001668b8;
  fVar1 = DAT_001668b4;
  iVar7 = 0;
  for (uVar4 = 0; iVar6 = *(int *)(this + 8), uVar4 < *(uint *)(iVar6 + 8); uVar4 = uVar4 + 1) {
    if (*(char *)(*(int *)(iVar6 + 0x40) + uVar4) != '\0') {
      if (*(char *)(*(int *)(this + 0xb8) + uVar4) != '\0') {
        *(undefined4 *)(this + 0xcc) = 0;
        iVar6 = *(int *)(iVar6 + 0x30);
        PlayerEgo::getPosition();
        AbyssEngine::AEMath::operator-((Vector *)&local_50,(Vector *)(iVar6 + iVar7));
        fVar5 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_50);
        in_fpscr = in_fpscr & 0xfffffff;
        fVar8 = fVar1;
        if (fVar5 < fVar1) {
          fVar8 = fVar5;
        }
        *(float *)(this + 0xd0) = 1.0 - fVar8 / fVar1;
        iVar6 = PlayerEgo::getTargetFollowCamera();
        TargetFollowCamera::setRumblePercentage(extraout_s0,iVar6);
        local_50 = 0;
        local_48 = local_48 & 0xffffffff00000000;
        Explosion::start(*(Vector **)(*(int *)(*(int *)(this + 0xb4) + 4) + uVar4 * 4),
                         (Vector *)(*(int *)(*(int *)(this + 8) + 0x30) + iVar7));
        *(undefined1 *)(*(int *)(this + 0xb8) + uVar4) = 0;
      }
      Explosion::update(*(Explosion **)(*(int *)(*(int *)(this + 0xb4) + 4) + uVar4 * 4),param_1,
                        (TargetFollowCamera *)0x0);
      iVar6 = *(int *)(this + 0xcc) + param_1;
      if (1999 < iVar6) {
        iVar6 = 2000;
      }
      *(int *)(this + 0xcc) = iVar6;
      iVar6 = PlayerEgo::getTargetFollowCamera();
      fVar8 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0xcc),(byte)(in_fpscr >> 0x16) & 3);
      TargetFollowCamera::setRumblePercentage(*(float *)(this + 0xd0) * (fVar8 / fVar2 + 1.0),iVar6)
      ;
      iVar6 = Explosion::isPlaying();
      if (iVar6 == 0) {
        iVar6 = PlayerEgo::getTargetFollowCamera();
        TargetFollowCamera::setRumblePercentage(extraout_s0_00,iVar6);
        iVar6 = *(int *)(this + 8);
        *(undefined4 *)(this + 0xcc) = 0;
        *(undefined1 *)(*(int *)(iVar6 + 0x40) + uVar4) = 0;
        *(undefined1 *)(iVar6 + 0x88) = 0;
        *(undefined1 *)(*(int *)(this + 0xb8) + uVar4) = 1;
        Explosion::reset(*(Explosion **)(*(int *)(*(int *)(this + 0xb4) + 4) + uVar4 * 4));
      }
    }
    iVar7 = iVar7 + 0xc;
  }
  if (*piVar3 != local_48._4_4_) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001666ec: push {r4,r5,r6,r7,lr}
  001666ee: add r7,sp,#0xc
  001666f0: push {r7,r8,r9,r10,r11}
  001666f4: vpush {d3,d4,d5,d6,d7,d8,d9,d10}
  001666f8: mov r5,r0
  001666fa: ldr r0,[0x001668bc]
  001666fc: mov r4,r1
  001666fe: add r0,pc
  00166700: ldr r0,[r0,#0x0]
  00166702: str r0,[sp,#0x8]
  00166704: ldr r0,[r0,#0x0]
  00166706: str r0,[sp,#0x24]
  00166708: mov r0,r5
  0016670a: blx 0x000775fc
  0016670e: ldr r0,[r5,#0x8]
  00166710: ldrb.w r0,[r0,#0x4c]
  00166714: cbz r0,0x00166734
  00166716: ldr r0,[0x001668c0]
  00166718: ldr.w r1,[r5,#0xbc]
  0016671c: add r0,pc
  0016671e: ldr r0,[r0,#0x0]
  00166720: ldr r1,[r1,#0xc]
  00166722: ldr r0,[r0,#0x0]
  00166724: blx 0x00072088
  00166728: movs r1,#0x0
  0016672a: asrs r3,r4,#0x1f
  0016672c: mov r2,r4
  0016672e: str r1,[sp,#0x0]
  00166730: blx 0x0006f7cc
  00166734: vmov.f32 s18,0x3f800000
  00166738: vldr.32 s16,[pc,#0x17c]
  0016673c: vldr.32 s20,[pc,#0x174]
  00166740: mov.w r9,#0x0
  00166744: mov.w r11,#0x0
  00166748: movs r6,#0x0
  0016674a: b 0x0016688e
  0016674c: ldr r1,[r0,#0x40]
  0016674e: ldrb.w r1,[r1,r11]
  00166752: cmp r1,#0x0
  00166754: beq.w 0x00166886
  00166758: ldr.w r1,[r5,#0xb8]
  0016675c: ldrb.w r1,[r1,r11]
  00166760: cmp r1,#0x0
  00166762: beq 0x001667ec
  00166764: str.w r6,[r5,#0xcc]
  00166768: add.w r10,sp,#0xc
  0016676c: ldr.w r1,[r5,#0xb0]
  00166770: ldr.w r8,[r0,#0x30]
  00166774: mov r0,r10
  00166776: blx 0x0007264c
  0016677a: add.w r1,r8,r9
  0016677e: mov r8,r4
  00166780: add r4,sp,#0x18
  00166782: mov r2,r10
  00166784: mov r0,r4
  00166786: blx 0x0006ec38
  0016678a: mov r0,r4
  0016678c: blx 0x0006ec44
  00166790: vmov s0,r0
  00166794: vmov.f32 s2,s20
  00166798: vcmpe.f32 s0,s20
  0016679c: vmrs apsr,fpscr
  001667a0: it mi
  001667a2: vmov.mi.f32 s2,s0
  001667a6: ldr.w r0,[r5,#0xb0]
  001667aa: vdiv.f32 s0,s2,s20
  001667ae: vsub.f32 s0,s18,s0
  001667b2: vstr.32 s0,[r5,#0xd0]
  001667b6: blx 0x0007588c
  001667ba: ldr.w r1,[r5,#0xd0]
  001667be: movs r2,#0x32
  001667c0: blx 0x000728ec
  001667c4: ldr.w r1,[r5,#0xb4]
  001667c8: ldr r0,[r5,#0x8]
  001667ca: ldr r1,[r1,#0x4]
  001667cc: ldr r2,[r0,#0x30]
  001667ce: ldr.w r0,[r1,r11,lsl #0x2]
  001667d2: add.w r1,r2,r9
  001667d6: mov r2,r4
  001667d8: strd r6,r6,[sp,#0x18]
  001667dc: mov r4,r8
  001667de: str r6,[sp,#0x20]
  001667e0: blx 0x00072808
  001667e4: ldr.w r0,[r5,#0xb8]
  001667e8: strb.w r6,[r0,r11]
  001667ec: ldr.w r0,[r5,#0xb4]
  001667f0: mov r1,r4
  001667f2: movs r2,#0x0
  001667f4: ldr r0,[r0,#0x4]
  001667f6: ldr.w r0,[r0,r11,lsl #0x2]
  001667fa: blx 0x00075934
  001667fe: ldr.w r1,[r5,#0xcc]
  00166802: ldr.w r0,[r5,#0xb0]
  00166806: add r1,r4
  00166808: cmp.w r1,#0x7d0
  0016680c: it ge
  0016680e: mov.ge.w r1,#0x7d0
  00166812: str.w r1,[r5,#0xcc]
  00166816: blx 0x0007588c
  0016681a: vldr.32 s0,[r5,#0xcc]
  0016681e: movs r2,#0x32
  00166820: vldr.32 s2,[r5,#0xd0]
  00166824: vcvt.f32.s32 s0,s0
  00166828: vdiv.f32 s0,s0,s16
  0016682c: vadd.f32 s0,s0,s18
  00166830: vmul.f32 s0,s2,s0
  00166834: vmov r1,s0
  00166838: blx 0x000728ec
  0016683c: ldr.w r0,[r5,#0xb4]
  00166840: ldr r0,[r0,#0x4]
  00166842: ldr.w r0,[r0,r11,lsl #0x2]
  00166846: blx 0x000757fc
  0016684a: cbnz r0,0x00166886
  0016684c: ldr.w r0,[r5,#0xb0]
  00166850: blx 0x0007588c
  00166854: movs r1,#0x0
  00166856: movs r2,#0x0
  00166858: blx 0x000728ec
  0016685c: ldr r0,[r5,#0x8]
  0016685e: movs r2,#0x0
  00166860: str.w r2,[r5,#0xcc]
  00166864: ldr r1,[r0,#0x40]
  00166866: strb.w r2,[r1,r11]
  0016686a: movs r1,#0x1
  0016686c: strb.w r2,[r0,#0x88]
  00166870: ldr.w r0,[r5,#0xb8]
  00166874: strb.w r1,[r0,r11]
  00166878: ldr.w r0,[r5,#0xb4]
  0016687c: ldr r0,[r0,#0x4]
  0016687e: ldr.w r0,[r0,r11,lsl #0x2]
  00166882: blx 0x00072c88
  00166886: add.w r9,r9,#0xc
  0016688a: add.w r11,r11,#0x1
  0016688e: ldr r0,[r5,#0x8]
  00166890: ldr r1,[r0,#0x8]
  00166892: cmp r11,r1
  00166894: bcc.w 0x0016674c
  00166898: ldr r0,[sp,#0x24]
  0016689a: ldr r1,[sp,#0x8]
  0016689c: ldr r1,[r1,#0x0]
  0016689e: subs r0,r1,r0
  001668a0: itttt eq
  001668a2: vpop.eq {d3,d4,d5,d6,d7,d8,d9,d10}
  001668a6: add.eq sp,#0x4
  001668a8: pop.eq.w {r8,r9,r10,r11}
  001668ac: pop.eq {r4,r5,r6,r7,pc}
  001668ae: blx 0x0006e824
```
