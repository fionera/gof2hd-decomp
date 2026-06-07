# ApplicationManager::ApplicationManager
elf 0x8193c body 1318
Sig: undefined __thiscall ApplicationManager(ApplicationManager * this, Engine * param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::ApplicationManager(AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::ApplicationManager::ApplicationManager(ApplicationManager *this,Engine *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  PaintCanvas *this_00;
  AESoundRessource *this_01;
  ConfigReader *this_02;
  CheatHandler *this_03;
  uint uVar5;
  int *piVar6;
  int iVar7;
  undefined4 local_348;
  String aSStack_344 [12];
  undefined4 local_338;
  String aSStack_334 [12];
  undefined4 local_328;
  String aSStack_324 [12];
  undefined4 local_318;
  String aSStack_314 [12];
  undefined4 local_308;
  String aSStack_304 [12];
  undefined4 local_2f8;
  String aSStack_2f4 [12];
  undefined4 local_2e8;
  String aSStack_2e4 [12];
  undefined4 local_2d8;
  String aSStack_2d4 [12];
  undefined4 local_2c8;
  String aSStack_2c4 [12];
  undefined4 local_2b8;
  String aSStack_2b4 [12];
  undefined4 local_2a8;
  String aSStack_2a4 [12];
  undefined4 local_298;
  String aSStack_294 [12];
  undefined4 local_288;
  String aSStack_284 [12];
  undefined4 local_278;
  String aSStack_274 [12];
  undefined4 local_268;
  String aSStack_264 [12];
  undefined4 local_258;
  String aSStack_254 [12];
  undefined4 local_248;
  String aSStack_244 [12];
  undefined4 local_238;
  String aSStack_234 [12];
  undefined4 local_228;
  String aSStack_224 [12];
  undefined4 local_218;
  String aSStack_214 [12];
  undefined4 local_208;
  String aSStack_204 [12];
  undefined4 local_1f8;
  String aSStack_1f4 [12];
  undefined4 local_1e8;
  String aSStack_1e4 [12];
  undefined4 local_1d8;
  String aSStack_1d4 [12];
  undefined4 local_1c8;
  String aSStack_1c4 [12];
  undefined4 local_1b8;
  String aSStack_1b4 [12];
  undefined4 local_1a8;
  String aSStack_1a4 [12];
  undefined4 local_198;
  String aSStack_194 [12];
  undefined4 local_188;
  String aSStack_184 [12];
  undefined4 local_178;
  String aSStack_174 [12];
  undefined4 local_168;
  String aSStack_164 [12];
  undefined4 local_158;
  String aSStack_154 [12];
  undefined4 local_148;
  String aSStack_144 [12];
  undefined4 local_138;
  String aSStack_134 [12];
  undefined4 local_128;
  String aSStack_124 [12];
  undefined4 local_118;
  String aSStack_114 [12];
  undefined4 local_108;
  String aSStack_104 [12];
  undefined4 local_f8;
  String aSStack_f4 [12];
  undefined4 local_e8;
  String aSStack_e4 [12];
  undefined4 local_d8;
  String aSStack_d4 [12];
  undefined4 local_c8;
  String aSStack_c4 [12];
  undefined4 local_b8;
  String aSStack_b4 [12];
  undefined4 local_a8;
  String aSStack_a4 [12];
  undefined4 local_98;
  String aSStack_94 [12];
  undefined4 local_88;
  String aSStack_84 [12];
  undefined4 local_78;
  String aSStack_74 [12];
  undefined4 local_68;
  String aSStack_64 [12];
  undefined4 local_58;
  String aSStack_54 [12];
  undefined4 local_48;
  String aSStack_44 [12];
  undefined4 local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar6 = *(int **)(DAT_0009196c + 0x91956);
  local_28 = *piVar6;
  Array<AbyssEngine::IApplicationModule*>::Array
            ((Array<AbyssEngine::IApplicationModule*> *)(this + 0x44));
  Array<unsigned_int>::Array((Array<unsigned_int> *)(this + 0x50));
  Array<long_long>::Array((Array<long_long> *)(this + 0x88));
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar4 = *(undefined4 **)(DAT_000919bc + 0x91988);
  *puVar4 = 0;
  puVar4[1] = uVar1;
  puVar4[2] = uVar2;
  puVar4[3] = uVar3;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = uVar1;
  *(undefined4 *)(this + 0x78) = uVar2;
  *(undefined4 *)(this + 0x7c) = uVar3;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = uVar1;
  *(undefined4 *)(this + 0x68) = uVar2;
  *(undefined4 *)(this + 0x6c) = uVar3;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x3c) = 5;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(Engine **)(this + 0xa8) = param_1;
  this_00 = operator_new(0x20c);
  PaintCanvas::PaintCanvas(this_00,param_1);
  *(PaintCanvas **)this = this_00;
  this_01 = operator_new(0x14);
  AESoundRessource::AESoundRessource(this_01);
  this[0x34] = (ApplicationManager)0x0;
  *(AESoundRessource **)(this + 0xac) = this_01;
  this_02 = operator_new(0x14);
  ConfigReader::ConfigReader(this_02,param_1);
  iVar7 = DAT_00091a24;
  *(undefined4 *)(this + 0x20) = 0;
  *(ConfigReader **)(this + 0x38) = this_02;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined2 *)(this + 0xb0) = 0x101;
  this[0xb2] = (ApplicationManager)0x1;
  this[0x14] = (ApplicationManager)0x0;
  local_348 = 0;
  String::String(aSStack_344,(char *)(iVar7 + 0x91a04),false);
  local_338 = 1;
  String::String(aSStack_334,(char *)(DAT_00091e14 + 0x91a30),false);
  local_328 = 2;
  String::String(aSStack_324,(char *)(DAT_00091e18 + 0x91a42),false);
  local_318 = 3;
  String::String(aSStack_314,(char *)(DAT_00091e1c + 0x91a54),false);
  local_308 = 4;
  String::String(aSStack_304,(char *)(DAT_00091e20 + 0x91a66),false);
  local_2f8 = 5;
  String::String(aSStack_2f4,(char *)(DAT_00091e24 + 0x91a78),false);
  local_2e8 = 6;
  String::String(aSStack_2e4,(char *)(DAT_00091e28 + 0x91a8a),false);
  local_2d8 = 7;
  String::String(aSStack_2d4,(char *)(DAT_00091e2c + 0x91a9c),false);
  local_2c8 = 8;
  String::String(aSStack_2c4,(char *)(DAT_00091e30 + 0x91aae),false);
  local_2b8 = 9;
  String::String(aSStack_2b4,(char *)(DAT_00091e34 + 0x91ac0),false);
  local_2a8 = 10;
  String::String(aSStack_2a4,(char *)(DAT_00091e38 + 0x91ad2),false);
  local_298 = 0xb;
  String::String(aSStack_294,(char *)(DAT_00091e3c + 0x91ae4),false);
  local_288 = 0xc;
  String::String(aSStack_284,(char *)(DAT_00091e40 + 0x91af6),false);
  local_278 = 0xd;
  String::String(aSStack_274,(char *)(DAT_00091e44 + 0x91b08),false);
  local_268 = 0xe;
  String::String(aSStack_264,(char *)(DAT_00091e48 + 0x91b1a),false);
  local_258 = 0xf;
  String::String(aSStack_254,(char *)(DAT_00091e4c + 0x91b2c),false);
  local_248 = 0;
  String::String(aSStack_244,(char *)(DAT_00091e50 + 0x91b40),false);
  local_238 = 0;
  String::String(aSStack_234,(char *)(DAT_00091e54 + 0x91b54),false);
  local_228 = 0;
  String::String(aSStack_224,(char *)(DAT_00091e58 + 0x91b66),false);
  local_218 = 0;
  String::String(aSStack_214,(char *)(DAT_00091e5c + 0x91b7a),false);
  local_208 = 0x61;
  String::String(aSStack_204,(char *)(DAT_00091f48 + 0x91b8e),false);
  local_1f8 = 0x62;
  String::String(aSStack_1f4,(char *)(DAT_00091f4c + 0x91ba2),false);
  local_1e8 = 99;
  String::String(aSStack_1e4,(char *)(DAT_00091f50 + 0x91bb6),false);
  local_1d8 = 100;
  String::String(aSStack_1d4,(char *)(DAT_00091f54 + 0x91bca),false);
  local_1c8 = 0x65;
  String::String(aSStack_1c4,(char *)(DAT_00091f58 + 0x91bde),false);
  local_1b8 = 0x66;
  String::String(aSStack_1b4,(char *)(DAT_00091f5c + 0x91bf2),false);
  local_1a8 = 0x67;
  String::String(aSStack_1a4,(char *)(DAT_00091f60 + 0x91c06),false);
  local_198 = 0x68;
  String::String(aSStack_194,(char *)(DAT_00091f64 + 0x91c1a),false);
  local_188 = 0x69;
  String::String(aSStack_184,(char *)(DAT_00091f68 + 0x91c2e),false);
  local_178 = 0x6a;
  String::String(aSStack_174,(char *)(DAT_00091f6c + 0x91c42),false);
  local_168 = 0x6b;
  String::String(aSStack_164,(char *)(DAT_00091f70 + 0x91c56),false);
  local_158 = 0x6c;
  String::String(aSStack_154,(char *)(DAT_00091f74 + 0x91c6a),false);
  local_148 = 0x6d;
  String::String(aSStack_144,(char *)(DAT_00091f78 + 0x91c7e),false);
  local_138 = 0x6e;
  String::String(aSStack_134,(char *)(DAT_00091f7c + 0x91c92),false);
  local_128 = 0x6f;
  String::String(aSStack_124,(char *)(DAT_00091f80 + 0x91ca6),false);
  local_118 = 0x70;
  String::String(aSStack_114,(char *)(DAT_00091f84 + 0x91cba),false);
  local_108 = 0x71;
  String::String(aSStack_104,(char *)(DAT_00091f88 + 0x91cce),false);
  local_f8 = 0x72;
  String::String(aSStack_f4,(char *)(DAT_00091f8c + 0x91ce2),false);
  local_e8 = 0x73;
  String::String(aSStack_e4,(char *)(DAT_00091f90 + 0x91cf6),false);
  local_d8 = 0x74;
  String::String(aSStack_d4,(char *)(DAT_00091f94 + 0x91d0a),false);
  local_c8 = 0x75;
  String::String(aSStack_c4,(char *)(DAT_00091f98 + 0x91d1e),false);
  local_b8 = 0x76;
  String::String(aSStack_b4,(char *)(DAT_00091f9c + 0x91d32),false);
  local_a8 = 0x77;
  String::String(aSStack_a4,(char *)(DAT_00091fa0 + 0x91d46),false);
  local_98 = 0x78;
  String::String(aSStack_94,(char *)(DAT_00091fa4 + 0x91d5a),false);
  local_88 = 0x79;
  String::String(aSStack_84,(char *)(DAT_00091fa8 + 0x91d6e),false);
  local_78 = 0x7a;
  String::String(aSStack_74,(char *)(DAT_00091fac + 0x91d82),false);
  local_68 = 0x20;
  String::String(aSStack_64,(char *)(DAT_00091fb0 + 0x91d96),false);
  local_58 = 10;
  String::String(aSStack_54,(char *)(DAT_00091fb4 + 0x91daa),false);
  local_48 = 0x10;
  String::String(aSStack_44,(char *)(DAT_00091fb8 + 0x91dbe),false);
  local_38 = 0x11;
  String::String(aSStack_34,(char *)(DAT_00091fbc + 0x91dd2),false);
  puVar4 = operator_new__(0x408);
  iVar7 = 0;
  *puVar4 = 0x10;
  puVar4[1] = 0x40;
  do {
    String::String((String *)((int)puVar4 + iVar7 + 0xc));
    iVar7 = iVar7 + 0x10;
  } while (iVar7 != 0x400);
  *(undefined4 **)(this + 0x10) = puVar4 + 2;
  iVar7 = 0;
  for (uVar5 = 0; uVar5 < 0x32; uVar5 = uVar5 + 1) {
    KeyCode::operator=((KeyCode *)(*(int *)(this + 0x10) + iVar7),
                       (KeyCode *)(aSStack_344 + iVar7 + -4));
    iVar7 = iVar7 + 0x10;
  }
  this_03 = operator_new(0x10);
  CheatHandler::CheatHandler(this_03,*(KeyCode **)(this + 0x10));
  iVar7 = 0x314;
  *(CheatHandler **)(this + 0x30) = this_03;
  *(undefined4 *)(this + 0xb4) = 0xffffffff;
  *(undefined4 *)(this + 0xb8) = 0xffffffff;
  do {
    String::~String(aSStack_344 + iVar7 + -4);
    iVar7 = iVar7 + -0x10;
  } while (iVar7 != -0xc);
  iVar7 = *piVar6 - local_28;
  if (iVar7 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar7);
  }
  return;
}

