# BumpShaderV2::BumpShaderV2
elf 0x8d6b8 body 94
Sig: undefined __thiscall BumpShaderV2(BumpShaderV2 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV2::BumpShaderV2() */

void __thiscall AbyssEngine::BumpShaderV2::BumpShaderV2(BumpShaderV2 *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderV2 *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009d730 + 0x9d6c4);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009d734 + 0x9d6d6);
  puVar3 = *(undefined4 **)(DAT_0009d73c + 0x9d6de);
  *(int *)this = *(int *)(DAT_0009d738 + 0x9d6da) + 8;
  iVar2 = DAT_0009d740;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9d6ee),false);
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
  0009d6b8: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009d6ba: add r7,sp,#0x18
  0009d6bc: mov r4,r0
  0009d6be: ldr r0,[0x0009d730]
  0009d6c0: add r0,pc
  0009d6c2: ldr r5,[r0,#0x0]
  0009d6c4: ldr r0,[r5,#0x0]
  0009d6c6: str r0,[sp,#0xc]
  0009d6c8: mov r0,r4
  0009d6ca: blx 0x00070930
  0009d6ce: ldr r0,[0x0009d734]
  0009d6d0: ldr r1,[0x0009d738]
  0009d6d2: add r0,pc
  0009d6d4: ldr r2,[0x0009d73c]
  0009d6d6: add r1,pc
  0009d6d8: ldr r0,[r0,#0x0]
  0009d6da: add r2,pc
  0009d6dc: ldr r1,[r1,#0x0]
  0009d6de: ldr r2,[r2,#0x0]
  0009d6e0: adds r1,#0x8
  0009d6e2: str r1,[r4,#0x0]
  0009d6e4: ldr r0,[r0,#0x0]
  0009d6e6: ldr r1,[0x0009d740]
  0009d6e8: str r0,[r2,#0x0]
  0009d6ea: add r1,pc
  0009d6ec: mov r0,sp
  0009d6ee: movs r2,#0x0
  0009d6f0: blx 0x0006ee18
  0009d6f4: add.w r0,r4,#0xc
  0009d6f8: mov r1,sp
  0009d6fa: blx 0x0006f2b0
  0009d6fe: mov r0,sp
  0009d700: blx 0x0006ee30
  0009d704: ldr r0,[sp,#0xc]
  0009d706: ldr r1,[r5,#0x0]
  0009d708: subs r0,r1,r0
  0009d70a: ittt eq
  0009d70c: mov.eq r0,r4
  0009d70e: add.eq sp,#0x10
  0009d710: pop.eq {r4,r5,r7,pc}
  0009d712: blx 0x0006e824
```
