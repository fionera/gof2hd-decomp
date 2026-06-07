# NFC::free_credits_subscribeToYoutubeChannel
elf 0x69a80 body 66
Sig: undefined __stdcall free_credits_subscribeToYoutubeChannel(void)

## decompile
```c

/* NFC::free_credits_subscribeToYoutubeChannel() */

void NFC::free_credits_subscribeToYoutubeChannel(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079ac4 + 0x79a8a);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079ac8 + 0x79a96));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079acc + 0x79aae,DAT_00079ad0 + 0x79ab0);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079a80: push {r4,r5,r7,lr}
  00079a82: add r7,sp,#0x8
  00079a84: ldr r0,[0x00079ac4]
  00079a86: add r0,pc
  00079a88: ldr r5,[r0,#0x0]
  00079a8a: ldr r0,[r5,#0x0]
  00079a8c: cbz r0,0x00079ac0
  00079a8e: ldr r1,[0x00079ac8]
  00079a90: ldr r2,[r0,#0x0]
  00079a92: add r1,pc
  00079a94: ldr r1,[r1,#0x0]
  00079a96: ldr r2,[r2,#0x18]
  00079a98: ldr r1,[r1,#0x0]
  00079a9a: blx r2
  00079a9c: mov r4,r0
  00079a9e: ldr r0,[r5,#0x0]
  00079aa0: ldr r1,[r0,#0x0]
  00079aa2: ldr r2,[0x00079acc]
  00079aa4: ldr r3,[0x00079ad0]
  00079aa6: ldr.w r12,[r1,#0x1c4]
  00079aaa: add r2,pc
  00079aac: add r3,pc
  00079aae: mov r1,r4
  00079ab0: blx r12
  00079ab2: mov r2,r0
  00079ab4: ldr r0,[r5,#0x0]
  00079ab6: mov r1,r4
  00079ab8: pop.w {r4,r5,r7,lr}
  00079abc: b.w 0x001aae98
  00079ac0: pop {r4,r5,r7,pc}
```
