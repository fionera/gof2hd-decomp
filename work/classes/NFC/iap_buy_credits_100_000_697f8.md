# NFC::iap_buy_credits_100_000
elf 0x697f8 body 76
Sig: undefined __stdcall iap_buy_credits_100_000(void)

## decompile
```c

/* NFC::iap_buy_credits_100_000() */

void NFC::iap_buy_credits_100_000(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079844 + 0x79802);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_0007984c + 0x79812);
  **(undefined4 **)(DAT_00079848 + 0x79810) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_00079850 + 0x79830,DAT_00079854 + 0x79832);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  000797f8: push {r4,r5,r7,lr}
  000797fa: add r7,sp,#0x8
  000797fc: ldr r0,[0x00079844]
  000797fe: add r0,pc
  00079800: ldr r5,[r0,#0x0]
  00079802: ldr r0,[r5,#0x0]
  00079804: cbz r0,0x00079842
  00079806: ldr r1,[0x00079848]
  00079808: movs r3,#0x1
  0007980a: ldr r2,[0x0007984c]
  0007980c: add r1,pc
  0007980e: add r2,pc
  00079810: ldr r1,[r1,#0x0]
  00079812: ldr r2,[r2,#0x0]
  00079814: str r3,[r1,#0x0]
  00079816: ldr r3,[r0,#0x0]
  00079818: ldr r1,[r2,#0x0]
  0007981a: ldr r2,[r3,#0x18]
  0007981c: blx r2
  0007981e: mov r4,r0
  00079820: ldr r0,[r5,#0x0]
  00079822: ldr r1,[r0,#0x0]
  00079824: ldr r2,[0x00079850]
  00079826: ldr r3,[0x00079854]
  00079828: ldr.w r12,[r1,#0x1c4]
  0007982c: add r2,pc
  0007982e: add r3,pc
  00079830: mov r1,r4
  00079832: blx r12
  00079834: mov r2,r0
  00079836: ldr r0,[r5,#0x0]
  00079838: mov r1,r4
  0007983a: pop.w {r4,r5,r7,lr}
  0007983e: b.w 0x001aae98
  00079842: pop {r4,r5,r7,pc}
```
