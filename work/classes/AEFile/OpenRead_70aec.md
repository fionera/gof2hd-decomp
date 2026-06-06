# AEFile::OpenRead
elf 0x70aec body 58
Sig: undefined __thiscall OpenRead(AEFile * this, char * param_1, uint * param_2)

## decompile
```c

/* AEFile::OpenRead(char const*, unsigned int*) */

void __thiscall AEFile::OpenRead(AEFile *this,char *param_1,uint *param_2)

{
  String *pSVar1;
  int iVar2;
  int *piVar3;
  AEFile *pAStack_20;
  char *pcStack_1c;
  uint *puStack_18;
  int local_14;
  
  piVar3 = *(int **)(DAT_00080b34 + 0x80afc);
  local_14 = *piVar3;
  pAStack_20 = this;
  pcStack_1c = param_1;
  puStack_18 = param_2;
  pSVar1 = (String *)AbyssEngine::String::String((String *)&pAStack_20,(char *)this,false);
  OpenRead(pSVar1,(uint *)param_1);
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
  00080aec: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00080aee: add r7,sp,#0x18
  00080af0: mov r4,r1
  00080af2: mov r1,r0
  00080af4: ldr r0,[0x00080b34]
  00080af6: movs r2,#0x0
  00080af8: add r0,pc
  00080afa: ldr r5,[r0,#0x0]
  00080afc: ldr r0,[r5,#0x0]
  00080afe: str r0,[sp,#0xc]
  00080b00: mov r0,sp
  00080b02: blx 0x0006ee18
  00080b06: mov r1,r4
  00080b08: blx 0x0006f34c
  00080b0c: mov r4,r0
  00080b0e: mov r0,sp
  00080b10: blx 0x0006ee30
  00080b14: ldr r0,[sp,#0xc]
  00080b16: ldr r1,[r5,#0x0]
  00080b18: subs r0,r1,r0
  00080b1a: ittt eq
  00080b1c: mov.eq r0,r4
  00080b1e: add.eq sp,#0x10
  00080b20: pop.eq {r4,r5,r7,pc}
  00080b22: blx 0x0006e824
```
