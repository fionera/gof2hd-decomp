# VertexColorShader::Init
elf 0x901fc body 228
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::VertexColorShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::VertexColorShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a02e0 + 0xa020a));
  iVar2 = DAT_000a02ec;
  puVar3 = (undefined4 *)(DAT_000a02e8 + 0xa0216);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa021a);
  iVar2 = DAT_000a02f0 + 0xa0224;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a02f4 + 0xa0230;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a02f8 + 0xa023c;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a02fc + 0xa0248;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0300 + 0xa0254;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a0304 + 0xa0260);
  iVar2 = DAT_000a0308 + 0xa0264;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a030c + 0xa0272;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0310 + 0xa027e;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0314 + 0xa028a;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0318 + 0xa0296;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a031c + 0xa02a2;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0320 + 0xa02ae;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0324 + 0xa02ba;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0328 + 0xa02c6;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x48),0);
  return;
}

```

## target disasm
```
  000a01fc: push {r4,r5,r7,lr}
  000a01fe: add r7,sp,#0x8
  000a0200: ldr r1,[0x000a02e0]
  000a0202: mov r4,r0
  000a0204: ldr r2,[0x000a02e4]
  000a0206: add r1,pc
  000a0208: add r2,pc
  000a020a: blx 0x00070954
  000a020e: ldr r2,[0x000a02e8]
  000a0210: ldr r1,[0x000a02ec]
  000a0212: add r2,pc
  000a0214: str r0,[r4,#0x4]
  000a0216: add r1,pc
  000a0218: ldr r5,[r2,#0x0]
  000a021a: blx r5
  000a021c: ldr r1,[0x000a02f0]
  000a021e: ldr r2,[r4,#0x4]
  000a0220: add r1,pc
  000a0222: str r0,[r4,#0x34]
  000a0224: mov r0,r2
  000a0226: blx r5
  000a0228: ldr r1,[0x000a02f4]
  000a022a: ldr r2,[r4,#0x4]
  000a022c: add r1,pc
  000a022e: str r0,[r4,#0x20]
  000a0230: mov r0,r2
  000a0232: blx r5
  000a0234: ldr r1,[0x000a02f8]
  000a0236: ldr r2,[r4,#0x4]
  000a0238: add r1,pc
  000a023a: str r0,[r4,#0x24]
  000a023c: mov r0,r2
  000a023e: blx r5
  000a0240: ldr r1,[0x000a02fc]
  000a0242: ldr r2,[r4,#0x4]
  000a0244: add r1,pc
  000a0246: str r0,[r4,#0x28]
  000a0248: mov r0,r2
  000a024a: blx r5
  000a024c: ldr r1,[0x000a0300]
  000a024e: ldr r2,[r4,#0x4]
  000a0250: add r1,pc
  000a0252: str r0,[r4,#0x2c]
  000a0254: mov r0,r2
  000a0256: blx r5
  000a0258: ldr r3,[0x000a0304]
  000a025a: ldr r1,[0x000a0308]
  000a025c: add r3,pc
  000a025e: ldr r2,[r4,#0x4]
  000a0260: add r1,pc
  000a0262: str r0,[r4,#0x30]
  000a0264: ldr r5,[r3,#0x0]
  000a0266: mov r0,r2
  000a0268: blx r5
  000a026a: ldr r1,[0x000a030c]
  000a026c: ldr r2,[r4,#0x4]
  000a026e: add r1,pc
  000a0270: str r0,[r4,#0x38]
  000a0272: mov r0,r2
  000a0274: blx r5
  000a0276: ldr r1,[0x000a0310]
  000a0278: ldr r2,[r4,#0x4]
  000a027a: add r1,pc
  000a027c: str r0,[r4,#0x3c]
  000a027e: mov r0,r2
  000a0280: blx r5
  000a0282: ldr r1,[0x000a0314]
  000a0284: ldr r2,[r4,#0x4]
  000a0286: add r1,pc
  000a0288: str r0,[r4,#0x40]
  000a028a: mov r0,r2
  000a028c: blx r5
  000a028e: ldr r1,[0x000a0318]
  000a0290: ldr r2,[r4,#0x4]
  000a0292: add r1,pc
  000a0294: str r0,[r4,#0x44]
  000a0296: mov r0,r2
  000a0298: blx r5
  000a029a: ldr r1,[0x000a031c]
  000a029c: ldr r2,[r4,#0x4]
  000a029e: add r1,pc
  000a02a0: str r0,[r4,#0x48]
  000a02a2: mov r0,r2
  000a02a4: blx r5
  000a02a6: ldr r1,[0x000a0320]
  000a02a8: ldr r2,[r4,#0x4]
  000a02aa: add r1,pc
  000a02ac: str r0,[r4,#0x4c]
  000a02ae: mov r0,r2
  000a02b0: blx r5
  000a02b2: ldr r1,[0x000a0324]
  000a02b4: ldr r2,[r4,#0x4]
  000a02b6: add r1,pc
  000a02b8: str r0,[r4,#0x50]
  000a02ba: mov r0,r2
  000a02bc: blx r5
  000a02be: ldr r1,[0x000a0328]
  000a02c0: ldr r2,[r4,#0x4]
  000a02c2: add r1,pc
  000a02c4: str r0,[r4,#0x54]
  000a02c6: mov r0,r2
  000a02c8: blx r5
  000a02ca: ldr r1,[r4,#0x4]
  000a02cc: str r0,[r4,#0x58]
  000a02ce: mov r0,r1
  000a02d0: blx 0x00070960
  000a02d4: ldr r0,[r4,#0x48]
  000a02d6: movs r1,#0x0
  000a02d8: pop.w {r4,r5,r7,lr}
  000a02dc: b.w 0x001ab768
```
