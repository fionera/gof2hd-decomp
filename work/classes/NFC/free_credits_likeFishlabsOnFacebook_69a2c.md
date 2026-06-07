# NFC::free_credits_likeFishlabsOnFacebook
elf 0x69a2c body 66
Sig: undefined __stdcall free_credits_likeFishlabsOnFacebook(void)

## decompile
```c

/* NFC::free_credits_likeFishlabsOnFacebook() */

void NFC::free_credits_likeFishlabsOnFacebook(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079a70 + 0x79a36);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079a74 + 0x79a42));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079a78 + 0x79a5a,DAT_00079a7c + 0x79a5c);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079a2c: push {r4,r5,r7,lr}
  00079a2e: add r7,sp,#0x8
  00079a30: ldr r0,[0x00079a70]
  00079a32: add r0,pc
  00079a34: ldr r5,[r0,#0x0]
  00079a36: ldr r0,[r5,#0x0]
  00079a38: cbz r0,0x00079a6c
  00079a3a: ldr r1,[0x00079a74]
  00079a3c: ldr r2,[r0,#0x0]
  00079a3e: add r1,pc
  00079a40: ldr r1,[r1,#0x0]
  00079a42: ldr r2,[r2,#0x18]
  00079a44: ldr r1,[r1,#0x0]
  00079a46: blx r2
  00079a48: mov r4,r0
  00079a4a: ldr r0,[r5,#0x0]
  00079a4c: ldr r1,[r0,#0x0]
  00079a4e: ldr r2,[0x00079a78]
  00079a50: ldr r3,[0x00079a7c]
  00079a52: ldr.w r12,[r1,#0x1c4]
  00079a56: add r2,pc
  00079a58: add r3,pc
  00079a5a: mov r1,r4
  00079a5c: blx r12
  00079a5e: mov r2,r0
  00079a60: ldr r0,[r5,#0x0]
  00079a62: mov r1,r4
  00079a64: pop.w {r4,r5,r7,lr}
  00079a68: b.w 0x001aae98
  00079a6c: pop {r4,r5,r7,pc}
```
