# SimpleRefractionShader::SimpleRefractionShader
elf 0x8c6c0 body 94
Sig: undefined __thiscall SimpleRefractionShader(SimpleRefractionShader * this)

## decompile
```c

/* AbyssEngine::SimpleRefractionShader::SimpleRefractionShader() */

void __thiscall
AbyssEngine::SimpleRefractionShader::SimpleRefractionShader(SimpleRefractionShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  SimpleRefractionShader *apSStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009c738 + 0x9c6cc);
  local_14 = *piVar4;
  apSStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009c73c + 0x9c6de);
  puVar3 = *(undefined4 **)(DAT_0009c744 + 0x9c6e6);
  *(int *)this = *(int *)(DAT_0009c740 + 0x9c6e2) + 8;
  iVar2 = DAT_0009c748;
  *puVar3 = *puVar1;
  String::String((String *)apSStack_20,(char *)(iVar2 + 0x9c6f6),false);
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
  0009c6c0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009c6c2: add r7,sp,#0x18
  0009c6c4: mov r4,r0
  0009c6c6: ldr r0,[0x0009c738]
  0009c6c8: add r0,pc
  0009c6ca: ldr r5,[r0,#0x0]
  0009c6cc: ldr r0,[r5,#0x0]
  0009c6ce: str r0,[sp,#0xc]
  0009c6d0: mov r0,r4
  0009c6d2: blx 0x00070930
  0009c6d6: ldr r0,[0x0009c73c]
  0009c6d8: ldr r1,[0x0009c740]
  0009c6da: add r0,pc
  0009c6dc: ldr r2,[0x0009c744]
  0009c6de: add r1,pc
  0009c6e0: ldr r0,[r0,#0x0]
  0009c6e2: add r2,pc
  0009c6e4: ldr r1,[r1,#0x0]
  0009c6e6: ldr r2,[r2,#0x0]
  0009c6e8: adds r1,#0x8
  0009c6ea: str r1,[r4,#0x0]
  0009c6ec: ldr r0,[r0,#0x0]
  0009c6ee: ldr r1,[0x0009c748]
  0009c6f0: str r0,[r2,#0x0]
  0009c6f2: add r1,pc
  0009c6f4: mov r0,sp
  0009c6f6: movs r2,#0x0
  0009c6f8: blx 0x0006ee18
  0009c6fc: add.w r0,r4,#0xc
  0009c700: mov r1,sp
  0009c702: blx 0x0006f2b0
  0009c706: mov r0,sp
  0009c708: blx 0x0006ee30
  0009c70c: ldr r0,[sp,#0xc]
  0009c70e: ldr r1,[r5,#0x0]
  0009c710: subs r0,r1,r0
  0009c712: ittt eq
  0009c714: mov.eq r0,r4
  0009c716: add.eq sp,#0x10
  0009c718: pop.eq {r4,r5,r7,pc}
  0009c71a: blx 0x0006e824
```
