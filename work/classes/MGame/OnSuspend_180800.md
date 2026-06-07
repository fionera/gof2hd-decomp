# MGame::OnSuspend
elf 0x180800 body 178
Sig: undefined __thiscall OnSuspend(MGame * this)

## decompile
```c

/* MGame::OnSuspend() */

void __thiscall MGame::OnSuspend(MGame *this)

{
  DialogueWindow *pDVar1;
  uint *puVar2;
  uint uVar3;
  
  if ((RecordHandler *)**(undefined4 **)(DAT_001908b8 + 0x190810) != (RecordHandler *)0x0) {
    RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(DAT_001908b8 + 0x190810));
  }
  pauseSounds();
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
    if (this->m_pDialogueWindow == (DialogueWindow *)0x0) {
      pDVar1 = operator_new(0x240);
      MenuTouchWindow::MenuTouchWindow((MenuTouchWindow *)pDVar1,1);
      this->m_pDialogueWindow = pDVar1;
    }
    *(undefined1 *)((int)&this[2].field_14 + 2) = 1;
    *(undefined1 *)((int)&this->field_5C + 1) = 1;
    FModSound::pauseAllPlaying();
    PlayerEgo::PauseEngineSound();
    puVar2 = (uint *)Level::getEnemies();
    if (puVar2 != (uint *)0x0) {
      for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
        KIPlayer::PauseEngineSound(*(KIPlayer **)(puVar2[1] + uVar3 * 4));
      }
    }
    pDVar1 = this->m_pDialogueWindow;
    if (*(char *)((int)&this->field_5C + 3) == '\0') {
      PlayerEgo::isDead(this->m_pPlayerEgo);
    }
    MenuTouchWindow::setCutsceneMode(SUB41(pDVar1,0));
    *(undefined1 *)((int)&this[1].field_0 + 1) = 1;
  }
  (*(code *)(DAT_001ac7f4 + 0x1ac7f8))(this->m_pLevel);
  return;
}

```

## target disasm
```
  00190800: push {r4,r5,r6,r7,lr}
  00190802: add r7,sp,#0xc
  00190804: push.w r11
  00190808: mov r4,r0
  0019080a: ldr r0,[0x001908b8]
  0019080c: add r0,pc
  0019080e: ldr r0,[r0,#0x0]
  00190810: ldr r0,[r0,#0x0]
  00190812: cbz r0,0x00190818
  00190814: blx 0x00071788
  00190818: mov r0,r4
  0019081a: blx 0x0007822c
  0019081e: ldrb.w r0,[r4,#0x5d]
  00190822: cbnz r0,0x0019089a
  00190824: ldr.w r0,[r4,#0x88]
  00190828: cbnz r0,0x0019083e
  0019082a: mov.w r0,#0x240
  0019082e: blx 0x0006eb24
  00190832: mov r5,r0
  00190834: movs r1,#0x1
  00190836: blx 0x00075268
  0019083a: str.w r5,[r4,#0x88]
  0019083e: ldr r0,[0x001908bc]
  00190840: movs r1,#0x1
  00190842: strb.w r1,[r4,#0x1a6]
  00190846: add r0,pc
  00190848: strb.w r1,[r4,#0x5d]
  0019084c: ldr r0,[r0,#0x0]
  0019084e: ldr r0,[r0,#0x0]
  00190850: blx 0x00078310
  00190854: ldr r0,[r4,#0x58]
  00190856: blx 0x00078190
  0019085a: ldr r0,[r4,#0x78]
  0019085c: blx 0x00071eb4
  00190860: mov r5,r0
  00190862: cbz r0,0x0019087a
  00190864: movs r6,#0x0
  00190866: b 0x00190874
  00190868: ldr r0,[r5,#0x4]
  0019086a: ldr.w r0,[r0,r6,lsl #0x2]
  0019086e: blx 0x0007819c
  00190872: adds r6,#0x1
  00190874: ldr r0,[r5,#0x0]
  00190876: cmp r6,r0
  00190878: bcc 0x00190868
  0019087a: ldrb.w r0,[r4,#0x5f]
  0019087e: movs r6,#0x1
  00190880: ldr.w r5,[r4,#0x88]
  00190884: movs r1,#0x1
  00190886: cbnz r0,0x00190890
  00190888: ldr r0,[r4,#0x58]
  0019088a: blx 0x000724b4
  0019088e: mov r1,r0
  00190890: mov r0,r5
  00190892: blx 0x0007831c
  00190896: strb.w r6,[r4,#0xc9]
  0019089a: ldr r0,[r4,#0x74]
  0019089c: pop.w r11
  001908a0: pop.w {r4,r5,r6,r7,lr}
  001908a4: b.w 0x001ac7e8
  001ac7e8: bx pc
  001ac7ec: ldr r12,[0x1ac7f4]
  001ac7f0: add pc,r12,pc
```
