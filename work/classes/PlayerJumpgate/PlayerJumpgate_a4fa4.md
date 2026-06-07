# PlayerJumpgate::PlayerJumpgate
elf 0xa4fa4 body 222
Sig: undefined __thiscall PlayerJumpgate(PlayerJumpgate * this, int param_1, AEGeometry * param_2, float param_3, float param_4, float param_5, bool param_6)

## decompile
```c

/* PlayerJumpgate::PlayerJumpgate(int, AEGeometry*, float, float, float, bool) */

PlayerJumpgate * __thiscall
PlayerJumpgate::PlayerJumpgate
          (PlayerJumpgate *this,int param_1,AEGeometry *param_2,float param_3,float param_4,
          float param_5,bool param_6)

{
  Array<BoundingVolume*> *this_00;
  int iVar1;
  BoundingSphere *this_01;
  int iVar2;
  uint in_fpscr;
  float extraout_s0;
  float fVar3;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar4;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar5;
  float extraout_s3;
  float extraout_s4;
  float extraout_s5;
  float extraout_s6;
  int in_stack_00000008;
  
  PlayerStaticFar::PlayerStaticFar((PlayerStaticFar *)this,param_1,param_2,param_3,param_4,param_5);
  *(int *)this = *(int *)(DAT_000b50a4 + 0xb4fd0) + 8;
  KIPlayer::setVisible((KIPlayer *)this,SUB41(in_stack_00000008,0));
  fVar3 = extraout_s0;
  fVar4 = extraout_s1;
  fVar5 = extraout_s2;
  if (in_stack_00000008 != 0) {
    this_00 = operator_new(0xc);
    Array<BoundingVolume*>::Array(this_00);
    *(Array<BoundingVolume*> **)(this + 0x130) = this_00;
    ArraySetLength<BoundingVolume*>(1,(Array *)this_00);
    iVar1 = Status::inAlienOrbit();
    if (iVar1 == 0) {
      Status::getSystem();
      iVar1 = SolarSystem::getRace();
      iVar2 = 0x1d4c;
      if (iVar1 == 1) {
        iVar2 = 0x2bf2;
      }
    }
    else {
      iVar2 = 0x1d4c;
    }
    Player::setRadius(*(Player **)(this + 4),iVar2);
    this_01 = operator_new(0x48);
    fVar3 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    BoundingSphere::BoundingSphere
              (this_01,fVar3,extraout_s1_00,extraout_s2_00,extraout_s3,extraout_s4,extraout_s5,
               extraout_s6);
    **(undefined4 **)(*(int *)(this + 0x130) + 4) = this_01;
    fVar3 = extraout_s0_00;
    fVar4 = extraout_s1_01;
    fVar5 = extraout_s2_01;
  }
  *(undefined4 *)(this + 0x144) = 0xffffffff;
  this[0x140] = (PlayerJumpgate)0x0;
  AEGeometry::setRotation(*(AEGeometry **)(this + 8),fVar3,fVar4,fVar5);
  return this;
}

```

## target disasm
```
  000b4fa4: push {r4,r5,r6,r7,lr}
  000b4fa6: add r7,sp,#0xc
  000b4fa8: push.w r8
  000b4fac: vpush {d6,d7,d8,d9}
  000b4fb0: vldr.32 s16,[r7,#0x8]
  000b4fb4: mov r8,r3
  000b4fb6: vldr.32 s18,[r7,#0xc]
  000b4fba: mov r4,r0
  000b4fbc: vstr.32 s16,[sp]
  000b4fc0: vstr.32 s18,[sp,#0x4]
  000b4fc4: blx 0x000730a8
  000b4fc8: ldr r0,[0x000b50a4]
  000b4fca: ldr r5,[r7,#0x10]
  000b4fcc: add r0,pc
  000b4fce: ldr r0,[r0,#0x0]
  000b4fd0: adds r0,#0x8
  000b4fd2: str r0,[r4,#0x0]
  000b4fd4: mov r0,r4
  000b4fd6: mov r1,r5
  000b4fd8: blx 0x000730b4
  000b4fdc: cbz r5,0x000b505c
  000b4fde: movs r0,#0xc
  000b4fe0: blx 0x0006eb24
  000b4fe4: mov r5,r0
  000b4fe6: blx 0x000730c0
  000b4fea: str.w r5,[r4,#0x130]
  000b4fee: movs r0,#0x1
  000b4ff0: mov r1,r5
  000b4ff2: blx 0x000730cc
  000b4ff6: ldr r0,[0x000b50a8]
  000b4ff8: add r0,pc
  000b4ffa: ldr r5,[r0,#0x0]
  000b4ffc: ldr r0,[r5,#0x0]
  000b4ffe: blx 0x000723d0
  000b5002: cbz r0,0x000b500a
  000b5004: movw r6,#0x1d4c
  000b5008: b 0x000b5020
  000b500a: ldr r0,[r5,#0x0]
  000b500c: blx 0x00071a10
  000b5010: blx 0x00071950
  000b5014: movw r6,#0x1d4c
  000b5018: cmp r0,#0x1
  000b501a: it eq
  000b501c: movw.eq r6,#0x2bf2
  000b5020: ldr r0,[r4,#0x4]
  000b5022: mov r1,r6
  000b5024: blx 0x000730d8
  000b5028: movs r0,#0x48
  000b502a: blx 0x0006eb24
  000b502e: vmov s0,r6
  000b5032: mov r5,r0
  000b5034: vmov r2,s16
  000b5038: vmov r3,s18
  000b503c: vcvt.f32.s32 s0,s0
  000b5040: movs r0,#0x0
  000b5042: mov r1,r8
  000b5044: strd r0,r0,[sp,#0x0]
  000b5048: str r0,[sp,#0x8]
  000b504a: mov r0,r5
  000b504c: vstr.32 s0,[sp,#0xc]
  000b5050: blx 0x000730e4
  000b5054: ldr.w r0,[r4,#0x130]
  000b5058: ldr r0,[r0,#0x4]
  000b505a: str r5,[r0,#0x0]
  000b505c: mov.w r0,#0xffffffff
  000b5060: movs r1,#0x0
  000b5062: str.w r0,[r4,#0x144]
  000b5066: ldr r0,[r4,#0x8]
  000b5068: ldr r2,[0x000b50a0]
  000b506a: strb.w r1,[r4,#0x140]
  000b506e: movs r1,#0x0
  000b5070: movs r3,#0x0
  000b5072: blx 0x00073054
  000b5076: mov r0,r4
  000b5078: vpop {d6,d7,d8,d9}
  000b507c: pop.w r8
  000b5080: pop {r4,r5,r6,r7,pc}
```
