# AEFile::Write
elf 0x70dec body 46
Sig: undefined __stdcall Write(char param_1, uint param_2)

## decompile
```c

/* AEFile::Write(char, unsigned int) */

void AEFile::Write(char param_1,uint param_2)

{
  int *piVar1;
  undefined1 uStack_15;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080e1c + 0x80df8);
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
  00080dec: push {r2,r3,r4,r6,r7,lr}
  00080dee: add r7,sp,#0x10
  00080df0: mov r2,r1
  00080df2: ldr r1,[0x00080e1c]
  00080df4: add r1,pc
  00080df6: ldr r4,[r1,#0x0]
  00080df8: ldr r1,[r4,#0x0]
  00080dfa: strb.w r0,[r7,#-0xd]
  00080dfe: movs r0,#0x1
  00080e00: str r1,[sp,#0x4]
  00080e02: sub.w r1,r7,#0xd
  00080e06: blx 0x0006f40c
  00080e0a: ldr r1,[sp,#0x4]
  00080e0c: ldr r2,[r4,#0x0]
  00080e0e: subs r1,r2,r1
  00080e10: itt eq
  00080e12: add.eq sp,#0x8
  00080e14: pop.eq {r4,r6,r7,pc}
  00080e16: blx 0x0006e824
```
