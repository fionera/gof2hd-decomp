# NFC::openPrivacyPolicy
elf 0x69c24 body 78
Sig: undefined __stdcall openPrivacyPolicy(void)

## decompile
```c

/* NFC::openPrivacyPolicy() */

void NFC::openPrivacyPolicy(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079c68 + 0x79c2e);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079c6c + 0x79c3a));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079c70 + 0x79c52,DAT_00079c74 + 0x79c54);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  0006e818: adr r12,0x16e820
  0006e81c: add r12,r12,#0xb1000
  0006e820: ldr pc,[r12,#0xe80]!
  00079c24: push {r4,r5,r7,lr}
  00079c26: add r7,sp,#0x8
  00079c28: ldr r0,[0x00079c68]
  00079c2a: add r0,pc
  00079c2c: ldr r5,[r0,#0x0]
  00079c2e: ldr r0,[r5,#0x0]
  00079c30: cbz r0,0x00079c64
  00079c32: ldr r1,[0x00079c6c]
  00079c34: ldr r2,[r0,#0x0]
  00079c36: add r1,pc
  00079c38: ldr r1,[r1,#0x0]
  00079c3a: ldr r2,[r2,#0x18]
  00079c3c: ldr r1,[r1,#0x0]
  00079c3e: blx r2
  00079c40: mov r4,r0
  00079c42: ldr r0,[r5,#0x0]
  00079c44: ldr r1,[r0,#0x0]
  00079c46: ldr r2,[0x00079c70]
  00079c48: ldr r3,[0x00079c74]
  00079c4a: ldr.w r12,[r1,#0x1c4]
  00079c4e: add r2,pc
  00079c50: add r3,pc
  00079c52: mov r1,r4
  00079c54: blx r12
  00079c56: mov r2,r0
  00079c58: ldr r0,[r5,#0x0]
  00079c5a: mov r1,r4
  00079c5c: pop.w {r4,r5,r7,lr}
  00079c60: b.w 0x001aae98
  00079c64: pop {r4,r5,r7,pc}
```
