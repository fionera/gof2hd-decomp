# NFC::free_credits_followOnTwitter
elf 0x69ad4 body 66
Sig: undefined __stdcall free_credits_followOnTwitter(void)

## decompile
```c

/* NFC::free_credits_followOnTwitter() */

void NFC::free_credits_followOnTwitter(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079b18 + 0x79ade);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079b1c + 0x79aea));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079b20 + 0x79b02,DAT_00079b24 + 0x79b04);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079ad4: push {r4,r5,r7,lr}
  00079ad6: add r7,sp,#0x8
  00079ad8: ldr r0,[0x00079b18]
  00079ada: add r0,pc
  00079adc: ldr r5,[r0,#0x0]
  00079ade: ldr r0,[r5,#0x0]
  00079ae0: cbz r0,0x00079b14
  00079ae2: ldr r1,[0x00079b1c]
  00079ae4: ldr r2,[r0,#0x0]
  00079ae6: add r1,pc
  00079ae8: ldr r1,[r1,#0x0]
  00079aea: ldr r2,[r2,#0x18]
  00079aec: ldr r1,[r1,#0x0]
  00079aee: blx r2
  00079af0: mov r4,r0
  00079af2: ldr r0,[r5,#0x0]
  00079af4: ldr r1,[r0,#0x0]
  00079af6: ldr r2,[0x00079b20]
  00079af8: ldr r3,[0x00079b24]
  00079afa: ldr.w r12,[r1,#0x1c4]
  00079afe: add r2,pc
  00079b00: add r3,pc
  00079b02: mov r1,r4
  00079b04: blx r12
  00079b06: mov r2,r0
  00079b08: ldr r0,[r5,#0x0]
  00079b0a: mov r1,r4
  00079b0c: pop.w {r4,r5,r7,lr}
  00079b10: b.w 0x001aae98
  00079b14: pop {r4,r5,r7,pc}
```
