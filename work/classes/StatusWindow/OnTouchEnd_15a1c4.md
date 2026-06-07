# StatusWindow::OnTouchEnd
elf 0x15a1c4 body 642
Sig: undefined __thiscall OnTouchEnd(StatusWindow * this, int param_1, int param_2)

## decompile
```c

/* StatusWindow::OnTouchEnd(int, int) */

void __thiscall StatusWindow::OnTouchEnd(StatusWindow *this,int param_1,int param_2)

{
  undefined1 *puVar1;
  int iVar2;
  int iVar3;
  void *pvVar4;
  String *pSVar5;
  int iVar6;
  code *pcVar7;
  uint uVar8;
  undefined4 *puVar9;
  int *piVar10;
  int *piVar11;
  uint in_fpscr;
  undefined4 uVar12;
  undefined4 uVar13;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar10 = *(int **)(DAT_0016a498 + 0x16a1da);
  local_28 = *piVar10;
  iVar6 = *(int *)(this + 0x44);
  iVar2 = *(int *)(this + 0x38) + iVar6;
  uVar13 = VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
  if (iVar6 < 0) {
    iVar6 = -iVar6;
  }
  piVar11 = *(int **)(DAT_0016a49c + 0x16a1f8);
  uVar12 = DAT_0016a490;
  if (3 < iVar6) {
    uVar12 = uVar13;
  }
  *(undefined4 *)(this + 0x48) = DAT_0016a494;
  this[0x54] = (StatusWindow)0x0;
  *(int *)(this + 0x38) = iVar2;
  *(int *)(this + 0x40) = iVar2;
  *(undefined4 *)(this + 0x4c) = uVar12;
  iVar6 = Layout::OnTouchEnd((Layout *)*piVar11,param_1,param_2);
  if (iVar6 != 0) {
    uVar13 = 1;
    goto LAB_0016a430;
  }
  if (**(char **)(DAT_0016a4a0 + 0x16a238) == '\0') {
    for (uVar8 = 0; uVar8 < **(uint **)(this + 4); uVar8 = uVar8 + 1) {
      iVar6 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 4))[1] + uVar8 * 4),param_1);
      if (iVar6 != 0) {
        *(uint *)(this + 0x30) = uVar8;
        uVar13 = *(undefined4 *)(*(int *)(this + 0x68) + uVar8 * 4);
        *(undefined4 *)(this + 0x38) = 0;
        *(undefined4 *)(this + 0x58) = uVar13;
      }
    }
  }
  if (*(int *)(this + 0x30) == 1) {
    for (iVar6 = 0; iVar6 < *(int *)this; iVar6 = iVar6 + 1) {
      iVar2 = TouchButton::OnTouchEnd
                        (*(int *)(*(int *)(*(int *)(this + 8) + 4) + iVar6 * 4),param_1);
      if (iVar2 != 0) {
        puVar9 = *(undefined4 **)(DAT_0016a4a4 + 0x16a29e);
        iVar2 = Achievements::getMedals((Achievements *)*puVar9);
        iVar3 = Achievements::isEliteMedal((Achievements *)*puVar9,iVar6);
        if ((iVar3 != 0) || (*(int *)(iVar2 + iVar6 * 4) != 0)) {
          if (-1 < *(int *)(this + 0x34)) {
            TouchButton::setAlwaysPressed
                      (*(TouchButton **)
                        (*(int *)(*(int *)(this + 8) + 4) + *(int *)(this + 0x34) * 4),false);
          }
          *(int *)(this + 0x34) = iVar6;
          if (*(Array **)(this + 0x10) != (Array *)0x0) {
            ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x10));
            if (*(Array<AbyssEngine::String*> **)(this + 0x10) != (Array<AbyssEngine::String*> *)0x0
               ) {
              pvVar4 = (void *)Array<AbyssEngine::String*>::~Array
                                         (*(Array<AbyssEngine::String*> **)(this + 0x10));
              operator_delete(pvVar4);
            }
          }
          *(undefined4 *)(this + 0x10) = 0;
          pvVar4 = operator_new(0xc);
          Array<AbyssEngine::String*>::Array();
          *(void **)(this + 0x10) = pvVar4;
          if (iVar3 == 0) {
            iVar2 = *(int *)(iVar2 + *(int *)(this + 0x34) * 4);
          }
          else {
            iVar2 = 1;
          }
          uVar13 = **(undefined4 **)(DAT_0016a4a8 + 0x16a320);
          pSVar5 = (String *)GameText::getText(**(int **)(DAT_0016a4ac + 0x16a322));
          AbyssEngine::String::String(aSStack_40,pSVar5,false);
          iVar2 = Achievements::getValue((Achievements *)*puVar9,*(int *)(this + 0x34),iVar2);
          AbyssEngine::String::String(aSStack_58,iVar2);
          AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
          Status::replaceHash(aSStack_34,uVar13,aSStack_40,aSStack_4c);
          pcVar7 = *(code **)(DAT_0016a4b0 + 0x16a366);
          (*pcVar7)(aSStack_4c);
          (*pcVar7)(aSStack_58);
          (*pcVar7)(aSStack_40);
          getMedalHintText((int)aSStack_58);
          AbyssEngine::String::operator+=(aSStack_34,aSStack_58);
          AbyssEngine::String::~String(aSStack_58);
          Globals::getLineArray
                    (**(uint **)(DAT_0016a4b4 + 0x16a390),
                     (String *)**(undefined4 **)(DAT_0016a4b8 + 0x16a396),(int)aSStack_34,
                     (Array *)(*(int *)(this + 0x6c) + *(int *)(*piVar11 + 0x4c) * -2));
          TouchButton::setAlwaysPressed
                    (*(TouchButton **)(*(int *)(*(int *)(this + 8) + 4) + iVar6 * 4),true);
          AbyssEngine::String::~String(aSStack_34);
        }
        break;
      }
    }
  }
  iVar6 = Layout::helpPressed((Layout *)*piVar11);
  if (iVar6 != 0) {
    if (*(int *)(this + 0x30) == 1) {
      iVar6 = *piVar11;
      pSVar5 = (String *)GameText::getText(**(int **)(DAT_0016a4c0 + 0x16a40e));
      AbyssEngine::String::String(aSStack_70,pSVar5,false);
      Layout::initHelpWindow(iVar6,aSStack_70);
      puVar1 = &stack0xffffffe4;
    }
    else {
      if (*(int *)(this + 0x30) != 0) goto LAB_0016a42e;
      iVar6 = *piVar11;
      pSVar5 = (String *)GameText::getText(**(int **)(DAT_0016a4bc + 0x16a3e2));
      AbyssEngine::String::String(aSStack_64,pSVar5,false);
      Layout::initHelpWindow(iVar6,aSStack_64);
      puVar1 = &stack0xfffffff0;
    }
    AbyssEngine::String::~String((String *)(puVar1 + -0x54));
  }
LAB_0016a42e:
  uVar13 = 0;
LAB_0016a430:
  if (*piVar10 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar13);
}

```

