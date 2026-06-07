# NFC::free_credits_rateGame
elf 0x69bd0 body 66
Sig: undefined __stdcall free_credits_rateGame(void)

## decompile
```c

/* NFC::free_credits_rateGame() */

void NFC::free_credits_rateGame(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079c14 + 0x79bda);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079c18 + 0x79be6));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079c1c + 0x79bfe,DAT_00079c20 + 0x79c00);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079bd0: push {r4,r5,r7,lr}
  00079bd2: add r7,sp,#0x8
  00079bd4: ldr r0,[0x00079c14]
  00079bd6: add r0,pc
  00079bd8: ldr r5,[r0,#0x0]
  00079bda: ldr r0,[r5,#0x0]
  00079bdc: cbz r0,0x00079c10
  00079bde: ldr r1,[0x00079c18]
  00079be0: ldr r2,[r0,#0x0]
  00079be2: add r1,pc
  00079be4: ldr r1,[r1,#0x0]
  00079be6: ldr r2,[r2,#0x18]
  00079be8: ldr r1,[r1,#0x0]
  00079bea: blx r2
  00079bec: mov r4,r0
  00079bee: ldr r0,[r5,#0x0]
  00079bf0: ldr r1,[r0,#0x0]
  00079bf2: ldr r2,[0x00079c1c]
  00079bf4: ldr r3,[0x00079c20]
  00079bf6: ldr.w r12,[r1,#0x1c4]
  00079bfa: add r2,pc
  00079bfc: add r3,pc
  00079bfe: mov r1,r4
  00079c00: blx r12
  00079c02: mov r2,r0
  00079c04: ldr r0,[r5,#0x0]
  00079c06: mov r1,r4
  00079c08: pop.w {r4,r5,r7,lr}
  00079c0c: b.w 0x001aae98
  00079c10: pop {r4,r5,r7,pc}
```
