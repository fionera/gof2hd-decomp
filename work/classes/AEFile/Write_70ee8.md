# AEFile::Write
elf 0x70ee8 body 46
Sig: undefined __stdcall Write(uchar param_1, uint param_2)

## decompile
```c

/* AEFile::Write(unsigned char, unsigned int) */

void AEFile::Write(uchar param_1,uint param_2)

{
  int *piVar1;
  undefined1 uStack_15;
  int local_14;
  
  piVar1 = *(int **)(DAT_00080f18 + 0x80ef4);
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
  00080ee8: push {r2,r3,r4,r6,r7,lr}
  00080eea: add r7,sp,#0x10
  00080eec: mov r2,r1
  00080eee: ldr r1,[0x00080f18]
  00080ef0: add r1,pc
  00080ef2: ldr r4,[r1,#0x0]
  00080ef4: ldr r1,[r4,#0x0]
  00080ef6: strb.w r0,[r7,#-0xd]
  00080efa: movs r0,#0x1
  00080efc: str r1,[sp,#0x4]
  00080efe: sub.w r1,r7,#0xd
  00080f02: blx 0x0006f40c
  00080f06: ldr r1,[sp,#0x4]
  00080f08: ldr r2,[r4,#0x0]
  00080f0a: subs r1,r2,r1
  00080f0c: itt eq
  00080f0e: add.eq sp,#0x8
  00080f10: pop.eq {r4,r6,r7,pc}
  00080f12: blx 0x0006e824
```
