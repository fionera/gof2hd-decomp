# PaintCanvas::GetScreenPosition
elf 0x7bce8 body 88
Sig: undefined __stdcall GetScreenPosition(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector&) */

void AbyssEngine::PaintCanvas::GetScreenPosition(Vector *param_1,Vector *param_2)

{
  Vector *in_r2;
  undefined4 *puVar1;
  int *piVar2;
  undefined4 local_50 [5];
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined4 local_18;
  int local_14;
  
  uStack_20 = DAT_0008bd48;
  uStack_28 = DAT_0008bd40;
  uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_30 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_2c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar2 = *(int **)(DAT_0008bd50 + 0x8bcfc);
  local_14 = *piVar2;
  puVar1 = (undefined4 *)((uint)local_50 | 4);
  *puVar1 = 0;
  puVar1[1] = uStack_34;
  puVar1[2] = uStack_30;
  puVar1[3] = uStack_2c;
  local_38 = 0;
  local_50[0] = 0x3f800000;
  local_3c = 0x3f800000;
  local_18 = 0x3f800000;
  GetScreenPosition((PaintCanvas *)param_1,(Matrix *)local_50,param_2,in_r2);
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008bce8: push {r4,r6,r7,lr}
  0008bcea: add r7,sp,#0x8
  0008bcec: sub sp,#0x40
  0008bcee: mov r12,r2
  0008bcf0: mov r2,r1
  0008bcf2: ldr r1,[0x0008bd50]
  0008bcf4: vmov.i32 q9,#0x0
  0008bcf8: add r1,pc
  0008bcfa: ldr r4,[r1,#0x0]
  0008bcfc: adr r1,[0x8bd40]
  0008bcfe: vld1.64 {d16,d17},[r1]
  0008bd02: ldr r1,[r4,#0x0]
  0008bd04: str r1,[sp,#0x3c]
  0008bd06: mov r1,sp
  0008bd08: orr r3,r1,#0x4
  0008bd0c: vst1.32 {d18,d19},[r3]
  0008bd10: add.w r3,r1,#0x18
  0008bd14: vst1.64 {d18,d19},[r3]
  0008bd18: add.w r3,r1,#0x28
  0008bd1c: vst1.64 {d16,d17},[r3]
  0008bd20: mov.w r3,#0x3f800000
  0008bd24: str r3,[sp,#0x0]
  0008bd26: str r3,[sp,#0x14]
  0008bd28: str r3,[sp,#0x38]
  0008bd2a: mov r3,r12
  0008bd2c: blx 0x0006fdc0
  0008bd30: ldr r1,[sp,#0x3c]
  0008bd32: ldr r2,[r4,#0x0]
  0008bd34: subs r1,r2,r1
  0008bd36: itt eq
  0008bd38: add.eq sp,#0x40
  0008bd3a: pop.eq {r4,r6,r7,pc}
  0008bd3c: blx 0x0006e824
```
