# CubeMapping::CubeMapping
elf 0x9074c body 94
Sig: undefined __thiscall CubeMapping(CubeMapping * this)

## decompile
```c

/* AbyssEngine::CubeMapping::CubeMapping() */

void __thiscall AbyssEngine::CubeMapping::CubeMapping(CubeMapping *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  CubeMapping *apCStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a07c4 + 0xa0758);
  local_14 = *piVar4;
  apCStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a07c8 + 0xa076a);
  puVar3 = *(undefined4 **)(DAT_000a07d0 + 0xa0772);
  *(int *)this = *(int *)(DAT_000a07cc + 0xa076e) + 8;
  iVar2 = DAT_000a07d4;
  *puVar3 = *puVar1;
  String::String((String *)apCStack_20,(char *)(iVar2 + 0xa0782),false);
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
  000a074c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a074e: add r7,sp,#0x18
  000a0750: mov r4,r0
  000a0752: ldr r0,[0x000a07c4]
  000a0754: add r0,pc
  000a0756: ldr r5,[r0,#0x0]
  000a0758: ldr r0,[r5,#0x0]
  000a075a: str r0,[sp,#0xc]
  000a075c: mov r0,r4
  000a075e: blx 0x00070930
  000a0762: ldr r0,[0x000a07c8]
  000a0764: ldr r1,[0x000a07cc]
  000a0766: add r0,pc
  000a0768: ldr r2,[0x000a07d0]
  000a076a: add r1,pc
  000a076c: ldr r0,[r0,#0x0]
  000a076e: add r2,pc
  000a0770: ldr r1,[r1,#0x0]
  000a0772: ldr r2,[r2,#0x0]
  000a0774: adds r1,#0x8
  000a0776: str r1,[r4,#0x0]
  000a0778: ldr r0,[r0,#0x0]
  000a077a: ldr r1,[0x000a07d4]
  000a077c: str r0,[r2,#0x0]
  000a077e: add r1,pc
  000a0780: mov r0,sp
  000a0782: movs r2,#0x0
  000a0784: blx 0x0006ee18
  000a0788: add.w r0,r4,#0xc
  000a078c: mov r1,sp
  000a078e: blx 0x0006f2b0
  000a0792: mov r0,sp
  000a0794: blx 0x0006ee30
  000a0798: ldr r0,[sp,#0xc]
  000a079a: ldr r1,[r5,#0x0]
  000a079c: subs r0,r1,r0
  000a079e: ittt eq
  000a07a0: mov.eq r0,r4
  000a07a2: add.eq sp,#0x10
  000a07a4: pop.eq {r4,r5,r7,pc}
  000a07a6: blx 0x0006e824
```