```

## target disasm
```
  0009193c: push {r4,r5,r6,r7,lr}
  0009193e: add r7,sp,#0xc
  00091940: push {r8,r9,r10,r11}
  00091944: sub.w sp,sp,#0x32c
  00091948: mov r8,r0
  0009194a: ldr r0,[0x0009196c]
  0009194c: add.w r4,r8,#0x44
  00091950: mov r5,r1
  00091952: add r0,pc
  00091954: ldr r6,[r0,#0x0]
  00091956: ldr r0,[r6,#0x0]
  00091958: str r0,[sp,#0x328]
  0009195a: mov r0,r4
  0009195c: blx 0x0007024c
  00091960: add.w r9,r8,#0x50
  00091964: mov r0,r9
  00091966: blx 0x0006f8ec
  0009196a: b 0x00091970
  00091970: add.w r10,r8,#0x88
  00091974: str r6,[sp,#0x0]
  00091976: mov r0,r10
  00091978: blx 0x00070258
  0009197c: ldr r0,[0x000919bc]
  0009197e: vmov.i32 q8,#0x0
  00091982: movs r1,#0x5
  00091984: add r0,pc
  00091986: ldr r0,[r0,#0x0]
  00091988: vst1.32 {d16,d17},[r0]
  0009198c: add.w r0,r8,#0x70
  00091990: vst1.64 {d16,d17},[r0]
  00091994: add.w r0,r8,#0x60
  00091998: vst1.64 {d16,d17},[r0]
  0009199c: movs r0,#0x0
  0009199e: strd r0,r0,[r8,#0x8]
  000919a2: str.w r1,[r8,#0x3c]
  000919a6: strd r0,r0,[r8,#0x18]
  000919aa: strd r0,r0,[r8,#0x80]
  000919ae: str.w r5,[r8,#0xa8]
  000919b2: mov.w r0,#0x20c
  000919b6: blx 0x0006eb24
  000919ba: b 0x000919c0
  000919c0: mov r6,r0
  000919c2: mov r1,r5
  000919c4: blx 0x00070264
  000919c8: str.w r6,[r8,#0x0]
  000919cc: movs r0,#0x14
  000919ce: blx 0x0006eb24
  000919d2: mov r6,r0
  000919d4: blx 0x00070270
  000919d8: movs r0,#0x0
  000919da: strb.w r0,[r8,#0x34]
  000919de: str.w r6,[r8,#0xac]
  000919e2: movs r0,#0x14
  000919e4: blx 0x0006eb24
  000919e8: mov r6,r0
  000919ea: mov r1,r5
  000919ec: blx 0x0007027c
  000919f0: str r4,[sp,#0x4]
  000919f2: movs r0,#0x0
  000919f4: ldr r1,[0x00091a24]
  000919f6: movw r2,#0x101
  000919fa: movs r5,#0x1
  000919fc: str.w r0,[r8,#0x20]
  00091a00: add r1,pc
  00091a02: str.w r6,[r8,#0x38]
  00091a06: str.w r0,[r8,#0x28]
  00091a0a: strh.w r2,[r8,#0xb0]
  00091a0e: strb.w r5,[r8,#0xb2]
  00091a12: strb.w r0,[r8,#0x14]
  00091a16: str r0,[sp,#0x8]
  00091a18: add r6,sp,#0x8
  00091a1a: movs r2,#0x0
  00091a1c: adds r0,r6,#0x4
  00091a1e: blx 0x0006ee18
  00091a22: b 0x00091a28
  00091a28: ldr r1,[0x00091e14]
  00091a2a: str r5,[sp,#0x18]
  00091a2c: add r1,pc
  00091a2e: add.w r0,r6,#0x14
  00091a32: movs r2,#0x0
  00091a34: blx 0x0006ee18
  00091a38: ldr r1,[0x00091e18]
  00091a3a: movs r5,#0x2
  00091a3c: str r5,[sp,#0x28]
  00091a3e: add r1,pc
  00091a40: add.w r0,r6,#0x24
  00091a44: movs r2,#0x0
  00091a46: blx 0x0006ee18
  00091a4a: ldr r1,[0x00091e1c]
  00091a4c: movs r5,#0x3
  00091a4e: str r5,[sp,#0x38]
  00091a50: add r1,pc
  00091a52: add.w r0,r6,#0x34
  00091a56: movs r2,#0x0
  00091a58: blx 0x0006ee18
  00091a5c: ldr r1,[0x00091e20]
  00091a5e: movs r5,#0x4
  00091a60: str r5,[sp,#0x48]
  00091a62: add r1,pc
  00091a64: add.w r0,r6,#0x44
  00091a68: movs r2,#0x0
  00091a6a: blx 0x0006ee18
  00091a6e: ldr r1,[0x00091e24]
  00091a70: movs r5,#0x5
  00091a72: str r5,[sp,#0x58]
  00091a74: add r1,pc
  00091a76: add.w r0,r6,#0x54
  00091a7a: movs r2,#0x0
  00091a7c: blx 0x0006ee18
  00091a80: ldr r1,[0x00091e28]
  00091a82: movs r5,#0x6
  00091a84: str r5,[sp,#0x68]
  00091a86: add r1,pc
  00091a88: add.w r0,r6,#0x64
  00091a8c: movs r2,#0x0
  00091a8e: blx 0x0006ee18
  00091a92: ldr r1,[0x00091e2c]
  00091a94: movs r5,#0x7
  00091a96: str r5,[sp,#0x78]
  00091a98: add r1,pc
  00091a9a: add.w r0,r6,#0x74
  00091a9e: movs r2,#0x0
  00091aa0: blx 0x0006ee18
  00091aa4: ldr r1,[0x00091e30]
  00091aa6: movs r5,#0x8
  00091aa8: str r5,[sp,#0x88]
  00091aaa: add r1,pc
  00091aac: add.w r0,r6,#0x84
  00091ab0: movs r2,#0x0
  00091ab2: blx 0x0006ee18
  00091ab6: ldr r1,[0x00091e34]
  00091ab8: movs r5,#0x9
  00091aba: str r5,[sp,#0x98]
  00091abc: add r1,pc
  00091abe: add.w r0,r6,#0x94
  00091ac2: movs r2,#0x0
  00091ac4: blx 0x0006ee18
  00091ac8: ldr r1,[0x00091e38]
  00091aca: movs r5,#0xa
  00091acc: str r5,[sp,#0xa8]
  00091ace: add r1,pc
  00091ad0: add.w r0,r6,#0xa4
  00091ad4: movs r2,#0x0
  00091ad6: blx 0x0006ee18
  00091ada: ldr r1,[0x00091e3c]
  00091adc: movs r5,#0xb
  00091ade: str r5,[sp,#0xb8]
  00091ae0: add r1,pc
  00091ae2: add.w r0,r6,#0xb4
  00091ae6: movs r2,#0x0
  00091ae8: blx 0x0006ee18
  00091aec: ldr r1,[0x00091e40]
  00091aee: movs r5,#0xc
  00091af0: str r5,[sp,#0xc8]
  00091af2: add r1,pc
  00091af4: add.w r0,r6,#0xc4
  00091af8: movs r2,#0x0
  00091afa: blx 0x0006ee18
  00091afe: ldr r1,[0x00091e44]
  00091b00: movs r5,#0xd
  00091b02: str r5,[sp,#0xd8]
  00091b04: add r1,pc
  00091b06: add.w r0,r6,#0xd4
  00091b0a: movs r2,#0x0
  00091b0c: blx 0x0006ee18
  00091b10: ldr r1,[0x00091e48]
  00091b12: movs r5,#0xe
  00091b14: str r5,[sp,#0xe8]
  00091b16: add r1,pc
  00091b18: add.w r0,r6,#0xe4
  00091b1c: movs r2,#0x0
  00091b1e: blx 0x0006ee18
  00091b22: ldr r1,[0x00091e4c]
  00091b24: movs r5,#0xf
  00091b26: str r5,[sp,#0xf8]
  00091b28: add r1,pc
  00091b2a: add.w r0,r6,#0xf4
  00091b2e: movs r2,#0x0
  00091b30: movs r4,#0x0
  00091b32: blx 0x0006ee18
  00091b36: ldr r1,[0x00091e50]
  00091b38: movs r5,#0x10
  00091b3a: str r4,[sp,#0x108]
  00091b3c: add r1,pc
  00091b3e: add.w r0,r6,#0x104
  00091b42: movs r2,#0x0
  00091b44: blx 0x0006ee18
  00091b48: ldr r1,[0x00091e54]
  00091b4a: movs r4,#0x0
  00091b4c: movs r5,#0x11
  00091b4e: str r4,[sp,#0x118]
  00091b50: add r1,pc
  00091b52: add.w r0,r6,#0x114
  00091b56: movs r2,#0x0
  00091b58: blx 0x0006ee18
  00091b5c: ldr r1,[0x00091e58]
  00091b5e: movs r5,#0x12
  00091b60: str r4,[sp,#0x128]
  00091b62: add r1,pc
  00091b64: add.w r0,r6,#0x124
  00091b68: movs r2,#0x0
  00091b6a: blx 0x0006ee18
  00091b6e: ldr r1,[0x00091e5c]
  00091b70: movs r0,#0x0
  00091b72: movs r5,#0x13
  00091b74: str r0,[sp,#0x138]
  00091b76: add r1,pc
  00091b78: add.w r0,r6,#0x134
  00091b7c: movs r2,#0x0
  00091b7e: blx 0x0006ee18
  00091b82: ldr r1,[0x00091f48]
  00091b84: movs r0,#0x61
  00091b86: movs r5,#0x14
  00091b88: str r0,[sp,#0x148]
  00091b8a: add r1,pc
  00091b8c: add.w r0,r6,#0x144
  00091b90: movs r2,#0x0
  00091b92: blx 0x0006ee18
  00091b96: ldr r1,[0x00091f4c]
  00091b98: movs r0,#0x62
  00091b9a: movs r5,#0x15
  00091b9c: str r0,[sp,#0x158]
  00091b9e: add r1,pc
  00091ba0: add.w r0,r6,#0x154
  00091ba4: movs r2,#0x0
  00091ba6: blx 0x0006ee18
  00091baa: ldr r1,[0x00091f50]
  00091bac: movs r0,#0x63
  00091bae: movs r5,#0x16
  00091bb0: str r0,[sp,#0x168]
  00091bb2: add r1,pc
  00091bb4: add.w r0,r6,#0x164
  00091bb8: movs r2,#0x0
  00091bba: blx 0x0006ee18
  00091bbe: ldr r1,[0x00091f54]
  00091bc0: movs r0,#0x64
  00091bc2: movs r5,#0x17
  00091bc4: str r0,[sp,#0x178]
  00091bc6: add r1,pc
  00091bc8: add.w r0,r6,#0x174
  00091bcc: movs r2,#0x0
  00091bce: blx 0x0006ee18
  00091bd2: ldr r1,[0x00091f58]
  00091bd4: movs r0,#0x65
  00091bd6: movs r5,#0x18
  00091bd8: str r0,[sp,#0x188]
  00091bda: add r1,pc
  00091bdc: add.w r0,r6,#0x184
  00091be0: movs r2,#0x0
  00091be2: blx 0x0006ee18
  00091be6: ldr r1,[0x00091f5c]
  00091be8: movs r0,#0x66
  00091bea: movs r5,#0x19
  00091bec: str r0,[sp,#0x198]
  00091bee: add r1,pc
  00091bf0: add.w r0,r6,#0x194
  00091bf4: movs r2,#0x0
  00091bf6: blx 0x0006ee18
  00091bfa: ldr r1,[0x00091f60]
  00091bfc: movs r0,#0x67
  00091bfe: movs r5,#0x1a
  00091c00: str r0,[sp,#0x1a8]
  00091c02: add r1,pc
  00091c04: add.w r0,r6,#0x1a4
  00091c08: movs r2,#0x0
  00091c0a: blx 0x0006ee18
  00091c0e: ldr r1,[0x00091f64]
  00091c10: movs r0,#0x68
  00091c12: movs r5,#0x1b
  00091c14: str r0,[sp,#0x1b8]
  00091c16: add r1,pc
  00091c18: add.w r0,r6,#0x1b4
  00091c1c: movs r2,#0x0
  00091c1e: blx 0x0006ee18
  00091c22: ldr r1,[0x00091f68]
  00091c24: movs r0,#0x69
  00091c26: movs r5,#0x1c
  00091c28: str r0,[sp,#0x1c8]
  00091c2a: add r1,pc
  00091c2c: add.w r0,r6,#0x1c4
  00091c30: movs r2,#0x0
  00091c32: blx 0x0006ee18
  00091c36: ldr r1,[0x00091f6c]
  00091c38: movs r0,#0x6a
  00091c3a: movs r5,#0x1d
  00091c3c: str r0,[sp,#0x1d8]
  00091c3e: add r1,pc
  00091c40: add.w r0,r6,#0x1d4
  00091c44: movs r2,#0x0
  00091c46: blx 0x0006ee18
  00091c4a: ldr r1,[0x00091f70]
  00091c4c: movs r0,#0x6b
  00091c4e: movs r5,#0x1e
  00091c50: str r0,[sp,#0x1e8]
  00091c52: add r1,pc
  00091c54: add.w r0,r6,#0x1e4
  00091c58: movs r2,#0x0
  00091c5a: blx 0x0006ee18
  00091c5e: ldr r1,[0x00091f74]
  00091c60: movs r0,#0x6c
  00091c62: movs r5,#0x1f
  00091c64: str r0,[sp,#0x1f8]
  00091c66: add r1,pc
  00091c68: add.w r0,r6,#0x1f4
  00091c6c: movs r2,#0x0
  00091c6e: blx 0x0006ee18
  00091c72: ldr r1,[0x00091f78]
  00091c74: movs r0,#0x6d
  00091c76: movs r5,#0x20
  00091c78: str r0,[sp,#0x208]
  00091c7a: add r1,pc
  00091c7c: add.w r0,r6,#0x204
  00091c80: movs r2,#0x0
  00091c82: blx 0x0006ee18
  00091c86: ldr r1,[0x00091f7c]
  00091c88: movs r0,#0x6e
  00091c8a: movs r5,#0x21
  00091c8c: str r0,[sp,#0x218]
  00091c8e: add r1,pc
  00091c90: add.w r0,r6,#0x214
  00091c94: movs r2,#0x0
  00091c96: blx 0x0006ee18
  00091c9a: ldr r1,[0x00091f80]
  00091c9c: movs r0,#0x6f
  00091c9e: movs r5,#0x22
  00091ca0: str r0,[sp,#0x228]
  00091ca2: add r1,pc
  00091ca4: add.w r0,r6,#0x224
  00091ca8: movs r2,#0x0
  00091caa: blx 0x0006ee18
  00091cae: ldr r1,[0x00091f84]
  00091cb0: movs r0,#0x70
  00091cb2: movs r5,#0x23
  00091cb4: str r0,[sp,#0x238]
  00091cb6: add r1,pc
  00091cb8: add.w r0,r6,#0x234
  00091cbc: movs r2,#0x0
  00091cbe: blx 0x0006ee18
  00091cc2: ldr r1,[0x00091f88]
  00091cc4: movs r0,#0x71
  00091cc6: movs r5,#0x24
  00091cc8: str r0,[sp,#0x248]
  00091cca: add r1,pc
  00091ccc: add.w r0,r6,#0x244
  00091cd0: movs r2,#0x0
  00091cd2: blx 0x0006ee18
  00091cd6: ldr r1,[0x00091f8c]
  00091cd8: movs r0,#0x72
  00091cda: movs r5,#0x25
  00091cdc: str r0,[sp,#0x258]
  00091cde: add r1,pc
  00091ce0: add.w r0,r6,#0x254
  00091ce4: movs r2,#0x0
  00091ce6: blx 0x0006ee18
  00091cea: ldr r1,[0x00091f90]
  00091cec: movs r0,#0x73
  00091cee: movs r5,#0x26
  00091cf0: str r0,[sp,#0x268]
  00091cf2: add r1,pc
  00091cf4: add.w r0,r6,#0x264
  00091cf8: movs r2,#0x0
  00091cfa: blx 0x0006ee18
  00091cfe: ldr r1,[0x00091f94]
  00091d00: movs r0,#0x74
  00091d02: movs r5,#0x27
  00091d04: str r0,[sp,#0x278]
  00091d06: add r1,pc
  00091d08: add.w r0,r6,#0x274
  00091d0c: movs r2,#0x0
  00091d0e: blx 0x0006ee18
  00091d12: ldr r1,[0x00091f98]
  00091d14: movs r0,#0x75
  00091d16: movs r5,#0x28
  00091d18: str r0,[sp,#0x288]
  00091d1a: add r1,pc
  00091d1c: add.w r0,r6,#0x284
  00091d20: movs r2,#0x0
  00091d22: blx 0x0006ee18
  00091d26: ldr r1,[0x00091f9c]
  00091d28: movs r0,#0x76
  00091d2a: movs r5,#0x29
  00091d2c: str r0,[sp,#0x298]
  00091d2e: add r1,pc
  00091d30: add.w r0,r6,#0x294
  00091d34: movs r2,#0x0
  00091d36: blx 0x0006ee18
  00091d3a: ldr r1,[0x00091fa0]
  00091d3c: movs r0,#0x77
  00091d3e: movs r5,#0x2a
  00091d40: str r0,[sp,#0x2a8]
  00091d42: add r1,pc
  00091d44: add.w r0,r6,#0x2a4
  00091d48: movs r2,#0x0
  00091d4a: blx 0x0006ee18
  00091d4e: ldr r1,[0x00091fa4]
  00091d50: movs r0,#0x78
  00091d52: movs r5,#0x2b
  00091d54: str r0,[sp,#0x2b8]
  00091d56: add r1,pc
  00091d58: add.w r0,r6,#0x2b4
  00091d5c: movs r2,#0x0
  00091d5e: blx 0x0006ee18
  00091d62: ldr r1,[0x00091fa8]
  00091d64: movs r0,#0x79
  00091d66: movs r5,#0x2c
  00091d68: str r0,[sp,#0x2c8]
  00091d6a: add r1,pc
  00091d6c: add.w r0,r6,#0x2c4
  00091d70: movs r2,#0x0
  00091d72: blx 0x0006ee18
  00091d76: ldr r1,[0x00091fac]
  00091d78: movs r0,#0x7a
  00091d7a: movs r5,#0x2d
  00091d7c: str r0,[sp,#0x2d8]
  00091d7e: add r1,pc
  00091d80: add.w r0,r6,#0x2d4
  00091d84: movs r2,#0x0
  00091d86: blx 0x0006ee18
  00091d8a: ldr r1,[0x00091fb0]
  00091d8c: movs r0,#0x20
  00091d8e: movs r5,#0x2e
  00091d90: str r0,[sp,#0x2e8]
  00091d92: add r1,pc
  00091d94: add.w r0,r6,#0x2e4
  00091d98: movs r2,#0x0
  00091d9a: blx 0x0006ee18
  00091d9e: ldr r1,[0x00091fb4]
  00091da0: movs r0,#0xa
  00091da2: movs r5,#0x2f
  00091da4: str r0,[sp,#0x2f8]
  00091da6: add r1,pc
  00091da8: add.w r0,r6,#0x2f4
  00091dac: movs r2,#0x0
  00091dae: blx 0x0006ee18
  00091db2: ldr r1,[0x00091fb8]
  00091db4: movs r0,#0x10
  00091db6: movs r5,#0x30
  00091db8: str r0,[sp,#0x308]
  00091dba: add r1,pc
  00091dbc: add.w r0,r6,#0x304
  00091dc0: movs r2,#0x0
  00091dc2: blx 0x0006ee18
  00091dc6: ldr r1,[0x00091fbc]
  00091dc8: movs r0,#0x11
  00091dca: movs r5,#0x31
  00091dcc: str r0,[sp,#0x318]
  00091dce: add r1,pc
  00091dd0: add.w r0,r6,#0x314
  00091dd4: movs r2,#0x0
  00091dd6: blx 0x0006ee18
  00091dda: mov.w r0,#0x408
  00091dde: blx 0x0006ec08
  00091de2: mov r5,r0
  00091de4: add.w r4,r5,#0x8
  00091de8: mov.w r11,#0x0
  00091dec: movs r0,#0x40
  00091dee: movs r1,#0x10
  00091df0: strd r1,r0,[r5,#0x0]
  00091df4: add.w r0,r5,r11
  00091df8: adds r0,#0xc
  00091dfa: blx 0x0006efbc
  00091dfe: add.w r11,r11,#0x10
  00091e02: cmp.w r11,#0x400
  00091e06: bne 0x00091df4
  00091e08: str.w r4,[r8,#0x10]
  00091e0c: movs r5,#0x0
  00091e0e: movs r4,#0x0
  00091e10: b 0x00091e70
  00091e60: ldr.w r0,[r8,#0x10]
  00091e64: adds r1,r6,r5
  00091e66: add r0,r5
  00091e68: blx 0x00070288
  00091e6c: adds r5,#0x10
  00091e6e: adds r4,#0x1
  00091e70: cmp r4,#0x32
  00091e72: bcc 0x00091e60
  00091e74: movs r0,#0x10
  00091e76: blx 0x0006eb24
  00091e7a: ldr.w r1,[r8,#0x10]
  00091e7e: mov r5,r0
  00091e80: blx 0x00070294
  00091e84: mov.w r4,#0x314
  00091e88: mov.w r0,#0xffffffff
  00091e8c: str.w r5,[r8,#0x30]
  00091e90: strd r0,r0,[r8,#0xb4]
  00091e94: adds r0,r6,r4
  00091e96: blx 0x0006ee30
  00091e9a: subs r4,#0x10
  00091e9c: adds.w r0,r4,#0xc
  00091ea0: bne 0x00091e94
  00091ea2: ldr r0,[sp,#0x328]
  00091ea4: ldr r1,[sp,#0x0]
  00091ea6: ldr r1,[r1,#0x0]
  00091ea8: subs r0,r1,r0
  00091eaa: itttt eq
  00091eac: mov.eq r0,r8
  00091eae: add.eq.w sp,sp,#0x32c
  00091eb2: pop.eq.w {r8,r9,r10,r11}
  00091eb6: pop.eq {r4,r5,r6,r7,pc}
  00091eb8: blx 0x0006e824
```
