# PostBWShader::PostBWShader
elf 0x8eef4 body 94
Sig: undefined __thiscall PostBWShader(PostBWShader * this)

## decompile
```c

/* AbyssEngine::PostBWShader::PostBWShader() */

void __thiscall AbyssEngine::PostBWShader::PostBWShader(PostBWShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  PostBWShader *apPStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009ef6c + 0x9ef00);
  local_14 = *piVar4;
  apPStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009ef70 + 0x9ef12);
  puVar3 = *(undefined4 **)(DAT_0009ef78 + 0x9ef1a);
  *(int *)this = *(int *)(DAT_0009ef74 + 0x9ef16) + 8;
  iVar2 = DAT_0009ef7c;
  *puVar3 = *puVar1;
  String::String((String *)apPStack_20,(char *)(iVar2 + 0x9ef2a),false);
  String::operator=((String *)(this + 0xc),(String *)apPStack_20);
  String::~String((String *)apPStack_20);
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
  0009eef4: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009eef6: add r7,sp,#0x18
  0009eef8: mov r4,r0
  0009eefa: ldr r0,[0x0009ef6c]
  0009eefc: add r0,pc
  0009eefe: ldr r5,[r0,#0x0]
  0009ef00: ldr r0,[r5,#0x0]
  0009ef02: str r0,[sp,#0xc]
  0009ef04: mov r0,r4
  0009ef06: blx 0x00070930
  0009ef0a: ldr r0,[0x0009ef70]
  0009ef0c: ldr r1,[0x0009ef74]
  0009ef0e: add r0,pc
  0009ef10: ldr r2,[0x0009ef78]
  0009ef12: add r1,pc
  0009ef14: ldr r0,[r0,#0x0]
  0009ef16: add r2,pc
  0009ef18: ldr r1,[r1,#0x0]
  0009ef1a: ldr r2,[r2,#0x0]
  0009ef1c: adds r1,#0x8
  0009ef1e: str r1,[r4,#0x0]
  0009ef20: ldr r0,[r0,#0x0]
  0009ef22: ldr r1,[0x0009ef7c]
  0009ef24: str r0,[r2,#0x0]
  0009ef26: add r1,pc
  0009ef28: mov r0,sp
  0009ef2a: movs r2,#0x0
  0009ef2c: blx 0x0006ee18
  0009ef30: add.w r0,r4,#0xc
  0009ef34: mov r1,sp
  0009ef36: blx 0x0006f2b0
  0009ef3a: mov r0,sp
  0009ef3c: blx 0x0006ee30
  0009ef40: ldr r0,[sp,#0xc]
  0009ef42: ldr r1,[r5,#0x0]
  0009ef44: subs r0,r1,r0
  0009ef46: ittt eq
  0009ef48: mov.eq r0,r4
  0009ef4a: add.eq sp,#0x10
  0009ef4c: pop.eq {r4,r5,r7,pc}
  0009ef4e: blx 0x0006e824
```
