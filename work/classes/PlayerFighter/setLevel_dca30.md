# PlayerFighter::setLevel
elf 0xdca30 body 218
Sig: undefined __stdcall setLevel(Level * param_1)

## decompile
```c

/* PlayerFighter::setLevel(Level*) */

void PlayerFighter::setLevel(Level *param_1)

{
  undefined4 uVar1;
  Level *in_r1;
  undefined4 *puVar2;
  undefined4 uVar3;
  code *pcVar4;
  code *pcVar5;
  code *pcVar6;
  
  KIPlayer::setLevel((KIPlayer *)param_1,in_r1);
  pcVar6 = *(code **)(DAT_000ecb0c + 0xeca46);
  uVar3 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x74);
  uVar1 = (*pcVar6)(*(undefined4 *)(param_1 + 8));
  pcVar5 = *(code **)(DAT_000ecb10 + 0xeca5a);
  uVar1 = (*pcVar5)(uVar3,uVar1,9,0);
  puVar2 = (undefined4 *)(DAT_000ecb14 + 0xeca6a);
  *(undefined4 *)(param_1 + 0x1a0) = uVar1;
  pcVar4 = (code *)*puVar2;
  (*pcVar4)(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x74),uVar1,0);
  uVar3 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x78);
  uVar1 = (*pcVar6)(*(undefined4 *)(param_1 + 8));
  uVar1 = (*pcVar5)(uVar3,uVar1,0xf,0);
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  (*pcVar4)(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x78),uVar1,0);
  uVar3 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x84);
  uVar1 = (*pcVar6)(*(undefined4 *)(param_1 + 8));
  uVar1 = (*pcVar5)(uVar3,uVar1,0x2a,0);
  *(undefined4 *)(param_1 + 0x84) = uVar1;
  (*pcVar4)(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x84),uVar1,0);
  uVar3 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x8c);
  uVar1 = (*pcVar6)(*(undefined4 *)(param_1 + 8));
  uVar1 = (*pcVar5)(uVar3,uVar1,0x11,0);
  *(undefined4 *)(param_1 + 0x134) = uVar1;
  (*pcVar4)(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x8c),uVar1,0);
  uVar3 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x8c);
  uVar1 = (*pcVar6)(*(undefined4 *)(param_1 + 8));
  uVar1 = (*pcVar5)(uVar3,uVar1,0x12,0);
  *(undefined4 *)(param_1 + 0x138) = uVar1;
  (*pcVar4)(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x8c),uVar1,0);
  return;
}

```

## target disasm
```
  000eca30: push {r4,r5,r6,r7,lr}
  000eca32: add r7,sp,#0xc
  000eca34: push {r8,r9,r11}
  000eca38: mov r4,r0
  000eca3a: blx 0x00073138
  000eca3e: ldr r1,[0x000ecb0c]
  000eca40: ldr r2,[r4,#0x54]
  000eca42: add r1,pc
  000eca44: ldr r0,[r4,#0x8]
  000eca46: ldr.w r9,[r1,#0x0]
  000eca4a: ldr r5,[r2,#0x74]
  000eca4c: blx r9
  000eca4e: mov r1,r0
  000eca50: ldr r0,[0x000ecb10]
  000eca52: movs r2,#0x9
  000eca54: movs r3,#0x0
  000eca56: add r0,pc
  000eca58: ldr.w r8,[r0,#0x0]
  000eca5c: mov r0,r5
  000eca5e: blx r8
  000eca60: ldr r2,[0x000ecb14]
  000eca62: mov r1,r0
  000eca64: ldr r0,[r4,#0x54]
  000eca66: add r2,pc
  000eca68: str.w r1,[r4,#0x1a0]
  000eca6c: ldr r6,[r2,#0x0]
  000eca6e: movs r2,#0x0
  000eca70: ldr r0,[r0,#0x74]
  000eca72: blx r6
  000eca74: ldr r0,[r4,#0x8]
  000eca76: ldr r1,[r4,#0x54]
  000eca78: ldr r5,[r1,#0x78]
  000eca7a: blx r9
  000eca7c: mov r1,r0
  000eca7e: mov r0,r5
  000eca80: movs r2,#0xf
  000eca82: movs r3,#0x0
  000eca84: blx r8
  000eca86: mov r1,r0
  000eca88: ldr r0,[r4,#0x54]
  000eca8a: str.w r1,[r4,#0x80]
  000eca8e: movs r2,#0x0
  000eca90: ldr r0,[r0,#0x78]
  000eca92: blx r6
  000eca94: ldr r0,[r4,#0x8]
  000eca96: ldr r1,[r4,#0x54]
  000eca98: ldr.w r5,[r1,#0x84]
  000eca9c: blx r9
  000eca9e: mov r1,r0
  000ecaa0: mov r0,r5
  000ecaa2: movs r2,#0x2a
  000ecaa4: movs r3,#0x0
  000ecaa6: blx r8
  000ecaa8: mov r1,r0
  000ecaaa: ldr r0,[r4,#0x54]
  000ecaac: str.w r1,[r4,#0x84]
  000ecab0: movs r2,#0x0
  000ecab2: ldr.w r0,[r0,#0x84]
  000ecab6: blx r6
  000ecab8: ldr r0,[r4,#0x8]
  000ecaba: ldr r1,[r4,#0x54]
  000ecabc: ldr.w r5,[r1,#0x8c]
  000ecac0: blx r9
  000ecac2: mov r1,r0
  000ecac4: mov r0,r5
  000ecac6: movs r2,#0x11
  000ecac8: movs r3,#0x0
  000ecaca: blx r8
  000ecacc: mov r1,r0
  000ecace: ldr r0,[r4,#0x54]
  000ecad0: str.w r1,[r4,#0x134]
  000ecad4: movs r2,#0x0
  000ecad6: ldr.w r0,[r0,#0x8c]
  000ecada: blx r6
  000ecadc: ldr r0,[r4,#0x8]
  000ecade: ldr r1,[r4,#0x54]
  000ecae0: ldr.w r5,[r1,#0x8c]
  000ecae4: blx r9
  000ecae6: mov r1,r0
  000ecae8: mov r0,r5
  000ecaea: movs r2,#0x12
  000ecaec: movs r3,#0x0
  000ecaee: blx r8
  000ecaf0: mov r1,r0
  000ecaf2: ldr r0,[r4,#0x54]
  000ecaf4: str.w r1,[r4,#0x138]
  000ecaf8: movs r2,#0x0
  000ecafa: mov r3,r6
  000ecafc: ldr.w r0,[r0,#0x8c]
  000ecb00: pop.w {r8,r9,r11}
  000ecb04: pop.w {r4,r5,r6,r7,lr}
  000ecb08: bx r3
```
