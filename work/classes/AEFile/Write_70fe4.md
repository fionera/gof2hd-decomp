# AEFile::Write
elf 0x70fe4 body 138
Sig: undefined __stdcall Write(String * param_1, uint param_2, bool param_3)

## decompile
```c

/* AEFile::Write(AbyssEngine::String const&, unsigned int, bool) */

void AEFile::Write(String *param_1,uint param_2,bool param_3)

{
  void *pvVar1;
  int iVar2;
  int *piVar3;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_00081070 + 0x80ffa);
  local_1c = *piVar3;
  if (param_3) {
    pvVar1 = (void *)AbyssEngine::String::GetAEWChar();
    local_20 = *(undefined4 *)(param_1 + 8);
    iVar2 = Write(4,&local_20,param_2);
    if (iVar2 != 0) {
      Write(*(int *)(param_1 + 8) << 1,pvVar1,param_2);
    }
  }
  else {
    pvVar1 = (void *)AbyssEngine::String::GetAEChar();
    local_24 = *(undefined4 *)(param_1 + 8);
    iVar2 = Write(4,&local_24,param_2);
    if (iVar2 != 0) {
      Write(*(uint *)(param_1 + 8),pvVar1,param_2);
    }
    operator_delete__(pvVar1);
  }
  iVar2 = *piVar3 - local_1c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```
## target disasm
```
  00080fe4: push {r4,r5,r6,r7,lr}
  00080fe6: add r7,sp,#0xc
  00080fe8: push.w r8
  00080fec: sub sp,#0x10
  00080fee: mov r5,r0
  00080ff0: ldr r0,[0x00081070]
  00080ff2: mov r4,r1
  00080ff4: cmp r2,#0x0
  00080ff6: add r0,pc
  00080ff8: ldr r6,[r0,#0x0]
  00080ffa: ldr r0,[r6,#0x0]
  00080ffc: str r0,[sp,#0xc]
  00080ffe: mov r0,r5
  00081000: beq 0x00081028
  00081002: blx 0x0006ef44
  00081006: mov r8,r0
  00081008: ldr r0,[r5,#0x8]
  0008100a: add r1,sp,#0x8
  0008100c: str r0,[sp,#0x8]
  0008100e: movs r0,#0x4
  00081010: mov r2,r4
  00081012: blx 0x0006f40c
  00081016: cbz r0,0x0008104c
  00081018: ldr r0,[r5,#0x8]
  0008101a: mov r1,r8
  0008101c: mov r2,r4
  0008101e: lsls r0,r0,#0x1
  00081020: blx 0x0006f40c
  00081024: mov r4,r0
  00081026: b 0x00081058
  00081028: blx 0x0006ef68
  0008102c: mov r8,r0
  0008102e: ldr r0,[r5,#0x8]
  00081030: add r1,sp,#0x4
  00081032: str r0,[sp,#0x4]
  00081034: movs r0,#0x4
  00081036: mov r2,r4
  00081038: blx 0x0006f40c
  0008103c: cbz r0,0x00081050
  0008103e: ldr r0,[r5,#0x8]
  00081040: mov r1,r8
  00081042: mov r2,r4
  00081044: blx 0x0006f40c
  00081048: mov r4,r0
  0008104a: b 0x00081052
  0008104c: movs r4,#0x0
  0008104e: b 0x00081058
  00081050: movs r4,#0x0
  00081052: mov r0,r8
  00081054: blx 0x0006ebfc
  00081058: ldr r0,[sp,#0xc]
  0008105a: ldr r1,[r6,#0x0]
  0008105c: subs r0,r1,r0
  0008105e: itttt eq
  00081060: mov.eq r0,r4
  00081062: add.eq sp,#0x10
  00081064: pop.eq.w r8
  00081068: pop.eq {r4,r5,r6,r7,pc}
  0008106a: blx 0x0006e824
```
