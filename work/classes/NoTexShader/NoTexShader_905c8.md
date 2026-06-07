# NoTexShader::NoTexShader
elf 0x905c8 body 94
Sig: undefined __thiscall NoTexShader(NoTexShader * this)

## decompile
```c

/* AbyssEngine::NoTexShader::NoTexShader() */

void __thiscall AbyssEngine::NoTexShader::NoTexShader(NoTexShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  NoTexShader *apNStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a0640 + 0xa05d4);
  local_14 = *piVar4;
  apNStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a0644 + 0xa05e6);
  puVar3 = *(undefined4 **)(DAT_000a064c + 0xa05ee);
  *(int *)this = *(int *)(DAT_000a0648 + 0xa05ea) + 8;
  iVar2 = DAT_000a0650;
  *puVar3 = *puVar1;
  String::String((String *)apNStack_20,(char *)(iVar2 + 0xa05fe),false);
  String::operator=((String *)(this + 0xc),(String *)apNStack_20);
  String::~String((String *)apNStack_20);
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
  000a05c8: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a05ca: add r7,sp,#0x18
  000a05cc: mov r4,r0
  000a05ce: ldr r0,[0x000a0640]
  000a05d0: add r0,pc
  000a05d2: ldr r5,[r0,#0x0]
  000a05d4: ldr r0,[r5,#0x0]
  000a05d6: str r0,[sp,#0xc]
  000a05d8: mov r0,r4
  000a05da: blx 0x00070930
  000a05de: ldr r0,[0x000a0644]
  000a05e0: ldr r1,[0x000a0648]
  000a05e2: add r0,pc
  000a05e4: ldr r2,[0x000a064c]
  000a05e6: add r1,pc
  000a05e8: ldr r0,[r0,#0x0]
  000a05ea: add r2,pc
  000a05ec: ldr r1,[r1,#0x0]
  000a05ee: ldr r2,[r2,#0x0]
  000a05f0: adds r1,#0x8
  000a05f2: str r1,[r4,#0x0]
  000a05f4: ldr r0,[r0,#0x0]
  000a05f6: ldr r1,[0x000a0650]
  000a05f8: str r0,[r2,#0x0]
  000a05fa: add r1,pc
  000a05fc: mov r0,sp
  000a05fe: movs r2,#0x0
  000a0600: blx 0x0006ee18
  000a0604: add.w r0,r4,#0xc
  000a0608: mov r1,sp
  000a060a: blx 0x0006f2b0
  000a060e: mov r0,sp
  000a0610: blx 0x0006ee30
  000a0614: ldr r0,[sp,#0xc]
  000a0616: ldr r1,[r5,#0x0]
  000a0618: subs r0,r1,r0
  000a061a: ittt eq
  000a061c: mov.eq r0,r4
  000a061e: add.eq sp,#0x10
  000a0620: pop.eq {r4,r5,r7,pc}
  000a0622: blx 0x0006e824
```
