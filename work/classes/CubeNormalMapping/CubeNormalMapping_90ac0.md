# CubeNormalMapping::CubeNormalMapping
elf 0x90ac0 body 94
Sig: undefined __thiscall CubeNormalMapping(CubeNormalMapping * this)

## decompile
```c

/* AbyssEngine::CubeNormalMapping::CubeNormalMapping() */

void __thiscall AbyssEngine::CubeNormalMapping::CubeNormalMapping(CubeNormalMapping *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  CubeNormalMapping *apCStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a0b38 + 0xa0acc);
  local_14 = *piVar4;
  apCStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a0b3c + 0xa0ade);
  puVar3 = *(undefined4 **)(DAT_000a0b44 + 0xa0ae6);
  *(int *)this = *(int *)(DAT_000a0b40 + 0xa0ae2) + 8;
  iVar2 = DAT_000a0b48;
  *puVar3 = *puVar1;
  String::String((String *)apCStack_20,(char *)(iVar2 + 0xa0af6),false);
  String::operator=((String *)(this + 0xc),(String *)apCStack_20);
  String::~String((String *)apCStack_20);
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
  000a0ac0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a0ac2: add r7,sp,#0x18
  000a0ac4: mov r4,r0
  000a0ac6: ldr r0,[0x000a0b38]
  000a0ac8: add r0,pc
  000a0aca: ldr r5,[r0,#0x0]
  000a0acc: ldr r0,[r5,#0x0]
  000a0ace: str r0,[sp,#0xc]
  000a0ad0: mov r0,r4
  000a0ad2: blx 0x00070930
  000a0ad6: ldr r0,[0x000a0b3c]
  000a0ad8: ldr r1,[0x000a0b40]
  000a0ada: add r0,pc
  000a0adc: ldr r2,[0x000a0b44]
  000a0ade: add r1,pc
  000a0ae0: ldr r0,[r0,#0x0]
  000a0ae2: add r2,pc
  000a0ae4: ldr r1,[r1,#0x0]
  000a0ae6: ldr r2,[r2,#0x0]
  000a0ae8: adds r1,#0x8
  000a0aea: str r1,[r4,#0x0]
  000a0aec: ldr r0,[r0,#0x0]
  000a0aee: ldr r1,[0x000a0b48]
  000a0af0: str r0,[r2,#0x0]
  000a0af2: add r1,pc
  000a0af4: mov r0,sp
  000a0af6: movs r2,#0x0
  000a0af8: blx 0x0006ee18
  000a0afc: add.w r0,r4,#0xc
  000a0b00: mov r1,sp
  000a0b02: blx 0x0006f2b0
  000a0b06: mov r0,sp
  000a0b08: blx 0x0006ee30
  000a0b0c: ldr r0,[sp,#0xc]
  000a0b0e: ldr r1,[r5,#0x0]
  000a0b10: subs r0,r1,r0
  000a0b12: ittt eq
  000a0b14: mov.eq r0,r4
  000a0b16: add.eq sp,#0x10
  000a0b18: pop.eq {r4,r5,r7,pc}
  000a0b1a: blx 0x0006e824
```
