# BumpShaderCloak::BumpShaderCloak
elf 0x8abf0 body 94
Sig: undefined __thiscall BumpShaderCloak(BumpShaderCloak * this)

## decompile
```c

/* AbyssEngine::BumpShaderCloak::BumpShaderCloak() */

void __thiscall AbyssEngine::BumpShaderCloak::BumpShaderCloak(BumpShaderCloak *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpShaderCloak *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009ac68 + 0x9abfc);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009ac6c + 0x9ac0e);
  puVar3 = *(undefined4 **)(DAT_0009ac74 + 0x9ac16);
  *(int *)this = *(int *)(DAT_0009ac70 + 0x9ac12) + 8;
  iVar2 = DAT_0009ac78;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9ac26),false);
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
  0009abf0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009abf2: add r7,sp,#0x18
  0009abf4: mov r4,r0
  0009abf6: ldr r0,[0x0009ac68]
  0009abf8: add r0,pc
  0009abfa: ldr r5,[r0,#0x0]
  0009abfc: ldr r0,[r5,#0x0]
  0009abfe: str r0,[sp,#0xc]
  0009ac00: mov r0,r4
  0009ac02: blx 0x00070930
  0009ac06: ldr r0,[0x0009ac6c]
  0009ac08: ldr r1,[0x0009ac70]
  0009ac0a: add r0,pc
  0009ac0c: ldr r2,[0x0009ac74]
  0009ac0e: add r1,pc
  0009ac10: ldr r0,[r0,#0x0]
  0009ac12: add r2,pc
  0009ac14: ldr r1,[r1,#0x0]
  0009ac16: ldr r2,[r2,#0x0]
  0009ac18: adds r1,#0x8
  0009ac1a: str r1,[r4,#0x0]
  0009ac1c: ldr r0,[r0,#0x0]
  0009ac1e: ldr r1,[0x0009ac78]
  0009ac20: str r0,[r2,#0x0]
  0009ac22: add r1,pc
  0009ac24: mov r0,sp
  0009ac26: movs r2,#0x0
  0009ac28: blx 0x0006ee18
  0009ac2c: add.w r0,r4,#0xc
  0009ac30: mov r1,sp
  0009ac32: blx 0x0006f2b0
  0009ac36: mov r0,sp
  0009ac38: blx 0x0006ee30
  0009ac3c: ldr r0,[sp,#0xc]
  0009ac3e: ldr r1,[r5,#0x0]
  0009ac40: subs r0,r1,r0
  0009ac42: ittt eq
  0009ac44: mov.eq r0,r4
  0009ac46: add.eq sp,#0x10
  0009ac48: pop.eq {r4,r5,r7,pc}
  0009ac4a: blx 0x0006e824
```
