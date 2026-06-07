# PaintCanvas::DrawTextLines
elf 0x7c726 body 102
Sig: undefined __thiscall DrawTextLines(PaintCanvas * this, uint param_1, Array * param_2, int param_3, int param_4, uint param_5, bool param_6)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int,
   unsigned int, bool) */

void __thiscall
AbyssEngine::PaintCanvas::DrawTextLines
          (PaintCanvas *this,uint param_1,Array *param_2,int param_3,int param_4,uint param_5,
          bool param_6)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined3 in_stack_00000009;
  
  iVar3 = 0;
  for (uVar2 = 0; uVar2 < *(uint *)param_2; uVar2 = uVar2 + 1) {
    if (_param_6 == 0) {
      iVar3 = GetTextWidth((uint)this,(String *)param_1);
      iVar3 = param_5 - iVar3;
    }
    DrawString((uint)this,(String *)param_1,*(int *)(*(int *)(param_2 + 4) + uVar2 * 4),
               iVar3 + param_3,SUB41(param_4,0));
    iVar1 = GetTextHeight((uint)this);
    param_4 = param_4 + iVar1;
  }
  return;
}

```

## target disasm
```
  0008c726: push {r4,r5,r6,r7,lr}
  0008c728: add r7,sp,#0xc
  0008c72a: push {r5,r6,r7,r8,r9,r10,r11}
  0008c72e: ldr r6,[r7,#0x8]
  0008c730: mov r8,r3
  0008c732: mov r5,r2
  0008c734: mov r11,r1
  0008c736: mov r4,r0
  0008c738: mov.w r10,#0x0
  0008c73c: mov.w r9,#0x0
  0008c740: b 0x0008c780
  0008c742: ldr r0,[r7,#0x10]
  0008c744: cbnz r0,0x0008c75a
  0008c746: ldr r0,[r5,#0x4]
  0008c748: mov r1,r11
  0008c74a: ldr.w r2,[r0,r9,lsl #0x2]
  0008c74e: mov r0,r4
  0008c750: blx 0x0006faa8
  0008c754: ldr r1,[r7,#0xc]
  0008c756: sub.w r10,r1,r0
  0008c75a: ldr r0,[r5,#0x4]
  0008c75c: add.w r3,r10,r8
  0008c760: mov r1,r11
  0008c762: ldr.w r2,[r0,r9,lsl #0x2]
  0008c766: movs r0,#0x0
  0008c768: str r0,[sp,#0x4]
  0008c76a: mov r0,r4
  0008c76c: str r6,[sp,#0x0]
  0008c76e: blx 0x0006fe14
  0008c772: mov r0,r4
  0008c774: mov r1,r11
  0008c776: blx 0x0006fe20
  0008c77a: add r6,r0
  0008c77c: add.w r9,r9,#0x1
  0008c780: ldr r0,[r5,#0x0]
  0008c782: cmp r9,r0
  0008c784: bcc 0x0008c742
  0008c786: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0008c78a: pop {r4,r5,r6,r7,pc}
```
