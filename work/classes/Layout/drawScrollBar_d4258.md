# Layout::drawScrollBar
elf 0xd4258 body 280
Sig: undefined __thiscall drawScrollBar(Layout * this, int param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* Layout::drawScrollBar(int, int, int, int, int) */

void __thiscall
Layout::drawScrollBar(Layout *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  code *pcVar5;
  uint *puVar6;
  int iVar7;
  
  puVar6 = *(uint **)(DAT_000e4378 + 0xe426e);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar6);
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar6);
  pcVar5 = *(code **)(DAT_000e437c + 0xe428e);
  (*pcVar5)(*puVar6,DAT_000e4370);
  (*pcVar5)(*puVar6,DAT_000e4374);
  AbyssEngine::PaintCanvas::DrawRectangle
            ((PaintCanvas *)*puVar6,param_1,*(int *)(this + 0x3e0) + param_2,
             *(int *)(**(int **)(DAT_000e4380 + 0xe42a2) + 0x48),
             param_3 + *(int *)(this + 0x3e0) * -2);
  (*pcVar5)(*puVar6,*(undefined4 *)(this + 0x3b0));
  iVar7 = param_5 + -1;
  if (param_5 + -1 <= iVar2 * 2) {
    iVar7 = iVar2 * 2;
  }
  iVar4 = param_4 + 1;
  if (param_3 <= iVar7 + iVar4) {
    iVar4 = (param_3 - iVar7) - *(int *)(this + 0x3e0);
  }
  iVar3 = *(int *)(this + 0x3e4);
  iVar7 = iVar7 + iVar3 * -4;
  iVar4 = iVar4 + iVar3 * 2;
  if (iVar2 * 2 < iVar7) {
    drawBGPattern(this,*(uint *)(this + 0x378),param_1 + 1 + iVar3,iVar2 + param_2 + iVar4,iVar1,
                  iVar7 + iVar2 * -2);
    iVar3 = *(int *)(this + 0x3e4);
  }
  else {
    iVar1 = param_3 + iVar2 * -2;
    if (iVar1 <= iVar4) {
      iVar4 = iVar1;
    }
  }
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar6,*(int *)(this + 0x374),iVar3 + param_1 + 1);
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar6,*(uint *)(this + 0x374),*(int *)(this + 0x3e4) + param_1 + 1,
             (iVar7 - iVar2) + param_2 + iVar4,'\x02');
  return;
}

```

## target disasm
```
  000e4258: push {r4,r5,r6,r7,lr}
  000e425a: add r7,sp,#0xc
  000e425c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000e4260: mov r4,r0
  000e4262: ldr r0,[0x000e4378]
  000e4264: mov r10,r1
  000e4266: ldr.w r1,[r4,#0x374]
  000e426a: add r0,pc
  000e426c: mov r9,r3
  000e426e: mov r8,r2
  000e4270: ldr r6,[r0,#0x0]
  000e4272: ldr r0,[r6,#0x0]
  000e4274: blx 0x00072d84
  000e4278: str r0,[sp,#0x8]
  000e427a: ldr r0,[r6,#0x0]
  000e427c: ldr.w r1,[r4,#0x374]
  000e4280: blx 0x00072d90
  000e4284: ldr r2,[0x000e437c]
  000e4286: mov r11,r0
  000e4288: ldr r1,[0x000e4370]
  000e428a: add r2,pc
  000e428c: ldr r0,[r6,#0x0]
  000e428e: ldr r5,[r2,#0x0]
  000e4290: blx r5
  000e4292: ldr r0,[r6,#0x0]
  000e4294: ldr r1,[0x000e4374]
  000e4296: blx r5
  000e4298: ldr r0,[0x000e4380]
  000e429a: ldr.w r1,[r4,#0x3e0]
  000e429e: add r0,pc
  000e42a0: str.w r8,[sp,#0x10]
  000e42a4: str.w r10,[sp,#0xc]
  000e42a8: ldr r0,[r0,#0x0]
  000e42aa: sub.w r2,r9,r1, lsl #0x1
  000e42ae: ldr r0,[r0,#0x0]
  000e42b0: ldr r3,[r0,#0x48]
  000e42b2: ldr r0,[r6,#0x0]
  000e42b4: str r2,[sp,#0x0]
  000e42b6: add.w r2,r1,r8
  000e42ba: mov r1,r10
  000e42bc: blx 0x00074e18
  000e42c0: ldr.w r1,[r4,#0x3b0]
  000e42c4: ldr r0,[r6,#0x0]
  000e42c6: blx r5
  000e42c8: ldrd r3,r0,[r7,#0x8]
  000e42cc: subs r1,r0,#0x1
  000e42ce: lsl.w r2,r11,#0x1
  000e42d2: cmp.w r1,r11, lsl #0x1
  000e42d6: it le
  000e42d8: mov.le r1,r2
  000e42da: adds r3,#0x1
  000e42dc: adds r0,r1,r3
  000e42de: cmp r0,r9
  000e42e0: ittt ge
  000e42e2: ldr.ge.w r0,[r4,#0x3e0]
  000e42e6: sub.ge.w r3,r9,r1
  000e42ea: sub.ge r3,r3,r0
  000e42ec: ldr.w r0,[r4,#0x3e4]
  000e42f0: sub.w r10,r1,r0, lsl #0x2
  000e42f4: add.w r8,r3,r0, lsl #0x1
  000e42f8: cmp r10,r2
  000e42fa: ble 0x000e432a
  000e42fc: sub.w r2,r10,r2
  000e4300: ldr r3,[sp,#0x8]
  000e4302: ldr.w r1,[r4,#0x378]
  000e4306: strd r3,r2,[sp,#0x0]
  000e430a: ldr r2,[sp,#0xc]
  000e430c: ldr r5,[sp,#0x10]
  000e430e: add.w r9,r2,#0x1
  000e4312: add.w r2,r9,r0
  000e4316: add.w r0,r11,r5
  000e431a: add.w r3,r0,r8
  000e431e: mov r0,r4
  000e4320: blx 0x00074e00
  000e4324: ldr.w r0,[r4,#0x3e4]
  000e4328: b 0x000e433c
  000e432a: sub.w r1,r9,r2
  000e432e: cmp r8,r1
  000e4330: it ge
  000e4332: mov.ge r8,r1
  000e4334: ldr r1,[sp,#0xc]
  000e4336: ldr r5,[sp,#0x10]
  000e4338: add.w r9,r1,#0x1
  000e433c: add.w r2,r0,r9
  000e4340: add r5,r8
  000e4342: ldr.w r1,[r4,#0x374]
  000e4346: ldr r0,[r6,#0x0]
  000e4348: mov r3,r5
  000e434a: blx 0x00072dc0
  000e434e: ldr.w r2,[r4,#0x3e4]
  000e4352: movs r3,#0x2
  000e4354: ldr.w r1,[r4,#0x374]
  000e4358: str r3,[sp,#0x0]
  000e435a: sub.w r3,r10,r11
  000e435e: ldr r0,[r6,#0x0]
  000e4360: add r3,r5
  000e4362: add r2,r9
  000e4364: blx 0x00074e3c
  000e4368: add sp,#0x14
  000e436a: pop.w {r8,r9,r10,r11}
  000e436e: pop {r4,r5,r6,r7,pc}
```
