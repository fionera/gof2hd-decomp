# TextureShader::TextureShader
elf 0x90ee8 body 94
Sig: undefined __thiscall TextureShader(TextureShader * this)

## decompile
```c

/* AbyssEngine::TextureShader::TextureShader() */

void __thiscall AbyssEngine::TextureShader::TextureShader(TextureShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TextureShader *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a0f60 + 0xa0ef4);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a0f64 + 0xa0f06);
  puVar3 = *(undefined4 **)(DAT_000a0f6c + 0xa0f0e);
  *(int *)this = *(int *)(DAT_000a0f68 + 0xa0f0a) + 8;
  iVar2 = DAT_000a0f70;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0xa0f1e),false);
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
  000a0ee8: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a0eea: add r7,sp,#0x18
  000a0eec: mov r4,r0
  000a0eee: ldr r0,[0x000a0f60]
  000a0ef0: add r0,pc
  000a0ef2: ldr r5,[r0,#0x0]
  000a0ef4: ldr r0,[r5,#0x0]
  000a0ef6: str r0,[sp,#0xc]
  000a0ef8: mov r0,r4
  000a0efa: blx 0x00070930
  000a0efe: ldr r0,[0x000a0f64]
  000a0f00: ldr r1,[0x000a0f68]
  000a0f02: add r0,pc
  000a0f04: ldr r2,[0x000a0f6c]
  000a0f06: add r1,pc
  000a0f08: ldr r0,[r0,#0x0]
  000a0f0a: add r2,pc
  000a0f0c: ldr r1,[r1,#0x0]
  000a0f0e: ldr r2,[r2,#0x0]
  000a0f10: adds r1,#0x8
  000a0f12: str r1,[r4,#0x0]
  000a0f14: ldr r0,[r0,#0x0]
  000a0f16: ldr r1,[0x000a0f70]
  000a0f18: str r0,[r2,#0x0]
  000a0f1a: add r1,pc
  000a0f1c: mov r0,sp
  000a0f1e: movs r2,#0x0
  000a0f20: blx 0x0006ee18
  000a0f24: add.w r0,r4,#0xc
  000a0f28: mov r1,sp
  000a0f2a: blx 0x0006f2b0
  000a0f2e: mov r0,sp
  000a0f30: blx 0x0006ee30
  000a0f34: ldr r0,[sp,#0xc]
  000a0f36: ldr r1,[r5,#0x0]
  000a0f38: subs r0,r1,r0
  000a0f3a: ittt eq
  000a0f3c: mov.eq r0,r4
  000a0f3e: add.eq sp,#0x10
  000a0f40: pop.eq {r4,r5,r7,pc}
  000a0f42: blx 0x0006e824
```
