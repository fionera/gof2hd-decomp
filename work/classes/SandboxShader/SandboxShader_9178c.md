# SandboxShader::SandboxShader
elf 0x9178c body 94
Sig: undefined __thiscall SandboxShader(SandboxShader * this)

## decompile
```c

/* AbyssEngine::SandboxShader::SandboxShader() */

void __thiscall AbyssEngine::SandboxShader::SandboxShader(SandboxShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  SandboxShader *apSStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a1804 + 0xa1798);
  local_14 = *piVar4;
  apSStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a1808 + 0xa17aa);
  puVar3 = *(undefined4 **)(DAT_000a1810 + 0xa17b2);
  *(int *)this = *(int *)(DAT_000a180c + 0xa17ae) + 8;
  iVar2 = DAT_000a1814;
  *puVar3 = *puVar1;
  String::String((String *)apSStack_20,(char *)(iVar2 + 0xa17c2),false);
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
  000a178c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a178e: add r7,sp,#0x18
  000a1790: mov r4,r0
  000a1792: ldr r0,[0x000a1804]
  000a1794: add r0,pc
  000a1796: ldr r5,[r0,#0x0]
  000a1798: ldr r0,[r5,#0x0]
  000a179a: str r0,[sp,#0xc]
  000a179c: mov r0,r4
  000a179e: blx 0x00070930
  000a17a2: ldr r0,[0x000a1808]
  000a17a4: ldr r1,[0x000a180c]
  000a17a6: add r0,pc
  000a17a8: ldr r2,[0x000a1810]
  000a17aa: add r1,pc
  000a17ac: ldr r0,[r0,#0x0]
  000a17ae: add r2,pc
  000a17b0: ldr r1,[r1,#0x0]
  000a17b2: ldr r2,[r2,#0x0]
  000a17b4: adds r1,#0x8
  000a17b6: str r1,[r4,#0x0]
  000a17b8: ldr r0,[r0,#0x0]
  000a17ba: ldr r1,[0x000a1814]
  000a17bc: str r0,[r2,#0x0]
  000a17be: add r1,pc
  000a17c0: mov r0,sp
  000a17c2: movs r2,#0x0
  000a17c4: blx 0x0006ee18
  000a17c8: add.w r0,r4,#0xc
  000a17cc: mov r1,sp
  000a17ce: blx 0x0006f2b0
  000a17d2: mov r0,sp
  000a17d4: blx 0x0006ee30
  000a17d8: ldr r0,[sp,#0xc]
  000a17da: ldr r1,[r5,#0x0]
  000a17dc: subs r0,r1,r0
  000a17de: ittt eq
  000a17e0: mov.eq r0,r4
  000a17e2: add.eq sp,#0x10
  000a17e4: pop.eq {r4,r5,r7,pc}
  000a17e6: blx 0x0006e824
```
