# PostBWShader::Init
elf 0x8efa4 body 92
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::PostBWShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::PostBWShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009f000 + 0x9efb2));
  iVar2 = DAT_0009f008;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x9efbe);
  iVar2 = DAT_0009f00c + 0x9efc8;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f010 + 0x9efd6;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f014 + 0x9efe4;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x2c),0);
  return;
}

```

## target disasm
```
  0009efa4: push {r4,r6,r7,lr}
  0009efa6: add r7,sp,#0x8
  0009efa8: ldr r1,[0x0009f000]
  0009efaa: mov r4,r0
  0009efac: ldr r2,[0x0009f004]
  0009efae: add r1,pc
  0009efb0: add r2,pc
  0009efb2: blx 0x00070954
  0009efb6: ldr r1,[0x0009f008]
  0009efb8: str r0,[r4,#0x4]
  0009efba: add r1,pc
  0009efbc: blx 0x00070ac8
  0009efc0: ldr r1,[0x0009f00c]
  0009efc2: ldr r2,[r4,#0x4]
  0009efc4: add r1,pc
  0009efc6: str r0,[r4,#0x20]
  0009efc8: mov r0,r2
  0009efca: blx 0x00070ac8
  0009efce: ldr r1,[0x0009f010]
  0009efd0: ldr r2,[r4,#0x4]
  0009efd2: add r1,pc
  0009efd4: str r0,[r4,#0x28]
  0009efd6: mov r0,r2
  0009efd8: blx 0x00070ad4
  0009efdc: ldr r1,[0x0009f014]
  0009efde: ldr r2,[r4,#0x4]
  0009efe0: add r1,pc
  0009efe2: str r0,[r4,#0x24]
  0009efe4: mov r0,r2
  0009efe6: blx 0x00070ad4
  0009efea: ldr r1,[r4,#0x4]
  0009efec: str r0,[r4,#0x2c]
  0009efee: mov r0,r1
  0009eff0: blx 0x00070960
  0009eff4: ldr r0,[r4,#0x2c]
  0009eff6: movs r1,#0x0
  0009eff8: pop.w {r4,r6,r7,lr}
  0009effc: b.w 0x001ab768
```
