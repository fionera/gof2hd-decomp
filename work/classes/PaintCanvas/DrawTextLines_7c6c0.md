# PaintCanvas::DrawTextLines
elf 0x7c6c0 body 102
Sig: undefined __thiscall DrawTextLines(PaintCanvas * this, uint param_1, Array * param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int,
   bool) */

void __thiscall
AbyssEngine::PaintCanvas::DrawTextLines
          (PaintCanvas *this,uint param_1,Array *param_2,int param_3,int param_4,bool param_5)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined3 in_stack_00000005;
  
  iVar3 = 0;
  for (uVar2 = 0; uVar2 < *(uint *)param_2; uVar2 = uVar2 + 1) {
    if (_param_5 != 0) {
      iVar3 = GetTextWidth((uint)this,(String *)param_1);
      iVar3 = -(iVar3 >> 1);
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
  0008c6c0: push {r4,r5,r6,r7,lr}
  0008c6c2: add r7,sp,#0xc
  0008c6c4: push {r5,r6,r7,r8,r9,r10,r11}
  0008c6c8: ldr r6,[r7,#0x8]
  0008c6ca: mov r8,r3
  0008c6cc: mov r5,r2
  0008c6ce: mov r10,r1
  0008c6d0: mov r4,r0
  0008c6d2: mov.w r11,#0x0
  0008c6d6: mov.w r9,#0x0
  0008c6da: b 0x0008c71a
  0008c6dc: ldr r0,[r7,#0xc]
  0008c6de: cbz r0,0x0008c6f4
  0008c6e0: ldr r0,[r5,#0x4]
  0008c6e2: mov r1,r10
  0008c6e4: ldr.w r2,[r0,r9,lsl #0x2]
  0008c6e8: mov r0,r4
  0008c6ea: blx 0x0006faa8
  0008c6ee: movs r1,#0x0
  0008c6f0: sub.w r11,r1,r0, asr #0x1
  0008c6f4: ldr r0,[r5,#0x4]
  0008c6f6: add.w r3,r11,r8
  0008c6fa: mov r1,r10
  0008c6fc: ldr.w r2,[r0,r9,lsl #0x2]
  0008c700: movs r0,#0x0
  0008c702: str r0,[sp,#0x4]
  0008c704: mov r0,r4
  0008c706: str r6,[sp,#0x0]
  0008c708: blx 0x0006fe14
  0008c70c: mov r0,r4
  0008c70e: mov r1,r10
  0008c710: blx 0x0006fe20
  0008c714: add r6,r0
  0008c716: add.w r9,r9,#0x1
  0008c71a: ldr r0,[r5,#0x0]
  0008c71c: cmp r9,r0
  0008c71e: bcc 0x0008c6dc
  0008c720: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0008c724: pop {r4,r5,r6,r7,pc}
```
