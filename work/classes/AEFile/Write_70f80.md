# AEFile::Write
elf 0x70f80 body 46
Sig: undefined __stdcall Write(bool param_1, uint param_2)

## decompile
```c

/* AEFile::Write(bool, unsigned int) */

void AEFile::Write(bool param_1,uint param_2)

{
  int *piVar1;
  undefined1 uStack_15;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080fb0 + 0x80f8c);
  local_14 = *piVar1;
  Write(1,&uStack_15,param_2);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080f80: push {r2,r3,r4,r6,r7,lr}
  00080f82: add r7,sp,#0x10
  00080f84: mov r2,r1
  00080f86: ldr r1,[0x00080fb0]
  00080f88: add r1,pc
  00080f8a: ldr r4,[r1,#0x0]
  00080f8c: ldr r1,[r4,#0x0]
  00080f8e: strb.w r0,[r7,#-0xd]
  00080f92: movs r0,#0x1
  00080f94: str r1,[sp,#0x4]
  00080f96: sub.w r1,r7,#0xd
  00080f9a: blx 0x0006f40c
  00080f9e: ldr r1,[sp,#0x4]
  00080fa0: ldr r2,[r4,#0x0]
  00080fa2: subs r1,r2,r1
  00080fa4: itt eq
  00080fa6: add.eq sp,#0x8
  00080fa8: pop.eq {r4,r6,r7,pc}
  00080faa: blx 0x0006e824
```
