# PlayerWormHole::PlayerWormHole
elf 0xa5254 body 126
Sig: undefined __thiscall PlayerWormHole(PlayerWormHole * this, int param_1, AEGeometry * param_2, float param_3, float param_4, float param_5, bool param_6)

## decompile
```c

/* PlayerWormHole::PlayerWormHole(int, AEGeometry*, float, float, float, bool) */

PlayerWormHole * __thiscall
PlayerWormHole::PlayerWormHole
          (PlayerWormHole *this,int param_1,AEGeometry *param_2,float param_3,float param_4,
          float param_5,bool param_6)

{
  int *piVar1;
  String *pSVar2;
  undefined4 uVar3;
  float in_stack_00000000;
  float in_stack_00000004;
  bool in_stack_00000008;
  
  PlayerStaticFar::PlayerStaticFar
            ((PlayerStaticFar *)this,param_1,param_2,in_stack_00000000,param_4,in_stack_00000004);
  piVar1 = *(int **)(DAT_000b52e0 + 0xb5276);
  *(int *)this = *(int *)(DAT_000b52e4 + 0xb5278) + 8;
  pSVar2 = (String *)GameText::getText(*piVar1);
  AbyssEngine::String::operator=((String *)(this + 0x18),pSVar2);
  KIPlayer::setVisible((KIPlayer *)this,in_stack_00000008);
  Player::setRadius(*(Player **)(this + 4),40000);
  uVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000b52e8 + 0xb52ac));
  AbyssEngine::Transform::SetAnimationState(uVar3,2,0);
  this[0x15c] = (PlayerWormHole)0x1;
  *(undefined4 *)(this + 0x150) = 0;
  *(undefined4 *)(this + 0x154) = 0x1000;
  return this;
}

```

## target disasm
```
  000b5254: push {r2,r3,r4,r5,r7,lr}
  000b5256: add r7,sp,#0x10
  000b5258: vldr.32 s0,[r7,#0x8]
  000b525c: mov r4,r0
  000b525e: vldr.32 s2,[r7,#0xc]
  000b5262: vstr.32 s0,[sp]
  000b5266: vstr.32 s2,[sp,#0x4]
  000b526a: blx 0x000730a8
  000b526e: ldr r0,[0x000b52e0]
  000b5270: ldr r1,[0x000b52e4]
  000b5272: add r0,pc
  000b5274: add r1,pc
  000b5276: ldr r0,[r0,#0x0]
  000b5278: ldr r1,[r1,#0x0]
  000b527a: adds r1,#0x8
  000b527c: str r1,[r4,#0x0]
  000b527e: ldr r0,[r0,#0x0]
  000b5280: movw r1,#0x221
  000b5284: blx 0x00072f70
  000b5288: mov r1,r0
  000b528a: add.w r0,r4,#0x18
  000b528e: blx 0x0006f2b0
  000b5292: ldr r1,[r7,#0x10]
  000b5294: mov r0,r4
  000b5296: blx 0x000730b4
  000b529a: ldr r0,[r4,#0x4]
  000b529c: movw r1,#0x9c40
  000b52a0: blx 0x000730d8
  000b52a4: ldr r0,[0x000b52e8]
  000b52a6: ldr r1,[r4,#0x8]
  000b52a8: add r0,pc
  000b52aa: ldr r0,[r0,#0x0]
  000b52ac: ldr r1,[r1,#0xc]
  000b52ae: ldr r0,[r0,#0x0]
  000b52b0: blx 0x00072088
  000b52b4: movs r1,#0x2
  000b52b6: movs r2,#0x0
  000b52b8: movs r5,#0x0
  000b52ba: blx 0x0006fd18
  000b52be: movs r0,#0x1
  000b52c0: strb.w r0,[r4,#0x15c]
  000b52c4: mov.w r0,#0x1000
  000b52c8: strd r5,r0,[r4,#0x150]
  000b52cc: mov r0,r4
  000b52ce: add sp,#0x8
  000b52d0: pop {r4,r5,r7,pc}
```
