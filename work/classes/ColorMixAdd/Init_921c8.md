# ColorMixAdd::Init
elf 0x921c8 body 142
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::ColorMixAdd::Init(AbyssEngine::Engine*) */

void AbyssEngine::ColorMixAdd::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a2258 + 0xa21d6));
  iVar2 = DAT_000a2264;
  puVar3 = (undefined4 *)(DAT_000a2260 + 0xa21e2);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa21e6);
  iVar2 = DAT_000a2268 + 0xa21f0;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a226c + 0xa21fe;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a2270 + 0xa220c;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a2274 + 0xa2218;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a2278 + 0xa2224;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a227c + 0xa2230;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a2280 + 0xa223c;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x30),0);
  return;
}

```

## target disasm
```
  000a21c8: push {r4,r5,r7,lr}
  000a21ca: add r7,sp,#0x8
  000a21cc: ldr r1,[0x000a2258]
  000a21ce: mov r4,r0
  000a21d0: ldr r2,[0x000a225c]
  000a21d2: add r1,pc
  000a21d4: add r2,pc
  000a21d6: blx 0x00070954
  000a21da: ldr r2,[0x000a2260]
  000a21dc: ldr r1,[0x000a2264]
  000a21de: add r2,pc
  000a21e0: str r0,[r4,#0x4]
  000a21e2: add r1,pc
  000a21e4: ldr r5,[r2,#0x0]
  000a21e6: blx r5
  000a21e8: ldr r1,[0x000a2268]
  000a21ea: ldr r2,[r4,#0x4]
  000a21ec: add r1,pc
  000a21ee: str r0,[r4,#0x30]
  000a21f0: mov r0,r2
  000a21f2: blx 0x00070ac8
  000a21f6: ldr r1,[0x000a226c]
  000a21f8: ldr r2,[r4,#0x4]
  000a21fa: add r1,pc
  000a21fc: str r0,[r4,#0x20]
  000a21fe: mov r0,r2
  000a2200: blx 0x00070ac8
  000a2204: ldr r1,[0x000a2270]
  000a2206: ldr r2,[r4,#0x4]
  000a2208: add r1,pc
  000a220a: str r0,[r4,#0x24]
  000a220c: mov r0,r2
  000a220e: blx r5
  000a2210: ldr r1,[0x000a2274]
  000a2212: ldr r2,[r4,#0x4]
  000a2214: add r1,pc
  000a2216: str r0,[r4,#0x28]
  000a2218: mov r0,r2
  000a221a: blx r5
  000a221c: ldr r1,[0x000a2278]
  000a221e: ldr r2,[r4,#0x4]
  000a2220: add r1,pc
  000a2222: str r0,[r4,#0x2c]
  000a2224: mov r0,r2
  000a2226: blx r5
  000a2228: ldr r1,[0x000a227c]
  000a222a: ldr r2,[r4,#0x4]
  000a222c: add r1,pc
  000a222e: str r0,[r4,#0x38]
  000a2230: mov r0,r2
  000a2232: blx r5
  000a2234: ldr r1,[0x000a2280]
  000a2236: ldr r2,[r4,#0x4]
  000a2238: add r1,pc
  000a223a: str r0,[r4,#0x34]
  000a223c: mov r0,r2
  000a223e: blx r5
  000a2240: ldr r1,[r4,#0x4]
  000a2242: str r0,[r4,#0x3c]
  000a2244: mov r0,r1
  000a2246: blx 0x00070960
  000a224a: ldr r0,[r4,#0x30]
  000a224c: movs r1,#0x0
  000a224e: pop.w {r4,r5,r7,lr}
  000a2252: b.w 0x001ab768
```
