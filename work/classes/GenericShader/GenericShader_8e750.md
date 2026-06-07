# GenericShader::GenericShader
elf 0x8e750 body 94
Sig: undefined __thiscall GenericShader(GenericShader * this)

## decompile
```c

/* AbyssEngine::GenericShader::GenericShader() */

void __thiscall AbyssEngine::GenericShader::GenericShader(GenericShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GenericShader *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009e7c8 + 0x9e75c);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009e7cc + 0x9e76e);
  puVar3 = *(undefined4 **)(DAT_0009e7d4 + 0x9e776);
  *(int *)this = *(int *)(DAT_0009e7d0 + 0x9e772) + 8;
  iVar2 = DAT_0009e7d8;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0x9e786),false);
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
  0009e750: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009e752: add r7,sp,#0x18
  0009e754: mov r4,r0
  0009e756: ldr r0,[0x0009e7c8]
  0009e758: add r0,pc
  0009e75a: ldr r5,[r0,#0x0]
  0009e75c: ldr r0,[r5,#0x0]
  0009e75e: str r0,[sp,#0xc]
  0009e760: mov r0,r4
  0009e762: blx 0x00070930
  0009e766: ldr r0,[0x0009e7cc]
  0009e768: ldr r1,[0x0009e7d0]
  0009e76a: add r0,pc
  0009e76c: ldr r2,[0x0009e7d4]
  0009e76e: add r1,pc
  0009e770: ldr r0,[r0,#0x0]
  0009e772: add r2,pc
  0009e774: ldr r1,[r1,#0x0]
  0009e776: ldr r2,[r2,#0x0]
  0009e778: adds r1,#0x8
  0009e77a: str r1,[r4,#0x0]
  0009e77c: ldr r0,[r0,#0x0]
  0009e77e: ldr r1,[0x0009e7d8]
  0009e780: str r0,[r2,#0x0]
  0009e782: add r1,pc
  0009e784: mov r0,sp
  0009e786: movs r2,#0x0
  0009e788: blx 0x0006ee18
  0009e78c: add.w r0,r4,#0xc
  0009e790: mov r1,sp
  0009e792: blx 0x0006f2b0
  0009e796: mov r0,sp
  0009e798: blx 0x0006ee30
  0009e79c: ldr r0,[sp,#0xc]
  0009e79e: ldr r1,[r5,#0x0]
  0009e7a0: subs r0,r1,r0
  0009e7a2: ittt eq
  0009e7a4: mov.eq r0,r4
  0009e7a6: add.eq sp,#0x10
  0009e7a8: pop.eq {r4,r5,r7,pc}
  0009e7aa: blx 0x0006e824
```
