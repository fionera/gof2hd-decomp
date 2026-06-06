# Level::createRoute
elf 0xc0ce0 body 210
Sig: undefined __thiscall createRoute(Level * this, int param_1)

## decompile
```c

/* Level::createRoute(int) */

undefined8 __thiscall Level::createRoute(Level *this,int param_1)

{
  uint uVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  Route *this_00;
  int *piVar5;
  undefined4 unaff_r5;
  int *piVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  
  uVar1 = param_1 * 3;
  uVar2 = (uint)((ulonglong)uVar1 * 4);
  if ((int)((ulonglong)uVar1 * 4 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  piVar3 = operator_new__(uVar2);
  piVar6 = piVar3 + 1;
  piVar5 = *(int **)(DAT_000d0dc0 + 0xd0d1a);
  pcVar9 = *(code **)(DAT_000d0dc4 + 0xd0d1c);
  for (iVar8 = -1; iVar8 + 1 < (int)uVar1; iVar8 = iVar8 + 3) {
    iVar4 = (*pcVar9)(*piVar5,2);
    iVar7 = -1;
    if (iVar4 == 0) {
      iVar7 = 1;
    }
    iVar4 = (*pcVar9)(*piVar5,30000);
    piVar6[-1] = (iVar4 + 50000) * iVar7;
    iVar4 = (*pcVar9)(*piVar5,20000);
    *piVar6 = iVar4 + -10000;
    if (iVar8 == -1) {
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar5);
      piVar3[2] = iVar4 + 50000;
    }
    else {
      iVar7 = piVar6[-2];
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar5);
      piVar6[1] = iVar4 + iVar7 + 50000;
    }
    piVar6 = piVar6 + 3;
  }
  this_00 = operator_new(0x18);
  Route::Route(this_00,piVar3,uVar1);
  return CONCAT44(unaff_r5,this_00);
}

```
## target disasm
```
  000d0ce0: push {r4,r5,r6,r7,lr}
  000d0ce2: add r7,sp,#0xc
  000d0ce4: push {r5,r6,r7,r8,r9,r10,r11}
  000d0ce8: add.w r1,r1,r1, lsl #0x1
  000d0cec: movs r0,#0x4
  000d0cee: str r1,[sp,#0x8]
  000d0cf0: umull r0,r1,r1,r0
  000d0cf4: cmp r1,#0x0
  000d0cf6: it ne
  000d0cf8: mov.ne r1,#0x1
  000d0cfa: cmp r1,#0x0
  000d0cfc: it ne
  000d0cfe: mov.ne.w r0,#0xffffffff
  000d0d02: blx 0x0006ec08
  000d0d06: str r0,[sp,#0x4]
  000d0d08: adds r6,r0,#0x4
  000d0d0a: ldr r0,[0x000d0dc0]
  000d0d0c: mov.w r10,#0xffffffff
  000d0d10: ldr r1,[0x000d0dc4]
  000d0d12: movw r5,#0xc350
  000d0d16: add r0,pc
  000d0d18: add r1,pc
  000d0d1a: ldr r4,[r0,#0x0]
  000d0d1c: ldr.w r11,[r1,#0x0]
  000d0d20: b 0x000d0d90
  000d0d22: ldr r0,[r4,#0x0]
  000d0d24: movs r1,#0x2
  000d0d26: blx r11
  000d0d28: ldr r1,[r4,#0x0]
  000d0d2a: cmp r0,#0x0
  000d0d2c: mov.w r9,#0xffffffff
  000d0d30: it eq
  000d0d32: mov.eq.w r9,#0x1
  000d0d36: mov r0,r1
  000d0d38: movw r1,#0x7530
  000d0d3c: blx r11
  000d0d3e: add r0,r5
  000d0d40: movw r1,#0x4e20
  000d0d44: mul r0,r0,r9
  000d0d48: str.w r0,[r6,#-0x4]
  000d0d4c: ldr r0,[r4,#0x0]
  000d0d4e: blx r11
  000d0d50: cmp.w r8,#0x0
  000d0d54: movw r1,#0x2710
  000d0d58: sub.w r0,r0,r1
  000d0d5c: str r0,[r6,#0x0]
  000d0d5e: beq 0x000d0d7a
  000d0d60: ldr r0,[r4,#0x0]
  000d0d62: movw r1,#0x7530
  000d0d66: ldr.w r5,[r6,#-0x8]
  000d0d6a: blx 0x00071848
  000d0d6e: add r0,r5
  000d0d70: movw r5,#0xc350
  000d0d74: add r0,r5
  000d0d76: str r0,[r6,#0x4]
  000d0d78: b 0x000d0d8a
  000d0d7a: ldr r0,[r4,#0x0]
  000d0d7c: movw r1,#0x7530
  000d0d80: blx 0x00071848
  000d0d84: ldr r1,[sp,#0x4]
  000d0d86: add r0,r5
  000d0d88: str r0,[r1,#0x8]
  000d0d8a: adds r6,#0xc
  000d0d8c: add.w r10,r10,#0x3
  000d0d90: ldr r0,[sp,#0x8]
  000d0d92: add.w r8,r10,#0x1
  000d0d96: cmp r8,r0
  000d0d98: blt 0x000d0d22
  000d0d9a: movs r0,#0x18
  000d0d9c: blx 0x0006eb24
  000d0da0: mov r6,r0
  000d0da2: ldrd r1,r2,[sp,#0x4]
  000d0da6: blx 0x00073eb8
  000d0daa: mov r0,r6
  000d0dac: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000d0db0: pop {r4,r5,r6,r7,pc}
```
