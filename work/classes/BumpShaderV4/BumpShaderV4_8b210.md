# BumpShaderV4::BumpShaderV4
elf 0x8b210 body 94
Sig: undefined __thiscall BumpShaderV4(BumpShaderV4 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV4::BumpShaderV4() */

void __thiscall AbyssEngine::BumpShaderV4::BumpShaderV4(BumpShaderV4 *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderV4 *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009b288 + 0x9b21c);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009b28c + 0x9b22e);
  puVar3 = *(undefined4 **)(DAT_0009b294 + 0x9b236);
  *(int *)this = *(int *)(DAT_0009b290 + 0x9b232) + 8;
  iVar2 = DAT_0009b298;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9b246),false);
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
  0009b210: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009b212: add r7,sp,#0x18
  0009b214: mov r4,r0
  0009b216: ldr r0,[0x0009b288]
  0009b218: add r0,pc
  0009b21a: ldr r5,[r0,#0x0]
  0009b21c: ldr r0,[r5,#0x0]
  0009b21e: str r0,[sp,#0xc]
  0009b220: mov r0,r4
  0009b222: blx 0x00070930
  0009b226: ldr r0,[0x0009b28c]
  0009b228: ldr r1,[0x0009b290]
  0009b22a: add r0,pc
  0009b22c: ldr r2,[0x0009b294]
  0009b22e: add r1,pc
  0009b230: ldr r0,[r0,#0x0]
  0009b232: add r2,pc
  0009b234: ldr r1,[r1,#0x0]
  0009b236: ldr r2,[r2,#0x0]
  0009b238: adds r1,#0x8
  0009b23a: str r1,[r4,#0x0]
  0009b23c: ldr r0,[r0,#0x0]
  0009b23e: ldr r1,[0x0009b298]
  0009b240: str r0,[r2,#0x0]
  0009b242: add r1,pc
  0009b244: mov r0,sp
  0009b246: movs r2,#0x0
  0009b248: blx 0x0006ee18
  0009b24c: add.w r0,r4,#0xc
  0009b250: mov r1,sp
  0009b252: blx 0x0006f2b0
  0009b256: mov r0,sp
  0009b258: blx 0x0006ee30
  0009b25c: ldr r0,[sp,#0xc]
  0009b25e: ldr r1,[r5,#0x0]
  0009b260: subs r0,r1,r0
  0009b262: ittt eq
  0009b264: mov.eq r0,r4
  0009b266: add.eq sp,#0x10
  0009b268: pop.eq {r4,r5,r7,pc}
  0009b26a: blx 0x0006e824
```
