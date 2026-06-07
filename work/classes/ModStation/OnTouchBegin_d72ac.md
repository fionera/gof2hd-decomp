# ModStation::OnTouchBegin
elf 0xd72ac body 448
Sig: undefined __thiscall OnTouchBegin(ModStation * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* ModStation::OnTouchBegin(int, int, void*) */

void __thiscall ModStation::OnTouchBegin(ModStation *this,int param_1,int param_2,void *param_3)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  
  if (this[2].field_8 == (int *)0x0) {
    *(undefined1 *)&this[1].field_88 = 1;
    this[2].field_8 = param_3;
    iVar4 = this->m_nStarMapWindowOpen;
    this[1].m_pDialogueWindow = (DialogueWindow *)param_1;
    this[1].field_84 = param_2;
    if ((char)iVar4 == '\0') {
      piVar3 = *(int **)(DAT_000e7478 + 0xe731a);
      if (*(char *)*piVar3 == '\0') {
        if (*(char *)((int)&this->field_68 + 1) == '\0') {
          if (*(char *)((int)&this->m_nStarMapWindowOpen + 3) == '\0') {
            if (*(char *)((int)&this->field_68 + 2) == '\0') {
              if (*(char *)((int)&this->field_64 + 2) != '\0') {
                HangarWindow::OnTouchBegin(this->field_78,param_1);
                return;
              }
              if (*(char *)((int)&this->field_64 + 1) != '\0') {
                SpaceLounge::OnTouchBegin((SpaceLounge *)this->field_74,param_1,param_2);
                return;
              }
              if (*(char *)((int)&this->field_64 + 3) != '\0') {
                StarMap::OnTouchBegin(this->field_10,param_1);
                return;
              }
              if ((char)this->field_68 != '\0') {
                StatusWindow::OnTouchBegin((StatusWindow *)this->field_7C,param_1,param_2);
                return;
              }
              if ((char)this->field_64 != '\0') {
                MissionsWindow::OnTouchBegin((int)this->m_pDialogueWindow,param_1);
                return;
              }
              if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) != '\0') {
                MenuTouchWindow::OnTouchBegin(this->field_50,param_1,(void *)param_2);
                return;
              }
              if (*(char *)((int)&this->m_nStarMapWindowOpen + 1) == '\0') {
                return;
              }
              TouchButton::OnTouchBegin(this[1].field_0,param_1);
              TouchButton::OnTouchBegin((int)this[1].field_4,param_1);
              for (iVar4 = 0; iVar4 != 5; iVar4 = iVar4 + 1) {
                TouchButton::OnTouchBegin(*(int *)(this->field_8C[1] + iVar4 * 4),param_1);
              }
              Layout::OnTouchBegin(*piVar3,param_1);
              iVar4 = NewsTicker::OnTouchBegin((int)this->field_1C,param_1);
              if (iVar4 != 0) {
                return;
              }
              if (param_2 <= *(int *)(*piVar3 + 0xc)) {
                return;
              }
              if (**(int **)(DAT_000e747c + 0xe7442) - *(int *)(*piVar3 + 0x10) <= param_2) {
                return;
              }
              if (param_1 <= (int)this[1].field_18) {
                return;
              }
              this[1].field_6C = (int *)param_1;
              this[1].field_58 = param_1;
              this[1].m_nStarMapWindowOpen = 0;
              *(undefined1 *)&this[1].field_70 = 1;
              return;
            }
            piVar3 = this->field_88;
          }
          else {
            piVar3 = (int *)this->field_70;
          }
          ChoiceWindow::OnTouchBegin((int)piVar3,param_1);
        }
        else {
          DialogueWindow::OnTouchBegin(this->field_84,param_1);
        }
      }
      else {
        Layout::OnTouchBegin(*piVar3,param_1);
      }
    }
    else {
      iVar1 = Radio::lastMessageShown();
      iVar4 = DAT_000e746c;
      if (iVar1 != 0) {
        *(undefined1 *)&this->field_24 = 0;
        Status::nextCampaignMission(SUB41(**(undefined4 **)(iVar4 + 0xe72f2),0));
        uVar2 = **(uint **)(DAT_000e7470 + 0xe7300);
        **(undefined4 **)(DAT_000e7474 + 0xe7302) = 0;
        AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar2);
        *(undefined1 *)&this->m_nStarMapWindowOpen = 0;
      }
    }
  }
  return;
}

