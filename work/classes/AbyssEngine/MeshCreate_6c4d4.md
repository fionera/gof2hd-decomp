# AbyssEngine::MeshCreate
elf 0x6c4d4 body 302
Sig: undefined __stdcall MeshCreate(Engine * param_1, ushort param_2, ushort param_3, signed param_4, Mesh * * param_5)

## decompile
```c

/* AbyssEngine::MeshCreate(AbyssEngine::Engine*, unsigned short, unsigned short, signed char,
   AbyssEngine::Mesh**) */

undefined4
AbyssEngine::MeshCreate
          (undefined4 param_1,uint param_2,int param_3,uint param_4,undefined4 *param_5)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 *puVar4;
  void *pvVar5;
  uint uVar6;
  
  if (((param_2 < 4) || (param_3 == 0)) || ((param_4 & 1) == 0)) {
    uVar3 = 0xfffffffc;
  }
  else {
    puVar4 = operator_new(0x88);
    uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    *(undefined4 *)(puVar4 + 0x82) = 0;
    *(undefined4 *)(puVar4 + 0x7e) = 0;
    *(undefined4 *)(puVar4 + 0x2c) = 0;
    *(undefined4 *)(puVar4 + 0x30) = 0;
    *(undefined4 *)(puVar4 + 0x35) = 0;
    *(undefined4 *)(puVar4 + 0x31) = 0;
    *(undefined4 *)(puVar4 + 0x4c) = 0x3f800000;
    *(undefined4 *)(puVar4 + 0x50) = 0;
    *(undefined4 *)(puVar4 + 0x54) = 0;
    *(undefined4 *)(puVar4 + 0x59) = 0;
    *(undefined4 *)(puVar4 + 0x70) = 0;
    *(undefined4 *)(puVar4 + 0x74) = uVar3;
    *(undefined4 *)(puVar4 + 0x78) = uVar1;
    *(undefined4 *)(puVar4 + 0x7c) = uVar2;
    *(undefined4 *)(puVar4 + 0x60) = 0;
    *(undefined4 *)(puVar4 + 100) = uVar3;
    *(undefined4 *)(puVar4 + 0x68) = uVar1;
    *(undefined4 *)(puVar4 + 0x6c) = uVar2;
    *(undefined4 *)(puVar4 + 0x14) = 0;
    *(undefined4 *)(puVar4 + 0x18) = uVar3;
    *(undefined4 *)(puVar4 + 0x1c) = uVar1;
    *(undefined4 *)(puVar4 + 0x20) = uVar2;
    *(undefined4 *)(puVar4 + 4) = 0;
    *(undefined4 *)(puVar4 + 8) = uVar3;
    *(undefined4 *)(puVar4 + 0xc) = uVar1;
    *(undefined4 *)(puVar4 + 0x10) = uVar2;
    *(undefined4 *)(puVar4 + 0x3c) = 0;
    *(undefined4 *)(puVar4 + 0x40) = uVar3;
    *(undefined4 *)(puVar4 + 0x44) = uVar1;
    *(undefined4 *)(puVar4 + 0x48) = uVar2;
    *(undefined4 *)(puVar4 + 0x55) = 0;
    *param_5 = puVar4;
    *(short *)(puVar4 + 2) = (short)param_2;
    *puVar4 = (char)param_4;
    *(short *)(puVar4 + 0x28) = (short)param_3 + (short)(param_3 << 1);
    *(short *)(puVar4 + 0x2a) = (short)param_3;
    uVar6 = param_2 * 0xc;
    pvVar5 = operator_new__(uVar6);
    *(void **)(puVar4 + 4) = pvVar5;
    __aeabi_memclr4(pvVar5,uVar6);
    if ((int)(param_4 << 0x1b) < 0) {
      pvVar5 = operator_new__(param_3 * 6);
      *(void **)(puVar4 + 0x2c) = pvVar5;
      __aeabi_memclr(pvVar5,param_3 * 6);
    }
    if ((int)(param_4 << 0x1e) < 0) {
      pvVar5 = operator_new__(param_2 << 3);
      *(void **)(puVar4 + 8) = pvVar5;
      __aeabi_memclr4(pvVar5,param_2 << 3);
    }
    if ((int)(param_4 << 0x1d) < 0) {
      pvVar5 = operator_new__(uVar6);
      *(void **)(puVar4 + 0x10) = pvVar5;
      __aeabi_memclr4(pvVar5,uVar6);
      if (**(char **)(DAT_0007c604 + 0x7c5c8) != '\0') {
        pvVar5 = operator_new__(uVar6);
        *(void **)(puVar4 + 0x14) = pvVar5;
        __aeabi_memclr4(pvVar5,uVar6);
        pvVar5 = operator_new__(uVar6);
        *(void **)(puVar4 + 0x18) = pvVar5;
        __aeabi_memclr4(pvVar5,uVar6);
      }
    }
    if ((int)(param_4 << 0x1c) < 0) {
      pvVar5 = operator_new__(param_2 << 4);
      *(void **)(puVar4 + 0xc) = pvVar5;
      __aeabi_memclr4(pvVar5,param_2 << 4);
    }
    uVar3 = 1;
  }
  return uVar3;
}

```

