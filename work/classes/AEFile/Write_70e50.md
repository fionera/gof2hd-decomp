# AEFile::Write
elf 0x70e50 body 42
Sig: undefined __stdcall Write(int param_1, uint param_2)

## decompile
```c

/* AEFile::Write(int, unsigned int) */

void AEFile::Write(int param_1,uint param_2)

{
  int *piVar1;
  int local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080e7c + 0x80e5c);
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
  00080e50: push {r2,r3,r4,r6,r7,lr}
  00080e52: add r7,sp,#0x10
  00080e54: mov r2,r1
  00080e56: ldr r1,[0x00080e7c]
  00080e58: add r1,pc
  00080e5a: ldr r4,[r1,#0x0]
  00080e5c: ldr r1,[r4,#0x0]
  00080e5e: str r0,[sp,#0x0]
  00080e60: movs r0,#0x4
  00080e62: str r1,[sp,#0x4]
  00080e64: mov r1,sp
  00080e66: blx 0x0006f40c
  00080e6a: ldr r1,[sp,#0x4]
  00080e6c: ldr r2,[r4,#0x0]
  00080e6e: subs r1,r2,r1
  00080e70: itt eq
  00080e72: add.eq sp,#0x8
  00080e74: pop.eq {r4,r6,r7,pc}
  00080e76: blx 0x0006e824
```
