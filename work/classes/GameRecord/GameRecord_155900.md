# GameRecord::GameRecord
elf 0x155900 body 188
Sig: undefined __thiscall GameRecord(GameRecord * this)

## decompile
```c

/* GameRecord::GameRecord() */

GameRecord * __thiscall GameRecord::GameRecord(GameRecord *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  void *pvVar4;
  int iVar5;
  
  AbyssEngine::String::String((String *)(this + 0x188));
  AbyssEngine::String::String((String *)(this + 0x194));
  pvVar4 = operator_new__(0x87);
  *(undefined4 *)(this + 0x11c) = 0;
  *(void **)this = pvVar4;
  __aeabi_memclr8(this + 8,0x8c);
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = uVar1;
  *(undefined4 *)(this + 0xc0) = uVar2;
  *(undefined4 *)(this + 0xc4) = uVar3;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = uVar1;
  *(undefined4 *)(this + 0xb0) = uVar2;
  *(undefined4 *)(this + 0xb4) = uVar3;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = uVar1;
  *(undefined4 *)(this + 0xa0) = uVar2;
  *(undefined4 *)(this + 0xa4) = uVar3;
  *(undefined4 *)(this + 0x10b) = 0;
  *(undefined4 *)(this + 0x10f) = uVar1;
  *(undefined4 *)(this + 0x113) = uVar2;
  *(undefined4 *)(this + 0x117) = uVar3;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x104) = uVar1;
  *(undefined4 *)(this + 0x108) = uVar2;
  *(undefined4 *)(this + 0x10c) = uVar3;
  *(undefined4 *)(this + 0xf0) = 0;
  *(undefined4 *)(this + 0xf4) = uVar1;
  *(undefined4 *)(this + 0xf8) = uVar2;
  *(undefined4 *)(this + 0xfc) = uVar3;
  *(undefined4 *)(this + 0xe0) = 0;
  *(undefined4 *)(this + 0xe4) = uVar1;
  *(undefined4 *)(this + 0xe8) = uVar2;
  *(undefined4 *)(this + 0xec) = uVar3;
  *(undefined4 *)(this + 0x1a0) = 0xffffffff;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 0xd0) = 0;
  this[0xd4] = (GameRecord)0x0;
  *(undefined4 *)(this + 0xd8) = 0;
  this[0xdc] = (GameRecord)0x0;
  *(undefined4 *)(this + 0x1b8) = 0;
  this[0x1c0] = (GameRecord)0x0;
  __aeabi_memclr8(this + 0x130,0x58);
  for (iVar5 = 0x69; iVar5 != 0x6d; iVar5 = iVar5 + 1) {
    *(undefined4 *)(this + iVar5 * 4) = 0;
  }
  *(undefined4 *)(this + 0x1b4) = 0;
  return this;
}

```

## target disasm
```
  00165900: push {r4,r5,r6,r7,lr}
  00165902: add r7,sp,#0xc
  00165904: push.w r11
  00165908: add.w r5,r0,#0x188
  0016590c: mov r4,r0
  0016590e: mov r0,r5
  00165910: blx 0x0006efbc
  00165914: add.w r6,r4,#0x194
  00165918: mov r0,r6
  0016591a: blx 0x0006efbc
  0016591e: movs r0,#0x87
  00165920: blx 0x0006ec08
  00165924: movs r5,#0x0
  00165926: movs r1,#0x8c
  00165928: str.w r5,[r4,#0x11c]
  0016592c: str r0,[r4,#0x0]
  0016592e: add.w r0,r4,#0x8
  00165932: blx 0x00071494
  00165936: vmov.i32 q8,#0x0
  0016593a: add.w r0,r4,#0xb8
  0016593e: movs r1,#0x58
  00165940: vst1.64 {d16,d17},[r0]
  00165944: add.w r0,r4,#0xa8
  00165948: vst1.64 {d16,d17},[r0]
  0016594c: add.w r0,r4,#0x98
  00165950: vst1.64 {d16,d17},[r0]
  00165954: addw r0,r4,#0x10b
  00165958: vst1.8 {d16,d17},[r0]
  0016595c: add.w r0,r4,#0x100
  00165960: vst1.64 {d16,d17},[r0]
  00165964: add.w r0,r4,#0xf0
  00165968: vst1.64 {d16,d17},[r0]
  0016596c: add.w r0,r4,#0xe0
  00165970: vst1.64 {d16,d17},[r0]
  00165974: mov.w r0,#0xffffffff
  00165978: str.w r0,[r4,#0x1a0]
  0016597c: add.w r0,r4,#0x130
  00165980: strd r5,r5,[r4,#0xc8]
  00165984: str.w r5,[r4,#0xd0]
  00165988: strb.w r5,[r4,#0xd4]
  0016598c: str.w r5,[r4,#0xd8]
  00165990: strb.w r5,[r4,#0xdc]
  00165994: str.w r5,[r4,#0x1b8]
  00165998: strb.w r5,[r4,#0x1c0]
  0016599c: blx 0x00071494
  001659a0: movs r0,#0x69
  001659a2: b 0x001659aa
  001659a4: str.w r5,[r4,r0,lsl #0x2]
  001659a8: adds r0,#0x1
  001659aa: cmp r0,#0x6d
  001659ac: bne 0x001659a4
  001659ae: movs r0,#0x0
  001659b0: str.w r0,[r4,#0x1b4]
  001659b4: mov r0,r4
  001659b6: pop.w r11
  001659ba: pop {r4,r5,r6,r7,pc}
```
