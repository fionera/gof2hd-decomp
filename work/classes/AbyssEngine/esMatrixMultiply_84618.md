# AbyssEngine::esMatrixMultiply
elf 0x84618 body 144
Sig: undefined __stdcall esMatrixMultiply(ESMatrix * param_1, ESMatrix * param_2, ESMatrix * param_3)

## decompile
```c

/* AbyssEngine::esMatrixMultiply(AbyssEngine::ESMatrix*, AbyssEngine::ESMatrix*,
   AbyssEngine::ESMatrix*) */

void AbyssEngine::esMatrixMultiply(ESMatrix *param_1,ESMatrix *param_2,ESMatrix *param_3)

{
  undefined8 uVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  int iVar8;
  ESMatrix *pEVar9;
  int *piVar10;
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined8 local_60 [8];
  int local_1c;
  
  piVar10 = *(int **)(DAT_000946a8 + 0x9462e);
  local_1c = *piVar10;
  auVar4 = *(undefined1 (*) [16])param_3;
  auVar5 = *(undefined1 (*) [16])(param_3 + 0x10);
  auVar7 = *(undefined1 (*) [16])(param_3 + 0x30);
  auVar6 = *(undefined1 (*) [16])(param_3 + 0x20);
  iVar8 = 0;
  while (iVar8 != 0x40) {
    pEVar9 = param_2 + iVar8;
    uVar1 = CONCAT44(*(undefined4 *)pEVar9,*(undefined4 *)pEVar9);
    auVar11._8_8_ = uVar1;
    auVar11._0_8_ = uVar1;
    FloatVectorMult(auVar11,auVar4,2,0x20);
    uVar1 = CONCAT44(*(undefined4 *)(pEVar9 + 4),*(undefined4 *)(pEVar9 + 4));
    auVar12._8_8_ = uVar1;
    auVar12._0_8_ = uVar1;
    FloatVectorMultiplyAccumulate(auVar12,auVar5,2,0x10);
    uVar1 = CONCAT44(*(undefined4 *)(pEVar9 + 8),*(undefined4 *)(pEVar9 + 8));
    auVar2._8_8_ = uVar1;
    auVar2._0_8_ = uVar1;
    FloatVectorMultiplyAccumulate(auVar2,auVar6,2,0x10);
    uVar1 = CONCAT44(*(undefined4 *)(pEVar9 + 0xc),*(undefined4 *)(pEVar9 + 0xc));
    auVar3._8_8_ = uVar1;
    auVar3._0_8_ = uVar1;
    auVar12 = FloatVectorMultiplyAccumulate(auVar3,auVar7,2,0x10);
    *(longlong *)((int)local_60 + iVar8) = auVar12._0_8_;
    *(longlong *)((int)local_60 + iVar8 + 8) = auVar12._8_8_;
    iVar8 = iVar8 + 0x10;
  }
  __aeabi_memcpy(param_1,local_60,0x40);
  if (*piVar10 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00094618: push {r4,r5,r6,r7,lr}
  0009461a: add r7,sp,#0xc
  0009461c: push.w r8
  00094620: sub sp,#0x48
  00094622: ldr r3,[0x000946a8]
  00094624: mov r4,r2
  00094626: mov.w r12,#0xc
  0009462a: add r3,pc
  0009462c: ldr.w r8,[r3,#0x0]
  00094630: ldr.w r3,[r8,#0x0]
  00094634: str r3,[sp,#0x44]
  00094636: movs r3,#0x30
  00094638: vld1.32 {d16,d17},[r4],r3
  0009463c: add.w r3,r2,#0x20
  00094640: adds r2,#0x10
  00094642: vld1.32 {d20,d21},[r2]
  00094646: movs r2,#0x0
  00094648: vld1.32 {d22,d23},[r4]
  0009464c: mov r4,sp
  0009464e: vld1.32 {d18,d19},[r3]
  00094652: b 0x00094686
  00094654: adds r5,r1,r2
  00094656: adds r3,r5,#0x4
  00094658: mov r6,r5
  0009465a: vld1.32 {d24[],d25[]},[r6:32],r12
  0009465e: vmul.f32 q12,q12,q8
  00094662: vld1.32 {d26[],d27[]},[r3:32]
  00094666: add.w r3,r5,#0x8
  0009466a: vmla.f32 q12,q13,q10
  0009466e: vld1.32 {d26[],d27[]},[r3:32]
  00094672: adds r3,r4,r2
  00094674: adds r2,#0x10
  00094676: vmla.f32 q12,q13,q9
  0009467a: vld1.32 {d26[],d27[]},[r6:32]
  0009467e: vmla.f32 q12,q13,q11
  00094682: vst1.32 {d24,d25},[r3]
  00094686: cmp r2,#0x40
  00094688: bne 0x00094654
  0009468a: mov r1,sp
  0009468c: movs r2,#0x40
  0009468e: blx 0x0006f1e4
  00094692: ldr r0,[sp,#0x44]
  00094694: ldr.w r1,[r8,#0x0]
  00094698: subs r0,r1,r0
  0009469a: ittt eq
  0009469c: add.eq sp,#0x48
  0009469e: pop.eq.w r8
  000946a2: pop.eq {r4,r5,r6,r7,pc}
  000946a4: blx 0x0006e824
```
