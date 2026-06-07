# PlayerCreature::PlayerCreature
elf 0x11cb5c body 178
Sig: undefined __thiscall PlayerCreature(PlayerCreature * this, int param_1, int param_2, Player * param_3, AEGeometry * param_4, float param_5, float param_6, float param_7)

## decompile
```c

/* PlayerCreature::PlayerCreature(int, int, Player*, AEGeometry*, float, float, float) */

PlayerCreature * __thiscall
PlayerCreature::PlayerCreature
          (PlayerCreature *this,int param_1,int param_2,Player *param_3,AEGeometry *param_4,
          float param_5,float param_6,float param_7)

{
  undefined4 uVar1;
  int iVar2;
  uint in_fpscr;
  float fVar3;
  float in_stack_00000004;
  float in_stack_00000008;
  
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,param_3,param_4,in_stack_00000004,param_6,
                     in_stack_00000008,SUB41(in_stack_00000004,0));
  *(int *)this = *(int *)(DAT_0012cc20 + 0x12cb9c) + 8;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x144));
  this[0x12a] = (PlayerCreature)0x0;
  *(int *)(this + 0x140) = param_2;
  *(undefined4 *)(this + 300) = 0x3f800000;
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined2 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 0x138) = *(undefined4 *)(this + 0x134);
  uVar1 = Player::getHitpoints();
  fVar3 = (float)VectorSignedToFloat(*(undefined4 *)(DAT_0012cc24 + 0x12cbd4 + param_1 * 4),
                                     (byte)(in_fpscr >> 0x16) & 3);
  iVar2 = (int)((fVar3 / 10.0) * DAT_0012cc1c) + 8000;
  *(int *)(this + 0x134) = iVar2;
  *(int *)(this + 0x138) = iVar2;
  *(undefined4 *)(this + 0x13c) = uVar1;
  reset(this);
  return this;
}

```

## target disasm
```
  0012cb5c: push {r4,r5,r6,r7,lr}
  0012cb5e: add r7,sp,#0xc
  0012cb60: push.w r8
  0012cb64: sub sp,#0x18
  0012cb66: mov r4,r0
  0012cb68: vldr.32 s0,[r7,#0xc]
  0012cb6c: ldr r0,[r7,#0x8]
  0012cb6e: mov r8,r2
  0012cb70: vldr.32 s2,[r7,#0x10]
  0012cb74: movs r6,#0x0
  0012cb76: vldr.32 s4,[r7,#0x14]
  0012cb7a: mov.w r2,#0xffffffff
  0012cb7e: str r0,[sp,#0x0]
  0012cb80: mov r0,r4
  0012cb82: mov r5,r1
  0012cb84: str r6,[sp,#0x10]
  0012cb86: vstr.32 s0,[sp,#0x4]
  0012cb8a: vstr.32 s2,[sp,#0x8]
  0012cb8e: vstr.32 s4,[sp,#0xc]
  0012cb92: blx 0x00075790
  0012cb96: ldr r0,[0x0012cc20]
  0012cb98: add r0,pc
  0012cb9a: ldr r0,[r0,#0x0]
  0012cb9c: adds r0,#0x8
  0012cb9e: str r0,[r4,#0x0]
  0012cba0: add.w r0,r4,#0x144
  0012cba4: blx 0x0006f118
  0012cba8: ldr r0,[r4,#0x4]
  0012cbaa: mov.w r1,#0x3f800000
  0012cbae: ldr.w r2,[r4,#0x134]
  0012cbb2: strb.w r6,[r4,#0x12a]
  0012cbb6: str.w r8,[r4,#0x140]
  0012cbba: strd r1,r6,[r4,#0x12c]
  0012cbbe: strh.w r6,[r4,#0x128]
  0012cbc2: str.w r2,[r4,#0x138]
  0012cbc6: blx 0x000724f0
  0012cbca: ldr r1,[0x0012cc24]
  0012cbcc: vmov.f32 s2,0x41200000
  0012cbd0: add r1,pc
  0012cbd2: add.w r1,r1,r5, lsl #0x2
  0012cbd6: vldr.32 s0,[r1]
  0012cbda: vcvt.f32.s32 s0,s0
  0012cbde: vdiv.f32 s0,s0,s2
  0012cbe2: vldr.32 s2,[pc,#0x38]
  0012cbe6: vmul.f32 s0,s0,s2
  0012cbea: vcvt.s32.f32 s0,s0
  0012cbee: vmov r1,s0
  0012cbf2: add.w r1,r1,#0x1f40
  0012cbf6: strd r1,r1,[r4,#0x134]
  0012cbfa: str.w r0,[r4,#0x13c]
  0012cbfe: mov r0,r4
  0012cc00: blx 0x00076078
  0012cc04: mov r0,r4
  0012cc06: add sp,#0x18
  0012cc08: pop.w r8
  0012cc0c: pop {r4,r5,r6,r7,pc}
```
