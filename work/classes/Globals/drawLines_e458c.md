# Globals::drawLines
elf 0xe458c body 114
Sig: undefined __stdcall drawLines(uint param_1, Array * param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int, bool) */

void Globals::drawLines(uint param_1,Array *param_2,int param_3,int param_4,bool param_5)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int in_stack_00000004;
  
  _param_5 = (uint)param_5;
  iVar3 = 0;
  puVar1 = *(uint **)(DAT_000f4600 + 0xf45a6);
  piVar2 = *(int **)(DAT_000f4604 + 0xf45b0);
  for (uVar4 = 0; uVar4 < *(uint *)param_3; uVar4 = uVar4 + 1) {
    if (in_stack_00000004 != 0) {
      iVar3 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar1,param_2);
      iVar3 = -(iVar3 >> 1);
    }
    AbyssEngine::PaintCanvas::DrawString
              (*puVar1,param_2,*(int *)(*(int *)(param_3 + 4) + uVar4 * 4),iVar3 + param_4,
               SUB41(_param_5,0));
    _param_5 = _param_5 + *(int *)(*piVar2 + 4);
  }
  return;
}

```

## target disasm
```
  000f458c: push {r4,r5,r6,r7,lr}
  000f458e: add r7,sp,#0xc
  000f4590: push {r5,r6,r7,r8,r9,r10,r11}
  000f4594: str r3,[sp,#0x8]
  000f4596: mov r5,r2
  000f4598: ldr r0,[0x000f4600]
  000f459a: mov r11,r1
  000f459c: ldr r4,[r7,#0x8]
  000f459e: mov.w r9,#0x0
  000f45a2: add r0,pc
  000f45a4: mov.w r10,#0x0
  000f45a8: ldr r6,[r0,#0x0]
  000f45aa: ldr r0,[0x000f4604]
  000f45ac: add r0,pc
  000f45ae: ldr.w r8,[r0,#0x0]
  000f45b2: b 0x000f45f2
  000f45b4: ldr r0,[r7,#0xc]
  000f45b6: cbz r0,0x000f45cc
  000f45b8: ldr r0,[r5,#0x4]
  000f45ba: mov r1,r11
  000f45bc: ldr.w r2,[r0,r10,lsl #0x2]
  000f45c0: ldr r0,[r6,#0x0]
  000f45c2: blx 0x0006faa8
  000f45c6: movs r1,#0x0
  000f45c8: sub.w r9,r1,r0, asr #0x1
  000f45cc: ldr r0,[r5,#0x4]
  000f45ce: movs r1,#0x0
  000f45d0: ldr.w r2,[r0,r10,lsl #0x2]
  000f45d4: str r1,[sp,#0x4]
  000f45d6: ldr r1,[sp,#0x8]
  000f45d8: ldr r0,[r6,#0x0]
  000f45da: add.w r3,r9,r1
  000f45de: mov r1,r11
  000f45e0: str r4,[sp,#0x0]
  000f45e2: blx 0x0006fe14
  000f45e6: ldr.w r0,[r8,#0x0]
  000f45ea: add.w r10,r10,#0x1
  000f45ee: ldr r0,[r0,#0x4]
  000f45f0: add r4,r0
  000f45f2: ldr r0,[r5,#0x0]
  000f45f4: cmp r10,r0
  000f45f6: bcc 0x000f45b4
  000f45f8: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000f45fc: pop {r4,r5,r6,r7,pc}
```
