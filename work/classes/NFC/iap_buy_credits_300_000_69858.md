# NFC::iap_buy_credits_300_000
elf 0x69858 body 76
Sig: undefined __stdcall iap_buy_credits_300_000(void)

## decompile
```c

/* NFC::iap_buy_credits_300_000() */

void NFC::iap_buy_credits_300_000(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_000798a4 + 0x79862);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_000798ac + 0x79872);
  **(undefined4 **)(DAT_000798a8 + 0x79870) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_000798b0 + 0x79890,DAT_000798b4 + 0x79892);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079858: push {r4,r5,r7,lr}
  0007985a: add r7,sp,#0x8
  0007985c: ldr r0,[0x000798a4]
  0007985e: add r0,pc
  00079860: ldr r5,[r0,#0x0]
  00079862: ldr r0,[r5,#0x0]
  00079864: cbz r0,0x000798a2
  00079866: ldr r1,[0x000798a8]
  00079868: movs r3,#0x1
  0007986a: ldr r2,[0x000798ac]
  0007986c: add r1,pc
  0007986e: add r2,pc
  00079870: ldr r1,[r1,#0x0]
  00079872: ldr r2,[r2,#0x0]
  00079874: str r3,[r1,#0x0]
  00079876: ldr r3,[r0,#0x0]
  00079878: ldr r1,[r2,#0x0]
  0007987a: ldr r2,[r3,#0x18]
  0007987c: blx r2
  0007987e: mov r4,r0
  00079880: ldr r0,[r5,#0x0]
  00079882: ldr r1,[r0,#0x0]
  00079884: ldr r2,[0x000798b0]
  00079886: ldr r3,[0x000798b4]
  00079888: ldr.w r12,[r1,#0x1c4]
  0007988c: add r2,pc
  0007988e: add r3,pc
  00079890: mov r1,r4
  00079892: blx r12
  00079894: mov r2,r0
  00079896: ldr r0,[r5,#0x0]
  00079898: mov r1,r4
  0007989a: pop.w {r4,r5,r7,lr}
  0007989e: b.w 0x001aae98
  000798a2: pop {r4,r5,r7,pc}
```
