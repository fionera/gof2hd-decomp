# NFC::iap_buy_dlc_kaamo_club
elf 0x69624 body 76
Sig: undefined __stdcall iap_buy_dlc_kaamo_club(void)

## decompile
```c

/* NFC::iap_buy_dlc_kaamo_club() */

void NFC::iap_buy_dlc_kaamo_club(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079670 + 0x7962e);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_00079678 + 0x7963e);
  **(undefined4 **)(DAT_00079674 + 0x7963c) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_0007967c + 0x7965c,DAT_00079680 + 0x7965e);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079624: push {r4,r5,r7,lr}
  00079626: add r7,sp,#0x8
  00079628: ldr r0,[0x00079670]
  0007962a: add r0,pc
  0007962c: ldr r5,[r0,#0x0]
  0007962e: ldr r0,[r5,#0x0]
  00079630: cbz r0,0x0007966e
  00079632: ldr r1,[0x00079674]
  00079634: movs r3,#0x1
  00079636: ldr r2,[0x00079678]
  00079638: add r1,pc
  0007963a: add r2,pc
  0007963c: ldr r1,[r1,#0x0]
  0007963e: ldr r2,[r2,#0x0]
  00079640: str r3,[r1,#0x0]
  00079642: ldr r3,[r0,#0x0]
  00079644: ldr r1,[r2,#0x0]
  00079646: ldr r2,[r3,#0x18]
  00079648: blx r2
  0007964a: mov r4,r0
  0007964c: ldr r0,[r5,#0x0]
  0007964e: ldr r1,[r0,#0x0]
  00079650: ldr r2,[0x0007967c]
  00079652: ldr r3,[0x00079680]
  00079654: ldr.w r12,[r1,#0x1c4]
  00079658: add r2,pc
  0007965a: add r3,pc
  0007965c: mov r1,r4
  0007965e: blx r12
  00079660: mov r2,r0
  00079662: ldr r0,[r5,#0x0]
  00079664: mov r1,r4
  00079666: pop.w {r4,r5,r7,lr}
  0007966a: b.w 0x001aae98
  0007966e: pop {r4,r5,r7,pc}
```
