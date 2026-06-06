# AEFile::OpenWrite
elf 0x70b40 body 58
Sig: undefined __thiscall OpenWrite(AEFile * this, char * param_1, uint * param_2)

## decompile
```c

/* AEFile::OpenWrite(char const*, unsigned int*) */

void __thiscall AEFile::OpenWrite(AEFile *this,char *param_1,uint *param_2)

{
  String *pSVar1;
  int iVar2;
  int *piVar3;
  AEFile *pAStack_20;
  char *pcStack_1c;
  uint *puStack_18;
  int local_14;
  
  piVar3 = *(int **)(DAT_00080b88 + 0x80b50);
  local_14 = *piVar3;
  pAStack_20 = this;
  pcStack_1c = param_1;
  puStack_18 = param_2;
  pSVar1 = (String *)AbyssEngine::String::String((String *)&pAStack_20,(char *)this,false);
  OpenWrite(pSVar1,(uint *)param_1);
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
  00080b40: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00080b42: add r7,sp,#0x18
  00080b44: mov r4,r1
  00080b46: mov r1,r0
  00080b48: ldr r0,[0x00080b88]
  00080b4a: movs r2,#0x0
  00080b4c: add r0,pc
  00080b4e: ldr r5,[r0,#0x0]
  00080b50: ldr r0,[r5,#0x0]
  00080b52: str r0,[sp,#0xc]
  00080b54: mov r0,sp
  00080b56: blx 0x0006ee18
  00080b5a: mov r1,r4
  00080b5c: blx 0x0006f3dc
  00080b60: mov r4,r0
  00080b62: mov r0,sp
  00080b64: blx 0x0006ee30
  00080b68: ldr r0,[sp,#0xc]
  00080b6a: ldr r1,[r5,#0x0]
  00080b6c: subs r0,r1,r0
  00080b6e: ittt eq
  00080b70: mov.eq r0,r4
  00080b72: add.eq sp,#0x10
  00080b74: pop.eq {r4,r5,r7,pc}
  00080b76: blx 0x0006e824
```
