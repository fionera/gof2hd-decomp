# BloomShader::BloomShader
elf 0x893dc body 94
Sig: undefined __thiscall BloomShader(BloomShader * this)

## decompile
```c

/* AbyssEngine::BloomShader::BloomShader() */

void __thiscall AbyssEngine::BloomShader::BloomShader(BloomShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BloomShader *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_00099454 + 0x993e8);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_00099458 + 0x993fa);
  puVar3 = *(undefined4 **)(DAT_00099460 + 0x99402);
  *(int *)this = *(int *)(DAT_0009945c + 0x993fe) + 8;
  iVar2 = DAT_00099464;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x99412),false);
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
  000993dc: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000993de: add r7,sp,#0x18
  000993e0: mov r4,r0
  000993e2: ldr r0,[0x00099454]
  000993e4: add r0,pc
  000993e6: ldr r5,[r0,#0x0]
  000993e8: ldr r0,[r5,#0x0]
  000993ea: str r0,[sp,#0xc]
  000993ec: mov r0,r4
  000993ee: blx 0x00070930
  000993f2: ldr r0,[0x00099458]
  000993f4: ldr r1,[0x0009945c]
  000993f6: add r0,pc
  000993f8: ldr r2,[0x00099460]
  000993fa: add r1,pc
  000993fc: ldr r0,[r0,#0x0]
  000993fe: add r2,pc
  00099400: ldr r1,[r1,#0x0]
  00099402: ldr r2,[r2,#0x0]
  00099404: adds r1,#0x8
  00099406: str r1,[r4,#0x0]
  00099408: ldr r0,[r0,#0x0]
  0009940a: ldr r1,[0x00099464]
  0009940c: str r0,[r2,#0x0]
  0009940e: add r1,pc
  00099410: mov r0,sp
  00099412: movs r2,#0x0
  00099414: blx 0x0006ee18
  00099418: add.w r0,r4,#0xc
  0009941c: mov r1,sp
  0009941e: blx 0x0006f2b0
  00099422: mov r0,sp
  00099424: blx 0x0006ee30
  00099428: ldr r0,[sp,#0xc]
  0009942a: ldr r1,[r5,#0x0]
  0009942c: subs r0,r1,r0
  0009942e: ittt eq
  00099430: mov.eq r0,r4
  00099432: add.eq sp,#0x10
  00099434: pop.eq {r4,r5,r7,pc}
  00099436: blx 0x0006e824
```
