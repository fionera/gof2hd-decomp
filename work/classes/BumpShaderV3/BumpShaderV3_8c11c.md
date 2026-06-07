# BumpShaderV3::BumpShaderV3
elf 0x8c11c body 94
Sig: undefined __thiscall BumpShaderV3(BumpShaderV3 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV3::BumpShaderV3() */

void __thiscall AbyssEngine::BumpShaderV3::BumpShaderV3(BumpShaderV3 *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderV3 *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009c194 + 0x9c128);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009c198 + 0x9c13a);
  puVar3 = *(undefined4 **)(DAT_0009c1a0 + 0x9c142);
  *(int *)this = *(int *)(DAT_0009c19c + 0x9c13e) + 8;
  iVar2 = DAT_0009c1a4;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9c152),false);
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
  0009c11c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009c11e: add r7,sp,#0x18
  0009c120: mov r4,r0
  0009c122: ldr r0,[0x0009c194]
  0009c124: add r0,pc
  0009c126: ldr r5,[r0,#0x0]
  0009c128: ldr r0,[r5,#0x0]
  0009c12a: str r0,[sp,#0xc]
  0009c12c: mov r0,r4
  0009c12e: blx 0x00070930
  0009c132: ldr r0,[0x0009c198]
  0009c134: ldr r1,[0x0009c19c]
  0009c136: add r0,pc
  0009c138: ldr r2,[0x0009c1a0]
  0009c13a: add r1,pc
  0009c13c: ldr r0,[r0,#0x0]
  0009c13e: add r2,pc
  0009c140: ldr r1,[r1,#0x0]
  0009c142: ldr r2,[r2,#0x0]
  0009c144: adds r1,#0x8
  0009c146: str r1,[r4,#0x0]
  0009c148: ldr r0,[r0,#0x0]
  0009c14a: ldr r1,[0x0009c1a4]
  0009c14c: str r0,[r2,#0x0]
  0009c14e: add r1,pc
  0009c150: mov r0,sp
  0009c152: movs r2,#0x0
  0009c154: blx 0x0006ee18
  0009c158: add.w r0,r4,#0xc
  0009c15c: mov r1,sp
  0009c15e: blx 0x0006f2b0
  0009c162: mov r0,sp
  0009c164: blx 0x0006ee30
  0009c168: ldr r0,[sp,#0xc]
  0009c16a: ldr r1,[r5,#0x0]
  0009c16c: subs r0,r1,r0
  0009c16e: ittt eq
  0009c170: mov.eq r0,r4
  0009c172: add.eq sp,#0x10
  0009c174: pop.eq {r4,r5,r7,pc}
  0009c176: blx 0x0006e824
```
