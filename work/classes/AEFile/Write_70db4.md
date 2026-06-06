# AEFile::Write
elf 0x70db4 body 48
Sig: undefined __stdcall Write(uint param_1, void * param_2, uint param_3)

## decompile
```c

/* AEFile::Write(unsigned int, void*, unsigned int) */

undefined4 AEFile::Write(uint param_1,void *param_2,uint param_3)

{
  int *piVar1;
  undefined4 uVar2;
  
  if (((**(int **)(DAT_00080de4 + 0x80dbe) != 0) &&
      (param_3 < *(uint *)**(int **)(DAT_00080de8 + 0x80dc8))) &&
     (piVar1 = *(int **)(((uint *)**(int **)(DAT_00080de8 + 0x80dc8))[1] + param_3 * 4),
     piVar1 != (int *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x00080dde. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(*piVar1 + 8))(piVar1,param_1,param_2);
    return uVar2;
  }
  return 0;
}

```
## target disasm
```
  00080db4: mov r12,r1
  00080db6: mov r1,r0
  00080db8: ldr r0,[0x00080de4]
  00080dba: add r0,pc
  00080dbc: ldr r0,[r0,#0x0]
  00080dbe: ldr r0,[r0,#0x0]
  00080dc0: cbz r0,0x00080de0
  00080dc2: ldr r0,[0x00080de8]
  00080dc4: add r0,pc
  00080dc6: ldr r0,[r0,#0x0]
  00080dc8: ldr r0,[r0,#0x0]
  00080dca: ldr r3,[r0,#0x0]
  00080dcc: cmp r3,r2
  00080dce: bls 0x00080de0
  00080dd0: ldr r0,[r0,#0x4]
  00080dd2: ldr.w r0,[r0,r2,lsl #0x2]
  00080dd6: cbz r0,0x00080de0
  00080dd8: ldr r2,[r0,#0x0]
  00080dda: ldr r3,[r2,#0x8]
  00080ddc: mov r2,r12
  00080dde: bx r3
  00080de0: movs r0,#0x0
  00080de2: bx lr
```
