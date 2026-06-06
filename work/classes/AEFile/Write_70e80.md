# AEFile::Write
elf 0x70e80 body 46
Sig: undefined __stdcall Write(ushort param_1, uint param_2)

## decompile
```c

/* AEFile::Write(unsigned short, unsigned int) */

void AEFile::Write(ushort param_1,uint param_2)

{
  int *piVar1;
  undefined1 auStack_16 [2];
  int local_14;
  
  piVar1 = *(int **)(DAT_00080eb0 + 0x80e8c);
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
  00080e80: push {r2,r3,r4,r6,r7,lr}
  00080e82: add r7,sp,#0x10
  00080e84: mov r2,r1
  00080e86: ldr r1,[0x00080eb0]
  00080e88: add r1,pc
  00080e8a: ldr r4,[r1,#0x0]
  00080e8c: ldr r1,[r4,#0x0]
  00080e8e: strh r0,[r7,#-0xe]
  00080e92: movs r0,#0x2
  00080e94: str r1,[sp,#0x4]
  00080e96: sub.w r1,r7,#0xe
  00080e9a: blx 0x0006f40c
  00080e9e: ldr r1,[sp,#0x4]
  00080ea0: ldr r2,[r4,#0x0]
  00080ea2: subs r1,r2,r1
  00080ea4: itt eq
  00080ea6: add.eq sp,#0x8
  00080ea8: pop.eq {r4,r6,r7,pc}
  00080eaa: blx 0x0006e824
```
