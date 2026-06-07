# BumpRimCubeShader_new::BumpRimCubeShader_new
elf 0x88820 body 94
Sig: undefined __thiscall BumpRimCubeShader_new(BumpRimCubeShader_new * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader_new::BumpRimCubeShader_new() */

void __thiscall
AbyssEngine::BumpRimCubeShader_new::BumpRimCubeShader_new(BumpRimCubeShader_new *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BumpRimCubeShader_new *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_00098898 + 0x9882c);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009889c + 0x9883e);
  puVar3 = *(undefined4 **)(DAT_000988a4 + 0x98846);
  *(int *)this = *(int *)(DAT_000988a0 + 0x98842) + 8;
  iVar2 = DAT_000988a8;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x98856),false);
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
  00098820: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00098822: add r7,sp,#0x18
  00098824: mov r4,r0
  00098826: ldr r0,[0x00098898]
  00098828: add r0,pc
  0009882a: ldr r5,[r0,#0x0]
  0009882c: ldr r0,[r5,#0x0]
  0009882e: str r0,[sp,#0xc]
  00098830: mov r0,r4
  00098832: blx 0x00070930
  00098836: ldr r0,[0x0009889c]
  00098838: ldr r1,[0x000988a0]
  0009883a: add r0,pc
  0009883c: ldr r2,[0x000988a4]
  0009883e: add r1,pc
  00098840: ldr r0,[r0,#0x0]
  00098842: add r2,pc
  00098844: ldr r1,[r1,#0x0]
  00098846: ldr r2,[r2,#0x0]
  00098848: adds r1,#0x8
  0009884a: str r1,[r4,#0x0]
  0009884c: ldr r0,[r0,#0x0]
  0009884e: ldr r1,[0x000988a8]
  00098850: str r0,[r2,#0x0]
  00098852: add r1,pc
  00098854: mov r0,sp
  00098856: movs r2,#0x0
  00098858: blx 0x0006ee18
  0009885c: add.w r0,r4,#0xc
  00098860: mov r1,sp
  00098862: blx 0x0006f2b0
  00098866: mov r0,sp
  00098868: blx 0x0006ee30
  0009886c: ldr r0,[sp,#0xc]
  0009886e: ldr r1,[r5,#0x0]
  00098870: subs r0,r1,r0
  00098872: ittt eq
  00098874: mov.eq r0,r4
  00098876: add.eq sp,#0x10
  00098878: pop.eq {r4,r5,r7,pc}
  0009887a: blx 0x0006e824
```
