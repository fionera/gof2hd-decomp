# AEFile::Write
elf 0x70f1c body 46
Sig: undefined __stdcall Write(signed param_1, uint param_2)

## decompile
```c

/* AEFile::Write(signed char, unsigned int) */

void AEFile::Write(undefined4 param_1,uint param_2)

{
  int *piVar1;
  undefined1 uStack_15;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080f4c + 0x80f28);
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
  00080f1c: push {r2,r3,r4,r6,r7,lr}
  00080f1e: add r7,sp,#0x10
  00080f20: mov r2,r1
  00080f22: ldr r1,[0x00080f4c]
  00080f24: add r1,pc
  00080f26: ldr r4,[r1,#0x0]
  00080f28: ldr r1,[r4,#0x0]
  00080f2a: strb.w r0,[r7,#-0xd]
  00080f2e: movs r0,#0x1
  00080f30: str r1,[sp,#0x4]
  00080f32: sub.w r1,r7,#0xd
  00080f36: blx 0x0006f40c
  00080f3a: ldr r1,[sp,#0x4]
  00080f3c: ldr r2,[r4,#0x0]
  00080f3e: subs r1,r2,r1
  00080f40: itt eq
  00080f42: add.eq sp,#0x8
  00080f44: pop.eq {r4,r6,r7,pc}
  00080f46: blx 0x0006e824
```
