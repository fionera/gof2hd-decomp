# MGame::dialogueEvent
elf 0x17fe34 body 274
Sig: undefined __stdcall dialogueEvent(void)

## decompile
```c

/* MGame::dialogueEvent() */

void MGame::dialogueEvent(void)

{
  int in_r0;
  int iVar1;
  Mission *pMVar2;
  DialogueWindow *this;
  
  iVar1 = LevelScript::startSequenceOver();
  if (iVar1 != 0) {
    iVar1 = Status::getCurrentCampaignMission();
    iVar1 = DialogueWindow::hasBriefingDialogue(iVar1);
    if (iVar1 == 0) {
      pMVar2 = (Mission *)Status::getMission();
      iVar1 = Mission::isCampaignMission(pMVar2);
      if (iVar1 != 0) {
        return;
      }
    }
    Status::getMission();
    iVar1 = Mission::isEmpty();
    if (iVar1 == 0) {
      Status::getMission();
      iVar1 = Mission::getType();
      if (iVar1 != 8) {
        Status::getMission();
        iVar1 = Mission::getType();
        if (iVar1 != 0xa6) {
          Status::getMission();
          iVar1 = Mission::getType();
          if (iVar1 != 0) {
            Status::getMission();
            iVar1 = Mission::getType();
            if (iVar1 != 0xb7) {
              pMVar2 = (Mission *)Status::getMission();
              iVar1 = Mission::isVisible(pMVar2);
              if (iVar1 != 0) {
                pMVar2 = (Mission *)Status::getMission();
                iVar1 = Mission::isCampaignMission(pMVar2);
                if (iVar1 == 0) {
                  Status::getMission();
                  iVar1 = Mission::getType();
                  if (iVar1 == 0xb) {
                    return;
                  }
                }
                if (*(int *)(in_r0 + 0x8c) == 0) {
                  this = operator_new(0x74);
                  pMVar2 = (Mission *)Status::getMission();
                  DialogueWindow::DialogueWindow(this,pMVar2,*(Level **)(in_r0 + 0x78),0);
                  *(DialogueWindow **)(in_r0 + 0x8c) = this;
                }
                PlayerEgo::setTurretMode(SUB41(*(undefined4 *)(in_r0 + 0x58),0));
                LevelScript::resetCamera(*(Level **)(in_r0 + 0x7c));
                PlayerEgo::setFreeLookMode(*(PlayerEgo **)(in_r0 + 0x58),false);
                TargetFollowCamera::enableFirstPersonCam
                          (*(TargetFollowCamera **)(in_r0 + 0xf4),false);
                PlayerEgo::hideShipForFirstPersonCameraView(SUB41(*(undefined4 *)(in_r0 + 0x58),0));
                iVar1 = *(int *)(in_r0 + 0x7c);
                *(undefined1 *)(in_r0 + 0x111) = 1;
                *(undefined4 *)(iVar1 + 8) = 0;
                *(undefined4 *)(iVar1 + 0xc) = 0;
                *(undefined1 *)(in_r0 + 0x5d) = 1;
                pauseSounds();
                *(undefined1 *)(in_r0 + 0x5e) = 1;
              }
            }
          }
        }
      }
    }
  }
  return;
}

```

