# AEFile::Read
elf 0x70c44 body 162
Sig: undefined __stdcall Read(String * param_1, uint param_2, bool param_3)

## decompile
```c

/* AEFile::Read(AbyssEngine::String&, unsigned int, bool) */

void AEFile::Read(String *param_1,uint param_2,bool param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  ushort *puVar5;
  uint unaff_r6;
  int *piVar6;
  
  piVar6 = *(int **)(DAT_00080ce8 + 0x80c58);
  iVar1 = *piVar6;
  iVar2 = Read(4,&stack0xffffffd8,param_2);
  if (param_3) {
    if (iVar2 == 0) goto LAB_00080cd0;
    uVar3 = unaff_r6 + 1;
    uVar4 = uVar3 * 2;
    if (CARRY4(uVar3,uVar3)) {
      uVar4 = 0xffffffff;
    }
    puVar5 = operator_new__(uVar4);
    iVar2 = Read(unaff_r6 << 1,puVar5,param_2);
    if (iVar2 != 0) {
      puVar5[unaff_r6] = 0;
      AbyssEngine::String::Set((String *)param_1,puVar5);
    }
  }
  else {
    if (iVar2 == 0) goto LAB_00080cd0;
    puVar5 = operator_new__(unaff_r6 + 1);
    iVar2 = Read(unaff_r6,puVar5,param_2);
    if (iVar2 != 0) {
      *(undefined1 *)((int)puVar5 + unaff_r6) = 0;
      AbyssEngine::String::Set((String *)param_1,(char *)puVar5);
    }
  }
  operator_delete__(puVar5);
LAB_00080cd0:
  iVar1 = *piVar6 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```
## target disasm
```
  00080c44: push {r4,r5,r6,r7,lr}
  00080c46: add r7,sp,#0xc
  00080c48: push {r6,r7,r8,r9,r11}
  00080c4c: mov r8,r0
  00080c4e: ldr r0,[0x00080ce8]
  00080c50: mov r5,r1
  00080c52: mov r6,r2
  00080c54: add r0,pc
  00080c56: mov r1,sp
  00080c58: mov r2,r5
  00080c5a: ldr.w r9,[r0,#0x0]
  00080c5e: ldr.w r0,[r9,#0x0]
  00080c62: str r0,[sp,#0x4]
  00080c64: movs r0,#0x4
  00080c66: blx 0x0006eb90
  00080c6a: cbz r6,0x00080c9e
  00080c6c: cbz r0,0x00080cce
  00080c6e: ldr r4,[sp,#0x0]
  00080c70: adds r0,r4,#0x1
  00080c72: adds r0,r0,r0
  00080c74: it cs
  00080c76: mov.cs.w r0,#0xffffffff
  00080c7a: blx 0x0006ec08
  00080c7e: mov r6,r0
  00080c80: lsls r0,r4,#0x1
  00080c82: mov r1,r6
  00080c84: mov r2,r5
  00080c86: blx 0x0006eb90
  00080c8a: movs r5,#0x0
  00080c8c: cbz r0,0x00080cc6
  00080c8e: ldr r0,[sp,#0x0]
  00080c90: mov r1,r6
  00080c92: strh.w r5,[r6,r0,lsl #0x1]
  00080c96: mov r0,r8
  00080c98: blx 0x0006f3f4
  00080c9c: b 0x00080cc4
  00080c9e: cbz r0,0x00080cce
  00080ca0: ldr r4,[sp,#0x0]
  00080ca2: adds r0,r4,#0x1
  00080ca4: blx 0x0006ec08
  00080ca8: mov r6,r0
  00080caa: mov r0,r4
  00080cac: mov r1,r6
  00080cae: mov r2,r5
  00080cb0: blx 0x0006eb90
  00080cb4: movs r5,#0x0
  00080cb6: cbz r0,0x00080cc6
  00080cb8: ldr r0,[sp,#0x0]
  00080cba: mov r1,r6
  00080cbc: strb r5,[r6,r0]
  00080cbe: mov r0,r8
  00080cc0: blx 0x0006f2bc
  00080cc4: movs r5,#0x1
  00080cc6: mov r0,r6
  00080cc8: blx 0x0006ebfc
  00080ccc: b 0x00080cd0
  00080cce: movs r5,#0x0
  00080cd0: ldr r0,[sp,#0x4]
  00080cd2: ldr.w r1,[r9,#0x0]
  00080cd6: subs r0,r1,r0
  00080cd8: ittt eq
  00080cda: mov.eq r0,r5
  00080cdc: pop.eq.w {r2,r3,r8,r9,r11}
  00080ce0: pop.eq {r4,r5,r6,r7,pc}
  00080ce2: blx 0x0006e824
```
