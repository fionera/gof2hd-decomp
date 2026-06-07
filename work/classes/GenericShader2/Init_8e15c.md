# GenericShader2::Init
elf 0x8e15c body 216
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GenericShader2::Init(AbyssEngine::Engine*) */

void AbyssEngine::GenericShader2::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009e234 + 0x9e16a));
  iVar2 = DAT_0009e240;
  puVar3 = (undefined4 *)(DAT_0009e23c + 0x9e176);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9e17a);
  iVar2 = DAT_0009e244 + 0x9e184;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e248 + 0x9e190;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e24c + 0x9e19c;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e250 + 0x9e1a8;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009e254 + 0x9e1b4);
  iVar2 = DAT_0009e258 + 0x9e1b8;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e25c + 0x9e1c6;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e260 + 0x9e1d2;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e264 + 0x9e1de;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e268 + 0x9e1ea;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e26c + 0x9e1f6;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e270 + 0x9e202;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e274 + 0x9e20e;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e278 + 0x9e21a;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x44),0);
  return;
}

```

## target disasm
```
  0009e15c: push {r4,r5,r7,lr}
  0009e15e: add r7,sp,#0x8
  0009e160: ldr r1,[0x0009e234]
  0009e162: mov r4,r0
  0009e164: ldr r2,[0x0009e238]
  0009e166: add r1,pc
  0009e168: add r2,pc
  0009e16a: blx 0x00070954
  0009e16e: ldr r2,[0x0009e23c]
  0009e170: ldr r1,[0x0009e240]
  0009e172: add r2,pc
  0009e174: str r0,[r4,#0x4]
  0009e176: add r1,pc
  0009e178: ldr r5,[r2,#0x0]
  0009e17a: blx r5
  0009e17c: ldr r1,[0x0009e244]
  0009e17e: ldr r2,[r4,#0x4]
  0009e180: add r1,pc
  0009e182: str r0,[r4,#0x20]
  0009e184: mov r0,r2
  0009e186: blx r5
  0009e188: ldr r1,[0x0009e248]
  0009e18a: ldr r2,[r4,#0x4]
  0009e18c: add r1,pc
  0009e18e: str r0,[r4,#0x24]
  0009e190: mov r0,r2
  0009e192: blx r5
  0009e194: ldr r1,[0x0009e24c]
  0009e196: ldr r2,[r4,#0x4]
  0009e198: add r1,pc
  0009e19a: str r0,[r4,#0x28]
  0009e19c: mov r0,r2
  0009e19e: blx r5
  0009e1a0: ldr r1,[0x0009e250]
  0009e1a2: ldr r2,[r4,#0x4]
  0009e1a4: add r1,pc
  0009e1a6: str r0,[r4,#0x2c]
  0009e1a8: mov r0,r2
  0009e1aa: blx r5
  0009e1ac: ldr r3,[0x0009e254]
  0009e1ae: ldr r1,[0x0009e258]
  0009e1b0: add r3,pc
  0009e1b2: ldr r2,[r4,#0x4]
  0009e1b4: add r1,pc
  0009e1b6: str r0,[r4,#0x30]
  0009e1b8: ldr r5,[r3,#0x0]
  0009e1ba: mov r0,r2
  0009e1bc: blx r5
  0009e1be: ldr r1,[0x0009e25c]
  0009e1c0: ldr r2,[r4,#0x4]
  0009e1c2: add r1,pc
  0009e1c4: str r0,[r4,#0x34]
  0009e1c6: mov r0,r2
  0009e1c8: blx r5
  0009e1ca: ldr r1,[0x0009e260]
  0009e1cc: ldr r2,[r4,#0x4]
  0009e1ce: add r1,pc
  0009e1d0: str r0,[r4,#0x38]
  0009e1d2: mov r0,r2
  0009e1d4: blx r5
  0009e1d6: ldr r1,[0x0009e264]
  0009e1d8: ldr r2,[r4,#0x4]
  0009e1da: add r1,pc
  0009e1dc: str r0,[r4,#0x3c]
  0009e1de: mov r0,r2
  0009e1e0: blx r5
  0009e1e2: ldr r1,[0x0009e268]
  0009e1e4: ldr r2,[r4,#0x4]
  0009e1e6: add r1,pc
  0009e1e8: str r0,[r4,#0x40]
  0009e1ea: mov r0,r2
  0009e1ec: blx r5
  0009e1ee: ldr r1,[0x0009e26c]
  0009e1f0: ldr r2,[r4,#0x4]
  0009e1f2: add r1,pc
  0009e1f4: str r0,[r4,#0x44]
  0009e1f6: mov r0,r2
  0009e1f8: blx r5
  0009e1fa: ldr r1,[0x0009e270]
  0009e1fc: ldr r2,[r4,#0x4]
  0009e1fe: add r1,pc
  0009e200: str r0,[r4,#0x48]
  0009e202: mov r0,r2
  0009e204: blx r5
  0009e206: ldr r1,[0x0009e274]
  0009e208: ldr r2,[r4,#0x4]
  0009e20a: add r1,pc
  0009e20c: str r0,[r4,#0x4c]
  0009e20e: mov r0,r2
  0009e210: blx r5
  0009e212: ldr r1,[0x0009e278]
  0009e214: ldr r2,[r4,#0x4]
  0009e216: add r1,pc
  0009e218: str r0,[r4,#0x50]
  0009e21a: mov r0,r2
  0009e21c: blx r5
  0009e21e: ldr r1,[r4,#0x4]
  0009e220: str r0,[r4,#0x54]
  0009e222: mov r0,r1
  0009e224: blx 0x00070960
  0009e228: ldr r0,[r4,#0x44]
  0009e22a: movs r1,#0x0
  0009e22c: pop.w {r4,r5,r7,lr}
  0009e230: b.w 0x001ab768
```
