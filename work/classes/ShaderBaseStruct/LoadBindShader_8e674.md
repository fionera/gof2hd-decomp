# ShaderBaseStruct::LoadBindShader
elf 0x8e674 body 202
Sig: undefined __stdcall LoadBindShader(char * param_1, char * param_2)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::LoadBindShader(char const*, char const*) */

void AbyssEngine::ShaderBaseStruct::LoadBindShader(char *param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  void *pvVar6;
  AEFile *in_r2;
  uint *extraout_r2;
  uint unaff_r6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_0009e740 + 0x9e688);
  iVar1 = *piVar7;
  if ((param_2 != (char *)0x0) && (in_r2 != (AEFile *)0x0)) {
    *(char **)(param_1 + 0x18) = param_2;
    *(AEFile **)(param_1 + 0x1c) = in_r2;
    iVar2 = AEFile::OpenRead((AEFile *)param_2,&stack0xffffffd8,(uint *)&stack0xffffffd8);
    if (iVar2 != 0) {
      uVar3 = AEFile::GetFileSize(unaff_r6);
      uVar4 = uVar3 + 1;
      if ((int)uVar3 < -1) {
        uVar4 = 0xffffffff;
      }
      pcVar5 = operator_new__(uVar4);
      AEFile::Read(uVar3,pcVar5,unaff_r6);
      AEFile::Close(unaff_r6);
      pcVar5[uVar3] = '\0';
      iVar2 = AEFile::OpenRead(in_r2,&stack0xffffffd8,extraout_r2);
      if (iVar2 != 0) {
        uVar3 = AEFile::GetFileSize(unaff_r6);
        uVar4 = uVar3 + 1;
        if ((int)uVar3 < -1) {
          uVar4 = 0xffffffff;
        }
        pvVar6 = operator_new__(uVar4);
        AEFile::Read(uVar3,pvVar6,unaff_r6);
        AEFile::Close(unaff_r6);
        *(undefined1 *)((int)pvVar6 + uVar3) = 0;
        ES2LoadProgram(param_1,pcVar5);
        operator_delete__(pvVar6);
        operator_delete__(pcVar5);
      }
    }
  }
  iVar1 = *piVar7 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  0009e674: push {r4,r5,r6,r7,lr}
  0009e676: add r7,sp,#0xc
  0009e678: push {r6,r7,r8,r9,r10}
  0009e67c: mov r9,r0
  0009e67e: ldr r0,[0x0009e740]
  0009e680: mov r6,r2
  0009e682: movs r5,#0x0
  0009e684: add r0,pc
  0009e686: cmp r1,#0x0
  0009e688: ldr.w r10,[r0,#0x0]
  0009e68c: ldr.w r0,[r10,#0x0]
  0009e690: str r0,[sp,#0x4]
  0009e692: beq 0x0009e728
  0009e694: cmp r6,#0x0
  0009e696: beq 0x0009e728
  0009e698: mov r2,sp
  0009e69a: strd r1,r6,[r9,#0x18]
  0009e69e: mov r0,r1
  0009e6a0: mov r1,r2
  0009e6a2: blx 0x0006ecd4
  0009e6a6: movs r5,#0x0
  0009e6a8: cbz r0,0x0009e728
  0009e6aa: ldr r0,[sp,#0x0]
  0009e6ac: blx 0x00071044
  0009e6b0: mov r4,r0
  0009e6b2: adds r0,#0x1
  0009e6b4: cmp.w r4,#0xffffffff
  0009e6b8: it lt
  0009e6ba: mov.lt.w r0,#0xffffffff
  0009e6be: blx 0x0006ec08
  0009e6c2: ldr r2,[sp,#0x0]
  0009e6c4: mov r8,r0
  0009e6c6: mov r0,r4
  0009e6c8: mov r1,r8
  0009e6ca: blx 0x0006eb90
  0009e6ce: ldr r0,[sp,#0x0]
  0009e6d0: blx 0x0006ece0
  0009e6d4: mov r1,sp
  0009e6d6: mov r0,r6
  0009e6d8: strb.w r5,[r8,r4]
  0009e6dc: blx 0x0006ecd4
  0009e6e0: cbz r0,0x0009e728
  0009e6e2: ldr r0,[sp,#0x0]
  0009e6e4: blx 0x00071044
  0009e6e8: mov r4,r0
  0009e6ea: adds r0,#0x1
  0009e6ec: cmp.w r4,#0xffffffff
  0009e6f0: it lt
  0009e6f2: mov.lt.w r0,#0xffffffff
  0009e6f6: blx 0x0006ec08
  0009e6fa: ldr r2,[sp,#0x0]
  0009e6fc: mov r6,r0
  0009e6fe: mov r0,r4
  0009e700: mov r1,r6
  0009e702: blx 0x0006eb90
  0009e706: ldr r0,[sp,#0x0]
  0009e708: blx 0x0006ece0
  0009e70c: movs r0,#0x0
  0009e70e: mov r1,r8
  0009e710: strb r0,[r6,r4]
  0009e712: mov r0,r9
  0009e714: mov r2,r6
  0009e716: blx 0x00070954
  0009e71a: mov r5,r0
  0009e71c: mov r0,r6
  0009e71e: blx 0x0006ebfc
  0009e722: mov r0,r8
  0009e724: blx 0x0006ebfc
  0009e728: ldr r0,[sp,#0x4]
  0009e72a: ldr.w r1,[r10,#0x0]
  0009e72e: subs r0,r1,r0
  0009e730: ittt eq
  0009e732: mov.eq r0,r5
  0009e734: pop.eq.w {r2,r3,r8,r9,r10}
  0009e738: pop.eq {r4,r5,r6,r7,pc}
  0009e73a: blx 0x0006e824
```
