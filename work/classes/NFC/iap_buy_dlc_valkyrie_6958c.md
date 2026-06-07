# NFC::iap_buy_dlc_valkyrie
elf 0x6958c body 76
Sig: undefined __stdcall iap_buy_dlc_valkyrie(void)

## decompile
```c

/* NFC::iap_buy_dlc_valkyrie() */

void NFC::iap_buy_dlc_valkyrie(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_000795d8 + 0x79596);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_000795e0 + 0x795a6);
  **(undefined4 **)(DAT_000795dc + 0x795a4) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_000795e4 + 0x795c4,DAT_000795e8 + 0x795c6);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  0007958c: push {r4,r5,r7,lr}
  0007958e: add r7,sp,#0x8
  00079590: ldr r0,[0x000795d8]
  00079592: add r0,pc
  00079594: ldr r5,[r0,#0x0]
  00079596: ldr r0,[r5,#0x0]
  00079598: cbz r0,0x000795d6
  0007959a: ldr r1,[0x000795dc]
  0007959c: movs r3,#0x1
  0007959e: ldr r2,[0x000795e0]
  000795a0: add r1,pc
  000795a2: add r2,pc
  000795a4: ldr r1,[r1,#0x0]
  000795a6: ldr r2,[r2,#0x0]
  000795a8: str r3,[r1,#0x0]
  000795aa: ldr r3,[r0,#0x0]
  000795ac: ldr r1,[r2,#0x0]
  000795ae: ldr r2,[r3,#0x18]
  000795b0: blx r2
  000795b2: mov r4,r0
  000795b4: ldr r0,[r5,#0x0]
  000795b6: ldr r1,[r0,#0x0]
  000795b8: ldr r2,[0x000795e4]
  000795ba: ldr r3,[0x000795e8]
  000795bc: ldr.w r12,[r1,#0x1c4]
  000795c0: add r2,pc
  000795c2: add r3,pc
  000795c4: mov r1,r4
  000795c6: blx r12
  000795c8: mov r2,r0
  000795ca: ldr r0,[r5,#0x0]
  000795cc: mov r1,r4
  000795ce: pop.w {r4,r5,r7,lr}
  000795d2: b.w 0x001aae98
  000795d6: pop {r4,r5,r7,pc}
```
