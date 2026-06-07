# TextureAlphaTestShader::ConnectShaderComponents
elf 0x8916c body 150
Sig: undefined __stdcall ConnectShaderComponents(uint param_1, int param_2)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::ConnectShaderComponents(unsigned int, int) */

void AbyssEngine::TextureAlphaTestShader::ConnectShaderComponents(uint param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int in_r2;
  int iVar3;
  code *pcVar4;
  
  pcVar4 = *(code **)(DAT_00099204 + 0x99180);
  uVar1 = (*pcVar4)(param_2,DAT_00099208 + 0x99184);
  iVar3 = param_1 + in_r2 * 4;
  iVar2 = DAT_0009920c + 0x99192;
  *(undefined4 *)(iVar3 + 0x44) = uVar1;
  uVar1 = glGetAttribLocation(param_2,iVar2);
  iVar2 = DAT_00099210;
  *(undefined4 *)(iVar3 + 0x24) = uVar1;
  uVar1 = glGetAttribLocation(param_2,iVar2 + 0x991a2);
  iVar2 = DAT_00099214;
  *(undefined4 *)(iVar3 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991ae);
  iVar2 = DAT_00099218;
  *(undefined4 *)(iVar3 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991b8);
  iVar2 = DAT_0009921c;
  *(undefined4 *)(iVar3 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991c2);
  iVar2 = DAT_00099220;
  *(undefined4 *)(iVar3 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991cc);
  iVar2 = DAT_00099224;
  *(undefined4 *)(iVar3 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991d6);
  iVar2 = DAT_00099228;
  *(undefined4 *)(iVar3 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991e0);
  iVar2 = DAT_0009922c;
  *(undefined4 *)(iVar3 + 100) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0x991ea);
  *(undefined4 *)(iVar3 + 0x6c) = uVar1;
  glUseProgram(param_2);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(iVar3 + 0x44),0);
  return;
}

```

## target disasm
```
  0009916c: push {r4,r5,r6,r7,lr}
  0009916e: add r7,sp,#0xc
  00099170: push.w r8
  00099174: mov r5,r0
  00099176: ldr r0,[0x00099204]
  00099178: mov r4,r1
  0009917a: ldr r1,[0x00099208]
  0009917c: add r0,pc
  0009917e: mov r8,r2
  00099180: add r1,pc
  00099182: ldr r6,[r0,#0x0]
  00099184: mov r0,r4
  00099186: blx r6
  00099188: ldr r1,[0x0009920c]
  0009918a: add.w r5,r5,r8, lsl #0x2
  0009918e: add r1,pc
  00099190: str r0,[r5,#0x44]
  00099192: mov r0,r4
  00099194: blx 0x00070ac8
  00099198: ldr r1,[0x00099210]
  0009919a: str r0,[r5,#0x24]
  0009919c: mov r0,r4
  0009919e: add r1,pc
  000991a0: blx 0x00070ac8
  000991a4: ldr r1,[0x00099214]
  000991a6: str r0,[r5,#0x2c]
  000991a8: mov r0,r4
  000991aa: add r1,pc
  000991ac: blx r6
  000991ae: ldr r1,[0x00099218]
  000991b0: str r0,[r5,#0x34]
  000991b2: mov r0,r4
  000991b4: add r1,pc
  000991b6: blx r6
  000991b8: ldr r1,[0x0009921c]
  000991ba: str r0,[r5,#0x3c]
  000991bc: mov r0,r4
  000991be: add r1,pc
  000991c0: blx r6
  000991c2: ldr r1,[0x00099220]
  000991c4: str r0,[r5,#0x4c]
  000991c6: mov r0,r4
  000991c8: add r1,pc
  000991ca: blx r6
  000991cc: ldr r1,[0x00099224]
  000991ce: str r0,[r5,#0x54]
  000991d0: mov r0,r4
  000991d2: add r1,pc
  000991d4: blx r6
  000991d6: ldr r1,[0x00099228]
  000991d8: str r0,[r5,#0x5c]
  000991da: mov r0,r4
  000991dc: add r1,pc
  000991de: blx r6
  000991e0: ldr r1,[0x0009922c]
  000991e2: str r0,[r5,#0x64]
  000991e4: mov r0,r4
  000991e6: add r1,pc
  000991e8: blx r6
  000991ea: str r0,[r5,#0x6c]
  000991ec: mov r0,r4
  000991ee: blx 0x00070960
  000991f2: ldr r0,[r5,#0x44]
  000991f4: movs r1,#0x0
  000991f6: pop.w r8
  000991fa: pop.w {r4,r5,r6,r7,lr}
  000991fe: b.w 0x001ab768
```
