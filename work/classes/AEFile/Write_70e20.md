# AEFile::Write
elf 0x70e20 body 42
Sig: undefined __stdcall Write(uint param_1, uint param_2)

## decompile
```c

/* AEFile::Write(unsigned int, unsigned int) */

void AEFile::Write(uint param_1,uint param_2)

{
  int *piVar1;
  uint local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080e4c + 0x80e2c);
  local_14 = *piVar1;
  local_18 = param_1;
  Write(4,&local_18,param_2);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080e20: push {r2,r3,r4,r6,r7,lr}
  00080e22: add r7,sp,#0x10
  00080e24: mov r2,r1
  00080e26: ldr r1,[0x00080e4c]
  00080e28: add r1,pc
  00080e2a: ldr r4,[r1,#0x0]
  00080e2c: ldr r1,[r4,#0x0]
  00080e2e: str r0,[sp,#0x0]
  00080e30: movs r0,#0x4
  00080e32: str r1,[sp,#0x4]
  00080e34: mov r1,sp
  00080e36: blx 0x0006f40c
  00080e3a: ldr r1,[sp,#0x4]
  00080e3c: ldr r2,[r4,#0x0]
  00080e3e: subs r1,r2,r1
  00080e40: itt eq
  00080e42: add.eq sp,#0x8
  00080e44: pop.eq {r4,r6,r7,pc}
  00080e46: blx 0x0006e824
```
