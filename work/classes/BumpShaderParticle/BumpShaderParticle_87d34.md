# BumpShaderParticle::BumpShaderParticle
elf 0x87d34 body 94
Sig: undefined __thiscall BumpShaderParticle(BumpShaderParticle * this)

## decompile
```c

/* AbyssEngine::BumpShaderParticle::BumpShaderParticle() */

void __thiscall AbyssEngine::BumpShaderParticle::BumpShaderParticle(BumpShaderParticle *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderParticle *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_00097dac + 0x97d40);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_00097db0 + 0x97d52);
  puVar3 = *(undefined4 **)(DAT_00097db8 + 0x97d5a);
  *(int *)this = *(int *)(DAT_00097db4 + 0x97d56) + 8;
  iVar2 = DAT_00097dbc;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x97d6a),false);
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
  00097d34: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00097d36: add r7,sp,#0x18
  00097d38: mov r4,r0
  00097d3a: ldr r0,[0x00097dac]
  00097d3c: add r0,pc
  00097d3e: ldr r5,[r0,#0x0]
  00097d40: ldr r0,[r5,#0x0]
  00097d42: str r0,[sp,#0xc]
  00097d44: mov r0,r4
  00097d46: blx 0x00070930
  00097d4a: ldr r0,[0x00097db0]
  00097d4c: ldr r1,[0x00097db4]
  00097d4e: add r0,pc
  00097d50: ldr r2,[0x00097db8]
  00097d52: add r1,pc
  00097d54: ldr r0,[r0,#0x0]
  00097d56: add r2,pc
  00097d58: ldr r1,[r1,#0x0]
  00097d5a: ldr r2,[r2,#0x0]
  00097d5c: adds r1,#0x8
  00097d5e: str r1,[r4,#0x0]
  00097d60: ldr r0,[r0,#0x0]
  00097d62: ldr r1,[0x00097dbc]
  00097d64: str r0,[r2,#0x0]
  00097d66: add r1,pc
  00097d68: mov r0,sp
  00097d6a: movs r2,#0x0
  00097d6c: blx 0x0006ee18
  00097d70: add.w r0,r4,#0xc
  00097d74: mov r1,sp
  00097d76: blx 0x0006f2b0
  00097d7a: mov r0,sp
  00097d7c: blx 0x0006ee30
  00097d80: ldr r0,[sp,#0xc]
  00097d82: ldr r1,[r5,#0x0]
  00097d84: subs r0,r1,r0
  00097d86: ittt eq
  00097d88: mov.eq r0,r4
  00097d8a: add.eq sp,#0x10
  00097d8c: pop.eq {r4,r5,r7,pc}
  00097d8e: blx 0x0006e824
```
