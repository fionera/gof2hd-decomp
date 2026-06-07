# MGame::freeCamTouchBegin
elf 0x178a28 body 202
Sig: undefined __stdcall freeCamTouchBegin(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::freeCamTouchBegin(int, int, void*) */

void MGame::freeCamTouchBegin(int param_1,int param_2,void *param_3)

{
  int iVar1;
  Vector *pVVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 in_r3;
  int *piVar5;
  uint in_fpscr;
  
  piVar5 = *(int **)(DAT_00188af4 + 0x188a3c);
  iVar1 = *piVar5;
  if (*(int *)(param_1 + 0x98) == 0) {
    if (*(int *)(param_1 + 0x9c) == 0) {
      *(undefined4 *)(param_1 + 0xa0) = 0;
    }
    VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(param_1 + 0x98) = in_r3;
    iVar4 = 0xa4;
  }
  else {
    if (999 < *(int *)(param_1 + 0xa0)) goto LAB_00188aca;
    *(undefined4 *)(param_1 + 0xbc) = 0;
    pVVar2 = (Vector *)TargetFollowCamera::getCamOffset(*(TargetFollowCamera **)(param_1 + 0xf4));
    uVar3 = AbyssEngine::AEMath::VectorLength(pVVar2);
    VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(param_1 + 0xbc) = uVar3;
    iVar4 = 0xb0;
    *(undefined4 *)(param_1 + 0x9c) = in_r3;
  }
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + iVar4),(Vector *)&stack0xffffffd0);
LAB_00188aca:
  *(int *)(param_1 + 0x124) = param_2;
  *(void **)(param_1 + 0x128) = param_3;
  *(int *)(param_1 + 0x154) = param_2;
  *(void **)(param_1 + 0x158) = param_3;
  *(undefined4 *)(param_1 + 0x134) = 0;
  *(undefined4 *)(param_1 + 0x138) = 0;
  *(undefined1 *)(param_1 + 0x15c) = 1;
  iVar4 = *piVar5;
  iVar1 = iVar4 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar4);
  }
  return;
}

```

## target disasm
```
  00188a28: push {r4,r5,r6,r7,lr}
  00188a2a: add r7,sp,#0xc
  00188a2c: push {r4,r5,r6,r7,r8,r9,r10}
  00188a30: mov r4,r0
  00188a32: ldr r0,[0x00188af4]
  00188a34: mov r8,r3
  00188a36: mov r5,r2
  00188a38: add r0,pc
  00188a3a: mov r6,r1
  00188a3c: ldr.w r9,[r0,#0x0]
  00188a40: ldr.w r0,[r9,#0x0]
  00188a44: str r0,[sp,#0xc]
  00188a46: ldr.w r0,[r4,#0x98]
  00188a4a: cbz r0,0x00188a94
  00188a4c: ldr.w r0,[r4,#0xa0]
  00188a50: cmp.w r0,#0x3e8
  00188a54: bge 0x00188aca
  00188a56: ldr.w r0,[r4,#0xf4]
  00188a5a: mov.w r10,#0x0
  00188a5e: str.w r10,[r4,#0xbc]
  00188a62: blx 0x00078160
  00188a66: blx 0x0006ec44
  00188a6a: vmov s0,r5
  00188a6e: vmov s2,r6
  00188a72: vcvt.f32.s32 s0,s0
  00188a76: vcvt.f32.s32 s2,s2
  00188a7a: str.w r0,[r4,#0xbc]
  00188a7e: add.w r0,r4,#0xb0
  00188a82: str.w r8,[r4,#0x9c]
  00188a86: vstr.32 s0,[sp,#0x4]
  00188a8a: vstr.32 s2,[sp]
  00188a8e: str.w r10,[sp,#0x8]
  00188a92: b 0x00188ac4
  00188a94: ldr.w r0,[r4,#0x9c]
  00188a98: cbnz r0,0x00188aa0
  00188a9a: movs r0,#0x0
  00188a9c: str.w r0,[r4,#0xa0]
  00188aa0: vmov s0,r5
  00188aa4: movs r0,#0x0
  00188aa6: vmov s2,r6
  00188aaa: vcvt.f32.s32 s0,s0
  00188aae: vcvt.f32.s32 s2,s2
  00188ab2: str.w r8,[r4,#0x98]
  00188ab6: str r0,[sp,#0x8]
  00188ab8: add.w r0,r4,#0xa4
  00188abc: vstr.32 s0,[sp,#0x4]
  00188ac0: vstr.32 s2,[sp]
  00188ac4: mov r1,sp
  00188ac6: blx 0x0006eb3c
  00188aca: movs r0,#0x0
  00188acc: strd r6,r5,[r4,#0x124]
  00188ad0: strd r6,r5,[r4,#0x154]
  00188ad4: strd r0,r0,[r4,#0x134]
  00188ad8: movs r0,#0x1
  00188ada: strb.w r0,[r4,#0x15c]
  00188ade: ldr r0,[sp,#0xc]
  00188ae0: ldr.w r1,[r9,#0x0]
  00188ae4: subs r0,r1,r0
  00188ae6: itt eq
  00188ae8: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00188aec: pop.eq {r4,r5,r6,r7,pc}
  00188aee: blx 0x0006e824
```
