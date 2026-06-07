# GreenShader::GreenShader
elf 0x88214 body 94
Sig: undefined __thiscall GreenShader(GreenShader * this)

## decompile
```c

/* AbyssEngine::GreenShader::GreenShader() */

void __thiscall AbyssEngine::GreenShader::GreenShader(GreenShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  GreenShader *apGStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009828c + 0x98220);
  local_14 = *piVar4;
  apGStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_00098290 + 0x98232);
  puVar3 = *(undefined4 **)(DAT_00098298 + 0x9823a);
  *(int *)this = *(int *)(DAT_00098294 + 0x98236) + 8;
  iVar2 = DAT_0009829c;
  *puVar3 = *puVar1;
  String::String((String *)apGStack_20,(char *)(iVar2 + 0x9824a),false);
  String::operator=((String *)(this + 0xc),(String *)apGStack_20);
  String::~String((String *)apGStack_20);
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
  00098214: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00098216: add r7,sp,#0x18
  00098218: mov r4,r0
  0009821a: ldr r0,[0x0009828c]
  0009821c: add r0,pc
  0009821e: ldr r5,[r0,#0x0]
  00098220: ldr r0,[r5,#0x0]
  00098222: str r0,[sp,#0xc]
  00098224: mov r0,r4
  00098226: blx 0x00070930
  0009822a: ldr r0,[0x00098290]
  0009822c: ldr r1,[0x00098294]
  0009822e: add r0,pc
  00098230: ldr r2,[0x00098298]
  00098232: add r1,pc
  00098234: ldr r0,[r0,#0x0]
  00098236: add r2,pc
  00098238: ldr r1,[r1,#0x0]
  0009823a: ldr r2,[r2,#0x0]
  0009823c: adds r1,#0x8
  0009823e: str r1,[r4,#0x0]
  00098240: ldr r0,[r0,#0x0]
  00098242: ldr r1,[0x0009829c]
  00098244: str r0,[r2,#0x0]
  00098246: add r1,pc
  00098248: mov r0,sp
  0009824a: movs r2,#0x0
  0009824c: blx 0x0006ee18
  00098250: add.w r0,r4,#0xc
  00098254: mov r1,sp
  00098256: blx 0x0006f2b0
  0009825a: mov r0,sp
  0009825c: blx 0x0006ee30
  00098260: ldr r0,[sp,#0xc]
  00098262: ldr r1,[r5,#0x0]
  00098264: subs r0,r1,r0
  00098266: ittt eq
  00098268: mov.eq r0,r4
  0009826a: add.eq sp,#0x10
  0009826c: pop.eq {r4,r5,r7,pc}
  0009826e: blx 0x0006e824
```
