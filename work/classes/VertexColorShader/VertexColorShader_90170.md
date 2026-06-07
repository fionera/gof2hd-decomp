# VertexColorShader::VertexColorShader
elf 0x90170 body 94
Sig: undefined __thiscall VertexColorShader(VertexColorShader * this)

## decompile
```c

/* AbyssEngine::VertexColorShader::VertexColorShader() */

void __thiscall AbyssEngine::VertexColorShader::VertexColorShader(VertexColorShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  VertexColorShader *apVStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a01e8 + 0xa017c);
  local_14 = *piVar4;
  apVStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a01ec + 0xa018e);
  puVar3 = *(undefined4 **)(DAT_000a01f4 + 0xa0196);
  *(int *)this = *(int *)(DAT_000a01f0 + 0xa0192) + 8;
  iVar2 = DAT_000a01f8;
  *puVar3 = *puVar1;
  String::String((String *)apVStack_20,(char *)(iVar2 + 0xa01a6),false);
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
  000a0170: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a0172: add r7,sp,#0x18
  000a0174: mov r4,r0
  000a0176: ldr r0,[0x000a01e8]
  000a0178: add r0,pc
  000a017a: ldr r5,[r0,#0x0]
  000a017c: ldr r0,[r5,#0x0]
  000a017e: str r0,[sp,#0xc]
  000a0180: mov r0,r4
  000a0182: blx 0x00070930
  000a0186: ldr r0,[0x000a01ec]
  000a0188: ldr r1,[0x000a01f0]
  000a018a: add r0,pc
  000a018c: ldr r2,[0x000a01f4]
  000a018e: add r1,pc
  000a0190: ldr r0,[r0,#0x0]
  000a0192: add r2,pc
  000a0194: ldr r1,[r1,#0x0]
  000a0196: ldr r2,[r2,#0x0]
  000a0198: adds r1,#0x8
  000a019a: str r1,[r4,#0x0]
  000a019c: ldr r0,[r0,#0x0]
  000a019e: ldr r1,[0x000a01f8]
  000a01a0: str r0,[r2,#0x0]
  000a01a2: add r1,pc
  000a01a4: mov r0,sp
  000a01a6: movs r2,#0x0
  000a01a8: blx 0x0006ee18
  000a01ac: add.w r0,r4,#0xc
  000a01b0: mov r1,sp
  000a01b2: blx 0x0006f2b0
  000a01b6: mov r0,sp
  000a01b8: blx 0x0006ee30
  000a01bc: ldr r0,[sp,#0xc]
  000a01be: ldr r1,[r5,#0x0]
  000a01c0: subs r0,r1,r0
  000a01c2: ittt eq
  000a01c4: mov.eq r0,r4
  000a01c6: add.eq sp,#0x10
  000a01c8: pop.eq {r4,r5,r7,pc}
  000a01ca: blx 0x0006e824
```
