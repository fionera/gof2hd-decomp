# TextureAlphaTestShader::TextureAlphaTestShader
elf 0x890c0 body 94
Sig: undefined __thiscall TextureAlphaTestShader(TextureAlphaTestShader * this)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::TextureAlphaTestShader() */

void __thiscall
AbyssEngine::TextureAlphaTestShader::TextureAlphaTestShader(TextureAlphaTestShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  TextureAlphaTestShader *apTStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_00099138 + 0x990cc);
  local_14 = *piVar4;
  apTStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009913c + 0x990de);
  puVar3 = *(undefined4 **)(DAT_00099144 + 0x990e6);
  *(int *)this = *(int *)(DAT_00099140 + 0x990e2) + 8;
  iVar2 = DAT_00099148;
  *puVar3 = *puVar1;
  String::String((String *)apTStack_20,(char *)(iVar2 + 0x990f6),false);
  String::operator=((String *)(this + 0xc),(String *)apTStack_20);
  String::~String((String *)apTStack_20);
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
  000990c0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000990c2: add r7,sp,#0x18
  000990c4: mov r4,r0
  000990c6: ldr r0,[0x00099138]
  000990c8: add r0,pc
  000990ca: ldr r5,[r0,#0x0]
  000990cc: ldr r0,[r5,#0x0]
  000990ce: str r0,[sp,#0xc]
  000990d0: mov r0,r4
  000990d2: blx 0x00070930
  000990d6: ldr r0,[0x0009913c]
  000990d8: ldr r1,[0x00099140]
  000990da: add r0,pc
  000990dc: ldr r2,[0x00099144]
  000990de: add r1,pc
  000990e0: ldr r0,[r0,#0x0]
  000990e2: add r2,pc
  000990e4: ldr r1,[r1,#0x0]
  000990e6: ldr r2,[r2,#0x0]
  000990e8: adds r1,#0x8
  000990ea: str r1,[r4,#0x0]
  000990ec: ldr r0,[r0,#0x0]
  000990ee: ldr r1,[0x00099148]
  000990f0: str r0,[r2,#0x0]
  000990f2: add r1,pc
  000990f4: mov r0,sp
  000990f6: movs r2,#0x0
  000990f8: blx 0x0006ee18
  000990fc: add.w r0,r4,#0xc
  00099100: mov r1,sp
  00099102: blx 0x0006f2b0
  00099106: mov r0,sp
  00099108: blx 0x0006ee30
  0009910c: ldr r0,[sp,#0xc]
  0009910e: ldr r1,[r5,#0x0]
  00099110: subs r0,r1,r0
  00099112: ittt eq
  00099114: mov.eq r0,r4
  00099116: add.eq sp,#0x10
  00099118: pop.eq {r4,r5,r7,pc}
  0009911a: blx 0x0006e824
```
