# HackingGame::reInit
elf 0x14ed9c body 404
Sig: undefined __thiscall reInit(HackingGame * this)

## decompile
```c

/* HackingGame::reInit() */

void __thiscall HackingGame::reInit(HackingGame *this)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int local_40 [6];
  int local_28;
  
  piVar1 = *(int **)(DAT_0015ef30 + 0x15edb0);
  local_28 = *piVar1;
  iVar2 = *(int *)this;
  *(undefined4 *)(this + 300) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  if (iVar2 == 1) {
    for (uVar5 = 0; uVar5 != 6; uVar5 = uVar5 + 1) {
      *(uint *)(this + uVar5 * 4 + 4) = uVar5 >> 1;
    }
  }
  else {
    if (iVar2 == 2) {
      for (iVar2 = 0; iVar2 != 4; iVar2 = iVar2 + 1) {
        *(int *)(this + iVar2 * 4 + 4) = iVar2;
      }
      piVar7 = *(int **)(DAT_0015ef34 + 0x15ee10);
      uVar3 = AbyssEngine::AERandom::nextInt(*piVar7);
      *(undefined4 *)(this + 0x14) = uVar3;
      iVar2 = *piVar7;
    }
    else {
      if (iVar2 != 3) {
        for (iVar2 = 0; iVar2 != 6; iVar2 = iVar2 + 1) {
          *(int *)(this + iVar2 * 4 + 4) = iVar2;
        }
        goto LAB_0015ee34;
      }
      for (iVar2 = 0; iVar2 != 5; iVar2 = iVar2 + 1) {
        *(int *)(this + iVar2 * 4 + 4) = iVar2;
      }
      iVar2 = **(int **)(DAT_0015ef38 + 0x15ede0);
    }
    uVar3 = AbyssEngine::AERandom::nextInt(iVar2);
    *(undefined4 *)(this + 0x18) = uVar3;
  }
LAB_0015ee34:
  piVar7 = *(int **)(DAT_0015ef3c + 0x15ee3c);
  for (iVar2 = 0x28; iVar2 != 0; iVar2 = iVar2 + -1) {
    iVar6 = AbyssEngine::AERandom::nextInt(*piVar7);
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar7);
    uVar3 = *(undefined4 *)(this + iVar6 * 4 + 4);
    *(undefined4 *)(this + iVar6 * 4 + 4) = *(undefined4 *)(this + iVar4 * 4 + 4);
    *(undefined4 *)(this + iVar4 * 4 + 4) = uVar3;
  }
  for (iVar2 = 0; iVar2 != 6; iVar2 = iVar2 + 1) {
    *(undefined4 *)(this + iVar2 * 4 + 0x1c) = *(undefined4 *)(this + iVar2 * 4 + 4);
    *(undefined4 *)(this + iVar2 * 4 + 0x34) = *(undefined4 *)(this + iVar2 * 4 + 4);
  }
  for (uVar5 = 0; (int)uVar5 < *(int *)this * 2; uVar5 = uVar5 + 1) {
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
    for (iVar6 = 0; iVar6 <= iVar2; iVar6 = iVar6 + 1) {
      if ((uVar5 & 1) == 0) {
        rotateRightCW(SUB41(this,0));
      }
      else {
        rotateLeftCW(SUB41(this,0));
      }
    }
    iVar2 = *(int *)this;
    if (uVar5 == iVar2 * 2 - 1U) {
      for (iVar6 = 0; iVar6 != 6; iVar6 = iVar6 + 1) {
        local_40[iVar6] = *(int *)(this + iVar6 * 4 + 0x34);
      }
      iVar2 = solvableInNSteps(this,iVar2,0,0,0,local_40);
      if (iVar2 != 0) {
        uVar5 = 0;
      }
    }
  }
  for (iVar2 = 0; iVar2 != 6; iVar2 = iVar2 + 1) {
    *(undefined4 *)(this + iVar2 * 4 + 0x1c) = *(undefined4 *)(this + iVar2 * 4 + 0x34);
  }
  *(undefined2 *)(this + 0x128) = 0;
  if (*piVar1 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0015ed9c: push {r4,r5,r6,r7,lr}
  0015ed9e: add r7,sp,#0xc
  0015eda0: push {r8,r9,r10,r11}
  0015eda4: sub sp,#0x2c
  0015eda6: mov r4,r0
  0015eda8: ldr r0,[0x0015ef30]
  0015edaa: movs r1,#0x0
  0015edac: add r0,pc
  0015edae: ldr r0,[r0,#0x0]
  0015edb0: str r0,[sp,#0xc]
  0015edb2: ldr r0,[r0,#0x0]
  0015edb4: str r0,[sp,#0x28]
  0015edb6: ldr r0,[r4,#0x0]
  0015edb8: strd r1,r1,[r4,#0x12c]
  0015edbc: cmp r0,#0x1
  0015edbe: beq 0x0015ede4
  0015edc0: cmp r0,#0x2
  0015edc2: beq 0x0015edf8
  0015edc4: cmp r0,#0x3
  0015edc6: bne 0x0015ee24
  0015edc8: adds r0,r4,#0x4
  0015edca: movs r1,#0x0
  0015edcc: b 0x0015edd4
  0015edce: str.w r1,[r0,r1,lsl #0x2]
  0015edd2: adds r1,#0x1
  0015edd4: cmp r1,#0x5
  0015edd6: bne 0x0015edce
  0015edd8: ldr r0,[0x0015ef38]
  0015edda: movs r1,#0x5
  0015eddc: add r0,pc
  0015edde: ldr r0,[r0,#0x0]
  0015ede0: ldr r0,[r0,#0x0]
  0015ede2: b 0x0015ee1c
  0015ede4: adds r0,r4,#0x4
  0015ede6: movs r1,#0x0
  0015ede8: b 0x0015edf2
  0015edea: lsrs r2,r1,#0x1
  0015edec: str.w r2,[r0,r1,lsl #0x2]
  0015edf0: adds r1,#0x1
  0015edf2: cmp r1,#0x6
  0015edf4: bne 0x0015edea
  0015edf6: b 0x0015ee34
  0015edf8: adds r0,r4,#0x4
  0015edfa: movs r1,#0x0
  0015edfc: b 0x0015ee04
  0015edfe: str.w r1,[r0,r1,lsl #0x2]
  0015ee02: adds r1,#0x1
  0015ee04: cmp r1,#0x4
  0015ee06: bne 0x0015edfe
  0015ee08: ldr r0,[0x0015ef34]
  0015ee0a: movs r1,#0x4
  0015ee0c: add r0,pc
  0015ee0e: ldr r5,[r0,#0x0]
  0015ee10: ldr r0,[r5,#0x0]
  0015ee12: blx 0x00071848
  0015ee16: str r0,[r4,#0x14]
  0015ee18: movs r1,#0x4
  0015ee1a: ldr r0,[r5,#0x0]
  0015ee1c: blx 0x00071848
  0015ee20: str r0,[r4,#0x18]
  0015ee22: b 0x0015ee34
  0015ee24: adds r0,r4,#0x4
  0015ee26: movs r1,#0x0
  0015ee28: b 0x0015ee30
  0015ee2a: str.w r1,[r0,r1,lsl #0x2]
  0015ee2e: adds r1,#0x1
  0015ee30: cmp r1,#0x6
  0015ee32: bne 0x0015ee2a
  0015ee34: ldr r0,[0x0015ef3c]
  0015ee36: movs r6,#0x28
  0015ee38: add r0,pc
  0015ee3a: ldr.w r11,[r0,#0x0]
  0015ee3e: b 0x0015ee6a
  0015ee40: ldr.w r0,[r11,#0x0]
  0015ee44: movs r1,#0x6
  0015ee46: blx 0x00071848
  0015ee4a: mov r5,r0
  0015ee4c: ldr.w r0,[r11,#0x0]
  0015ee50: movs r1,#0x6
  0015ee52: blx 0x00071848
  0015ee56: adds r1,r4,#0x4
  0015ee58: subs r6,#0x1
  0015ee5a: ldr.w r2,[r1,r0,lsl #0x2]
  0015ee5e: ldr.w r3,[r1,r5,lsl #0x2]
  0015ee62: str.w r2,[r1,r5,lsl #0x2]
  0015ee66: str.w r3,[r1,r0,lsl #0x2]
  0015ee6a: cmp r6,#0x0
  0015ee6c: bne 0x0015ee40
  0015ee6e: movs r0,#0x0
  0015ee70: b 0x0015ee7e
  0015ee72: add.w r1,r4,r0, lsl #0x2
  0015ee76: adds r0,#0x1
  0015ee78: ldr r2,[r1,#0x4]
  0015ee7a: str r2,[r1,#0x1c]
  0015ee7c: str r2,[r1,#0x34]
  0015ee7e: cmp r0,#0x6
  0015ee80: bne 0x0015ee72
  0015ee82: add r6,sp,#0x10
  0015ee84: mov.w r8,#0x0
  0015ee88: b 0x0015eefa
  0015ee8a: ldr.w r0,[r11,#0x0]
  0015ee8e: movs r1,#0x2
  0015ee90: blx 0x00071848
  0015ee94: and r9,r8,#0x1
  0015ee98: mov r5,r0
  0015ee9a: mov.w r10,#0x0
  0015ee9e: b 0x0015eeb8
  0015eea0: mov r0,r4
  0015eea2: movs r1,#0x0
  0015eea4: cmp.w r9,#0x0
  0015eea8: bne 0x0015eeb0
  0015eeaa: blx 0x00072c58
  0015eeae: b 0x0015eeb4
  0015eeb0: blx 0x00072c64
  0015eeb4: add.w r10,r10,#0x1
  0015eeb8: cmp r10,r5
  0015eeba: ble 0x0015eea0
  0015eebc: ldr r1,[r4,#0x0]
  0015eebe: mov.w r0,#0xffffffff
  0015eec2: add.w r0,r0,r1, lsl #0x1
  0015eec6: cmp r8,r0
  0015eec8: bne 0x0015eef6
  0015eeca: movs r0,#0x0
  0015eecc: b 0x0015eeda
  0015eece: add.w r2,r4,r0, lsl #0x2
  0015eed2: ldr r2,[r2,#0x34]
  0015eed4: str.w r2,[r6,r0,lsl #0x2]
  0015eed8: adds r0,#0x1
  0015eeda: cmp r0,#0x6
  0015eedc: bne 0x0015eece
  0015eede: movs r0,#0x0
  0015eee0: movs r2,#0x0
  0015eee2: strd r0,r6,[sp,#0x0]
  0015eee6: mov r0,r4
  0015eee8: movs r3,#0x0
  0015eeea: blx 0x00077254
  0015eeee: cmp r0,#0x0
  0015eef0: it ne
  0015eef2: mov.ne.w r8,#0x0
  0015eef6: add.w r8,r8,#0x1
  0015eefa: ldr r0,[r4,#0x0]
  0015eefc: cmp.w r8,r0, lsl #0x1
  0015ef00: blt 0x0015ee8a
  0015ef02: movs r0,#0x0
  0015ef04: b 0x0015ef10
  0015ef06: add.w r1,r4,r0, lsl #0x2
  0015ef0a: adds r0,#0x1
  0015ef0c: ldr r2,[r1,#0x34]
  0015ef0e: str r2,[r1,#0x1c]
  0015ef10: cmp r0,#0x6
  0015ef12: bne 0x0015ef06
  0015ef14: movs r0,#0x0
  0015ef16: strh.w r0,[r4,#0x128]
  0015ef1a: ldr r0,[sp,#0x28]
  0015ef1c: ldr r1,[sp,#0xc]
  0015ef1e: ldr r1,[r1,#0x0]
  0015ef20: subs r0,r1,r0
  0015ef22: ittt eq
  0015ef24: add.eq sp,#0x2c
  0015ef26: pop.eq.w {r8,r9,r10,r11}
  0015ef2a: pop.eq {r4,r5,r6,r7,pc}
  0015ef2c: blx 0x0006e824
```
