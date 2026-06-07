# TextureConference::TextureConference
elf 0x8cb08 body 100
Sig: undefined __thiscall TextureConference(TextureConference * this)

## decompile
```c

/* AbyssEngine::TextureConference::TextureConference() */

void __thiscall AbyssEngine::TextureConference::TextureConference(TextureConference *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TextureConference *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009cb84 + 0x9cb14);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009cb88 + 0x9cb26);
  puVar3 = *(undefined4 **)(DAT_0009cb90 + 0x9cb2e);
  *(int *)this = *(int *)(DAT_0009cb8c + 0x9cb2a) + 8;
  iVar2 = DAT_0009cb94;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0x9cb3e),false);
  String::operator=((String *)(this + 0xc),(String *)apTStack_20);
  String::~String((String *)apTStack_20);
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
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
  0009cb08: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009cb0a: add r7,sp,#0x18
  0009cb0c: mov r4,r0
  0009cb0e: ldr r0,[0x0009cb84]
  0009cb10: add r0,pc
  0009cb12: ldr r5,[r0,#0x0]
  0009cb14: ldr r0,[r5,#0x0]
  0009cb16: str r0,[sp,#0xc]
  0009cb18: mov r0,r4
  0009cb1a: blx 0x00070930
  0009cb1e: ldr r0,[0x0009cb88]
  0009cb20: ldr r1,[0x0009cb8c]
  0009cb22: add r0,pc
  0009cb24: ldr r2,[0x0009cb90]
  0009cb26: add r1,pc
  0009cb28: ldr r0,[r0,#0x0]
  0009cb2a: add r2,pc
  0009cb2c: ldr r1,[r1,#0x0]
  0009cb2e: ldr r2,[r2,#0x0]
  0009cb30: adds r1,#0x8
  0009cb32: str r1,[r4,#0x0]
  0009cb34: ldr r0,[r0,#0x0]
  0009cb36: ldr r1,[0x0009cb94]
  0009cb38: str r0,[r2,#0x0]
  0009cb3a: add r1,pc
  0009cb3c: mov r0,sp
  0009cb3e: movs r2,#0x0
  0009cb40: blx 0x0006ee18
  0009cb44: add.w r0,r4,#0xc
  0009cb48: mov r1,sp
  0009cb4a: blx 0x0006f2b0
  0009cb4e: mov r0,sp
  0009cb50: blx 0x0006ee30
  0009cb54: movs r0,#0x0
  0009cb56: strd r0,r0,[r4,#0x38]
  0009cb5a: ldr r0,[sp,#0xc]
  0009cb5c: ldr r1,[r5,#0x0]
  0009cb5e: subs r0,r1,r0
  0009cb60: ittt eq
  0009cb62: mov.eq r0,r4
  0009cb64: add.eq sp,#0x10
  0009cb66: pop.eq {r4,r5,r7,pc}
  0009cb68: blx 0x0006e824
```
