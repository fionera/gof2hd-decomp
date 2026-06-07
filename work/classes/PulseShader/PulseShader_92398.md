# PulseShader::PulseShader
elf 0x92398 body 94
Sig: undefined __thiscall PulseShader(PulseShader * this)

## decompile
```c

/* AbyssEngine::PulseShader::PulseShader() */

void __thiscall AbyssEngine::PulseShader::PulseShader(PulseShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  PulseShader *apPStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a2410 + 0xa23a4);
  local_14 = *piVar4;
  apPStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a2414 + 0xa23b6);
  puVar3 = *(undefined4 **)(DAT_000a241c + 0xa23be);
  *(int *)this = *(int *)(DAT_000a2418 + 0xa23ba) + 8;
  iVar2 = DAT_000a2420;
  *puVar3 = *puVar1;
  String::String((String *)apPStack_20,(char *)(iVar2 + 0xa23ce),false);
  String::operator=((String *)(this + 0xc),(String *)apPStack_20);
  String::~String((String *)apPStack_20);
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
  000a2398: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a239a: add r7,sp,#0x18
  000a239c: mov r4,r0
  000a239e: ldr r0,[0x000a2410]
  000a23a0: add r0,pc
  000a23a2: ldr r5,[r0,#0x0]
  000a23a4: ldr r0,[r5,#0x0]
  000a23a6: str r0,[sp,#0xc]
  000a23a8: mov r0,r4
  000a23aa: blx 0x00070930
  000a23ae: ldr r0,[0x000a2414]
  000a23b0: ldr r1,[0x000a2418]
  000a23b2: add r0,pc
  000a23b4: ldr r2,[0x000a241c]
  000a23b6: add r1,pc
  000a23b8: ldr r0,[r0,#0x0]
  000a23ba: add r2,pc
  000a23bc: ldr r1,[r1,#0x0]
  000a23be: ldr r2,[r2,#0x0]
  000a23c0: adds r1,#0x8
  000a23c2: str r1,[r4,#0x0]
  000a23c4: ldr r0,[r0,#0x0]
  000a23c6: ldr r1,[0x000a2420]
  000a23c8: str r0,[r2,#0x0]
  000a23ca: add r1,pc
  000a23cc: mov r0,sp
  000a23ce: movs r2,#0x0
  000a23d0: blx 0x0006ee18
  000a23d4: add.w r0,r4,#0xc
  000a23d8: mov r1,sp
  000a23da: blx 0x0006f2b0
  000a23de: mov r0,sp
  000a23e0: blx 0x0006ee30
  000a23e4: ldr r0,[sp,#0xc]
  000a23e6: ldr r1,[r5,#0x0]
  000a23e8: subs r0,r1,r0
  000a23ea: ittt eq
  000a23ec: mov.eq r0,r4
  000a23ee: add.eq sp,#0x10
  000a23f0: pop.eq {r4,r5,r7,pc}
  000a23f2: blx 0x0006e824
```
