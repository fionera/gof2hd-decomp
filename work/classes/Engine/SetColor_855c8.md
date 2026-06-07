# Engine::SetColor
elf 0x855c8 body 228
Sig: undefined __stdcall SetColor(float param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::Engine::SetColor(float, float, float, float) */

void AbyssEngine::Engine::SetColor(float param_1,float param_2,float param_3,float param_4)

{
  float fVar1;
  int iVar2;
  Engine *in_r0;
  float in_r1;
  char *pcVar3;
  float in_r2;
  float in_r3;
  float fVar4;
  float in_stack_00000000;
  
  iVar2 = DAT_000956b0;
  fVar1 = DAT_000956ac;
  if ((((*(float *)(in_r0 + 0xd0) == in_r1) && (*(float *)(in_r0 + 0xd4) == in_r2)) &&
      (*(float *)(in_r0 + 0xd8) == in_r3)) && (*(float *)(in_r0 + 0xdc) == in_stack_00000000)) {
    return;
  }
  fVar4 = in_r3 * DAT_000956ac;
  *(float *)(in_r0 + 0xd4) = in_r2;
  *(float *)(in_r0 + 0xd0) = in_r1;
  pcVar3 = *(char **)(iVar2 + 0x9563c);
  *(float *)(in_r0 + 0xd8) = in_r3;
  *(float *)(in_r0 + 0xdc) = in_stack_00000000;
  *(int *)(in_r0 + 0xe0) =
       (int)(in_r2 * fVar1) * 0x10000 + (int)(in_r1 * fVar1) * 0x1000000 + (int)fVar4 * 0x100 +
       (int)(in_stack_00000000 * fVar1);
  if (*pcVar3 == '\0') {
    LightSetMaterialColorAlpha(in_r0,in_stack_00000000);
    (*(code *)(&LAB_001ab678 + DAT_001ab674))();
    return;
  }
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  000955c8: push {r4,r5,r6,r7,lr}
  000955ca: add r7,sp,#0xc
  000955cc: push.w r8
  000955d0: vmov s4,r1
  000955d4: vldr.32 s8,[r0,#0xd0]
  000955d8: vldr.32 s0,[r7,#0x8]
  000955dc: vmov s2,r3
  000955e0: vcmpe.f32 s8,s4
  000955e4: mov r8,r3
  000955e6: vmov s6,r2
  000955ea: mov r5,r2
  000955ec: mov r6,r1
  000955ee: vmrs apsr,fpscr
  000955f2: bne 0x0009561e
  000955f4: vldr.32 s8,[r0,#0xd4]
  000955f8: vcmpe.f32 s8,s6
  000955fc: vmrs apsr,fpscr
  00095600: bne 0x0009561e
  00095602: vldr.32 s8,[r0,#0xd8]
  00095606: vcmpe.f32 s8,s2
  0009560a: vmrs apsr,fpscr
  0009560e: bne 0x0009561e
  00095610: vldr.32 s8,[r0,#0xdc]
  00095614: vcmpe.f32 s8,s0
  00095618: vmrs apsr,fpscr
  0009561c: beq 0x000956a6
  0009561e: vldr.32 s8,[pc,#0x8c]
  00095622: ldr r1,[0x000956b0]
  00095624: vmul.f32 s10,s2,s8
  00095628: vstr.32 s6,[r0,#0xd4]
  0009562c: vmul.f32 s12,s0,s8
  00095630: vstr.32 s4,[r0,#0xd0]
  00095634: vmul.f32 s14,s6,s8
  00095638: add r1,pc
  0009563a: vmul.f32 s8,s4,s8
  0009563e: ldr r1,[r1,#0x0]
  00095640: vcvt.s32.f32 s4,s10
  00095644: vstr.32 s2,[r0,#0xd8]
  00095648: vcvt.s32.f32 s2,s12
  0009564c: vstr.32 s0,[r0,#0xdc]
  00095650: vcvt.s32.f32 s6,s14
  00095654: vcvt.s32.f32 s8,s8
  00095658: vmov r2,s6
  0009565c: vmov r3,s8
  00095660: lsls r2,r2,#0x10
  00095662: add.w r2,r2,r3, lsl #0x18
  00095666: vmov r3,s4
  0009566a: add.w r2,r2,r3, lsl #0x8
  0009566e: vmov r3,s2
  00095672: add r2,r3
  00095674: str.w r2,[r0,#0xe0]
  00095678: ldrb r1,[r1,#0x0]
  0009567a: cbz r1,0x00095688
  0009567c: pop.w r8
  00095680: pop.w {r4,r5,r6,r7,lr}
  00095684: b.w 0x001ab658
  00095688: vmov r4,s0
  0009568c: mov r1,r4
  0009568e: blx 0x0007078c
  00095692: mov r0,r6
  00095694: mov r1,r5
  00095696: mov r2,r8
  00095698: mov r3,r4
  0009569a: pop.w r8
  0009569e: pop.w {r4,r5,r6,r7,lr}
  000956a2: b.w 0x001ab668
  000956a6: pop.w r8
  000956aa: pop {r4,r5,r6,r7,pc}
```
