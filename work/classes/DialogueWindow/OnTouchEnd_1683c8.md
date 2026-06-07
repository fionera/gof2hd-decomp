# DialogueWindow::OnTouchEnd
elf 0x1683c8 body 270
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* DialogueWindow::OnTouchEnd(int, int) */

undefined4 DialogueWindow::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  String *pSVar2;
  undefined4 uVar3;
  ChoiceWindow *this;
  int *piVar4;
  float extraout_s0;
  float fVar5;
  
  if (*(char *)(param_1 + 0x54) == '\0') {
    ScrollTouchWindow::OnTouchEnd(*(int *)(param_1 + 0x40),param_2);
    iVar1 = TouchButton::OnTouchEnd(*(int *)param_1,param_2);
    if (iVar1 != 0) {
      FModSound::stop(**(int **)(DAT_001784e4 + 0x178466));
      previousPage((DialogueWindow *)param_1);
    }
    iVar1 = TouchButton::OnTouchEnd(*(int *)(param_1 + 4),param_2);
    if (iVar1 != 0) {
      FModSound::stop(**(int **)(DAT_001784e8 + 0x178486));
      iVar1 = nextPage((DialogueWindow *)param_1);
      if (iVar1 == 0) goto LAB_001784c6;
    }
    iVar1 = TouchButton::OnTouchEnd(*(int *)(param_1 + 8),param_2);
    if (iVar1 != 0) {
      this = *(ChoiceWindow **)(param_1 + 0x50);
      pSVar2 = (String *)GameText::getText(**(int **)(DAT_001784ec + 0x1784ac));
      ChoiceWindow::set(this,pSVar2,true);
      *(undefined1 *)(param_1 + 0x54) = 1;
    }
    uVar3 = 0;
  }
  else {
    iVar1 = ChoiceWindow::OnTouchEnd(*(int *)(param_1 + 0x50),param_2);
    if (iVar1 == 1) {
      *(undefined1 *)(param_1 + 0x54) = 0;
      return 0;
    }
    if (iVar1 != 0) {
      return 0;
    }
    *(undefined1 *)(param_1 + 0x54) = 0;
    iVar1 = Status::getCurrentCampaignMission();
    if (iVar1 == 0xf) {
      piVar4 = *(int **)(DAT_001784dc + 0x178416);
      FModSound::play(*piVar4,(Vector *)0xa2,(Vector *)0x0,extraout_s0);
      fVar5 = (float)FModSound::stop(*piVar4);
      FModSound::play(*piVar4,(Vector *)0x88,(Vector *)0x0,fVar5);
    }
    if (*(int *)(param_1 + 100) != -1) {
      FModSound::stop(**(int **)(DAT_001784e0 + 0x178440));
    }
LAB_001784c6:
    uVar3 = 1;
  }
  return uVar3;
}

```

## target disasm
```
  001783c8: push {r4,r5,r6,r7,lr}
  001783ca: add r7,sp,#0xc
  001783cc: push.w r11
  001783d0: sub sp,#0x8
  001783d2: mov r4,r0
  001783d4: ldrb.w r0,[r0,#0x54]
  001783d8: mov r5,r2
  001783da: mov r6,r1
  001783dc: cbz r0,0x00178448
  001783de: ldr r0,[r4,#0x50]
  001783e0: mov r1,r6
  001783e2: mov r2,r5
  001783e4: blx 0x00074770
  001783e8: mov r1,r0
  001783ea: movs r0,#0x0
  001783ec: cmp r1,#0x1
  001783ee: beq 0x001784ca
  001783f0: cmp r1,#0x0
  001783f2: bne 0x001784ce
  001783f4: ldr r0,[0x001784d8]
  001783f6: movs r5,#0x0
  001783f8: strb.w r5,[r4,#0x54]
  001783fc: add r0,pc
  001783fe: ldr r0,[r0,#0x0]
  00178400: ldr r0,[r0,#0x0]
  00178402: blx 0x00071770
  00178406: cmp r0,#0xf
  00178408: bne 0x00178434
  0017840a: ldr r0,[0x001784dc]
  0017840c: movs r1,#0xa2
  0017840e: movs r2,#0x0
  00178410: movs r3,#0x0
  00178412: add r0,pc
  00178414: str r5,[sp,#0x0]
  00178416: ldr r6,[r0,#0x0]
  00178418: ldr r0,[r6,#0x0]
  0017841a: blx 0x00071548
  0017841e: ldr r0,[r6,#0x0]
  00178420: ldr r1,[r0,#0x0]
  00178422: blx 0x000724a8
  00178426: ldr r0,[r6,#0x0]
  00178428: movs r1,#0x88
  0017842a: movs r2,#0x0
  0017842c: movs r3,#0x0
  0017842e: str r5,[sp,#0x0]
  00178430: blx 0x00071548
  00178434: ldr r1,[r4,#0x64]
  00178436: adds r0,r1,#0x1
  00178438: beq 0x001784c6
  0017843a: ldr r0,[0x001784e0]
  0017843c: add r0,pc
  0017843e: ldr r0,[r0,#0x0]
  00178440: ldr r0,[r0,#0x0]
  00178442: blx 0x000724a8
  00178446: b 0x001784c6
  00178448: ldr r0,[r4,#0x40]
  0017844a: mov r1,r6
  0017844c: mov r2,r5
  0017844e: blx 0x00075bc8
  00178452: ldr r0,[r4,#0x0]
  00178454: mov r1,r6
  00178456: mov r2,r5
  00178458: blx 0x00074788
  0017845c: cbz r0,0x00178472
  0017845e: ldr r0,[0x001784e4]
  00178460: ldr r1,[r4,#0x64]
  00178462: add r0,pc
  00178464: ldr r0,[r0,#0x0]
  00178466: ldr r0,[r0,#0x0]
  00178468: blx 0x000724a8
  0017846c: mov r0,r4
  0017846e: blx 0x00077c74
  00178472: ldr r0,[r4,#0x4]
  00178474: mov r1,r6
  00178476: mov r2,r5
  00178478: blx 0x00074788
  0017847c: cbz r0,0x00178494
  0017847e: ldr r0,[0x001784e8]
  00178480: ldr r1,[r4,#0x64]
  00178482: add r0,pc
  00178484: ldr r0,[r0,#0x0]
  00178486: ldr r0,[r0,#0x0]
  00178488: blx 0x000724a8
  0017848c: mov r0,r4
  0017848e: blx 0x00077c68
  00178492: cbz r0,0x001784c6
  00178494: ldr r0,[r4,#0x8]
  00178496: mov r1,r6
  00178498: mov r2,r5
  0017849a: blx 0x00074788
  0017849e: cbz r0,0x001784c2
  001784a0: ldr r0,[0x001784ec]
  001784a2: mov.w r1,#0x18c
  001784a6: ldr r5,[r4,#0x50]
  001784a8: add r0,pc
  001784aa: ldr r0,[r0,#0x0]
  001784ac: ldr r0,[r0,#0x0]
  001784ae: blx 0x00072f70
  001784b2: mov r1,r0
  001784b4: mov r0,r5
  001784b6: movs r2,#0x1
  001784b8: movs r6,#0x1
  001784ba: blx 0x000746f8
  001784be: strb.w r6,[r4,#0x54]
  001784c2: movs r0,#0x0
  001784c4: b 0x001784ce
  001784c6: movs r0,#0x1
  001784c8: b 0x001784ce
  001784ca: strb.w r0,[r4,#0x54]
  001784ce: add sp,#0x8
  001784d0: pop.w r11
  001784d4: pop {r4,r5,r6,r7,pc}
```
