# PlayerStaticFar::PlayerStaticFar
elf 0x11c254 body 92
Sig: undefined __thiscall PlayerStaticFar(PlayerStaticFar * this, int param_1, AEGeometry * param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* PlayerStaticFar::PlayerStaticFar(int, AEGeometry*, float, float, float) */

PlayerStaticFar * __thiscall
PlayerStaticFar::PlayerStaticFar
          (PlayerStaticFar *this,int param_1,AEGeometry *param_2,float param_3,float param_4,
          float param_5)

{
  int iVar1;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  undefined4 in_stack_00000004;
  
  PlayerStatic::PlayerStatic((PlayerStatic *)this,param_1,param_2,param_3,param_4,param_5);
  iVar1 = DAT_0012c2c0;
  *(undefined4 *)(this + 0x134) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = 0;
  *(undefined4 *)(this + 0x58) = in_r3;
  iVar1 = *(int *)(iVar1 + 0x12c284);
  *(undefined4 *)(this + 0x5c) = in_stack_00000000;
  *(undefined4 *)(this + 0x60) = in_stack_00000004;
  *(int *)this = iVar1 + 8;
  Player::setRadius(*(Player **)(this + 4),0x1d4c);
  *(undefined4 *)(this + 0x130) = 0;
  return this;
}

```

## target disasm
```
  0012c254: push {r4,r5,r6,r7,lr}
  0012c256: add r7,sp,#0xc
  0012c258: push.w r11
  0012c25c: vpush {d7,d8,d9}
  0012c260: vldr.32 s16,[r7,#0x8]
  0012c264: mov r5,r3
  0012c266: vldr.32 s18,[r7,#0xc]
  0012c26a: mov r4,r0
  0012c26c: vstr.32 s16,[sp]
  0012c270: vstr.32 s18,[sp,#0x4]
  0012c274: blx 0x00075fdc
  0012c278: ldr r0,[0x0012c2c0]
  0012c27a: movs r6,#0x0
  0012c27c: strd r6,r6,[r4,#0x134]
  0012c280: add r0,pc
  0012c282: str.w r6,[r4,#0x13c]
  0012c286: str r5,[r4,#0x58]
  0012c288: ldr r1,[r0,#0x0]
  0012c28a: vstr.32 s16,[r4,#0x5c]
  0012c28e: vstr.32 s18,[r4,#0x60]
  0012c292: adds r1,#0x8
  0012c294: ldr r0,[r4,#0x4]
  0012c296: str r1,[r4,#0x0]
  0012c298: movw r1,#0x1d4c
  0012c29c: blx 0x000730d8
  0012c2a0: str.w r6,[r4,#0x130]
  0012c2a4: mov r0,r4
  0012c2a6: vpop {d7,d8,d9}
  0012c2aa: pop.w r11
  0012c2ae: pop {r4,r5,r6,r7,pc}
```
