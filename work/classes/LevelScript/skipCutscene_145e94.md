# LevelScript::skipCutscene
elf 0x145e94 body 388
Sig: undefined __stdcall skipCutscene(void)

## decompile
```c

/* LevelScript::skipCutscene() */

void LevelScript::skipCutscene(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  Route *this;
  int *piVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  int *piVar6;
  code *pcVar7;
  int *piVar8;
  undefined1 auStack_28 [12];
  int local_1c;
  
  piVar8 = *(int **)(DAT_00156020 + 0x155ea8);
  local_1c = *piVar8;
  iVar1 = Status::getCurrentCampaignMission();
  if (iVar1 == 0x9a) {
    if (*(int *)(in_r0 + 0x1c) - 1U < 9) {
      *(undefined4 *)(in_r0 + 0x1c) = 9;
      for (iVar1 = 0; iVar1 != 8; iVar1 = iVar1 + 1) {
        iVar2 = Level::getMessages();
        RadioMessage::trigger(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
        iVar2 = Level::getMessages();
        RadioMessage::finish(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
      }
      puVar5 = (undefined4 *)(DAT_00156028 + 0x155f02);
      *(undefined4 *)(in_r0 + 0x90) = 0x7d1;
      *(undefined4 *)(in_r0 + 0x94) = 0;
      pcVar7 = (code *)*puVar5;
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      PlayerFighter::setAIDisabled((PlayerFighter *)**(undefined4 **)(iVar1 + 4),false);
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      piVar6 = (int *)**(undefined4 **)(iVar1 + 4);
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      this = (Route *)KIPlayer::getRoute((KIPlayer *)**(undefined4 **)(iVar1 + 4));
      piVar3 = (int *)Route::getWaypoint(this,0);
      (**(code **)(*piVar3 + 0x28))(auStack_28,piVar3);
      (**(code **)(*piVar6 + 0x44))(piVar6,auStack_28);
    }
  }
  else {
    iVar1 = Status::getCurrentCampaignMission();
    if (iVar1 == 0x9d) {
      if (4 < *(int *)(in_r0 + 0x1c)) goto LAB_00156002;
      for (iVar1 = 0; iVar1 != 4; iVar1 = iVar1 + 1) {
        iVar2 = Level::getMessages();
        RadioMessage::trigger(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
        iVar2 = Level::getMessages();
        RadioMessage::finish(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
      }
      *(undefined4 *)(in_r0 + 0x90) = 0x4651;
      *(undefined4 *)(in_r0 + 0x94) = 0;
      uVar4 = 4;
    }
    else {
      iVar1 = Status::getCurrentCampaignMission();
      if (iVar1 != 0x9e) goto LAB_00156002;
      *(undefined4 *)(in_r0 + 0x98) = 0x2ee1;
      *(undefined4 *)(in_r0 + 0x9c) = 0;
      for (iVar1 = 0; iVar1 != 3; iVar1 = iVar1 + 1) {
        iVar2 = Level::getMessages();
        RadioMessage::trigger(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
        iVar2 = Level::getMessages();
        RadioMessage::finish(*(RadioMessage **)(*(int *)(iVar2 + 4) + iVar1 * 4));
      }
      pcVar7 = *(code **)(DAT_0015602c + 0x155fd0);
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      (**(code **)(*(int *)**(undefined4 **)(iVar1 + 4) + 0x48))
                ((int *)**(undefined4 **)(iVar1 + 4),DAT_00156018,0,DAT_0015601c);
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      KIPlayer::setVisible((KIPlayer *)**(undefined4 **)(iVar1 + 4),true);
      iVar1 = (*pcVar7)(*(undefined4 *)(in_r0 + 0x18));
      KIPlayer::setActive(SUB41(**(undefined4 **)(iVar1 + 4),0));
      uVar4 = 2;
    }
    *(undefined4 *)(in_r0 + 0x1c) = uVar4;
  }
LAB_00156002:
  if (*piVar8 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00155e94: push {r4,r5,r6,r7,lr}
  00155e96: add r7,sp,#0xc
  00155e98: push.w r8
  00155e9c: sub sp,#0x10
  00155e9e: mov r4,r0
  00155ea0: ldr r0,[0x00156020]
  00155ea2: ldr r1,[0x00156024]
  00155ea4: add r0,pc
  00155ea6: add r1,pc
  00155ea8: ldr.w r8,[r0,#0x0]
  00155eac: ldr r5,[r1,#0x0]
  00155eae: ldr.w r1,[r8,#0x0]
  00155eb2: ldr r0,[r5,#0x0]
  00155eb4: str r1,[sp,#0xc]
  00155eb6: blx 0x00071770
  00155eba: cmp r0,#0x9a
  00155ebc: bne 0x00155f46
  00155ebe: ldr r0,[r4,#0x1c]
  00155ec0: subs r0,#0x1
  00155ec2: cmp r0,#0x8
  00155ec4: bhi.w 0x00156002
  00155ec8: movs r0,#0x9
  00155eca: movs r5,#0x0
  00155ecc: str r0,[r4,#0x1c]
  00155ece: b 0x00155ef2
  00155ed0: ldr r0,[r4,#0x18]
  00155ed2: blx 0x00071ef0
  00155ed6: ldr r0,[r0,#0x4]
  00155ed8: ldr.w r0,[r0,r5,lsl #0x2]
  00155edc: blx 0x00076d5c
  00155ee0: ldr r0,[r4,#0x18]
  00155ee2: blx 0x00071ef0
  00155ee6: ldr r0,[r0,#0x4]
  00155ee8: ldr.w r0,[r0,r5,lsl #0x2]
  00155eec: blx 0x00076d68
  00155ef0: adds r5,#0x1
  00155ef2: cmp r5,#0x8
  00155ef4: bne 0x00155ed0
  00155ef6: ldr r1,[0x00156028]
  00155ef8: movs r0,#0x0
  00155efa: movw r2,#0x7d1
  00155efe: add r1,pc
  00155f00: strd r2,r0,[r4,#0x90]
  00155f04: ldr r0,[r4,#0x18]
  00155f06: ldr r6,[r1,#0x0]
  00155f08: blx r6
  00155f0a: ldr r0,[r0,#0x4]
  00155f0c: movs r1,#0x0
  00155f0e: ldr r0,[r0,#0x0]
  00155f10: blx 0x00074158
  00155f14: ldr r0,[r4,#0x18]
  00155f16: blx r6
  00155f18: ldr r0,[r0,#0x4]
  00155f1a: ldr r5,[r0,#0x0]
  00155f1c: ldr r0,[r4,#0x18]
  00155f1e: blx r6
  00155f20: ldr r0,[r0,#0x4]
  00155f22: ldr r0,[r0,#0x0]
  00155f24: blx 0x00073fd8
  00155f28: movs r1,#0x0
  00155f2a: blx 0x00073ec4
  00155f2e: mov r1,r0
  00155f30: ldr r0,[r0,#0x0]
  00155f32: mov r4,sp
  00155f34: ldr r2,[r0,#0x28]
  00155f36: mov r0,r4
  00155f38: blx r2
  00155f3a: ldr r0,[r5,#0x0]
  00155f3c: mov r1,r4
  00155f3e: ldr r2,[r0,#0x44]
  00155f40: mov r0,r5
  00155f42: blx r2
  00155f44: b 0x00156002
  00155f46: ldr r0,[r5,#0x0]
  00155f48: blx 0x00071770
  00155f4c: cmp r0,#0x9d
  00155f4e: bne 0x00155f8e
  00155f50: ldr r0,[r4,#0x1c]
  00155f52: cmp r0,#0x4
  00155f54: bgt 0x00156002
  00155f56: movs r5,#0x0
  00155f58: b 0x00155f7c
  00155f5a: ldr r0,[r4,#0x18]
  00155f5c: blx 0x00071ef0
  00155f60: ldr r0,[r0,#0x4]
  00155f62: ldr.w r0,[r0,r5,lsl #0x2]
  00155f66: blx 0x00076d5c
  00155f6a: ldr r0,[r4,#0x18]
  00155f6c: blx 0x00071ef0
  00155f70: ldr r0,[r0,#0x4]
  00155f72: ldr.w r0,[r0,r5,lsl #0x2]
  00155f76: blx 0x00076d68
  00155f7a: adds r5,#0x1
  00155f7c: cmp r5,#0x4
  00155f7e: bne 0x00155f5a
  00155f80: movs r0,#0x0
  00155f82: movw r1,#0x4651
  00155f86: strd r1,r0,[r4,#0x90]
  00155f8a: movs r0,#0x4
  00155f8c: b 0x00156000
  00155f8e: ldr r0,[r5,#0x0]
  00155f90: blx 0x00071770
  00155f94: cmp r0,#0x9e
  00155f96: bne 0x00156002
  00155f98: movs r5,#0x0
  00155f9a: movw r0,#0x2ee1
  00155f9e: strd r0,r5,[r4,#0x98]
  00155fa2: b 0x00155fc4
  00155fa4: blx 0x00071ef0
  00155fa8: ldr r0,[r0,#0x4]
  00155faa: ldr.w r0,[r0,r5,lsl #0x2]
  00155fae: blx 0x00076d5c
  00155fb2: ldr r0,[r4,#0x18]
  00155fb4: blx 0x00071ef0
  00155fb8: ldr r0,[r0,#0x4]
  00155fba: ldr.w r0,[r0,r5,lsl #0x2]
  00155fbe: blx 0x00076d68
  00155fc2: adds r5,#0x1
  00155fc4: ldr r0,[r4,#0x18]
  00155fc6: cmp r5,#0x3
  00155fc8: bne 0x00155fa4
  00155fca: ldr r1,[0x0015602c]
  00155fcc: add r1,pc
  00155fce: ldr r5,[r1,#0x0]
  00155fd0: blx r5
  00155fd2: ldr r0,[r0,#0x4]
  00155fd4: movs r2,#0x0
  00155fd6: ldr r3,[0x0015601c]
  00155fd8: ldr r0,[r0,#0x0]
  00155fda: ldr r1,[r0,#0x0]
  00155fdc: ldr r6,[r1,#0x48]
  00155fde: ldr r1,[0x00156018]
  00155fe0: blx r6
  00155fe2: ldr r0,[r4,#0x18]
  00155fe4: blx r5
  00155fe6: ldr r0,[r0,#0x4]
  00155fe8: movs r1,#0x1
  00155fea: ldr r0,[r0,#0x0]
  00155fec: blx 0x000730b4
  00155ff0: ldr r0,[r4,#0x18]
  00155ff2: blx r5
  00155ff4: ldr r0,[r0,#0x4]
  00155ff6: movs r1,#0x1
  00155ff8: ldr r0,[r0,#0x0]
  00155ffa: blx 0x000732f4
  00155ffe: movs r0,#0x2
  00156000: str r0,[r4,#0x1c]
  00156002: ldr r0,[sp,#0xc]
  00156004: ldr.w r1,[r8,#0x0]
  00156008: subs r0,r1,r0
  0015600a: ittt eq
  0015600c: add.eq sp,#0x10
  0015600e: pop.eq.w r8
  00156012: pop.eq {r4,r5,r6,r7,pc}
  00156014: blx 0x0006e824
```
