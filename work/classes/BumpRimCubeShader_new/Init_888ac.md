# BumpRimCubeShader_new::Init
elf 0x888ac body 476
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader_new::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpRimCubeShader_new::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  iVar1 = ShaderBaseStruct::LoadBindShader((char *)param_1,(char *)(DAT_00098a88 + 0x988be));
  *(int *)(param_1 + 4) = iVar1;
  if (iVar1 == 0) {
    iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00098a90 + 0x988d2));
    *(int *)(param_1 + 4) = iVar1;
  }
  pcVar4 = *(code **)(DAT_00098a98 + 0x988e0);
  uVar2 = (*pcVar4)(iVar1,DAT_00098a9c + 0x988e2);
  iVar1 = DAT_00098aa0 + 0x988ec;
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098aa4 + 0x988f8;
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098aa8 + 0x98904;
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098aac + 0x98910;
  *(undefined4 *)(param_1 + 0x2c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  puVar3 = (undefined4 *)(DAT_00098ab0 + 0x9891c);
  iVar1 = DAT_00098ab4 + 0x98920;
  *(undefined4 *)(param_1 + 0x30) = uVar2;
  pcVar4 = (code *)*puVar3;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ab8 + 0x9892e;
  *(undefined4 *)(param_1 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098abc + 0x9893a;
  *(undefined4 *)(param_1 + 0x38) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ac0 + 0x98946;
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ac4 + 0x98952;
  *(undefined4 *)(param_1 + 0x40) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ac8 + 0x9895e;
  *(undefined4 *)(param_1 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098acc + 0x9896a;
  *(undefined4 *)(param_1 + 0x48) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ad0 + 0x98976;
  *(undefined4 *)(param_1 + 0x4c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ad4 + 0x98982;
  *(undefined4 *)(param_1 + 0x50) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ad8 + 0x9898e;
  *(undefined4 *)(param_1 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098adc + 0x9899a;
  *(undefined4 *)(param_1 + 0x58) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ae0 + 0x989a6;
  *(undefined4 *)(param_1 + 0x5c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ae4 + 0x989b2;
  *(undefined4 *)(param_1 + 0x60) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098ae8 + 0x989be;
  *(undefined4 *)(param_1 + 0x68) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098aec + 0x989ca;
  *(undefined4 *)(param_1 + 100) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098af0 + 0x989d6;
  *(undefined4 *)(param_1 + 0x6c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098af4 + 0x989e2;
  *(undefined4 *)(param_1 + 0x70) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098af8 + 0x989ee;
  *(undefined4 *)(param_1 + 0x74) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098afc + 0x989fa;
  *(undefined4 *)(param_1 + 0x78) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b00 + 0x98a06;
  *(undefined4 *)(param_1 + 0x7c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b04 + 0x98a12;
  *(undefined4 *)(param_1 + 0x80) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b08 + 0x98a20;
  *(undefined4 *)(param_1 + 0x84) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b0c + 0x98a2e;
  *(undefined4 *)(param_1 + 0x88) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b10 + 0x98a3c;
  *(undefined4 *)(param_1 + 0x8c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00098b14 + 0x98a4a;
  *(undefined4 *)(param_1 + 0x90) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  *(undefined4 *)(param_1 + 0x94) = uVar2;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar1 = 0; iVar1 != 2; iVar1 = iVar1 + 1) {
    if (-1 < *(int *)(param_1 + iVar1 * 4 + 0x4c)) {
      glUniform1i(*(int *)(param_1 + iVar1 * 4 + 0x4c),iVar1);
    }
  }
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x54),7);
  return;
}

```

## target disasm
```
  000988ac: push {r4,r5,r6,r7,lr}
  000988ae: add r7,sp,#0xc
  000988b0: push.w r11
  000988b4: ldr r1,[0x00098a88]
  000988b6: mov r4,r0
  000988b8: ldr r2,[0x00098a8c]
  000988ba: add r1,pc
  000988bc: add r2,pc
  000988be: blx 0x00070948
  000988c2: cmp r0,#0x0
  000988c4: str r0,[r4,#0x4]
  000988c6: bne 0x000988d8
  000988c8: ldr r1,[0x00098a90]
  000988ca: mov r0,r4
  000988cc: ldr r2,[0x00098a94]
  000988ce: add r1,pc
  000988d0: add r2,pc
  000988d2: blx 0x00070954
  000988d6: str r0,[r4,#0x4]
  000988d8: ldr r2,[0x00098a98]
  000988da: ldr r1,[0x00098a9c]
  000988dc: add r2,pc
  000988de: add r1,pc
  000988e0: ldr r5,[r2,#0x0]
  000988e2: blx r5
  000988e4: ldr r1,[0x00098aa0]
  000988e6: ldr r2,[r4,#0x4]
  000988e8: add r1,pc
  000988ea: str r0,[r4,#0x20]
  000988ec: mov r0,r2
  000988ee: blx r5
  000988f0: ldr r1,[0x00098aa4]
  000988f2: ldr r2,[r4,#0x4]
  000988f4: add r1,pc
  000988f6: str r0,[r4,#0x24]
  000988f8: mov r0,r2
  000988fa: blx r5
  000988fc: ldr r1,[0x00098aa8]
  000988fe: ldr r2,[r4,#0x4]
  00098900: add r1,pc
  00098902: str r0,[r4,#0x28]
  00098904: mov r0,r2
  00098906: blx r5
  00098908: ldr r1,[0x00098aac]
  0009890a: ldr r2,[r4,#0x4]
  0009890c: add r1,pc
  0009890e: str r0,[r4,#0x2c]
  00098910: mov r0,r2
  00098912: blx r5
  00098914: ldr r3,[0x00098ab0]
  00098916: ldr r1,[0x00098ab4]
  00098918: add r3,pc
  0009891a: ldr r2,[r4,#0x4]
  0009891c: add r1,pc
  0009891e: str r0,[r4,#0x30]
  00098920: ldr r5,[r3,#0x0]
  00098922: mov r0,r2
  00098924: blx r5
  00098926: ldr r1,[0x00098ab8]
  00098928: ldr r2,[r4,#0x4]
  0009892a: add r1,pc
  0009892c: str r0,[r4,#0x34]
  0009892e: mov r0,r2
  00098930: blx r5
  00098932: ldr r1,[0x00098abc]
  00098934: ldr r2,[r4,#0x4]
  00098936: add r1,pc
  00098938: str r0,[r4,#0x38]
  0009893a: mov r0,r2
  0009893c: blx r5
  0009893e: ldr r1,[0x00098ac0]
  00098940: ldr r2,[r4,#0x4]
  00098942: add r1,pc
  00098944: str r0,[r4,#0x3c]
  00098946: mov r0,r2
  00098948: blx r5
  0009894a: ldr r1,[0x00098ac4]
  0009894c: ldr r2,[r4,#0x4]
  0009894e: add r1,pc
  00098950: str r0,[r4,#0x40]
  00098952: mov r0,r2
  00098954: blx r5
  00098956: ldr r1,[0x00098ac8]
  00098958: ldr r2,[r4,#0x4]
  0009895a: add r1,pc
  0009895c: str r0,[r4,#0x44]
  0009895e: mov r0,r2
  00098960: blx r5
  00098962: ldr r1,[0x00098acc]
  00098964: ldr r2,[r4,#0x4]
  00098966: add r1,pc
  00098968: str r0,[r4,#0x48]
  0009896a: mov r0,r2
  0009896c: blx r5
  0009896e: ldr r1,[0x00098ad0]
  00098970: ldr r2,[r4,#0x4]
  00098972: add r1,pc
  00098974: str r0,[r4,#0x4c]
  00098976: mov r0,r2
  00098978: blx r5
  0009897a: ldr r1,[0x00098ad4]
  0009897c: ldr r2,[r4,#0x4]
  0009897e: add r1,pc
  00098980: str r0,[r4,#0x50]
  00098982: mov r0,r2
  00098984: blx r5
  00098986: ldr r1,[0x00098ad8]
  00098988: ldr r2,[r4,#0x4]
  0009898a: add r1,pc
  0009898c: str r0,[r4,#0x54]
  0009898e: mov r0,r2
  00098990: blx r5
  00098992: ldr r1,[0x00098adc]
  00098994: ldr r2,[r4,#0x4]
  00098996: add r1,pc
  00098998: str r0,[r4,#0x58]
  0009899a: mov r0,r2
  0009899c: blx r5
  0009899e: ldr r1,[0x00098ae0]
  000989a0: ldr r2,[r4,#0x4]
  000989a2: add r1,pc
  000989a4: str r0,[r4,#0x5c]
  000989a6: mov r0,r2
  000989a8: blx r5
  000989aa: ldr r1,[0x00098ae4]
  000989ac: ldr r2,[r4,#0x4]
  000989ae: add r1,pc
  000989b0: str r0,[r4,#0x60]
  000989b2: mov r0,r2
  000989b4: blx r5
  000989b6: ldr r1,[0x00098ae8]
  000989b8: ldr r2,[r4,#0x4]
  000989ba: add r1,pc
  000989bc: str r0,[r4,#0x68]
  000989be: mov r0,r2
  000989c0: blx r5
  000989c2: ldr r1,[0x00098aec]
  000989c4: ldr r2,[r4,#0x4]
  000989c6: add r1,pc
  000989c8: str r0,[r4,#0x64]
  000989ca: mov r0,r2
  000989cc: blx r5
  000989ce: ldr r1,[0x00098af0]
  000989d0: ldr r2,[r4,#0x4]
  000989d2: add r1,pc
  000989d4: str r0,[r4,#0x6c]
  000989d6: mov r0,r2
  000989d8: blx r5
  000989da: ldr r1,[0x00098af4]
  000989dc: ldr r2,[r4,#0x4]
  000989de: add r1,pc
  000989e0: str r0,[r4,#0x70]
  000989e2: mov r0,r2
  000989e4: blx r5
  000989e6: ldr r1,[0x00098af8]
  000989e8: ldr r2,[r4,#0x4]
  000989ea: add r1,pc
  000989ec: str r0,[r4,#0x74]
  000989ee: mov r0,r2
  000989f0: blx r5
  000989f2: ldr r1,[0x00098afc]
  000989f4: ldr r2,[r4,#0x4]
  000989f6: add r1,pc
  000989f8: str r0,[r4,#0x78]
  000989fa: mov r0,r2
  000989fc: blx r5
  000989fe: ldr r1,[0x00098b00]
  00098a00: ldr r2,[r4,#0x4]
  00098a02: add r1,pc
  00098a04: str r0,[r4,#0x7c]
  00098a06: mov r0,r2
  00098a08: blx r5
  00098a0a: ldr r1,[0x00098b04]
  00098a0c: ldr r2,[r4,#0x4]
  00098a0e: add r1,pc
  00098a10: str.w r0,[r4,#0x80]
  00098a14: mov r0,r2
  00098a16: blx r5
  00098a18: ldr r1,[0x00098b08]
  00098a1a: ldr r2,[r4,#0x4]
  00098a1c: add r1,pc
  00098a1e: str.w r0,[r4,#0x84]
  00098a22: mov r0,r2
  00098a24: blx r5
  00098a26: ldr r1,[0x00098b0c]
  00098a28: ldr r2,[r4,#0x4]
  00098a2a: add r1,pc
  00098a2c: str.w r0,[r4,#0x88]
  00098a30: mov r0,r2
  00098a32: blx r5
  00098a34: ldr r1,[0x00098b10]
  00098a36: ldr r2,[r4,#0x4]
  00098a38: add r1,pc
  00098a3a: str.w r0,[r4,#0x8c]
  00098a3e: mov r0,r2
  00098a40: blx r5
  00098a42: ldr r1,[0x00098b14]
  00098a44: ldr r2,[r4,#0x4]
  00098a46: add r1,pc
  00098a48: str.w r0,[r4,#0x90]
  00098a4c: mov r0,r2
  00098a4e: blx r5
  00098a50: ldr r1,[r4,#0x4]
  00098a52: str.w r0,[r4,#0x94]
  00098a56: mov r0,r1
  00098a58: blx 0x00070960
  00098a5c: add.w r6,r4,#0x4c
  00098a60: movs r5,#0x0
  00098a62: b 0x00098a74
  00098a64: ldr.w r0,[r6,r5,lsl #0x2]
  00098a68: cmp r0,#0x0
  00098a6a: blt 0x00098a72
  00098a6c: mov r1,r5
  00098a6e: blx 0x0007096c
  00098a72: adds r5,#0x1
  00098a74: cmp r5,#0x2
  00098a76: bne 0x00098a64
  00098a78: ldr r0,[r4,#0x54]
  00098a7a: movs r1,#0x7
  00098a7c: pop.w r11
  00098a80: pop.w {r4,r5,r6,r7,lr}
  00098a84: b.w 0x001ab768
```
