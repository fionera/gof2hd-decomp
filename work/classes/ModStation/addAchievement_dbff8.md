# ModStation::addAchievement
elf 0xdbff8 body 112
Sig: undefined __thiscall addAchievement(ModStation * this, int param_1, int param_2)

## decompile
```c

/* ModStation::addAchievement(int, int) */

void __thiscall ModStation::addAchievement(ModStation *this,int param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = *(undefined4 **)(DAT_000ec068 + 0xec00a);
  iVar1 = Achievements::isEliteMedal((Achievements *)*puVar2,param_1);
  if (iVar1 == 0) {
    if (param_2 - 1U < 2) {
      iVar1 = Achievements::getValue((Achievements *)*puVar2,param_1,3);
      if (iVar1 != -1) {
        **(int **)(DAT_000ec06c + 0xec030) = param_1 * 3;
      }
      if ((param_2 == 1) &&
         (iVar1 = Achievements::getValue((Achievements *)*puVar2,param_1,2), iVar1 != -1)) {
        *(int *)(*(int *)(DAT_000ec070 + 0xec050) + 4) = param_1 * 3 + 1;
      }
    }
    *(int *)(*(int *)(DAT_000ec074 + 0xec05e) + 8) = (param_1 * 3 + 3) - param_2;
  }
  return;
}

```

## target disasm
```
  000ebff8: push {r4,r5,r6,r7,lr}
  000ebffa: add r7,sp,#0xc
  000ebffc: push.w r11
  000ec000: ldr r0,[0x000ec068]
  000ec002: mov r4,r2
  000ec004: mov r5,r1
  000ec006: add r0,pc
  000ec008: ldr r6,[r0,#0x0]
  000ec00a: ldr r0,[r6,#0x0]
  000ec00c: blx 0x000756ac
  000ec010: cbnz r0,0x000ec062
  000ec012: subs r0,r4,#0x1
  000ec014: cmp r0,#0x1
  000ec016: bhi 0x000ec052
  000ec018: ldr r0,[r6,#0x0]
  000ec01a: mov r1,r5
  000ec01c: movs r2,#0x3
  000ec01e: blx 0x00072f04
  000ec022: adds r0,#0x1
  000ec024: beq 0x000ec032
  000ec026: ldr r0,[0x000ec06c]
  000ec028: add.w r1,r5,r5, lsl #0x1
  000ec02c: add r0,pc
  000ec02e: ldr r0,[r0,#0x0]
  000ec030: str r1,[r0,#0x0]
  000ec032: cmp r4,#0x1
  000ec034: bne 0x000ec052
  000ec036: ldr r0,[r6,#0x0]
  000ec038: mov r1,r5
  000ec03a: movs r2,#0x2
  000ec03c: blx 0x00072f04
  000ec040: adds r0,#0x1
  000ec042: beq 0x000ec052
  000ec044: ldr r0,[0x000ec070]
  000ec046: add.w r1,r5,r5, lsl #0x1
  000ec04a: adds r1,#0x1
  000ec04c: add r0,pc
  000ec04e: ldr r0,[r0,#0x0]
  000ec050: str r1,[r0,#0x4]
  000ec052: ldr r0,[0x000ec074]
  000ec054: add.w r1,r5,r5, lsl #0x1
  000ec058: adds r1,#0x3
  000ec05a: add r0,pc
  000ec05c: subs r1,r1,r4
  000ec05e: ldr r0,[r0,#0x0]
  000ec060: str r1,[r0,#0x8]
  000ec062: pop.w r11
  000ec066: pop {r4,r5,r6,r7,pc}
```
