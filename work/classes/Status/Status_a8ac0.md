# Status::Status
elf 0xa8ac0 body 442
Sig: undefined __thiscall Status(Status * this)

## decompile
```c

/* Status::Status() */

Status * __thiscall Status::Status(Status *this)

{
  Array<Mission*> *this_00;
  Array<Station*> *this_01;
  Array<bool> *pAVar1;
  void *pvVar2;
  Station *this_02;
  int iVar3;
  
  AbyssEngine::String::String((String *)(this + 0x168));
  this_00 = operator_new(0xc);
  Array<Mission*>::Array(this_00);
  *(Array<Mission*> **)(this + 0x198) = this_00;
  this_01 = operator_new(0xc);
  Array<Station*>::Array(this_01);
  *(Array<Station*> **)(this + 0x1a0) = this_01;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x38) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x94) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x98) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0xac) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0xb4) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x4c) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x50) = pAVar1;
  pvVar2 = operator_new(0xc);
  Array<int>::Array();
  *(void **)(this + 0x90) = pvVar2;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x54) = pAVar1;
  pAVar1 = operator_new(0xc);
  Array<bool>::Array(pAVar1);
  *(Array<bool> **)(this + 0x58) = pAVar1;
  ArraySetLength<Mission*>(2,*(Array **)(this + 0x198));
  ArraySetLength<Station*>(3,*(Array **)(this + 0x1a0));
  ArraySetLength<bool>(0x22,*(Array **)(this + 0x38));
  ArraySetLength<bool>(0xb,*(Array **)(this + 0x94));
  ArraySetLength<bool>(0xb,*(Array **)(this + 0x98));
  ArraySetLength<bool>(0x16,*(Array **)(this + 0xac));
  ArraySetLength<bool>(0x22,*(Array **)(this + 0xb4));
  ArraySetLength<bool>(4,*(Array **)(this + 0x4c));
  ArraySetLength<bool>(0xf,*(Array **)(this + 0x50));
  ArraySetLength<bool>(0xe9,*(Array **)(this + 0x54));
  ArraySetLength<bool>(5,*(Array **)(this + 0x58));
  *(undefined4 *)(this + 0x1d0) = 0;
  *(undefined4 *)(this + 0x1e8) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x10c) = 0;
  *(undefined2 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x1c0) = 0;
  *(undefined4 *)(this + 0x1c4) = 0;
  *(undefined4 *)(this + 0x1c8) = 1;
  *(undefined4 *)(this + 0x1b0) = 0;
  *(undefined4 *)(this + 0x1b4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x1b8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x1bc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this_02 = operator_new(0x34);
  Station::Station(this_02);
  *(Station **)(this + 0x78) = this_02;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x194) = 0;
  *(undefined4 *)(this + 0x19c) = 0;
  *(undefined4 *)(this + 0x1a4) = 0;
  *(undefined4 *)(this + 0x1a8) = 0;
  *(undefined4 *)(this + 0x1ac) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x44) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x48) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  for (iVar3 = 1; iVar3 != 5; iVar3 = iVar3 + 1) {
    *(undefined4 *)(this + iVar3 * 4) = 0;
  }
  this[0x17c] = (Status)0x0;
  *(undefined4 *)(this + 0x178) = 0;
  return this;
}

```
## target disasm
```
  000b8ac0: push {r4,r5,r6,r7,lr}
  000b8ac2: add r7,sp,#0xc
  000b8ac4: push.w r11
  000b8ac8: add.w r4,r0,#0x168
  000b8acc: mov r5,r0
  000b8ace: mov r0,r4
  000b8ad0: blx 0x0006efbc
  000b8ad4: movs r0,#0xc
  000b8ad6: blx 0x0006eb24
  000b8ada: mov r6,r0
  000b8adc: blx 0x00073588
  000b8ae0: str.w r6,[r5,#0x198]
  000b8ae4: movs r0,#0xc
  000b8ae6: blx 0x0006eb24
  000b8aea: mov r6,r0
  000b8aec: blx 0x00073594
  000b8af0: str.w r6,[r5,#0x1a0]
  000b8af4: movs r0,#0xc
  000b8af6: blx 0x0006eb24
  000b8afa: mov r6,r0
  000b8afc: blx 0x000735a0
  000b8b00: str r6,[r5,#0x38]
  000b8b02: movs r0,#0xc
  000b8b04: blx 0x0006eb24
  000b8b08: mov r6,r0
  000b8b0a: blx 0x000735a0
  000b8b0e: str.w r6,[r5,#0x94]
  000b8b12: movs r0,#0xc
  000b8b14: blx 0x0006eb24
  000b8b18: mov r6,r0
  000b8b1a: blx 0x000735a0
  000b8b1e: str.w r6,[r5,#0x98]
  000b8b22: movs r0,#0xc
  000b8b24: blx 0x0006eb24
  000b8b28: mov r6,r0
  000b8b2a: blx 0x000735a0
  000b8b2e: str.w r6,[r5,#0xac]
  000b8b32: movs r0,#0xc
  000b8b34: blx 0x0006eb24
  000b8b38: mov r6,r0
  000b8b3a: blx 0x000735a0
  000b8b3e: str.w r6,[r5,#0xb4]
  000b8b42: movs r0,#0xc
  000b8b44: blx 0x0006eb24
  000b8b48: mov r6,r0
  000b8b4a: blx 0x000735a0
  000b8b4e: str r6,[r5,#0x4c]
  000b8b50: movs r0,#0xc
  000b8b52: blx 0x0006eb24
  000b8b56: mov r6,r0
  000b8b58: blx 0x000735a0
  000b8b5c: str r6,[r5,#0x50]
  000b8b5e: movs r0,#0xc
  000b8b60: blx 0x0006eb24
  000b8b64: mov r6,r0
  000b8b66: blx 0x000701f8
  000b8b6a: str.w r6,[r5,#0x90]
  000b8b6e: movs r0,#0xc
  000b8b70: blx 0x0006eb24
  000b8b74: mov r6,r0
  000b8b76: blx 0x000735a0
  000b8b7a: str r6,[r5,#0x54]
  000b8b7c: movs r0,#0xc
  000b8b7e: blx 0x0006eb24
  000b8b82: mov r6,r0
  000b8b84: blx 0x000735a0
  000b8b88: ldr.w r1,[r5,#0x198]
  000b8b8c: str r6,[r5,#0x58]
  000b8b8e: movs r0,#0x2
  000b8b90: blx 0x000735ac
  000b8b94: ldr.w r1,[r5,#0x1a0]
  000b8b98: movs r0,#0x3
  000b8b9a: blx 0x000735b8
  000b8b9e: ldr r1,[r5,#0x38]
  000b8ba0: movs r0,#0x22
  000b8ba2: blx 0x000735c4
  000b8ba6: ldr.w r1,[r5,#0x94]
  000b8baa: movs r0,#0xb
  000b8bac: blx 0x000735c4
  000b8bb0: ldr.w r1,[r5,#0x98]
  000b8bb4: movs r0,#0xb
  000b8bb6: blx 0x000735c4
  000b8bba: ldr.w r1,[r5,#0xac]
  000b8bbe: movs r0,#0x16
  000b8bc0: blx 0x000735c4
  000b8bc4: ldr.w r1,[r5,#0xb4]
  000b8bc8: movs r0,#0x22
  000b8bca: blx 0x000735c4
  000b8bce: ldr r1,[r5,#0x4c]
  000b8bd0: movs r0,#0x4
  000b8bd2: blx 0x000735c4
  000b8bd6: ldr r1,[r5,#0x50]
  000b8bd8: movs r0,#0xf
  000b8bda: blx 0x000735c4
  000b8bde: ldr r1,[r5,#0x54]
  000b8be0: movs r0,#0xe9
  000b8be2: blx 0x000735c4
  000b8be6: ldr r1,[r5,#0x58]
  000b8be8: movs r0,#0x5
  000b8bea: blx 0x000735c4
  000b8bee: movs r0,#0x0
  000b8bf0: vmov.i32 q8,#0x0
  000b8bf4: str.w r0,[r5,#0x1d0]
  000b8bf8: movs r1,#0x1
  000b8bfa: str.w r0,[r5,#0x1e8]
  000b8bfe: str r0,[r5,#0x34]
  000b8c00: str.w r0,[r5,#0x10c]
  000b8c04: strh.w r0,[r5,#0x110]
  000b8c08: strd r0,r0,[r5,#0x1c0]
  000b8c0c: add.w r0,r5,#0x1b0
  000b8c10: str.w r1,[r5,#0x1c8]
  000b8c14: vst1.64 {d16,d17},[r0]
  000b8c18: movs r0,#0x34
  000b8c1a: blx 0x0006eb24
  000b8c1e: mov r6,r0
  000b8c20: blx 0x000735d0
  000b8c24: vmov.i32 q8,#0x0
  000b8c28: add.w r1,r5,#0x14
  000b8c2c: movs r0,#0x0
  000b8c2e: str r6,[r5,#0x78]
  000b8c30: str.w r0,[r5,#0x8c]
  000b8c34: strd r0,r0,[r5,#0x190]
  000b8c38: str.w r0,[r5,#0x19c]
  000b8c3c: strd r0,r0,[r5,#0x1a4]
  000b8c40: str.w r0,[r5,#0x1ac]
  000b8c44: str.w r0,[r5,#0x14c]
  000b8c48: str r0,[r5,#0x0]
  000b8c4a: strd r0,r0,[r5,#0x24]
  000b8c4e: vst1.32 {d16,d17},[r1]
  000b8c52: add.w r1,r5,#0x3c
  000b8c56: vst1.32 {d16,d17},[r1]
  000b8c5a: movs r1,#0x1
  000b8c5c: b 0x000b8c64
  000b8c5e: str.w r0,[r5,r1,lsl #0x2]
  000b8c62: adds r1,#0x1
  000b8c64: cmp r1,#0x5
  000b8c66: bne 0x000b8c5e
  000b8c68: movs r0,#0x0
  000b8c6a: strb.w r0,[r5,#0x17c]
  000b8c6e: str.w r0,[r5,#0x178]
  000b8c72: mov r0,r5
  000b8c74: pop.w r11
  000b8c78: pop {r4,r5,r6,r7,pc}
```
