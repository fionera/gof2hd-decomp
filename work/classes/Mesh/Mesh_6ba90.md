# Mesh::Mesh
elf 0x6ba90 body 250
Sig: undefined __thiscall Mesh(Mesh * this, Mesh * param_1)

## decompile
```c

/* AbyssEngine::Mesh::Mesh(AbyssEngine::Mesh*) */

Mesh * __thiscall AbyssEngine::Mesh::Mesh(Mesh *this,Mesh *param_1)

{
  Mesh MVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined4 *puVar7;
  Transform *this_00;
  Transform *pTVar8;
  char *pcVar9;
  
  uVar6 = DAT_0007bba0;
  uVar5 = DAT_0007bb98;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x44) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x48) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined8 *)(this + 0x4c) = uVar5;
  *(undefined8 *)(this + 0x54) = uVar6;
  if (param_1 != (Mesh *)0x0) {
    if (param_1[0x84] != (Mesh)0x0) {
      MeshConvertToVBO(param_1);
    }
    AEMath::BSphere::operator=((BSphere *)(this + 0x3c),(BSphere *)(param_1 + 0x3c));
    MVar1 = *param_1;
    puVar7 = (undefined4 *)(DAT_0007bba8 + 0x7bad8);
    *(undefined4 *)(this + 0x7c) = 0;
    *this = MVar1;
    *(undefined4 *)(this + 0x1c) = 0;
    *(undefined4 *)(this + 0x20) = 0;
    pcVar9 = (char *)*puVar7;
    *(undefined2 *)(this + 2) = *(undefined2 *)(param_1 + 2);
    uVar2 = *(undefined4 *)(param_1 + 8);
    uVar3 = *(undefined4 *)(param_1 + 0xc);
    uVar4 = *(undefined4 *)(param_1 + 0x10);
    *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 4);
    *(undefined4 *)(this + 8) = uVar2;
    *(undefined4 *)(this + 0xc) = uVar3;
    *(undefined4 *)(this + 0x10) = uVar4;
    if (*pcVar9 != '\0') {
      *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x14);
      *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x18);
    }
    *(undefined2 *)(this + 0x28) = *(undefined2 *)(param_1 + 0x28);
    *(undefined2 *)(this + 0x2a) = *(undefined2 *)(param_1 + 0x2a);
    *(undefined4 *)(this + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
    *(undefined4 *)(this + 0x30) = *(undefined4 *)(param_1 + 0x30);
    pTVar8 = *(Transform **)(param_1 + 0x34);
    if (pTVar8 == (Transform *)0x0) {
      *(undefined4 *)(this + 0x34) = 0;
    }
    else {
      this_00 = operator_new(0x180);
      Transform::Transform(this_00,pTVar8);
      *(Transform **)(this + 0x34) = this_00;
    }
    AEMath::Vector::operator=((Vector *)(this + 0x50),(Vector *)(param_1 + 0x50));
    MVar1 = param_1[0x85];
    this[0x38] = (Mesh)0x1;
    this[0x85] = MVar1;
    this[0x5c] = param_1[0x5c];
    uVar2 = *(undefined4 *)(param_1 + 100);
    uVar3 = *(undefined4 *)(param_1 + 0x68);
    uVar4 = *(undefined4 *)(param_1 + 0x6c);
    *(undefined4 *)(this + 0x60) = *(undefined4 *)(param_1 + 0x60);
    *(undefined4 *)(this + 100) = uVar2;
    *(undefined4 *)(this + 0x68) = uVar3;
    *(undefined4 *)(this + 0x6c) = uVar4;
    *(undefined4 *)(this + 0x78) = *(undefined4 *)(param_1 + 0x78);
    this[0x84] = param_1[0x84];
    if (*pcVar9 != '\0') {
      *(undefined4 *)(this + 0x70) = *(undefined4 *)(param_1 + 0x70);
      *(undefined4 *)(this + 0x74) = *(undefined4 *)(param_1 + 0x74);
    }
    *(undefined4 *)(this + 0x80) = *(undefined4 *)(param_1 + 0x80);
  }
  return this;
}

```

