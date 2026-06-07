# DialogueWindow::set
elf 0x166ee4 body 116
Sig: undefined __stdcall set(Mission * param_1, int param_2, int param_3)

## decompile
```c

/* DialogueWindow::set(Mission*, int, int) */

void DialogueWindow::set(Mission *param_1,int param_2,int param_3)

{
  Agent *this;
  int iVar1;
  int in_r3;
  
  *(int *)(param_1 + 0x44) = param_3;
  *(int *)(param_1 + 0x4c) = param_2;
  if (param_3 == 1) {
    Mission::getAgent((Mission *)param_2);
    Mission::setWon((Mission *)param_2,true);
  }
  else if (param_3 == 2) {
    this = (Agent *)Mission::getAgent((Mission *)param_2);
    if ((this != (Agent *)0x0) && (iVar1 = Agent::isGenericAgent(this), iVar1 == 0)) {
      Agent::setOfferAccepted(this,false);
    }
    Mission::setFailed((Mission *)param_2,true);
  }
  *(undefined4 *)(param_1 + 0x48) = 0;
  if (in_r3 == -1) {
    in_r3 = Status::getCurrentCampaignMission();
  }
  *(int *)(param_1 + 0x10) = in_r3;
  (*(code *)(DAT_001ac674 + 0x1ac678))(param_1);
  return;
}

```

## target disasm
```
  00176ee4: push {r4,r5,r6,r7,lr}
  00176ee6: add r7,sp,#0xc
  00176ee8: push.w r8
  00176eec: mov r5,r3
  00176eee: mov r6,r1
  00176ef0: mov r4,r0
  00176ef2: cmp r2,#0x1
  00176ef4: str r2,[r0,#0x44]
  00176ef6: str r1,[r0,#0x4c]
  00176ef8: beq 0x00176f22
  00176efa: cmp r2,#0x2
  00176efc: bne 0x00176f30
  00176efe: mov r0,r6
  00176f00: blx 0x00073438
  00176f04: mov r8,r0
  00176f06: cbz r0,0x00176f18
  00176f08: mov r0,r8
  00176f0a: blx 0x00071b9c
  00176f0e: cbnz r0,0x00176f18
  00176f10: mov r0,r8
  00176f12: movs r1,#0x0
  00176f14: blx 0x00071b18
  00176f18: mov r0,r6
  00176f1a: movs r1,#0x1
  00176f1c: blx 0x00077bf0
  00176f20: b 0x00176f30
  00176f22: mov r0,r6
  00176f24: blx 0x00073438
  00176f28: mov r0,r6
  00176f2a: movs r1,#0x1
  00176f2c: blx 0x00073984
  00176f30: movs r0,#0x0
  00176f32: str r0,[r4,#0x48]
  00176f34: adds r0,r5,#0x1
  00176f36: bne 0x00176f46
  00176f38: ldr r0,[0x00176f58]
  00176f3a: add r0,pc
  00176f3c: ldr r0,[r0,#0x0]
  00176f3e: ldr r0,[r0,#0x0]
  00176f40: blx 0x00071770
  00176f44: mov r5,r0
  00176f46: str r5,[r4,#0x10]
  00176f48: mov r0,r4
  00176f4a: pop.w r8
  00176f4e: pop.w {r4,r5,r6,r7,lr}
  00176f52: b.w 0x001ac668
  001ac668: bx pc
```
