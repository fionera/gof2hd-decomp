# BumpMapping::BumpMapping
elf 0x91268 body 94
Sig: undefined __thiscall BumpMapping(BumpMapping * this)

## decompile
```c

/* AbyssEngine::BumpMapping::BumpMapping() */

void __thiscall AbyssEngine::BumpMapping::BumpMapping(BumpMapping *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpMapping *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a12e0 + 0xa1274);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a12e4 + 0xa1286);
  puVar3 = *(undefined4 **)(DAT_000a12ec + 0xa128e);
  *(int *)this = *(int *)(DAT_000a12e8 + 0xa128a) + 8;
  iVar2 = DAT_000a12f0;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0xa129e),false);
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
  000a1268: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a126a: add r7,sp,#0x18
  000a126c: mov r4,r0
  000a126e: ldr r0,[0x000a12e0]
  000a1270: add r0,pc
  000a1272: ldr r5,[r0,#0x0]
  000a1274: ldr r0,[r5,#0x0]
  000a1276: str r0,[sp,#0xc]
  000a1278: mov r0,r4
  000a127a: blx 0x00070930
  000a127e: ldr r0,[0x000a12e4]
  000a1280: ldr r1,[0x000a12e8]
  000a1282: add r0,pc
  000a1284: ldr r2,[0x000a12ec]
  000a1286: add r1,pc
  000a1288: ldr r0,[r0,#0x0]
  000a128a: add r2,pc
  000a128c: ldr r1,[r1,#0x0]
  000a128e: ldr r2,[r2,#0x0]
  000a1290: adds r1,#0x8
  000a1292: str r1,[r4,#0x0]
  000a1294: ldr r0,[r0,#0x0]
  000a1296: ldr r1,[0x000a12f0]
  000a1298: str r0,[r2,#0x0]
  000a129a: add r1,pc
  000a129c: mov r0,sp
  000a129e: movs r2,#0x0
  000a12a0: blx 0x0006ee18
  000a12a4: add.w r0,r4,#0xc
  000a12a8: mov r1,sp
  000a12aa: blx 0x0006f2b0
  000a12ae: mov r0,sp
  000a12b0: blx 0x0006ee30
  000a12b4: ldr r0,[sp,#0xc]
  000a12b6: ldr r1,[r5,#0x0]
  000a12b8: subs r0,r1,r0
  000a12ba: ittt eq
  000a12bc: mov.eq r0,r4
  000a12be: add.eq sp,#0x10
  000a12c0: pop.eq {r4,r5,r7,pc}
  000a12c2: blx 0x0006e824
```
