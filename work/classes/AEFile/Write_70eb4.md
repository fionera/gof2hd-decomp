# AEFile::Write
elf 0x70eb4 body 46
Sig: undefined __stdcall Write(short param_1, uint param_2)

## decompile
```c

/* AEFile::Write(short, unsigned int) */

void AEFile::Write(short param_1,uint param_2)

{
  int *piVar1;
  undefined1 auStack_16 [2];
  int local_14;
  
  piVar1 = *(int **)(DAT_00080ee4 + 0x80ec0);
  local_14 = *piVar1;
  Write(2,auStack_16,param_2);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080eb4: push {r2,r3,r4,r6,r7,lr}
  00080eb6: add r7,sp,#0x10
  00080eb8: mov r2,r1
  00080eba: ldr r1,[0x00080ee4]
  00080ebc: add r1,pc
  00080ebe: ldr r4,[r1,#0x0]
  00080ec0: ldr r1,[r4,#0x0]
  00080ec2: strh r0,[r7,#-0xe]
  00080ec6: movs r0,#0x2
  00080ec8: str r1,[sp,#0x4]
  00080eca: sub.w r1,r7,#0xe
  00080ece: blx 0x0006f40c
  00080ed2: ldr r1,[sp,#0x4]
  00080ed4: ldr r2,[r4,#0x0]
  00080ed6: subs r1,r2,r1
  00080ed8: itt eq
  00080eda: add.eq sp,#0x8
  00080edc: pop.eq {r4,r6,r7,pc}
  00080ede: blx 0x0006e824
```
