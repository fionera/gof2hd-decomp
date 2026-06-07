# NFC::iap_restore_purchases
elf 0x697a4 body 66
Sig: undefined __stdcall iap_restore_purchases(void)

## decompile
```c

/* NFC::iap_restore_purchases() */

void NFC::iap_restore_purchases(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000797e8 + 0x797ae);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_000797ec + 0x797ba));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_000797f0 + 0x797d2,DAT_000797f4 + 0x797d4);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  000797a4: push {r4,r5,r7,lr}
  000797a6: add r7,sp,#0x8
  000797a8: ldr r0,[0x000797e8]
  000797aa: add r0,pc
  000797ac: ldr r5,[r0,#0x0]
  000797ae: ldr r0,[r5,#0x0]
  000797b0: cbz r0,0x000797e4
  000797b2: ldr r1,[0x000797ec]
  000797b4: ldr r2,[r0,#0x0]
  000797b6: add r1,pc
  000797b8: ldr r1,[r1,#0x0]
  000797ba: ldr r2,[r2,#0x18]
  000797bc: ldr r1,[r1,#0x0]
  000797be: blx r2
  000797c0: mov r4,r0
  000797c2: ldr r0,[r5,#0x0]
  000797c4: ldr r1,[r0,#0x0]
  000797c6: ldr r2,[0x000797f0]
  000797c8: ldr r3,[0x000797f4]
  000797ca: ldr.w r12,[r1,#0x1c4]
  000797ce: add r2,pc
  000797d0: add r3,pc
  000797d2: mov r1,r4
  000797d4: blx r12
  000797d6: mov r2,r0
  000797d8: ldr r0,[r5,#0x0]
  000797da: mov r1,r4
  000797dc: pop.w {r4,r5,r7,lr}
  000797e0: b.w 0x001aae98
  000797e4: pop {r4,r5,r7,pc}
```
