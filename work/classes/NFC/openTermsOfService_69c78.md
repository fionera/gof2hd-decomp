# NFC::openTermsOfService
elf 0x69c78 body 76
Sig: undefined __stdcall openTermsOfService(void)

## decompile
```c

/* NFC::openTermsOfService() */

void NFC::openTermsOfService(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079cbc + 0x79c82);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079cc0 + 0x79c8e));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079cc4 + 0x79ca6,DAT_00079cc8 + 0x79ca8);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079c78: push {r4,r5,r7,lr}
  00079c7a: add r7,sp,#0x8
  00079c7c: ldr r0,[0x00079cbc]
  00079c7e: add r0,pc
  00079c80: ldr r5,[r0,#0x0]
  00079c82: ldr r0,[r5,#0x0]
  00079c84: cbz r0,0x00079cb8
  00079c86: ldr r1,[0x00079cc0]
  00079c88: ldr r2,[r0,#0x0]
  00079c8a: add r1,pc
  00079c8c: ldr r1,[r1,#0x0]
  00079c8e: ldr r2,[r2,#0x18]
  00079c90: ldr r1,[r1,#0x0]
  00079c92: blx r2
  00079c94: mov r4,r0
  00079c96: ldr r0,[r5,#0x0]
  00079c98: ldr r1,[r0,#0x0]
  00079c9a: ldr r2,[0x00079cc4]
  00079c9c: ldr r3,[0x00079cc8]
  00079c9e: ldr.w r12,[r1,#0x1c4]
  00079ca2: add r2,pc
  00079ca4: add r3,pc
  00079ca6: mov r1,r4
  00079ca8: blx r12
  00079caa: mov r2,r0
  00079cac: ldr r0,[r5,#0x0]
  00079cae: mov r1,r4
  00079cb0: pop.w {r4,r5,r7,lr}
  00079cb4: b.w 0x001aae98
  00079cb8: pop {r4,r5,r7,pc}
  001aae98: bx pc
  001aae9c: ldr r12,[0x1aaea4]
  001aaea0: add pc,r12,pc
```