## target disasm
```
  0018fe34: push {r4,r5,r6,r7,lr}
  0018fe36: add r7,sp,#0xc
  0018fe38: push.w r11
  0018fe3c: mov r4,r0
  0018fe3e: ldr r0,[r0,#0x7c]
  0018fe40: blx 0x00078544
  0018fe44: cmp r0,#0x0
  0018fe46: beq 0x0018ff40
  0018fe48: ldr r0,[0x0018ff54]
  0018fe4a: add r0,pc
  0018fe4c: ldr r6,[r0,#0x0]
  0018fe4e: ldr r0,[r6,#0x0]
  0018fe50: blx 0x00071770
  0018fe54: blx 0x00078640
  0018fe58: cbnz r0,0x0018fe68
  0018fe5a: ldr r0,[r6,#0x0]
  0018fe5c: blx 0x00072850
  0018fe60: blx 0x0007372c
  0018fe64: cmp r0,#0x0
  0018fe66: bne 0x0018ff40
  0018fe68: ldr r0,[r6,#0x0]
  0018fe6a: blx 0x00072850
  0018fe6e: blx 0x00072868
  0018fe72: cmp r0,#0x0
  0018fe74: bne 0x0018ff40
  0018fe76: ldr r0,[r6,#0x0]
  0018fe78: blx 0x00072850
  0018fe7c: blx 0x00072874
  0018fe80: cmp r0,#0x8
  0018fe82: beq 0x0018ff40
  0018fe84: ldr r0,[r6,#0x0]
  0018fe86: blx 0x00072850
  0018fe8a: blx 0x00072874
  0018fe8e: cmp r0,#0xa6
  0018fe90: beq 0x0018ff40
  0018fe92: ldr r0,[r6,#0x0]
  0018fe94: blx 0x00072850
  0018fe98: blx 0x00072874
  0018fe9c: cmp r0,#0x0
  0018fe9e: beq 0x0018ff40
  0018fea0: ldr r0,[r6,#0x0]
  0018fea2: blx 0x00072850
  0018fea6: blx 0x00072874
  0018feaa: cmp r0,#0xb7
  0018feac: beq 0x0018ff40
  0018feae: ldr r0,[r6,#0x0]
  0018feb0: blx 0x00072850
  0018feb4: blx 0x00074590
  0018feb8: cmp r0,#0x0
  0018feba: beq 0x0018ff40
  0018febc: ldr r0,[r6,#0x0]
  0018febe: blx 0x00072850
  0018fec2: blx 0x0007372c
  0018fec6: cbnz r0,0x0018fed6
  0018fec8: ldr r0,[r6,#0x0]
  0018feca: blx 0x00072850
  0018fece: blx 0x00072874
  0018fed2: cmp r0,#0xb
  0018fed4: beq 0x0018ff40
  0018fed6: ldr.w r0,[r4,#0x8c]
  0018feda: cbnz r0,0x0018fefa
  0018fedc: movs r0,#0x74
  0018fede: blx 0x0006eb24
  0018fee2: mov r5,r0
  0018fee4: ldr r0,[r6,#0x0]
  0018fee6: blx 0x00072850
  0018feea: mov r1,r0
  0018feec: ldr r2,[r4,#0x78]
  0018feee: mov r0,r5
  0018fef0: movs r3,#0x0
  0018fef2: blx 0x0007501c
  0018fef6: str.w r5,[r4,#0x8c]
  0018fefa: ldr r0,[r4,#0x58]
  0018fefc: movs r1,#0x0
  0018fefe: movs r5,#0x0
  0018ff00: blx 0x00072b08
  0018ff04: ldrd r1,r0,[r4,#0x78]
  0018ff08: blx 0x0007246c
  0018ff0c: ldr r0,[r4,#0x58]
  0018ff0e: movs r1,#0x0
  0018ff10: blx 0x00076b94
  0018ff14: ldr.w r0,[r4,#0xf4]
  0018ff18: movs r1,#0x0
  0018ff1a: blx 0x00076ba0
  0018ff1e: ldr r0,[r4,#0x58]
  0018ff20: movs r1,#0x0
  0018ff22: blx 0x00076bac
  0018ff26: ldr r0,[r4,#0x7c]
  0018ff28: movs r6,#0x1
  0018ff2a: strb.w r6,[r4,#0x111]
  0018ff2e: strd r5,r5,[r0,#0x8]
  0018ff32: mov r0,r4
  0018ff34: strb.w r6,[r4,#0x5d]
  0018ff38: blx 0x0007822c
  0018ff3c: strb.w r6,[r4,#0x5e]
  0018ff40: pop.w r11
  0018ff44: pop {r4,r5,r6,r7,pc}
```
