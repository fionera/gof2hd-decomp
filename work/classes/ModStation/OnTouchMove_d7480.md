# ModStation::OnTouchMove
elf 0xd7480 body 376
Sig: undefined __thiscall OnTouchMove(ModStation * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* ModStation::OnTouchMove(int, int, void*) */

void __thiscall ModStation::OnTouchMove(ModStation *this,int param_1,int param_2,void *param_3)

{
  int *piVar1;
  int iVar2;
  
  if (this[2].field_8 == param_3) {
    iVar2 = this->m_nStarMapWindowOpen;
    this[1].m_pDialogueWindow = (DialogueWindow *)param_1;
    this[1].field_84 = param_2;
    if ((char)iVar2 == '\0') {
      piVar1 = *(int **)(DAT_000e75f8 + 0xe74a6);
      if (*(char *)*piVar1 == '\0') {
        if (*(char *)((int)&this->field_68 + 1) == '\0') {
          if (*(char *)((int)&this->m_nStarMapWindowOpen + 3) == '\0') {
            if (*(char *)((int)&this->field_68 + 2) == '\0') {
              if (*(char *)((int)&this->field_64 + 2) != '\0') {
                HangarWindow::OnTouchMove(this->field_78,param_1);
                return;
              }
              if (*(char *)((int)&this->field_64 + 3) != '\0') {
                StarMap::OnTouchMove(this->field_10,param_1);
                return;
              }
              if (*(char *)((int)&this->field_64 + 1) != '\0') {
                SpaceLounge::OnTouchMove(this->field_74,param_1);
                return;
              }
              if ((char)this->field_68 != '\0') {
                StatusWindow::OnTouchMove(this->field_7C,param_1);
                return;
              }
              if ((char)this->field_64 != '\0') {
                MissionsWindow::OnTouchMove((int)this->m_pDialogueWindow,param_1);
                return;
              }
              if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) != '\0') {
                MenuTouchWindow::OnTouchMove(this->field_50,param_1,(void *)param_2);
                return;
              }
              if (*(char *)((int)&this->m_nStarMapWindowOpen + 1) == '\0') {
                return;
              }
              TouchButton::OnTouchMove(this[1].field_0,param_1);
              TouchButton::OnTouchMove((int)this[1].field_4,param_1);
              for (iVar2 = 0; iVar2 != 5; iVar2 = iVar2 + 1) {
                TouchButton::OnTouchMove(*(int *)(this->field_8C[1] + iVar2 * 4),param_1);
              }
              Layout::OnTouchMove(*piVar1,param_1);
              iVar2 = NewsTicker::OnTouchMove((int)this->field_1C,param_1);
              if (iVar2 != 0) {
                return;
              }
              if (param_2 <= *(int *)(*piVar1 + 0xc)) {
                return;
              }
              if (**(int **)(DAT_000e75fc + 0xe75ce) - *(int *)(*piVar1 + 0x10) <= param_2) {
                return;
              }
              if (param_1 <= (int)this[1].field_18) {
                return;
              }
              iVar2 = param_1 - this[1].field_58;
              this[1].m_nStarMapWindowOpen = iVar2;
              this[1].field_64 = 0x3f800000;
              this[1].field_54 = this[1].field_54 + iVar2;
              this[1].field_58 = param_1;
              return;
            }
            piVar1 = this->field_88;
          }
          else {
            piVar1 = (int *)this->field_70;
          }
          ChoiceWindow::OnTouchMove((int)piVar1,param_1);
        }
        else {
          DialogueWindow::OnTouchMove(this->field_84,param_1);
        }
      }
      else {
        Layout::OnTouchMove(*piVar1,param_1);
      }
    }
  }
  return;
}

