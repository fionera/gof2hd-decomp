# NFC::free_credits_likeGOF2OnFacebook
elf 0x699d8 body 66
Sig: undefined __stdcall free_credits_likeGOF2OnFacebook(void)

## decompile
```c

/* NFC::free_credits_likeGOF2OnFacebook() */

void NFC::free_credits_likeGOF2OnFacebook(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079a1c + 0x799e2);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079a20 + 0x799ee));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079a24 + 0x79a06,DAT_00079a28 + 0x79a08);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  000799d8: push {r4,r5,r7,lr}
  000799da: add r7,sp,#0x8
  000799dc: ldr r0,[0x00079a1c]
  000799de: add r0,pc
  000799e0: ldr r5,[r0,#0x0]
  000799e2: ldr r0,[r5,#0x0]
  000799e4: cbz r0,0x00079a18
  000799e6: ldr r1,[0x00079a20]
  000799e8: ldr r2,[r0,#0x0]
  000799ea: add r1,pc
  000799ec: ldr r1,[r1,#0x0]
  000799ee: ldr r2,[r2,#0x18]
  000799f0: ldr r1,[r1,#0x0]
  000799f2: blx r2
  000799f4: mov r4,r0
  000799f6: ldr r0,[r5,#0x0]
  000799f8: ldr r1,[r0,#0x0]
  000799fa: ldr r2,[0x00079a24]
  000799fc: ldr r3,[0x00079a28]
  000799fe: ldr.w r12,[r1,#0x1c4]
  00079a02: add r2,pc
  00079a04: add r3,pc
  00079a06: mov r1,r4
  00079a08: blx r12
  00079a0a: mov r2,r0
  00079a0c: ldr r0,[r5,#0x0]
  00079a0e: mov r1,r4
  00079a10: pop.w {r4,r5,r7,lr}
  00079a14: b.w 0x001aae98
  00079a18: pop {r4,r5,r7,pc}
```
