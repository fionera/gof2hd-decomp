# AEFile::Write
elf 0x70fb4 body 42
Sig: undefined __stdcall Write(float param_1, uint param_2)

## decompile
```c

/* AEFile::Write(float, unsigned int) */

void AEFile::Write(float param_1,uint param_2)

{
  uint in_r1;
  int *piVar1;
  uint local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080fe0 + 0x80fc0);
  local_14 = *piVar1;
  local_18 = param_2;
  Write(4,&local_18,in_r1);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080fb4: push {r2,r3,r4,r6,r7,lr}
  00080fb6: add r7,sp,#0x10
  00080fb8: mov r2,r1
  00080fba: ldr r1,[0x00080fe0]
  00080fbc: add r1,pc
  00080fbe: ldr r4,[r1,#0x0]
  00080fc0: ldr r1,[r4,#0x0]
  00080fc2: str r0,[sp,#0x0]
  00080fc4: movs r0,#0x4
  00080fc6: str r1,[sp,#0x4]
  00080fc8: mov r1,sp
  00080fca: blx 0x0006f40c
  00080fce: ldr r1,[sp,#0x4]
  00080fd0: ldr r2,[r4,#0x0]
  00080fd2: subs r1,r2,r1
  00080fd4: itt eq
  00080fd6: add.eq sp,#0x8
  00080fd8: pop.eq {r4,r6,r7,pc}
  00080fda: blx 0x0006e824
```
