# HackingGame::solvableInNSteps
elf 0x14efc8 body 216
Sig: undefined __thiscall solvableInNSteps(HackingGame * this, int param_1, int param_2, int param_3, int param_4, int * param_5)

## decompile
```c

/* HackingGame::solvableInNSteps(int, int, int, int, int*) */

void __thiscall
HackingGame::solvableInNSteps
          (HackingGame *this,int param_1,int param_2,int param_3,int param_4,int *param_5)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int *piVar9;
  int *piVar10;
  int local_54 [6];
  int local_3c [6];
  int local_24;
  
  piVar10 = local_3c;
  piVar9 = *(int **)(DAT_0015f0a0 + 0x15efe0);
  local_24 = *piVar9;
  for (iVar7 = 0; iVar4 = local_3c[1], iVar3 = local_3c[0], iVar2 = local_54[2], iVar6 = local_54[1]
      , iVar7 != 6; iVar7 = iVar7 + 1) {
    iVar6 = param_5[iVar7];
    local_54[iVar7] = iVar6;
    piVar10[iVar7] = iVar6;
  }
  uVar8 = 0;
  do {
    if (5 < uVar8) {
      uVar5 = 1;
      goto LAB_0015f08a;
    }
    iVar7 = uVar8 * 4;
    piVar1 = param_5 + uVar8;
    uVar8 = uVar8 + 1;
  } while (*piVar1 == *(int *)(this + iVar7 + 4));
  if (param_2 < param_1) {
    uVar8 = 0;
    do {
      if (1 < uVar8) goto LAB_0015f05e;
      if ((param_3 < 3) && (uVar8 == 0)) {
        iVar6 = param_3 + 1;
        local_3c[0] = local_3c[3];
        local_3c[1] = iVar3;
        local_3c[3] = local_3c[4];
        local_3c[4] = iVar4;
        this[0x128] = (HackingGame)0x1;
        *(undefined4 *)(this + 300) = 0;
        iVar7 = 0;
        goto LAB_0015f086;
      }
      uVar8 = uVar8 + 1;
    } while (2 < param_4);
    local_54[1] = local_54[4];
    local_54[2] = iVar6;
    local_54[4] = local_54[5];
    local_54[5] = iVar2;
    this[0x129] = (HackingGame)0x1;
    *(undefined4 *)(this + 300) = 0;
    iVar7 = param_4 + 1;
    iVar6 = 0;
    piVar10 = local_54;
LAB_0015f086:
    uVar5 = solvableInNSteps(this,param_1,param_2 + 1,iVar6,iVar7,piVar10);
  }
  else {
LAB_0015f05e:
    uVar5 = 0;
  }
LAB_0015f08a:
  if (*piVar9 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

```

## target disasm
```
  0015efc8: push {r4,r5,r6,r7,lr}
  0015efca: add r7,sp,#0xc
  0015efcc: push {r8,r9,r10}
  0015efd0: sub sp,#0x40
  0015efd2: ldr r4,[0x0015f0a0]
  0015efd4: add.w lr,sp,#0xc
  0015efd8: add.w r9,sp,#0x24
  0015efdc: add r4,pc
  0015efde: ldr.w r8,[r4,#0x0]
  0015efe2: movs r4,#0x0
  0015efe4: ldr.w r5,[r8,#0x0]
  0015efe8: ldrd r12,r10,[r7,#0x8]
  0015efec: str r5,[sp,#0x3c]
  0015efee: b 0x0015effe
  0015eff0: ldr.w r5,[r10,r4,lsl #0x2]
  0015eff4: str.w r5,[lr,r4,lsl #0x2]
  0015eff8: str.w r5,[r9,r4,lsl #0x2]
  0015effc: adds r4,#0x1
  0015effe: cmp r4,#0x6
  0015f000: bne 0x0015eff0
  0015f002: movs r4,#0x0
  0015f004: cmp r4,#0x5
  0015f006: bhi 0x0015f05a
  0015f008: add.w r6,r0,r4, lsl #0x2
  0015f00c: ldr.w r5,[r10,r4,lsl #0x2]
  0015f010: adds r4,#0x1
  0015f012: ldr r6,[r6,#0x4]
  0015f014: cmp r5,r6
  0015f016: beq 0x0015f004
  0015f018: cmp r2,r1
  0015f01a: bge 0x0015f05e
  0015f01c: movs r6,#0x0
  0015f01e: cmp r6,#0x1
  0015f020: bhi 0x0015f05e
  0015f022: cmp r3,#0x2
  0015f024: bgt 0x0015f028
  0015f026: cbz r6,0x0015f062
  0015f028: adds r6,#0x1
  0015f02a: cmp.w r12,#0x2
  0015f02e: bgt 0x0015f01e
  0015f030: ldrd r3,r4,[sp,#0x10]
  0015f034: adds r2,#0x1
  0015f036: ldrd r5,r6,[sp,#0x1c]
  0015f03a: strd r5,r3,[sp,#0x10]
  0015f03e: movs r3,#0x1
  0015f040: strd r6,r4,[sp,#0x1c]
  0015f044: strb.w r3,[r0,#0x129]
  0015f048: movs r3,#0x0
  0015f04a: str.w r3,[r0,#0x12c]
  0015f04e: add.w r3,r12,#0x1
  0015f052: strd r3,lr,[sp,#0x0]
  0015f056: movs r3,#0x0
  0015f058: b 0x0015f086
  0015f05a: movs r0,#0x1
  0015f05c: b 0x0015f08a
  0015f05e: movs r0,#0x0
  0015f060: b 0x0015f08a
  0015f062: ldrd r6,r12,[sp,#0x24]
  0015f066: adds r2,#0x1
  0015f068: ldrd r4,r5,[sp,#0x30]
  0015f06c: adds r3,#0x1
  0015f06e: strd r4,r6,[sp,#0x24]
  0015f072: movs r6,#0x1
  0015f074: strd r5,r12,[sp,#0x30]
  0015f078: strb.w r6,[r0,#0x128]
  0015f07c: movs r6,#0x0
  0015f07e: str.w r6,[r0,#0x12c]
  0015f082: strd r6,r9,[sp,#0x0]
  0015f086: blx 0x00077254
  0015f08a: ldr r1,[sp,#0x3c]
  0015f08c: ldr.w r2,[r8,#0x0]
  0015f090: subs r1,r2,r1
  0015f092: ittt eq
  0015f094: add.eq sp,#0x40
  0015f096: pop.eq.w {r8,r9,r10}
  0015f09a: pop.eq {r4,r5,r6,r7,pc}
  0015f09c: blx 0x0006e824
```
