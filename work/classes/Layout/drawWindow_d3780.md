# Layout::drawWindow
elf 0xd3780 body 228
Sig: undefined __thiscall drawWindow(Layout * this, String param_1, int param_2, int param_3, int param_4, int param_5, bool param_6)

## decompile
```c

/* Layout::drawWindow(AbyssEngine::String, int, int, int, int, bool) */

void __thiscall
Layout::drawWindow(Layout *this,String *param_2,int param_3,int param_4,int param_5,int param_6,
                  int param_7)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  int *piVar4;
  
  puVar3 = *(uint **)(DAT_000e3868 + 0xe3796);
  uVar1 = AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar3);
  if (param_7 != 0) {
    iVar2 = *(int *)(**(int **)(DAT_000e386c + 0xe37b0) + 8);
    drawBGPattern(this,*(uint *)(this + 0x324),param_3,iVar2 + param_4,param_5,param_6 - iVar2);
  }
  AbyssEngine::PaintCanvas::SetColor(*puVar3);
  piVar4 = *(int **)(DAT_000e3870 + 0xe37d8);
  iVar2 = *(int *)(*piVar4 + 8);
  AbyssEngine::PaintCanvas::DrawRectangle
            ((PaintCanvas *)*puVar3,param_3,iVar2 + param_4,param_5,param_6 - iVar2);
  AbyssEngine::PaintCanvas::SetColor(*puVar3);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar3,*(int *)(this + 0x32c),param_3);
  if ((*(int *)(param_2 + 8) != 0) &&
     (iVar2 = AbyssEngine::String::Compare(param_2,(char *)(DAT_000e3874 + 0xe381a)), iVar2 == 0)) {
    iVar2 = *piVar4;
    AbyssEngine::PaintCanvas::DrawString
              (*puVar3,(String *)**(undefined4 **)(DAT_000e3878 + 0xe382c),(int)param_2,
               *(int *)(iVar2 + 0x28) + param_3,
               (bool)(((char)param_4 + (char)(*(int *)(iVar2 + 8) / 2) + '\x01') -
                     (char)*(undefined4 *)(this + 0x3ac)));
  }
  (*(code *)(DAT_001ac094 + 0x1ac098))(*puVar3,uVar1);
  return;
}

```

## target disasm
```
  000e3780: push {r4,r5,r6,r7,lr}
  000e3782: add r7,sp,#0xc
  000e3784: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000e3788: str r1,[sp,#0x10]
  000e378a: mov r10,r0
  000e378c: ldr r0,[0x000e3868]
  000e378e: mov r11,r3
  000e3790: mov r4,r2
  000e3792: add r0,pc
  000e3794: ldr r5,[r0,#0x0]
  000e3796: ldr r0,[r5,#0x0]
  000e3798: blx 0x0006fa90
  000e379c: str r0,[sp,#0xc]
  000e379e: ldrd r6,r9,[r7,#0x8]
  000e37a2: ldr r0,[r7,#0x10]
  000e37a4: cbz r0,0x000e37c8
  000e37a6: ldr r0,[0x000e386c]
  000e37a8: ldr.w r1,[r10,#0x324]
  000e37ac: add r0,pc
  000e37ae: ldr r0,[r0,#0x0]
  000e37b0: ldr r0,[r0,#0x0]
  000e37b2: ldr r0,[r0,#0x8]
  000e37b4: sub.w r2,r9,r0
  000e37b8: add.w r3,r0,r11
  000e37bc: mov r0,r10
  000e37be: strd r6,r2,[sp,#0x0]
  000e37c2: mov r2,r4
  000e37c4: blx 0x00074e00
  000e37c8: ldr r0,[r5,#0x0]
  000e37ca: ldr r1,[0x000e3864]
  000e37cc: blx 0x0006fac0
  000e37d0: ldr r0,[0x000e3870]
  000e37d2: mov r3,r6
  000e37d4: add r0,pc
  000e37d6: ldr.w r8,[r0,#0x0]
  000e37da: ldr.w r0,[r8,#0x0]
  000e37de: ldr r1,[r0,#0x8]
  000e37e0: ldr r0,[r5,#0x0]
  000e37e2: sub.w r2,r9,r1
  000e37e6: str r2,[sp,#0x0]
  000e37e8: add.w r2,r1,r11
  000e37ec: mov r1,r4
  000e37ee: blx 0x00074e18
  000e37f2: ldr.w r1,[r10,#0x3b0]
  000e37f6: ldr r0,[r5,#0x0]
  000e37f8: blx 0x0006fac0
  000e37fc: ldr.w r1,[r10,#0x32c]
  000e3800: mov r2,r4
  000e3802: ldr r0,[r5,#0x0]
  000e3804: mov r3,r11
  000e3806: mov r9,r4
  000e3808: blx 0x00072dc0
  000e380c: ldr r4,[sp,#0x10]
  000e380e: ldr r0,[r4,#0x8]
  000e3810: cbz r0,0x000e3852
  000e3812: ldr r1,[0x000e3874]
  000e3814: mov r0,r4
  000e3816: add r1,pc
  000e3818: blx 0x0006fa3c
  000e381c: cbnz r0,0x000e3852
  000e381e: ldr r0,[0x000e3878]
  000e3820: mov.w r12,#0x0
  000e3824: ldr.w r2,[r8,#0x0]
  000e3828: add r0,pc
  000e382a: ldr.w r3,[r10,#0x3ac]
  000e382e: ldr r6,[r2,#0x8]
  000e3830: ldr r0,[r0,#0x0]
  000e3832: ldr r2,[r2,#0x28]
  000e3834: add.w r6,r6,r6, lsr #0x1f
  000e3838: ldr r1,[r0,#0x0]
  000e383a: add.w r6,r11,r6, asr #0x1
  000e383e: ldr r0,[r5,#0x0]
  000e3840: adds r6,#0x1
  000e3842: subs r3,r6,r3
  000e3844: strd r3,r12,[sp,#0x0]
  000e3848: add.w r3,r2,r9
  000e384c: mov r2,r4
  000e384e: blx 0x0006fe14
  000e3852: ldr r0,[r5,#0x0]
  000e3854: ldr r1,[sp,#0xc]
  000e3856: add sp,#0x14
  000e3858: pop.w {r8,r9,r10,r11}
  000e385c: pop.w {r4,r5,r6,r7,lr}
  000e3860: b.w 0x001ac088
```