## target disasm
```
  0007c4d4: push {r4,r5,r6,r7,lr}
  0007c4d6: add r7,sp,#0xc
  0007c4d8: push {r8,r9,r11}
  0007c4dc: mov r9,r3
  0007c4de: mov r5,r2
  0007c4e0: mov r4,r1
  0007c4e2: cmp r1,#0x4
  0007c4e4: bcc 0x0007c4ee
  0007c4e6: cbz r5,0x0007c4ee
  0007c4e8: tst r9,#0x1
  0007c4ec: bne 0x0007c4f8
  0007c4ee: mvn r0,#0x3
  0007c4f2: pop.w {r8,r9,r11}
  0007c4f6: pop {r4,r5,r6,r7,pc}
  0007c4f8: movs r0,#0x88
  0007c4fa: ldr.w r8,[r7,#0x8]
  0007c4fe: blx 0x0006eb24
  0007c502: mov r6,r0
  0007c504: movs r0,#0x0
  0007c506: vmov.i32 q8,#0x0
  0007c50a: mov.w r1,#0x3f800000
  0007c50e: str.w r0,[r6,#0x82]
  0007c512: str.w r0,[r6,#0x7e]
  0007c516: strd r0,r0,[r6,#0x2c]
  0007c51a: str.w r0,[r6,#0x35]
  0007c51e: str.w r0,[r6,#0x31]
  0007c522: strd r1,r0,[r6,#0x4c]
  0007c526: add.w r1,r6,#0x70
  0007c52a: str r0,[r6,#0x54]
  0007c52c: str.w r0,[r6,#0x59]
  0007c530: vst1.32 {d16,d17},[r1]
  0007c534: add.w r1,r6,#0x60
  0007c538: vst1.32 {d16,d17},[r1]
  0007c53c: add.w r1,r6,#0x14
  0007c540: vst1.16 {d16,d17},[r1]
  0007c544: adds r1,r6,#0x4
  0007c546: vst1.16 {d16,d17},[r1]
  0007c54a: add.w r1,r6,#0x3c
  0007c54e: vst1.32 {d16,d17},[r1]
  0007c552: str.w r0,[r6,#0x55]
  0007c556: add.w r0,r5,r5, lsl #0x1
  0007c55a: str.w r6,[r8,#0x0]
  0007c55e: strh r4,[r6,#0x2]
  0007c560: strb.w r9,[r6,#0x0]
  0007c564: strh r0,[r6,#0x28]
  0007c566: add.w r0,r4,r4, lsl #0x1
  0007c56a: strh r5,[r6,#0x2a]
  0007c56c: lsl.w r8,r0,#0x2
  0007c570: mov r0,r8
  0007c572: blx 0x0006ec08
  0007c576: mov r1,r8
  0007c578: str r0,[r6,#0x4]
  0007c57a: blx 0x0006ec14
  0007c57e: lsls.w r0,r9,#0x1b
  0007c582: bpl 0x0007c598
  0007c584: add.w r0,r5,r5, lsl #0x1
  0007c588: lsls r5,r0,#0x1
  0007c58a: mov r0,r5
  0007c58c: blx 0x0006ec08
  0007c590: mov r1,r5
  0007c592: str r0,[r6,#0x2c]
  0007c594: blx 0x0006ec20
  0007c598: lsls.w r0,r9,#0x1e
  0007c59c: bpl 0x0007c5ae
  0007c59e: lsls r5,r4,#0x3
  0007c5a0: mov r0,r5
  0007c5a2: blx 0x0006ec08
  0007c5a6: mov r1,r5
  0007c5a8: str r0,[r6,#0x8]
  0007c5aa: blx 0x0006ec14
  0007c5ae: lsls.w r0,r9,#0x1d
  0007c5b2: bpl 0x0007c5e8
  0007c5b4: mov r0,r8
  0007c5b6: blx 0x0006ec08
  0007c5ba: mov r1,r8
  0007c5bc: str r0,[r6,#0x10]
  0007c5be: blx 0x0006ec14
  0007c5c2: ldr r0,[0x0007c604]
  0007c5c4: add r0,pc
  0007c5c6: ldr r0,[r0,#0x0]
  0007c5c8: ldrb r0,[r0,#0x0]
  0007c5ca: cbz r0,0x0007c5e8
  0007c5cc: mov r0,r8
  0007c5ce: blx 0x0006ec08
  0007c5d2: mov r1,r8
  0007c5d4: str r0,[r6,#0x14]
  0007c5d6: blx 0x0006ec14
  0007c5da: mov r0,r8
  0007c5dc: blx 0x0006ec08
  0007c5e0: mov r1,r8
  0007c5e2: str r0,[r6,#0x18]
  0007c5e4: blx 0x0006ec14
  0007c5e8: lsls.w r0,r9,#0x1c
  0007c5ec: bpl 0x0007c5fe
  0007c5ee: lsls r4,r4,#0x4
  0007c5f0: mov r0,r4
  0007c5f2: blx 0x0006ec08
  0007c5f6: mov r1,r4
  0007c5f8: str r0,[r6,#0xc]
  0007c5fa: blx 0x0006ec14
  0007c5fe: movs r0,#0x1
  0007c600: b 0x0007c4f2
```