## target disasm
```
  0007ba90: push {r4,r5,r6,r7,lr}
  0007ba92: add r7,sp,#0xc
  0007ba94: push {r8,r9,r11}
  0007ba98: mov r4,r0
  0007ba9a: vmov.i32 q8,#0x0
  0007ba9e: add.w r6,r4,#0x3c
  0007baa2: adr r0,[0x7bb98]
  0007baa4: vld1.64 {d18,d19},[r0]
  0007baa8: mov r5,r1
  0007baaa: mov r0,r6
  0007baac: cmp r1,#0x0
  0007baae: vst1.32 {d16,d17},[r0]!
  0007bab2: vst1.32 {d18,d19},[r0]
  0007bab6: beq 0x0007bb82
  0007bab8: ldrb.w r0,[r5,#0x84]
  0007babc: cbz r0,0x0007bac4
  0007babe: mov r0,r5
  0007bac0: blx 0x0006eb0c
  0007bac4: add.w r1,r5,#0x3c
  0007bac8: mov r0,r6
  0007baca: blx 0x0006eb18
  0007bace: ldr r0,[0x0007bba8]
  0007bad0: movs r2,#0x0
  0007bad2: ldrb r1,[r5,#0x0]
  0007bad4: add r0,pc
  0007bad6: str r2,[r4,#0x7c]
  0007bad8: strb r1,[r4,#0x0]
  0007bada: adds r1,r5,#0x4
  0007badc: strd r2,r2,[r4,#0x1c]
  0007bae0: ldr.w r9,[r0,#0x0]
  0007bae4: ldrh r0,[r5,#0x2]
  0007bae6: strh r0,[r4,#0x2]
  0007bae8: adds r0,r4,#0x4
  0007baea: vld1.32 {d16,d17},[r1]
  0007baee: vst1.32 {d16,d17},[r0]
  0007baf2: ldrb.w r0,[r9,#0x0]
  0007baf6: cbz r0,0x0007bb00
  0007baf8: ldr r0,[r5,#0x14]
  0007bafa: str r0,[r4,#0x14]
  0007bafc: ldr r0,[r5,#0x18]
  0007bafe: str r0,[r4,#0x18]
  0007bb00: ldrh r0,[r5,#0x28]
  0007bb02: strh r0,[r4,#0x28]
  0007bb04: ldrh r0,[r5,#0x2a]
  0007bb06: strh r0,[r4,#0x2a]
  0007bb08: ldr r0,[r5,#0x2c]
  0007bb0a: str r0,[r4,#0x2c]
  0007bb0c: ldr r0,[r5,#0x30]
  0007bb0e: str r0,[r4,#0x30]
  0007bb10: ldr r6,[r5,#0x34]
  0007bb12: cbz r6,0x0007bb2a
  0007bb14: mov.w r0,#0x180
  0007bb18: blx 0x0006eb24
  0007bb1c: mov r8,r0
  0007bb1e: mov r1,r6
  0007bb20: blx 0x0006eb30
  0007bb24: str.w r8,[r4,#0x34]
  0007bb28: b 0x0007bb2e
  0007bb2a: movs r0,#0x0
  0007bb2c: str r0,[r4,#0x34]
  0007bb2e: add.w r0,r4,#0x50
  0007bb32: add.w r1,r5,#0x50
  0007bb36: blx 0x0006eb3c
  0007bb3a: movs r0,#0x1
  0007bb3c: ldrb.w r1,[r5,#0x85]
  0007bb40: strb.w r0,[r4,#0x38]
  0007bb44: strb.w r1,[r4,#0x85]
  0007bb48: ldrb.w r0,[r5,#0x5c]
  0007bb4c: strb.w r0,[r4,#0x5c]
  0007bb50: add.w r0,r5,#0x60
  0007bb54: vld1.32 {d16,d17},[r0]
  0007bb58: add.w r0,r4,#0x60
  0007bb5c: vst1.32 {d16,d17},[r0]
  0007bb60: ldr r0,[r5,#0x78]
  0007bb62: str r0,[r4,#0x78]
  0007bb64: ldrb.w r0,[r5,#0x84]
  0007bb68: strb.w r0,[r4,#0x84]
  0007bb6c: ldrb.w r0,[r9,#0x0]
  0007bb70: cbz r0,0x0007bb7a
  0007bb72: ldr r0,[r5,#0x70]
  0007bb74: str r0,[r4,#0x70]
  0007bb76: ldr r0,[r5,#0x74]
  0007bb78: str r0,[r4,#0x74]
  0007bb7a: ldr.w r0,[r5,#0x80]
  0007bb7e: str.w r0,[r4,#0x80]
  0007bb82: mov r0,r4
  0007bb84: pop.w {r8,r9,r11}
  0007bb88: pop {r4,r5,r6,r7,pc}
```
