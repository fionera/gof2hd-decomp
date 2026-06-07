# SpecCubeAlphaMapping::SpecCubeAlphaMapping
elf 0x87984 body 94
Sig: undefined __thiscall SpecCubeAlphaMapping(SpecCubeAlphaMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping() */

void __thiscall AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping(SpecCubeAlphaMapping *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  SpecCubeAlphaMapping *apSStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000979fc + 0x97990);
  local_14 = *piVar4;
  apSStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_00097a00 + 0x979a2);
  puVar3 = *(undefined4 **)(DAT_00097a08 + 0x979aa);
  *(int *)this = *(int *)(DAT_00097a04 + 0x979a6) + 8;
  iVar2 = DAT_00097a0c;
  *puVar3 = *puVar1;
  String::String((String *)apSStack_20,(char *)(iVar2 + 0x979ba),false);
  String::operator=((String *)(this + 0xc),(String *)apSStack_20);
  String::~String((String *)apSStack_20);
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
  00097984: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00097986: add r7,sp,#0x18
  00097988: mov r4,r0
  0009798a: ldr r0,[0x000979fc]
  0009798c: add r0,pc
  0009798e: ldr r5,[r0,#0x0]
  00097990: ldr r0,[r5,#0x0]
  00097992: str r0,[sp,#0xc]
  00097994: mov r0,r4
  00097996: blx 0x00070930
  0009799a: ldr r0,[0x00097a00]
  0009799c: ldr r1,[0x00097a04]
  0009799e: add r0,pc
  000979a0: ldr r2,[0x00097a08]
  000979a2: add r1,pc
  000979a4: ldr r0,[r0,#0x0]
  000979a6: add r2,pc
  000979a8: ldr r1,[r1,#0x0]
  000979aa: ldr r2,[r2,#0x0]
  000979ac: adds r1,#0x8
  000979ae: str r1,[r4,#0x0]
  000979b0: ldr r0,[r0,#0x0]
  000979b2: ldr r1,[0x00097a0c]
  000979b4: str r0,[r2,#0x0]
  000979b6: add r1,pc
  000979b8: mov r0,sp
  000979ba: movs r2,#0x0
  000979bc: blx 0x0006ee18
  000979c0: add.w r0,r4,#0xc
  000979c4: mov r1,sp
  000979c6: blx 0x0006f2b0
  000979ca: mov r0,sp
  000979cc: blx 0x0006ee30
  000979d0: ldr r0,[sp,#0xc]
  000979d2: ldr r1,[r5,#0x0]
  000979d4: subs r0,r1,r0
  000979d6: ittt eq
  000979d8: mov.eq r0,r4
  000979da: add.eq sp,#0x10
  000979dc: pop.eq {r4,r5,r7,pc}
  000979de: blx 0x0006e824
```
