# NFC::iap_buy_credits_3_000_000
elf 0x69918 body 76
Sig: undefined __stdcall iap_buy_credits_3_000_000(void)

## decompile
```c

/* NFC::iap_buy_credits_3_000_000() */

void NFC::iap_buy_credits_3_000_000(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079964 + 0x79922);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_0007996c + 0x79932);
  **(undefined4 **)(DAT_00079968 + 0x79930) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_00079970 + 0x79950,DAT_00079974 + 0x79952);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079918: push {r4,r5,r7,lr}
  0007991a: add r7,sp,#0x8
  0007991c: ldr r0,[0x00079964]
  0007991e: add r0,pc
  00079920: ldr r5,[r0,#0x0]
  00079922: ldr r0,[r5,#0x0]
  00079924: cbz r0,0x00079962
  00079926: ldr r1,[0x00079968]
  00079928: movs r3,#0x1
  0007992a: ldr r2,[0x0007996c]
  0007992c: add r1,pc
  0007992e: add r2,pc
  00079930: ldr r1,[r1,#0x0]
  00079932: ldr r2,[r2,#0x0]
  00079934: str r3,[r1,#0x0]
  00079936: ldr r3,[r0,#0x0]
  00079938: ldr r1,[r2,#0x0]
  0007993a: ldr r2,[r3,#0x18]
  0007993c: blx r2
  0007993e: mov r4,r0
  00079940: ldr r0,[r5,#0x0]
  00079942: ldr r1,[r0,#0x0]
  00079944: ldr r2,[0x00079970]
  00079946: ldr r3,[0x00079974]
  00079948: ldr.w r12,[r1,#0x1c4]
  0007994c: add r2,pc
  0007994e: add r3,pc
  00079950: mov r1,r4
  00079952: blx r12
  00079954: mov r2,r0
  00079956: ldr r0,[r5,#0x0]
  00079958: mov r1,r4
  0007995a: pop.w {r4,r5,r7,lr}
  0007995e: b.w 0x001aae98
  00079962: pop {r4,r5,r7,pc}
```
