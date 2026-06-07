# Engine::Engine
elf 0x83eec body 470
Sig: undefined __thiscall Engine(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::Engine() */

void __thiscall AbyssEngine::Engine::Engine(Engine *this)

{
  undefined1 auVar1 [16];
  ApplicationManager *this_00;
  int iVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar3 = *(int **)(DAT_00094124 + 0x93f00);
  local_3c = *piVar3;
  String::String((String *)this);
  String::String((String *)(this + 0x14));
  String::String((String *)(this + 0x3c));
  String::String((String *)(this + 0x4c));
  uVar4 = 0;
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar6 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar7 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x340) = 0;
  *(undefined4 *)(this + 0x344) = 0;
  *(undefined4 *)(this + 0x34c) = 0;
  *(undefined4 *)(this + 0x350) = 0;
  *(undefined4 *)(this + 0x354) = 0;
  *(undefined4 *)(this + 0x3cc) = 0;
  *(undefined4 *)(this + 0x3d0) = 0;
  *(undefined4 *)(this + 0x3d4) = 0;
  *(undefined4 *)(this + 0x330) = 0;
  *(undefined4 *)(this + 0x334) = uVar5;
  *(undefined4 *)(this + 0x338) = uVar6;
  *(undefined4 *)(this + 0x33c) = uVar7;
  Array<int>::Array();
  *(undefined4 *)(this + 0x478) = 0;
  *(undefined4 *)(this + 0x47c) = 0;
  *(undefined4 *)(this + 0x400) = 0;
  *(undefined4 *)(this + 0x404) = 0;
  *(undefined4 *)(this + 0x468) = uVar4;
  *(undefined4 *)(this + 0x46c) = uVar5;
  *(undefined4 *)(this + 0x470) = uVar6;
  *(undefined4 *)(this + 0x474) = uVar7;
  *(undefined4 *)(this + 0x3f0) = uVar4;
  *(undefined4 *)(this + 0x3f4) = uVar5;
  *(undefined4 *)(this + 0x3f8) = uVar6;
  *(undefined4 *)(this + 0x3fc) = uVar7;
  Array<AbyssEngine::ShaderBaseStruct*>::Array
            ((Array<AbyssEngine::ShaderBaseStruct*> *)(this + 0x510));
  uVar4 = DAT_00094118;
  *(undefined4 *)(this + 0x380) = 0;
  *(undefined4 *)(this + 0x40c) = 0;
  *(undefined4 *)(this + 0x410) = 0;
  this[0x3c4] = (Engine)0x0;
  *(undefined4 *)(this + 0x3c8) = 0;
  local_48 = CONCAT44(uVar4,0x3f000000);
  local_40 = 0;
  AEMath::Vector::operator=((Vector *)(this + 0x3cc),(Vector *)&local_48);
  *(undefined4 *)(this + 0x358) = 0;
  *(undefined4 *)(this + 0x414) = 0;
  *(undefined4 *)(this + 0x418) = 0;
  this[0x41c] = (Engine)0x0;
  *(undefined4 *)(this + 0x360) = 0;
  *(undefined4 *)(this + 0x4a8) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x3e4) = 0xffffffff;
  for (iVar2 = 0x1f; iVar2 != 0x33; iVar2 = iVar2 + 1) {
    *(undefined4 *)(this + iVar2 * 4) = 0xffffffff;
  }
  uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar6 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x48c) = 0xffffffff;
  *(undefined4 *)(this + 0x490) = 0xffffffff;
  this[0xff] = (Engine)0x0;
  *(undefined4 *)(this + 0x4a4) = 0;
  *(undefined2 *)(this + 0xfd) = 0x100;
  *(undefined4 *)(this + 0x78) = 0xffffffff;
  this[0x74] = (Engine)0x0;
  *(undefined4 *)(this + 0x420) = 0;
  this[0x424] = (Engine)0x0;
  this[0x48] = (Engine)0x0;
  *(undefined4 *)(this + 0x484) = 0;
  *(undefined4 *)(this + 800) = 0;
  *(undefined4 *)(this + 0x324) = 0;
  *(undefined4 *)(this + 0x328) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  this[0x38] = (Engine)0x0;
  *(undefined4 *)(this + 0x4c0) = 0;
  *(undefined4 *)(this + 0x4c4) = 0;
  *(undefined4 *)(this + 0x368) = 0;
  *(undefined4 *)(this + 0x36c) = uVar4;
  *(undefined4 *)(this + 0x370) = uVar5;
  *(undefined4 *)(this + 0x374) = uVar6;
  *(undefined4 *)(this + 0x4b0) = 0;
  *(undefined4 *)(this + 0x4b4) = uVar4;
  *(undefined4 *)(this + 0x4b8) = uVar5;
  *(undefined4 *)(this + 0x4bc) = uVar6;
  uVar7 = DAT_0009411c;
  *(undefined4 *)(this + 0x4e0) = 0;
  *(undefined4 *)(this + 0x4e4) = uVar4;
  *(undefined4 *)(this + 0x4e8) = uVar5;
  *(undefined4 *)(this + 0x4ec) = uVar6;
  *(undefined4 *)(this + 0x4f0) = 0;
  *(undefined4 *)(this + 0x4f4) = 0;
  *(undefined4 *)(this + 0x28) = 0x14;
  this[0x20] = (Engine)0x1;
  *(undefined4 *)(this + 0xcc) = uVar7;
  this_00 = operator_new(0xc0);
  ApplicationManager::ApplicationManager(this_00,this);
  uVar4 = DAT_00094120;
  auVar1._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
  auVar1._0_8_ = 0xbf800000bf800000;
  auVar1 = auVar1 << 0x40 | auVar1;
  *(ApplicationManager **)(this + 0x30) = this_00;
  *(undefined4 *)(this + 1000) = 0;
  *(undefined4 *)(this + 0x3ec) = uVar4;
  *(undefined4 *)(this + 0xd0) = *(undefined4 *)auVar1;
  *(undefined4 *)(this + 0xd4) = *(undefined4 *)(auVar1 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xd8) = *(undefined4 *)(auVar1 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xdc) = *(undefined4 *)(auVar1 + (undefined1  [16])0xc);
  local_48 = 0x3f800000;
  local_40 = 0;
  AEMath::Vector::operator=((Vector *)(this + 0x3f0),(Vector *)&local_48);
  initFileInterface(this);
  iVar2 = *piVar3 - local_3c;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  00093eec: push {r4,r5,r6,r7,lr}
  00093eee: add r7,sp,#0xc
  00093ef0: push {r7,r8,r9,r10,r11}
  00093ef4: vpush {d5,d6,d7,d8,d9}
  00093ef8: mov r4,r0
  00093efa: ldr r0,[0x00094124]
  00093efc: add r0,pc
  00093efe: ldr.w r9,[r0,#0x0]
  00093f02: ldr.w r0,[r9,#0x0]
  00093f06: str r0,[sp,#0x14]
  00093f08: mov r0,r4
  00093f0a: blx 0x0006efbc
  00093f0e: add.w r5,r4,#0x14
  00093f12: mov r0,r5
  00093f14: blx 0x0006efbc
  00093f18: add.w r6,r4,#0x3c
  00093f1c: mov r0,r6
  00093f1e: str r5,[sp,#0x4]
  00093f20: blx 0x0006efbc
  00093f24: add.w r8,r4,#0x4c
  00093f28: mov r0,r8
  00093f2a: str r6,[sp,#0x0]
  00093f2c: blx 0x0006efbc
  00093f30: vmov.i32 q4,#0x0
  00093f34: add.w r0,r4,#0x330
  00093f38: add.w r11,r4,#0x3d8
  00093f3c: movs r5,#0x0
  00093f3e: strd r5,r5,[r4,#0x340]
  00093f42: strd r5,r5,[r4,#0x34c]
  00093f46: str.w r5,[r4,#0x354]
  00093f4a: strd r5,r5,[r4,#0x3cc]
  00093f4e: str.w r5,[r4,#0x3d4]
  00093f52: vst1.32 {d8,d9},[r0]
  00093f56: mov r0,r11
  00093f58: blx 0x000701f8
  00093f5c: add.w r0,r4,#0x468
  00093f60: add.w r6,r4,#0x3f0
  00093f64: add.w r10,r4,#0x510
  00093f68: str.w r5,[r4,#0x478]
  00093f6c: str.w r5,[r4,#0x47c]
  00093f70: str.w r5,[r4,#0x400]
  00093f74: str.w r5,[r4,#0x404]
  00093f78: vst1.32 {d8,d9},[r0]
  00093f7c: vst1.32 {d8,d9},[r6]
  00093f80: mov r0,r10
  00093f82: blx 0x000704bc
  00093f86: movs r5,#0x0
  00093f88: add.w r0,r4,#0x3cc
  00093f8c: ldr r1,[0x00094118]
  00093f8e: str.w r5,[r4,#0x380]
  00093f92: str.w r5,[r4,#0x40c]
  00093f96: str.w r5,[r4,#0x410]
  00093f9a: strb.w r5,[r4,#0x3c4]
  00093f9e: str.w r5,[r4,#0x3c8]
  00093fa2: str r1,[sp,#0xc]
  00093fa4: mov.w r1,#0x3f000000
  00093fa8: str r1,[sp,#0x8]
  00093faa: str r5,[sp,#0x10]
  00093fac: add r1,sp,#0x8
  00093fae: blx 0x0006eb3c
  00093fb2: mov.w r0,#0xffffffff
  00093fb6: movs r1,#0x1f
  00093fb8: str.w r5,[r4,#0x358]
  00093fbc: str.w r5,[r4,#0x414]
  00093fc0: str.w r5,[r4,#0x418]
  00093fc4: strb.w r5,[r4,#0x41c]
  00093fc8: str.w r5,[r4,#0x360]
  00093fcc: str.w r5,[r4,#0x4a8]
  00093fd0: str r5,[r4,#0x70]
  00093fd2: str.w r5,[r4,#0x100]
  00093fd6: str.w r0,[r4,#0x3e4]
  00093fda: b 0x00093fe2
  00093fdc: str.w r0,[r4,r1,lsl #0x2]
  00093fe0: adds r1,#0x1
  00093fe2: cmp r1,#0x33
  00093fe4: bne 0x00093fdc
  00093fe6: mov.w r1,#0xffffffff
  00093fea: vmov.i32 q8,#0x0
  00093fee: movs r2,#0x0
  00093ff0: mov.w r3,#0x100
  00093ff4: str.w r1,[r4,#0x48c]
  00093ff8: mov r5,r8
  00093ffa: str.w r1,[r4,#0x490]
  00093ffe: strb.w r2,[r4,#0xff]
  00094002: str.w r2,[r4,#0x4a4]
  00094006: strh.w r3,[r4,#0xfd]
  0009400a: str r1,[r4,#0x78]
  0009400c: add.w r1,r4,#0x368
  00094010: strb.w r2,[r4,#0x74]
  00094014: str.w r2,[r4,#0x420]
  00094018: strb.w r2,[r4,#0x424]
  0009401c: strb.w r2,[r4,#0x48]
  00094020: str.w r2,[r4,#0x484]
  00094024: strd r2,r2,[r4,#0x320]
  00094028: str.w r2,[r4,#0x328]
  0009402c: str r2,[r4,#0x34]
  0009402e: strb.w r2,[r4,#0x38]
  00094032: str.w r2,[r4,#0x4c0]
  00094036: str.w r2,[r4,#0x4c4]
  0009403a: vst1.64 {d16,d17},[r1]
  0009403e: add.w r1,r4,#0x4b0
  00094042: vst1.64 {d16,d17},[r1]
  00094046: add.w r1,r4,#0x4e0
  0009404a: ldr r0,[0x0009411c]
  0009404c: vst1.64 {d16,d17},[r1]
  00094050: movs r1,#0x14
  00094052: str.w r2,[r4,#0x4f0]
  00094056: str.w r2,[r4,#0x4f4]
  0009405a: str r1,[r4,#0x28]
  0009405c: movs r1,#0x1
  0009405e: strb.w r1,[r4,#0x20]
  00094062: str.w r0,[r4,#0xcc]
  00094066: movs r0,#0xc0
  00094068: blx 0x0006eb24
  0009406c: mov r8,r0
  0009406e: mov r1,r4
  00094070: blx 0x000704c8
  00094074: ldr r0,[0x00094120]
  00094076: vmov.f32 q8,#0xbf800000
  0009407a: movs r1,#0x0
  0009407c: str.w r8,[r4,#0x30]
  00094080: strd r1,r0,[r4,#0x3e8]
  00094084: add.w r0,r4,#0xd0
  00094088: vst1.64 {d16,d17},[r0]
  0009408c: mov.w r0,#0x3f800000
  00094090: str r1,[sp,#0xc]
  00094092: str r0,[sp,#0x8]
  00094094: str r1,[sp,#0x10]
  00094096: add r1,sp,#0x8
  00094098: mov r0,r6
  0009409a: blx 0x0006eb3c
  0009409e: mov r0,r4
  000940a0: blx 0x000704d4
  000940a4: ldr r0,[sp,#0x14]
  000940a6: ldr.w r1,[r9,#0x0]
  000940aa: subs r0,r1,r0
  000940ac: itttt eq
  000940ae: mov.eq r0,r4
  000940b0: vpop.eq {d5,d6,d7,d8,d9}
  000940b4: add.eq sp,#0x4
  000940b6: pop.eq.w {r8,r9,r10,r11}
  000940ba: it eq
  000940bc: pop.eq {r4,r5,r6,r7,pc}
  000940be: blx 0x0006e824
```
