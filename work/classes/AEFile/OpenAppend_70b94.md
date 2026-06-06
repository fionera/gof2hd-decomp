# AEFile::OpenAppend
elf 0x70b94 body 58
Sig: undefined __thiscall OpenAppend(AEFile * this, char * param_1, uint * param_2)

## decompile
```c

/* AEFile::OpenAppend(char const*, unsigned int*) */

void __thiscall AEFile::OpenAppend(AEFile *this,char *param_1,uint *param_2)

{
  String *pSVar1;
  int iVar2;
  int *piVar3;
  AEFile *pAStack_20;
  char *pcStack_1c;
  uint *puStack_18;
  int local_14;
  
  piVar3 = *(int **)(DAT_00080bdc + 0x80ba4);
  local_14 = *piVar3;
  pAStack_20 = this;
  pcStack_1c = param_1;
  puStack_18 = param_2;
  pSVar1 = (String *)AbyssEngine::String::String((String *)&pAStack_20,(char *)this,false);
  OpenAppend(pSVar1,(uint *)param_1);
  AbyssEngine::String::~String((String *)&pAStack_20);
  iVar2 = *piVar3 - local_14;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```
## target disasm
```
  00080b94: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00080b96: add r7,sp,#0x18
  00080b98: mov r4,r1
  00080b9a: mov r1,r0
  00080b9c: ldr r0,[0x00080bdc]
  00080b9e: movs r2,#0x0
  00080ba0: add r0,pc
  00080ba2: ldr r5,[r0,#0x0]
  00080ba4: ldr r0,[r5,#0x0]
  00080ba6: str r0,[sp,#0xc]
  00080ba8: mov r0,sp
  00080baa: blx 0x0006ee18
  00080bae: mov r1,r4
  00080bb0: blx 0x0006f3e8
  00080bb4: mov r4,r0
  00080bb6: mov r0,sp
  00080bb8: blx 0x0006ee30
  00080bbc: ldr r0,[sp,#0xc]
  00080bbe: ldr r1,[r5,#0x0]
  00080bc0: subs r0,r1,r0
  00080bc2: ittt eq
  00080bc4: mov.eq r0,r4
  00080bc6: add.eq sp,#0x10
  00080bc8: pop.eq {r4,r5,r7,pc}
  00080bca: blx 0x0006e824
```
