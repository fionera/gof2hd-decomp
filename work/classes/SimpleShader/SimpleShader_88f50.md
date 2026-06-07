# SimpleShader::SimpleShader
elf 0x88f50 body 94
Sig: undefined __thiscall SimpleShader(SimpleShader * this)

## decompile
```c

/* AbyssEngine::SimpleShader::SimpleShader() */

void __thiscall AbyssEngine::SimpleShader::SimpleShader(SimpleShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  SimpleShader *apSStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_00098fc8 + 0x98f5c);
  local_14 = *piVar4;
  apSStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_00098fcc + 0x98f6e);
  puVar3 = *(undefined4 **)(DAT_00098fd4 + 0x98f76);
  *(int *)this = *(int *)(DAT_00098fd0 + 0x98f72) + 8;
  iVar2 = DAT_00098fd8;
  *puVar3 = *puVar1;
  String::String((String *)apSStack_20,(char *)(iVar2 + 0x98f86),false);
  String::operator=((String *)(this + 0xc),(String *)apSStack_20);
  String::~String((String *)apSStack_20);
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
  00098f50: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00098f52: add r7,sp,#0x18
  00098f54: mov r4,r0
  00098f56: ldr r0,[0x00098fc8]
  00098f58: add r0,pc
  00098f5a: ldr r5,[r0,#0x0]
  00098f5c: ldr r0,[r5,#0x0]
  00098f5e: str r0,[sp,#0xc]
  00098f60: mov r0,r4
  00098f62: blx 0x00070930
  00098f66: ldr r0,[0x00098fcc]
  00098f68: ldr r1,[0x00098fd0]
  00098f6a: add r0,pc
  00098f6c: ldr r2,[0x00098fd4]
  00098f6e: add r1,pc
  00098f70: ldr r0,[r0,#0x0]
  00098f72: add r2,pc
  00098f74: ldr r1,[r1,#0x0]
  00098f76: ldr r2,[r2,#0x0]
  00098f78: adds r1,#0x8
  00098f7a: str r1,[r4,#0x0]
  00098f7c: ldr r0,[r0,#0x0]
  00098f7e: ldr r1,[0x00098fd8]
  00098f80: str r0,[r2,#0x0]
  00098f82: add r1,pc
  00098f84: mov r0,sp
  00098f86: movs r2,#0x0
  00098f88: blx 0x0006ee18
  00098f8c: add.w r0,r4,#0xc
  00098f90: mov r1,sp
  00098f92: blx 0x0006f2b0
  00098f96: mov r0,sp
  00098f98: blx 0x0006ee30
  00098f9c: ldr r0,[sp,#0xc]
  00098f9e: ldr r1,[r5,#0x0]
  00098fa0: subs r0,r1,r0
  00098fa2: ittt eq
  00098fa4: mov.eq r0,r4
  00098fa6: add.eq sp,#0x10
  00098fa8: pop.eq {r4,r5,r7,pc}
  00098faa: blx 0x0006e824
```
