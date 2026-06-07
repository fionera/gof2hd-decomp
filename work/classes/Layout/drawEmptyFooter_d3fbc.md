# Layout::drawEmptyFooter
elf 0xd3fbc body 196
Sig: undefined __stdcall drawEmptyFooter(bool param_1)

## decompile
```c

/* Layout::drawEmptyFooter(bool) */

void Layout::drawEmptyFooter(bool param_1)

{
  Layout *this;
  int iVar1;
  int in_r1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  uint *puVar6;
  undefined4 uVar7;
  
  this = (Layout *)(uint)param_1;
  puVar6 = *(uint **)(DAT_000e4080 + 0xe3fd2);
  AbyssEngine::PaintCanvas::SetColor(*puVar6);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar6);
  piVar5 = *(int **)(DAT_000e4084 + 0xe3ff8);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar6,*(int *)(this + 0x334),0,(uchar)*piVar5,'\x11');
  piVar4 = *(int **)(DAT_000e4088 + 0xe4016);
  piVar3 = *(int **)(DAT_000e408c + 0xe401c);
  iVar2 = *(int *)(*piVar3 + 0x10);
  drawBGPattern(this,*(uint *)(this + 0x344),iVar1,*piVar5 - iVar2,*piVar4 + iVar1 * -2,iVar2);
  uVar7 = 1;
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar6,*(uint *)(this + 0x334),*piVar4 - iVar1,
             *piVar5 - *(int *)(*piVar3 + 0x10),'\x01');
  if (in_r1 == 0) {
    return;
  }
  TouchButton::setVisible(*(TouchButton **)(this + 0x3b4),true);
  (*(code *)(DAT_001ac0b4 + 0x1ac0b8))(*(undefined4 *)(this + 0x3b4),uVar7,iVar2,&stack0xfffffff8);
  return;
}

```

## target disasm
```
  000e3fbc: push {r4,r5,r6,r7,lr}
  000e3fbe: add r7,sp,#0xc
  000e3fc0: push {r5,r6,r7,r8,r9,r10,r11}
  000e3fc4: mov r4,r0
  000e3fc6: ldr r0,[0x000e4080]
  000e3fc8: mov r8,r1
  000e3fca: ldr.w r1,[r4,#0x3b0]
  000e3fce: add r0,pc
  000e3fd0: ldr.w r11,[r0,#0x0]
  000e3fd4: ldr.w r0,[r11,#0x0]
  000e3fd8: blx 0x0006fac0
  000e3fdc: ldr.w r1,[r4,#0x340]
  000e3fe0: ldr.w r0,[r11,#0x0]
  000e3fe4: blx 0x00072d84
  000e3fe8: mov r9,r0
  000e3fea: ldr r0,[0x000e4084]
  000e3fec: ldr.w r1,[r4,#0x334]
  000e3ff0: mov.w r12,#0x21
  000e3ff4: add r0,pc
  000e3ff6: movs r2,#0x11
  000e3ff8: ldr.w r10,[r0,#0x0]
  000e3ffc: ldr.w r0,[r11,#0x0]
  000e4000: ldr.w r3,[r10,#0x0]
  000e4004: strd r2,r12,[sp,#0x0]
  000e4008: movs r2,#0x0
  000e400a: blx 0x00071d70
  000e400e: ldr r0,[0x000e4088]
  000e4010: ldr r1,[0x000e408c]
  000e4012: add r0,pc
  000e4014: ldr.w r3,[r10,#0x0]
  000e4018: add r1,pc
  000e401a: ldr r6,[r0,#0x0]
  000e401c: ldr r5,[r1,#0x0]
  000e401e: ldr r0,[r6,#0x0]
  000e4020: ldr r1,[r5,#0x0]
  000e4022: sub.w r0,r0,r9, lsl #0x1
  000e4026: ldr r2,[r1,#0x10]
  000e4028: ldr.w r1,[r4,#0x344]
  000e402c: subs r3,r3,r2
  000e402e: strd r0,r2,[sp,#0x0]
  000e4032: mov r0,r4
  000e4034: mov r2,r9
  000e4036: blx 0x00074e00
  000e403a: ldr r2,[r5,#0x0]
  000e403c: ldr.w r3,[r10,#0x0]
  000e4040: ldr r6,[r6,#0x0]
  000e4042: ldr r5,[r2,#0x10]
  000e4044: movs r2,#0x1
  000e4046: ldr.w r1,[r4,#0x334]
  000e404a: ldr.w r0,[r11,#0x0]
  000e404e: subs r3,r3,r5
  000e4050: str r2,[sp,#0x0]
  000e4052: sub.w r2,r6,r9
  000e4056: blx 0x00074e3c
  000e405a: cmp.w r8,#0x0
  000e405e: beq 0x000e407a
  000e4060: ldr.w r0,[r4,#0x3b4]
  000e4064: movs r1,#0x1
  000e4066: blx 0x00074e48
  000e406a: ldr.w r0,[r4,#0x3b4]
  000e406e: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000e4072: pop.w {r4,r5,r6,r7,lr}
  000e4076: b.w 0x001ac0a8
  000e407a: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000e407e: pop {r4,r5,r6,r7,pc}
```
