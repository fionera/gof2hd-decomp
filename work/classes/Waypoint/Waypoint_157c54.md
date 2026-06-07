# Waypoint::Waypoint
elf 0x157c54 body 180
Sig: undefined __thiscall Waypoint(Waypoint * this, int param_1, int param_2, int param_3, Route * param_4)

## decompile
```c

/* Waypoint::Waypoint(int, int, int, Route*) */

Waypoint * __thiscall
Waypoint::Waypoint(Waypoint *this,int param_1,int param_2,int param_3,Route *param_4)

{
  int iVar1;
  Player *this_00;
  uint in_fpscr;
  float extraout_s1;
  float extraout_s2;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  this_00 = operator_new(0x114);
  Player::Player(this_00,2000,0,0,0,0);
  uVar2 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
  uVar3 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  uVar4 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  KIPlayer::KIPlayer((KIPlayer *)this,0,-1,this_00,(AEGeometry *)0x0,(float)param_1,extraout_s1,
                     extraout_s2,SUB41(uVar4,0));
  iVar1 = DAT_00167d20;
  *(Route **)(this + 0x134) = param_4;
  *(int *)this = *(int *)(iVar1 + 0x167cc8) + 8;
  Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
  *(int *)(this + 0x124) = param_1;
  *(int *)(this + 0x128) = param_2;
  *(int *)(this + 300) = param_3;
  *(undefined4 *)(this + 0x58) = uVar4;
  *(undefined4 *)(this + 0x5c) = uVar3;
  *(undefined4 *)(this + 0x60) = uVar2;
  *(undefined2 *)(this + 0x130) = 0;
  this[0x72] = (Waypoint)0x1;
  this[0x4c] = (Waypoint)0x0;
  return this;
}

```

## target disasm
```
  00167c54: push {r4,r5,r6,r7,lr}
  00167c56: add r7,sp,#0xc
  00167c58: push {r7,r8,r9,r10,r11}
  00167c5c: vpush {d5,d6,d7,d8,d9,d10}
  00167c60: mov r4,r0
  00167c62: mov.w r0,#0x114
  00167c66: mov r8,r3
  00167c68: mov r9,r2
  00167c6a: mov r10,r1
  00167c6c: blx 0x0006eb24
  00167c70: mov r6,r0
  00167c72: movs r5,#0x0
  00167c74: mov.w r1,#0x7d0
  00167c78: movs r2,#0x0
  00167c7a: movs r3,#0x0
  00167c7c: strd r5,r5,[sp,#0x0]
  00167c80: blx 0x00073d80
  00167c84: vmov s0,r8
  00167c88: mov r0,r4
  00167c8a: movs r1,#0x0
  00167c8c: mov.w r2,#0xffffffff
  00167c90: vcvt.f32.s32 s16,s0
  00167c94: mov r3,r6
  00167c96: vmov s0,r9
  00167c9a: ldr.w r11,[r7,#0x8]
  00167c9e: vcvt.f32.s32 s18,s0
  00167ca2: vmov s0,r10
  00167ca6: vcvt.f32.s32 s20,s0
  00167caa: str r5,[sp,#0x10]
  00167cac: str r5,[sp,#0x0]
  00167cae: vstr.32 s16,[sp,#0xc]
  00167cb2: vstr.32 s18,[sp,#0x8]
  00167cb6: vstr.32 s20,[sp,#0x4]
  00167cba: blx 0x00075790
  00167cbe: ldr r0,[0x00167d20]
  00167cc0: str.w r11,[r4,#0x134]
  00167cc4: add r0,pc
  00167cc6: ldr r0,[r0,#0x0]
  00167cc8: adds r0,#0x8
  00167cca: str r0,[r4,#0x0]
  00167ccc: ldr r0,[r4,#0x4]
  00167cce: movs r1,#0x0
  00167cd0: blx 0x00072580
  00167cd4: movs r0,#0x0
  00167cd6: movs r1,#0x1
  00167cd8: strd r10,r9,[r4,#0x124]
  00167cdc: str.w r8,[r4,#0x12c]
  00167ce0: vstr.32 s20,[r4,#0x58]
  00167ce4: vstr.32 s18,[r4,#0x5c]
  00167ce8: vstr.32 s16,[r4,#0x60]
  00167cec: strh.w r0,[r4,#0x130]
  00167cf0: strb.w r1,[r4,#0x72]
  00167cf4: strb.w r0,[r4,#0x4c]
  00167cf8: mov r0,r4
  00167cfa: add sp,#0x18
  00167cfc: vpop {d8,d9,d10}
  00167d00: add sp,#0x4
  00167d02: pop.w {r8,r9,r10,r11}
  00167d06: pop {r4,r5,r6,r7,pc}
```
