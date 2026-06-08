# String::Set
elf 0x72a0c body 204
Sig: undefined __stdcall Set(longlong param_1)

## decompile
```c

/* AbyssEngine::String::Set(long long) */

void AbyssEngine::String::Set(longlong param_1)

{
  byte bVar1;
  int iVar2;
  ushort *puVar3;
  ushort uVar4;
  int in_r2;
  uint uVar5;
  byte bVar6;
  int in_r3;
  uint uVar7;
  ushort *puVar8;
  int iVar9;
  ushort *puVar10;
  uint uVar11;
  bool bVar12;
  undefined8 uVar13;
  undefined1 *puVar14;
  
  iVar2 = (int)param_1;
  puVar14 = &stack0xfffffff8;
  if (*(void **)(iVar2 + 4) != (void *)0x0) {
    operator_delete__(*(void **)(iVar2 + 4));
  }
  *(undefined4 *)(iVar2 + 4) = 0;
  if (in_r2 == 0 && in_r3 == 0) {
    (*(code *)(DAT_001ab104 + 0x1ab108))(iVar2,DAT_00082ad8 + 0x82a4e);
    return;
  }
  puVar3 = operator_new__(0x28);
  *(ushort **)(iVar2 + 4) = puVar3;
  *(undefined4 *)(iVar2 + 8) = 0;
  if (in_r3 < 0) {
    puVar10 = puVar3 + 1;
    *puVar3 = 0x2d;
    bVar12 = in_r2 != 0;
    in_r2 = -in_r2;
    *(undefined4 *)(iVar2 + 8) = 1;
    iVar9 = -(uint)bVar12 - in_r3;
  }
  else {
    iVar9 = 0;
    puVar10 = puVar3;
  }
  uVar13 = CONCAT44(iVar9,in_r2);
  uVar7 = (uint)(in_r3 < 0);
  puVar3 = puVar10 + -2;
  do {
    puVar8 = puVar3;
    iVar9 = (int)((ulonglong)uVar13 >> 0x20);
    uVar11 = (uint)uVar13;
    uVar13 = __aeabi_uldivmod(uVar11,iVar9,10,0,puVar14);
    uVar7 = uVar7 + 1;
    uVar5 = uVar11 + (int)uVar13 * -10;
    bVar1 = (byte)uVar5;
    bVar6 = bVar1 | 0x30;
    if (9 < uVar5) {
      bVar6 = bVar1 + 0x57;
    }
    uVar4 = (ushort)bVar6;
    puVar8[2] = (ushort)bVar6;
    puVar3 = puVar8 + 1;
  } while ((uint)(uVar11 < 10) <= (uint)-iVar9);
  *(uint *)(iVar2 + 8) = uVar7;
  puVar8[3] = 0;
  while( true ) {
    puVar3[1] = *puVar10;
    *puVar10 = uVar4;
    if (puVar3 <= puVar10 + 1) break;
    uVar4 = *puVar3;
    puVar3 = puVar3 + -1;
    puVar10 = puVar10 + 1;
  }
  return;
}

```

## target disasm
```
  00082a0c: push {r4,r5,r6,r7,lr}
  00082a0e: add r7,sp,#0xc
  00082a10: push {r7,r8,r9,r10,r11}
  00082a14: mov r9,r0
  00082a16: ldr r0,[r0,#0x4]
  00082a18: mov r6,r3
  00082a1a: mov r10,r2
  00082a1c: cbz r0,0x00082a22
  00082a1e: blx 0x0006ebfc
  00082a22: movs r4,#0x0
  00082a24: orrs.w r0,r10,r6
  00082a28: str.w r4,[r9,#0x4]
  00082a2c: beq 0x00082a46
  00082a2e: movs r0,#0x28
  00082a30: blx 0x0006ec08
  00082a34: mov r8,r0
  00082a36: cmp.w r6,#0xffffffff
  00082a3a: strd r0,r4,[r9,#0x4]
  00082a3e: ble 0x00082a58
  00082a40: movs r4,#0x0
  00082a42: movs r6,#0x0
  00082a44: b 0x00082a6e
  00082a46: ldr r1,[0x00082ad8]
  00082a48: mov r0,r9
  00082a4a: add r1,pc
  00082a4c: pop.w {r3,r8,r9,r10,r11}
  00082a50: pop.w {r4,r5,r6,r7,lr}
  00082a54: b.w 0x001ab0f8
  00082a58: movs r0,#0x2d
  00082a5a: movs r4,#0x1
  00082a5c: strh r0,[r8],#0x2
  00082a60: movs r0,#0x0
  00082a62: rsbs.w r10,r10,#0x0
  00082a66: str.w r4,[r9,#0x8]
  00082a6a: sbc.w r6,r0,r6
  00082a6e: sub.w r5,r8,#0x4
  00082a72: mov.w r11,#0x0
  00082a76: mov r0,r10
  00082a78: mov r1,r6
  00082a7a: movs r2,#0xa
  00082a7c: movs r3,#0x0
  00082a7e: blx 0x0006f5ec
  00082a82: add.w r2,r0,r0, lsl #0x2
  00082a86: adds r4,#0x1
  00082a88: sub.w r2,r10,r2, lsl #0x1
  00082a8c: orr r3,r2,#0x30
  00082a90: cmp r2,#0x9
  00082a92: it hi
  00082a94: add.hi.w r3,r2,#0x57
  00082a98: uxtb r2,r3
  00082a9a: rsbs.w r3,r10,#0x9
  00082a9e: strh r2,[r5,#0x4]
  00082aa0: add.w r5,r5,#0x2
  00082aa4: sbcs.w r3,r11,r6
  00082aa8: mov r10,r0
  00082aaa: mov r6,r1
  00082aac: bcc 0x00082a76
  00082aae: movs r0,#0x0
  00082ab0: str.w r4,[r9,#0x8]
  00082ab4: strh r0,[r5,#0x4]
  00082ab6: add.w r0,r8,#0x2
  00082aba: b 0x00082ac2
  00082abc: ldrh.w r2,[r5],#-0x2
  00082ac0: adds r0,#0x2
  00082ac2: ldrh.w r1,[r0,#-0x2]
  00082ac6: cmp r0,r5
  00082ac8: strh r1,[r5,#0x2]
  00082aca: strh r2,[r0,#-0x2]
  00082ace: bcc 0x00082abc
  00082ad0: pop.w {r3,r8,r9,r10,r11}
  00082ad4: pop {r4,r5,r6,r7,pc}
  001ab0f8: bx pc
```
