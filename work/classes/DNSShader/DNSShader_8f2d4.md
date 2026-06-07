# DNSShader::DNSShader
elf 0x8f2d4 body 94
Sig: undefined __thiscall DNSShader(DNSShader * this)

## decompile
```c

/* AbyssEngine::DNSShader::DNSShader() */

void __thiscall AbyssEngine::DNSShader::DNSShader(DNSShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  DNSShader *apDStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009f34c + 0x9f2e0);
  local_14 = *piVar4;
  apDStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009f350 + 0x9f2f2);
  puVar3 = *(undefined4 **)(DAT_0009f358 + 0x9f2fa);
  *(int *)this = *(int *)(DAT_0009f354 + 0x9f2f6) + 8;
  iVar2 = DAT_0009f35c;
  *puVar3 = *puVar1;
  String::String((String *)apDStack_20,(char *)(iVar2 + 0x9f30a),false);
  String::operator=((String *)(this + 0xc),(String *)apDStack_20);
  String::~String((String *)apDStack_20);
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
  0009f2d4: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009f2d6: add r7,sp,#0x18
  0009f2d8: mov r4,r0
  0009f2da: ldr r0,[0x0009f34c]
  0009f2dc: add r0,pc
  0009f2de: ldr r5,[r0,#0x0]
  0009f2e0: ldr r0,[r5,#0x0]
  0009f2e2: str r0,[sp,#0xc]
  0009f2e4: mov r0,r4
  0009f2e6: blx 0x00070930
  0009f2ea: ldr r0,[0x0009f350]
  0009f2ec: ldr r1,[0x0009f354]
  0009f2ee: add r0,pc
  0009f2f0: ldr r2,[0x0009f358]
  0009f2f2: add r1,pc
  0009f2f4: ldr r0,[r0,#0x0]
  0009f2f6: add r2,pc
  0009f2f8: ldr r1,[r1,#0x0]
  0009f2fa: ldr r2,[r2,#0x0]
  0009f2fc: adds r1,#0x8
  0009f2fe: str r1,[r4,#0x0]
  0009f300: ldr r0,[r0,#0x0]
  0009f302: ldr r1,[0x0009f35c]
  0009f304: str r0,[r2,#0x0]
  0009f306: add r1,pc
  0009f308: mov r0,sp
  0009f30a: movs r2,#0x0
  0009f30c: blx 0x0006ee18
  0009f310: add.w r0,r4,#0xc
  0009f314: mov r1,sp
  0009f316: blx 0x0006f2b0
  0009f31a: mov r0,sp
  0009f31c: blx 0x0006ee30
  0009f320: ldr r0,[sp,#0xc]
  0009f322: ldr r1,[r5,#0x0]
  0009f324: subs r0,r1,r0
  0009f326: ittt eq
  0009f328: mov.eq r0,r4
  0009f32a: add.eq sp,#0x10
  0009f32c: pop.eq {r4,r5,r7,pc}
  0009f32e: blx 0x0006e824
```
