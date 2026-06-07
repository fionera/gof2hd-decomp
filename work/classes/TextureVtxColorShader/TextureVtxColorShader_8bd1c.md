# TextureVtxColorShader::TextureVtxColorShader
elf 0x8bd1c body 94
Sig: undefined __thiscall TextureVtxColorShader(TextureVtxColorShader * this)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::TextureVtxColorShader() */

void __thiscall
AbyssEngine::TextureVtxColorShader::TextureVtxColorShader(TextureVtxColorShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TextureVtxColorShader *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009bd94 + 0x9bd28);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009bd98 + 0x9bd3a);
  puVar3 = *(undefined4 **)(DAT_0009bda0 + 0x9bd42);
  *(int *)this = *(int *)(DAT_0009bd9c + 0x9bd3e) + 8;
  iVar2 = DAT_0009bda4;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0x9bd52),false);
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
  0009bd1c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009bd1e: add r7,sp,#0x18
  0009bd20: mov r4,r0
  0009bd22: ldr r0,[0x0009bd94]
  0009bd24: add r0,pc
  0009bd26: ldr r5,[r0,#0x0]
  0009bd28: ldr r0,[r5,#0x0]
  0009bd2a: str r0,[sp,#0xc]
  0009bd2c: mov r0,r4
  0009bd2e: blx 0x00070930
  0009bd32: ldr r0,[0x0009bd98]
  0009bd34: ldr r1,[0x0009bd9c]
  0009bd36: add r0,pc
  0009bd38: ldr r2,[0x0009bda0]
  0009bd3a: add r1,pc
  0009bd3c: ldr r0,[r0,#0x0]
  0009bd3e: add r2,pc
  0009bd40: ldr r1,[r1,#0x0]
  0009bd42: ldr r2,[r2,#0x0]
  0009bd44: adds r1,#0x8
  0009bd46: str r1,[r4,#0x0]
  0009bd48: ldr r0,[r0,#0x0]
  0009bd4a: ldr r1,[0x0009bda4]
  0009bd4c: str r0,[r2,#0x0]
  0009bd4e: add r1,pc
  0009bd50: mov r0,sp
  0009bd52: movs r2,#0x0
  0009bd54: blx 0x0006ee18
  0009bd58: add.w r0,r4,#0xc
  0009bd5c: mov r1,sp
  0009bd5e: blx 0x0006f2b0
  0009bd62: mov r0,sp
  0009bd64: blx 0x0006ee30
  0009bd68: ldr r0,[sp,#0xc]
  0009bd6a: ldr r1,[r5,#0x0]
  0009bd6c: subs r0,r1,r0
  0009bd6e: ittt eq
  0009bd70: mov.eq r0,r4
  0009bd72: add.eq sp,#0x10
  0009bd74: pop.eq {r4,r5,r7,pc}
  0009bd76: blx 0x0006e824
```
