# HackingGame::update
elf 0x14f138 body 142
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* HackingGame::update(int) */

undefined4 HackingGame::update(int param_1)

{
  int iVar1;
  uint uVar2;
  int in_r1;
  int iVar3;
  float in_s0;
  
  if (((*(char *)(param_1 + 0x128) != '\0') || (*(char *)(param_1 + 0x129) != '\0')) &&
     (iVar1 = *(int *)(param_1 + 300) + in_r1, *(int *)(param_1 + 300) = iVar1, 300 < iVar1)) {
    *(undefined4 *)(param_1 + 300) = 0;
    *(undefined2 *)(param_1 + 0x128) = 0;
    for (iVar1 = 0; iVar1 != 6; iVar1 = iVar1 + 1) {
      iVar3 = param_1 + iVar1 * 4;
      *(undefined4 *)(iVar3 + 0x1c) = *(undefined4 *)(iVar3 + 0x34);
    }
  }
  uVar2 = 0;
  while (uVar2 < 6) {
    iVar1 = param_1 + uVar2 * 4;
    uVar2 = uVar2 + 1;
    if (*(int *)(iVar1 + 0x1c) != *(int *)(iVar1 + 4)) {
      return 1;
    }
  }
  iVar1 = *(int *)(param_1 + 0x130);
  if (iVar1 == 0) {
    FModSound::play(**(int **)(DAT_0015f1c8 + 0x15f1a0),(Vector *)0x8e1,(Vector *)0x0,in_s0);
    iVar1 = *(int *)(param_1 + 0x130);
  }
  *(int *)(param_1 + 0x130) = iVar1 + in_r1;
  if (iVar1 + in_r1 < 0x5dd) {
    return 1;
  }
  return 0;
}

```

## target disasm
```
  0015f138: push {r2,r3,r4,r5,r7,lr}
  0015f13a: add r7,sp,#0x10
  0015f13c: mov r4,r0
  0015f13e: ldrb.w r0,[r0,#0x128]
  0015f142: mov r5,r1
  0015f144: cbnz r0,0x0015f14c
  0015f146: ldrb.w r0,[r4,#0x129]
  0015f14a: cbz r0,0x0015f176
  0015f14c: ldr.w r0,[r4,#0x12c]
  0015f150: add r0,r5
  0015f152: str.w r0,[r4,#0x12c]
  0015f156: cmp.w r0,#0x12c
  0015f15a: ble 0x0015f176
  0015f15c: movs r0,#0x0
  0015f15e: str.w r0,[r4,#0x12c]
  0015f162: strh.w r0,[r4,#0x128]
  0015f166: b 0x0015f172
  0015f168: add.w r1,r4,r0, lsl #0x2
  0015f16c: adds r0,#0x1
  0015f16e: ldr r2,[r1,#0x34]
  0015f170: str r2,[r1,#0x1c]
  0015f172: cmp r0,#0x6
  0015f174: bne 0x0015f168
  0015f176: movs r0,#0x0
  0015f178: cmp r0,#0x5
  0015f17a: bhi 0x0015f18c
  0015f17c: add.w r1,r4,r0, lsl #0x2
  0015f180: adds r0,#0x1
  0015f182: ldr r2,[r1,#0x4]
  0015f184: ldr r1,[r1,#0x1c]
  0015f186: cmp r1,r2
  0015f188: beq 0x0015f178
  0015f18a: b 0x0015f1c0
  0015f18c: ldr.w r0,[r4,#0x130]
  0015f190: cbnz r0,0x0015f1ae
  0015f192: ldr r0,[0x0015f1c8]
  0015f194: movs r1,#0x0
  0015f196: str r1,[sp,#0x0]
  0015f198: movw r1,#0x8e1
  0015f19c: add r0,pc
  0015f19e: movs r2,#0x0
  0015f1a0: movs r3,#0x0
  0015f1a2: ldr r0,[r0,#0x0]
  0015f1a4: ldr r0,[r0,#0x0]
  0015f1a6: blx 0x00071548
  0015f1aa: ldr.w r0,[r4,#0x130]
  0015f1ae: add r0,r5
  0015f1b0: movw r1,#0x5dc
  0015f1b4: cmp r0,r1
  0015f1b6: str.w r0,[r4,#0x130]
  0015f1ba: ble 0x0015f1c0
  0015f1bc: movs r0,#0x0
  0015f1be: b 0x0015f1c2
  0015f1c0: movs r0,#0x1
  0015f1c2: add sp,#0x8
  0015f1c4: pop {r4,r5,r7,pc}
```
