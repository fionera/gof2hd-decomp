# TextureConference::Init
elf 0x8cb98 body 118
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::TextureConference::Init(AbyssEngine::Engine*) */

void AbyssEngine::TextureConference::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009cc10 + 0x9cba6));
  iVar2 = DAT_0009cc1c;
  puVar3 = (undefined4 *)(DAT_0009cc18 + 0x9cbb2);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9cbb6);
  iVar2 = DAT_0009cc20 + 0x9cbc0;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009cc24 + 0x9cbce;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009cc28 + 0x9cbdc;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009cc2c + 0x9cbe8;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009cc30 + 0x9cbf4;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x34),0);
  return;
}

```

## target disasm
```
  0009cb98: push {r4,r5,r7,lr}
  0009cb9a: add r7,sp,#0x8
  0009cb9c: ldr r1,[0x0009cc10]
  0009cb9e: mov r4,r0
  0009cba0: ldr r2,[0x0009cc14]
  0009cba2: add r1,pc
  0009cba4: add r2,pc
  0009cba6: blx 0x00070954
  0009cbaa: ldr r2,[0x0009cc18]
  0009cbac: ldr r1,[0x0009cc1c]
  0009cbae: add r2,pc
  0009cbb0: str r0,[r4,#0x4]
  0009cbb2: add r1,pc
  0009cbb4: ldr r5,[r2,#0x0]
  0009cbb6: blx r5
  0009cbb8: ldr r1,[0x0009cc20]
  0009cbba: ldr r2,[r4,#0x4]
  0009cbbc: add r1,pc
  0009cbbe: str r0,[r4,#0x34]
  0009cbc0: mov r0,r2
  0009cbc2: blx 0x00070ac8
  0009cbc6: ldr r1,[0x0009cc24]
  0009cbc8: ldr r2,[r4,#0x4]
  0009cbca: add r1,pc
  0009cbcc: str r0,[r4,#0x20]
  0009cbce: mov r0,r2
  0009cbd0: blx 0x00070ac8
  0009cbd4: ldr r1,[0x0009cc28]
  0009cbd6: ldr r2,[r4,#0x4]
  0009cbd8: add r1,pc
  0009cbda: str r0,[r4,#0x24]
  0009cbdc: mov r0,r2
  0009cbde: blx r5
  0009cbe0: ldr r1,[0x0009cc2c]
  0009cbe2: ldr r2,[r4,#0x4]
  0009cbe4: add r1,pc
  0009cbe6: str r0,[r4,#0x2c]
  0009cbe8: mov r0,r2
  0009cbea: blx r5
  0009cbec: ldr r1,[0x0009cc30]
  0009cbee: ldr r2,[r4,#0x4]
  0009cbf0: add r1,pc
  0009cbf2: str r0,[r4,#0x28]
  0009cbf4: mov r0,r2
  0009cbf6: blx r5
  0009cbf8: ldr r1,[r4,#0x4]
  0009cbfa: str r0,[r4,#0x30]
  0009cbfc: mov r0,r1
  0009cbfe: blx 0x00070960
  0009cc02: ldr r0,[r4,#0x34]
  0009cc04: movs r1,#0x0
  0009cc06: pop.w {r4,r5,r7,lr}
  0009cc0a: b.w 0x001ab768
```
