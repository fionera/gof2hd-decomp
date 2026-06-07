# PlayerGasCloud::PlayerGasCloud
elf 0x1763cc body 292
Sig: undefined __thiscall PlayerGasCloud(PlayerGasCloud * this, int param_1, ParticleSystemManager * param_2, AEGeometry * param_3, Vector * param_4)

## decompile
```c

/* PlayerGasCloud::PlayerGasCloud(int, ParticleSystemManager*, AEGeometry*,
   AbyssEngine::AEMath::Vector const&) */

PlayerGasCloud * __thiscall
PlayerGasCloud::PlayerGasCloud
          (PlayerGasCloud *this,int param_1,ParticleSystemManager *param_2,AEGeometry *param_3,
          Vector *param_4)

{
  Player *this_00;
  AEGeometry *this_01;
  int iVar1;
  int iVar2;
  float extraout_s1;
  
  this_00 = operator_new(0x114);
  Player::Player(this_00,0,DAT_00186514,0,0,0);
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,this_00,param_3,*(float *)param_4,extraout_s1,
                     *(float *)(param_4 + 4),SUB41(*(float *)param_4,0));
  *(int *)this = *(int *)(DAT_00186518 + 0x18642a) + 8;
  *(undefined4 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 300) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  Player::setKIPlayer(*(Player **)(this + 4),(KIPlayer *)this);
  Player::setMaxHitpoints(*(Player **)(this + 4),1);
  (**(code **)(*(int *)this + 0x44))(this,param_4);
  *(undefined4 *)(this + 0x158) = 0;
  iVar1 = 0x4a35;
  *(undefined4 *)(this + 0x78) = 0;
  this[0x25] = (PlayerGasCloud)0x0;
  if (param_1 - 0xc9U < 4) {
    iVar1 = param_1 + 0x496c;
  }
  iVar2 = 0x4a39;
  if (param_1 - 0xc9U < 4) {
    iVar2 = param_1 + 0x4970;
  }
  *(int *)(this + 0x160) = param_1;
  *(int *)(this + 0x164) = iVar2;
  *(int *)(this + 0x168) = iVar1;
  *(undefined4 *)(this + 0x145) = 0;
  *(undefined4 *)(this + 0x149) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x14d) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x151) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x140) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x144) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x128),param_4);
  this_01 = operator_new(0xc0);
  AEGeometry::AEGeometry
            (this_01,*(ushort *)(this + 0x168),
             (PaintCanvas *)**(undefined4 **)(DAT_0018651c + 0x1864b8),false);
  *(AEGeometry **)(this + 0x134) = this_01;
  AEGeometry::setPosition(this_01,param_4);
  this[0x4c] = (PlayerGasCloud)0x1;
  this[0x44] = (PlayerGasCloud)0x1;
  *(undefined4 *)(this + 0x88) = 0;
  this[0xf5] = (PlayerGasCloud)0x1;
  this[0x15c] = (PlayerGasCloud)0x0;
  return this;
}

```

## target disasm
```
  001863cc: push {r4,r5,r6,r7,lr}
  001863ce: add r7,sp,#0xc
  001863d0: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  001863d4: mov r4,r0
  001863d6: mov.w r0,#0x114
  001863da: mov r9,r3
  001863dc: mov r10,r1
  001863de: blx 0x0006eb24
  001863e2: mov r5,r0
  001863e4: ldr r2,[0x00186514]
  001863e6: movs r6,#0x0
  001863e8: movs r1,#0x0
  001863ea: movs r3,#0x0
  001863ec: strd r6,r6,[sp,#0x0]
  001863f0: blx 0x00073d80
  001863f4: ldr.w r8,[r7,#0x8]
  001863f8: mov r0,r4
  001863fa: mov r1,r10
  001863fc: mov.w r2,#0xffffffff
  00186400: mov r3,r5
  00186402: vldr.32 s0,[r8]
  00186406: vldr.32 s2,[r8,#0x4]
  0018640a: vldr.32 s4,[r8,#0x8]
  0018640e: str r6,[sp,#0x10]
  00186410: vstr.32 s0,[sp,#0x4]
  00186414: vstr.32 s2,[sp,#0x8]
  00186418: vstr.32 s4,[sp,#0xc]
  0018641c: str.w r9,[sp,#0x0]
  00186420: blx 0x00075790
  00186424: ldr r0,[0x00186518]
  00186426: add r0,pc
  00186428: ldr r0,[r0,#0x0]
  0018642a: adds r0,#0x8
  0018642c: str r0,[r4,#0x0]
  0018642e: strd r6,r6,[r4,#0x128]
  00186432: str.w r6,[r4,#0x130]
  00186436: ldr r0,[r4,#0x4]
  00186438: mov r1,r4
  0018643a: blx 0x00073288
  0018643e: ldr r0,[r4,#0x4]
  00186440: movs r1,#0x1
  00186442: blx 0x00073f78
  00186446: ldr r0,[r4,#0x0]
  00186448: ldr r2,[r0,#0x44]
  0018644a: mov r0,r4
  0018644c: mov r1,r8
  0018644e: blx r2
  00186450: movs r1,#0x0
  00186452: sub.w r2,r10,#0xc9
  00186456: str.w r1,[r4,#0x158]
  0018645a: movw r3,#0x4a35
  0018645e: str r1,[r4,#0x78]
  00186460: vmov.i32 q8,#0x0
  00186464: strb.w r1,[r4,#0x25]
  00186468: movw r1,#0x496c
  0018646c: cmp r2,#0x4
  0018646e: it cc
  00186470: add.cc.w r3,r10,r1
  00186474: movw r1,#0x4970
  00186478: movw r6,#0x4a39
  0018647c: cmp r2,#0x4
  0018647e: it cc
  00186480: add.cc.w r6,r10,r1
  00186484: addw r1,r4,#0x145
  00186488: strd r10,r6,[r4,#0x160]
  0018648c: str.w r3,[r4,#0x168]
  00186490: add.w r0,r4,#0x128
  00186494: vst1.8 {d16,d17},[r1]
  00186498: add.w r1,r4,#0x138
  0018649c: vst1.32 {d16,d17},[r1]
  001864a0: mov r1,r8
  001864a2: blx 0x0006eb3c
  001864a6: movs r0,#0xc0
  001864a8: blx 0x0006eb24
  001864ac: mov r5,r0
  001864ae: ldr r0,[0x0018651c]
  001864b0: ldrh.w r1,[r4,#0x168]
  001864b4: add r0,pc
  001864b6: ldr r0,[r0,#0x0]
  001864b8: ldr r2,[r0,#0x0]
  001864ba: mov r0,r5
  001864bc: movs r3,#0x0
  001864be: blx 0x0007207c
  001864c2: str.w r5,[r4,#0x134]
  001864c6: mov r0,r5
  001864c8: mov r1,r8
  001864ca: blx 0x00072148
  001864ce: movs r0,#0x1
  001864d0: movs r1,#0x0
  001864d2: strb.w r0,[r4,#0x4c]
  001864d6: strb.w r0,[r4,#0x44]
  001864da: str.w r1,[r4,#0x88]
  001864de: strb.w r0,[r4,#0xf5]
  001864e2: mov r0,r4
  001864e4: strb.w r1,[r4,#0x15c]
  001864e8: add sp,#0x18
  001864ea: pop.w {r8,r9,r10}
  001864ee: pop {r4,r5,r6,r7,pc}
```
