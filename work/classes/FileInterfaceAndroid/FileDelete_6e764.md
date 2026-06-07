# FileInterfaceAndroid::FileDelete
elf 0x6e764 body 44
Sig: undefined __stdcall FileDelete(String param_1)

## decompile
```c

/* FileInterfaceAndroid::FileDelete(AbyssEngine::String) */

void FileInterfaceAndroid::FileDelete(undefined4 param_1,String *param_2,undefined4 param_3)

{
  String *this;
  int iVar1;
  int *piVar2;
  undefined4 uStack_20;
  String *pSStack_1c;
  undefined4 uStack_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_0007e790 + 0x7e770);
  local_14 = *piVar2;
  uStack_20 = param_1;
  pSStack_1c = param_2;
  uStack_18 = param_3;
  this = (String *)AbyssEngine::String::String((String *)&uStack_20,param_2,false);
  AbyssEngine::String::~String(this);
  iVar1 = *piVar2 - local_14;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  0007e764: push {r0,r1,r2,r3,r4,r6,r7,lr}
  0007e766: add r7,sp,#0x18
  0007e768: ldr r0,[0x0007e790]
  0007e76a: movs r2,#0x0
  0007e76c: add r0,pc
  0007e76e: ldr r4,[r0,#0x0]
  0007e770: ldr r0,[r4,#0x0]
  0007e772: str r0,[sp,#0xc]
  0007e774: mov r0,sp
  0007e776: blx 0x0006f028
  0007e77a: blx 0x0006ee30
  0007e77e: ldr r0,[sp,#0xc]
  0007e780: ldr r1,[r4,#0x0]
  0007e782: subs r0,r1,r0
  0007e784: ittt eq
  0007e786: mov.eq r0,#0x0
  0007e788: add.eq sp,#0x10
  0007e78a: pop.eq {r4,r6,r7,pc}
  0007e78c: blx 0x0006e824
```
