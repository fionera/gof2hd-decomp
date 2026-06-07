# BumpShader::BumpShader
elf 0x8b7f4 body 94
Sig: undefined __thiscall BumpShader(BumpShader * this)

## decompile
```c

/* AbyssEngine::BumpShader::BumpShader() */

void __thiscall AbyssEngine::BumpShader::BumpShader(BumpShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShader *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009b86c + 0x9b800);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009b870 + 0x9b812);
  puVar3 = *(undefined4 **)(DAT_0009b878 + 0x9b81a);
  *(int *)this = *(int *)(DAT_0009b874 + 0x9b816) + 8;
  iVar2 = DAT_0009b87c;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9b82a),false);
  String::operator=((String *)(this + 0xc),(String *)apBStack_20);
  String::~String((String *)apBStack_20);
  iVar2 = *piVar4 - local_14;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0009b7f4: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009b7f6: add r7,sp,#0x18
  0009b7f8: mov r4,r0
  0009b7fa: ldr r0,[0x0009b86c]
  0009b7fc: add r0,pc
  0009b7fe: ldr r5,[r0,#0x0]
  0009b800: ldr r0,[r5,#0x0]
  0009b802: str r0,[sp,#0xc]
  0009b804: mov r0,r4
  0009b806: blx 0x00070930
  0009b80a: ldr r0,[0x0009b870]
  0009b80c: ldr r1,[0x0009b874]
  0009b80e: add r0,pc
  0009b810: ldr r2,[0x0009b878]
  0009b812: add r1,pc
  0009b814: ldr r0,[r0,#0x0]
  0009b816: add r2,pc
  0009b818: ldr r1,[r1,#0x0]
  0009b81a: ldr r2,[r2,#0x0]
  0009b81c: adds r1,#0x8
  0009b81e: str r1,[r4,#0x0]
  0009b820: ldr r0,[r0,#0x0]
  0009b822: ldr r1,[0x0009b87c]
  0009b824: str r0,[r2,#0x0]
  0009b826: add r1,pc
  0009b828: mov r0,sp
  0009b82a: movs r2,#0x0
  0009b82c: blx 0x0006ee18
  0009b830: add.w r0,r4,#0xc
  0009b834: mov r1,sp
  0009b836: blx 0x0006f2b0
  0009b83a: mov r0,sp
  0009b83c: blx 0x0006ee30
  0009b840: ldr r0,[sp,#0xc]
  0009b842: ldr r1,[r5,#0x0]
  0009b844: subs r0,r1,r0
  0009b846: ittt eq
  0009b848: mov.eq r0,r4
  0009b84a: add.eq sp,#0x10
  0009b84c: pop.eq {r4,r5,r7,pc}
  0009b84e: blx 0x0006e824
```
