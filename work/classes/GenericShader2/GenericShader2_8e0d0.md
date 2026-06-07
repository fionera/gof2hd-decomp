# GenericShader2::GenericShader2
elf 0x8e0d0 body 94
Sig: undefined __thiscall GenericShader2(GenericShader2 * this)

## decompile
```c

/* AbyssEngine::GenericShader2::GenericShader2() */

void __thiscall AbyssEngine::GenericShader2::GenericShader2(GenericShader2 *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GenericShader2 *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009e148 + 0x9e0dc);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009e14c + 0x9e0ee);
  puVar3 = *(undefined4 **)(DAT_0009e154 + 0x9e0f6);
  *(int *)this = *(int *)(DAT_0009e150 + 0x9e0f2) + 8;
  iVar2 = DAT_0009e158;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0x9e106),false);
  String::operator=((String *)(this + 0xc),(String *)apGStack_20);
  String::~String((String *)apGStack_20);
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
  0009e0d0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009e0d2: add r7,sp,#0x18
  0009e0d4: mov r4,r0
  0009e0d6: ldr r0,[0x0009e148]
  0009e0d8: add r0,pc
  0009e0da: ldr r5,[r0,#0x0]
  0009e0dc: ldr r0,[r5,#0x0]
  0009e0de: str r0,[sp,#0xc]
  0009e0e0: mov r0,r4
  0009e0e2: blx 0x00070930
  0009e0e6: ldr r0,[0x0009e14c]
  0009e0e8: ldr r1,[0x0009e150]
  0009e0ea: add r0,pc
  0009e0ec: ldr r2,[0x0009e154]
  0009e0ee: add r1,pc
  0009e0f0: ldr r0,[r0,#0x0]
  0009e0f2: add r2,pc
  0009e0f4: ldr r1,[r1,#0x0]
  0009e0f6: ldr r2,[r2,#0x0]
  0009e0f8: adds r1,#0x8
  0009e0fa: str r1,[r4,#0x0]
  0009e0fc: ldr r0,[r0,#0x0]
  0009e0fe: ldr r1,[0x0009e158]
  0009e100: str r0,[r2,#0x0]
  0009e102: add r1,pc
  0009e104: mov r0,sp
  0009e106: movs r2,#0x0
  0009e108: blx 0x0006ee18
  0009e10c: add.w r0,r4,#0xc
  0009e110: mov r1,sp
  0009e112: blx 0x0006f2b0
  0009e116: mov r0,sp
  0009e118: blx 0x0006ee30
  0009e11c: ldr r0,[sp,#0xc]
  0009e11e: ldr r1,[r5,#0x0]
  0009e120: subs r0,r1,r0
  0009e122: ittt eq
  0009e124: mov.eq r0,r4
  0009e126: add.eq sp,#0x10
  0009e128: pop.eq {r4,r5,r7,pc}
  0009e12a: blx 0x0006e824
```
