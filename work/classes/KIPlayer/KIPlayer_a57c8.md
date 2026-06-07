# KIPlayer::KIPlayer
elf 0xa57c8 body 496
Sig: undefined __thiscall KIPlayer(KIPlayer * this, int param_1, int param_2, Player * param_3, AEGeometry * param_4, float param_5, float param_6, float param_7, bool param_8)

## decompile
```c

/* KIPlayer::KIPlayer(int, int, Player*, AEGeometry*, float, float, float, bool) */

void __thiscall
KIPlayer::KIPlayer(KIPlayer *this,int param_1,int param_2,Player *param_3,AEGeometry *param_4,
                  float param_5,float param_6,float param_7,bool param_8)

{
  undefined4 uVar1;
  undefined4 uVar2;
  AEGeometry *this_00;
  Matrix *pMVar3;
  int iVar4;
  int extraout_r1;
  int *piVar5;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  undefined4 in_s14;
  undefined3 in_stack_00000005;
  undefined4 in_stack_00000008;
  undefined4 in_stack_0000000c;
  int in_stack_00000010;
  String aSStack_50 [8];
  undefined4 uStack_48;
  int local_44;
  
  piVar5 = *(int **)(DAT_000b59e4 + 0xb57e0);
  _uStack_48 = CONCAT44(*piVar5,in_s14);
  *(int *)this = *(int *)(DAT_000b59e8 + 0xb57e6) + 8;
  AbyssEngine::String::String((String *)(this + 0x18));
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x98) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x9c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x10c) = 0;
  *(undefined4 *)(this + 0x110) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x114) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x118) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xa0) = 0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0x11c) = 0;
  *(undefined4 *)(this + 0x120) = 0;
  *(Player **)(this + 4) = param_3;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  iVar4 = extraout_r1;
  if (param_4 != (AEGeometry *)0x0) {
    iVar4 = in_stack_00000010;
  }
  if (param_4 != (AEGeometry *)0x0 && iVar4 != 0) {
    *(AEGeometry **)(this + 0xc) = param_4;
    this_00 = operator_new(0xc0);
    AEGeometry::AEGeometry(this_00,(PaintCanvas *)**(undefined4 **)(DAT_000b59ec + 0xb5862));
    *(AEGeometry **)(this + 8) = this_00;
    AEGeometry::addChild((uint)this_00);
    *(undefined4 *)(*(int *)(this + 0xc) + 0x24) = *(undefined4 *)(*(int *)(this + 8) + 0xc);
  }
  else {
    *(AEGeometry **)(this + 8) = param_4;
    *(undefined4 *)(this + 0xc) = 0;
  }
  iVar4 = DAT_000b59f0;
  *(undefined4 *)(this + 0x50) = 0;
  *(int *)(this + 0x28) = param_2;
  this[0x72] = (KIPlayer)0x0;
  this[0x25] = (KIPlayer)0x0;
  this[0x75] = (KIPlayer)0x0;
  *(undefined2 *)(this + 0x42) = 0;
  this[0x44] = (KIPlayer)0x0;
  *(undefined4 *)(this + 0x48) = 0xffffffff;
  this[0x8c] = (KIPlayer)0x1;
  *(undefined4 *)(this + 0x3c) = 0;
  this[0x4c] = (KIPlayer)0x0;
  this[0xd0] = (KIPlayer)0x0;
  *(undefined2 *)(this + 0x68) = 0;
  this[0x6a] = (KIPlayer)0x0;
  this[0xdc] = (KIPlayer)0x0;
  this[0xec] = (KIPlayer)0x0;
  this[0xf4] = (KIPlayer)0x0;
  AbyssEngine::String::String(aSStack_50,(char *)(iVar4 + 0xb588c),false);
  AbyssEngine::String::operator=((String *)(this + 0x18),aSStack_50);
  AbyssEngine::String::~String(aSStack_50);
  this[0x24] = (KIPlayer)0x0;
  this[0xf5] = (KIPlayer)0x1;
  *(undefined4 *)(this + 0xe8) = 0;
  *(undefined2 *)(this + 0x70) = 0;
  *(undefined2 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0xd8) = 0;
  *(undefined4 *)(this + 0x80) = 0xffffffff;
  *(undefined4 *)(this + 0x84) = 0xffffffff;
  *(undefined4 *)(this + 0x104) = 0;
  Player::setKIPlayer(*(Player **)(this + 4),this);
  uVar2 = DAT_000b59e0;
  uVar1 = DAT_000b59dc;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined2 *)(this + 0xb1) = 0x100;
  this[0xbc] = (KIPlayer)0x0;
  *(undefined4 *)(this + 0xc0) = 0xff;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0xa8) = uVar2;
  *(int *)(this + 0xac) = param_1;
  *(undefined4 *)(this + 100) = uVar1;
  if (param_4 != (AEGeometry *)0x0) {
    AEGeometry::setPosition(extraout_s0,extraout_s1,extraout_s2);
    pMVar3 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar3);
    if (*(int *)(this + 0xc) != 0) {
      pMVar3 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(*(int *)(this + 4) + 4),pMVar3);
    }
  }
  *(undefined4 *)(this + 0x58) = _param_8;
  *(undefined4 *)(this + 0x5c) = in_stack_00000008;
  *(undefined4 *)(this + 0x60) = in_stack_0000000c;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined2 *)(this + 0x73) = 0;
  this[0x76] = (KIPlayer)0x0;
  *(undefined4 *)(this + 0xf8) = 0xffffffff;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined2 *)(this + 0x100) = 0x100;
  iVar4 = *piVar5 - local_44;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  000b57c8: push {r4,r5,r6,r7,lr}
  000b57ca: add r7,sp,#0xc
  000b57cc: push {r7,r8,r9,r10,r11}
  000b57d0: vpush {d6,d7,d8,d9,d10}
  000b57d4: mov r5,r0
  000b57d6: ldr r0,[0x000b59e4]
  000b57d8: mov r8,r1
  000b57da: ldr r1,[0x000b59e8]
  000b57dc: add r0,pc
  000b57de: add.w r9,r5,#0x18
  000b57e2: add r1,pc
  000b57e4: mov r6,r3
  000b57e6: ldr.w r11,[r0,#0x0]
  000b57ea: mov r4,r2
  000b57ec: ldr r0,[r1,#0x0]
  000b57ee: ldr.w r1,[r11,#0x0]
  000b57f2: adds r0,#0x8
  000b57f4: str r1,[sp,#0xc]
  000b57f6: str r0,[r5,#0x0]
  000b57f8: mov r0,r9
  000b57fa: blx 0x0006efbc
  000b57fe: vmov.i32 q8,#0x0
  000b5802: add.w r0,r5,#0x90
  000b5806: ldr.w r10,[r7,#0x8]
  000b580a: vst1.32 {d16,d17},[r0]
  000b580e: add.w r0,r5,#0x10c
  000b5812: cmp.w r10,#0x0
  000b5816: vst1.32 {d16,d17},[r0]
  000b581a: mov.w r0,#0x0
  000b581e: strd r0,r0,[r5,#0x2c]
  000b5822: str r0,[r5,#0x34]
  000b5824: str r0,[r5,#0x54]
  000b5826: str r0,[r5,#0x6c]
  000b5828: str.w r0,[r5,#0xb4]
  000b582c: strd r0,r0,[r5,#0xa0]
  000b5830: strd r0,r0,[r5,#0x11c]
  000b5834: strd r6,r0,[r5,#0x4]
  000b5838: str r0,[r5,#0xc]
  000b583a: strd r0,r0,[r5,#0xc4]
  000b583e: str.w r0,[r5,#0xcc]
  000b5842: itt ne
  000b5844: ldr.ne r1,[r7,#0x18]
  000b5846: cmp.ne r1,#0x0
  000b5848: bne 0x000b5850
  000b584a: strd r10,r0,[r5,#0x8]
  000b584e: b 0x000b587e
  000b5850: str.w r10,[r5,#0xc]
  000b5854: movs r0,#0xc0
  000b5856: blx 0x0006eb24
  000b585a: mov r6,r0
  000b585c: ldr r0,[0x000b59ec]
  000b585e: add r0,pc
  000b5860: ldr r0,[r0,#0x0]
  000b5862: ldr r1,[r0,#0x0]
  000b5864: mov r0,r6
  000b5866: blx 0x000720f4
  000b586a: ldr r0,[r5,#0xc]
  000b586c: str r6,[r5,#0x8]
  000b586e: ldr r1,[r0,#0xc]
  000b5870: mov r0,r6
  000b5872: blx 0x000720e8
  000b5876: ldrd r0,r1,[r5,#0x8]
  000b587a: ldr r0,[r0,#0xc]
  000b587c: str r0,[r1,#0x24]
  000b587e: ldr r1,[0x000b59f0]
  000b5880: movs r0,#0x0
  000b5882: mov.w r2,#0xffffffff
  000b5886: str r0,[r5,#0x50]
  000b5888: add r1,pc
  000b588a: str r4,[r5,#0x28]
  000b588c: strb.w r0,[r5,#0x72]
  000b5890: strb.w r0,[r5,#0x25]
  000b5894: strb.w r0,[r5,#0x75]
  000b5898: strh.w r0,[r5,#0x42]
  000b589c: strb.w r0,[r5,#0x44]
  000b58a0: str r2,[r5,#0x48]
  000b58a2: movs r2,#0x1
  000b58a4: strb.w r2,[r5,#0x8c]
  000b58a8: str r0,[r5,#0x3c]
  000b58aa: strb.w r0,[r5,#0x4c]
  000b58ae: strb.w r0,[r5,#0xd0]
  000b58b2: strh.w r0,[r5,#0x68]
  000b58b6: strb.w r0,[r5,#0x6a]
  000b58ba: strb.w r0,[r5,#0xdc]
  000b58be: strb.w r0,[r5,#0xec]
  000b58c2: strb.w r0,[r5,#0xf4]
  000b58c6: mov r0,sp
  000b58c8: movs r2,#0x0
  000b58ca: blx 0x0006ee18
  000b58ce: mov r1,sp
  000b58d0: mov r0,r9
  000b58d2: blx 0x0006f2b0
  000b58d6: mov r0,sp
  000b58d8: blx 0x0006ee30
  000b58dc: movs r6,#0x0
  000b58de: movs r0,#0x1
  000b58e0: strb.w r6,[r5,#0x24]
  000b58e4: mov.w r1,#0xffffffff
  000b58e8: strb.w r0,[r5,#0xf5]
  000b58ec: str.w r6,[r5,#0xe8]
  000b58f0: strh.w r6,[r5,#0x70]
  000b58f4: strh.w r6,[r5,#0x40]
  000b58f8: str.w r6,[r5,#0xd8]
  000b58fc: ldr r0,[r5,#0x4]
  000b58fe: strd r1,r1,[r5,#0x80]
  000b5902: str.w r6,[r5,#0x104]
  000b5906: mov r1,r5
  000b5908: blx 0x00073288
  000b590c: vldr.32 s16,[r7,#0x14]
  000b5910: mov.w r4,#0x100
  000b5914: vldr.32 s18,[r7,#0x10]
  000b5918: movs r2,#0xff
  000b591a: vldr.32 s20,[r7,#0xc]
  000b591e: cmp.w r10,#0x0
  000b5922: ldr r0,[0x000b59dc]
  000b5924: ldr r1,[0x000b59e0]
  000b5926: str.w r6,[r5,#0x88]
  000b592a: str r6,[r5,#0x10]
  000b592c: strh.w r4,[r5,#0xb1]
  000b5930: strb.w r6,[r5,#0xbc]
  000b5934: str.w r2,[r5,#0xc0]
  000b5938: str r6,[r5,#0x78]
  000b593a: strd r1,r8,[r5,#0xa8]
  000b593e: str r0,[r5,#0x64]
  000b5940: beq 0x000b5976
  000b5942: vmov r1,s20
  000b5946: vmov r2,s18
  000b594a: vmov r3,s16
  000b594e: mov r0,r10
  000b5950: blx 0x00073048
  000b5954: mov r0,r10
  000b5956: blx 0x000721cc
  000b595a: mov r1,r0
  000b595c: ldr r0,[r5,#0x4]
  000b595e: adds r0,#0x4
  000b5960: blx 0x0006f148
  000b5964: ldr r0,[r5,#0xc]
  000b5966: cbz r0,0x000b5976
  000b5968: blx 0x000721cc
  000b596c: mov r1,r0
  000b596e: ldr r0,[r5,#0x4]
  000b5970: adds r0,#0x4
  000b5972: blx 0x0006f4a8
  000b5976: movs r0,#0x0
  000b5978: mov.w r1,#0xffffffff
  000b597c: vstr.32 s20,[r5,#0x58]
  000b5980: vstr.32 s18,[r5,#0x5c]
  000b5984: vstr.32 s16,[r5,#0x60]
  000b5988: str r0,[r5,#0x14]
  000b598a: strh.w r0,[r5,#0x73]
  000b598e: strb.w r0,[r5,#0x76]
  000b5992: strd r1,r0,[r5,#0xf8]
  000b5996: strh.w r4,[r5,#0x100]
  000b599a: ldr r0,[sp,#0xc]
  000b599c: ldr.w r1,[r11,#0x0]
  000b59a0: subs r0,r1,r0
  000b59a2: itttt eq
  000b59a4: mov.eq r0,r5
  000b59a6: vpop.eq {d6,d7,d8,d9,d10}
  000b59aa: add.eq sp,#0x4
  000b59ac: pop.eq.w {r8,r9,r10,r11}
  000b59b0: it eq
  000b59b2: pop.eq {r4,r5,r6,r7,pc}
  000b59b4: blx 0x0006e824
```
