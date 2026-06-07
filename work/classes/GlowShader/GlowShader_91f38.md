# GlowShader::GlowShader
elf 0x91f38 body 94
Sig: undefined __thiscall GlowShader(GlowShader * this)

## decompile
```c

/* AbyssEngine::GlowShader::GlowShader() */

void __thiscall AbyssEngine::GlowShader::GlowShader(GlowShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GlowShader *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a1fb0 + 0xa1f44);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a1fb4 + 0xa1f56);
  puVar3 = *(undefined4 **)(DAT_000a1fbc + 0xa1f5e);
  *(int *)this = *(int *)(DAT_000a1fb8 + 0xa1f5a) + 8;
  iVar2 = DAT_000a1fc0;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0xa1f6e),false);
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
  000a1f38: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a1f3a: add r7,sp,#0x18
  000a1f3c: mov r4,r0
  000a1f3e: ldr r0,[0x000a1fb0]
  000a1f40: add r0,pc
  000a1f42: ldr r5,[r0,#0x0]
  000a1f44: ldr r0,[r5,#0x0]
  000a1f46: str r0,[sp,#0xc]
  000a1f48: mov r0,r4
  000a1f4a: blx 0x00070930
  000a1f4e: ldr r0,[0x000a1fb4]
  000a1f50: ldr r1,[0x000a1fb8]
  000a1f52: add r0,pc
  000a1f54: ldr r2,[0x000a1fbc]
  000a1f56: add r1,pc
  000a1f58: ldr r0,[r0,#0x0]
  000a1f5a: add r2,pc
  000a1f5c: ldr r1,[r1,#0x0]
  000a1f5e: ldr r2,[r2,#0x0]
  000a1f60: adds r1,#0x8
  000a1f62: str r1,[r4,#0x0]
  000a1f64: ldr r0,[r0,#0x0]
  000a1f66: ldr r1,[0x000a1fc0]
  000a1f68: str r0,[r2,#0x0]
  000a1f6a: add r1,pc
  000a1f6c: mov r0,sp
  000a1f6e: movs r2,#0x0
  000a1f70: blx 0x0006ee18
  000a1f74: add.w r0,r4,#0xc
  000a1f78: mov r1,sp
  000a1f7a: blx 0x0006f2b0
  000a1f7e: mov r0,sp
  000a1f80: blx 0x0006ee30
  000a1f84: ldr r0,[sp,#0xc]
  000a1f86: ldr r1,[r5,#0x0]
  000a1f88: subs r0,r1,r0
  000a1f8a: ittt eq
  000a1f8c: mov.eq r0,r4
  000a1f8e: add.eq sp,#0x10
  000a1f90: pop.eq {r4,r5,r7,pc}
  000a1f92: blx 0x0006e824
```
