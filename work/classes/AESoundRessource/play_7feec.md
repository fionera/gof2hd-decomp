# AESoundRessource::play
elf 0x7feec body 98
Sig: undefined __stdcall play(int param_1, float param_2)

## decompile
```c

/* AbyssEngine::AESoundRessource::play(int, float) */

void AbyssEngine::AESoundRessource::play(int param_1,float param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  EVP_PKEY_CTX *in_r1;
  int *piVar4;
  int unaff_r6;
  AESoundInfo aAStack_34 [4];
  
  piVar4 = *(int **)(DAT_0008ff50 + 0x8ff00);
  iVar1 = *piVar4;
  getSoundInfo((AESoundRessource *)param_1,(int)in_r1,aAStack_34,(int *)&stack0xffffffd8);
  if (unaff_r6 != -1) {
    iVar2 = (**(code **)(**(int **)(*(int *)(param_1 + 8) + unaff_r6 * 4) + 0x38))();
    if (iVar2 == 0) {
      init((AESoundRessource *)param_1,in_r1);
    }
    piVar3 = *(int **)(*(int *)(param_1 + 8) + unaff_r6 * 4);
    if (piVar3 != (int *)0x0) {
      (**(code **)(*piVar3 + 0xc))();
    }
  }
  if (*piVar4 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008feec: push {r4,r5,r6,r7,lr}
  0008feee: add r7,sp,#0xc
  0008fef0: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0008fef4: mov r5,r0
  0008fef6: ldr r0,[0x0008ff50]
  0008fef8: mov r8,r2
  0008fefa: add r2,sp,#0x4
  0008fefc: add r0,pc
  0008fefe: add r3,sp,#0x10
  0008ff00: mov r9,r1
  0008ff02: ldr r4,[r0,#0x0]
  0008ff04: ldr r0,[r4,#0x0]
  0008ff06: str r0,[sp,#0x14]
  0008ff08: mov r0,r5
  0008ff0a: blx 0x000700c0
  0008ff0e: ldr r6,[sp,#0x10]
  0008ff10: adds r0,r6,#0x1
  0008ff12: beq 0x0008ff3a
  0008ff14: ldr r0,[r5,#0x8]
  0008ff16: ldr.w r0,[r0,r6,lsl #0x2]
  0008ff1a: ldr r1,[r0,#0x0]
  0008ff1c: ldr r1,[r1,#0x38]
  0008ff1e: blx r1
  0008ff20: cbnz r0,0x0008ff2a
  0008ff22: mov r0,r5
  0008ff24: mov r1,r9
  0008ff26: blx 0x000700d8
  0008ff2a: ldr r0,[r5,#0x8]
  0008ff2c: ldr.w r0,[r0,r6,lsl #0x2]
  0008ff30: cbz r0,0x0008ff3a
  0008ff32: ldr r1,[r0,#0x0]
  0008ff34: ldr r2,[r1,#0xc]
  0008ff36: mov r1,r8
  0008ff38: blx r2
  0008ff3a: ldr r0,[sp,#0x14]
  0008ff3c: ldr r1,[r4,#0x0]
  0008ff3e: subs r0,r1,r0
  0008ff40: ittt eq
  0008ff42: add.eq sp,#0x18
  0008ff44: pop.eq.w {r8,r9,r11}
  0008ff48: pop.eq {r4,r5,r6,r7,pc}
  0008ff4a: blx 0x0006e824
```