```

## target disasm
```
  000e72ac: push {r4,r5,r6,r7,lr}
  000e72ae: add r7,sp,#0xc
  000e72b0: push {r8,r9,r11}
  000e72b4: mov r4,r0
  000e72b6: ldr.w r0,[r0,#0x128]
  000e72ba: mov r5,r2
  000e72bc: mov r6,r1
  000e72be: cbz r0,0x000e72c6
  000e72c0: pop.w {r8,r9,r11}
  000e72c4: pop {r4,r5,r6,r7,pc}
  000e72c6: movs r0,#0x1
  000e72c8: strb.w r0,[r4,#0x118]
  000e72cc: str.w r3,[r4,#0x128]
  000e72d0: ldrb.w r0,[r4,#0x60]
  000e72d4: strd r6,r5,[r4,#0x110]
  000e72d8: cbz r0,0x000e7314
  000e72da: ldr r0,[r4,#0x54]
  000e72dc: blx 0x00075064
  000e72e0: cmp r0,#0x0
  000e72e2: beq 0x000e72c0
  000e72e4: ldr r0,[0x000e746c]
  000e72e6: movs r5,#0x0
  000e72e8: strb.w r5,[r4,#0x24]
  000e72ec: movs r1,#0x1
  000e72ee: add r0,pc
  000e72f0: ldr r0,[r0,#0x0]
  000e72f2: ldr r0,[r0,#0x0]
  000e72f4: blx 0x00075070
  000e72f8: ldr r0,[0x000e7470]
  000e72fa: ldr r1,[0x000e7474]
  000e72fc: add r0,pc
  000e72fe: add r1,pc
  000e7300: ldr r0,[r0,#0x0]
  000e7302: ldr r1,[r1,#0x0]
  000e7304: ldr r0,[r0,#0x0]
  000e7306: str r5,[r1,#0x0]
  000e7308: movs r1,#0x5
  000e730a: blx 0x00071d64
  000e730e: strb.w r5,[r4,#0x60]
  000e7312: b 0x000e72c0
  000e7314: ldr r0,[0x000e7478]
  000e7316: add r0,pc
  000e7318: ldr.w r8,[r0,#0x0]
  000e731c: ldr.w r0,[r8,#0x0]
  000e7320: ldrb r1,[r0,#0x0]
  000e7322: cbz r1,0x000e732e
  000e7324: mov r1,r6
  000e7326: mov r2,r5
  000e7328: blx 0x00074728
  000e732c: b 0x000e72c0
  000e732e: ldrb.w r0,[r4,#0x69]
  000e7332: cbz r0,0x000e7342
  000e7334: ldr.w r0,[r4,#0x84]
  000e7338: mov r1,r6
  000e733a: mov r2,r5
  000e733c: blx 0x0007507c
  000e7340: b 0x000e72c0
  000e7342: ldrb.w r0,[r4,#0x63]
  000e7346: cbz r0,0x000e734c
  000e7348: ldr r0,[r4,#0x70]
  000e734a: b 0x000e7356
  000e734c: ldrb.w r0,[r4,#0x6a]
  000e7350: cbz r0,0x000e7360
  000e7352: ldr.w r0,[r4,#0x88]
  000e7356: mov r1,r6
  000e7358: mov r2,r5
  000e735a: blx 0x0007471c
  000e735e: b 0x000e72c0
  000e7360: ldrb.w r0,[r4,#0x66]
  000e7364: cbz r0,0x000e7372
  000e7366: ldr r0,[r4,#0x78]
  000e7368: mov r1,r6
  000e736a: mov r2,r5
  000e736c: blx 0x00075088
  000e7370: b 0x000e72c0
  000e7372: ldrb.w r0,[r4,#0x65]
  000e7376: cbz r0,0x000e7384
  000e7378: ldr r0,[r4,#0x74]
  000e737a: mov r1,r6
  000e737c: mov r2,r5
  000e737e: blx 0x00075094
  000e7382: b 0x000e72c0
  000e7384: ldrb.w r0,[r4,#0x67]
  000e7388: cbz r0,0x000e7396
  000e738a: ldr r0,[r4,#0x10]
  000e738c: mov r1,r6
  000e738e: mov r2,r5
  000e7390: blx 0x00074704
  000e7394: b 0x000e72c0
  000e7396: ldrb.w r0,[r4,#0x68]
  000e739a: cbz r0,0x000e73a8
  000e739c: ldr r0,[r4,#0x7c]
  000e739e: mov r1,r6
  000e73a0: mov r2,r5
  000e73a2: blx 0x000750a0
  000e73a6: b 0x000e72c0
  000e73a8: ldrb.w r0,[r4,#0x64]
  000e73ac: cbz r0,0x000e73bc
  000e73ae: ldr.w r0,[r4,#0x80]
  000e73b2: mov r1,r6
  000e73b4: mov r2,r5
  000e73b6: blx 0x000750ac
  000e73ba: b 0x000e72c0
  000e73bc: ldrb.w r0,[r4,#0x62]
  000e73c0: cbz r0,0x000e73ce
  000e73c2: ldr r0,[r4,#0x50]
  000e73c4: mov r1,r6
  000e73c6: mov r2,r5
  000e73c8: blx 0x000750b8
  000e73cc: b 0x000e72c0
  000e73ce: ldrb.w r0,[r4,#0x61]
  000e73d2: cmp r0,#0x0
  000e73d4: beq.w 0x000e72c0
  000e73d8: ldr.w r0,[r4,#0x90]
  000e73dc: mov r1,r6
  000e73de: mov r2,r5
  000e73e0: blx 0x00074734
  000e73e4: ldr.w r0,[r4,#0x94]
  000e73e8: mov r1,r6
  000e73ea: mov r2,r5
  000e73ec: blx 0x00074734
  000e73f0: mov.w r9,#0x0
  000e73f4: b 0x000e740c
  000e73f6: ldr.w r0,[r4,#0x8c]
  000e73fa: mov r1,r6
  000e73fc: mov r2,r5
  000e73fe: ldr r0,[r0,#0x4]
  000e7400: ldr.w r0,[r0,r9,lsl #0x2]
  000e7404: blx 0x00074734
  000e7408: add.w r9,r9,#0x1
  000e740c: cmp.w r9,#0x5
  000e7410: bne 0x000e73f6
  000e7412: ldr.w r0,[r8,#0x0]
  000e7416: mov r1,r6
  000e7418: mov r2,r5
  000e741a: blx 0x00074728
  000e741e: ldr r0,[r4,#0x1c]
  000e7420: mov r1,r6
  000e7422: mov r2,r5
  000e7424: blx 0x000750c4
  000e7428: cmp r0,#0x0
  000e742a: bne.w 0x000e72c0
  000e742e: ldr.w r0,[r8,#0x0]
  000e7432: ldr r1,[r0,#0xc]
  000e7434: cmp r1,r5
  000e7436: bge.w 0x000e72c0
  000e743a: ldr r1,[0x000e747c]
  000e743c: ldr r0,[r0,#0x10]
  000e743e: add r1,pc
  000e7440: ldr r1,[r1,#0x0]
  000e7442: ldr r1,[r1,#0x0]
  000e7444: subs r0,r1,r0
  000e7446: cmp r0,r5
  000e7448: ble.w 0x000e72c0
  000e744c: ldr.w r0,[r4,#0xa8]
  000e7450: cmp r0,r6
  000e7452: bge.w 0x000e72c0
  000e7456: movs r0,#0x0
  000e7458: str.w r6,[r4,#0xfc]
  000e745c: str.w r6,[r4,#0xe8]
  000e7460: str.w r0,[r4,#0xf0]
  000e7464: movs r0,#0x1
  000e7466: strb.w r0,[r4,#0x100]
  000e746a: b 0x000e72c0
```
