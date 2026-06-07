# GlowPPShader::GlowPPShader
elf 0x8cd1c body 94
Sig: undefined __thiscall GlowPPShader(GlowPPShader * this)

## decompile
```c

/* AbyssEngine::GlowPPShader::GlowPPShader() */

void __thiscall AbyssEngine::GlowPPShader::GlowPPShader(GlowPPShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GlowPPShader *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009cd94 + 0x9cd28);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009cd98 + 0x9cd3a);
  puVar3 = *(undefined4 **)(DAT_0009cda0 + 0x9cd42);
  *(int *)this = *(int *)(DAT_0009cd9c + 0x9cd3e) + 8;
  iVar2 = DAT_0009cda4;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0x9cd52),false);
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
  0009cd1c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009cd1e: add r7,sp,#0x18
  0009cd20: mov r4,r0
  0009cd22: ldr r0,[0x0009cd94]
  0009cd24: add r0,pc
  0009cd26: ldr r5,[r0,#0x0]
  0009cd28: ldr r0,[r5,#0x0]
  0009cd2a: str r0,[sp,#0xc]
  0009cd2c: mov r0,r4
  0009cd2e: blx 0x00070930
  0009cd32: ldr r0,[0x0009cd98]
  0009cd34: ldr r1,[0x0009cd9c]
  0009cd36: add r0,pc
  0009cd38: ldr r2,[0x0009cda0]
  0009cd3a: add r1,pc
  0009cd3c: ldr r0,[r0,#0x0]
  0009cd3e: add r2,pc
  0009cd40: ldr r1,[r1,#0x0]
  0009cd42: ldr r2,[r2,#0x0]
  0009cd44: adds r1,#0x8
  0009cd46: str r1,[r4,#0x0]
  0009cd48: ldr r0,[r0,#0x0]
  0009cd4a: ldr r1,[0x0009cda4]
  0009cd4c: str r0,[r2,#0x0]
  0009cd4e: add r1,pc
  0009cd50: mov r0,sp
  0009cd52: movs r2,#0x0
  0009cd54: blx 0x0006ee18
  0009cd58: add.w r0,r4,#0xc
  0009cd5c: mov r1,sp
  0009cd5e: blx 0x0006f2b0
  0009cd62: mov r0,sp
  0009cd64: blx 0x0006ee30
  0009cd68: ldr r0,[sp,#0xc]
  0009cd6a: ldr r1,[r5,#0x0]
  0009cd6c: subs r0,r1,r0
  0009cd6e: ittt eq
  0009cd70: mov.eq r0,r4
  0009cd72: add.eq sp,#0x10
  0009cd74: pop.eq {r4,r5,r7,pc}
  0009cd76: blx 0x0006e824
```
