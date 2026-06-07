# BlurShader::BlurShader
elf 0x8a5e0 body 104
Sig: undefined __thiscall BlurShader(BlurShader * this)

## decompile
```c

/* AbyssEngine::BlurShader::BlurShader() */

void __thiscall AbyssEngine::BlurShader::BlurShader(BlurShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  BlurShader *apBStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009a664 + 0x9a5ec);
  local_14 = *piVar4;
  apBStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009a668 + 0x9a5fe);
  puVar3 = *(undefined4 **)(DAT_0009a670 + 0x9a606);
  *(int *)this = *(int *)(DAT_0009a66c + 0x9a602) + 8;
  iVar2 = DAT_0009a674;
  *puVar3 = *puVar1;
  String::String((String *)apBStack_20,(char *)(iVar2 + 0x9a616),false);
  String::operator=((String *)(this + 0xc),(String *)apBStack_20);
  String::~String((String *)apBStack_20);
  *(undefined4 *)(this + 0x58) = DAT_0009a660;
  *(undefined4 *)(this + 0x5c) = 0x40000000;
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
  0009a5e0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009a5e2: add r7,sp,#0x18
  0009a5e4: mov r4,r0
  0009a5e6: ldr r0,[0x0009a664]
  0009a5e8: add r0,pc
  0009a5ea: ldr r5,[r0,#0x0]
  0009a5ec: ldr r0,[r5,#0x0]
  0009a5ee: str r0,[sp,#0xc]
  0009a5f0: mov r0,r4
  0009a5f2: blx 0x00070930
  0009a5f6: ldr r0,[0x0009a668]
  0009a5f8: ldr r1,[0x0009a66c]
  0009a5fa: add r0,pc
  0009a5fc: ldr r2,[0x0009a670]
  0009a5fe: add r1,pc
  0009a600: ldr r0,[r0,#0x0]
  0009a602: add r2,pc
  0009a604: ldr r1,[r1,#0x0]
  0009a606: ldr r2,[r2,#0x0]
  0009a608: adds r1,#0x8
  0009a60a: str r1,[r4,#0x0]
  0009a60c: ldr r0,[r0,#0x0]
  0009a60e: ldr r1,[0x0009a674]
  0009a610: str r0,[r2,#0x0]
  0009a612: add r1,pc
  0009a614: mov r0,sp
  0009a616: movs r2,#0x0
  0009a618: blx 0x0006ee18
  0009a61c: add.w r0,r4,#0xc
  0009a620: mov r1,sp
  0009a622: blx 0x0006f2b0
  0009a626: mov r0,sp
  0009a628: blx 0x0006ee30
  0009a62c: ldr r0,[0x0009a660]
  0009a62e: mov.w r1,#0x40000000
  0009a632: strd r0,r1,[r4,#0x58]
  0009a636: ldr r0,[sp,#0xc]
  0009a638: ldr r1,[r5,#0x0]
  0009a63a: subs r0,r1,r0
  0009a63c: ittt eq
  0009a63e: mov.eq r0,r4
  0009a640: add.eq sp,#0x10
  0009a642: pop.eq {r4,r5,r7,pc}
  0009a644: blx 0x0006e824
```
