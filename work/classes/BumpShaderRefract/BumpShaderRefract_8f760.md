# BumpShaderRefract::BumpShaderRefract
elf 0x8f760 body 94
Sig: undefined __thiscall BumpShaderRefract(BumpShaderRefract * this)

## decompile
```c

/* AbyssEngine::BumpShaderRefract::BumpShaderRefract() */

void __thiscall AbyssEngine::BumpShaderRefract::BumpShaderRefract(BumpShaderRefract *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderRefract *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009f7d8 + 0x9f76c);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009f7dc + 0x9f77e);
  puVar3 = *(undefined4 **)(DAT_0009f7e4 + 0x9f786);
  *(int *)this = *(int *)(DAT_0009f7e0 + 0x9f782) + 8;
  iVar2 = DAT_0009f7e8;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9f796),false);
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
  0009f760: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009f762: add r7,sp,#0x18
  0009f764: mov r4,r0
  0009f766: ldr r0,[0x0009f7d8]
  0009f768: add r0,pc
  0009f76a: ldr r5,[r0,#0x0]
  0009f76c: ldr r0,[r5,#0x0]
  0009f76e: str r0,[sp,#0xc]
  0009f770: mov r0,r4
  0009f772: blx 0x00070930
  0009f776: ldr r0,[0x0009f7dc]
  0009f778: ldr r1,[0x0009f7e0]
  0009f77a: add r0,pc
  0009f77c: ldr r2,[0x0009f7e4]
  0009f77e: add r1,pc
  0009f780: ldr r0,[r0,#0x0]
  0009f782: add r2,pc
  0009f784: ldr r1,[r1,#0x0]
  0009f786: ldr r2,[r2,#0x0]
  0009f788: adds r1,#0x8
  0009f78a: str r1,[r4,#0x0]
  0009f78c: ldr r0,[r0,#0x0]
  0009f78e: ldr r1,[0x0009f7e8]
  0009f790: str r0,[r2,#0x0]
  0009f792: add r1,pc
  0009f794: mov r0,sp
  0009f796: movs r2,#0x0
  0009f798: blx 0x0006ee18
  0009f79c: add.w r0,r4,#0xc
  0009f7a0: mov r1,sp
  0009f7a2: blx 0x0006f2b0
  0009f7a6: mov r0,sp
  0009f7a8: blx 0x0006ee30
  0009f7ac: ldr r0,[sp,#0xc]
  0009f7ae: ldr r1,[r5,#0x0]
  0009f7b0: subs r0,r1,r0
  0009f7b2: ittt eq
  0009f7b4: mov.eq r0,r4
  0009f7b6: add.eq sp,#0x10
  0009f7b8: pop.eq {r4,r5,r7,pc}
  0009f7ba: blx 0x0006e824
```
