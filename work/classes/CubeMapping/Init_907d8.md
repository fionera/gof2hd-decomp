# CubeMapping::Init
elf 0x907d8 body 236
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::CubeMapping::Init(AbyssEngine::Engine*) */

void AbyssEngine::CubeMapping::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a08c4 + 0xa07e6));
  iVar2 = DAT_000a08d0;
  puVar3 = (undefined4 *)(DAT_000a08cc + 0xa07f2);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa07f6);
  iVar2 = DAT_000a08d4 + 0xa0800;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08d8 + 0xa080c;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a08dc + 0xa0818);
  iVar2 = DAT_000a08e0 + 0xa081c;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08e4 + 0xa082a;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08e8 + 0xa0836;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08ec + 0xa0842;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08f0 + 0xa084e;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08f4 + 0xa085a;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08f8 + 0xa0866;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a08fc + 0xa0872;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0900 + 0xa087e;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0904 + 0xa088a;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0908 + 0xa0896;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a090c + 0xa08a2;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  glUniform1i(*(undefined4 *)(param_1 + 0x40),0);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x3c),1);
  return;
}

```

## target disasm
```
  000a07d8: push {r4,r5,r7,lr}
  000a07da: add r7,sp,#0x8
  000a07dc: ldr r1,[0x000a08c4]
  000a07de: mov r4,r0
  000a07e0: ldr r2,[0x000a08c8]
  000a07e2: add r1,pc
  000a07e4: add r2,pc
  000a07e6: blx 0x00070954
  000a07ea: ldr r2,[0x000a08cc]
  000a07ec: ldr r1,[0x000a08d0]
  000a07ee: add r2,pc
  000a07f0: str r0,[r4,#0x4]
  000a07f2: add r1,pc
  000a07f4: ldr r5,[r2,#0x0]
  000a07f6: blx r5
  000a07f8: ldr r1,[0x000a08d4]
  000a07fa: ldr r2,[r4,#0x4]
  000a07fc: add r1,pc
  000a07fe: str r0,[r4,#0x20]
  000a0800: mov r0,r2
  000a0802: blx r5
  000a0804: ldr r1,[0x000a08d8]
  000a0806: ldr r2,[r4,#0x4]
  000a0808: add r1,pc
  000a080a: str r0,[r4,#0x24]
  000a080c: mov r0,r2
  000a080e: blx r5
  000a0810: ldr r3,[0x000a08dc]
  000a0812: ldr r1,[0x000a08e0]
  000a0814: add r3,pc
  000a0816: ldr r2,[r4,#0x4]
  000a0818: add r1,pc
  000a081a: str r0,[r4,#0x28]
  000a081c: ldr r5,[r3,#0x0]
  000a081e: mov r0,r2
  000a0820: blx r5
  000a0822: ldr r1,[0x000a08e4]
  000a0824: ldr r2,[r4,#0x4]
  000a0826: add r1,pc
  000a0828: str r0,[r4,#0x2c]
  000a082a: mov r0,r2
  000a082c: blx r5
  000a082e: ldr r1,[0x000a08e8]
  000a0830: ldr r2,[r4,#0x4]
  000a0832: add r1,pc
  000a0834: str r0,[r4,#0x30]
  000a0836: mov r0,r2
  000a0838: blx r5
  000a083a: ldr r1,[0x000a08ec]
  000a083c: ldr r2,[r4,#0x4]
  000a083e: add r1,pc
  000a0840: str r0,[r4,#0x34]
  000a0842: mov r0,r2
  000a0844: blx r5
  000a0846: ldr r1,[0x000a08f0]
  000a0848: ldr r2,[r4,#0x4]
  000a084a: add r1,pc
  000a084c: str r0,[r4,#0x38]
  000a084e: mov r0,r2
  000a0850: blx r5
  000a0852: ldr r1,[0x000a08f4]
  000a0854: ldr r2,[r4,#0x4]
  000a0856: add r1,pc
  000a0858: str r0,[r4,#0x40]
  000a085a: mov r0,r2
  000a085c: blx r5
  000a085e: ldr r1,[0x000a08f8]
  000a0860: ldr r2,[r4,#0x4]
  000a0862: add r1,pc
  000a0864: str r0,[r4,#0x3c]
  000a0866: mov r0,r2
  000a0868: blx r5
  000a086a: ldr r1,[0x000a08fc]
  000a086c: ldr r2,[r4,#0x4]
  000a086e: add r1,pc
  000a0870: str r0,[r4,#0x44]
  000a0872: mov r0,r2
  000a0874: blx r5
  000a0876: ldr r1,[0x000a0900]
  000a0878: ldr r2,[r4,#0x4]
  000a087a: add r1,pc
  000a087c: str r0,[r4,#0x48]
  000a087e: mov r0,r2
  000a0880: blx r5
  000a0882: ldr r1,[0x000a0904]
  000a0884: ldr r2,[r4,#0x4]
  000a0886: add r1,pc
  000a0888: str r0,[r4,#0x4c]
  000a088a: mov r0,r2
  000a088c: blx r5
  000a088e: ldr r1,[0x000a0908]
  000a0890: ldr r2,[r4,#0x4]
  000a0892: add r1,pc
  000a0894: str r0,[r4,#0x50]
  000a0896: mov r0,r2
  000a0898: blx r5
  000a089a: ldr r1,[0x000a090c]
  000a089c: ldr r2,[r4,#0x4]
  000a089e: add r1,pc
  000a08a0: str r0,[r4,#0x54]
  000a08a2: mov r0,r2
  000a08a4: blx r5
  000a08a6: ldr r1,[r4,#0x4]
  000a08a8: str r0,[r4,#0x58]
  000a08aa: mov r0,r1
  000a08ac: blx 0x00070960
  000a08b0: ldr r0,[r4,#0x40]
  000a08b2: movs r1,#0x0
  000a08b4: blx 0x0007096c
  000a08b8: ldr r0,[r4,#0x3c]
  000a08ba: movs r1,#0x1
  000a08bc: pop.w {r4,r5,r7,lr}
  000a08c0: b.w 0x001ab768
```
