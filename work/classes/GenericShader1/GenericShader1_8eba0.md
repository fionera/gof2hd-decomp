# GenericShader1::GenericShader1
elf 0x8eba0 body 94
Sig: undefined __thiscall GenericShader1(GenericShader1 * this)

## decompile
```c

/* AbyssEngine::GenericShader1::GenericShader1() */

void __thiscall AbyssEngine::GenericShader1::GenericShader1(GenericShader1 *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GenericShader1 *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009ec18 + 0x9ebac);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009ec1c + 0x9ebbe);
  puVar3 = *(undefined4 **)(DAT_0009ec24 + 0x9ebc6);
  *(int *)this = *(int *)(DAT_0009ec20 + 0x9ebc2) + 8;
  iVar2 = DAT_0009ec28;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0x9ebd6),false);
  String::operator=((String *)(this + 0xc),(String *)apGStack_20);
  String::~String((String *)apGStack_20);
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
  0009eba0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009eba2: add r7,sp,#0x18
  0009eba4: mov r4,r0
  0009eba6: ldr r0,[0x0009ec18]
  0009eba8: add r0,pc
  0009ebaa: ldr r5,[r0,#0x0]
  0009ebac: ldr r0,[r5,#0x0]
  0009ebae: str r0,[sp,#0xc]
  0009ebb0: mov r0,r4
  0009ebb2: blx 0x00070930
  0009ebb6: ldr r0,[0x0009ec1c]
  0009ebb8: ldr r1,[0x0009ec20]
  0009ebba: add r0,pc
  0009ebbc: ldr r2,[0x0009ec24]
  0009ebbe: add r1,pc
  0009ebc0: ldr r0,[r0,#0x0]
  0009ebc2: add r2,pc
  0009ebc4: ldr r1,[r1,#0x0]
  0009ebc6: ldr r2,[r2,#0x0]
  0009ebc8: adds r1,#0x8
  0009ebca: str r1,[r4,#0x0]
  0009ebcc: ldr r0,[r0,#0x0]
  0009ebce: ldr r1,[0x0009ec28]
  0009ebd0: str r0,[r2,#0x0]
  0009ebd2: add r1,pc
  0009ebd4: mov r0,sp
  0009ebd6: movs r2,#0x0
  0009ebd8: blx 0x0006ee18
  0009ebdc: add.w r0,r4,#0xc
  0009ebe0: mov r1,sp
  0009ebe2: blx 0x0006f2b0
  0009ebe6: mov r0,sp
  0009ebe8: blx 0x0006ee30
  0009ebec: ldr r0,[sp,#0xc]
  0009ebee: ldr r1,[r5,#0x0]
  0009ebf0: subs r0,r1,r0
  0009ebf2: ittt eq
  0009ebf4: mov.eq r0,r4
  0009ebf6: add.eq sp,#0x10
  0009ebf8: pop.eq {r4,r5,r7,pc}
  0009ebfa: blx 0x0006e824
```