```

## target disasm
```
  000e7480: push {r4,r5,r6,r7,lr}
  000e7482: add r7,sp,#0xc
  000e7484: push {r8,r9,r11}
  000e7488: mov r6,r0
  000e748a: ldr.w r0,[r0,#0x128]
  000e748e: mov r5,r2
  000e7490: mov r4,r1
  000e7492: cmp r0,r3
  000e7494: bne 0x000e74ea
  000e7496: ldrb.w r0,[r6,#0x60]
  000e749a: strd r4,r5,[r6,#0x110]
  000e749e: cbnz r0,0x000e74ea
  000e74a0: ldr r0,[0x000e75f8]
  000e74a2: add r0,pc
  000e74a4: ldr.w r8,[r0,#0x0]
  000e74a8: ldr.w r0,[r8,#0x0]
  000e74ac: ldrb r1,[r0,#0x0]
  000e74ae: cbz r1,0x000e74ba
  000e74b0: mov r1,r4
  000e74b2: mov r2,r5
  000e74b4: blx 0x00074758
  000e74b8: b 0x000e74ea
  000e74ba: ldrb.w r0,[r6,#0x69]
  000e74be: cbz r0,0x000e74ce
  000e74c0: ldr.w r0,[r6,#0x84]
  000e74c4: mov r1,r4
  000e74c6: mov r2,r5
  000e74c8: blx 0x000750d0
  000e74cc: b 0x000e74ea
  000e74ce: ldrb.w r0,[r6,#0x63]
  000e74d2: cbz r0,0x000e74d8
  000e74d4: ldr r0,[r6,#0x70]
  000e74d6: b 0x000e74e2
  000e74d8: ldrb.w r0,[r6,#0x6a]
  000e74dc: cbz r0,0x000e74f0
  000e74de: ldr.w r0,[r6,#0x88]
  000e74e2: mov r1,r4
  000e74e4: mov r2,r5
  000e74e6: blx 0x0007474c
  000e74ea: pop.w {r8,r9,r11}
  000e74ee: pop {r4,r5,r6,r7,pc}
  000e74f0: ldrb.w r0,[r6,#0x66]
  000e74f4: cbz r0,0x000e7502
  000e74f6: ldr r0,[r6,#0x78]
  000e74f8: mov r1,r4
  000e74fa: mov r2,r5
  000e74fc: blx 0x000750dc
  000e7500: b 0x000e74ea
  000e7502: ldrb.w r0,[r6,#0x67]
  000e7506: cbz r0,0x000e7514
  000e7508: ldr r0,[r6,#0x10]
  000e750a: mov r1,r4
  000e750c: mov r2,r5
  000e750e: blx 0x000750e8
  000e7512: b 0x000e74ea
  000e7514: ldrb.w r0,[r6,#0x65]
  000e7518: cbz r0,0x000e7526
  000e751a: ldr r0,[r6,#0x74]
  000e751c: mov r1,r4
  000e751e: mov r2,r5
  000e7520: blx 0x000750f4
  000e7524: b 0x000e74ea
  000e7526: ldrb.w r0,[r6,#0x68]
  000e752a: cbz r0,0x000e7538
  000e752c: ldr r0,[r6,#0x7c]
  000e752e: mov r1,r4
  000e7530: mov r2,r5
  000e7532: blx 0x00075100
  000e7536: b 0x000e74ea
  000e7538: ldrb.w r0,[r6,#0x64]
  000e753c: cbz r0,0x000e754c
  000e753e: ldr.w r0,[r6,#0x80]
  000e7542: mov r1,r4
  000e7544: mov r2,r5
  000e7546: blx 0x0007510c
  000e754a: b 0x000e74ea
  000e754c: ldrb.w r0,[r6,#0x62]
  000e7550: cbz r0,0x000e755e
  000e7552: ldr r0,[r6,#0x50]
  000e7554: mov r1,r4
  000e7556: mov r2,r5
  000e7558: blx 0x00075118
  000e755c: b 0x000e74ea
  000e755e: ldrb.w r0,[r6,#0x61]
  000e7562: cmp r0,#0x0
  000e7564: beq 0x000e74ea
  000e7566: ldr.w r0,[r6,#0x90]
  000e756a: mov r1,r4
  000e756c: mov r2,r5
  000e756e: blx 0x00074764
  000e7572: ldr.w r0,[r6,#0x94]
  000e7576: mov r1,r4
  000e7578: mov r2,r5
  000e757a: blx 0x00074764
  000e757e: mov.w r9,#0x0
  000e7582: b 0x000e759a
  000e7584: ldr.w r0,[r6,#0x8c]
  000e7588: mov r1,r4
  000e758a: mov r2,r5
  000e758c: ldr r0,[r0,#0x4]
  000e758e: ldr.w r0,[r0,r9,lsl #0x2]
  000e7592: blx 0x00074764
  000e7596: add.w r9,r9,#0x1
  000e759a: cmp.w r9,#0x5
  000e759e: bne 0x000e7584
  000e75a0: ldr.w r0,[r8,#0x0]
  000e75a4: mov r1,r4
  000e75a6: mov r2,r5
  000e75a8: blx 0x00074758
  000e75ac: ldr r0,[r6,#0x1c]
  000e75ae: mov r1,r4
  000e75b0: mov r2,r5
  000e75b2: blx 0x00075124
  000e75b6: cmp r0,#0x0
  000e75b8: bne 0x000e74ea
  000e75ba: ldr.w r0,[r8,#0x0]
  000e75be: ldr r1,[r0,#0xc]
  000e75c0: cmp r1,r5
  000e75c2: bge.w 0x000e74ea
  000e75c6: ldr r1,[0x000e75fc]
  000e75c8: ldr r0,[r0,#0x10]
  000e75ca: add r1,pc
  000e75cc: ldr r1,[r1,#0x0]
  000e75ce: ldr r1,[r1,#0x0]
  000e75d0: subs r0,r1,r0
  000e75d2: cmp r0,r5
  000e75d4: ble.w 0x000e74ea
  000e75d8: ldr.w r0,[r6,#0xa8]
  000e75dc: cmp r0,r4
  000e75de: bge.w 0x000e74ea
  000e75e2: ldrd r0,r1,[r6,#0xe4]
  000e75e6: mov.w r2,#0x3f800000
  000e75ea: subs r1,r4,r1
  000e75ec: strd r1,r2,[r6,#0xf0]
  000e75f0: add r0,r1
  000e75f2: strd r0,r4,[r6,#0xe4]
  000e75f6: b 0x000e74ea
```
