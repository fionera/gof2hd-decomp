# TexOnlyShader::TexOnlyShader
elf 0x8b634 body 94
Sig: undefined __thiscall TexOnlyShader(TexOnlyShader * this)

## decompile
```c

/* AbyssEngine::TexOnlyShader::TexOnlyShader() */

void __thiscall AbyssEngine::TexOnlyShader::TexOnlyShader(TexOnlyShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TexOnlyShader *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009b6ac + 0x9b640);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009b6b0 + 0x9b652);
  puVar3 = *(undefined4 **)(DAT_0009b6b8 + 0x9b65a);
  *(int *)this = *(int *)(DAT_0009b6b4 + 0x9b656) + 8;
  iVar2 = DAT_0009b6bc;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0x9b66a),false);
  String::operator=((String *)(this + 0xc),(String *)apTStack_20);
  String::~String((String *)apTStack_20);
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
  0009b634: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009b636: add r7,sp,#0x18
  0009b638: mov r4,r0
  0009b63a: ldr r0,[0x0009b6ac]
  0009b63c: add r0,pc
  0009b63e: ldr r5,[r0,#0x0]
  0009b640: ldr r0,[r5,#0x0]
  0009b642: str r0,[sp,#0xc]
  0009b644: mov r0,r4
  0009b646: blx 0x00070930
  0009b64a: ldr r0,[0x0009b6b0]
  0009b64c: ldr r1,[0x0009b6b4]
  0009b64e: add r0,pc
  0009b650: ldr r2,[0x0009b6b8]
  0009b652: add r1,pc
  0009b654: ldr r0,[r0,#0x0]
  0009b656: add r2,pc
  0009b658: ldr r1,[r1,#0x0]
  0009b65a: ldr r2,[r2,#0x0]
  0009b65c: adds r1,#0x8
  0009b65e: str r1,[r4,#0x0]
  0009b660: ldr r0,[r0,#0x0]
  0009b662: ldr r1,[0x0009b6bc]
  0009b664: str r0,[r2,#0x0]
  0009b666: add r1,pc
  0009b668: mov r0,sp
  0009b66a: movs r2,#0x0
  0009b66c: blx 0x0006ee18
  0009b670: add.w r0,r4,#0xc
  0009b674: mov r1,sp
  0009b676: blx 0x0006f2b0
  0009b67a: mov r0,sp
  0009b67c: blx 0x0006ee30
  0009b680: ldr r0,[sp,#0xc]
  0009b682: ldr r1,[r5,#0x0]
  0009b684: subs r0,r1,r0
  0009b686: ittt eq
  0009b688: mov.eq r0,r4
  0009b68a: add.eq sp,#0x10
  0009b68c: pop.eq {r4,r5,r7,pc}
  0009b68e: blx 0x0006e824
```