## target disasm
```
  0016a1c4: push {r4,r5,r6,r7,lr}
  0016a1c6: add r7,sp,#0xc
  0016a1c8: push {r8,r9,r10,r11}
  0016a1cc: sub sp,#0x54
  0016a1ce: mov r4,r0
  0016a1d0: ldr r0,[0x0016a498]
  0016a1d2: mov r5,r1
  0016a1d4: mov r9,r2
  0016a1d6: add r0,pc
  0016a1d8: vldr.32 s0,[pc,#0x2b4]
  0016a1dc: ldr.w r10,[r0,#0x0]
  0016a1e0: ldr.w r0,[r10,#0x0]
  0016a1e4: str r0,[sp,#0x50]
  0016a1e6: ldr r1,[r4,#0x44]
  0016a1e8: ldr r0,[r4,#0x38]
  0016a1ea: ldr r2,[0x0016a49c]
  0016a1ec: cmp r1,#0x0
  0016a1ee: vmov s2,r1
  0016a1f2: mov r3,r1
  0016a1f4: add r2,pc
  0016a1f6: add r0,r1
  0016a1f8: vcvt.f32.s32 s2,s2
  0016a1fc: it mi
  0016a1fe: rsb.mi r3,r1
  0016a200: ldr.w r11,[r2,#0x0]
  0016a204: cmp r3,#0x3
  0016a206: ldr r3,[0x0016a494]
  0016a208: mov.w r2,#0x0
  0016a20c: mov r1,r5
  0016a20e: it gt
  0016a210: vmov.gt.f32 s0,s2
  0016a214: str r3,[r4,#0x48]
  0016a216: strb.w r2,[r4,#0x54]
  0016a21a: mov r2,r9
  0016a21c: str r0,[r4,#0x38]
  0016a21e: str r0,[r4,#0x40]
  0016a220: vstr.32 s0,[r4,#0x4c]
  0016a224: ldr.w r0,[r11,#0x0]
  0016a228: blx 0x0007477c
  0016a22c: cbz r0,0x0016a232
  0016a22e: movs r0,#0x1
  0016a230: b 0x0016a430
  0016a232: ldr r0,[0x0016a4a0]
  0016a234: add r0,pc
  0016a236: ldr r0,[r0,#0x0]
  0016a238: ldrb r0,[r0,#0x0]
  0016a23a: cbnz r0,0x0016a26c
  0016a23c: mov.w r8,#0x0
  0016a240: movs r6,#0x0
  0016a242: b 0x0016a264
  0016a244: ldr r0,[r0,#0x4]
  0016a246: mov r1,r5
  0016a248: mov r2,r9
  0016a24a: ldr.w r0,[r0,r6,lsl #0x2]
  0016a24e: blx 0x00074788
  0016a252: cbz r0,0x0016a262
  0016a254: ldr r0,[r4,#0x68]
  0016a256: str r6,[r4,#0x30]
  0016a258: ldr.w r0,[r0,r6,lsl #0x2]
  0016a25c: str.w r8,[r4,#0x38]
  0016a260: str r0,[r4,#0x58]
  0016a262: adds r6,#0x1
  0016a264: ldr r0,[r4,#0x4]
  0016a266: ldr r1,[r0,#0x0]
  0016a268: cmp r6,r1
  0016a26a: bcc 0x0016a244
  0016a26c: ldr r0,[r4,#0x30]
  0016a26e: cmp r0,#0x1
  0016a270: bne.w 0x0016a3c2
  0016a274: mov.w r8,#0x0
  0016a278: b 0x0016a290
  0016a27a: ldr r0,[r4,#0x8]
  0016a27c: mov r1,r5
  0016a27e: mov r2,r9
  0016a280: ldr r0,[r0,#0x4]
  0016a282: ldr.w r0,[r0,r8,lsl #0x2]
  0016a286: blx 0x00074788
  0016a28a: cbnz r0,0x0016a298
  0016a28c: add.w r8,r8,#0x1
  0016a290: ldr r0,[r4,#0x0]
  0016a292: cmp r8,r0
  0016a294: blt 0x0016a27a
  0016a296: b 0x0016a3c2
  0016a298: ldr r0,[0x0016a4a4]
  0016a29a: add r0,pc
  0016a29c: ldr.w r9,[r0,#0x0]
  0016a2a0: ldr.w r0,[r9,#0x0]
  0016a2a4: blx 0x00074ac4
  0016a2a8: mov r5,r0
  0016a2aa: ldr.w r0,[r9,#0x0]
  0016a2ae: mov r1,r8
  0016a2b0: blx 0x000756ac
  0016a2b4: mov r6,r0
  0016a2b6: cbnz r0,0x0016a2c2
  0016a2b8: ldr.w r0,[r5,r8,lsl #0x2]
  0016a2bc: cmp r0,#0x0
  0016a2be: beq.w 0x0016a3c2
  0016a2c2: ldr r0,[r4,#0x34]
  0016a2c4: cmp r0,#0x0
  0016a2c6: blt 0x0016a2d6
  0016a2c8: ldr r1,[r4,#0x8]
  0016a2ca: ldr r1,[r1,#0x4]
  0016a2cc: ldr.w r0,[r1,r0,lsl #0x2]
  0016a2d0: movs r1,#0x0
  0016a2d2: blx 0x00075acc
  0016a2d6: ldr r0,[r4,#0x10]
  0016a2d8: str r5,[sp,#0x4]
  0016a2da: cmp r0,#0x0
  0016a2dc: str.w r8,[r4,#0x34]
  0016a2e0: beq 0x0016a2f2
  0016a2e2: blx 0x0006facc
  0016a2e6: ldr r0,[r4,#0x10]
  0016a2e8: cbz r0,0x0016a2f2
  0016a2ea: blx 0x0006f64c
  0016a2ee: blx 0x0006eb48
  0016a2f2: movs r0,#0x0
  0016a2f4: str r0,[r4,#0x10]
  0016a2f6: movs r0,#0xc
  0016a2f8: blx 0x0006eb24
  0016a2fc: mov r5,r0
  0016a2fe: blx 0x0006f628
  0016a302: str r5,[r4,#0x10]
  0016a304: cmp r6,#0x0
  0016a306: ldr r1,[r4,#0x34]
  0016a308: beq 0x0016a30e
  0016a30a: movs r6,#0x1
  0016a30c: b 0x0016a314
  0016a30e: ldr r0,[sp,#0x4]
  0016a310: ldr.w r6,[r0,r1,lsl #0x2]
  0016a314: ldr r0,[0x0016a4a8]
  0016a316: add.w r1,r1,#0x610
  0016a31a: ldr r2,[0x0016a4ac]
  0016a31c: add r0,pc
  0016a31e: add r2,pc
  0016a320: ldr r0,[r0,#0x0]
  0016a322: ldr r2,[r2,#0x0]
  0016a324: ldr r5,[r0,#0x0]
  0016a326: ldr r0,[r2,#0x0]
  0016a328: blx 0x00072f70
  0016a32c: mov r1,r0
  0016a32e: add r0,sp,#0x38
  0016a330: movs r2,#0x0
  0016a332: blx 0x0006f028
  0016a336: ldr r1,[r4,#0x34]
  0016a338: ldr.w r0,[r9,#0x0]
  0016a33c: mov r2,r6
  0016a33e: blx 0x00072f04
  0016a342: mov r1,r0
  0016a344: add r0,sp,#0x20
  0016a346: blx 0x0006f658
  0016a34a: add r0,sp,#0x2c
  0016a34c: add r1,sp,#0x20
  0016a34e: movs r2,#0x0
  0016a350: blx 0x0006f028
  0016a354: add r0,sp,#0x44
  0016a356: add r2,sp,#0x38
  0016a358: add r3,sp,#0x2c
  0016a35a: mov r1,r5
  0016a35c: blx 0x00076d8c
  0016a360: ldr r0,[0x0016a4b0]
  0016a362: add r0,pc
  0016a364: ldr r5,[r0,#0x0]
  0016a366: add r0,sp,#0x2c
  0016a368: blx r5
  0016a36a: add r0,sp,#0x20
  0016a36c: blx r5
  0016a36e: add r0,sp,#0x38
  0016a370: blx r5
  0016a372: ldr r2,[r4,#0x34]
  0016a374: add r0,sp,#0x20
  0016a376: blx 0x0007777c
  0016a37a: add r0,sp,#0x44
  0016a37c: add r1,sp,#0x20
  0016a37e: blx 0x0006ef5c
  0016a382: add r0,sp,#0x20
  0016a384: blx 0x0006ee30
  0016a388: ldr r0,[0x0016a4b4]
  0016a38a: ldr r1,[0x0016a4b8]
  0016a38c: add r0,pc
  0016a38e: ldr.w r2,[r11,#0x0]
  0016a392: add r1,pc
  0016a394: ldr r3,[r4,#0x10]
  0016a396: ldr r0,[r0,#0x0]
  0016a398: ldr r1,[r1,#0x0]
  0016a39a: ldr r6,[r4,#0x6c]
  0016a39c: ldr r0,[r0,#0x0]
  0016a39e: ldr r1,[r1,#0x0]
  0016a3a0: ldr r2,[r2,#0x4c]
  0016a3a2: str r3,[sp,#0x0]
  0016a3a4: sub.w r3,r6,r2, lsl #0x1
  0016a3a8: add r2,sp,#0x44
  0016a3aa: blx 0x00076a44
  0016a3ae: ldr r0,[r4,#0x8]
  0016a3b0: ldr r0,[r0,#0x4]
  0016a3b2: ldr.w r0,[r0,r8,lsl #0x2]
  0016a3b6: movs r1,#0x1
  0016a3b8: blx 0x00075acc
  0016a3bc: add r0,sp,#0x44
  0016a3be: blx 0x0006ee30
  0016a3c2: ldr.w r0,[r11,#0x0]
  0016a3c6: blx 0x000747b8
  0016a3ca: cbz r0,0x0016a42e
  0016a3cc: ldr r0,[r4,#0x30]
  0016a3ce: cmp r0,#0x1
  0016a3d0: beq 0x0016a400
  0016a3d2: cbnz r0,0x0016a42e
  0016a3d4: ldr r0,[0x0016a4bc]
  0016a3d6: mov.w r1,#0x280
  0016a3da: ldr.w r4,[r11,#0x0]
  0016a3de: add r0,pc
  0016a3e0: ldr r0,[r0,#0x0]
  0016a3e2: ldr r0,[r0,#0x0]
  0016a3e4: blx 0x00072f70
  0016a3e8: add r5,sp,#0x14
  0016a3ea: mov r1,r0
  0016a3ec: movs r2,#0x0
  0016a3ee: mov r0,r5
  0016a3f0: blx 0x0006f028
  0016a3f4: mov r0,r4
  0016a3f6: mov r1,r5
  0016a3f8: blx 0x000747c4
  0016a3fc: add r0,sp,#0x14
  0016a3fe: b 0x0016a42a
  0016a400: ldr r0,[0x0016a4c0]
  0016a402: movw r1,#0x287
  0016a406: ldr.w r4,[r11,#0x0]
  0016a40a: add r0,pc
  0016a40c: ldr r0,[r0,#0x0]
  0016a40e: ldr r0,[r0,#0x0]
  0016a410: blx 0x00072f70
  0016a414: add r5,sp,#0x8
  0016a416: mov r1,r0
  0016a418: movs r2,#0x0
  0016a41a: mov r0,r5
  0016a41c: blx 0x0006f028
  0016a420: mov r0,r4
  0016a422: mov r1,r5
  0016a424: blx 0x000747c4
  0016a428: add r0,sp,#0x8
  0016a42a: blx 0x0006ee30
  0016a42e: movs r0,#0x0
  0016a430: ldr r1,[sp,#0x50]
  0016a432: ldr.w r2,[r10,#0x0]
  0016a436: subs r1,r2,r1
  0016a438: ittt eq
  0016a43a: add.eq sp,#0x54
  0016a43c: pop.eq.w {r8,r9,r10,r11}
  0016a440: pop.eq {r4,r5,r6,r7,pc}
  0016a442: blx 0x0006e824
```
