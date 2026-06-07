# Layout::drawBox
elf 0xd4384 body 1306
Sig: undefined __thiscall drawBox(Layout * this, int param_1, int param_2, int param_3, int param_4, int param_5, String param_6, uchar param_7)

## decompile
```c

/* Layout::drawBox(int, int, int, int, int, AbyssEngine::String, unsigned char) */

void __thiscall
Layout::drawBox(Layout *this,undefined4 param_1,int param_2,int param_3,int param_4,int param_5,
               int param_7,uint param_8)

{
  Layout *this_00;
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  String *pSVar4;
  uint uVar5;
  char cVar6;
  uint *puVar7;
  int *piVar8;
  
  puVar7 = *(uint **)(DAT_000e4748 + 0xe439c);
  uVar1 = AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar7);
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  cVar6 = (char)param_3;
  switch(param_1) {
  case 0:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x348),param_2);
    drawBGPattern(this,*(uint *)(this + 0x34c),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    AbyssEngine::PaintCanvas::DrawImage2D
              ((PaintCanvas *)*puVar7,*(uint *)(this + 0x348),(param_4 + param_2) - iVar2,param_3,
               '\x01');
    if (*(int *)(param_7 + 8) == 0) goto switchD_000e43c2_default;
    piVar8 = *(int **)(DAT_000e474c + 0xe444a);
    iVar2 = *(int *)(*piVar8 + 0x44);
    if ((param_8 & 2) == 0) {
      if ((int)(param_8 << 0x1d) < 0) {
        iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                          (*puVar7,(String *)**(undefined4 **)(DAT_000e4750 + 0xe446a));
        iVar2 = param_4 / 2 - iVar2 / 2;
      }
    }
    else {
      iVar3 = AbyssEngine::PaintCanvas::GetTextWidth
                        (*puVar7,(String *)**(undefined4 **)(DAT_000e48d0 + 0xe47f2));
      iVar2 = (param_4 - iVar2) - iVar3;
    }
    pSVar4 = (String *)**(undefined4 **)(DAT_000e48d4 + 0xe480c);
    uVar5 = *puVar7;
    cVar6 = (cVar6 + (char)(*(int *)(*piVar8 + 0x1c) >> 1) + '\x01') -
            (char)*(undefined4 *)(this + 0x3ac);
    goto LAB_000e48a2;
  case 1:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x350),param_2);
    drawBGPattern(this,*(uint *)(this + 0x354),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    AbyssEngine::PaintCanvas::DrawImage2D
              ((PaintCanvas *)*puVar7,*(uint *)(this + 0x350),(param_4 + param_2) - iVar2,param_3,
               '\x01');
    if (*(int *)(param_7 + 8) == 0) goto switchD_000e43c2_default;
    piVar8 = *(int **)(DAT_000e48c0 + 0xe44f6);
    iVar2 = *(int *)(*piVar8 + 0x44);
    if ((param_8 & 2) == 0) {
      if ((int)(param_8 << 0x1d) < 0) {
        iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                          (*puVar7,(String *)**(undefined4 **)(DAT_000e48c8 + 0xe4516));
        iVar2 = param_4 / 2 - iVar2 / 2;
      }
    }
    else {
      iVar3 = AbyssEngine::PaintCanvas::GetTextWidth
                        (*puVar7,(String *)**(undefined4 **)(DAT_000e48c4 + 0xe4830));
      iVar2 = (param_4 - iVar2) - iVar3;
    }
    pSVar4 = (String *)**(undefined4 **)(DAT_000e48cc + 0xe484a);
    uVar5 = *puVar7;
    cVar6 = (cVar6 + (char)(*(int *)(*piVar8 + 0x5c) >> 1) + '\x01') -
            (char)*(undefined4 *)(this + 0x3ac);
    param_2 = iVar2 + param_2;
    goto LAB_000e48a8;
  case 2:
    drawBGPattern(this,*(uint *)(this + 0x324),param_2,param_3,param_4,param_5);
    goto switchD_000e43c2_default;
  case 3:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x358),param_2);
    drawBGPattern(this,*(uint *)(this + 0x35c),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    uVar5 = *(uint *)(this + 0x358);
    break;
  case 4:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x36c),param_2);
    drawBGPattern(this,*(uint *)(this + 0x370),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    uVar5 = *(uint *)(this + 0x36c);
    break;
  case 5:
    uVar5 = *(uint *)(this + 0x380);
    iVar2 = *(int *)(this + 900);
    goto LAB_000e4736;
  case 6:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x388),param_2);
    drawBGPattern(this,*(uint *)(this + 0x38c),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    AbyssEngine::PaintCanvas::DrawImage2D
              ((PaintCanvas *)*puVar7,*(uint *)(this + 0x388),(param_4 + param_2) - iVar2,param_3,
               '\x01');
    if (*(int *)(param_7 + 8) == 0) goto switchD_000e43c2_default;
    iVar2 = *(int *)(**(int **)(DAT_000e48d8 + 0xe463a) + 0x44);
    if ((param_8 & 2) == 0) {
      if ((int)(param_8 << 0x1d) < 0) {
        iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                          (*puVar7,(String *)**(undefined4 **)(DAT_000e48e0 + 0xe4656));
        iVar2 = param_4 / 2 - iVar2 / 2;
      }
    }
    else {
      iVar3 = AbyssEngine::PaintCanvas::GetTextWidth
                        (*puVar7,(String *)**(undefined4 **)(DAT_000e48dc + 0xe4874));
      iVar2 = (param_4 - iVar2) - iVar3;
    }
    cVar6 = (cVar6 + (char)(param_5 >> 1) + '\x01') - (char)*(undefined4 *)(this + 0x3ac);
    pSVar4 = (String *)**(undefined4 **)(DAT_000e48e4 + 0xe4894);
    uVar5 = *puVar7;
LAB_000e48a2:
    param_2 = iVar2 + param_2;
    goto LAB_000e48a8;
  case 7:
    piVar8 = *(int **)(DAT_000e48e8 + 0xe467c);
    drawBGPattern(this,*(uint *)(this + 0x324),param_2,*(int *)(*piVar8 + 8) + param_3,param_4,
                  param_5 - *(int *)(*piVar8 + 8));
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar7);
    this_00 = (Layout *)-iVar2;
    drawBGBorder(this_00,*(uint *)(this + 0x390),*(uint *)(this + 0x394),param_2,
                 *(int *)(*piVar8 + 8) + param_3,param_4,param_5 - *(int *)(*piVar8 + 8),
                 (int)this_00,(int)this_00);
    if (*(int *)(param_7 + 8) == 0) goto switchD_000e43c2_default;
    AbyssEngine::PaintCanvas::SetColor(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x32c),param_2);
    pSVar4 = (String *)**(undefined4 **)(DAT_000e48ec + 0xe4708);
    uVar5 = *puVar7;
    cVar6 = (cVar6 + (char)(*(int *)(*piVar8 + 8) / 2) + '\x01') -
            (char)*(undefined4 *)(this + 0x3ac);
    param_2 = *(int *)(*piVar8 + 0x28) + param_2;
LAB_000e48a8:
    AbyssEngine::PaintCanvas::DrawString(uVar5,pSVar4,param_7,param_2,(bool)cVar6);
    goto switchD_000e43c2_default;
  case 8:
    uVar5 = *(uint *)(this + 0x39c);
    iVar2 = *(int *)(this + 0x3a0);
LAB_000e4736:
    drawBGBorder((uint)this,uVar5,iVar2,param_2,param_3,param_4);
    goto switchD_000e43c2_default;
  case 9:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x360),param_2);
    drawBGPattern(this,*(uint *)(this + 0x364),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    uVar5 = *(uint *)(this + 0x360);
    break;
  case 10:
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar7,*(int *)(this + 0x368),param_2);
    drawBGPattern(this,*(uint *)(this + 0x370),iVar2 + param_2,param_3,param_4 + iVar2 * -2,param_5)
    ;
    uVar5 = *(uint *)(this + 0x368);
    break;
  default:
    goto switchD_000e43c2_default;
  }
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar7,uVar5,(param_4 + param_2) - iVar2,param_3,'\x01');
switchD_000e43c2_default:
  (*(code *)(DAT_001ac094 + 0x1ac098))(*puVar7,uVar1);
  return;
}

```

