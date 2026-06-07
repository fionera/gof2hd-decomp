# NFC::iap_buy_credits_1_000_000
elf 0x698b8 body 76
Sig: undefined __stdcall iap_buy_credits_1_000_000(void)

## decompile
```c

/* NFC::iap_buy_credits_1_000_000() */

void NFC::iap_buy_credits_1_000_000(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079904 + 0x798c2);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_0007990c + 0x798d2);
  **(undefined4 **)(DAT_00079908 + 0x798d0) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_00079910 + 0x798f0,DAT_00079914 + 0x798f2);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  000798b8: push {r4,r5,r7,lr}
  000798ba: add r7,sp,#0x8
  000798bc: ldr r0,[0x00079904]
  000798be: add r0,pc
  000798c0: ldr r5,[r0,#0x0]
  000798c2: ldr r0,[r5,#0x0]
  000798c4: cbz r0,0x00079902
  000798c6: ldr r1,[0x00079908]
  000798c8: movs r3,#0x1
  000798ca: ldr r2,[0x0007990c]
  000798cc: add r1,pc
  000798ce: add r2,pc
  000798d0: ldr r1,[r1,#0x0]
  000798d2: ldr r2,[r2,#0x0]
  000798d4: str r3,[r1,#0x0]
  000798d6: ldr r3,[r0,#0x0]
  000798d8: ldr r1,[r2,#0x0]
  000798da: ldr r2,[r3,#0x18]
  000798dc: blx r2
  000798de: mov r4,r0
  000798e0: ldr r0,[r5,#0x0]
  000798e2: ldr r1,[r0,#0x0]
  000798e4: ldr r2,[0x00079910]
  000798e6: ldr r3,[0x00079914]
  000798e8: ldr.w r12,[r1,#0x1c4]
  000798ec: add r2,pc
  000798ee: add r3,pc
  000798f0: mov r1,r4
  000798f2: blx r12
  000798f4: mov r2,r0
  000798f6: ldr r0,[r5,#0x0]
  000798f8: mov r1,r4
  000798fa: pop.w {r4,r5,r7,lr}
  000798fe: b.w 0x001aae98
  00079902: pop {r4,r5,r7,pc}
```
