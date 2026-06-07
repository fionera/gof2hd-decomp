# PlayerCreature::reset
elf 0x11cc40 body 126
Sig: undefined __thiscall reset(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::reset() */

void __thiscall PlayerCreature::reset(PlayerCreature *this)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int *piVar3;
  undefined4 local_50 [5];
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined4 local_18;
  int local_14;
  
  piVar3 = *(int **)(DAT_0012ccd0 + 0x12cc4e);
  local_14 = *piVar3;
  KIPlayer::reset((KIPlayer *)this);
  *(undefined4 *)(this + 0x88) = 0;
  KIPlayer::setActive(SUB41(this,0));
  *(undefined4 *)(this + 0x138) = *(undefined4 *)(this + 0x134);
  uVar1 = Player::getHitpoints();
  uStack_20 = DAT_0012ccc8;
  uStack_28 = DAT_0012ccc0;
  uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_30 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_2c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x13c) = uVar1;
  puVar2 = (undefined4 *)((uint)local_50 | 4);
  *puVar2 = 0;
  puVar2[1] = uStack_34;
  puVar2[2] = uStack_30;
  puVar2[3] = uStack_2c;
  local_38 = 0;
  local_50[0] = 0x3f800000;
  local_3c = 0x3f800000;
  local_18 = 0x3f800000;
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x144),(Matrix *)local_50);
  if (*piVar3 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012cc40: push {r4,r5,r7,lr}
  0012cc42: add r7,sp,#0x8
  0012cc44: sub sp,#0x40
  0012cc46: mov r4,r0
  0012cc48: ldr r0,[0x0012ccd0]
  0012cc4a: add r0,pc
  0012cc4c: ldr r5,[r0,#0x0]
  0012cc4e: ldr r0,[r5,#0x0]
  0012cc50: str r0,[sp,#0x3c]
  0012cc52: mov r0,r4
  0012cc54: blx 0x00074518
  0012cc58: movs r0,#0x0
  0012cc5a: movs r1,#0x1
  0012cc5c: str.w r0,[r4,#0x88]
  0012cc60: mov r0,r4
  0012cc62: blx 0x000732f4
  0012cc66: ldr r0,[r4,#0x4]
  0012cc68: ldr.w r1,[r4,#0x134]
  0012cc6c: str.w r1,[r4,#0x138]
  0012cc70: blx 0x000724f0
  0012cc74: adr r1,[0x12ccc0]
  0012cc76: vmov.i32 q9,#0x0
  0012cc7a: vld1.64 {d16,d17},[r1]
  0012cc7e: mov r1,sp
  0012cc80: str.w r0,[r4,#0x13c]
  0012cc84: orr r0,r1,#0x4
  0012cc88: vst1.32 {d18,d19},[r0]
  0012cc8c: add.w r0,r1,#0x18
  0012cc90: vst1.64 {d18,d19},[r0]
  0012cc94: add.w r0,r1,#0x28
  0012cc98: vst1.64 {d16,d17},[r0]
  0012cc9c: mov.w r0,#0x3f800000
  0012cca0: str r0,[sp,#0x0]
  0012cca2: str r0,[sp,#0x14]
  0012cca4: str r0,[sp,#0x38]
  0012cca6: add.w r0,r4,#0x144
  0012ccaa: blx 0x0006f148
  0012ccae: ldr r0,[sp,#0x3c]
  0012ccb0: ldr r1,[r5,#0x0]
  0012ccb2: subs r0,r1,r0
  0012ccb4: itt eq
  0012ccb6: add.eq sp,#0x40
  0012ccb8: pop.eq {r4,r5,r7,pc}
  0012ccba: blx 0x0006e824
```
