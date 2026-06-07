# PlayerTurret::PlayerTurret
elf 0x157040 body 670
Sig: undefined __thiscall PlayerTurret(PlayerTurret * this, int param_1, Player * param_2, AEGeometry * param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* PlayerTurret::PlayerTurret(int, Player*, AEGeometry*, float, float, float) */

void __thiscall
PlayerTurret::PlayerTurret
          (PlayerTurret *this,int param_1,Player *param_2,AEGeometry *param_3,float param_4,
          float param_5,float param_6)

{
  undefined8 uVar1;
  undefined8 uVar2;
  AEGeometry *pAVar3;
  int iVar4;
  void *pvVar5;
  Explosion *this_00;
  ushort uVar6;
  int *piVar7;
  uint *puVar8;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s2_00;
  longlong in_d6;
  undefined4 in_s14;
  bool in_stack_00000000;
  undefined4 in_stack_00000008;
  longlong local_48;
  undefined4 local_40;
  int local_3c;
  
  piVar7 = *(int **)(DAT_00167334 + 0x16705a);
  local_3c = *piVar7;
  local_48 = in_d6;
  local_40 = in_s14;
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,param_2,param_3,param_4,param_5,param_6,
                     in_stack_00000000);
  *(int *)this = *(int *)(DAT_00167338 + 0x167092) + 8;
  this[0x25] = (PlayerTurret)0x1;
  this[0x3e] = (PlayerTurret)0x1;
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined4 *)(this + 0x144) = 0;
  *(undefined4 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 0x154) = 0;
  *(undefined4 *)(this + 0x158) = 0;
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined4 *)(this + 0x164) = 50000;
  pAVar3 = operator_new(0xc0);
  puVar8 = *(uint **)(DAT_0016733c + 0x1670c6);
  AEGeometry::AEGeometry(pAVar3,(ushort)param_1,(PaintCanvas *)*puVar8,false);
  *(AEGeometry **)(this + 0x140) = pAVar3;
  if (param_1 == 0x381b) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,0x381c,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 0x144) = pAVar3;
    AEGeometry::setRotationOrder(pAVar3,2);
    local_48 = (ulonglong)DAT_00167328 << 0x20;
    local_40 = DAT_00167324;
    AEGeometry::setPosition(*(AEGeometry **)(this + 0x144),(Vector *)&local_48);
  }
  else if (param_1 == 0x1a76) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,0x1a77,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 0x144) = pAVar3;
    AEGeometry::setRotationOrder(pAVar3,2);
    local_48 = (ulonglong)DAT_00167320 << 0x20;
    local_40 = 0;
    AEGeometry::setPosition(*(AEGeometry **)(this + 0x144),(Vector *)&local_48);
  }
  else if (param_1 == 0x1a74) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,0x1a75,(PaintCanvas *)*puVar8,false);
    *(AEGeometry **)(this + 0x144) = pAVar3;
    AEGeometry::setRotationOrder(pAVar3,2);
    local_48 = (ulonglong)DAT_00167320 << 0x20;
    local_40 = 0;
    AEGeometry::setPosition(*(AEGeometry **)(this + 0x144),(Vector *)&local_48);
  }
  pAVar3 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar3,(PaintCanvas *)*puVar8);
  *(AEGeometry **)(this + 0x148) = pAVar3;
  iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
  *(undefined4 *)(iVar4 + 0xe0) = DAT_0016732c;
  _local_40 = CONCAT44(local_3c,in_stack_00000008);
  (**(code **)(*(int *)this + 0x44))(this,&local_48);
  if (param_1 == 0x381b) {
    AEGeometry::rotate(extraout_s0,extraout_s1,extraout_s2);
    AEGeometry::rotate(extraout_s0_00,extraout_s1_00,extraout_s2_00);
    AEGeometry::addChild(*(uint *)(this + 0x148));
  }
  else if ((param_1 | 2U) == 0x1a76) {
    AEGeometry::addChild(*(uint *)(this + 0x148));
  }
  else if (param_1 - 0x49c0U < 3) {
    this[0x3f] = (PlayerTurret)0x1;
    pAVar3 = operator_new(0xc0);
    uVar6 = 0x49c8;
    if (param_1 == 0x49c1) {
      uVar6 = 0x49c7;
    }
    if (param_1 == 0x49c0) {
      uVar6 = 0x49c6;
    }
    AEGeometry::AEGeometry(pAVar3,uVar6,(PaintCanvas *)*puVar8,false);
    AEGeometry::addChild((uint)param_3);
    pvVar5 = (void *)AEGeometry::~AEGeometry(pAVar3);
    operator_delete(pvVar5);
    AEGeometry::setScaling(extraout_s0_01);
  }
  AEGeometry::addChild(*(uint *)(this + 0x148));
  if (this[0x3f] == (PlayerTurret)0x0) {
    AEGeometry::addChild((uint)param_3);
  }
  this_00 = operator_new(0x68);
  Explosion::Explosion(this_00,0);
  *(Explosion **)(this + 0x13c) = this_00;
  Explosion::addFireStreaks(this_00);
  uVar2 = DAT_00167318;
  uVar1 = DAT_00167310;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x150) = 0;
  *(undefined8 *)(this + 300) = uVar1;
  *(undefined8 *)(this + 0x134) = uVar2;
  iVar4 = *piVar7 - local_3c;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  00167040: push {r4,r5,r6,r7,lr}
  00167042: add r7,sp,#0xc
  00167044: push {r8,r9,r10}
  00167048: vpush {d3,d4,d5,d6,d7,d8,d9,d10}
  0016704c: mov r4,r0
  0016704e: ldr r0,[0x00167334]
  00167050: vldr.32 s16,[r7,#0x10]
  00167054: mov r8,r3
  00167056: add r0,pc
  00167058: vldr.32 s20,[r7,#0xc]
  0016705c: vldr.32 s18,[r7,#0x8]
  00167060: mov r3,r2
  00167062: ldr.w r9,[r0,#0x0]
  00167066: movs r5,#0x0
  00167068: mov.w r2,#0xffffffff
  0016706c: mov r6,r1
  0016706e: ldr.w r0,[r9,#0x0]
  00167072: str r0,[sp,#0x24]
  00167074: mov r0,r4
  00167076: str r5,[sp,#0x10]
  00167078: vstr.32 s18,[sp,#0x4]
  0016707c: vstr.32 s20,[sp,#0x8]
  00167080: vstr.32 s16,[sp,#0xc]
  00167084: str.w r8,[sp,#0x0]
  00167088: blx 0x00075790
  0016708c: ldr r0,[0x00167338]
  0016708e: add r0,pc
  00167090: ldr r0,[r0,#0x0]
  00167092: adds r0,#0x8
  00167094: str r0,[r4,#0x0]
  00167096: movs r0,#0x1
  00167098: strb.w r0,[r4,#0x25]
  0016709c: strb.w r0,[r4,#0x3e]
  001670a0: movw r0,#0xc350
  001670a4: strd r5,r5,[r4,#0x140]
  001670a8: str.w r5,[r4,#0x128]
  001670ac: strd r5,r5,[r4,#0x154]
  001670b0: strd r5,r5,[r4,#0x15c]
  001670b4: str.w r0,[r4,#0x164]
  001670b8: movs r0,#0xc0
  001670ba: blx 0x0006eb24
  001670be: mov r5,r0
  001670c0: ldr r0,[0x0016733c]
  001670c2: add r0,pc
  001670c4: ldr.w r10,[r0,#0x0]
  001670c8: ldr.w r2,[r10,#0x0]
  001670cc: uxth r1,r6
  001670ce: mov r0,r5
  001670d0: movs r3,#0x0
  001670d2: blx 0x0007207c
  001670d6: movw r0,#0x381b
  001670da: cmp r6,r0
  001670dc: str.w r5,[r4,#0x140]
  001670e0: bne 0x0016711c
  001670e2: movs r0,#0xc0
  001670e4: blx 0x0006eb24
  001670e8: mov r5,r0
  001670ea: ldr.w r2,[r10,#0x0]
  001670ee: movw r1,#0x381c
  001670f2: movs r3,#0x0
  001670f4: blx 0x0007207c
  001670f8: str.w r5,[r4,#0x144]
  001670fc: mov r0,r5
  001670fe: movs r1,#0x2
  00167100: blx 0x00072100
  00167104: ldr.w r0,[r4,#0x144]
  00167108: ldr r1,[0x00167324]
  0016710a: ldr r2,[0x00167328]
  0016710c: str r2,[sp,#0x1c]
  0016710e: movs r2,#0x0
  00167110: str r2,[sp,#0x18]
  00167112: str r1,[sp,#0x20]
  00167114: add r1,sp,#0x18
  00167116: blx 0x00072148
  0016711a: b 0x0016719a
  0016711c: movw r0,#0x1a76
  00167120: cmp r6,r0
  00167122: beq 0x00167164
  00167124: movw r0,#0x1a74
  00167128: cmp r6,r0
  0016712a: bne 0x0016719a
  0016712c: movs r0,#0xc0
  0016712e: blx 0x0006eb24
  00167132: mov r5,r0
  00167134: ldr.w r2,[r10,#0x0]
  00167138: movw r1,#0x1a75
  0016713c: movs r3,#0x0
  0016713e: blx 0x0007207c
  00167142: str.w r5,[r4,#0x144]
  00167146: mov r0,r5
  00167148: movs r1,#0x2
  0016714a: blx 0x00072100
  0016714e: ldr.w r0,[r4,#0x144]
  00167152: ldr r1,[0x00167320]
  00167154: str r1,[sp,#0x1c]
  00167156: movs r1,#0x0
  00167158: str r1,[sp,#0x18]
  0016715a: str r1,[sp,#0x20]
  0016715c: add r1,sp,#0x18
  0016715e: blx 0x00072148
  00167162: b 0x0016719a
  00167164: movs r0,#0xc0
  00167166: blx 0x0006eb24
  0016716a: mov r5,r0
  0016716c: ldr.w r2,[r10,#0x0]
  00167170: movw r1,#0x1a77
  00167174: movs r3,#0x0
  00167176: blx 0x0007207c
  0016717a: str.w r5,[r4,#0x144]
  0016717e: mov r0,r5
  00167180: movs r1,#0x2
  00167182: blx 0x00072100
  00167186: ldr.w r0,[r4,#0x144]
  0016718a: ldr r1,[0x00167320]
  0016718c: str r1,[sp,#0x1c]
  0016718e: movs r1,#0x0
  00167190: str r1,[sp,#0x18]
  00167192: str r1,[sp,#0x20]
  00167194: add r1,sp,#0x18
  00167196: blx 0x00072148
  0016719a: movs r0,#0xc0
  0016719c: blx 0x0006eb24
  001671a0: ldr.w r1,[r10,#0x0]
  001671a4: mov r5,r0
  001671a6: blx 0x000720f4
  001671aa: str.w r5,[r4,#0x148]
  001671ae: ldr r1,[r5,#0xc]
  001671b0: ldr.w r0,[r10,#0x0]
  001671b4: blx 0x00072088
  001671b8: ldr r1,[0x0016732c]
  001671ba: str.w r1,[r0,#0xe0]
  001671be: vstr.32 s20,[sp,#0x1c]
  001671c2: vstr.32 s18,[sp,#0x18]
  001671c6: vstr.32 s16,[sp,#0x20]
  001671ca: ldr r0,[r4,#0x0]
  001671cc: ldr r2,[r0,#0x44]
  001671ce: add r1,sp,#0x18
  001671d0: mov r0,r4
  001671d2: blx r2
  001671d4: movw r0,#0x381b
  001671d8: cmp r6,r0
  001671da: bne 0x00167204
  001671dc: ldr.w r0,[r4,#0x140]
  001671e0: ldr r2,[0x00167330]
  001671e2: movs r1,#0x0
  001671e4: movs r3,#0x0
  001671e6: blx 0x00072160
  001671ea: ldr.w r0,[r4,#0x144]
  001671ee: ldr r2,[0x00167330]
  001671f0: movs r1,#0x0
  001671f2: movs r3,#0x0
  001671f4: blx 0x00072160
  001671f8: ldrd r1,r0,[r4,#0x144]
  001671fc: ldr r1,[r1,#0xc]
  001671fe: blx 0x000720e8
  00167202: b 0x00167278
  00167204: orr r0,r6,#0x2
  00167208: movw r1,#0x1a76
  0016720c: cmp r0,r1
  0016720e: bne 0x0016721c
  00167210: ldrd r1,r0,[r4,#0x144]
  00167214: ldr r1,[r1,#0xc]
  00167216: blx 0x000720e8
  0016721a: b 0x00167278
  0016721c: movw r0,#0x49c0
  00167220: subs r0,r6,r0
  00167222: cmp r0,#0x2
  00167224: bhi 0x00167278
  00167226: movs r0,#0x1
  00167228: strb.w r0,[r4,#0x3f]
  0016722c: movs r0,#0xc0
  0016722e: blx 0x0006eb24
  00167232: movw r1,#0x49c8
  00167236: mov r5,r0
  00167238: movw r0,#0x49c1
  0016723c: cmp r6,r0
  0016723e: it eq
  00167240: movw.eq r1,#0x49c7
  00167244: movw r0,#0x49c0
  00167248: ldr.w r2,[r10,#0x0]
  0016724c: cmp r6,r0
  0016724e: it eq
  00167250: movw.eq r1,#0x49c6
  00167254: mov r0,r5
  00167256: movs r3,#0x0
  00167258: blx 0x0007207c
  0016725c: ldr r1,[r5,#0xc]
  0016725e: mov r0,r8
  00167260: blx 0x000720e8
  00167264: mov r0,r5
  00167266: blx 0x00071fc8
  0016726a: blx 0x0006eb48
  0016726e: mov r0,r8
  00167270: mov.w r1,#0x3f000000
  00167274: blx 0x00076a20
  00167278: ldr.w r1,[r4,#0x140]
  0016727c: ldr.w r0,[r4,#0x148]
  00167280: ldr r1,[r1,#0xc]
  00167282: blx 0x000720e8
  00167286: ldrb.w r0,[r4,#0x3f]
  0016728a: cbnz r0,0x00167298
  0016728c: ldr.w r0,[r4,#0x148]
  00167290: ldr r1,[r0,#0xc]
  00167292: mov r0,r8
  00167294: blx 0x000720e8
  00167298: movs r0,#0x68
  0016729a: blx 0x0006eb24
  0016729e: mov r5,r0
  001672a0: movs r1,#0x0
  001672a2: blx 0x000727f0
  001672a6: str.w r5,[r4,#0x13c]
  001672aa: mov r0,r5
  001672ac: blx 0x0007579c
  001672b0: adr r0,[0x167310]
  001672b2: vld1.64 {d16,d17},[r0]
  001672b6: movs r0,#0x0
  001672b8: strd r0,r0,[r4,#0x14c]
  001672bc: add.w r0,r4,#0x12c
  001672c0: vst1.32 {d16,d17},[r0]
  001672c4: ldr r0,[sp,#0x24]
  001672c6: ldr.w r1,[r9,#0x0]
  001672ca: subs r0,r1,r0
  001672cc: itttt eq
  001672ce: mov.eq r0,r4
  001672d0: vpop.eq {d3,d4,d5,d6,d7,d8,d9,d10}
  001672d4: pop.eq.w {r8,r9,r10}
  001672d8: pop.eq {r4,r5,r6,r7,pc}
  001672da: blx 0x0006e824
```
