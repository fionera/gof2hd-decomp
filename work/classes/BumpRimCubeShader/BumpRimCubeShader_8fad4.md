# BumpRimCubeShader::BumpRimCubeShader
elf 0x8fad4 body 94
Sig: undefined __thiscall BumpRimCubeShader(BumpRimCubeShader * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader::BumpRimCubeShader() */

void __thiscall AbyssEngine::BumpRimCubeShader::BumpRimCubeShader(BumpRimCubeShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpRimCubeShader *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009fb4c + 0x9fae0);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009fb50 + 0x9faf2);
  puVar3 = *(undefined4 **)(DAT_0009fb58 + 0x9fafa);
  *(int *)this = *(int *)(DAT_0009fb54 + 0x9faf6) + 8;
  iVar2 = DAT_0009fb5c;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9fb0a),false);
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
  0009fad4: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009fad6: add r7,sp,#0x18
  0009fad8: mov r4,r0
  0009fada: ldr r0,[0x0009fb4c]
  0009fadc: add r0,pc
  0009fade: ldr r5,[r0,#0x0]
  0009fae0: ldr r0,[r5,#0x0]
  0009fae2: str r0,[sp,#0xc]
  0009fae4: mov r0,r4
  0009fae6: blx 0x00070930
  0009faea: ldr r0,[0x0009fb50]
  0009faec: ldr r1,[0x0009fb54]
  0009faee: add r0,pc
  0009faf0: ldr r2,[0x0009fb58]
  0009faf2: add r1,pc
  0009faf4: ldr r0,[r0,#0x0]
  0009faf6: add r2,pc
  0009faf8: ldr r1,[r1,#0x0]
  0009fafa: ldr r2,[r2,#0x0]
  0009fafc: adds r1,#0x8
  0009fafe: str r1,[r4,#0x0]
  0009fb00: ldr r0,[r0,#0x0]
  0009fb02: ldr r1,[0x0009fb5c]
  0009fb04: str r0,[r2,#0x0]
  0009fb06: add r1,pc
  0009fb08: mov r0,sp
  0009fb0a: movs r2,#0x0
  0009fb0c: blx 0x0006ee18
  0009fb10: add.w r0,r4,#0xc
  0009fb14: mov r1,sp
  0009fb16: blx 0x0006f2b0
  0009fb1a: mov r0,sp
  0009fb1c: blx 0x0006ee30
  0009fb20: ldr r0,[sp,#0xc]
  0009fb22: ldr r1,[r5,#0x0]
  0009fb24: subs r0,r1,r0
  0009fb26: ittt eq
  0009fb28: mov.eq r0,r4
  0009fb2a: add.eq sp,#0x10
  0009fb2c: pop.eq {r4,r5,r7,pc}
  0009fb2e: blx 0x0006e824
```
