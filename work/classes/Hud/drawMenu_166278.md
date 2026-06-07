# Hud::drawMenu
elf 0x166278 body 604
Sig: undefined __stdcall drawMenu(int param_1)

## decompile
```c

/* Hud::drawMenu(int) */

void Hud::drawMenu(int param_1)

{
  int *piVar1;
  int iVar2;
  String *pSVar3;
  uchar uVar4;
  char cVar5;
  int *piVar6;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint *puVar11;
  int iVar12;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  int iVar7;
  
  piVar1 = *(int **)(DAT_001764f4 + 0x17628c);
  piVar6 = *(int **)(DAT_001764f8 + 0x17628e);
  local_28 = *piVar1;
  Layout::drawMask();
  puVar11 = *(uint **)(DAT_001764fc + 0x1762a6);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar11,*(int *)(param_1 + 0x298),
             *(int *)(param_1 + 0x3c4) + *(int *)(param_1 + 0x4cc));
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar11,*(int *)(param_1 + 0x35c),
             *(int *)(param_1 + 0x4cc) + *(int *)(param_1 + 0x3d4) + *(int *)(param_1 + 0x3dc) / 2,
             ((char)*(undefined4 *)(param_1 + 0x4d0) + (char)*(undefined4 *)(param_1 + 0x3c8) +
             (char)(*(int *)(param_1 + 0x3cc) / 2)) - (char)*(undefined4 *)(*piVar6 + 0x22c),'\x11')
  ;
  iVar7 = *(int *)(param_1 + 0x3c8) + *(int *)(param_1 + 0x4d0) + *(int *)(param_1 + 0x3cc);
  cVar5 = (char)iVar7;
  if ((*(int **)(param_1 + 0x18) != (int *)0x0) && (iVar2 = **(int **)(param_1 + 0x18), iVar2 != 0))
  {
    for (uVar8 = 0; cVar5 = (char)iVar7, uVar8 < iVar2 - 1U; uVar8 = uVar8 + 1) {
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar11,*(int *)(param_1 + 0x2a0),
                 *(int *)(param_1 + 0x3c4) + *(int *)(param_1 + 0x4cc));
      iVar7 = iVar7 + *(int *)(param_1 + 0x3d0);
      iVar2 = **(int **)(param_1 + 0x18);
    }
  }
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar11,*(int *)(param_1 + 0x29c),
             *(int *)(param_1 + 0x3c4) + *(int *)(param_1 + 0x4cc));
  if ((*(uint **)(param_1 + 0x18) != (uint *)0x0) &&
     (uVar8 = **(uint **)(param_1 + 0x18), uVar8 != 0)) {
    for (uVar9 = 0; uVar9 < uVar8; uVar9 = uVar9 + 1) {
      TouchButton::draw();
      uVar8 = **(uint **)(param_1 + 0x18);
    }
  }
  if (*(int *)(param_1 + 0x238) == 0) {
    Status::getShip();
    iVar7 = Ship::hasCloak();
    if (iVar7 == 0) {
      Status::getShip();
      iVar7 = Ship::hasJumpDrive();
      if (iVar7 == 0) goto LAB_001764be;
    }
    AbyssEngine::String::String(aSStack_40,(char *)(DAT_00176504 + 0x1763be),false);
    AbyssEngine::String::String(aSStack_4c,*(int *)(param_1 + 0x27c));
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::~String(aSStack_4c);
    AbyssEngine::String::~String(aSStack_40);
    iVar12 = *(int *)(param_1 + 0x4cc) + *(int *)(param_1 + 0x3d4) + *(int *)(param_1 + 0x3dc) / 2;
    uVar4 = cVar5 + (char)(*(int *)(*piVar6 + 0x30) / 2) + (char)*(undefined4 *)(*piVar6 + 0x288);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar11,*(int *)(param_1 + 0x374),iVar12,uVar4,'\x11');
    iVar7 = *piVar6;
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar11,*(int *)(param_1 + 0x370),iVar12 - *(int *)(iVar7 + 0x230),
               (char)*(undefined4 *)(iVar7 + 0x30) + uVar4 + (char)*(undefined4 *)(iVar7 + 0x28c),
               '\x11');
    iVar10 = *(int *)(*piVar6 + 0x230);
    pSVar3 = (String *)**(undefined4 **)(DAT_00176508 + 0x176460);
    uVar8 = *puVar11;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DHeight(uVar8);
    iVar2 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar11);
    AbyssEngine::PaintCanvas::DrawString
              (uVar8,pSVar3,(int)aSStack_34,iVar10 + iVar12,
               (bool)(((uVar4 + (char)(iVar7 / 2)) - (char)(iVar2 / 2)) +
                     (char)*(undefined4 *)(*piVar6 + 0x234)));
    AbyssEngine::String::~String(aSStack_34);
  }
LAB_001764be:
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00176278: push {r4,r5,r6,r7,lr}
  0017627a: add r7,sp,#0xc
  0017627c: push {r8,r9,r10,r11}
  00176280: sub sp,#0x44
  00176282: mov r4,r0
  00176284: ldr r0,[0x001764f4]
  00176286: ldr r1,[0x001764f8]
  00176288: add r0,pc
  0017628a: add r1,pc
  0017628c: ldr r0,[r0,#0x0]
  0017628e: ldr r5,[r1,#0x0]
  00176290: str r0,[sp,#0x18]
  00176292: ldr r1,[r0,#0x0]
  00176294: ldr r0,[r5,#0x0]
  00176296: str r1,[sp,#0x40]
  00176298: blx 0x0007696c
  0017629c: ldr r0,[0x001764fc]
  0017629e: ldr.w r1,[r4,#0x298]
  001762a2: add r0,pc
  001762a4: ldrd r2,r12,[r4,#0x3c4]
  001762a8: ldr.w r3,[r4,#0x4d0]
  001762ac: ldr.w r10,[r0,#0x0]
  001762b0: ldr.w r6,[r4,#0x4cc]
  001762b4: add r3,r12
  001762b6: ldr.w r0,[r10,#0x0]
  001762ba: add r2,r6
  001762bc: blx 0x00072dc0
  001762c0: ldr r2,[r5,#0x0]
  001762c2: mov.w r11,#0x44
  001762c6: str r5,[sp,#0x14]
  001762c8: mov.w r8,#0x11
  001762cc: ldr.w r1,[r4,#0x35c]
  001762d0: ldrd r12,lr,[r4,#0x3c8]
  001762d4: ldr.w r9,[r2,#0x22c]
  001762d8: ldr.w r2,[r4,#0x3dc]
  001762dc: ldr.w r5,[r4,#0x3d4]
  001762e0: ldr.w r3,[r4,#0x4cc]
  001762e4: ldr.w r6,[r4,#0x4d0]
  001762e8: add.w r2,r2,r2, lsr #0x1f
  001762ec: add r3,r5
  001762ee: ldr.w r0,[r10,#0x0]
  001762f2: add.w r2,r3,r2, asr #0x1
  001762f6: add.w r3,r6,r12
  001762fa: add.w r6,lr,lr, lsr #0x1f
  001762fe: strd r8,r11,[sp,#0x0]
  00176302: add.w r3,r3,r6, asr #0x1
  00176306: sub.w r3,r3,r9
  0017630a: blx 0x00071d70
  0017630e: ldrd r1,r2,[r4,#0x3c8]
  00176312: ldr.w r3,[r4,#0x4d0]
  00176316: ldr r0,[r4,#0x18]
  00176318: add r1,r3
  0017631a: adds r5,r1,r2
  0017631c: cbz r0,0x00176350
  0017631e: ldr r0,[r0,#0x0]
  00176320: cbz r0,0x00176350
  00176322: movs r6,#0x0
  00176324: b 0x0017634a
  00176326: ldr.w r2,[r4,#0x3c4]
  0017632a: ldr.w r3,[r4,#0x4cc]
  0017632e: ldr.w r1,[r4,#0x2a0]
  00176332: ldr.w r0,[r10,#0x0]
  00176336: add r2,r3
  00176338: mov r3,r5
  0017633a: blx 0x00072dc0
  0017633e: ldr r0,[r4,#0x18]
  00176340: adds r6,#0x1
  00176342: ldr.w r1,[r4,#0x3d0]
  00176346: add r5,r1
  00176348: ldr r0,[r0,#0x0]
  0017634a: subs r0,#0x1
  0017634c: cmp r6,r0
  0017634e: bcc 0x00176326
  00176350: ldr.w r2,[r4,#0x3c4]
  00176354: ldr.w r3,[r4,#0x4cc]
  00176358: ldr.w r1,[r4,#0x29c]
  0017635c: ldr.w r0,[r10,#0x0]
  00176360: add r2,r3
  00176362: mov r3,r5
  00176364: blx 0x00072dc0
  00176368: ldr r0,[r4,#0x18]
  0017636a: cbz r0,0x00176388
  0017636c: ldr r1,[r0,#0x0]
  0017636e: cbz r1,0x00176388
  00176370: movs r6,#0x0
  00176372: b 0x00176384
  00176374: ldr r0,[r0,#0x4]
  00176376: ldr.w r0,[r0,r6,lsl #0x2]
  0017637a: blx 0x000746e0
  0017637e: ldr r0,[r4,#0x18]
  00176380: adds r6,#0x1
  00176382: ldr r1,[r0,#0x0]
  00176384: cmp r6,r1
  00176386: bcc 0x00176374
  00176388: ldr.w r0,[r4,#0x238]
  0017638c: cmp r0,#0x0
  0017638e: bne.w 0x001764be
  00176392: ldr r0,[0x00176500]
  00176394: add r0,pc
  00176396: ldr r6,[r0,#0x0]
  00176398: ldr r0,[r6,#0x0]
  0017639a: blx 0x00071a58
  0017639e: blx 0x000779e0
  001763a2: cbnz r0,0x001763b4
  001763a4: ldr r0,[r6,#0x0]
  001763a6: blx 0x00071a58
  001763aa: blx 0x00071a64
  001763ae: cmp r0,#0x0
  001763b0: beq.w 0x001764be
  001763b4: ldr r1,[0x00176504]
  001763b6: add r0,sp,#0x28
  001763b8: movs r2,#0x0
  001763ba: add r1,pc
  001763bc: blx 0x0006ee18
  001763c0: ldr.w r1,[r4,#0x27c]
  001763c4: add r0,sp,#0x1c
  001763c6: blx 0x0006f658
  001763ca: add r0,sp,#0x34
  001763cc: add r1,sp,#0x28
  001763ce: add r2,sp,#0x1c
  001763d0: blx 0x0006ef98
  001763d4: ldr.w r8,[sp,#0x14]
  001763d8: add r0,sp,#0x1c
  001763da: blx 0x0006ee30
  001763de: add r0,sp,#0x28
  001763e0: blx 0x0006ee30
  001763e4: ldr.w r2,[r8,#0x0]
  001763e8: ldr.w r6,[r4,#0x3dc]
  001763ec: ldr.w r12,[r4,#0x3d4]
  001763f0: ldr.w r3,[r4,#0x4cc]
  001763f4: ldr.w lr,[r2,#0x30]
  001763f8: add.w r6,r6,r6, lsr #0x1f
  001763fc: add r3,r12
  001763fe: ldr.w r1,[r4,#0x374]
  00176402: add.w r11,r3,r6, asr #0x1
  00176406: ldr.w r0,[r10,#0x0]
  0017640a: add.w r3,lr,lr, lsr #0x1f
  0017640e: ldr.w r2,[r2,#0x288]
  00176412: add.w r3,r5,r3, asr #0x1
  00176416: add r3,r2
  00176418: movs r2,#0x14
  0017641a: mov.w r9,#0x11
  0017641e: strd r9,r2,[sp,#0x0]
  00176422: mov r2,r11
  00176424: mov r6,r3
  00176426: str r3,[sp,#0x10]
  00176428: blx 0x00071d70
  0017642c: ldr.w r2,[r8,#0x0]
  00176430: ldr.w r1,[r4,#0x370]
  00176434: ldr.w r0,[r10,#0x0]
  00176438: ldr r3,[r2,#0x30]
  0017643a: ldr.w r6,[r2,#0x230]
  0017643e: ldr.w r5,[r2,#0x28c]
  00176442: movs r2,#0x12
  00176444: strd r9,r2,[sp,#0x0]
  00176448: sub.w r2,r11,r6
  0017644c: ldr r6,[sp,#0x10]
  0017644e: add r3,r6
  00176450: add r3,r5
  00176452: blx 0x00071d70
  00176456: ldr r0,[0x00176508]
  00176458: ldr.w r1,[r8,#0x0]
  0017645c: add r0,pc
  0017645e: ldr.w r9,[r0,#0x0]
  00176462: ldr.w r6,[r1,#0x230]
  00176466: ldr.w r1,[r4,#0x374]
  0017646a: ldr.w r0,[r9,#0x0]
  0017646e: ldr.w r4,[r10,#0x0]
  00176472: str r0,[sp,#0xc]
  00176474: mov r0,r4
  00176476: blx 0x00072d90
  0017647a: mov r5,r0
  0017647c: ldr.w r1,[r9,#0x0]
  00176480: ldr.w r0,[r10,#0x0]
  00176484: blx 0x0006fe20
  00176488: ldr.w r1,[r8,#0x0]
  0017648c: add.w r3,r6,r11
  00176490: ldr.w r1,[r1,#0x234]
  00176494: add.w r6,r5,r5, lsr #0x1f
  00176498: ldr r5,[sp,#0x10]
  0017649a: add.w r0,r0,r0, lsr #0x1f
  0017649e: movs r2,#0x0
  001764a0: add.w r6,r5,r6, asr #0x1
  001764a4: sub.w r0,r6,r0, asr #0x1
  001764a8: add r0,r1
  001764aa: strd r0,r2,[sp,#0x0]
  001764ae: ldr r1,[sp,#0xc]
  001764b0: add r2,sp,#0x34
  001764b2: mov r0,r4
  001764b4: blx 0x0006fe14
  001764b8: add r0,sp,#0x34
  001764ba: blx 0x0006ee30
  001764be: ldr r0,[sp,#0x40]
  001764c0: ldr r1,[sp,#0x18]
  001764c2: ldr r1,[r1,#0x0]
  001764c4: subs r0,r1,r0
  001764c6: ittt eq
  001764c8: add.eq sp,#0x44
  001764ca: pop.eq.w {r8,r9,r10,r11}
  001764ce: pop.eq {r4,r5,r6,r7,pc}
  001764d0: blx 0x0006e824
```