## target disasm
```
  000e4384: push {r4,r5,r6,r7,lr}
  000e4386: add r7,sp,#0xc
  000e4388: push {r8,r9,r10,r11}
  000e438c: sub sp,#0x2c
  000e438e: str r2,[sp,#0x28]
  000e4390: mov r4,r0
  000e4392: ldr r0,[0x000e4748]
  000e4394: mov r6,r3
  000e4396: mov r5,r1
  000e4398: add r0,pc
  000e439a: ldr.w r10,[r0,#0x0]
  000e439e: ldr.w r0,[r10,#0x0]
  000e43a2: blx 0x0006fa90
  000e43a6: str r0,[sp,#0x24]
  000e43a8: ldr.w r0,[r10,#0x0]
  000e43ac: ldr.w r1,[r4,#0x3b0]
  000e43b0: blx 0x0006fac0
  000e43b4: cmp r5,#0xa
  000e43b6: bhi.w 0x000e48ac
  000e43ba: ldrd r2,r1,[r7,#0x10]
  000e43be: ldrd r3,r9,[r7,#0x8]
  000e43c2: tbh [pc,r5]
  000e43dc: str r1,[sp,#0x18]
  000e43de: mov r11,r3
  000e43e0: ldr.w r1,[r4,#0x348]
  000e43e4: ldr.w r0,[r10,#0x0]
  000e43e8: str r2,[sp,#0x20]
  000e43ea: blx 0x00072d84
  000e43ee: ldr r5,[sp,#0x28]
  000e43f0: mov r8,r0
  000e43f2: ldr.w r0,[r10,#0x0]
  000e43f6: mov r3,r6
  000e43f8: ldr.w r1,[r4,#0x348]
  000e43fc: mov r2,r5
  000e43fe: blx 0x00072dc0
  000e4402: ldr.w r1,[r4,#0x34c]
  000e4406: sub.w r0,r11,r8, lsl #0x1
  000e440a: add.w r2,r8,r5
  000e440e: strd r0,r9,[sp,#0x0]
  000e4412: mov r0,r4
  000e4414: mov r3,r6
  000e4416: blx 0x00074e00
  000e441a: movs r2,#0x1
  000e441c: ldr.w r1,[r4,#0x348]
  000e4420: ldr.w r0,[r10,#0x0]
  000e4424: mov r3,r6
  000e4426: str r2,[sp,#0x0]
  000e4428: add.w r2,r11,r5
  000e442c: sub.w r2,r2,r8
  000e4430: str.w r11,[sp,#0x1c]
  000e4434: ldr r5,[sp,#0x20]
  000e4436: mov r11,r6
  000e4438: blx 0x00074e3c
  000e443c: ldr r0,[r5,#0x8]
  000e443e: cmp r0,#0x0
  000e4440: beq.w 0x000e48ac
  000e4444: ldr r0,[0x000e474c]
  000e4446: add r0,pc
  000e4448: ldr.w r9,[r0,#0x0]
  000e444c: ldr.w r0,[r9,#0x0]
  000e4450: ldr r6,[r0,#0x44]
  000e4452: ldr r0,[sp,#0x18]
  000e4454: tst r0,#0x2
  000e4458: bne.w 0x000e47ea
  000e445c: lsls r0,r0,#0x1d
  000e445e: bpl.w 0x000e4802
  000e4462: ldr r0,[0x000e4750]
  000e4464: mov r2,r5
  000e4466: add r0,pc
  000e4468: ldr r0,[r0,#0x0]
  000e446a: ldr r1,[r0,#0x0]
  000e446c: ldr.w r0,[r10,#0x0]
  000e4470: blx 0x0006faa8
  000e4474: ldr r1,[sp,#0x1c]
  000e4476: add.w r0,r0,r0, lsr #0x1f
  000e447a: add.w r1,r1,r1, lsr #0x1f
  000e447e: asrs r1,r1,#0x1
  000e4480: sub.w r6,r1,r0, asr #0x1
  000e4484: b 0x000e4802
  000e4486: str r1,[sp,#0x18]
  000e4488: mov r11,r3
  000e448a: ldr.w r1,[r4,#0x350]
  000e448e: ldr.w r0,[r10,#0x0]
  000e4492: str r2,[sp,#0x20]
  000e4494: blx 0x00072d84
  000e4498: ldr.w r8,[sp,#0x28]
  000e449c: mov r5,r0
  000e449e: ldr.w r0,[r10,#0x0]
  000e44a2: mov r3,r6
  000e44a4: ldr.w r1,[r4,#0x350]
  000e44a8: mov r2,r8
  000e44aa: blx 0x00072dc0
  000e44ae: ldr.w r1,[r4,#0x354]
  000e44b2: sub.w r0,r11,r5, lsl #0x1
  000e44b6: add.w r2,r5,r8
  000e44ba: strd r0,r9,[sp,#0x0]
  000e44be: mov r0,r4
  000e44c0: mov r3,r6
  000e44c2: blx 0x00074e00
  000e44c6: movs r2,#0x1
  000e44c8: ldr.w r1,[r4,#0x350]
  000e44cc: ldr.w r0,[r10,#0x0]
  000e44d0: mov r3,r6
  000e44d2: str r2,[sp,#0x0]
  000e44d4: add.w r2,r11,r8
  000e44d8: subs r2,r2,r5
  000e44da: str.w r11,[sp,#0x1c]
  000e44de: ldr.w r8,[sp,#0x20]
  000e44e2: blx 0x00074e3c
  000e44e6: ldr.w r0,[r8,#0x8]
  000e44ea: cmp r0,#0x0
  000e44ec: beq.w 0x000e48ac
  000e44f0: ldr r0,[0x000e48c0]
  000e44f2: add r0,pc
  000e44f4: ldr.w r9,[r0,#0x0]
  000e44f8: ldr.w r0,[r9,#0x0]
  000e44fc: ldr r5,[r0,#0x44]
  000e44fe: ldr r0,[sp,#0x18]
  000e4500: tst r0,#0x2
  000e4504: bne.w 0x000e4828
  000e4508: lsls r0,r0,#0x1d
  000e450a: bpl.w 0x000e4840
  000e450e: ldr r0,[0x000e48c8]
  000e4510: mov r2,r8
  000e4512: add r0,pc
  000e4514: ldr r0,[r0,#0x0]
  000e4516: ldr r1,[r0,#0x0]
  000e4518: ldr.w r0,[r10,#0x0]
  000e451c: blx 0x0006faa8
  000e4520: ldr r1,[sp,#0x1c]
  000e4522: add.w r0,r0,r0, lsr #0x1f
  000e4526: add.w r1,r1,r1, lsr #0x1f
  000e452a: asrs r1,r1,#0x1
  000e452c: sub.w r5,r1,r0, asr #0x1
  000e4530: b 0x000e4840
  000e4532: ldr.w r1,[r4,#0x324]
  000e4536: mov r0,r4
  000e4538: strd r3,r9,[sp,#0x0]
  000e453c: mov r3,r6
  000e453e: ldr r2,[sp,#0x28]
  000e4540: blx 0x00074e00
  000e4544: b 0x000e48ac
  000e4546: ldr.w r1,[r4,#0x358]
  000e454a: mov r11,r3
  000e454c: ldr.w r0,[r10,#0x0]
  000e4550: blx 0x00072d84
  000e4554: ldr r5,[sp,#0x28]
  000e4556: mov r8,r0
  000e4558: ldr.w r0,[r10,#0x0]
  000e455c: mov r3,r6
  000e455e: ldr.w r1,[r4,#0x358]
  000e4562: mov r2,r5
  000e4564: blx 0x00072dc0
  000e4568: ldr.w r1,[r4,#0x35c]
  000e456c: sub.w r0,r11,r8, lsl #0x1
  000e4570: add.w r2,r8,r5
  000e4574: strd r0,r9,[sp,#0x0]
  000e4578: mov r0,r4
  000e457a: mov r3,r6
  000e457c: blx 0x00074e00
  000e4580: ldr.w r1,[r4,#0x358]
  000e4584: b 0x000e47d2
  000e4586: ldr.w r1,[r4,#0x36c]
  000e458a: mov r11,r3
  000e458c: ldr.w r0,[r10,#0x0]
  000e4590: blx 0x00072d84
  000e4594: ldr r5,[sp,#0x28]
  000e4596: mov r8,r0
  000e4598: ldr.w r0,[r10,#0x0]
  000e459c: mov r3,r6
  000e459e: ldr.w r1,[r4,#0x36c]
  000e45a2: mov r2,r5
  000e45a4: blx 0x00072dc0
  000e45a8: ldr.w r1,[r4,#0x370]
  000e45ac: sub.w r0,r11,r8, lsl #0x1
  000e45b0: add.w r2,r8,r5
  000e45b4: strd r0,r9,[sp,#0x0]
  000e45b8: mov r0,r4
  000e45ba: mov r3,r6
  000e45bc: blx 0x00074e00
  000e45c0: ldr.w r1,[r4,#0x36c]
  000e45c4: b 0x000e47d2
  000e45c6: ldrd r1,r2,[r4,#0x380]
  000e45ca: b 0x000e4736
  000e45cc: str r1,[sp,#0x18]
  000e45ce: mov r11,r3
  000e45d0: ldr.w r1,[r4,#0x388]
  000e45d4: ldr.w r0,[r10,#0x0]
  000e45d8: str r2,[sp,#0x20]
  000e45da: blx 0x00072d84
  000e45de: ldr r5,[sp,#0x28]
  000e45e0: mov r8,r0
  000e45e2: ldr.w r0,[r10,#0x0]
  000e45e6: mov r3,r6
  000e45e8: ldr.w r1,[r4,#0x388]
  000e45ec: mov r2,r5
  000e45ee: blx 0x00072dc0
  000e45f2: ldr.w r1,[r4,#0x38c]
  000e45f6: sub.w r0,r11,r8, lsl #0x1
  000e45fa: add.w r2,r8,r5
  000e45fe: strd r0,r9,[sp,#0x0]
  000e4602: mov r0,r4
  000e4604: mov r3,r6
  000e4606: blx 0x00074e00
  000e460a: movs r2,#0x1
  000e460c: ldr.w r1,[r4,#0x388]
  000e4610: ldr.w r0,[r10,#0x0]
  000e4614: mov r3,r6
  000e4616: str r2,[sp,#0x0]
  000e4618: add.w r2,r11,r5
  000e461c: sub.w r2,r2,r8
  000e4620: str.w r11,[sp,#0x1c]
  000e4624: ldr r5,[sp,#0x20]
  000e4626: mov r11,r6
  000e4628: blx 0x00074e3c
  000e462c: ldr r0,[r5,#0x8]
  000e462e: cmp r0,#0x0
  000e4630: beq.w 0x000e48ac
  000e4634: ldr r0,[0x000e48d8]
  000e4636: add r0,pc
  000e4638: ldr r0,[r0,#0x0]
  000e463a: ldr r0,[r0,#0x0]
  000e463c: ldr r6,[r0,#0x44]
  000e463e: ldr r0,[sp,#0x18]
  000e4640: tst r0,#0x2
  000e4644: bne.w 0x000e486c
  000e4648: lsls r0,r0,#0x1d
  000e464a: bpl.w 0x000e4884
  000e464e: ldr r0,[0x000e48e0]
  000e4650: mov r2,r5
  000e4652: add r0,pc
  000e4654: ldr r0,[r0,#0x0]
  000e4656: ldr r1,[r0,#0x0]
  000e4658: ldr.w r0,[r10,#0x0]
  000e465c: blx 0x0006faa8
  000e4660: ldr r1,[sp,#0x1c]
  000e4662: add.w r0,r0,r0, lsr #0x1f
  000e4666: add.w r1,r1,r1, lsr #0x1f
  000e466a: asrs r1,r1,#0x1
  000e466c: sub.w r6,r1,r0, asr #0x1
  000e4670: b 0x000e4884
  000e4672: ldr r0,[0x000e48e8]
  000e4674: ldr.w r1,[r4,#0x324]
  000e4678: add r0,pc
  000e467a: ldr.w r11,[r0,#0x0]
  000e467e: ldr.w r0,[r11,#0x0]
  000e4682: ldr r0,[r0,#0x8]
  000e4684: sub.w r5,r9,r0
  000e4688: strd r3,r5,[sp,#0x0]
  000e468c: strd r3,r2,[sp,#0x1c]
  000e4690: adds r3,r0,r6
  000e4692: ldr.w r8,[sp,#0x28]
  000e4696: mov r0,r4
  000e4698: mov r2,r8
  000e469a: blx 0x00074e00
  000e469e: ldr.w r1,[r4,#0x394]
  000e46a2: ldr.w r0,[r10,#0x0]
  000e46a6: blx 0x00072d90
  000e46aa: ldr.w r3,[r11,#0x0]
  000e46ae: rsbs r0,r0
  000e46b0: str.w r11,[sp,#0x18]
  000e46b4: ldrd r1,r2,[r4,#0x390]
  000e46b8: ldr r3,[r3,#0x8]
  000e46ba: sub.w r5,r9,r3
  000e46be: add r3,r6
  000e46c0: str r3,[sp,#0x0]
  000e46c2: ldr r3,[sp,#0x1c]
  000e46c4: str r3,[sp,#0x4]
  000e46c6: mov r3,r8
  000e46c8: ldr.w r9,[sp,#0x20]
  000e46cc: strd r5,r0,[sp,#0x8]
  000e46d0: str r0,[sp,#0x10]
  000e46d2: blx 0x00074e30
  000e46d6: ldr.w r0,[r9,#0x8]
  000e46da: cmp r0,#0x0
  000e46dc: beq.w 0x000e48ac
  000e46e0: ldr.w r0,[r10,#0x0]
  000e46e4: mov.w r1,#0xffffffff
  000e46e8: blx 0x0006fac0
  000e46ec: ldr.w r8,[sp,#0x28]
  000e46f0: mov r3,r6
  000e46f2: ldr.w r1,[r4,#0x32c]
  000e46f6: ldr.w r0,[r10,#0x0]
  000e46fa: mov r2,r8
  000e46fc: blx 0x00072dc0
  000e4700: ldr r0,[0x000e48ec]
  000e4702: ldr r1,[sp,#0x18]
  000e4704: add r0,pc
  000e4706: ldr.w r3,[r4,#0x3ac]
  000e470a: movs r4,#0x0
  000e470c: ldr r2,[r1,#0x0]
  000e470e: ldr r0,[r0,#0x0]
  000e4710: ldr r5,[r2,#0x8]
  000e4712: ldr r2,[r2,#0x28]
  000e4714: ldr r1,[r0,#0x0]
  000e4716: add.w r5,r5,r5, lsr #0x1f
  000e471a: ldr.w r0,[r10,#0x0]
  000e471e: add.w r5,r6,r5, asr #0x1
  000e4722: adds r5,#0x1
  000e4724: subs r3,r5,r3
  000e4726: strd r3,r4,[sp,#0x0]
  000e472a: add.w r3,r2,r8
  000e472e: mov r2,r9
  000e4730: b 0x000e48a8
  000e4732: ldrd r1,r2,[r4,#0x39c]
  000e4736: strd r6,r3,[sp,#0x0]
  000e473a: mov r0,r4
  000e473c: str.w r9,[sp,#0x8]
  000e4740: ldr r3,[sp,#0x28]
  000e4742: blx 0x00074e78
  000e4746: b 0x000e48ac
  000e4754: ldr.w r1,[r4,#0x360]
  000e4758: mov r11,r3
  000e475a: ldr.w r0,[r10,#0x0]
  000e475e: blx 0x00072d84
  000e4762: ldr r5,[sp,#0x28]
  000e4764: mov r8,r0
  000e4766: ldr.w r0,[r10,#0x0]
  000e476a: mov r3,r6
  000e476c: ldr.w r1,[r4,#0x360]
  000e4770: mov r2,r5
  000e4772: blx 0x00072dc0
  000e4776: ldr.w r1,[r4,#0x364]
  000e477a: sub.w r0,r11,r8, lsl #0x1
  000e477e: add.w r2,r8,r5
  000e4782: strd r0,r9,[sp,#0x0]
  000e4786: mov r0,r4
  000e4788: mov r3,r6
  000e478a: blx 0x00074e00
  000e478e: ldr.w r1,[r4,#0x360]
  000e4792: b 0x000e47d2
  000e4794: ldr.w r1,[r4,#0x368]
  000e4798: mov r11,r3
  000e479a: ldr.w r0,[r10,#0x0]
  000e479e: blx 0x00072d84
  000e47a2: ldr r5,[sp,#0x28]
  000e47a4: mov r8,r0
  000e47a6: ldr.w r0,[r10,#0x0]
  000e47aa: mov r3,r6
  000e47ac: ldr.w r1,[r4,#0x368]
  000e47b0: mov r2,r5
  000e47b2: blx 0x00072dc0
  000e47b6: ldr.w r1,[r4,#0x370]
  000e47ba: sub.w r0,r11,r8, lsl #0x1
  000e47be: add.w r2,r8,r5
  000e47c2: strd r0,r9,[sp,#0x0]
  000e47c6: mov r0,r4
  000e47c8: mov r3,r6
  000e47ca: blx 0x00074e00
  000e47ce: ldr.w r1,[r4,#0x368]
  000e47d2: movs r2,#0x1
  000e47d4: ldr.w r0,[r10,#0x0]
  000e47d8: str r2,[sp,#0x0]
  000e47da: add.w r2,r11,r5
  000e47de: sub.w r2,r2,r8
  000e47e2: mov r3,r6
  000e47e4: blx 0x00074e3c
  000e47e8: b 0x000e48ac
  000e47ea: ldr r0,[0x000e48d0]
  000e47ec: mov r2,r5
  000e47ee: add r0,pc
  000e47f0: ldr r0,[r0,#0x0]
  000e47f2: ldr r1,[r0,#0x0]
  000e47f4: ldr.w r0,[r10,#0x0]
  000e47f8: blx 0x0006faa8
  000e47fc: ldr r1,[sp,#0x1c]
  000e47fe: subs r1,r1,r6
  000e4800: subs r6,r1,r0
  000e4802: ldr r0,[0x000e48d4]
  000e4804: ldr.w r2,[r9,#0x0]
  000e4808: add r0,pc
  000e480a: ldr.w r3,[r4,#0x3ac]
  000e480e: movs r4,#0x0
  000e4810: ldr r2,[r2,#0x1c]
  000e4812: ldr r0,[r0,#0x0]
  000e4814: add.w r2,r11,r2, asr #0x1
  000e4818: adds r2,#0x1
  000e481a: ldr r1,[r0,#0x0]
  000e481c: ldr.w r0,[r10,#0x0]
  000e4820: subs r2,r2,r3
  000e4822: strd r2,r4,[sp,#0x0]
  000e4826: b 0x000e48a2
  000e4828: ldr r0,[0x000e48c4]
  000e482a: mov r2,r8
  000e482c: add r0,pc
  000e482e: ldr r0,[r0,#0x0]
  000e4830: ldr r1,[r0,#0x0]
  000e4832: ldr.w r0,[r10,#0x0]
  000e4836: blx 0x0006faa8
  000e483a: ldr r1,[sp,#0x1c]
  000e483c: subs r1,r1,r5
  000e483e: subs r5,r1,r0
  000e4840: ldr r0,[0x000e48cc]
  000e4842: ldr.w r2,[r9,#0x0]
  000e4846: add r0,pc
  000e4848: ldr.w r3,[r4,#0x3ac]
  000e484c: movs r4,#0x0
  000e484e: ldr r2,[r2,#0x5c]
  000e4850: ldr r0,[r0,#0x0]
  000e4852: add.w r2,r6,r2, asr #0x1
  000e4856: adds r2,#0x1
  000e4858: ldr r1,[r0,#0x0]
  000e485a: ldr.w r0,[r10,#0x0]
  000e485e: subs r2,r2,r3
  000e4860: strd r2,r4,[sp,#0x0]
  000e4864: ldr r2,[sp,#0x28]
  000e4866: adds r3,r5,r2
  000e4868: mov r2,r8
  000e486a: b 0x000e48a8
  000e486c: ldr r0,[0x000e48dc]
  000e486e: mov r2,r5
  000e4870: add r0,pc
  000e4872: ldr r0,[r0,#0x0]
  000e4874: ldr r1,[r0,#0x0]
  000e4876: ldr.w r0,[r10,#0x0]
  000e487a: blx 0x0006faa8
  000e487e: ldr r1,[sp,#0x1c]
  000e4880: subs r1,r1,r6
  000e4882: subs r6,r1,r0
  000e4884: ldr r0,[0x000e48e4]
  000e4886: movs r3,#0x0
  000e4888: ldr.w r2,[r4,#0x3ac]
  000e488c: add.w r4,r11,r9, asr #0x1
  000e4890: add r0,pc
  000e4892: adds r4,#0x1
  000e4894: subs r2,r4,r2
  000e4896: ldr r0,[r0,#0x0]
  000e4898: ldr r1,[r0,#0x0]
  000e489a: ldr.w r0,[r10,#0x0]
  000e489e: strd r2,r3,[sp,#0x0]
  000e48a2: ldr r2,[sp,#0x28]
  000e48a4: adds r3,r6,r2
  000e48a6: mov r2,r5
  000e48a8: blx 0x0006fe14
  000e48ac: ldr.w r0,[r10,#0x0]
  000e48b0: ldr r1,[sp,#0x24]
  000e48b2: add sp,#0x2c
  000e48b4: pop.w {r8,r9,r10,r11}
  000e48b8: pop.w {r4,r5,r6,r7,lr}
  000e48bc: b.w 0x001ac088
```
