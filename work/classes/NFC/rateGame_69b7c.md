# NFC::rateGame
elf 0x69b7c body 66
Sig: undefined __stdcall rateGame(void)

## decompile
```c

/* NFC::rateGame() */

void NFC::rateGame(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079bc0 + 0x79b86);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079bc4 + 0x79b92));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079bc8 + 0x79baa,DAT_00079bcc + 0x79bac);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079b7c: push {r4,r5,r7,lr}
  00079b7e: add r7,sp,#0x8
  00079b80: ldr r0,[0x00079bc0]
  00079b82: add r0,pc
  00079b84: ldr r5,[r0,#0x0]
  00079b86: ldr r0,[r5,#0x0]
  00079b88: cbz r0,0x00079bbc
  00079b8a: ldr r1,[0x00079bc4]
  00079b8c: ldr r2,[r0,#0x0]
  00079b8e: add r1,pc
  00079b90: ldr r1,[r1,#0x0]
  00079b92: ldr r2,[r2,#0x18]
  00079b94: ldr r1,[r1,#0x0]
  00079b96: blx r2
  00079b98: mov r4,r0
  00079b9a: ldr r0,[r5,#0x0]
  00079b9c: ldr r1,[r0,#0x0]
  00079b9e: ldr r2,[0x00079bc8]
  00079ba0: ldr r3,[0x00079bcc]
  00079ba2: ldr.w r12,[r1,#0x1c4]
  00079ba6: add r2,pc
  00079ba8: add r3,pc
  00079baa: mov r1,r4
  00079bac: blx r12
  00079bae: mov r2,r0
  00079bb0: ldr r0,[r5,#0x0]
  00079bb2: mov r1,r4
  00079bb4: pop.w {r4,r5,r7,lr}
  00079bb8: b.w 0x001aae98
  00079bbc: pop {r4,r5,r7,pc}
```
