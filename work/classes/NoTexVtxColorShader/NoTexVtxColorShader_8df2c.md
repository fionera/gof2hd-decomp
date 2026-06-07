# NoTexVtxColorShader::NoTexVtxColorShader
elf 0x8df2c body 94
Sig: undefined __thiscall NoTexVtxColorShader(NoTexVtxColorShader * this)

## decompile
```c

/* AbyssEngine::NoTexVtxColorShader::NoTexVtxColorShader() */

void __thiscall AbyssEngine::NoTexVtxColorShader::NoTexVtxColorShader(NoTexVtxColorShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  NoTexVtxColorShader *apNStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009dfa4 + 0x9df38);
  local_14 = *piVar4;
  apNStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009dfa8 + 0x9df4a);
  puVar3 = *(undefined4 **)(DAT_0009dfb0 + 0x9df52);
  *(int *)this = *(int *)(DAT_0009dfac + 0x9df4e) + 8;
  iVar2 = DAT_0009dfb4;
  *puVar3 = *puVar1;
  String::String((String *)apNStack_20,(char *)(iVar2 + 0x9df62),false);
  String::operator=((String *)(this + 0xc),(String *)apNStack_20);
  String::~String((String *)apNStack_20);
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
  0009df2c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009df2e: add r7,sp,#0x18
  0009df30: mov r4,r0
  0009df32: ldr r0,[0x0009dfa4]
  0009df34: add r0,pc
  0009df36: ldr r5,[r0,#0x0]
  0009df38: ldr r0,[r5,#0x0]
  0009df3a: str r0,[sp,#0xc]
  0009df3c: mov r0,r4
  0009df3e: blx 0x00070930
  0009df42: ldr r0,[0x0009dfa8]
  0009df44: ldr r1,[0x0009dfac]
  0009df46: add r0,pc
  0009df48: ldr r2,[0x0009dfb0]
  0009df4a: add r1,pc
  0009df4c: ldr r0,[r0,#0x0]
  0009df4e: add r2,pc
  0009df50: ldr r1,[r1,#0x0]
  0009df52: ldr r2,[r2,#0x0]
  0009df54: adds r1,#0x8
  0009df56: str r1,[r4,#0x0]
  0009df58: ldr r0,[r0,#0x0]
  0009df5a: ldr r1,[0x0009dfb4]
  0009df5c: str r0,[r2,#0x0]
  0009df5e: add r1,pc
  0009df60: mov r0,sp
  0009df62: movs r2,#0x0
  0009df64: blx 0x0006ee18
  0009df68: add.w r0,r4,#0xc
  0009df6c: mov r1,sp
  0009df6e: blx 0x0006f2b0
  0009df72: mov r0,sp
  0009df74: blx 0x0006ee30
  0009df78: ldr r0,[sp,#0xc]
  0009df7a: ldr r1,[r5,#0x0]
  0009df7c: subs r0,r1,r0
  0009df7e: ittt eq
  0009df80: mov.eq r0,r4
  0009df82: add.eq sp,#0x10
  0009df84: pop.eq {r4,r5,r7,pc}
  0009df86: blx 0x0006e824
```
