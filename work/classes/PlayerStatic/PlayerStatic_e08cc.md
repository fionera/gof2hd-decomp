# PlayerStatic::PlayerStatic
elf 0xe08cc body 140
Sig: undefined __thiscall PlayerStatic(PlayerStatic * this, int param_1, AEGeometry * param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* PlayerStatic::PlayerStatic(int, AEGeometry*, float, float, float) */

PlayerStatic * __thiscall
PlayerStatic::PlayerStatic
          (PlayerStatic *this,int param_1,AEGeometry *param_2,float param_3,float param_4,
          float param_5)

{
  Player *this_00;
  float in_r3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float in_stack_00000000;
  float in_stack_00000004;
  
  this_00 = operator_new(0x114);
  Player::Player(this_00,2000,0,0,0,0);
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,this_00,param_2,extraout_s0,extraout_s1,extraout_s2
                     ,SUB41(in_r3,0));
  *(int *)this = *(int *)(DAT_000f0968 + 0xf093a) + 8;
  *(int *)(this + 0x124) = (int)in_r3;
  *(int *)(this + 0x128) = (int)in_stack_00000000;
  *(int *)(this + 300) = (int)in_stack_00000004;
  return this;
}

```

## target disasm
```
  000f08cc: push {r4,r5,r6,r7,lr}
  000f08ce: add r7,sp,#0xc
  000f08d0: push {r8,r9,r10}
  000f08d4: vpush {d5,d6,d7,d8,d9,d10}
  000f08d8: mov r4,r0
  000f08da: mov.w r0,#0x114
  000f08de: mov r10,r3
  000f08e0: mov r9,r2
  000f08e2: mov r8,r1
  000f08e4: blx 0x0006eb24
  000f08e8: mov r6,r0
  000f08ea: movs r5,#0x0
  000f08ec: mov.w r1,#0x7d0
  000f08f0: movs r2,#0x0
  000f08f2: movs r3,#0x0
  000f08f4: strd r5,r5,[sp,#0x0]
  000f08f8: blx 0x00073d80
  000f08fc: vmov s16,r10
  000f0900: vldr.32 s18,[r7,#0xc]
  000f0904: vldr.32 s20,[r7,#0x8]
  000f0908: mov r0,r4
  000f090a: mov r1,r8
  000f090c: mov.w r2,#0xffffffff
  000f0910: mov r3,r6
  000f0912: str r5,[sp,#0x10]
  000f0914: vstr.32 s16,[sp,#0x4]
  000f0918: vstr.32 s20,[sp,#0x8]
  000f091c: vstr.32 s18,[sp,#0xc]
  000f0920: str.w r9,[sp,#0x0]
  000f0924: blx 0x00075790
  000f0928: ldr r0,[0x000f0968]
  000f092a: vcvt.s32.f32 s0,s16
  000f092e: vcvt.s32.f32 s2,s20
  000f0932: vcvt.s32.f32 s4,s18
  000f0936: add r0,pc
  000f0938: ldr r0,[r0,#0x0]
  000f093a: adds r0,#0x8
  000f093c: str r0,[r4,#0x0]
  000f093e: vstr.32 s0,[r4,#0x124]
  000f0942: mov r0,r4
  000f0944: vstr.32 s2,[r4,#0x128]
  000f0948: vstr.32 s4,[r4,#0x12c]
  000f094c: add sp,#0x18
  000f094e: vpop {d8,d9,d10}
  000f0952: pop.w {r8,r9,r10}
  000f0956: pop {r4,r5,r6,r7,pc}
```
