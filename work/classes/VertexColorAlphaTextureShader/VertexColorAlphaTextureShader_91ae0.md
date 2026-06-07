# VertexColorAlphaTextureShader::VertexColorAlphaTextureShader
elf 0x91ae0 body 94
Sig: undefined __thiscall VertexColorAlphaTextureShader(VertexColorAlphaTextureShader * this)

## decompile
```c

/* AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader() */

void __thiscall
AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader
          (VertexColorAlphaTextureShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  VertexColorAlphaTextureShader *apVStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a1b58 + 0xa1aec);
  local_14 = *piVar4;
  apVStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a1b5c + 0xa1afe);
  puVar3 = *(undefined4 **)(DAT_000a1b64 + 0xa1b06);
  *(int *)this = *(int *)(DAT_000a1b60 + 0xa1b02) + 8;
  iVar2 = DAT_000a1b68;
  *puVar3 = *puVar1;
  String::String((String *)apVStack_20,(char *)(iVar2 + 0xa1b16),false);
  String::operator=((String *)(this + 0xc),(String *)apVStack_20);
  String::~String((String *)apVStack_20);
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
  000a1ae0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a1ae2: add r7,sp,#0x18
  000a1ae4: mov r4,r0
  000a1ae6: ldr r0,[0x000a1b58]
  000a1ae8: add r0,pc
  000a1aea: ldr r5,[r0,#0x0]
  000a1aec: ldr r0,[r5,#0x0]
  000a1aee: str r0,[sp,#0xc]
  000a1af0: mov r0,r4
  000a1af2: blx 0x00070930
  000a1af6: ldr r0,[0x000a1b5c]
  000a1af8: ldr r1,[0x000a1b60]
  000a1afa: add r0,pc
  000a1afc: ldr r2,[0x000a1b64]
  000a1afe: add r1,pc
  000a1b00: ldr r0,[r0,#0x0]
  000a1b02: add r2,pc
  000a1b04: ldr r1,[r1,#0x0]
  000a1b06: ldr r2,[r2,#0x0]
  000a1b08: adds r1,#0x8
  000a1b0a: str r1,[r4,#0x0]
  000a1b0c: ldr r0,[r0,#0x0]
  000a1b0e: ldr r1,[0x000a1b68]
  000a1b10: str r0,[r2,#0x0]
  000a1b12: add r1,pc
  000a1b14: mov r0,sp
  000a1b16: movs r2,#0x0
  000a1b18: blx 0x0006ee18
  000a1b1c: add.w r0,r4,#0xc
  000a1b20: mov r1,sp
  000a1b22: blx 0x0006f2b0
  000a1b26: mov r0,sp
  000a1b28: blx 0x0006ee30
  000a1b2c: ldr r0,[sp,#0xc]
  000a1b2e: ldr r1,[r5,#0x0]
  000a1b30: subs r0,r1,r0
  000a1b32: ittt eq
  000a1b34: mov.eq r0,r4
  000a1b36: add.eq sp,#0x10
  000a1b38: pop.eq {r4,r5,r7,pc}
  000a1b3a: blx 0x0006e824
```
