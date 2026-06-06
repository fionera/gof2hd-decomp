# AEFile::ReadSwitched
elf 0x70d40 body 110
Sig: undefined __stdcall ReadSwitched(String * param_1, uint param_2)

## decompile
```c

/* AEFile::ReadSwitched(AbyssEngine::String&, unsigned int) */

void AEFile::ReadSwitched(String *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  uint unaff_r6;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00080db0 + 0x80d52);
  iVar1 = *piVar4;
  iVar2 = ReadSwitched((ushort *)&stack0xffffffda,param_2);
  if (iVar2 != 0) {
    pcVar3 = operator_new__((unaff_r6 >> 0x10) + 1);
    iVar2 = Read(unaff_r6 >> 0x10,pcVar3,param_2);
    if (iVar2 != 0) {
      pcVar3[unaff_r6 >> 0x10] = '\0';
      AbyssEngine::String::Set((String *)param_1,pcVar3);
    }
    operator_delete__(pcVar3);
  }
  iVar1 = *piVar4 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```
## target disasm
```
  00080d40: push {r4,r5,r6,r7,lr}
  00080d42: add r7,sp,#0xc
  00080d44: push {r6,r7,r8,r9,r11}
  00080d48: mov r8,r0
  00080d4a: ldr r0,[0x00080db0]
  00080d4c: mov r5,r1
  00080d4e: add r0,pc
  00080d50: ldr.w r9,[r0,#0x0]
  00080d54: ldr.w r0,[r9,#0x0]
  00080d58: str r0,[sp,#0x4]
  00080d5a: sub.w r0,r7,#0x1e
  00080d5e: blx 0x0006f400
  00080d62: cbz r0,0x00080d96
  00080d64: ldrh.w r4,[r7,#-0x1e]
  00080d68: adds r0,r4,#0x1
  00080d6a: blx 0x0006ec08
  00080d6e: mov r6,r0
  00080d70: mov r0,r4
  00080d72: mov r1,r6
  00080d74: mov r2,r5
  00080d76: blx 0x0006eb90
  00080d7a: movs r5,#0x0
  00080d7c: cbz r0,0x00080d8e
  00080d7e: ldrh.w r0,[r7,#-0x1e]
  00080d82: mov r1,r6
  00080d84: strb r5,[r6,r0]
  00080d86: mov r0,r8
  00080d88: blx 0x0006f2bc
  00080d8c: movs r5,#0x1
  00080d8e: mov r0,r6
  00080d90: blx 0x0006ebfc
  00080d94: b 0x00080d98
  00080d96: movs r5,#0x0
  00080d98: ldr r0,[sp,#0x4]
  00080d9a: ldr.w r1,[r9,#0x0]
  00080d9e: subs r0,r1,r0
  00080da0: ittt eq
  00080da2: mov.eq r0,r5
  00080da4: pop.eq.w {r2,r3,r8,r9,r11}
  00080da8: pop.eq {r4,r5,r6,r7,pc}
  00080daa: blx 0x0006e824
```
