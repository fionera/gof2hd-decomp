# TextureLightShader::TextureLightShader
elf 0x8dadc body 94
Sig: undefined __thiscall TextureLightShader(TextureLightShader * this)

## decompile
```c

/* AbyssEngine::TextureLightShader::TextureLightShader() */

void __thiscall AbyssEngine::TextureLightShader::TextureLightShader(TextureLightShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TextureLightShader *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009db54 + 0x9dae8);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009db58 + 0x9dafa);
  puVar3 = *(undefined4 **)(DAT_0009db60 + 0x9db02);
  *(int *)this = *(int *)(DAT_0009db5c + 0x9dafe) + 8;
  iVar2 = DAT_0009db64;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0x9db12),false);
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
  0009dadc: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009dade: add r7,sp,#0x18
  0009dae0: mov r4,r0
  0009dae2: ldr r0,[0x0009db54]
  0009dae4: add r0,pc
  0009dae6: ldr r5,[r0,#0x0]
  0009dae8: ldr r0,[r5,#0x0]
  0009daea: str r0,[sp,#0xc]
  0009daec: mov r0,r4
  0009daee: blx 0x00070930
  0009daf2: ldr r0,[0x0009db58]
  0009daf4: ldr r1,[0x0009db5c]
  0009daf6: add r0,pc
  0009daf8: ldr r2,[0x0009db60]
  0009dafa: add r1,pc
  0009dafc: ldr r0,[r0,#0x0]
  0009dafe: add r2,pc
  0009db00: ldr r1,[r1,#0x0]
  0009db02: ldr r2,[r2,#0x0]
  0009db04: adds r1,#0x8
  0009db06: str r1,[r4,#0x0]
  0009db08: ldr r0,[r0,#0x0]
  0009db0a: ldr r1,[0x0009db64]
  0009db0c: str r0,[r2,#0x0]
  0009db0e: add r1,pc
  0009db10: mov r0,sp
  0009db12: movs r2,#0x0
  0009db14: blx 0x0006ee18
  0009db18: add.w r0,r4,#0xc
  0009db1c: mov r1,sp
  0009db1e: blx 0x0006f2b0
  0009db22: mov r0,sp
  0009db24: blx 0x0006ee30
  0009db28: ldr r0,[sp,#0xc]
  0009db2a: ldr r1,[r5,#0x0]
  0009db2c: subs r0,r1,r0
  0009db2e: ittt eq
  0009db30: mov.eq r0,r4
  0009db32: add.eq sp,#0x10
  0009db34: pop.eq {r4,r5,r7,pc}
  0009db36: blx 0x0006e824
```
