# Globals::drawLines
elf 0xe4608 body 120
Sig: undefined __thiscall drawLines(Globals * this, uint param_1, Array * param_2, int param_3, int param_4, uint param_5, bool param_6)

## decompile
```c

/* Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int, unsigned int, bool) */

void __thiscall
Globals::drawLines(Globals *this,uint param_1,Array *param_2,int param_3,int param_4,uint param_5,
                  bool param_6)

{
  int *piVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  undefined3 in_stack_00000009;
  
  iVar3 = 0;
  puVar2 = *(uint **)(DAT_000f4680 + 0xf4622);
  piVar1 = *(int **)(DAT_000f4684 + 0xf462e);
  for (uVar4 = 0; uVar4 < *(uint *)param_2; uVar4 = uVar4 + 1) {
    if (_param_6 == 0) {
      iVar3 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar2,(String *)param_1);
      iVar3 = param_5 - iVar3;
    }
    AbyssEngine::PaintCanvas::DrawString
              (*puVar2,(String *)param_1,*(int *)(*(int *)(param_2 + 4) + uVar4 * 4),iVar3 + param_3
               ,SUB41(param_4,0));
    param_4 = param_4 + *(int *)(*piVar1 + 4);
  }
  return;
}

```

## target disasm
```
  000f4608: push {r4,r5,r6,r7,lr}
  000f460a: add r7,sp,#0xc
  000f460c: push {r5,r6,r7,r8,r9,r10,r11}
  000f4610: str r3,[sp,#0x8]
  000f4612: mov r5,r2
  000f4614: ldr r0,[0x000f4680]
  000f4616: mov r6,r1
  000f4618: ldr r4,[r7,#0x8]
  000f461a: mov.w r10,#0x0
  000f461e: add r0,pc
  000f4620: mov.w r11,#0x0
  000f4624: ldr.w r9,[r0,#0x0]
  000f4628: ldr r0,[0x000f4684]
  000f462a: add r0,pc
  000f462c: ldr.w r8,[r0,#0x0]
  000f4630: b 0x000f4674
  000f4632: ldr r0,[r7,#0x10]
  000f4634: cbnz r0,0x000f464c
  000f4636: ldr r0,[r5,#0x4]
  000f4638: mov r1,r6
  000f463a: ldr.w r2,[r0,r11,lsl #0x2]
  000f463e: ldr.w r0,[r9,#0x0]
  000f4642: blx 0x0006faa8
  000f4646: ldr r1,[r7,#0xc]
  000f4648: sub.w r10,r1,r0
  000f464c: ldr r0,[r5,#0x4]
  000f464e: movs r1,#0x0
  000f4650: ldr.w r2,[r0,r11,lsl #0x2]
  000f4654: str r1,[sp,#0x4]
  000f4656: ldr r1,[sp,#0x8]
  000f4658: ldr.w r0,[r9,#0x0]
  000f465c: add.w r3,r10,r1
  000f4660: mov r1,r6
  000f4662: str r4,[sp,#0x0]
  000f4664: blx 0x0006fe14
  000f4668: ldr.w r0,[r8,#0x0]
  000f466c: add.w r11,r11,#0x1
  000f4670: ldr r0,[r0,#0x4]
  000f4672: add r4,r0
  000f4674: ldr r0,[r5,#0x0]
  000f4676: cmp r11,r0
  000f4678: bcc 0x000f4632
  000f467a: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000f467e: pop {r4,r5,r6,r7,pc}
```
