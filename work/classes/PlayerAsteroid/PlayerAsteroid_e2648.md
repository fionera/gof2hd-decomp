# PlayerAsteroid::PlayerAsteroid
elf 0xe2648 body 734
Sig: undefined __thiscall PlayerAsteroid(PlayerAsteroid * this, int param_1, AEGeometry * param_2, int param_3, int param_4, Vector * param_5, float param_6, int param_7)

## decompile
```c

/* PlayerAsteroid::PlayerAsteroid(int, AEGeometry*, int, int, AbyssEngine::AEMath::Vector const&,
   float, int) */

void __thiscall
PlayerAsteroid::PlayerAsteroid
          (PlayerAsteroid *this,int param_1,AEGeometry *param_2,int param_3,int param_4,
          Vector *param_5,float param_6,int param_7)

{
  int *piVar1;
  Player *pPVar2;
  uint *puVar3;
  int iVar4;
  undefined4 uVar5;
  Explosion *this_00;
  int iVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  int *piVar9;
  AEGeometry *this_01;
  uint in_fpscr;
  float fVar10;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s2;
  undefined4 extraout_s3;
  undefined8 unaff_d8;
  float fVar11;
  int in_stack_0000000c;
  float local_48;
  float local_44;
  float local_40;
  int local_3c;
  
  piVar1 = *(int **)(DAT_000f2968 + 0xf2660);
  local_3c = *piVar1;
  pPVar2 = operator_new(0x114);
  Player::Player(pPVar2,0x5dc,0x1e,0,0,0);
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,pPVar2,param_2,*(float *)param_5,extraout_s1,
                     *(float *)(param_5 + 4),SUB41(*(float *)param_5,0));
  *(int *)this = *(int *)(DAT_000f296c + 0xf26be) + 8;
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined4 *)(this + 0x144) = 0;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  *(undefined4 *)(this + 0x16c) = 0;
  Player::setKIPlayer(*(Player **)(this + 4),(KIPlayer *)this);
  puVar8 = (undefined4 *)(DAT_000f2970 + 0xf26e4);
  this[0x124] = (PlayerAsteroid)0x0;
  *(int *)(this + 0x128) = param_4;
  puVar3 = (uint *)*puVar8;
  *(int *)(this + 0x138) = param_7;
  *(int *)(this + 0x150) = in_stack_0000000c;
  pPVar2 = *(Player **)(this + 4);
  iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
  Player::setRadius(pPVar2,(int)(*(float *)(iVar4 + 0xe0) * (float)param_7 * DAT_000f294c));
  Player::setMaxHitpoints(*(Player **)(this + 4),(int)((float)param_7 * DAT_000f2950 + 30.0));
  uVar5 = Player::getHitpoints();
  this[0x13c] = (PlayerAsteroid)(3 < in_stack_0000000c);
  *(undefined4 *)(this + 0x158) = uVar5;
  this_00 = operator_new(0x68);
  fVar10 = (float)Explosion::Explosion(this_00,param_3 + 2);
  *(Explosion **)(this + 300) = this_00;
  Explosion::setScaling(fVar10);
  AEGeometry::setScaling(extraout_s0,extraout_s1_00,extraout_s2);
  (**(code **)(*(int *)this + 0x44))(this,param_5);
  piVar9 = *(int **)(DAT_000f2974 + 0xf2790);
  iVar4 = AbyssEngine::AERandom::nextInt(*piVar9);
  fVar11 = (float)VectorSignedToFloat(iVar4 + -0x1000,(byte)(in_fpscr >> 0x16) & 3);
  iVar4 = AbyssEngine::AERandom::nextInt(*piVar9);
  iVar6 = AbyssEngine::AERandom::nextInt(*piVar9);
  fVar10 = (float)VectorSignedToFloat(iVar4 + -0x1000,(byte)(in_fpscr >> 0x16) & 3);
  local_40 = (float)VectorSignedToFloat(iVar6 + -0x1000,(byte)(in_fpscr >> 0x16) & 3);
  local_40 = local_40 * DAT_000f2954;
  _local_48 = CONCAT44(fVar10 * DAT_000f2954,fVar11 * DAT_000f2954);
  AbyssEngine::AEMath::Vector::operator=(*(Vector **)(DAT_000f2978 + 0xf27e2),(Vector *)&local_48);
  this_01 = *(AEGeometry **)(this + 8);
  uVar5 = AbyssEngine::AERandom::nextInt(*piVar9);
  fVar11 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  uVar5 = AbyssEngine::AERandom::nextInt(*piVar9);
  uVar7 = AbyssEngine::AERandom::nextInt(*piVar9);
  VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  fVar10 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
  AEGeometry::setRotation
            (this_01,fVar10 * DAT_000f2958 * DAT_000f295c,extraout_s1_01,
             fVar11 * DAT_000f2958 * DAT_000f295c);
  *(undefined4 *)(this + 0x78) = 0;
  this[0x14c] = (PlayerAsteroid)0x1;
  iVar4 = AbyssEngine::AERandom::nextInt(*piVar9);
  VectorSignedToFloat(iVar4 + -1,(byte)(in_fpscr >> 0x16) & 3);
  iVar4 = AbyssEngine::AERandom::nextInt(*piVar9);
  iVar6 = AbyssEngine::AERandom::nextInt(*piVar9);
  FloatVectorMax(CONCAT44((int)((ulonglong)unaff_d8 >> 0x20),param_7),
                 CONCAT44(extraout_s3,DAT_000f2960),2,0x20);
  fVar10 = (float)VectorSignedToFloat(iVar4 + -1,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
  VectorSignedToFloat(iVar6 + -1,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
  AbyssEngine::AEMath::operator*((Vector *)&local_48,fVar10);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x140),(Vector *)&local_48);
  uVar5 = DAT_000f2964;
  puVar8 = (undefined4 *)(DAT_000f297c + 0xf28f4);
  this[0x4c] = (PlayerAsteroid)0x1;
  this[0x3c] = (PlayerAsteroid)0x1;
  puVar8 = (undefined4 *)*puVar8;
  *(undefined4 *)(this + 0x88) = 0;
  *puVar8 = 0;
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x160) = uVar5;
  iVar4 = *piVar1 - local_3c;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  000f2648: push {r4,r5,r6,r7,lr}
  000f264a: add r7,sp,#0xc
  000f264c: push {r7,r8,r9,r10,r11}
  000f2650: vpush {d1,d2,d3,d4,d5,d6,d7,d8,d9}
  000f2654: mov r4,r0
  000f2656: ldr r0,[0x000f2968]
  000f2658: mov r8,r3
  000f265a: mov r10,r2
  000f265c: add r0,pc
  000f265e: mov r5,r1
  000f2660: ldr r0,[r0,#0x0]
  000f2662: str r0,[sp,#0x18]
  000f2664: ldr r0,[r0,#0x0]
  000f2666: str r0,[sp,#0x34]
  000f2668: mov.w r0,#0x114
  000f266c: blx 0x0006eb24
  000f2670: mov r6,r0
  000f2672: mov.w r11,#0x0
  000f2676: movw r1,#0x5dc
  000f267a: movs r2,#0x1e
  000f267c: movs r3,#0x0
  000f267e: strd r11,r11,[sp,#0x0]
  000f2682: blx 0x00073d80
  000f2686: ldr.w r9,[r7,#0xc]
  000f268a: mov r0,r4
  000f268c: mov r1,r5
  000f268e: mov.w r2,#0xffffffff
  000f2692: mov r3,r6
  000f2694: vldr.32 s0,[r9]
  000f2698: vldr.32 s2,[r9,#0x4]
  000f269c: vldr.32 s4,[r9,#0x8]
  000f26a0: str.w r11,[sp,#0x10]
  000f26a4: vstr.32 s0,[sp,#0x4]
  000f26a8: vstr.32 s2,[sp,#0x8]
  000f26ac: vstr.32 s4,[sp,#0xc]
  000f26b0: str.w r10,[sp,#0x0]
  000f26b4: blx 0x00075790
  000f26b8: ldr r0,[0x000f296c]
  000f26ba: add r0,pc
  000f26bc: ldr r0,[r0,#0x0]
  000f26be: adds r0,#0x8
  000f26c0: str r0,[r4,#0x0]
  000f26c2: strd r11,r11,[r4,#0x140]
  000f26c6: str.w r11,[r4,#0x148]
  000f26ca: strd r11,r11,[r4,#0x164]
  000f26ce: str.w r11,[r4,#0x16c]
  000f26d2: ldr r0,[r4,#0x4]
  000f26d4: mov r1,r4
  000f26d6: blx 0x00073288
  000f26da: ldr r1,[0x000f2970]
  000f26dc: movs r2,#0x0
  000f26de: ldr r0,[r7,#0x8]
  000f26e0: add r1,pc
  000f26e2: strb.w r2,[r4,#0x124]
  000f26e6: str.w r0,[r4,#0x128]
  000f26ea: vldr.32 s16,[r7,#0x10]
  000f26ee: ldr r0,[r1,#0x0]
  000f26f0: ldr r6,[r7,#0x14]
  000f26f2: vstr.32 s16,[r4,#0x138]
  000f26f6: str.w r6,[r4,#0x150]
  000f26fa: ldr r5,[r4,#0x4]
  000f26fc: ldr.w r1,[r10,#0xc]
  000f2700: ldr r0,[r0,#0x0]
  000f2702: blx 0x00072088
  000f2706: vldr.32 s0,[r0,#0xe0]
  000f270a: vldr.32 s2,[pc,#0x240]
  000f270e: vmul.f32 s0,s0,s16
  000f2712: vmul.f32 s0,s0,s2
  000f2716: vcvt.s32.f32 s0,s0
  000f271a: vmov r1,s0
  000f271e: mov r0,r5
  000f2720: blx 0x000730d8
  000f2724: vmov.f32 s0,0x41f00000
  000f2728: vldr.32 s2,[pc,#0x224]
  000f272c: ldr r0,[r4,#0x4]
  000f272e: vmla.f32 s0,s16,s2
  000f2732: vcvt.s32.f32 s0,s0
  000f2736: vmov r1,s0
  000f273a: blx 0x00073f78
  000f273e: ldr r0,[r4,#0x4]
  000f2740: blx 0x000724f0
  000f2744: movs r1,#0x0
  000f2746: cmp r6,#0x3
  000f2748: it gt
  000f274a: mov.gt r1,#0x1
  000f274c: strb.w r1,[r4,#0x13c]
  000f2750: str.w r0,[r4,#0x158]
  000f2754: movs r0,#0x68
  000f2756: blx 0x0006eb24
  000f275a: mov r6,r0
  000f275c: add.w r1,r8,#0x2
  000f2760: blx 0x000727f0
  000f2764: vmov r5,s16
  000f2768: str.w r6,[r4,#0x12c]
  000f276c: mov r0,r6
  000f276e: mov r1,r5
  000f2770: blx 0x000727fc
  000f2774: ldr r0,[r4,#0x8]
  000f2776: mov r1,r5
  000f2778: mov r2,r5
  000f277a: mov r3,r5
  000f277c: blx 0x000727b4
  000f2780: ldr r0,[r4,#0x0]
  000f2782: ldr r2,[r0,#0x44]
  000f2784: mov r0,r4
  000f2786: mov r1,r9
  000f2788: blx r2
  000f278a: ldr r0,[0x000f2974]
  000f278c: add r0,pc
  000f278e: ldr r6,[r0,#0x0]
  000f2790: ldr r0,[r6,#0x0]
  000f2792: mov.w r1,#0x2000
  000f2796: blx 0x00071848
  000f279a: sub.w r0,r0,#0x1000
  000f279e: vmov s0,r0
  000f27a2: vcvt.f32.s32 s18,s0
  000f27a6: ldr r0,[r6,#0x0]
  000f27a8: mov.w r1,#0x2000
  000f27ac: blx 0x00071848
  000f27b0: mov r5,r0
  000f27b2: ldr r0,[r6,#0x0]
  000f27b4: mov.w r1,#0x2000
  000f27b8: blx 0x00071848
  000f27bc: sub.w r1,r5,#0x1000
  000f27c0: sub.w r0,r0,#0x1000
  000f27c4: vldr.32 s0,[pc,#0x18c]
  000f27c8: vmov s4,r1
  000f27cc: vmov s6,r0
  000f27d0: vcvt.f32.s32 s4,s4
  000f27d4: vcvt.f32.s32 s6,s6
  000f27d8: ldr r0,[0x000f2978]
  000f27da: vmul.f32 s2,s18,s0
  000f27de: add r0,pc
  000f27e0: ldr r0,[r0,#0x0]
  000f27e2: vmul.f32 s4,s4,s0
  000f27e6: vmul.f32 s0,s6,s0
  000f27ea: vstr.32 s2,[sp,#0x28]
  000f27ee: vstr.32 s4,[sp,#0x2c]
  000f27f2: vstr.32 s0,[sp,#0x30]
  000f27f6: add r1,sp,#0x28
  000f27f8: blx 0x0006eb3c
  000f27fc: ldr.w r8,[r4,#0x8]
  000f2800: ldr r0,[r6,#0x0]
  000f2802: movs r1,#0x64
  000f2804: blx 0x00071848
  000f2808: vmov s0,r0
  000f280c: vcvt.f32.s32 s18,s0
  000f2810: ldr r0,[r6,#0x0]
  000f2812: movs r1,#0x64
  000f2814: blx 0x00071848
  000f2818: mov r5,r0
  000f281a: ldr r0,[r6,#0x0]
  000f281c: movs r1,#0x64
  000f281e: blx 0x00071848
  000f2822: vldr.32 s0,[pc,#0x134]
  000f2826: vmov s6,r5
  000f282a: vldr.32 s4,[pc,#0x130]
  000f282e: vmov s8,r0
  000f2832: vmul.f32 s2,s18,s0
  000f2836: vcvt.f32.s32 s6,s6
  000f283a: vcvt.f32.s32 s8,s8
  000f283e: vmul.f32 s2,s2,s4
  000f2842: vmul.f32 s6,s6,s0
  000f2846: vmul.f32 s0,s8,s0
  000f284a: vmov r1,s2
  000f284e: vmul.f32 s6,s6,s4
  000f2852: vmul.f32 s0,s0,s4
  000f2856: vmov r2,s6
  000f285a: vmov r3,s0
  000f285e: mov r0,r8
  000f2860: blx 0x00073054
  000f2864: movs r0,#0x0
  000f2866: str r0,[r4,#0x78]
  000f2868: movs r0,#0x1
  000f286a: strb.w r0,[r4,#0x14c]
  000f286e: ldr r0,[r6,#0x0]
  000f2870: movs r1,#0x3
  000f2872: blx 0x00071848
  000f2876: subs r0,#0x1
  000f2878: vmov s0,r0
  000f287c: vcvt.f32.s32 s18,s0
  000f2880: ldr r0,[r6,#0x0]
  000f2882: movs r1,#0x3
  000f2884: blx 0x00071848
  000f2888: mov r5,r0
  000f288a: ldr r0,[r6,#0x0]
  000f288c: movs r1,#0x3
  000f288e: blx 0x00071848
  000f2892: vldr.32 s2,[pc,#0xcc]
  000f2896: vmov.f32 s0,0x3f800000
  000f289a: subs r1,r5,#0x1
  000f289c: subs r0,#0x1
  000f289e: vmax.f32 d1,d8,d1
  000f28a2: vcmpe.f32 s2,s0
  000f28a6: vmrs apsr,fpscr
  000f28aa: vmov.f32 s4,s0
  000f28ae: it mi
  000f28b0: vmov.mi.f32 s4,s2
  000f28b4: vmov s2,r0
  000f28b8: vsub.f32 s0,s0,s4
  000f28bc: vmov r2,s0
  000f28c0: vmov s0,r1
  000f28c4: vcvt.f32.s32 s0,s0
  000f28c8: vcvt.f32.s32 s2,s2
  000f28cc: vstr.32 s18,[sp,#0x1c]
  000f28d0: vstr.32 s0,[sp,#0x20]
  000f28d4: vstr.32 s2,[sp,#0x24]
  000f28d8: add r0,sp,#0x28
  000f28da: add r1,sp,#0x1c
  000f28dc: blx 0x0006ec74
  000f28e0: add.w r0,r4,#0x140
  000f28e4: add r1,sp,#0x28
  000f28e6: blx 0x0006eb3c
  000f28ea: ldr r0,[0x000f297c]
  000f28ec: movs r2,#0x1
  000f28ee: ldr r1,[0x000f2964]
  000f28f0: add r0,pc
  000f28f2: strb.w r2,[r4,#0x4c]
  000f28f6: strb.w r2,[r4,#0x3c]
  000f28fa: movs r2,#0x0
  000f28fc: ldr r0,[r0,#0x0]
  000f28fe: str.w r2,[r4,#0x88]
  000f2902: str r2,[r0,#0x0]
  000f2904: strd r2,r1,[r4,#0x15c]
  000f2908: ldr r0,[sp,#0x34]
  000f290a: ldr r1,[sp,#0x18]
  000f290c: ldr r1,[r1,#0x0]
  000f290e: subs r0,r1,r0
  000f2910: itttt eq
  000f2912: mov.eq r0,r4
  000f2914: vpop.eq {d1,d2,d3,d4,d5,d6,d7,d8,d9}
  000f2918: add.eq sp,#0x4
  000f291a: pop.eq.w {r8,r9,r10,r11}
  000f291e: it eq
  000f2920: pop.eq {r4,r5,r6,r7,pc}
  000f2922: blx 0x0006e824
```
